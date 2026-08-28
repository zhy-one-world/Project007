#ifndef _WS_MSGPROC_CLIENT_H_
#define _WS_MSGPROC_CLIENT_H_

#include "logic/type_def.hpp"
#include "../server/client_session_mgr.hpp"

namespace hld
{
	class legion_proto_legion_call_operate;
	class goods_proto_goods_operate;
	class game_proto_get_best_record;
	class relation_proto_c2s_req_syc_nearly_state;
	class relation_proto_c2s_confirm_receive_marry_mission;
	class team_proto_invite_player;
	class team_proto_ready;
	class team_proto_search_team_by_map_id;
	class team_proto_get_team_list_by_aim;
	class team_proto_team_mem_operate;
	class legion_proto_get_legion_bonus_info;
	class legion_proto_send_stimulate_bonus;
	class legion_proto_join_cross_city_war;
	// legion

	void c2ws_req_create_legion(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_leave_legion(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_disslove_legion(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_legion_list(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_legion_list_logic(client_session* session, int32 start_index, int64 base_fighting_power, int32 only_get_auto_accpet_new_member_legion);
		
	void c2ws_req_apply_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len); // cs
	void c2ws_req_get_legion_applicant_list(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_process_legion_apply(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_legion_construction_level_up(uint32 conn_index, const void* data_ptr, size_t data_len); 
	void c2ws_req_get_construction_buff(uint32 conn_index, const void* data_ptr, size_t data_len); //×¢ÊÍµôÁË
	void c2ws_req_legion_donate_money(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_legion_donate_money_logic(client_session* session, int32 donate_times);

	void c2ws_req_legion_donate_item(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_legion_donate_item_logic(client_session* session, int32 donate_item_index, int32	donate_times);
	void c2ws_req_update_legion_announcement(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_event_list(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_event_list_logic(client_session* session, int32 from_happen_time);
	void c2ws_req_kick_out_legion_member(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_appoint_legion_member_job_title(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_enter_legion_boss_map(uint32 conn_index, const void* data_ptr, size_t data_len); // ×¢ÊÍ·ÏÆú
	void c2ws_req_get_legion_boss_award(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_legion_boss_award_logic(client_session* session);
	void c2ws_req_get_banquet_info(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_banquet_info_logic(client_session* session);
	void c2ws_req_request_hold_banquet(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_request_hold_banquet_logic(client_session* session, int32	banquet_level);
	void c2ws_req_take_part_in_banquet(uint32 conn_index, const void* data_ptr, size_t data_len);

	void c2ws_req_get_city_war_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_apply_city_war_bid(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_enter_city_war_map(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_enter_city_war_map_logic(client_session* session, int32	in_terr_id);
	void c2ws_req_city_war_operate(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_city_war_operate_logic(client_session* session, int32	in_operate_type, int64 in_prop_npc_guid, int32	in_terr_id);
	void c2ws_req_set_auto_accept_new_legion_member(uint32 conn_index, const void* data_ptr, size_t data_len);

	void c2ws_req_get_city_master_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_occupation_daily_award(uint32 conn_index, const void* data_ptr, size_t data_len); // Ö±½Ó·µ»Ø£¬·ÏÆú
	void c2ws_req_get_legion_rank(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_fire_bonfire(uint32 conn_index, const void* data_ptr, size_t data_len);   // ×¢ÊÍ·ÏÆú
	void c2ws_req_add_fuel_to_fire(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_add_fuel_to_fire_logic(client_session* session);

	void c2ws_req_send_legion_recruit_in_world(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_send_legion_recruit_in_world_logic(client_session* session, bool	in_is_rm_cd);
	void c2ws_req_get_city_war_terr_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_city_war_legion_rank_info(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_city_war_legion_rank_info_logic(client_session* session);
	void c2ws_req_set_territory_prior_maintain(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_invite_player_to_join_legion(uint32 conn_index, const void* data_ptr, size_t data_len); //cs

	void c2ws_req_draw_bonfire_daily_reward(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_draw_bonfire_daily_reward_logic(client_session * session);
	void c2ws_req_legion_call_operate(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_legion_call_operate_logic(client_session * session, legion_proto_legion_call_operate& req);
		void c2ws_req_legion_call_operate_lua(client_session * session, const char* msg, int32 msg_len);
	void c2ws_req_level_up_legion_skill(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_transfer_to_bonfire_map(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_bonus_rewards(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_bonus_rewards_logic(client_session * session, legion_proto_get_legion_bonus_info& req);
		void c2ws_req_get_bonus_rewards_lua(client_session * session, const char* msg, int32 msg_len);

	void c2ws_req_stimulate_bonus(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_stimulate_bonus_logic(client_session * session, legion_proto_send_stimulate_bonus& req);
		void c2ws_req_stimulate_bonus_lua(client_session * session, const char* msg, int32 msg_len);
	void c2ws_req_get_legion_question_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_legion_warehouse_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_legion_warehouse_log(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_change_legion_name(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_confirm_change_legion_name(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_join_cross_city_war(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_join_cross_city_war_logic(client_session * session, legion_proto_join_cross_city_war& req);
		void c2ws_req_join_cross_city_war_lua(client_session * session, const char* msg, int32 msg_len);
	void get_legion_answer_rank_list(uint32 conn_index, const void* data_ptr, size_t data_len);

	//store
	void c2ws_req_goods_operate(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_goods_operate_logic(client_session * session, goods_proto_goods_operate& req);
		void c2ws_req_goods_operate_lua(client_session * session, const char* msg, int32 msg_len);

	// team
	void c2ws_req_let_role_join_team(uint32 conn_index, const void* data_ptr, size_t data_len);   // Ïà¹ØÂß¼­Ã»ÓÐ
	void c2ws_req_leave_team(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_kickout_team_member(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_set_team_captain(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_invite_player_join_team(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_invite_player_join_team_logic(client_session * session, team_proto_invite_player& req);
		void c2ws_req_invite_player_join_team_lua(client_session * session, const char* msg, int32 msg_len);
	void c2ws_req_transfer_team_to_scene(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_ready(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_ready_logic(client_session * session, team_proto_ready& req);
		void c2ws_req_ready_lua(client_session * session, const char* msg, int32 msg_len);
	void c2ws_req_ready_transfer(uint32 conn_index, const void* data_ptr, size_t data_len);  // ×¢ÊÍ·ÏÆú
	void c2ws_req_search_team_list(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_search_team_list_logic(client_session * session, team_proto_search_team_by_map_id& req);
		void c2ws_req_search_team_list_lua(client_session * session, const char* msg, int32 msg_len);

	void c2ws_req_get_no_team_legionmate_list(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_apply_to_join_team(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_team_list_by_aim(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_team_list_by_aim_logic(client_session * session, team_proto_get_team_list_by_aim& req);
		void c2ws_req_get_team_list_by_aim_lua(client_session * session, const char* msg, int32 msg_len);
	void c2ws_req_auto_match_team(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_update_team_attribute(uint32 conn_index, const void* data_ptr, size_t data_len); // ÏûÏ¢×¢²á·ÏÆú
	void c2ws_req_team_mem_operate(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_team_mem_operate_logic(client_session * session, team_proto_team_mem_operate& req);
		void c2ws_req_team_mem_operate_lua(client_session * session, const char* msg, int32 msg_len);
	void c2ws_refused_join_team(uint32 conn_index, const void* data_ptr, size_t data_len);

	// activity
	void c2ws_req_get_best_record(uint32 conn_index, const void* data_ptr, size_t data_len);
		void c2ws_req_get_best_record_logic(client_session * session, game_proto_get_best_record& req);
		void c2ws_req_get_best_record_lua(client_session * session, const char* msg, int32 msg_len);

	// map
	void c2ws_req_transfer_to_pk_king_map(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_transfer_to_shui_jing_map(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_transfer_to_broken_sky_map(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_one_map_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_transfer_to_cross_pk_map(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_get_world_boss_damage_list(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_transfer_to_harry_map(uint32 conn_index, const void* data_ptr, size_t data_len);
	//activatecode
	void c2ws_get_welfare_activatecode(uint32 conn_index, const void* data_ptr, size_t data_len);

	//relation
	void c2ws_add_friend(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_del_friend(uint32 conn_index, const void *data_ptr, size_t data_len);

	void c2ws_find_player_to_add_friend(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_req_syc_nearly_state(uint32 conn_index, const void *data_ptr, size_t data_len);
		void c2ws_req_syc_nearly_state_logic(client_session * session, relation_proto_c2s_req_syc_nearly_state& req);
		void c2ws_req_syc_nearly_state_lua(client_session * session, const char* msg, int32 msg_len);
	void c2ws_req_operate_other_req(uint32 conn_index, const void *data_ptr, size_t data_len);
		void c2ws_req_operate_other_req_logic(client_session * session, relation_proto_c2s_operate_other_add_req& req);
		void c2ws_req_operate_other_req_lua(client_session * session, const char* msg, int32 msg_len);
	//marry
	void c2ws_req_marry_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_reply_marry_choice_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_get_marry_couple_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len); // ·ÏÆú
	void c2ws_req_divorce_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_req_get_marry_reward_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2s_req_get_marry_task(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2s_confirm_receive_marry_task(uint32 conn_index, const void *data_ptr, size_t data_len);
		void c2s_confirm_receive_marry_task_logic(client_session * session, relation_proto_c2s_confirm_receive_marry_mission& req);
		void c2s_confirm_receive_marry_task_lua(client_session * session, const char* msg, int32 msg_len);
	void c2s_req_bless_one_couple_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//rangking
	void c2ws_req_get_rank_list_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_player_rank_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_get_player_ranking_value_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_service_rank_get_first_player_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//worldboss
	void c2ws_req_frist_kill_world_boss_prize(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_get_world_boss_level(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_get_strategy_world_boss_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_get_cur_world_boss_info(uint32 conn_index, const void* data_ptr, size_t data_len);


	//gain_treasure
	void c2ws_gain_treasure_req_kill_record_proc(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_gain_treasure_req_gain_item_record_proc(uint32 conn_index, const void *data_ptr, size_t data_len);

	//chat
	void c2ws_join_voice_channel_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	//cloud shop
	void c2ws_req_cloud_shop_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	//time limit activity
	void c2ws_time_limit_get_my_rank(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_time_limit_get_common_data(uint32 conn_index, const void *data_ptr, size_t data_len);

	//rename
	void c2ws_req_change_player_name(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_req_confirm_change_name(uint32 conn_index, const void *data_ptr, size_t data_len);

	//cross_server_world_boss
	void c2ws_req_transfer_to_cross_server_world_boss_map(uint32 conn_index, const void *data_ptr, size_t data_len);
	void c2ws_get_cross_boss_show_info(uint32 conn_index, const void *data_ptr, size_t data_len);


	void c2ws_get_oracle_trial_rank(uint32 conn_index, const void *data_ptr, size_t data_len);

	//legion_station
	void c2s_join_legion_station_proc(uint32 conn_index, const void * data_ptr, size_t data_len);
	
	//assist_fight
	void c2s_get_assist_fight_info_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_req_assist_fight_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_cancel_assist_fight_proc(uint32 connindex, const void * data_ptr, size_t data_len); // ÏûÏ¢ÔÚCS

	void c2s_save_appearance_info_to_id_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_load_appearance_info_by_id_proc(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_send_need_break_line_login_end_proc(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2ws_special_server_msg(uint32 conn_index, const void* data_ptr, size_t data_len);

	void c2ws_element_war_send_sign_up_msg(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_element_war_send_sign_up_team_ready_msg(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_special_element_war_send_close_sign_up_msg(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_element_war_get_mission_reward_msg(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_element_war_get_role_info_msg(uint32 conn_index, const void* data_ptr, size_t data_len);

	// attack_city
	void c2ws_transfer_attack_city_map(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_get_attack_city_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_get_attack_city_rank_list(uint32 conn_index, const void* data_ptr, size_t data_len);
}


#endif