/*
 * file.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/4/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
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
    #include <direct.h>
#endif
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"

#define getcwd _getcwd

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
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
int RetryMessageBox(const char* header, const char* message)
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

const char* GetErrorString(void)
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

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
File::File(const char* filename, const char* mode)
    : _file(NULL)
{
    char workingDirectory[256];
    int result = kOk;
    strncpy(_filename, filename, sizeof(_filename));
    do
    {  
        result = kOk;
        _file = fopen(filename, mode);
        if(_file == NULL)
        {
            char fileLoadBuffer[1024*2];
            getcwd(workingDirectory, sizeof(workingDirectory));
            sprintf(fileLoadBuffer, "File load failed!\nWorking Directory:\t%s\nFilename:\t\t\t%s\nError:\t\t\t%s\nFile mode:\t\t\t%s", workingDirectory, filename, GetErrorString(), mode);
            result = RetryMessageBox("File Load Error", fileLoadBuffer);
        }
    } while(result == kRetry);

}
File::~File()
{
    fclose(_file);
}

size_t File::Size(void)
{
    size_t fileSize = 0;
    fseek (_file , 0 , SEEK_END);
    fileSize = ftell (_file);
    rewind (_file);
    return fileSize;
}
size_t File::Read(void* buffer, size_t readSize, size_t readCount)
{
    int error = 0;
    size_t countRead = fread(buffer, readSize, readCount, _file);
    if(countRead != readCount)
    {
        error = feof(_file);
        assert(error != 0);
        if(error == 0)
        {
            error = ferror(_file);
            assert(error == 0);
        }
    }
    return countRead;
}
size_t File::Write(const void* buffer, size_t writeSize, size_t writeCount)
{
    int error = 0;
    size_t count = fwrite(buffer, writeSize, writeCount, _file);
    if(count != writeCount)
    {
        error = ferror(_file);
        assert(error == 0);
    }
    return count;
}

void File::Close(void)
{
    fclose(_file);
}

size_t FileLoadAndRead(void* buffer, size_t bufferSize, const char* filename)
{
    File file(filename, "r");
    return file.Read(buffer, 1, bufferSize);
}
