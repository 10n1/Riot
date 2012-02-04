/*
 * core.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_core_h__
#define __RiotLib_core_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */

/*******************************************************************\
External Constants And types
\*******************************************************************/
class GraphicsDevice;

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
class Core
{
/* Methods */
public:
    void Init(bool createWindow);

    int Frame(void);

    void Shutdown(void);

/* Members */
private:
    GraphicsDevice* _graphicsDevice;
    int             _frameNumber;
};

#endif /* include guard */
