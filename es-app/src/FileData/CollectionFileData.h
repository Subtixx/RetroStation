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

class CollectionFileData : public FileData {
  public:
    CollectionFileData(FileData *file, SystemData *system);
    ~CollectionFileData();
    const std::string &getName();
    FileData *getSourceFileData();
    std::string getKey();
    virtual const std::string getPath() const;

    virtual std::string getSystemName() const;
    virtual SystemEnvironmentData *getSystemEnvData() const;

    virtual const MetaDataList &getMetadata() const {
        return mSourceFileData->getMetadata();
    }
    virtual MetaDataList &getMetadata() {
        return mSourceFileData->getMetadata();
    }
    virtual std::string &getDisplayName() {
        return mSourceFileData->getDisplayName();
    }

  private:
    // needs to be updated when metadata changes
    FileData *mSourceFileData;
};
