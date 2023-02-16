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

#include "Settings.h"

#include <iostream>
#include <argagg/argagg.hpp>

#include "Paths.h"
#include "utils/FileSystemUtil.h"

extern std::string gPlayVideo;
extern int gPlayVideoDuration;
extern bool enable_startup_game;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-c-arrays"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"

int ParseScreenArguments(argagg::parser_results &args) {
    if (args["monitor"]) {
        auto const monitorId = args["monitor"].as<int>();

        // TODO: check if monitor exists
        Settings::getInstance()->setInt("MonitorID", monitorId);
    }

    if (args["resolution"]) {
        auto const resolution = args["resolution"].as<std::string>();
        int width = 0;
        int height = 0;

        if (resolution.find('x') != std::string::npos) {
            auto const pos = resolution.find('x');
            width = std::stoi(resolution.substr(0, pos));
            height = std::stoi(resolution.substr(pos + 1));
        } else {
            auto const pos = resolution.find(' ');
            width = std::stoi(resolution.substr(0, pos));
            height = std::stoi(resolution.substr(pos + 1));
        }

        if (width == 0 || height == 0) {
            std::cerr << "Invalid resolution: " << resolution << '\n';
            return EXIT_FAILURE;
        }

        Settings::getInstance()->setInt("WindowWidth", width);
        Settings::getInstance()->setInt("WindowHeight", height);
        Settings::getInstance()->setBool("FullscreenBorderless", false);
    }

    if (args["screensize"]) {
        auto const resolution = args["screensize"].as<std::string>();

        int width = 0;
        int height = 0;

        if (resolution.find('x') != std::string::npos) {
            auto const pos = resolution.find('x');
            width = std::stoi(resolution.substr(0, pos));
            height = std::stoi(resolution.substr(pos + 1));
        } else {
            auto const pos = resolution.find(' ');
            width = std::stoi(resolution.substr(0, pos));
            height = std::stoi(resolution.substr(pos + 1));
        }

        Settings::getInstance()->setInt("ScreenWidth", width);
        Settings::getInstance()->setInt("ScreenHeight", height);
    }

    if (args["screenoffset"]) {
        auto const offset = args["screenoffset"].as<std::string>();

        int xOffset = 0;
        int yOffset = 0;

        if (offset.find(',') != std::string::npos) {
            auto const pos = offset.find(',');
            xOffset = std::stoi(offset.substr(0, pos));
            yOffset = std::stoi(offset.substr(pos + 1));
        } else {
            auto const pos = offset.find(' ');
            xOffset = std::stoi(offset.substr(0, pos));
            yOffset = std::stoi(offset.substr(pos + 1));
        }

        Settings::getInstance()->setInt("ScreenOffsetX", xOffset);
        Settings::getInstance()->setInt("ScreenOffsetY", yOffset);
    }

    if (args["screenrotate"]) {
        int const rotate = args["screenrotate"].as<int>();
        Settings::getInstance()->setInt("ScreenRotate", rotate);
    }


    if (args["fullscreen-borderless"]) {
        Settings::getInstance()->setBool("FullscreenBorderless", true);
        Settings::getInstance()->setBool("Windowed", false);
    }

    if (args["fullscreen"]) {
        Settings::getInstance()->setBool("FullscreenBorderless", false);
    }

    if (args["windowed"]) {
        Settings::getInstance()->setBool("FullscreenBorderless", false);
        Settings::getInstance()->setBool("Windowed", true);
    }

    if (args["vsync"]) {
        Settings::getInstance()->setBool("VSync", args["vsync"].as<bool>());
    }

    if (args["max-vram"]) {
        Settings::getInstance()->setInt("MaxVRAM", args["max-vram"].as<int>());
    }

    return EXIT_SUCCESS;
}

