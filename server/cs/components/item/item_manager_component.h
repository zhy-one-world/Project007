#pragma	once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"
namespace faith
{
	struct item_manager_component
	{
		std::map<e_bag_type, std::set<Entity*>> m_bag_map;
	};
}
