# Copyright (c) 2020 NVIDIA Corporation.  All rights reserved. 

FC := nvfortran
CC := nvc++

FLAGS := -O3 -w 
ACCFLAGS := -stdpar=multicore -Minfo

NVTXLIB_F := -lnvhpcwrapnvtx

VER=$(shell nvc -dumpversion)
NVARCH=$(shell uname -s)_$(shell uname -m)
NVTXLIB_C := -I/opt/nvidia/hpc_sdk/$(NVARCH)/$(VER)/cuda/include

rdf_f: rdf.f90
	${FC} ${FLAGS} ${ACCFLAGS} -o rdf_f rdf.f90 ${NVTXLIB_F} 

rdf_c: rdf.cpp
	${CC} ${FLAGS} -std=c++17 ${ACCFLAGS} -o rdf_c rdf.cpp -fopenmp ${NVTXLIB_C} 

clean:
	rm -f *.o rdf_f rdf_c
