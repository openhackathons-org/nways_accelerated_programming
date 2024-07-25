# Copyright (c) 2020 NVIDIA Corporation.  All rights reserved. 

CC := nvc++
CFLAGS := -O3 -w -ldl
ACCFLAGS := -Minfo=accel
VER=$(shell nvc -dumpversion)
NVARCH=$(shell uname -s)_$(shell uname -m)
NVTXLIB_c := -I/opt/nvidia/hpc_sdk/$(NVARCH)/$(VER)/cuda/include

FC := nvfortran
FLAGS := -O3 -w 
NVTXLIB_f := -lnvhpcwrapnvtx

rdf_f:rdf.f90
	${FC} ${FLAGS} ${ACCFLAGS} rdf.f90 -o rdf_f ${NVTXLIB_f} 
    
    
rdf_c: rdf.cpp
	${CC} ${CFLAGS} ${ACCFLAGS} -o rdf_c rdf.cpp ${NVTXLIB_c} 

clean:
	rm -f *.o rdf_c rdf_f