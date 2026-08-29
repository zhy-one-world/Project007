#pragma	once

#include "logic/type_def.hpp"
#include "template/MapTemplate_S.h"

namespace faith
{
	struct base_map_component
	{
		int32 m_map_id;
		int32 m_line_id;
		MapTemplate* m_map_template;
		std::set<int32> m_npc_list;
		std::set<int32> m_player_list;
	};
}
