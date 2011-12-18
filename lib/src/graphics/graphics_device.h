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

union Shader
{
    void*   pointerShader;
    int64_t intShader;
};

struct Material
{
    Shader  vertexShader;
    Shader  pixelShader;
    int64_t program;
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

extern Shader (*CreateVertexShader)(const char* shaderSource);
extern Shader (*CreatePixelShader)(const char* shaderSource);
extern Material (*CreateMaterial)(const Shader& vertexShader, const Shader& pixelShader);

/* Null device */
void CreateNullDevice(void* window);

/* OpenGL device */
void CreateOpenGLDevice(void* window);
void* GetOpenGLCGLContext(void);

}

#endif
