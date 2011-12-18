/*
 *  core.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "engine/core.h"

#include <memory>

#include "system/memory.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
enum
{
    kTotalMemory = Memory::kMegabyte*128,
};

/*******************************************************************\
 Internal variables
\*******************************************************************/
engine_t*  s_engine = nullptr;

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace Core
{

/*******************************************************************\
 External variables
\*******************************************************************/
const engine_t*  g_engine       = nullptr;
engine_status_e  g_engineStatus = kEngineOk;
int              g_engineError  = 0;

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void)
{
    /* Initialize memory */
    Memory::Initialize(kTotalMemory);
    
    /* Create engine state */
    s_engine = (engine_t*)Memory::GlobalAllocator()->Allocate(kEngineSize);
    g_engine = s_engine;
    memset(s_engine, 0, kEngineSize);
    
    /* Initialize subsystems */

    
    /* Perform initialization */
    s_engine->initialized   = 1;
    g_engineError           = 0;
    g_engineStatus          = engine_status_e::kEngineInitialized;
    Timer::Init(&s_engine->mainTimer);
}
void Frame(void)
{
    s_engine->numFrames++;
    s_engine->deltaTime = Timer::GetDeltaTime(&s_engine->mainTimer);
}
void Shutdown(void)
{
    /* Shutdown memory last */
    Memory::Shutdown();
}

} //namespace Core
