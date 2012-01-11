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
typedef void (callback_t)(void);
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

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void);
void Shutdown(void);
void* GetMainWindow(void);

void SetFrameCallback(callback_t* callback);
void SetShutdownCallback(callback_t* callback);
void RunMainLoop(void);

void SpawnWindow(int width, int height, int fullscreen);
void CloseWindow(void);
void SetWindowTitle(const char* title);

message_box_e MessageBox(const char* title, const char* message, message_box_type_e type);

void GetExecutableDirectory(size_t bufferSize, char* directory);
void GetCurrentDirectory(size_t bufferSize, char* directory);
void SetCurrentDirectory(const char* directory);
void RestoreDirectory(void);

} // namespace System

#endif /* include guard */
