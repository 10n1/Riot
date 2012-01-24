/*
 *  scripting.h
 *  scripting
 *
 *  Created by Kyle C Weicht on 1/23/12..
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#ifndef scripting_scripting_h
#define scripting_scripting_h

/*******************************************************************\
 External constants and types
\*******************************************************************/
struct lua_State;

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void scriptingInit(void);
void scriptingShutdown(void);
void scriptingDoScript(const char* script);
void scriptingDoScriptFile(const char* filename);

template<class T_return, class T_param1, class T_param2 >
int _scriptingCallLuaFunction(lua_State* L, T_return (*func)(T_param1,T_param2));
template<class T_return, class T_param1, class T_param2 >
int _scriptingCallLuaFunction(lua_State* L, void (*func)(T_param1,T_param2));
int _scriptingCallLuaFunction(lua_State* L, void (*func)(void));

void scriptingRegisterFunction(const char* funcName, int (*func)(lua_State* L));

#define scriptingDeclareFunction(function) \
    static int __##function##_lua(lua_State* L) \
    { \
        return _scriptingCallLuaFunction(L, function); \
    }
    
#define scriptingRegisterFunction(function) \
    scriptingRegisterFunction(#function, __##function##_lua)

#endif /* include guard */