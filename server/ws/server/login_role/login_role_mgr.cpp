/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-17 13:19
	
	File Name :	login_role_mgr.cpp
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////

#include "utility/serialize_msg.h"
#include "world_server.hpp"
#include "login_role_mgr.h"
#include "base.hpp"
#include "character.pb.h"
#include "net.pb.h"
//////////////////////////////////////////////////////////////////////////
//	Class Implement
//////////////////////////////////////////////////////////////////////////
namespace faith
{

	login_role_mgr::login_role_mgr()
	{
		m_login_info_map.clear();
	}

	login_role_mgr::~login_role_mgr()
	{
	}
	void login_role_mgr::tick(float tick_time)
	{
		if (m_login_info_map.empty())
		{
			return;
		}
		for (login_info_map_it it = m_login_info_map.begin(); it != m_login_info_map.end();)
		{
			login_info_map_it temp_it = it++;
			if (temp_it->second.is_send())
			{
				guid_64 temp_guid(temp_it->first);
				send_role_login_data(temp_guid);
				m_login_info_map.erase(temp_it);
			}
		}
	}
	void login_role_mgr::set_account_info(const std::string& account, int32 role_num)
	{
		m_login_account_map[account] = role_num;
		for (login_info_map_it it = m_login_info_map.begin(); it != m_login_info_map.end();)
		{
			login_info_map_it temp_it = it++;
			int32 len = account.size() > max_account_length ? max_account_length : account.size();
			if (strncmp(account.c_str(), temp_it->second.role_info.account, len) == 0)
			{
				m_login_info_map.erase(temp_it);
			}			
		}
	}
	void login_role_mgr::set_role_info(const s_unit_info& role_info, const s_client_uid& client_uid)
	{
		login_info_map_it it = m_login_info_map.find(role_info.role_guid.server_64);
		if (it == m_login_info_map.end())
		{
			m_login_info_map[role_info.role_guid.server_64].role_info = role_info;
			m_login_info_map[role_info.role_guid.server_64].client_uid = client_uid;
		}
		else
		{
			it->second.role_info = role_info;
			it->second.client_uid = client_uid;
		}
	}
	void login_role_mgr::set_item_data(const guid_64& role_guid, const s_item_info* item_array)
	{
		if (nullptr == item_array)
		{
			return;
		}
		login_info_map_it it = m_login_info_map.find(role_guid.server_64);
		if (it == m_login_info_map.end())
		{
			return;
		}
		memcpy(it->second.item_data, item_array, sizeof(it->second.item_data));
		it->second.item_ok = true;
	}
	void login_role_mgr::set_buff_data(const guid_64& role_guid, const int32* buff_array)
	{
		if (nullptr == buff_array)
		{
			return;
		}
		login_info_map_it it = m_login_info_map.find(role_guid.server_64);
		if (it == m_login_info_map.end())
		{
			return;
		}
		memcpy(it->second.buff_data, buff_array, sizeof(it->second.buff_data));
		it->second.buff_ok = true;
	}
	void login_role_mgr::set_spirit_data(const guid_64& role_guid, const s_item_info& temp_spirit_info)
	{
		login_info_map_it it = m_login_info_map.find(role_guid.server_64);
		if (it == m_login_info_map.end())
		{
			return;
		}
		it->second.temp_sprite_info = temp_spirit_info;
		it->second.spirit_ok = true;
	}
	void login_role_mgr::set_time_data(const guid_64& role_guid, const int64 login_out_time)
	{
		login_info_map_it it = m_login_info_map.find(role_guid.server_64);
		if (it == m_login_info_map.end())
		{
			return;
		}
		it->second.log_out_time = login_out_time;
		it->second.time_ok = true;
	}

	void login_role_mgr::send_role_login_data(guid_64& role_guid)
	{
		login_info_map_it it = m_login_info_map.find(role_guid.server_64);
		
		if (it == m_login_info_map.end())
		{
			return;
		}

		s_login_info& login_info = it->second;
		login_account_map_it account_it = m_login_account_map.find(login_info.role_info.account);
		if (account_it != m_login_account_map.end())
		{
			account_it->second -= 1;
		}
		character_proto_enum_character_end_info char_info;
		char_info.set_role_guid(login_info.role_info.role_guid.server_64);
		char_info.set_name(login_info.role_info.role_name);
		char_info.set_appearance(login_info.role_info.role_appearance);
		char_info.set_template_id(login_info.role_info.data_ary[e_role_info_template_id]);
		char_info.set_exp_level(login_info.role_info.data_ary[e_role_info_exp_level]);
		char_info.set_del_time(login_info.role_info.data_ary[e_role_info_del_time]);
		char_info.set_wing_id(login_info.role_info.data_ary[e_role_info_wing_showd_template_id]);
		char_info.set_mount_id(login_info.role_info.data_ary[e_role_info_mount_showd_template_id]);
		char_info.set_show_fashion(login_info.role_info.data_ary[e_role_info_show_fashion]);
		char_info.set_head_frame_id(login_info.role_info.data_ary[e_role_info_head_frame]);
		if (account_it == m_login_account_map.end() || account_it->second <= 0)
		{
			char_info.set_last_role(1);
			char_info.set_is_only_create_role(world_server::getInstance().get_server_info_arr(e_server_info_type_only_create_role));
		}
		else
		{
			char_info.set_last_role(0);
			char_info.set_is_only_create_role(0);
		}
		for (int32 i_len = 0; i_len < role_show_item_num; ++i_len)
		{
			if (login_info.item_data[i_len].item_guid.is_valid())
			{
				character_proto_enum_character_item* item_info = char_info.add_item_info();
				item_info->add_item_guid(login_info.item_data[i_len].item_guid.A);
				item_info->add_item_guid(login_info.item_data[i_len].item_guid.B);
				item_info->add_item_ary(login_info.item_data[i_len].data_ary[e_item_info_info_id]);
				item_info->add_item_ary(login_info.item_data[i_len].data_ary[e_item_info_slot]);
				item_info->add_item_ary(login_info.item_data[i_len].data_ary[e_item_info_container_type]);
				item_info->add_item_ary(login_info.item_data[i_len].data_ary[e_item_info_activate]);
				item_info->add_item_ary(login_info.item_data[i_len].data_ary[e_item_info_upgrade_count]);
			}
		}
		for (int32 i_len = 0; i_len < max_enum_buff_data_num; i_len += e_enum_buff_info_max)
		{
			if (login_info.buff_data[i_len] > 0)
			{
				char_info.add_buff_info(login_info.buff_data[i_len]);
				char_info.add_buff_info(login_info.buff_data[i_len + 1]);
			}
		}
		char_info.set_spirit_guid(login_info.temp_sprite_info.item_guid);
		char_info.set_spirit_template_id(login_info.temp_sprite_info.data_ary[e_item_info_prototype_id]);
		char_info.set_spirit_illusion_index(login_info.temp_sprite_info.data_ary[e_item_info_showing_illusion_idex]);
		char_info.set_spirit_upgrade_count(login_info.temp_sprite_info.data_ary[e_item_info_upgrade_count]);
		char_info.set_login_out_time(login_info.log_out_time);

		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, &char_info, login_info.client_uid, e_msgindex_s2c_enum_char);
		world_server::getInstance().send_to_fep(login_info.client_uid.fepserver_uid, &msg, msg.get_packet_len());
	}
}