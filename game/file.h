/*
 *  file.h
 *  game
 *
 *  Created by Kyle C Weicht on 1/24/12..
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#ifndef game_file_h
#define game_file_h

/* C headers */
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
/* External headers */
/* Internal headers */

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*******************************************************************\
 External constants and types
\*******************************************************************/
typedef struct file_t
{
    char    filename[256-sizeof(FILE*)];
    FILE*   file;
} file_t;

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void fileOpen(const char* filename, const char* mode, file_t* file);
size_t fileLoadAndRead(void* buffer, size_t bufferSize, const char* filename);
size_t fileSize(file_t* file);
size_t fileRead(void* buffer, size_t readSize, size_t readCount, file_t* file);
size_t fileWrite(const void* buffer, size_t writeSize, size_t writeCount, file_t* file);
void fileClose(file_t* file);


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif /* include guard */
