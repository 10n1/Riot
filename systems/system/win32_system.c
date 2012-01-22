/*
 *  win32_system.c
 *  graphicsDevice
 *
 *  Created by Kyle C Weicht on 1/16/12.
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#include "system.h"
//#include "global.h"

/* C headers */
#include <stdint.h>
#include <stdlib.h>
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
/* C++ headers */
/* External headers */
/* Internal headers */

#define UNUSED_PARAMETER(param) (void)(sizeof((param)))
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

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
static const char kClassName[] = "Class";
struct system_t
{
    uint8_t                 keyState[kMAX_KEYS];
    HWND                    hwnd;
    HINSTANCE               hinstance;
    resize_callback_t*      resizeCallback;
    keyboard_callback_t*    keyboardCallback;
    uint32_t                flags;
};

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/
static void SetKeyState(system_t* system, uint8_t key, uint8_t state)
{
    sys_key_e   keyIndex;
    int         keyBase;
    if(key >= 'A' && key <= 'Z')
    {
        keyBase = (key - 'A') + kSysKeyA;
        keyIndex = (sys_key_e)keyBase;
    }
    else if(key >= '0' && key <= '9')
    {
        keyBase = (key - '0') + kSysKey0;
        keyIndex = (sys_key_e)keyBase;
    }
    else if(key >= VK_F1 && key <= VK_F12)
    {
        keyBase = (key - VK_F1) + kSysKeyF1;
        keyIndex = (sys_key_e)keyBase;
    }
    else
    {
        switch(key)
        {
        case VK_ESCAPE:
            keyIndex = kSysKeyEscape; break;

        case VK_SPACE:  
            keyIndex = kSysKeySpace; break;

        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            keyIndex = kSysKeyShift; break;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            keyIndex = kSysKeyCtrl; break;
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            keyIndex = kSysKeyAlt; break;

        case VK_LEFT:
            keyIndex = kSysKeyLeft; break;
        case VK_RIGHT:
            keyIndex = kSysKeyRight; break;
        case VK_UP:
            keyIndex = kSysKeyUp; break;
        case VK_DOWN:
            keyIndex = kSysKeyDown; break;

        default:
            return;
        }
    }

    system->keyState[keyIndex] = state;
    if(state && system->keyboardCallback)
    {
        system->keyboardCallback(keyIndex);
    }
}
static LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    static system_t*    s_system = NULL;
    CREATESTRUCT*       createStruct;
    int                 width;
    int                 height;
    switch (message) 
    {
    case WM_CREATE:
        createStruct = ((CREATESTRUCT*)lParam);
        s_system = createStruct->lpCreateParams;
        return 0;
    /* Keyboard input */
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        SetKeyState(s_system, (uint8_t)wParam, 1);
        // Handle Alt-F4 here. TODO: Support disabling it
        if(GetKeyState(kSysKeyF4) && GetKeyState(kSysKeyAlt))
            PostQuitMessage(0);
        return 0;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        SetKeyState(s_system, (uint8_t)wParam, 0);
        // Handle Alt-F4 here. TODO: Support disabling it
        if(GetKeyState(kSysKeyF4) && GetKeyState(kSysKeyAlt))
            PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        if(s_system->resizeCallback)
        {
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            s_system->resizeCallback(width, height);
        }
        return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
static void TurnFlagOn(uint32_t* flags, uint32_t flag)
{
    *flags |= flag;
}
static void TurnFlagOff(uint32_t* flags, uint32_t flag)
{
    *flags &= ~flag;
} 

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
system_t* sysCreate(void)
{
    system_t*   system = NULL;    
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
    
    /*
     * Create system object
     */
    system = malloc(sizeof(system_t));
    memset(system, 0, sizeof(*system));
    
    system->hinstance = hinstance;
    system->flags |= kSysRunning;

    return system;
}
void sysShutdown(system_t* system)
{
    system->resizeCallback = NULL;
    CloseWindow(system->hwnd);
    UnregisterClass(kClassName, system->hinstance);
    free(system);
}
void sysCreateWindow(system_t* system, int width, int height)
{
    UINT    uiStyle = WS_OVERLAPPEDWINDOW;
    RECT    rc      = { 0, 0, width, height };
    HWND    hwnd    = NULL;

    AdjustWindowRect(&rc, uiStyle, FALSE);
    hwnd = CreateWindow(kClassName, 
                        "Window", 
                        uiStyle,
                        0, 0, 
                        rc.right - rc.left, 
                        rc.bottom - rc.top, 
                        NULL, NULL, 
                        system->hinstance, system);
    assert(hwnd);
    ShowWindow(hwnd, SW_SHOWNORMAL);

    assert(system);
    system->hwnd = hwnd;
}
void sysPollSystemEvents(system_t* system)
{
    MSG msg = {0};
    while(PeekMessage(&msg, NULL, 0, 0,PM_REMOVE))
	{
		TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if(msg.message == WM_QUIT)
    {   
        TurnFlagOff(&system->flags, kSysRunning);
    }
}
void sysSetResizeCallback(system_t* system, resize_callback_t* callback)
{
    system->resizeCallback = callback;
}
void sysSetKeyboardCallback(system_t* system, keyboard_callback_t* callback)
{
    system->keyboardCallback = callback;
}
void* sysGetWindow(system_t* system)
{
    return system->hwnd;
}
int sysGetFlag(system_t* system, sys_flag_e flag)
{
    return system->flags & flag;
}
void sysSetFlag(system_t* system, sys_flag_e flag, int value)
{
    if(value)
        system->flags |= flag;
    else
        system->flags &= ~flag;
}
int sysGetKeyState(system_t* system, sys_key_e key)
{
    return system->keyState[key];
}
sys_mb_return_e sysMessageBox(  system_t* system, 
                                const char* header, 
                                const char* message, 
                                sys_mb_type_e type)
{
    int messageBoxResult = MessageBox(NULL, message, header, (type == kRetryCancel) ? MB_RETRYCANCEL : MB_OKCANCEL);

    switch(messageBoxResult)
    {
    case IDRETRY:   return kRetry;
    case IDCANCEL:  return kCancel;
    case IDOK:      return kOk;
    }
    return (sys_mb_return_e)-1;
    UNUSED_PARAMETER(system);
}
