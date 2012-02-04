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
#include "graphicsDevice.h"

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
    _graphicsDevice = GraphicsDevice::Create(params.graphicsApi, System::GetWindow());

    /* Engine initialization */
    _frameNumber = 0;
    _graphicsDevice->SetClearColor(0.67f, 0.23f, 0.15f, 1.0f, 0.0f);
    _graphicsDevice->Clear();
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
        _graphicsDevice->Resize(width, height);
        _windowWidth = width, _windowHeight = height;
    }

    if(System::GetKeyState(System::Key::kEscape))
        return 1;

    // Rendering
    _graphicsDevice->Present();
    _graphicsDevice->Clear();

    _frameNumber++;
    return 0;
}

void Core::Shutdown(void)
{
    /* Destroy subsystems */
    _graphicsDevice->Destroy();
    System::Shutdown();
}
