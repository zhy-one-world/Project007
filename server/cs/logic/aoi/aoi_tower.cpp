/********************************************************************
created:	2016年1月4日17:26:07
file base:	aoi_tower_h
file ext:	cpp
author:		zhy

purpose:
*********************************************************************/

#include "aoi_tower.h"
#include "count_def.hpp"

namespace faith
{
	aoi_tower::aoi_tower()
	{
		clear_data();
	}
	aoi_tower::~aoi_tower()
	{
	}
	void aoi_tower::clear_data()
	{
		m_unit_index_list.clear();
		m_watch_index_list.clear();
	}
	bool aoi_tower::get_unit_index(const int32& unit_index)
	{
		unit_index_map_it it = m_unit_index_list.find(unit_index);
		if (it != m_unit_index_list.end())
		{
			return true;
		}
		return false;
	}
	void aoi_tower::add_unit(const int32& unit_index)
	{
		m_unit_index_list[unit_index] = 1;
	}
	void aoi_tower::remove_unit(const int32& unit_index)
	{
		unit_index_map_it it = m_unit_index_list.find(unit_index);
		if (it != m_unit_index_list.end())
		{
			m_unit_index_list.erase(it);
		}
	}
	void aoi_tower::get_unit_all(int32* unit_ary, int32& watch_array_num)
	{
		for (unit_index_map_it it = m_unit_index_list.begin(); it != m_unit_index_list.end(); ++it)
		{
			if (watch_array_num >= init_player_max)
			{
				return;
			}
			unit_ary[watch_array_num] = it->first;
			++watch_array_num;
		}
	}
	void aoi_tower::add_watch(const int32& unit_index)
	{
		m_watch_index_list[unit_index] = 1;
	}
	void aoi_tower::remove_watch(const int32& unit_index)
	{
		unit_index_map_it it = m_watch_index_list.find(unit_index);
		if (it != m_watch_index_list.end())
		{
			m_watch_index_list.erase(it);
		}
	}
	const unit_index_map& aoi_tower::get_watch_all() const
	{
		return m_watch_index_list;
	}
}