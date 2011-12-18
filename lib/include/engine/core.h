/*
 *  core.h
 *  riot
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef riot_core_h_
#define riot_core_h_

#include "system/timer.h"
#include "types.h"

/*******************************************************************\
 External constants and types
\*******************************************************************/
enum engine_status_e
{    
    kEngineError = -1,
    kEngineOk = 0,
    kEngineInitialized,
    kEngineRunning,
    kEngineShutdown
};

struct engine_t
{
    timer_t mainTimer;
    float   deltaTime;
    uint    initialized;
    uint    numFrames;
};


namespace Core
{

/*******************************************************************\
 Variables
\*******************************************************************/
static const size_t     kEngineSize = sizeof(engine_t);
extern const engine_t*  g_engine;
extern engine_status_e  g_engineStatus;
extern int              g_engineError;

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void);
void Frame(void);
void Shutdown(void);

} // namespace Engine

#endif /* include guard */
