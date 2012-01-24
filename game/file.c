/*
 *  file.c
 *  game
 *
 *  Created by Kyle C Weicht on 1/24/12.
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#include "file.h"

/* C headers */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif
/* External headers */
/* Internal headers */


/*******************************************************************\
 Internal constants and types
\*******************************************************************/
struct file_t
{
    char    filename[256-sizeof(FILE*)];
    FILE*   file;
};

enum
{
    kOk,
    kRetry,
    kCancel
};

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/
static int RetryMessageBox(const char* header, const char* message)
{
#ifdef _WIN32
    int messageBoxResult = MessageBox(NULL, message, header, MB_RETRYCANCEL);
    if(messageBoxResult == IDRETRY)
        return kRetry;
#else    
    /*convert the strings from char* to CFStringRef */
    CFStringRef header_ref  = CFStringCreateWithCString(NULL, header, kCFStringEncodingASCII);
    CFStringRef message_ref = CFStringCreateWithCString(NULL, message, kCFStringEncodingASCII);
    CFStringRef buttonText = CFSTR("Retry");

    CFOptionFlags result;  /*result code from the message box */
   
    /*launch the message box */
    CFUserNotificationDisplayAlert( 0.0f, /* no timeout */
                                    kCFUserNotificationNoteAlertLevel, /*change it depending message_type flags ( MB_ICONASTERISK.... etc.) */
                                    NULL, /*icon url, use default, you can change it depending message_type flags */
                                    NULL, /*not used */
                                    NULL, /*localization of strings */
                                    header_ref, /*header text  */
                                    message_ref, /*message text */
                                    buttonText, /*default "ok" text in button */
                                    CFSTR("Cancel"), /*alternate button title */
                                    NULL, /*other button title, null--> no other button */
                                    &result /*response flags */ );

    /*Clean up the strings */
    CFRelease( header_ref );
    CFRelease( message_ref );

    /*Convert the result */
    if(result == kCFUserNotificationDefaultResponse)
        return kRetry;
    
#endif
    return kCancel;
}

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
file_t* fileLoad(const char* filename, const char* mode)
{
    char workingDirectory[256];
    char fileLoadBuffer[1024];
    int result;
    file_t* file = malloc(sizeof(*file));
    memset(file, 0, sizeof(*file));
    strncpy(file->filename, filename, sizeof(file->filename));
    do
    {  
        result = kOk;
        file->file = fopen(filename, mode);
        if(file->file == NULL)
        {
            getcwd(workingDirectory, sizeof(workingDirectory));
            sprintf(fileLoadBuffer, "File load failed!\nWorking Directory:\t%s\nFilename:\t\t%s\nError:\t\t\t%d", workingDirectory, filename, errno);
            result = RetryMessageBox("File Load Error", fileLoadBuffer);
        }
    } while(result == kRetry);
    
    return file;
}
size_t fileLoadAndRead(void* buffer, size_t bufferSize, size_t readSize, const char* filename);
size_t fileRead(void* buffer, size_t bufferSize, size_t readSize, size_t readCount, file_t* file);
size_t fileWrite(const void* buffer, size_t writeSize, size_t writeCount, file_t* file);
void fileClose(file_t* file);
