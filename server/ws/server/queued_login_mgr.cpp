/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   15:02
	file base:	queued_login_mgr
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "queued_login_mgr.h"
#include "game_cfg/servers_config.h"
#include "client_session_mgr.hpp"
#include <login_msg.hpp>
#include <Logic/char_def.hpp>
#include "csv_synchronization_mgr.h"

namespace hld
{
	queued_login_mgr::queued_login_mgr(void)
	{
		m_queued_client_list.clear();
		m_online_counter = 0;
		m_update_time_interval = 0;
		m_max_login_player = SERVERCONFIG->init_player_num;
		m_max_login_normal_player = m_max_login_player * 0.9f;
	}

	queued_login_mgr::~queued_login_mgr(void)
	{

	}

	void queued_login_mgr::pop_gm()
	{
		for (unit_index_list_it it = m_queued_client_list.begin(); it != m_queued_client_list.end();)
		{
			unit_index_list_it temp_it = it++;
			client_session* client_session_ptr = client_session_mgr::getInstance().get_session_by_array_index(*temp_it);
			if (client_session_ptr && client_session_ptr->m_jewel_num > 0)
			{
				if (m_online_counter < init_player_max)
				{
					login_complete(client_session_ptr);
					m_queued_client_list.erase(temp_it);
				}
			}
		}
	}
	bool queued_login_mgr::pop_front()
	{
		unit_index_list_it it = m_queued_client_list.begin();
		if (it != m_queued_client_list.end())
		{
			client_session* client_session_ptr = client_session_mgr::getInstance().get_session_by_array_index(*it);
			if (client_session_ptr)
			{
				login_complete(client_session_ptr);
			}
			m_queued_client_list.pop_front();
			return true;
		}
		return false;
	}

	void queued_login_mgr::tick(const int64& new_time)
	{
		if (m_queued_client_list.empty())
			return;

		if (new_time >= m_update_time_interval)
		{
			m_update_time_interval = new_time + deci_second_tick_time;
			if (m_online_counter < m_max_login_player)
			{
				pop_gm();
				if (m_online_counter < m_max_login_normal_player)
				{
					for (int32 i = 0; i < queue_player_count; ++i)
					{
						pop_front();
					}
				}
			}
			// 刷新排位
			ws2fep_login_queue_status msg;
			int32 queue_count = 1;
			for (unit_index_list_it it = m_queued_client_list.begin(); it != m_queued_client_list.end(); ++it)
			{
				client_session* client_session_ptr = client_session_mgr::getInstance().get_session_by_array_index(*it);
				if (client_session_ptr)
				{
					msg.queue_pos = queue_count;
					if (m_online_counter < m_max_login_normal_player)
					{
						msg.left_time_in_sec = queue_count / queue_player_count * deci_second_tick_time / 1000 + 3;
					}
					else
					{
						msg.left_time_in_sec = 9999;
					}
					msg.client_uid = client_session_ptr->m_client_uid;
					client_session_ptr->send_to_fep(&msg, sizeof(msg));
					queue_count++;
				}
			}
		}
	}

	//正常登陆流程
	void queued_login_mgr::login_complete(client_session* client_session_ptr)
	{
		if (nullptr == client_session_ptr)
		{
			CONSOLE_ERROR("queued_login_mgr::login_complete client_session_ptr is nullptr");
			return;
		}
		client_session_ptr->m_status = client_session::e_ss_login_ok;
		client_session_ptr->m_online_state = client_session::e_os_online;
		change_online_counter(1);
		if(client_session_ptr->m_login_type == e_login_type_new_token)
		{
			client_session_ptr->m_is_need_send_cross_server_notice = true;
			//token登陆到这就可以传送了
			client_session_ptr->m_step_num = client_session::e_session_step_login_win;
			client_session_ptr->m_status = client_session::e_ss_map_transfer;
			client_session_ptr->set_cs_array_index(-1);
			client_session_ptr->set_legion_info(client_session_mgr::getInstance().get_cross_player_legion(client_session_ptr->get_role_guid()));
			client_session_ptr->player_enter_game_transfer(client_session_ptr->get_role_guid(), client_session_ptr->get_role_info_data(e_role_info_server_id));
		}
		else
		{
			ws2fep_client_logined msg;
			msg.client_uid = client_session_ptr->m_client_uid;
			memcpy(msg.account, client_session_ptr->m_account, max_account_length);
			msg.eResult = e_error_code_success;
			msg.login_type = client_session_ptr->m_login_type;
			msg.login_type_plus = client_session_ptr->m_login_type_plus;
			client_session_ptr->send_to_fep(&msg, sizeof(msg));
			client_session_ptr->m_step_num = client_session::e_session_step_login_win;
		}
	}

	void queued_login_mgr::on_session_login(client_session* client_session_ptr, e_login_type_new login_type)
	{
		if (nullptr == client_session_ptr)
		{
			CONSOLE_ERROR("queued_login_mgr::on_session_login client_session_ptr is nullptr");
			return;
		}
		
		int32 queue_num = m_queued_client_list.size();
		if ((m_online_counter + queue_num) >= init_socket_link)
		{//send服务器已到达最大连接数，登出
			ws2fep_client_logined msg;
			msg.client_uid = client_session_ptr->m_client_uid;
			msg.eResult = e_error_code_login_login_queue_full;
			msg.login_type = client_session_ptr->m_login_type;
			msg.login_type_plus = client_session_ptr->m_login_type_plus;
			client_session_ptr->send_to_fep(&msg, sizeof(msg));
			client_session_mgr::getInstance().logout_client(client_session_ptr);
			return;
		}
		if (login_type == e_login_type_new_token)
		{
			login_complete(client_session_ptr);
			return;
		}

		if (client_session_ptr->get_role_info_data(e_role_info_server_id) == SERVERCONFIG->game_id)
		{
			csv_synchronization_mgr::get_instance().send_template(client_session_ptr);
		}
		//加入队列
		m_queued_client_list.push_back(client_session_ptr->get_array_index());

		client_session_ptr->m_status = client_session::e_ss_queue;
		client_session_ptr->m_online_state = client_session::e_os_inqueue;
		if (m_queued_client_list.size() > queue_player_count || m_online_counter >= m_max_login_normal_player)
		{
			ws2fep_client_logined msg;
			msg.client_uid = client_session_ptr->m_client_uid;
			msg.login_type = client_session_ptr->m_login_type;
			msg.login_type_plus = client_session_ptr->m_login_type_plus;
			msg.eResult = e_error_code_success_queue_login;
			msg.queue_pos = m_queued_client_list.size();
			if (m_online_counter < m_max_login_normal_player)
			{
				msg.left_time_in_sec = msg.queue_pos / queue_player_count * deci_second_tick_time / 1000 + 3;
			}
			else
			{
				msg.left_time_in_sec = 9999;
			}
			memcpy(msg.account, client_session_ptr->m_account, sizeof(msg.account));
			client_session_ptr->send_to_fep(&msg, sizeof(msg));
		}
	}

	void queued_login_mgr::on_session_logout(client_session* client_session_ptr, bool is_need_reduce_count)
	{
		if (nullptr == client_session_ptr)
		{
			return;
		}
		for (unit_index_list_it it = m_queued_client_list.begin(); it != m_queued_client_list.end(); ++it)
		{
			if (client_session_ptr->get_array_index() == *it)
			{
				m_queued_client_list.erase(it);
				return;
			}
		}
		if (!is_need_reduce_count)
		{
			return;
		}
		if (client_session_ptr->m_online_state == client_session::e_os_online)
		{
			m_online_counter--;
		}
	}
}
