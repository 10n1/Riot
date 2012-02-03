/*
 * main.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
/* C++ headers */
/* External headers */
#include "UnitTest++.h"
/* Internal headers */
#include "system.h"

TEST(SystemCreateShutdown)
{
    system_t* system = NULL;

    system = sysCreate();
    CHECK(system);

    sysShutdown(system);
}

TEST(SystemCreateWindow)
{
    system_t* system = sysCreate();
    sysCreateWindow(system, 256, 256);
    sysShutdown(system);
}

TEST(SystemPollEvents)
{
    system_t* system = sysCreate();
    sysPollSystemEvents(system);
    sysShutdown(system);
}
