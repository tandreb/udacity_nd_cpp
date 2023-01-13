#include "format.h"

#include <sstream>
#include <string>

using std::string;

string Format::getTwoDigitsNumber(const int number) {
  std::stringstream sstream;

  if (number < 10) {
    sstream << "0";
  }
  sstream << number;

  return sstream.str();
}

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int HH = seconds / 3600;
  int remaining_seconds = seconds % 3600;
  int MM = remaining_seconds / 60;

  int SS = remaining_seconds % 60;

  std::stringstream timeAsString;

  timeAsString << getTwoDigitsNumber(HH) << ":" << getTwoDigitsNumber(MM) << ":"
               << getTwoDigitsNumber(SS);

  return timeAsString.str();
}
