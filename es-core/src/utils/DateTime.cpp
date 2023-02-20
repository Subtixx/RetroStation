/*
 * Copyright (c) Subtixx (Dominic Hock) 2023.
 * Copyright (c) 2014 Alec Lofquist
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 */

#include "DateTime.h"
#include <array>

using namespace Utils;

DateTime::DateTime() : mTimeStruct({0, 0, 0, 1, 0, 0, 0, 0, -1}) {
}

DateTime::DateTime(const time_t &_time) {
    setTime(_time);
}

DateTime::DateTime(const tm &_timeStruct) {
    setTimeStruct(_timeStruct);
}

DateTime::DateTime(const std::string &_isoString) {
    setIsoString(_isoString);
}

void DateTime::setTime(const time_t &_time) {
    try {
        mTime = (_time < 0) ? 0 : _time;
        mTimeStruct = *localtime(&mTime);
        mIsoString = Time::timeToString(mTime);
    } catch (...) {
        mTime = 0;
        mTimeStruct = {0, 0, 0, 1, 0, 0, 0, 0, -1};
        mIsoString = "00000000T000000";
    }
}

void DateTime::setTimeStruct(const tm &_timeStruct) {
    setTime(mktime((tm *)&_timeStruct));

}

void DateTime::setIsoString(const std::string &_isoString) {
    setTime(Time::stringToTime(_isoString));

}

double DateTime::elapsedSecondsSince(const DateTime &_since) const {
    return difftime(mTime, _since.mTime);
}

double DateTime::elapsedMinutesSince(const DateTime &_since) const {
    return elapsedSecondsSince(_since) / 60.0;
}

double DateTime::elapsedHoursSince(const DateTime &_since) const {
    return elapsedMinutesSince(_since) / 60.0;
}

std::string DateTime::toLocalTimeString() const {
    time_t const clockNow = getTime();
    struct tm const clockTstruct = *localtime(&clockNow);

    std::array<char, 256> clockBuf{};
    if (strftime(clockBuf.data(), clockBuf.size(), "%x %R", &clockTstruct) == 0) {
        return "00/00/0000 00:00";
    }
    return clockBuf.data();
}
