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
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
  unsigned int pp() { return p; }

  // TODO: Define any necessary private members
 private:
  Processor cpu_;
  std::vector<Process> processes_;
  unsigned int p {0};
};

#endif
