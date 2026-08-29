#include "logic/unit_man.h"
#include "server_log.hpp"
#include "player_aoi_watch.h"
#include "logic/player.hpp"
#include "utility/init_unit.h"

namespace faith
{
	//cs2fep_aoi_msg m_aoi_msg;
	player_aoi_watch::player_aoi_watch()
	{
		m_unit_index = 0;
		clear_data();
	}
	void player_aoi_watch::clear_data()
	{
		memset(m_aoi_watch, 0, sizeof(m_aoi_watch));
	}
	void player_aoi_watch::set_unit_index(const int32& unit_index)
	{
		m_unit_index = unit_index;
	}
	void player_aoi_watch::end_team(const int32& unit_index)
	{
		for (int32 i = aoi_watch_num - 1; i < aoi_watch_max; ++i)
		{
			if (m_aoi_watch[i] == unit_index)
			{
				m_aoi_watch[i] = 0;
				if (can_join_aoi(unit_index))
				{
					add_aoi_index(unit_index);
				}
			}
		}
	}
	bool player_aoi_watch::can_join_aoi(const int32& unit_index)
	{
		player& player_ref = unit_man::get_player(unit_index);
		player& self_player_ref = unit_man::get_player(m_unit_index);
		if (false == player_ref.is_valid() || false == self_player_ref.is_valid())
		{
			return false;
		}
		for (int32 i = 0; i < aoi_watch_num; i++)
		{
			if (m_aoi_watch[i] == unit_index)
			{
				return true;
			}
			if (0 == m_aoi_watch[i])
			{
				return true;
			}
		}
		if (player_ref.is_in_same_team(self_player_ref))
		{
			return true;
		}
		return false;
	}
	void player_aoi_watch::add_aoi_index(const int32& unit_index)
	{
		int32 empty_index = -1;
		for (int32 i = 0; i < aoi_watch_max; i++)
		{
			if (m_aoi_watch[i] == unit_index)
			{
				return;
			}
			if (empty_index == -1 && m_aoi_watch[i] == 0)
			{
				empty_index = i;
			}
		}
		if (empty_index == -1)
		{
			return;
		}
		m_aoi_watch[empty_index] = unit_index;
	}
	void player_aoi_watch::del_aoi_index(const int32& unit_index)
	{
		for (int32 i = 0; i < aoi_watch_max; ++i)
		{
			if (m_aoi_watch[i] == unit_index)
			{
				m_aoi_watch[i] = 0;
			}
		}
	}
	bool player_aoi_watch::is_in_aoi(const int32& unit_index)
	{
		for (int32 i = 0; i < aoi_watch_max; ++i)
		{
			if (m_aoi_watch[i] == unit_index)
			{
				return true;
			}
		}
		return false;
	}
	void player_aoi_watch::send_message_to_aoi(google::protobuf::Message* net_pro, uint32 header, bool to_self, bool use_length)
	{
		player& self_player_ref = unit_man::get_player(m_unit_index);
		if (false == self_player_ref.is_valid())
		{
			return;
		}
		int32 playr_num = 0;
		if (to_self)
		{
			if (self_player_ref.get_client_uid().is_valid())
			{
				self_player_ref.send_message_to_self(net_pro, header);
				//m_aoi_msg.client_uid[playr_num++] = self_player_ref.get_client_uid();
			}
		}
		for (int32 i = 0; i < aoi_watch_max; ++i)
		{
			const int32& aoi_unit_index = m_aoi_watch[i];
			if (aoi_unit_index == 0 || (to_self == false && aoi_unit_index == m_unit_index))
			{
				continue;
			}
			player& player_ref = unit_man::get_player(aoi_unit_index);
			if (use_length)
			{
				if (init_unit::is_in_distance(player_ref.get_new_map_pos().unit_location, self_player_ref.get_new_map_pos().unit_location, 2500) == false)
				{
					continue;
				}
			}
			if (player_ref.get_client_uid().is_valid())
			{
				player_ref.send_message_to_self(net_pro, header);
				//m_aoi_msg.client_uid[playr_num++] = player_ref.get_client_uid();
			}
		}
		//if (playr_num > 0)
		//{
		//	m_aoi_msg.data_num = playr_num;
		//	m_aoi_msg.wheader = e_msg_index_cs2fep_aoi_msg;
		//	m_aoi_msg.header = header;
		//	m_aoi_msg.data_size = net_pro->ByteSize();
		//	if (net_pro->SerializeToArray(m_aoi_msg.data, sizeof(m_aoi_msg.data)))
		//	{
		//		cell_server::getInstance().broadcast(&m_aoi_msg, m_aoi_msg.get_pak_length(), e_server_type_fep);
		//		//unit_man::log_msg_info(header, net_pro->ByteSize());
		//	}
		//	else
		//	{
		//		CONSOLE_INFO("header:{}", header);
		//	}
		//}
	}
	void  player_aoi_watch::send_message_to_aoi(const char* msg, int32 msg_len, uint32 header, bool to_self, bool use_length)
	{
		player& self_player_ref = unit_man::get_player(m_unit_index);
		if (false == self_player_ref.is_valid())
		{
			return;
		}
		int32 playr_num = 0;
		if (to_self)
		{
			if (self_player_ref.get_client_uid().is_valid())
			{
				self_player_ref.send_message(msg, msg_len);
				//m_aoi_msg.client_uid[playr_num++] = self_player_ref.get_client_uid();
			}
		}
		for (int32 i = 0; i < aoi_watch_max; ++i)
		{
			const int32& aoi_unit_index = m_aoi_watch[i];
			if (aoi_unit_index == 0 || (to_self == false && aoi_unit_index == m_unit_index))
			{
				continue;
			}
			player& player_ref = unit_man::get_player(aoi_unit_index);
			if (use_length)
			{
				if (init_unit::is_in_distance(player_ref.get_new_map_pos().unit_location, self_player_ref.get_new_map_pos().unit_location, 2500) == false)
				{
					continue;
				}
			}
			if (player_ref.get_client_uid().is_valid())
			{

				player_ref.send_message(msg, msg_len);
				//m_aoi_msg.client_uid[playr_num++] = player_ref.get_client_uid();
			}
		}
		//if (playr_num > 0)
		//{
		//	m_aoi_msg.data_num = playr_num;
		//	m_aoi_msg.wheader = e_msg_index_cs2fep_aoi_msg;
		//	m_aoi_msg.header = header;
		//	memcpy(m_aoi_msg.data, msg, msg_len);
		//	m_aoi_msg.data_size = msg_len;
		//	cell_server::getInstance().broadcast(&m_aoi_msg, m_aoi_msg.get_pak_length(), e_server_type_fep);
		//	//unit_man::log_msg_info(header, net_pro->ByteSize());
		//}
	}
}