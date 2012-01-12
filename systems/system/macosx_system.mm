/*
 *  macosx_System.cpp
 *  SystemLib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#include "System.h"

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

#include <memory.h>
#include <string.h>
#import  <Foundation/Foundation.h>

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
void    DefaultCallback(void) {}
void    DefaultResizeCallback(int,int){}
char    s_className[256] = {0};
char    s_keyState[System::kMAX_KEYS] = {0};

System::void_callback_t* s_frameCallback     = nullptr;
System::void_callback_t* s_shutdownCallback  = nullptr;
System::resize_callback_t* s_resizeCallback  = nullptr;

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
//    else if(key >= VK_F1 && key <= VK_F12)
//    {
//        int keyBase = (key - VK_F1) + kKeyF1;
//        key_e keyIndex = (key_e)keyBase;
//        s_keyState[keyIndex] = state;
//    }
    else
    {
        switch(key)
        {
        default:
            int x = 0;
            x++;
            return;
        }
    }
}
void SetMouseState(unsigned int button)
{
    s_mouseState = 0;

}
void MouseMove(int x, int y)
{
    s_mouseX = x;
    s_mouseY = y;
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
    // Initialize Mac stuff
    //

    //
    // Other initialization
    //
    RestoreDirectory();
    s_shutdown = 0;
    s_frameCallback     = &DefaultCallback;
    s_shutdownCallback  = &DefaultCallback;
    s_resizeCallback    = &DefaultResizeCallback;
}


void Shutdown(void)
{
    s_className[0]   = 0;
    s_shutdown       = 1;
    
    s_frameCallback     = nullptr;
    s_shutdownCallback  = nullptr;
    s_resizeCallback    = nullptr;
}
void* GetMainWindow(void)
{
}


void SetFrameCallback(void_callback_t* callback) { s_frameCallback = callback; }
void SetShutdownCallback(void_callback_t* callback) { s_shutdownCallback = callback; }
void SetResizeCallback(resize_callback_t* callback) { s_resizeCallback = callback; }


void RunMainLoop(void)
{
    
    s_shutdownCallback();
    s_shutdown = 0;
}

void SpawnWindow(int width, int height, int fullscreen)
{
}

void SetWindowTitle(const char* title)
{
}

void GetExecutableDirectory(size_t bufferSize, char* directory)
{
    NSString* bundle_path = [[NSBundle mainBundle] bundlePath];
    const char* executable_path = [[bundle_path stringByDeletingLastPathComponent] UTF8String];
    strcpy(directory, executable_path);
}

message_box_e MessageBox(const char* title, const char* message, message_box_type_e type)
{
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
