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

#include "application.h"
#include "engine/core.h"
#include "graphics/render_engine.h"
#include "system/file.h"

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

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
void GameFrame(void)
{
    Core::Frame();

    #if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    if(GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        Application::Shutdown();
    #endif
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
    Application::Initialize();
    Application::SetFrameCallback(GameFrame);
    Application::CreateMainWindow(1280,800,0);
    window_t* mainWindow = Application::GetMainWindow();

    char fileBuffer[1024*4] = {0};
    size_t bytesRead;
    file_t file;
    File::Open(&file, "2d_pos_tex_20.psh", file_mode_e::kFileRead);
    File::Read(&file, sizeof(fileBuffer), fileBuffer, &bytesRead);
    File::Close(&file);
    fileBuffer[bytesRead] = '\0';

    Core::Initialize();
    RenderEngine::CreateDevice(mainWindow, GraphicsDeviceType::kOpenGL);
    RenderEngine::shader_id_t shaderId = RenderEngine::CreateVertexShader(fileBuffer);

    Application::StartMainLoop();

    Core::Shutdown();

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
