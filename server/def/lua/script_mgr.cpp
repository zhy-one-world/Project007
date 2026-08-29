/********************************************************************
  created: 2014/08/13
  created: 13:8:2014 9:36
  file base: script_mgr
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/

#include "script_mgr.h"

namespace faith
{
	script_mgr::script_mgr()
	{
		init();
	}

	script_mgr::~script_mgr()
	{
		exit();
	}

	script_mgr& script_mgr::get_instance()
	{
		static script_mgr script;
		return script;
	}

	bool script_mgr::init()
	{
		m_script_name = "../res/script/faith.lua";		
		bool ret = m_script.init(m_script_name.c_str());
		return ret;
	}

	void script_mgr::exit()
	{
		m_script.exit();
	}

// 	bool script_mgr::register_func(const xchar* event_type,const xchar* func_name)
// 	{
// 		return m_script.register_function(get_event_id(event_type),func_name);
// 	}
// 
// 	bool script_mgr::unregister_func(const xchar* event_type)
// 	{
// 		return m_script.unregister_function(get_event_id(event_type));
// 	}

	void script_mgr::call_func(const xchar* table_name, const xchar* func_name,int32 nResults, bool use_buff, const xchar* cFormat, ...)
	{
		va_list vlist;
		va_start(vlist, cFormat);
		//bResult = m_script.call_function(get_event_id(event_type), nResults, cFormat, vlist);
		int32 bResult = m_script.call_function(table_name, func_name, nResults, use_buff, cFormat, vlist);
		va_end(vlist);
		m_script.script_error(bResult);
	}

// 	int32 script_mgr::get_event_id(const xchar* event_type)
// 	{
// 		return m_script.get_event_id(event_type);
// 	}

	int32	script_mgr::get_lua_int_variable(const xstring lua_variable_name)
	{
		return m_script.get_lua_int_variable(lua_variable_name);
	}

	f64	script_mgr::get_lua_double_variable(const xstring lua_variable_name)
	{
		return m_script.get_lua_double_variable(lua_variable_name);
	}

	xstring	script_mgr::get_lua_string_variable(const xstring lua_variable_name)
	{
		return m_script.get_lua_string_variable(lua_variable_name);
	}

	bool script_mgr::get_lua_bool_variable(const xstring lua_variable_name)
	{
		return  m_script.get_lua_bool_variable(lua_variable_name);
	}

	xstring script_mgr::get_lua_table_item_variable(const xstring lua_table_name,int lua_item_row,const xstring lua_item_col_name)
	{
		return m_script.get_lua_table_item_variable(lua_table_name,lua_item_row,lua_item_col_name);
	}

}




