/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   17:44
	file base:	client_session
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include <time.hpp>
#include <boost/bind.hpp>
#include <net/scheduler.hpp>
#include <net/tcp_client.hpp>
#include <login_msg.hpp>
#include "client_session.hpp"
#include "proxy_service_cli.hpp"
#include "net/net_server_mgr.hpp"
#include "fep_client.hpp"
#include "server_log.hpp"
#include "Logic/time_def.hpp"
#include "utility/random.h"
#include "utility/cs_date.hpp"
#include "login.pb.h"
#include "error.pb.h"
#include "net.pb.h"

namespace faith
{
	client_session::client_session()
		: m_update_timer_index(net::scheduler::scheduler_invalid_timer_index)
	{
		m_array_index = 0;
		clear_data();
	}

	client_session::~client_session()
	{
	}

	void client_session::set_data_use(bool is_use)
	{
		if (!is_use && m_update_timer_index != net::scheduler::scheduler_invalid_timer_index)
		{
			net::scheduler::getInstance().remove_timer(m_update_timer_index);
			m_update_timer_index = net::scheduler::scheduler_invalid_timer_index;
		}
		m_data_is_use = is_use;
		if (m_data_is_use)
		{
			start_update_timer();
		}
	}

	void client_session::start_update_timer()
	{
		if (!m_data_is_use || m_update_timer_index != net::scheduler::scheduler_invalid_timer_index)
		{
			return;
		}
		m_update_timer_index = net::scheduler::getInstance().add_timer(
			500,
			m_scheduler_thread_id,
			boost::bind(&client_session::on_update_timer, this, _1));
	}

	void client_session::on_update_timer(uint32 timer_index)
	{
		if (m_data_is_use)
		{
			update(utility::get_tick_count());
		}
	}

	void client_session::clear_data()
	{
		if (m_update_timer_index != net::scheduler::scheduler_invalid_timer_index)
		{
			net::scheduler::getInstance().remove_timer(m_update_timer_index);
			m_update_timer_index = net::scheduler::scheduler_invalid_timer_index;
		}
		m_data_is_use = false;
		memset(m_account, 0, sizeof(m_account));
		memset(m_server_msg, 0, sizeof(m_server_msg));
		memset(m_ipaddr, 0, sizeof(m_ipaddr));
		m_is_create_account = false;
		m_heart_beat_time = 0;
		m_heart_login_time = 0;
		m_client_send_msg_count = 0;

		m_is_logout = false;
		m_msg_index = 0;
		m_conn_index = 0;
		m_cs_conn_index = e_invalid_server_uid;
		m_cs_array_index = 0;
		m_scheduler_thread_id = 0;
		m_client_uid.clear_data();
		m_is_send_login_success = false;

		m_client_send_msg_time = 0;
		m_client_send_error_count = 0;
		m_client_random_close_num = random_gen::get_random(50, 100);
		m_login_proto_login.Clear();
		m_radio_host = 0;
		m_online_duration = 0;
		m_online_time = 0;
		m_month_recharge_num = 0;
		m_month_recharge_time = 0;

		m_online_tick = 0;
	}
	void client_session::set_array_index(uint32 array_index)
	{
		m_array_index = array_index;
	}
	void client_session::set_client_uid()
	{
		s_client_uid client_uid(net_server_mgr::getInstance().get_server_index(), m_array_index);
		m_client_uid = client_uid;
	}
	void client_session::update(const int64& new_time)
	{
		check_session(new_time);
		// 检测是否还存在心跳
		if (new_time > m_heart_beat_time)
		{
			CONSOLE_INFO(" heart beat close, connindex:{} new_time:{} m_heart_beat_time:{}", m_conn_index, new_time, m_heart_beat_time);
			proxy_service_cli::getInstance().disconn_session(m_conn_index, e_logout_result_time_out);
		}
		//if (new_time > m_heart_login_time && m_cs_array_index <= 0)
		//{
		//	CONSOLE_INFO(__XFUNCTION__
		//		<< _XTEXT(" heart beat close, connindex:", m_conn_index
		//		<< _XTEXT(", new_time:", new_time
		//		<< _XTEXT(", m_heart_login_time:", m_heart_login_time
		//		<< _XTEXT(", m_cs_array_index:", m_cs_array_index
		//		);
		//	m_heart_login_time = new_time + 100000000000;
		//	proxy_service_cli::getInstance().disconn_session(m_conn_index, e_logout_result_connect_dis);
		//}

		if (new_time >= m_online_tick)
		{
			time_info cur_time_info = time_helper::get_cur_time_new();
			if ((m_login_proto_login.login_type() == e_login_type_new_zw || m_login_proto_login.login_type() == e_login_type_new_account_and_password)
				&& m_age < 18)
			{
				m_online_tick = new_time + second_tick_time * 10;
				m_online_time = time_helper::get_time();
				m_online_duration += 10;
			}
		}
	}
	void client_session::check_session(const int64& new_time)
	{
		if (new_time < m_client_send_msg_time)
		{
			return;
		}
		m_client_send_msg_time = new_time + second_tick_time;
		if (m_client_send_msg_count > max_packege_in_one_second)
		{
			m_client_send_error_count++;
		}
		else
		{
			m_client_send_error_count = 0;
		}
		if (m_client_send_error_count > m_client_random_close_num)
		{
			CONSOLE_INFO("so fast, close connindex:{} m_client_send_error_count:{} m_client_send_msg_count:{} m_client_random_close_num:{} m_account:{} m_ipaddr:{}", get_conn_index(), m_client_send_error_count, m_client_send_msg_count, m_client_random_close_num, m_account, m_ipaddr);
			//proxy_service_cli::getInstance().disconn_session(get_conn_index(), e_logout_result_connect_dis);
		}
		m_client_send_error_count = 0;
		m_client_send_msg_count = 0;
		m_client_random_close_num = random_gen::get_random(50, 100);
	}
	void client_session::set_account(const xchar account[max_account_length + 1])
	{
		int32 size = strlen(account);
		memcpy(m_account, account, size > faith::max_account_length ? faith::max_account_length : size);
	}
	void client_session::set_account(const xstring& account)
	{
		memset(m_account, 0, sizeof(m_account));
		memcpy(m_account, account.c_str(), account.size() > faith::max_account_length ? faith::max_account_length : account.size());
	}
	void client_session::set_server_msg(const xchar server_msg[max_server_msg_length + 1])
	{
		memcpy(m_server_msg, server_msg, max_server_msg_length * sizeof(xchar));
	}

