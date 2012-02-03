/*
 * system_win32.cpp
 * PROJECTNAME
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "system.h"

/* C headers */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
/* C++ headers */
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir

/* External headers */
/* Internal headers */
#include "global.h"

using namespace System;

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
uint8_t     s_keyState[Key::kMAX_KEYS] = {0};
HWND        s_hwnd      = NULL;
HINSTANCE   s_hinstance = NULL;
int         s_running   = 0;
int         s_mouseX    = 0;
int         s_mouseY    = 0;
int         s_mouseState = 0;

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
namespace System
{

void Init(int window, int windowWidth, int windowHeight)
{
    // Zero out variables
    memset(s_keyState, 0, sizeof(s_keyState));
    s_hwnd      = NULL;
    s_hinstance = NULL;

    // Set working directory
    ChangeWorkingDir(GetExecutableDir());

    // Set it running
    s_running = 1;
}
void Shutdown(void)
{
    // Zero out variables
    memset(s_keyState, 0, sizeof(s_keyState));
    s_hwnd      = NULL;
    s_hinstance = NULL;
    s_running   = 0;
}
int IsRunning(void)
{
    return s_running;
}
void Stop(void)
{
    s_running = 0;
}

// Window stuff
void* GetWindow(void)
{
    return s_hwnd;
}
void PollEvents(void)
{
    MSG msg = {0};
    while(PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
	{
		TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if(msg.message == WM_QUIT)
    {
        System::Stop();
    }
}
void GetWindowSize(int* width, int* height)
{
    if(s_hwnd == NULL)
    {
        *width = *height = 0;
        return;
    }

    RECT clientRect;
    GetClientRect(s_hwnd, &clientRect);
    *width = clientRect.right - clientRect.left;
    *height = clientRect.bottom - clientRect.top;
}

// Input support
int GetKeyState(Key::Enum key);
int GetMouseButtonState(Mouse::Enum button)
{
    if(s_hwnd == NULL)
        return 0;

    return s_mouseState & button;
}
int GetMousePosition(int* x, int* y)
{
    if(s_hwnd == NULL)
    {
        *x = *y = 0;
        return;
    }

    *x = s_mouseX;
    *y = s_mouseY;
}

// Utility stuff
void ChangeWorkingDir(const char* dir)
{
    chdir(dir);
}
const char* GetWorkingDir(void)
{
    static char workingDirectory[MAX_PATH];
    return getcwd(workingDirectory, sizeof(workingDirectory));
}
const char* GetExecutableDir(void)
{
    static char filename[256] = {0};
    char* end = NULL;
    GetModuleFileName(NULL, filename, sizeof(filename));

    end = &filename[strlen(filename)];
    while(end && *end != '\\' && *end != '/' )
    {
        --end;
    }

    ++end; /* Increment this so the trailing slash is part of the path */
    *end = 0;

    return filename;
}

// Misc
MessageBoxResult::Enum MessageBox(  const char* header, 
                                    const char* message, 
                                    MessageBoxType::Enum type);

}
