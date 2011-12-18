/*
 *  application.h
 *  riot
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */

#ifndef riot_application_h
#define riot_application_h

/*******************************************************************\
 External constants and types
\*******************************************************************/

/*******************************************************************\
 External variables
\*******************************************************************/

namespace Application
{

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void);
void StartMainLoop(void);
void Shutdown(void);

} // namespace Application

#endif /* Include guard */
