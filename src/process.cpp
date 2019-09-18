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
// using std::to_string; // TODO: Employ or delete.
// using std::vector; // TODO: Employ or delete.

// Return this process's ID
int Process::Pid() { return id; }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(id); }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
