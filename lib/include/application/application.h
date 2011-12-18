/*
 *  application.h
 *  riot
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

#ifndef riot_application_h
#define riot_application_h

/*******************************************************************\
 External constants and types
\*******************************************************************/
typedef void (frame_callback_t)(void);

/*******************************************************************\
 External variables
\*******************************************************************/
enum application_status_e
{
    kNotStarted,
    kStarted,
    kRunning,
    kStopped,
};

namespace Application
{

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void);
void StartMainLoop(void);
void Shutdown(void);
void SetFrameCallback(frame_callback_t* callback);

application_status_e GetStatus(void);
const char* GetExecutableDirectory(void);

void* GetOSApplication(void);   // Windows: HINSTANCE
                                // OS X:    NSApplication*

} // namespace Application

#endif /* Include guard */
