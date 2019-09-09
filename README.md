# Linux Monitor
<!-- TODO: Replace statement below with:
This is a student completion of ...
-->
Objective: This is an ongoing student effort toward the completion of the [System Monitor Project](https://github.com/udacity/CppND-System-Monitor).

## Description
This project is a Linux system process monitor. It is written in C++, compliant with the 2017 standard. This project employs [ncurses](https://www.gnu.org/software/ncurses/) for graphical output, specifically, `libncurses5-dev`.

<!-- TODO: Replace image below with one of your own. -->
![System Monitor](images/monitor.png)<br>
*Sample Execution*

## Environment
This project was completed on Bento/Ubuntu 19.04 with the following packages installed via package manager `build-essential` and `cmake`. The build-essential package included the GCC 8.3.0 (Ubuntu 8.3.0-6ubuntu1) compiler employed to compile C++ code according to the 2017 standard. After the prior installations, it was noticed that `libncurses5-dev` had already been installed.
<!--
  libncursesw5 is possibly an error. I have built and executed the program without error and only libncurses5 installed.
 -->

## Make
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

## Instructions

1. Clone the project repository: `git clone https://github.com/udacity/CppND-System-Monitor.git`

2. Build the project: `make build`

3. Run the resulting executable: `./build/monitor`
![Starting System Monitor](images/starting_monitor.png)

4. Follow along with the lesson.

5. Implement the `System`, `Process`, and `Processor` classes, as well as functions within the `LinuxParser` namespace.
