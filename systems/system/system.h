/*
 *  System.h
 *  SystemLib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __SystemLib_h__
#define __SystemLib_h__

//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace System
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
typedef void (void_callback_t)(void);
typedef void (resize_callback_t)(int,int);

enum message_box_e
{
    kOk,
    kCancel,
    kRetry,
};
enum message_box_type_e
{
    kOkCancel,
    kRetryCancel,
};
enum mouse_button_e
{
    kMouseButtonLeft    = 0x1,
    kMouseButtonRight   = 0x2,
    kMouseButtonMiddle  = 0x4,
};

enum key_e
{
    kKey0,
    kKey1,
    kKey2,
    kKey3,
    kKey4,
    kKey5,
    kKey6,
    kKey7,
    kKey8,
    kKey9,

    kKeyA,
    kKeyB,
    kKeyC,
    kKeyD,
    kKeyE,
    kKeyF,
    kKeyG,
    kKeyH,
    kKeyI,
    kKeyJ,
    kKeyK,
    kKeyL,
    kKeyM,
    kKeyN,
    kKeyO,
    kKeyP,
    kKeyQ,
    kKeyR,
    kKeyS,
    kKeyT,
    kKeyU,
    kKeyV,
    kKeyW,
    kKeyX,
    kKeyY,
    kKeyZ,

    kKeyEscape,
    kKeyShift,
    kKeyCtrl,
    kKeyAlt,

    kKeyUp,
    kKeyDown,
    kKeyRight,
    kKeyLeft,

    kKeyF1,
    kKeyF2,
    kKeyF3,
    kKeyF4,
    kKeyF5,
    kKeyF6,
    kKeyF7,
    kKeyF8,
    kKeyF9,
    kKeyF10,
    kKeyF11,
    kKeyF12,

    kMAX_KEYS,
};

typedef unsigned int mouse_state_t;

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void);
void Shutdown(void);
void* GetMainWindow(void);

void SetFrameCallback(void_callback_t* callback);
void SetShutdownCallback(void_callback_t* callback);
void SetResizeCallback(resize_callback_t* callback);
void RunMainLoop(void);

void SpawnWindow(int width, int height, int fullscreen);
void SetWindowTitle(const char* title);

message_box_e MessageBox(const char* title, const char* message, message_box_type_e type);

void GetExecutableDirectory(size_t bufferSize, char* directory);
void GetCurrentDirectory(size_t bufferSize, char* directory);
void SetCurrentDirectory(const char* directory);
void RestoreDirectory(void);

//
// Mouse position is relative to the top left
//
void GetMousePosition(int* x, int *y);
mouse_state_t GetMouseState(void);
void GetKeyboardState(char keys[kMAX_KEYS]);
int GetKeyState(key_e key);

} // namespace System

#endif /* include guard */
