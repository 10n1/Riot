/*
 *  graphics.cpp
 *  engine
 *
 *  Created by Kyle C Weicht on 11/2/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "gtest/gtest.h"
#include "graphics/render_engine.h"
#include "system/memory.h"

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
TEST(RenderEngine, Initialize)
{
    EXPECT_EQ(RenderStatus::kUninitialized, RenderEngine::g_status);
    Memory::Initialize(Memory::kMegabyte*32);
    RenderEngine::Initialize();    
    EXPECT_EQ(RenderStatus::kOk, RenderEngine::g_status);

    RenderEngine::Shutdown();
    Memory::Shutdown();
}

TEST(RenderEngine, CreateGraphicsDevice)
{
    Memory::Initialize(Memory::kMegabyte*32);
    RenderEngine::Initialize();
    EXPECT_EQ(RenderStatus::kOk, RenderEngine::g_status);

    RenderEngine::CreateDevice(nullptr, GraphicsDeviceType::kNull);
    EXPECT_EQ(RenderStatus::kOk, RenderEngine::g_status);
    
    RenderEngine::Shutdown();
    Memory::Shutdown();
}

TEST(RenderEngine, Frame)
{
    Memory::Initialize(Memory::kMegabyte*32);
    RenderEngine::Initialize();
    RenderEngine::CreateDevice(nullptr, GraphicsDeviceType::kNull);
    
    RenderEngine::Frame();
    
    RenderEngine::Shutdown();
    Memory::Shutdown();
}