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
vector<Process>& System::Processes() {
  vector<int> pids{LinuxParser::Pids()};

  processes_.clear();
  for(int pid: pids)
    processes_.emplace_back(pid, LinuxParser::Ram(pid), LinuxParser::UpTime(pid));

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
