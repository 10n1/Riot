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
#include <stdlib.h>
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
    //
    // Render initialization
    //

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

    //
    // Game initialization
    //
    GenerateNewTerrain();
}

void World::Destroy(void)
{
    delete [] _terrainTextureData;
}

void World::UpdateTerrainTexture(void)
{
    for(int xx=0; xx<kWorldSize; ++xx)
    {
        for(int yy=0; yy<kWorldSize; ++yy)
        {
            const tixel_t& tixel = _worldData[xx][yy];
            switch(tixel.type)
            {
            case kDirt:
                SetColor(148,100,12,255,xx,yy,_terrainTextureData);
                break;
            case kGrass:
                SetColor(25,200,50,255,xx,yy,_terrainTextureData);
                break;
            case kSky:
                SetColor(132,194,232,255,xx,yy,_terrainTextureData);
                break;
            default:
                break;
            }
        }
    }
    Render::UpdateTextureData(_terrainTexture, kWorldSize, kWorldSize, 32, _terrainTextureData);
}
void World::GenerateNewTerrain(void)
{
    int terrainHeights[kWorldSize];
    //
    // Completely random
    //
    for(int ii=0; ii<kWorldSize; ++ii)
    {
        int height = rand()%kWorldSize;
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
        for(int ii=0; ii<kWorldSize; ++ii)
        {
            int count = 0;
            int value = 0;
            for(int jj=-half; jj<half+1; ++jj)
            {
                int index = ii+jj;
                if(index < 0 || index > kWorldSize)
                    continue;
                value += terrainHeights[index];
                count++;
            }

            terrainHeights[ii] = value/count;
        }
    }

    //
    // Now update the texture
    //
    for(int ii=0; ii<kWorldSize; ++ii)
    {
        int jj;
        // Dirt
        for(jj=0; jj<terrainHeights[ii]; ++jj)
        {
            _worldData[ii][jj].type = kDirt;
        }
        // Grass
        for(int kk=0; kk<10; ++kk, ++jj)
        {   
            if(ii > kWorldSize || jj > kWorldSize)
                continue;
            _worldData[ii][jj].type = kGrass;
        }
        // Sky
        for(; jj<kWorldSize; ++jj)
        {
            if(ii > kWorldSize || jj > kWorldSize)
                continue;
            _worldData[ii][jj].type = kSky;
        }
    }

    UpdateTerrainTexture();
}

void World::Update(float elapsedTime)
{
}

void World::Render(void)
{
    Render::SubmitCommand(_terrainMaterial, _terrainMesh);
}
