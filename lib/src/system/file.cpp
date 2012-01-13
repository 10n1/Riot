/*
 *  file.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "system/file.h"
#include "assert.h"
#include <stdlib.h>
#include <errno.h>

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

namespace File
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
int Open(file_t* file, const char* filename, file_mode_e mode)
{
    char fopenMode[3] = {0,0,0};

    switch(mode)
    {
    case (kFileRead | kFileWrite):
        fopenMode[0] = 'r', fopenMode[1] ='+';
        break;
    case kFileRead:
        fopenMode[0] = 'r';
        break;
    case kFileWrite:
        fopenMode[0] = 'w';
        break;
    }
        
    file->file = fopen(filename, fopenMode);
    if(file->file == nullptr)
    {
        int error = errno;
        if(error != 0)
        {
        }
    }
    return file->file ? 0 : 1;
}
void Close(file_t* file)
{
    int result = fclose(file->file);
    assert(result != EOF);
    file->file = NULL; /* should this be NULLed out? */
}
int Read(file_t* file, size_t bufferSize, void* buffer, size_t* bytesRead )
{
    *bytesRead = fread(buffer, (size_t)1, bufferSize, file->file);
    if(*bytesRead != bufferSize)
    {
        if(feof(file->file))
            return 0;
        else if(ferror(file->file))
            return 1;
        else
            assert(0 && "WTF?");
    }
    
    return 0;
}
int Write(file_t* file, size_t bufferSize, const void* buffer, size_t* bytesWritten)
{
    *bytesWritten = fwrite(buffer, (size_t)1, bufferSize, file->file);
    if(*bytesWritten != bufferSize)
    {
        if(ferror(file->file))
            return 1;
        
        assert(0 && "Not an error but didn't write everything?! WTF?");
    }
    
    return 0;
}

} // namespace File
