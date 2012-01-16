/*
 *  graphicsDevice_opengl.cpp
 *  renderEngine
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#include "graphicsDevice.h"
#include "renderEngineInternal.h"

/*
 * Platform detection
 */
#define BUILD_PLATFORM_WINDOWS 1
#define BUILD_PLATFORM_MACOS   2
#define BUILD_PLATFORM_LINUX   3
#define BUILD_PLATFORM_IOS     4
#define BUILD_PLATFORM_ANDROID 5

#if defined( WIN32 )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#elif defined( __MACH__ )
    #include <TargetConditionals.h>
    #if( TARGET_OS_IPHONE )
        #define BUILD_PLATFORM_ID BUILD_PLATFORM_IOS
    #else
        #define BUILD_PLATFORM_ID BUILD_PLATFORM_MACOS
    #endif
#elif defined( __ANDROID__ )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_ANDROID
#elif defined( __linux__ )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_LINUX
#else
    #error No valid target found
#endif


#include "glew/gl/glew.h"
#include "glew/gl/wglew.h"
#include <stdio.h>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
HDC     s_hDC         = nullptr;
HGLRC   s_hGLRC       = nullptr;
int     s_pixelFormat = -1;

/*******************************************************************\
 Internal functions
\*******************************************************************/
GLuint CompileShader(GLenum shaderType, const char* shaderSource)
{   
    GLchar  statusBuffer[1024] = {0};
    GLint   status = GL_TRUE;
    GLuint  shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderInfoLog(shader, sizeof(statusBuffer), NULL, statusBuffer);
        printf("Error:\t%s\n", statusBuffer);
        assert(0);
    }
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
    return shader;
}

} // namespace

namespace GraphicsDevice
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
void Initialize(void* window)
{
    HDC     hDC = NULL;
    HGLRC   hGLRC = NULL;
    HWND    hWnd = static_cast<HWND>(window);
    int     pixelFormat = 0;
    
    //
    // Create context to load extensions
    //
    hDC = GetDC(hWnd);
    
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    assert(pixelFormat != 0);

    BOOL bSuccess = SetPixelFormat(hDC, pixelFormat, &pfd);
    assert(bSuccess);

    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);

    // Initialize Glew
    GLenum error = glewInit();
    if(GLEW_OK != error)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("Error: %s\n", glewGetErrorString(error));
        assert(0);
    }

    //
    // Create new OpenGL context
    //

    assert(wglewIsSupported("WGL_ARB_create_context") == 1);

    GLint pCreationAttributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,
        0,
    };
    HGLRC newContext = wglCreateContextAttribsARB( hDC, 0, pCreationAttributes );
    assert(hGLRC);

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(hGLRC);
    wglMakeCurrent(hDC, newContext);
    hGLRC = newContext;

    s_hDC   = hDC;
    s_hGLRC = hGLRC;
    s_pixelFormat = pixelFormat;

    const char* majorVersion = (const char*)glGetString(GL_VERSION);

    //
    // Perform OpenGL initialization
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void Shutdown(void)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(s_hGLRC);
    s_hGLRC = nullptr;
}
#else
    #error Need OpenGL
#endif // BUILD_PLATFORM_ID

// Pipeline control
void SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}
void SetClearDepth(float d)
{
    glClearDepth(d);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}

// Frame controls
void Clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}
void Present(void)
{
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    SwapBuffers(s_hDC);
#else
    #error Need OpenGL
#endif // BUILD_PLATFORM_ID
}


