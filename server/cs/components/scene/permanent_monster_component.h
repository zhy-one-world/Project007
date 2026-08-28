#pragma	once

#include "logic/type_def.hpp"

namespace hld
{
	struct permanent_monster_component
	{
	};
	struct target_time_monster_component
	{
		std::vector<target_time_spawn_info> m_target_time_spawn_vec;
	};
}
