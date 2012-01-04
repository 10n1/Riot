/*
 *  application_win32.cpp
 *  lib
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

#include "application.h"
#include "build.h"

#include <stdio.h>
#include <Windows.h>

#include "assert.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
static const char   kClassName[] = "Main Game Class";
static const char   kGameName[] = "Game";

void DefaultFrameCallback(void) { }

/*******************************************************************\
 Internal variables
\*******************************************************************/
char                s_executableDir[FILENAME_MAX] = {0};
HINSTANCE           s_systemInstance    = nullptr;
frame_callback_t*   s_frameCallback     = &DefaultFrameCallback;

application_status_e    s_status    = kNotStarted;
int                     s_shutdown  = 0;

/*******************************************************************\
 Internal functions
\*******************************************************************/
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message) 
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

} // Anonymous namespace

namespace Application
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void)
{
    s_systemInstance = GetModuleHandle(nullptr);
    /* Register the class */
    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX); 
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = MainWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = s_systemInstance;
    wcex.hIcon          = nullptr;
    wcex.hCursor        = (HCURSOR)LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = kClassName;
    wcex.hIconSm        = nullptr;
    if( !RegisterClassEx(&wcex) )
    {
        assert(0);
    }

    s_status = kStarted;
}
void StartMainLoop(void)
{
    s_status = kRunning;
    MSG msg = {0};
	while(msg.message != WM_QUIT)
    {
        if(s_shutdown)
            break;

		if(PeekMessage( &msg, nullptr, 0, 0,PM_REMOVE ) )
		{
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

        //
        // Frame
        //
        s_frameCallback();
    }

    //
    // Shutdown
    //
    s_status = kStopped;
}
void Shutdown(void)
{
    s_shutdown = 1;
}
void SetFrameCallback(frame_callback_t* callback)
{
    s_frameCallback = callback;
}

application_status_e GetStatus(void)
{
    return s_status;
}

const char* GetExecutableDirectory(void)
{
    if(s_executableDir[0] != 0)
        return s_executableDir;
    
    GetModuleFileName(s_systemInstance, s_executableDir, sizeof(s_executableDir));

    // Strip off the executable name and extension
    int filenameLength = strlen(s_executableDir);
    char* currChar = s_executableDir + filenameLength - 1;
    while(*currChar != '\\')
        currChar--;

    currChar++; // Move forward one so the backslash is part of the path
    *currChar = 0;
    return s_executableDir;
}

void* GetOSApplication(void)
{
    return s_systemInstance;
}

} // namespace Application
