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

#include <csignal>
#include <iostream>

#include <FreeImage.h>
#include <SDL_events.h>
#include <SDL_main.h>
#include <SDL_timer.h>
#include <SystemConf.h>
#include <ctime>

#include <loguru.hpp>

#include "ApiSystem.h"
#include "AudioManager.h"
#include "CollectionSystemManager.h"
#include "CommandLineArguments.h"
#include "EmulationStation.h"
#include "Genres.h"
#include "ImageIO.h"
#include "InputConfig.h"
#include "InputManager.h"
#include "LocaleES.h"
#include "Log.h"
#include "MameNames.h"
#include "NetworkThread.h"
#include "Paths.h"
#include "PowerSaver.h"
#include "RetroAchievements.h"
#include "Settings.h"
#include "SystemData.h"
#include "SystemScreenSaver.h"
#include "TextToSpeech.h"
#include "ThreadedHasher.h"
#include "components/VideoVlcComponent.h"
#include "guis/GuiDetectDevice.h"
#include "guis/GuiMsgBox.h"
#include "platform.h"
#include "resources/TextureData.h"
#include "scrapers/ThreadedScraper.h"
#include "scripting/LuaScript.h"
#include "services/HttpServerThread.h"
#include "utils/FileSystemUtil.h"
#include "views/ViewController.h"

#ifdef WIN32
#include <Windows.h>
#include <direct.h>
#define PATH_MAX MAX_PATH
#include "client/windows/handler/exception_handler.h"
#else
#include "client/linux/handler/exception_handler.h"
#endif

std::string gPlayVideo;
int gPlayVideoDuration = 0;
bool enable_startup_game = true;

static bool dumpCallback(const google_breakpad::MinidumpDescriptor &descriptor, void *context, bool succeeded) {
    printf("Dump path: %s\n", descriptor.path());
    return succeeded;
}

bool verifyHomeFolderExists() {
    // make sure the config directory exists
    std::string const configDir = Paths::getUserEmulationStationPath();
    if (!Utils::FileSystem::exists(configDir)) {
        std::cout << "Creating config directory \"" << configDir << "\"\n";
        Utils::FileSystem::createDirectory(configDir);
        if (!Utils::FileSystem::exists(configDir)) {
            std::cerr << "Config directory could not be created!\n";
            return false;
        }
    }

    return true;
}

// Returns true if everything is OK,
bool loadSystemConfigFile(Window *window, const char **errorString) {
    *errorString = nullptr;

    StopWatch const stopWatch("loadSystemConfigFile :", LogDebug);

    ImageIO::loadImageCache();

    if (!SystemData::loadConfig(window)) {
        LOG_S(ERROR) << "Error while parsing systems configuration file!";
        *errorString = "IT LOOKS LIKE YOUR SYSTEMS CONFIGURATION FILE HAS NOT BEEN SET UP OR IS INVALID. YOU'LL "
                       "NEED TO DO THIS BY HAND, UNFORTUNATELY.\n\n"
                       "VISIT EMULATIONSTATION.ORG FOR MORE INFORMATION.";
        return false;
    }

    if (SystemData::sSystemVector.empty()) {
        LOG_S(ERROR) << "No systems found! Does at least one system have a game present? (check that extensions "
                         "match!)\n(Also, make sure you've updated your es_systems.cfg for XML!)";
        *errorString = "WE CAN'T FIND ANY SYSTEMS!\n"
                       "CHECK THAT YOUR PATHS ARE CORRECT IN THE SYSTEMS CONFIGURATION FILE, "
                       "AND YOUR GAME DIRECTORY HAS AT LEAST ONE GAME WITH THE CORRECT EXTENSION.\n\n"
                       "VISIT EMULATIONSTATION.ORG FOR MORE INFORMATION.";
        return false;
    }

    return true;
}

// called on exit, assuming we get far enough to have the log initialized
void onExit() {
    Log::close();
}

#ifdef WIN32
#define PATH_MAX MAX_PATH
#include <direct.h>
#endif

int setLocale(char * /*argv1*/) {
#if WIN32
    std::locale::global(std::locale("en-US"));
#else
    if (Utils::FileSystem::exists("./locale/lang")) { // for local builds
        EsLocale::init("", "./locale/lang");
    } else {
        EsLocale::init("", "/usr/share/locale");
    }
#endif

    setlocale(LC_TIME, "");

    return 0;
}

