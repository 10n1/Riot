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
#include <math.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "system/system.h"
#include "system/file.h"
#include "assert.h"

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
void SetColor(const uint8_t color[4], int x, int y, void* tex)
{
    uint32_t* textureDataAsInt = (uint32_t*)tex;
    uint8_t* pixel = (uint8_t*)&textureDataAsInt[x + (y*World::kWorldSize)];
    pixel[0] = color[2];
    pixel[1] = color[1];
    pixel[2] = color[0];
    pixel[3] = color[3];
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

    // Sky box stuff
    _skyMaterial = _terrainMaterial;
    _skyMesh = _terrainMesh;

    uint8_t* skyTextureData = new uint8_t[textureSize];
    for(int yy=0; yy<kWorldSize; ++yy)
    {
        for(int xx=0; xx<kWorldSize; ++xx)
        {
            if( (xx == kWorldSize/2 || xx == kWorldSize/2-1) && yy%3 == 0)
            {
                uint8_t dashColor[] = {100,100,100,255};
                SetColor(dashColor,xx,yy,skyTextureData);
                continue;             
            }
            uint8_t skyColor[] = {132,194,232,255};
            SetColor(skyColor,xx,yy,skyTextureData);
        }
    }
    _skyTexture = Render::CreateTexture(kWorldSize, kWorldSize, 32, skyTextureData);
    delete [] skyTextureData;

    //
    // Game initialization
    //
    GenerateNewTerrain();
    _player.Create();
    _player._world = this;
}

void World::Destroy(void)
{   
    _player.Destroy();
    delete [] _terrainTextureData;
}

