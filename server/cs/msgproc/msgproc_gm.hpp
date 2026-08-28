#pragma once

#include "logic/type_def.hpp"
namespace hld
{
	class player;
	void init_gm_function_map();
	void do_gm_order_logic(player& player_ref, const std::string& gm_command, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_add_item(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_add_money(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_level_up(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_level_down(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_set_vip_point(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_modify_class(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_add_buff(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_del_buff(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_create_npc(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_clear_bag(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_clear_wave(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_add_offline_time(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_add_online_time(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_kill_self(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_mission_done(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_activate_all(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_kill_all(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_transfer_map(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_show_att(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_show_npc_pos(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_show_all_npc(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_add_treasure_pic(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_reload_data(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_clear_time_activity(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_add_time_activity_score(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
	void gm_change_online_time(player& player_ref, const std::vector<int>& arg_int, const std::vector<std::string>& arg_string);
}
