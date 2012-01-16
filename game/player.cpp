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
#include "materials.h"

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
    // Player initialization
    //
    for(int ii=0; ii<sizeof(_inventory)/sizeof(_inventory[0]); ++ii)
    {
        _inventory[ii] = 0;
    }
}
void Player::Destroy(void)
{
}

void Player::AddToInventory(material_type_e material, int count)
{
    _inventory[material] += count;
}
void Player::Update(float elapsedTime)
{
}
void Player::Render(void)
{
}
