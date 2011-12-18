/*
 *  memory.cpp
 *  riot test
 *
 *  Created by Kyle C Weicht on 11/1/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "gtest/gtest.h"
#include "system/memory.h"

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
enum
{
    kGlobalAllocationSize   = Memory::kMegabyte*16,
    kAllocationSmall        = 128,
    kAllocationLarge        = Memory::kMegabyte,
    kAllocationWeird        = 653,
};

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
TEST(Memory, GlobalAllocator)
{    
    Memory::Allocator* globalAllocator = Memory::GlobalAllocator();
    
    // Create the allocator
    EXPECT_TRUE(globalAllocator);
    EXPECT_EQ((size_t)0, globalAllocator->FreeSize());    
    Memory::Initialize(kGlobalAllocationSize);
    EXPECT_EQ((size_t)kGlobalAllocationSize, globalAllocator->FreeSize());
    
    // Allocate
    globalAllocator->Allocate(kAllocationLarge);
    EXPECT_EQ((size_t)kGlobalAllocationSize-kAllocationLarge, globalAllocator->FreeSize());
    
    // Reset
    globalAllocator->Reset();
    EXPECT_EQ((size_t)kGlobalAllocationSize, globalAllocator->FreeSize());
    
    // Shutdown
    Memory::Shutdown();
    EXPECT_EQ((size_t)0, globalAllocator->FreeSize());
}

TEST(Memory, FrameAllocator)
{   
    Memory::Initialize(kGlobalAllocationSize);
    
    // Create it
    const unsigned int allocator_size = Memory::kMegabyte*4;
    Memory::Allocator* allocator = Memory::CreateFrameAllocator(NULL, allocator_size);
    EXPECT_EQ(allocator_size, allocator->FreeSize());
    
    // Allocations
    allocator->Allocate(kAllocationWeird);
    EXPECT_LE(allocator->FreeSize(), allocator_size-kAllocationWeird);
    
    allocator->Allocate(kAllocationWeird);
    EXPECT_LE(allocator->FreeSize(), allocator_size-kAllocationWeird*2);
    
    allocator->Reset();
    EXPECT_EQ(allocator_size, allocator->FreeSize());
    
    Memory::Shutdown();
}
