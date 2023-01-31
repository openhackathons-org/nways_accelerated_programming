[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) 

# HPC_Bootcamp

This repository contains training content for the HPC_Bootcamp materials. This repository includes the following file structure in the initial two levels:

```
 _advanced
│   ├── 
├── _basic
│   ├── cuda_basic
│   ├── iso
│   ├── openacc_basic
│   └── openmp
├── LICENSE
├── README.md
├── _scripts
└── start_notebook
```

- The __advanced_ directory contains all of the advanced training materials for CUDA, OpenACC, and multiGPU.
- The __basic_ directory contains all of the introductory training materials for CUDA, Standard Languages, OpenMP Offloading, and OpenACC.
- The __scripts_ directory contains container definition files for each bootcamp type.

Please note there is a container definition file for each content in `_advanced` and `_basic` directories that can be found inside the `_scripts` folder and those can be used on their own without mixing with other contents. 

### Building the container using the definition files inside the `_script` folder

To build the singularity container, run: 
`sudo singularity build miniapp.simg {Name of the content}_Singularity` , alternatively you can use `singularity build --fakeroot miniapp.simg {Name of the content}_Singularity` if you do not have `sudo` rights.

Next, copy the files to a local directory to make sure changes are stored locally:
`singularity run miniapp.simg cp -rT /labs ~/labs`

Then, run the container:
`singularity run --nv miniapp.simg jupyter-lab --notebook-dir=~/labs`

Once inside the container, open the jupyter lab in browser: http://localhost:8888, and start the lab by clicking on the `_start_{Name of the content}.ipynb` notebook. 

