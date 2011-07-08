//
//  config.h
//  Cosmos
//
//  Created by Kyle Weicht on 7/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

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
#define PLATFORM_WINDOWS 1
#define PLATFORM_MACOS   2
#define PLATFORM_LINUX   3
#define PLATFORM_IOS     4
#define PLATFORM_ANDROID 5

#if defined( WIN32 )
    #define PLATFORM_ID PLATFORM_WINDOWS
#elif defined( __MACH__ )
    #include <TargetConditionals.h>
    #if( TARGET_OS_IPHONE )
        #define PLATFORM_ID PLATFORM_IOS
    #else
        #define PLATFORM_ID PLATFORM_MACOS
    #endif
#elif defined( __ANDROID__ )
    #define PLATFORM_ID PLATFORM_ANDROID
#elif defined( __linux__ )
    #define PLATFORM_ID PLATFORM_LINUX
#else
    #error No valid target found
#endif

/*
    ISA detection
*/
#define ISA_IA32 1
#define ISA_ARM  2

#if (PLATFORM_ID == PLATFORM_ANDROID) || (PLATFORM_ID == PLATFORM_IOS)
    #define ISA_ID ISA_ARM
#elif (PLATFORM_ID == PLATFORM_MACOS) ||\
      (PLATFORM_ID == PLATFORM_WINDOWS) || \
      (PLATFORM_ID == PLATFORM_LINUX)
    #define ISA_ID ISA_IA32
#endif

/*
    Architecture detection
*/
#define ARCH_64BIT 1
#define ARCH_32BIT 2

#if defined( _M_X64 ) || \
    defined( __LP64__ ) || \
    (defined(TARGET_RT_64_BIT) && ( TARGET_RT_64_BIT == 1 ))
    #define ARCH_ID ARCH_64BIT
#else
    #define ARCH_ID ARCH_32BIT
#endif

/*
    Debug information
*/
#if PLATFORM_ID == PLATFORM_WINDOWS
    static const wchar_t g_kPlatformName[] = L"Microsoft Windows";
#elif PLATFORM_ID == PLATFORM_IOS
    static const wchar_t g_kPlatformName[] = L"Apple iOS";
#elif PLATFORM_ID == PLATFORM_MACOS
    static const wchar_t g_kPlatformName[] = L"Apple OS X";
#elif PLATFORM_ID == PLATFORM_ANDROID
    static const wchar_t g_kPlatformName[] = L"Google Android";
#elif PLATFORM_ID == PLATFORM_LINUX
    static const wchar_t g_kPlatformName[] = L"Linux";
#endif

#if ISA_ID == ISA_IA32
    static const wchar_t g_kISAName[] = L"Intel x86";
#elif ISA_ID == ISA_ARM
    static const wchar_t g_kISAName[] = L"ARM";
#endif

#if ARCH_ID == ARCH_64BIT
    static const wchar_t g_kArchName[] = L"64-bit";
#elif ARCH_ID == ARCH_32BIT
    static const wchar_t g_kArchName[] = L"32-bit";
#endif
