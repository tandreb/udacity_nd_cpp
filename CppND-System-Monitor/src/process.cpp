#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  CpuUtilization();
  command_ = LinuxParser::Command(pid_);
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  int totalTimeActive = LinuxParser::ActiveJiffies(pid_);
  int processUpTime = LinuxParser::UpTime(pid_);

  cpuUtilization_ = (1.0 * totalTimeActive) / processUpTime;

  return (cpuUtilization_);
}

// DONE: Return the command that generated this process
// only returning stored value which is read out once during construction of
// process as this value won't change during runtime of the process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return cpuUtilization_ < a.cpuUtilization_;
}

bool Process::operator>(Process const& a) const {
  return cpuUtilization_ > a.cpuUtilization_;
}