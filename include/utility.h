/*
 * utility.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/4/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_utility_h__
#define __RiotLib_utility_h__

/* C headers */
#include <string.h>
/* C++ headers */
/* External headers */
/* Internal headers */

/*******************************************************************\
External Constants And types
\*******************************************************************/
template <unsigned int N, unsigned int I>
struct FnvHash
{
    inline static unsigned int Hash(const char (&str)[N])
    {
        return (FnvHash<N, I-1>::Hash(str) ^ str[I-1])*16777619u;
    }
};

template <unsigned int N>
struct FnvHash<N, 1>
{
    inline static unsigned int Hash(const char (&str)[N])
    {
        return (2166136261u ^ str[0])*16777619u;
    }
};


struct StringHash
{
public:
    template <unsigned int N>
    inline StringHash(const char (&str)[N])
        : hash(FnvHash<N, N>::Hash(str))
    {
#if (defined(_DEBUG) || defined(_DEBUG))  && !defined(NDEBUG)
        strcpy(string, str);
#endif
    }

    unsigned int    hash;
#if (defined(_DEBUG) || defined(_DEBUG))  && !defined(NDEBUG)
    char            string[252];
#endif
};


/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
