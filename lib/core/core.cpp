/*
 * core.cpp
 * PROJECTNAME
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "core.h"

/* C headers */
#include <string.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "system.h"
#include "renderEngine.h"
#include "camera.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
static mesh_id_t    _quadMesh = -1;
static mesh_id_t    _cubeMesh = -1;
static texture_id_t _cubeTexture = -1;
static texture_id_t _grassTexture = -1;
static camera_t     _camera;

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
void Core::Init(const engine_params_t& params)
{
    /* Zero out self */
    memset(this, 0, sizeof(*this));

    /* Initialize subsystems */
    System::Init(params.createWindow, params.windowWidth, params.windowHeight);

    render_engine_params_t renderParams;
    renderParams.graphicsApi = params.graphicsApi;
    RenderEngine::Init(renderParams);

    /* Engine initialization */
    _frameNumber = 0;

    _quadMesh = RenderEngine::CreateMesh("assets/quadMesh.json");
    _cubeMesh = RenderEngine::CreateMesh("assets/cubeMesh.json");
    _grassTexture = RenderEngine::CreateTexture("assets/grass.png");
    _cubeTexture = RenderEngine::CreateTexture("assets/test.png");

    camInit(&_camera);

    _camera.position.y += 0.25f;
}

int Core::Frame(void)
{
    /*
     * System stuff first
     */
    if(System::PollEvents() == 0)
        return 1;
    
    RenderEngine::SetWorldProjectionType(ProjectionType::kPerspective);
    int width, height;
    System::GetWindowSize(&width, &height);
    if(width != _windowWidth || height != _windowHeight)
    {
        RenderEngine::Resize(width, height);
        _windowWidth = width, _windowHeight = height;
    }

    if(System::GetKeyState(System::Key::kEscape))
        return 1;

    // Camera controls
    if(System::GetKeyState(System::Key::kUp))
        camRotateX(&_camera, -0.001f);
    if(System::GetKeyState(System::Key::kDown))
        camRotateX(&_camera, +0.001f);
    if(System::GetKeyState(System::Key::kLeft))
        camRotateY(&_camera, -0.001f);
    if(System::GetKeyState(System::Key::kRight))
        camRotateY(&_camera, +0.001f);

    if(System::GetKeyState(System::Key::kW))
        camTranslateZ(&_camera, 0.005f);
    if(System::GetKeyState(System::Key::kS))
        camTranslateZ(&_camera, -0.005f);
        
    if(System::GetKeyState(System::Key::kA))
        camTranslateX(&_camera, -0.005f);
    if(System::GetKeyState(System::Key::kD))
        camTranslateX(&_camera, +0.005f);
        
    if(System::GetKeyState(System::Key::kSpace))
        camTranslateY(&_camera, +0.005f);
    if(System::GetKeyState(System::Key::kC))
        camTranslateY(&_camera, -0.005f);

    RenderEngine::SetWorldViewMatrix(camGetViewMatrix(&_camera));

    // Ground
    Matrix4 worldMatrix = Matrix4RotationX(DegToRad(90.0f));
    worldMatrix = Matrix4MatrixMultiply(worldMatrix, Matrix4Scale(500.0f, 500.0f, 500.0f));
    RenderEngine::Render(1, worldMatrix, _quadMesh, _grassTexture);
    RenderEngine::Render(1, Matrix4Identity(), _cubeMesh, _cubeTexture);

    RenderEngine::Frame();

    _frameNumber++;
    return 0;
}

void Core::Shutdown(void)
{
    /* Destroy subsystems */
    RenderEngine::Shutdown();
    System::Shutdown();
}
