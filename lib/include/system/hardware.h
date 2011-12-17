/*
 *  hardware.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_hardware_h
#define riot_hardware_h


namespace System
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
enum instruction_set_e
{
    kInstructionSetNull=-1,
    kInstructionSetStd=0,
    kInstructionSetMMX,
    kInstructionSetSSE,
    kInstructionSetSSE2,
    kInstructionSetSSE3,
    kInstructionSetSSSE3,
    kInstructionSetSSE41,
    kInstructionSetSSE42,
    kInstructionSetAVX,
    kInstructionSetAVX2
};

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
instruction_set_e HWInstructionSet(void);
int HWMaxDataWidth(void);
int HWCacheLineSize(void);
int HWPhysMemory(void);
int HWThreadCount(void);
void HWcpuid(int info[4], int eax, int ecx);

}

#endif /* include guard */
