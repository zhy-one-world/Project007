/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   15:01
	file base:	msg_dispatch
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MSG_DISPATCH_HPP_
#define _MSG_DISPATCH_HPP_

#include <singleton.hpp>
#include "logic/type_def.hpp"
#include <base.hpp>

namespace hld
{
	class msg_dispatch : public singleton<msg_dispatch>
	{
	private:
		friend class singleton<msg_dispatch>;

	private:
		bool res_load();
		bool res_unload();

		bool register_lua_handler();
		bool unregister_lua_handler();

	protected:
		msg_dispatch();

	public:
		~msg_dispatch(); 

		bool init();
	};

}


#endif // __MSG_DISPATCH_HPP__