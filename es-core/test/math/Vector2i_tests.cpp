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

#include <math/Vector2i.h>

#include <gtest/gtest.h>

TEST(Vector2iTest, Vector2iCompare)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    EXPECT_EQ(vec1, vec2);
}

TEST(Vector2iTest, Vector2iAdd)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(2, 4);
    EXPECT_EQ(vec1 + vec2, vec3);
}

TEST(Vector2iTest, Vector2iSubtract)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(0, 0);
    EXPECT_EQ(vec1 - vec2, vec3);
}

TEST(Vector2iTest, Vector2iMultiply)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(1, 4);
    EXPECT_EQ(vec1 * vec2, vec3);
}

TEST(Vector2iTest, Vector2iDivide)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(1, 1);
    EXPECT_EQ(vec1 / vec2, vec3);
}

TEST(Vector2iTest, Vector2iAddScalar)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(2, 3);
    EXPECT_EQ(vec1 + 1, vec2);
}

TEST(Vector2iTest, Vector2iSubtractScalar)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(0, 1);
    EXPECT_EQ(vec1 - 1, vec2);
}

TEST(Vector2iTest, Vector2iMultiplyScalar)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(2, 4);
    EXPECT_EQ(vec1 * 2, vec2);
}

TEST(Vector2iTest, Vector2iDivideScalar)
{
    Vector2i const vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(0, 1);
    EXPECT_EQ(vec1 / 2, vec2);
}

TEST(Vector2iTest, Vector2iAddAssign)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(2, 4);
    vec1 += vec2;
    EXPECT_EQ(vec1, vec3);
}

TEST(Vector2iTest, Vector2iSubtractAssign)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(0, 0);
    vec1 -= vec2;
    EXPECT_EQ(vec1, vec3);
}

TEST(Vector2iTest, Vector2iMultiplyAssign)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(1, 4);
    vec1 *= vec2;
    EXPECT_EQ(vec1, vec3);
}

TEST(Vector2iTest, Vector2iDivideAssign)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(1, 2);
    Vector2i const vec3 = Vector2i(1, 1);
    vec1 /= vec2;
    EXPECT_EQ(vec1, vec3);
}

TEST(Vector2iTest, Vector2iAddAssignScalar)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(2, 3);
    vec1 += 1;
    EXPECT_EQ(vec1, vec2);
}

TEST(Vector2iTest, Vector2iSubtractAssignScalar)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(0, 1);
    vec1 -= 1;
    EXPECT_EQ(vec1, vec2);
}

TEST(Vector2iTest, Vector2iMultiplyAssignScalar)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(2, 4);
    vec1 *= 2;
    EXPECT_EQ(vec1, vec2);
}

TEST(Vector2iTest, Vector2iDivideAssignScalar)
{
    Vector2i vec1 = Vector2i(1, 2);
    Vector2i const vec2 = Vector2i(0, 1);
    vec1 /= 2;
    EXPECT_EQ(vec1, vec2);
}