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

#include "KeyboardMapping.h"
#include "MetaData.h"
#include "SaveState.h"
#include "SystemData.h"
#include "utils/FileSystemUtil.h"
#include <memory>
#include <stack>
#include <unordered_map>
#include <vector>

class Window;
struct SystemEnvironmentData;

enum NetPlayMode { DISABLED, CLIENT, SERVER, SPECTATOR };

struct GetFileContext {
    bool showHiddenFiles;
    bool filterKidGame;
    std::set<std::string> hiddenExtensions;
};

struct LaunchGameOptions {
    LaunchGameOptions() = default;

    int netPlayMode{NetPlayMode::DISABLED};
    std::string ip;
    int port{};

    std::string core;
    std::string netplayClientPassword;

    SaveState saveStateInfo;
};

class FolderData;

/**
 * @brief A tree node that holds information for a file.
 */
class FileData : public IKeyboardMapContainer {
  public:
    enum FileType {
        GAME = 1, // Cannot have children.
        FOLDER = 2,
        PLACEHOLDER = 3
    };

    enum FileChangeType { FILE_ADDED, FILE_METADATA_CHANGED, FILE_REMOVED, FILE_SORTED };

    FileData(FileType type, const std::string &path, SystemData *system);
    virtual ~FileData();
    FileData(const FileData &) = delete;
    FileData &operator=(const FileData &) = delete;
    FileData(FileData &&) = delete;
    FileData &operator=(FileData &&) = delete;

    static FileData *GetRunningGame() {
        return mRunningGame;
    }

    virtual const std::string &getName();

    [[nodiscard]] inline FileType getType() const {
        return mType;
    }

    [[nodiscard]] inline FolderData *getParent() const {
        return mParent;
    }
    inline void setParent(FolderData *parent) {
        mParent = parent;
    }

    [[nodiscard]] inline SystemData *getSystem() const {
        return mSystem;
    }

    [[nodiscard]] virtual const std::string getPath() const;
    std::string getBreadCrumbPath();

    [[nodiscard]] virtual SystemEnvironmentData *getSystemEnvData() const;

    virtual const std::string getThumbnailPath();
    virtual const std::string getVideoPath();
    virtual const std::string getMarqueePath();
    virtual const std::string getImagePath();

    virtual const std::string getCore(bool resolveDefault = true);
    virtual const std::string getEmulator(bool resolveDefault = true);

    virtual bool isNetplaySupported();

    void setCore(const std::string& value);
    void setEmulator(const std::string& value);

    virtual const bool getHidden();
    virtual const bool getFavorite();
    virtual const bool getKidGame();
    virtual const bool hasCheevos();

    bool hasAnyMedia();
    std::vector<std::string> getFileMedias();

    const std::string getConfigurationName();

    [[nodiscard]] inline bool isPlaceHolder() const {
        return mType == PLACEHOLDER;
    };

    virtual std::string getKey();
    const bool isArcadeAsset();
    const bool isVerticalArcadeGame();
    const bool isLightGunGame();

    [[nodiscard]] inline std::string getFullPath() const {
        return getPath();
    };

    [[nodiscard]] inline std::string getFileName() const {
        return Utils::FileSystem::getFileName(getPath());
    };
    virtual FileData *getSourceFileData();
    [[nodiscard]] virtual std::string getSystemName() const;

    // Returns our best guess at the "real" name for this file (will attempt to perform MAME name translation)
    virtual std::string &getDisplayName();

    // As above, but also remove parenthesis
    std::string getCleanName();

    std::string getLaunchCommand(bool includeControllers = true) {
        LaunchGameOptions options;
        return getLaunchCommand(options, includeControllers);
    };
    std::string getLaunchCommand(LaunchGameOptions &options, bool includeControllers = true);

    bool launchGame(Window *window, LaunchGameOptions options = LaunchGameOptions());

    static void resetSettings();

    [[nodiscard]] virtual const MetaDataList &getMetadata() const {
        return mMetadata;
    }

    virtual MetaDataList &getMetadata() {
        return mMetadata;
    }

    void setMetadata(const MetaDataList& value) {
        getMetadata() = value;
    }

    std::string getMetadata(MetaDataId key) {
        return getMetadata().get(key);
    }
    void setMetadata(MetaDataId key, const std::string &value) {
        return getMetadata().set(key, value);
    }

    void detectLanguageAndRegion(bool overWrite);

    void deleteGameFiles();

    void checkCrc32(bool force = false);
    void checkMd5(bool force = false);
    void checkCheevosHash(bool force = false);

    void importP2k(const std::string &p2k);
    std::string convertP2kFile();
    bool hasP2kFile();

    bool hasKeyboardMapping() override;
    KeyMappingFile getKeyboardMapping() override;
    bool isFeatureSupported(EmulatorFeatures::Features feature);
    bool isExtensionCompatible();

    std::string getCurrentGameSetting(const std::string &settingName);

    bool hasContentFiles();
    std::set<std::string> getContentFiles();

    void setSelectedGame();

    std::string getProperty(const std::string &name);

  private:
    std::string getKeyboardMappingFilePath();
    std::string getMessageFromExitCode(int exitCode);
    MetaDataList mMetadata;

  protected:
    std::string findLocalArt(const std::string &type = "", std::vector<std::string> exts = {".png", ".jpg"});

    static FileData *mRunningGame;

    FolderData *mParent;
    std::string mPath;
    FileType mType;
    SystemData *mSystem;
    std::string *mDisplayName;
};
