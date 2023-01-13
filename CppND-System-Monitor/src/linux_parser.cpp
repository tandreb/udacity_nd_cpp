#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
  string os, kernel, version;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  const string filepath(kProcDirectory + kMeminfoFilename);

  float totalMemory =
      readSimpleValueFromFile<float>(filepath, "MemTotal:");
  float freeMemory = readSimpleValueFromFile<float>(filepath, "MemFree:");

  float memoryUsage = (totalMemory - freeMemory) / totalMemory;
  return memoryUsage;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = 0;
  std::ifstream ifStream(kProcDirectory + kUptimeFilename);

  if (ifStream.is_open()) {
    ifStream >> uptime;
  }
  return uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]) + stol(jiffies[CPUStates::kIdle_]) +
         stol(jiffies[CPUStates::kIOwait_]);
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE:  once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long totalTime = 0;

  std::stringstream path;
  path << kProcDirectory << pid << kStatFilename;

  std::ifstream filestream(path.str());
  if (filestream.is_open()) {
    const int position = 13;
    std::string uselessValue;
    for (int i = 0; i < position; i++) {
      if (!(filestream >> uselessValue)) {
        return 10000;
      }
    }

    long userTimeTicks, kernelTimeTicks = 0, userChildrenTimeTicks = 0,
                        kernelChildrenTimeTicks = 0;
    if (filestream >> userTimeTicks >> kernelTimeTicks >>
        userChildrenTimeTicks >> kernelChildrenTimeTicks) {
      totalTime = (userTimeTicks + kernelTimeTicks + userChildrenTimeTicks +
                   kernelChildrenTimeTicks);
    }
  }

  return totalTime / sysconf(_SC_CLK_TCK);
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, cpu, value;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> cpu;

    while (linestream >> value) {
      jiffies.push_back(value);
    }
  }
  return jiffies;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return readSimpleValueFromFile<int>(kProcDirectory + kStatFilename,
                                      "processes");
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string filepath = kProcDirectory + kStatFilename;
  return readSimpleValueFromFile<int>(filepath, "procs_running");
}

// DONE: Read and return the command associated with a process
// REMOVE:  once you define the function
string LinuxParser::Command(int pid) {
  string cmd = " ";

  std::ifstream ifStream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (ifStream.is_open()) {
    string line;
    if (std::getline(ifStream, line)) {
      cmd = line;
    }
  }
  return cmd;
}

// DONE: Read and return the memory used by a process
// REMOVE:  once you define the function
string LinuxParser::Ram(int pid) {
  int ram = readSimpleValueFromFile<int>(
      kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:");
  int ramInMb = ram / 1000;
  return to_string(ramInMb);
}

// DONE: Read and return the user ID associated with a process
// REMOVE:  once you define the function
string LinuxParser::Uid(int pid) {
  string uid = "0000";
  uid = readSimpleValueFromFile<string>(
      kProcDirectory + to_string(pid) + kStatusFilename, "Uid:");
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE:  once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string userName = "none";
  std::ifstream ifStream(kPasswordPath);

  if (ifStream.is_open()) {
    string line;

    while (std::getline(ifStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string currId;
      string notNeededField;

      if (linestream >> userName >> notNeededField >> currId) {
        if (currId == uid) {
          return userName;
        }
      }
    }
  }

  return userName;
}

// DONE: Read and return the uptime of a process
// REMOVE:  once you define the function
long LinuxParser::UpTime(int pid) {
  long upTimeInClockTicks = 0;

  std::ifstream ifStream(kProcDirectory + to_string(pid) + kStatFilename);
  if (ifStream.is_open()) {
    const int position = 22;
    std::string value;
    for (int i = 0; i < position; i++) {
      if (!(ifStream >> value)) {
        return 10000;
      }
    }
    upTimeInClockTicks = std::stol(value);
  }

  return upTimeInClockTicks / sysconf(_SC_CLK_TCK);
}
