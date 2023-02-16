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

#include "LuaScript.h"
#include <loguru.hpp>
#include <functional>


LuaScript::LuaScript() : mLuaState(luaL_newstate()) {
    if (mLuaState == nullptr) {
        std::cerr << "Error creating Lua state" << std::endl;
        return;
    }

    initialize();
}

bool LuaScript::initialize() {
    luaL_openlibs(mLuaState);

    luaL_requiref(mLuaState, "math", luaopen_math, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "string", luaopen_string, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "table", luaopen_table, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "os", luaopen_os, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "io", luaopen_io, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "debug", luaopen_debug, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "package", luaopen_package, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "coroutine", luaopen_coroutine, 1);
    lua_pop(mLuaState, 1);

    luaL_requiref(mLuaState, "utf8", luaopen_utf8, 1);
    lua_pop(mLuaState, 1);

    lua_atpanic(mLuaState, onLuaError);

    lua_register(mLuaState, "print", LuaScript::print);
    lua_register(mLuaState, "log", LuaScript::log);
}

LuaScript::~LuaScript() {
    lua_close(mLuaState);
}

bool LuaScript::loadScript(const std::string &filename) {
    if (luaL_loadfile(mLuaState, filename.c_str()) || lua_pcall(mLuaState, 0, 0, 0)) {
        std::cerr << "Error loading script: " << lua_tostring(mLuaState, -1) << std::endl;
        return false;
    }
    return true;
}

bool LuaScript::unloadScript(const std::string &filename) {
    // TODO: Implement unloading of scripts
    return false;
}

int LuaScript::executeFunction(const std::string &funcName, const std::vector<std::string> &args) {
    // TODO: Implement function execution
    return 0;
}

int LuaScript::print(lua_State *state) {
    const int num = lua_gettop(state);
    for (int i = 1; i <= num; i++) {
        if (i > 1) {
            std::cout << "\t";
        }
        if (lua_isstring(state, i) != 0) {
            std::cout << lua_tostring(state, i);
        } else if (lua_isnil(state, i)) {
            std::cout << "nil";
        } else if (lua_isboolean(state, i)) {
            std::cout << (lua_toboolean(state, i) != 0 ? "true" : "false");
        } else {
            std::cout << lua_typename(state, i);
        }
    }
    std::cout << std::endl;
    return 0;
}

int LuaScript::log(lua_State *state) {
    const int num = lua_gettop(state);
    if (num < 2) {
        LOG_S(ERROR) << "Error: log() requires at least 2 arguments";
        return 1;
    }

    if (lua_isnumber(state, 1) == 0) {
        LOG_S(ERROR) << "Error: log() requires the first argument to be a number";
        return 1;
    }

    auto type = static_cast<LogType>(lua_tointeger(state, 1));
    std::string message;

    for (int i = 2; i <= num; i++) {
        if (i > 2) {
            message += "\t";
        }
        if (lua_isstring(state, i) != 0) {
            message += lua_tostring(state, i);
        } else if (lua_isnil(state, i)) {
            message += "nil";
        } else if (lua_isboolean(state, i)) {
            message += (lua_toboolean(state, i) != 0 ? "true" : "false");
        } else {
            message += lua_typename(state, i);
        }
    }

    switch (type) {
        case LogType::INFO:
            LOG_S(INFO) << message;
            break;
        case LogType::WARNING:
            LOG_S(WARNING) << message;
            break;
        case LogType::ERROR:
            LOG_S(ERROR) << message;
            lua_getglobal(state, "debug");
            lua_getfield(state, -1, "traceback");
            lua_call(state, 0, 1);
            LOG_S(ERROR) << lua_tostring(state, -1);
            lua_pop(state, 1);
            break;
    }

    return 0;
}

int LuaScript::onLuaError(lua_State *state) {
    lua_getglobal(state, "debug");
    lua_getfield(state, -1, "traceback");
    lua_call(state, 0, 1);

    LOG_S(ERROR) << "Lua error: " << lua_tostring(state, -1) << std::endl << lua_tostring(state, -1);

    lua_pop(state, 1);
    return 0;
}
