
#include "map_record.h"
#include "internet/map.pb.h"
#include "system/scene/box_map_system.h"
#include "system/scene/element_map_system.h"
#include "system/scene/battle_map_system.h"

using namespace faith;
map_record::map_record()
{
	m_map_record_data = nullptr;
	m_map_type = e_map_type_ui;
}
void map_record::set_data_info(int32 info_index, int32 value)
{
	m_map_record_data->set_data_info(info_index, value);
}
void map_record::add_data_info(int32 info_index, int32 value)
{
	m_map_record_data->add_data_info(info_index, value);
}
int32 map_record::get_data_info(int32 info_index)
{
	return m_map_record_data->get_data_info(info_index);
}
void map_record::cast_join_count(int32 join_count)
{
	m_map_record_data->cast_join_count(join_count);
}
int32 map_record::get_join_count()
{
	return m_map_record_data->get_join_count();
}
void map_record::clear_data_info()
{
	return m_map_record_data->clear_data_info();
}
std::shared_ptr<s_map_record_base> map_record::create_map_record_data(e_map_type map_type)
{
	m_map_type = map_type;
	if (map_type == e_map_type_big_map)
	{
		m_map_record_data = std::make_shared<s_box_map_info>();
	}
	if (map_type == e_map_type_demon_tower_fuben)
	{
		m_map_record_data = std::make_shared<s_element_map_info>();
	}
	if (map_type == e_map_type_battle)
	{
		m_map_record_data = std::make_shared<s_battle_map_info>();
	}
	return m_map_record_data;

}
void map_record::load_map_record(const map_s2s_map_record& msg, Entity* map_ent)
{
	m_map_id = msg.map_template_id();
	if (msg.has_box_map_data())
	{
		auto msg_data = msg.box_map_data();
		auto record_data = std::dynamic_pointer_cast<s_box_map_info>(create_map_record_data(e_map_type_big_map));
		for (int32 i = 0; i < msg_data.data_array_size(); i++)
		{
			record_data->set_data_info(i, msg_data.data_array(i));
		}
		for (auto& it : msg_data.npc_point_array())
		{
			s_box_map_info::npc_point_info temp_info;
			temp_info.m_npc_id = it.npc_id();
			temp_info.m_spawn_point_id = it.spawn_point_id();
			record_data->m_npc_point.push_back(temp_info);
		}
		record_data->m_flip_box.m_flip_box_id = msg_data.flip_box_info().flip_box_id();
		for (int32 i = 0; i < msg_data.flip_box_info().award_list_size(); i++)
		{
			record_data->m_flip_box.m_award_list.push_back(msg_data.flip_box_info().award_list(i));
		}
		for (int32 i = 0; i < msg_data.flip_box_info().open_list_size(); i++)
		{
			record_data->m_flip_box.m_open_list.insert(msg_data.flip_box_info().open_list(i));
		}
	}
	if (msg.has_element_map_data())
	{
		auto msg_data = msg.element_map_data();
		auto record_data = std::dynamic_pointer_cast<s_element_map_info>(create_map_record_data(e_map_type_demon_tower_fuben));
		for (int32 i = 0; i < msg_data.data_array_size(); i++)
		{
			record_data->set_data_info(i, msg_data.data_array(i));
		}
		m_map_record_data = record_data;
	}
	if (msg.has_battle_map_data())
	{
		auto msg_data = msg.battle_map_data();
		auto record_data = std::dynamic_pointer_cast<s_battle_map_info>(create_map_record_data(e_map_type_battle));
		for (int32 i = 0; i < msg_data.data_array_size(); i++)
		{
			record_data->set_data_info(i, msg_data.data_array(i));
		}
		m_map_record_data = record_data;
	}
}
void map_record::save_map_record(map_s2s_map_record* msg, Entity* map_ent)
{
	msg->set_map_template_id(m_map_id);
	if (m_map_type == e_map_type_big_map)
	{
		auto msg_data = msg->mutable_box_map_data();
		auto record_data = std::dynamic_pointer_cast<s_box_map_info>(m_map_record_data);
		for (int32 i = 0; i < e_role_box_map_max; i++)
		{
			msg_data->add_data_array(record_data->get_data_info(i));
		}
		for (auto& it : record_data->m_npc_point)
		{
			auto npc_point = msg_data->add_npc_point_array();
			npc_point->set_npc_id(it.m_npc_id);
			npc_point->set_spawn_point_id(it.m_spawn_point_id);
		}
		auto flip_box_msg = msg_data->mutable_flip_box_info();
		flip_box_msg->set_flip_box_id(record_data->m_flip_box.m_flip_box_id);
		for (auto& it : record_data->m_flip_box.m_award_list)
		{
			flip_box_msg->add_award_list(it);
		}
		for (auto& it : record_data->m_flip_box.m_open_list)
		{
			flip_box_msg->add_open_list(it);
		}
		box_map_system::on_player_save(map_ent, msg_data);
	}
	if (m_map_type == e_map_type_demon_tower_fuben)
	{
		auto msg_data = msg->mutable_element_map_data();
		auto record_data = std::dynamic_pointer_cast<s_element_map_info>(m_map_record_data);
		for (int32 i = 0; i < e_role_element_map_max; i++)
		{
			msg_data->add_data_array(record_data->get_data_info(i));
		}
		element_map_system::on_player_save(map_ent, msg_data);
	}
	if (m_map_type == e_map_type_battle)
	{
		auto msg_data = msg->mutable_battle_map_data();
		auto record_data = std::dynamic_pointer_cast<s_battle_map_info>(m_map_record_data);
		for (int32 i = 0; i < e_role_battle_map_max; i++)
		{
			msg_data->add_data_array(record_data->get_data_info(i));
		}
		battle_map_system::on_player_save(map_ent, msg_data);
	}
}

void map_record::send_map_record_msg(map_s2c_map_record* msg)
{
	msg->set_map_template_id(m_map_id);
	if (m_map_type == e_map_type_big_map)
	{
	}
	if (m_map_type == e_map_type_demon_tower_fuben)
	{
		auto record_data = std::dynamic_pointer_cast<s_element_map_info>(m_map_record_data);
		auto msg_data = msg->mutable_element_map_data();
		for (int32 i = 0; i < e_role_element_map_max; i++)
		{
			msg_data->add_data_array(record_data->get_data_info(i));
		}
	}
	if (m_map_type == e_map_type_battle)
	{
		auto record_data = std::dynamic_pointer_cast<s_battle_map_info>(m_map_record_data);
		auto msg_data = msg->mutable_battle_map_data();
		for (int32 i = 0; i < e_role_battle_map_max; i++)
		{
			msg_data->add_data_array(record_data->get_data_info(i));
		}
	}
}