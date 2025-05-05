# GLFramework

To submit your work on moodle, we ask you to use [`cmake`](https://cmake.org/) to create a project for the platform you are using. A detailed cmake tutorial can be found [here](https://cmake.org/getting-started/). 

A very simplistic example can be found below (Ubuntu).

1. Install cmake 

```bash
sudo apt update
sudo apt install cmake
```

2. Navigate to the code and run cmake. (You might need to install further dependencies such that the build works with no errors.)

```bash
cd glframework
mkdir build
cd build 
cmake ..
make 
``` 

3. Open the application
```bash
./GLFramework
```
