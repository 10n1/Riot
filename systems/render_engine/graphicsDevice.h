/*
 *  graphicsDevice.h
 *  graphicsDevice.lib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __graphicsDevice_h__
#define __graphicsDevice_h__

#include <stdint.h>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace GraphicsDevice
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
union shader_t
{
    void*   pointerShader;
    int64_t intShader;
};
union buffer_t
{
    void*   pointerBuffer;
    int64_t intBuffer;
};
enum index_format_e
{
    kIndex16,
    kIndex32,
};

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

// Render object creation
shader_t CreateVertexShader(const char* shaderSource);
shader_t CreatePixelShader(const char* shaderSource);
buffer_t CreateVertexBuffer(size_t size, const void* data);
buffer_t CreateIndexBuffer(size_t size, const void* data);

// Set functions

// Draw commands
void Draw(index_format_e indexFormat, int indexCount);

} // namespace Render

#endif /* include guard */
