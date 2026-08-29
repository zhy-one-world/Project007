
#include "map_record_set.h"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "internet/map.pb.h"
#include "template/template_manager.h"
#include "logic/player.hpp"
#include "internal/char_msg.hpp"
#include "internet/net.pb.h"
#include "map_record.h"

using namespace faith;

map_record_set::map_record_set()
{
	m_unit_array_index = 0;
	clear_data();
}

void map_record_set::clear_data()
{
	m_map_record_map.clear();
}

void map_record_set::heart_tick(const int64& new_time)
{
}

void map_record_set::save_map_record_to_db(int32 save_type_ex)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	map_s2s_sl_map_record msg;

	for (auto& it : m_map_record_map)
	{
		it.second->save_map_record(msg.add_map_record_list(), player_ref.get_map_ent());
	}
	player_ref.send_message_to_dp(&msg, e_msgindex_dp2cs_save_char_data, e_data_flag_map, save_type_ex);
}

void map_record_set::load_map_record_by_db(const map_s2s_sl_map_record& msg)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	for (auto& it : msg.map_record_list())
	{
		auto map_template_id = it.map_template_id();
		auto map_template = GET_TEMPLATE(MapTemplate, map_template_id);
		if (map_template)
		{
			auto map_record_ptr = std::make_shared<map_record>();
			map_record_ptr->load_map_record(it, player_ref.get_map_ent());
			s_record_key record_key(map_template->Type, map_template->SubType);
			m_map_record_map[record_key.key] = map_record_ptr;
		}
	}

	send_map_record_all();
}

void map_record_set::send_map_record_all()
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player is not valid");
		return;
	}
	map_s2c_map_record_all msg;

	for (auto& it : m_map_record_map)
	{
		it.second->send_map_record_msg(msg.add_map_record_array());
		if (msg.ByteSize() > packege_length_max)
		{
			msg.set_is_end_record(0);
			player_ref.send_message_to_self(&msg, e_msgindex_s2c_update_map_record_all);
			msg.clear_map_record_array();
		}
	}

	msg.set_is_end_record(1);
	player_ref.send_message_to_self(&msg, e_msgindex_s2c_update_map_record_all);
}
void map_record_set::send_map_record_one(int32 record_key)
{
	player& player_ref = unit_man::get_player(m_unit_array_index);
	if (false == player_ref.is_valid())
	{
		CONSOLE_ERROR("player is not valid");
		return;
	}
	auto it = m_map_record_map.find(record_key);
	if (it == m_map_record_map.end())
	{
		CONSOLE_ERROR("record_key:{} is not exist", record_key);
		return;
	}
	map_s2c_map_record_all msg;

	it->second->send_map_record_msg(msg.add_map_record_array());
	msg.set_is_end_record(1);
	player_ref.send_message_to_self(&msg, e_msgindex_s2c_update_map_record_all);
}
std::shared_ptr<map_record> map_record_set::create_map_record(int32 map_template_id)
{
	auto map_template = GET_TEMPLATE(MapTemplate, map_template_id);
	if (nullptr == map_template)
	{
		CONSOLE_ERROR("map_template is null map_template_id:{}", map_template_id);
		return nullptr;
	}
	s_record_key record_key(map_template->Type, map_template->SubType);
	auto it = m_map_record_map.find(record_key.key);
	if (it == m_map_record_map.end())
	{
		auto map_record_ptr = std::make_shared<map_record>();
		map_record_ptr->set_map_id(map_template_id);
		map_record_ptr->create_map_record_data((e_map_type)map_template->Type);
		m_map_record_map[record_key.key] = map_record_ptr;
		return map_record_ptr;
	}
	return it->second;
}
std::shared_ptr<map_record> map_record_set::get_map_record(int32 map_template_id)
{
	auto map_template = GET_TEMPLATE(MapTemplate, map_template_id);
	if (nullptr == map_template)
	{
		CONSOLE_ERROR("map_template is null map_template_id:{}", map_template_id);
		return nullptr;
	}
	s_record_key record_key(map_template->Type, map_template->SubType);
	auto it = m_map_record_map.find(record_key.key);
	if (it == m_map_record_map.end())
	{
		return nullptr;
	}
	return it->second;
}
std::shared_ptr<map_record> map_record_set::get_map_record(e_map_type map_type, int32 sub_type)
{
	s_record_key record_key(map_type, sub_type);	
	auto it = m_map_record_map.find(record_key.key);
	if (it != m_map_record_map.end())
	{
		return it->second;
	}
	return nullptr;
}
void  map_record_set::upgrade_map(int32 map_template_id)
{
	auto map_template = GET_TEMPLATE(MapTemplate, map_template_id);
	if (nullptr == map_template)
	{
		CONSOLE_ERROR("map_template is null map_template_id:{}", map_template_id);
		return;
	}
	s_record_key record_key(map_template->Type, map_template->SubType);
	auto it = m_map_record_map.find(record_key.key);
	if (it != m_map_record_map.end())
	{
		it->second->set_map_id(map_template->NextMapId);
	}
	else
	{
		auto map_record_ptr = std::make_shared<map_record>();
		map_record_ptr->set_map_id(map_template->NextMapId);
		map_record_ptr->create_map_record_data((e_map_type)map_template->Type);
		m_map_record_map[record_key.key] = map_record_ptr;
	}
	send_map_record_one(record_key.key);
}
