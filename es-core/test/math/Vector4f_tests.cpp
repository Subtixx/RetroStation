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

#include <math/Vector4f.h>

#include <gtest/gtest.h>

TEST(Vector4fTest, Vector4fCompare)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   EXPECT_EQ(vec1, vec2);
}

TEST(Vector4fTest, Vector4fCompareNotEqual)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(1.0F, 2.0F, 3.0F, 5.0F);
   EXPECT_NE(vec1, vec2);
}

TEST(Vector4fTest, Vector4fIndex)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   EXPECT_EQ(vec1[0], 1.0F);
   EXPECT_EQ(vec1[1], 2.0F);
   EXPECT_EQ(vec1[2], 3.0F);
   EXPECT_EQ(vec1[3], 4.0F);
}

TEST(Vector4fTest, Vector4fEmpty)
{
   Vector4f vec1 = Vector4f();
   EXPECT_TRUE(vec1.empty());
}

TEST(Vector4fTest, Vector4fAdd)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec3 = Vector4f(2.0F, 4.0F, 6.0F, 8.0F);
   EXPECT_EQ(vec1 + vec2, vec3);
}

TEST(Vector4fTest, Vector4fSubtract)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec3 = Vector4f(0.0F, 0.0F, 0.0F, 0.0F);
   EXPECT_EQ(vec1 - vec2, vec3);
}

TEST(Vector4fTest, Vector4fMultiply)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec3 = Vector4f(1.0F, 4.0F, 9.0F, 16.0F);
   EXPECT_EQ(vec1 * vec2, vec3);
}

TEST(Vector4fTest, Vector4fDivide)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec3 = Vector4f(1.0F, 1.0F, 1.0F, 1.0F);
   EXPECT_EQ(vec1 / vec2, vec3);
}

TEST(Vector4fTest, Vector4fAddScalar)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(2.0F, 3.0F, 4.0F, 5.0F);
   EXPECT_EQ(vec1 + 1.0F, vec2);
}

TEST(Vector4fTest, Vector4fSubtractScalar)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(0.0F, 1.0F, 2.0F, 3.0F);
   EXPECT_EQ(vec1 - 1.0F, vec2);
}

TEST(Vector4fTest, Vector4fMultiplyScalar)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(2.0F, 4.0F, 6.0F, 8.0F);
   EXPECT_EQ(vec1 * 2.0F, vec2);
}

TEST(Vector4fTest, Vector4fDivideScalar)
{
   Vector4f const vec1 = Vector4f(1.0F, 2.0F, 3.0F, 4.0F);
   Vector4f const vec2 = Vector4f(0.5F, 1.0F, 1.5F, 2.0F);
   EXPECT_EQ(vec1 / 2.0F, vec2);
}

