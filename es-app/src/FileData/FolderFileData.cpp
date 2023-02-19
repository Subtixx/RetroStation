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

#include "FolderFileData.h"
#include "CollectionSystemManager.h"
#include "FileSorts.h"
#include "Settings.h"
#include "utils/StringUtil.h"
#include "views/UIModeController.h"

FolderData::FolderData(const std::string& startpath, SystemData* system, bool ownsChildrens) : FileData(FOLDER, startpath, system)
{
    mIsDisplayableAsVirtualFolder = false;
    mOwnsChildrens = ownsChildrens;
}

FolderData::~FolderData()
{
    clear();
}

void FolderData::clear()
{
    if (mOwnsChildrens)
    {
        for (int i = mChildren.size() - 1; i >= 0; i--)
            delete mChildren.at(i);
    }

    mChildren.clear();
}

void FolderData::removeFromVirtualFolders(FileData* game)
{
    for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
    {
        if ((*it)->getType() == FOLDER)
        {
            ((FolderData*)(*it))->removeFromVirtualFolders(game);
            continue;
        }

        if ((*it) == game)
        {
            mChildren.erase(it);
            return;
        }
    }
}

std::shared_ptr<std::vector<FileData*>> FolderData::findChildrenListToDisplayAtCursor(FileData* toFind, std::stack<FileData*>& stack)
{
    auto items = getChildrenListToDisplay();

    for (auto item : items)
        if (toFind == item)
            return std::make_shared<std::vector<FileData*>>(items);

    for (auto item : items)
    {
        if (item->getType() != FOLDER)
            continue;

        stack.push(item);

        auto ret = ((FolderData*)item)->findChildrenListToDisplayAtCursor(toFind, stack);
        if (ret != nullptr)
            return ret;

        stack.pop();
    }

    if (stack.empty())
        return std::make_shared<std::vector<FileData*>>(items);

    return nullptr;
}

FileData* FolderData::findUniqueGameForFolder()
{
    auto games = this->getFilesRecursive(GAME);

    FileData* found = nullptr;

    int count = 0;
    for (auto game : games)
    {
        if (game->getHidden())
        {
            bool showHiddenFiles = Settings::ShowHiddenFiles() && !UIModeController::getInstance()->isUIModeKiosk();

            auto shv = Settings::getInstance()->getString(getSystem()->getName() + ".ShowHiddenFiles");
            if (shv == "1") showHiddenFiles = true;
            else if (shv == "0") showHiddenFiles = false;

            if (!showHiddenFiles)
                continue;
        }

        found = game;
        count++;
        if (count > 1)
            break;
    }

    if (count == 1)
        return found;
    /*{
        auto it = games.cbegin();
        if ((*it)->getType() == GAME)
            return (*it);
    }
    */
    return nullptr;
}



void FolderData::getFilesRecursiveWithContext(std::vector<FileData*>& out, unsigned int typeMask, GetFileContext* filter, bool displayedOnly, SystemData* system, bool includeVirtualStorage) const
{
    if (filter == nullptr)
        return;

    auto isVirtualFolder = [](FileData* file)
    {
        if (file->getType() == GAME)
            return false;

        FolderData* fld = (FolderData*)file;
        return fld->isVirtualStorage();
    };

    SystemData* pSystem = (system != nullptr ? system : mSystem);

    FileFilterIndex* idx = pSystem->getIndex(false);

    for (auto it : mChildren)
    {
        if (it->getType() & typeMask)
        {
            if (!displayedOnly || idx == nullptr || !idx->isFiltered() || idx->showFile(it))
            {
                if (displayedOnly)
                {
                    if (!filter->showHiddenFiles && it->getHidden())
                        continue;

                    if (filter->filterKidGame && it->getKidGame())
                        continue;

                    if (typeMask == GAME && filter->hiddenExtensions.size() > 0)
                    {
                        std::string extlow = Utils::String::toLower(Utils::FileSystem::getExtension(it->getFileName(), false));
                        if (filter->hiddenExtensions.find(extlow) != filter->hiddenExtensions.cend())
                            continue;
                    }
                }

                if (includeVirtualStorage || !isVirtualFolder(it))
                    out.push_back(it);
            }
        }

        if (it->getType() != FOLDER)
            continue;

        FolderData* folder = (FolderData*)it;
        if (folder->getChildren().size() > 0)
        {
            if (includeVirtualStorage || !isVirtualFolder(folder))
            {
                if (folder->isVirtualStorage() && folder->getSourceFileData()->getSystem()->isGroupChildSystem() && folder->getSourceFileData()->getSystem()->getName() == "windows_installers")
                    out.push_back(it);
                else
                    folder->getFilesRecursiveWithContext(out, typeMask, filter, displayedOnly, system, includeVirtualStorage);
            }
        }
    }
}

