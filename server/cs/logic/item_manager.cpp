
#include "logic/item_manager.h"
#include "logic/item_set.h"
#include "count_def.hpp"

namespace faith
{
	item_set m_empty_item_set;
	item_set* m_item_player_ary[init_player_max];
	item_set* m_item_npc_ary[init_npc_max];

	void item_manager::init_manager()
	{
		memset(m_item_player_ary, 0, sizeof(m_item_player_ary));
		memset(m_item_npc_ary, 0, sizeof(m_item_npc_ary));
	}

	// 对所有物品发射计时脉冲
	void item_manager::heart_tick(int32 unit_array_index, const int64& new_time)
	{
		item_set& temp_item_set = get_item_set(unit_array_index);
		temp_item_set.heart_tick(new_time);
	}

	// 清空所有物品的数据(包括物品栏+装备栏)
	void item_manager::clear_data(int32 unit_array_index)
	{
		item_set& temp_item_set = get_item_set(unit_array_index);
		temp_item_set.clear_data();
	}

	item_set& item_manager::get_item_set(int32 unit_array_index)
	{
		if (unit_array_index >= npc_arrary_index_begin)
		{
			int32 npc_array_index = unit_array_index - npc_arrary_index_begin;
			if (npc_array_index >= 0 && npc_array_index < init_npc_max)
			{
				if (m_item_npc_ary[npc_array_index] == nullptr)
				{
					m_item_npc_ary[npc_array_index] = new item_set();
					m_item_npc_ary[npc_array_index]->clear_data();
					m_item_npc_ary[npc_array_index]->set_unit_ptr(unit_array_index);
				}
				return *m_item_npc_ary[npc_array_index];
			}
		}
		else
		{
			if (unit_array_index >= 0 && unit_array_index < init_player_max)
			{
				if (m_item_player_ary[unit_array_index] == nullptr)
				{
					m_item_player_ary[unit_array_index] = new item_set();
					m_item_player_ary[unit_array_index]->clear_data();
					m_item_player_ary[unit_array_index]->set_unit_ptr(unit_array_index);
				}
				return *m_item_player_ary[unit_array_index];
			}
		}
		return m_empty_item_set;
	}
}