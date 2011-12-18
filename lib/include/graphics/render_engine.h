/*
 *  render_engine.h
 *  riot
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef riot_render_engine_h_
#define riot_render_engine_h_
#include "build.h"
//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

/*******************************************************************\
 External constants and types
\*******************************************************************/
namespace RenderStatus
{
    enum Enum
    {
        kOk = 0,
        kError,
        kUninitialized,
    };
}

namespace GraphicsDeviceType
{
    enum Enum
    {
        kNull = 0,
        kOpenGL,
        kDirect3D,
    };
}

namespace RenderEngine
{

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/

} // namespace RenderEngine

#endif /* include guard */
