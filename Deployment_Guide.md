# N-ways to GPU programming Deployment Guide
The N-Ways to GPU Programming Bootcamp covers the basics of GPU programming and provides an overview of different methods for porting scientific application to GPUs using NVIDIA® CUDA®, OpenACC, standard languages, OpenMP offloading, and/or CuPy and Numba. Throughout the bootcamp, attendees with learn how to analyze GPU-enabled applications using NVIDIA Nsight™ Systems and participate in hands-on activities to apply these learned skills to real-world problems.

## Deploying the materials

### Prerequisites
To run this tutorial, you will need a machine with NVIDIA GPU.

- Install the latest [Docker](https://docs.nvidia.com/datacenter/cloud-native/container-toolkit/install-guide.html#docker) or [Singularity](https://sylabs.io/docs/).

- Install the latest [NVIDIA Nsight™ Systems](https://developer.nvidia.com/nsight-systems).

- The base containers required for the lab may require users to create an NGC account and generate an API key (https://docs.nvidia.com/ngc/ngc-catalog-user-guide/index.html#registering-activating-ngc-account)

The material is also tested to be working with NVIDIA V100 and T4 GPUs, please contact us if you require assistance in deploying the content.


### Tested environment

These materials was tested with both Docker and Singularity on an NVIDIA A100 GPU in an x86-64 platform installed with a driver version of `525.105.17`. 

### Deploying with container 

These materials can be deployed with either Docker or Singularity container, refer to the respective sections for the instructions.

#### Docker Container

To build a docker container, specify the dockerfile name using `-f` flag: 
`sudo docker build -f <dockerfile name> -t <imagename>:<tagnumber> .`

For instance:

- To build the docker container, for N-Ways to GPU Programming-Python, follow the below steps: 

  1. `sudo docker build -f nways_Dockerfile_python -t openhackathons:nways_python .`
  2. `sudo docker run --rm -it --gpus=all -p 8888:8888 openhackathons:nways_python`
  3. To access the labs, run: `jupyter-lab --ip 0.0.0.0 --port 8888 --no-browser --allow-root`
  4. Now, open the jupyter lab in browser: http://localhost:8888, and start working on the lab by clicking on the `_start_nways.ipynb` notebook


- To build the docker container, for N-Ways to GPU Programming-C-Fortran, follow the below steps:  

  1. `sudo docker build -f nways_Dockerfile -t openhackathons:nways_CFortran .`
  2. `sudo docker run --rm -it --gpus=all -p 8888:8888 openhackathons:nways_CFortran`
  3. To access the labs, run: `jupyter-lab --ip 0.0.0.0 --port 8888 --no-browser --allow-root`
  4. Now, open the jupyter lab in browser: http://localhost:8888, and start working on the lab by clicking on the `_start_nways.ipynb` notebook

Please note, if you are to run both contents, you would need to change the ports to access them seperately.

#### Singularity Container

- To build the singularity container, for N-Ways to GPU Programming-Python, follow the below steps: 

  1. `singularity build --fakeroot nways_python.simg nways_Singularity_python`
  2. `singularity run nways_python.simg cp -rT /labs ~/labs`
  3. `singularity run --nv nways_python.simg jupyter-lab --notebook-dir=~/labs`
  4. Now, open the jupyter lab in browser: http://localhost:8888, and start working on the lab by clicking on the `_start_nways.ipynb` notebook


- To build the singularity container, for N-Ways to GPU Programming-C-Fortran, follow the below steps:   

  1. `singularity build --fakeroot nways_CFortran.simg nways_Singularity`
  2. `singularity run nways_CFortran.simg cp -rT /labs ~/labs`
  3. `singularity run --nv nways_CFortran.simg jupyter-lab --notebook-dir=~/labs`
  4. Now, open the jupyter lab in browser: http://localhost:8888, and start working on the lab by clicking on the `_start_nways.ipynb` notebook

### Known issues

- Please go through the list of exisiting bugs/issues or file a new issue at [Github](https://github.com/openhackathons-org/nways_accelerated_programming/issues).



