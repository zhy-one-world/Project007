/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   15:00
	file base:	msgproc_dp
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WS_MSGPROC_DP_H_
#define _WS_MSGPROC_DP_H_
#include "logic/type_def.hpp"
#include "Logic/world_def.hpp"
namespace faith
{
	void dp2ws_rep_get_game_info_ws_lua(uint32 conn_index, int32 server_id, int32 begin_time, int32 server_info_arr[e_server_info_type_max], int32 gm_commond_arr[e_need_server_cross_max - 1]);
	void dp2ws_rep_get_role_info_ws_lua(uint32 conn_index, int64 role_guid, xstring account,xstring role_name,xstring role_appearance,xstring role_first_kill_npc_id_arr,int64 map_guid
	,int64 data_i64_ary[e_role_i64_info_max],int32 data_ary[e_role_info_max],int32 ws_data_ary[e_role_ws_info_max],int32 is_login,int32 is_send_chat,
		int32 worship_value,int32 eresult);
	void dp2ws_rep_load_arena_rank_lua(uint32 conn_index,const char* data_ptr, size_t data_len, int32 start_index);
	void dp2ws_rep_load_all_ranking_info_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int32 ranking_type);
	void dp2ws_rep_load_one_ranking_info_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int64 sender_guid, int32 ranking_type, int32 worship_add_value
	, int32 worship_type, int32 end_result);
	void dp2ws_rep_load_one_worship_info_proc_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int64 sender, bool is_load);
	void dp2ws_rep_load_all_service_rank_info_lua(uint32 conn_index, const char* data_ptr, size_t data_len);
	void dp2ws_rep_load_last_server_list_lua(uint32 conn_index, int32 server_id, xstring last_server_list);
	void dp2ws_gm_set_result_proc_lua(uint32 conn_index, int32 listen_port, int64 handle_index, int32 result_code, xstring result_msg);
	void dp2ws_save_recharge_proc_lua(uint32 conn_index, xstring order_id, int64 role_guid, int32 game_goods_id, float pay_price, int32 success, int32 order_num, int32 payment_type
		, int32 direct_diamond, int64 client_uid, xstring order_token);
	void dp2ws_send_recharge_end_result_proc_lua(uint32 conn_index, int32 result_type, int64 order_num, int32 payment_type, int32 goods_id, int32 direct_diamond, int64 role_guid
		, xstring order_id);
	void dp2ws_gm_get_server_info_proc_lua(uint32 conn_index, int32 acc_num, int32 role_num);
	void dp2ws_gm_change_account_proc_lua(uint32 conn_index, int32 change_res, int32 listen_port, int64 handle_index);
	void dp2ws_gm_get_role_info_proc_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int32 listen_port, int64 handle_index, int32 check_type);
	void dp2ws_gm_modify_role_info_proc_lua(uint32 conn_index, int64 role_guid, int32 listen_port, int64 handle_index);

	void dp2ls_reply_client_login(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ls_reply_character_list(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ls_reply_create_character(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ls_reply_delete_character(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ls_load_enum_item_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ls_load_enum_buff_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ls_load_enum_spirit_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ls_load_enum_time_end(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_rep_get_game_info_ws(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_rep_get_role_info_ws(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_resend_mail_to_sender(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_rep_load_arena_rank(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_create_arena_robot(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_arena_finish(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_rep_load_last_server_list(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	//ranking
	void dp2ws_rep_load_all_ranking_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_rep_load_one_ranking_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_rep_load_one_worship_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);


	void dp2ws_load_mail_event_end_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_mail_event_item_end_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_check_id_mail_end_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_other_server_mail_record_end_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_other_server_mail_end_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_rep_load_all_service_rank_info(uint32 conn_index, const void* data_ptr, size_t data_len);

	// legion
	void dp2ws_load_all_legion_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_all_legion_member_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_all_legion_city_war_member_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_all_legion_applicant_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_all_legion_boss_award_get_log_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_city_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_all_city_bid_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_all_legion_event_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_legion_bonus_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_legion_warehouse_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_legion_warehouse_log_process(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_legion_cross_city_info_process(uint32 conn_index, const void* data_ptr, size_t data_len);

	// activity
	void dp2ws_load_all_best_record_process(uint32 conn_index, const void* data_ptr, size_t data_len);


	// big_player
	void dp2ws_load_big_player_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_load_big_player_base_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_big_player_skill_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_big_player_att_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_big_player_item_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_big_player_buff_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//gm
	void dp2ws_gm_set_result_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_save_recharge_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_send_recharge_end_result_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_gm_change_account_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	void dp2ws_gm_get_server_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_gm_get_role_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_gm_modify_role_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_gm_get_role_time_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_gm_get_role_money_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_gm_get_role_lock_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_gm_get_role_items_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//red_package
	void dp2ws_load_red_package_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_dp2ws_load_red_package_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len);
	void dp2ws_load_red_package_receiver_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void lua_dp2ws_load_red_package_receiver_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len);

	//lucky_draw_record
	void dp2ws_load_lucky_draw_record_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//relation
	void dp2ws_rep_load_relationlist_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_rep_load_relation_info_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_add_relation_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_rep_load_relation_info_other_server_end(uint32 conn_index, const void *data_ptr, size_t data_len);

	
	//auction
	void dp2ws_rep_auction_find_buy_target_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_rep_auction_cancel_sell_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_rep_auction_sell_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_rep_auction_find_bid_target_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_auction_clear_selling_lock_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_auction_clear_item_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_req_share_list_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_auction_check_legion_selling_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_check_world_auction_sell(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_auction_show_sell_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_auction_time_out_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void db2ws_auction_bid_db_sucess_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	//marry
	void dp2ws_rep_marry_couple_info_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_ret_marry_couple_last_login_time_end(uint32 conn_index, const void *data_ptr, size_t data_len);
	void dp2ws_load_marry_info_cross_server_end_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//server_time
	void dp2ws_load_server_attr_val_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_load_all_gain_treasure_record_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//cloud shop
	void dp2ws_rep_cloud_shop_role_record_end(uint32 conn_index, const void* data_ptr, size_t data_len);

	//rename
	void dp2ws_check_player_name_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_change_player_name_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//chat
	void dp2ws_load_chat_with_offline_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_load_all_cross_server_pk_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_load_all_cross_server_harry_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_load_harry_player_base_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_harry_player_skill_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_harry_player_att_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_harry_player_item_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_harry_player_buff_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_harry_player_belief_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_harry_player_fight_att_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//cross_ladder
	void dp2ws_cross_ladder_load_role_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	//Cross_boss
	void dp2ws_load_cross_server_world_boss_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_load_all_ws_time_limit_act_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void cs2ws_sync_oracle_trial_rank_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_time_limit_temp_end_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_time_limit_branch_temp_end_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2ws_save_appearance_info_to_id_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_load_appearance_info_by_id_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//元素争霸
	void dp2ws_element_war_load_role_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//攻城战
	void dp2ws_attack_city_load_legion_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_attack_city_load_rank_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
}

#endif
