/*
 * renderEngine.h
 * renderEngine
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __renderEngine_renderEngine_h__
#define __renderEngine_renderEngine_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*******************************************************************\
External Constants And types
\*******************************************************************/
struct graphics_t;

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void renderInit(graphics_t* graphics);
void renderShutdown(void);


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif /* include guard */
