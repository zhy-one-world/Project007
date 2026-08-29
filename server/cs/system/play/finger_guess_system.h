#pragma once

#include "logic/type_def.hpp"

namespace faith
{
	class citem;
	class player;

	class finger_guess_system
	{
	public:
		static bool start_up(player* player_ptr);
		static void shut_down(player* player_ptr);
		static void update();
		static void player_off_line(player* player_ptr);
	public:
		static void guess_end(player* player_ptr);
	};
}
