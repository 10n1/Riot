/*
 *  thread.cpp
 *  Riot test
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

#include "system/thread.h"
#include "gtest/gtest.h"

static void testFunc(void* param)
{
    uint64_t* i = (uint64_t*)param;
    
    *i = (uint64_t)0xDEADBEEF01234567;
}

TEST(Thread, Create)
{
    uint64_t testVal = 0x0;
    int result = Thread::Spawn(&testFunc, &testVal);
    EXPECT_FALSE(result);
    
    while(testVal == 0x0)
        ;
    
    EXPECT_EQ(0xDEADBEEF01234567, testVal);
}
