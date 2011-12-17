/*
 *  atomic.h
 *  engine
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef riot_atomic_h_
#define riot_atomic_h_

#include <stdint.h>

/*******************************************************************\
 Atomic operations
\*******************************************************************/
#if defined (_MSC_VER)
    typedef volatile long       atomic32_t;
    typedef volatile int64_t    atomic64_t;
    typedef atomic32_t          atomic_t;
    #include <intrin.h>
    #define AtomicIncrement32(value)   _InterlockedIncrement(value)
    #define AtomicDecrement32(value)   _InterlockedDecrement(value)
    #define AtomicIncrement64(value)   _InterlockedIncrement64(value)
    #define AtomicDecrement64(value)   _InterlockedDecrement64(value)
        
    #define AtomicAdd32(value, amount) _InterlockedExchangeAdd(value, amount)
    #define AtomicAdd64(value, amount) _InterlockedExchangeAdd64(value, amount)

    #define AtomicOr32(value, mask)    _InterlockedOr(value, mask)
    #define AtomicAnd32(value, mask)   _InterlockedAnd(value, mask)
    #define AtomicOr64(value, mask)    _InterlockedOr64(value, mask)
    #define AtomicAnd64(value, mask)   _InterlockedAnd64(value, mask)

    #define AtomicExchange32(old_value, new_value) _InterlockedExchange(old_value, new_value)
    #define AtomicExchange64(old_value, new_value) _InterlockedExchange64(old_value, new_value)

    #define AtomicCompareAndSwap32(value, new_value, compare) _InterlockedCompareExchange(value, new_value, compare)
    #define AtomicCompareAndSwap64(value, new_value, compare) _InterlockedCompareExchange64(value, new_value, compare)
#elif defined(__GNUC__)
    typedef volatile int32_t    atomic32_t;
    typedef volatile int64_t    atomic64_t;
    typedef atomic32_t          atomic_t;
    #define AtomicIncrement32(value)   __sync_add_and_fetch(value,1)
    #define AtomicDecrement32(value)   __sync_sub_and_fetch(value,1)
    #define AtomicIncrement64(value)   __sync_add_and_fetch(value,1)
    #define AtomicDecrement64(value)   __sync_sub_and_fetch(value,1)

    #define AtomicAdd32(value, amount) __sync_add_and_fetch(value, amount)
    #define AtomicAdd64(value, amount) __sync_add_and_fetch(value, amount)

    #define AtomicOr32(value, mask)    __sync_or_and_fetch(value, mask)
    #define AtomicAnd32(value, mask)   __sync_and_and_fetch(value, mask)
    #define AtomicOr64(value, mask)    __sync_or_and_fetch(value, mask)
    #define AtomicAnd64(value, mask)   __sync_and_and_fetch(value, mask)

    #define AtomicExchange32(old_value, new_value) __sync_lock_test_and_set(old_value, new_value)
    #define AtomicExchange64(old_value, new_value) __sync_lock_test_and_set(old_value, new_value)

    #define AtomicCompareAndSwap32(value, new_value, compare) __sync_bool_compare_and_swap(value, compare, new_value)
    #define AtomicCompareAndSwap64(value, new_value, compare) __sync_bool_compare_and_swap(value, compare, new_value)
#else
    #error Unsupported compiler
#endif


#endif /* include guard */
