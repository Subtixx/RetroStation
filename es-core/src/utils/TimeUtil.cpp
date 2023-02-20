#include "utils/TimeUtil.h"
#include "LocaleES.h"

namespace Utils::Time {
time_t now() {
    time_t time;
    ::time(&time);
    return time;

} // now

time_t stringToTime(const std::string &_string, const std::string &_format) {
    const char *s = _string.c_str();
    const char *f = _format.c_str();
    tm timeStruct = {0, 0, 0, 1, 0, 0, 0, 0, -1};
    size_t parsedChars = 0;

    if (_string == "not-a-date-time")
        return mktime(&timeStruct);

    while (*f && (parsedChars < _string.length())) {
        if (*f == '%') {
            ++f;

            switch (*f++) {
            case 'Y': // The year [1970,xxxx]
            {
                if ((parsedChars + 4) <= _string.length()) {
                    timeStruct.tm_year = (*s++ - '0') * 1000;
                    timeStruct.tm_year += (*s++ - '0') * 100;
                    timeStruct.tm_year += (*s++ - '0') * 10;
                    timeStruct.tm_year += (*s++ - '0');
                    if (timeStruct.tm_year >= 1900)
                        timeStruct.tm_year -= 1900;
                }

                parsedChars += 4;
            } break;

            case 'm': // The month number [01,12]
            {
                if ((parsedChars + 2) <= _string.length()) {
                    timeStruct.tm_mon = (*s++ - '0') * 10;
                    timeStruct.tm_mon += (*s++ - '0');
                    if (timeStruct.tm_mon >= 1)
                        timeStruct.tm_mon -= 1;
                }

                parsedChars += 2;
            } break;

            case 'd': // The day of the month [01,31]
            {
                if ((parsedChars + 2) <= _string.length()) {
                    timeStruct.tm_mday = (*s++ - '0') * 10;
                    timeStruct.tm_mday += (*s++ - '0');
                }

                parsedChars += 2;
            } break;

            case 'H': // The hour (24-hour clock) [00,23]
            {
                if ((parsedChars + 2) <= _string.length()) {
                    timeStruct.tm_hour = (*s++ - '0') * 10;
                    timeStruct.tm_hour += (*s++ - '0');
                }

                parsedChars += 2;
            } break;

            case 'M': // The minute [00,59]
            {
                if ((parsedChars + 2) <= _string.length()) {
                    timeStruct.tm_min = (*s++ - '0') * 10;
                    timeStruct.tm_min += (*s++ - '0');
                }

                parsedChars += 2;
            } break;

            case 'S': // The second [00,59]
            {
                if ((parsedChars + 2) <= _string.length()) {
                    timeStruct.tm_sec = (*s++ - '0') * 10;
                    timeStruct.tm_sec += (*s++ - '0');
                }

                parsedChars += 2;
            } break;
            }
        } else {
            ++s;
            ++f;
        }
    }

    return mktime(&timeStruct);

} // stringToTime

std::string timeToString(const time_t &_time, const std::string &_format) {
    const char *f = _format.c_str();
    const tm timeStruct = *localtime(&_time);
    char buf[256] = {'\0'};
    char *s = buf;

    while (*f) {
        if (*f == '%') {
            ++f;

            switch (*f++) {
            case 'Y': // The year, including the century (1900)
            {
                const int year = timeStruct.tm_year + 1900;
                *s++ = (char)((year - (year % 1000)) / 1000) + '0';
                *s++ = (char)(((year % 1000) - (year % 100)) / 100) + '0';
                *s++ = (char)(((year % 100) - (year % 10)) / 10) + '0';
                *s++ = (char)(year % 10) + '0';
            } break;

            case 'm': // The month number [00,11]
            {
                const int mon = timeStruct.tm_mon + 1;
                *s++ = (char)(mon / 10) + '0';
                *s++ = (char)(mon % 10) + '0';
            } break;

            case 'd': // The day of the month [01,31]
            {
                *s++ = (char)(timeStruct.tm_mday / 10) + '0';
                *s++ = (char)(timeStruct.tm_mday % 10) + '0';
            } break;

            case 'H': // The hour (24-hour clock) [00,23]
            {
                *s++ = (char)(timeStruct.tm_hour / 10) + '0';
                *s++ = (char)(timeStruct.tm_hour % 10) + '0';
            } break;

            case 'I': // The hour (12-hour clock) [01,12]
            {
                int h = timeStruct.tm_hour;
                if (h >= 12)
                    h -= 12;
                if (h == 0)
                    h = 12;

                *s++ = (char)(h / 10) + '0';
                *s++ = (char)(h % 10) + '0';
            } break;

            case 'p': // AM / PM
            {
                *s++ = timeStruct.tm_hour < 12 ? 'A' : 'P';
                *s++ = 'M';
            } break;

            case 'M': // The minute [00,59]
            {
                *s++ = (char)(timeStruct.tm_min / 10) + '0';
                *s++ = (char)(timeStruct.tm_min % 10) + '0';
            } break;

            case 'S': // The second [00,59]
            {
                *s++ = (char)(timeStruct.tm_sec / 10) + '0';
                *s++ = (char)(timeStruct.tm_sec % 10) + '0';
            } break;
            }
        } else {
            *s++ = *f++;
        }

        *s = '\0';
    }

    return std::string(buf);

} // timeToString

// transforms a number of seconds into a human readable string
std::string secondsToString(const long seconds) {
    if (seconds == 0)
        return _("never");

    char buf[256];

    int h = 0, m = 0, s = 0;
    h = (seconds / 3600) % 24;
    m = (seconds / 60) % 60;
    s = seconds % 60;

    if (h > 0) {
        snprintf(buf, 256, _("%d h").c_str(), h);
        if (m > 0) {
            std::string hours(buf);
            snprintf(buf, 256, _("%d mn").c_str(), m);
            return hours + " " + std::string(buf);
        }
    } else if (m > 0)
        snprintf(buf, 256, _("%d mn").c_str(), m);
    else
        snprintf(buf, 256, _("%d sec").c_str(), s);

    return std::string(buf);
}

int daysInMonth(const int _year, const int _month) {
    tm timeStruct = {0, 0, 0, 0, _month, _year - 1900, 0, 0, -1};
    mktime(&timeStruct);

    return timeStruct.tm_mday;

} // daysInMonth

int daysInYear(const int _year) {
    tm timeStruct = {0, 0, 0, 0, 0, _year - 1900 + 1, 0, 0, -1};
    mktime(&timeStruct);

    return timeStruct.tm_yday + 1;

} // daysInYear

} // namespace Utils::Time
