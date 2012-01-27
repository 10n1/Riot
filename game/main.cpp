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
#include "file.h"
#include "renderEngine/renderEngine.h"

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
float       s_explosionRadius = 0.0f;
float       s_explosionForce = 0.0f;

/*******************************************************************\
Internal functions
\*******************************************************************/
void TestScript(void)
{
    printf("...From C\n");
}

scriptingDeclareFunction(TestScript)
void Initialize(void)
{
    /* Global init */
    s_graphics = gfxCreate(sysGetWindow(s_system));
    gfxSetClearColor(s_graphics, 132/255.0f,194/255.0f,232/255.0f,255/255.0f, 1.0f);
    gfxSetDepthTest(s_graphics, 0, 0);
    gfxSetAlphaTest(s_graphics, 1);
    renderInit(s_graphics);

    scriptingInit();
    scriptingRegisterFunction(TestScript);
    scriptingDoScript(  "print(\"Hello from Lua...\")\n"
                        "TestScript()\n");

    char buffer[1024] = {0};    
    fileLoadAndRead(buffer, sizeof(buffer), "Assets/gameData.json");
    
    cJSON* s_root = cJSON_Parse(buffer);
    if(s_root == NULL)
        printf(cJSON_GetErrorPtr());
    cJSON* gameData = cJSON_GetObjectItem(s_root, "gameData");
    cJSON* explosion = cJSON_GetObjectItem(gameData, "explosion");
    s_explosionRadius = (float)cJSON_GetObjectItem(explosion, "radius")->valuedouble;
    s_explosionForce = (float)cJSON_GetObjectItem(explosion, "force")->valuedouble;
    cJSON_Delete(s_root);

    /* Game init */
    //s_world.SetGraphicsDevice(s_graphics);
    s_world.Create();
    timerInit(&s_timer);
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
            
            char buffer[1024] = {0};
            fileLoadAndRead(buffer, sizeof(buffer), "Assets/gameData.json");

            cJSON* s_root = cJSON_Parse(buffer);
            if(s_root == NULL)
                printf(cJSON_GetErrorPtr() - 10);
            cJSON* gameData = cJSON_GetObjectItem(s_root, "gameData");
            cJSON* explosion = cJSON_GetObjectItem(gameData, "explosion");
            s_explosionRadius = (float)cJSON_GetObjectItem(explosion, "radius")->valuedouble;
            s_explosionForce = (float)cJSON_GetObjectItem(explosion, "force")->valuedouble;
            cJSON_Delete(s_root);
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
            s_world.Explosion(modXPosition, modYPosition, s_explosionRadius, s_explosionForce);
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
    //gfxClear(s_graphics);
    s_world.Render();
    //gfxPresent(s_graphics);
    renderFrame();
}
void Shutdown(void)
{
    /* Game shutdown */
    s_world.Destroy();

    /* Global shutdown */
    renderShutdown();
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
