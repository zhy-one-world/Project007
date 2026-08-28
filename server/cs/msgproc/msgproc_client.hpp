/********************************************************************
  created: 2015年12月28日16:10:39
  file base: msgproc_client
  file ext: hpp
  author: zhy
  
  purpose: 客户端到服务器的请求处理
*********************************************************************/
#ifndef _FEP_MSGPROC_CLIENT_H_
#define _FEP_MSGPROC_CLIENT_H_

#include "logic/type_def.hpp"

namespace hld
{
	class mail_proto_mail_operation;
	class chat_proto_notice_info_with_param;
	class chat_proto_chat_content;
	class assist_fight_proto_create_assist_fight_npc;
	class mount_power_proto_mount_power_operate;
	class buff_proto_buff_operation;
	class aoi_proto_unit_location;
	class aoi_proto_unit_rotation;
	class aoi_proto_simple_unit_pos;
	class game_proto_cross_server_pk_map_transfer;
	class game_proto_transfer_scene;
	class game_proto_transfer_scene_pos;
	class game_proto_transfer_scene_born;
	class skill_proto_skill_operation;
	class game_proto_get_map_boss_is_dead;
	class game_proto_get_boss_god_time;
	class game_proto_element_war_buy_ticket;
	class item_proto_item_quick_call_mount;
	class character_proto_unit_info_body_att;
	class character_proto_ask_unit_dead;
	class character_proto_character_gmorder_message;
	class character_proto_add_guide_record;
	class player;

