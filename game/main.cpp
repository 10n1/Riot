/*
 * main.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
#include <stdio.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"
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
int main(int, char*[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_16_DF );
#endif

    System::Init(1, 1024, 768);

    GraphicsDevice* device = GraphicsDevice::Create(GraphicsAPI::kOpenGL, System::GetWindow());
    
    device->SetClearColor(0.67f, 0.23f, 0.15f, 1.0f, 0.0f);
    while(System::PollEvents())
    {
        device->Clear();
        device->Present();

        if(System::GetKeyState(System::Key::kEscape))
            System::Stop();
    }

    device->Destroy();
    System::Shutdown();

    return 0;
}
