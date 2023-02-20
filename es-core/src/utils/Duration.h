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

#include <ctime>

namespace Utils {
class Duration {
  public:
    explicit Duration(const time_t &_time);
    ~Duration() = default;

    Duration(const Duration &other) = default;
    Duration(Duration &&other) noexcept = default;
    Duration &operator=(const Duration &other) = default;
    Duration &operator=(Duration &&other) noexcept = default;

    /**
     * @brief Get the total seconds of the duration
     * @return The total seconds of the duration
     */
    [[nodiscard]] unsigned int getDays() const {
        return mDays;
    }

    /**
     * @brief Get the total seconds of the duration
     * @return The total seconds of the duration
     */
    [[nodiscard]] unsigned int getHours() const {
        return mHours;
    }

    /**
     * @brief Get the total seconds of the duration
     * @return The total seconds of the duration
     */
    [[nodiscard]] unsigned int getMinutes() const {
        return mMinutes;
    }

    /**
     * @brief Get the total seconds of the duration
     * @return The total seconds of the duration
     */
    [[nodiscard]] unsigned int getSeconds() const {
        return mSeconds;
    }

  private:
    unsigned int mTotalSeconds;
    unsigned int mDays;
    unsigned int mHours;
    unsigned int mMinutes;
    unsigned int mSeconds;
};
} // namespace Utils
