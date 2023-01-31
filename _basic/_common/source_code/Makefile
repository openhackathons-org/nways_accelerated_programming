# Copyright (c) 2020 NVIDIA Corporation.  All rights reserved. 

CC := nvc++
CFLAGS := -O3 -w -ldl
ACCFLAGS := -Minfo=accel
NVTXLIB_c := -I/opt/nvidia/hpc_sdk/Linux_x86_64/22.7/cuda/11.7/include -L/opt/nvidia/hpc_sdk/Linux_x86_64/22.7/cuda/11.7/lib64 -lnvToolsExt

FC := nvfortran
FLAGS := -O3 -w 
NVTXLIB_f := -lnvhpcwrapnvtx

rdf_f:rdf.f90
	${FC} ${FLAGS} ${ACCFLAGS} rdf.f90 -o rdf_f ${NVTXLIB_f} 
    
    
rdf_c: rdf.cpp
	${CC} ${CFLAGS} ${ACCFLAGS} -o rdf_c rdf.cpp ${NVTXLIB_c} 

clean:
	rm -f *.o rdf_c rdf_f