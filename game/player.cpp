/*
 * player.cpp
 * Game
 *
 * Created by Kyle Weicht on 1/14/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "player.h"

/* C headers */
#include <memory.h>
#include <math.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "world.h"
#include "system/file.h"
#include "system/system.h"

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
static const int kWorldSize = World::kWorldSize;

/*******************************************************************\
Internal functions
\*******************************************************************/
void SetColor(char r, char g, char b, char a, int x, int y, void* tex)
{
    uint32_t* textureDataAsInt = (uint32_t*)tex;
    uint8_t* pixel = (uint8_t*)&textureDataAsInt[x + (y*kWorldSize)];
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
void Player::Create(void)
{
    //
    // Render initialization
    //

    // Initialize texture
    const int textureSize = kWorldSize*kWorldSize*4;
    _playerTextureData = new uint8_t[textureSize];
    memset(_playerTextureData, 0, textureSize);
    _playerTexture = Render::CreateTexture(kWorldSize, kWorldSize, 32, _playerTextureData);

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
    _playerMaterial = Render::CreateMaterial(vertexShader, pixelShader);

    // Create the mesh
    _playerMesh = Render::CreateMesh(  Render::kPosTex, 
                                        sizeof(kQuadIndices)/sizeof(kQuadIndices[0]), 
                                        sizeof(kQuadVerts)/sizeof(kQuadVerts[0]), 
                                        sizeof(kQuadVerts[0]), 
                                        sizeof(kQuadIndices[0]), 
                                        kQuadVerts, 
                                        kQuadIndices);

    //
    // Player initialization
    //
    _position[0] = _position[1] = _previousPosition[0] = _previousPosition[1] = 0;
    _facingRight = 1;
    for(int ii=0; ii<sizeof(_inventory)/sizeof(_inventory[0]); ++ii)
    {
        _inventory[ii] = 0;
    }
}
void Player::Destroy(void)
{
    delete [] _playerTextureData;
}
void Player::UpdatePlayerTexture(void)
{
    static const int kPlayerWidth = 5;
    static const int kPlayerHeight = 20;

    // Remove old position
    for(int xx=_previousPosition[0]-2; xx<_previousPosition[0]+3; ++xx)
    {
        for(int yy=_previousPosition[1]; yy<_previousPosition[1]+20; ++yy)
        {
            if(xx < 0 || xx >= kWorldSize || yy < 0 || yy >= kWorldSize)
                continue;
            SetColor(0,0,0,0,xx,yy,_playerTextureData);
        }
    }

    // Add new position
    for(int xx=_position[0]-2; xx<_position[0]+3; ++xx)
    {
        for(int yy=_position[1]; yy<_position[1]+20; ++yy)
        {
            if(xx < 0 || xx >= kWorldSize || yy < 0 || yy >= kWorldSize)
                continue;
            SetColor(0,0,0,255,xx,yy,_playerTextureData);
        }
    }

    Render::UpdateTextureData(_playerTexture, kWorldSize, kWorldSize, 32, _playerTextureData);
    _previousPosition[0] = _position[0];
    _previousPosition[1] = _position[1];
}
void Player::AddToInventory(material_e material, int count)
{
    _inventory[material] += count;
}
void Dig(World::tixel_t* tixel, void* param)
{
    Player* player = (Player*)param;

    tixel->durability -= 0.01f;
    if(tixel->durability <= 0.0f)
    {
        player->AddToInventory((material_e)tixel->type, 1);
        tixel->type = World::kNothing;
        tixel->durability = 0.0f;
    }
}
void Player::Update(float elapsedTime)
{
    const float playerYSpeed = 100.0f;
    const float playerXSpeed = 100.0f;
    const float kMaxSlope = 1.5f;

    if(System::GetKeyState(System::kKeyLeft))
    {
        _position[0] -= playerXSpeed*elapsedTime;
    }
    if(System::GetKeyState(System::kKeyRight))
    {
        _position[0] += playerXSpeed*elapsedTime;
    }

    int mouseX, mouseY;
    System::GetMousePosition(&mouseX, &mouseY);
    System::mouse_state_t mouseState = System::GetMouseState();
    mouseY = 1024 - mouseY;
    if(mouseState & System::kMouseButtonLeft)
    {
        int diffX = mouseX - _position[0];
        int diffY = mouseY - _position[1];

        if(diffX < -10 || diffX > 10 || diffY < -10 || diffY > 10)
        {
        }
        else
        {
            _world->CircleFunc(mouseX, mouseY, 10, 1, Dig, this);
        }
    }
    
    _position[1] = _world->CalculateNearestHeight(_position[0], _position[1]);
    printf("Inventory:\n");

    UpdatePlayerTexture();
}
void Player::Render(void)
{
    Render::SubmitCommand(_playerMaterial, _playerMesh, _playerTexture);
}
