/*
 *  hardware.h
 *  Riot
 *
 *  Created by Kyle C Weicht on 10/20/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#ifndef riot_hardware_h
#define riot_hardware_h

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

namespace Hardware
{

/*******************************************************************\
 External functions
\*******************************************************************/
instruction_set_e InstructionSet(void);
int MaxDataWidth(void);
int CacheLineSize(void);
int PhysMemory(void);
int ThreadCount(void);
void cpuid(int info[4], int eax, int ecx);

} // namespace Hardware

#endif /* include guard */
