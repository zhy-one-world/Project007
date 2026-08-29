#pragma once
#include "logic/type_def.hpp"

namespace faith
{
	class player;

	class online_award_system
	{
	public:
		static bool start_up(player* player_ptr);
		static void shut_down(player* player_ptr);
		static void heart_tick(player* player_ptr, const int64& new_time, const int32& tick_time);
	public:
		static void get_online_award(player* player_ptr);
	};
}