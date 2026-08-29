/********************************************************************
	created:	2014/07/31
	created:	31:7:2014   15:20
	file base:	msg_dispatch
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "cell_server.hpp"
#include "connection_mgr.hpp"
#include "internal/achievement_msg.hpp"
#include "internal/arena_msg.hpp"
#include "internal/attack_city_msg.hpp"
#include "internal/auction_msg.hpp"
#include "internal/big_player_msg.hpp"
#include "internal/boss_island_msg.hpp"
#include "internal/buff_msg.hpp"
#include "internal/char_msg.hpp"
#include "internal/chat_msg.hpp"
#include "internal/cloud_shop_msg.hpp"
#include "internal/convert_msg.hpp"
#include "internal/core.hpp"
#include "internal/cross_ladder_msg.hpp"
#include "internal/cross_server_harry_msg.hpp"
#include "internal/cross_server_world_boss_msg.hpp"
#include "internal/element_heart_msg.hpp"
#include "internal/element_war_msg.hpp"
#include "internal/event_msg.hpp"
#include "internal/first_time_do_msg.hpp"
#include "internal/gain_treasure_msg.hpp"
#include "internal/gate_msg.hpp"
#include "internal/goods_msg.hpp"
#include "internal/guide_msg.hpp"
#include "internal/internal.hpp"
#include "internal/item_msg.hpp"
#include "internal/legion_station_msg.hpp"
#include "internal/login_msg.hpp"
#include "internal/mail_msg.hpp"
#include "internal/map_msg.hpp"
#include "internal/marry_msg.hpp"
#include "internal/mission_msg.hpp"
#include "internal/oracle_trial_msg.hpp"
#include "internal/phantom_msg.hpp"
#include "internal/pk_king_msg.hpp"
#include "internal/pokedex_msg.hpp"
#include "internal/ranking_msg.hpp"
#include "internal/recycle_msg.h"
#include "internal/service_goal_msg.hpp"
#include "internal/service_rank_msg.hpp"
#include "internal/seven_day_goal_msg.hpp"
#include "internal/skill_msg.hpp"
#include "internal/special_name_msg.hpp"
#include "internal/spirit_msg.hpp"
#include "internal/talent_msg.hpp"
#include "internal/time_limit_activity_msg.hpp"
#include "internal/title_msg.hpp"
#include "internal/treasure_msg.hpp"
#include "internal/welfare_msg.hpp"
#include "internal/world_boss_msg.hpp"
#include "internet/net.pb.h"
#include "msg_dispatch.hpp"
#include "msgproc/msgproc_client.hpp"
#include "msgproc/msgproc_dp.hpp"
#include "msgproc/msgproc_ws.hpp"
#include "net/message_manager.hpp"


namespace faith
{
	msg_dispatch::msg_dispatch() 
	{
	}

	msg_dispatch::~msg_dispatch() 
	{
	}

	bool msg_dispatch::res_load()
	{
		return true;
	}

	bool msg_dispatch::res_unload()
	{
		return true;
	}

	bool msg_dispatch::register_lua_handler()
	{
		return true;
	}

	bool msg_dispatch::unregister_lua_handler()
	{
		return true;
	}

bool msg_dispatch::init()
{
	message_manager::getInstance().register_handler(e_msg_index_rep_login, boost::bind(&connection_mgr::internal_rep_login, &connection_mgr::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_index_req_login, boost::bind(&cell_server::on_req_login, &cell_server::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_index_req_stop, boost::bind(&cell_server::on_req_stop, &cell_server::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_server2gate_send_to_server, boost::bind(&connection_mgr::internal_gate_msg, &connection_mgr::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_gate2gate_new, boost::bind(&connection_mgr::gate2ws_rep_gate_new, &connection_mgr::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_index_reload_csv, ws2cs_req_reload_csv);

	message_manager::getInstance().register_handler(e_msgindex_fep2cs_in_game, req_fep2cs_in_game);
	//message_manager::getInstance().register_handler(e_msgindex_c2cs_ping, c2cs_ping);
	message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_del_cache, ws2cs_req_del_unit);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_del_unit_item, ws2cs_req_del_unit_item);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_change_unit_vip, ws2cs_req_change_unit_vip);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_del_unit_specified_item, ws2cs_req_del_unit_specified_item);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_add_guide_record, ws2cs_req_add_guide_record);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_enter_game, ws2cs_req_enter_game);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_client_logout, ws2cs_req_logout_client);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_time_out, ws2cs_req_time_out);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_reconnect_game, ws2cs_req_reconnect_game);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_enter_scene, ws2cs_req_enter_scene);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_server_config, ws2cs_req_server_config);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_clear_server_config, ws2cs_req_clear_server_config);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_map_create, ws2cs_map_create_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_map_delete, ws2cs_map_delete_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_transfer_to_map, ws2cs_transfer_to_map_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_broken_sky_creat_boss, ws2cs_broken_sky_sync_creat_boss_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_broken_sky_info_sync, ws2cs_broken_sky_sync_game_info_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_broken_sky_kill_boss, ws2cs_broken_sky_sync_kill_boss_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_chest_arrivai_spawn_chest_wave, ws2cs_chest_arrival_spawn_chests_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_clear_pk_king_title, ws2cs_clear_big_player_title_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_apply_to_join_team, ws2cs_apply_to_join_team_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_set_player_team_info, ws2cs_set_player_team_info_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_clear_player_team_info, ws2cs_clear_player_team_info_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_join_team, ws2cs_join_team_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_transfer_team_member_to_scene, ws2cs_transfer_team_member_to_scene_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_team_common_check_in_cs, ws2cs_team_common_check_in_cs_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_sync_team_start_aim, ws2cs_sync_team_start_aim_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_set_player_legion_info, ws2cs_set_player_legion_guid_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_clear_player_legion_info, ws2cs_clear_player_legion_info_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_create_legion, ws2cs_create_legion_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_donate_legion_money, ws2cs_legion_donate_money_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_donate_legion_item, ws2cs_legion_donate_item_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_enter_legion_boss_map, ws2cs_enter_legion_boss_map_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_get_legion_boss_award, ws2cs_get_legion_boss_award_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_level_up_legion_skill, ws2cs_level_up_legion_skill_func);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_legion_warehouse_operation, ws2cs_legion_warehouse_operation);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_get_map_info_from_cs, ws2cs_get_map_info_from_cs_func);
	message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_open_time, ws2cs_gm_open_time);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_need_cross, ws2cs_gm_begin_cross);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_is_need_refresh_cs_daily_info_at_zero_hour, ws2cs_server_refresh_daily_info_at_zero_hour);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_server_Info_arr, ws2cs_server_info_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_server_info_all, ws2cs_server_info_all_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_begin_city_war, ws2cs_begin_city_war_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_enter_city_war_map, ws2cs_enter_city_war_map_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_city_war_operate, ws2cs_city_war_operate_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_set_get_legion_boss_award_log, ws2cs_set_get_legion_boss_award_log_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_get_construction_buff, ws2cs_get_construction_buff_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_get_occupation_daily_award, ws2cs_get_occupation_daily_award_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_get_city_master_stuff, ws2cs_get_city_war_master_stuff_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_del_city_master_stuff, ws2cs_del_city_war_master_stuff_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_fire_bonfire, ws2cs_fire_bonfire_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_add_city_war_winner_stuff, ws2cs_add_city_war_winner_stuff_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_del_city_war_winner_stuff, ws2cs_del_city_war_winner_stuff_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_add_fuel_to_bonfire, ws2cs_add_fuel_to_bonfire_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_legion_rm_recruit_cd, ws2cs_legion_rm_recruit_cd_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_draw_bonfire_daily_rwd, ws2cs_draw_bonfire_daily_rwd_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_get_bonus_award, ws2cs_get_bonus_award_func);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_legion_answer_question_right, ws2cs_answer_question_right);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_rep_get_arena_rank, ws2cs_rep_get_rank);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_req_challenge, ws2cs_res_challenge);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_req_challenge_list, ws2cs_get_choose_ist);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_req_worship_player_end, ws2cs_worship_player_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_sync_first_rank_end, ws2cs_req_sync_first_rank_end);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_sync_server_avg_lv, ws2cs_req_sync_server_avg_lv);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_req_service_rank_info_end, ws2cs_req_service_rank_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_check_item_enough, ws2cs_marry_check_item_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_marry_result, ws2cs_marry_result_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_marry_get_reward, ws2cs_marry_get_reward_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_update_wedding_ring_state, ws2cs_update_wedding_ring_state_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_update_wedding_ring_level, ws2cs_update_wedding_ring_level_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_marry_get_marry_task, ws2cs_marry_get_marry_task_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_sync_marry_task, ws2cs_marry_sync_marry_task);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_refresh_heart_value, ws2cs_refresh_heart_value_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_req_cost_bless_money, ws2cs_req_cost_bless_money_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_return_special_item_times, ws2cs_return_special_item_times_proc);

	message_manager::getInstance().register_handler(e_msgindex_ws2cs_recive_mail, ws2cs_receive_mail);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_had_new_globel_mail, ws2cs_refresh_globel_mail_sign_state);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_get_red_pack_end, ws2cs_get_red_pack_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_send_red_pack_end, ws2cs_send_red_pack_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_receive_notice, ws2cs_receive_notice_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_send_chat_end, ws2cs_send_chat_end_process);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_role_right, ws2cs_role_right_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_role_info_change, ws2cs_role_info_change_func);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_recharge, ws2cs_recharge_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_buy_good, ws2cs_role_buy_good);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_send_connect_success, dp2cs_connect_success_func);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_character_error, dp2cs_rep_load_char_data_error);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_save_character, dp2cs_rep_save_char_data_complete);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_character, dp2cs_rep_load_char_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_data, dp2cs_rep_load_char_data);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_buff, dp2cs_rep_load_char_buff);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_skill, dp2cs_rep_load_char_skill);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_legion_skill, dp2cs_rep_load_char_legion_skill);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_item, dp2cs_rep_load_char_item);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_money, dp2cs_rep_load_char_money);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_talent, dp2cs_rep_load_char_talent);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_treasure, dp2cs_rep_load_char_treasure);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_pokedexs, dp2cs_rep_load_char_pokedex);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_tinder, dp2cs_rep_load_char_tinder);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_spirit, dp2cs_rep_load_char_spirit);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_welfare, dp2cs_rep_load_char_welfare);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_title, dp2cs_rep_load_char_title);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_active_degree, dp2cs_rep_load_char_active_degree);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_welfare_random_get, dp2cs_rep_load_char_welfare_random_get);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_mission, dp2cs_rep_load_char_mission);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_att, dp2cs_rep_load_char_att);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_goods, dp2cs_rep_load_char_goods);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_worship_target, dp2cs_rep_load_worship_target);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_map, dp2cs_rep_load_char_map_record);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_demons_tower, dp2cs_rep_load_char_demons_tower);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_element_heart, dp2cs_rep_load_char_element_heart);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_special_name, dp2cs_rep_load_char_special_name);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_convert, dp2cs_rep_load_char_convert);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_achievement, dp2cs_rep_load_char_achievement);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_show, dp2cs_rep_load_char_show_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_yesterday_must_do, dp2cs_rep_load_char_yester_must_do);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_guide_record, dp2cs_rep_load_char_guide_record);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_first_time_do_record, dp2cs_rep_load_char_first_time_do);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_service_goal, dp2cs_req_load_char_service_goal_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_seven_day_goal, dp2cs_req_load_char_seven_day_goal_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_daily_must_do_count, dp2cs_rep_load_char_daily_must_do_count);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_resp_load_arena_log, dp2cs_rep_load_arena_log);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_arena_data_error, dp2cs_rep_arena_data_error);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_mail, dp2cs_rep_load_char_mail);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_del_char_mail_end, dp2cs_rep_del_char_mail_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_operate_load_lock_mail, dp2cs_operate_load_lock_mail_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_mail_item, dp2cs_rep_load_char_mail_item);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_player_had_globel_mail_end, dp2cs_rep_load_char_had_globel_mail_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_error, dp2cs_rep_load_arena_char_error);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_info, dp2cs_rep_load_arena_char_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_att, dp2cs_rep_load_arena_char_att);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_skill, dp2cs_rep_load_arena_char_skill);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_item, dp2cs_rep_load_arena_char_item);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_spirit, dp2cs_rep_load_arena_char_spirit);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_fight_att, dp2cs_rep_load_arena_char_fight_att);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_buff, dp2cs_rep_load_arena_char_buff);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_arena_load_char_belief, dp2cs_rep_load_arena_char_belief);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_service_rank_reward_state, dp2cs_req_load_service_rank_reward_state);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_phantom, dp2cs_rep_load_char_phantom);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_chat_record, dp2cs_rep_load_chat_record);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_recycle, dp2cs_rep_load_char_recycle);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_recycle_task, dp2cs_rep_load_char_recycle_task);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_recycle_invited, dp2cs_rep_load_char_recycle_invited);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_recycle_get_inviter_info_end, dp2cs_rep_recycle_get_inviter_info_end);
	//get other player info
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_base_end, dp2cs_rep_load_other_player_info_base);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_equiping_end, dp2cs_rep_load_other_player_info_equiping);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_special_name_end, dp2cs_rep_load_other_player_info_special_name);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_base_group_end, dp2cs_rep_load_other_player_info_base_group);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_spirit_end, dp2cs_rep_load_other_player_info_spirit);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_wing_or_mount_end, dp2cs_rep_load_other_player_info_wing_or_mount);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_buff_end, dp2cs_rep_load_other_player_info_buff);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_feather_end, dp2cs_rep_load_other_player_info_feather);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_gain_treasure_info, dp2cs_req_load_gain_treasure_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_oracle_trial_all_info, dp2cs_rep_load_oracle_trial_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_boss_island_info, dp2cs_req_load_boss_island_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_role_harry_info, dp2cs_req_load_role_harry_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_player_info_appearance_end, dp2cs_rep_load_other_player_info_appearance);
	// relation
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_full_friend_guid, ws2cs_full_friend_guid_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_req_relation_end, ws2cs_req_relation_end_proc);
	//auction
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_auction_sell_end, ws2cs_auction_sell_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_auction_purchase_success, ws2cs_auction_purchase_success_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_auction_sell_success_info, ws2cs_auction_sell_success_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_auction_load_self_selling_end, dp2cs_auction_load_self_selling_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_auction_cancel_sell, ws2cs_auction_cancel_sell_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_auction_req_goods_list_end, dp2cs_auction_get_auction_list_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_auction_req_trade_record_end, dp2cs_auction_get_trade_record_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_auction_req_another_sell_info_end, dp2cs_auction_get_another_sell_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_auction_find_bid_target_end, ws2cs_auction_find_bid_target_end_proc);
	//message_manager::getInstance().register_handler(e_msgindex_ws2cs_bid_replace_info, ws2cs_bid_replace_info_proc);
	//message_manager::getInstance().register_handler(e_msgindex_dp2cs_req_bid_record_failure_end, dp2cs_req_bid_record_failure_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_time_out_del, ws2cs_time_out_del_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_req_bid_notice_end, dp2cs_auction_req_bid_notice_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_req_bid_notice_red_end, dp2cs_auction_req_bid_notice_red_end_proc);
	//message_manager::getInstance().register_handler(e_msgindex_dp2cs_auction_find,dp2cs_auction_find_proc);
	//message_manager::getInstance().register_handler(e_msgindex_dp2cs_auction_buy,dp2cs_auction_buy_proc);
	// highest record
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_hightest_record_end, dp2cs_load_hightest_record_end_proc);
	//world boss
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_remove_world_boss, ws2cs_remove_world_boss_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_create_world_boss, ws2cs_create_world_boss_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_create_first_kill_prize, ws2cs_create_first_kill_world_boss_prize);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_create_big_player_statue, ws2cs_create_big_player_statue_proc);
	//world
	//message_manager::getInstance().register_handler(e_msgindex_c2s_map_game_gift, c2cs_req_temple_gift_num);
	message_manager::getInstance().register_handler(e_msgindex_c2s_map_targger_volume, c2cs_req_map_tigger_volume);
	message_manager::getInstance().register_handler(e_msgindex_c2s_interact_with_npc, c2cs_req_interact_with_npc);
	// team
	message_manager::getInstance().register_handler(e_msgindex_c2s_create_team, c2cs_create_team);
	message_manager::getInstance().register_handler(e_msgindex_c2s_join_team, c2cs_join_team);
	message_manager::getInstance().register_handler(e_msgindex_c2s_update_team_attribute, c2cs_change_team_type);
	// player
	message_manager::getInstance().register_handler(e_msgindex_c2s_back_to_self_server, c2cs_back_to_self_server);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_scene, c2cs_req_transfer_scene);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_scene_pos, c2cs_req_transfer_scene_pos);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_scene_born, c2cs_req_transfer_scene_born);
	message_manager::getInstance().register_handler(e_msgindex_c2s_character_gmorder_message, c2s_player_gmorder_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_character_pick_drop_box, c2s_player_pick_drop_box);
	message_manager::getInstance().register_handler(e_msgindex_c2s_aoi_location, c2cs_req_aoi_location);
	message_manager::getInstance().register_handler(e_msgindex_c2s_aoi_rotation, c2cs_req_aoi_rotation);
	message_manager::getInstance().register_handler(e_msgindex_c2s_aoi_show_player, c2cs_req_aoi_show_player);
	message_manager::getInstance().register_handler(e_msgindex_c2s_aoi_simple_pos, c2cs_req_aoi_simple_pos);
	message_manager::getInstance().register_handler(e_msgindex_c2s_leave_map, c2cs_leave_map);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_hit_and_get_npc, c2cs_req_hit_and_get_npc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_character_one_att, c2cs_req_att_game_change);
	message_manager::getInstance().register_handler(e_msgindex_c2s_grade_up_level_ani_finish, c2cs_grade_level_ani_finish_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_body_base_att, c2cs_req_base_body_att);
	message_manager::getInstance().register_handler(e_msgindex_c2s_add_damage_buff_in_expraid, c2cs_add_damage_buff_in_exp_raid);
	message_manager::getInstance().register_handler(e_msgindex_c2s_buy_raid_count, c2cs_buy_raid_count);
	message_manager::getInstance().register_handler(e_msgindex_c2s_show_vip, c2s_is_show_vip);
	// skill
	message_manager::getInstance().register_handler(e_msgindex_c2s_skill_spell, skill_spell_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_skill_cast, skill_cast_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_skill_hurt, skill_hurt_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_skill_operation, skill_req_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_skill_change_index, skill_use_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_buff_operation, buff_req_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_passive_skill_up_leve, passive_skill_up_leve);
	message_manager::getInstance().register_handler(e_msgindex_c2s_passive_skill_equip, c2cs_passive_skill_equip);
	//item
	message_manager::getInstance().register_handler(e_msgindex_c2s_item_operation, item_operate_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_element_heart_operate, item_element_operate_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_item_composit, item_composit_operate_message);
	message_manager::getInstance().register_handler(e_msgindex_c2s_show_fashion, show_fashion);
	message_manager::getInstance().register_handler(e_msgindex_c2s_unlock_skill, star_skill_unlock);
	message_manager::getInstance().register_handler(e_msgindex_c2s_find_back_must_do_resource, c2s_find_back_must_do_resource);
	message_manager::getInstance().register_handler(e_msgindex_c2s_recharge, c2s_recharge);
	message_manager::getInstance().register_handler(e_msgindex_c2s_recharge_widget_type, c2s_recharge_widget_type);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_first_recharge_time, c2s_get_first_recharge_time);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_hope_item, item_get_hope_item);
	message_manager::getInstance().register_handler(e_msgindex_c2s_set_quick_call_mount, item_set_quick_call_mount_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_character_fuhuo, c2cs_unit_fuhuo);
	message_manager::getInstance().register_handler(e_msgindex_c2s_bag_unlock_slot_req, c2cs_unlock_bag_slot);
	message_manager::getInstance().register_handler(e_msgindex_c2s_lucky_draw, c2cs_lucky_draw_req);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_first_raid_award, c2cs_get_first_raid_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_character_auto_use_hp_pot, c2cs_auto_use_hp_pot);
	message_manager::getInstance().register_handler(e_msgindex_c2s_ask_unit_dead, c2cs_ask_unit_dead);
	message_manager::getInstance().register_handler(e_msgindex_c2s_mopping_up, c2cs_mopping_up);
	message_manager::getInstance().register_handler(e_msgindex_c2s_mopping_up_demons_tower, c2cs_mopping_up_demons_tower);
	message_manager::getInstance().register_handler(e_msgindex_c2s_mopping_up_demons_tower_get_award, c2cs_mopping_up_demons_tower_get_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_change_pk_mode, c2cs_change_pk_mode);
	message_manager::getInstance().register_handler(e_msgindex_c2s_change_role_class, c2cs_change_role_class);
	message_manager::getInstance().register_handler(e_msgindex_c2s_all_mopping_up, c2cs_all_mopping_up);
	message_manager::getInstance().register_handler(e_msgindex_c2s_spirit_fetter_upgrade, c2cs_spirit_fetter_upgrade);
	
	message_manager::getInstance().register_handler(e_msgindex_c2s_change_sky_suit_shape, req_change_sky_suit_state);

	// legion
	message_manager::getInstance().register_handler(e_msgindex_c2s_convenient_join_legion, c2cs_convenient_join_legion);
	message_manager::getInstance().register_handler(e_msgindex_c2s_draw_mem_daily_award, c2cs_draw_mem_daily_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_legion_territory_buff_operate, c2cs_territory_buff_operate);
	message_manager::getInstance().register_handler(e_msgindex_c2s_goto_next_stage_of_bonfire_map, c2cs_goto_next_stage_of_bonfire_map);
	message_manager::getInstance().register_handler(e_msgindex_c2s_operation_legion_warehouse_item, c2cs_legion_warehouse_operation);
	message_manager::getInstance().register_handler(e_msgindex_c2s_invite_player_to_join_legion, c2cs_invite_player_to_join_legion);
	message_manager::getInstance().register_handler(e_msgindex_c2s_apply_to_join_legion, c2cs_req_apply_to_join_legion);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_set_legion_average_lv_to_map, ws2cs_set_legion_average_lv_to_map_proc);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_confirm_change_legion_name, ws2cs_confirm_change_legion_name_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_rem_item_change_legion_name_care, ws2cs_rem_item_change_legion_name_care_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_change_gate_legion_name, ws2cs_change_gate_legion_name_func);
	message_manager::getInstance().register_handler(e_msgindex_c2s_add_legion_boss_damage, c2cs_add_damage_buff_in_legion_boss);

	// talen
	message_manager::getInstance().register_handler(e_msgindex_c2s_talent_level_up, c2cs_talent_level_up);
	message_manager::getInstance().register_handler(e_msgindex_c2s_talent_reset, c2cs_talent_reset);
	//meditation
	message_manager::getInstance().register_handler(e_msgindex_c2s_start_meditation, c2cs_start_meditation);
	message_manager::getInstance().register_handler(e_msgindex_c2s_stop_mediation, c2cs_stop_meditation);
	//treasure
	message_manager::getInstance().register_handler(e_msgindex_c2s_treasure_notice, c2cs_req_treasure_notice);
	message_manager::getInstance().register_handler(e_msgindex_c2s_jump_raid_animation, c2cs_jump_raid_animation);
	// guide
	message_manager::getInstance().register_handler(e_msgindex_c2s_add_guide_record, c2s_add_guide_record);
	message_manager::getInstance().register_handler(e_msgindex_c2s_guide_trigger_start, c2s_guide_trigger_start);
	//arena
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_rank, req_get_arena_rank);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_match_player, req_get_match_player);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_top_three_player, req_get_first_three_player);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_challenge, req_do_challenge);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_get_challenge_log, get_challenge_log);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_arena_reward, get_arena_reward);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_mopping_remain_times, mopping_arena_remain_times);
	message_manager::getInstance().register_handler(e_msgindex_c2s_ranking_worship, c2cs_worship_top_player_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_service_rank_recevice_reward, c2cs_service_rank_receice_reward_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_mail_operation, c2s_operate_mail);
	message_manager::getInstance().register_handler(e_msgindex_c2s_mail_send_mail, c2s_send_mail);
	message_manager::getInstance().register_handler(e_msgindex_s2c_mail_send_to_all_player, c2s_send_mail_to_all_player);
	message_manager::getInstance().register_handler(e_msgindex_c2s_send_notice, c2s_send_notice);
	message_manager::getInstance().register_handler(e_msgindex_c2s_send_notice_with_param, c2s_send_notice_with_param);
	message_manager::getInstance().register_handler(e_msgindex_c2s_send_chat_message_new, c2s_send_chat_new);
	//title
	message_manager::getInstance().register_handler(e_msgindex_c2s_title_set_title_no_need_see, c2s_title_set_title_no_need_see);
	//big_player
	message_manager::getInstance().register_handler(e_msgindex_c2s_worship, c2s_worship_by_type);
	//find_other_player_info
	message_manager::getInstance().register_handler(e_msgindex_c2s_find_other_unit, c2s_find_other_unit);
	//red package
	message_manager::getInstance().register_handler(e_msgindex_c2s_send_red_package, c2s_send_red_package);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_red_package, c2s_get_red_package);
	//transfer buff
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_buff, c2s_transfer_buff);
	// first time do
	message_manager::getInstance().register_handler(e_msgindex_c2s_add_first_time_do_record, c2cs_add_first_time_do_record);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_func_unlock_award, c2cs_get_func_unlock_award);
	//auction
	message_manager::getInstance().register_handler(e_msgindex_c2s_auction_operate, c2cs_auction_operate_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_auction_list, c2cs_auction_req_show_list);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_trade_record, c2cs_auction_req_trade_record);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_another_sell_info, c2cs_auction_req_another_sell_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_self_bid_list, c2cs_auction_req_self_bid_list);
	// achievement
	message_manager::getInstance().register_handler(e_msgindex_c2s_achievement_finish, c2s_achievement_finish);
	message_manager::getInstance().register_handler(e_msgindex_c2s_achievement_operate, c2s_achievement_all_finish);
	// phantom
	message_manager::getInstance().register_handler(e_msgindex_c2s_phantom_operate, c2cs_phantom_oper);
	message_manager::getInstance().register_handler(e_msgindex_c2s_chat_record_oper, c2cs_chat_record_oper);
	// recycle
	message_manager::getInstance().register_handler(e_msgindex_c2s_recycle_invited_reward, c2cs_recycle_invited_get_reward);

	//SendGift
	message_manager::getInstance().register_handler(e_msgindex_c2s_add_friendliness_value, c2cs_add_friendliness_value);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_service_goal_operate, c2s_service_goal_operate);
	// interaction
	message_manager::getInstance().register_handler(e_msgindex_c2s_c_s_interaction_invite, c2s_interaction_invite);
	message_manager::getInstance().register_handler(e_msgindex_c2s_c_s_interaction_invite_reply, c2s_interaction_invite_reply);
	message_manager::getInstance().register_handler(e_msgindex_c2s_c_s_real_start_interaction, c2s_interaction_invite_start);
	message_manager::getInstance().register_handler(e_msgindex_c2s_c_s_stop_interaction, c2s_stop_interaction);
	//cloud shop
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_cloud_shop_buy_req, ws2cs_cloud_shop_buy_proc);
	//time limit activity
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_time_limit_activity_info_end, dp2cs_time_limit_activity_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_time_limit_get_act_reward, c2cs_time_limit_get_act_reward);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_check_time_limit_activity_common_data, ws2cs_check_time_limit_activity_common_data_resp);
	message_manager::getInstance().register_handler(e_msgindex_c2s_time_limit_add_schedule, c2cs_time_limit_add_schedule);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_set_time_limit_activity_pause_flag, ws2cs_gm_set_time_limit_activity_pause_flag_resp);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_end_act_type, ws2cs_end_act_type_func);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_time_limit_temp, ws2cs_time_limit_act_temp);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_time_limit_branch_temp, ws2cs_time_limit_act_branch_temp);
	message_manager::getInstance().register_handler(e_msgindex_c2s_buy_time_limit_item, c2s_buy_time_limit_item);
	message_manager::getInstance().register_handler(e_msgindex_c2s_time_limit_buy_diamond_shop, c2s_buy_diamond_shop);
	message_manager::getInstance().register_handler(e_msgindex_c2s_star_trip_operate, c2s_star_trip_operate);

	//rename
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_sub_player_rename_item, ws2cs_sub_player_item_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_change_player_name, ws2cs_change_player_name_proc);
	//first
	message_manager::getInstance().register_handler(e_msgindex_c2s_character_legion_look_first, c2s_first_look_legion);
	//gm
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_spawn_npc, ws2cs_gm_spawn_npc_func);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_modify_money, ws2cs_gm_modify_money_func);
	//server_harry
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_send_cur_server_harry_info, ws2cs_set_cur_server_harry_func);
	message_manager::getInstance().register_handler(e_msgindex_c2s_give_server_item_to_npc, c2s_give_server_item_to_npc_func);

	message_manager::getInstance().register_handler(e_msgindex_ws2cs_send_create_normal_player, ws2cs_send_create_normal_player_func);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_send_create_first_harry_player, ws2cs_create_harry_player_statue_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_send_clear_harry_player, ws2cs_clear_harry_player_statue_proc);
	//cross_ladder
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_ladder_request_buy_join_ticket, c2s_cross_ladder_req_buy_ticket);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_cross_ladder_refresh_ticket_info, ws2cs_cross_ladder_refresh_ticket_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_ladder_req_mission_reward, c2s_cross_ladder_req_mission_reward);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_cross_ladder_send_last_score, ws2cs_cross_ladder_send_last_score_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_ladder_return_to_source_zone, c2s_cross_ladder_return_to_source_zone_proc);

	//cross_server_world_boss
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_cross_server_world_boss_creat_boss, ws2cs_cross_server_world_boss_creat_boss_func);
	message_manager::getInstance().register_handler(e_msgindex_c2s_send_get_boss_is_dead, c2s_cross_boss_get_boss_is_dead);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_req_upgrade_wedding_ring, c2s_req_upgrade_wedding_ring_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_boss_god_time, c2s_get_boss_god_time);

	//marry
	//message_manager::getInstance().register_handler(e_msgindex_c2s_get_marry_examination, c2s_get_marry_examination);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_confirm_marry_examination, c2s_confirm_marry_examination);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_operate_marry_examination, c2s_operate_marry_examination);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_use_fireworks, c2s_req_use_fireworks_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_lucky_info, c2s_req_lucky_info_proc);

	//oracle_trial
	//message_manager::getInstance().register_handler(e_msgindex_c2s_send_oracle_trial_commpingup, c2s_send_oracle_trial_commpingup_proc);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_get_oracle_trial_customs_award, c2s_get_oracle_trial_customs_award);

	//person_information
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_person_information, c2s_get_person_information_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_other_person_information, c2s_get_other_person_information_proc);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_get_person_infor_reward, c2s_get_person_infor_reward_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_person_information, dp2cs_get_person_information_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_other_person_information, dp2cs_get_other_person_information_proc);

	//competition
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_role_competition, dp2cs_get_role_competition_proc);

	message_manager::getInstance().register_handler(e_msgindex_c2s_get_role_competition, c2s_get_role_competition_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_competition_lv_reward, c2s_get_competition_lv_reward_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_competition_lv_reward_all, c2s_get_competition_lv_reward_all_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_buy_competition_lv, c2s_buy_competition_lv_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_buy_speical_manual, c2s_buy_speical_manual_proc);

	message_manager::getInstance().register_handler(e_msgindex_c2s_get_element_competition, c2s_get_element_competition_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_element_competition_lv_reward, c2s_get_element_competition_lv_reward_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_element_competition_lv_reward_all, c2s_get_element_competition_lv_reward_all_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_buy_element_competition_lv, c2s_buy_element_competition_lv_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_buy_element_speical_manual, c2s_buy_speical_element_manual_proc);

	//legion_station
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_send_bonfire_level, ws2cs_send_bonfire_level_func);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_set_legion_guid_to_map, ws2cs_set_map_own_legion);
	message_manager::getInstance().register_handler(e_msg_index_ws2cs_transfer_all_player_this_map, ws2cs_transfer_all_player_this_map);

	message_manager::getInstance().register_handler(e_msgindex_c2s_get_map_legion_player, c2s_get_map_legion_player_proc);

	//assist_fight
	message_manager::getInstance().register_handler(e_msgindex_c2s_start_assist_fight, c2s_start_assist_fight_proc);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_req_assist_fight, c2s_req_assist_fight_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cancel_assist_fight, c2s_cancel_assist_fight_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_sync_assist_fight_info, ws2cs_sync_assist_fight_info_proc);

	//check_in
	//message_manager::getInstance().register_handler(e_mgsindex_c2s_get_cumulative_sign_in_reward, c2s_get_cumulative_sign_in_reward);
	//message_manager::getInstance().register_handler(e_mgsindex_c2s_retroactive_all_days, c2s_retroactive_all_days);

	message_manager::getInstance().register_handler(e_msgindex_c2s_send_buy_simage_boss_num, c2s_buy_simage_boss_num);

	message_manager::getInstance().register_handler(e_msgindex_c2s_character_save_appearance_info, c2s_save_appearance_info);

	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_role_dragontrip, dp2cs_get_role_dragontrip_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_dragontrip_operate, c2s_dragontrip_oper);
	message_manager::getInstance().register_handler(e_msgindex_c2s_mountpower_operate, c2s_mountpower_oper);
	message_manager::getInstance().register_handler(e_msgindex_c2s_item_jewel_carve_oper, c2s_jewel_carve_oper);
	message_manager::getInstance().register_handler(e_msgindex_c2s_item_upgrade, c2s_item_upgrade);



	message_manager::getInstance().register_handler(e_msgindex_ws2cs_create_pk_king_combat_map_end, ws2cs_create_pk_king_combat_map_end_proc);

	message_manager::getInstance().register_handler(e_msgindex_ws2cs_element_war_send_create_robot, ws2cs_element_war_map_init_proc);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_element_war_check_can_get_mission_reward_end, ws2cs_element_war_get_mission_reward);
	message_manager::getInstance().register_handler(e_mgsindex_c2s_element_war_buy_ticket, c2s_element_war_buy_ticket);
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_element_war_send_last_score, ws2cs_element_war_send_last_score_proc);

	message_manager::getInstance().register_handler(e_mgsindex_c2s_add_use_power_up, c2s_add_use_power_up);
	message_manager::getInstance().register_handler(e_msgindex_c2s_create_assist_fight_npc, c2s_create_assist_fight_npc_proc);

	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_role_skytreasure, dp2cs_get_role_skytreasure_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_skytreasure_operate, c2s_skytreasure_operation);

	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_role_starark, dp2cs_get_role_starark_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_starark_operate, c2s_starark_oper);

	message_manager::getInstance().register_handler(e_msgindex_c2s_lucky_card_operate, c2s_lucky_card_operate);

	message_manager::getInstance().register_handler(e_msgindex_c2s_change_enchant_show_type, c2s_change_enchant_show_type);

	message_manager::getInstance().register_handler(e_msg_index_ws2cs_set_ladder_world_level, ws2cs_set_ladder_world_level_proc);
	
	// attack_city
	message_manager::getInstance().register_handler(e_msgindex_ws2cs_attack_city_init_data, ws2cs_attack_city_init_data_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_attack_city_operate, c2s_attack_city_operate_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_attack_city_map_rank_list, c2s_get_attack_city_map_rank_list_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_bron_pos, c2s_transfer_to_bron_pos_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_attack_legion_call, c2s_attack_legion_call_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_end_attack_transfer_info, c2s_end_attack_transfer_info_proc);

	// time_feed_back
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_time_feed_back_to_db_end, dp2cs_get_time_feed_back_to_db_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_time_feed_back_operation, c2s_time_feed_back_operation_proc);

	// time_limit_gift
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_time_limit_gift_to_db_end, dp2cs_get_time_limit_gift_to_db_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_time_limit_gift_operation, c2s_time_limit_gift_operation_proc);

	message_manager::getInstance().register_handler(e_msgindex_c2s_send_subscribe_daily, c2s_send_subscribe_daily);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_get_subscribe_daily_info_to_db_end, dp2cs_get_subscribe_daily_info_to_db_end_proc);

	message_manager::getInstance().register_handler(e_msgindex_c2s_fuben_vip_mopping, c2s_fuben_vip_mopping);

	//world boss
	message_manager::getInstance().register_handler(e_msgindex_c2s_kill_boss, c2s_fuben_kill_boss);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_offline_award, c2s_get_offline_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_online_award, c2s_get_online_award);

	message_manager::getInstance().register_handler(e_msgindex_c2s_kill_award_npc, c2s_kill_award_npc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_upgrade_search_level, c2s_upgrade_search_level);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_npc_award, c2s_get_npc_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_open_treasuer_pic, c2s_open_treasure_pic);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_treasuer_award, c2s_get_treasuer_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_map_anim_end, c2s_map_anim_end);
	message_manager::getInstance().register_handler(e_msgindex_c2s_refrush_award_npc, c2s_refrush_award_npc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_search_award_npc, c2s_get_next_award_npc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_flip_box_open, c2s_flip_box_end);
	message_manager::getInstance().register_handler(e_msgindex_c2s_mop_up_map, c2s_mop_up_map);

	message_manager::getInstance().register_handler(e_msgindex_c2s_finger_guess_end, c2s_finger_guess_end);
	message_manager::getInstance().register_handler(e_msgindex_c2s_smashing_bottle_end, c2s_smashing_bottle_end);

	message_manager::getInstance().register_handler(e_msgindex_c2s_get_activity, c2s_wheel_plate_begin);
	message_manager::getInstance().register_handler(e_msgindex_c2s_wheel_plate_end, c2s_wheel_plate_end);
	return true;
}
}
