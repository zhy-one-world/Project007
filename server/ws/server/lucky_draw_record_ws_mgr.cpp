#include "lucky_draw_record_ws_mgr.h"
#include "utility/cs_date.hpp"
#include "utility/guid_gen.h"
#include "utility/globle_data.h"
#include "client_session_mgr.hpp"
#include <mail_msg.hpp>
#include "ws_client.hpp"
#include <lucky_draw_record_msg.hpp>
#include "item.pb.h"
#include "net.pb.h"

namespace faith
{
	lucky_draw_record_ws_mgr::lucky_draw_record_ws_mgr()
	{
		clear_data();
	}

	lucky_draw_record_ws_mgr::~lucky_draw_record_ws_mgr()
	{


	}

	void lucky_draw_record_ws_mgr::clear_data()
	{
		m_sprite_record_array.clear();
		m_hope_item_record_array.clear();

	}

	void lucky_draw_record_ws_mgr::tick(int32 dt)
	{

	}

	void lucky_draw_record_ws_mgr::save_data_to_db()
	{
		//peer* dp = ws_client::getInstance().get_dp();
		//if (dp == NULL)
		//{
		//	return;
		//}


		//ws2dp_save_big_player_to_db msg;
		//for (int32 i = 0; i < e_big_player_type_max; ++i)
		//{
		//	msg.data_info[i].role_guid = player_guid_array[i];
		//	msg.data_info[i].player_type = i;
		//}
		//dp->send(&msg, sizeof(ws2dp_save_big_player_to_db));
	}
	void lucky_draw_record_ws_mgr::load_data_by_db()
	{
		server2dp_proto_ws2dp_load_draw_record_by_db msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_info_from_db);

