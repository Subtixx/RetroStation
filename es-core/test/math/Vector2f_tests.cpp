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

#include <math/Vector2f.h>

TEST(Vector2fTest, Vector2fCompare)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(1.0F, 2.0F);
    EXPECT_EQ(vec1, vec2);
}

TEST(Vector2fTest, Vector2fAdd)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(3.0F, 4.0F);
    Vector2f const vec3 = Vector2f(4.0F, 6.0F);
    EXPECT_EQ(vec1 + vec2, vec3);
}

TEST(Vector2fTest, Vector2fSubtract)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(3.0F, 4.0F);
    Vector2f const vec3 = Vector2f(-2.0F, -2.0F);
    EXPECT_EQ(vec1 - vec2, vec3);
}

TEST(Vector2fTest, Vector2fMultiply)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(3.0F, 4.0F);
    Vector2f const vec3 = Vector2f(3.0F, 8.0F);
    EXPECT_EQ(vec1 * vec2, vec3);
}

TEST(Vector2fTest, Vector2fDivide)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(3.0F, 4.0F);
    Vector2f const vec3 = Vector2f(1.0F / 3.0F, 2.0F / 4.0F);
    EXPECT_EQ(vec1 / vec2, vec3);
}

TEST(Vector2fTest, Vector2fAddScalar)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(4.0F, 5.0F);
    EXPECT_EQ(vec1 + 3.0F, vec2);
}

TEST(Vector2fTest, Vector2fSubtractScalar)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(-2.0F, -1.0F);
    EXPECT_EQ(vec1 - 3.0F, vec2);
}

TEST(Vector2fTest, Vector2fMultiplyScalar)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(3.0F, 6.0F);
    EXPECT_EQ(vec1 * 3.0F, vec2);
}

TEST(Vector2fTest, Vector2fDivideScalar)
{
    Vector2f const vec1 = Vector2f(1.0F, 2.0F);
    Vector2f const vec2 = Vector2f(1.0F / 3.0F, 2.0F / 3.0F);
    EXPECT_EQ(vec1 / 3.0F, vec2);
}