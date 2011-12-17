/*
 *  timer.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "system/timer.h"
#include "build.h"

/* System includes */
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS || BUILD_PLATFORM_ID == BUILD_PLATFORM_IOS
    #include <mach/mach_time.h>
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <windows.h>
#endif


namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace Timer
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
void Init(timer_t* timer)
{
    mach_timebase_info_data_t frequency = {0,0};
    mach_timebase_info(&frequency);
    timer->frequency = (double)frequency.numer / (double)frequency.denom;
    timer->frequency *= 1e-9;
    timer->startTime = timer->prevTime = mach_absolute_time();
}
void Reset(timer_t* timer)
{
    timer->startTime = timer->prevTime = mach_absolute_time();
}
double GetDeltaTime(timer_t* timer)
{
    uint64_t    current_time    = mach_absolute_time();
    double      delta_time      = (double)(current_time - timer->prevTime) * timer->frequency;
    timer->prevTime = current_time;
    
    return delta_time;
}
double GetRunningTime(timer_t* timer)
{
    uint64_t    current_time = mach_absolute_time();
    double      running_time = (double)(current_time - timer->startTime) * timer->frequency;
    
    return running_time;
}
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
void Init(timer_t* timer)
{
    LARGE_INTEGER   freq;
    QueryPerformanceFrequency( &freq );
    timer->frequency = 1.0/(double)freq.QuadPart;
    
    QueryPerformanceCounter((LARGE_INTEGER*)&timer->startTime);
    timer->prevTime = timer->startTime;
}
void Reset(timer_t* timer)
{
    QueryPerformanceCounter((LARGE_INTEGER*)&timer->startTime);
    timer->prevTime = timer->startTime;
}
double GetDeltaTime(timer_t* timer)
{  
    uint64_t    current_time;

    QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    double delta_time = (current_time - timer->prevTime) * timer->frequency;
    timer->prevTime = current_time;
    return delta_time;
}
double GetRunningTime(timer_t* timer)
{
    uint64_t  current_time;

    QueryPerformanceCounter( (LARGE_INTEGER*)&current_time );
    double running_time = (current_time - timer->startTime) * timer->frequency;
    return running_time;
}
#else
    #error Need a timer!
#endif // PLATFORM_ID

} // namespace Timer
