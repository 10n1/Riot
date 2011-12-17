/*
 *  utility.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_utility_h
#define riot_utility_h

namespace System
{

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

/*******************************************************************\
 External functions
\*******************************************************************/
msg_box_result_e UtilityMessageBox(const char* header, const char* message);

}

#endif /* include guard */
