/*
 *  timer.cpp
 *  Riot test
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

#include "system/timer.h"
#include "gtest/gtest.h"

TEST(Timer, Timer)
{
    timer_t timer;
    Timer::Init(&timer);
    
    EXPECT_NE(0.0f, timer.frequency);
    EXPECT_NE(0, timer.startTime);
    EXPECT_NE(0, timer.prevTime);
    EXPECT_EQ(timer.startTime, timer.prevTime);

    double delta = Timer::GetDeltaTime(&timer);
    EXPECT_NE(0.0, delta);
    
    EXPECT_NE(timer.startTime, timer.prevTime);

    Timer::Reset(&timer);
    EXPECT_EQ(timer.startTime, timer.prevTime);
}
