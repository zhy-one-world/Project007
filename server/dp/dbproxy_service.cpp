/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   20:59
	file base:	dbproxy_service
	file ext:	cpp
	author:		zhy

	purpose:
*********************************************************************/
#include "app/app_server.hpp"
#include "connection/daemon_client.hpp"
#include "data_manager/data_manager.hpp"
#include "db_manager.hpp"
#include "dbproxy_service.hpp"
#include "dbproxy_service.hpp"
#include "dp_client.hpp"
#include "eye_proto.pb.h"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "Logic/time_def.hpp"
#include "msgproc/cs/msgproc_achievement.hpp"
#include "msgproc/cs/msgproc_arena.hpp"
#include "msgproc/cs/msgproc_att.hpp"
#include "msgproc/cs/msgproc_auction.hpp"
#include "msgproc/cs/msgproc_boss_island.hpp"
#include "msgproc/cs/msgproc_buff.hpp"
#include "msgproc/cs/msgproc_chat.hpp"
#include "msgproc/cs/msgproc_convert.hpp"
#include "msgproc/cs/msgproc_cs.hpp"
#include "msgproc/cs/msgproc_cs_time_limit_activity.hpp"
#include "msgproc/cs/msgproc_demons_tower.hpp"
#include "msgproc/cs/msgproc_element_heart.hpp"
#include "msgproc/cs/msgproc_first_time_do.hpp"
#include "msgproc/cs/msgproc_gain_treasure.hpp"
#include "msgproc/cs/msgproc_goods.hpp"
#include "msgproc/cs/msgproc_guide.hpp"
#include "msgproc/cs/msgproc_highest_record.hpp"
#include "msgproc/cs/msgproc_item.hpp"
#include "msgproc/cs/msgproc_mail.hpp"
#include "msgproc/cs/msgproc_map.hpp"
#include "msgproc/cs/msgproc_mission.hpp"
#include "msgproc/cs/msgproc_money.hpp"
#include "msgproc/cs/msgproc_must_do.hpp"
#include "msgproc/cs/msgproc_oracle_trial.hpp"
#include "msgproc/cs/msgproc_phantom.hpp"
#include "msgproc/cs/msgproc_pokedex.hpp"
#include "msgproc/cs/msgproc_recycle.h"
#include "msgproc/cs/msgproc_role_harry_info.hpp"
#include "msgproc/cs/msgproc_service_goal.hpp"
#include "msgproc/cs/msgproc_service_rank.hpp"
#include "msgproc/cs/msgproc_seven_day_goal.hpp"
#include "msgproc/cs/msgproc_show.hpp"
#include "msgproc/cs/msgproc_skill.hpp"
#include "msgproc/cs/msgproc_special_name.hpp"
#include "msgproc/cs/msgproc_talent.hpp"
#include "msgproc/cs/msgproc_title.hpp"
#include "msgproc/cs/msgproc_treasure.hpp"
#include "msgproc/cs/msgproc_welfare.hpp"
#include "msgproc/cs/msgproc_worship_target.hpp"
#include "msgproc/log/log_cache.hpp"
#include "msgproc/log/msgproc_server_log.hpp"
#include "msgproc/ls/msgproc_ls.hpp"
#include "msgproc/ws/msgproc_appearance_info.h"
#include "msgproc/ws/msgproc_attack_city_ws.hpp"
#include "msgproc/ws/msgproc_auction_ws.hpp"
#include "msgproc/ws/msgproc_best_record_ws.hpp"
#include "msgproc/ws/msgproc_big_player_ws.hpp"
#include "msgproc/ws/msgproc_chat_ws.hpp"
#include "msgproc/ws/msgproc_cloud_shop.hpp"
#include "msgproc/ws/msgproc_cross_ladder_info_ws.hpp"
#include "msgproc/ws/msgproc_cross_server_harry_ws.hpp"
#include "msgproc/ws/msgproc_cross_server_pk_ws.hpp"
#include "msgproc/ws/msgproc_cross_server_world_boss_ws.hpp"
#include "msgproc/ws/msgproc_element_war_info_ws.hpp"
#include "msgproc/ws/msgproc_legion_ws.hpp"
#include "msgproc/ws/msgproc_lucky_draw_record_ws.hpp"
#include "msgproc/ws/msgproc_mail_ws.hpp"
#include "msgproc/ws/msgproc_marry.hpp"
#include "msgproc/ws/msgproc_ranking_ws.hpp"
#include "msgproc/ws/msgproc_red_package_ws.hpp"
#include "msgproc/ws/msgproc_relation.hpp"
#include "msgproc/ws/msgproc_role_pk_ws.hpp"
#include "msgproc/ws/msgproc_world_boss_ws.hpp"
#include "msgproc/ws/msgproc_ws.hpp"
#include "net.pb.h"
#include "net/message_manager.hpp"
#include "time.hpp"
#include "tracy/Tracy.hpp"
#include "utility/guid_gen.h"
#include "utility/parse_msg.h"
#include "utility/serialize_msg.h"
#include <achievement_msg.hpp>
#include <activity_msg.hpp>
#include <appearance_info_msg.hpp>
#include <arena_msg.hpp>
#include <arena_msg.hpp>
#include <att_msg.hpp>
#include <attack_city_msg.hpp>
#include <auction_msg.hpp>
#include <big_player_msg.hpp>
#include <boss_island_msg.hpp>
#include <buff_msg.hpp>
#include <char_msg.hpp>
#include <chat_msg.hpp>
#include <cloud_shop_msg.hpp>
#include <convert_msg.hpp>
#include <core.hpp>
#include <cross_ladder_msg.hpp>
#include <cross_server_harry_msg.hpp>
#include <cross_server_pk_msg.hpp>
#include <cross_server_world_boss_msg.hpp>
#include <element_heart_msg.hpp>
#include <event_msg.hpp>
#include <event_msg.hpp>
#include <event_msg.hpp>
#include <first_time_do_msg.hpp>
#include <gain_treasure_msg.hpp>
#include <gain_treasure_msg.hpp>
#include <gate_msg.hpp>
#include <goods_msg.hpp>
#include <guide_msg.hpp>
#include <internal.hpp>
#include <item_msg.hpp>
#include <legion_msg.hpp>
#include <login_msg.hpp>
#include <login_msg.hpp>
#include <lucky_draw_record_msg.hpp>
#include <mail_msg.hpp>
#include <mail_msg.hpp>
#include <map_msg.hpp>
#include <marry_msg.hpp>
#include <mission_msg.hpp>
#include <net/scheduler.hpp>
#include <oracle_trial_msg.hpp>
#include <phantom_msg.hpp>
#include <pokedex_msg.hpp>
#include <ranking_msg.hpp>
#include <ranking_msg.hpp>
#include <recycle_msg.h>
#include <role_pk_msg.hpp>
#include <server_log_msg.hpp>
#include <service_goal_msg.hpp>
#include <service_rank_msg.hpp>
#include <seven_day_goal_msg.hpp>
#include <skill_msg.hpp>
#include <special_name_msg.hpp>
#include <spirit_msg.hpp>
#include <talent_msg.hpp>
#include <time_limit_activity_msg.hpp>
#include <title_msg.hpp>
#include <treasure_msg.hpp>
#include <welfare_msg.hpp>
#include <world_boss_msg.hpp>