int ParseArguments(int argc, char *argv[]) {
    Paths::setExePath(argv[0]);

    const argagg::parser argparser{{
        {"help", {"-p", "--help"}, "shows this help message", 0},
        {"home", {"-h", "--home"}, "Set the home path", 1},
        {"resolution", {"-r", "--resolution"}, "Set the resolution", 2},
        {"gamelist-only", {"--gamelist-only"}, "Skip automatic game search, only read from gamelist.xml", 0},
        {"ignore-gamelist", { "--ignore-gamelist"}, "Ignore the gamelist (useful for troubleshooting)", 0},
        {"draw-framerate", { "--draw-framerate"}, "Display the framerate", 0},
        {"no-exit", { "--no-exit"}, "Don't show the exit option in the menu", 0},
        {"no-splash", {"-s", "--no-splash"}, "Don't show the splash screen", 0},
        {"debug", {"-d", "--debug"}, "More logging, show console on Windows", 0},
        {"windowed", {"-w", "--windowed"}, "Not fullscreen, should be used with --resolution", 0},
        {"vsync", {"-v", "--vsync"}, "Turn vsync on or off (default is on)", 1},
        {"max-vram", { "--max-vram"}, "Max VRAM to use in Mb before swapping. 0 for unlimited", 1},
        {"force-kid", {"--force-kid"}, "Force the UI mode to be Kid", 0},
        {"force-kiosk", {"-k", "--force-kiosk"}, "Force the UI mode to be Kiosk", 0},
        {"force-disable-filters", {"-f", "--force-disable-filters"}, "Force the UI to ignore applied filters in gamelist", 0},
        {"monitor", {"-m", "--monitor"}, "Monitor index", 1},
        {"video", {"--video"}, "Video to play", 1},
        {"screenoffset", { "--screenoffset"}, "Screen offset", 2},
        {"screenrotate", { "--screenrotate"}, "Screen rotation", 1},
        {"videoduration", { "--videoduration"}, "Video duration", 1},
        {"show-hidden-files", {"--show-hidden-files"}, "Show hidden files", 0},
        {"screensize", {"--screensize"}, "Set the screen size", 2},
        {"exit-on-reboot-required", {"-e", "--exit-on-reboot-required"}, "Exit on reboot required", 0},
        {"no-startup-game", { "--no-startup-game"}, "Don't start a game on startup", 0},
        {"splash-image", { "--splash-image"}, "Splash image", 1},
        {"fullscreen-borderless", {"-b", "--fullscreen-borderless"}, "Fullscreen borderless", 0},
        {"fullscreen", { "--fullscreen"}, "Fullscreen", 0},

    }};

    argagg::parser_results args;
    try {
        args = argparser.parse(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    if (args["help"]) {
        std::cout << argparser << '\n';
        return EXIT_SUCCESS;
    }

    if (args["home"]) {
        // check if path exists
        if (!Utils::FileSystem::isDirectory(args["home"].as<std::string>())) {
            std::cerr << "Home path does not exist: " << args["home"].as<std::string>() << '\n';
            return EXIT_FAILURE;
        }
        Settings::getInstance()->setString("HomePath", args["home"].as<std::string>());
    }

    if(ParseScreenArguments(args) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    if (args["videoduration"]) {
        gPlayVideoDuration = args["videoduration"].as<int>();
    }

    if (args["video"]) {
        gPlayVideo = args["video"].as<std::string>();
    }

    if (args["gamelist-only"]) {
        Settings::getInstance()->setBool("ParseGamelistOnly", true);
    }

    if (args["ignore-gamelist"]) {
        Settings::getInstance()->setBool("IgnoreGamelist", true);
    }

    if (args["show-hidden-files"]) {
        Settings::setShowHiddenFiles(true);
    }

    if (args["draw-framerate"]) {
        Settings::getInstance()->setBool("DrawFramerate", true);
    }

    if (args["no-exit"]) {
        Settings::getInstance()->setBool("ShowExit", false);
    }

    if (args["exit-on-reboot-required"]) {
        Settings::getInstance()->setBool("ExitOnRebootRequired", true);
    }

    if (args["no-startup-game"]) {
        enable_startup_game = false;
    }

    if (args["no-splash"]) {
        Settings::getInstance()->setBool("SplashScreen", false);
    }

    if (args["splash-image"]) {
        const auto& splashImagePath = args["splash-image"].as<std::string>();
        if (!Utils::FileSystem::exists(splashImagePath)) {
            std::cerr << "Splash image does not exist: " << splashImagePath << '\n';
        } else {
            Settings::getInstance()->setString("AlternateSplashScreen", splashImagePath);
        }
    }

    if (args["debug"]) {
        Settings::getInstance()->setBool("Debug", true);
        Settings::getInstance()->setBool("HideConsole", false);
    }

    if (args["force-kiosk"]) {
        Settings::getInstance()->setBool("ForceKiosk", true);
    }

    if (args["force-kid"]) {
        Settings::getInstance()->setBool("ForceKid", true);
    }

    if (args["force-disable-filters"]) {
        Settings::getInstance()->setBool("ForceDisableFilters", true);
    }

    return EXIT_SUCCESS;
}
#pragma clang diagnostic pop