std::vector<FileData*> FolderData::getFlatGameList(bool displayedOnly, SystemData* system) const
{
    return getFilesRecursive(GAME, displayedOnly, system);
}

std::vector<FileData*> FolderData::getFilesRecursive(unsigned int typeMask, bool displayedOnly, SystemData* system, bool includeVirtualStorage) const
{
    SystemData* pSystem = (system != nullptr ? system : mSystem);

    GetFileContext ctx;
    ctx.showHiddenFiles = Settings::ShowHiddenFiles() && !UIModeController::getInstance()->isUIModeKiosk();

    auto shv = Settings::getInstance()->getString(getSystem()->getName() + ".ShowHiddenFiles");
    if (shv == "1")
        ctx.showHiddenFiles = true;
    else if (shv == "0")
        ctx.showHiddenFiles = false;

    if (pSystem->isGameSystem() && !pSystem->isCollection())
    {
        for (auto ext : Utils::String::split(Utils::String::toLower(Settings::getInstance()->getString(pSystem->getName() + ".HiddenExt")), ';'))
            if (ctx.hiddenExtensions.find(ext) == ctx.hiddenExtensions.cend())
                ctx.hiddenExtensions.insert(ext);
    }

    ctx.filterKidGame = UIModeController::getInstance()->isUIModeKid();

    std::vector<FileData*> out;
    getFilesRecursiveWithContext(out, typeMask, &ctx, displayedOnly, system, includeVirtualStorage);
    return out;
}

void FolderData::addChild(FileData* file, bool assignParent)
{
#if DEBUG
    assert(file->getParent() == nullptr || !assignParent);
#endif

    mChildren.push_back(file);

    if (assignParent)
        file->setParent(this);
}

void FolderData::removeChild(FileData* file)
{
#if DEBUG
    assert(mType == FOLDER);
    assert(file->getParent() == this);
#endif

    for (auto it = mChildren.cbegin(); it != mChildren.cend(); it++)
    {
        if (*it == file)
        {
            file->setParent(NULL);
            mChildren.erase(it);
            return;
        }
    }

    // File somehow wasn't in our children.
#if DEBUG
    assert(false);
#endif
}

FileData* FolderData::FindByPath(const std::string& path)
{
    std::vector<FileData*> children = getChildren();

    for (std::vector<FileData*>::const_iterator it = children.cbegin(); it != children.cend(); ++it)
    {
        if ((*it)->getPath() == path)
            return (*it);

        if ((*it)->getType() != FOLDER)
            continue;

        auto item = ((FolderData*)(*it))->FindByPath(path);
        if (item != nullptr)
            return item;
    }

    return nullptr;
}

void FolderData::createChildrenByFilenameMap(std::unordered_map<std::string, FileData*>& map)
{
    std::vector<FileData*> children = getChildren();

    for (std::vector<FileData*>::const_iterator it = children.cbegin(); it != children.cend(); ++it)
    {
        if ((*it)->getType() == FOLDER)
            ((FolderData*)(*it))->createChildrenByFilenameMap(map);
        else
            map[(*it)->getKey()] = (*it);
    }
}

void FolderData::removeVirtualFolders()
{
    if (!mOwnsChildrens)
        return;

    for (int i = mChildren.size() - 1; i >= 0; i--)
    {
        auto file = mChildren.at(i);
        if (file->getType() != FOLDER)
            continue;

        if (((FolderData*)file)->mOwnsChildrens)
            continue;

        removeChild(file);
        delete file;
    }
}

