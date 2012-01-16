/*
 *  win32_System.cpp
 *  SystemLib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#include "System.h"

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <ctime>


/*
 * Define debugBreak
 */
#if defined( _MSC_VER )
    #define debugBreak() __debugbreak()
#elif defined( __GNUC__ )
    #define debugBreak() __asm__( "int $3\n" : : )
#else
    #error Unsupported compiler
#endif

/*
 * Define assert
 */
#ifndef assert
    #define assert(condition)   \
        do                      \
        {                       \
            if(!(condition))    \
            {                   \
                debugBreak();   \
            }                   \
        } while(__LINE__ == -1)
        /* This causes warning 4127 with VC++ (conditional expression is constant) */
    #if defined( _MSC_VER )
        #pragma warning(disable:4127)
    #endif /* defined( _MSC_VER ) */
#endif

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace
{

using namespace System;
/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
void DefaultCallback(void) {}
void DefaultResizeCallback(int,int){}
void DefaultMouseClickCallback(mouse_button_e,int,int){}
char    s_className[256] = {0};
char    s_keyState[System::kMAX_KEYS] = {0};

HINSTANCE           s_systemInstance    = nullptr;
HWND                s_hWnd              = nullptr;
System::void_callback_t* s_frameCallback     = nullptr;
System::void_callback_t* s_shutdownCallback  = nullptr;
System::resize_callback_t* s_resizeCallback  = nullptr;
System::mouse_click_callback_t* s_mouseClickCallback = nullptr;

int s_shutdown   = 0;
int s_mouseX     = 0;
int s_mouseY     = 0;
mouse_state_t s_mouseState = 0;

/*******************************************************************\
 Internal functions
\*******************************************************************/
void SetKeyState(char key, char state)
{
    if(key >= 'A' && key <= 'Z')
    {
        int keyBase = (key - 'A') + kKeyA;
        key_e keyIndex = (key_e)keyBase;
        s_keyState[keyIndex] = state;
    }
    else if(key >= '0' && key <= '9')
    {
        int keyBase = (key - '0') + kKey0;
        key_e keyIndex = (key_e)keyBase;
        s_keyState[keyIndex] = state;
    }
    else if(key >= VK_F1 && key <= VK_F12)
    {
        int keyBase = (key - VK_F1) + kKeyF1;
        key_e keyIndex = (key_e)keyBase;
        s_keyState[keyIndex] = state;
    }
    else
    {
        switch(key)
        {
        case VK_ESCAPE:
            s_keyState[kKeyEscape] = state; break;

        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            s_keyState[kKeyShift] = state; break;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            s_keyState[kKeyCtrl] = state; break;
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            s_keyState[kKeyAlt] = state; break;

        case VK_LEFT:
            s_keyState[kKeyLeft] = state; break;
        case VK_RIGHT:
            s_keyState[kKeyRight] = state; break;
        case VK_UP:
            s_keyState[kKeyUp] = state; break;
        case VK_DOWN:
            s_keyState[kKeyDown] = state; break;

        default:
            int x = 0;
            x++;
            return;
        }
    }
}
void SetMouseState(unsigned int button)
{
    if(button & MK_LBUTTON && (s_mouseState & kMouseButtonLeft) == 0)
        s_mouseClickCallback(kMouseButtonLeft, s_mouseX, s_mouseY);
    if(button & MK_RBUTTON && (s_mouseState & kMouseButtonRight) == 0)
        s_mouseClickCallback(kMouseButtonRight, s_mouseX, s_mouseY);
    if(button & MK_MBUTTON && (s_mouseState & kMouseButtonMiddle) == 0)
        s_mouseClickCallback(kMouseButtonMiddle, s_mouseX, s_mouseY);
    
    s_mouseState = 0;
    if(button & MK_LBUTTON)
        s_mouseState |= kMouseButtonLeft;
    if(button & MK_RBUTTON)
        s_mouseState |= kMouseButtonRight;
    if(button & MK_MBUTTON)
        s_mouseState |= kMouseButtonMiddle;
}
void MouseMove(int x, int y)
{
    RECT clientRect;
    GetClientRect(s_hWnd, &clientRect);

    //int width = clientRect.right - clientRect.left;
    int height = clientRect.bottom - clientRect.top;

    s_mouseX = x;
    s_mouseY = height-y;
}

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch (message) 
    {
    // Keyboard input
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        SetKeyState(char(wParam), 1);
        // Handle Alt-F4 here. TODO: Support disabling it
        if(GetKeyState(kKeyF4) && GetKeyState(kKeyAlt))
            PostQuitMessage(0);
        return 0;
    // Keyboard input
    case WM_KEYUP:
    case WM_SYSKEYUP:
        SetKeyState(char(wParam), 0);
        // Handle Alt-F4 here. TODO: Support disabling it
        if(GetKeyState(kKeyF4) && GetKeyState(kKeyAlt))
            PostQuitMessage(0);
        return 0;

    // Mouse input
    case WM_MOUSEMOVE:
        {
            POINTS pos = MAKEPOINTS(lParam);
            MouseMove(pos.x, pos.y);
        }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
        SetMouseState((int)wParam);
        return 0;

    case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            s_resizeCallback(width, height);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;


    case WM_CAPTURECHANGED:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

} // namespace

namespace System
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void)
{
    //
    // Initialize Windows stuff
    //
    assert(s_systemInstance == nullptr);
    s_systemInstance = GetModuleHandle(nullptr);

    /* Use the filename hashed with the time as the class name */
    GetModuleFileName(s_systemInstance, s_className, (DWORD)sizeof(s_className));

    intptr_t hasher = reinterpret_cast<intptr_t>(s_systemInstance);
    char* classNameHash = s_className + strlen(s_className);
    int i = 10;
    while(i)
    {
        *classNameHash = '0' + hasher%10;
        hasher /= 10;
        classNameHash++;
        i--;
    }
    classNameHash = 0;

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
    wcex.lpszClassName  = s_className;
    wcex.hIconSm        = nullptr;
    if( !RegisterClassEx(&wcex) )
    {
        assert(0);
    }

    //
    // Other initialization
    //
    RestoreDirectory();
    s_shutdown = 0;
    s_frameCallback     = &DefaultCallback;
    s_shutdownCallback  = &DefaultCallback;
    s_resizeCallback    = &DefaultResizeCallback;
    s_mouseClickCallback = &DefaultMouseClickCallback;
}


