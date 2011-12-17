/*
 *  thread.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "system/thread.h"
#include "assert.h"
#include "build.h"

#if BUILD_SYSTEM_TYPE == BUILD_SYSTEM_POSIX
    #include <pthread.h>
    #include <semaphore.h>
    #include <errno.h>
    #define SYSTEM_API_CONVENTION
#elif BUILD_SYSTEM_TYPE == BUILD_SYSTEM_WINDOWS
    #define SYSTEM_API_CONVENTION __stdcall
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#else
    #error Need a threading implementation
#endif

namespace
{

using namespace System;
/*******************************************************************\
 Internal constants and types
\*******************************************************************/
#if BUILD_SYSTEM_TYPE == BUILD_SYSTEM_POSIX
    typedef void*           thread_return_t;
    typedef pthread_t       thread_handle_t;
    typedef pthread_mutex_t mutex_t;
    typedef pthread_cond_t  wait_condition_t;
    #define kThreadReturn   0
#else
    typedef DWORD   thread_return_t;
    typedef void*   thread_handle_t;
    typedef void*   mutex_t;
    typedef void*   wait_condition_t;
    #define kThreadReturn
#endif

struct thread_data_t
{
    thread_proc_t*  func;
    void*           param;
};


/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/
thread_return_t SYSTEM_API_CONVENTION SystemProc(void* data)
{
    thread_proc_t* func = ((thread_data_t*)data)->func;
    void* param = ((thread_data_t*)data)->param;
    ((thread_data_t*)data)->param = nullptr;// We null it out so the caller knows 
                                            // the thread was spawned successfully

    func(param);

    return 0;
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
int ThreadSpawn(thread_proc_t* func, void* param)
{
    thread_data_t   threadData  = {func,param};
    thread_handle_t handle      = nullptr;

#if BUILD_SYSTEM_TYPE == BUILD_SYSTEM_POSIX
    pthread_create(&handle, nullptr, SystemProc, &threadData);
#else
    handle = CreateThread(nullptr, 0, SystemProc, &threadData, 0, 0);
#endif
    
    // Spin while we wait for the thread to spawn (should be near-instant)
    while(threadData.param)
        ;

    return handle ? 0 : 1;
}

/* Spinlock */
int SpinlockTryLock(spinlock_t* lock)
{
    return AtomicExchange32(lock, 1) == 0;
}
void SpinlockLock(spinlock_t* lock)
{
    while(!SpinlockTryLock(lock))
    {
        ;
    }
}
void SpinlockUnlock(spinlock_t* lock)
{
    AtomicExchange32(lock,0);
}


} // namespace System
