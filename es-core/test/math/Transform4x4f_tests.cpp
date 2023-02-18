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

#include <math/Transform4x4f.h>

#include <gtest/gtest.h>

TEST(Transform4x4fTest, Transform4x4fCompare)
{
    Transform4x4f const vec1 = Transform4x4f(Vector4f(1, 2, 3, 4), Vector4f(5, 6, 7, 8), Vector4f(9, 10, 11, 12), Vector4f(13, 14, 15, 16));
    Transform4x4f const vec2 = Transform4x4f(Vector4f(1, 2, 3, 4), Vector4f(5, 6, 7, 8), Vector4f(9, 10, 11, 12), Vector4f(13, 14, 15, 16));
    EXPECT_EQ(vec1, vec2);
}
