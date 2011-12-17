/*
 *  utility.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_utility_h
#define riot_utility_h

/*******************************************************************\
 External constants and types
\*******************************************************************/
enum msg_box_result_e
{
    kMsgBoxOk,
    //kMsgBoxRetry, // TODO
    kMsgBoxCancel,
};

/*******************************************************************\
 Variables
\*******************************************************************/

namespace Utility
{

/*******************************************************************\
 External functions
\*******************************************************************/
msg_box_result_e MessageBox(const char* header, const char* message);

} // namespace Utility

#endif /* include guard */
