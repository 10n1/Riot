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

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
void DefaultCallback(void) {}
System::callback_t* s_frameCallback     = &DefaultCallback;
System::callback_t* s_shutdownCallback  = &DefaultCallback;

HINSTANCE   s_systemInstance = nullptr;
HWND        s_hWnd           = nullptr;
char        s_className[256] = {0};
int         s_shutdown       = 0;

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
}


void Shutdown(void)
{
    System::CloseWindow();
    BOOL result = ::UnregisterClass(s_className, s_systemInstance);
    assert(result != 0);

    s_systemInstance = nullptr;
    s_hWnd           = nullptr;
    s_className[0]   = 0;
    s_frameCallback  = &DefaultCallback;
    s_shutdownCallback  = &DefaultCallback;
    s_shutdown       = 1;
}
void* GetMainWindow(void)
{
    return s_hWnd;
}


void SetFrameCallback(callback_t* callback)
{
    s_frameCallback = callback;
}
void SetShutdownCallback(callback_t* callback)
{
    s_shutdownCallback = callback;
}


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
    System::Shutdown();
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
}

void CloseWindow(void)
{
    ::DestroyWindow(s_hWnd);
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

} // namespace System