		//ws2dp_load_draw_record_by_db msg;
		//ws_client::getInstance().send_to_dp(&msg, sizeof(ws2dp_load_draw_record_by_db));
	}

	bool record_sort(const s_hope_hunt_record_info& a, const s_hope_hunt_record_info& b)
	{
		if (b.record_time > a.record_time)
		{
			return true;
		}
		return false;
	}
	void lucky_draw_record_ws_mgr::load_data_by_db_end(const s_hope_hunt_record_info* record_info, int32 data_num)
	{
		m_sprite_record_array.clear();
		m_hope_item_record_array.clear();
		for (int32 i = 0; i < data_num; ++i)
		{
			if (i >= draw_record_total_num)
			{
				break;
			}
			switch (record_info[i].record_type)
			{
			case faith::e_luck_draw_record_type_sprite:
			{
				if (m_sprite_record_array.size() < hope_and_hunt_record_num)
				{
					m_sprite_record_array.push_back(record_info[i]);
				}
			}
			break;
			case faith::e_luck_draw_record_type_hope_item:
			{
				if (m_sprite_record_array.size() < hope_and_hunt_record_num)
				{
					m_hope_item_record_array.push_back(record_info[i]);
				}
			}
			break;
			default:
				return;
			}
		}
		m_sprite_record_array.sort(&record_sort);
		m_hope_item_record_array.sort(&record_sort);
	}

	void lucky_draw_record_ws_mgr::add_record(s_hope_hunt_record_info record_info)
	{
		if (!record_info.info_guid.is_valid())
		{
			return;
		}
		item_proto_lucky_draw_record_operate operate_msg;

		item_proto_lucky_draw_record* temp_record = operate_msg.add_add_array();
		temp_record->set_info_guid(record_info.info_guid.server_64);
		temp_record->set_role_guid(record_info.role_guid.server_64);
		temp_record->set_role_name(record_info.role_name);
		temp_record->set_item_template_id(record_info.item_template_id);
		temp_record->set_get_type(record_info.get_type);
		temp_record->set_record_type(record_info.record_type);
		temp_record->set_record_time(record_info.record_time);

		switch (record_info.record_type)
		{
		case faith::e_luck_draw_record_type_sprite:
		{
			if (m_sprite_record_array.size() >= hope_and_hunt_record_num)
			{
				for (int32 i = 0; i < hope_and_hunt_record_num; ++i)
				{//从头清，不用while防止死循环
					if (m_sprite_record_array.empty())
					{
						break;
					}
					server2dp_proto_ws2dp_del_draw_record del_msg;
					del_msg.set_info_guid(m_sprite_record_array.front().info_guid.server_64);
					ws_client::getInstance().send_to_dp_new(&del_msg, e_msg_ws2dp_del_draw_info_to_db);
					//ws2dp_del_draw_record del_msg;
					//del_msg.info_guid = m_sprite_record_array.front().info_guid;
					//ws_client::getInstance().send_to_dp(&del_msg, sizeof(ws2dp_del_draw_record));

					operate_msg.add_del_array(m_sprite_record_array.front().info_guid.server_64);

					m_sprite_record_array.pop_front();
					if (m_sprite_record_array.size() < hope_and_hunt_record_num)
					{
						break;
					}
				}
			}
			m_sprite_record_array.push_back(record_info);
		}
			break;
		case faith::e_luck_draw_record_type_hope_item:
		{
			if (m_hope_item_record_array.size() >= hope_and_hunt_record_num)
			{
				for (int32 i = 0; i < hope_and_hunt_record_num; ++i)
				{//从头清，不用while防止死循环
					if (m_hope_item_record_array.empty())
					{
						break;
					}
					server2dp_proto_ws2dp_del_draw_record del_msg;
					del_msg.set_info_guid(m_hope_item_record_array.front().info_guid.server_64);
					ws_client::getInstance().send_to_dp_new(&del_msg, e_msg_ws2dp_del_draw_info_to_db);

					//ws2dp_del_draw_record del_msg;
					//del_msg.info_guid = m_hope_item_record_array.front().info_guid;
					//ws_client::getInstance().send_to_dp(&del_msg, sizeof(ws2dp_del_draw_record));

					operate_msg.add_del_array(m_hope_item_record_array.front().info_guid.server_64);

					m_hope_item_record_array.pop_front();
					if (m_hope_item_record_array.size() < hope_and_hunt_record_num)
					{
						break;
					}
				}
			}
			m_hope_item_record_array.push_back(record_info);
		}
			break;
		default:
			return;
		}

		server2dp_proto_ws2dp_add_new_draw_record add_msg;
		add_msg.set_info_guid(record_info.info_guid.server_64);
		add_msg.set_role_guid(record_info.role_guid.server_64);
		add_msg.set_role_name(record_info.role_name);
		add_msg.set_item_template_id(record_info.item_template_id);
		add_msg.set_get_type(record_info.get_type);
		add_msg.set_record_type(record_info.record_type);
		add_msg.set_record_time(record_info.record_time);
		ws_client::getInstance().send_to_dp_new(&add_msg, e_msg_ws2dp_add_draw_info_to_db);

		//ws2dp_add_new_draw_record add_msg;
		//add_msg.record_info = record_info;
		//ws_client::getInstance().send_to_dp(&add_msg, sizeof(ws2dp_add_new_draw_record));

		client_session_mgr::getInstance().send_message_to_all_client(&operate_msg, e_msgindex_s2c_operate_record_info);
	}

	void lucky_draw_record_ws_mgr::send_record_all_to_req_player(guid_64 target_guid)
	{
		client_session* temp_session = client_session_mgr::getInstance().get_session(target_guid);
		if (temp_session == nullptr)
		{
			return;
		}

		item_proto_load_lucky_draw_record_end msg;
		for (std::list<s_hope_hunt_record_info>::iterator it = m_sprite_record_array.begin(); it != m_sprite_record_array.end(); ++it)
		{
			s_hope_hunt_record_info temp_info = *it;
			item_proto_lucky_draw_record* temp_record = msg.add_data_array();
			temp_record->set_info_guid(temp_info.info_guid.server_64);
			temp_record->set_role_guid(temp_info.role_guid.server_64);
			temp_record->set_role_name(temp_info.role_name);
			temp_record->set_item_template_id(temp_info.item_template_id);
			temp_record->set_get_type(temp_info.get_type);
			temp_record->set_record_type(temp_info.record_type);
			temp_record->set_record_time(temp_info.record_time);
			//防止数据过长无法发送
			if (msg.ByteSize() > packege_length_max)
			{
				temp_session->send_to_client(&msg, e_msgindex_s2c_load_record_info);
				return;
			}
		}

		for (std::list<s_hope_hunt_record_info>::iterator it = m_hope_item_record_array.begin(); it != m_hope_item_record_array.end(); ++it)
		{
			s_hope_hunt_record_info temp_info = *it;
			item_proto_lucky_draw_record* temp_record = msg.add_data_array();
			temp_record->set_info_guid(temp_info.info_guid.server_64);
			temp_record->set_role_guid(temp_info.role_guid.server_64);
			temp_record->set_role_name(temp_info.role_name);
			temp_record->set_item_template_id(temp_info.item_template_id);
			temp_record->set_get_type(temp_info.get_type);
			temp_record->set_record_type(temp_info.record_type);
			temp_record->set_record_time(temp_info.record_time);
			//防止数据过长无法发送
			if (msg.ByteSize() > packege_length_max)
			{
				temp_session->send_to_client(&msg, e_msgindex_s2c_load_record_info);
				return;
			}
		}

		temp_session->send_to_client(&msg, e_msgindex_s2c_load_record_info);
	}

	void lucky_draw_record_ws_mgr::change_player_name_func(const guid_64& role_guid, const xstring& role_name)
	{
		auto iter_sprite_record = m_sprite_record_array.begin();
		for (; iter_sprite_record != m_sprite_record_array.end(); ++iter_sprite_record)
		{
			if (iter_sprite_record->role_guid == role_guid)
			{
				iter_sprite_record->set_role_name(role_name);
			}
		}

		auto iter_hope_item_record = m_hope_item_record_array.begin();
		for (; iter_hope_item_record != m_hope_item_record_array.end(); ++iter_hope_item_record)
		{
			if (iter_sprite_record->role_guid == role_guid)
			{
				iter_hope_item_record->set_role_name(role_name);
			}
		}
	}
}
