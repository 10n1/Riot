/*
 * renderEngine.cpp
 * renderEngine
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "renderEngine.h"

/* C headers */
#include <stdlib.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "../graphicsDevice/graphicsDevice.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
struct render_t
{
    graphics_t* graphics;
}* s_render;

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
void renderInit(graphics_t* graphics)
{
    /* Allocate structure */
    s_render = (render_t*)malloc(sizeof(*s_render));
    s_render->graphics = graphics;
}
void renderShutdown(void)
{
}