	void req_fep2cs_in_game(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_ping(uint32 connindex, const void *data_ptr, size_t data_len);

	//npc
	void c2cs_req_temple_gift_num(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_req_map_tigger_volume(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_jump_raid_animation(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_req_interact_with_npc(uint32 connindex, const void *data_ptr, size_t data_len);

	//player
	void c2cs_req_aoi_location(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_req_aoi_location_logic(player& player_ref, aoi_proto_unit_location& req);
	void c2cs_req_aoi_location_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2cs_req_aoi_rotation(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_req_aoi_rotation_logic(player& player_ref, aoi_proto_unit_rotation& req);
	void c2cs_req_aoi_rotation_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2cs_req_aoi_show_player(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_req_aoi_simple_pos(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_req_aoi_simple_pos_logic(player& player_ref, aoi_proto_simple_unit_pos& req);
	void c2cs_req_aoi_simple_pos_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2cs_back_to_self_server(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_back_to_self_server_logic(player& player_ref, game_proto_cross_server_pk_map_transfer& req);
	void c2cs_back_to_self_server_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2cs_req_transfer_scene(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_req_transfer_scene_logic(player& player_ref, game_proto_transfer_scene& req);
	void c2cs_req_transfer_scene_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2cs_req_transfer_scene_pos(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_req_transfer_scene_pos_logic(player& player_ref, game_proto_transfer_scene_pos& req);
	void c2cs_req_transfer_scene_pos_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2cs_req_transfer_scene_born(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_req_transfer_scene_born_logic(player& player_ref, game_proto_transfer_scene_born& req);
	void c2cs_req_transfer_scene_born_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2cs_lucky_draw_req(uint32 connindex, const void*data_ptr, size_t data_len);
	void c2cs_unlock_bag_slot(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_unit_fuhuo(uint32 connindex, const void *data_ptr, size_t data_len);

	void c2cs_mopping_up(uint32 connindex, const void *data_ptr, size_t data_len);  // lua dungeon
	void c2cs_mopping_up_demons_tower(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_mopping_up_demons_tower_get_award(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_all_mopping_up(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_spirit_fetter_upgrade(uint32 connindex, const void *data_ptr, size_t data_len);

	
	void c2cs_get_func_unlock_award(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_leave_map(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_req_hit_and_get_npc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_req_att_game_change(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_grade_level_ani_finish_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_req_base_body_att(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_req_base_body_att_logic(player& player_ref, character_proto_unit_info_body_att& req);
	void c2cs_req_base_body_att_lua(player& player_ref, const char* msg, int32 msg_len);

	void c2cs_add_damage_buff_in_exp_raid(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_buy_raid_count(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_change_role_class(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_get_first_raid_award(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_auto_use_hp_pot(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_ask_unit_dead(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_ask_unit_dead_logic(player& player_ref, character_proto_ask_unit_dead& req);
	void c2cs_ask_unit_dead_lua(player& player_ref, const char* msg, int32 msg_len);

	// team
	void c2cs_create_team(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_join_team(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_change_team_type(uint32 connindex, const void *data_ptr, size_t data_len);

	//item
	void item_operate_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void item_element_operate_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void item_composit_operate_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void show_fashion(uint32 connindex, const void *data_ptr, size_t data_len);
	void star_skill_unlock(uint32 connindex, const void *data_ptr, size_t data_len);
	void item_set_quick_call_mount_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void item_set_quick_call_mount_proc_logic(player& player_ref, item_proto_item_quick_call_mount& req);
	void item_set_quick_call_mount_proc_lua(player& player_ref, const char* msg, int32 msg_len);
	void item_get_hope_item(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_item_upgrade(uint32 connindex, const void *data_ptr, size_t data_len);


	void req_change_sky_suit_state(uint32 connindex, const void *data_ptr, size_t data_len);

	//skill
	void skill_req_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void skill_req_message_logic(player& player_ref, skill_proto_skill_operation& req);
	void skill_req_message_lua(player& player_ref, const char* msg, int32 msg_len);

	void skill_spell_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void skill_cast_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void skill_hurt_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void skill_use_message(uint32 connindex, const void* data_ptr, size_t data_len);
	void passive_skill_up_leve(uint32 connindex, const void* data_ptr, size_t data_len);
	void c2cs_passive_skill_equip(uint32 connindex, const void *data_ptr, size_t data_len);

	void buff_req_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void buff_req_message_logic(player& player_ref, buff_proto_buff_operation& req);
	void buff_req_message_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2s_player_gmorder_message(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_player_pick_drop_box(uint32 connindex, const void *data_ptr, size_t data_len);
	//阵营和PK
	void c2cs_change_pk_mode(uint32 connindex, const void *data_ptr, size_t data_len);

	//冥想
	void c2cs_start_meditation(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_stop_meditation(uint32 connindex, const void *data_ptr, size_t data_len);
	//天赋
	void c2cs_talent_level_up(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_talent_reset(uint32 connindex, const void *data_ptr, size_t data_len);

	// legion
	void c2cs_convenient_join_legion(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_draw_mem_daily_award(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_territory_buff_operate(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_goto_next_stage_of_bonfire_map(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_legion_warehouse_operation(uint32 connindex, const void *data_ptr, size_t data_len);  //屏蔽
	void c2cs_invite_player_to_join_legion(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_req_apply_to_join_legion(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_add_damage_buff_in_legion_boss(uint32 connindex, const void *data_ptr, size_t data_len);

	//treasure
	void c2cs_req_treasure_notice(uint32 connindex, const void *data_ptr, size_t data_len);

	//guide
	void c2s_add_guide_record(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_add_guide_record_logic(player& player_ref, character_proto_add_guide_record& req);
	void c2s_add_guide_record_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2s_guide_trigger_start(uint32 connindex, const void *data_ptr, size_t data_len);

	//Arena
	void req_get_arena_rank(uint32 connidex, const void *data_ptr, size_t data_len);
	void req_get_match_player(uint32 connidex, const void *data_ptr, size_t data_len);
	void req_get_first_three_player(uint32 connidex, const void *data_ptr, size_t data_len);
	void req_do_challenge(uint32 connidex, const void *data_ptr, size_t data_len);
	void get_challenge_log(uint32 connidex, const void *data_ptr, size_t data_len);
	void get_arena_reward(uint32 connidex, const void *data_ptr, size_t data_len);
	void mopping_arena_remain_times(uint32 connidex, const void *data_ptr, size_t data_len);  // 屏蔽


	//排行榜
	void c2cs_worship_top_player_proc(uint32 connidex, const void *data_ptr, size_t data_len);
	void c2cs_service_rank_receice_reward_proc(uint32 connidex, const void *data_ptr, size_t data_len);

	//邮件
	void c2s_operate_mail(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_operate_mail_logic(player& player_ref, mail_proto_mail_operation& req);
	void c2s_operate_mail_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2s_send_mail(uint32 connindex, const void *data_ptr, size_t data_len);           // 未使用
	void c2s_send_mail_to_all_player(uint32 connindex, const void *data_ptr, size_t data_len); //未使用

	//提示信息和聊天
	void c2s_send_notice(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_send_notice_with_param(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_send_notice_with_param_logic(player& player_ref, chat_proto_notice_info_with_param& req);
	void c2s_send_notice_with_param_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2s_send_chat_new(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_send_chat_new_logic(player& player_ref, chat_proto_chat_content& req);
	void c2s_send_chat_new_lua(player& player_ref, const char* msg, int32 msg_len);

	void c2s_send_audio(uint32 connindex, const void *data_ptr, size_t data_len); // 未调用

	//资源找回
	void c2s_find_back_must_do_resource(uint32 connindex, const void *data_ptr, size_t data_len);

	//vip
	void c2s_recharge(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_recharge_widget_type(uint32 connindex, const void *data_ptr, size_t data_len);

	//新首充奖励
	void c2s_get_first_recharge_time(uint32 connindex, const void *data_ptr, size_t data_len);

	//稀有名称
	void c2s_title_set_title_no_need_see(uint32 connindex, const void *data_ptr, size_t data_len);

	//big player worship
	void c2s_worship_by_type(uint32 connindex, const void *data_ptr, size_t data_len);

	//查看他人装备
	void c2s_find_other_unit(uint32 connidex, const void *data_ptr, size_t data_len);

	//红包
	void c2s_send_red_package(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_red_package(uint32 connindex, const void *data_ptr, size_t data_len);

	//传送buff
	void c2s_transfer_buff(uint32 connindex, const void *data_ptr, size_t data_len);



	// first time do
	void c2cs_add_first_time_do_record(uint32 connindex, const void *data_ptr, size_t data_len);

	//auction
	void c2cs_auction_operate_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_auction_req_show_list(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_auction_req_trade_record(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_auction_req_another_sell_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_auction_req_self_bid_list(uint32 connindex, const void *data_ptr, size_t data_len);

	//achievement
	void c2s_achievement_finish(uint32 connindex, const void *data_ptr, size_t data_len);

	void c2s_achievement_all_finish(uint32 connindex, const void *data_ptr, size_t data_len);//领取所有可领的成就
	//send gift
	void c2cs_add_friendliness_value(uint32 connindex, const void *data_ptr, size_t data_len);

	//phantom
	void c2cs_phantom_oper(int32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_chat_record_oper(int32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_recycle_invited_get_reward(int32 connindex, const void *data_ptr, size_t data_len);

	//servicegoal
	void c2s_service_goal_operate(uint32 connindex, const void *data_ptr, size_t data_len);

	//interaction
	void c2s_interaction_invite(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_interaction_invite_reply(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_interaction_invite_start(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_stop_interaction(uint32 connindex, const void *data_ptr, size_t data_len);

	//time limit activity
	void c2cs_time_limit_get_act_reward(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2cs_time_limit_add_schedule(uint32 connindex, const void *data_ptr, size_t data_len);

	void c2s_first_look_legion(uint32 connindex, const void *data_ptr, size_t data_len);

	void c2s_give_server_item_to_npc_func(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_buy_time_limit_item(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_buy_diamond_shop(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_star_trip_operate(uint32 connindex, const void *data_ptr, size_t data_len);

	//cross_ladder
	void c2s_cross_ladder_req_buy_ticket(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_cross_ladder_req_mission_reward(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_cross_ladder_return_to_source_zone_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	//crossworldboss
	void c2s_cross_boss_get_boss_is_dead(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_cross_boss_get_boss_is_dead_logic(player& player_ref, game_proto_get_map_boss_is_dead& req);
	void c2s_cross_boss_get_boss_is_dead_lua(player& player_ref, const char* msg, int32 msg_len);
	void c2s_get_boss_god_time(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_boss_god_time_logic(player& player_ref, game_proto_get_boss_god_time& req);
	void c2s_get_boss_god_time_lua(player& player_ref, const char* msg, int32 msg_len);

	//weddingring
	void c2s_req_upgrade_wedding_ring_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_marry_examination(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_confirm_marry_examination(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_operate_marry_examination(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_req_use_fireworks_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_req_lucky_info_proc(uint32 connindex, const void *data_ptr, size_t data_len);


	void c2s_send_oracle_trial_commpingup_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_oracle_trial_customs_award(uint32 connindex, const void *data_ptr, size_t data_len);

	//person_information

	void c2s_get_person_information_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_other_person_information_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_person_infor_reward_proc(uint32 connindex, const void *data_ptr, size_t data_len);//已翻译

	void c2s_get_role_competition_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_competition_lv_reward_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_competition_lv_reward_all_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_buy_competition_lv_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_buy_speical_manual_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	void c2s_get_element_competition_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_element_competition_lv_reward_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_get_element_competition_lv_reward_all_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_buy_element_competition_lv_proc(uint32 connindex, const void *data_ptr, size_t data_len);
	void c2s_buy_speical_element_manual_proc(uint32 connindex, const void *data_ptr, size_t data_len);


	//legionDance
	void c2s_get_map_legion_player_proc(uint32 connindex, const void *data_ptr, size_t data_len);

	//assist_fight
	void c2s_start_assist_fight_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	//void c2s_req_assist_fight_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_cancel_assist_fight_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_create_assist_fight_npc_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	//check_in
	void c2s_get_cumulative_sign_in_reward(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_retroactive_all_days(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_buy_simage_boss_num(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_save_appearance_info(uint32 connindex, const void * data_ptr, size_t data_len);

	//vip
	void c2s_is_show_vip(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_dragontrip_oper(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_mountpower_oper(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_jewel_carve_oper(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_element_war_buy_ticket(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_add_use_power_up(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_skytreasure_operation(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_starark_oper(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_lucky_card_operate(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_change_enchant_show_type(uint32 connindex, const void * data_ptr, size_t data_len);

	////////////////////////////////////////////////////////////////////////////////////////
	void c2s_attack_city_operate_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_get_attack_city_map_rank_list_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_transfer_to_bron_pos_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_attack_legion_call_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	void c2s_end_attack_transfer_info_proc(uint32 connindex, const void * data_ptr, size_t data_len);
	
	void c2s_time_feed_back_operation_proc(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_time_limit_gift_operation_proc(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_send_subscribe_daily(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_fuben_vip_mopping(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_fuben_kill_boss(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_get_offline_award(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_get_online_award(uint32 connindex, const void * data_ptr, size_t data_len);

	void c2s_upgrade_search_level(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_get_npc_award(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_kill_award_npc(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_open_treasure_pic(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_get_treasuer_award(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_map_anim_end(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_refrush_award_npc(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_get_next_award_npc(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_flip_box_end(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_mop_up_map(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_finger_guess_end(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_smashing_bottle_end(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_wheel_plate_begin(uint32 connindex, const void* data_ptr, size_t data_len);

	void c2s_wheel_plate_end(uint32 connindex, const void* data_ptr, size_t data_len);
	
}


#endif
