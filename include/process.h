#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, long mem): id(pid), ram(mem) {};
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

  // TODO: Declare any necessary private members
 private:
   int id;
   long ram;
};

#endif
