// Copyright (c) 2021 NVIDIA Corporation.  All rights reserved.
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <iomanip>
#include "dcdread.h"
#include <assert.h>

#include <algorithm>
#include <vector>
#include <atomic>
#include <execution>
#include <ranges>      // for std::views::iota_view
#include <nvtx3/nvToolsExt.h>

void pair_gpu(double *d_x, double *d_y, double *d_z,
			  std::atomic<int> *d_g2, int numatm, int nconf,
			  const double xbox, const double ybox, const double zbox, int d_bin);

int main(int argc, char *argv[])
{
	double xbox, ybox, zbox;

	int nbin;
	int numatm, nconf, inconf;
	std::string file;

	///////////////////////////////////////////////////////////////

	inconf = 10;
	nbin = 2000;
	file = "../../_common/input/alk.traj.dcd";
	///////////////////////////////////////
	std::ifstream infile;
	infile.open(file.c_str());
	if (!infile)
	{
		std::cout << "file " << file.c_str() << " not found\n";
		return 1;
	}
	assert(infile);

	std::ofstream pairfile, stwo;
	pairfile.open("RDF.dat");
	stwo.open("Pair_entropy.dat");

	/////////////////////////////////////////////////////////

	dcdreadhead(&numatm, &nconf, infile);
	std::cout << "Dcd file has " << numatm << " atoms and " << nconf << " frames" << std::endl;
	if (inconf > nconf)
		std::cout << "nconf is reset to " << nconf << std::endl;
	else
	{
		nconf = inconf;
	}
	std::cout << "Calculating RDF for " << nconf << " frames" << std::endl;
	////////////////////////////////////////////////////////

	std::vector<double> h_x(nconf * numatm);
    std::vector<double> h_y(nconf * numatm); 
    std::vector<double> h_z(nconf * numatm); 
	
	double *x = &h_x[0];
	double *y = &h_y[0];
	double *z = &h_z[0];
  

	//Note
	std::atomic<int> *h_g2 = new std::atomic<int>[nbin];
	std::fill(std::execution::par, h_g2, h_g2 + nbin, 0);

	/////////reading cordinates//////////////////////////////////////////////
	nvtxRangePush("Read_File");
	double ax[numatm], ay[numatm], az[numatm];
	for (int i = 0; i < nconf; i++)
	{
		dcdreadframe(ax, ay, az, infile, numatm, xbox, ybox, zbox);
		for (int j = 0; j < numatm; j++)
		{
			x[i * numatm + j] = ax[j];
			y[i * numatm + j] = ay[j];
			z[i * numatm + j] = az[j];
		}
	}
	nvtxRangePop(); //pop for Reading file
	std::cout << "Reading of input file is completed" << std::endl;
	//////////////////////////////////////////////////////////////////////////
	nvtxRangePush("Pair_Calculation");
	pair_gpu(x, y, z, h_g2, numatm, nconf, xbox, ybox, zbox, nbin);
	nvtxRangePop(); //Pop for Pair Calculation

	double g2[nbin];
	double  s2 = 0.0l, s2bond = 0.0l;
	
	nvtxRangePush("Entropy_Calculation");
	for (int i = 0; i < nbin; i++)
	{

		double pi = acos(-1.0l);
		double rho = (numatm) / (xbox * ybox * zbox);
		double norm = (4.0l * pi * rho) / 3.0l;
		double rl, ru, nideal;
		double r, gr, lngr, lngrbond;
		double box = std::min(xbox, ybox);
		box = std::min(box, zbox);
		double del = box / (2.0l * nbin);

		rl = (i)*del;
		ru = rl + del;
		nideal = norm * (ru * ru * ru - rl * rl * rl);
		g2[i] = (double)h_g2[i] / ((double)nconf * (double)numatm * nideal);
		r = (i)*del;
		pairfile << (i + 0.5l) * del << " " << g2[i] << std::endl;
		if (r < 2.0l)
		{
			gr = 0.0l;
		}
		else
		{
			gr = g2[i];
		}
		if (gr < 1e-5)
		{
			lngr = 0.0l;
		}
		else
		{
			lngr = log(gr);
		}

		if (g2[i] < 1e-6)
		{
			lngrbond = 0.0l;
		}
		else
		{
			lngrbond = log(g2[i]);
		}
		s2 = s2 - 2.0l * pi * rho * ((gr * lngr) - gr + 1.0l) * del * r * r;
		s2bond = s2bond - 2.0l * pi * rho * ((g2[i] * lngrbond) - g2[i] + 1.0l) * del * r * r;
	}
	nvtxRangePop(); //Pop for Entropy Calculation
	stwo << "s2 value is " << s2 << std::endl;
	stwo << "s2bond value is " << s2bond << std::endl;

	std::cout << "\n#Freeing Host memory" << std::endl;

	delete[] h_g2;

	std::cout << "#Number of atoms processed: " << numatm << std::endl
		 << std::endl;
	std::cout << "#Number of confs processed: " << nconf << std::endl
		 << std::endl;
	return 0;
}

void pair_gpu(double *d_x, double *d_y, double *d_z,
			  std::atomic<int> *d_g2, int numatm, int nconf,
			  const double xbox, const double ybox, const double zbox, int d_bin)
{
	double cut;
	double box;
	box = std::min(xbox, ybox);
	box = std::min(box, zbox);

	double del = box / (2.0 * d_bin);
	cut = box * 0.5;

    auto res = std::ranges::views::iota(0, numatm*numatm);

	std::cout << "\n" << nconf << " "<< numatm; 
	for (int frame = 0; frame < nconf; frame++)
	{
		std::cout << "\n" << frame;

		std::for_each(std::execution::par, res.begin(), res.end(),
		
					  [d_x, d_y, d_z, d_g2, numatm, frame, xbox, ybox, zbox, cut, del](unsigned int index) {
						  int id1 = index / numatm;
						  int id2 = index % numatm;

						  double dx = d_x[frame * numatm + id1] - d_x[frame * numatm + id2];
						  double dy = d_y[frame * numatm + id1] - d_y[frame * numatm + id2];
						  double dz = d_z[frame * numatm + id1] - d_z[frame * numatm + id2];

						  dx = dx - xbox * (std::round(dx / xbox));
						  dy = dy - ybox * (std::round(dy / ybox));
						  dz = dz - zbox * (std::round(dz / zbox));

						  double r = sqrtf(dx * dx + dy * dy + dz * dz);
						  if (r < cut)
						  {
							  int ig2 = (int)(r / del);
							  ++d_g2[ig2];
						  }
					  });
	}
}
