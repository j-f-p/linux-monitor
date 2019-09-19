#include "linux_parser.h"

// dirent.h and unistd.h are used by LinuxParser::Pids()
// unistd.h is also used by LinuxParser::Uptime(int)
#include <dirent.h>
#include <unistd.h> // sysconf(_SC_CLK_TCK)

#include <algorithm> // replace, all_of
#include <fstream> // ifstream
// #include <regex> // regex // TODO: Employ or delete.
#include <sstream> // istringstream
// Included and needed in linux_parser.h:
// <string> // getline, stoi and to_string
// <unordered_map>
// <vector>

using std::replace;
using std::all_of;
using std::ifstream;
// using std::regex; // TODO: Employ or delete.
using std::istringstream;
using std::getline;
using std::string;
using std::stoi;
using std::to_string;
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
    linestream >> uptime_as_long; // time since system boot in seconds
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

// Read and return the user ID associated with a process.
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string firstValue;
  bool not_done = true;
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (not_done and getline(filestream, line)) {
      istringstream linestream(line);
      linestream >> key >> firstValue;
      if (key == "Uid:")
        return firstValue;
    }
  }
  return string();
}

// Read and return the user associated with a process.
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);

  string line;
  string user, pass, user_id;
  ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), ':', ' ');
      istringstream linestream(line);
      while (linestream >> user >> pass >> user_id) {
        if (user_id == uid) {
          return user;
        }
      }
    }
  }

  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) {
  return string();
}

// Read and return the uptime of a process.
long LinuxParser::UpTime(int pid) {
  long starttime; // process start time, measured since boot
  ifstream stream(kProcDirectory  + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    string line, value;
    getline(stream, line);
    istringstream linestream(line);
    const int valNum = 22;
    for(int i=1; i<valNum; i++){
      linestream >> value;
    }
    // 22nd space-separated value in line stored as a long int
    linestream >> starttime; // start time in clock ticks after boot
  }
  starttime /= sysconf(_SC_CLK_TCK); // start time in seconds after boot

  // process uptime = time since boot - process start time since boot
  // process uptime = system uptime - process start time
  return UpTime() - starttime; // in seconds
}

// Read and return the command associated with a process.
string LinuxParser::Command(int pid) {
  ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    string line;
    getline(stream, line);
    return line;
  }
  return string();
}
