/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 11:46
  file base: msgproc_dp
  file ext: hpp
  author: lidongyang
  
  purpose: 
 *  加载顺序 loading
 *	1. 基本属性
 *	2. 物品信息
 
*********************************************************************/
#ifndef _MESSAGE_PROCESS_DP_HPP_
#define _MESSAGE_PROCESS_DP_HPP_

#include "logic/type_def.hpp"


namespace faith
{
	bool dp2cs_load_role_info_lua(int32 unit_array_index,const char *data_ptr, int32 data_len);
	void dp2cs_rep_load_char_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_data(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_buff(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_skill(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_legion_skill(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_item(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_money(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_talent(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_treasure(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_pokedex(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_tinder(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_spirit(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_mission(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_att(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_goods(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_worship_target(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_map_record(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_demons_tower(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_element_heart(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_special_name(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_achievement(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_mail(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_mail_item(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_del_char_mail_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_operate_load_lock_mail_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_convert(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	void dp2cs_rep_load_char_had_globel_mail_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_show_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_yester_must_do(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_guide_record(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_first_time_do(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_daily_must_do_count(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_rep_load_char_data_error(uint32 conn_index, const void* data_ptr, size_t data_len);		// 错误信息
	void dp2cs_rep_save_char_data_complete(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_req_load_gain_treasure_info(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_req_load_boss_island_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_req_load_role_harry_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_phantom(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_oracle_trial_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_chat_record(uint32 conn_index, const void* data_ptr, size_t data_len);
	// recycle
	void dp2cs_rep_load_char_recycle(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_recycle_task(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_recycle_invited(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_recycle_get_inviter_info_end(uint32 conn_index, const void* data_ptr, size_t data_len);
	
	
	//arena
	void dp2cs_rep_load_arena_log(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_arena_data_error(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_rep_load_arena_char_error(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_att(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_skill(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_item(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_fight_att(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_buff(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_spirit(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_arena_char_belief(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_req_load_service_rank_reward_state(uint32 conn_index, const void* data_ptr, size_t data_len);

	// welfare
	void dp2cs_rep_load_char_welfare(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_active_degree(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_char_welfare_random_get(uint32 conn_index, const void* data_ptr, size_t data_len);
	//title
	void dp2cs_rep_load_char_title(uint32 conn_index, const void* data_ptr, size_t data_len);

	//find other player info
	void dp2cs_rep_load_other_player_info_base(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_equiping(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_special_name(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_base_group(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_spirit(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_wing_or_mount(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_buff(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_feather(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_rep_load_other_player_info_appearance(uint32 conn_index, const void* data_ptr, size_t data_len);

	//relation
	//void dp2cs_rep_load_relationlist_end(uint32 conn_index, const void* data_ptr, size_t data_len);

	//hightest record
	void dp2cs_load_hightest_record_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//auction
	void dp2cs_auction_load_self_selling_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_auction_get_auction_list_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_auction_get_trade_record_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_auction_get_another_sell_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_auction_find_bid_target_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_req_bid_record_failure_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_auction_req_bid_notice_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_auction_req_bid_notice_red_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	// service
	void dp2cs_req_load_char_service_goal_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_req_load_char_seven_day_goal_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	//time limit activity
	void dp2cs_time_limit_activity_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_connect_success_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	//person_information
	void dp2cs_get_person_information_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_get_other_person_information_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_get_role_competition_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_get_role_dragontrip_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_get_role_skytreasure_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2cs_get_role_starark_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_get_time_feed_back_to_db_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_get_time_limit_gift_to_db_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void dp2cs_get_subscribe_daily_info_to_db_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	
}
#endif
