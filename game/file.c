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
#include <string.h>
#if defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <errno.h>
#endif
/* External headers */
/* Internal headers */


/*******************************************************************\
 Internal constants and types
\*******************************************************************/
/*
 * Define debugBreak
 */
#if defined( _MSC_VER )
    #define debugBreak() __debugbreak()
#elif defined( __GNUC__ )
    #define debugBreak() __asm__( "int $3\n" : : )
#else
    #error Unsupported compiler
#endif

/*
 * Define assert
 */
#ifndef assert
    #define assert(condition)   \
        do                      \
        {                       \
            if(!(condition))    \
            {                   \
                debugBreak();   \
            }                   \
        } while(__LINE__ == -1)
        /* This causes warning 4127 with VC++ (conditional expression is constant) */
    #if defined( _MSC_VER )
        #pragma warning(disable:4127)
    #endif /* defined( _MSC_VER ) */
#endif

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

static const char* GetErrorString(void)
{
    switch(errno)
    {
    case EINVAL: return "Invalid Mode argument";
    case EACCES: return "Access permissions invalid";
    case EEXIST: return "File already exists";
    case EISDIR: return "File is a directory";
    case ENAMETOOLONG: return "Filename too long";
    case ENOTDIR: return "A component of the path prefix is not a directory";
    case ENOENT: return "File does not exist";
    default:
        return "Unknown error";
    }	
}

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void fileOpen(const char* filename, const char* mode, file_t* file)
{
    char workingDirectory[256];
    char fileLoadBuffer[1024*2];
    int result;
    memset(file, 0, sizeof(*file));
    strncpy(file->filename, filename, sizeof(file->filename));
    do
    {  
        result = kOk;
        file->file = fopen(filename, mode);
        if(file->file == NULL)
        {
            getcwd(workingDirectory, sizeof(workingDirectory));
            sprintf(fileLoadBuffer, "File load failed!\nWorking Directory:\t%s\nFilename:\t\t\t%s\nError:\t\t\t%s\nFile mode:\t\t\t%s", workingDirectory, filename, GetErrorString(), mode);
            result = RetryMessageBox("File Load Error", fileLoadBuffer);
        }
    } while(result == kRetry);
}
size_t fileLoadAndRead(void* buffer, size_t bufferSize, const char* filename)
{
    size_t bytesRead = 0;
    file_t file;
    fileOpen(filename, "r", &file);
    bytesRead = fileRead(buffer, sizeof(char), bufferSize, &file);
    fileClose(&file);
    return bytesRead;
}
size_t fileSize(file_t* file)
{
    size_t fileSize = 0;
    fseek (file->file , 0 , SEEK_END);
    fileSize = ftell (file->file);
    rewind (file->file);
    return fileSize;
}
size_t fileRead(void* buffer, size_t readSize, size_t readCount, file_t* file)
{
    int error = 0;
    size_t countRead = fread(buffer, readSize, readCount, file->file);
    if(countRead != readCount)
    {
        error = feof(file->file);
        assert(error != 0);
        if(error == 0)
        {
            error = ferror(file->file);
            assert(error == 0);
        }
    }
    return countRead;
}
size_t fileWrite(const void* buffer, size_t writeSize, size_t writeCount, file_t* file)
{
    int error = 0;
    size_t count = fwrite(buffer, writeSize, writeCount, file->file);
    if(count != writeCount)
    {
        error = ferror(file->file);
        assert(error == 0);
    }
    return count;
}
void fileClose(file_t* file)
{
    fclose(file->file);
    file->file = NULL;
    file->filename[0] = '\0';
}
