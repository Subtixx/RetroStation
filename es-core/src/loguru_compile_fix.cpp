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

/**
 * Since microsoft are a bunch of idiots and don't properly implement loguru into vcpkg, we must
 * manually fix their mistake by defining the streamlogger and abortlogger manually....
 */

#include <loguru.hpp>
#pragma region Loguru compile fix
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-vararg"
loguru::StreamLogger::~StreamLogger() noexcept(false)
{
    auto message = _ss.str();
    loguru::log(_verbosity, _file, _line, LOGURU_FMT(s), message.c_str());
}

loguru::AbortLogger::~AbortLogger() noexcept(false)
{
    auto message = _ss.str();
    loguru::log_and_abort(1, _expr, _file, _line, LOGURU_FMT(s), message.c_str());
}
#pragma clang diagnostic pop
#pragma endregion