	void client_session::refresh_heart_beat()
	{
		m_heart_beat_time = utility::get_tick_count() + client_session_heart_time;
	}
	void client_session::response_login(int32 eResult, int32 queue_pos, int32 left_time_in_sec)
	{
		if (m_is_send_login_success)
		{
			return;
		}
		if (eResult == e_error_code_success || eResult == e_error_code_success_queue_complete)
		{
			m_is_send_login_success = true;
		}
		login_proto_login_end login_end;
		login_end.set_result(eResult);
		login_end.set_account(m_account);
		login_end.set_server_msg(m_server_msg);
		login_end.set_login_type(m_login_proto_login.login_type());
		login_end.set_login_type_plus(m_login_proto_login.login_type());
		login_end.set_queue_pos(queue_pos);
		login_end.set_left_time_in_sec(left_time_in_sec);
		login_end.set_radio_host(m_radio_host);
		login_end.set_online_duration(m_online_duration);
		login_end.set_online_time(m_online_time);
		login_end.set_age(m_age);
		login_end.set_month_recharge_num(m_month_recharge_num);
		login_end.set_month_recharge_time(m_month_recharge_time);
		login_end.set_is_create_account(m_is_create_account);
		security_communication_layer::getInstance().send_to_session(get_conn_index(), &login_end, e_msgindex_s2c_client_login);
	}

	void client_session::send_to_ws_leave_queued()
	{
		fep2ws_leave_queued request;
		request.client_uid = get_client_uid();
		memcpy(request.account, m_account, sizeof(request.account));
		fep_client::getInstance().send_message_to_ws(&request, sizeof(request));

	}

