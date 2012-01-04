/***********************************************************************\
 Copyright (c) 2011 Kyle Weicht
 
 Permission is hereby granted, free of charge, to any person obtaining a 
 copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 THE SOFTWARE.
\***********************************************************************/

/*
    Configuration detection
*/
#if defined( DEBUG ) || defined( _DEBUG )
    #ifndef DEBUG
        #define DEBUG
    #endif
#else
    #ifndef RELEASE
        #define RELEASE
    #endif
#endif

/*
    Platform detection
*/
#define PLATFORM_WINDOWS    1
#define PLATFORM_POSIX      2

#if defined( WIN32 )
    #define PLATFORM_ID PLATFORM_WINDOWS
#elif defined( __MACH__ ) || defined( __ANDROID__ ) || defined( __linux__ )
    #define PLATFORM_ID PLATFORM_POSIX
#else
    #error No valid target found
#endif

/*
    Atomic functions
*/
#if defined (_MSC_VER)
    #include <intrin.h>
    #define AtomicIncrement(value)   _InterlockedIncrement(value)
    #define AtomicDecrement(value)   _InterlockedDecrement(value)

    #define AtomicExchange(old_value, new_value) _InterlockedExchange(old_value, new_value)
#elif defined(__GNUC__)
    #define AtomicIncrement(value)   __sync_add_and_fetch(value,1)
    #define AtomicDecrement(value)   __sync_sub_and_fetch(value,1)

    #define AtomicExchange(old_value, new_value) __sync_lock_test_and_set(old_value, new_value)
#else
    #error Unsupported compiler
#endif

/*
    Define debugBreak
*/
#if defined( _MSC_VER )
    #define debugBreak() __debugbreak()
#elif defined( __GNUC__ )
    #define debugBreak() __asm__( "int $3\n" : : )
#else
    #error Unsupported compiler
#endif

/*
    Define ASSERT
*/
#ifdef ASSERTS_DISABLED
    #define ASSERT(condition) do{ (void)sizeof((condition)); } while(__LINE__ == -1)
#else
    #define ASSERT(condition)   \
        do                      \
        {                       \
            if(!(condition))    \
            {                   \
                debugBreak();   \
            }                   \
        } while(__LINE__ == -1)
#endif

#define assert(condition) ASSERT((condition))


/*
    Various macros
*/
#ifndef ARRAY_LENGTH
    #define ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))
#endif

#if defined( _MSC_VER )
    #define ALIGN_STRUCT(alignment) __declspec(align(alignment))
#elif defined( __GNUC__ )
    #define ALIGN_STRUCT(alignment) __attribute((aligned(alignment)))
#else
    #error Unsupported compiler
#endif

#ifndef UNUSED_PARAMETER
    #define UNUSED_PARAMETER(param) ((void)sizeof(param))
#endif

/*
    System includes
*/
#if PLATFORM_ID == PLATFORM_POSIX
    #include <pthread.h>
    #include <semaphore.h>
    #include <errno.h>
    #define SYSTEM_API_CONVENTION
#elif PLATFORM_ID == PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN 
    #include <Windows.h>
    #define SYSTEM_API_CONVENTION __stdcall
#endif

#if defined(__APPLE__)
    #include <sys/sysctl.h>
#endif

#include <stddef.h>
#include <stdint.h> /* This is not part of C99, but GCC has no equivilent to __int* */
#include <string.h>

#include <stdio.h> /* HACK */

#include "system/task_manager.h"


/*******************************************************************\
 Spinlocks
\*******************************************************************/
typedef volatile long spinlock_t;

static int spinlockTryLock(spinlock_t* lock)
{
    return AtomicExchange(lock, 1) == 0;
}
static void spinlockLock(spinlock_t* lock) 
{ 
    while( !spinlockTryLock(lock) )
    {
        ;
    }
}
static void spinlockUnlock(spinlock_t* lock)
{
    AtomicExchange(lock, 0);
}

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
#if PLATFORM_ID == PLATFORM_POSIX
typedef void*           thread_return_t;
typedef pthread_t       thread_handle_t;
typedef pthread_mutex_t mutex_t;
typedef pthread_cond_t  wait_condition_t;
#define kThreadReturn   0
#else
typedef void    thread_return_t;
typedef void*   thread_handle_t;
typedef void*   mutex_t;
typedef void*   wait_condition_t;
#define kThreadReturn
#endif

typedef volatile int    atomic_t;

enum 
{ 
    kMaxTasks = 16*128,
    kMaxThreads = 64
};

typedef struct __Task
{
    void        (*work_item)(void*);
    void*       param;
    taskid_t    task_id;
    taskid_t    dependency;
} Task;
typedef struct ALIGN_STRUCT(64) __Thread
{
    thread_handle_t handle;
    int             thread_id;
    int             finished;
} Thread;
typedef struct ALIGN_STRUCT(64) __TaskManager
{
    Task*       tasks;
    Thread*     threads;
    int         total_tasks;
    int         num_threads;
    int         shutdown;
    int         read_index;
    int         write_index;
    spinlock_t  lock;
    mutex_t     global_mutex;
    wait_condition_t    thread_wake_condition;
} TaskManager;


