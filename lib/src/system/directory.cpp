/*
 *  directory.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "system/directory.h"
#include "build.h"

#include <stdio.h>
#if BUILD_SYSTEM_TYPE == BUILD_SYSTEM_POSIX
    #include <unistd.h>
#elif BUILD_SYSTEM_TYPE == BUILD_SYSTEM_WINDOWS
    #include <direct.h>
    #define getcwd _getcwd
    #define chdir _chdir
#endif

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
char    s_currentWorkingDirectory[FILENAME_MAX] = {0};

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace Directory
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
const char* GetCurrent(void)
{
    getcwd(s_currentWorkingDirectory, sizeof(s_currentWorkingDirectory));
    return s_currentWorkingDirectory;
}
void SetCurrent(const char* dir)
{
    chdir(dir);
}

} //namespace Directory
