/*
** Lua binding: faith
** Generated automatically by tolua++-1.0.92 on 02/12/15 17:03:46.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_faith_open (lua_State* tolua_S);

#include "logic/type_def.hpp"
#include "server_def.hpp"
#include "../../def/lua/script_mgr.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_faith__script_mgr (lua_State* tolua_S)
{
 hld::script_mgr* self = (hld::script_mgr*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"hld::script_mgr");
}

/* method: new of class  hld::script_mgr */
#ifndef TOLUA_DISABLE_tolua_faith_faith_script_mgr_new00
static int tolua_faith_faith_script_mgr_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::script_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::script_mgr* tolua_ret = (hld::script_mgr*)  Mtolua_new((hld::script_mgr)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::script_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  hld::script_mgr */
#ifndef TOLUA_DISABLE_tolua_faith_faith_script_mgr_new00_local
static int tolua_faith_faith_script_mgr_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::script_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::script_mgr* tolua_ret = (hld::script_mgr*)  Mtolua_new((hld::script_mgr)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::script_mgr");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  hld::script_mgr */
#ifndef TOLUA_DISABLE_tolua_faith_faith_script_mgr_delete00
static int tolua_faith_faith_script_mgr_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::script_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::script_mgr* self = (hld::script_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  hld::script_mgr */
#ifndef TOLUA_DISABLE_tolua_faith_faith_script_mgr_get_instance00
static int tolua_faith_faith_script_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::script_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::script_mgr& tolua_ret = (hld::script_mgr&)  hld::script_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::script_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_faith_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"script_mgr","hld::script_mgr","",tolua_collect_faith__script_mgr);
   #else
   tolua_cclass(tolua_S,"script_mgr","hld::script_mgr","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"script_mgr");
    tolua_function(tolua_S,"new",tolua_faith_faith_script_mgr_new00);
    tolua_function(tolua_S,"new_local",tolua_faith_faith_script_mgr_new00_local);
    tolua_function(tolua_S,".call",tolua_faith_faith_script_mgr_new00_local);
    tolua_function(tolua_S,"delete",tolua_faith_faith_script_mgr_delete00);
    tolua_function(tolua_S,"get_instance",tolua_faith_faith_script_mgr_get_instance00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_faith (lua_State* tolua_S) {
 return tolua_faith_open(tolua_S);
};
#endif

