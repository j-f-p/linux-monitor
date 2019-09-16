#ifndef SYSTEM_H
#define SYSTEM_H

#include "process.h"
#include "processor.h"

#include <vector>
// Included and needed in process.h:
// <string>

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  // TODO: Define any necessary private members
 private:
  Processor cpu_;
  std::vector<Process> processes_ = {};
};

#endif
