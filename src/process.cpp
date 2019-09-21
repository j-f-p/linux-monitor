#include "process.h"

// #include <unistd.h> // TODO: Employ or delete.
// #include <cctype> // TODO: Employ or delete.
// #include <sstream> // TODO: Employ or delete.
// Included and needed in process.h:
// linux_parser.h which requires inclusion of <vector>
// <string> // to_string

using std::string;
using std::to_string;
// using std::vector; // TODO: Employ or delete.

// Return this process's ID
int Process::Pid() { return id; }

// Return the user (name) that generated this process.
string Process::User() { return LinuxParser::User(id); }

// Return this process's CPU utilization.
float Process::CpuUtilization() {
  return fraction_cpu;
}

// Return this process's memory utilization.
string Process::Ram() {
//if (ram > -1) // This case is guaranteed in System::Processes().
    return to_string(ram);
//return "-";
}

// Return the age of this process (in seconds).
long int Process::UpTime() { return uptime; }

// Return the command that generated this process.
string Process::Command() {
  return LinuxParser::Command(id);
}

// Overload the "greater than" comparison operator for Process objects,
// for sorting processes according to CPU utilization.
bool Process::operator>(Process const& a) const {
  if(fraction_cpu > a.fraction_cpu)
    return true;
  return false;
}

// Overload the "greater than" comparison operator for Process objects,
// for sorting processes according to memory utilization.
// bool Process::operator>(Process const& a) const {
//   if(ram > a.ram)
//     return true;
//   return false;
//  }
