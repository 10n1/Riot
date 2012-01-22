/*
 * main.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
#include <stdio.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"
#include "system/system.h"
#include "graphicsDevice/graphicsDevice.h"
#include "timer.h"
#include "world.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
system_t*   s_system = NULL;
timer_t     s_timer;
float       s_elapsedTime = 0.0f;
graphics_t* s_graphics = NULL;
World       s_world;

/*******************************************************************\
Internal functions
\*******************************************************************/
void Initialize(void)
{
    /* Global init */
    timerInit(&s_timer);
    s_graphics = gfxCreate(sysGetWindow(s_system));
    gfxSetClearColor(s_graphics, 132/255.0f,194/255.0f,232/255.0f,255/255.0f, 1.0f);
    gfxSetDepthTest(s_graphics, 0, 0);
    gfxSetAlphaTest(s_graphics, 1);

    /* Game init */
    s_world.SetGraphicsDevice(s_graphics);
    s_world.Create();
}
void Frame(void)
{
    static int leftMouseDown = 0;
    /*
     * Update
     */
    s_elapsedTime = (float)timerGetDeltaTime(&s_timer);

    if(sysGetKeyState(s_system, kSysKeyEscape))
        sysStop(s_system);

    s_world.Update(s_elapsedTime);

    if(sysGetMouseState(s_system) & kSysMouseLeft)
    {
        if(leftMouseDown == 0)
        {
            //s_world.BuildBuilding();
            s_world.Explosion(0.0f, 10.0f, 15.0f);
        }
        leftMouseDown = 1;
    }
    else
    {
        leftMouseDown = 0;
    }
    
    /*
     * Render
     */
    gfxClear(s_graphics);
    s_world.Render();
    gfxPresent(s_graphics);
}
void Shutdown(void)
{
    /* Game shutdown */
    s_world.Destroy();

    /* Global shutdown */
    gfxDestroy(s_graphics);
}
void Resize(int width, int height)
{
    if(s_graphics)
    {
        gfxResize(s_graphics, width, height);
    }
}

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
int main(int, char*[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_16_DF );
#endif
    /* Initialize system */
    s_system = sysCreate();
    sysSetResizeCallback(s_system, Resize);
    sysCreateWindow(s_system, 1024, 1024);

    /* Start main loop */
    Initialize();
    for(;;)
    {
        sysPollSystemEvents(s_system);
        Frame();

        if(!sysGetFlag(s_system, kSysRunning))
            break;
    }
    Shutdown();
    sysShutdown(s_system);

    return 0;
}
