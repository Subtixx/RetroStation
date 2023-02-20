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

#include "Duration.h"

using namespace Utils;

constexpr unsigned int SecondsPerMinute = 60;
constexpr unsigned int SecondsPerHour = 60 * SecondsPerMinute;
constexpr unsigned int SecondsPerDay = 24 * SecondsPerHour;

Duration::Duration(const time_t &_time) :
    mTotalSeconds((unsigned int)_time),
    mDays((mTotalSeconds - (mTotalSeconds % (SecondsPerDay))) / (SecondsPerDay)),
    mHours(((mTotalSeconds % (SecondsPerDay)) - (mTotalSeconds % (SecondsPerHour))) / (SecondsPerHour)),
    mMinutes(((mTotalSeconds % (SecondsPerHour)) - (mTotalSeconds % (SecondsPerMinute))) / SecondsPerMinute),
    mSeconds(mTotalSeconds % SecondsPerMinute) {
}