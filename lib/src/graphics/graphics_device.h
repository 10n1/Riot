/*
 *  graphics_device.h
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef engine_graphics_device_h_
#define engine_graphics_device_h_

#include "types.h"

namespace GraphicsDevice
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
struct Mesh;

union shader_t
{
    void*   pointerShader;
    int64_t intShader;
};

struct material_t
{
    shader_t    vertexShader;
    shader_t    pixelShader;
    int64_t     program;
};

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
extern void (*Initialize)(void* window);
extern void (*Shutdown)(void);
extern void (*BeginFrame)(void);
extern void (*Clear)(void);
extern void (*Present)(void);
extern void (*EndFrame)(void);
extern void (*SetClearColor)(float r, float g, float b, float a, float depth);

extern shader_t (*CreateVertexShader)(const char* shaderSource);
extern shader_t (*CreatePixelShader)(const char* shaderSource);
extern material_t (*CreateMaterial)(const shader_t& vertexShader, const shader_t& pixelShader);

/* Null device */
void CreateNullDevice(void* window);

/* OpenGL device */
void CreateOpenGLDevice(void* window);
void* GetOpenGLCGLContext(void);

}

#endif
