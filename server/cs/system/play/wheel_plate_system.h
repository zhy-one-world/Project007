#pragma once

#include "logic/type_def.hpp"

namespace faith
{
	class player;
	class npc;
	class aoi_proto_unit_aoi_all;

	class wheel_plate_system
	{
	public:
		static void start_up(player* player_ptr, npc* npc_ptr);
		static void shut_down(npc* npc_ptr);
		static void aoi_msg(npc* npc_ptr, aoi_proto_unit_aoi_all& msg);
	public:
		static void wheel_plate_begin(player* player_ptr, int32 activity_id);
		static void wheel_plate_end(player* player_ptr, int32 activity_id);
	public:
		static void wheel_plate_add_score(player* player_ptr, npc* npc_ptr);
	private:
		static void wheel_plate_create_score(npc* npc_ptr, int32 activity_id, e_time_activity_score_type score_type);
		static e_time_activity_score_type get_activity_score_type(int32 sub_type);
	};
}
