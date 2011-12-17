/*
 *  utility.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "system/utility.h"
#include "build.h"

/* System includes */
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    #include <CoreFoundation/CoreFoundation.h>
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <windows.h>
#endif


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

namespace Utility
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
msg_box_result_e MessageBox(const char* header, const char* message)
{
    /*convert the strings from char* to CFStringRef */
    CFStringRef header_ref  = CFStringCreateWithCString(NULL, header, kCFStringEncodingASCII);
    CFStringRef message_ref = CFStringCreateWithCString(NULL, message, kCFStringEncodingASCII);

    CFOptionFlags result;  /*result code from the message box */
   
    /*launch the message box */
    CFUserNotificationDisplayAlert( 0.0f, /* no timeout */
                                    kCFUserNotificationNoteAlertLevel, /*change it depending message_type flags ( MB_ICONASTERISK.... etc.) */
                                    NULL, /*icon url, use default, you can change it depending message_type flags */
                                    NULL, /*not used */
                                    NULL, /*localization of strings */
                                    header_ref, /*header text  */
                                    message_ref, /*message text */
                                    NULL, /*default "ok" text in button */
                                    CFSTR("Cancel"), /*alternate button title */
                                    NULL, /*other button title, null--> no other button */
                                    &result /*response flags */
                                    );

    /*Clean up the strings */
    CFRelease( header_ref );
    CFRelease( message_ref );

    /*Convert the result */
    if(result == kCFUserNotificationDefaultResponse)
        return kMsgBoxOk;
    
    return kMsgBoxCancel;
}
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
msg_box_result_e MessageBox(const char* header, const char* message)
{
    int result = ::MessageBox(NULL, message, header, MB_OKCANCEL);

    if(result == IDOK)
        return kMsgBoxOk;

    return kMsgBoxCancel;
}
#else
    #error No Message Box
#endif // PLATFORM_ID

} // namespace System
