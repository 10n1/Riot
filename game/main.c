/*
 * main.c
 * Riot
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
#include <stdio.h>
/* External headers */
/* Internal headers */
#include "global.h"
#include "system/system.h"
#include "timer.h"

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
static system_t*    s_system = NULL;
static timer_t      s_timer;
static float        s_elapsedTime = 0.0f;

/*******************************************************************\
Internal functions
\*******************************************************************/
static void Initialize(void)
{
    /* Global init */
    timerInit(&s_timer);
}
static void Frame(void)
{
    static int frameCount = 0;
    static float frameTime = 0.0f;
    static float fps = 0.0f;
    float elapsedTime = (float)timerGetDeltaTime(&s_timer);
    frameTime += elapsedTime;
    if(frameCount == 128)
    {
        fps = frameCount/frameTime;
        frameCount = 0;
        frameTime = 0.0f;
    }

    if(sysGetKeyState(s_system, kSysKeyEscape))
        sysSetFlag(s_system, kSysRunning, 0);
    if(sysGetKeyState(s_system, kSysKeyF))
    {
        printf("FPS: %f\n", fps);
    }

    frameCount++;
}
static void Shutdown(void)
{
}
static void Resize(int width, int height)
{
    UNUSED_PARAMETER(width & height);
}

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
int main(int argc, char* argv[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_16_DF );
#endif
    /* Initialize system */
    s_system = sysCreate();
    sysSetResizeCallback(s_system, Resize);
    sysCreateWindow(s_system, 1280, 800);

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
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv[0]);
}
