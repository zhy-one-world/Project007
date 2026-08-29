/********************************************************************
  created: 2014/08/13
  created: 13:8:2014 9:36
  file base: script_mgr
  file ext: h
  author: YU REN
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_SCRIPT_MGR_H_
#define _FAITH_SCRIPT_MGR_H_

#include <Utility/guid_gen.h>
#include "lua/lua_script.h"

namespace faith
{
	#define LUA_SCRIPE_FILE_NAME 128
	class script_mgr
	{
	public:
		script_mgr();
		virtual ~script_mgr();

	public:
		static script_mgr&			get_instance();
		/*
		* 功能 注册lua函数
		* 参数 const xchar* event_type  函数对应的事件类型，lua文件中配置
		* 参数 const xchar* func_name   函数名
		*/
		//bool						register_func(const xchar* event_type,const xchar* func_name);
		//bool						unregister_func(const xchar* event_type);
		/*
		* 功能 调用已注册的lua函数
		* 参数 const xchar* event_type   函数对应的事件类型，lua文件中配置
		* 参数 int32 nResults				函数返回值个数
		* 参数 const xchar* cFormat      调用时所传参数的类型 
		*				n:数字型(double) d:整形(int32) s:字符串型 f:C函数型  n:Nil v:Value p:Point
		*			    v:为Lua支持的，参数为整形的数index，指明将index所指堆栈的变量作为该函数的调用参数
		* 注意：由于该函数有不定参数…,对于数字，系统并不确定数是以double还是以int32存在，两种保存形式是不同的。因此需要注意当传入的数是整形时，格式符应用d,而不能用n,或者强行改变为double形。否则会出现计算的错误。
		*/
		void						call_func(const xchar* table_name, const xchar* func_name,int32 nResults, bool use_buff, const xchar* cFormat, ...);
		/*
		 * 函数:	
		 * 功能:	根据类型获取lua中的全局普简单类型变量
		*/
		int32							get_lua_int_variable(const xstring lua_variable_name);
		f64							get_lua_double_variable(const xstring lua_variable_name);
		xstring						get_lua_string_variable(const xstring lua_variable_name);
		bool						get_lua_bool_variable(const xstring lua_variable_name);
		/*
		 * 函数:	
		 * 功能:	根据类型获取lua中的全局复合类型变量
		 * 形如 test_table = 
					{
						[1] = 0,
						[2] = {a = 1,b = 2},
						[3] = {c = 3}
				    }
					两层嵌套的table结构，第一层嵌套使用下标结构，第二层嵌套使用key-value结构，value值可以为lua中任意有效类型值
		 * 参数 const xstring lua_table_name  			全局table变量
		 * 参数 int lua_item_row							table变量中嵌套子table的位置，如[2] 为 row = 2
		 * 参数 const xstring lua_item_col_name			table变量中嵌套子table的中的item key值
		 * 返回值统一为xstring类型，需在逻辑中进行实际类型转换
		*/
		xstring						get_lua_table_item_variable(const xstring lua_table_name,int lua_item_row,const xstring lua_item_col_name);
	private:
		bool						init();
		void						exit();
	    //int32						get_event_id(const xchar* event_type);
	private:
		xstring						m_script_name;
		faith::lua::lua_script		m_script;
	};
}

#endif