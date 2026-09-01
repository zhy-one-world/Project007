/********************************************************************
created:	2016年1月4日17:26:07
file base:	aoi_set_cpp
file ext:	cpp
author:		zhy

purpose:
*********************************************************************/

#include "aoi_set.h"
#include "aoi_event.h"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "logic/npc.hpp"
namespace faith
{
	aoi_set::aoi_set()
	{
		m_tower_pos_max.clear_data();
		m_aoi_tower_array.clear();
		m_map_begin_x = 0;	//地图的开始的x
		m_map_end_x = 0;	//地图的结束的x
		m_map_begin_y = 0;	//地图的开始的y
		m_map_end_y = 0;	//地图的结束的y
		m_tower_width = 0;	//格子宽度
		m_tower_length = 0;	//格子长度
		m_empty_map.clear();
	}
	aoi_set::~aoi_set()
	{
		m_aoi_tower_array.clear();
	}
	bool aoi_set::create_aoi(int32 begin_x, int32 end_x, int32 begin_y, int32 end_y, int32 tower_width, int32 tower_length)
	{
		ZoneScoped;
		if (begin_x > end_x || begin_y > end_y || tower_width == 0 || tower_length == 0)
		{
			CONSOLE_ERROR("begin_x:{} end_x:{} begin_y:{} end_y:{} tower_width:{} tower_length:{}", begin_x, end_x, begin_y, end_y, tower_width, tower_length);
			return false;
		}
		m_map_begin_x = begin_x;
		m_map_end_x = end_x;
		m_map_begin_y = begin_y;
		m_map_end_y = end_y;
		m_tower_width = tower_width;
		m_tower_length = tower_length;
		m_tower_pos_max.x = (m_map_end_x - m_map_begin_x) / tower_width + 3;//防止填的地图宽度太小
		m_tower_pos_max.y = (m_map_end_x - m_map_begin_x) / tower_length + 3;//防止填的地图宽度太小
		return true;
	}
	void aoi_set::get_unit_all(const fvector& unit_location, int32* unit_array, int32& watch_array_num)
	{
		ZoneScoped;
		if (!check_pos(unit_location))
			return;

		s_tower_pos tower_pos;
		trans_pos(unit_location, tower_pos);
		return get_tower(tower_pos)->get_unit_all(unit_array, watch_array_num);
	}
	const unit_index_map& aoi_set::get_watcher_all(const fvector& map_pos)
	{
		ZoneScoped;
		if (false == check_pos(map_pos))
		{
			return m_empty_map;
		}
		s_tower_pos tower_pos;
		trans_pos(map_pos, tower_pos);
		return get_tower(tower_pos)->get_watch_all();
	}
	void aoi_set::get_watcher_by_type(const fvector& map_pos_old, const fvector& map_pos_new, e_aoi_move_type move_type, int32* unit_ary, int32& watch_array_num)
	{
		ZoneScoped;
		if (false == check_pos(map_pos_old) || false == check_pos(map_pos_new))
		{
			return;
		}
		s_tower_pos tower_pos_old;
		trans_pos(map_pos_old, tower_pos_old);
		s_tower_pos tower_pos_new;
		trans_pos(map_pos_new, tower_pos_new);
		get_pos_limit(tower_pos_old, tower_pos_new, move_type, m_tower_array);

		for (int32 i = 1; i <= m_tower_array[0].x; i++)
		{
			s_tower_pos& temp_tower = m_tower_array[i];
			get_tower(temp_tower)->get_unit_all(unit_ary, watch_array_num);
		}
	}
	bool aoi_set::add_object(const fvector& unit_location, const int32 unit_index)
	{
		ZoneScoped;
		if (false == check_pos(unit_location))
		{
			return false;
		}
		aoi_event::add_object(unit_index);
		s_tower_pos tower_pos;
		trans_pos(unit_location, tower_pos);
		get_tower(tower_pos)->add_unit(unit_index);

		get_pos_limit(tower_pos, m_tower_array);
		for (int32 i = 1; i <= m_tower_array[0].x; ++i)
		{
			get_tower(m_tower_array[i])->add_watch(unit_index);
		}
		return true;
	}
	bool aoi_set::remove_object(const fvector& unit_location, const int32 unit_index, bool is_dead)
	{
		ZoneScoped;
		if (false == check_pos(unit_location))
		{
			return false;
		}
		s_tower_pos tower_pos;
		trans_pos(unit_location, tower_pos);
		get_tower(tower_pos)->remove_unit(unit_index);
		get_pos_limit(tower_pos, m_tower_array);
		for (int32 i = 1; i <= m_tower_array[0].x; ++i)
		{
			get_tower(m_tower_array[i])->remove_watch(unit_index);
		}

		npc& npc_ref = unit_man::get_npc(unit_index);

		bool is_gather_npc = false;
		if (npc_ref.is_valid() && npc_ref.get_npc_template()->NpcType == e_unit_type_npc)
		{
			switch (npc_ref.get_npc_template()->SubType)
			{
			case e_npc_type_interact_gather:
			case e_npc_type_world_boss_chests:
			case e_npc_type_touch_gather:
			case e_npc_type_hit_and_get_npc:
			case e_npc_type_island_small_chests:
			case e_npc_type_island_big_chests:
				is_gather_npc = true;
				break;
			}
		}

		if (false == is_dead || is_gather_npc)
		{
			aoi_event::remove_object(unit_index);
		}
		return true;
	}
	bool aoi_set::update_object(const fvector& map_pos_old, const fvector& map_pos_new, const int32& unit_index)
	{
		ZoneScoped;
		if (!check_pos(map_pos_old) || !check_pos(map_pos_new))
			return false;

		s_tower_pos tower_old_pos;
		trans_pos(map_pos_old, tower_old_pos);
		s_tower_pos tower_new_pos;
		trans_pos(map_pos_new, tower_new_pos);

		if (tower_old_pos.x != tower_new_pos.x || tower_old_pos.y != tower_new_pos.y)
		{
			auto old_tower = get_tower(tower_old_pos);
			auto new_tower = get_tower(tower_new_pos);
			old_tower->remove_unit(unit_index);
			new_tower->add_unit(unit_index);
		}
		if (tower_old_pos.x == tower_new_pos.x && tower_old_pos.y == tower_new_pos.y)
		{
			aoi_event::update_object(unit_index);
		}
		else
		{
			aoi_event::update_object(unit_index, e_aoi_move_type_update);
			get_pos_limit(tower_old_pos, m_tower_array);
			for (int32 i = 1; i <= m_tower_array[0].x; ++i)
			{
				get_tower(m_tower_array[i])->remove_watch(unit_index);
			}
			get_pos_limit(tower_new_pos, m_tower_array);
			for (int32 i = 1; i <= m_tower_array[0].x; ++i)
			{
				get_tower(m_tower_array[i])->add_watch(unit_index);
			}
		}
		return true;
	}
	const bool aoi_set::check_pos(const fvector& location)const
	{
		ZoneScoped;
		if (location.x < m_map_begin_x || location.y < m_map_begin_y || location.x > m_map_end_x || location.y > m_map_end_y)
		{
			return false;
		}
		return true;
	}
	const void aoi_set::trans_pos(const fvector& unit_location, s_tower_pos& tower_pos)const
	{
		ZoneScoped;
		if (m_tower_width == 0 || m_tower_length == 0)
		{
			return;
		}

		tower_pos.x = (unit_location.x - m_map_begin_x) / m_tower_width;
		tower_pos.y = (unit_location.y - m_map_begin_y) / m_tower_length;
		if (tower_pos.x < 0)
		{
			tower_pos.x = 0;
		}
		if (tower_pos.x >= m_tower_pos_max.x)
		{
			tower_pos.x = m_tower_pos_max.x - 1;
		}
		if (tower_pos.y < 0)
		{
			tower_pos.y = 0;
		}
		if (tower_pos.y >= m_tower_pos_max.y)
		{
			tower_pos.y = m_tower_pos_max.y - 1;
		}
	}
	void aoi_set::get_pos_limit(const s_tower_pos& tower_pos, s_tower_pos* result)
	{
		ZoneScoped;
		memset(result, 0, sizeof(s_tower_pos)* AOI_SEE_TOWER_NUM);
		if (tower_pos.x <= 0) {
			m_tower_num[0].x = 0;
			m_tower_num[1].x = 1;
		}
		else if (tower_pos.x >= m_tower_pos_max.x - 1) {
			m_tower_num[0].x = m_tower_pos_max.x - 2;
			m_tower_num[1].x = m_tower_pos_max.x - 1;
		}
		else {
			m_tower_num[0].x = tower_pos.x - 1;
			m_tower_num[1].x = tower_pos.x + 1;
		}

		if (tower_pos.y <= 0) {
			m_tower_num[0].y = 0;
			m_tower_num[1].y = 1;
		}
		else if (tower_pos.y >= m_tower_pos_max.y - 1) {
			m_tower_num[0].y = m_tower_pos_max.y - 2;
			m_tower_num[1].y = m_tower_pos_max.y - 1;
		}
		else {
			m_tower_num[0].y = tower_pos.y - 1;
			m_tower_num[1].y = tower_pos.y + 1;
		}
		for (int32 i = m_tower_num[0].x; i <= m_tower_num[1].x; i++) {
			for (int32 j = m_tower_num[0].y; j <= m_tower_num[1].y; j++) {
				int32 temp_num = result[0].x + 1;
				result[temp_num].x = i;
				result[temp_num].y = j;
				result[0].x = temp_num;
			}
		}
	}

