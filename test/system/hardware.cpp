/*
 *  hardware.cpp
 *  riot test
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "system/Hardware.h"
#include "gtest/gtest.h"

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
TEST(Hardware, ThreadCount)
{
    EXPECT_GT(Hardware::ThreadCount(), 0);
}
TEST(Hardware, CacheLineSize)
{
    EXPECT_GT(Hardware::CacheLineSize(), 8);
}
TEST(Hardware, PhysMemory)
{
    EXPECT_GT(Hardware::PhysMemory(), 1024);
}
TEST(Hardware, cpuid)
{
    int info[4] = {0,0,0,0};
    Hardware::cpuid(info, 1, 0);
    
    EXPECT_TRUE(info[0]);
    EXPECT_TRUE(info[1]);
    EXPECT_TRUE(info[2]);
    EXPECT_TRUE(info[3]);
}
TEST(Hardware, ISA)
{
    EXPECT_GT(Hardware::InstructionSet(), kInstructionSetSSSE3);
}
TEST(Hardware, DataWidth)
{
    EXPECT_GE(Hardware::MaxDataWidth(), 16);
}
