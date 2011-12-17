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

namespace Timer
{

/*******************************************************************\
 External functions
\*******************************************************************/
void Init(timer_t* timer);
void Reset(timer_t* timer);
double GetDeltaTime(timer_t* timer);
double GetRunningTime(timer_t* timer);

} // namespace Timer

#endif /* include guard */
