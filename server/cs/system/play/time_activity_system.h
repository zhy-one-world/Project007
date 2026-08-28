#pragma once

#include "logic/type_def.hpp"

namespace hld
{
	class player;
	class c_time_activity;
	class time_activity_s2s_sl_activity_info;

	class time_activity_system
	{
	public:
		static bool start_up();
		static void shut_down(player* player_ptr);
		static void heart_tick(const int64& new_time);
		static void load_data_from_db(player* player_ptr, const time_activity_s2s_sl_activity_info& msg);
		static void save_data_to_db(player* player_ptr, int32 save_type_ex);
		static void send_time_activity_info_all(player* player_ptr);
		static void send_time_activity_info_one(player* player_ptr, int32 activity_id);
		static void send_time_activity_info_del(player* player_ptr, int32 activity_id);
	public:
		static std::shared_ptr<c_time_activity> get_time_activity(player* player_ptr, int32 activity_id);
		static void clear_old_activity(player* player_ptr);
		static void generate_new_activity(player* player_ptr);
	public:
		static void gm_clear_time_activity(player* player_ptr);
		static void gm_add_time_activity_score(player* player_ptr, int32 activity_id, int32 add_score);
	};
}
