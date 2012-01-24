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
#include "src/lua.h"
#include "src/lualib.h"
#include "src/lauxlib.h"
}
/* Internal headers */

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
    return luaL_checknumber(L, index); 
}

template<> const char* GetLuaType<const char*>(lua_State* L, int index) 
{ 
    return luaL_checkstring(L, index);
}
template<> ptrdiff_t GetLuaType<ptrdiff_t>(lua_State* L, int index)
{
    return luaL_checkinteger(L, index);
}

template<class T>
void PushLuaType(lua_State* L, T param);

template<> void PushLuaType(lua_State* L, float param)
{
    lua_pushnumber(L, param);
}

template<> void PushLuaType(lua_State* L, const char* param)
{
    lua_pushstring(L, param);
}

template<> void PushLuaType(lua_State* L, ptrdiff_t param)
{
    lua_pushinteger(L, param);
}

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
    int result = luaL_dofile(_L, script);
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
int _scriptingCallLuaFunction(lua_State* L, void (*func)(void))
{
    func();
    return 0;
}

static float _FloatFloatFloat(float, float) {return 0.0f;}
static const char* _StringStringString(const char*, const char*){return 0;}
/* Because we don't want Lua exposed externally, the templated 
functions are in this file. With the templated function defined here,
the compiler doesn't generate the code elsewhere. So we have to make
these temporary functions so the linker can find them */
void _compilerHack(void)
{
    _scriptingCallLuaFunction(NULL, _FloatFloatFloat);
    _scriptingCallLuaFunction(NULL, _StringStringString);
}