void signalHandler(int signum) {
    if (signum == SIGSEGV) {
        LOG_S(ERROR) << "Interrupt signal SIGSEGV received.\n";
    } else if (signum == SIGFPE) {
        LOG_S(ERROR) << "Interrupt signal SIGFPE received.\n";
    } else
        LOG_S(ERROR) << "Interrupt signal (" << signum << ") received.\n";

    // cleanup and close up stuff here
    exit(signum);
}

void playVideo() {
    ApiSystem::getInstance()->setReadyFlag(false);
    Settings::getInstance()->setBool("AlwaysOnTop", true);

    Window window;
    if (!window.init(true)) {
        LOG_S(ERROR) << "Window failed to initialize!";
        return;
    }

    Settings::getInstance()->setBool("VideoAudio", true);

    bool exitLoop = false;

    VideoVlcComponent vid(&window);
    vid.setVideo(gPlayVideo);
    vid.setOrigin(0.5F, 0.5F);
    vid.setPosition(Renderer::getScreenWidth() / 2.0F, Renderer::getScreenHeight() / 2.0F);
    vid.setMaxSize(Renderer::getScreenWidth(), Renderer::getScreenHeight());

    vid.setOnVideoEnded([&exitLoop]() {
        exitLoop = true;
        return false;
    });

    window.pushGui(&vid);

    vid.onShow();
    vid.topWindow(true);

    int lastTime = SDL_GetTicks();
    int totalTime = 0;

    while (!exitLoop) {
        SDL_Event event;

        if (SDL_PollEvent(&event) != 0) {
            do {
                if (event.type == SDL_QUIT) {
                    return;
}
            } while (SDL_PollEvent(&event) != 0);
        }

        int curTime = SDL_GetTicks();
        int deltaTime = curTime - lastTime;

        if (vid.isPlaying()) {
            totalTime += deltaTime;

            if (gPlayVideoDuration > 0 && totalTime > gPlayVideoDuration * 100) {
                break;
}
        }

        Transform4x4f transform = Transform4x4f::Identity();
        vid.update(deltaTime);
        vid.render(transform);

        Renderer::swapBuffers();

        if (ApiSystem::getInstance()->isReadyFlagSet()) {
            break;
}
    }

    window.deinit(true);
}

void launchStartupGame() {
    auto gamePath = SystemConf::getInstance()->get("global.bootgame.path");
    if (gamePath.empty() || !Utils::FileSystem::exists(gamePath)) {
        return;
}

    auto command = SystemConf::getInstance()->get("global.bootgame.cmd");
    if (!command.empty()) {
        InputManager::getInstance()->init();
        command =
            Utils::String::replace(command, "%CONTROLLERSCONFIG%", InputManager::getInstance()->configureEmulators());
        runSystemCommand(command, gamePath, nullptr);
    }
}

