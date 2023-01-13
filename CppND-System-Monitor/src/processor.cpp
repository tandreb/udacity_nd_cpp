#include "processor.h"

#include "linux_parser.h"

Processor::Processor()
    : previousIdle_(LinuxParser::IdleJiffies()),
      previousNonIdle_(LinuxParser::ActiveJiffies()),
      previousTotal_(previousIdle_ + previousNonIdle_) {}

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long currentIdle = LinuxParser::IdleJiffies();
  long currentNonIdle = LinuxParser::ActiveJiffies();
  long currentTotal = currentIdle + currentNonIdle;

  long deltaIdle = currentIdle - previousIdle_;
  long deltaTotal = currentTotal - previousTotal_;

  previousIdle_ = currentIdle;
  previousNonIdle_ = currentNonIdle;
  previousTotal_ = currentTotal;

  float utilization = 0.0f;
  if (deltaTotal == 0 || (deltaTotal < deltaIdle)) {
    return utilization;
  }

  utilization = 1.0 * (deltaTotal - deltaIdle) / deltaTotal;
  if (utilization < 0) {
    utilization = 0.0f;
  }

  return utilization;
}