// /*@@
// 
// 	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
// 	
// 	Created_datetime : 	2011-5-31 15:43
// 	
// 	File Name :	login_service_robot.cpp
// 	
// 	Author : zhangjianyu (zhangjianyu@163.com)
// 	
// 	Description : 
// 	
// 	Change List :
// 
// 
// @@*/

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "login_service_robot.hpp"
#include "login_service_mgr.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"
#include "server_log.hpp"
#include <login_msg.hpp>
#include <Utility/parse_msg.h>
#include "utility/init_unit.h"

namespace hld
{

	void login_service_robot::req_login(const s_client_uid client_uid, const login_proto_login proto_data)
	{
		ls2dp_client_login request;
		request.client_uid = client_uid;
		request.server_id = proto_data.server_id();

		memcpy(request.account, proto_data.logic_account().c_str(), sizeof(request.account) > proto_data.logic_account().size() ? proto_data.logic_account().size() : sizeof(request.account));
		memcpy(request.password, proto_data.password().c_str(), sizeof(request.password) > proto_data.password().size() ? proto_data.password().size() : sizeof(request.password));
		memcpy(request.device_type, proto_data.device_type().c_str(), sizeof(request.device_type) > proto_data.device_type().size() ? proto_data.device_type().size() : sizeof(request.device_type));
		memcpy(request.create_ip, proto_data.ip().c_str(), sizeof(request.create_ip) > proto_data.ip().size() ? proto_data.ip().size() : sizeof(request.create_ip));
		request.login_type = e_login_type_new_robot;
		request.login_type_plus = 0;

		ws_client::getInstance().send_to_dp( &request, sizeof(request));
	}

	void login_service_robot::end_login(const dp2ls_client_login* pdata)
	{
		if(!pdata)
		{
			return;
		}
		ls2fep_client_login request;
		request.eResult = pdata->e_result;
		request.client_uid = pdata->client_uid;
		request.login_type = pdata->login_type;
		request.login_type_plus = pdata->login_type_plus;
		request.jewel_num = pdata->jewel_num;
		request.radio_host = pdata->radio_host;
		request.is_create_new = pdata->is_create_new;
		memcpy(request.account, pdata->account, sizeof(request.account));
		world_server::getInstance().send_to_fep(pdata->client_uid.fepserver_uid, &request, sizeof(request));
	}

}