	void client_session::send_to_ws_login(
		int32 login_type,
		int32 login_type_plus,
		int32 jewel_num,
		const int64* ban_role_array,
		const int64* ban_chat_array,
		const guid_64& role_guid,
		const guid_64& map_guid,
		const int32& war_index,
		const int32& server_id)
	{
		fep2ws_client_logined request;
		request.client_uid = get_client_uid();
		request.jewel_num = jewel_num;
		memcpy(request.account, m_account, sizeof(request.account));

		login_proto_login& login_data = get_login_proto_login();
		//1:app key
		memcpy(request.fixed_data.param1, login_data.sdk_data().app_key().c_str(), sizeof(request.fixed_data.param1) > login_data.sdk_data().app_key().size() ? login_data.sdk_data().app_key().size() : sizeof(request.fixed_data.param1));

		//2:客户端版本
		memcpy(request.fixed_data.param2, login_data.client_version().c_str(), sizeof(request.fixed_data.param2) > login_data.client_version().size() ? login_data.client_version().size() : sizeof(request.fixed_data.param2));

		//3:服务器ID
		xchar server_id_buff[128];
		sprintf(server_id_buff, "%d", login_data.server_id());
		memcpy(request.fixed_data.param3, server_id_buff, sizeof(request.fixed_data.param3) > sizeof(server_id_buff) ? sizeof(server_id_buff) : sizeof(request.fixed_data.param3));

		//4:media_id
		memcpy(request.fixed_data.param4, login_data.sdk_data().app_secret().c_str(), sizeof(request.fixed_data.param4) > login_data.sdk_data().app_secret().size() ? login_data.sdk_data().app_secret().size() : sizeof(request.fixed_data.param4));

		//5:设备标识
		memcpy(request.fixed_data.param5, login_data.sdk_data().device_id().c_str(), sizeof(request.fixed_data.param5) > login_data.sdk_data().device_id().size() ? login_data.sdk_data().device_id().size() : sizeof(request.fixed_data.param5));

		//6:登录IP
		memcpy(request.fixed_data.param6, m_ipaddr, sizeof(request.fixed_data.param6) > sizeof(m_ipaddr) ? sizeof(m_ipaddr) : sizeof(request.fixed_data.param6));
		
		//7:ChannelId
		memcpy(request.fixed_data.param7, login_data.sdk_data().channel_id().c_str(), sizeof(request.fixed_data.param7) > login_data.sdk_data().channel_id().size() ? login_data.sdk_data().channel_id().size() : sizeof(request.fixed_data.param7));
		
		//8:model
		memcpy(request.fixed_data.param8, login_data.sdk_data().model().c_str(), sizeof(request.fixed_data.param8) > login_data.sdk_data().model().size() ? login_data.sdk_data().model().size() : sizeof(request.fixed_data.param8));
		//9:idfa
		memcpy(request.fixed_data.param9, login_data.sdk_data().idfa().c_str(), sizeof(request.fixed_data.param9) > login_data.sdk_data().idfa().size() ? login_data.sdk_data().idfa().size() : sizeof(request.fixed_data.param9));
		//10:imei
		memcpy(request.fixed_data.param10, login_data.sdk_data().imei().c_str(), sizeof(request.fixed_data.param10) > login_data.sdk_data().imei().size() ? login_data.sdk_data().imei().size() : sizeof(request.fixed_data.param10));
		//11:android_id
		memcpy(request.fixed_data.param11, login_data.sdk_data().android_id().c_str(), sizeof(request.fixed_data.param11) > login_data.sdk_data().android_id().size() ? login_data.sdk_data().android_id().size() : sizeof(request.fixed_data.param11));
		//12:mac
		memcpy(request.fixed_data.param12, login_data.sdk_data().mac().c_str(), sizeof(request.fixed_data.param12) > login_data.sdk_data().mac().size() ? login_data.sdk_data().mac().size() : sizeof(request.fixed_data.param12));
		//13:sn
		memcpy(request.fixed_data.param13, login_data.sdk_data().sn().c_str(), sizeof(request.fixed_data.param13) > login_data.sdk_data().sn().size() ? login_data.sdk_data().sn().size() : sizeof(request.fixed_data.param13));
		//14:recharge_type
		memcpy(request.fixed_data.param14, login_data.sdk_data().recharge_type().c_str(), sizeof(request.fixed_data.param14) > login_data.sdk_data().recharge_type().size() ? login_data.sdk_data().recharge_type().size() : sizeof(request.fixed_data.param14));

		memcpy(request.fixed_data.param15, login_data.custom_info().c_str(), sizeof(request.fixed_data.param15) > login_data.custom_info().size() ? login_data.custom_info().size() : sizeof(request.fixed_data.param15));
		
		request.login_type = login_type;
		request.login_type_plus = login_type_plus;
		request.role_guid = role_guid;
		request.map_guid = map_guid;
		request.war_index = war_index;
		request.server_id = server_id;
		memcpy(request.ban_role_array, ban_role_array, sizeof(request.ban_role_array));
		memcpy(request.ban_chat_array, ban_chat_array, sizeof(request.ban_chat_array));
		fep_client::getInstance().send_message_to_ws( &request, sizeof(request));
	}

	void client_session::set_radio_host(int32 radio_host)
	{
		m_radio_host = radio_host;
	}

	void client_session::set_online_duration(int32 online_duration)
	{
		m_online_duration = online_duration;
	}

	void client_session::set_online_time(int32 online_time)
	{
		m_online_time = online_time;
	}

	void client_session::set_age(int32 age)
	{
		m_age = age;
	}

	faith::int32 client_session::get_online_duration()
	{
		return m_online_duration;
	}

	faith::int32 client_session::get_online_time()
	{
		return m_online_time;
	}

	faith::int32 client_session::get_age()
	{
		return m_age;
	}

	void client_session::set_month_recharge_num(int32 month_recharge_num)
	{
		m_month_recharge_num = month_recharge_num;
	}

	faith::int32 client_session::get_month_recharge_num()
	{
		return m_month_recharge_num;
	}

	void client_session::set_month_recharge_time(int32 month_recharge_time)
	{
		m_month_recharge_time = month_recharge_time;
	}

	faith::int32 client_session::get_month_recharge_time()
	{
		return m_month_recharge_time;
	}

}
