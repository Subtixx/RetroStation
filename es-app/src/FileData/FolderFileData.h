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

#include "FileData.h"

class FolderData : public FileData {
    friend class FileData;
    friend class SystemData;

  public:
    FolderData(const std::string &startpath, SystemData *system, bool ownsChildrens = true);
    ~FolderData();

    inline bool isVirtualStorage() {
        return !mOwnsChildrens;
    }
    inline bool isVirtualFolderDisplay() {
        return mIsDisplayableAsVirtualFolder && !mOwnsChildrens;
    }

    void enableVirtualFolderDisplay(bool value) {
        mIsDisplayableAsVirtualFolder = value;
    };
    bool isVirtualFolderDisplayEnabled() {
        return mIsDisplayableAsVirtualFolder;
    };

    FileData *FindByPath(const std::string &path);

    inline const std::vector<FileData *> &getChildren() const {
        return mChildren;
    }
    const std::vector<FileData *> getChildrenListToDisplay();
    std::shared_ptr<std::vector<FileData *>>
    findChildrenListToDisplayAtCursor(FileData *toFind, std::stack<FileData *> &stack);

    std::vector<FileData *> getFilesRecursive(
        unsigned int typeMask,
        bool displayedOnly = false,
        SystemData *system = nullptr,
        bool includeVirtualStorage = true) const;
    std::vector<FileData *> getFlatGameList(bool displayedOnly, SystemData *system) const;

    void addChild(FileData *file, bool assignParent = true); // Error if mType != FOLDER
    void removeChild(FileData *file);                        // Error if mType != FOLDER

    void createChildrenByFilenameMap(std::unordered_map<std::string, FileData *> &map);

    FileData *findUniqueGameForFolder();

    void clear();
    void removeVirtualFolders();
    void removeFromVirtualFolders(FileData *game);

  private:
    void getFilesRecursiveWithContext(
        std::vector<FileData *> &out,
        unsigned int typeMask,
        GetFileContext *filter,
        bool displayedOnly,
        SystemData *system,
        bool includeVirtualStorage) const;

    std::vector<FileData *> mChildren;
    bool mOwnsChildrens;
    bool mIsDisplayableAsVirtualFolder;
};