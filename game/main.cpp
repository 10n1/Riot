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

#include "application.h"
#include "engine/core.h"
#include "graphics/render_engine.h"
#include "system/file.h"

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#include "system/system.h"
#include "render_engine/renderEngine.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
int THIS_SHOULD_BECOME_1_AT_SHUTDOWN = 0;

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
    
    //
    // System initialization
    //
    System::Initialize();
    System::SetFrameCallback(&Frame);
    System::SetShutdownCallback(&Shutdown);
    System::SetResizeCallback(&Resize);
    System::SpawnWindow(1280,800,0);
    System::SetWindowTitle("My Window!");

    //
    // Render Engine
    //
    void* renderEngineMemory = malloc(Render::kRenderEngineSize);
    void* systemWindow = System::GetMainWindow();
    assert(systemWindow);
    Render::Initialize(systemWindow, renderEngineMemory, Render::kRenderEngineSize);

    //
    // Asset loading
    //
    char vertexShaderSource[1024] = {0};
    char pixelShaderSource[1024] = {0};
    size_t bytesRead;
    file_t file;
    File::Open(&file, "assets/pos.vsh", file_mode_e::kFileRead);
    File::Read(&file, sizeof(vertexShaderSource), vertexShaderSource, &bytesRead);
    File::Close(&file);
    File::Open(&file, "assets/color.psh", file_mode_e::kFileRead);
    File::Read(&file, sizeof(pixelShaderSource), pixelShaderSource, &bytesRead);
    File::Close(&file);

    const Render::shader_t vertexShader = Render::CreateShader(vertexShaderSource, Render::kVertexShader);
    const Render::shader_t pixelShader = Render::CreateShader(pixelShaderSource, Render::kPixelShader);

    System::RunMainLoop();

    //
    // Post-shutdown
    //
    free(renderEngineMemory);

    //Application::Initialize();
    //Application::SetFrameCallback(GameFrame);
    //Application::CreateMainWindow(1280,800,0);
    //window_t* mainWindow = Application::GetMainWindow();

    //char fileBuffer[1024*4] = {0};
    //size_t bytesRead;
    //file_t file;
    //File::Open(&file, "2d_pos_tex_20.psh", file_mode_e::kFileRead);
    //File::Read(&file, sizeof(fileBuffer), fileBuffer, &bytesRead);
    //File::Close(&file);
    //fileBuffer[bytesRead] = '\0';

    //Core::Initialize();
    //RenderEngine::CreateDevice(mainWindow, GraphicsDeviceType::kOpenGL);
    //RenderEngine::shader_id_t shaderId = RenderEngine::CreateVertexShader(fileBuffer);

    //Application::StartMainLoop();

    //Core::Shutdown();

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
