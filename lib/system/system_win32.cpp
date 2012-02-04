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
static const char kClassName[] = "Class";

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
void SetMouseState(unsigned int button)
{
    s_mouseState = 0;
    if(button & MK_LBUTTON)
        s_mouseState |= Mouse::kLeft;
    if(button & MK_RBUTTON)
        s_mouseState |= Mouse::kRight;
    if(button & MK_MBUTTON)
        s_mouseState |= Mouse::kMiddle;
}

static void SetKeyState(uint8_t key, uint8_t state)
{
    Key::Enum   keyIndex;
    int         keyBase;
    if(key >= 'A' && key <= 'Z')
    {
        keyBase = (key - 'A') + Key::kA;
        keyIndex = (Key::Enum)keyBase;
    }
    else if(key >= '0' && key <= '9')
    {
        keyBase = (key - '0') + Key::k0;
        keyIndex = (Key::Enum)keyBase;
    }
    else if(key >= VK_F1 && key <= VK_F12)
    {
        keyBase = (key - VK_F1) + Key::kF1;
        keyIndex = (Key::Enum)keyBase;
    }
    else
    {
        switch(key)
        {
        case VK_ESCAPE:
            keyIndex = Key::kEscape; break;

        case VK_SPACE:  
            keyIndex = Key::kSpace; break;

        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            keyIndex = Key::kShift; break;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            keyIndex = Key::kCtrl; break;
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            keyIndex = Key::kAlt; break;

        case VK_LEFT:
            keyIndex = Key::kLeft; break;
        case VK_RIGHT:
            keyIndex = Key::kRight; break;
        case VK_UP:
            keyIndex = Key::kUp; break;
        case VK_DOWN:
            keyIndex = Key::kDown; break;

        default:
            return;
        }
    }

    s_keyState[keyIndex] = state;
}

static LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message) 
    {
    /* Keyboard input */
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        SetKeyState((uint8_t)wParam, 1);
        // Handle Alt-F4 here. TODO: Support disabling it
        if(GetKeyState(Key::kF4) && GetKeyState(Key::kAlt))
            PostQuitMessage(0);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        SetKeyState((uint8_t)wParam, 0);
        // Handle Alt-F4 here. TODO: Support disabling it
        if(GetKeyState(Key::kF4) && GetKeyState(Key::kAlt))
            PostQuitMessage(0);
        return 0;
    
    /* Mouse input */
    case WM_MOUSEMOVE:
        {
            POINTS pos = MAKEPOINTS(lParam);
            s_mouseX = pos.x;
            s_mouseY = pos.y;
        }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        SetMouseState((int)wParam);
        return 0;
    /* Window close */
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

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
    s_running   = 0;

    s_hinstance = GetModuleHandle(NULL);

    // Set working directory
    ChangeWorkingDir(GetExecutableDir());

    // Set it running
    s_running = 1;

    if(window == 0)
        return;

    // Now create the window
    WNDCLASSEX  wcex;
    HINSTANCE   hinstance   = GetModuleHandle(NULL);

    /* Register the class */
    wcex.cbSize         = sizeof(WNDCLASSEX); 
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = MainWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hinstance;
    wcex.hIcon          = NULL;
    wcex.hCursor        = (HCURSOR)LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = kClassName;
    wcex.hIconSm        = NULL;
    if( !RegisterClassEx(&wcex) )
    {
        assert(0);
    }

    UINT    uiStyle = WS_OVERLAPPEDWINDOW;
    RECT    rc      = { 0, 0, windowWidth, windowHeight };
    HWND    hwnd    = NULL;

    AdjustWindowRect(&rc, uiStyle, FALSE);
    hwnd = CreateWindow(kClassName, 
                        "Window", 
                        uiStyle,
                        0, 0, 
                        rc.right - rc.left, 
                        rc.bottom - rc.top, 
                        NULL, NULL, 
                        s_hinstance, system);
    assert(hwnd);
    ShowWindow(hwnd, SW_SHOWNORMAL);

    s_hwnd = hwnd;

}
void Shutdown(void)
{
    CloseWindow(s_hwnd);
    UnregisterClass(kClassName, s_hinstance);

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
int PollEvents(void)
{
    MSG msg = {0};
    while(PeekMessage(&msg, NULL, 0, 0,PM_NOREMOVE))
	{
        if(!GetMessage(&msg, NULL, 0, 0))
        {
            System::Stop();
        }
		TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return s_running;
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
int GetKeyState(Key::Enum key)
{
    if(s_hwnd == NULL)
        return 0;

    return s_keyState[key];
}
int GetMouseButtonState(Mouse::Enum button)
{
    if(s_hwnd == NULL)
        return 0;

    return s_mouseState & button;
}
void GetMousePosition(int* x, int* y)
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
                                    MessageBoxType::Enum type)
{
    int messageBoxResult = ::MessageBox(s_hwnd, message, header, (type == MessageBoxType::kRetryCancel) ? MB_RETRYCANCEL : MB_OKCANCEL);

    switch(messageBoxResult)
    {
    case IDRETRY:   return MessageBoxResult::kRetry;
    case IDCANCEL:  return MessageBoxResult::kCancel;
    case IDOK:      return MessageBoxResult::kOk;
    }
    return (MessageBoxResult::Enum)-1;
}


}
