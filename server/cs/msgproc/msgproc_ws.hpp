/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 11:47
  file base: msgproc_ws
  file ext: hpp
  author: lidongyang
  
  purpose: 
*********************************************************************/
#ifndef _MESSAGE_PROCESS_WS_HPP_
#define _MESSAGE_PROCESS_WS_HPP_

#include "logic/type_def.hpp"



namespace faith
{

	void ws2cs_req_reload_csv(uint32 conn_index, const void* data_ptr, size_t data_len);


	void ws2cs_req_del_unit(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_del_unit_lua(int64 role_id);

	void ws2cs_req_del_unit_item(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_change_unit_vip(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_del_unit_specified_item(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_add_guide_record(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_time_out(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_req_reconnect_game(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_reconnect_game_lua(int64 client_id, int64 role_id, int32 cur_map_id, int32 cur_map_line_id);

	void ws2cs_req_enter_game(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_req_enter_game(uint32 conn_index, const char *data_ptr, int32 data_len);

	///////////////////////////////////////////////////////////////////////////////////////////

	void ws2cs_req_logout_client(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_logout_client_lua(int64 role_id, bool need_send_save_end);

	void ws2cs_req_enter_scene(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_req_server_config(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_req_server_config(uint32 conn_index, const char *data_ptr, int32 data_len);

	void ws2cs_req_clear_server_config(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_map_create_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_map_delete_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_transfer_to_map_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_get_map_info_from_cs_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_get_map_info_from_cs_func(uint32 conn_index, const char * data_ptr, int32 data_len);

	void ws2cs_gm_open_time(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	void ws2cs_gm_begin_cross(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_server_info_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_server_info_all_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_server_refresh_daily_info_at_zero_hour(uint32 conn_index, const void* data_ptr, size_t data_len);

	// activity
	void ws2cs_broken_sky_sync_creat_boss_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_broken_sky_sync_creat_boss_func_lua(int32 in_boss_level, int64 in_all_damage, int64 in_from_map_guid);

	void ws2cs_broken_sky_sync_game_info_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_broken_sky_sync_game_info_func_lua(int32 in_map_template_id, int64 in_from_map_guid, int64 in_damage);

	void ws2cs_broken_sky_sync_kill_boss_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_broken_sky_sync_kill_boss_func_lua(int32 in_map_template_id, int64 in_killer_guid, int32 in_killer_unit_index);

	void ws2cs_chest_arrival_spawn_chests_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_chest_arrival_spawn_chests_func_lua(int32 in_chest_disappear_time);

	void ws2cs_clear_big_player_title_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	// team
	void ws2cs_apply_to_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_set_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_clear_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_transfer_team_member_to_scene_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_team_common_check_in_cs_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_sync_team_start_aim_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	// legion
	void ws2cs_set_player_legion_guid_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_set_player_legion_guid_func(uint32 conn_index, const char* data_ptr, int32 data_len);
	void ws2cs_clear_player_legion_info_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_create_legion_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_legion_donate_money_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_legion_donate_item_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_enter_legion_boss_map_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_get_legion_boss_award_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_set_get_legion_boss_award_log_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_fire_bonfire_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_add_fuel_to_bonfire_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_legion_rm_recruit_cd_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_draw_bonfire_daily_rwd_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	void ws2cs_level_up_legion_skill_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_legion_warehouse_operation(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_confirm_change_legion_name_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_rem_item_change_legion_name_care_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_change_gate_legion_name_func(uint32 conn_index, const void* data_ptr, size_t data_len);


	void ws2cs_begin_city_war_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_enter_city_war_map_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_city_war_operate_func(uint32 conn_index, const void* data_ptr, size_t data_len); 
	void ws2cs_get_construction_buff_func(uint32 conn_index, const void* data_ptr, size_t data_len); //无
	void ws2cs_get_occupation_daily_award_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_get_city_war_master_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_del_city_war_master_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_add_city_war_winner_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_del_city_war_winner_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_get_bonus_award_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_answer_question_right(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_set_legion_average_lv_to_map_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_rep_get_rank(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_res_challenge(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_get_choose_ist(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_resp_fresh_challenge_time(uint32 conn_index, const void* data_ptr, size_t data_len);//无

	//ranking 
	void ws2cs_worship_player_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_sync_first_rank_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_sync_server_avg_lv(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_service_rank_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	//marry
	void ws2cs_marry_check_item_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_marry_result_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_marry_result_end_proc(uint32 conn_index, const char* data_ptr, int32 data_len);
	void ws2cs_marry_get_reward_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_update_wedding_ring_state_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_update_wedding_ring_level_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_refresh_heart_value_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_cost_bless_money_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_return_special_item_times_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_marry_get_marry_task_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_marry_sync_marry_task(uint32 conn_index, const void* data_ptr, size_t data_len);

	// mail/notice/chat
	void ws2cs_receive_mail(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_receive_mail(uint32 conn_index, const char* data_ptr, int32 data_len);

	void ws2cs_refresh_globel_mail_sign_state(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_refresh_globel_mail_sign_state(uint32 conn_index, const char* data_ptr, int32 data_len);

	void ws2cs_receive_globel_message_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_receive_notice_proc(uint32 conn_index, const void* data_ptr, size_t data_len); // 沒有發送地方
	void ws2cs_send_chat_end_process(uint32 conn_index, const void* data_ptr, size_t data_len);

	//world_boss
	void ws2cs_create_world_boss_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_create_world_boss_proc(uint32 conn_index, const char* data_ptr, int32 data_len);
	void ws2cs_remove_world_boss_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_remove_world_boss_proc(uint32 conn_index, const char* data_ptr, int32 data_len);
	void ws2cs_create_first_kill_world_boss_prize(uint32 conn_index, const void* data_ptr, size_t data_len);

	//big_player
	void ws2cs_create_big_player_statue_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//role_right
	void ws2cs_role_right_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	//void ws2cs_gmtool_req_kickout_player(uint32 conn_index, const void* data_ptr, size_t data_len);

	void ws2cs_recharge_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_recharge_proc(uint32 conn_index, const char* data_ptr, int32 data_len);

	void ws2cs_role_info_change_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	//store
	void ws2cs_role_buy_good(uint32 conn_index, const void* data_ptr, size_t data_len);

	//red_bag
	void ws2cs_get_red_pack_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_send_red_pack_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//relation
	void ws2cs_full_friend_guid_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_req_relation_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//auction
	void ws2cs_auction_purchase_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_auction_purchase_success_proc(uint32 conn_index, const char* data_ptr, int32 data_len);

	void ws2cs_auction_sell_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_auction_sell_success_proc(uint32 conn_index, const char* data_ptr, int32 data_len);

	void ws2cs_auction_cancel_sell_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_auction_cancel_sell_end_proc(uint32 conn_index, const char* data_ptr, int32 data_len);

	void ws2cs_auction_sell_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_auction_sell_end_proc(uint32 conn_index, const char* data_ptr, int32 data_len);


	void ws2cs_auction_find_bid_target_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_auction_find_bid_target_end_proc(uint32 conn_index, const char* data_ptr, int32 data_len);



	//void ws2cs_bid_replace_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_time_out_del_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_time_out_del_proc(uint32 conn_index, const char* data_ptr, int32 data_len);

	//cloud shop
	void ws2cs_cloud_shop_buy_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void lua_ws2cs_cloud_shop_buy_proc(uint32 conn_index, const char *data_ptr, int32 data_len);

	//rename
	void ws2cs_sub_player_item_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2cs_change_player_name_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	//GM
	void ws2cs_gm_spawn_npc_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_gm_spawn_npc_func(uint32 conn_index, const char* data_ptr, int32 data_len);
	void ws2cs_gm_modify_money_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	//Server_Harry
	void ws2cs_set_cur_server_harry_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_set_cur_server_harry_func(uint32 conn_index, const char* data_ptr, int32 data_len);
	void ws2cs_send_create_normal_player_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_create_harry_player_statue_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_create_harry_player_statue_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_clear_harry_player_statue_proc(uint32 conn_index, const void* data_ptr, size_t data_len);



	//cross_ladder
	void ws2cs_cross_ladder_refresh_ticket_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void ws2cs_cross_ladder_send_last_score_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	//cross_server_world_boss
	void ws2cs_cross_server_world_boss_creat_boss_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_ws2cs_cross_server_world_boss_creat_boss_func(uint32 conn_index, const char* data_ptr, int32 data_len);

	//time_limit_activity
	void ws2cs_check_time_limit_activity_common_data_resp(uint32 conn_index, const void * data_ptr, size_t data_len);
	void ws2cs_gm_set_time_limit_activity_pause_flag_resp(uint32 conn_index, const void * data_ptr, size_t data_len); // 废弃
	void ws2cs_end_act_type_func(uint32 conn_index, const void * data_ptr, size_t data_len);
	void ws2cs_time_limit_act_temp(uint32 conn_index, const void *data_ptr, size_t data_len);
	void lua_ws2cs_time_limit_act_temp(uint32 conn_index, const char *data_ptr, int32 data_len);
	void ws2cs_time_limit_act_branch_temp(uint32 conn_index, const void *data_ptr, size_t data_len);
	void lua_ws2cs_time_limit_act_branch_temp(uint32 conn_index, const char *data_ptr, int32 data_len);

	//legion_station
	void ws2cs_send_bonfire_level_func(uint32 conn_index, const void * data_ptr, size_t data_len);  // 废弃
	void ws2cs_set_map_own_legion(uint32 conn_index, const void * data_ptr, size_t data_len);
	void ws2cs_transfer_all_player_this_map(uint32 conn_index, const void * data_ptr, size_t data_len);

	//assist_fight
	void ws2cs_sync_assist_fight_info_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void lua_ws2cs_sync_assist_fight_info_proc(uint32 conn_index, const char * data_ptr, int32 data_len);

	//pk_king
	void ws2cs_create_pk_king_combat_map_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void lua_ws2cs_create_pk_king_combat_map_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len);
	
	//element_war
	void ws2cs_element_war_map_init_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void lua_ws2cs_element_war_map_init_proc(uint32 conn_index, const char * data_ptr, int32 data_len);

	void ws2cs_element_war_get_mission_reward(uint32 conn_index, const void * data_ptr, size_t data_len);
	void lua_ws2cs_element_war_get_mission_reward(uint32 conn_index, const char * data_ptr, int32 data_len);

	void ws2cs_element_war_send_last_score_proc(uint32 conn_index, const void * data_ptr, size_t data_len);

	void ws2cs_set_ladder_world_level_proc(uint32 conn_index, const void * data_ptr, size_t data_len);

	void ws2cs_attack_city_init_data_proc(uint32 conn_index, const void * data_ptr, size_t data_len);

}
#endif
