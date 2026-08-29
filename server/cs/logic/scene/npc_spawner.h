#pragma once

#include "Logic/map_def.hpp"
#include "../top_tip/plot_tip.h"

namespace faith
{
	enum e_respawn_period_tuple
	{
		e_respawn_period_tuple_begin_time,	// 重生时间段的起始时间
		e_respawn_period_tuple_end_time,	// 重生时间段的结束时间
		e_respawn_period_tuple_max
	};

	struct s_tigger_volume
	{
		int32       m_trigger_wave_id;					// 波次对应的波次ID
		int32		m_wave_num;							// 波次
		float		m_next_time;						// 下一波开始的剩余时间
		guid_64		m_npc_guid_ary[wave_npc_num_max];	// 这一波刷出的npc的index
		int32		m_npc_index_ary[wave_npc_num_max];	// 这一波刷出的npc的guid
		int32       m_npc_count;						// npc的数量
		plot_tip	m_plot_tip;							// 剧情提示管理器

		s_tigger_volume()
		{
			clear_data();
		}

		s_tigger_volume(int32 trigger_wave_id, int32 wave_num, float next_time)
		{
			m_trigger_wave_id = trigger_wave_id;
			m_wave_num = wave_num;
			m_next_time = next_time;
			m_npc_count = 0;
			memset(m_npc_guid_ary, 0, sizeof(m_npc_guid_ary));
			memset(m_npc_index_ary, 0, sizeof(m_npc_index_ary));
		}
		void clear_data()
		{
			m_trigger_wave_id = 0;
			m_wave_num = 0;
			m_next_time = 0;
			m_npc_count = 0;
			memset(m_npc_guid_ary, 0, sizeof(m_npc_guid_ary));
			memset(m_npc_index_ary, 0, sizeof(m_npc_index_ary));
		}

		bool is_npc_all_dead()
		{
			if (m_npc_count <= 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		int32 get_cur_npc_count()
		{
			return m_npc_count;
		}

		bool set_npc_index(const int32& unit_index, bool is_add)
		{
			if (is_add)
			{
				for (int32 i = 0; i < wave_npc_num_max; ++i)
				{
					if (m_npc_index_ary[i] <= 0)
					{
						m_npc_index_ary[i] = unit_index;
						return true;
					}
				}
			}
			else
			{
				for (int32 i = 0; i < wave_npc_num_max; ++i)
				{
					if (m_npc_index_ary[i] == unit_index)
					{
						m_npc_index_ary[i] = 0;
						--m_npc_count;
						if (m_npc_count < 0)
						{
							m_npc_count = 0;
						}
						return true;
					}
				}
			}

			return false;
		}

		bool set_npc_guid(const guid_64& unit_guid, bool is_add)
		{
			if (is_add)
			{
				for (int32 i = 0; i < wave_npc_num_max; ++i)
				{
					if (!m_npc_guid_ary[i].is_valid())
					{
						m_npc_guid_ary[i] = unit_guid;
						return true;
					}
				}
			}
			else
			{
				for (int32 i = 0; i < wave_npc_num_max; ++i)
				{
					if (m_npc_guid_ary[i] == unit_guid)
					{
						m_npc_guid_ary[i].clear_data();
						--m_npc_count;
						if (m_npc_count < 0)
						{
							m_npc_count = 0;
						}
						return true;
					}
				}
			}
			return false;
		}

		void set_npc_count(int32 npc_count)
		{
			m_npc_count = npc_count;
		}
	};

	typedef std::map<int32, s_tigger_volume>	tiggervolumemap;
	typedef tiggervolumemap::iterator	tiggervolumemap_it;

	// 刷怪队列的节点,节点中的怪会在delay_time表示的时间之后被刷出
	struct s_add_npc_queue_node
	{
		int32        npc_template_id;
		s_map_pos    born_pos;
		float        live_time;
		int64        delay_time;
		int32        dynamic_level;

		//下面这几个成员，是给 无限刷新 的怪用的，仍需加入波次信息
		int32		dead_time;
		int32		refresh_delta_time;
		guid_64		cur_npc_guid;

		s_tigger_volume* belong_wave;

		s_add_npc_queue_node()
		{
			npc_template_id = 0;
			live_time = 0.0f;
			delay_time = 0;
			dynamic_level = -1;
			belong_wave = nullptr;

			dead_time = 0;
			refresh_delta_time = 0;
			cur_npc_guid.clear_data();
		}
	};

	struct s_add_npc_base_data
	{
		guid_64 npc_guid;
		int32 npc_template_id;
		int32 npc_revive_time;
		s_map_pos born_pos;

		s_add_npc_base_data()
		{
			clear_data();
		}

		void clear_data()
		{
			npc_guid.clear_data();
			npc_template_id = 0;
			npc_revive_time = -1;
			born_pos.clear_data();
		}
	};
}