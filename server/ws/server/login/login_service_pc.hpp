/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2011-5-31 15:40
	
	File Name :	login_service_naive.hpp
	
	Author : zhangjianyu (zhangjianyu@163.com)
	
	Description : 
	
	Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#ifndef _LOGIN_SERVICE_NAIVE_HPP_
#define _LOGIN_SERVICE_NAIVE_HPP_

#include "server_def.hpp"
#include "login_service.hpp"
#include <login_msg.hpp>

//////////////////////////////////////////////////////////////////////////
//
//	Class Declare
//
//////////////////////////////////////////////////////////////////////////

namespace hld
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Declare
	//
	//////////////////////////////////////////////////////////////////////////
	class login_service_pc : public login_service
	{
	public:
		login_service_pc() {}
		~login_service_pc() {}

	public:
		virtual void req_login(const s_client_uid client_uid, const login_proto_login proto_data);
		virtual void end_login(const dp2ls_client_login* pdata);
	};
}


#endif