int main(int argc, char *argv[]) {
    google_breakpad::MinidumpDescriptor descriptor("/tmp");
    google_breakpad::ExceptionHandler eh(descriptor, NULL, dumpCallback, NULL, true, -1);

    loguru::init(argc, argv);

    // signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGSEGV, signalHandler);
    // signal(SIGTERM, signalHandler);

    srand((unsigned int)time(nullptr));

    std::locale::global(std::locale("C"));

    if(ParseArguments(argc, argv) != EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }

        // only show the console on Windows if HideConsole is false
#ifdef WIN32
    // MSVC has a "SubSystem" option, with two primary options: "WINDOWS" and "CONSOLE".
    // In "WINDOWS" mode, no console is automatically created for us.  This is good,
    // because we can choose to only create the console window if the user explicitly
    // asks for it, preventing it from flashing open and then closing.
    // In "CONSOLE" mode, a console is always automatically created for us before we
    // enter main. In this case, we can only hide the console after the fact, which
    // will leave a brief flash.
    // TL;DR: You should compile ES under the "WINDOWS" subsystem.
    // I have no idea how this works with non-MSVC compilers.
    if (!Settings::getInstance()->getBool("HideConsole")) {
        // we want to show the console
        // if we're compiled in "CONSOLE" mode, this is already done.
        // if we're compiled in "WINDOWS" mode, no console is created for us automatically;
        // the user asked for one, so make one and then hook stdin/stdout/sterr up to it
        if (AllocConsole()) // should only pass in "WINDOWS" mode
        {
            freopen("CONIN$", "r", stdin);
            freopen("CONOUT$", "wb", stdout);
            freopen("CONOUT$", "wb", stderr);
        }
    } else {
        // we want to hide the console
        // if we're compiled with the "WINDOWS" subsystem, this is already done.
        // if we're compiled with the "CONSOLE" subsystem, a console is already created;
        // it'll flash open, but we hide it nearly immediately
        if (GetConsoleWindow()) // should only pass in "CONSOLE" mode
            ShowWindow(GetConsoleWindow(), SW_HIDE);
    }
#endif

    // call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
#endif

    // if ~/.emulationstation doesn't exist and cannot be created, bail
    if (!verifyHomeFolderExists()) {
        return 1;
    }

    if (!gPlayVideo.empty()) {
        playVideo();
        return 0;
    }

    // start the logger
    Log::init();

    // verbosity
    loguru::Verbosity verbosity = loguru::Verbosity_INFO;
    if (Settings::getInstance()->getBool("Debug")) {
        verbosity = loguru::Verbosity_MAX;
    }
    loguru::g_stderr_verbosity = verbosity;

    auto logPath = Paths::getUserEmulationStationPath() + "/es.log";

    loguru::add_file(logPath.c_str(), loguru::Append, verbosity);
    LOG_S(INFO) << "EmulationStation - v" << PROGRAM_VERSION_STRING << ", built " << PROGRAM_BUILT_STRING;

    // always close the log on exit
    atexit(&onExit);

    // Set locale
    setLocale(argv[0]);

    std::shared_ptr<LuaScript> script = std::make_shared<LuaScript>();
    script->loadScript("scripts/main.lua");

#if !WIN32
    if (enable_startup_game) {
        // Run boot game, before Window Create for linux
        launchStartupGame();
    }
#endif

    // metadata init
    Genres::init();
    MetaDataList::initMetadata();

    Window window;
    SystemScreenSaver screensaver(&window);
    PowerSaver::init();
    ViewController::init(&window);
    CollectionSystemManager::init(&window);
    VideoVlcComponent::init();

    window.pushGui(ViewController::get());
    if (!window.init(true, false)) {
        LOG_S(ERROR) << "Window failed to initialize!";
        return 1;
    }

    bool splashScreen = Settings::getInstance()->getBool("SplashScreen");
    bool splashScreenProgress = Settings::getInstance()->getBool("SplashScreenProgress");

    if (splashScreen) {
        std::string progressText = _("Loading...");
        if (splashScreenProgress) {
            progressText = _("Loading system config...");
}

        window.renderSplashScreen(progressText);
    }

    MameNames::init();

    const char *errorMsg = nullptr;
    if (!loadSystemConfigFile(splashScreen && splashScreenProgress ? &window : nullptr, &errorMsg)) {
        // something went terribly wrong
        if (errorMsg == nullptr) {
            LOG_S(ERROR) << "Unknown error occured while parsing system config file.";
            Renderer::deinit();
            return 1;
        }

        // we can't handle es_systems.cfg file problems inside ES itself, so display the error message then quit
        window.pushGui(new GuiMsgBox(&window, errorMsg, _("QUIT"), [] { quitES(); }));
    }

    SystemConf *systemConf = SystemConf::getInstance();

#ifdef _ENABLE_KODI_
    if (systemConf->getBool("kodi.enabled", true) && systemConf->getBool("kodi.atstartup")) {
        if (splashScreen) {
            window.closeSplashScreen();
}

        ApiSystem::getInstance()->launchKodi(&window);

        if (splashScreen) {
            window.renderSplashScreen("");
            splashScreen = false;
        }
    }
#endif

    if (ApiSystem::getInstance()->isScriptingSupported(ApiSystem::PDFEXTRACTION))
    {
        TextureData::PdfHandler = ApiSystem::getInstance();
    }

    ApiSystem::getInstance()->getIpAdress();

    // preload what we can right away instead of waiting for the user to select it
    // this makes for no delays when accessing content, but a longer startup time
    ViewController::get()->preload();

    // Initialize input
    InputConfig::AssignActionButtons();
    InputManager::getInstance()->init();
    SDL_StopTextInput();

    NetworkThread *nthread = new NetworkThread(&window);
    HttpServerThread httpServer(&window);

    // tts
    TextToSpeech::getInstance()->enable(Settings::getInstance()->getBool("TTS"), false);

    if (errorMsg == nullptr) {
        ViewController::get()->goToStart(true);
}

    window.closeSplashScreen();

    // Create a flag in  temporary directory to signal READY state
    ApiSystem::getInstance()->setReadyFlag();

    // Play music
    AudioManager::getInstance()->init();

    if (ViewController::get()->getState().viewing == ViewController::GAME_LIST ||
        ViewController::get()->getState().viewing == ViewController::SYSTEM_SELECT) {
        AudioManager::getInstance()->changePlaylist(ViewController::get()->getState().getSystem()->getTheme());
    } else {
        AudioManager::getInstance()->playRandomMusic();
}

#ifdef WIN32
    DWORD displayFrequency = 60;

    DEVMODE lpDevMode;
    memset(&lpDevMode, 0, sizeof(DEVMODE));
    lpDevMode.dmSize = sizeof(DEVMODE);
    lpDevMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFLAGS | DM_DISPLAYFREQUENCY;
    lpDevMode.dmDriverExtra = 0;

    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode) != 0) {
        displayFrequency = lpDevMode.dmDisplayFrequency; // default value if cannot retrieve from user settings.
    }

    int timeLimit = (1000 / displayFrequency) - 10; // Margin for vsync
    if (timeLimit < 0)
        timeLimit = 0;
