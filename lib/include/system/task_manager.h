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

#ifndef _TASK_MANAGER_H
#define _TASK_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int        taskid_t;
extern const unsigned int   kTaskManagerSize;

/*
    Task manager functions
*/
void taskManagerInitialize(int num_threads, void* buffer, unsigned int buffer_size);
void taskManagerShutdown(void);

/*
Submitting work
*/
taskid_t taskManagerSubmitFunc(void (*func)(void*), void* param);
void taskManagerFinishWork(void);

    
#ifdef __cplusplus
} //  extern "C" {
#endif

#endif
