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

#include <gtest/gtest.h>

#include <utils/DateTime.h>

using namespace Utils;

TEST(DateTimeTest, DateTimeNowTest)
{
    DateTime const now = DateTime::now();
    EXPECT_TRUE(now.isValid());
}

TEST(DateTimeTest, DateTimeFromStringTest)
{
    DateTime const now = DateTime::now();
    DateTime const fromString = DateTime(now.getIsoString());
    EXPECT_TRUE(fromString.isValid());
    EXPECT_EQ(now, fromString);
}

TEST(DateTimeTest, DateTimeTimeTTest)
{
    DateTime const now = DateTime::now();
    DateTime const fromTimeT = DateTime(time(nullptr));
    EXPECT_TRUE(fromTimeT.isValid());
    EXPECT_EQ(now, fromTimeT);
}

TEST(DateTimeTest, DateTimeTimeTMTest)
{
    DateTime const now = DateTime::now();
    // Create datetime from tm struct
    struct tm const timeStruct = now.getTimeStruct();
    DateTime const fromTimeTM = DateTime(timeStruct);
    EXPECT_TRUE(fromTimeTM.isValid());
    EXPECT_EQ(now, fromTimeTM);
}

// elapsedSecondsSince
TEST(DateTimeTest, DateTimeElapsedSecondsSinceTest)
{
    DateTime const now = DateTime::now();
    DateTime const fromTimeT = DateTime(time(nullptr));
    EXPECT_TRUE(fromTimeT.isValid());
    EXPECT_EQ(now, fromTimeT);
    EXPECT_EQ(now.elapsedSecondsSince(fromTimeT), 0);
}

TEST(DateTimeTest, DateTimeElapsedMinutesSinceTest)
{
    DateTime const now = DateTime::now();
    DateTime const fromTimeT = DateTime(time(nullptr));
    EXPECT_TRUE(fromTimeT.isValid());
    EXPECT_EQ(now, fromTimeT);
    EXPECT_EQ(now.elapsedMinutesSince(fromTimeT), 0);
}

TEST(DateTimeTest, DateTimeElapsedHoursSinceTest)
{
    DateTime const now = DateTime::now();
    DateTime const fromTimeT = DateTime(time(nullptr));
    EXPECT_TRUE(fromTimeT.isValid());
    EXPECT_EQ(now, fromTimeT);
    EXPECT_EQ(now.elapsedHoursSince(fromTimeT), 0);
}

TEST(DateTimeTest, DateTimeToStringTest)
{
    DateTime const now = DateTime::now();
    EXPECT_TRUE(now.isValid());
    EXPECT_FALSE(now.toString().empty());
}