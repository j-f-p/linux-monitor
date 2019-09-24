# Linux Monitor
Objective: This is a completion of the [System Monitor Project](https://github.com/udacity/CppND-System-Monitor).

## Description
This project is a Linux system monitor. Data that it reports includes memory and CPU utilization of the system and of the system's most intense software processes. The top ten processes are listed in order of descending CPU usage. This project is written in C++, compliant with the 2017 standard. It employs the [ncurses](https://www.gnu.org/software/ncurses/) C/C++ user interface library.

![System Monitor](images/screenshot.png)<br>
*Sample Execution Screenshot*

## Environment
The project development was completed on Bento/Ubuntu 19.04 with the following packages `libncurses5-dev`, `build-essential` and `cmake`. The build-essential package included the GCC 8.3.0 (Ubuntu 8.3.0-6ubuntu1) compiler employed to compile C++ code according to the 2017 standard.

## Execution
This project runs on a Linux command line interface. It uses [Make](https://www.gnu.org/software/make/) and [CMake](https://cmake.org/). The following commands are executed in the top project directory, where `Makefile` and `CMakeLists.txt` reside.

1. To build the project executable, enter: `make build`

2. To run the project executable, enter: `build/monitor`
