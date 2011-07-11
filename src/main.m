//
//  main.m
//  Riot
//
//  Created by Kyle Weicht on 7/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include <stdio.h>
#include <wchar.h>
#include "osx_application.h"

int main(int argc, char *argv[])
{
    wprintf( L"%ls, %ls, %ls\n", g_kPlatformName, g_kArchName, g_kISAName );
    return ApplicationMain(argc, (const char **)argv);
}
