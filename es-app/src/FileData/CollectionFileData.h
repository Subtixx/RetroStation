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
    ~CollectionFileData() override;

    CollectionFileData(const CollectionFileData &) = delete;
    CollectionFileData &operator=(const CollectionFileData &) = delete;
    CollectionFileData(CollectionFileData &&) = delete;
    CollectionFileData &operator=(CollectionFileData &&) = delete;

    const std::string &getName() override;
    FileData *getSourceFileData() override;
    std::string getKey() override;
    [[nodiscard]] const std::string getPath() const override;

    [[nodiscard]] std::string getSystemName() const override;
    [[nodiscard]] SystemEnvironmentData *getSystemEnvData() const override;

    [[nodiscard]] const MetaDataList &getMetadata() const override {
        return mSourceFileData->getMetadata();
    }
    MetaDataList &getMetadata() override {
        return mSourceFileData->getMetadata();
    }
    std::string &getDisplayName() override {
        return mSourceFileData->getDisplayName();
    }

  private:
    // needs to be updated when metadata changes
    FileData *mSourceFileData;
};
