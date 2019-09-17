#include "linux_parser.h"

// dirent.h and unistd.h are used by vector<int> LinuxParser::Pids()
#include <dirent.h>
#include <unistd.h>

#include <algorithm> // replace, all_of
#include <fstream> // ifstream
// #include <regex> // TODO: Employ or delete.
#include <sstream> // istringstream
// Included and needed in linux_parser.h:
// <string> // stoi and getline
// <unordered_map>
// <vector>

using std::replace;
using std::all_of;
using std::ifstream;
using std::istringstream;
using std::string;
using std::stoi;
using std::getline;
using std::unordered_map;
using std::vector;

// Read OS from the filesystem.
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read OS version from the filesystem.
string LinuxParser::Kernel() {
  string os, versionLabel, kernel;
  string line;
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> os >> versionLabel >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization.
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  int firstValue;
  float mem_tot, mem_free;
  bool not_done = true;
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (not_done and getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> firstValue;
      if (key == "MemTotal:") {
        mem_tot = firstValue;
      } else
      if (key == "MemFree:") {
        mem_free = firstValue;
        not_done = false;
      }
    }
  }
  return (mem_tot - mem_free) / mem_tot;
}

// Read system uptime from filesystem.
long LinuxParser::UpTime() {
  string line;
  long uptime_as_long;
  ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> uptime_as_long;
  }
  return uptime_as_long;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return CPU utilization.
unordered_map<string, long> LinuxParser::aggregateCPUtickData() {
  string line;
  string label;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    getline(stream, line);
    istringstream linestream(line);
    linestream >> label >> user >> nice >> system
               >> idle >> iowait >> irq >> softirq >> steal;
  }
  return unordered_map<string, long> {
    {"user", user}, {"nice", nice}, {"system", system},
    {"idle", idle}, {"iowait", iowait},
    {"irq", irq}, {"softirq", softirq}, {"steal", steal}
  };
}

// Read and return the total number of processes.
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int firstValue;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> firstValue;
      if (key == "processes")
        return firstValue;
    }
  }
  return -1;
}

// Read and return the number of running processes.
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int firstValue;
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> firstValue;
      if (key == "procs_running")
        return firstValue;
    }
  }
  return -1;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }
