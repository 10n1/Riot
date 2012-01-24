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
/* External headers */
/* Internal headers */

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*******************************************************************\
 External constants and types
\*******************************************************************/
typedef struct file_t file_t;

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
file_t* fileLoad(const char* filename, const char* mode);
size_t fileLoadAndRead(void* buffer, size_t bufferSize, size_t readSize, const char* filename);
size_t fileRead(void* buffer, size_t bufferSize, size_t readSize, size_t readCount, file_t* file);
size_t fileWrite(const void* buffer, size_t writeSize, size_t writeCount, file_t* file);
void fileClose(file_t* file);


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif /* include guard */
