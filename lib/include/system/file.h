/*
 *  file.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_file_h
#define riot_file_h

#include <stdio.h>

namespace System
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
struct file_t
{
    FILE*   file;
};

enum file_mode_e
{
    kFileRead     = 0x1,
    kFileWrite    = 0x2
};

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
int FileOpen(file_t* file, const char* filename, file_mode_e mode);
void FileClose(file_t* file);
int FileRead(file_t* file, size_t bufferSize, void* buffer, size_t* bytesRead );
int FileWrite(file_t* file, size_t bufferSize, const void* buffer, size_t* bytesWritten);

}

#endif /* include guard */
