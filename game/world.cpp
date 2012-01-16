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

    // Sky box stuff
    _skyMaterial = _terrainMaterial;
    _skyMesh = _terrainMesh;

    uint8_t* skyTextureData = new uint8_t[textureSize];
    for(int yy=0; yy<kWorldSize; ++yy)
    {
        for(int xx=0; xx<kWorldSize; ++xx)
        {
            SetColor(132,194,232,255,xx,yy,skyTextureData);
        }
    }
    _skyTexture = Render::CreateTexture(kWorldSize, kWorldSize, 32, skyTextureData);
    delete [] skyTextureData;

    //
    // Game initialization
    //
    int posX = kWorldSize/2;
    int posY = kWorldSize-1;

    GenerateNewTerrain();
    _player.Create();
    _player._world = this;
    while(_worldData[posX][posY].type == kNothing)
        posY--;
    _player._position[0] = posX;
    _player._position[1] = posY;
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
            switch(tixel.type)
            {
            case kDirt:
                SetColor(148,100,12,tixel.durability*255,xx,yy,_terrainTextureData);
                break;
            case kGrass:
                SetColor(25,200,50,tixel.durability*255,xx,yy,_terrainTextureData);
                break;
            case kNothing:
                SetColor(0,0,0,0,xx,yy,_terrainTextureData);
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
    for(int kk=0; kk<12; ++kk)
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

#if 0
    //
    // Flat
    //
    for(int ii=0; ii<kWorldSize; ++ii)
    {
        terrainHeights[ii] = kWorldSize/2;
        terrainHeights[ii] = ii;
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
            _worldData[ii][jj].type = kDirt;
            _worldData[ii][jj].durability = 1.0f;
        }
        // Grass
        for(int kk=0; kk<10; ++kk, ++jj)
        {   
            if(ii > kWorldSize || jj > kWorldSize)
                continue;
            _worldData[ii][jj].type = kGrass;
            _worldData[ii][jj].durability = 1.0f;
        }
        // Sky
        for(; jj<kWorldSize; ++jj)
        {
            if(ii > kWorldSize || jj > kWorldSize)
                continue;
            _worldData[ii][jj].type = kNothing;
            _worldData[ii][jj].durability = 0.0f;
        }
    }

    UpdateTerrainTexture();
}
const World::tixel_t* World::GetTixel(int x, int y)
{
    if(x < 0 || x >= kWorldSize || y < 0 || y >= kWorldSize)
        return nullptr;

    return &_worldData[x][y];
}
void World::Update(float elapsedTime)
{
    _player.Update(elapsedTime);
    UpdateTerrainTexture();
}
void GrassFunc(World::tixel_t* tixel)
{
    tixel->durability -= 0.01f;
    if(tixel->durability <= 0.0f)
        tixel->type = World::kNothing;
}
void World::MouseClick(int x, int y)
{
    //_worldData[x][y].type = kNothing;
    //CircleFill(x,y, 20, kNothing, true);
    //CircleFunc(x,y,10, true, &GrassFunc);
    CalculateSlope(x,y);

    UpdateTerrainTexture();
}
void World::CircleFill(int x0, int y0, int radius, tixel_type_e type, int fill)
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
float Length(int v[2])
{
    float len = v[0]*v[0] + v[1]*v[1];
    return sqrtf(len);
}
float DotProduct(int v1[2], int v2[2])
{
    return (v1[0]*v2[0] + v1[1]*v2[1]);
}

float World::CalculateSlope(int x, int y)
{
    static const int kBoxSize = 20;

    float avgSlope = 0;
    int count = 0;

    int prevX = x-kBoxSize;
    int prevY = 0;
    for(int yy=y+kBoxSize; yy>y-kBoxSize; --yy)
    {
        if(yy < 0 || yy > kWorldSize)
            continue;

        const tixel_t* tixel = GetTixel(prevX,yy);
        if(tixel->type == kNothing)
            continue;

        prevY = yy;
        break;
    }

    for(int xx=x-(kBoxSize-1); xx<x+kBoxSize; ++xx)
    {
        int yy;
        for(yy=y+kBoxSize; yy>y-kBoxSize; --yy)
        {
            if(xx < 0 || xx > kWorldSize || yy < 0 || yy > kWorldSize)
                continue;

            const tixel_t* tixel = GetTixel(xx,yy);
            if(tixel->type == kNothing)
                continue;

            float rise = yy-prevY;
            float run = xx-prevX;
            float slope = rise/run;
            
            avgSlope += slope;
            count++;

            break;
        }
        prevX = xx;
        prevY = yy;
    }

    if(count == 0)
    {
        printf("Oh shit!\n");
    }
    else
    {
        avgSlope /= count;
        printf("Slope: %f\n", avgSlope );
    }

    return avgSlope;
}
int World::CalculateNearestHeight(int x, int y)
{   
    int newY = y;
    if(_worldData[x][newY].type == kNothing)
    {
        int count = 0;
        while(_worldData[x][newY].type == kNothing && count >= -5)
        {
            --newY;
            --count;
        }
        if(count == -5)
            return y;

        return newY;
    }

    int count = 0;
    while(_worldData[x][newY].type != kNothing && count <= 5)
    {
        ++newY;
        --count;
    }
    --newY;
    if(count == 5)
        return y;

    return newY;
}
void World::Render(void)
{
    Render::SubmitCommand(_skyMaterial, _skyMesh, _skyTexture);
    Render::SubmitCommand(_terrainMaterial, _terrainMesh, _terrainTexture);
    _player.Render();
}
