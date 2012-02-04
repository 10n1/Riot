/*
 * graphicsDevice_Null.cpp
 * PROJECTNAME
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "graphicsDevice.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"
#include "graphicsDevice_null.h"

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
GraphicsDevice* GraphicsDevice::Create(GraphicsAPI::Enum api, void* window)
{
    GraphicsDevice* device = NULL;

    switch(api)
    {
    case GraphicsAPI::kNull: device = new GraphicsDeviceNull(); break;
    default:
        assert(0);
    }

    device->Create(window);
    return device;
}
