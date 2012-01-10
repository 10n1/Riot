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
#include "system/task_manager.h"
#include "graphics/render_engine.h"

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
engine_t*   s_engine = nullptr;
void*       s_taskManager = nullptr;

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace Core
{

/*******************************************************************\
 External variables
\*******************************************************************/
const engine_t const*  g_engine       = nullptr;
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
    s_taskManager = Memory::GlobalAllocator()->Allocate(kTaskManagerSize);
    taskManagerInitialize(0, s_taskManager, kTaskManagerSize);
    RenderEngine::Initialize();
    
    /* Perform initialization */
    s_engine->initialized   = 1;
    g_engineError           = 0;
    g_engineStatus          = engine_status_e::kEngineInitialized;
    Timer::Init(&s_engine->mainTimer);
}
void Frame(void)
{
    RenderEngine::Frame();
    s_engine->numFrames++;
    s_engine->deltaTime = (float)Timer::GetDeltaTime(&s_engine->mainTimer);
}
void Shutdown(void)
{
    RenderEngine::Shutdown();
    taskManagerShutdown();
    /* Shutdown memory last */
    Memory::Shutdown();
}

} //namespace Core
