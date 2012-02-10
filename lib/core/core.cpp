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
#include "entity.h"
#include "component.h"
#include "Box2D/Box2D.h"

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

    camInit(&_camera);

    /* Initialize components */
    PhysicsComponent::Initialize();
}

int Core::Frame(void)
{
    /*
     * System stuff first
     */
    if(System::PollEvents() == 0)
        return 1;
    
    //RenderEngine::SetWorldProjectionType(ProjectionType::kOrthographic, -8.0f, 120.0f);
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
        camTranslateZ(&_camera, 0.05f);
    if(System::GetKeyState(System::Key::kS))
        camTranslateZ(&_camera, -0.05f);
        
    if(System::GetKeyState(System::Key::kA))
        camTranslateX(&_camera, -0.05f);
    if(System::GetKeyState(System::Key::kD))
        camTranslateX(&_camera, +0.05f);
        
    if(System::GetKeyState(System::Key::kSpace))
        camTranslateY(&_camera, +0.05f);
    if(System::GetKeyState(System::Key::kC))
        camTranslateY(&_camera, -0.05f);

    //RenderEngine::SetWorldViewMatrix(camGetViewMatrix(&_camera));

    // Ground
    RenderEngine::Frame();

    _frameNumber++;
    return 0;
}

void Core::Shutdown(void)
{
    delete _box2d;
    /* Destroy subsystems */
    RenderEngine::Shutdown();
    System::Shutdown();
}
