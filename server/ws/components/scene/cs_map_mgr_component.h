#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"
#include "map_def.hpp"

namespace faith
{	
	struct cs_map_mgr_component
	{
		std::unordered_map<int32, std::set<Entity*>> m_cs_map;
		std::unordered_map<int32, int32> m_line_info_map;
		std::vector<s_double_line_info>	m_double_line_info;
		std::unordered_map<int32, std::unordered_map<ui64, uint32>> m_rm_units_stamp_data;
	};
}