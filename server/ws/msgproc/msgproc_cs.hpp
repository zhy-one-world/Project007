/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   15:01
	file base:	msgproc_cs
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WS_MSGPROC_CS_H_
#define _WS_MSGPROC_CS_H_

 #include "logic/type_def.hpp"

namespace hld
{
	// 	class client_session;
	void cs2ws_rep_reconnect_game(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_rep_enter_game(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_rep_enter_scene(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_rep_logout(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_transfer_player(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_sync_character_data(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_rep_role_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_rep_map_state(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	// map
	void cs2ws_req_set_big_player_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_broken_sky_creat_boss(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_broken_sky_info_sync(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_broken_sky_kill_boss(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_rep_get_map_info_from_cs_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_pk_king_begin(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_receive_role_info_gm(uint32 conn_index, const void* data_ptr, size_t data_len);

	// team
	void cs2ws_req_update_team_member_info_one(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_update_team_member_pos_info_one(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_create_team(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_join_team(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_change_team_type(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_apply_to_join_team(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_del_team_member(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_team_game_over(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_recv_team_common_check_in_cs(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_team_change_aim_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_after_team_aim_finish_op(uint32 conn_index, const void* data_ptr, size_t data_len);

	// legion
	void cs2ws_req_create_legion_result(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_convenient_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_donate_legion_money(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_donate_legion_item(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_kill_legion_boss(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_set_city_occupation(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_update_legion_member_info_one(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_fire_bonfire_ret(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_add_fuel_to_bonfire_ret(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_add_legion_glory_glow(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_legion_rm_recruit_cd_ret(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_legion_bonfire_dead(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_add_legion_event(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_get_legion_contribution(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_add_legion_act_score(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_settle_legion_act_rank(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_add_bonus_count(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_get_bonus_award_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_operation_legion_warehouse_item(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_invite_player_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_apply_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_confirm_change_legion_name(uint32 conn_index, const void* data_ptr, size_t data_len);
	// best_record
	void cs2ws_req_set_best_record(uint32 conn_index, const void* data_ptr, size_t data_len);

	//arena
	void cs2ws_get_arena_rank(uint32 conn_index, const void* data_ptr, size_t data_len);
		 void lua_cs2ws_get_arena_rank(uint32 conn_index, const char* data_ptr, int32 data_len);
	void cs2ws_req_challenge(uint32 conn_index, const void* data_ptr, size_t data_len);
		void lua_cs2ws_req_challenge(uint32 conn_index, const char* data_ptr, int32 data_len);
	void cs2ws_req_get_choose_list(uint32 conn_index, const void* data_ptr, size_t data_len);
		void lua_cs2ws_req_get_choose_list(uint32 conn_index, const char* data_ptr, int32 data_len);
	void cs2ws_req_get_first_three(uint32 conn_index, const void* data_ptr, size_t data_len);
		void lua_cs2ws_req_get_first_three(uint32 conn_index, const char* data_ptr, int32 data_len);
	void cs2ws_chellenge_over_req(uint32 conn_index, const void* data_ptr, size_t data_len);
		void lua_cs2ws_chellenge_over_req(uint32 conn_index, const char* data_ptr, int32 data_len);
	void cs2ws_arena_sync_gs_value_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
		void lua_cs2ws_arena_sync_gs_value_proc(uint32 conn_index, const char* data_ptr, int32 data_len);

	//ranking
	void cs2ws_sync_ranking_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_worship_player_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_sync_ranking_player_vip_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_req_service_rank_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//mail
	void cs2ws_find_and_send_mail_to_player(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_mail_to_all_player_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_check_id_mail_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//event&chat&globel_message
	void cs2ws_check_player_globel_mail_process(uint32 conn_index, const void* data_ptr, size_t data_len);

	void cs2ws_send_globel_message_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_notice_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_chat_result(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_chat_to_ws_process_new(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_notice_with_param_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	void cs2ws_join_voice_channel(uint32 conn_index, const void* data_ptr, size_t data_len);

	void cs2ws_reconnect_send_red_package_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_red_package_process(uint32 conn_index, const void* data_ptr, size_t data_len);
		void lua_cs2ws_send_red_package_process(uint32 conn_index, const char* data_ptr, int32 data_len);
	void cs2ws_get_red_package_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	    void lua_cs2ws_get_red_package_process(uint32 conn_index, const char* data_ptr, int32 data_len);

	//world_boss
	//ws端的管理器从数据库读数据
	void dp2ws_load_world_boss_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//世界Boss死亡、刷新处理
	void cs2ws_world_boss_dead_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_create_world_boss_sucess_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_first_killer_get_prize(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_npc_left_hp_per_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//增加友好度
	void cs2ws_add_friendliness_value_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_fuben_add_friendliness_value_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//marry
	void cs2ws_check_item_enough_end(uint32 conn_index, const void *data_ptr, size_t data_len); 
	void cs2ws_ret_get_reward_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_update_wedding_ring_level_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_add_heart_value_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_cost_bless_money_result_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_send_lucky_info_to_client_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//big_player
	void cs2ws_create_big_player_sucess_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//lucky draw record
	void cs2ws_add_lucky_draw_record(uint32 conn_index, const void* data_ptr, size_t data_len);

	//auction
	void cs2ws_auction_sell_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_buy_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_puchase_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_cancel_sell_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void cs2ws_trade_bid_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_clear_selling_lock_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_bid_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_share_sell_reward_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_add_trade_record_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_del_bid_record_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_auction_request_person_refresh_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//recharge
	void cs2ws_recharge_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_recharge_end_send_mail_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//gm指令专用
	void cs2ws_gm_order_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//gain_treasure
	void cs2ws_update_gain_treasure_boss_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_add_gain_treasure_record_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//boss_island
	void cs2ws_update_boss_island_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	void cs2ws_update_boss_damage_list_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	//cloud_shop
	void cs2ws_cloud_shop_buy_proc_end(uint32 conn_index, const void *data_ptr, size_t data_len);

	//rename
	void cs2ws_sub_rename_card_end(uint32 conn_index, const void* data_ptr, size_t data_len);

	//cross_server_pk
	void cs2ws_update_cross_server_pk_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_update_cross_server_pk_map_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//cross_server_harry
	void cs2ws_update_server_change_money(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_create_harry_player(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_send_harry_notice_to_all_player_func(uint32 conn_index, const void *data_ptr, size_t data_len);

	//cross_ladder
	void cs2ws_cross_ladder_req_buy_ticket_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_cross_ladder_send_game_result_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_cross_ladder_req_last_score_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//cross_server_world_boss
	void cs2ws_cross_server_kill_world_boss_func(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_cross_server_world_boss_creat_boss_begin_func(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_make_cross_server_world_legion_award_func(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_cross_boss_map_game_over_func(uint32 conn_index, const void *data_ptr, size_t data_len);
	//time_limit_activity
	void cs2ws_time_limit_sync_common_data(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_check_time_limit_sync_common_data(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_send_kill_boss_notic_func(uint32 conn_index, const void *data_ptr, size_t data_len);

	void cs2ws_sync_person_information_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_delete_person_information_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void cs2ws_get_person_couple_proc(uint32 conn_index, const void * data_ptr, size_t data_len);

	void cs2ws_cross_player_save_end_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//legion_station
	void cs2ws_create_bonfire_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void cs2ws_bonfire_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void cs2ws_legion_boss_create_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void cs2ws_damage_legion_boss_player_arr_proc (uint32 conn_index, const void * data_ptr, size_t data_len);

	void cs2ws_check_can_assist_fight_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void cs2ws_cancel_assist_fight_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void cs2ws_start_assist_fight_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void cs2ws_end_assist_fight_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	void cs2ws_join_team_assist_fight(uint32 conn_index, const void* data_ptr, size_t data_len);

	void cs2ws_create_pk_king_combat_map_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_is_pk_king_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_break_line_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	void cs2ws_element_war_map_game_end_role_info_msg(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_element_war_element_war_buy_ticket_msg(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_element_ladder_req_last_score_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_element_war_gm_send_add_ticket_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void cs2ws_reload_csv_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	void cs2ws_send_attack_city_legion_reward_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void send_attack_city_legion_end_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void send_attack_city_rank_end_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_send_attack_city_legion_call_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	
}
#endif
