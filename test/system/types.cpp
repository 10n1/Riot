/*
 *  timer.cpp
 *  Riot test
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "gtest/gtest.h"
#include "types.h"

TEST(Types, Size)
{
    EXPECT_EQ(1, sizeof(byte));
    EXPECT_EQ(1, sizeof(uint8));
    EXPECT_EQ(1, sizeof(sint8));
    EXPECT_EQ(1, sizeof(int8));
    EXPECT_EQ(2, sizeof(uint16));
    EXPECT_EQ(2, sizeof(sint16));
    EXPECT_EQ(2, sizeof(int16));
    EXPECT_EQ(4, sizeof(bool32));
    EXPECT_EQ(4, sizeof(uint32));
    EXPECT_EQ(4, sizeof(sint32));
    EXPECT_EQ(4, sizeof(int32));
    EXPECT_EQ(8, sizeof(uint64));
    EXPECT_EQ(8, sizeof(sint64));
    EXPECT_EQ(8, sizeof(int64));
    
    EXPECT_EQ(4, sizeof(uint));
    EXPECT_EQ(4, sizeof(sint));

    EXPECT_EQ(4, sizeof(float32));
    EXPECT_EQ(8, sizeof(float64));
    
    EXPECT_FALSE(NULL);
}
