/*
 *  directory.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_directory_h
#define riot_directory_h

namespace System
{

/*******************************************************************\
 External constants and types
\*******************************************************************/

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
const char* DirectoryGetCurrent(void);
void DirectorySetCurrent(const char* dir);

}

#endif /* include guard */
