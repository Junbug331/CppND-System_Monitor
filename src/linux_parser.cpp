#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <unistd.h>
#include "linux_parser.h"

using std::stoi;
using std::stol;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string key;
  std::string value;
  std::string line;
  float MemTotal = -1.0;
  float MemFree = -1.0;
  float MemUsed = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if (MemTotal > 0.0 && MemFree > 0.0) {
        break;
      }
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = std::stof(value);
        }
        else if (key == "MemFree") {
          MemFree = std::stof(value);
        }
      }
    }
  }
  MemUsed = MemTotal - MemFree;
  return MemUsed/MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::string line;
  std::string uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  long uptimelong = std::stol(uptime); 
  return uptimelong;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  std::vector<string> cpuState(10, "");
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    if(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string tmp;
      linestream >> tmp;
      for (int i = 0; i < 10; i++) {
        linestream >> cpuState[i];
      }
    }
  }
  return cpuState; 
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int numOfProcesses = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          numOfProcesses = stoi(value);
        }
      }
    }
  }
  return numOfProcesses; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0;
  string line;
  string key;
  string value;
  int numOfProcesses = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          numOfProcesses = stoi(value);
        }
      }
    }
  }
  return numOfProcesses; 
}


// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
   string command ="";
   std::ifstream filestream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
   if (filestream.is_open()) {
     std::getline(filestream, command);
   }

  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  string line, key, ram = "";
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> ram) {
        if (key == "VmSize") {
          return ram;
        }
      }
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value, uid = "";
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line, _Dummy, username = "", uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> username >> _Dummy >> uid) {
        if (uid == LinuxParser::Uid(pid))
          return username;
      }
    }
  }
  return username; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, uptime;
  std::ifstream filestream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
    if (getline(filestream, line)) {
      std::istringstream linestream(line);
      for (int i = 0; i < 22; i++) {
        linestream >> uptime;
      }
    }
  }
  
  return sysconf(stol(uptime) / sysconf(_SC_CLK_TCK));
}