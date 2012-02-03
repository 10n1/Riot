/*
 * main.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
#include <stdio.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"
#include "system.h"
#include "timer.h"
#include "world.h"
#include "scripting.h"
#include "cJSON.h"
#include "file.h"
#include "renderEngine.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/

/*******************************************************************\
Internal functions
\*******************************************************************/

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
int main(int, char*[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_16_DF );
#endif

    return 0;
}