// Render object creation
shader_t CreateVertexShader(const char* shaderSource)
{
    GLuint shaderId = CompileShader(GL_VERTEX_SHADER, shaderSource);
    shader_t shader;
    shader.intShader = shaderId;

    return shader;
}
shader_t CreatePixelShader(const char* shaderSource)
{
    GLuint shaderId = CompileShader(GL_FRAGMENT_SHADER, shaderSource);
    shader_t shader;
    shader.intShader = shaderId;

    return shader;
}
program_t CreateProgram(shader_t vertexShader, shader_t pixelShader)
{
    GLchar  statusBuffer[1024] = {0};
    GLint   status  = GL_TRUE;
    GLuint  program = glCreateProgram();
    glAttachShader(program, vertexShader.intShader);
    glAttachShader(program, pixelShader.intShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetProgramInfoLog(program, sizeof(statusBuffer), NULL, statusBuffer);
        printf("Link error:\t%s\n", statusBuffer);
        assert(0);
    }
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    
    return program;
}
texture_t CreateTexture(int width, int height, int bits, void* data)
{
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLint components;
    GLint format;    
    switch(bits)
    {
        case 24:
            format = GL_BGR;
            components = GL_RGB;
            break;
        case 32:
            format = GL_BGRA;
            components = GL_RGBA;
            break;
        case 8:
            assert(0);
            break;
        default:
            assert(0);
            break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, (GLint)components, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    GLenum error = glGetError();
    switch(error)
    {
    case GL_NO_ERROR:
        break;
    case GL_INVALID_ENUM:
        assert(0);
    case GL_INVALID_VALUE:
        assert(0);
    case GL_INVALID_OPERATION:
        assert(0);
    default:
        format = 0;
        printf("Error not handled\n");
        assert(0);
    }

    //glBindTexture(GL_TEXTURE_2D, 0);

    texture_t texture;
    texture.intTexture = textureId;

    return texture;
}
buffer_t CreateVertexBuffer(size_t size, const void* data)
{
    GLuint bufferId = 0;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    buffer_t buffer;
    buffer.intBuffer = bufferId;
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    return buffer;
}
buffer_t CreateIndexBuffer(size_t size, const void* data)
{
    GLuint bufferId = 0;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    buffer_t buffer;
    buffer.intBuffer = bufferId;
    
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
    return buffer;
}

mesh_t CreateMesh(  const vertex_layout_t* layout,
                    int indexCount, 
                    int vertexCount, 
                    size_t vertexStride, 
                    size_t indexSize, 
                    const void* vertices, 
                    const void* indices)
{
    // Create buffers
    buffer_t vertexBuffer = CreateVertexBuffer(vertexStride*vertexCount, vertices);
    buffer_t indexBuffer = CreateIndexBuffer(indexSize*indexCount, indices);

    // Create layout
    int     offset = 0;
    GLuint  vao = 0;

    glGenVertexArrays(1, &vao);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);

    assert(vao);
    assert(layout);
    assert(vertexStride);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.intBuffer);
    error = glGetError();
    assert(error == GL_NO_ERROR);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.intBuffer);
    error = glGetError();
    assert(error == GL_NO_ERROR);

    error = glGetError();
    assert(error == GL_NO_ERROR);

    while(layout && layout->slot != kShaderInputNull)
    {
        glEnableVertexAttribArray((uint32_t)layout->slot);

        error = glGetError();
        assert(error == GL_NO_ERROR);

        glVertexAttribPointer((uint32_t)layout->slot, layout->count, GL_FLOAT, GL_FALSE, vertexStride, (void*)offset);

        error = glGetError();
        assert(error == GL_NO_ERROR);
        switch(error)
        {
        case GL_NO_ERROR:
            break;
        case GL_INVALID_VALUE:
            error = error;
            break;
        case GL_INVALID_ENUM:
            error = error;
            break;
        default:
            assert(0);
        };

        offset += layout->count*(int)sizeof(float);
        layout++;
    }
    glBindVertexArray(0);

    error = glGetError();
    assert(error == GL_NO_ERROR);

    vertex_format_t format;
    format.intFormat = vao;

    mesh_t mesh;
    mesh.indexBuffer = indexBuffer;
    mesh.vertexBuffer = vertexBuffer;
    mesh.vertexLayout = format;
    mesh.indexCount = indexCount;
    mesh.indexFormat = (indexSize == 2) ? GraphicsDevice::kIndex16 : GraphicsDevice::kIndex32;

    return mesh;
}
vertex_format_t CreateVertexLayout(const vertex_layout_t* layout, int vertexStride)
{
    int     offset = 0;
    GLuint  vao = 0;

    glGenVertexArrays(1, &vao);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);

    assert(vao);
    assert(layout);
    assert(vertexStride);
    glBindVertexArray(vao);

    error = glGetError();
    assert(error == GL_NO_ERROR);

    while(layout && layout->slot != kShaderInputNull)
    {
        glEnableVertexAttribArray((uint32_t)layout->slot);

        error = glGetError();
        assert(error == GL_NO_ERROR);

        glVertexAttribPointer((uint32_t)layout->slot, layout->count, GL_FLOAT, GL_FALSE, vertexStride, (void*)offset);

        error = glGetError();
        assert(error == GL_NO_ERROR);
        switch(error)
        {
        case GL_INVALID_VALUE:
            error = error;
            break;
        case GL_INVALID_ENUM:
            error = error;
            break;
        default:
            assert(0);
        };

        offset += layout->count*(int)sizeof(float);
        layout++;
    }
    glBindVertexArray(0);

    error = glGetError();
    assert(error == GL_NO_ERROR);

    vertex_format_t format;
    format.intFormat = vao;

    return format;
}

// Set functions
void SetVertexShader(shader_t)
{
    /* invalid in OpenGL */
}
void SetPixelShader(shader_t)
{
    /* invalid in OpenGL */
}
void SetProgram(program_t program)
{
    glUseProgram(program);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}
void SetVertexLayout(vertex_format_t layout)
{
    glBindVertexArray(layout.intFormat);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}
void BindVertexBuffer(buffer_t buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer.intBuffer);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}
void BindIndexBuffer(buffer_t buffer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.intBuffer);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}
void SetTexture(texture_t texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.intTexture);
}
void UpdateTextureData(texture_t texture, int width, int height, int bits, void* data)
{
    GLenum format;
    switch(bits)
    {
        case 24: format = GL_BGR; break;
        case 32: format = GL_BGRA; break;
        default: assert(0);
    }
    glBindTexture(GL_TEXTURE_2D, texture.intTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Draw commands
void Draw(index_format_e indexFormat, int indexCount)
{
    glDrawElements(GL_TRIANGLES, indexCount, (indexFormat == kIndex16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0);
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
}
void DrawMesh(mesh_t mesh)
{
    glBindVertexArray(mesh.vertexLayout.intFormat);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, (mesh.indexFormat == kIndex16) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0);
}

} // namespace GraphicsDevice
