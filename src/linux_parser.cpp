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
// <string> // getline, stoi, stol and to_string
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
using std::stol;
using std::to_string;
using std::unordered_map;
using std::vector;

// Read OS from the filesystem.
string LinuxParser::OperatingSystem() {
  ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    string line;
    string key;
    string value;
    istringstream linestream;
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), '=', ' ');
      replace(line.begin(), line.end(), '"', ' ');
      linestream.str(line);
      linestream >> key >> value;
      if (key == "PRETTY_NAME") {
        replace(value.begin(), value.end(), '_', ' ');
        return value;
      }
      linestream.clear();
    }
  }
  return string();
}

// Read OS version from the filesystem.
string LinuxParser::Kernel() {
  ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    string line;
    getline(stream, line);
    istringstream linestream(line);
    string os, versionLabel, kernel;
    linestream >> os >> versionLabel >> kernel;
    return kernel;
  }
  return string();
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
  ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    string line, key;
    long firstValue;
    float mem_tot, mem_free;
    bool not_done = true;
    istringstream linestream;
    while (not_done and getline(filestream, line)) {
      linestream.str(line);
      linestream >> key >> firstValue;
      if (key == "MemTotal:") {
        mem_tot = firstValue;
      } else
      if (key == "MemFree:") {
        mem_free = firstValue;
        not_done = false;
      }
      linestream.clear();
    }
    return (mem_tot - mem_free) / mem_tot;
  }
  return -1;
}

// Read system uptime from filesystem.
long LinuxParser::UpTime() {
  ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    string line;
    getline(stream, line, ' ');
    return stol(line);
  }
  return -1;
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
    return unordered_map<string, long> {
      {"user", user}, {"nice", nice}, {"system", system},
      {"idle", idle}, {"iowait", iowait},
      {"irq", irq}, {"softirq", softirq}, {"steal", steal}
    };
  }
  return unordered_map<string, long> {};
}

// Read and return the total number of processes.
int LinuxParser::TotalProcesses() {
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line, key;
    int firstValue;
    istringstream linestream;
    while (getline(filestream, line)) {
      linestream.str(line);
      linestream >> key >> firstValue;
      if (key == "processes")
        return firstValue;
      linestream.clear();
    }
  }
  return -1;
}

// Read and return the number of running processes.
int LinuxParser::RunningProcesses() {
  ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    string line, key;
    int firstValue;
    istringstream linestream;
    while (getline(filestream, line)) {
      linestream.str(line);
      linestream >> key >> firstValue;
      if (key == "procs_running")
        return firstValue;
      linestream.clear();
    }
  }
  return -1;
}

// Read and return the user ID associated with a process.
string LinuxParser::Uid(int pid) {
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    string line, key, firstValue;
    istringstream linestream;
    while (getline(filestream, line)) {
      linestream.str(line);
      linestream >> key >> firstValue;
      if (key == "Uid:")
        return firstValue;
      linestream.clear();
    }
  }
  return string();
}

// Read and return the user associated with a process.
string LinuxParser::User(int pid) {
  ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    string uid = LinuxParser::Uid(pid);
    string line, user, pass, user_id;
    istringstream linestream;
    while (getline(filestream, line)) {
      replace(line.begin(), line.end(), ' ', '_');
      replace(line.begin(), line.end(), ':', ' ');
      linestream.str(line);
      while (linestream >> user >> pass >> user_id) {
        if (user_id == uid)
          return user;
      }
      linestream.clear();
    }
  }
  return string();
}

// Read and return the memory used by a process.
long LinuxParser::Ram(int pid) {
  ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    string line, key, firstValue;
    long memory;
    istringstream linestream;
    while (getline(filestream, line)) {
      linestream.str(line);
      linestream >> key >> firstValue;
      if (key == "VmSize:") {
        memory = stol(firstValue); // in kB
        memory /= 1000; // in MB
        return memory;
      }
      linestream.clear();
    }
  }
  return -1;
}

// Read and return the uptime of a process.
long LinuxParser::UpTime(int pid) {
  string line;
  ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    getline(stream, line);
    stream.close();
  }
  else {
    return -1;
  }
  istringstream linestream(line);
  string value;
  const int valNum = 22;
  for(int i=1; i<valNum; i++)
    linestream >> value; // skip first 21 values
  long starttime; // 22nd space-separated value in line stored as a long int
  linestream >> starttime; // process start time in clock ticks after boot
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
