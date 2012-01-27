/*
 * openglDevice.c
 * graphicsDevice
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "graphicsDevice.h"

#if (GFX_API == GFX_OPENGL)
#include "openglDevice.h"

/* C headers */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
/* External headers */
/* Internal headers */
#include "global.h"
#include "stb_image.h"

static GLenum   _glError = GL_NO_ERROR;
#define CheckGLError() \
    _glError = glGetError(); \
    assert(_glError == GL_NO_ERROR)


/*******************************************************************\
Internal Constants And types
\*******************************************************************/
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

enum
{
    kOk,
    kRetry,
    kCancel
};

/*******************************************************************\
Internal variables
\*******************************************************************/
static malloc_func_t*   gfx_malloc = &malloc;
static free_func_t*     gfx_free   = &free;

/*******************************************************************\
Internal functions
\*******************************************************************/
static int RetryMessageBox(const char* header, const char* message)
{
#ifdef _WIN32
    int messageBoxResult = MessageBox(NULL, message, header, MB_RETRYCANCEL);
    if(messageBoxResult == IDRETRY)
        return kRetry;
#else    
    /*convert the strings from char* to CFStringRef */
    CFStringRef header_ref  = CFStringCreateWithCString(NULL, header, kCFStringEncodingASCII);
    CFStringRef message_ref = CFStringCreateWithCString(NULL, message, kCFStringEncodingASCII);
    CFStringRef buttonText = CFSTR("Retry");

    CFOptionFlags result;  /*result code from the message box */
   
    /*launch the message box */
    CFUserNotificationDisplayAlert( 0.0f, /* no timeout */
                                    kCFUserNotificationNoteAlertLevel, /*change it depending message_type flags ( MB_ICONASTERISK.... etc.) */
                                    NULL, /*icon url, use default, you can change it depending message_type flags */
                                    NULL, /*not used */
                                    NULL, /*localization of strings */
                                    header_ref, /*header text  */
                                    message_ref, /*message text */
                                    buttonText, /*default "ok" text in button */
                                    CFSTR("Cancel"), /*alternate button title */
                                    NULL, /*other button title, null--> no other button */
                                    &result /*response flags */ );

    /*Clean up the strings */
    CFRelease( header_ref );
    CFRelease( message_ref );

    /*Convert the result */
    if(result == kCFUserNotificationDefaultResponse)
        return kRetry;
    
#endif
    return kCancel;
}

static GLuint CompileShader(GLenum shaderType, const char* filename)
{
    char        buffer[1024*8]  = "#version 330\n#define OPENGL\n";
    char*       readPosition    = buffer+(sizeof("#version 330\n#define OPENGL\n")-1);
    GLchar      statusBuffer[1024] = {0};
    char        messageBoxBuffer[1024] = {0};
    GLint       status      = GL_TRUE;
    GLuint      shader      = glCreateShader(shaderType);
    int         result      = kOk;
    FILE*       shaderFile  = NULL;
    size_t      bytesRead   = 0;
    const char* shaderSource = buffer;

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
            result = RetryMessageBox("Shader Error", messageBoxBuffer);
        }
        else
            break;
    } while(result == kRetry);
    CheckGLError();
    
    return shader;
}

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
/* Graphics device managment */
void gfxDestroy(graphics_t* device)
{
#ifdef _WIN32
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(device->rc);
#endif
    free(device);
}
gfx_api_e gfxGetApi(void)
{
    return kOpenGL;
}

/* Device contols */
void gfxClear(graphics_t* device)
{
    UNUSED_PARAMETER(device);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CheckGLError();
}

/* Pipeline management */
void gfxSetClearColor(graphics_t* device, float r, float g, float b, float a, float depth)
{
    UNUSED_PARAMETER(device);
    glClearColor(r,g,b,a);
    CheckGLError();
    glClearDepth(depth);
    CheckGLError();
}
void gfxSetDepthTest(graphics_t* device, int test, int write)
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
    UNUSED_PARAMETER(device);
}
void gfxSetAlphaTest(graphics_t* device, int enable)
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
    UNUSED_PARAMETER(device);
}

