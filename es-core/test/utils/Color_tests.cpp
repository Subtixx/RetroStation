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

#include <utils/HtmlColor.h>

#include <gtest/gtest.h>

TEST(ColorTest, ColorHexString)
{
    Utils::Color const color = Utils::Color("#FF00FF");
    EXPECT_EQ(color, "#FF00FF");
}

TEST(ColorTest, ColorHexInt)
{
    Utils::Color const color = Utils::Color(0xFF00FF);
    EXPECT_EQ(color, "#FF00FF");
}

TEST(ColorTest, ColorHexIntEquals)
{
    Utils::Color const color = Utils::Color("#FF00FF");
    EXPECT_EQ(color, 0xFF00FF);
}

TEST(ColorTest, HtmlColorToColor)
{
    Utils::Color const color = Utils::Color::getColor("red");
    EXPECT_EQ(color, Utils::Red);
}