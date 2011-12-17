/*
 *  hardware.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "system/hardware.h"
#include "assert.h"
#include "build.h"

#include <stdint.h>

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    #include <sys/types.h>
    #include <sys/sysctl.h>
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <malloc.h>
    #include <intrin.h>
#endif

namespace
{

using namespace System;

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
static struct
{
    /* EAX contents */
    unsigned int    stepping_id:4;
    unsigned int    model:4;
    unsigned int    family:4;
    unsigned int    processor_type:2;
    unsigned int    _reserved0:2;
    unsigned int    extended_model:4;
    unsigned int    extended_family:8;
    unsigned int    _reserved1:4;
    
    /* EBX contents */
    unsigned int    brand_id:8;
    unsigned int    chunks:8;
    unsigned int    count:8;
    unsigned int    apic_physical_id:8;
    
    /* ECX contents */
    unsigned int    sse3:1;
    unsigned int    pclmuldq:1;
    unsigned int    dtes64:1;
    unsigned int    monitor:1;
    unsigned int    ds_cpl:1;
    unsigned int    vmx:1;
    unsigned int    smx:1;
    unsigned int    eist:1;
    
    unsigned int    tm2:1;
    unsigned int    ssse3:1;
    unsigned int    cntx_id:1;
    unsigned int    _reserved3:1;
    unsigned int    fma:1;
    unsigned int    cx16:1;
    unsigned int    xtpr:1;
    unsigned int    pdcm:1;
    
    unsigned int    _reserved4:1;
    unsigned int    pcid:1;
    unsigned int    dca:1;
    unsigned int    sse41:1;
    unsigned int    sse42:1;
    unsigned int    x2apic:1;
    unsigned int    movbe:1;
    unsigned int    popcnt:1;
    
    unsigned int    tsc_deadline:1;
    unsigned int    aes:1;
    unsigned int    xsave:1;
    unsigned int    osxsave:1;
    unsigned int    avx:1;
    unsigned int    _reserved5:2;
    unsigned int    _zero0:1;
    
    /* EDX contents */
    unsigned int    fpu:1;
    unsigned int    vme:1;
    unsigned int    de:1;
    unsigned int    pse:1;
    unsigned int    tsc:1;
    unsigned int    msr:1;
    unsigned int    pae:1;
    unsigned int    mce:1;
    
    unsigned int    cx8:1;
    unsigned int    apic:1;
    unsigned int    _reserved6:1;
    unsigned int    sep:1;
    unsigned int    mtrr:1;
    unsigned int    pge:1;
    unsigned int    mca:1;
    unsigned int    cmov:1;
    
    unsigned int    pat:1;
    unsigned int    pse_36:1;
    unsigned int    psn:1;
    unsigned int    clfsh:1;
    unsigned int    _reserved7:1;
    unsigned int    ds:1;
    unsigned int    acpi:1;
    unsigned int    mmx:1;
    
    unsigned int    fxsr:1;
    unsigned int    sse:1;
    unsigned int    sse2:1;
    unsigned int    ss:1;
    unsigned int    htt:1;
    unsigned int    tm:1;
    unsigned int    _reserved8:1;
    unsigned int    pbe:1;
    
} s_cpuid_info_eax1;

