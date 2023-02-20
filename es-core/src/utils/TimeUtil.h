#pragma once

#include <string>
#include <ctime>

namespace Utils::Time {
time_t now();
time_t stringToTime(const std::string &_string, const std::string &_format = "%Y%m%dT%H%M%S");
std::string timeToString(const time_t &_time, const std::string &_format = "%Y%m%dT%H%M%S");
int daysInMonth(const int _year, const int _month);
int daysInYear(const int _year);
std::string secondsToString(const long seconds);

} // namespace Utils::Time