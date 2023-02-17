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

#include "Debug.h"

#include <loguru.hpp>

bool Debug::TryCatch(const std::function<void()> &func, const std::string &message) {
    try {
        func();
    } catch (const std::exception &e) {
        LOG_S(ERROR) << message << " Exception " << e.what();
        throw e;
    } catch (...) {
        LOG_S(ERROR) << message << " Unknown Exception occured";
        throw;
    }
    return true;
}

bool Debug::TryCatch(const std::function<bool()> &func, const std::string &message) {
    try {
        return func();
    } catch (const std::exception &e) {
        LOG_S(ERROR) << message << " Exception " << e.what();
        throw e;
    } catch (...) {
        LOG_S(ERROR) << message << " Unknown Exception occured";
        throw;
    }
}
