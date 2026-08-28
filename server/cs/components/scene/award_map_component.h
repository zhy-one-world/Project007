#pragma	once

#include "logic/type_def.hpp"
#include "template/BrushStrangeTemplate_S.h"

namespace hld
{
	struct award_map_component
	{
		int32 m_player_index;
		std::map<int32, std::list<int32>>	m_npc_wave_map;
		std::list<int32>	m_npc_damage_list;
		int32 m_wave_index;
		BrushStrangeTemplate* m_brush_strange_template;
	};

	struct award_map_boss_component
	{
		bool m_be_call;
	};
}
