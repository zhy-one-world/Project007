#pragma once
#include "logic/type_def.hpp"

namespace faith
{
	class aoi_proto_unit_aoi_all;
	class player;
	class npc;

	class box_award_system
	{
	public:
		static void start_up(player* player_ptr, npc* npc_ptr);
		static void shut_down(npc* npc_ptr);
		static void heart_tick(const int64& new_time);
		static void aoi_msg(npc* npc_ptr, aoi_proto_unit_aoi_all& msg);
	public:
		static void get_npc_award(player* player_ptr, npc* npc_ptr);
	private:
		static void play_collection(player* player_ptr, npc* npc_ptr);
		static void drop_item_by_player_search_level(player* player_ptr, npc* npc_ptr);
		static void drop_item_by_self_drop(player* player_ptr, npc* npc_ptr);
	};
}