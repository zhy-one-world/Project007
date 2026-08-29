#pragma once

#include "logic/type_def.hpp"

namespace faith
{
	class player;

	class luck_draw_system
	{
	public:
		static void luck_draw_begin(player* player_ptr, int32 activity_id);
		static void luck_draw_end(player* player_ptr, int32 activity_id);
	};
}
