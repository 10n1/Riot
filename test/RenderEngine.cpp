/*
 * RenderEngine.cpp
 * RiotTest
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
/* C++ headers */
/* External headers */
#include "UnitTest++.h"
/* Internal headers */
#include "renderEngine.h"

TEST(RenderEngineInitShutdown)
{
    RenderEngine::Init(render_engine_params_t());
    RenderEngine::Shutdown();
}