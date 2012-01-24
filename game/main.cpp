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
#include "scripting.h"
#include "cJSON.h"

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
int         s_windowWidth = 0;
int         s_windowHeight = 0;
cJSON*      s_root = NULL;

/*******************************************************************\
Internal functions
\*******************************************************************/
void TestScript(void)
{
    printf("From C\n");
}

scriptingDeclareFunction(TestScript);
void Initialize(void)
{
    /* Global init */
    timerInit(&s_timer);
    s_graphics = gfxCreate(sysGetWindow(s_system));
    gfxSetClearColor(s_graphics, 132/255.0f,194/255.0f,232/255.0f,255/255.0f, 1.0f);
    gfxSetDepthTest(s_graphics, 0, 0);
    gfxSetAlphaTest(s_graphics, 1);

    scriptingInit();
    scriptingRegisterFunction(TestScript);
    scriptingDoScript(  "print(\"Hello from Lua...\")\n"
                        "TestScript()\n");

    char buffer[1024];
    FILE* file = fopen("Assets/testJson.json", "r");
    fread(buffer,sizeof(buffer),1, file);
    fclose(file);

    //s_root = cJSON_Parse(buffer);
    //printf(cJSON_GetErrorPtr());
    //cJSON* format = cJSON_GetObjectItem(s_root, "format");
    //int framerate = cJSON_GetObjectItem(format, "frame rate")->valueint;

    /* Game init */
    s_world.SetGraphicsDevice(s_graphics);
    s_world.Create();
}
void Frame(void)
{
    static int leftMouseDown = 0;
    static int reset = 0;
    /*
     * Update
     */
    s_elapsedTime = (float)timerGetDeltaTime(&s_timer);

    if(sysGetKeyState(s_system, kSysKeyEscape))
        sysStop(s_system);
    if(sysGetKeyState(s_system, kSysKeyR))
    {
        if(reset == 0)
        {
            s_world.Reset();
        }
        reset = 1;
    }
    else
    {
        reset = 0;
    }

    s_world.Update(s_elapsedTime);

    if(sysGetMouseState(s_system) & kSysMouseLeft)
    {
        if(leftMouseDown == 0)
        {
            int xPosition;
            int yPosition;
            sysGetMousePosition(s_system, &xPosition, &yPosition);
            float modXPosition = (xPosition/(float)s_windowWidth*2) - 1.0f;
            float modYPosition = -1.0f * ((yPosition/(float)s_windowHeight*2) - 1.0f);
            s_world.ConvertToWorldPos(&modXPosition, &modYPosition);
            s_world.Explosion(modXPosition, modYPosition, 20.0f, 20000000.0f);
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
    scriptingShutdown();
}
void Resize(int width, int height)
{
    if(s_graphics)
    {
        gfxResize(s_graphics, width, height);
    }
    s_windowWidth = width;
    s_windowHeight = height;
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
