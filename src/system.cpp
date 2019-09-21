#include "system.h"

#include "linux_parser.h"

#include <algorithm> // sort
#include <functional> // greater

// #include <cstddef> // size_t // TODO: Employ or delete.
// #include <set> // TODO: Employ or delete.

// Included and needed in system.h:
// process.h which includes <string>
// <vector>

// Inlcuded and needed in linux_parser.h:
// <unist.h> // TODO: Employ or delete these two lines.

// using std::set; // TODO: Employ or delete.
// using std::size_t; // TODO: Employ or delete.
using std::sort;
using std::greater;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes.
//#include <fstream>
vector<Process>& System::Processes() {
  p++;
//std::ofstream writer("debug.txt", std::ios::app);
//writer << p << std::endl;
//Refresh rate test.
  vector<int> pids{LinuxParser::Pids()};
 
//processes_.clear();
  vector<Process>().swap(processes_);
  long mem;
  for(int pid: pids) {
    mem = LinuxParser::Ram(pid);
    if(mem > -1) 
      processes_.emplace_back(pid, mem, LinuxParser::UpTime(pid));
  //else: 
  //  Process status file couldn't be opened. Thus, it is not listed.
  }

  sort(processes_.begin(), processes_.end(), greater<>());
  return processes_;
}

// Return the system's kernel identifier(string).
string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name.
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system.
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system.
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running.
long System::UpTime() { return LinuxParser::UpTime(); }
