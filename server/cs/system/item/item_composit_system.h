#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace faith
{
	class item_proto_item_composit;
	class player;

	class item_composit_system
	{
	public:
		static void composit_operate_message(player* player_ptr, const item_proto_item_composit& msg);
	};
}
