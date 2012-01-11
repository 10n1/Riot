/*
 *  renderEngine.cpp
 *  renderEngineLib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#include "renderEngine.h"
#include "renderEngineInternal.h"
#include "graphicsDevice.h"

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


//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace Render
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void* window)
{
    // Initialization
    GraphicsDevice::Initialize(window);

    // Pipeline setup
    GraphicsDevice::SetClearColor(0.0f, 0.3f, 0.4f, 1.0f);
    GraphicsDevice::SetClearDepth(1.0f);
}
void Shutdown(void)
{
    GraphicsDevice::Shutdown();
}
void Frame(void)
{
    GraphicsDevice::Present();
    GraphicsDevice::Clear();
}

} // namespace Render
