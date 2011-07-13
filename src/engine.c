//
//  engine.cpp
//  Riot
//
//  Created by Kyle Weicht on 7/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <immintrin.h>
#include <fstream.h>

#include "engine.h"
#include "error.h"
#include "utility.h"

typedef struct Engine
{
    FILE*   _log_file;
    int     _initialized;
} Engine;

static Engine* g_engine;

void EngineInit(void)
{
    g_engine = (Engine*)malloc(sizeof(Engine));
    
    g_engine->_initialized = 1;
    fopen("log.txt", "a");
    
    PRINT_ERROR("Engine Initialized");
}

void EngineShutdown(void)
{
    PRINT_ERROR("Engine Shutdown!");
    free(g_engine);
}

void EngineFrame(void)
{   
    ASSERT( g_engine->_initialized == 1 );
}
