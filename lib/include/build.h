/*
 *  config.h
 *  engine
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef riot_config_h_
#define riot_config_h_

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*
 * Configuration detection
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
 * Platform detection
 */
#define BUILD_PLATFORM_WINDOWS 1
#define BUILD_PLATFORM_MACOS   2
#define BUILD_PLATFORM_LINUX   3
#define BUILD_PLATFORM_IOS     4
#define BUILD_PLATFORM_ANDROID 5

#if defined( WIN32 )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#elif defined( __MACH__ )
    #include <TargetConditionals.h>
    #if( TARGET_OS_IPHONE )
        #define BUILD_PLATFORM_ID BUILD_PLATFORM_IOS
    #else
        #define BUILD_PLATFORM_ID BUILD_PLATFORM_MACOS
    #endif
#elif defined( __ANDROID__ )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_ANDROID
#elif defined( __linux__ )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_LINUX
#else
    #error No valid target found
#endif

/*
 * ISA detection
 */
#define BUILD_ISA_IA32 1
#define BUILD_ISA_ARM  2

#if (BUILD_PLATFORM_ID == BUILD_PLATFORM_ANDROID) || (BUILD_PLATFORM_ID == BUILD_PLATFORM_IOS)
    #define ISA_ID ISA_ARM
#elif (BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS) ||\
      (BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS) || \
      (BUILD_PLATFORM_ID == BUILD_PLATFORM_LINUX)
    #define BUILD_ISA_ID BUILD_ISA_IA32
#endif

/*
 * Architecture detection
 */
#define BUILD_ARCH_64BIT 1
#define BUILD_ARCH_32BIT 2

#if defined( _M_X64 ) || \
    defined( __LP64__ ) || \
    (defined(TARGET_RT_64_BIT) && ( TARGET_RT_64_BIT == 1 ))
    #define BUILD_ARCH_ID BUILD_ARCH_64BIT
#else
    #define BUILD_ARCH_ID BUILD_ARCH_32BIT
#endif

/*
 * System information
 */
#define BUILD_SYSTEM_POSIX    1
#define BUILD_SYSTEM_WINDOWS  2

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS || \
    BUILD_PLATFORM_ID == BUILD_PLATFORM_LINUX || \
    BUILD_PLATFORM_ID == BUILD_PLATFORM_IOS   || \
    BUILD_PLATFORM_ID == BUILD_PLATFORM_ANDROID
    #define BUILD_SYSTEM_TYPE BUILD_SYSTEM_POSIX
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #define BUILD_SYSTEM_TYPE BUILD_SYSTEM_WINDOWS
#else
    #error Unsupported system
#endif

/*
 * Debug information
 */
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    static const char g_kPlatformName[] = "Microsoft Windows";
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_IOS
    static const char g_kPlatformName[] = "Apple iOS";
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    static const char g_kPlatformName[] = "Apple Mac OS X";
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_ANDROID
    static const char g_kPlatformName[] = "Google Android";
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_LINUX
    static const char g_kPlatformName[] = "Linux";
#endif

#if BUILD_ISA_ID == BUILD_ISA_IA32
    static const char g_kISAName[] = "Intel x86";
#elif BUILD_ISA_ID == BUILD_ISA_ARM
    static const char g_kISAName[] = "ARM";
#endif

#if BUILD_ARCH_ID == BUILD_ARCH_64BIT
    static const char g_kArchName[] = "64-bit";
#elif BUILD_ARCH_ID == BUILD_ARCH_32BIT
    static const char g_kArchName[] = "32-bit";
#endif

/*
 * Various build macros
 */
#ifndef UNUSED_PARAMETER
    #define UNUSED_PARAMETER(param) (void)sizeof((param))
#endif

#ifndef ARRAY_LENGTH
    #define ARRAY_LENGTH(x) ( sizeof((x))/sizeof((x)[0]) )
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* include guard */
