#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, long mem, long ut): id(pid), ram(mem), uptime(ut) {};
  int Pid();
  std::string User();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  std::string Command();
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;

 private:
   int id;
   long ram;
   long uptime;
};

#endif
