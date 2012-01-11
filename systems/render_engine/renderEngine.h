/*
 *  renderEngine.h
 *  renderEngine.lib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __renderEngineLib_h__
#define __renderEngineLib_h__

//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace Render
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
void Initialize(void* window);
void Shutdown(void);
void Frame(void);

} // namespace Render

#endif /* include guard */