void World::UpdateTerrainTexture(void)
{
    for(int xx=0; xx<kWorldSize; ++xx)
    {
        for(int yy=0; yy<kWorldSize; ++yy)
        {
            const tixel_t& tixel = _worldData[xx][yy];
            SetColor(kMaterialTemplates[tixel.type].color,xx,yy,_terrainTextureData);
        }
    }
    Render::UpdateTextureData(_terrainTexture, kWorldSize, kWorldSize, 32, _terrainTextureData);
}
void World::GenerateNewTerrain(void)
{
    int terrainHeights[kWorldSize];
#if 1
    //
    // Flat
    //
    for(int ii=0; ii<kWorldSize; ++ii)
    {
        terrainHeights[ii] = 20;
    }
#endif

    //
    // Now update the texture
    //
    for(int ii=0; ii<kWorldSize; ++ii)
    {
        int jj;
        // Dirt
        for(jj=0; jj<terrainHeights[ii]; ++jj)
        {
            _worldData[ii][jj] = kMaterialTemplates[kDirt];
        }
        // Grass
        for(int kk=0; kk<5; ++kk, ++jj)
        {   
            if(ii > kWorldSize || jj > kWorldSize)
                continue;
            _worldData[ii][jj] = kMaterialTemplates[kGrass];
        }
        // Sky
        for(; jj<kWorldSize; ++jj)
        {
            if(ii > kWorldSize || jj > kWorldSize)
                continue;
            _worldData[ii][jj] = kMaterialTemplates[kNothing];
        }
    }

    UpdateTerrainTexture();
}
const tixel_t* World::GetTixel(int x, int y)
{
    if(x < 0 || x >= kWorldSize || y < 0 || y >= kWorldSize)
        return nullptr;

    return &_worldData[x][y];
}
void World::Update(float elapsedTime)
{
    //
    // Mouse input
    //
    static const int kBuildBoxSize = 5;
    int mouseX, mouseY;
    System::GetMousePosition(&mouseX, &mouseY);
    System::mouse_state_t mouseState = System::GetMouseState();

    if(mouseState & System::kMouseButtonRight)
    {
        for(int x=mouseX-kBuildBoxSize; x<mouseX+kBuildBoxSize; ++x)
        {
            for(int y=mouseY-kBuildBoxSize; y<mouseY+kBuildBoxSize; ++y)
            {
                SetTileType(x, y, kDirt);
            }
        }
    }

    _player.Update(elapsedTime);

    // 
    // Make terrain fall
    //
    static float fallAmount = 0.0f;
    fallAmount += elapsedTime*50.0f;
    if(fallAmount >= 1.0f)
    {
        for(int x=0; x<kWorldSize; ++x)
        {
            for(int y=1; y < kWorldSize; ++y)
            {
                if(_worldData[x][y].type != kNothing && _worldData[x][y-1].type == kNothing)
                {
                    _worldData[x][y-1] = _worldData[x][y];
                    _worldData[x][y].type = kNothing;
                }
            }
        }
        fallAmount -= 1.0f;
    }

    UpdateTerrainTexture();
}
void Explosion(tixel_t* tixel, void* param)
{
    tixel->type = kNothing;
}
void World::MouseClick(int x, int y)
{
    //CircleFill(x,y,10,kNothing,1);
    CircleFunc(x,y,10,1,Explosion, this);
    UpdateTerrainTexture();
}
void World::SetTileType(int x, int y, material_type_e type)
{
    if(x < 0 || x >= kWorldSize || y < 0 || y >= kWorldSize)
        return;

    assert(type >= kNothing && type < kMAX_MATERIAL_TYPES);
    _worldData[x][y] = kMaterialTemplates[type];
}
void World::CircleFill(int x0, int y0, int radius, material_type_e type, int fill)
{   // Souce: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    _worldData[x0][y0 + radius].type = type;
    _worldData[x0][y0 - radius].type = type;
    _worldData[x0 + radius][y0].type = type;
    _worldData[x0 - radius][y0].type = type;

    while(x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;    

        if(fill)
        {
            for(int xx=x0 - x; xx <= x0 + x; ++xx)
            {
                for(int yy=y0 - y; yy <= y0 + y; ++yy)
                {
                    _worldData[xx][yy].type = type;
                }
            }
            for(int xx=x0 - y; xx <= x0 + y; ++xx)
            {
                for(int yy=y0 - x; yy <= y0 + x; ++yy)
                {
                    _worldData[xx][yy].type = type;
                }
            }
        }

        //_worldData[x0 + x][y0 + y].type = type;
        //_worldData[x0 - x][y0 + y].type = type;
        //_worldData[x0 + x][y0 - y].type = type;
        //_worldData[x0 - x][y0 - y].type = type;
        //_worldData[x0 + y][y0 + x].type = type;
        //_worldData[x0 - y][y0 + x].type = type;
        //_worldData[x0 + y][y0 - x].type = type;
        //_worldData[x0 - y][y0 - x].type = type;

    }
}
void World::CircleFunc(int x0, int y0, int radius, int fill, void (*func)(tixel_t*,void*), void* param)
{// Souce: http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    func(&_worldData[x0][y0 + radius],param);
    func(&_worldData[x0][y0 - radius],param);
    func(&_worldData[x0 + radius][y0],param);
    func(&_worldData[x0 - radius][y0],param);

    while(x < y)
    {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if(f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;    

        if(fill)
        {
            for(int xx=x0 - x; xx <= x0 + x; ++xx)
            {
                for(int yy=y0 - y; yy <= y0 + y; ++yy)
                {
                    func(&_worldData[xx][yy],param);
                }
            }
            for(int xx=x0 - y; xx <= x0 + y; ++xx)
            {
                for(int yy=y0 - x; yy <= y0 + x; ++yy)
                {
                    func(&_worldData[xx][yy],param);
                }
            }
        }

        func(&_worldData[x0 + x][y0 + y],param);
        func(&_worldData[x0 - x][y0 + y],param);
        func(&_worldData[x0 + x][y0 - y],param);
        func(&_worldData[x0 - x][y0 - y],param);
        func(&_worldData[x0 + y][y0 + x],param);
        func(&_worldData[x0 - y][y0 + x],param);
        func(&_worldData[x0 + y][y0 - x],param);
        func(&_worldData[x0 - y][y0 - x],param);

    }
}
void World::Render(void)
{
    Render::SubmitCommand(_skyMaterial, _skyMesh, _skyTexture);
    Render::SubmitCommand(_terrainMaterial, _terrainMesh, _terrainTexture);
    _player.Render();
}
