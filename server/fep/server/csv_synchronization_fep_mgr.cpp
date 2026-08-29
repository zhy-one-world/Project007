/********************************************************************
created: 2020年01月07日
file base: csv_synchronization_fep_mgr
file ext: cpp
author: wangdi

purpose: csv同步信息管理器
*********************************************************************/

#include "csv_synchronization_fep_mgr.h"
#include "time_limit_activity_temp_fep_mgr.h"
#include "utility/init_unit.h"
#include "../server/security_communication_layer.hpp"
#include "proxy_service_cli.hpp"
#include "net.pb.h"
namespace faith
{

	csv_synchronization_fep_mgr::csv_synchronization_fep_mgr()
	{

	}

	csv_synchronization_fep_mgr::~csv_synchronization_fep_mgr()
	{

	}

	void csv_synchronization_fep_mgr::clear_data()
	{

	}

	void csv_synchronization_fep_mgr::heart_tick(const int64& time_new)
	{
		ZoneScoped;
		list<ui64> delete_uid_list;
		for (auto ite = m_send_csv_player_map.begin(); ite != m_send_csv_player_map.end(); ++ite)
		{
			//序号从0开始 每次发送后加一 直到超过总包长度
			if (!send_csv_msg_to_client(proxy_service_cli::getInstance().get_session_by_fep_uid_64(ite->first), ite->second))
			{
				delete_uid_list.push_back(ite->first);
			}
			else
			{
				ite->second++;
			}
		}

		for (auto ite = delete_uid_list.begin(); ite != delete_uid_list.end(); ++ite)
		{
			ui64 temp_uid = *ite;
			erase_from_send_csv_msg_map_by_session_uid(temp_uid);
		}
	}

	void csv_synchronization_fep_mgr::send_time_limit_template(client_session* session_ptr)
	{
		if (nullptr == session_ptr || !session_ptr->is_vaild())
		{
			return;
		}
		m_send_csv_player_map[session_ptr->get_client_uid().fep_uid_64] = 0;
	}

	void csv_synchronization_fep_mgr::add_time_limit_activity_template(xstring table_name)
	{
		const std::map<int32, s_time_limit_activity_temp>& act_temp_map = time_limit_activity_temp_fep_mgr::get_instance().get_temp_map();
		bool if_first_send = true;
		game_proto_set_template_table resp;
		resp.set_tabel_name(table_name);
		for (auto ite = act_temp_map.begin(); ite != act_temp_map.end(); ite++)
		{
			if (ite->second.ActGuid <= 0)
			{
				continue;
			}
			game_proto_template_table_data activity_template_data_temp;
			time_limit_activity_temp_fep_mgr::get_instance().fill_time_limit_activity_template_one_msg(&ite->second, &activity_template_data_temp);
			if (resp.ByteSize() + activity_template_data_temp.ByteSize() > packege_length_max)
			{
				if (if_first_send)
				{
					resp.set_if_first_send(1);
					if_first_send = false;
				}
				else
				{
					resp.set_if_first_send(0);
				}
				m_send_csv_template_msg_array.push_back(resp);
				resp.clear_template_data();
			}
			game_proto_template_table_data* activity_template_data = resp.add_template_data();
			activity_template_data->CopyFrom(activity_template_data_temp);
		}
		if (if_first_send)
		{
			resp.set_if_first_send(1);
		}
		else
		{
			resp.set_if_first_send(0);
		}
		m_send_csv_template_msg_array.push_back(resp);
	}

	void csv_synchronization_fep_mgr::add_time_limit_branch_activity_template(xstring table_name)
	{
		const std::map<int32, s_time_limit_activity_branch_temp>& branch_temp_map = time_limit_activity_temp_fep_mgr::get_instance().get_branch_temp_map();
		bool if_first_send = true;
		game_proto_set_template_table resp;
		resp.set_tabel_name(table_name);
		for (auto ite = branch_temp_map.begin(); ite != branch_temp_map.end(); ite++)
		{
			if (ite->second.ActGuid <= 0)
			{
				continue;
			}
			game_proto_template_table_data activity_template_data_temp;
			time_limit_activity_temp_fep_mgr::get_instance().fill_time_limit_activity_branch_template_one_msg(&ite->second, &activity_template_data_temp);
			if (resp.ByteSize() + activity_template_data_temp.ByteSize() > packege_length_max)
			{
				if (if_first_send)
				{
					resp.set_if_first_send(1);
					if_first_send = false;
				}
				else
				{
					resp.set_if_first_send(0);
				}
				m_send_csv_template_msg_array.push_back(resp);
				resp.clear_template_data();
			}
			game_proto_template_table_data* activity_template_data = resp.add_template_data();
			activity_template_data->CopyFrom(activity_template_data_temp);
		}
		if (if_first_send)
		{
			resp.set_if_first_send(1);
		}
		else
		{
			resp.set_if_first_send(0);
		}
		m_send_csv_template_msg_array.push_back(resp);
	}

	void csv_synchronization_fep_mgr::erase_from_send_csv_msg_map_by_session_uid(ui64 session_uid)
	{
		auto ite = m_send_csv_player_map.find(session_uid);
		if (ite != m_send_csv_player_map.end())
		{
			m_send_csv_player_map.erase(ite);
		}
	}

	bool csv_synchronization_fep_mgr::send_csv_msg_to_client(client_session* session_ptr, int32 msg_index)
	{
		if (nullptr == session_ptr || !session_ptr->is_vaild() || msg_index >= m_send_csv_template_msg_array.size())
		{
			return false;
		}
		else
		{
			security_communication_layer::getInstance().send_to_session(session_ptr->get_conn_index(), &m_send_csv_template_msg_array[msg_index], e_msgindex_s2c_time_limit_activity_template);
			return true;
		}
	}

	void csv_synchronization_fep_mgr::refresh_csv_template_msg_array()
	{
		m_send_csv_template_msg_array.clear();
		add_time_limit_activity_template("TimeLimitActivityTemplate");
		add_time_limit_branch_activity_template("TimeLimitActivityBranchTemplate");
	}

}