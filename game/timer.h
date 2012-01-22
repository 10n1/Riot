/*
 * timer.h
 * riot
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __riot_timer_h__
#define __riot_timer_h__

/* C headers */
#include <stdint.h>
/* External headers */
/* Internal headers */

/*******************************************************************\
External Constants And types
\*******************************************************************/
typedef struct timer_t
{
    uint64_t    startTime;
    uint64_t    prevTime;
    double      frequency;
} timer_t;

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void timerInit(timer_t* timer);
void timerReset(timer_t* timer);
double timerGetDeltaTime(timer_t* timer);
double timerGetRunningTime(timer_t* timer);

#endif /* include guard */
