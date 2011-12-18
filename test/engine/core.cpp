/*
 *  core.cpp
 *  Riot test
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

#include "system/thread.h"
#include "gtest/gtest.h"

#include "engine/core.h"

TEST(EngineCore, Init)
{
    EXPECT_FALSE(Core::g_engine);
    Core::Initialize();
    
    ASSERT_TRUE(Core::g_engine);
    EXPECT_TRUE(Core::g_engine->initialized);
    EXPECT_NE(engine_status_e::kEngineError, Core::g_engineStatus);
    
    Core::Shutdown();
}

TEST(EngineCore, Frame)
{
    Core::Initialize();
    
    for(int ii=0; ii<5; ++ii)
    {
        Core::Frame();
    }
    EXPECT_EQ(5, Core::g_engine->numFrames);
    
    Core::Shutdown();
}
