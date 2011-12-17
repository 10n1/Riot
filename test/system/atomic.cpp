/*
 *  atomic.cpp
 *  Riot test
 *
 *  Created by Kyle Weicht on 12/16/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "gtest/gtest.h"

#include "system/atomic.h"
#include "types.h"
#include "build.h"

TEST(Atomic, BasicOperations32)
{
    EXPECT_EQ(4, sizeof(atomic_t));
    EXPECT_EQ(4, sizeof(atomic32_t));

    atomic_t atomicTest = 42;

    AtomicAdd32(&atomicTest, 3);
    EXPECT_EQ(45, (int32)atomicTest);
    AtomicAdd32(&atomicTest, -7);
    EXPECT_EQ(38, (int32)atomicTest);

    AtomicIncrement32(&atomicTest);
    EXPECT_EQ(39, (int32)atomicTest);
    AtomicDecrement32(&atomicTest);
    EXPECT_EQ(38, (int32)atomicTest);

    AtomicOr32(&atomicTest, 0x80000000);
    EXPECT_EQ((38 | 0x80000000), (int32)atomicTest);
    AtomicAnd32(&atomicTest, 0x80FF000F);
    EXPECT_EQ(((38 | 0x80000000) & 0x80FF000F), (int32)atomicTest);

    AtomicExchange32(&atomicTest, 0xDEADBEEF);
    EXPECT_EQ(0xDEADBEEF, (int32)atomicTest);

    AtomicCompareAndSwap32(&atomicTest, 0xDEADB00F, 0xDEADB00F);
    EXPECT_EQ(0xDEADBEEF, (int32)atomicTest);
    AtomicCompareAndSwap32(&atomicTest, 0xDEADB00F, 0xDEADBEEF);
    EXPECT_EQ(0xDEADB00F, (int32)atomicTest);
    AtomicCompareAndSwap32(&atomicTest, 0xDEADBEEF, 0xDEADB00F);
    EXPECT_EQ(0xDEADBEEF, (int32)atomicTest);
}

#if BUILD_ARCH_ID == BUILD_ARCH_64BIT
TEST(Atomic, BasicOperations64)
{
    EXPECT_EQ(8, sizeof(atomic64_t));

    atomic64_t atomicTest = 42;

    AtomicAdd64(&atomicTest, 3);
    EXPECT_EQ(45, (int64)atomicTest);
    AtomicAdd64(&atomicTest, -7);
    EXPECT_EQ(38, (int64)atomicTest);

    AtomicIncrement64(&atomicTest);
    EXPECT_EQ(39, (int64)atomicTest);
    AtomicDecrement64(&atomicTest);
    EXPECT_EQ(38, (int64)atomicTest);

    AtomicOr64(&atomicTest, (int64)0xFEDCBA9880000000);
    EXPECT_EQ( (int64)((int64)38|(int64)0xFEDCBA9880000000), (int64)atomicTest);
    AtomicAnd64(&atomicTest, (int64)0x0EDCBA9880FF000F);
    // EXPECT_EQ( ((int64)((int64)38|(int64)0xFEDCBA9880000000) & (int64)0x0EDCBA9880FF000F), (int64)atomicTest); 
    // Currently broken with Clang in Xcode 4.2.1

    AtomicExchange64(&atomicTest, 0xDEADBEEFFEDCBA98);
    EXPECT_EQ(0xDEADBEEFFEDCBA98, (int64)atomicTest);

    AtomicCompareAndSwap64(&atomicTest, 0xDEADB00FFEDCBA98, 0xDEADB00FFEDCBA98);
    EXPECT_EQ(0xDEADBEEFFEDCBA98, (int64)atomicTest);
    AtomicCompareAndSwap64(&atomicTest, 0xDEADB00FFEDCBA98, 0xDEADBEEFFEDCBA98);
    EXPECT_EQ(0xDEADB00FFEDCBA98, (int64)atomicTest);
    AtomicCompareAndSwap64(&atomicTest, 0xDEADBEEFFEDCBA98, 0xDEADB00FFEDCBA98);
    EXPECT_EQ(0xDEADBEEFFEDCBA98, (int64)atomicTest);
}
#endif
