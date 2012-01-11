/*
 *  graphicsDevice.h
 *  graphicsDevice.lib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __graphicsDevice_h__
#define __graphicsDevice_h__

//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace GraphicsDevice
{

/*******************************************************************\
 External constants and types
\*******************************************************************/

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/

// Device functions
void Initialize(void* window);
void Shutdown(void);

// Pipeline control
void SetClearColor(float r, float g, float b, float a);
void SetClearDepth(float d);

// Frame controls
void Clear(void);
void Present(void);

} // namespace Render

#endif /* include guard */
