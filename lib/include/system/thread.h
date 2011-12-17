/*
 *  thread.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_thread_h
#define riot_thread_h

#include "atomic.h"

namespace System
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
typedef void (thread_proc_t)(void*);
typedef atomic_t spinlock_t;

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
int ThreadSpawn(thread_proc_t* func, void* param);

/* Spinlock */
int SpinlockTryLock(spinlock_t* lock);
void SpinlockLock(spinlock_t* lock);
void SpinlockUnlock(spinlock_t* lock);

}

#endif /* include guard */
