/*
 * graphicsDevice_opengl.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "graphicsDevice_opengl.h"

/* C headers */
#include <stdio.h>
#include <stdlib.h>
/* C++ headers */
/* External headers */
#include "stb_image.h"
/* Internal headers */
#include "system.h"
#include "global.h"

#define CheckGLError() \
    { GLenum _glError = glGetError(); \
    assert(_glError == GL_NO_ERROR); }
    
#define malloc_and_zero(pointer, type) pointer = (type)malloc(sizeof(*pointer)); memset(pointer, 0, sizeof(*pointer))

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
static GLuint CompileShader(GLenum shaderType, const char* filename)
{
    char            buffer[1024*8]  = "#version 330\n#define OPENGL\n";
    char*           readPosition    = buffer+(sizeof("#version 330\n#define OPENGL\n")-1);
    GLchar          statusBuffer[1024] = {0};
    char            messageBoxBuffer[1024] = {0};
    GLint           status      = GL_TRUE;
    GLuint          shader      = glCreateShader(shaderType);
    System::MessageBoxResult::Enum result   = System::MessageBoxResult::kOk;
    FILE*           shaderFile  = NULL;
    size_t          bytesRead   = 0;
    const char*     shaderSource = buffer;

    do
    {
        shaderFile = fopen(filename, "r");
        bytesRead = fread(readPosition, sizeof(buffer[0]), sizeof(buffer), shaderFile);
        readPosition[bytesRead] = '\0';
        fclose(shaderFile);

        CheckGLError();
        glShaderSource(shader, 1, &shaderSource, NULL);
        CheckGLError();
        glCompileShader(shader);
        CheckGLError();
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        CheckGLError();
        if(status == GL_FALSE)
        {
            glGetShaderInfoLog(shader, sizeof(statusBuffer), NULL, statusBuffer);
            sprintf(messageBoxBuffer, "Error compiling %s:\t%s\n", filename, statusBuffer);
            result = System::MessageBox("Shader Error", messageBoxBuffer, System::MessageBoxType::kRetryCancel);
        }
        else
            break;
    } while(result == System::MessageBoxResult::kRetry);
    CheckGLError();
    
    return shader;
}

} // anonymous namespace

struct vertex_shader_t
{
    GLuint shader;
};
struct pixel_shader_t
{
    GLuint shader;
};
struct material_t
{
    GLuint program;
};
struct mesh_t
{
    GLuint      vertexBuffer;
    GLuint      indexBuffer;
    GLuint      inputLayout;
    uint32_t    indexSize;
    uint32_t    indexCount;
};
struct texture_t
{
    GLuint  texture;
};
struct constant_buffer_t
{
    GLuint  buffer;
};
/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
#ifdef _WIN32
void GraphicsDeviceOpenGL::Create(void* window)
{
    HDC     hDC         = NULL;
    HGLRC   hGLRC       = NULL;
    HGLRC   newContext = NULL;
    HWND    hWnd        = (HWND)(window);
    int     pixelFormat = 0;
    BOOL    success     = 1;
    GLenum  error       = GL_NO_ERROR;
	PIXELFORMATDESCRIPTOR   pfd;

    GLint pCreationAttributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,
        0,
    };
    
    /*
     * Create context to load extensions
     */
    hDC = GetDC(hWnd);
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize  = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    assert(pixelFormat != 0);

    success = SetPixelFormat(hDC, pixelFormat, &pfd);
    assert(success);

    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);
    CheckGLError();

    /* Glew */
    error = glewInit();
    CheckGLError();
    if(GLEW_OK != error)
    {
        /* Glew init failed...not sure what happened */
        printf("Error: %s\n", glewGetErrorString(error));
        assert(0);
    }
    
    /*
     * Create new OpenGL context
     */
    assert(wglewIsSupported("WGL_ARB_create_context") == 1);

    newContext = wglCreateContextAttribsARB(hDC, 0, pCreationAttributes);
    assert(hGLRC);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hGLRC);
    wglMakeCurrent(hDC, newContext);
    hGLRC = newContext;

    /*
     * Create the device
     */
    memset( (char*)this + sizeof(void*) , 0, sizeof(this)-sizeof(void*));

    _dc = hDC;
    _rc = hGLRC;

    CheckGLError();
    glFrontFace(GL_CW);
    CheckGLError();
    glCullFace(GL_BACK);
    CheckGLError();
    glEnable(GL_CULL_FACE);
    CheckGLError();
}
#elif defined(__APPLE__)
#endif
void GraphicsDeviceOpenGL::Destroy(void)
{
#ifdef _WIN32
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(_rc);
#endif
    delete this;
}
GraphicsAPI::Enum GraphicsDeviceOpenGL::GetAPI(void)
{
    return GraphicsAPI::kOpenGL;
}