const std::vector<FileData*> FolderData::getChildrenListToDisplay()
{
    std::vector<FileData*> ret;

    std::string showFoldersMode = getSystem()->getFolderViewMode();

    bool showHiddenFiles = Settings::ShowHiddenFiles();

    auto shv = Settings::getInstance()->getString(getSystem()->getName() + ".ShowHiddenFiles");
    if (shv == "1") showHiddenFiles = true;
    else if (shv == "0") showHiddenFiles = false;

    bool filterKidGame = false;

    if (!Settings::getInstance()->getBool("ForceDisableFilters"))
    {
        if (UIModeController::getInstance()->isUIModeKiosk())
            showHiddenFiles = false;

        if (UIModeController::getInstance()->isUIModeKid())
            filterKidGame = true;
    }

    auto sys = CollectionSystemManager::get()->getSystemToView(mSystem);

    std::vector<std::string> hiddenExts;
    if (mSystem->isGameSystem() && !mSystem->isCollection())
        hiddenExts = Utils::String::split(Utils::String::toLower(Settings::getInstance()->getString(mSystem->getName() + ".HiddenExt")), ';');

    FileFilterIndex* idx = sys->getIndex(false);
    if (idx != nullptr && !idx->isFiltered())
        idx = nullptr;

    std::vector<FileData*>* items = &mChildren;

    std::vector<FileData*> flatGameList;
    if (showFoldersMode == "never")
    {
        flatGameList = getFlatGameList(false, sys);
        items = &flatGameList;
    }

    std::map<FileData*, int> scoringBoard;

    bool refactorUniqueGameFolders = (showFoldersMode == "having multiple games");

    for (auto it = items->cbegin(); it != items->cend(); it++)
    {
        if (!showHiddenFiles && (*it)->getHidden())
            continue;

        if (filterKidGame && (*it)->getType() == GAME && !(*it)->getKidGame())
            continue;

        if (hiddenExts.size() > 0 && (*it)->getType() == GAME)
        {
            std::string extlow = Utils::String::toLower(Utils::FileSystem::getExtension((*it)->getFileName(), false));
            if (std::find(hiddenExts.cbegin(), hiddenExts.cend(), extlow) != hiddenExts.cend())
                continue;
        }

        if (idx != nullptr)
        {
            int score = idx->showFile(*it);
            if (score == 0)
                continue;

            scoringBoard[*it] = score;
        }

        if ((*it)->getType() == FOLDER && refactorUniqueGameFolders)
        {
            FolderData* pFolder = (FolderData*)(*it);
            if (pFolder->getChildren().size() == 0)
                continue;

            if (pFolder->isVirtualStorage() && pFolder->getSourceFileData()->getSystem()->isGroupChildSystem() && pFolder->getSourceFileData()->getSystem()->getName() == "windows_installers")
            {
                ret.push_back(*it);
                continue;
            }

            auto fd = pFolder->findUniqueGameForFolder();
            if (fd != nullptr)
            {
                if (idx != nullptr && !idx->showFile(fd))
                    continue;

                if (!showHiddenFiles && fd->getHidden())
                    continue;

                if (filterKidGame && !fd->getKidGame())
                    continue;

                ret.push_back(fd);

                continue;
            }
        }

        ret.push_back(*it);
    }

    unsigned int currentSortId = sys->getSortId();
    if (currentSortId > FileSorts::getSortTypes().size())
        currentSortId = 0;

    const FileSorts::SortType& sort = FileSorts::getSortTypes().at(currentSortId);

    if (idx != nullptr && idx->hasRelevency())
    {
        auto compf = sort.comparisonFunction;

        std::sort(ret.begin(), ret.end(), [scoringBoard, compf](const FileData* file1, const FileData* file2) -> bool
                  {
                      auto s1 = scoringBoard.find((FileData*) file1);
                      auto s2 = scoringBoard.find((FileData*) file2);

                      if (s1 != scoringBoard.cend() && s2 != scoringBoard.cend() && s1->second != s2->second)
                          return s1->second < s2->second;

                      return compf(file1, file2);
                  });
    }
    else
    {
        std::sort(ret.begin(), ret.end(), sort.comparisonFunction);

        if (!sort.ascending)
            std::reverse(ret.begin(), ret.end());
    }

    return ret;
}
