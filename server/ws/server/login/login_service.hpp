/********************************************************************
	created:	2014/06/03
	created:	3:6:2014   13:35
	file base:	login_service
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _LOGIN_SERVICE_H_
#define _LOGIN_SERVICE_H_

#include "server_def.hpp"
#include <login_msg.hpp>

namespace faith
{
	class login_proto_login;
	class login_service
	{
	public:
		virtual void req_login(const s_client_uid client_uid, const login_proto_login proto_data) = 0;
		virtual void end_login(const dp2ls_client_login* pData) = 0;
	};
}

#endif
