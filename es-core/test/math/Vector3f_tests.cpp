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

#include <math/Vector3f.h>

TEST(Vector3fTest, Vector3fCompare)
{
    Vector3f const vec1 = Vector3f(1.0F, 2.0F, 3.0F);
    Vector3f const vec2 = Vector3f(1.0F, 2.0F, 3.0F);
    EXPECT_EQ(vec1, vec2);
}

TEST(Vector3fTest, Vector3fNotEqual)
{
    Vector3f const vec1 = Vector3f(1.0F, 2.0F, 3.0F);
    Vector3f const vec2 = Vector3f(2.0F, 2.0F, 3.0F);
    EXPECT_FALSE(vec1 == vec2);
}

TEST(Vector3fTest, Vector3fAddition)
{
    Vector3f const vec1 = Vector3f(1.0F, 2.0F, 3.0F);
    Vector3f const vec2 = Vector3f(2.0F, 2.0F, 3.0F);
    Vector3f const vec3 = Vector3f(3.0F, 4.0F, 6.0F);
    EXPECT_EQ(vec3, vec1 + vec2);
}

TEST(Vector3fTest, Vector3fSubtraction)
{
    Vector3f const vec1 = Vector3f(1.0F, 2.0F, 3.0F);
    Vector3f const vec2 = Vector3f(2.0F, 2.0F, 3.0F);
    Vector3f const vec3 = Vector3f(-1.0F, 0.0F, 0.0F);
    EXPECT_EQ(vec3, vec1 - vec2);
}

TEST(Vector3fTest, Vector3fScalarMultiplication)
{
    Vector3f const vec1 = Vector3f(1.0F, 2.0F, 3.0F);
    Vector3f const vec2 = Vector3f(2.0F, 4.0F, 6.0F);
    EXPECT_EQ(vec2, vec1 * 2.0F);
}

TEST(Vector3fTest, Vector3fScalarDivision)
{
    Vector3f const vec1 = Vector3f(1.0F, 2.0F, 3.0F);
    Vector3f const vec2 = Vector3f(0.5F, 1.0F, 1.5F);
    EXPECT_EQ(vec2, vec1 / 2.0F);
}