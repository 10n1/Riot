/*
 *  main.cpp
 *  Riot Game
 *
 *  Created by Kyle Weicht on 12/16/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
//#include "source_header.h"

//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

#include "assert.h"
#include "build.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctime>


#include "application.h"
#include "engine/core.h"
#include "graphics/render_engine.h"
#include "system/file.h"

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#include "system/system.h"
#include "render_engine/renderEngine.h"
#include "world.h"
#include "system/timer.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
int THIS_SHOULD_BECOME_1_AT_SHUTDOWN = 0;

int     s_windowWidth;
int     s_windowHeight;

World   s_world;

timer_t s_timer;

/*******************************************************************\
 Internal functions
\*******************************************************************/
void Frame(void)
{
    static int frame = 0;
    int x, y;
    System::GetMousePosition(&x, &y);
    System::mouse_state_t state = System::GetMouseState();
    /*printf("%d, %d, %d, %d, %d\n", 
            x, 
            y, 
            state & System::kMouseButtonLeft, 
            state & System::kMouseButtonMiddle, 
            state & System::kMouseButtonRight );*/

    char keys[System::kMAX_KEYS];
    System::GetKeyboardState(keys);
    if(keys[System::kKeyQ])
        System::Shutdown();
    if(System::GetKeyState(System::kKeyEscape))
        System::Shutdown();

    //
    // Perform actual update stuff
    //
    float elapsedTime = Timer::GetDeltaTime(&s_timer);
    s_world.Update(elapsedTime);
    s_world.Render();
    Render::Frame();
}
void Shutdown(void)
{
    //
    // Shutdown Subsystems
    //
    Render::Shutdown();

    printf("Shutdown\n");
    THIS_SHOULD_BECOME_1_AT_SHUTDOWN = 1;
}
void Resize(int width, int height)
{
    printf("%d, %d\n", width, height);
    s_windowWidth = width;
    s_windowHeight = height;
}
void MouseClick(System::mouse_button_e button, int x, int y)
{
    printf("Button: %d. Pos: %d, %d\n", button, x, y);
    // Flip the Y so its down in the lower left hand corner
    //y = s_windowHeight - y;

    if(button == System::kMouseButtonLeft)
        s_world.MouseClick(x,y);
}

} // namespace

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
int main(int argc, char* argv[])
{
    /*
     * Test system
     */
#if 0
    char exeDirectory[256] = {0};
    System::GetExecutableDirectory(sizeof(exeDirectory), exeDirectory);
    printf("%s\n", exeDirectory);

    System::Initialize();
    System::Shutdown();
    
    System::Initialize();
    System::Shutdown();
    
    System::Initialize();
    System::Shutdown();
    System::message_box_e result = System::MessageBox("Message Box!", "Hit OK!", System::kOkCancel);
    result = System::MessageBox("Message Box!", "Hit cancel!", System::kOkCancel);
    result = System::MessageBox("Message Box!", "Hit Retry!", System::kRetryCancel);
    result = System::MessageBox("Message Box!", "Hit Cancel!", System::kRetryCancel);
#endif
    srand(time(NULL));
    //
    // System initialization
    //
    System::Initialize();
    System::SetFrameCallback(&Frame);
    System::SetShutdownCallback(&Shutdown);
    System::SetResizeCallback(&Resize);
    System::SetMouseClickCallback(&MouseClick);
    System::SpawnWindow(1024,1024,0);
    System::SetWindowTitle("My Window!");

    //
    // Render Engine
    //
    void* renderEngineMemory = (void*)new char[Render::kRenderEngineSize];
    void* systemWindow = System::GetMainWindow();
    assert(systemWindow);
    Render::Initialize(systemWindow, renderEngineMemory, Render::kRenderEngineSize);

    // 
    // Game initialization
    //
    s_world.Create();
    Timer::Init(&s_timer);

    //
    // Run main loop
    //
    System::RunMainLoop();

    //
    // Post-shutdown
    //
    s_world.Destroy();
    delete [] renderEngineMemory;

    assert(THIS_SHOULD_BECOME_1_AT_SHUTDOWN);
    return 0;
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv[0]);
}



/* Header */

/*
 *  .
 *  riot
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef riot_XXXX_h_
#define riot_XXXX_h_
//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

/*******************************************************************\
 External constants and types
\*******************************************************************/

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/

#endif /* include guard */

/* Source */
/*
 *  .
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
//#include "source_header.h"

//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
