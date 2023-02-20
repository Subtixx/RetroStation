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

#include "CollectionFileData.h"

#include "FolderFileData.h"

CollectionFileData::CollectionFileData(FileData *file, SystemData *system) :
    FileData(file->getSourceFileData()->getType(), "", system), mSourceFileData(file->getSourceFileData()) {
    mParent = nullptr;
}

SystemEnvironmentData *CollectionFileData::getSystemEnvData() const {
    return mSourceFileData->getSystemEnvData();
}

const std::string CollectionFileData::getPath() const {
    return mSourceFileData->getPath();
}

std::string CollectionFileData::getSystemName() const {
    return mSourceFileData->getSystem()->getName();
}

CollectionFileData::~CollectionFileData() {
    // need to remove collection file data at the collection object destructor
    if (mParent != nullptr) {
        mParent->removeChild(this);
    }

    mParent = nullptr;
}

std::string CollectionFileData::getKey() {
    return getFullPath();
}

FileData *CollectionFileData::getSourceFileData() {
    return mSourceFileData;
}

const std::string &CollectionFileData::getName() {
    return mSourceFileData->getName();
}
