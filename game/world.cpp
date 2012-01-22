/*
 * world.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "world.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "graphicsDevice/graphicsDevice.h"

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
void World::Create(void)
{
}
void World::Destroy(void)
{
}

void World::Update(float)
{
}
void World::Render(void)
{
}
void World::SetGraphicsDevice(graphics_t* graphics)
{
    _graphics = graphics;
}
