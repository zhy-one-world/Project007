/********************************************************************
	created:	2014/07/31
	created:	31:7:2014   15:20
	file base:	msg_dispatch
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "cross_group/msgproc_cross.hpp"
#include "msg_dispatch.hpp"
#include "msgproc/msgproc_client.hpp"
#include "msgproc/msgproc_cs.hpp"
#include "msgproc/msgproc_dp.hpp"
#include "msgproc/msgproc_fep.hpp"
#include "net.pb.h"
#include "net/message_manager.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"
#include <activity_msg.hpp>
#include <appearance_info_msg.hpp>
#include <arena_msg.hpp>
#include <attack_city_msg.hpp>
#include <auction_msg.hpp>
#include <big_player_msg.hpp>
#include <boss_island_msg.hpp>
#include <char_msg.hpp>
#include <chat_msg.hpp>
#include <cloud_shop_msg.hpp>
#include <core.hpp>
#include <cross_ladder_msg.hpp>
#include <cross_server_harry_msg.hpp>
#include <cross_server_pk_msg.hpp>
#include <cross_server_world_boss_msg.hpp>
#include <daemon.hpp>
#include <element_war_msg.hpp>
#include <event_msg.hpp>
#include <gain_treasure_msg.hpp>
#include <gate_msg.hpp>
#include <internal.hpp>
#include <item_msg.hpp>
#include <legion_msg.hpp>
#include <legion_station_msg.hpp>
#include <login_msg.hpp>
#include <lucky_draw_record_msg.hpp>
#include <mail_msg.hpp>
#include <marry_msg.hpp>
#include <pk_king_msg.hpp>
#include <ranking_msg.hpp>
#include <role_pk_msg.hpp>
#include <time_limit_activity_msg.hpp>
#include <world_boss_msg.hpp>
#include <world_server_msg.hpp>


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
	// 加载资源
	res_load();

	// 注册lua脚本函数
	register_lua_handler();

	// 注册消息处理函数
	// internal message
	message_manager::getInstance().register_handler(e_msg_index_req_login, boost::bind(&world_server::internal_req_login, &world_server::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_index_appserver_dataupdate, boost::bind(&world_server::internal_appserver_dataupdate, &world_server::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_server2gate_send_to_server, boost::bind(&ws_client::internal_gate_msg, &ws_client::getInstance(), _1, _2, _3));
	// gate2ws
	message_manager::getInstance().register_handler(e_msg_server2gate_register, gate2ws_rep_register);
	message_manager::getInstance().register_handler(e_msg_gate2server_off_line, gate2ws_rep_off_line);
	message_manager::getInstance().register_handler(e_msg_gate2gate_new, gate2ws_rep_gate_new);
	message_manager::getInstance().register_handler(e_msg_gate2ws_rep_check_from_ws, gate2ws_rep_from_ws);
	message_manager::getInstance().register_handler(e_msg_gate2ws_change_cross_id, gate2ws_change_server_id_func);
	message_manager::getInstance().register_handler(e_msg_gate2ws_all_server_arr, gate2ws_this_cross_all_server_info);
	message_manager::getInstance().register_handler(e_msg_gate2ws_end_refresh_list, gate2ws_end_refresh_server_list_func);
	message_manager::getInstance().register_handler(e_msg_gate2ws_server_hide_act, gate2ws_set_server_act_hide_func);
	message_manager::getInstance().register_handler(e_msg_gate2ws_reload_csv, gate2ws_reload_csv);
	// cs2ws
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_reconnect_game,						cs2ws_rep_reconnect_game);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_enter_game,							cs2ws_rep_enter_game);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_client_logout,						cs2ws_rep_logout);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_enter_scene,							cs2ws_rep_enter_scene);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_transfer_player,						cs2ws_req_transfer_player);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_role_info,							cs2ws_rep_role_info);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_map_state,							cs2ws_rep_map_state);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_get_map_info_from_cs_end,				cs2ws_rep_get_map_info_from_cs_end);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_send_role_info_to_gm,					cs2ws_receive_role_info_gm);
	
	// fep2ws internal protocol
	message_manager::getInstance().register_handler(e_msgindex_c2s_client_login, fep2ls_request_client_login);
	message_manager::getInstance().register_handler(e_msgindex_c2s_enum_char, fep2ls_enum_character_func);
	message_manager::getInstance().register_handler(e_msgindex_fep2ls_create_character, fep2ls_create_character_func);
	message_manager::getInstance().register_handler(e_msgindex_c2s_del_char, fep2ls_delete_character_func);

	message_manager::getInstance().register_handler(e_msg_index_fep2ws_client_reconnect,					fep2ws_client_reconnect_func);
	message_manager::getInstance().register_handler(e_msg_index_fep2ws_client_logined,						fep2ws_client_loginned);
	message_manager::getInstance().register_handler(e_msg_index_fep2ws_leave_queued,						fep2ws_client_leave_queued);
	message_manager::getInstance().register_handler(e_msg_index_fep2ws_client_logout,						fep2ws_req_logout_client);
	
	//internet protocol
	message_manager::getInstance().register_handler(e_msgindex_c2s_enter_game,								c2ws_req_enter_game);
	message_manager::getInstance().register_handler(e_msgindex_c2s_enter_scene,							c2ws_enter_scene);
	message_manager::getInstance().register_handler(e_msgindex_c2s_recharge_success_by_sdk,				c2ws_recharge_success_by_sdk_proc);

	// dp2ws
	message_manager::getInstance().register_handler(e_msg_index_dp2ls_client_login, dp2ls_reply_client_login);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_enum_character, dp2ls_reply_character_list);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_create_character, dp2ls_reply_create_character);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_delete_character, dp2ls_reply_delete_character);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_item, dp2ls_load_enum_item_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_buff, dp2ls_load_enum_buff_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_spirit, dp2ls_load_enum_spirit_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_time, dp2ls_load_enum_time_end);

	message_manager::getInstance().register_handler(e_msgindex_dp2ws_get_game_info,						dp2ws_rep_get_game_info_ws);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_character,						dp2ws_rep_get_role_info_ws);
	//message_manager::getInstance().register_handler(e_msg_index_dp2ws_add_mail_to_sql_failed,				dp2ws_resend_mail_to_sender);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_arena_rank,						dp2ws_rep_load_arena_rank);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_create_robot,							dp2ws_create_arena_robot);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_finish,							dp2ws_load_arena_finish);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_ranking_info,				dp2ws_rep_load_all_ranking_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_ranking_info_one,				dp2ws_rep_load_one_ranking_info);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_get_worship_value_end,				dp2ws_rep_load_one_worship_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_service_rank_info,			dp2ws_rep_load_all_service_rank_info);

	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_server_list_end,				dp2ws_rep_load_last_server_list);
	

	//gm
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_gm_set_result,							dp2ws_gm_set_result_proc);

	message_manager::getInstance().register_handler(e_msgindex_dp2ws_save_recharge,						dp2ws_save_recharge_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_send_recharge_end,					dp2ws_send_recharge_end_result_proc);
	message_manager::getInstance().register_handler(e_msg_index_dp2ws_rep_query_server_info_gm,			dp2ws_gm_get_server_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_gm_change_name,						dp2ws_gm_change_account_proc);

	message_manager::getInstance().register_handler(e_msgindex_dp2ws_gm_get_role_info, dp2ws_gm_get_role_info_proc);

	message_manager::getInstance().register_handler(e_msgindex_dp2ws_gm_modify_role_info, dp2ws_gm_modify_role_info_proc);
	//message_manager::getInstance().register_handler(e_msgindex_dp2ws_save_recharge, dp2ws_save_recharge_proc);
	//message_manager::getInstance().register_handler(e_msgindex_dp2ws_send_recharge_end, dp2ws_send_recharge_end_result_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_time, dp2ws_gm_get_role_time_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_money, dp2ws_gm_get_role_money_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_gm_get_role_lock, dp2ws_gm_get_role_lock_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_char_item, dp2ws_gm_get_role_items_info_proc);
	message_manager::getInstance().register_handler(e_msg_index_dp2ws_rep_query_server_info_gm, dp2ws_gm_get_server_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_gm_change_name, dp2ws_gm_change_account_proc);

	// activity
	message_manager::getInstance().register_handler(e_msgindex_c2s_send_special_server_msg,					c2ws_special_server_msg);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_shui_jing_map,				c2ws_req_transfer_to_shui_jing_map);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_pk_king_map,				c2ws_req_transfer_to_pk_king_map);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_pk_king_winner,						cs2ws_req_set_big_player_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_broken_sky_map,				c2ws_req_transfer_to_broken_sky_map);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_broken_sky_creat_boss,				cs2ws_req_broken_sky_creat_boss);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_broken_sky_info_sync,				cs2ws_req_broken_sky_info_sync);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_broken_sky_kill_boss,				cs2ws_req_broken_sky_kill_boss);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_pk_king_begin,						cs2ws_req_pk_king_begin);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_cross_server_pk_info,			dp2ws_load_all_cross_server_pk_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_world_boss_rank,					c2ws_get_world_boss_damage_list);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_cross_server_pk_map,		c2ws_req_transfer_to_cross_pk_map);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_cross_server_harry_info,			dp2ws_load_all_cross_server_harry_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_harry_map,					c2ws_req_transfer_to_harry_map);

	//mapinfo
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_one_map_info,						c2ws_req_get_one_map_info);

	// team
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_create_team,							cs2ws_req_create_team);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_join_team,							cs2ws_req_join_team);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_change_team_type,					cs2ws_req_change_team_type);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_apply_to_join_team,					cs2ws_req_apply_to_join_team);
	message_manager::getInstance().register_handler(e_msgindex_c2s_let_role_join_team,						c2ws_req_let_role_join_team);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_del_team_member,						cs2ws_req_del_team_member);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_team_game_over,						cs2ws_req_team_game_over);
	message_manager::getInstance().register_handler(e_msgindex_c2s_leave_team,								c2ws_req_leave_team);
	message_manager::getInstance().register_handler(e_msgindex_c2s_kickout_team_member,					c2ws_req_kickout_team_member);
	message_manager::getInstance().register_handler(e_msgindex_c2s_set_captain,							c2ws_req_set_team_captain);
	message_manager::getInstance().register_handler(e_msgindex_c2s_invite_player,							c2ws_req_invite_player_join_team);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_update_team_member_info_one,			cs2ws_req_update_team_member_info_one);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_update_team_member_pos_info_one,		cs2ws_req_update_team_member_pos_info_one);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_team_to_scene,					c2ws_req_transfer_team_to_scene);
	message_manager::getInstance().register_handler(e_msgindex_c2s_ready,									c2ws_req_ready);
	message_manager::getInstance().register_handler(e_msgindex_c2s_ready_to_transfer_team,					c2ws_req_ready_transfer);
	message_manager::getInstance().register_handler(e_msgindex_c2s_search_team_by_map_id,					c2ws_req_search_team_list);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_no_team_legionmate_list,			c2ws_req_get_no_team_legionmate_list);
	message_manager::getInstance().register_handler(e_msgindex_c2s_apply_to_join_team,						c2ws_req_apply_to_join_team);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_aim_team_list,						c2ws_req_get_team_list_by_aim);
	message_manager::getInstance().register_handler(e_msgindex_c2s_auto_match_team,						c2ws_req_auto_match_team);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_update_team_attribute,					c2ws_req_update_team_attribute);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_team_common_check_in_cs,				cs2ws_recv_team_common_check_in_cs);
	message_manager::getInstance().register_handler(e_msgindex_c2s_update_team_mem_operate,				c2ws_req_team_mem_operate);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_team_change_aim_info,				cs2ws_req_team_change_aim_info);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_after_team_aim_finish_op,			cs2ws_req_after_team_aim_finish_op);
	message_manager::getInstance().register_handler(e_msgindex_c2s_refused_join_team,						c2ws_refused_join_team);

	// legion
	message_manager::getInstance().register_handler(e_msgindex_c2s_create_legion,							c2ws_req_create_legion);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_create_legion_result,				cs2ws_req_create_legion_result);
	message_manager::getInstance().register_handler(e_msgindex_c2s_leave_legion,							c2ws_req_leave_legion);
	message_manager::getInstance().register_handler(e_msgindex_c2s_dissolve_legion,						c2ws_req_disslove_legion);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_list,						c2ws_req_get_legion_list);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_apply_to_join_legion,					c2ws_req_apply_to_join_legion);挪到CS上判断解锁
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_applicant_info_list,			c2ws_req_get_legion_applicant_list);
	message_manager::getInstance().register_handler(e_msgindex_c2s_process_legion_apply,					c2ws_req_process_legion_apply);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_convenient_join_legion,				cs2ws_req_convenient_join_legion);
	message_manager::getInstance().register_handler(e_msgindex_c2s_legion_construction_level_up,			c2ws_req_legion_construction_level_up);
	message_manager::getInstance().register_handler(e_msgindex_c2s_legion_get_construction_buff,			c2ws_req_get_construction_buff);
	message_manager::getInstance().register_handler(e_msgindex_c2s_legion_donate_money,					c2ws_req_legion_donate_money);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_donate_legion_money,					cs2ws_req_donate_legion_money);
	message_manager::getInstance().register_handler(e_msgindex_c2s_legion_donate_item,						c2ws_req_legion_donate_item);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_donate_legion_item,					cs2ws_req_donate_legion_item);
	message_manager::getInstance().register_handler(e_msgindex_c2s_update_legion_announcement,				c2ws_req_update_legion_announcement);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_event_list,					c2ws_req_get_event_list);
	message_manager::getInstance().register_handler(e_msgindex_c2s_kickout_legion_member,					c2ws_req_kick_out_legion_member);
	message_manager::getInstance().register_handler(e_msgindex_c2s_appoint_legion_member_job_title,		c2ws_req_appoint_legion_member_job_title);
	message_manager::getInstance().register_handler(e_msgindex_c2s_enter_legion_boss_map,					c2ws_req_enter_legion_boss_map);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_kill_legion_boss,					cs2ws_req_kill_legion_boss);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_boss_award,					c2ws_req_get_legion_boss_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_banquet_info,						c2ws_req_get_banquet_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_request_hold_banquet,					c2ws_req_request_hold_banquet);
	message_manager::getInstance().register_handler(e_msgindex_c2s_take_part_in_banquet,					c2ws_req_take_part_in_banquet);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_city_war_info,						c2ws_req_get_city_war_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_apply_city_war_bid,						c2ws_req_apply_city_war_bid);
	message_manager::getInstance().register_handler(e_msgindex_c2s_entry_city_war_map,						c2ws_req_enter_city_war_map);
	message_manager::getInstance().register_handler(e_msgindex_c2s_city_war_operate,						c2ws_req_city_war_operate);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_set_city_occupation,					cs2ws_req_set_city_occupation);
	message_manager::getInstance().register_handler(e_msgindex_c2s_set_auto_accept_new_member_setting,		c2ws_req_set_auto_accept_new_legion_member);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_update_legion_member_info_one,		cs2ws_req_update_legion_member_info_one);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_city_master_info,					c2ws_req_get_city_master_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_occupation_daily_award,				c2ws_req_get_occupation_daily_award);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_rank,						c2ws_req_get_legion_rank);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_legion_info,					dp2ws_load_all_legion_info_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_legion_member_info,			dp2ws_load_all_legion_member_info_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_legion_applicant, dp2ws_load_all_legion_applicant_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_legion_city_war_member_info, dp2ws_load_all_legion_city_war_member_info_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_legion_boss_award_get_log,	dp2ws_load_all_legion_boss_award_get_log_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_city_info,						dp2ws_load_city_info_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_war_bid_info,				dp2ws_load_all_city_bid_info_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_legion_event,				dp2ws_load_all_legion_event_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_legion_bonus_info,				dp2ws_load_legion_bonus_info_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_legion_warehouse_info,           dp2ws_load_legion_warehouse_info_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_legion_warehouse_log,            dp2ws_load_legion_warehouse_log_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_cross_city_info_all,				dp2ws_load_legion_cross_city_info_process);
	message_manager::getInstance().register_handler(e_msgindex_c2s_create_bonfire,							c2ws_req_fire_bonfire);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_fire_bonfire_ret,					cs2ws_req_fire_bonfire_ret);
	message_manager::getInstance().register_handler(e_msgindex_c2s_add_fuel_to_bonfire,					c2ws_req_add_fuel_to_fire);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_add_fuel_to_bonfire_ret,				cs2ws_req_add_fuel_to_bonfire_ret);
	message_manager::getInstance().register_handler(e_msgindex_c2s_legion_recruit_in_world,				c2ws_req_send_legion_recruit_in_world);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_legion_rm_recruit_cd_ret,			cs2ws_req_legion_rm_recruit_cd_ret);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_legion_add_glory_glow,				cs2ws_req_add_legion_glory_glow);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_city_war_territory_info,			c2ws_req_get_city_war_terr_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_city_war_rank_info,					c2ws_req_get_city_war_legion_rank_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_set_territory_prior_maintain,			c2ws_req_set_territory_prior_maintain);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_invite_player_to_join_legion,			c2ws_req_invite_player_to_join_legion);挪到CS上判断被邀请的对象是否解锁军团功能
	message_manager::getInstance().register_handler(e_msgindex_c2s_draw_bonfire_daily_award,				c2ws_req_draw_bonfire_daily_reward);
	message_manager::getInstance().register_handler(e_msgindex_c2s_legion_call_operate,					c2ws_req_legion_call_operate);
	message_manager::getInstance().register_handler(e_msgindex_c2s_level_up_legion_skill,					c2ws_req_level_up_legion_skill);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_bonfire_dead,						cs2ws_req_legion_bonfire_dead);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_add_legion_event,					cs2ws_req_add_legion_event);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_get_legion_contribution,			    cs2ws_req_get_legion_contribution);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_add_legion_act_score,			    cs2ws_req_add_legion_act_score);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_settle_legion_act_rank,				cs2ws_req_settle_legion_act_rank);
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_bonfire_map,				c2ws_req_transfer_to_bonfire_map);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_bonus_rewards,						c2ws_req_get_bonus_rewards);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_add_bonus_count,						cs2ws_req_add_bonus_count);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_legion_get_bonus_award_end,			cs2ws_req_get_bonus_award_end);
	message_manager::getInstance().register_handler(e_msgindex_c2s_stimulate_bonus_msg,					c2ws_req_stimulate_bonus);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_answer_question_info,		c2ws_req_get_legion_question_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_change_legion_name,						c2ws_req_change_legion_name);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_confirm_change_legion_name,			cs2ws_req_confirm_change_legion_name);
	message_manager::getInstance().register_handler(e_msgindex_c2s_confirm_change_legion_name,				c2ws_req_confirm_change_legion_name);
	message_manager::getInstance().register_handler(e_msgindex_c2s_join_cross_overload_city_war,			c2ws_req_join_cross_city_war);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_answer_rank_list,				get_legion_answer_rank_list);
	
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_warehouse_item_info,         c2ws_req_get_legion_warehouse_info);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_legion_warehouse_operation,           cs2ws_req_operation_legion_warehouse_item);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_legion_warehouse_log,               c2ws_req_get_legion_warehouse_log);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_invite_to_join_legion,				cs2ws_req_invite_player_to_join_legion);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_apply_to_join_legion,				 cs2ws_req_apply_to_join_legion);

	//store
	message_manager::getInstance().register_handler(e_msgindex_c2s_goods_operate,							c2ws_req_goods_operate);

	// best_record
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_best_record,						c2ws_req_get_best_record);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_set_best_record,						cs2ws_req_set_best_record);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_best_record,					dp2ws_load_all_best_record_process);

	//character_gs
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_sync_char_data,						cs2ws_sync_character_data);

	//arena
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_req_get_arena_rank,					cs2ws_get_arena_rank);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_req_challenge,						cs2ws_req_challenge);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_req_challenge_list,					cs2ws_req_get_choose_list);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_first_three,							cs2ws_req_get_first_three);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_chellenge_over,						cs2ws_chellenge_over_req);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_arena_sync_gs_value,					cs2ws_arena_sync_gs_value_proc);

	//ranking
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_req_worship_player,					cs2ws_worship_player_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_ranking_get_top,							c2ws_req_get_rank_list_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_ranking_get_my_rank,						c2ws_req_get_player_rank_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_sync_ranking_info,						cs2ws_sync_ranking_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_sync_vip_level_data,					cs2ws_sync_ranking_player_vip_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_player_ranking_value,				c2ws_req_get_player_ranking_value_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_service_rank_get_first_player,			c2ws_req_service_rank_get_first_player_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_req_service_rank_info,					cs2ws_req_service_rank_info_proc);
	
	// mail
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_mail,							cs2ws_find_and_send_mail_to_player);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_mail_send_to_all_player,				cs2ws_send_mail_to_all_player_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_check_id_mail,						cs2ws_check_id_mail_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_check_id_mail_end,					dp2ws_check_id_mail_end_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_other_server_mail_record_end, dp2ws_load_other_server_mail_record_end_process);
	message_manager::getInstance().register_handler(e_msg_index_dp2ws_load_other_server_mail_end, dp2ws_load_other_server_mail_end_process);


	// event
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_check_player_globel_mail_event,		cs2ws_check_player_globel_mail_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_mail_event_end,					dp2ws_load_mail_event_end_process);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_mail_event_item_end,				dp2ws_load_mail_event_item_end_process);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_reconnect_send_red_pack_info,			cs2ws_reconnect_send_red_package_info_process);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_red_pack,						cs2ws_send_red_package_process);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_get_red_pack,							cs2ws_get_red_package_process);

	//chat&globel message
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_globel_message,					cs2ws_send_globel_message_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_notice,							cs2ws_send_notice_proc);

	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_chat_to_ws_new,					cs2ws_send_chat_to_ws_process_new);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_chat_end,						cs2ws_send_chat_result);
	message_manager::getInstance().register_handler(e_msgindex_c2s_send_recruit_message,					c2ws_send_recruit_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_notice_with_param,				cs2ws_send_notice_with_param_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_chat_with_offline,				dp2ws_load_chat_with_offline_proc);

	//cross
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_cross_info,						c2ws_req_cross_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_sign_up,							c2ws_req_sign_up);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_role_pk_top,						dp2ws_req_role_pk_top);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_role_pk_info,					dp2ws_req_role_pk_info);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_role_pk_result,						cs2ws_req_role_pk_result);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_war_log,							c2ws_req_role_pk_log);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_role_pk_log,						dp2ws_req_role_pk_log);

	//world_boss
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_world_boss_end,					dp2ws_load_world_boss_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_world_boss_dead,						cs2ws_world_boss_dead_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_create_world_boss_success,				cs2ws_create_world_boss_sucess_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_frist_kill_world_boss_prize,			c2ws_req_frist_kill_world_boss_prize);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_has_get_first_kill_prize,				cs2ws_first_killer_get_prize);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_npc_left_hp_per,						cs2ws_npc_left_hp_per_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_all_gain_treasure_record_info,	dp2ws_load_all_gain_treasure_record_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_npc_damage_list,						cs2ws_update_boss_damage_list_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_world_level,							c2ws_get_world_boss_level);

	//big_player
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_big_player_end,					dp2ws_load_big_player_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_big_player_base_info_end,		dp2ws_load_big_player_base_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_big_player_skill_info_end,		dp2ws_load_big_player_skill_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_big_player_item_info_end,		dp2ws_load_big_player_item_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_big_player_att_info_end,			dp2ws_load_big_player_att_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_big_player_buff_end,				dp2ws_load_big_player_buff_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_create_big_player_success,			cs2ws_create_big_player_sucess_proc);

	//red_package
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_red_pack_receiver_end,			dp2ws_load_red_package_receiver_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_red_pack_end,					dp2ws_load_red_package_end_proc);

	//activatecode
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_welfare_activatecode,				c2ws_get_welfare_activatecode);

	//lucky draw record
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_add_new_draw_record,					cs2ws_add_lucky_draw_record);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_info_from_db_end,				dp2ws_load_lucky_draw_record_end_proc);

	//relation
	message_manager::getInstance().register_handler(e_msgindex_c2s_addfriend,								c2ws_add_friend);
	message_manager::getInstance().register_handler(e_msgindex_c2s_delfriend,								c2ws_del_friend);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_friend_userinfo,					c2ws_find_player_to_add_friend);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_add_friendliness_value,				cs2ws_add_friendliness_value_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_fuben_add_friendliness_value,			cs2ws_fuben_add_friendliness_value_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_syc_nearly_state,					c2ws_req_syc_nearly_state);
	message_manager::getInstance().register_handler(e_msgindex_c2s_operate_other_add_req,					c2ws_req_operate_other_req);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_player_relation,					dp2ws_rep_load_relationlist_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_relation_info,					dp2ws_rep_load_relation_info_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_add_relation,							dp2ws_add_relation_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_relation_info_other_server,		dp2ws_rep_load_relation_info_other_server_end);
	//marry
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_marrage,							c2ws_req_marry_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_marry_info_end,					dp2ws_rep_marry_couple_info_end); 
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_operate_result_end,					cs2ws_check_item_enough_end);
	message_manager::getInstance().register_handler(e_msgindex_c2s_reply_marry_choice,						c2ws_reply_marry_choice_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_divorce,							c2ws_req_divorce_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_marry_get_last_login_time_end,		dp2ws_ret_marry_couple_last_login_time_end);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_marry_get_reward_end,					cs2ws_ret_get_reward_end);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_marry_get_reward,					c2ws_req_get_marry_reward_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_update_wedding_ring_level,			cs2ws_update_wedding_ring_level_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_marry_info_cross_server_end,		dp2ws_load_marry_info_cross_server_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_add_heart_value,						cs2ws_add_heart_value_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_bless_one_couple,					c2s_req_bless_one_couple_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cost_bless_money_result,				cs2ws_cost_bless_money_result_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_lucky_info_to_client,			cs2ws_send_lucky_info_to_client_proc);
	
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_get_marry_mission,						c2s_req_get_marry_task); 
	message_manager::getInstance().register_handler(e_msgindex_c2s_confirm_receive_marry_mission,				c2s_confirm_receive_marry_task);
	//message_manager::getInstance().register_handler(e_msgindex_c2s_req_couple_info,						c2ws_get_marry_couple_info_proc);

	//auction
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_add_auction_into_db,					cs2ws_auction_sell_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_buy,							cs2ws_auction_buy_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_find_buy_target_end,			dp2ws_rep_auction_find_buy_target_end);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_sell_success_info,			cs2ws_auction_puchase_success_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_cancel_sell,					cs2ws_auction_cancel_sell_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_cancel_sell,					dp2ws_rep_auction_cancel_sell_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_sell_end,						dp2ws_rep_auction_sell_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_find_bid_target_end,			dp2ws_rep_auction_find_bid_target_end);

	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_bid,							cs2ws_trade_bid_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_clear_selling_lock,					cs2ws_clear_selling_lock_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_bid_success,							cs2ws_auction_bid_success_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_clear_selling_lock,					dp2ws_auction_clear_selling_lock_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_clear_old_item_end,			dp2ws_auction_clear_item_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_share_sell_reward,			cs2ws_auction_share_sell_reward_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_add_purchase_record,			cs2ws_auction_add_trade_record_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_req_share_list_end,			dp2ws_req_share_list_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_check_legion_selling_end,		dp2ws_auction_check_legion_selling_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_del_bid_record,				cs2ws_auction_del_bid_record_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_check_world_sell_end,			dp2ws_check_world_auction_sell);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_show_sell_end,                 dp2ws_auction_show_sell_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_time_out_end,                  dp2ws_auction_time_out_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_auction_bid_db_sucess,                 db2ws_auction_bid_db_sucess_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_auction_refresh_person,                cs2ws_auction_request_person_refresh_proc);

	//recharge
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_recharge_end,							cs2ws_recharge_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_recharge_end_send_mail,				cs2ws_recharge_end_send_mail_proc);

	//server_time
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_server_attr_val_end,				dp2ws_load_server_attr_val_proc);
	//gm指令专用
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_gm_order,								cs2ws_gm_order_proc);

	//gain_treasure
	message_manager::getInstance().register_handler(e_msgindex_c2s_gain_treasure_req_kill_record,			c2ws_gain_treasure_req_kill_record_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_gain_treasure_req_gain_item_record,		c2ws_gain_treasure_req_gain_item_record_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_update_gain_treasure_boss_info,		cs2ws_update_gain_treasure_boss_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_add_gain_treasure_record_info,		cs2ws_add_gain_treasure_record_info_proc);

	//boss_island
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_update_boss_island_info,				cs2ws_update_boss_island_info_proc);

	//chat
	message_manager::getInstance().register_handler(e_msgindex_c2s_join_voice_channel,						c2ws_join_voice_channel_proc);

	//cloud shop
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_cloud_shop_players_info_end,		dp2ws_rep_cloud_shop_role_record_end);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cloud_shop_req,							c2ws_req_cloud_shop_func);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cloud_shop_buy_resp,					cs2ws_cloud_shop_buy_proc_end);
	
	//time limit activity
	message_manager::getInstance().register_handler(e_msgindex_c2s_time_limit_get_my_rank,					c2ws_time_limit_get_my_rank);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_time_limit_activity_common_data_sync, cs2ws_time_limit_sync_common_data);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_check_time_limit_activity_common_data, cs2ws_check_time_limit_sync_common_data);
	message_manager::getInstance().register_handler(e_msgindex_c2s_time_limit_get_common_data,				c2ws_time_limit_get_common_data);
	
	//rename
	message_manager::getInstance().register_handler(e_msgindex_c2s_change_player_name,						c2ws_req_change_player_name);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_sub_player_rename_item_end,			cs2ws_sub_rename_card_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_check_player_name,					dp2ws_check_player_name_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_change_player_name_end,				dp2ws_change_player_name_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_confirm_change_name,					c2ws_req_confirm_change_name);

	//cross_server_pk
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_update_cross_server_pk_info,			cs2ws_update_cross_server_pk_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_pk_map_is_end,					cs2ws_update_cross_server_pk_map_info_proc);

	//cross_server_harry
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_change_server_money,				 cs2ws_update_server_change_money);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_harry_player_base_info_end,		 dp2ws_load_harry_player_base_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_harry_player_skill_info_end,		 dp2ws_load_harry_player_skill_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_harry_player_item_info_end,		 dp2ws_load_harry_player_item_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_harry_player_att_info_end,		 dp2ws_load_harry_player_att_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_harry_player_buff_end,			 dp2ws_load_harry_player_buff_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_harry_player_belief_end,			dp2ws_load_harry_player_belief_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_create_harry_player,				 cs2ws_create_harry_player);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_harry_notice_to_all_player,		cs2ws_send_harry_notice_to_all_player_func);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_harry_player_fight_att_info_end,	dp2ws_load_harry_player_fight_att_info_end_proc);

	//cross_ladder
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_ladder_req_buy_ticket,			cs2ws_cross_ladder_req_buy_ticket_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_ladder_send_game_result,			cs2ws_cross_ladder_send_game_result_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_ladder_request_role_info,			c2ws_cross_ladder_req_role_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_cross_ladder_request_sign_up,			c2ws_cross_ladder_req_sign_up);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_cross_ladder_load_role_info,			dp2ws_cross_ladder_load_role_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_ladder_req_last_score,			cs2ws_cross_ladder_req_last_score_proc);

	//cross_server_world_boss
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_to_cross_server_world_boss_map,	c2ws_req_transfer_to_cross_server_world_boss_map);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_cross_boss_show_info,					c2ws_get_cross_boss_show_info);

	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_server_kill_world_boss,				cs2ws_cross_server_kill_world_boss_func);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_server_world_boss_creat_boss_begin, cs2ws_cross_server_world_boss_creat_boss_begin_func);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_make_cross_server_world_legion_award,		cs2ws_make_cross_server_world_legion_award_func);	
	//message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_server_world_boss_info_sync,		cs2ws_cross_server_world_boss_info_sync_func);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_boss_map_game_over,					cs2ws_cross_boss_map_game_over_func);
	message_manager::getInstance().register_handler(e_msg_cs2ws_send_kill_boss_notic,							cs2ws_send_kill_boss_notic_func);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_cross_server_world_boss_info,		dp2ws_load_cross_server_world_boss_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_time_limit_info_end,					dp2ws_load_all_ws_time_limit_act_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_time_limit_temp_end,					dp2ws_load_time_limit_temp_end_func);
	message_manager::getInstance().register_handler(e_msgindex_dp2cs_load_time_limit_branch_temp_end,			dp2ws_load_time_limit_branch_temp_end_func);

	
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_sync_oracle_trial_rank,					cs2ws_sync_oracle_trial_rank_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_oracle_trial_rank,						c2ws_get_oracle_trial_rank);

	message_manager::getInstance().register_handler(e_msgindex_cs2ws_sync_person_information,					cs2ws_sync_person_information_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_delete_person_information,			    cs2ws_delete_person_information_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_get_person_couple, cs2ws_get_person_couple_proc);
	
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cross_player_save_end,				cs2ws_cross_player_save_end_proc);

	//legion_station
	message_manager::getInstance().register_handler(e_msgindex_c2s_join_legion_station_map,					c2s_join_legion_station_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_bonfire_create,						cs2ws_create_bonfire_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_bonfire_end,							cs2ws_bonfire_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_legion_boss_create,					cs2ws_legion_boss_create_proc);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_legion_boss_damage_player,			cs2ws_damage_legion_boss_player_arr_proc);

	//assist_fight
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_assist_fight_info, c2s_get_assist_fight_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_req_assist_fight,c2s_req_assist_fight_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_check_can_assist_fight_end, cs2ws_check_can_assist_fight_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_cancel_assist_fight, cs2ws_cancel_assist_fight_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_start_assist_fight, cs2ws_start_assist_fight_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_end_assist_fight, cs2ws_end_assist_fight_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_join_team_assist_fight, cs2ws_join_team_assist_fight);

	message_manager::getInstance().register_handler(e_msgindex_c2s_save_appearance_info_to_id, c2s_save_appearance_info_to_id_proc);
	message_manager::getInstance().register_handler(e_msgindex_c2s_load_appearance_info_by_id, c2s_load_appearance_info_by_id_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_save_appearance_info_to_id_end, dp2ws_save_appearance_info_to_id_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_appearance_info_by_id_end, dp2ws_load_appearance_info_by_id_end_proc);
	
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_create_pk_king_combat_map, cs2ws_create_pk_king_combat_map_func);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_is_pk_king, cs2ws_send_is_pk_king_func);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_break_line, cs2ws_send_break_line_func);
	message_manager::getInstance().register_handler(e_mgsindex_c2s_send_need_break_line_login_end, c2s_send_need_break_line_login_end_proc);

	message_manager::getInstance().register_handler(e_msgindex_dp2ws_element_war_load_role_info_end, dp2ws_element_war_load_role_info_end_proc);
	message_manager::getInstance().register_handler(e_mgsindex_c2s_element_war_send_sign_up, c2ws_element_war_send_sign_up_msg);
	message_manager::getInstance().register_handler(e_mgsindex_c2s_element_war_send_sign_up_team_ready, c2ws_element_war_send_sign_up_team_ready_msg);
	message_manager::getInstance().register_handler(e_mgsindex_c2s_element_war_send_close_sign_up, c2ws_special_element_war_send_close_sign_up_msg);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_element_war_map_game_end_role_info, cs2ws_element_war_map_game_end_role_info_msg);
	message_manager::getInstance().register_handler(e_mgsindex_c2s_element_war_get_mission_reward, c2ws_element_war_get_mission_reward_msg);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_element_war_buy_ticket, cs2ws_element_war_element_war_buy_ticket_msg);
	message_manager::getInstance().register_handler(e_mgsindex_c2s_element_war_get_role_info, c2ws_element_war_get_role_info_msg);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_element_ladder_req_last_score, cs2ws_element_ladder_req_last_score_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_element_war_gm_send_add_ticket, cs2ws_element_war_gm_send_add_ticket_proc);
	
	message_manager::getInstance().register_handler(e_msgindex_c2s_request_legion_strategy_world_boss_info, c2ws_get_strategy_world_boss_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_request_world_boss_info, c2ws_get_cur_world_boss_info);

	message_manager::getInstance().register_handler(e_msg_index_cs_reload_csv_end, cs2ws_reload_csv_end);

	//attack_city
	message_manager::getInstance().register_handler(e_msgindex_c2s_transfer_attack_city_map, c2ws_transfer_attack_city_map);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_attack_city_info, c2ws_get_attack_city_info);
	message_manager::getInstance().register_handler(e_msgindex_c2s_get_attack_city_rank_list, c2ws_get_attack_city_rank_list);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_attack_city_load_legion_info_end, dp2ws_attack_city_load_legion_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_attack_city_load_rank_info_end, dp2ws_attack_city_load_rank_info_end_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_attack_city_legion_reward_info, cs2ws_send_attack_city_legion_reward_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_attack_city_legion_end_info, send_attack_city_legion_end_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_attack_city_rank_end_info, send_attack_city_rank_end_info_proc);
	message_manager::getInstance().register_handler(e_msgindex_cs2ws_send_attack_city_legion_call, cs2ws_send_attack_city_legion_call_proc);
	

	return true;
}
}
