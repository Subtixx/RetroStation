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

#include <string>
#include <vector>
#include <iostream>
#include <lua.hpp>

class LuaScript {
  private:

    enum class LogType : int {
        INFO = 0,
        WARNING,
        ERROR
    };

    lua_State *mLuaState;
  public:
    LuaScript();
    ~LuaScript();
    LuaScript(const LuaScript &) = delete;
    LuaScript &operator=(const LuaScript &) = delete;
    LuaScript(LuaScript &&) = delete;
    LuaScript &operator=(LuaScript &&) = delete;

    bool initialize();

    bool loadScript(const std::string &filename);

    bool unloadScript(const std::string &filename);
    int executeFunction(const std::string &funcName, const std::vector<std::string> &args);
    static int print(lua_State *state);
    static int log(lua_State *state);
    static int onLuaError(lua_State *state);
};