/* Object creation */
vertex_shader_t* gfxCreateVertexShader(graphics_t* device, const char* filename)
{
    vertex_shader_t*    shader       = NULL;
    GLuint              vertexShader = 0;

    vertexShader = CompileShader(GL_VERTEX_SHADER, filename);

    /* Create new shader */
    gfx_malloc_and_zero(shader, vertex_shader_t*);
    shader->shader = vertexShader;

    return shader;
    UNUSED_PARAMETER(device);
}
pixel_shader_t* gfxCreatePixelShader(graphics_t* device, const char* filename)
{
    pixel_shader_t*     shader       = NULL;
    GLuint              pixelShader = 0;
    
    pixelShader = CompileShader(GL_FRAGMENT_SHADER, filename);

    /* Create new shader */
    gfx_malloc_and_zero(shader, pixel_shader_t*);
    shader->shader = pixelShader;

    return shader;
    UNUSED_PARAMETER(device);
}
material_t* gfxCreateMaterial(graphics_t* device, vertex_shader_t* vertexShader, pixel_shader_t* pixelShader)
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
    gfx_malloc_and_zero(material, material_t*);
    material->program = program;
    
    return material;
    UNUSED_PARAMETER(device);
}
mesh_t* gfxCreateMesh(graphics_t* device,
                     vertex_shader_t* vertexShader,
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

    while(layout && layout->slot != kGfxShaderInputNull)
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
    gfx_malloc_and_zero(mesh, mesh_t*);
    mesh->vertexBuffer  = buffers[0];
    mesh->indexBuffer   = buffers[1];
    mesh->inputLayout   = vao;
    mesh->indexSize     = (uint32_t)indexSize;
    mesh->indexCount    = indexCount;

    return mesh;
    UNUSED_PARAMETER(device);
    UNUSED_PARAMETER(vertexShader);
}
texture_t* gfxCreateTexture(graphics_t* device, const char* filename)
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
    gfx_malloc_and_zero(texture, texture_t*);
    texture->texture = textureId;

    return texture;
    UNUSED_PARAMETER(device);
}
constant_buffer_t* gfxCreateConstantBuffer(graphics_t* device, size_t size, const void* data)
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
    gfx_malloc_and_zero(constantBuffer, constant_buffer_t*);
    constantBuffer->buffer = buffer;

    return constantBuffer;
    UNUSED_PARAMETER(device);
}

/* object controls */
void gfxUpdateConstantBuffer(graphics_t* device, constant_buffer_t* buffer, size_t size, const void* data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer->buffer);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    UNUSED_PARAMETER(device);
}
void gfxBindConstantBufferToIndex(graphics_t* device, material_t* material, const char* bufferName, int index)
{
    GLuint bufferIndex = glGetUniformBlockIndex(material->program, bufferName);
    CheckGLError();
    glUniformBlockBinding(material->program, bufferIndex, index);
    CheckGLError();
    UNUSED_PARAMETER(device);
}

/* Object destruction */
void gfxDestroyVertexShader(vertex_shader_t* shader)
{
    glUseProgram(0);
    glDeleteShader(shader->shader);
    free(shader);
}
void gfxDestroyPixelShader(pixel_shader_t* shader)
{
    glUseProgram(0);
    glDeleteShader(shader->shader);
    free(shader);
}
void gfxDestroyMaterial(material_t* material)
{
    glUseProgram(0);
    glDeleteProgram(material->program);
    free(material);
}
void gfxDestroyMesh(mesh_t* mesh)
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &mesh->inputLayout);
    glDeleteBuffers(1, &mesh->vertexBuffer);
    glDeleteBuffers(1, &mesh->indexBuffer);
    free(mesh);
}
void gfxDestroyTexture(texture_t* texture)
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture->texture);
    free(texture);
}
void gfxDestroyConstantBuffer(constant_buffer_t* buffer)
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glDeleteBuffers(1, &buffer->buffer);
    free(buffer);
}

/* Render controls */
void gfxSetMaterial(graphics_t* device, material_t* material)
{
    glUseProgram(material->program);
    CheckGLError();
    UNUSED_PARAMETER(device);
}
void gfxSetTexture(graphics_t* device, texture_t* texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->texture);
    UNUSED_PARAMETER(device);
}
void gfxSetVSConstantBuffer(graphics_t* device, constant_buffer_t* buffer, uint32_t index)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer->buffer);
    UNUSED_PARAMETER(device);
}
void gfxSetPSConstantBuffer(graphics_t* device, constant_buffer_t* buffer, uint32_t index)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer->buffer);
    UNUSED_PARAMETER(device);
}
void gfxDrawMesh(graphics_t* device, mesh_t* mesh)
{
    glBindVertexArray(mesh->inputLayout);
    glDrawElements(GL_TRIANGLES, (GLsizei)mesh->indexCount, (mesh->indexSize == 2) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0);
    UNUSED_PARAMETER(device);
}


#endif /* #if (GFX_API == GFX_OPENGL) */
