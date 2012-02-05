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
\*******************************************************************/
unsigned int CalculateFNV(const char* str)
{
  const size_t length = strlen(str) + 1;
  unsigned int hash = 2166136261u;
  for (size_t i=0; i<length; ++i)
  {
    hash ^= *str++;
    hash *= 16777619u;
  }
 
  return hash;
}

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
    inline StringHash()
        : hash(0)
    {
        string[0] = '\0';
    }
    template <unsigned int N>
    inline StringHash(const char (&str)[N])
        : hash(FnvHash<N, N>::Hash(str))
    {
#if (defined(_DEBUG) || defined(_DEBUG))  && !defined(NDEBUG)
        strcpy(string, str);
#endif
    }
    inline StringHash(const char* str)
        : hash(CalculateFNV(str))
    {
#if (defined(_DEBUG) || defined(_DEBUG))  && !defined(NDEBUG)
        strcpy(string, str);
#endif
    }

    bool operator==(const StringHash& _hash) { return hash == _hash.hash; }

    unsigned int    hash;
#if (defined(_DEBUG) || defined(_DEBUG))  && !defined(NDEBUG)
    char            string[252];
#endif
};


#endif /* include guard */