/* Device contols */
void GraphicsDeviceOpenGL::Clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckGLError();
}
void GraphicsDeviceOpenGL::Present(void)
{
    SwapBuffers(_dc);
}
void GraphicsDeviceOpenGL::Resize(int width, int height)
{
    glViewport(0,0,width,height);
    CheckGLError();
}
    
/* Pipeline management */
void GraphicsDeviceOpenGL::SetClearColor(float r, float g, float b, float a, float depth)
{
    glClearColor(r,g,b,a);
    CheckGLError();
    glClearDepth(depth);
    CheckGLError();
}
void GraphicsDeviceOpenGL::SetDepthTest(int test, int write)
{
    if((test & write) == 0)
    {
        glDisable(GL_DEPTH_TEST);
    }
    else
        glEnable(GL_DEPTH_TEST);
    
    glDepthMask( write ? GL_TRUE : GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
}
void GraphicsDeviceOpenGL::SetAlphaTest(int enable)
{
    if(enable)
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

/* Object creation */
vertex_shader_t* GraphicsDeviceOpenGL::CreateVertexShader(const char* filename)
{
    vertex_shader_t*    shader       = NULL;
    GLuint              vertexShader = 0;

    vertexShader = CompileShader(GL_VERTEX_SHADER, filename);

    /* Create new shader */
    malloc_and_zero(shader, vertex_shader_t*);
    shader->shader = vertexShader;

    return shader;
}
pixel_shader_t* GraphicsDeviceOpenGL::CreatePixelShader(const char* filename)
{
    pixel_shader_t*     shader       = NULL;
    GLuint              pixelShader = 0;
    
    pixelShader = CompileShader(GL_FRAGMENT_SHADER, filename);

    /* Create new shader */
    malloc_and_zero(shader, pixel_shader_t*);
    shader->shader = pixelShader;

    return shader;
}
material_t* GraphicsDeviceOpenGL::CreateMaterial(vertex_shader_t* vertexShader, pixel_shader_t* pixelShader)
{
    material_t* material = NULL;
    GLchar      statusBuffer[1024] = {0};
    GLint       status  = GL_TRUE;
    GLuint      program = glCreateProgram();

    glAttachShader(program, vertexShader->shader);
    CheckGLError();
    glAttachShader(program, pixelShader->shader);
    CheckGLError();
    glLinkProgram(program);
    CheckGLError();
    
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetProgramInfoLog(program, sizeof(statusBuffer), NULL, statusBuffer);
        printf("Link error:\t%s\n", statusBuffer);
        assert(0);
    }
    CheckGLError();
    
    /* allocate material */
    malloc_and_zero(material, material_t*);
    material->program = program;
    
    return material;
}
mesh_t* GraphicsDeviceOpenGL::CreateMesh( vertex_shader_t* vertexShader,
                                        const vertex_element_desc_t* layout,
                                        uint32_t vertexCount,
                                        uint32_t indexCount,
                                        size_t vertexSize,
                                        size_t indexSize,
                                        const void* vertices,
                                        const void* indices)
{
    mesh_t*     mesh        = NULL;
    GLuint      buffers[2]  = {0};
    GLuint      vao         = 0;
    intptr_t    offset      = 0;
    
    glGenBuffers(2, buffers);
    CheckGLError();
    assert(buffers[0] && buffers[1]);
    /* vertex buffer */
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    CheckGLError();
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertexSize*vertexCount), vertices, GL_STATIC_DRAW);
    CheckGLError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CheckGLError();
    /* index buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    CheckGLError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(indexCount*indexSize), indices, GL_STATIC_DRAW);
    CheckGLError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    CheckGLError();

    /* vao */
    glGenVertexArrays(1, &vao);
    CheckGLError();
    assert(vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

    while(layout && layout->slot != ShaderInputSlot::kNull)
    {
        glEnableVertexAttribArray((GLuint)layout->slot);
        CheckGLError();
        glVertexAttribPointer((GLuint)layout->slot, (GLint)layout->count, GL_FLOAT, GL_FALSE, (GLsizei)vertexSize, (void*)offset);
        CheckGLError();

        offset += layout->count*sizeof(float);
        ++layout;
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    CheckGLError();

    /* allocate mesh */
    malloc_and_zero(mesh, mesh_t*);
    mesh->vertexBuffer  = buffers[0];
    mesh->indexBuffer   = buffers[1];
    mesh->inputLayout   = vao;
    mesh->indexSize     = (uint32_t)indexSize;
    mesh->indexCount    = indexCount;

    return mesh;
}
texture_t* GraphicsDeviceOpenGL::CreateTexture(const char* filename)
{
    texture_t*  texture     = NULL;
    uint8_t*    textureData = NULL;
    int         width       = 0;
    int         height      = 0;
    int         comp        = 0;
    GLuint      textureId   = 0;
    GLint       components  = 0;
    GLenum      format      = 0;
    
    /* Load the texture data */
    textureData = stbi_load(filename, &width, &height, &comp, 0);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    switch(comp)
    {
    case 4:
        format = GL_RGBA;
        components = GL_RGBA;
        break;
    case 3:
        format = GL_RGB;
        components = GL_RGBA;
        break;
    default: assert(0);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
    CheckGLError();
    glBindTexture(GL_TEXTURE_2D, 0);

    /* release texture data */
    stbi_image_free(textureData);

    /* allocate texture */
    malloc_and_zero(texture, texture_t*);
    texture->texture = textureId;

    return texture;
}
constant_buffer_t* GraphicsDeviceOpenGL::CreateConstantBuffer(size_t size, const void* data)
{
    constant_buffer_t*  constantBuffer = NULL;
    GLuint              buffer = 0;

    glGenBuffers(1, &buffer);
    if(data && size)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    CheckGLError();

    /* allocate buffer */
    malloc_and_zero(constantBuffer, constant_buffer_t*);
    constantBuffer->buffer = buffer;

    return constantBuffer;
}

/* object controls */
void GraphicsDeviceOpenGL::UpdateConstantBuffer(constant_buffer_t* buffer, size_t size, const void* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->buffer);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void GraphicsDeviceOpenGL::BindConstantBufferToIndex(material_t* material, const char* bufferName, int index)
{
    GLuint bufferIndex = glGetUniformBlockIndex(material->program, bufferName);
    glUniformBlockBinding(material->program, bufferIndex, index);
}

/* Object destruction */
void GraphicsDeviceOpenGL::DestroyVertexShader(vertex_shader_t* shader)
{
    glUseProgram(0);
    glDeleteShader(shader->shader);
    free(shader);
}
void GraphicsDeviceOpenGL::DestroyPixelShader(pixel_shader_t* shader)
{
    glUseProgram(0);
    glDeleteShader(shader->shader);
    free(shader);
}
void GraphicsDeviceOpenGL::DestroyMaterial(material_t* material)
{
    glUseProgram(0);
    glDeleteProgram(material->program);
    free(material);
}
void GraphicsDeviceOpenGL::DestroyMesh(mesh_t* mesh)
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &mesh->inputLayout);
    glDeleteBuffers(1, &mesh->vertexBuffer);
    glDeleteBuffers(1, &mesh->indexBuffer);
    free(mesh);
}
void GraphicsDeviceOpenGL::DestroyTexture(texture_t* texture)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture->texture);
    free(texture);
}
void GraphicsDeviceOpenGL::DestroyConstantBuffer(constant_buffer_t* buffer)
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glDeleteBuffers(1, &buffer->buffer);
    free(buffer);
}

/* Render controls */
void GraphicsDeviceOpenGL::SetMaterial(material_t* material)
{
    glUseProgram(material->program);
    CheckGLError();
}
void GraphicsDeviceOpenGL::SetTexture(texture_t* texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->texture);
}
void GraphicsDeviceOpenGL::SetVSConstantBuffer(constant_buffer_t* buffer, uint32_t index)
{    
    glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer->buffer);
}
void GraphicsDeviceOpenGL::SetPSConstantBuffer(constant_buffer_t* buffer, uint32_t index)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer->buffer);
}
void GraphicsDeviceOpenGL::DrawMesh(mesh_t* mesh)
{
    glBindVertexArray(mesh->inputLayout);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indexCount, (mesh->indexSize == 2) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0);
}
