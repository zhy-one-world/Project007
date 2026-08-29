/********************************************************************
created:	2016Äê1ÔÂ4ÈÕ17:26:07
file base:	aoi_event_h
file ext:	cpp
author:		zhy

purpose:
*********************************************************************/

#include "aoi_event.h"
#include "aoi_system.h"
#include "internet/aoi.pb.h"
#include "logic/buff_man.h"
#include "logic/unit.hpp"
#include "logic/unit_man.h"
#include "internet/net.pb.h"
#include "server_log.hpp"
#include "aoi_def.h"
namespace faith
{
	int32 m_aoi_tower_ary[init_player_max];
	aoi_proto_aoi_pos_update m_aoi_update_msg;
	aoi_proto_unit_aoi_all m_aoi_in_all;
	aoi_proto_unit_aoi_out m_aoi_out_msg;

	void aoi_event::add_object(const int32& unit_index)
	{
		ZoneScoped;
		unit& add_unit = unit_man::get_unit(unit_index);
		if (false == add_unit.is_valid())
		{
			return;
		}
		if (add_unit.get_unit_type() == e_unit_type_player)
		{
			const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(add_unit.get_map_ent(), add_unit.get_old_map_pos().unit_location);
			if (aoi_tower_watch.empty())
			{
				return;
			}
			for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
			{
				const int32& unit_array_index = it->first;
				if (unit_array_index == unit_index || unit_array_index == 0)
				{
					continue;
				}
				unit& temp_unit_ref = unit_man::get_unit(unit_array_index);
				if (temp_unit_ref.is_valid() == false)
				{
					continue;
				}
				if (temp_unit_ref.get_unit_type() != e_unit_type_player && temp_unit_ref.is_dead())
				{
					continue;
				}
				if (temp_unit_ref.aoi_watch_all_data_in(unit_index) && add_unit.aoi_watch_all_data_in(unit_array_index))
				{
					m_aoi_in_all.Clear();
					temp_unit_ref.get_aoi_msg(m_aoi_in_all);
					add_unit.send_message_to_self(&m_aoi_in_all, e_msgindex_s2c_aoi_simple_one);
					buff_man::sync_buff_set_info(unit_array_index, unit_index);
					m_aoi_in_all.Clear();
				}
				else
				{
					temp_unit_ref.aoi_watch_all_data_out(unit_index);
					add_unit.aoi_watch_all_data_out(unit_array_index);
				}
			}
		}
		m_aoi_in_all.Clear();
		add_unit.get_aoi_msg(m_aoi_in_all);
		add_unit.send_message_to_aoi(&m_aoi_in_all, e_msgindex_s2c_aoi_simple_one, false, false);
		buff_man::sync_buff_set_info(unit_index, 0);
	}
	void aoi_event::add_object(const int32& unit_index, e_aoi_move_type move_type)
	{
		ZoneScoped;
		unit& add_unit = unit_man::get_unit(unit_index);
		if (false == add_unit.is_valid())
		{ 
			return;
		}
		if (add_unit.get_unit_type() == e_unit_type_player)
		{
			int32 watch_array_num = 0;
			aoi_system::get_watch_by_type(add_unit.get_map_ent(), add_unit.get_old_map_pos().unit_location, add_unit.get_new_map_pos().unit_location, move_type, m_aoi_tower_ary, watch_array_num);

			for (int32 i = 0; i < watch_array_num; ++i)
			{
				const int32& unit_array_index = m_aoi_tower_ary[i];
				if (unit_array_index == unit_index || unit_array_index == 0)
				{
					continue;
				}
				unit& temp_unit_ref = unit_man::get_unit(unit_array_index);
				if (temp_unit_ref.is_valid() == false)
				{
					continue;
				}
				if (temp_unit_ref.get_unit_type() != e_unit_type_player && temp_unit_ref.is_dead())
				{
					continue;
				}
				if (temp_unit_ref.aoi_watch_all_data_in(unit_index) && add_unit.aoi_watch_all_data_in(unit_array_index))
				{
					m_aoi_in_all.Clear();
					temp_unit_ref.get_aoi_msg(m_aoi_in_all);
					add_unit.send_message_to_self(&m_aoi_in_all, e_msgindex_s2c_aoi_simple_one);
					buff_man::sync_buff_set_info(unit_array_index, unit_index);
				}
				else
				{
					temp_unit_ref.aoi_watch_all_data_out(unit_index);
					add_unit.aoi_watch_all_data_out(unit_array_index);
				}
			}
		}
		m_aoi_in_all.Clear();
		add_unit.get_aoi_msg(m_aoi_in_all);
		aoi_system::send_message_aoi_by_type(
			add_unit.get_map_ent(),
			&m_aoi_in_all,
			e_msgindex_s2c_aoi_simple_one,
			add_unit.get_old_map_pos().unit_location,
			add_unit.get_new_map_pos().unit_location,
			move_type,
			add_unit.get_array_index());
		buff_man::sync_buff_set_info(unit_index, 0);
	}
	void aoi_event::remove_object(const int32& unit_index)
	{
		ZoneScoped;
		unit& remove_unit = unit_man::get_unit(unit_index);
		if (false == remove_unit.is_valid())
		{
			return;
		}

		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(remove_unit.get_map_ent(), remove_unit.get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return;
		}
		if (remove_unit.get_unit_type() == e_unit_type_player)
		{
			m_aoi_out_msg.Clear();
			for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
			{
				const int32& unit_array_index = it->first;
				if (unit_array_index != unit_index)
				{
					unit& unit_ref = unit_man::get_unit(unit_array_index);
					if (unit_ref.is_valid())
					{
						m_aoi_out_msg.add_unit_guid(unit_ref.get_unit_guid().server_64);
						if (m_aoi_out_msg.ByteSize() >= packege_length_max)
						{
							remove_unit.send_message_to_self(&m_aoi_out_msg, e_msgindex_s2c_aoi_out);
							m_aoi_out_msg.clear_unit_guid();
						}
					}
				}
			}
			if (m_aoi_out_msg.unit_guid_size())
			{
				remove_unit.send_message_to_self(&m_aoi_out_msg, e_msgindex_s2c_aoi_out);
			}
		}
		m_aoi_out_msg.Clear();
		m_aoi_out_msg.add_unit_guid(remove_unit.get_unit_guid().server_64);
		aoi_system::send_message_aoi_all(
			remove_unit.get_map_ent(),
			&m_aoi_out_msg,
			e_msgindex_s2c_aoi_out,
			remove_unit.get_old_map_pos().unit_location,
			unit_index);
		if (remove_unit.get_unit_type() == e_unit_type_player)
		{
			for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
			{
				const int32& unit_array_index = it->first;
				unit& unit_ref = unit_man::get_unit(unit_array_index);
				if (unit_ref.is_valid())
				{
					remove_unit.aoi_watch_all_data_out(unit_array_index);
					unit_ref.aoi_watch_all_data_out(unit_index);
				}
			}
		}
	}
	void aoi_event::remove_object(const int32& unit_index, e_aoi_move_type move_type)
	{
		ZoneScoped;
		unit& remove_unit = unit_man::get_unit(unit_index);
		if (false == remove_unit.is_valid())
		{
			return;
		}

		int32 watch_array_num = 0;
		aoi_system::get_watch_by_type(remove_unit.get_map_ent(), remove_unit.get_old_map_pos().unit_location, remove_unit.get_new_map_pos().unit_location, move_type, m_aoi_tower_ary, watch_array_num);
		if (watch_array_num > 0)
		{
			if (remove_unit.get_unit_type() == e_unit_type_player)
			{
				m_aoi_out_msg.Clear();
				int32 add_guid_num = 0;
				for (int32 i = 0; i < watch_array_num; ++i)
				{
					unit& unit_ref = unit_man::get_unit(m_aoi_tower_ary[i]);
					if (unit_ref.is_valid())
					{
						m_aoi_out_msg.add_unit_guid(unit_ref.get_unit_guid().server_64);
						add_guid_num++;
						if (add_guid_num * 10 >= packege_length_max)
						{
							add_guid_num = 0;
							remove_unit.send_message_to_self(&m_aoi_out_msg, e_msgindex_s2c_aoi_out);
							m_aoi_out_msg.clear_unit_guid();
						}
					}
				}
				if (m_aoi_out_msg.unit_guid_size())
				{
					remove_unit.send_message_to_self(&m_aoi_out_msg, e_msgindex_s2c_aoi_out);
				}
			}
		}
		m_aoi_out_msg.Clear();
		m_aoi_out_msg.add_unit_guid(remove_unit.get_unit_guid().server_64);
		aoi_system::send_message_aoi_by_type(
			remove_unit.get_map_ent(),
			&m_aoi_out_msg,
			e_msgindex_s2c_aoi_out,
			remove_unit.get_old_map_pos().unit_location,
			remove_unit.get_new_map_pos().unit_location,
			move_type,
			unit_index);
		aoi_system::get_watch_by_type(remove_unit.get_map_ent(), remove_unit.get_old_map_pos().unit_location, remove_unit.get_new_map_pos().unit_location, move_type, m_aoi_tower_ary, watch_array_num);
		if (watch_array_num > 0)
		{
			if (remove_unit.get_unit_type() == e_unit_type_player)
			{
				for (int32 i = 0; i < watch_array_num; ++i)
				{
					unit& unit_ref = unit_man::get_unit(m_aoi_tower_ary[i]);
					if (unit_ref.is_valid())
					{
						remove_unit.aoi_watch_all_data_out(m_aoi_tower_ary[i]);
						unit_ref.aoi_watch_all_data_out(unit_index);
					}
				}
			}
		}
	}
	void aoi_event::update_object(const int32& unit_index)
	{
		ZoneScoped;
		unit& update_unit = unit_man::get_unit(unit_index);
		if (false == update_unit.is_valid() || update_unit.get_unit_type() != e_unit_type_player)
		{
			return;
		}
		const s_map_pos& new_map_pos = update_unit.get_new_map_pos();
		const s_map_pos& old_map_pos = update_unit.get_old_map_pos();
		if (new_map_pos != old_map_pos)
		{
			m_aoi_update_msg.Clear();
			m_aoi_update_msg.set_unit_guid(update_unit.get_unit_guid().server_64);
			m_aoi_update_msg.add_move_vector(new_map_pos.unit_location.x);
			m_aoi_update_msg.add_move_vector(new_map_pos.unit_location.y);
			m_aoi_update_msg.add_move_vector(new_map_pos.unit_location.z);
			aoi_system::send_message_aoi_all(
				update_unit.get_map_ent(),
				&m_aoi_update_msg,
				e_msgindex_s2c_aoi_update,
				update_unit.get_old_map_pos().unit_location,
				unit_index,
				false);
		}
	}
	void aoi_event::update_object(const int32& unit_index, e_aoi_move_type move_type)
	{
		ZoneScoped;
		unit& update_unit = unit_man::get_unit(unit_index);
		if (false == update_unit.is_valid())
		{
			return;
		}
		if (move_type == e_aoi_move_type_update)
		{
			add_object(unit_index, e_aoi_move_type_add);
			remove_object(unit_index, e_aoi_move_type_remove);
		}
		if (update_unit.get_unit_type() != e_unit_type_player)
		{
			return;
		}
		const s_map_pos& new_map_pos = update_unit.get_new_map_pos();
		const s_map_pos& old_map_pos = update_unit.get_old_map_pos();
		if (new_map_pos != old_map_pos)
		{
			m_aoi_update_msg.Clear();
			m_aoi_update_msg.set_unit_guid(update_unit.get_unit_guid().server_64);
			m_aoi_update_msg.add_move_vector(new_map_pos.unit_location.x);
			m_aoi_update_msg.add_move_vector(new_map_pos.unit_location.y);
			m_aoi_update_msg.add_move_vector(new_map_pos.unit_location.z);
			aoi_system::send_message_aoi_by_type(
				update_unit.get_map_ent(),
				&m_aoi_update_msg,
				e_msgindex_s2c_aoi_update,
				update_unit.get_old_map_pos().unit_location,
				update_unit.get_new_map_pos().unit_location,
				move_type,
				unit_index);
		}
	}
}