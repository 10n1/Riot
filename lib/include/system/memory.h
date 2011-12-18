/*
 *  mmemory.h
 *  enginelib
 *
 *  Created by Kyle C Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef enginelib_memory_h
#define enginelib_memory_h

#include "types.h"

namespace Memory
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
enum
{
    kKilobyte = 1024,
    kMegabyte = 1024*1024,
    kGigabyte = 1024*1024*1024
};

class Allocator
{
public:
    Allocator() {}
    virtual ~Allocator() { }
    virtual void  Init(void) = 0;
    virtual void  Reset(void) = 0;
    virtual void* Allocate(size_t size, uint32_t alignment = 16) = 0;
    virtual void  Deallocate(void* memory) = 0;
    virtual size_t FreeSize(void) = 0;
};

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(size_t totalSize);
void Shutdown(void);
Allocator* GlobalAllocator(void);

Allocator* CreateFrameAllocator(Allocator* allocator, size_t size);

} // namespace Memory

#endif
