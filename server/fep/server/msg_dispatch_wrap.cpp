/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   14:30
	file base:	msg_dispatch_wrap
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "msg_dispatch_wrap.hpp"
#include <base.hpp>
#include <internal.hpp>
#include <login_msg.hpp>
#include <char_msg.hpp>
#include <core.hpp>
#include "../msgproc/msgproc_client.hpp"
#include "../msgproc/msgproc_ws.hpp"
#include "../msgproc/msgproc_ls.hpp"
#include "../msgproc/msgproc_cs.hpp"
#include "net/message_manager.hpp"
#include "proxy_service_cli.hpp"
#include "fep_client.hpp"
#include "security_communication_layer.hpp"
#include <Utility/parse_msg.h>
#include <time_limit_activity_msg.hpp>
#include "net.pb.h"

namespace faith
{
	void transfer_c2ws(uint32 array_index, const void* data_ptr, size_t data_len)
	{
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}

		fep_client::getInstance().send_message_to_ws( data_ptr, data_len);
	}

	void transfer_c2cs(uint32 array_index, const void* data_ptr, size_t data_len)
	{
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(array_index);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false)
		{
			return;
		}
		packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data_ptr;
		if (NULL == package_ptr)
		{
			return;
		}
		package_ptr->client_uid.fepsession_uid = client_session_ptr->get_cs_array_index();
		fep_client::getInstance().send_message_to_cs(data_ptr, data_len, client_session_ptr->get_cs_uid());
	}

	void transfer_s2c(uint32 connindex, const void *data_ptr,size_t data_len)
	{
		s_client_uid client_uid = parse_msg::getInstance().get_packet_connect_idx(data_ptr, data_len);
		client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_id(client_uid.fepsession_uid);
		if (nullptr == client_session_ptr || client_session_ptr->is_vaild() == false || client_session_ptr->get_client_uid() != client_uid)
		{
			return;
		}

		security_communication_layer::getInstance().send_to_session(client_session_ptr->get_conn_index(), data_ptr, data_len);
	}

	#define REGISTER_MESSAGE_C2S(msg_index,handler)\
	{\
		proxy_service_cli::getInstance().register_message(\
		msg_index, handler);\
	}

	#define REGISTER_MESSAGE_C2WS(msg_index)\
	{\
		proxy_service_cli::getInstance().register_message(\
		msg_index, boost::bind(&transfer_c2ws, _1, _2, _3));\
	}

	#define REGISTER_MESSAGE_C2CS(msg_index)\
	{\
		proxy_service_cli::getInstance().register_message(\
		msg_index, boost::bind(&transfer_c2cs, _1, _2, _3));\
	}

	#define REGISTER_MESSAGE_S2C(msg_index)\
	{\
		message_manager::getInstance().register_handler(msg_index, boost::bind(&transfer_s2c, _1, _2, _3));\
	}

	#define REGISTER_MESSAGE_S2S(msg_index, handler)\
	{\
		message_manager::getInstance().register_handler(msg_index, handler);\
	}

	message_from_server_dispatcher::message_from_server_dispatcher() 
	{
	}

	message_from_server_dispatcher::~message_from_server_dispatcher() 
	{
	}
	bool message_from_server_dispatcher::init()
	{
		// ��ʼ��������Ϣ

		REGISTER_MESSAGE_S2S(e_msg_index_rep_login, boost::bind(&fep_client::internal_rep_login, &fep_client::getInstance(), _1, _2, _3));
		REGISTER_MESSAGE_S2S(e_msg_index_req_stop, boost::bind(&fep_client::internal_rep_stop, &fep_client::getInstance(), _1, _2, _3));
		REGISTER_MESSAGE_S2S(e_msg_index_ls2fep_client_login, ls2fep_rep_client_login);
		REGISTER_MESSAGE_S2S(e_msg_index_ws2c_login_queue_status, ws2fep_client_login_queue_status);
		REGISTER_MESSAGE_S2S(e_msg_index_ws2fep_client_logined, ws2fep_client_loginned);
		REGISTER_MESSAGE_S2S(e_msg_index_ws2fep_reconnect_game, ws2fep_reconnect_game_func);
		REGISTER_MESSAGE_S2S(e_msg_index_ws2fep_enter_game, ws2fep_enter_game_func);
		REGISTER_MESSAGE_S2S(e_msg_index_ws2fep_kickout_account, ws2fep_kickout_account_func);
		REGISTER_MESSAGE_S2S(e_msg_index_ws2fep_broadcast_msg, ws2fep_broadcast_msg_fun);
		REGISTER_MESSAGE_S2S(e_msg_index_cs2fep_aoi_msg, cs2fep_aoi_msg_func);
		REGISTER_MESSAGE_S2S(e_msgindex_cs2fep_in_game, cs2fep_in_game_msg_func);
		REGISTER_MESSAGE_S2S(e_msgindex_s2c_enum_char, ls2fep_rep_client_enum_char);
		REGISTER_MESSAGE_S2S(e_msgindex_ls2fep_create_character, ls2fep_rep_client_create_char);
		REGISTER_MESSAGE_S2S(e_msgindex_s2c_del_char, ls2fep_rep_client_del_char);
		REGISTER_MESSAGE_S2S(e_msgindex_ws2fep_time_limit_temp, ws2fep_recv_act_limit_temp);
		REGISTER_MESSAGE_S2S(e_msgindex_ws2fep_time_limit_branch_temp, ws2fep_recv_act_limit_branch_temp);

		// ע����Ϣ��������, ������Ϣ
		// ------------------------------------------login system---------------------------------------------
		REGISTER_MESSAGE_C2S(e_msgindex_c2fep_ping, c2fep_ping);
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_logout, c2fep_logout);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2cs_ping);
		REGISTER_MESSAGE_S2C(e_msgindex_cs2c_ping);
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_login_reconnect, c2ls_req_login_reconnect);
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_client_login,							c2ls_req_login);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_enter_scene);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_enter_scene);
		//REGISTER_MESSAGE_S2C(e_msgindex_s2c_del_char);

		//world
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_back_to_self_server);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_transfer_scene);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_transfer_scene);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_map_line_ary);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_transfer_scene_pos);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_map_load_finish);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_set_timekeeping);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_wave_trigger_message);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_boss_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_protect_npc_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_map_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_map_record_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_fuben_info_to_client);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_score_record_list);		
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_broadcast_character_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_map_game_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_map_game_over);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_map_game_gift);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_map_game_gift);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pop_plot_tip);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_msg_to_insert_player);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_transfer_error);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_leave_map);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_bosses_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_transfer_local_scene_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_one_map_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_one_map_info_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_transfer_scene_born);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_server_exp_level);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_reconnect_enter_scene);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_hot_update);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_buy_raid_count);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buy_raid_count_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_fuben_vip_mopping);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_fuben_vip_mopping_end);
		// activity
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_to_shui_jing_map);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_to_pk_king_map);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pk_king_game_over);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_to_broken_sky_map);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_broken_sky_boss_dead);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_broken_sky_game_over);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_to_cross_server_pk_map);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_server_pk_msg_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_server_pk_msg_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_pk_map_msg);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pk_map_island_state_msg);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_clear_pk_msg_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pk_state_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_world_boss_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_server_harry_msg_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_server_harry_msg_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_role_harry_msg_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_role_harry_msg_info_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_give_server_item_to_npc);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_to_harry_map);
		

		//character
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_enum_char, c2fep_enum_character);
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_enter_game, c2fep_select_character);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_recharge_success_by_sdk);
		//REGISTER_MESSAGE_C2WS(e_msgindex_c2s_enter_game);
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_create_char, c2fep_create_character);
		//REGISTER_MESSAGE_C2LS(e_msgindex_c2s_create_char);
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_del_char, c2fep_del_character);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_get_money);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_load_character_att);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_load_role_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_all_att);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_one_att);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_body_base_att);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_one_att);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_spawn_npc);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_map_targger_volume);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_game_over);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_game_over);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_fuhuo);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_fuhuo);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_unit_dead);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_player_levelup);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_money_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_money_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_map_fuben_over);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_fuben_send_notice);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_receive_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_bag_open_slot_num);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_bag_unlock_slot_req);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_gmorder_message);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_gmorder_message);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_pick_drop_box);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_addition_buff);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_addition_buff_arr);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_addition_buff_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_gs_value);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_find_other_appearance_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_save_appearance_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_save_appearance_info_to_id);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_save_appearance_info_to_id_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_load_appearance_info_by_id);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_load_appearance_info_by_id_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_pick_drop_box);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_base_att_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_base_att_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_grade_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_grade_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_warning);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_error);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_lucky_draw);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_lucky_draw);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mopping_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mopping_up_award);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mopping_up_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mopping_up_demons_tower);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mopping_up_demons_tower_get_award);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mopping_up_demons_tower_get_award_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_demons_tower_info_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_change_pk_mode);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_change_pk_mode_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pk_fight_back_sync);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_community);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_start_meditation_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_stop_mediation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_meditation_reward_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_start_meditation);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_stop_mediation);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_meditation_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_logic_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_logic_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_time_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_treasure_get_przie);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_treasure_get_przie);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_treasure_record_list);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_treasure_notice);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_jump_raid_animation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_character_time_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_map_arena_fuben_over);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_add_guide_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_guide_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_syn_tick_time);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_transfer_buff);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_func_unlock_award);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_func_unlock_award_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_interact_with_npc);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_interact_with_npc_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_npc_gather_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_finish_gather_result);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_hit_and_get_npc);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_highest_record_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_highest_record_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_purchase_patron_saint_mark_slot);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_purchase_patron_saint_mark_slot_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_grade_up_level_ani_finish);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_grade_up_level_ani_finish);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_open_server_time);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_npc_left_hp_per);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_add_damage_buff_in_expraid);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_add_damage_buff_in_expraid_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_guide_trigger_start);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_change_role_class);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_change_role_class_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cur_server_id);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_first_raid_award);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_auto_use_hp_pot);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_kill_prompt);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_ask_unit_dead);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_change_sky_suit_shape);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_change_sky_suit_shape_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_transfer_new_map_error);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_boss_damage_list_msg);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_leave_boss_damage_list_msg);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_help_success_msg);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_the_attacker_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_break_map_over);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_all_mopping_up);
		
		//person_information
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_person_information);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sync_person_information);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_modify_person_information);	
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_other_person_information);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_person_infor_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_person_infor_reward_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sync_other_person_couple_data);
		//competition
		
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_role_competition);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sync_role_competition);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_competition_lv_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_competition_lv_reward_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_competition_lv_reward_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_buy_competition_lv);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buy_competition_lv_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_buy_speical_manual);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buy_speical_manual_end);

		//element_competition
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_element_competition);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sync_element_competition);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_element_competition_lv_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_element_competition_lv_reward_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_element_competition_lv_reward_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_buy_element_competition_lv);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buy_element_competition_lv_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_buy_element_speical_manual);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buy_element_speical_manual_end);
	
		//buff
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_buff_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buff_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buff_damage);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buff_info_all);

		//Skill
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_skill_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_sub_cd);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_change_skill);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_skill_change_index);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_sync_fixed_param);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_skill_spell);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_spell);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_skill_cast);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_cast);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_fly);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_skill_hurt);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skill_patron_saint_effect);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_passive_skill);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_trigger_passive_skill);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_passive_skill_up_leve);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_passive_skill_up_leve);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_passive_skill_equip);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_passive_skill_equip);
		//Item
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_item_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_operation);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_set_quick_call_mount);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_quick_call_mount);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_update_character);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_tidy);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_recovery_earn_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_composit);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_item_composit);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_succinct_temp_property_value);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_show_this_whig);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_change_mount_shape);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_cd);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_hope_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_hope_item_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_use_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_one_key_use_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_sell_earn_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_merge_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_delete);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_auto_inherit_result);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_show_item_upgrade_effect);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_add_money);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_add_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_load_record_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_operate_record_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_patron_saint_mark_upgrade);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_patron_saint_mark_upgrade_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_patron_saint_mark_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_patron_saint_mark_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_show_goto_bag);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_quickly_hp);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_show_fashion);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_show_fashion);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_unlock_skill);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_unlock_skill_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_item_forge);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_forge_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_item_forge_begin);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_spirit_forge);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_forge_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_spirit_forge_begin);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_tinder_prop_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_tinder_prop_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_meditation_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_composit_failed_num);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_wing_feather_skill);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_wing_feather_skill_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_goddess_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_goddess_operation_end);

		//aoi
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_update);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_simple_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_simple_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_aoi_get_all);
		REGISTER_MESSAGE_S2C(e_msgindex_c2s_aoi_get_all_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_out);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_aoi_show_player);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_face_unit);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_marry_unit);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_aoi_simple_pos);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_simple_pos_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_aoi_location);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_aoi_rotation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_aoi_rotation);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_update_can_show_head);
		
		//arena
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_rank_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_match_player);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_match_player_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_top_three_player);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_top_three_player_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_challenge);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_req_challenge_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_challenge_over);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_get_challenge_log);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_req_get_challenge_log_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_arena_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_arena_reward_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_mopping_remain_times);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_req_mopping_remain_times_end);
		//belief
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_belief_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_belief_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_belief_update_character_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_belief_update_character_one);

		//pokedex
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_pokedex_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pokedex_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pokedex_update_character_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_pokedex_update_character_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_protect_spirit_operation_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_tinder_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_tinder_info_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_tinder_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_patron_saint_operated);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_psyche_recovery_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_psyche_recovery_operation_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_tinder_operate_end);
		//ranking
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_ranking_get_top);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ranking_get_top_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_ranking_worship);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ranking_worship_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_ranking_get_my_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ranking_get_my_rank_end);
		//REGISTER_MESSAGE_C2CS(e_msgindex_c2s_ranking_sync_player_data);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ranking_worship_record_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ranking_worship_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ranking_operate_result);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_player_ranking_value);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_player_ranking_value_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_rank_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_rank_info_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_service_rank_recevice_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_rank_reward_state_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_rank_reward_state_all);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_service_rank_get_first_player);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_rank_get_first_player_end);
		

		// mission
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mission_operate);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mission_alien_kill_npc);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mission_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mission_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mission_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mission_finish);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_reset_test);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mission_create_npc_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mission_show_chapter_finish);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_daily_must_do_count_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_daily_must_do_count_all);

		//spirit
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_spirit_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_operation);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_spirit_hunt);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_hunt_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_update);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_tidy);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_update_free_hunt_time);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_qiyuan_level);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_jiban_list);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_exchange_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_exchange_report);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_auto_sell_display);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_fetter_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_spirit_fetter_upgrade);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_spirit_fetter_upgrade);

		//goods
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_goods_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_goods_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_goods_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_goods_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_update_store);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_store); 
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_server_open_time);
		//elementheart
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_element_heart_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_heart_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_heart_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_heart_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_heart_delete);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_heart_tidy);

		//special_name
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_special_name_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_special_name_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_special_name_get_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_special_name_get_all);

		//achievement
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_achievement_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_achievement_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_achievement_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_achievement_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_achievement_finish);

		//phantom
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_phantom_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_phantom_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_phantom_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_phantom_operate_end);

		// awaken
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_item_awaken);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_awaken_end);

		//jewel_carve 
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_item_jewel_carve_oper);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_jewel_carve_oper_end);

		REGISTER_MESSAGE_S2C(e_msgindex_s2c_item_upgrade_data);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_item_upgrade);

		//recycle
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_data_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_login_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_recycle_oper);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_oper_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_buy_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_item_one);

		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_invited_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_invited_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_invited_reward_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_recycle_invited_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recycle_invited_level_one);

		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buy_tip);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_boss_result_rank_end);


		//chat_record
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_chat_record_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_chat_record_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_chat_record_oper);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_chat_record_end);

		//dragontrip
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_dragontrip_operate_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_dragontrip_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_dragontrip_operate);

		//starark
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_starark_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_starark_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_starark_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_starark_five_round_rewad_list);
		

		//skytreasure
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skytreasure_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_skytreasure_operate); 
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_skytreasure_operate_end);

		//mountpower
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mountpower_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mountpower_operate_end);

		// team
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_create_team);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_create_team_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_apply_to_join_team);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_let_role_join_team);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_add_team_member);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_del_team_member);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_leave_team);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_ready);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ready_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_kickout_team_member);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_kickout_team_member_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_leave_team_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_team_member_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_team_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_search_team_by_map_id);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_search_team_by_map_id_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_team_to_scene);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_join_team_applicant_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_invite_player);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_invite_player_success);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_invite_player_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_set_captain);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_team_error);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_ready_to_transfer_team);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ready_to_transfer_team_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_set_player_team_aoi_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_team_member_info_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_join_team);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_join_team_apply_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_clear_player_team_aoi_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_set_captain_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_team_member_pos_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_team_pos_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_no_team_legionmate_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_no_team_legionmate_list_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_aim_team_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_aim_team_list_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_auto_match_team);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_auto_match_team_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_update_team_attribute);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_team_attribute_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_update_team_mem_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_team_mem_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_notice_create_new_team);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_self_os_flags_update);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_refused_join_team);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_refused_join_team_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_team_gs_value);

		// legion
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_create_legion);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_create_legion_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_member_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_member_info_one);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_dissolve_legion);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_dissolve_legion_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_leave_legion);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_leave_legion_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_list_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_apply_to_join_legion);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_apply_to_join_legion_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_applicant_info_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_applicant_info_list_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_process_legion_apply);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_process_legion_apply_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_convenient_join_legion);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_convenient_join_legion_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_add_legion_member_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_del_legion_member);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_del_legion_member_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_legion_construction_level_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_construction_level_up_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_legion_get_construction_buff);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_get_construction_buff_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_legion_donate_money);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_legion_donate_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_donate_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_update_legion_announcement);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_announcement_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_event_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_event_list_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_kickout_legion_member);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_appoint_legion_member_job_title);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_appoint_legion_member_job_title_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_enter_legion_boss_map);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_kill_legion_boss);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_boss_award);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_boss_award_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_banquet_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_banquet_info_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_request_hold_banquet);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_request_hold_banquet_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_take_part_in_banquet);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_take_part_in_banquet_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_kickout_legion_member_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_boss_record_list);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_city_war_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_city_war_info_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_abstention_city_msg);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_apply_city_war_bid);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_apply_city_war_bid_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_entry_city_war_map);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_city_war_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_city_war_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_city_war_raid_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_city_war_update_role_score);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_city_war_over);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_set_auto_accept_new_member_setting);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_set_player_legion_aoi_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_clear_player_legion_aoi_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_set_player_legion_boss_award_get_log);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_city_master_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_city_master_info_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_occupation_daily_award);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_occupation_daily_award_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_rank_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_error);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_create_bonfire);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_bonfire_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_add_fuel_to_bonfire);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_member_add_fuel_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_legion_recruit_in_world);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_recruit_in_world_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_bonfire_dead);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_legion_call_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_call_operate_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_city_war_territory_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_city_war_territory_info_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_city_war_rank_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_city_war_rank_info_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_set_territory_prior_maintain);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_set_territory_prior_maintain_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_invite_player_to_join_legion);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_invite_player_to_join_legion_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_draw_mem_daily_award);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_draw_bonfire_daily_award);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_suck_blood_value);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_level_up_legion_skill);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_level_up_legion_skill_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_invite_player_succeed);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_auction_selling);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_bonus_rewards);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_bonus_rewards_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_bonus_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_bonus_info_one);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_stimulate_bonus_msg);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_skill_level_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_answer_question_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_answer_question_info_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_answer_result);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_answer_rank_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_legion_answer_rank_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_answer_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_legion_answer_box_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_legion_gs_value);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_legion_pdate_legion_member_gs_value);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_legion_pdate_legion_member_contribution);
		
		// ���Ųֿ�
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_warehouse_item_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_warehouse_item_info_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_operation_legion_warehouse_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_operation_legion_warehouse_item_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_legion_warehouse_log);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_legion_warehouse_log_end);
		
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_legion_territory_buff_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_bonfire_map_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_goto_next_stage_of_bonfire_map);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_to_bonfire_map);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_notice_new_item_in_legion_auction);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_members_info_list_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_change_legion_name);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_change_legion_name_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_confirm_change_legion_name);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_city_info_all);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_join_cross_overload_city_war);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_city_war_members_info_list_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_welfare_members_info_list_end);

		// best record
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_best_record);
		REGISTER_MESSAGE_S2C (e_msgindex_s2c_get_best_record_end);

		//mail
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mail_update);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mail_delete_these_mail);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mail_item_update);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mail_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mail_contents_get);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mail_operation);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_mail_send_mail);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_mail_delete_mail_by_live_time);
		REGISTER_MESSAGE_C2CS(e_msgindex_s2c_mail_send_to_all_player);

		//chat
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_globel_message);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_chat_message);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_chat_message_with_item_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_chat_result);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_globel_message);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_chat_message);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_chat_message_with_item_info);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_notice);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_notice);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_chat);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_chat_message_new);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_chat);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_chat_new);

		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_send_recruit_message);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_recruit_message);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_recruit_member_num);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_recruit_result);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_reveive_legion_recruit);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_notice_with_param);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_notice_with_param);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receive_top_tip);

		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_join_voice_channel);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_join_voice_channel_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_invite_join_voice_channel);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_join_or_leave_voice_channel_notice);

		//convert
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_convert_opereate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_convert_update_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_convert_update_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_convert_operate_end);

		//yesterday_must_do
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_find_back_must_do_resource);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_three_day_must_do_remain);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_back_must_do_resource_end);

		//vip
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_vip_item);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_recharge);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_recharge_widget_type);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_dead_recharge_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_vip_item_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_recharge_end);
		
		//talent
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_talent_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_talent_info_one);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_talent_level_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_talent_level_up_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_talent_reset);
		
		//welfare
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_welfare_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_welfare_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_active_degree_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_welfare);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_welfare_activatecode);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_welfare_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_random_get_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_active_degree_finish);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_reward_check_in);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_first_recharge_time);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_first_recharge_time_end);

		// cross
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_transfer);
		REGISTER_MESSAGE_C2S(e_msgindex_c2s_cross_transfer, c2ws_token_login);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_cross_cross_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_cross_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_cross_sign_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_sign_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_begin_war);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_cross_join_war);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_join_war);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_war_result);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_cross_war_log);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_war_log);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_game_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_server);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_server_pk);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_server_activity);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_server_harry);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_random_array);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_server_city_war);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_server_world_boss);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_server_ladder);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_move);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_rotation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_gm_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_need_cross_gm_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_is_close_exchange);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_reset_camera);
		//world_boss
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_world_boss);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_frist_kill_world_boss_prize);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_world_level);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_world_level);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_kill_boss);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_kill_boss);
	
		//title
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_title_equip_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_title_title_msg_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_title_own_title_msg_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_title_add_title_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_title_open_title_system_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_title_set_title_no_need_see);

		//grow_up_fund
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_growup_fund);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_purchase_growup_fund);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_growup_fund_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_purchase_growup_fund_end);

		//big_player
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_big_player_statue_guid);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_worship);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_worship_end);

		//find_other_player
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_find_other_unit);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_base_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_equiping_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_buff_equip_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_special_name_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_base_group_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_spirit_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_wing_or_mount_one_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_buff_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_belief_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_show_money_get);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_find_other_unit_feather_end);

		//red package
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_red_package);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_red_package_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_load_role_ws_info_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_role_ws_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_red_package_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_red_bag_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_del_red_bag_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_new_draw_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_red_package);

		// first time do
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_first_do_record);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_add_first_time_do_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sync_first_time_do_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_first_time_do_records);


		// relation
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_addfriend);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_addfriend);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_otheraddfriend);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_delfriend);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_friend_userinfo);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_friend_userinfo);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_delfriend);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_syc_friend_state);		
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_syc_full_friend_list);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_addblacklist);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_addblacklist);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_syc_full_black_list);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_delblacklist);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_delblacklist);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_syn_loverinfo);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_marrage);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_marry_get_reward);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_get_marry_mission);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_confirm_receive_marry_mission); 
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_confirm_receive_marry_mission);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_marry_examination);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_confirm_marry_examination);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_confirm_marry_examination);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_operate_marry_examination);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sync_marry_examination);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_operate_marry_examination);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_marrage);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_syc_full_hate_list);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_delhatelist);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_delhatelist);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_addhatelist);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_addhatelist);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_first_do_record);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_add_friendliness_value);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_friendliness_value);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_syc_relation_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_relation_state);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_syc_nearly_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_syc_nearly_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_operate_relation_result);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_operate_other_add_req);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_syc_full_other_req_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_marry_operate_result);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_couple_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_couple_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_reply_marry_choice); 
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_divorce);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_marry_effect_notice);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_upgrade_wedding_ring);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_upgrade_wedding_ring_result);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_marry_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_bless_one_couple);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_bless_one_couple_result);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_use_fireworks);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_lucky_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_ret_lucky_info_vec);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_ret_special_lucky_info);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_sync_marry_heart_value);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_add_use_power_up);
		
		//auction
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_auction_operate);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_auction_seach);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_auction_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_selling_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_receieve_sell_success);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_auction_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_req_auction_list_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_trade_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_req_trade_record_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_another_sell_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_req_another_sell_info_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_req_self_bid_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_bid_are_overtaken);

		//trade
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_trade_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_trade_operate_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_update_bid_record_failure);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_bid_notice);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_bid_notice_red);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_auction_sell_flag);

		// service_goal
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_goal_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_goal_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_service_goal_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_service_goal_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_seven_day_goal_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_seven_day_goal_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_seven_day_goal_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_seven_day_goal_operate_end);

		// interaction
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_c_s_interaction_invite);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_s_c_interaction_invite);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_c_s_interaction_invite_reply);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_s_c_interaction_invite_reply);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_c_s_real_start_interaction);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_s_c_real_start_interaction);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_c_s_stop_interaction);		
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_s_c_stop_interaction);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_interaction_invite_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_interaction_invite_start);

		//gain_treasure
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_gain_treasure_info_update_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_gain_treasure_info_update_all);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_gain_treasure_req_kill_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_gain_treasure_kill_record_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_gain_treasure_req_gain_item_record);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_gain_treasure_gain_item_record_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_gain_treasure_boss_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_gain_treasure_boss_info_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_buy_simage_boss_num);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_buy_simage_boss_num_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_can_transfer_other_line);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_auction_item_tip);
		//cloud_shop
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_cloud_shop_req);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cloud_shop_resp);

		//time_limit_activity
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_activity_template);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_update_all_behavior);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_update_one_behavior);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_update_sub_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_update_sub_no_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_update_all_open_act);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_time_limit_get_act_reward); 
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_time_limit_get_my_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_update_common_data);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_time_limit_get_common_data);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_time_limit_add_schedule);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_limit_time_notice_info_list);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_buy_time_limit_item);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_buy_time_limit_item_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_send_diamond_shop_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_time_limit_buy_diamond_shop);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_buy_diamond_shop_end);

		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_star_trip_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_star_trip_goods_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_star_trip_mission_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_star_trip_operate_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_star_trip_operate);
		
		//rename
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_change_player_name);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_change_player_name_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_confirm_change_name);

		//bossisland
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_boss_island_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_boss_island_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_player_boss_island_info_one);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_player_boss_island_info_all);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_player_server_config);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_server_act_is_open);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_send_special_server_msg);

		// belief_rune
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_belief_rune_equip_on);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_belief_rune_equip_levelup);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_belief_rune_equip_decompose);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_belief_rune_operate_result);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_belief_cloister_point_update);

		//first
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_character_legion_look_first);

		//reply_client
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_reply_map_targger_volume);

		//cross_ladder
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_cross_ladder_request_role_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_ladder_send_role_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_cross_ladder_request_sign_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_ladder_respond_sign_up);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_ladder_send_match_success);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_cross_ladder_request_buy_join_ticket);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_ladder_respond_buy_join_ticket);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_ladder_update_map_game_state);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_cross_ladder_req_mission_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_ladder_ret_mission_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_cross_ladder_send_upgrade_notify);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_cross_ladder_return_to_source_zone);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_server_name);

		//CrossServerWorldBoss
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_to_cross_server_world_boss_map);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_cross_boss_show_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_cross_boss_damage_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_world_boss_map_transfer_with_gate_msg);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_set_cross_boss_show_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_cross_boss_damage_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_boss_god_begin_time);	
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_boss_is_dead);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_get_boss_is_dead);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_boss_god_time);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_boss_god_time);
		

		//OracleTrial
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_oracle_trial_info_all);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_oracle_trial_commpingup);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_oracle_trial_commpingup_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_oracle_trial_customs_award);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_oracle_trial_customs_award_end);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_oracle_trial_rank);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_get_oracle_trial_rank_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_oracle_trial_funben_over_info);

		//LegionStation
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_join_legion_station_map);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_add_legion_boss_damage);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_add_legion_boss_damage_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_add_legion_damage_big_player);

		//LegionDance
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_map_legion_player);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_invite_legion_dance);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_be_invite_legion_dance_end);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_legion_dance_end);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_legion_dance_interaction_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_invite_legion_dance_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_map_legion_player);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_be_invite_legion_dance);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_legion_dance_interaction_begin);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_legion_Dance_clear_data);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_start_assist_fight);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_assist_fight_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sycn_assist_fight_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_req_assist_fight);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_cancel_assist_fight);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_sync_assist_fight_state);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_assist_fight_end_to_show_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_assist_fight_end_to_show_thank_reward);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_be_assist_fight_tip);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_create_assist_fight_npc);
		

		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_legion_add_bonfire_end);
		
		//CheckIn
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_get_cumulative_sign_in_reward);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_get_cumulative_sign_in_reward_end);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_retroactive_all_days);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_retroactive_all_days_end);

		//month recharge num
		REGISTER_MESSAGE_S2S(e_msgindex_cs2fep_month_recharge_num, cs2fep_month_recharge_msg_func);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_show_vip);

		//pk_king
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_player_check_kill_num);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_refresh_player_pk_king_info);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_refresh_pk_king_map_state);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_pk_king_rank_info);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_need_break_line_login);
		REGISTER_MESSAGE_C2WS(e_mgsindex_c2s_send_need_break_line_login_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_create_pk_end_time);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_pk_king_target_list_msg);
		

		//element_war
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_send_upgrade_notify);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_send_sign_up_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_send_sign_up_team_ready_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_send_close_sign_up_end);
		REGISTER_MESSAGE_C2WS(e_mgsindex_c2s_element_war_send_sign_up);
		REGISTER_MESSAGE_C2WS(e_mgsindex_c2s_element_war_send_sign_up_team_ready);
		REGISTER_MESSAGE_C2WS(e_mgsindex_c2s_element_war_send_close_sign_up);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_send_role_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_war_send_match_success);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_war_send_map_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_war_send_map_role_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_war_end_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_element_war_send_boss_be_attack);
		REGISTER_MESSAGE_C2WS(e_mgsindex_c2s_element_war_get_mission_reward);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_get_mission_reward_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_buy_ticket_end);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_send_create_ready_ui);
		REGISTER_MESSAGE_C2WS(e_mgsindex_c2s_element_war_get_role_info);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_element_war_buy_ticket);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_create_boss_dead);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_element_war_send_map_rank_info);
		REGISTER_MESSAGE_S2C(e_mgsindex_s2c_send_map_target_show_info);
		REGISTER_MESSAGE_C2CS(e_mgsindex_c2s_get_map_target_show_info);

		//legion_world_boss
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_legion_strategy_world_boss_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_request_legion_strategy_world_boss_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_legion_strategy_world_boss_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_all_world_boss_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_world_boss_strategy_result);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_request_world_boss_info);

		//lucky_card
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_refresh_lucky_card_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_refresh_lucky_card);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_lucky_card_operate_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_lucky_card_operate);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_change_enchant_show_type);
		
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_new_server_treasure_finish);

		//AttackCity
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_transfer_attack_city_map);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_attack_city_map_legion_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_attack_city_info);
		REGISTER_MESSAGE_C2WS(e_msgindex_c2s_get_attack_city_rank_list);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_attack_city_map_rank_list);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_attack_city_operate);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_operate_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_npc_list);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_camp_cut);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_transfer_to_bron_pos);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_legion_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_end_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_legion_rank_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_legion_map_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_attack_legion_call);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_transfer_info);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_end_attack_transfer_info);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_attack_city_npc_state_change_info);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_time_feed_back_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_feed_back_operation_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_time_feed_back_info);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_time_limit_gift_operation);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_time_limit_gift_operation_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_update_time_limit_gift);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_initiative_time_limit_gift);


		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_send_subscribe_daily);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_subscribe_daily_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_subscribe_daily_info);

		REGISTER_MESSAGE_S2C(e_msgindex_s2c_send_offline_award);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_offline_award);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_online_award);

		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_upgrade_search_level);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_upgrade_search_level_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_npc_award);
		REGISTER_MESSAGE_S2C(e_msgindex_c2s_get_npc_award_end);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_open_treasuer_pic);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_get_treasuer_award);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_map_anim_end);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_map_settlement);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_refrush_award_npc);
		REGISTER_MESSAGE_C2CS(e_msgindex_c2s_search_award_npc);
		REGISTER_MESSAGE_S2C(e_msgindex_s2c_search_award_npc_end);
		return true; 
	}
}