/*******************************************************************\
 Internal variables
\*******************************************************************/
static TaskManager* _task_manager = NULL;
const unsigned int  kTaskManagerSize = (sizeof(TaskManager) + sizeof(Thread)*(size_t)kMaxThreads + sizeof(Task)*kMaxTasks) + 64;

static int  _added = 0;
static int  _not_added = 0;


/*******************************************************************\
 Platform-independent threading
\*******************************************************************/
#if PLATFORM_ID == PLATFORM_POSIX
static thread_handle_t spawnThread(thread_return_t (thread_proc)(void*), void* param)
{
    pthread_t   handle = NULL;
    pthread_create(&handle, NULL, thread_proc, param);
    /* TODO: Handle creation error */
    return handle;
}
static mutex_t createMutex(void)
{
    mutex_t mutex;

    pthread_mutex_init(&mutex, NULL);
    /*mutex = CreateMutex(NULL, false, NULL);*/

    return mutex;
}
static wait_condition_t createWaitCondition(void)
{
    wait_condition_t condition;
    
    pthread_cond_init(&condition, NULL);

    return condition;
}
static void waitForCondition(wait_condition_t* condition, mutex_t* mutex)
{
    pthread_mutex_lock(mutex);
    pthread_cond_wait(condition, mutex);
    pthread_mutex_unlock(mutex);
}
static void signalCondition(wait_condition_t* condition, mutex_t* mutex)
{
    pthread_mutex_lock(mutex);
    pthread_cond_broadcast(condition);
    pthread_mutex_unlock(mutex);
}
#else
static thread_handle_t spawnThread(thread_return_t (SYSTEM_API_CONVENTION thread_proc)(void*), void* param)
{
    HANDLE   handle = NULL;

    handle = CreateThread(NULL,0,thread_proc,param,0,0);

    return handle;
}
static mutex_t createMutex(void)
{
    mutex_t mutex;

    mutex = CreateMutex(NULL, 0, NULL);

    return mutex;
}
static wait_condition_t createWaitCondition(void)
{
    wait_condition_t condition;
    
    condition = CreateEvent(NULL, 0, 0, NULL);

    return condition;
}
static void waitForCondition(wait_condition_t* condition, mutex_t* mutex)
{
    WaitForSingleObject( *condition, INFINITE );
}
static void signalCondition(wait_condition_t* condition, mutex_t* mutex)
{
    int ii;
    for(ii=0; ii<_task_manager->num_threads; ++ii)
    {
        /* Is there a way to have all threads wait on one condition? */
        SetEvent(*condition);
        ResetEvent(*condition);
    }
}
#endif

/*******************************************************************\
 Internal functions
\*******************************************************************/

static unsigned int getNumHardwareThreads(void)
{
#if defined(WIN32)
    unsigned int num_threads = 0;
    SYSTEM_INFO si;
    GetSystemInfo( &si );
    num_threads = (unsigned int)si.dwNumberOfProcessors;
#elif defined(__APPLE__)
    unsigned int num_threads = 0;
    int mib[] = {CTL_HW, HW_AVAILCPU};
    size_t  length = sizeof( num_threads );
    
    sysctl(mib, (unsigned int)ARRAY_LENGTH(mib), &num_threads, &length, NULL, (size_t)0);
    
    if( num_threads < 1 ) 
    {
        mib[1] = HW_NCPU;
        sysctl(mib, (unsigned int)ARRAY_LENGTH(mib), &num_threads, &length, NULL, (size_t)0);
    }
    return num_threads;
#else
    #error Need to write this function
#endif
}

static int pushTask(Task task)
{
    int add_index;
    spinlockLock(&_task_manager->lock);
    
    if((_task_manager->write_index+1) == _task_manager->read_index)
    {
        spinlockUnlock(&_task_manager->lock);
        return 0;
    }
    
    add_index = _task_manager->write_index++;
    if(_task_manager->write_index == kMaxTasks)
        _task_manager->write_index = 0;
    
    assert(_task_manager->write_index < kMaxTasks);
    
    _task_manager->tasks[add_index] = task;
    spinlockUnlock(&_task_manager->lock);
    return 1;
}

static int popTask(Task* task)
{
    int task_index;
    spinlockLock(&_task_manager->lock);
    
    if(_task_manager->read_index == _task_manager->write_index)
    {
        spinlockUnlock(&_task_manager->lock);
        return 0;
    }
    
    task_index = _task_manager->read_index++;
    if(_task_manager->read_index == kMaxTasks)
        _task_manager->read_index = 0;
    
    assert(_task_manager->read_index < kMaxTasks);
    
    *task = _task_manager->tasks[task_index];
    
    spinlockUnlock(&_task_manager->lock);
    return 1;
}

