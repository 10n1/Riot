/*
 * file.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/4/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_file_h__
#define __RiotLib_file_h__

/* C headers */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
/* C++ headers */
/* External headers */
/* Internal headers */

/*******************************************************************\
External Constants And types
\*******************************************************************/

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
class File
{
/*Methods*/
public:
    File(const char* filename, const char* mode);
    ~File();

    size_t  Size(void);
    size_t  Read(void* buffer, size_t readSize, size_t readCount);
    size_t  Write(const void* buffer, size_t writeSize, size_t writeCount);

    void Close(void);

/*Members*/
private:
    char    _filename[256-sizeof(FILE*)];
    FILE*   _file;
};

size_t FileLoadAndRead(void* buffer, size_t bufferSize, const char* filename);

#endif /* include guard */
