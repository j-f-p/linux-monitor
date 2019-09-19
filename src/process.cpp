#include "process.h"
#include "linux_parser.h"

// #include <unistd.h> // TODO: Employ or delete.
// #include <cctype> // TODO: Employ or delete.
// #include <sstream> // TODO: Employ or delete.
// Included and needed in process.h:
// <string> // to_string

// Included and needed in linux_parser.h:
// <vector>

using std::string;
using std::to_string;
// using std::vector; // TODO: Employ or delete.

// Return this process's ID
int Process::Pid() { return id; }

// Return the user (name) that generated this process.
string Process::User() { return LinuxParser::User(id); }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// Return this process's memory utilization.
string Process::Ram() { return to_string(ram); }

// Return the age of this process (in seconds).
long int Process::UpTime() { return LinuxParser::UpTime(id); }

// Return the command that generated this process.
string Process::Command() {
  return LinuxParser::Command(id);
}

// Overload the "less than" comparison operator for Process objects.
bool Process::operator<(Process const& a) const {
  if(id < a.id)
    return true;
  return false;
 }

// Overload the "greater than" comparison operator for Process objects.
bool Process::operator>(Process const& a) const {
  if(ram > a.ram)
    return true;
  return false;
 }