static void threadIdle(Thread* thread)
{
    UNUSED_PARAMETER(thread);
    /*printf("Thread %d sleeping...\n", thread->thread_id);*/
    /*pthread_cond_wait(&_task_manager->thread_wake_condition, &_task_manager->global_mutex);
    pthread_mutex_unlock(&_task_manager->global_mutex);*/
    /*printf("Thread %d awake...\n", thread->thread_id);*/
    
    waitForCondition(&_task_manager->thread_wake_condition, &_task_manager->global_mutex);
}

static void wakeThreads(void)
{
    /*pthread_cond_broadcast(&_task_manager->thread_wake_condition);*/
    signalCondition(&_task_manager->thread_wake_condition,&_task_manager->global_mutex);
}

static void threadDoWork(Thread* thread)
{
    Task task;
    for(;;)
    {
        if(_task_manager->shutdown)
            break;
            
        if(popTask(&task) == 0)
            break;
            
        task.work_item(task.param);
        /*printf("Thread %d finished task %d\n", thread->thread_id, task->task_id);*/
        /*task->task_id = -1;*/
    }
    
    UNUSED_PARAMETER(thread);
}

static thread_return_t SYSTEM_API_CONVENTION threadProc(void* param)
{
    Thread* thread = param;
    
    do
    {
        threadIdle(thread);
        if(_task_manager->shutdown)
            break;
        
        threadDoWork(thread);
        
        if(_task_manager->shutdown)
            break;
        
    } while(_task_manager->shutdown == 0);
    
    thread->finished = 1;
    return kThreadReturn;
}

static void startThread(Thread* thread)
{
    thread->handle = spawnThread(threadProc, thread);
}

/*******************************************************************\
 External functions
\*******************************************************************/
/*
 Task manager functions
 */
void taskManagerInitialize(int num_threads, void* buffer, unsigned int buffer_size)
{
    int     ii;
    void*   raw_buffer  = buffer;
    
    /* If the task manager was already initialized, just return */
    if(_task_manager)
        return;
    
    /* Determine how many threads to spawn */
    if(num_threads <= 0)
        num_threads = (int)getNumHardwareThreads();
    
    assert(buffer_size >= kTaskManagerSize);
    
    if(num_threads < 1)
        num_threads = 1;
    else if(num_threads > kMaxThreads)
        num_threads = kMaxThreads;
    
    memset(buffer, 0, (size_t)buffer_size);
    
    /* Align buffer */
    while((intptr_t)raw_buffer % 64)
    {
        raw_buffer = ((uint8_t*)raw_buffer) + 1;
    }
    
    /* Allocate task manager */
    _task_manager = raw_buffer;
    raw_buffer = ((uint8_t*)raw_buffer) + sizeof(TaskManager);
    _task_manager->total_tasks  = kMaxTasks;
    _task_manager->num_threads  = num_threads;
    _task_manager->global_mutex = createMutex();
    _task_manager->thread_wake_condition = createWaitCondition();
    
    /* Allocate threads */
    _task_manager->threads = (Thread*)raw_buffer;
    raw_buffer = ((uint8_t*)raw_buffer) + ((int)sizeof(Thread) * num_threads);
    
    /* Allocate tasks */
    _task_manager->tasks = (Task*)raw_buffer;
    /* raw_buffer = ((uint8_t*)raw_buffer) + ((int)sizeof(Task) * kMaxTasks); */
    
    /* Start the threads */
    for(ii=1;ii<num_threads;++ii)
    {  
        _task_manager->threads[ii].thread_id = ii;
        startThread(&_task_manager->threads[ii]);
    }
}
void taskManagerShutdown(void)
{
    int ii;
    taskManagerFinishWork();
    
    _task_manager->shutdown = 1;
    wakeThreads();
    for(ii=1;ii<_task_manager->num_threads;++ii)
    {
        /*printf("Waiting for thread %d...", ii); */ /* HACK */
        while(_task_manager->threads[ii].finished == 0)
        {
            wakeThreads(); /* Dirty hack to get threads to wake up */
        }
        /*printf("done!\n"); */ /* HACK */
    }
}

/*
 Submitting work
 */
taskid_t taskManagerSubmitFunc(void (*func)(void*), void* param)
{
    Task    task;
    
    task.work_item  = func;
    task.param      = param;
    task.task_id    = 0;
    task.dependency = 0;
    
    assert(_task_manager);
    if(pushTask(task) == 0)
    {
        func(param);
        AtomicIncrement(&_not_added);
        printf("Task %d not added\n", task.task_id);
        return 1;
    }
    
    wakeThreads();
    AtomicIncrement(&_added);
        
    return 0;
}

void taskManagerFinishWork(void)
{
    Task task;
    while(popTask(&task))
    {
        task.work_item(task.param);
    }
}