#endif

    int lastTime = SDL_GetTicks();
    int ps_time = SDL_GetTicks();

    bool running = true;

    while (running) {
#ifdef WIN32
        int processStart = SDL_GetTicks();
#endif

        SDL_Event event;

        bool ps_standby = PowerSaver::getState() && (int)SDL_GetTicks() - ps_time > PowerSaver::getMode();
        if (ps_standby ? SDL_WaitEventTimeout(&event, PowerSaver::getTimeout()) : SDL_PollEvent(&event) != 0) {
            // PowerSaver can push events to exit SDL_WaitEventTimeout immediatly
            // Reset this event's state
            TRYCATCH("resetRefreshEvent", PowerSaver::resetRefreshEvent());

            do {
                TRYCATCH("InputManager::parseEvent", InputManager::getInstance()->parseEvent(event, &window));

                if (event.type == SDL_QUIT) {
                    running = false;
}
            } while (SDL_PollEvent(&event) != 0);

            // check guns
            InputManager::getInstance()->updateGuns(&window);

            // triggered if exiting from SDL_WaitEvent due to event
            if (ps_standby) {
                // show as if continuing from last event
                lastTime = SDL_GetTicks();
}

            // reset counter
            ps_time = SDL_GetTicks();
        } else if (!ps_standby) {
            // check guns
            InputManager::getInstance()->updateGuns(&window);

            // If exitting SDL_WaitEventTimeout due to timeout. Trail considering
            // timeout as an event
            //	ps_time = SDL_GetTicks();
        }

        if (window.isSleeping()) {
            lastTime = SDL_GetTicks();
            SDL_Delay(1); // this doesn't need to be accurate, we're just giving up our CPU time until something
                          // wakes us up
            continue;
        }

        int curTime = SDL_GetTicks();
        int deltaTime = curTime - lastTime;
        lastTime = curTime;

        // cap deltaTime if it ever goes negative
        if (deltaTime < 0) {
            deltaTime = 1000;
}

        TRYCATCH("Window.update", window.update(deltaTime))
        TRYCATCH("Window.render", window.render())

#ifdef WIN32
        int processDuration = SDL_GetTicks() - processStart;
        if (processDuration < timeLimit) {
            int timeToWait = timeLimit - processDuration;
            if (timeToWait > 0 && timeToWait < 25 && Settings::VSync())
                Sleep(timeToWait);
        }
#endif

        Renderer::swapBuffers();

        Log::flush();
    }

    if (isFastShutdown()) {
        Settings::getInstance()->setBool("IgnoreGamelist", true);
    }

    ThreadedHasher::stop();
    ThreadedScraper::stop();

    ApiSystem::getInstance()->deinit();

    while (window.peekGui() != ViewController::get()) {
        delete window.peekGui();
    }

    if (SystemData::hasDirtySystems()) {
        window.renderSplashScreen(_("SAVING METADATA. PLEASE WAIT..."));
    }

    ImageIO::saveImageCache();
    MameNames::deinit();
    ViewController::saveState();
    CollectionSystemManager::deinit();
    SystemData::deleteSystems();

    // call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
    FreeImage_DeInitialise();
#endif

    window.deinit();

    processQuitMode();

    LOG_S(INFO) << "EmulationStation cleanly shutting down.";

    return 0;
}
