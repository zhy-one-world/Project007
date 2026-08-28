/*
** Lua binding: logic
** Generated automatically by tolua++-1.0.92 on Wed Aug  7 10:44:12 2024.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_logic_open (lua_State* tolua_S);

#include "lua_call_handle.h"
using namespace hld;

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
#ifndef Mtolua_typeid
#define Mtolua_typeid(L,TI,T)
#endif
}

/* Open function */
TOLUA_API int tolua_logic_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_logic (lua_State* tolua_S) {
 return tolua_logic_open(tolua_S);
};
#endif

