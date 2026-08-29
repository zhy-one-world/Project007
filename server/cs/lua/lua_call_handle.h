/********************************************************************
  created: 2014/08/20
  created: 20:8:2014 9:50
  file base: lua_call_handle
  file ext: h
  author: YU REN
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_LUA_CALL_HANDLE_H_
#define _FAITH_LUA_CALL_HANDLE_H_

#define LUA_CALL_PARAM_NUM_MAX	10
#include "logic/type_def.hpp"

namespace faith
{
	enum e_lua_call_event_type
	{
		e_lua_event_known,
		e_lua_event_game_over,
		e_lua_event_open_chest,
		e_lua_event_hp_down,
	};

	void	event_handle(e_lua_call_event_type event_type,int32 param_num,...);

	void	lua_call_handle(e_lua_call_event_type event_type);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6, xstring param7);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6, xstring param7, xstring param8);
	void	lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6, xstring param7, xstring param8, xstring param9);
}

#endif