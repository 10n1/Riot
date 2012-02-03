/*
 *  scripting.cpp
 *  scripting
 *
 *  Created by Kyle C Weicht on 1/23/12.
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#include "scripting.h"

/* C headers */
#include <stdio.h>
#include <stdint.h>
/* C++ headers */
/* External headers */
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
/* Internal headers */


#undef scriptingDeclareFunction
#undef scriptingRegisterFunction

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
lua_State*  _L = NULL;

/*******************************************************************\
 Internal functions
\*******************************************************************/
uint32_t hashFunctionName(const char* func)
{
    uint32_t offset = 0;
    uint32_t count  = 0;
    uint32_t hash   = 0x7735940BU;
    while(func && *func)
    {
        hash ^= (uint32_t)(*func << (count*8 + offset));
        ++func;
        ++count;
        if(count == 4)
        {
            ++offset;
            count = 0;
            if(offset == 24)
            {
                offset = 0;
            }
        }
    }
    
    return hash;
}

/*
 * Lua-specific code
 */
static void printLuaError(void)
{
    printf("Lua error: %s\n", lua_tostring(_L, -1));
}


template<class T>
T GetLuaType(lua_State* L, int index);

template<> float GetLuaType<float>(lua_State* L, int index) 
{ 
    return (float)luaL_checknumber(L, index); 
}

template<> const char* GetLuaType<const char*>(lua_State* L, int index) 
{ 
    return luaL_checkstring(L, index);
}
template<> ptrdiff_t GetLuaType<ptrdiff_t>(lua_State* L, int index)
{
    return luaL_checkinteger(L, index);
}
template<> void* GetLuaType<void*>(lua_State* L, int index)
{
    return luaL_checkudata(L, index, NULL);
}
//template<> int GetLuaType<int>(lua_State* L, int index)
//{
//    return (int)luaL_checkinteger(L, index);
//}

template<class T>
void PushLuaType(lua_State* L, T param);
template<> void PushLuaType(lua_State* L, float param)          { lua_pushnumber(L, param); }
template<> void PushLuaType(lua_State* L, const char* param)    { lua_pushstring(L, param); }
template<> void PushLuaType(lua_State* L, ptrdiff_t param)      { lua_pushinteger(L, param); }
template<> void PushLuaType(lua_State* L, void* param)          { lua_pushlightuserdata(L, param); }
template<> void PushLuaType(lua_State* L, int param)            { lua_pushinteger(L, param); }

} // namespace


/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void scriptingInit(void)
{
    _L = luaL_newstate();
    luaL_openlibs(_L);
}
void scriptingShutdown(void)
{
    lua_close(_L);
}
void scriptingDoScript(const char* script)
{
    int result = luaL_dostring(_L, script);
    if(result)
        printLuaError();
}
void scriptingDoScriptFile(const char* filename)
{
    int result = luaL_dofile(_L, filename);
    if(result)
        printLuaError();
}
void scriptingRegisterFunction(const char* funcName, int (*func)(lua_State* L))
{
    lua_register(_L, funcName, func);
    //lua_pushstring(_L, funcName);
    //lua_pushcclosure(_L, func, 1);
    //lua_setglobal(_L, funcName);
}

/////////////////////////////////////
// 2 parameters
/////////////////////////////////////
template<class T_return, class T_param1, class T_param2 >
int _scriptingCallLuaFunction(lua_State* L, T_return (*func)(T_param1,T_param2))
{
    T_param1 a = GetLuaType<T_param1>(L, 1);
    T_param2 b = GetLuaType<T_param2>(L, 2);
    T_return ret = func(a,b); 
    PushLuaType(L, ret);
    return 1;
}
template<class T_param1, class T_param2 >
int _scriptingCallLuaFunction(lua_State* L, void (*func)(T_param1,T_param2))
{
    T_param1 a = GetLuaType<T_param1>(L, 1);
    T_param2 b = GetLuaType<T_param2>(L, 2);
    func(a,b); 
    return 0;
}
/////////////////////////////////////
// 1 parameter
/////////////////////////////////////
template<class T_return, class T_param1 >
int _scriptingCallLuaFunction(lua_State* L, T_return (*func)(T_param1))
{
    T_param1 a = GetLuaType<T_param1>(L, 1);
    T_return ret = func(a); 
    PushLuaType(L, ret);
    return 1;
}
template<class T_return, class T_param1 >
int _scriptingCallLuaFunction(lua_State* L, void (*func)(T_param1))
{
    T_param1 a = GetLuaType<T_param1>(L, 1);
    func(a);
    return 0;
}

/////////////////////////////////////
// 0 parameters
/////////////////////////////////////
int _scriptingCallLuaFunction(lua_State* L, void (*func)(void))
{
    func();
    return 0;
    (void)(sizeof(L));
}
int _scriptingCallLuaFunction(lua_State* L, void* (*func)(void))
{
    void* ret = func();
    PushLuaType(L, ret);
    return 1;
}

#define _LINK_LUA_FUNCTION(name, func) \
    func \
    void _##name##_linker(void) \
    { \
        _scriptingCallLuaFunction(NULL, name); \
    }

#define _LINK_LUA_1PARAM(ret, name, param1) \
    static ret _##name##_(param1) { return 0; } \
    void _##name##_linker(void) \
    { \
        _scriptingCallLuaFunction(NULL, _##name##_); \
    }

#define _LINK_LUA_2PARAM(ret, name, param1, param2) \
    static ret _##name##_(param1,param2) { return 0; } \
    void _##name##_linker(void) \
    { \
        _scriptingCallLuaFunction(NULL, _##name##_); \
    }

_LINK_LUA_2PARAM(float, fff, float, float);
_LINK_LUA_1PARAM(int, intvoidptr, void*);

//static float _FloatFloatFloat(float, float) {return 0.0f;}
////static const char* _StringStringString(const char*, const char*){return 0;}
//static void* _VoidPtrVoid(void) { return 0; }
//static void* _VoidPtrInt(int) { return 0; }
////static int _IntVoidPtr(void*) { return 0; }
////static int _IntPtrdiff(ptrdiff_t) { return 0; }
//
//_BETTER_LINK_LUA_1PARAM(int, intvoid, void*)
//
//_LINK_LUA_FUNCTION(_StringStringString, static const char* _StringStringString(const char*, const char*){return 0;});
//_LINK_LUA_FUNCTION(_IntPtrdiff, static int _IntPtrdiff(ptrdiff_t) { return 0; });
///* Because we don't want Lua exposed externally, the templated 
//functions are in this file. With the templated function defined here,
//the compiler doesn't generate the code elsewhere. So we have to make
//these temporary functions so the linker can find them */
//void _compilerHack(void)
//{
//    _scriptingCallLuaFunction(NULL, _FloatFloatFloat);
//    //_scriptingCallLuaFunction(NULL, _StringStringString);
//    _scriptingCallLuaFunction(NULL, _VoidPtrVoid);
//    _scriptingCallLuaFunction(NULL, _VoidPtrInt);
//    _scriptingCallLuaFunction(NULL, _IntVoidPtr);
//    //_scriptingCallLuaFunction(NULL, _IntPtrdiff);
//}