void Shutdown(void)
{
    BOOL result = 1;
    if(s_hWnd)
    {
        result = ::DestroyWindow(s_hWnd);
        assert(result);
    }
    result = ::UnregisterClass(s_className, s_systemInstance);
    assert(result != 0);

    s_systemInstance = nullptr;
    s_hWnd           = nullptr;
    s_className[0]   = 0;
    s_shutdown       = 1;
}
void* GetMainWindow(void)
{
    return s_hWnd;
}


void SetFrameCallback(void_callback_t* callback) { s_frameCallback = callback; }
void SetShutdownCallback(void_callback_t* callback) { s_shutdownCallback = callback; }
void SetResizeCallback(resize_callback_t* callback) { s_resizeCallback = callback; }
void SetMouseClickCallback(mouse_click_callback_t* callback) { s_mouseClickCallback = callback; }

void RunMainLoop(void)
{
    assert(s_systemInstance);
    MSG msg = {0};
	while(msg.message != WM_QUIT)
    {
        if(s_shutdown)
            break;

		while(PeekMessage( &msg, nullptr, 0, 0,PM_REMOVE ) )
		{
			TranslateMessage( &msg );
            DispatchMessage( &msg );
        }

        //
        // Frame
        //
        s_frameCallback();
    }
    s_shutdownCallback();
    s_shutdown = 0;
}

void SpawnWindow(int width, int height, int fullscreen)
{
    assert(s_hWnd == nullptr);

    UINT uiStyle,uiStyleX;
    if(fullscreen)
    {
        uiStyle = WS_POPUP;
        uiStyleX = WS_EX_TOPMOST;
        ShowCursor(false);
    }
    else
    {
        uiStyle = WS_OVERLAPPEDWINDOW;
        uiStyleX = 0;
    }
    
    DEVMODE	gDevMode;
    if(fullscreen)
    {
        gDevMode.dmPelsHeight   = width;
        gDevMode.dmPelsWidth    = height;
        gDevMode.dmSize         = sizeof(DEVMODE);
        gDevMode.dmFields       = DM_PELSWIDTH | DM_PELSHEIGHT;
        if(ChangeDisplaySettings(&gDevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
        {
            MessageBox( nullptr, 
                        TEXT("Cannot change to selected desktop resolution."),
                        nullptr, 
                        MB_OK | MB_ICONSTOP);
            assert(0);
        }
    }

    // Create window
    RECT rc = { 0, 0, width, height };
    AdjustWindowRect( &rc, uiStyle, FALSE );
    s_hWnd = CreateWindow(  s_className, 
                            "Window", 
                            uiStyle,
                            0, 0, 
                            rc.right - rc.left, 
                            rc.bottom - rc.top, 
                            nullptr, nullptr, 
                            s_systemInstance, nullptr);
    assert(s_hWnd);

    ShowWindow(s_hWnd, SW_SHOWNORMAL );

    // Get the mouse position
}

void SetWindowTitle(const char* title)
{
    ::SetWindowText(s_hWnd, title);
}

void GetExecutableDirectory(size_t bufferSize, char* directory)
{
    HINSTANCE exeInstance = GetModuleHandle(nullptr);
    GetModuleFileName(exeInstance, directory, (DWORD)bufferSize);

    // Strip off the executable name and extension
    int filenameLength = (int)strlen(directory);
    char* currChar = directory + filenameLength - 1;
    while(*currChar != '\\')
        currChar--;

    currChar++; // Move forward one so the backslash is part of the path
    *currChar = 0;
}

message_box_e MessageBox(const char* title, const char* message, message_box_type_e type)
{
    unsigned int mbType = MB_OKCANCEL;
    switch(type)
    {
    case kOkCancel:
        mbType = MB_OKCANCEL;
        break;
    case kRetryCancel:
        mbType = MB_RETRYCANCEL;
        break;
    default:
        assert(0);
        break; 
    }

    int result = ::MessageBox(NULL, message, title, mbType);

    switch(result)
    {
    case IDOK:
        return kOk;
    case IDCANCEL:
        return kCancel;
    case IDRETRY:
        return kRetry;
    default:
        assert(0);
        break;
    }

    return kCancel;
}


void GetMousePosition(int* x, int *y)
{
    *x = s_mouseX;
    *y = s_mouseY;
}
mouse_state_t GetMouseState(void)
{
    return s_mouseState;
}
void GetKeyboardState(char keys[kMAX_KEYS])
{
    memcpy(keys, s_keyState, sizeof(s_keyState));
}
int GetKeyState(key_e key)
{
    assert(key >= 0 && key < kMAX_KEYS);
    return s_keyState[key];
}

} // namespace System
