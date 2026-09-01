// /*@@
// 	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
// 	Created_datetime : 	2011-5-31 15:43
// 	File Name :	login_service_naive.cpp
// 	Author : zhangjianyu (zhangjianyu@163.com)
// 	Description : 
// 	Change List :
// @@*/

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include "login_service_pc.hpp"
#include "login_service_mgr.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"
#include "server_log.hpp"
#include <login_msg.hpp>
#include <Utility/parse_msg.h>
#include "utility/init_unit.h"
#include "template/template_manager.h"

namespace faith
{

	void login_service_pc::req_login(const s_client_uid client_uid, const login_proto_login proto_data)
	{
		if (world_server::getInstance().get_gm_state())//账号登陆与GM是否开启无关
		{
			ls2dp_client_login request;
			request.client_uid = client_uid;
			request.server_id = proto_data.server_id();

			memcpy(request.account, proto_data.logic_account().c_str(), sizeof(request.account) > proto_data.logic_account().size() ? proto_data.logic_account().size() : sizeof(request.account));
			memcpy(request.password, proto_data.password().c_str(), sizeof(request.password) > proto_data.password().size() ? proto_data.password().size() : sizeof(request.password));
			memcpy(request.device_type, proto_data.device_type().c_str(), sizeof(request.device_type) > proto_data.device_type().size() ? proto_data.device_type().size() : sizeof(request.device_type));
			memcpy(request.create_ip, proto_data.ip().c_str(), sizeof(request.create_ip) > proto_data.ip().size() ? proto_data.ip().size() : sizeof(request.create_ip));
			request.login_type = e_login_type_new_account_and_password;
			request.login_type_plus = 0;

			//wuhui test begin
			request.age = 18;
			//wuhui test end

			ws_client::getInstance().send_to_dp( &request, sizeof(request));
		}
		else
		{
			ls2fep_client_login request;
			request.eResult = e_error_code_login_ban;
			request.client_uid = client_uid;
			request.login_type = e_login_type_new_account_and_password;
			request.login_type_plus = 0;
			request.jewel_num = 0;
			request.radio_host = 0;
			memcpy(request.account, proto_data.logic_account().c_str(), sizeof(request.account) > proto_data.logic_account().size() ? proto_data.logic_account().size() : sizeof(request.account));
			world_server::getInstance().send_to_fep(client_uid.fepserver_uid, &request, sizeof(request));
		}
	}

	void login_service_pc::end_login(const dp2ls_client_login* pdata)
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
		request.online_duration = pdata->online_duration;
		request.online_time = pdata->online_time;
		request.month_recharge_num = pdata->month_recharge_num;
		request.month_recharge_time = pdata->month_recharge_time;
		request.age = pdata->age;
		memcpy(request.account, pdata->account, sizeof(request.account));

		if (request.eResult == e_error_code_success && request.age < 18)
		{
			if (GAMECONFIG->AntiIndulgenceTimeLimit.size() == 4
				&& GAMECONFIG->AntiIndulgenceHoliday.size() % 6 == 0)
			{
				time_info cur_time_info = time_helper::get_cur_time_new();
				if ((cur_time_info.minute_in_day < GAMECONFIG->AntiIndulgenceTimeLimit[0] * 60 + GAMECONFIG->AntiIndulgenceTimeLimit[1])
					|| (cur_time_info.minute_in_day > GAMECONFIG->AntiIndulgenceTimeLimit[2] * 60 + GAMECONFIG->AntiIndulgenceTimeLimit[3]))
				{
					request.eResult = e_error_code_login_online_time_limit;
				}
				else
				{
					if (time_helper::is_diff_day(request.online_time, time_helper::get_time()) == true)
					{
						request.online_duration = 0;
						request.online_time = time_helper::get_time();
					}
					else
					{
						int32 DurationLimit = GAMECONFIG->AntiIndulgenceDurationLimit * 3600;
						for (int32 i = 0; i < GAMECONFIG->AntiIndulgenceHoliday.size() / 6; i++)
						{
							time_info start_time = time_helper::get_time_by_info(GAMECONFIG->AntiIndulgenceHoliday[i * 6], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 1], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 2]);
							time_info end_time = time_helper::get_time_by_info(GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 3], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 4], GAMECONFIG->AntiIndulgenceHoliday[i * 6 + 5]);
							if (start_time.second < cur_time_info.second && cur_time_info.second < end_time.second + 3600 * 24)
							{
								DurationLimit = GAMECONFIG->AntiIndulgenceHolidayDurationLimit * 3600;
								break;
							}
						}
						if (request.online_duration >= DurationLimit)
						{
							request.eResult = e_error_code_login_online_duration_over;
						}
					}
				}

				//充值额度
				if (time_helper::is_diff_month(request.month_recharge_time, time_helper::get_time()) == true)
				{
					request.month_recharge_num = 0;
					request.month_recharge_time = time_helper::get_time();
				}
			}
		}

		world_server::getInstance().send_to_fep(pdata->client_uid.fepserver_uid, &request, sizeof(request));
	}

}
