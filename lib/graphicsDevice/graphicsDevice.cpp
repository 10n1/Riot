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
#include "graphicsDevice_null.h"
#ifdef _WIN32
    #include "graphicsDevice_directx.h"
#endif
#include "global.h"

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
    case GraphicsAPI::kDirectX: device = new GraphicsDeviceDirectX(); break;
    default:
        assert(0);
    }

    device->Create(window);
    return device;
}
