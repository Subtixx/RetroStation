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

#pragma once

#include "core_version.h"

#include "TimeUtil.h"
#include <ctime>
#include <string>

#include <hedley.h>

namespace Utils {
/**
 * @brief A class to represent a date and time.
 */
class DateTime {
  public:
    /**
     * @brief Get the current date and time.
     * @return The current date and time.
     */
    static DateTime now() {
        return DateTime{Utils::Time::now()};
    }

    DateTime();

    /**
     * @brief Construct a new DateTime object from a time_t.
     * @param _time The time_t to construct from.
     */
    explicit DateTime(const time_t &_time);

    /**
     * @brief Construct a new DateTime object from a tm struct.
     * @param _timeStruct The tm struct to construct from.
     */
    explicit DateTime(const tm &_timeStruct);

    /**
     * @brief Construct a new DateTime object from an ISO 8601 string.
     * @param _isoString The ISO 8601 string to construct from.
     */
    explicit DateTime(const std::string &_isoString);
    ~DateTime() = default;

    DateTime(const DateTime &_other) = default;
    DateTime &operator=(const DateTime &_other) = default;
    DateTime(DateTime &&_other) noexcept = default;
    DateTime &operator=(DateTime &&_other) noexcept = default;

    bool operator==(const DateTime &_other) const {
        return (mTime == _other.mTime);
    }

    bool operator<(const DateTime &_other) const {
        return (mTime < _other.mTime);
    }

    bool operator<=(const DateTime &_other) const {
        return (mTime <= _other.mTime);
    }

    bool operator>(const DateTime &_other) const {
        return (mTime > _other.mTime);
    }

    bool operator>=(const DateTime &_other) const {
        return (mTime >= _other.mTime);
    }

    /**
     * @brief Returns the time in the specified format.
     * @param _format The format to return the time in.
     */
    [[nodiscard]] std::string toString(const std::string &_format = "%Y%m%dT%H%M%S") const {
        return Utils::Time::timeToString(mTime, _format);
    }

    /**
     * @brief Convert the DateTime to a time_t.
     * @return The time_t.
     */
    explicit operator time_t() const {
        return mTime;
    }

    /**
     * @brief Convert the DateTime to a tm struct.
     * @return The tm struct.
     */
    explicit operator tm() const {
        return mTimeStruct;
    }

    /**
     * @brief Convert the DateTime to an ISO 8601 string.
     * @return The ISO 8601 string.
     */
    explicit operator std::string() const {
        return mIsoString;
    }

    /**
     * @brief Set the time from a time_t struct.
     * @param _time The time_t struct to set from.
     */
    void setTime(const time_t &_time);

    /**
     * @brief Get the time as a time_t struct.
     * @return The time as a time_t struct.
     */
    [[nodiscard]] const time_t &getTime() const {
        return mTime;
    }

    /**
     * @brief Set the time from a tm struct.
     * @param _timeStruct The tm struct to set from.
     */
    void setTimeStruct(const tm &_timeStruct);

    /**
     * @brief Get the time as a tm struct.
     * @return The time as a tm struct.
     */
    [[nodiscard]] const tm &getTimeStruct() const {
        return mTimeStruct;
    }

    /**
     * @brief Set the time from an ISO 8601 string.
     * @param _isoString The ISO 8601 string to set from.
     */
    void setIsoString(const std::string &_isoString);

    /**
     * @brief Get the time as an ISO 8601 string.
     * @return The time as an ISO 8601 string.
     */
    ESCORE_DEPRECATED_SINCE_1_1_FOR("toString")
    const std::string &getIsoString() const {
        return mIsoString;
    }

    /**
     * @brief Get the time as a local time string.
     * @return The time as a local time string.
     */
    ESCORE_DEPRECATED_SINCE_1_1_FOR("toString")
    std::string toLocalTimeString() const;

    /**
     * @brief Get the seconds that have elapsed since the given DateTime.
     * @param _since The DateTime to compare to.
     * @return The seconds that have elapsed since the given DateTime.
     */
    [[nodiscard]] double elapsedSecondsSince(const DateTime &_since) const;

    /**
     * @brief Get the minutes that have elapsed since the given DateTime.
     * @param _since The DateTime to compare to.
     * @return The minutes that have elapsed since the given DateTime.
     */
    [[nodiscard]] double elapsedMinutesSince(const DateTime &_since) const;

    /**
     * @brief Get the hours that have elapsed since the given DateTime.
     * @param _since The DateTime to compare to.
     * @return The hours that have elapsed since the given DateTime.
     */
    [[nodiscard]] double elapsedHoursSince(const DateTime &_since) const;

    /**
     * @brief Returns whether the DateTime is valid.
     * @return Whether the DateTime is valid.
     */
    [[nodiscard]] bool isValid() const {
        return mTime != 0;
    }

  private:
    time_t mTime{0};
    tm mTimeStruct{};
    std::string mIsoString{"00000000T000000"};
};
} // namespace Utils