namespace hld
{
	using namespace hld::net;

	dbproxy_service::dbproxy_service()
	{
		//	rewrite these 2 value in derived class for different service
		m_ipaddr = DPCONFIG->internal_host;
		m_port = DPCONFIG->internal_port;
		m_server_id = SERVERCONFIG->game_id;
		guid_gen::set_server_id(e_server_type_dp * server_uid_level_num + m_server_id);

		message_manager::getInstance().register_handler(e_msg_server2gate_send_to_server, boost::bind(&dp_client::internal_gate_msg, &dp_client::getInstance(), _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_gate2gate_new, boost::bind(&dp_client::gate2ws_rep_gate_new, &dp_client::getInstance(), _1, _2, _3));
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_open_time, gm_set_open_time);
		message_manager::getInstance().register_handler(e_msg_index_reload_csv, gm_reload_csv);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_set_cross_time, gm_set_cross_open_time);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_change_password, gm_set_new_password);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_begin_cross, gm_set_begin_cross);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_server_list, save_server_last_server_list);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_server_list, load_server_last_server_list);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_set_role_right, gm_set_role_right);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_recharge, save_recharge);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_recharge_end, save_recharge_end);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_change_name, gm_change_account);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_del_cache, gm_del_role_cache);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_del_role, gm_del_role);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_stop_login, gm_stop_login);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_character, get_char_info_ws);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_get_role_info, gm_get_role_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_modify_role_info, gm_modify_role_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_gm_get_role_items_info, gm_get_role_items_info);


		//-------------------------- Relation ---------------------------//
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_player_relation, ws2dp_req_save_player_relation);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_relation_info, ws2dp_req_load_offline_other_relation);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_relation_info_other_server, ws2dp_req_load_offline_other_relation_other_server);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_req_load_relation_list, ws2dp_req_load_player_relation);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_add_relation, ws2dp_req_add_relation);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_del_relation, ws2dp_req_del_relation);
		message_manager::getInstance().register_handler(hld::e_msg_index_ws2dp_add_mail_to_sql, check_player_is_exist);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_check_id_mail, check_id_mail_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_arena_rank, save_arena_rank);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_arena_rank_change, save_arena_rank_first);
		//message_manager::getInstance().register_handler(hld::e_mgsindex_ws2dp_msg_test, load_arena_rank);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_other_server_mail_record, save_other_server_mail_record);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_load_other_server_mail, load_other_server_mail);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_del_other_server_mail, del_other_server_mail);
		//ranking
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_load_all_ranking_info, load_all_ranking_info);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_ranking_info_one, save_ranking_info_one);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_one_total_ranking_info, save_one_ranking_list);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_one_service_rank_info, save_one_service_rank_list);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_load_all_service_rank_info, load_all_service_rank_info);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_ranking_worship, save_ranking_worship);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_get_worship_value, load_player_worship_value);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_add_ranking_worship, add_robot_worship);
		// legion
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_legion_info, save_legion_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_all_legion_info, load_all_legion_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_legion_member_info, save_legion_member_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_legion_city_war_member_info, save_legion_city_war_member_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_delete_legion_info, delete_legion_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_delete_legion_member_info, delete_legion_member_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_delete_legion_city_war_member_info, delete_legion_city_war_member_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_del_legion_applicant_one, delete_legion_applicant);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_legion_applicant, save_legion_applicant);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_legion_boss_award_get_log, save_legion_boss_award_get_log);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_all_legion_boss_award_get_log, load_all_legion_boss_award_get_log);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_store_legion_warehouse, store_legion_warehouse);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_store_legion_warehouse_log, store_legion_warehouse_log);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_role_get_legion_warehouse, role_get_legion_warehouse);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_destroy_legion_warehouse_item, destroy_legion_warehouse_item);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_legion_warehouse, clear_legion_warehouse);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_city_info, save_city_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_war_bid_info, save_war_bid_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_war_bid_info, clear_war_bid_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_overlord_legion_stuff, clear_city_war_reward_stuff);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_legion_event, save_legion_event);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_legion_event, clear_legion_event);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_legion_bonus_info, save_legion_bonus_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_legion_bonus_info, clear_legion_bonus_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_server_attr_val, save_server_attr_val);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_server_attr_val, load_server_attr_val);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_cross_city_info, load_cross_city_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_cross_city_info, save_cross_city_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_cross_city_info, clear_cross_city_info);
		// activity
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_best_record, save_best_record);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_all_best_record, load_all_best_record);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_gain_treasure_record_info, save_gain_treasure_record_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_all_gain_treasure_record_info, load_all_gain_treasure_record_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_cross_server_pk_info, save_cross_server_pk_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_cross_server_pk_info, load_cross_server_pk_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_cross_server_pk_info, clear_cross_server_pk_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_cross_server_harry_info, save_cross_server_harry_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_cross_server_harry_info, load_cross_server_harry_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_send_del_one_server_msg, del_one_server_msg_func);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_harry_player_detail, load_harry_player_detail);
		//role_pk
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_role_pk_top, load_role_pk_top);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_role_pk_info, load_role_pk_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_role_pk_info, save_role_pk_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_role_pk_honor_refresh, save_role_pk_honor_refresh);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_role_pk_log, load_role_pk_log);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_mail_event, save_globel_mail_to_sql);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_red_pack, save_red_package_to_sql);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_red_pack, load_red_package_by_sql);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_red_pack_receiver, load_red_package_receiver_by_sql);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_add_red_pack_to_sql, add_red_package_to_sql_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_add_red_pack_receiver_to_sql, add_red_package_receiver_to_sql_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_del_red_pack_by_sql, del_red_package_by_sql_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_mail_event_item, save_globel_mail_item_to_sql);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_mail_event, load_mail_event);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_load_mail_event_item, load_mail_event_item);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_new_globel_mail_had_info, save_globel_mail_new_get_info_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_world_boss, save_world_boss_to_sql);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_load_world_boss, load_world_boss);

		message_manager::getInstance().register_handler(e_msgindex_ws2dp_del_draw_info_to_db, del_draw_info_from_db);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_add_draw_info_to_db, add_draw_info_to_db);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_info_from_db, load_draw_info_by_db);

		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_save_big_player, save_big_player_to_sql);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_load_big_player, load_big_player);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_load_big_player_detail, load_big_player_detail);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_add_auction_into_db, auction_add_item_into_db_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_find_buy_target, auction_find_buy_target_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_cancel_sell, auction_find_cancel_target_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_find_bid_target, auction_find_bid_target_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_bid_success, auction_bid_success_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_clear_old_item, auction_get_and_clear_old_item_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_add_share_list, auction_add_share_list_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_add_purchase_record, auction_add_trade_record_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_req_share_list, auction_req_share_list_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_check_legion_selling, auction_check_legion_selling_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_del_bid_record, auction_del_bid_record_proc);
		message_manager::getInstance().register_handler(hld::e_msgindex_ws2dp_auction_clear_old_bid_record, auction_clear_old_bid_record_proc);

		message_manager::getInstance().register_handler(e_msgindex_ws2dp_auction_show_sell, auction_update_sell_show_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_auction_time_out, auction_time_out_proc);

		//--------------------------Role Marry---------------------------//
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_marry_info, ws2dp_req_save_marry_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_marry_info, ws2dp_req_load_marry_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_marry_get_last_login_time, ws2dp_req_load_couple_login_time);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_marry_info_cross_server, ws2dp_load_marry_info_cross_server_proc);
		//cloud shop
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_cloud_shop_players_info, ws2dp_req_load_cloud_role_record);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_cloud_shop_players_info, ws2dp_req_save_cloud_role_record);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_clear_cloud_shop_player_info, ws2dp_req_clear_cloud_shop_player_info);
		//rename
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_check_player_name, ws2dp_req_check_player_name);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_change_player_name, ws2dp_req_change_player_name);
		//chat
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_chat_with_offline, save_chat_with_offline);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_chat_with_offline, load_chat_with_offline);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_eliminate_chat_with_offline, eliminate_chat_with_offline);

		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_time_limit_temp, ws2dp_req_load_time_limit_temp_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_time_limit_temp, ws2dp_req_save_time_limit_temp_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_time_limit_branch_temp, ws2dp_req_save_time_limit_branch_temp_info);

		message_manager::getInstance().register_handler(e_msgindex_ws2dp_auction_check_world_sell, ws2dp_req_check_world_auction_sell);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_update_role_time_limit, ws2dp_req_update_role_time_limit_info_func);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_delete_time_limit_temp, ws2dp_req_update_delete_time_limit_temp);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_delete_time_limit_branch_temp, ws2dp_req_update_delete_time_limit_branch_temp);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_refresh_role_time_limit_guid, ws2dp_req_refresh_role_time_limit_guid);

		message_manager::getInstance().register_handler(e_msgindex_ws2dp_cross_ladder_load_info, ws2dp_cross_ladder_load_info_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_cross_ladder_add_role_info, ws2dp_cross_ladder_add_role_info_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_cross_ladder_update_info, ws2dp_cross_ladder_update_info_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_cross_ladder_update_ticket_info, ws2dp_cross_ladder_update_ticket_info_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_cross_ladder_update_role_info, ws2dp_cross_ladder_update_role_info_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_cross_ladder_refresh_ticket_info, ws2dp_cross_ladder_update_refresh_ticket_info_proc);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_cross_ladder_refresh_season_info, ws2dp_cross_ladder_update_refresh_season_info_proc);

		//cross_server_world_boss
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_cross_server_world_boss_info, save_cross_server_world_boss_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_cross_server_world_boss_info, load_cross_server_world_boss_ws_info);

		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_time_limit_info, load_ws_time_limit_func);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_time_limit_info, save_ws_time_limit_func);

		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_appearance_info_to_id, save_appearance_info_to_id);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_load_appearance_info_by_id, load_appearance_info_by_id);

		//save account online time
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_save_account_online_time, save_account_online_time);

		//元素争霸
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_add_role_info, save_element_war_add_role_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_update_info, save_element_war_update_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_refresh_season_info, save_element_war_refresh_season_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_update_role_info, save_element_war_update_role_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_load_info, Load_element_war_load_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_refresh_ticket_info, save_element_war_refresh_ticket_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_clear_mission_info, save_element_war_refresh_clear_mission_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_element_war_refresh_mission_info, save_element_war_refresh_mission_info);

		//攻城战
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_attack_city_save_legion_info, save_attack_city_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_attack_city_save_rank_info, save_attack_city_rank_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_attack_city_load_legion_info, load_attack_city_ws_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_attack_city_load_rank_info, load_attack_city_rank_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_attack_city_clear_legion_info, clear_attack_city_legion_info);
		message_manager::getInstance().register_handler(e_msgindex_ws2dp_attack_city_clear_rank_info, clear_attack_city_rank_info);

		//////////////////////////////////////////////////////////////ZymEnd/////////////////////////////////////////////////////////////////////////
		//--------------------------System Mail---------------------------//
		// message_manager::getInstance().register_handler(e_msg_index_ws2dp_system_mail_newmail,ws2dp_system_mail_new_mail_func);
		//gm群发邮件
		// message_manager::getInstance().register_handler(e_msg_index_ws2dp_system_all_mail,ws2dp_system_mail_all_mail);
		//----------------------------------------------------------------//

		message_manager::getInstance().register_handler(e_msg_index_req_login, boost::bind(&dbproxy_service::on_req_login, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_index_req_stop, boost::bind(&dbproxy_service::on_req_stop, this, _1, _2, _3));
		message_manager::getInstance().register_handler(e_msg_index_ls2dp_client_login, ls2dp_client_login_func);
		message_manager::getInstance().register_handler(e_msgindex_ls2dp_create_character, ls2dp_req_createchar_func);
		message_manager::getInstance().register_handler(e_msgindex_ls2dp_delete_character, ls2dp_req_deletechar_func);
		message_manager::getInstance().register_handler(e_msgindex_ls2dp_enum_character, ls2dp_req_enum_char);
		//message_manager::getInstance().register_handler(e_msg_index_ls2dp_pay_order_save,ls2dp_pay_save_func);
		//message_manager::getInstance().register_handler(msgindex_req_query_log_db,
		message_manager::getInstance().register_handler(e_msg_index_cs2dp_load_data_online, cs2dp_req_char_online_loading_data);
		message_manager::getInstance().register_handler(e_msg_index_cs2dp_load_arena_player_data, cs2dp_req_loading_arena_player_data);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_character, cs2dp_req_save_char_info_cs);
		message_manager::getInstance().register_handler(e_msgindex_dp2cs_save_char_data, cs2dp_req_save_char_data);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_yesterday_must_do, cs2dp_req_save_yesterday_must_do_remain);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_daily_must_do_count, cs2dp_req_save_daily_must_do_count);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_buff, cs2dp_req_save_char_buff);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_skill, cs2dp_req_save_char_skill);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_legion_skill, cs2dp_req_save_char_legion_skill);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_item, cs2dp_req_save_char_item);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_show, cs2dp_req_save_char_show);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_money, cs2dp_req_save_char_money);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_pokedexs, cs2dp_req_save_char_pokedex);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_tinder, cs2dp_req_save_char_tinder);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_worship_target, cs2dp_req_save_char_worship_target);
		//message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_ranking_info,cs2dp_req_save_role_ranking);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_mission, cs2dp_req_save_char_mission);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_att, cs2dp_req_save_char_att);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_goods, cs2dp_req_save_char_goods);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_map, cs2dp_req_save_char_map_record);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_demons_tower, cs2dp_req_save_char_demons_tower);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_element_heart, cs2dp_req_save_char_element_heart);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_special_name, cs2dp_req_save_char_special_name);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_convert, cs2dp_req_save_char_convert);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_achievement, cs2dp_req_save_char_achievement);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_add_arena_log, cs2dp_req_add_arena_log);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_load_arena_log, cs2dp_req_load_arena_log);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_del_char_mail, cs2dp_req_del_char_mail);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_del_char_mail_item, cs2dp_req_del_char_mail_item);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_auto_load_mail_by_db, cs2dp_req_auto_load_character_mail);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_add_char_mail, cs2dp_req_add_char_mail);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_welfare, cs2dp_req_save_char_welfare);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_active_degree, cs2dp_req_save_char_active_degree);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_welfare_random_get, cs2dp_req_save_char_welfare_random_get);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_title, cs2dp_req_save_char_title);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_phantom, cs2dp_req_save_char_phantom);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_chat_record, cs2dp_req_save_chat_record);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_recycle, cs2dp_req_save_char_recycle);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_recycle_task, cs2dp_req_save_char_recycle_task);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_recycle_get_inviter_info, cs2dp_req_get_inviter_player_info);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_recycle_inviter_add, cs2dp_req_save_inviter_player_add);
		//get other player info
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_get_other_player_info, cs2dp_req_get_other_player_info);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_guide_record, cs2dp_req_save_char_guide_record);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_first_time_do_record, cs2dp_req_save_char_first_time_do);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_talent, cs2dp_req_save_char_talent);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_treasure, cs2dp_req_save_char_treasure);
		//message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_player_relation,cs2dp_req_save_player_relation);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_hightest_record, cs2dp_req_save_player_highest_record);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_service_goal, cs2dp_req_save_char_service_goal_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_auction_req_goods_list, cs2dp_auction_req_auction_list_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_auction_req_trade_record, cs2dp_auction_req_trade_record_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_auction_req_another_sell_info, cs2dp_auction_req_another_sell_info_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_auction_req_self_bid_list, cs2dp_auction_req_self_bid_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_req_bid_record_failure, cs2dp_auction_req_bid_record_failure_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_req_bid_notice, cs2dp_auction_req_bid_notice_proc);

		message_manager::getInstance().register_handler(e_msgindex_server2dp_save_log, server2dp_req_save_log_info);
		message_manager::getInstance().register_handler(e_msgindex_server2dp_save_chat_log, server2dp_req_save_chat_log_info);
		message_manager::getInstance().register_handler(e_msgindex_server2dp_save_legion_log, server2dp_req_save_legion_log);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_service_rank_reward_state, cs2dp_req_save_service_rank_reward_state);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_gain_treasure_info, cs2dp_req_save_gain_treasure_info);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_time_limit_activity_info, cs2dp_req_save_char_time_limit_activity_info);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_boss_island_info, cs2dp_req_save_boss_island_info);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_char_seven_day_goal, cs2dp_req_save_char_seven_day_goal_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_role_harry_info, cs2dp_req_save_role_harry_info);

		//oracle_trial
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_oracle_trial_info, cs2dp_req_save_oracle_trial_info);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_person_information_to_db, cs2dp_save_person_information_to_db_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_get_other_person_information, cs2dp_get_other_person_information_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_role_competition_to_db, cs2dp_save_role_competition_to_db_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_role_dragontrip_to_db, cs2dp_save_role_dragontrip_to_db_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_role_skytreasure_to_db, cs2dp_save_role_skytreasure_to_db_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_role_starark_to_db, cs2dp_save_role_starark_to_db_proc);


		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_time_feed_back_to_db, cs2dp_save_role_time_feed_back_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_time_limit_gift_to_db, cs2dp_save_role_time_limit_gift_proc);
		message_manager::getInstance().register_handler(e_msgindex_cs2dp_save_subscribe_daily_info_to_db, cs2dp_save_subscribe_daily_info_proc);
	}

	dbproxy_service::~dbproxy_service()
	{
	}
	bool dbproxy_service::start()
	{
		if (false == net_server_mgr::getInstance().init(e_server_type_dp, m_ipaddr, m_port,
			DP_SERVER_SEND_BUFF_SIZE, DP_SERVER_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, DP_NEED_SERVER_COUNT, DP_NEED_SERVER_COUNT,
			boost::bind(&dbproxy_service::on_conn_closed, this, _1)))
		{
			CONSOLE_INFO("dbproxy_service init error");
			return false;
		}
		if (false == net_server_mgr::getInstance().start())
		{
			CONSOLE_INFO("dbproxy_service start error");
			return false;
		}
		m_timerindex_gameloop = scheduler::getInstance().add_timer(dp_interval_serverloop, boost::bind(&dbproxy_service::server_loop, this, _1));
		return true;
	}

	void dbproxy_service::stop()
	{
		//移出加入scheduler中的定时器
		net_server_mgr::getInstance().stop();
	}

	void dbproxy_service::server_loop(uint32 timer_index)
	{
		ZoneScoped;
		static uint64 game_time = utility::get_tick_count();
		uint64 time_now = utility::get_tick_count();
		static uint32 cnt = 0;
		if (time_now > game_time)
		{
			game_time = time_now + server_console_time;

			CONSOLE_INFO("==========dp server status==========");
			int32 ws_num = net_server_mgr::getInstance().get_server_count(e_server_type_ws);
			int32 cs_num = net_server_mgr::getInstance().get_server_count(e_server_type_cs);
			int32 gate_num = net_client_mgr::getInstance().get_server_count(e_server_type_gate);

			CONSOLE_INFO("data_num:{}/{}", data_manager::get_instance().get_data_set_num(), max_save_data_count);
			CONSOLE_INFO("player_guid_num: {} ", data_manager::get_instance().get_data_server_num());
			CONSOLE_INFO("account_num: {} ", data_manager::get_instance().get_data_account_num());
			CONSOLE_INFO("enum_char_num: {}", data_manager::get_instance().get_enum_char_num());
			CONSOLE_INFO("unit_info_arr_map_num: {}", data_manager::get_instance().get_unit_info_arr_map_num());
			CONSOLE_INFO("ws {}/{} cs {}/{} gate {}/{}", ws_num, SERVER_WS_COUNT, cs_num, SERVER_CS_COUNT, gate_num, SERVER_GATE_COUNT);
			db_manager::getInstance().ping_db();
			dp_client::getInstance().tick(time_now);
			if (cs_num <= 0 && daemon_client::getInstance().get_server_close())
			{
				CONSOLE_INFO(" daemon close dp");
				app_server::getInstance().stop();
			}
		}
		daemon_client::getInstance().heart_tick(time_now);
		http_access_mgr::get_instance().tick(time_now);
	}
	void dbproxy_service::on_conn_closed(const net_server* faith_server_ptr)
	{
		if (faith_server_ptr)
		{
			std::cout << __FUNCTION__ << " connindex = " << faith_server_ptr->get_conn_index() << std::endl;
		}
	}
	void dbproxy_service::on_req_login(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		//存下这台服务器的信息
		const hld::req_login* pdata = static_cast<const hld::req_login*>(data_ptr);
		if (NULL == pdata)
		{
			return;
		}
		net_server* faith_server_ptr = net_server_mgr::getInstance().get_peer_by_conn_index(connindex);
		if (nullptr == faith_server_ptr)
		{
			return;
		}
		faith_server_ptr->set_server_info(pdata->server_info);
		std::cout << pdata->server_info.ip_addr << " " << pdata->server_info.port << " " << pdata->server_info.server_type << std::endl;
		if (pdata->server_info.server_type == e_server_type_ws)
		{
			get_game_info_ws(connindex, data_ptr, data_len);
			dp_client::getInstance().start();
		}
		if (pdata->server_info.server_type == e_server_type_cs)
		{
			send_to_cs_dp_connect_success(connindex, data_ptr, data_len);
		}
	}
	void dbproxy_service::on_req_stop(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const hld::req_stop* msg = static_cast<const hld::req_stop*>(data_ptr);
		if (NULL == msg)
		{
			return;
		}
		if (msg->server_type != e_server_type_dp)
		{
			return;
		}
		daemon_client::getInstance().set_server_close(true);
		CONSOLE_INFO("FaithEye Stop Game!");
	}

	void dbproxy_service::send_message_lua(uint32 connindex, const char* msg, int32 msg_len, uint32 header)
	{
		if (msg_len > INTERNAL_SERVER_MAX_PACKET_SIZE || header <= 0)
		{
			CONSOLE_INFO("header:{} len:{}", header, msg_len);
			return;
		}
		m_dp2s_msg.wheader = header;
		memcpy(m_dp2s_msg.google_data, msg, msg_len);
		m_dp2s_msg.google_data_len = msg_len;
		send_message(connindex, (void*)&m_dp2s_msg, m_dp2s_msg.get_packet_len());
	}

	void dbproxy_service::send_message(uint32 connindex, google::protobuf::Message* proto_ptr, uint32 header)
	{
		serialize_msg::get_instance().set_serialize_msg_new(m_dp2s_msg, proto_ptr, header);
		send_message(connindex, (void*)&m_dp2s_msg, m_dp2s_msg.get_packet_len());
	}

	void dbproxy_service::send_message(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		if (connindex >= DP_NEED_SERVER_COUNT)
		{
			dp_client::getInstance().send_to_gate(data_ptr, data_len, connindex);
		}
		else
		{
			net_server_mgr::getInstance().send_message(data_ptr, data_len, connindex);
		}
	}
	void dbproxy_service::send_message_lua(uint32 connindex, google::protobuf::Message* net_pro, uint32 header)
	{
		packet_s2s* data = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
		if (nullptr == data)
		{
			return;
		}
		send_message(connindex, data, data->get_packet_len());
	}
	xstring dbproxy_service::get_ipaddr()
	{
		return m_ipaddr;
	}

	uint32 dbproxy_service::get_port()
	{
		return m_port;
	}
	bool dbproxy_service::init_db_conn()
	{
		auto db_game_config = DPCONFIG->m_db_info_map[0];
		auto db_log_config = DPCONFIG->m_db_info_map[1];
		s_db_conn_info db_conn_info;
		std::string game_db_name = db_manager::getInstance().get_game_db_name();
		memcpy(db_conn_info.game_name, game_db_name.c_str(), game_db_name.size());
		memcpy(db_conn_info.game_ip, db_game_config.ip_addr.c_str(), db_game_config.ip_addr.size());
		db_conn_info.game_port = db_game_config.ip_port;
		memcpy(db_conn_info.game_user_name, db_game_config.user_name.c_str(), db_game_config.user_name.size());
		memcpy(db_conn_info.game_user_password, db_game_config.user_password.c_str(), db_game_config.user_password.size());

		memcpy(db_conn_info.log_name, db_log_config.schema_name.c_str(), db_log_config.schema_name.size());
		memcpy(db_conn_info.log_ip, db_log_config.ip_addr.c_str(), db_log_config.ip_addr.size());
		db_conn_info.log_port = db_log_config.ip_port;
		memcpy(db_conn_info.log_user_name, db_log_config.user_name.c_str(), db_log_config.user_name.size());
		memcpy(db_conn_info.log_user_password, db_log_config.user_password.c_str(), db_log_config.user_password.size());
		if (db_manager::getInstance().add_db_link(db_conn_info) == false)
		{
			return false;
		}
		log_cache::getInstance().init();
		return true;
	}
	void dbproxy_service::handler_daemon_onrecv(const void* data_ptr, size_t data_len)
	{
		int32 msg_herder = parse_msg::getInstance().get_packet_header(data_ptr, data_len);

		switch (msg_herder)
		{
		case e_msgindex_d2s_close_process:
		{
			close_server(data_ptr, data_len);
		}
		break;
		default:
			break;
		}
	}

	void dbproxy_service::close_server(const void* data_ptr, size_t data_len)
	{
		eye_proto_d2s_close_process request;
		if (!parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len))
		{
			return;
		}
		if (request.server_id() != SERVERCONFIG->game_id
			|| request.server_type() != e_server_type_dp)
		{
			return;
		}
		daemon_client::getInstance().set_server_close(true);
		CONSOLE_INFO("FaithEye Stop Game!");
	}
}
