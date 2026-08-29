#pragma once

#include "logic/type_def.hpp"
#include "base/ecs_world.h"

namespace faith
{
	class player;
	enum e_error_code;

	const int32 g_flip_box_count = 3;//开宝箱数量
	const int32 g_flip_box_win_count = 3;//开几个宝箱获胜

	class flip_box_system
	{
	public:
		static void start_up(Entity* map_ent, int32 flip_box_id, player* player_ptr);
		static void shut_down(Entity* map_ent);
		static void heart_tick(const int64& new_time);
		static void load_data_from_db(Entity* map_ent, player* player_ptr);
	public:
		static int32 open_flip_box(Entity* map_ent, player* player_ptr, int32 open_index);
	private:
		static void send_box_info(Entity* map_ent, player* player_ptr);
	};
}
