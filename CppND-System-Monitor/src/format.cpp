#include "format.h"

#include <sstream>
#include <string>

using std::string;

string Format::getTwoDigitsNumber(const int number) {
  std::stringstream ss;

  if (number < 10) {
    ss << "0";
  }
  ss << number;

  return ss.str();
}

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int const HH = seconds / 3600;
  int const remaining_seconds = seconds % 3600;
  int const MM = remaining_seconds / 60;

  int const SS = remaining_seconds % 60;

  std::stringstream timeAsString;

  timeAsString << getTwoDigitsNumber(HH) << ":" << getTwoDigitsNumber(MM) << ":"
               << getTwoDigitsNumber(SS);

  return timeAsString.str();
}
