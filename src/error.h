//
//  error.h
//  Riot
//
//  Created by Kyle Weicht on 7/10/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include <stdio.h>

#define PRINT_ERROR( error_text ) printf( "%s:%d[%s]:\t%s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, error_text )
