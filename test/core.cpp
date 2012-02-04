/*
 * core.cpp
 * RiotTests
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "core.h"

/* C headers */
/* C++ headers */
/* External headers */
#include "UnitTest++.h"
/* Internal headers */
#include "core.h"

TEST(Core)
{
    Core core;
    core.Init(engine_params_t());
    
    for(int ii=0; ii<5; ++ii)
        core.Frame();

    CHECK_EQUAL(5, core.frameNumber());
    core.Shutdown();
}
