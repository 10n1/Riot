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
    { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } },
    { {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
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
Render::texture_t texture       = -1;

const int textureWidth = 1024;
const int textureHeight = 1024;
const int pixelSize = 3;
const int textureSize = textureWidth*textureHeight*pixelSize;

uint8_t* textureData = nullptr;

World   s_world;

int terrainHeights[textureWidth];

void SetColor(char r, char g, char b, int x, int y, void* tex)
{
    char* pixel = &((char*)tex)[(x*3) + (y*3*textureHeight)];
    pixel[0] = b;
    pixel[1] = g;
    pixel[2] = r;
}
void UpdateTerrain(void* textureData)
{
    for(int ii=0; ii<textureWidth; ++ii)
    {
        int jj;
        for(jj=0; jj<terrainHeights[ii]; ++jj)
        {
            SetColor(148,100,12,ii,jj,textureData);
        }
        for(int kk=0; kk<10; ++kk, ++jj)
        {   
            if(ii > textureWidth || jj > textureHeight)
                continue;
            SetColor(25,200,50,ii,jj,textureData);
        }
        for(; jj<textureHeight; ++jj)
        {
            if(ii > textureWidth || jj > textureHeight)
                continue;
            SetColor(132,194,232,ii,jj,textureData);
        }
    }
}
int RandomMidpoint(int low, int high)
{
    int difference = high-low;
    if(difference == 0)
        return high;

    int number = rand()%difference;
    return number+low;
}
void Midpoint(int low, int high)
{
    if(abs(high-low) == 1)
        return;

    int lowHeight = terrainHeights[low];
    int highHeight = terrainHeights[high];
    int newHeight;
    if(lowHeight > highHeight)
    {
        newHeight = RandomMidpoint(highHeight, lowHeight);
    }
    else
    {
        newHeight = RandomMidpoint(lowHeight, highHeight);
    }

    int midpoint = (high+low)/2;
    terrainHeights[midpoint] = newHeight;

    // Left side
    Midpoint(low, midpoint);
    Midpoint(midpoint, high);
}

void CalculateTerrainHeights(void)
{
    //
    // Completely random
    //
    for(int ii=0; ii<textureWidth; ++ii)
    {
        int height = rand()%textureHeight;
        for(int jj=0; jj<5; ++jj,++ii)
        {
            terrainHeights[ii] = height;
        }
        terrainHeights[ii] = height;
    }

    //
    // Half thing
    //
    const int smoothDistance = 20;
    const int half = smoothDistance/2;
    for(int kk=0; kk<6; ++kk)
    {
        for(int ii=0; ii<textureWidth; ++ii)
        {
            int count = 0;
            int value = 0;
            for(int jj=-half; jj<half+1; ++jj)
            {
                int index = ii+jj;
                if(index < 0 || index > textureWidth)
                    continue;
                value += terrainHeights[index];
                count++;
            }

            terrainHeights[ii] = value/count;
        }
    }
}

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
    if(System::GetKeyState(System::kKeyT))
        CalculateTerrainHeights();

    //
    // Perform actual update stuff
    //
    s_world.Update(0.0f);
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
    textureData = new uint8_t[textureWidth*textureHeight*pixelSize];
    for(int ii=0; ii<textureSize; ++ii)
    {
        textureData[ii] = 0;
    }
    texture = Render::CreateTexture(textureWidth, textureHeight, pixelSize*8, textureData);


    CalculateTerrainHeights();
    UpdateTerrain(textureData);

    s_world.Create();
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
