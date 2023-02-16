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

#include <exception>
#include <functional>
#include <sstream>
#include <hedley.h>

#define LOG(level) \
    if (!Log::enabled() || level > Log::getReportingLevel()) \
        ; \
    else \
        Log().get(level)

#define TRYCATCH(m, x) \
    { \
        try { \
            x; \
        } catch (const std::exception &e) { \
            LOG_S(ERROR) << m << " Exception " << e.what(); \
            Log::flush(); \
            throw e; \
        } catch (...) { \
            LOG_S(ERROR) << m << " Unknown Exception occured"; \
            Log::flush(); \
            throw; \
        } \
    }

enum LogLevel { LogError, LogWarning, LogInfo, LogDebug };

// mark as deprecated
HEDLEY_DEPRECATED("Use loguru instead")
class Log {
  public:
    Log() = default;
    ~Log();
    Log(const Log &other) = delete;
    Log &operator=(const Log &other) = delete;
    Log(Log &&other) = delete;
    Log &operator=(Log &&other) = delete;

    std::ostringstream &get(LogLevel level = LogInfo);

    static inline LogLevel &getReportingLevel() {
        return mReportingLevel;
    }
    static inline bool enabled() {
        return mFile != nullptr;
    }

    static void init();
    static void flush();
    static void close();

  private:
    static LogLevel mReportingLevel;
    static bool mDirty;
    static FILE *mFile;

  protected:
    std::ostringstream mStream;
    LogLevel mMessageLevel{LogInfo};
};

class StopWatch {
  public:
    explicit StopWatch(const std::string &elapsedMillisecondsMessage, LogLevel level = LogDebug);

    ~StopWatch();
    StopWatch(const StopWatch &other) = delete;
    StopWatch &operator=(const StopWatch &other) = delete;
    StopWatch(StopWatch &&other) = delete;
    StopWatch &operator=(StopWatch &&other) = delete;

  private:
    std::string mMessage;
    LogLevel mLevel;
    int mStartTicks;
};