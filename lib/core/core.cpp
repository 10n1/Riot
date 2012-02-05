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

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
static mesh_id_t    _mesh = -1;
static texture_id_t _texture = -1;

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

    _mesh = RenderEngine::CreateMesh("assets/quadMesh.json");
    _texture = RenderEngine::CreateTexture("assets/ground.png");
}

int Core::Frame(void)
{
    /*
     * System stuff first
     */
    if(System::PollEvents() == 0)
        return 1;
    
    int width, height;
    System::GetWindowSize(&width, &height);
    if(width != _windowWidth || height != _windowHeight)
    {
        RenderEngine::Resize(width, height);
        _windowWidth = width, _windowHeight = height;
    }

    if(System::GetKeyState(System::Key::kEscape))
        return 1;

    RenderEngine::SetWorldProjectionType(ProjectionType::kOrthographic);
    RenderEngine::SetWorldViewMatrix(Matrix4Identity());
    RenderEngine::Render(1, Matrix4Identity(), _mesh, _texture);

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
