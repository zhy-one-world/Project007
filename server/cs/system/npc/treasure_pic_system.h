#pragma once
#include "logic/type_def.hpp"

namespace hld
{
	class aoi_proto_unit_aoi_all;
	class player;
	class npc;

	class treasure_pic_system
	{
	public:
		static void start_up(player* player_ptr, npc* npc_ptr);
		static void shut_down(npc* npc_ptr);
	public:
		static void get_npc_award(player* player_ptr, npc* npc_ptr);
		static void get_treasure_award(player* player_ptr, npc* npc_ptr, int32 treasure_index);
	};
}