/*******************************************************************\
 Internal variables
\*******************************************************************/
instruction_set_e   s_isa = instruction_set_e::kInstructionSetNull;
int s_hardwareThreadCount = -1;
int s_physicalMemorySize  = -1;
int s_cacheLineSize       = -1;
int s_maxDataWidth        = -1;

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace System
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
instruction_set_e HWInstructionSet(void)
{
    if(s_isa > instruction_set_e::kInstructionSetNull)
        return s_isa;
        
    int isa = instruction_set_e::kInstructionSetStd;
    HWcpuid((int*)&s_cpuid_info_eax1, 1, 0);
    
    if(s_cpuid_info_eax1.mmx)
        ++isa;
    if(s_cpuid_info_eax1.sse)
        ++isa;
    if(s_cpuid_info_eax1.sse2)
        ++isa;
    if(s_cpuid_info_eax1.sse3)
        ++isa;
    if(s_cpuid_info_eax1.ssse3)
        ++isa;
    if(s_cpuid_info_eax1.sse41)
        ++isa;
    if(s_cpuid_info_eax1.sse42)
        ++isa;
    if(s_cpuid_info_eax1.osxsave)
    {
        if(s_cpuid_info_eax1.avx)
            ++isa;
        
        /* AVX2 detection */
        int cpuid_result[4];
        HWcpuid(cpuid_result, 7, 0);
        if(cpuid_result[2] & 0x20)
            ++isa;
    }
    
    s_isa = (instruction_set_e)isa;
    return s_isa;
}
int HWMaxDataWidth(void)
{
    if(s_maxDataWidth > 0)
        return s_maxDataWidth;
        
    assert(s_maxDataWidth == -1);
    
    instruction_set_e isa = HWInstructionSet();
    switch(isa)
    {
    case kInstructionSetAVX2:
    case kInstructionSetAVX:
        s_maxDataWidth = 32;
        break;
    case kInstructionSetSSE42:
    case kInstructionSetSSE41:
    case kInstructionSetSSSE3:
    case kInstructionSetSSE3:
    case kInstructionSetSSE2:
        s_maxDataWidth = 16;
        break;
    case kInstructionSetSSE:
    case kInstructionSetMMX:
        s_maxDataWidth = 8;
        break;
    default:
        assert(0); // While we're coding on desktops, this should never get hit
    #if BUILD_ARCH_32BIT
        s_maxDataWidth = 4;
    #else   
        s_maxDataWidth = 8;
    #endif
        break;
    }
            
    return s_maxDataWidth;
}
int HWCacheLineSize(void)
{
    if(s_cacheLineSize > -1)
        return s_cacheLineSize;
        
    assert(s_cacheLineSize == -1);

    size_t line_size = 0;
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    size_t sizeof_line_size = sizeof(line_size);
    sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, (size_t)0);
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    DWORD buffer_size = 0;
    DWORD i = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;
    GetLogicalProcessorInformation(0, &buffer_size);
    buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
    GetLogicalProcessorInformation(&buffer[0], &buffer_size);

    for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) 
    {
        if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) 
        {
            line_size = buffer[i].Cache.LineSize;
            break;
        }
    }

    free(buffer);
#else
    #error Finish
#endif

    s_cacheLineSize = (int)line_size;
    return s_cacheLineSize;
}
int HWPhysMemory(void)
{
    if(s_physicalMemorySize > 0)
        return s_physicalMemorySize;
    assert(s_physicalMemorySize == -1);
    
    uint64_t memory = 0;
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    int     mib[] = {CTL_HW, HW_MEMSIZE};
    size_t  length = sizeof(memory);
    sysctl(mib, 2, &memory, &length, NULL, (size_t)0);
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    memory = status.ullTotalPhys;
#else
    #error Finish
#endif    
    memory /= (1024*1024); /* convert to MB */
    s_physicalMemorySize = (int)memory;
    
    return s_physicalMemorySize;
}
int HWThreadCount(void)
{
    if(s_hardwareThreadCount > 0)
        return s_hardwareThreadCount;
    
    assert(s_hardwareThreadCount == -1);
    
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_MACOS
    int     mib[] = { CTL_HW, HW_AVAILCPU }; /* Interested in availible CPUs */
    size_t  length = sizeof(s_hardwareThreadCount);
    /* Reads system info */
    sysctl(mib, (uint)ARRAY_LENGTH(mib), &s_hardwareThreadCount, &length, NULL, (size_t)0);
    
    if(s_hardwareThreadCount < 1) 
    {   /* HW_AVAILCPU might have been the problem, try HW_NCPU */
        mib[1] = HW_NCPU;
        sysctl(mib, (uint)ARRAY_LENGTH(mib), &s_hardwareThreadCount, &length, NULL, (size_t)0);
    }
#elif BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    s_hardwareThreadCount = si.dwNumberOfProcessors;
#else
    #error Finish
#endif /* PLATFORM_ID */
    return s_hardwareThreadCount;
}
void HWcpuid(int info[4], int eax, int ecx)
{
#ifdef __GNUC__
    __asm__ volatile
    (
     "cpuid\n"
     
     :  "=a"(info[0]),
        "=b"(info[1]),
        "=c"(info[2]),
        "=d"(info[3])
     :  "a"(eax),
        "c"(ecx)
     );
#elif defined(_MSC_VER)
    __cpuid(info, eax);
    (void)(sizeof(ecx));
#else
    #error No cpuid
#endif
}


} // namespace System
