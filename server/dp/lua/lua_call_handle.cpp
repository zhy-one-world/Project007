/********************************************************************
  created: 2014/08/20
  created: 20:8:2014 9:50
  file base: lua_call_handle
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/

#include "lua_call_handle.h"

namespace faith
{
	void lua_call_handle(e_lua_call_event_type event_type)
	{
		event_handle(event_type, 0);
	}
	void lua_call_handle(e_lua_call_event_type event_type, xstring param1)
	{
		event_handle(event_type, 1, param1);
	}

	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2)
	{
		event_handle(event_type, 2, param1, param2);
	}

	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3)
	{
		event_handle(event_type, 3, param1, param2, param3);
	}

	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4)
	{
		event_handle(event_type, 4, param1, param2, param3, param4);
	}

	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5)
	{
		event_handle(event_type, 5, param1, param2, param3, param4, param5);
	}

	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6)
	{
		event_handle(event_type, 6, param1, param2, param3, param4, param5, param6);
	}
	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6, xstring param7)
	{
		event_handle(event_type, 7, param1, param2, param3, param4, param5, param6, param7);
	}
	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6, xstring param7, xstring param8)
	{
		event_handle(event_type, 8, param1, param2, param3, param4, param5, param6, param7, param8);
	}
	void lua_call_handle(e_lua_call_event_type event_type, xstring param1, xstring param2, xstring param3, xstring param4, xstring param5, xstring param6, xstring param7, xstring param8, xstring param9)
	{
		event_handle(event_type, 9, param1, param2, param3, param4, param5, param6, param7, param8, param9);
	}
	void event_handle(e_lua_call_event_type event_type,int32 param_num,...)
	{
		if(param_num > LUA_CALL_PARAM_NUM_MAX)
			return;

		xstring params[LUA_CALL_PARAM_NUM_MAX];
		va_list vlist;
		va_start(vlist, param_num);

		for(int32 i = 0;i < param_num;i++)
		{
			params[i] = va_arg(vlist,xstring).c_str();
		}

		switch(event_type)
		{
		case e_lua_event_game_over:
			{

			}
			break;
		case e_lua_event_open_chest :
			{

			}
			break;
		case e_lua_event_hp_down:
		{
		}
		break;
		default:
			break;
		}

		va_end(vlist);
	}
}