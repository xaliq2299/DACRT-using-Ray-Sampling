# Divide-And-Conquer Ray Tracing (DACRT) using Ray Sampling
### Introduction
This repository contains the implementation of the rendering approach described in this [paper](http://web.wakayama-u.ac.jp/~iwasaki/project/dacrt/hpg2013final.pdf).

### Requirements
* C++17 compiler
* CMake
* OpenMP
The tested platform is Linux.

### Running the project
#### Basic setup
Go the folder of the project and type the following commands on the terminal:
```
mkdir build
cd build
cmake ..
make
cd ..
```
At this stage, you should have the executable file in the *build* folder under the name **MyRayTracer**. To be able to run the program, you should type the following command with the option -d (or -dacrt) to indicate whether you want the Ray Tracing with the paper's approach or not:
```
./build/MyRayTracer -d 1
```
This command will launch the program taking into account the rendering approach of the paper since we have set the -d option (DACRT) to 1 (True). Otherwise, you can set it to 0 (False) and logically, it should take more running time. Since we have not set any input filename, by default it will operate on the *low_res_face.off* mesh file that can be found in the **mesh_library** folder.

#### Options
The DACRT option is important for our program to run properly, however we also have other options available:
* -d or -dacrt: sets True/False to indicate whether we want the approach from the paper or not. Requires 0 or 1 after.
* -w or -width: sets a certain width for the image. Requires an integer value.
* -h or -height: sets a certain height for the image. Requires an integer value.
* -i or -input: sets an input filename. Requires a string value.
* -o or -output: sets an output filename. Requires a string value.
* -h or -help: displays the usage of the program.
Note that in case of incorrect and inappropriate option choices, the program will terminate resulting in Segmentation Fault.
