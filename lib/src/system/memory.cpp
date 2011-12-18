/*
 *  hardware.cpp
 *  enginelib
 *
 *  Created by Kyle C Weicht on 11/1/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "system/memory.h"
#include "build.h"
#include "assert.h"
#include "system/hardware.h"

#include <memory>

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    #include <sys/mman.h>
    #include <errno.h>
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
#endif

namespace
{

void alignPointer(void** ptr, uint32_t align);
/*******************************************************************\
 Internal constants and types
\*******************************************************************/
class FrameAllocator : public Memory::Allocator
{
public:
    void Init(void)
    {
        _startMemory     = nullptr;
        _currentMemory   = nullptr;
        _endMemory       = nullptr;
        _initialSize     = 0;
    }
    void Reset(void)
    {
        _currentMemory = _startMemory;
    }
    void* Allocate(size_t size, uint32_t alignment)
    {
        alignPointer((void**)&_currentMemory, alignment);
        
        uint8* const nextAllocation = _currentMemory+size;
        assert(nextAllocation < _endMemory);
        
        uint8* const allocation = _currentMemory;
        _currentMemory = nextAllocation;
        
        return allocation;
    }
    void Deallocate(void* memory)
    {
        UNUSED_PARAMETER(memory);
    }
    size_t FreeSize(void)
    {
        return (size_t)(_endMemory-_currentMemory);
    }
    
public:
    uint8*  _startMemory;
    uint8*  _currentMemory;
    uint8*  _endMemory;
    size_t  _initialSize;
};

class PoolAllocator : public Memory::Allocator
{
public:
    void Init(void)
    {
    }
    void Reset(void)
    {
    }
    void* Allocate(size_t size, uint32_t alignment)
    {
        return nullptr;
        (void)sizeof(size),(void)sizeof(alignment);
    }
    void Deallocate(void* memory)
    {
        (void)sizeof(memory);
    }
};


const auto  kMaxAllocatorSize  = 
    (sizeof(FrameAllocator) > sizeof(PoolAllocator)) ? 
     sizeof(FrameAllocator) : sizeof(PoolAllocator);

/*******************************************************************\
 Internal variables
\*******************************************************************/
static FrameAllocator   s_globalFrameAllocator;
static uint8_t*         s_memory            = nullptr;
static size_t           s_totalBytes        = 0;
static size_t           s_allocatorPadding  = 0;


/*******************************************************************\
 Internal functions
\*******************************************************************/
int checkPowerOfTwo(uint32_t x)
{
    return ((x != 0) && !(x & (x - 1)));
}
void alignPointer(void** ptr, uint32_t align)
{   
    intptr_t current_alignment = (intptr_t)*ptr;
    int remainder = (int)(current_alignment % align);
    
    assert(checkPowerOfTwo(align));
    if(remainder)
    {
        *ptr = (void*)( ((uint8_t*)*ptr) + remainder);
    }
}

} // namespace

namespace Memory
{

/*******************************************************************\
 External constants and types
\*******************************************************************/

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(size_t totalSize)
{
    if(s_memory != nullptr)
        return;
        
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    s_memory = (uint8_t*)mmap(NULL, totalSize, PROT_WRITE | PROT_READ | PROT_EXEC, MAP_ANON | MAP_PRIVATE, -1, (size_t)0);
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    s_memory = (uint8_t*)VirtualAlloc(NULL, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else
    #error Memory
#endif
    assert(s_memory);
    s_totalBytes = totalSize;

    /* Create the main allocator */
    s_globalFrameAllocator.Init();
    s_globalFrameAllocator._startMemory      = s_memory;
    s_globalFrameAllocator._currentMemory    = s_memory;
    s_globalFrameAllocator._endMemory        = s_memory+totalSize;
    s_globalFrameAllocator._initialSize      = totalSize;
    
    s_allocatorPadding = (uint32)Hardware::CacheLineSize()-kMaxAllocatorSize;
}
void Shutdown(void)
{
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    const int result = munmap(s_memory, s_totalBytes);
    assert(result == 0);
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    const int result = VirtualFree(s_memory, 0, MEM_RELEASE);
    assert(result != 0);
#else
    #error Memory
#endif

    s_memory = nullptr;
    s_globalFrameAllocator.Init();
}
Allocator* GlobalAllocator(void)
{
    return &s_globalFrameAllocator;
}
Allocator* CreateFrameAllocator(Allocator* allocator, size_t size)
{
    if(allocator == nullptr)
        allocator = &s_globalFrameAllocator;
    
    uint8* allocation = (uint8*)allocator->Allocate(sizeof(FrameAllocator)+
                                                    s_allocatorPadding + 
                                                    size, (uint)Hardware::CacheLineSize());
                                                    
    FrameAllocator*  newAllocator = new(allocation) FrameAllocator();
    newAllocator->Init();
    newAllocator->_startMemory  = allocation+(uint)Hardware::CacheLineSize();
    newAllocator->_currentMemory= newAllocator->_startMemory;
    newAllocator->_endMemory    = newAllocator->_startMemory+size;
    newAllocator->_initialSize  = size;
    
    newAllocator->Reset();
    return newAllocator;
}

} // namespace Memory
