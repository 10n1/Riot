/*
 *  timer.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_timer_h
#define riot_timer_h

#include <stdint.h>

namespace System
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
struct timer_t
{
    uint64_t    startTime;
    uint64_t    prevTime;
    double      frequency;
};

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void TimerInit(timer_t* timer);
void TimerReset(timer_t* timer);
double TimerGetDeltaTime(timer_t* timer);
double TimerGetRunningTime(timer_t* timer);

}

#endif /* include guard */
