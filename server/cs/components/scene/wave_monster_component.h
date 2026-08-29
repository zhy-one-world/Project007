#pragma	once

#include "logic/type_def.hpp"
#include "template/BrushStrangeTemplate_S.h"
namespace faith
{
	struct s_brush_info
	{
		int32 m_brush_id;
		int32 m_cur_wave_index;
		int64 m_next_wave_time;
		std::map<int32, int32> m_wave_list;
		BrushStrangeTemplate* m_brush_template;
		s_brush_info()
		{
			m_brush_id = 0;
			m_cur_wave_index = 0;
			m_next_wave_time = 0;
			m_wave_list.clear();
			m_brush_template = nullptr;
		}
	};
	struct wave_monster_component
	{
		std::unordered_map<int32, s_brush_info> m_brush_list;
	};
	struct wave_monster_tick_component
	{
	};
}
