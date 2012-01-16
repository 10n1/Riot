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
#include "build.h"
#include "world.h"
#include "system/file.h"
#include "materials.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/

/*******************************************************************\
Internal functions
\*******************************************************************/

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
    for(int ii=0; ii<ARRAY_LENGTH(_inventory); ++ii)
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
