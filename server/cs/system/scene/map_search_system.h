#pragma once
#include "logic/type_def.hpp"

namespace faith
{
	class map_search_system
	{
	public:
		static bool start_up(int32 player_index);
		static void shut_down(int32 player_index);
		static void heart_tick(int32 player_index, const int64& new_time, const int32& tick_time);
	public:
		static int32 upgrade_search_level(int32 player_index);
	};
}