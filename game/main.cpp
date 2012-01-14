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

struct pos_vertex_t
{
    float f[3];
};
struct pos_tex_vertex_t
{
    float pos[3];
    float tex[2];
};

const pos_tex_vertex_t kQuadVerts[] = 
{
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f } },
    { { -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f } },
    { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f } },
};
const int kQuadIndices[] =
{
    0,1,2,
    2,1,3,
};

Render::shader_t vertexShader   = -1;
Render::shader_t pixelShader    = -1;
Render::material_t material     = -1;
Render::mesh_t quadMesh         = -1;

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
    Render::SubmitCommand(material, quadMesh);
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
    void* renderEngineMemory = (void*)new char[Render::kRenderEngineSize];
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
    File::Open(&file, "assets/pos_tex.vsh", file_mode_e::kFileRead);
    File::Read(&file, sizeof(vertexShaderSource), vertexShaderSource, &bytesRead);
    File::Close(&file);
    File::Open(&file, "assets/tex.psh", file_mode_e::kFileRead);
    File::Read(&file, sizeof(pixelShaderSource), pixelShaderSource, &bytesRead);
    File::Close(&file);

    vertexShader = Render::CreateShader(vertexShaderSource, Render::kVertexShader);
    pixelShader = Render::CreateShader(pixelShaderSource, Render::kPixelShader);
    material = Render::CreateMaterial(vertexShader, pixelShader);
    quadMesh = Render::CreateMesh(  Render::kPosTex, 
                                    sizeof(kQuadIndices)/sizeof(kQuadIndices[0]), 
                                    sizeof(kQuadVerts)/sizeof(kQuadVerts[0]), 
                                    sizeof(kQuadVerts[0]), 
                                    sizeof(kQuadIndices[0]), 
                                    kQuadVerts, 
                                    kQuadIndices);

    //
    // Create texture
    //
    const int textureWidth = 512;
    const int textureHeight = 512;
    const int pixelSize = 3;
    const int textureSize = textureWidth*textureHeight*pixelSize;

    uint8_t* textureData = new uint8_t[textureWidth*textureHeight*pixelSize];
    for(int ii=0; ii<textureSize; ++ii)
    {
        textureData[ii] = (char)rand();
    }
    
    Render::texture_t texture = Render::CreateTexture(textureWidth, textureHeight, pixelSize*8, textureData);

    System::RunMainLoop();

    //
    // Post-shutdown
    //
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
