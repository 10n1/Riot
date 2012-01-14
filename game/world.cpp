/*
 * world.cpp
 * game
 *
 * Created by Kyle Weicht on 1/14/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "world.h"

/* C headers */
#include <memory.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "system/file.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
struct pos_tex_vertex_t
{
    float pos[3];
    float tex[2];
};

/*******************************************************************\
Internal variables
\*******************************************************************/
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

/*******************************************************************\
Internal functions
\*******************************************************************/
void SetColor(char r, char g, char b, char a, int x, int y, void* tex)
{
    uint32_t* textureDataAsInt = (uint32_t*)tex;
    uint8_t* pixel = (uint8_t*)&textureDataAsInt[x + (y*World::kWorldSize)];
    pixel[0] = b;
    pixel[1] = g;
    pixel[2] = r;
    pixel[3] = a;
}

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void World::Create(void)
{
    // Initialize texture
    const int textureSize = kWorldSize*kWorldSize*4;
    _terrainTextureData = new uint8_t[textureSize];
    memset(_terrainTextureData, 0, textureSize);
    _terrainTexture = Render::CreateTexture(kWorldSize, kWorldSize, 32, _terrainTextureData);

    // Create shaders
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

    Render::shader_t vertexShader = Render::CreateShader(vertexShaderSource, Render::kVertexShader);
    Render::shader_t pixelShader = Render::CreateShader(pixelShaderSource, Render::kPixelShader);
    _terrainMaterial = Render::CreateMaterial(vertexShader, pixelShader);

    // Create the mesh
    _terrainMesh = Render::CreateMesh(  Render::kPosTex, 
                                        sizeof(kQuadIndices)/sizeof(kQuadIndices[0]), 
                                        sizeof(kQuadVerts)/sizeof(kQuadVerts[0]), 
                                        sizeof(kQuadVerts[0]), 
                                        sizeof(kQuadIndices[0]), 
                                        kQuadVerts, 
                                        kQuadIndices);
}

void World::Destroy(void)
{
    delete [] _terrainTextureData;
}

void World::UpdateTerrainTexture(void)
{
    Render::UpdateTextureData(_terrainTexture, kWorldSize, kWorldSize, 32, _terrainTextureData);
}

void World::Update(float elapsedTime)
{
    static int ii = 0;
    SetColor(255,255,150,255,ii++,0,_terrainTextureData);
    if(ii % 500 == 0)
        UpdateTerrainTexture();
}

void World::Render(void)
{
    Render::SubmitCommand(_terrainMaterial, _terrainMesh);
}