	void aoi_set::get_pos_limit(const s_tower_pos& map_pos_old, const s_tower_pos& map_pos_new, e_aoi_move_type move_type, s_tower_pos* result)
	{
		ZoneScoped;
		memset(result, 0, sizeof(s_tower_pos)* AOI_SEE_TOWER_NUM);

		s_tower_pos pol_list_old[AOI_SEE_TOWER_NUM];
		s_tower_pos pol_list_new[AOI_SEE_TOWER_NUM];
		get_pos_limit(map_pos_old, pol_list_old);
		get_pos_limit(map_pos_new, pol_list_new);

		switch (move_type)
		{
		case e_aoi_move_type_add:
		{
			for (int32 new_index = 1; new_index <= pol_list_new[0].x; ++new_index)
			{
				bool is_new = true;
				for (int32 old_index = 1; old_index <= pol_list_old[0].x; ++old_index)
				{
					if (pol_list_new[new_index].x == pol_list_old[old_index].x && pol_list_new[new_index].y == pol_list_old[old_index].y)
					{
						is_new = false;
						break;
					}
				}
				if (is_new)
				{
					int32 temp_num = result[0].x + 1;
					result[temp_num].x = pol_list_new[new_index].x;
					result[temp_num].y = pol_list_new[new_index].y;
					result[0].x = temp_num;
				}
			}
		}
		break;
		case e_aoi_move_type_remove:
		{
			for (int32 old_index = 1; old_index <= pol_list_old[0].x; ++old_index)
			{
				bool is_old = true;
				for (int32 new_index = 1; new_index <= pol_list_new[0].x; ++new_index)
				{
					if (pol_list_new[new_index].x == pol_list_old[old_index].x && pol_list_new[new_index].y == pol_list_old[old_index].y)
					{
						is_old = false;
						break;
					}
				}
				if (is_old)
				{
					int32 temp_num = result[0].x + 1;
					result[temp_num].x = pol_list_old[old_index].x;
					result[temp_num].y = pol_list_old[old_index].y;
					result[0].x = temp_num;
				}
			}
		}
		break;
		case e_aoi_move_type_update:
		{
			for (int32 new_index = 1; new_index <= pol_list_new[0].x; ++new_index)
			{
				bool is_update = false;
				for (int32 old_index = 1; old_index <= pol_list_old[0].x; ++old_index)
				{
					if (pol_list_new[new_index].x == pol_list_old[old_index].x && pol_list_new[new_index].y == pol_list_old[old_index].y)
					{
						is_update = true;
						break;
					}
				}
				if (is_update)
				{
					int32 temp_num = result[0].x + 1;
					result[temp_num].x = pol_list_new[new_index].x;
					result[temp_num].y = pol_list_new[new_index].y;
					result[0].x = temp_num;
				}
			}
		}
		break;
		}
	}
	std::shared_ptr<aoi_tower> aoi_set::get_tower(const s_tower_pos& tower_pos)
	{
		ZoneScoped;
		auto tower = m_aoi_tower_array[tower_pos.x][tower_pos.y];
		if (!tower)
		{
			m_aoi_tower_array[tower_pos.x][tower_pos.y] = std::make_shared<aoi_tower>();
			tower = m_aoi_tower_array[tower_pos.x][tower_pos.y];
		}
		return tower;
	}
}