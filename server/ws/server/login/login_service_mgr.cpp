/********************************************************************
	created:	2014/06/03
	created:	3:6:2014   13:25
	file base:	login_service_mgr
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "login_service_mgr.hpp"
#include <Utility/init_unit.h>
#include <Utility/parse_msg.h>
#include "server_log.hpp"
#include "login_service_pc.hpp"
#include "login_service_robot.hpp"
#include "login_service_zw.hpp"
#include "login_service_facebook.hpp"
#include "login_service_google.hpp"
#include "login_service_tw.hpp"
#include "login_service_korea.hpp"
#include "login_service_yw.hpp"
#include "login_service_umisdk.hpp"
#include "login_service_appstore.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"
#include "game_cfg/servers_config.h"

namespace hld
{

	login_service_mgr::login_service_mgr()
	{
		memset(m_login_service_array, 0, sizeof(m_login_service_array));
		m_login_service_array[e_login_type_new_account_and_password] = new login_service_pc();
		m_login_service_array[e_login_type_new_robot] = new login_service_robot();
		m_login_service_array[e_login_type_new_token] = nullptr;
		m_login_service_array[e_login_type_new_zw] = new login_service_zw();
		m_login_service_array[e_login_type_new_facebook] = new login_service_facebook();
		m_login_service_array[e_login_type_new_google] = new login_service_google();
		m_login_service_array[e_login_type_new_gate_server] = nullptr;
		m_login_service_array[e_login_type_new_tw] = new login_service_tw();
		m_login_service_array[e_login_type_new_korea] = new login_service_korea();
		m_login_service_array[e_login_type_new_en] = new login_service_umisdk(e_login_type_new_en);
		m_login_service_array[e_login_type_new_umisdk] = new login_service_umisdk(e_login_type_new_umisdk);
		m_login_service_array[e_login_type_new_umisdk_vn] = new login_service_umisdk(e_login_type_new_umisdk_vn);
		m_login_service_array[e_login_type_new_appstore] = new login_service_appstore();
		m_login_service_array[e_login_type_new_umisdk_tw] = new login_service_umisdk(e_login_type_new_umisdk_tw);
		m_login_service_array[e_login_type_new_umisdk_jp] = new login_service_umisdk(e_login_type_new_umisdk_jp);
		m_login_service_array[e_login_type_new_umisdk_rus] = new login_service_umisdk(e_login_type_new_umisdk_rus);
		m_login_service_array[e_login_type_new_umisdk_sg] = new login_service_umisdk(e_login_type_new_umisdk_sg);
		m_login_service_array[e_login_type_new_umisdk_new_tw] = new login_service_umisdk(e_login_type_new_umisdk_new_tw);
		
		m_next_tick_time = 0;
		m_login_info_keep_map.clear();
	}
	login_service_mgr::~login_service_mgr()
	{
		for (int32 i =0; i < e_login_type_new_max; ++i)
		{
			if (m_login_service_array[i] != nullptr)
			{
				delete m_login_service_array[i];
				m_login_service_array[i] = nullptr;
			}
		}
	}

	void	login_service_mgr::tick(int64 time_now)
	{
		if (false)
		{

		}
		if (time_now < m_next_tick_time)
		{
			return;
		}
		m_next_tick_time = init_unit::get_end_time(10);

		std::map<int64, login_keep_info>::iterator it;
		for (it = m_login_info_keep_map.begin(); it != m_login_info_keep_map.end(); ++it)
		{
			login_keep_info temp_info = it->second;
			if (time_now < temp_info.invalid_time)
			{
				continue;
			}
			it = m_login_info_keep_map.erase(it);
		}
	}

	void	login_service_mgr::req_login(const void* data, size_t data_len)
	{
		if (data == NULL)
		{
			return;
		}

		s_client_uid client_uid;
		login_proto_login temp_login_proto;
		parse_msg::getInstance().parse_message_new(&temp_login_proto, data, data_len);
		parse_msg::getInstance().parse_clientuid_from_msg_new(data, client_uid);

		e_login_type_new login_type =(e_login_type_new)temp_login_proto.login_type();
		if (login_type >= e_login_type_new_max)
		{
			return;
		}
		if (nullptr == m_login_service_array[login_type])
		{
			return;
		}
		m_login_service_array[login_type]->req_login(client_uid, temp_login_proto);
		
		if (temp_login_proto.login_type() != e_login_type_new_robot
			&& temp_login_proto.login_type() != e_login_type_new_token)
		{
			std::map<int64, login_keep_info>::iterator it = m_login_info_keep_map.find(client_uid.fep_uid_64);
			if (it != m_login_info_keep_map.end())
			{
				m_login_info_keep_map.erase(it);
			}

			login_keep_info temp_info;
			temp_info.data = temp_login_proto;
			temp_info.invalid_time = init_unit::get_end_time(login_proto_keep_time);
			m_login_info_keep_map.insert({ client_uid .fep_uid_64, temp_info });
		}
	}

	void	login_service_mgr::end_login(const void* data, size_t data_len)
	{
		if (data == NULL)
		{
			return;
		}
		const dp2ls_client_login* pdata = static_cast<const dp2ls_client_login*>(data);

		if (pdata->login_type < 0 || pdata->login_type >= e_login_type_new_max)
		{
			return;
		}
		if (nullptr == m_login_service_array[pdata->login_type])
		{
			return;
		}
		m_login_service_array[pdata->login_type]->end_login(pdata);
	
		if (pdata->login_type != e_login_type_new_robot
			&& pdata->login_type != e_login_type_new_token)
		{
			std::map<int64, login_keep_info>::iterator it = m_login_info_keep_map.find(pdata->client_uid.fep_uid_64);
			if (it != m_login_info_keep_map.end())
			{
				login_keep_info temp_info = it->second;
				m_login_info_keep_map.erase(it);

				login_proto_login login_data = temp_info.data;

				if (pdata->is_create_new)
				{
					server_log::login_create_role_log(world_server::getInstance().get_server_id(),
						login_data.sdk_data().app_key(),
						login_data.client_version(),
						login_data.sdk_data().channel_id(),
						login_data.sdk_data().app_secret(),
						pdata->account,
						login_data.sdk_data().device_id(),
						login_data.ip(),
						login_data.sdk_data().model(),
						login_data.sdk_data().idfa(),
						login_data.sdk_data().imei(),
						login_data.sdk_data().android_id(),
						login_data.sdk_data().mac(),
						login_data.sdk_data().sn(),
						pdata->login_type);
					int32 channel_id = 0;
					set_log_var(log_head);
					set_log_common_head_part1(log_head, SERVERCONFIG->app_key, login_data.sdk_data().channel_id(), "0", login_data.sdk_data().device_id());
					set_log_common_head_part2(log_head, login_data.client_version(), init_unit::change_string_to_i32(login_data.custom_info()));
					set_log_common_head_part3(log_head, pdata->account, world_server::getInstance().get_server_id(),utility::get_tick_count())
					server_log::serverAccountCreate(log_head);
				}
				else
				{
					server_log::login_role_log(world_server::getInstance().get_server_id(),
						login_data.sdk_data().app_key(),
						login_data.client_version(),
						login_data.sdk_data().channel_id(),
						login_data.sdk_data().app_secret(),
						pdata->account,
						login_data.sdk_data().device_id(),
						login_data.ip(),
						login_data.sdk_data().model(),
						login_data.sdk_data().idfa(),
						login_data.sdk_data().imei(),
						login_data.sdk_data().android_id(),
						login_data.sdk_data().mac(),
						login_data.sdk_data().sn());
					 // set_log_var(log_head);
					//set_log_common_head_part1(log_head, SERVERCONFIG->app_key, login_data.sdk_data().channel_id(), "0", login_data.sdk_data().device_id());
					//set_log_common_head_part2(log_head, login_data.client_version(), 0)
					//set_log_common_head_part3(log_head, pdata->account, init_unit::change_i32_to_string(world_server::getInstance().get_server_id()), utility::get_tick_count())
					//server_log::serverRoleLogin(log_head);
				}
			}
		}
	}

	const login_proto_login*	login_service_mgr::get_login_info(const int64 client_uid)
	{
		std::map<int64, login_keep_info>::iterator it = m_login_info_keep_map.find(client_uid);
		if (it == m_login_info_keep_map.end())
		{
			return nullptr;
		}
		return &it->second.data;
	}

}
