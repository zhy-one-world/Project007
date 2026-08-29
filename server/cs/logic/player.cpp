/********************************************************************
created: 2014/07/11
created: 11:7:2014 15:00
file base: player
file ext: cpp
author: zhy

purpose:
*********************************************************************/


/************************************************************************/
/*                           File Include                               */
/************************************************************************/

#include "achievement_def.h"
#include "aoi/aoi_system.h"
#include "belief_rune_def.hpp"
#include "buff_def.hpp"
#include "buff_set.h"
#include "cell_server.hpp"
#include "components/scene/base_map_component.h"
#include "connection_mgr.hpp"
#include "cross_server_world_boss_def.hpp"
#include "game_cfg/servers_config.h"
#include "game_enum_def_s.h"
#include "gm_order_def.hpp"
#include "http/http_send.hpp"
#include "internal/char_msg.hpp"
#include "internal/core.hpp"
#include "internal/cross_ladder_msg.hpp"
#include "internal/cross_server_world_boss_msg.hpp"
#include "internal/element_war_msg.hpp"
#include "internal/event_msg.hpp"
#include "internal/marry_msg.hpp"
#include "internal/world_boss_msg.hpp"
#include "internet/aoi.pb.h"
#include "internet/belief_rune.pb.h"
#include "internet/character.pb.h"
#include "internet/chat.pb.h"
#include "internet/cs2dp.pb.h"
#include "internet/game.pb.h"
#include "internet/goods.pb.h"
#include "internet/item.pb.h"
#include "internet/net.pb.h"
#include "invalid_word/trie_filter.h"
#include "Logic/activity_def.hpp"
#include "Logic/att_def.hpp"
#include "logic/buff_man.h"
#include "Logic/char_def.hpp"
#include "logic/drop.h"
#include "logic/item_set.h"
#include "Logic/mail_def.hpp"
#include "logic/map_record.h"
#include "logic/player.hpp"
#include "logic/skill_manager.h"
#include "logic/skill_set.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"

#include "mission_mgr.hpp"
#include "offline_award_system.h"
#include "online_award_system.h"
#include "pk_community_def.hpp"
#include "pk_king_def.hpp"
#include "player_def.hpp"
#include "pokedex_def.hpp"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "system/item/item_upgrade_system.h"
#include "system/play/time_activity_system.h"
#include "system/scene/base_map_system.h"
#include "system/scene/map_search_system.h"
#include "template/ActivityCommonConfigTemplate_S.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "threat_drop_processor.h"
#include "time.hpp"
#include "utility/cs_date.hpp"
#include "utility/parse_msg.h"
#include "utility/random.h"
#include <Logic/game_enum_def_s.h>
#include <Logic/item_def.hpp>
#include <Logic/time_def.hpp>
#include <memory>

namespace faith
{
#define DEBUG_MAIL  false

	/************************************************************************/
	/*                         Class Implement                              */
	/************************************************************************/

	player::player()
	{
		m_map_record_mgr = std::make_shared<map_record_set>();
	}

	player::~player()
	{
	}

	player* player::cast(unit* o)
	{
		ZoneScoped;
		if (o && faith::e_unit_type_player == o->m_unit_type)
		{
			return static_cast<player*>(o);
		}
		return nullptr;
	}

	void player::clear_data()
	{
		ZoneScoped;
		unit::clear_data();
		//������ҳ�ʼ��Ӫ
		m_pawn_att.set_unit_base_att(e_base_att_info_community_type, e_community_type_player_group1);
		/*m_relation_list_mgr.clear_data();*/
		m_last_auction_time = 0;
		m_sec_tick = 0;
		m_2sec_tick = 0;
		m_half_sec_tick = 0;
		m_hour_1_tick = 0;
		m_min_tick = 0;
		m_min_10_tick = 0;
		fuhuo_cut_hp = false;
		m_unit_type = faith::e_unit_type_player;
		m_status = e_session_status_create_player;
		refresh_heart_beat();
		m_is_in_ancient_battlefield = false;
		m_is_in_exp_raid = false;
		m_is_in_belief_cloister_battlefield = false;
		m_add_ancient_activity_degree = false;
		m_add_exp_raid_degree = false;
		m_already_login = false;
		m_login_time = 0;
		m_dead_stamp = 0;
		m_role_logic.clear_data();
		m_role_time.clear_data();
		m_player_aoi_watch.clear_data();
		m_is_waiting_auto_revive = false;
		m_money_info.clear_data();
		m_exp_buff_template_id = 0;
		m_first_npc_kill_id_arr.clear();
		m_loading_flag.clear();
		m_loading_flag.insert(e_data_flag_info);
		m_loading_flag.insert(e_data_flag_att);
		m_loading_flag.insert(e_data_flag_buff);
		m_loading_flag.insert(e_data_flag_skill);
		m_loading_flag.insert(e_data_flag_money);
		m_loading_flag.insert(e_data_flag_time_activity);
		//m_loading_flag.insert(e_data_flag_sprite_fetter);
		m_loading_flag.insert(e_data_flag_mission);
		m_loading_flag.insert(e_data_flag_goods);
		m_loading_flag.insert(e_data_flag_map);
		m_loading_flag.insert(e_data_flag_item_upgrade);
		m_loading_flag.insert(e_data_flag_special_name);
		m_loading_flag.insert(e_data_flag_achievement);
		m_loading_flag.insert(e_data_flag_pokedex);
		m_loading_flag.insert(e_data_flag_tinder);
		m_loading_flag.insert(e_data_flag_worship);
		m_loading_flag.insert(e_data_flag_yesterday_must_do_remain);
		m_loading_flag.insert(e_data_flag_convert);
		m_loading_flag.insert(e_data_flag_time);
		m_loading_flag.insert(e_data_flag_welfare);
		m_loading_flag.insert(e_data_flag_active_degree);
		m_loading_flag.insert(e_data_flag_title);
		m_loading_flag.insert(e_data_flag_guide);
		m_loading_flag.insert(e_data_flag_fist_time_do);
		m_loading_flag.insert(e_data_flag_talent);
		m_loading_flag.insert(e_data_flag_highest_record);
		m_loading_flag.insert(e_data_flag_service_goal);
		m_loading_flag.insert(e_data_flag_daily_must_do_count);
		m_loading_flag.insert(e_data_flag_auction_self_selling);
		m_loading_flag.insert(e_data_flag_globel_mail_had);
		m_loading_flag.insert(e_data_flag_service_rank);
		m_loading_flag.insert(e_data_flag_logic_info);
		m_loading_flag.insert(e_data_flag_gain_treasure);
		m_loading_flag.insert(e_data_flag_time_limit_activity);
		m_loading_flag.insert(e_data_flag_boss_island);
		m_loading_flag.insert(e_data_flag_passive_skill);
		m_loading_flag.insert(e_data_flag_legion_skill);
		m_loading_flag.insert(e_data_flag_seven_day_goal);
		m_loading_flag.insert(e_data_flag_phantom);
		m_loading_flag.insert(e_data_flag_oracle_trial);
		m_loading_flag.insert(e_data_flag_chat_record);
		m_loading_flag.insert(e_data_flag_player_information);
		m_loading_flag.insert(e_data_flag_recycle);
		m_loading_flag.insert(e_data_flag_recycle_task);
		m_loading_flag.insert(e_data_flag_player_competition);
		m_loading_flag.insert(e_data_flag_dragontrip);
		m_loading_flag.insert(e_data_flag_starark);
		m_loading_flag.insert(e_data_flag_element_competition);
		m_loading_flag.insert(e_data_flag_time_feed_back);
		m_loading_flag.insert(e_data_flag_time_limit_gift);
		m_loading_flag.insert(e_data_flag_subscribe_daily_info);
		for (int32 i = e_bag_type_none + 1; i < e_bag_type_max; ++i)
		{
			m_loading_flag.insert(e_data_flag_item + i);
		}

		for (int32 i = 0; i < e_save_data_type_max; ++i)
		{
			m_saving_flag[i].clear();
		}
		uint32 current_time = time_helper::get_cur_time_new().second;
		uint32 timer_num = sizeof(m_time_begin) / sizeof(uint32);
		for (uint32 i = 0; i < timer_num; i++)
		{
			m_time_begin[i] = current_time;
		}
		m_mission_mgr.clear_data();
		m_achievement_mgr.clear_data();
		m_mail_mgr.clear_data();
		m_chat_mgr.clear_data();
		m_store_helper.clear_data();
		m_arena_mgr.clear_data();
		m_special_name_mgr.clear_data();
		m_meditation_mgr.clear_data();
		m_convert_mgr.clear_data();
		m_belief_mgr.clear_data();
		m_belief_rune_mgr.clear_data();
		m_talent_mgr.clear_data();
		m_pokedex_mgr.clear_data();
		m_welfare_mgr.clear_data();
		m_auction_mgr.clear_data();
		m_title_mgr.clear_data();
		m_interaction_mgr.clear_data();
		m_role_right.clear_data();
		m_guide_mgr.clear_data();
		m_func_unlock_mgr.clear_data();
		m_first_time_do_mgr.clear_data();
		m_worship_target.clear_data();
		m_ranking_mgr.clear_data();
		m_passive_skill.clear_data();
		m_legion_skill.clear_data();
		memset(m_npc_position, 0, sizeof(m_npc_position));
		memset(m_three_day_must_do_remain_ary, 0, sizeof(m_three_day_must_do_remain_ary));
		m_update_db_flag = false;
		m_leave_time = 0;
		m_leave_num = 0;
		m_is_cross_cs_create = false;
		m_heart_beat_time = 0;
		m_time_save_db = 0;
		m_teleport_control.clear_data();
		clear_trans_cache();
		m_fuben_data_set.clear();
		m_history_high_record.clear_data();
		m_service_goal_mgr.clear_data();
		m_seven_day_goal_mgr.clear_data();
		m_battle_begin_time = 0;
		m_is_robot = false;
		m_relive_times_in_map = 0;
		m_service_rank_mgr.clear_data();
		m_gs_last_sync_ws = 0;
		m_is_have_cant_equip_item = false;
		m_gain_treasure_mgr.clear_data();
		m_treasure_mgr.clear_data();
		m_time_limit_activity_mgr.clear_data();
		clear_gain_exp_in_raid();
		m_boss_island_mgr.clear_data();
		m_get_relic_exp = 0;
		m_hp_percent = 0.7f;
		m_is_auto_buy_pot = true;
		m_continuity_kill_end_time_stamp = 0;
		m_kill_num_now = 0;
		m_server_harry_mgr.clear_data();
		m_random_array_time = 0;
		memset(m_random_array, 0, sizeof(m_random_array));
		m_feather_heart_mgr.clear_data();
		m_phantom_mgr.clear_data();
		m_recycle_mgr.clear_data();
		m_awakan_mgr.clear_data();
		m_cross_ladder_can_buy_ticket_num = 0;
		m_summon_tick = 0;
		memset(m_summon_pet_array, 0, sizeof(m_summon_pet_array));
		m_oracle_trial_mgr.clear_data();
		m_person_infor_mgr.clear_data();
		m_role_competition_mgr.clear_data();
		m_element_competition_mgr.clear_data();
		m_is_need_send_gate_save_end = false;
		m_attacker_index = 0;
		m_send_attacker_info_cd = 0;
		m_assist_fight_mgr.clear_data();
		m_dragontrip_mgr.clear_data();
		m_mount_power_mgr.clear_data();
		m_jewel_carve_mgr.clear_data();
		m_skytreasure_mgr.clear_data();
		m_starark_mgr.clear_data();
		m_is_begin_exp_raid = true;
		m_is_need_add_exp_buff = false;
		m_subscribe_daily_list.clear();
		m_map_record_mgr->clear_data();
	}

	void player::clear_trans_cache()
	{
		ZoneScoped;
		m_teleport_map_id = -1;
		m_teleport_line_id = -1;
		m_teleport_map_pos.clear_data();
		m_teleport_group_id = 0;
		m_teleport_map_guid.clear_data();
		m_teleport_war_index = 0;
	}

	void player::init_enter_data(int32 login_type, s_client_uid client_uid, const login_fixed_data& login_data)
	{
		ZoneScoped;
		m_status = e_session_status_dp_loading;
		m_login_type = login_type;
		m_client_uid = client_uid;
		memcpy(&m_login_third_data, &login_data, sizeof(login_fixed_data));
		send_open_server_time_to_self();		//���Ϳ���ʱ������

		//���ͷ�������Ϣ�����
		character_proto_player_cur_server_id server_id_msg;
		server_id_msg.Clear();
		server_id_msg.set_server_id(cell_server::getInstance().get_server_id());
		server_id_msg.set_gate_server_id(cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross));
		send_message_to_self(&server_id_msg, e_msgindex_s2c_cur_server_id);
		//m_interaction_type = e_interaction_type_none;
		//m_interaction_object_guid = guid_64();
	}

	void player::set_array_index(int32 array_index)
	{
		ZoneScoped;
		unit::set_array_index(array_index);
		m_teleport_control.set_player_ptr(this);
		m_belief_mgr.set_player_ptr(this);
		m_belief_rune_mgr.set_player_ptr(this);
		m_pokedex_mgr.set_player_ptr(this);
		m_worship_target.set_player_ptr(this);
		m_mail_mgr.set_player_ptr(this);
		m_chat_mgr.set_player_ptr(this);
		m_ranking_mgr.set_player_ptr(array_index);
		m_mission_mgr.set_player_ptr(array_index);
		m_talent_mgr.set_player_ptr(array_index);
		m_arena_mgr.set_player_ptr(array_index);
		m_achievement_mgr.set_player_ptr(array_index);
		m_store_helper.set_player_ptr(array_index);
		m_special_name_mgr.set_player_ptr(array_index);
		m_meditation_mgr.set_player_ptr(this);
		m_convert_mgr.set_player_ptr(this);
		m_welfare_mgr.set_player_ptr(array_index);
		m_title_mgr.set_unit_index(array_index);
		m_interaction_mgr.set_unit_index(array_index);
		m_guide_mgr.set_unit_index(array_index);
		m_func_unlock_mgr.set_unit_index(array_index);
		m_first_time_do_mgr.set_unit_index(array_index);
		m_patron_saint_mgr.set_player_array_index(array_index);
		m_service_goal_mgr.set_player_index(array_index);
		m_seven_day_goal_mgr.set_player_index(array_index);
		m_auction_mgr.set_array_index(array_index);
		m_service_rank_mgr.set_player_ptr(this);
		m_gain_treasure_mgr.set_player_ptr(this);
		m_player_aoi_watch.set_unit_index(array_index);
		m_treasure_mgr.set_player_ptr(array_index);
		m_time_limit_activity_mgr.set_player_ptr(this);
		m_boss_island_mgr.set_player_ptr(this);
		m_passive_skill.set_unit_index(array_index);
		m_legion_skill.set_unit_index(array_index);
		m_server_harry_mgr.set_player_ptr(this);
		m_feather_heart_mgr.set_player_ptr(this);
		m_phantom_mgr.set_player_ptr(array_index);
		m_recycle_mgr.set_player_ptr(array_index);
		m_awakan_mgr.set_player_ptr(array_index);
		m_goddess_mgr.set_unit_index(array_index);
		m_marry_mgr.set_unit_index(array_index);
		m_oracle_trial_mgr.set_unit_index(array_index);
		m_person_infor_mgr.set_unit_index(array_index);
		m_role_competition_mgr.set_unit_index(array_index);
		m_legion_dance_mgr.set_unit_index(array_index);
		m_assist_fight_mgr.set_unit_index(array_index);
		m_dragontrip_mgr.set_player_ptr(array_index);
		m_mount_power_mgr.set_player_ptr(array_index);
		m_jewel_carve_mgr.set_player_ptr(array_index);
		m_skytreasure_mgr.set_player_ptr(array_index);
		m_starark_mgr.set_player_ptr(array_index);
		m_lucky_card_mgr.set_player_ptr(array_index);
		m_star_trip_mgr.set_player_ptr(array_index);
		m_element_competition_mgr.set_unit_index(array_index);
		m_time_feed_back_mgr.set_player_ptr(array_index);
		m_time_limit_gift_mgr.set_player_ptr(array_index);
		m_map_record_mgr->set_player_ptr(array_index);
	}
	void player::reload_csv()
	{

	}
	void player::check_is_robot(/*const xchar* cur_account*/)
	{
		ZoneScoped;
		if (init_unit::is_robot_account(SERVERCONFIG->robot_account, m_role_info.account))
		{
			m_is_robot = true;
		}
	}
	bool player::get_write_log()
	{
		ZoneScoped;
		return m_is_robot == false;
	}
	void player::check_srv_world_level_buff()
	{
		ZoneScoped;
		float add_percent = 0;
		int32 buff_template_id = 0;
		globle_data::get_instance().get_exp_add_percent(get_unit_info(e_role_info_exp_level), m_cur_world_level, cell_server::getInstance().get_world_level_cur(), add_percent, buff_template_id);
		if (buff_template_id == m_exp_buff_template_id)
		{
			game_proto_server_exp_level msg;//���߷�һ�ε�ǰ�������ȼ�
			msg.set_exp_level(cell_server::getInstance().get_world_level_cur());
			msg.set_exp_percent(add_percent);
			msg.set_exp_last_level(cell_server::getInstance().get_world_level_last());
			send_message_to_self(&msg, e_msgindex_s2c_server_exp_level);
			return;
		}

		buff_man::del_buff_by_template_id(get_array_index(), get_array_index(), m_exp_buff_template_id);
		buff_man::add_buff_inst(get_array_index(), get_array_index(), buff_template_id);
		m_exp_buff_template_id = buff_template_id;

		game_proto_server_exp_level msg;
		msg.set_exp_level(cell_server::getInstance().get_world_level_cur());
		msg.set_exp_percent(add_percent);
		msg.set_exp_last_level(cell_server::getInstance().get_world_level_last());
		send_message_to_self(&msg, e_msgindex_s2c_server_exp_level);
	}

	void player::change_big_player_title(int32 title_type)
	{
		ZoneScoped;
		int32 title_id = get_title_mgr().get_title_template_id_by_type((e_title_type)title_type);
		bool is_have = get_title_mgr().is_have_title(title_id);
		if (is_have == true)
		{
			int32 show_title = get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
			get_title_mgr().remove_title_by_template_id(pk_king_title_id);
			if (show_title == pk_king_title_id)
			{
				show_title = get_title_mgr().get_first_active_title_template_id();
				get_title_mgr().equip_on_title(show_title);
			}
		}
	}

	void player::init_by_inst_data_over()
	{
		ZoneScoped;
		m_old_tick_time = utility::get_tick_count();
		m_time_save_db = m_old_tick_time + role_data_save_time;
		add_att_by_load();
		int32 cur_vip_level = get_vip_level();
		vip_buff_change(cur_vip_level, true);
		refresh_vip_title();
		int32 role_array_index = get_array_index();
		get_belief_rune_mgr().init_data();
		PlayerInitTemplate* player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, get_unit_info(e_role_info_template_id));
		if (player_init_template_ptr && player_init_template_ptr->Capsule.size() >= 2)
		{
			m_unit_half_height = player_init_template_ptr->Capsule[0];
			m_unit_capsule = player_init_template_ptr->Capsule[1];
		}

		auto base_map_cp = m_map_ent->get_component<base_map_component>();

		//if (map_object_ptr->get_is_double_line())
		//{
		//	buff_man::add_buff_inst(role_array_index, role_array_index, GAMECONFIG->DoubleLineBuffId);
		//}
		MapTemplate* move_map_template_ptr = GET_TEMPLATE(MapTemplate, get_unit_info(e_role_info_move_map_id));
		if (move_map_template_ptr && move_map_template_ptr->Type == faith::e_map_type_big_map)
		{
			if (get_unit_info(e_role_info_move_line_id) == 1)
			{
				if (GAMECONFIG->LineInfo.size() >= 2)
				{
					buff_man::add_buff_inst(role_array_index, role_array_index, GAMECONFIG->LineInfo[1]);
				}
			}
			if (get_unit_info(e_role_info_move_line_id) == 2)
			{
				if (GAMECONFIG->LineInfo.size() >= 4)
				{
					buff_man::add_buff_inst(role_array_index, role_array_index, GAMECONFIG->LineInfo[3]);
				}
			}
			// ���ش��ͼ��Ҫ������Ӫ��ֹ�����������Ӫ�Ķ�������ҿ��Ի��๥��
			get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_player_group1);
		}
		if (get_is_robot())
		{
			buff_man::add_buff_inst(role_array_index, role_array_index, ROBOT_BUFF_TEMPLATE_ID);
			buff_man::add_buff_inst(role_array_index, role_array_index, ROBOT_BUFF_EXP_TEMPLATE_ID);
		}

		if (base_map_cp->m_map_template->MustUseWing && get_unit_info(e_role_info_wing_showd_template_id) == 0)
		{
			citem* wing_item_ptr = get_item_set().get_cur_level_wing();
			if (wing_item_ptr)
			{
				get_item_set().show_this_wing(wing_item_ptr, -1);
			}
		}
		m_legion_cs_mgr.change_city_master_stuff();
		m_legion_cs_mgr.change_city_war_winner_legion_title_and_mount();

		m_legion_skill.init_legion_skill_buff();
		//��ʼ��װ��ս��
		get_item_set().init_skill_and_item_gs();
		get_item_set().init_mount_skill_att();

		m_feather_heart_mgr.init_set_feather_skill_info();
		get_item_set().init_wing_skill_att();
		get_item_set().init_break_will_skill_att();
		get_item_set().init_awaken_fetter_att();
		m_phantom_mgr.on_item_load_end();
		m_phantom_mgr.init_all_skill();

		//��ս����ʱ
		if (base_map_cp->m_map_template->Type == e_map_type_field)
		{
			m_is_in_ancient_battlefield = true;
		}
		if (base_map_cp->m_map_template->Type == e_map_type_belief_cloister)
		{
			m_is_in_belief_cloister_battlefield = true;
		}
		if (base_map_cp->m_map_template->Type == e_map_type_exp_fuben)
		{
			m_is_in_exp_raid = true;
			skill_manager::get_skill_set(m_array_index).set_skill_cd_by_type(est_patron_saint, get_time_data(e_time_type_exp_raid_skill_cd), true);
		}

		int32 last_map_temp_id = get_unit_info(e_role_info_leave_map_id);
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, last_map_temp_id);
		if (nullptr != map_temp_ptr)
		{
			if (map_temp_ptr->Type == e_map_type_exp_fuben)
			{
				skill* skill_ptr = skill_manager::get_skill_set(m_array_index).get_skill_inst_by_type(est_patron_saint);
				if (nullptr != skill_ptr)
				{
					int64 skill_cd = 0;
					skill_cd = skill_ptr->get_skill_cd() > 0 ? skill_ptr->get_skill_cd() : 0;
					set_time_data(e_time_type_exp_raid_skill_cd, skill_cd);
				}
			}
		}

		set_time_data(e_time_type_login_time, time_helper::get_cur_time_new().second);
		if (get_time_data(e_time_type_logout_time) == 0)
		{
			set_time_data(e_time_type_logout_time, time_helper::get_cur_time_new().second);
			for (int32 i = 0; i < player_init_template_ptr->InitMoney.size(); i += 2)
			{
				add_money((e_money_type)player_init_template_ptr->InitMoney[i], player_init_template_ptr->InitMoney[i + 1], e_server_log_add_money_player_init);
			}
		}

		m_pk_community_mgr.init_map_pk_type();

		//�������buff(eg:�ƺŻ�õ�buff)
		m_special_name_mgr.init_client_special_name();
		//����pkbuff
		m_pk_community_mgr.pk_value_add_buff(m_pk_community_mgr.get_pk_value());
		//�츳buff
		m_talent_mgr.set_talent_all_buff(true);
		m_talent_mgr.set_talent_all_skill(true);
		//����
		m_patron_saint_mgr.set_tinder_att_all(true);

		skill_set& skill_set_ref = skill_manager::get_skill_set(get_array_index());
		skill_set_ref.upgrade_skill_achievement();
		skill_set_ref.all_skill_level_count_to_achievement();
		get_arena_mgr().get_arena_rank();
		m_pawn_att.init_unit_att_over(false);
		m_pawn_att.syn_all_att();

		// ���Ĭ�����ó�e_community_type_player_group1 ���޸��ڳ�����enterscene�޸�Ϊ��������Ҫ��
		m_pawn_att.set_unit_base_att(e_base_att_info_community_type, e_community_type_player_group1);

		m_pawn_att.send_base_att_all();

		m_service_goal_mgr.refresh_service_goal_all();
		m_seven_day_goal_mgr.refresh_seven_day_goal_all();

		set_unit_gs_change();

		int64 gs_value = get_unit_gs_value();
		if (check_and_set_highest_record(e_role_history_highest_record_gs_value, gs_value) == true)
		{
			m_time_limit_activity_mgr.set_up_rank_activity_schedule(e_time_limit_activity_type_gs_up);
			m_time_limit_activity_mgr.set_up_rank_activity_schedule(e_time_limit_activity_type_cross_gs_up);
		}

		//���ܽ���
		get_func_unlock_mgr().load_func_unlock_template_map();
		//7�հ�ˢ��ʱ�� �������ܽ������ڽ�����
		m_service_goal_mgr.refresh_over_time_all();

		int64 test_local_time = time_helper::get_local_cur_time_stamp();//����ʱ��
		character_proto_syn_tick_time syn_time_msg;
		syn_time_msg.set_cur_server_time(test_local_time / 1000);
		syn_time_msg.set_cur_server_green_time(m_old_tick_time / 1000);
		//���룬һ��int32װ���£�����1000�����

		//ڤ��ʱ��
		m_meditation_mgr.offline_meditation_settlement();

		//���߽���
		offline_award_system::start_up(this);
		offline_award_system::player_online(this);

		send_message_to_self(&syn_time_msg, e_msgindex_s2c_syn_tick_time);

		//ˢ�� daily info
		refresh_daily_info(false);
		////����������а��¼
		get_ranking_mgr().init_ranking_data();
		//���ˢ�����û�߹��˲��ڵ�½ʱinit
		if (!is_need_refresh_daily_info_at_zero_hour_no_cross())
		{
			get_time_limit_activity_mgr().init_all_act();
			//�ͬһ���� �������ˢ�±�������������
			for (int32 i = e_time_limit_activity_type_gs_up; i <= e_time_limit_activity_type_spirit_up; i++)
			{
				get_time_limit_activity_mgr().reset_ranking_value((e_time_limit_activity_type)i);
			}
			for (int32 i = e_time_limit_activity_type_total_charge; i <= e_time_limit_activity_type_total_cost; i++)
			{
				get_time_limit_activity_mgr().reset_ranking_value((e_time_limit_activity_type)i);
			}
			get_time_limit_activity_mgr().reset_ranking_value(e_time_limit_activity_type_cross_total_charge);
		}
		if (!is_need_refresh_daily_info_at_zero_hour_cross())
		{
			get_time_limit_activity_mgr().init_template_id_cross();
		}

		for (int32 i = e_time_limit_activity_type_cross_gs_up; i <= e_time_limit_activity_type_cross_spirit_up; i++)
		{
			get_time_limit_activity_mgr().set_up_rank_activity_schedule((e_time_limit_activity_type)i);
		}


		//���ͳ�ݵļ�¼
		get_worship_target().send_worhip_record();
		// ���״̬��clear_data��Ĭ�ϱ���Ϊtrue Ҳ���ǲ��ɴ��� ������Ϊfalse ���Դ�����
		m_teleport_control.m_teleporting = false;

		m_battle_begin_time = 0;

		m_relive_times_in_map = 0;

		get_pk_community_mgr().on_player_transfer_init();

		set_cur_player_world_level();
		check_srv_world_level_buff();

		get_team_cs_mgr().on_player_transfer_finished();

		// ��Ծ��ɶ��
		fuben_map_enter_activity();

		//���긱�������� ����map_guid
		m_role_info.map_guid = get_map_guid();
		//���ø�����þ���
		clear_gain_exp_in_raid();
		//load��֮�����޸�buffʱ�䣬��ֹ�ص��������µ�һЩ����
		get_item_set().change_buff_time();

		change_harry_buff();//�Ӷ���Ʒ��buff

		//ͬ��������Ϣ
		get_marry_mgr().sync_dati_marry(0);

		//�����������������Ϣ
		get_oracle_trial_mgr().send_player_oracle_trial_info_all();

		get_person_infor_mgr().sync_to_client();

		get_competition_mgr().sync_competition_info();

		get_element_competition_mgr().sync_competition_info();

		get_recycle_mgr().load_player_time_end_init();

		get_welfare_mgr().get_first_recharge_welfare_by_id();

		get_time_feed_back_mgr().send_time_feed_back_info();
		get_time_limit_gift_mgr().update_all_info();
		get_assist_fight_mgr().sync_assist_fight_state(true);
		get_assist_fight_mgr().check_assist_fight_boss_island();
		get_assist_fight_mgr().check_enter_assist_map();
		send_subscribe_daily_info();

		check_and_set_highest_record(e_role_history_highest_record_max_awaken_level, 0, 0);
		check_and_set_highest_record(e_role_history_highest_record_max_forge_level, 0, 0);
		check_and_set_highest_record(e_role_history_highest_record_max_equip_enchant_level, 0, 0);
		check_and_set_highest_record(e_role_history_highest_record_max_jewelry_enchant_level, 0, 0);

		set_first_npc_kill_id_arr();

		buy_message_tip(e_buy_tip_type_money);

		get_item_set().init_item_from_treasure_to_mail();
		get_time_limit_activity_mgr().send_limit_time_notice_info();
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
		if (vip_template_ptr != nullptr)
		{
			if (get_unit_info(e_role_info_daily_activity_degree) > vip_template_ptr->DailyMustMax)
			{
				set_unit_info(e_role_info_daily_activity_degree, vip_template_ptr->DailyMustMax);
				send_info_one(e_role_info_daily_activity_degree);
			}
		}
		server_log::jewel_count_log(get_unit_info_inst(), get_logic_info_inst(), get_third_info(), get_money_data_inst(), get_login_type());
	}

	int32 player::put_in_scene()
	{
		ZoneScoped;
		send_time_all();
		if (e_session_status_in_gaming == m_status)
		{
			return cs2ws_enter_scene::e_enter_ret_success;
		}

		if (m_status != e_session_status_client_loading)
		{
			CONSOLE_INFO("player::put_in_scene m_status = {}", (int32)m_status);
			return cs2ws_enter_scene::e_enter_ret_player_err;
		}

		auto base_map_cp = m_map_ent->get_component<base_map_component>();

		m_status = e_session_status_in_gaming;

		for (int32 i = 0; i < base_map_cp->m_map_template->MapBuffSlowIdArray.size(); i += 2)
		{
			if (i + 1 < base_map_cp->m_map_template->MapBuffSlowIdArray.size())
			{
				if (base_map_cp->m_map_template->MapBuffSlowIdArray[i + 1] > e_class_type_none && base_map_cp->m_map_template->MapBuffSlowIdArray[i + 1] != get_unit_info(e_role_info_class_type))
				{
					continue;
				}
			}
			buff_man::add_buff_inst(get_array_index(), get_array_index(), base_map_cp->m_map_template->MapBuffSlowIdArray[i]);
		}

		get_item_set().equip_on_equip_show_buff();
		get_mail_mgr().auto_load_mail();

		if (base_map_cp->m_map_template->HpOperateWhenIn)
		{
			double max_hp = get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
			change_hp(-max_hp, get_array_index(), false, 0);
		}
		return cs2ws_enter_scene::e_enter_ret_success;

	}

	void player::cost_enter_scene_consume()
	{
		ZoneScoped;

		auto base_map_cp = m_map_ent->get_component<base_map_component>();
		int32 player_vip_level = get_vip_level();

		if (base_map_cp->m_map_template->TransportPrice.size() >= player_vip_level + 1)
		{
			e_money_type money_type = (e_money_type)base_map_cp->m_map_template->TransportPrice[0];
			int32 money_num = base_map_cp->m_map_template->TransportPrice[player_vip_level + 1];
			cut_money(money_type, money_num, e_server_log_cut_money_teleport, base_map_cp->m_map_id);
		}
	}

	void player::fuben_map_enter_activity()
	{
		ZoneScoped;
		auto base_map_cp = m_map_ent->get_component<base_map_component>();

		//map_template_id = init_unit::get_map_id_by_diff_level(map_template_id, map_template_ptr->Type, map_template_ptr->GroupType);
		int32 must_do_type = -1;

		switch (base_map_cp->m_map_template->Type)
		{
		case e_map_type_broken_sky: //��¼������ս������
			this->set_must_do_count(e_daily_must_do_typ_angel_temple, this->get_must_do_count(e_daily_must_do_typ_angel_temple) + 1);
			this->send_daily_must_do_count_info_one(e_daily_must_do_typ_angel_temple);
			this->set_daily_active_degree_info(e_daily_must_do_typ_angel_temple);
			must_do_type = e_daily_must_do_typ_angel_temple;
			break;
		case e_map_type_king_of_pk: //PK֮��
			this->set_must_do_count(e_daily_must_do_typ_pk_king, this->get_must_do_count(e_daily_must_do_typ_pk_king) + 1);
			this->send_daily_must_do_count_info_one(e_daily_must_do_typ_pk_king);
			this->set_daily_active_degree_info(e_daily_must_do_typ_pk_king);
			must_do_type = e_daily_must_do_typ_pk_king;
			break;

		case e_map_type_arena: //������
			//this->set_must_do_count(e_daily_must_do_typ_arena, this->get_must_do_count(e_daily_must_do_typ_arena) + 1);
			//if (this->get_must_do_count(e_daily_must_do_typ_arena) == this->get_unit_info(e_role_info_arena_times))
			//{
			//	return;
			//}
			//if (is_need_daily_finish(e_daily_must_do_typ_arena) <= 0)
			//{
			//	return;
			//}
			//this->set_must_do_count(e_daily_must_do_typ_arena, this->get_must_do_count(e_daily_must_do_typ_arena) + 1);
			////this->set_must_do_count(e_daily_must_do_typ_arena, this->get_unit_info(e_role_info_arena_times));
			//this->send_daily_must_do_count_info_one(e_daily_must_do_typ_arena);
			//this->set_daily_active_degree_info(e_daily_must_do_typ_arena);
			must_do_type = e_daily_must_do_typ_arena;
			break;
			//case e_map_type_legion_boss: //����BOSS
			//case e_map_type_legion_station:
				//this->set_must_do_count(e_daily_must_do_typ_legion_boss, this->get_must_do_count(e_daily_must_do_typ_legion_boss) + 1);
				//this->send_daily_must_do_count_info_one(e_daily_must_do_typ_legion_boss);
				//this->set_daily_active_degree_info(e_daily_must_do_typ_legion_boss);
				//must_do_type = e_daily_must_do_typ_legion_boss;
				//break;
		case e_map_type_belief_cloister://��������
			this->set_must_do_count(e_daily_must_do_typ_belief_cloister, this->get_must_do_count(e_daily_must_do_typ_belief_cloister) + 1);
			this->send_daily_must_do_count_info_one(e_daily_must_do_typ_belief_cloister);
			this->set_daily_active_degree_info(e_daily_must_do_typ_belief_cloister);
			must_do_type = e_daily_must_do_typ_belief_cloister;
			break;
		case e_map_type_money_fuben://��Ҹ���
			this->set_must_do_count(e_daily_must_do_typ_raid_siliver, this->get_must_do_count(e_daily_must_do_typ_raid_siliver) + 1);
			this->send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_siliver);
			this->set_daily_active_degree_info(e_daily_must_do_typ_raid_siliver);
			must_do_type = e_daily_must_do_typ_raid_siliver;
			break;
		case e_map_type_fallen_fantasy://�������
			this->set_must_do_count(e_daily_must_do_typ_bloody_castle, this->get_must_do_count(e_daily_must_do_typ_bloody_castle) + 1);
			this->send_daily_must_do_count_info_one(e_daily_must_do_typ_bloody_castle);
			this->set_daily_active_degree_info(e_daily_must_do_typ_bloody_castle);
			must_do_type = e_daily_must_do_typ_bloody_castle;
			break;
		case e_map_type_abyss_of_fear://�־���Ԩ
			this->set_must_do_count(e_daily_must_do_typ_raid_multiple_abyss_of_fear, this->get_must_do_count(e_daily_must_do_typ_raid_multiple_abyss_of_fear) + 1);
			this->send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_multiple_abyss_of_fear);
			this->set_daily_active_degree_info(e_daily_must_do_typ_raid_multiple_abyss_of_fear);
			must_do_type = e_daily_must_do_typ_raid_multiple_abyss_of_fear;
			break;
		case e_map_type_star_test://��������
			this->set_must_do_count(e_daily_must_do_typ_raid_devil_square, this->get_must_do_count(e_daily_must_do_typ_raid_devil_square) + 1);
			this->send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_devil_square);
			this->set_daily_active_degree_info(e_daily_must_do_typ_raid_devil_square);
			must_do_type = e_daily_must_do_typ_raid_devil_square;
			break;
		case e_map_type_cross_server_pk: //��¼���ְҵJJC�������
			if (base_map_cp->m_map_template->SubType != 4)
			{
				this->set_must_do_count(e_daily_must_do_typ_cross_pk, this->get_must_do_count(e_daily_must_do_typ_cross_pk) + 1);
				this->send_daily_must_do_count_info_one(e_daily_must_do_typ_cross_pk);
				this->set_daily_active_degree_info(e_daily_must_do_typ_cross_pk);
				must_do_type = e_daily_must_do_typ_cross_pk;
			}
			break;
		default:
			break;
		}
		if (must_do_type >= 0)
		{
			server_log::activity_role_log(get_third_info(), m_role_info, must_do_type, base_map_cp->m_map_id);

		}
	}

	void player::set_daily_active_degree_info(e_daily_must_do_typ eType, int32 add_num)
	{
		ZoneScoped;
		if (eType >= e_daily_must_do_typ_max)
		{
			return;
		}
		int32 unit_index = m_welfare_mgr.get_unit_array();
		player& player_ref = unit_man::get_player(unit_index);
		int32 vip_level = get_vip_level();
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
		if (vip_template_ptr == nullptr)
		{
			return;
		}

		DailyActivitiesTemplate* template_ptr = template_manager::get_instance().get_template_by_daily_active_degree_type(eType);
		if (template_ptr == nullptr)
		{
			return;
		}

		int32 vip_add_num = 0;
		vector<int32> vip_add_array = template_ptr->VIPAddNum;
		if (vip_add_array.size() > vip_level && vip_level >= 0)
		{
			vip_add_num = vip_add_array[vip_level];
		}

		int32 cur_acyivity = player_ref.get_unit_info(e_role_info_daily_activity_degree); //��ǰ��Ծֵ
		int32 vip_active_degree_add = vip_template_ptr->LivenessBonuse; //vip��Ȩ
		int32 cur_count = player_ref.get_must_do_count(eType);  //��ǰ ����/���� ��ɴ���
		int32 target_value = template_ptr->TargetValue + vip_add_num;  //��Ҫ��� ����/���� ����

		if (add_num > 1)
		{
			if (cur_count > target_value && cur_count - add_num < target_value)
			{
				add_num -= cur_count - target_value;
				cur_count = target_value;
			}
		}
		else if (add_num < 1)
		{
			add_num = 1;
		}

		if (cur_count <= target_value)
		{
			int32 end_value = cur_acyivity + ((template_ptr->AddValue + vip_active_degree_add) * add_num);
			if (end_value > vip_template_ptr->DailyMustMax)
			{
				end_value = vip_template_ptr->DailyMustMax;
			}

			if (eType == e_daily_must_do_typ_raid_ancient_battlefield)
			{
				if (cur_count != target_value)
				{
					return;
				}
				m_add_ancient_activity_degree = true;
				send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_ancient_battlefield);
			}
			//ͬ����Ծ��
			player_ref.set_unit_info(e_role_info_daily_activity_degree, end_value);
			player_ref.send_info_one(e_role_info_daily_activity_degree);
			player_ref.get_star_trip_mgr().target_mission(e_star_trip_target_type_join_activity, eType);
			if (player_ref.get_write_log())
			{
				server_log::activity_degree_log(player_ref.m_role_info, player_ref.get_unit_info(e_role_info_daily_activity_degree));
				server_log::activity_role_log(player_ref.get_third_info(), player_ref.m_role_info, eType, 0);
			}
		}
	}
	int32 player::is_need_daily_finish(e_daily_must_do_typ eType)
	{
		ZoneScoped;
		if (eType >= e_daily_must_do_typ_max)
		{
			return 0;
		}
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
		if (vip_template_ptr == nullptr)
		{
			return 0;
		}
		DailyActivitiesTemplate* template_ptr = template_manager::get_instance().get_template_by_daily_active_degree_type(eType);
		if (template_ptr == nullptr)
		{
			return 0;
		}
		int32 cur_count = get_must_do_count(eType);
		int32 target_value = 0;
		/*if (eType == e_daily_must_do_typ_raid_exp)
		{
			target_value = vip_template_ptr->ExpRaidTime / 60 + get_time_data(e_time_type_exp_raid_extra_time) / 60;  //���鸱�� ��Ҫ��� ����/���� ����
		}
		else
		{*/
		target_value = template_ptr->TargetValue;  //��Ҫ��� ����/���� ����
		//}

		return target_value - cur_count;

	}

	bool player::is_can_levelup()
	{
		ZoneScoped;
		int32 exp_level = get_unit_info(e_role_info_exp_level);

		if (exp_level + 1 > init_unit::get_max_player_level())
		{
			return false;
		}
		return true;
	}

	player_grade_and_level player::get_grade_num_and_level_num()
	{
		ZoneScoped;
		player_grade_and_level grade_and_level;

		int32 cur_level = get_unit_info(e_role_info_exp_level);

		grade_and_level.grade_num = cur_level / player_grade_up_level_interval;
		grade_and_level.level_of_cur_grade = cur_level % player_grade_up_level_interval;

		return grade_and_level;
	}

	bool player::gm_set_level_and_grade(int32 level)
	{
		ZoneScoped;
		if (level < 1)
		{
			return false;
		}
		if (level >= role_max_level)
		{
			level = role_max_level - 1;
		}
		// ��þɵȼ�(��ǰ�ȼ�)���µȼ�
		int32 old_level = get_unit_info(e_role_info_exp_level);
		int32 new_level = level;

		// ��þɵȼ�(��ǰ�ȼ�)ģ��ID���µȼ�ģ��ID
		int32 old_upgrade_id = get_unit_info(e_role_info_upgrade_id);
		int32 new_upgrade_id = old_upgrade_id - old_level + new_level;

		// �����ܾ���ֵ�����õ�ǰ�ȼ�����
		//
		// �Ѿ����ܾ����ۼƵ��㷨�����ÿ�ζ���1����ָ���ȼ������ۼ�,�����������ڼ��ݽ���ʱ�����
		int64 exp_sum = 0;
		for (int32 i = old_upgrade_id - old_level + 1; i < new_upgrade_id; ++i)
		{
			PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, i);
			if (nullptr == upgrade_template_ptr)
			{
				return false;
			}

			exp_sum += init_unit::change_string_to_i64(upgrade_template_ptr->NextLevelExp);
		}
		m_money_info.all_data_ary[e_money_type_exp] = exp_sum;
		set_money_data(e_money_type_exp, 0);
		send_money_one(e_money_type_exp);

		// 		//��ֹlevelup����������ͼ����
		// 		int32 branch = get_unit_info(e_role_info_class_branch);
		// 		if (new_level > 300 && branch == e_branch_type_common)
		// 		{
		// 			set_unit_info(e_role_info_class_branch, e_branch_type_ice);
		// 			broadcast_info_one(e_role_info_class_branch);
		// 		}
		// 
		// 		int32 old_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);
		// 		get_item_set().equip_off_upgrade_show_buff();
		// 
		// 		// ���ý�ɫģ��
		// 		int32 old_grade_num = init_unit::get_player_grade_num(old_level);
		// 		int32 new_grade_num = init_unit::get_player_grade_num(new_level);
		// 		int32 cur_player_template_id = this->get_unit_info(e_role_info_template_id);
		// 		int32 new_player_template_id = cur_player_template_id - old_grade_num + new_grade_num;
				//PlayerInitTemplate* new_player_tmpl_ptr = GET_TEMPLATE(PlayerInitTemplate, new_player_template_id);
				//if (nullptr == new_player_tmpl_ptr)
				//{		
				//	return false;
				//}
		//		set_unit_info(e_role_info_template_id, new_player_template_id);
		set_unit_info(e_role_info_exp_level, new_level);
		set_unit_info(e_role_info_upgrade_id, new_upgrade_id);

		broadcast_info_one(e_role_info_template_id);
		broadcast_info_one(e_role_info_exp_level);
		broadcast_info_one(e_role_info_upgrade_id);
		m_mission_mgr.target_check(e_mission_end_type_reach_level);

		//		sync_data_to_ws(e_sync_cs2ws_data_player_template, new_player_template_id);
		sync_data_to_ws(e_sync_cs2ws_data_exp_level, new_level);

		// ʹ�µĵȼ�ģ����Ч
		m_pawn_att.update_unit_att_info(old_upgrade_id, false);
		m_pawn_att.update_unit_att_info(new_upgrade_id, true);

		//skill_manager::exp_level_up(get_unit_guid(), new_final_level);
		broadcast_info_one(e_role_info_exp_level);
		get_ranking_mgr().set_and_sync_ranking_data(e_RankingIndex_level, new_level);

		int32 new_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);
		//get_item_set().equip_on_show_buff();	//��װ����Чbuff

// 		if (new_grade_num != old_grade_num)
// 		{
// 			send_grade_up_response((int32)e_create_result_success, old_equip_buff_id, new_equip_buff_id);
// 		}
		// ����������������Ĳ���
		m_func_unlock_mgr.func_unlock_trigger_activate();
		get_item_set().equip_on_upgrade_show_buff();

		check_srv_world_level_buff();
		m_achievement_mgr.player_changed(eat_role_level_count);
		m_service_goal_mgr.refresh_over_time_all();
		return true;
	}

	bool player::gm_set_career(int32 career_id)
	{
		ZoneScoped;
		if (career_id < e_class_type_none || career_id > e_class_type_max)
		{
			return false;
		}

		int32 old_template_id = get_unit_info(e_role_info_template_id);
		int32 old_upgrade_id = get_unit_info(e_role_info_upgrade_id);
		int32 old_class = get_unit_info(e_role_info_class_type);
		int32 old_exp_level = get_unit_info(e_role_info_exp_level);
		PlayerInitTemplate* player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, old_template_id - old_class * role_max_level + career_id * role_max_level);
		if (!player_template_ptr)
		{
			return false;
		}
		m_pawn_att.update_unit_att_info(old_upgrade_id, false);
		m_pawn_att.update_unit_att_info(player_template_ptr->UpgradeId + old_exp_level, true);
		set_unit_info(e_role_info_template_id, old_template_id - old_class * role_max_level + career_id * role_max_level);
		set_unit_info(e_role_info_upgrade_id, player_template_ptr->UpgradeId + old_exp_level);
		send_info_one(e_role_info_upgrade_id);
		send_info_one(e_role_info_template_id);
		s_map_pos temp_pos = get_old_map_pos();

		m_teleport_control.teleport_by_template_pos(base_map_system::get_map_template_id(m_map_ent), get_line_id(), temp_pos, guid_64());
		return true;
	}

	bool player::gm_set_vip_point(int32 vip_point)
	{
		ZoneScoped;
		int32 old_vip_level = get_unit_info(e_role_info_vip_level);

		set_unit_info(e_role_info_recharge_num, vip_point);
		calculate_vip_level();

		int32 new_vip_level = get_unit_info(e_role_info_vip_level);

		vip_buff_change(old_vip_level, false);
		vip_buff_change(new_vip_level, true);
		refresh_vip_title();
		broadcast_info_one(e_role_info_vip_level);
		send_info_one(e_role_info_recharge_num);
		//ͬ��vip�ȼ���session
		sync_data_to_ws(e_sync_cs2ws_data_vip_level, new_vip_level);
		return true;
	}

	bool player::gm_set_grade_level(int32 grade_level, int32 branch)
	{
		ZoneScoped;
		if (grade_level <= 0)
		{
			grade_level = 1;
		}
		if (grade_level > init_unit::get_max_grade_level() * player_grade_up_stage_nums)
		{
			grade_level = init_unit::get_max_grade_level() * player_grade_up_stage_nums;
		}

		int32 player_index = grade_level / faith::player_grade_up_stage_nums;
		int32 player_class = get_unit_info(e_role_info_class_type);
		if (player_class < 1 || player_class > GAMECONFIG->InitRoleAry.size())
		{
			return false;
		}
		int32 player_init_template_id = GAMECONFIG->InitRoleAry[player_class - 1];

		set_unit_info(e_role_info_template_id, player_init_template_id + player_index);

		int32 old_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);
		get_item_set().equip_off_upgrade_show_buff();	//ȥ��װ����ʾ��Чbuff

		int32 old_class = get_unit_info(e_role_info_class_branch);
		if (player_index >= 3)
		{
			if (branch >= 1 && branch <= 4)
			{
				set_unit_info(e_role_info_class_branch, branch);
			}
			else
			{
				set_unit_info(e_role_info_class_branch, 1);
			}
		}
		//send_info_all();
		broadcast_info_one(e_role_info_template_id);
		broadcast_info_one(e_role_info_class_branch);

		// mission check
		m_mission_mgr.target_check(e_mission_end_type_character_level);

		// ͨ����Ϣ��WS(���ƾ��ų�Ա��Ϣͬ������������õ�)
		sync_data_to_ws(e_sync_cs2ws_data_player_template, player_init_template_id + player_index);

		m_achievement_mgr.player_changed(eat_role_transmigration_level_count);
		//m_spirit_mgr.auto_get_equip_slot();
		m_patron_saint_mgr.auto_get_equip_slot();

		int32 new_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);

		get_item_set().equip_on_upgrade_show_buff();	//����װ����Чbuff

		set_logic_data(e_role_logic_info_god_hood_grade, grade_level);

		send_logic_one(e_role_logic_info_god_hood_grade);

		refresh_service_goal(e_service_goal_type_god_hood_grade);

		send_grade_up_response((int32)e_grade_up_result_success, old_equip_buff_id, new_equip_buff_id);

		int32 grade_mission_id = 0;
		int32 temp_player_init_template_id = player_init_template_id + player_index;
		PlayerInitTemplate* temp_player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, temp_player_init_template_id);
		if (nullptr != temp_player_init_template_ptr)
		{
			int32 temp_value = grade_level % faith::player_grade_up_stage_nums;
			if (temp_value == 0)
			{
				PlayerInitTemplate* old_player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, temp_player_init_template_id - 1);
				if (nullptr != old_player_init_template_ptr)
				{
					grade_mission_id = old_player_init_template_ptr->GradeUpMission;
				}
			}
			else
			{
				if (temp_value <= temp_player_init_template_ptr->GradeBranch.size() && temp_value >= 1)
				{
					int32 grade_god_template_id = temp_player_init_template_ptr->GradeBranch[temp_value - 1];
					GradeGodHoodTemplate* grade_god_template_ptr = GET_TEMPLATE(GradeGodHoodTemplate, grade_god_template_id);
					if (nullptr != grade_god_template_ptr)
					{
						grade_mission_id = grade_god_template_ptr->GradeUpMission;
					}
				}
			}
		}

		if (grade_mission_id != 0)
		{
			m_mission_mgr.gm_set_grade_mission_id(grade_mission_id);
		}

		add_att_by_load();

		return true;
	}

	void player::begin_interact_with_npc(const guid_64 npc_guid)
	{
		ZoneScoped;
		if (npc_guid.is_valid() == false)
		{
			return;
		}
		npc& npc_ref = unit_man::get_npc(npc_guid);
		if (false == npc_ref.is_valid())
		{
			return;
		}
		NpcTemplate* npc_template_ptr = npc_ref.get_npc_template();
		if (nullptr == npc_template_ptr || e_unit_type_npc != npc_template_ptr->NpcType)
		{
			return;
		}
		if (is_self_server() == false && false == init_unit::is_cross_server_npc(npc_template_ptr->SubType)
			&& false == init_unit::is_all_server_npc(npc_template_ptr->SubType)
			&& !cell_server::getInstance().get_is_self_gate_server())
		{
			return;
		}

		if (is_arround_npc(&npc_ref) == false)
		{
			return;
		}

		switch (npc_template_ptr->SubType)
		{
		case e_npc_type_interact_gather:
			if (is_interact_with_npc())
			{
				return;
			}
			if (is_gathering_crystal_dreamland_chest(&npc_ref) && get_left_gather_crystal_dreamland_chest_count() <= 0)
			{
				return;
			}
			set_interact_npc_info(npc_template_ptr->InteractionTime, &npc_ref);
			break;

		case e_npc_type_touch_gather:
			finish_interact_with_npc(&npc_ref);
			break;

		case e_npc_type_world_boss_chests:
			if (is_interact_with_npc())
			{
				return;
			}
			if (is_gathering_world_boss_chest(&npc_ref) && get_left_gather_world_boss_chest() <= 0)
			{
				return;
			}
			set_interact_npc_info(npc_template_ptr->InteractionTime, &npc_ref);
			break;

		case e_npc_type_cross_pk_chests:
			if (is_interact_with_npc())
			{
				return;
			}
			if (!is_gathering_cross_pk_chest(&npc_ref))
			{
				return;
			}
			if (is_gathering_cross_pk_chest(&npc_ref) && m_boss_island_mgr.get_left_cross_pk_chest() <= 0)
			{
				const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305027);
				send_notice(notice_str);
				return;
			}
			set_interact_npc_info(npc_template_ptr->InteractionTime, &npc_ref);
			break;

		case e_npc_type_island_big_chests:
		case e_npc_type_island_small_chests:
			if (is_interact_with_npc())
			{
				return;
			}
			if (!is_gathering_boss_island_chest(&npc_ref))
			{
				return;
			}
			if (is_gathering_boss_island_chest(&npc_ref) && m_boss_island_mgr.get_left_boss_island_chest() <= 0)
			{
				const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305027);
				send_notice(notice_str);
				return;
			}
			set_interact_npc_info(npc_template_ptr->InteractionTime, &npc_ref);
			break;
		case e_npc_type_server_harry_chests:
			if (is_interact_with_npc())
			{
				return;
			}
			if (!is_gathering_harry_chest(&npc_ref))
			{
				return;
			}
			if (is_gathering_harry_chest(&npc_ref) && m_server_harry_mgr.is_have_item())
			{
				const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90203504);
				send_notice(notice_str);
				return;
			}
			if (is_gathering_harry_chest(&npc_ref) && !m_server_harry_mgr.is_in_harry_game_time())
			{
				const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90201845);
				send_notice(notice_str);
				return;
			}
			if (is_gathering_harry_chest(&npc_ref) && !m_server_harry_mgr.can_harry_item())
			{
				const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305027);
				send_notice(notice_str);
				return;
			}
			if (is_gathering_harry_chest(&npc_ref) && !m_server_harry_mgr.can_harry_item_with_server_money())
			{
				const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90203427);
				send_notice(notice_str);
				return;
			}
			set_interact_npc_info(npc_template_ptr->InteractionTime, &npc_ref);
			break;
		default:
			return;
		}
	}

	void player::interact_with_npc_tick(const int64 new_time)
	{
		ZoneScoped;
		if (is_interact_with_npc() != true)
		{
			return;
		}

		if (new_time >= m_interact_npc_info.finish_interact_time)
		{
			npc& target_npc = unit_man::get_npc(m_interact_npc_info.npc_array_index);
			if (target_npc.get_unit_guid() == m_interact_npc_info.npc_guid)
			{
				finish_interact_with_npc(&target_npc);
			}
			else
			{
				character_proto_finish_gather_result result_msg;
				m_interact_npc_info.clear_data();
				result_msg.set_finish_result(1);
				send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
			}
			stop_interact_with_npc(false);
		}
		else
		{
			npc& target_npc = unit_man::get_npc(m_interact_npc_info.npc_array_index);
			if (target_npc.get_unit_guid() != m_interact_npc_info.npc_guid)
			{
				stop_interact_with_npc();
			}
		}

	}

	void player::finish_interact_with_npc(npc* npc_ptr)
	{
		ZoneScoped;
		character_proto_finish_gather_result result_msg;
		if (nullptr == npc_ptr || false == npc_ptr->is_valid())
		{
			m_interact_npc_info.clear_data();
			result_msg.set_finish_result(1);
			send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
			return;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (nullptr == npc_template_ptr)
		{
			result_msg.set_finish_result(1);
			send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
			return;
		}
		if (is_arround_npc(npc_ptr) == false)
		{
			result_msg.set_finish_result(1);
			send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
			return;
		}
		// ˮ���þ�Ҫ���Ʋɼ�����
		if (is_gathering_crystal_dreamland_chest(npc_ptr))
		{
			int32 cur_gather_count = get_unit_info(e_role_info_crystal_dreamland_chests);
			if (cur_gather_count >= GAMECONFIG->MaxGatherCrystalCount)
			{
				result_msg.set_finish_result(1);
				send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
				return;
			}
			set_unit_info(e_role_info_crystal_dreamland_chests, cur_gather_count + 1);
			set_must_do_count(e_daily_must_do_typ_crystal_illusion, get_must_do_count(e_daily_must_do_typ_crystal_illusion) + 1);
			set_daily_active_degree_info(e_daily_must_do_typ_crystal_illusion);
			send_daily_must_do_count_info_one(e_daily_must_do_typ_crystal_illusion);
			send_info_one(e_role_info_crystal_dreamland_chests);

			server_log::activity_role_log(get_third_info(), m_role_info, e_daily_must_do_typ_crystal_illusion, 0);
			record_server_crystal_log();
		}

		// ����BOSS���䱦��Ҫ���Ʋɼ�����
		if (is_gathering_world_boss_chest(npc_ptr))
		{
			int32 cur_gather_count = get_unit_info(e_role_info_world_boss_chests);
			if (cur_gather_count >= GAMECONFIG->MaxWorldBossChests)
			{
				result_msg.set_finish_result(1);
				send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
				return;
			}
			set_unit_info(e_role_info_world_boss_chests, cur_gather_count + 1);
			send_info_one(e_role_info_world_boss_chests);
		}

		//����������䱦��ɼ�����
		if (is_gathering_cross_pk_chest(npc_ptr))
		{
			int32 cur_count = m_boss_island_mgr.get_boss_island_info(e_island_record_cross_pk_collect_count);
			ActivityCommonConfigTemplate* temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_pk);
			if (temp_ptr == nullptr || cur_count >= temp_ptr->ParamInt6)
			{
				result_msg.set_finish_result(1);
				send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
				return;
			}
			int32 new_count = cur_count + 1 > temp_ptr->ParamInt6 ? temp_ptr->ParamInt6 : cur_count + 1;
			m_boss_island_mgr.set_boss_island_info(e_island_record_cross_pk_collect_count, new_count);
			m_boss_island_mgr.send_player_boss_island_info_one(e_island_record_cross_pk_collect_count);
		}

		//���㵺�ɼ�����
		if (is_gathering_boss_island_chest(npc_ptr))
		{
			NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
			if (nullptr == npc_template_ptr)
			{
				result_msg.set_finish_result(1);
				send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
				return;
			}
			int32 cur_count = m_boss_island_mgr.get_boss_island_info(e_island_record_island_collect_count);
			if (e_npc_type_island_big_chests == npc_template_ptr->SubType)
			{
				if (cur_count >= GAMECONFIG->BossIslandMaxChest)
				{
					result_msg.set_finish_result(1);
					send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
					return;
				}
			}
			int32 new_count = cur_count + 1 > GAMECONFIG->BossIslandMaxChest ? GAMECONFIG->BossIslandMaxChest : cur_count + 1;
			m_boss_island_mgr.set_boss_island_info(e_island_record_island_collect_count, new_count);
			m_boss_island_mgr.send_player_boss_island_info_one(e_island_record_island_collect_count);
		}

		//����Ӷ�ɼ�
		if (is_gathering_harry_chest(npc_ptr))
		{
			NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
			if (nullptr == npc_template_ptr)
			{
				result_msg.set_finish_result(1);
				send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
				return;
			}
			if (false == m_server_harry_mgr.can_harry_item())
			{
				result_msg.set_finish_result(1);
				send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
				return;
			}
			if (false == m_server_harry_mgr.can_harry_item_with_server_money())
			{
				return;
			}
			m_server_harry_mgr.finish_gather_item();
		}

		std::vector<s_item_template_info> drop_item_list;
		cdrop::gen_drop_list_by_drop_id(npc_template_ptr->InteractAward, drop_item_list, get_unit_info(e_role_info_class_type), get_unit_info(e_role_info_exp_level));
		if (drop_item_list.size() > 0)
		{
			add_item_by_drop_items_list(drop_item_list);
		}

		int32 npc_template_id = npc_ptr->get_unit_info(e_role_info_template_id);
		send_interact_with_npc_state(e_npc_interact_state_finish, npc_template_id, npc_ptr->get_unit_guid());

		//�����be_dead��ȷ��NPC�������߼���������
		//int32 npc_array_index = npc_ptr->get_array_index();
		//unit_man::remove_npc(npc_array_index, false);

		if (npc_template_ptr->InteractBuff > 0)
		{
			buff_man::del_buff_by_template_id(get_array_index(), get_array_index(), npc_template_ptr->InteractBuff);//ɾ��������buff
		}
		if (!is_gathering_harry_chest(npc_ptr))//����Ӷ��NPC���ɼ���������
		{
			if (npc_template_ptr != nullptr)//��������ɱ�߼�buff
			{
				std::vector<int32> buff_id_arr = npc_template_ptr->BuffIdWithKill;
				if (buff_id_arr.size() > 0)
				{
					for (int32 i = 0; i < buff_id_arr.size(); i++)
					{
						buff_man::add_buff_inst(get_array_index(), get_array_index(), buff_id_arr[i]);
					}
				}
			}
			npc_ptr->be_dead();
		}
		get_mission_mgr().target_check(e_mission_end_type_npc_kill, npc_template_id);

		result_msg.set_finish_result(0);
		send_message_to_self(&result_msg, e_msgindex_s2c_finish_gather_result);
		int32 item_len = drop_item_list.size();
		int32 notice_id_item = npc_template_ptr->Noticeld;
		if (notice_id_item > 0 && item_len > 0)
		{
			s_item_info item_data[chat_max_item];
			std::string item_names_str;
			item_names_str.reserve(100);
			int32 item_data_num = 0;
			for (int32 i = 0; i < item_len; i++)
			{
				int32 item_temp_id = drop_item_list[i].m_item_id;
				citem* item_ptr = get_item_set().get_item_by_template_id(item_temp_id);
				if (nullptr != item_ptr)
				{
					std::vector<citem*> items_ptr_array;
					items_ptr_array.clear();
					items_ptr_array.push_back(item_ptr);
					if (item_data_num < chat_max_item)
					{
						item_data[item_data_num] = get_item_set().filter_items_with_notice(items_ptr_array, item_names_str, notice_id_item);
						if (item_data[item_data_num].item_guid.is_valid())
						{
							item_data_num++;
						}
					}
					if (chat_max_item <= item_data_num)
					{
						std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id_item);
						std::vector<std::string> notice_str_params_vec;
						notice_str_params_vec.push_back(notice_str_id);
						notice_str_params_vec.push_back(get_name());
						notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(npc_template_ptr->NpcName));
						notice_str_params_vec.push_back(item_names_str);
						std::string notice_str = init_unit::implode(notice_str_params_vec);
						get_chat_mgr().send_notice(notice_id_item, notice_str, item_data, item_data_num - 1);
						item_data_num = 0;
						item_names_str.clear();
						memset(item_data, 0, sizeof(item_data));
					}
				}
			}
			if (item_data_num > 0)
			{
				std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id_item);
				std::vector<std::string> notice_str_params_vec;
				notice_str_params_vec.push_back(notice_str_id);
				notice_str_params_vec.push_back(get_name());
				notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(npc_template_ptr->NpcName));
				notice_str_params_vec.push_back(item_names_str);
				std::string notice_str = init_unit::implode(notice_str_params_vec);
				get_chat_mgr().send_notice(notice_id_item, notice_str, item_data, item_data_num - 1);
			}
		}
		//����ǽ����ɼ��� -- 9��14  ��ʹ�ÿ���������ʾ
		if (e_npc_type_interact_gather == npc_template_ptr->SubType || e_npc_type_world_boss_chests == npc_template_ptr->SubType
			|| e_npc_type_island_big_chests == npc_template_ptr->SubType
			|| e_npc_type_island_small_chests == npc_template_ptr->SubType
			|| e_npc_type_cross_pk_chests == npc_template_ptr->SubType)
		{
			std::vector<s_item_template_info> data_array;
			for (int32 index = 0; index < drop_item_list.size(); index++)
			{
				ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, drop_item_list[index].m_item_id);
				if (nullptr == item_template_ptr)
				{
					continue;
				}

				data_array.push_back({ drop_item_list[index].m_item_id ,drop_item_list[index].m_item_num,item_template_ptr->locked });
			}
			get_item_set().get_item_send_promp_msg_to_client(data_array);
		}
	}

	void player::set_interact_npc_info(const float interact_time, npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr || npc_ptr->is_valid() == false)
		{
			return;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (npc_template_ptr == nullptr)
		{
			return;
		}
		if (is_interact_with_npc())
		{
			return;
		}
		trigger_unit_event(e_game_event_type_unit_move);//�ɼ�Ҳ���ƶ�
		m_interact_npc_info.is_interact = true;
		m_interact_npc_info.finish_interact_time = init_unit::get_end_time(interact_time);
		m_interact_npc_info.npc_guid = npc_ptr->get_unit_guid();
		m_interact_npc_info.npc_array_index = npc_ptr->get_array_index();
		if (npc_ptr->get_npc_template() != nullptr)
		{
			m_interact_npc_info.npc_template = *npc_ptr->get_npc_template();
		}
		else
		{
			m_interact_npc_info.npc_template = NpcTemplate();
		}

		if (npc_template_ptr->Capsule.size() > 1)
		{
			m_interact_npc_info.max_distance = default_gather_distance + npc_template_ptr->Capsule[1];
		}
		if (m_interact_npc_info.npc_template.InteractBuff > 0)
		{
			buff_man::add_buff_inst(get_array_index(), get_array_index(), m_interact_npc_info.npc_template.InteractBuff);//�ɼ���Ʒʱ��õ�buff
		}
		int32 npc_template_id = npc_ptr->get_npc_template_id();
		get_meditation_mgr().break_meditation();
		send_interact_with_npc_state(e_npc_interact_state_begin, npc_template_id, npc_ptr->get_unit_guid());
	}

	void player::stop_interact_with_npc(bool send_msg, bool is_move)
	{
		ZoneScoped;
		// ���жϳ�ս ����ǳ�ս��ͼҪ������ͼ���ݽṹ�еĽ�����Ϣ
		//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(m_map_guid);
		//if (map_object_ptr)
		//{
		//	if (map_object_ptr->get_map_type() == e_map_type_city_war)
		//	{
		//		city_war_map_cs* city_war_map_ptr = (city_war_map_cs*)map_object_ptr;
		//		city_war_map_ptr->remove_npc_interaction_tick_node(get_unit_guid());
		//	}
		//	else if (map_object_ptr->get_map_type() == e_map_type_attack_city)
		//	{
		//		attack_city_map_cs* attack_city_map_ptr = (attack_city_map_cs*)map_object_ptr;
		//		if (nullptr != attack_city_map_ptr)
		//		{
		//			attack_city_map_ptr->remove_interaction_and_transfer(get_unit_guid(), is_move);
		//		}
		//	}
		//}

		if (m_interact_npc_info.is_interact == false)
		{
			return;
		}
		if (m_interact_npc_info.npc_template.InteractBuff > 0)
		{
			buff_man::del_buff_by_template_id(get_array_index(), get_array_index(), m_interact_npc_info.npc_template.InteractBuff);//ɾ��������buff
		}
		int32 npc_template_id = m_interact_npc_info.npc_template.attribute_id;
		guid_64 npc_guid = m_interact_npc_info.npc_guid;
		m_interact_npc_info.clear_data();
		if (send_msg)
		{
			send_interact_with_npc_state(e_npc_interact_state_stop, npc_template_id, npc_guid);
		}
	}

	bool player::is_gathering_crystal_dreamland_chest(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr || npc_ptr->is_valid() == false)
		{
			return false;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (nullptr == npc_template_ptr)
		{
			return false;
		}

		if (e_unit_type_npc == npc_template_ptr->NpcType
			&& (e_npc_type_interact_gather == npc_template_ptr->SubType)
			&& e_map_type_crystak_dreamland == base_map_system::get_map_type(get_map_ent()))
		{
			return true;
		}

		return false;
	}

	bool player::is_gathering_cross_pk_chest(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr || npc_ptr->is_valid() == false)
		{
			return false;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (nullptr == npc_template_ptr)
		{
			return false;
		}
		auto map_type = base_map_system::get_map_type(get_map_ent());
		auto map_sub_type = base_map_system::get_map_sub_type(get_map_ent());

		if (e_unit_type_npc == npc_template_ptr->NpcType
			&& e_npc_type_cross_pk_chests == npc_template_ptr->SubType
			&& e_map_type_cross_server_pk == map_type && map_sub_type != 4)
		{
			return true;
		}
		return false;
	}

	bool player::is_gathering_harry_chest(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr || npc_ptr->is_valid() == false)
		{
			return false;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (nullptr == npc_template_ptr)
		{
			return false;
		}

		if (e_unit_type_npc == npc_template_ptr->NpcType
			&& e_npc_type_server_harry_chests == npc_template_ptr->SubType
			&& e_map_type_cross_server_harry == base_map_system::get_map_type(get_map_ent()))
		{
			return true;
		}
		return false;
	}

	bool player::is_gathering_boss_island_chest(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr || npc_ptr->is_valid() == false)
		{
			return false;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (nullptr == npc_template_ptr)
		{
			return false;
		}

		auto map_type = base_map_system::get_map_type(get_map_ent());
		if (e_unit_type_npc == npc_template_ptr->NpcType
			&& (e_npc_type_island_small_chests == npc_template_ptr->SubType || e_npc_type_island_big_chests == npc_template_ptr->SubType)
			&& e_map_type_boss_island == map_type)
		{
			return true;
		}
		return false;
	}

	bool player::is_gathering_world_boss_chest(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr || npc_ptr->is_valid() == false)
		{
			return false;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (nullptr == npc_template_ptr)
		{
			return false;
		}

		auto map_type = base_map_system::get_map_type(get_map_ent());
		if (e_unit_type_npc == npc_template_ptr->NpcType
			&& (e_npc_type_world_boss_chests == npc_template_ptr->SubType)
			&& e_map_type_big_map == map_type)
		{
			return true;
		}
		return false;
	}

	int32 player::get_left_gather_crystal_dreamland_chest_count()
	{
		ZoneScoped;
		int32 cur_gather_count = get_unit_info(e_role_info_crystal_dreamland_chests);
		return (GAMECONFIG->MaxGatherCrystalCount - cur_gather_count);
	}

	int32 player::get_left_gather_world_boss_chest()
	{
		ZoneScoped;
		int32 cur_gather_count = get_unit_info(e_role_info_world_boss_chests);
		return (GAMECONFIG->MaxWorldBossChests - cur_gather_count);
	}

	bool player::is_arround_npc(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr || npc_ptr->is_valid() == false)
		{
			return false;
		}
		NpcTemplate* npc_template_ptr = npc_ptr->get_npc_template();
		if (npc_template_ptr == nullptr)
		{
			return false;
		}

		s_map_pos npc_pos = npc_ptr->get_new_map_pos();
		int32 temp_distance = default_gather_distance;
		if (npc_template_ptr->Capsule.size() > 1)
		{
			temp_distance += npc_template_ptr->Capsule[1];
		}

		// �жϵ�ǰ��ɫ�Ƿ���Ŀ��NPC�����ľ�������
		if (is_arround_pos(npc_pos, temp_distance))
		{
			return true;
		}

		return false;
	}

	void player::send_interact_with_npc_state(e_npc_interact_state interact_state, int32 npc_template_id, guid_64 npc_guid)
	{
		ZoneScoped;
		if (interact_state <= e_npc_interact_state_none || e_npc_interact_state_max <= interact_state)
		{
			return;
		}

		character_proto_interact_with_npc_state interact_with_npc_state_msg;
		interact_with_npc_state_msg.set_interact_state(interact_state);
		interact_with_npc_state_msg.set_npc_template_id(npc_template_id);
		interact_with_npc_state_msg.set_npc_guid(npc_guid.server_64);
		send_message_to_self(&interact_with_npc_state_msg, e_msgindex_s2c_interact_with_npc_state);

		character_proto_aoi_npc_gather_state update_npc_gather_msg;
		update_npc_gather_msg.set_interact_state(interact_state);
		update_npc_gather_msg.set_npc_guid(npc_guid.server_64);
		send_message_to_aoi(&update_npc_gather_msg, e_msgindex_s2c_aoi_npc_gather_state);
	}

	int32 player::get_cur_player_world_level_exp()
	{
		ZoneScoped;
		int32 default_level = 30;
		int32 world_level = m_cur_world_level;
		if (world_level <= 0)
		{
			world_level = default_level;
		}
		int32 upgrade_id = player_upgrade_cfg_first_id + world_level - 1;
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_id);
		if (nullptr == template_ptr)
		{
			return 0;
		}
		return template_ptr->BaseExp;
	}

	void player::set_cur_player_world_level()
	{
		ZoneScoped;
		int32 cur_world_level = cell_server::getInstance().get_world_level_last();
		m_cur_world_level = cur_world_level;

		if (GAMECONFIG->DelayLevelHour.size() <= 0 || GAMECONFIG->DelayLevelHour.size() % 2 != 0)
		{
			return;
		}
		int32 server_open_time = cell_server::getInstance().get_server_open_time();
		int32 role_create_time = get_unit_i64_info_data(e_role_i64_info_create_time);
		int32 diff_time = role_create_time - server_open_time;
		if (diff_time <= 0)
		{
			return;
		}
		int32 diff_hour = diff_time / 3600;
		int32 diff_days = 0;
		for (int32 i = 0; i < GAMECONFIG->DelayLevelHour.size() / 2; i++)
		{
			if (diff_hour >= GAMECONFIG->DelayLevelHour[i * 2])
			{
				diff_days = GAMECONFIG->DelayLevelHour[i * 2 + 1];
			}
		}
		int32 player_login_days = get_unit_info(e_role_info_total_login_days) + diff_days;

		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr)
		{
			return;
		}
		int32 region = region_template_ptr->RegionCode;
		int32 size = GAMECONFIG->ExpServerBeginLevel.size();

		int32 region_max = (int32)e_version_region_type_max;
		if (size == 0 || size % region_max != 0)
		{
			return;
		}
		int32 per_max = size / region_max;
		int32 length = 0;

		for (int32 i = 0; i < per_max; i++)
		{
			if (GAMECONFIG->ExpServerBeginLevel[i * region_max + region] == 0)
			{
				break;
			}
			length++;
		}

		if (player_login_days <= 1)
		{
			m_cur_world_level = 0;
			return;
		}
		else if (player_login_days <= length + 1)
		{
			int32 offset = player_login_days - 2;
			if (offset < 0 || offset >= per_max)
			{
				return;
			}
			m_cur_world_level = GAMECONFIG->ExpServerBeginLevel[offset * region_max + region];
			return;
		}
		else
		{
			return;
		}

	}

	int64 player::add_exp(int64 exp_value, bool is_need_show, bool gm, bool grade_up, float extra_fix, bool is_add_by_kill_npc)
	{
		ZoneScoped;
		if (exp_value <= 0)
		{
			return exp_value;
		}
		int32 count = 0;
		bool IsLevelUp = false;
		bool IsLevelMax = false;
		PlayerInitTemplate* player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, get_unit_info(e_role_info_template_id));
		if (nullptr == player_init_template_ptr)
		{
			return exp_value;
		}
		int32 cur_level_time = get_time_data(e_time_type_cur_level_time);	//�˴��ѱ仯ʱ���������ӡLOGʹ��
		int32 total_old_exp_level = get_unit_info(e_role_info_exp_level);
		int32 total_old_gs = get_unit_gs_value();

		float add_percent = 0;
		float add_all = 0;
		int32 buff_template_id = -1;
		add_all = get_pawn_att().get_attack_att_value(e_unit_attack_att_exp_add_all);
		exp_value *= (1 + add_all + extra_fix);

		globle_data::get_instance().get_exp_add_percent(total_old_exp_level, m_cur_world_level, cell_server::getInstance().get_world_level_cur(), add_percent, buff_template_id);

		if (!grade_up)//תְ�Ļ�����Ͳ��ӳ���
		{
			exp_value += exp_value * add_percent;
		}
		int64 showed_value = exp_value;//������Ƿ����ͻ��˵�ʵ�ʾ���ֵ��������
		bool is_show_max_exp = false;
		add_money(e_money_type_exp, exp_value, e_server_log_add_money_add_exp, -1, false);
		int32 old_upgrade_id = get_unit_info(e_role_info_upgrade_id);


		if (calcu_level_up() > 0)
		{
			//�ȼ������ж��Ƿ��������������Ϊ�ﵽ�ȼ�������
			cmission_mgr& mission_mgr_ref = get_mission_mgr();
			cmission* mission_ptr = mission_mgr_ref.find_mission_by_index(e_mission_slot_main);
			if (nullptr != mission_ptr)
			{
				MissionTemplate* mission_template_ptr = mission_ptr->get_mission_template_ptr();
				if (mission_template_ptr != nullptr)
				{
					if (mission_template_ptr->MissionEndType == e_mission_end_type_reach_level)
					{
						mission_mgr_ref.target_check(e_mission_end_type_reach_level);
						auto map_type = base_map_system::get_map_type(get_map_ent());
						if (map_type == e_map_type_level_up_map)
						{
							MissionTemplate* next_mission_ptr = GET_TEMPLATE(MissionTemplate, mission_template_ptr->NextMissionId);
							if (nullptr != next_mission_ptr)
							{
								if (get_unit_info(e_role_info_exp_level) >= next_mission_ptr->NeedLevel)
								{
									transfer_by_template(get_unit_info(faith::e_role_info_main_map_id), 0, 0, guid_64(), 0);
								}
							}
						}
					}
				}
			}

			// ����!����ҵ�ǰ�ȼ�������ģ���������һ��������ģ��
			m_pawn_att.update_unit_att_info(old_upgrade_id, false);
			m_pawn_att.update_unit_att_info(get_unit_info(e_role_info_upgrade_id), true);

			buff_man::add_buff_inst(get_array_index(), get_array_index(), GAMECONFIG->LevelUpBuffId); //��������Чbuff
			m_pawn_att.exp_levelup();
			m_achievement_mgr.player_changed(eat_role_level_count);
			broadcast_info_one(e_role_info_exp_level);
			send_info_one(e_role_info_upgrade_id);

			int32 new_level = get_unit_info(e_role_info_exp_level);

			// ��������ǲ����ܼ���һЩ����
			m_func_unlock_mgr.func_unlock_trigger_activate();
			m_mission_mgr.check_and_send_all_mission();

			//�������а�����
			m_ranking_mgr.set_and_sync_ranking_data(e_RankingIndex_level, new_level);

			get_time_limit_activity_mgr().check_send_new_server_mail();

			sync_data_to_ws(e_sync_cs2ws_data_exp_level, new_level);

			if (get_write_log())
			{
				int64 login_time = get_login_time();
				int64 online_time = utility::get_tick_count() - login_time;
				server_log::level_up_role_log(m_login_third_data, m_role_info, total_old_exp_level, get_unit_info(e_role_info_exp_level), cur_level_time, total_old_gs, online_time, m_login_type);
				set_log_var(log_head);
				get_log_common_head_info(log_head);
				server_log::serverRoleUpgrade(log_head, get_unit_info(e_role_info_exp_level), cur_level_time);
			}
			check_srv_world_level_buff();
			m_mission_mgr.target_check(e_mission_end_type_character_level);
			m_service_goal_mgr.refresh_over_time_all();
		}

		int64 exp_val = 0;
		if (is_show_max_exp)
		{
			exp_val = exp_value;
		}
		else
		{
			exp_val = showed_value;
		}

		if (is_need_show)
		{
			float temp_extra_num = (1 + extra_fix + add_all) * (1 + add_percent) - 1;
			character_proto_show_money_get show_msg;
			show_msg.set_money_percent(add_percent);
			show_msg.set_money_type(e_money_type_exp);
			show_msg.set_extra_percent(temp_extra_num);
			show_msg.set_is_add_by_kill_monster(is_add_by_kill_npc);
			if (is_show_max_exp)
			{
				show_msg.set_money_num(exp_val);
			}
			else
			{
				show_msg.set_money_num(exp_val);
			}

			auto map_type = base_map_system::get_map_type(get_map_ent());
			if (false == init_unit::get_map_public(map_type) && true == is_add_by_kill_npc)
			{
				m_get_gain_exp_in_raid += exp_val;
			}

			send_message_to_self(&show_msg, e_msgindex_s2c_show_money_get);
		}
		else
		{
			send_money_one(e_money_type_exp);
		}
		if (is_add_by_kill_npc)
		{
			set_relic_exp(exp_val);
		}
		refresh_service_goal(e_service_goal_type_role_level);
		return showed_value;
	}

	xstring player::lua_add_exp(xstring str_exp_value, bool is_need_show, bool gm, bool grade_up, float extra_fix, bool is_add_by_kill_npc)
	{
		ZoneScoped;
		int64 exp_value = init_unit::change_string_to_i64(str_exp_value);
		int64 value = add_exp(exp_value, is_need_show, gm, grade_up, extra_fix, is_add_by_kill_npc);
		return init_unit::change_i64_to_string(value);
	}

	int64 player::get_level_up_exp()
	{
		ZoneScoped;
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, get_unit_info(e_role_info_upgrade_id));
		if (nullptr == template_ptr)
		{
			return 0;
		}
		int64 exp_value = get_money_data(e_money_type_exp);
		int64 to_next_level_exp_req = init_unit::change_string_to_i64(template_ptr->NextLevelExp);
		int64 level_up_exp = to_next_level_exp_req - exp_value + 100;
		return level_up_exp > 0 ? level_up_exp : 1;
	}
	bool player::set_level_up_exp()
	{
		ZoneScoped;
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, get_unit_info(e_role_info_upgrade_id));
		if (nullptr == template_ptr)
		{
			return false;
		}
		int32 old_exp_level = get_unit_info(e_role_info_exp_level);
		int64 to_next_level_exp_req = init_unit::change_string_to_i64(template_ptr->NextLevelExp);
		int64 exp_value = get_money_data(e_money_type_exp);
		if (exp_value < to_next_level_exp_req)
		{
			set_money_data(e_money_type_exp, to_next_level_exp_req);
			add_exp(1, true);
		}
		else
		{
			add_exp(1, true);
			template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, get_unit_info(e_role_info_upgrade_id));
			to_next_level_exp_req = init_unit::change_string_to_i64(template_ptr->NextLevelExp);
			set_money_data(e_money_type_exp, to_next_level_exp_req);
			add_exp(1, true);
		}
		return true;
	}


	int32 player::calcu_level_up()
	{
		ZoneScoped;
		int64 exp_value = get_money_data(e_money_type_exp);
		int32 level_up_num = 0;
		int32 add_talent_num = 0;
		while (true)
		{
			PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, get_unit_info(e_role_info_upgrade_id));
			if (nullptr == template_ptr)
			{
				break;
			}

			int64 to_next_level_exp_req = init_unit::change_string_to_i64(template_ptr->NextLevelExp);
			if (!is_can_levelup()
				|| exp_value < to_next_level_exp_req)
			{
				//���������ۼƾ���
				//add_money(e_money_type_exp, exp_value, e_server_log_add_money_add_exp);
				break;
			}

			level_up_num++;
			exp_value -= to_next_level_exp_req;
			int32 old_upgrade_id = get_unit_info(e_role_info_upgrade_id);
			int32 old_exp_level = get_unit_info(e_role_info_exp_level);

			//�ȼ����Ӳ鿴�Ƿ��пɻ�óƺ�
			m_title_mgr.add_title_by_type_and_value(faith::e_title_type_level, get_unit_info(e_role_info_exp_level));

			set_unit_info(e_role_info_upgrade_id, old_upgrade_id + 1);
			set_unit_info(e_role_info_exp_level, old_exp_level + 1);


			set_time_data(e_time_type_cur_level_time, 0);

			set_money_data(e_money_type_exp, exp_value);

			//  ���츳��
			const PlayerUpgradeTemplate* player_upgrade_template_ptr = template_manager::get_instance().get_player_upgrade_by_level(get_unit_info(e_role_info_class_type), get_unit_info(e_role_info_exp_level));
			if (nullptr != player_upgrade_template_ptr)
			{
				if (0 != player_upgrade_template_ptr->TalentNum)
				{
					int64 talent_num = player_upgrade_template_ptr->TalentNum;
					add_money_or_exp(e_money_type_talent, talent_num, e_server_log_add_money_exp_level_up);
					add_talent_num += talent_num;
				}
			}
			http_send::get_instance().send_role_level(SERVERCONFIG->app_key, m_login_third_data.param7, "", m_role_info.account, SERVERCONFIG->game_id, m_role_info.role_guid, get_unit_info(e_role_info_exp_level));
			send_mail_with_level(old_exp_level + 1);
		}
		send_info_one(e_role_info_upgrade_id);
		send_money_one(e_money_type_exp);
		if (true == m_func_unlock_mgr.is_func_unlock("Talent") && add_talent_num > 0)
		{
			send_add_money_msg(e_money_type_talent, add_talent_num);
		}
		return level_up_num;
	}

	void player::send_mail_with_level(int32 cur_level)
	{
		ZoneScoped;
		int32 rule_level = GAMECONFIG->SendGiftMailLevel;
		if (cur_level != rule_level)
		{
			return;
		}
		RegionTemplate* region_temp_ptr = globle_data::get_instance().get_region_template_ptr();
		if (nullptr == region_temp_ptr)
		{
			return;
		}
		int32 region_code = region_temp_ptr->attribute_id;
		if (region_code == e_version_region_type_tha)
		{
			item_set& temp_item_set = get_item_set();
			std::vector<citem*> item_list;
			std::vector<int32> item_list_arr = GAMECONFIG->TlGiftMailItemList;
			if (item_list_arr.size() <= 0 || item_list_arr.size() % 2 != 0)
			{
				return;
			}
			for (int32 i = 0; i < item_list_arr.size() / 2; i++)
			{
				int32 award_template_id = item_list_arr[i * 2];
				int32 award_num = item_list_arr[i * 2 + 1];
				citem* award_item = temp_item_set.create_item_by_template(e_server_log_add_item_mail, cur_level, award_template_id, award_num, 1);
				if (nullptr == award_item)
				{
					return;
				}
				item_list.push_back(award_item);
			}
			std::string title_id = "90204382";
			std::string context_id = "90204383";
			get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), title_id, context_id, 0, 0, 0, 0, item_list);
		}
	}

	void player::change_pk_king_title()
	{
		ZoneScoped;
		bool is_have = get_title_mgr().is_have_title(pk_king_title_id);
		if (is_have)
		{
			get_title_mgr().equip_on_title(pk_king_title_id);
		}
	}

	void player::set_relic_exp(int64 exp_val)
	{
		ZoneScoped;
		auto map_type = base_map_system::get_map_type(get_map_ent());
		if (e_map_type_field != map_type)
		{
			return;
		}
		m_get_relic_exp += exp_val;
		set_unit_info(e_role_info_relic_exp_num, m_get_relic_exp / 10000);
		send_info_one(e_role_info_relic_exp_num);
	}

	void player::req_relation_end(int32 relation_type, guid_64 target_guid)
	{
		ZoneScoped;
		switch (relation_type)
		{
		case e_relationlist_type_friend:
			get_mission_mgr().target_check(e_mission_end_type_add_friend);
			refresh_service_goal(e_service_goal_type_friend_num);
			break;
		case e_relationlist_type_black:
			break;
		case e_relationlist_type_marry:
			break;
		case e_relationlist_type_hate:
			break;
		case e_relationlist_type_friendex:
			break;
		case e_relationlist_type_other_req_add:
			break;
		case e_relationlist_type_chat:
			break;
		default:
			break;
		}
	}

	void player::add_exp_on_kill_npc(npc& dead_npc_ref)
	{
		ZoneScoped;
		if (e_unit_type_trap == dead_npc_ref.get_owner_unit_type())//����unit���Ͳ����ԼӾ���
		{
			return;
		}
		auto map_type = base_map_system::get_map_type(dead_npc_ref.get_map_ent());
		if (false == init_unit::is_team_exp_map(map_type))
		{
			add_exp_with_fix(get_base_exp_of_dead_npc(dead_npc_ref));
			return;
		}
		if (is_player_in_team())
		{
			get_team_cs_mgr().share_team_exp(&dead_npc_ref);
		}
		else
		{
			add_exp_with_fix(get_base_exp_of_dead_npc(dead_npc_ref));
		}

		auto cur_map_type = base_map_system::get_map_type(get_map_ent());

		if (e_map_type_field == cur_map_type)
		{
			int32 cur_kill_num = get_unit_info(e_role_info_relic_kill_num);
			int32 new_kill_num = cur_kill_num + 1;
			set_unit_info(e_role_info_relic_kill_num, new_kill_num);
			send_info_one(e_role_info_relic_kill_num);
		}
	}

	int64 player::add_exp_with_fix(int64 exp_val, float extra_fix)
	{
		ZoneScoped;
		if (false == can_add_exp_from_monster())
		{
			return 0;
		}

		float fix_rate = 0.0f;

		//��ؼӳ�
		if (get_legion_guid().is_valid())
		{
			fix_rate += get_legion_cs_mgr().get_cur_territory_exp_fix();
		}

		//���Լӳ�
		fix_rate += get_pawn_att().get_attack_att_value(e_unit_attack_att_exp_addition);

		//����ӳ�
		fix_rate += extra_fix;

		return add_exp(exp_val, true, false, false, fix_rate, true);


	}


	bool player::can_add_exp_from_monster()
	{
		ZoneScoped;
		auto map_type = base_map_system::get_map_type(get_map_ent());
		switch (map_type)
		{
			//case e_map_type_exp_fuben:
			//{
			//	bool is_assist_state = get_team_cs_mgr().is_assist_in_raid();
			//	if (is_assist_state)
			//	{
			//		return false;
			//	}
			//	break;
			//}
		case e_map_type_field:
		case e_map_type_belief_cloister:
		case e_map_type_crystak_dreamland:
		{
			//if (map_object_ptr->get_time_left(get_array_index()) <= 0)
			//{
			//	return false;
			//}
		}
		default:
			break;
		}

		return true;
	}

	bool player::can_get_drop_from_monster()
	{
		ZoneScoped;
		auto map_type = base_map_system::get_map_type(get_map_ent());

		switch (map_type)
		{
		case e_map_type_field:
		case e_map_type_crystak_dreamland:
		{
			//if (map_object_ptr->get_time_left(get_array_index()) <= 0)
			//{
			//	return false;
			//}
		}
		break;
		case e_map_type_belief_cloister:
			ActivityCommonConfigTemplate* ActivityCommonConfigTemplatePtr = GET_TEMPLATE(ActivityCommonConfigTemplate, belief_cloister_attributeid_in_activity_tbl);
			if (ActivityCommonConfigTemplatePtr == nullptr)
			{
				return false;
			}
			if (get_belief_rune_mgr().get_stage_point() >= ActivityCommonConfigTemplatePtr->ParamInt1)
			{
				return false;
			}
			break;
		}
		//MapTemplate* map_temp_ptr = map_object_ptr->get_map_template_ptr();
		//if (nullptr == map_temp_ptr)
		//{
		//	return false;
		//}
		//if (e_consume_enter_count_cond_enter == map_temp_ptr->ConsumeEnterCounterCond)
		//{
		//	bool is_not_drop = get_team_cs_mgr().get_is_not_drop();
		//	if (is_not_drop)
		//	{
		//		return false;
		//	}
		//}
		//if (map_temp_ptr->Type == e_map_type_legion_station)
		//{
		//	if (true == is_get_kill_legion_boss_award_today())
		//	{
		//		return false;
		//	}
		//}
		return true;
	}

	bool player::add_att_permanent(int32 att_type, int32 add_val)
	{
		ZoneScoped;
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, get_unit_info(e_role_info_upgrade_id));
		if (nullptr == template_ptr)
		{
			return false;
		}
		int32 temp_len = template_ptr->AttPermanent.size();
		if (0 == temp_len || temp_len % 2 != 0)
		{
			return false;
		}
		if (att_type >= e_base_att_info_body_strength && att_type <= e_base_att_info_body_stamina)
		{
			int32 value = get_pawn_att().get_unit_base_att(att_type);
			get_pawn_att().change_base_att(att_type, value, false);
			value += add_val;
			if (value < 0)
			{
				value = 0;
			}
			get_pawn_att().change_base_att(att_type, value, true);
			get_pawn_att().set_unit_base_att(att_type, value);
			m_pawn_att.send_base_att_one(att_type);
			return true;
		}
		for (int32 i = 0; i < temp_len; i += 2)
		{
			if (template_ptr->AttPermanent[i] == att_type)
			{
				int32 old_value = m_pawn_att.get_unit_base_att(att_type);
				if (old_value >= template_ptr->AttPermanent[i + 1])
				{
					return false;
				}
				int32 att_base_type = att_type;
				get_pawn_att().change_base_att(att_type, old_value, false);
				old_value += add_val;
				if (old_value > template_ptr->AttPermanent[i + 1])
				{
					old_value = template_ptr->AttPermanent[i + 1];
				}
				get_pawn_att().change_base_att(att_type, old_value, true);
				m_pawn_att.set_unit_base_att(att_type, old_value);
				m_pawn_att.send_base_att_one(att_type);
				return true;
			}
		}
		return false;
	}
	void player::set_unit_gs_change()
	{
		ZoneScoped;
		if (m_status != e_session_status_in_gaming)
		{
			return;
		}
		int64 temp_gs = 0;
		for (int32 ers_index = e_unit_attack_att_strength; ers_index < e_unit_attack_att_max; ers_index++)
		{
			temp_gs += globle_data::get_instance().get_att_gs(ers_index) * m_pawn_att.get_attack_att_gs(ers_index);
		}

		temp_gs += get_talent_mgr().get_total_gs_value();
		temp_gs += get_passive_skill().get_passive_skill_gs();

		temp_gs -= GAMECONFIG->InitCutGS;
		if (temp_gs != get_unit_gs_value())
		{
			set_unit_i64_info_data(e_role_i64_info_gs_value, temp_gs);
			m_ranking_mgr.set_and_sync_ranking_data(e_RankingIndex_gs, temp_gs);
			m_arena_mgr.sync_gs_value(temp_gs);
			if (check_and_set_highest_record(e_role_history_highest_record_gs_value, temp_gs) == true)
			{
				m_time_limit_activity_mgr.set_up_rank_activity_schedule(e_time_limit_activity_type_gs_up);
				m_time_limit_activity_mgr.set_up_rank_activity_schedule(e_time_limit_activity_type_cross_gs_up);
			}

			if (m_loading_flag.empty())
			{
				//send_info_one(e_role_info_GS_value);
				send_role_i64_value(e_role_i64_info_gs_value);
			}
		}
		refresh_service_goal(e_service_goal_type_role_gs);
	}

	// cur_eated��can_eat_max���Ƿ���ֵ.
	// cur_eated��ʾ���ĿǰΪֹ�Թ��Ĺ�ʵ������
	// can_eat_max��ʾ��ҵ�ǰ�ܹ��ԵĹ�ʵ���������
	void player::get_fruit_eating_info(e_fruit_type fruit_type, int32& cur_eated, int32& can_eat_max)
	{
		ZoneScoped;
		if (fruit_type >= e_fruit_type_max)
		{
			cur_eated = can_eat_max = 0;
			return;
		}

		int32 player_upgrade_id = get_unit_info(e_role_info_upgrade_id);
		PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, player_upgrade_id);
		if (nullptr == upgrade_template_ptr)
		{
			cur_eated = can_eat_max = 0;
			return;
		}
		if (upgrade_template_ptr->AttPermanent.size() != 8)
		{
			cur_eated = can_eat_max = 0;
			return;
		}

		can_eat_max = upgrade_template_ptr->AttPermanent[(int32)fruit_type * 2 - 1];
		cur_eated = get_pawn_att().get_unit_base_att(fruit_type);

		if (cur_eated < 0 || can_eat_max < 0)
		{
			cur_eated = can_eat_max = 0;
			return;
		}
	}

	bool player::is_arround_pos(s_map_pos target_pos, float distance)
	{
		ZoneScoped;
		const s_map_pos& cur_player_pos = get_new_map_pos();
		const float x = cur_player_pos.unit_location.x;
		const float y = cur_player_pos.unit_location.y;
		const float z = cur_player_pos.unit_location.z;

		if ((target_pos.unit_location.x - distance <= x && x <= target_pos.unit_location.x + distance)
			&& (target_pos.unit_location.y - distance <= y && y <= target_pos.unit_location.y + distance)
			&& (target_pos.unit_location.z - distance <= z && z <= target_pos.unit_location.z + distance))
		{
			return true;
		}

		return false;
	}

	bool player::can_attack_other(int32 target_unit_idx)
	{
		ZoneScoped;
		auto map_type = base_map_system::get_map_type(m_map_ent);
		auto map_template_id = base_map_system::get_map_template_id(m_map_ent);

		unit& target_ref = unit_man::get_unit(target_unit_idx);
		if (false == target_ref.is_valid())
		{
			return target_unit_idx <= 0;
		}
		if (!get_assist_fight_mgr().is_assist_helper())
		{
			if (map_type == e_map_type_boss_home)
			{
				if (target_ref.get_owner_unit_type() == e_unit_type_monster
					&& target_ref.get_unit_sub_type() == e_monster_type_boss
					&& m_gain_treasure_mgr.is_finish_today_field_boss())
				{
					return false;
				}
			}

			if (map_type == e_map_type_boss_vip_home)
			{
				ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_home);
				if (nullptr == act_common_config_ptr)
					return false;

				int32 max_count = act_common_config_ptr->ParamInt2;
				int32 cur_count = get_logic_data(e_role_logic_info_boss_home_cur_used_times);
				if (target_ref.get_owner_unit_type() == e_unit_type_monster
					&& target_ref.get_unit_sub_type() == e_monster_type_boss
					&& cur_count >= max_count)
				{
					return false;
				}
			}

			if (map_type == e_map_type_boss_island)
			{
				if (target_ref.get_owner_unit_type() == e_unit_type_monster
					&& target_ref.get_unit_sub_type() == e_monster_type_boss
					&& m_boss_island_mgr.is_finish_today_boss_island())
				{
					return false;
				}
			}
		}



		if (false == init_unit::is_map_cross_server(map_template_id, true))
		{
			if (target_ref.get_owner_unit_type() == e_unit_type_monster
				&& false == is_self_server())
			{
				return false;
			}
		}

		//if (true == init_unit::is_map_boss_island(map_object_ptr->get_map_type()))
		//{
		//	if (target_ref.get_owner_unit_type() == e_unit_type_monster
		//		&& target_ref.get_unit_sub_type() == e_monster_type_boss
		//		&& m_boss_island_mgr.get_left_boss_island_kill() > 0)
		//	{
		//		return false;
		//	}
		//}

		if (target_ref.is_mission_see(m_array_index) == false)
		{
			return false;
		}

		return true;
	}
	void player::send_notice(const std::string& notice_str)
	{
		ZoneScoped;
		character_proto_map_fuben_send_notice msg;
		msg.set_notice_content(notice_str.c_str());
		send_message_to_self(&msg, e_msgindex_s2c_fuben_send_notice);
	}
	void player::send_notice()
	{
		ZoneScoped;
		int32 notice_id = globle_data::get_instance().get_notice_id();
		s_chat_notice_info& chat_notice_info = globle_data::get_instance().get_char_notice_info();
		get_chat_mgr().send_notice_new(notice_id, chat_notice_info);
	}

	void player::send_addition_notice_with_type(e_addition_buff addition_buff_type, int32 addtion_id)
	{
		ZoneScoped;
		AdditionBuffTemplate* cur_addtion_temp_ptr = GET_TEMPLATE(AdditionBuffTemplate, addtion_id);
		if (nullptr == cur_addtion_temp_ptr || cur_addtion_temp_ptr->AdditionValue.size() % e_att_one_max != 0)
		{
			return;
		}
		s_chat_notice_info notice_data;
		int32 how_many_att = cur_addtion_temp_ptr->AdditionValue.size() / e_att_one_max;
		int32 equip_num = cur_addtion_temp_ptr->EquipNum;
		int32 addtion_num = 0;
		std::vector<int32> att_arr;
		att_arr.clear();
		for (int32 i = 0; i < how_many_att; i++)
		{
			att_arr.push_back(0);
		}
		int32 notice_temp_id = cur_addtion_temp_ptr->ShowNoticeId;
		std::string succinct_level = "";
		for (int32 i = 0; i < how_many_att; i++)
		{
			if (0 == att_arr[i])
			{
				float value = cur_addtion_temp_ptr->AdditionValue[e_att_one_value + i * (int32)e_att_one_max];
				if (value < 1)
				{
					value *= 100;
				}
				att_arr[i] = (int32)value;
			}

		}
		notice_data.num_type_data.push_back(equip_num);
		switch (addition_buff_type)
		{
		case faith::e_addition_buff_upgrade:
			addtion_num = cur_addtion_temp_ptr->UpgradeNum;
			notice_data.num_type_data.push_back(addtion_num);
			for (int32 i = 0; i < how_many_att; i++)
			{
				if (att_arr[i] != 0)
				{
					notice_data.num_type_data.push_back(att_arr[i]);
				}
			}
			break;
		case faith::e_addition_buff_quality:
			for (int32 i = 0; i < how_many_att; i++)
			{
				if (att_arr[i] != 0)
				{
					notice_data.num_type_data.push_back(att_arr[i]);
				}
			}
			break;
		case faith::e_addition_buff_addon:
			addtion_num = cur_addtion_temp_ptr->AddOnLevel;
			notice_data.num_type_data.push_back(addtion_num);
			for (int32 i = 0; i < how_many_att; i++)
			{
				if (att_arr[i] != 0)
				{
					notice_data.num_type_data.push_back(att_arr[i]);
				}
			}
			break;
		case faith::e_addition_buff_succinct:
			addtion_num = cur_addtion_temp_ptr->SuccinctFullQualityLevel;
			switch (addtion_num)
			{
			case 1:
				succinct_level = "F";
				break;
			case 2:
				succinct_level = "E";
				break;
			case 3:
				succinct_level = "D";
				break;
			case 4:
				succinct_level = "C";
				break;
			case 5:
				succinct_level = "B";
				break;
			case 6:
				succinct_level = "A";
				break;
			case 7:
				succinct_level = "S";
				break;
			default:
				break;
			}
			notice_data.other_name.push_back(succinct_level);
			for (int32 i = 0; i < how_many_att; i++)
			{
				if (att_arr[i] != 0)
				{
					notice_data.num_type_data.push_back(att_arr[i]);
				}
			}
			break;
		case faith::e_addition_buff_sky_equip:
		case faith::e_addition_buff_skygod_equip:
		case faith::e_addition_buff_supreme_equip:
			break;
		case faith::e_addition_buff_max:
		case faith::e_addition_buff_achievement:
		case faith::e_addition_buff_rank:
		case faith::e_addition_buff_feather:
		default:
			return;
		}
		get_chat_mgr().send_notice_new(notice_temp_id, notice_data);
	}

	bool player::can_entry_fuben(int32 fuben_template_id, bool notice_to_clien, bool is_cross_to_gate_server)
	{
		ZoneScoped;
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, fuben_template_id);
		if (nullptr == map_template_ptr)
		{
			CONSOLE_ERROR("map_template_ptr is nullptr fuben_template_id:{}", fuben_template_id);
			return false;
		}
		auto map_type = (e_map_type)map_template_ptr->Type;
		auto map_sub_type = map_template_ptr->SubType;
		if (map_type == faith::e_map_type_big_map)
		{
			return true;
		}

		switch (map_type)
		{
		case e_map_type_demon_tower_fuben:
		{
			auto map_record_ptr = m_map_record_mgr->get_map_record(fuben_template_id);
			if (map_record_ptr)
			{
				if (fuben_template_id != map_record_ptr->get_map_id())
				{
					CONSOLE_ERROR("fuben_template_id:{} map_record_ptr->get_map_id():{}", fuben_template_id, map_record_ptr->get_map_id());
					return false;
				}
			}
			else
			{
				if (fuben_template_id != first_demons_tower_template_id)
				{
					CONSOLE_ERROR("fuben_template_id:{} first_demons_tower_template_id:{}", fuben_template_id, first_demons_tower_template_id);
					return false;
				}
			}
			break;
		}
		case e_map_type_battle:
		{
			auto map_record_ptr = m_map_record_mgr->get_map_record(fuben_template_id);
			if (map_record_ptr)
			{
				if (fuben_template_id > map_record_ptr->get_map_id())
				{
					CONSOLE_ERROR("fuben_template_id:{} map_record_ptr->get_map_id():{}", fuben_template_id, map_record_ptr->get_map_id());
					return false;
				}
			}
			else
			{
				if (fuben_template_id != first_battle_map_template_id)
				{
					CONSOLE_ERROR("fuben_template_id:{} first_battle_map_template_id:{}", fuben_template_id, first_battle_map_template_id);
					return false;
				}
			}
			break;
		}
		default:
			CONSOLE_ERROR("map_type:{} map_template_id:{}", (int32)map_type, fuben_template_id);
			return false;
		}

		int32 player_vip_level = get_vip_level();

		if (map_template_ptr->TransportPrice.size() >= player_vip_level + 1)
		{
			e_money_type money_type = (e_money_type)map_template_ptr->TransportPrice[0];
			int32 money_num = map_template_ptr->TransportPrice[player_vip_level + 1];
			if (!can_cut_money(money_type, money_num))
			{
				CONSOLE_ERROR("money not enough money_type:{} money_num:{}", (int32)money_type, money_num);
				return false;
			}
		}
		return true;
	}

	bool player::is_fuben_clearance(int32 fuben_id)
	{
		ZoneScoped;
		return true;
	}

	void player::check_captain_mission_by_npc_dead(e_mission_slot mission_index, int32 npc_id)
	{
		ZoneScoped;
		// û�п�����Ӧ����
		cmission* mission_ptr = get_mission_mgr().find_mission_by_index(mission_index);
		if (nullptr == mission_ptr)
		{
			return;
		}

		// û�ж���
		if (false == get_team_guid().is_valid())
		{
			return;
		}

		// �Ƕӳ�
		if (true == is_player_team_captain())
		{
			return;
		}

		// �ӳ�����aoi��Χ
		player* team_captain_ptr = get_team_cs_mgr().get_captain_from_aoi();
		if (nullptr == team_captain_ptr)
		{
			return;
		}

		// �ӳ�û�п�����Ӧ����
		cmission* team_captain_mission_ptr = team_captain_ptr->get_mission_mgr().find_mission_by_index(mission_index);
		if (nullptr == team_captain_mission_ptr)
		{
			return;
		}

		// ���Ƕӳ�����������
		if (false == team_captain_mission_ptr->is_mission_target(npc_id))
		{
			return;
		}

		// ���Լ�������������
		if (true == mission_ptr->is_mission_target(npc_id))
		{
			return;
		}

		const int32 target_id = mission_ptr->get_mission_goal_npc_kill_id(0);
		get_mission_mgr().target_check(e_mission_end_type_npc_kill, target_id);
	}

	int32 player::get_mission_id(e_mission_slot mission_index)
	{
		ZoneScoped;
		return get_mission_mgr().get_mission_id(mission_index);
	}

	void player::change_mission(e_mission_slot mission_index, int32 new_mission_id)
	{
		ZoneScoped;
		get_mission_mgr().change_mission(mission_index, new_mission_id);
	}

	int32 player::get_new_mission_id(e_mission_slot mission_index)
	{
		ZoneScoped;
		return get_mission_mgr().get_new_mission_id(mission_index);
	}

	bool player::is_cur_mission_accord_level(e_mission_slot mission_index)
	{
		ZoneScoped;
		return get_mission_mgr().is_cur_mission_accord_level(mission_index);
	}

	bool player::is_cur_mission_high_level(e_mission_slot mission_index)
	{
		ZoneScoped;
		return get_mission_mgr().is_cur_mission_high_level(mission_index);
	}

	bool player::is_special_mission_finished(e_mission_slot mission_slot)
	{
		ZoneScoped;
		switch (mission_slot)
		{
		case faith::e_mission_slot_daily:
			return get_unit_info(e_role_info_daily_done_num) >= DAILY_MISSION_MAX;
		case faith::e_mission_slot_against:
			return get_unit_info(e_role_info_against_done_num) >= AGAINST_MISSION_MAX;
		default:
			break;
		}

		return false;
	}

	bool player::is_special_mission_unlocked(e_mission_slot mission_slot)
	{
		ZoneScoped;
		switch (mission_slot)
		{
		case faith::e_mission_slot_daily:
			return get_func_unlock_mgr().is_func_unlock(daily_mission_unlock_id);
		case faith::e_mission_slot_against:
			return get_func_unlock_mgr().is_func_unlock(against_mission_unlock_id);
		default:
			break;
		}

		return false;
	}

	int32 player::get_element_heart_equip_num()
	{
		ZoneScoped;
		return get_item_set().get_element_heart_equip_num();
	}

	//void player::prepare_interaction(guid_64 be_invited_interaction_guid, guid_64 initiator_interaction_guid, int32 be_invited_interaction_type, int32 initiator_interaction_type)
	//{
	//	if (initiator_interaction_type == e_interaction_type_interrupted)
	//	{
	//		update_interaction(be_invited_interaction_guid, initiator_interaction_guid, be_invited_interaction_type, initiator_interaction_type, e_interaction_reply_agree);
	//	}
	//	else
	//	{
	//		player& interaction_be_invited_player_ref = unit_man::get_player(be_invited_interaction_guid);
	//		if (false == interaction_be_invited_player_ref.is_valid())
	//		{
	//			return;
	//		}
	//		faith::interaction_proto_interaction_invite interaction_invite_proto;
	//		interaction_invite_proto.set_interaction_be_invited_guid(be_invited_interaction_guid.server_64);
	//		interaction_invite_proto.set_interaction_initiator_guid(get_unit_guid().server_64);
	//		interaction_invite_proto.set_interaction_be_invited_interaction_type(be_invited_interaction_type);
	//		interaction_invite_proto.set_interaction_initiator_interaction_type(initiator_interaction_type);
	//		interaction_be_invited_player_ref.send_message_to_self(&interaction_invite_proto, e_msgindex_s2c_interaction_invite);
	//	}
	//}

	//void player::update_interaction(guid_64 be_invited_interaction_guid, guid_64 initiator_interaction_guid, int32 be_invited_interaction_type, int32 initiator_interaction_type, int32 interaction_result)
	//{
	//	player& interaction_be_invited_player_ref = unit_man::get_player(be_invited_interaction_guid);
	//	if (e_interaction_type(initiator_interaction_type) == e_interaction_type_interrupted && e_interaction_reply_agree == interaction_result)
	//	{
	//		get_pawn_att().set_game_att(e_unit_game_att_interaction, e_interaction_type_none, 0);
	//		if (m_interaction_type == e_interaction_type_doubleride_back)
	//		{
	//			buff_man::del_buff_by_template_id(get_array_index(), get_array_index(), 10090002);
	//			buff_man::add_buff_inst(get_array_index(), get_array_index(), 10090000);
	//		}
	//		m_interaction_type = e_interaction_type_none;
	//		m_interaction_object_guid = guid_64();

	//		if (interaction_be_invited_player_ref.is_valid())
	//		{
	//			interaction_be_invited_player_ref.get_pawn_att().set_game_att(e_unit_game_att_interaction, e_interaction_type_none, 0);
	//			if (interaction_be_invited_player_ref.m_interaction_type == e_interaction_type_doubleride_back)
	//			{
	//				buff_man::del_buff_by_template_id(interaction_be_invited_player_ref.get_array_index(), interaction_be_invited_player_ref.get_array_index(), 10090002);
	//				buff_man::add_buff_inst(interaction_be_invited_player_ref.get_array_index(), interaction_be_invited_player_ref.get_array_index(), 10090000);
	//			}
	//			interaction_be_invited_player_ref.m_interaction_type = e_interaction_type_none;
	//			interaction_be_invited_player_ref.m_interaction_object_guid = guid_64();
	//		}
	//		return;
	//	}

	//	if (interaction_be_invited_player_ref.is_valid())
	//	{
	//		faith::interaction_proto_interaction_invite_end interaction_invite_end_proto;
	//		interaction_invite_end_proto.set_interaction_be_invited_guid(be_invited_interaction_guid.server_64);
	//		interaction_invite_end_proto.set_interaction_initiator_guid(initiator_interaction_guid.server_64);
	//		interaction_invite_end_proto.set_interaction_be_invited_interaction_type(be_invited_interaction_type);
	//		interaction_invite_end_proto.set_interaction_initiator_interaction_type(initiator_interaction_type);
	//		interaction_invite_end_proto.set_interaction_result(interaction_result);
	//		interaction_be_invited_player_ref.send_message_to_aoi(&interaction_invite_end_proto, e_msgindex_s2c_interaction_invite_end);
	//	}

	//	if (e_interaction_reply_agree == interaction_result)
	//	{
	//		m_interaction_type = (e_interaction_type)initiator_interaction_type;
	//		m_interaction_object_guid = be_invited_interaction_guid;
	//		if (interaction_be_invited_player_ref.is_valid())
	//		{
	//			interaction_be_invited_player_ref.m_interaction_type = (e_interaction_type)be_invited_interaction_type;
	//			interaction_be_invited_player_ref.m_interaction_object_guid = initiator_interaction_guid;
	//		}
	//	}
	//	else
	//	{
	//		m_interaction_type = e_interaction_type_none;
	//		m_interaction_object_guid = guid_64();
	//		if (interaction_be_invited_player_ref.is_valid())
	//		{
	//			interaction_be_invited_player_ref.m_interaction_type = e_interaction_type_none;
	//			interaction_be_invited_player_ref.m_interaction_object_guid = guid_64();
	//		}
	//	}
	//}

	//void player::start_interaction(guid_64 be_invited_interaction_guid, guid_64 initiator_interaction_guid, int32 be_invited_interaction_type, int32 initiator_interaction_type)
	//{
	//	player& interaction_be_invited_player_ref = unit_man::get_player(be_invited_interaction_guid);
	//	if (false == interaction_be_invited_player_ref.is_valid())
	//	{
	//		return;
	//	}
	//	interaction_be_invited_player_ref.get_pawn_att().set_game_att(e_unit_game_att_interaction, be_invited_interaction_type, 0);

	//	player& initiator_invited_player_ref = unit_man::get_player(initiator_interaction_guid);
	//	if (false == initiator_invited_player_ref.is_valid())
	//	{
	//		return;
	//	}
	//	initiator_invited_player_ref.get_pawn_att().set_game_att(e_unit_game_att_interaction, initiator_interaction_type, 0);
	//}

	void player::load_role_time(const character_s2s_sl_time_data& msg)
	{
		ZoneScoped;
		for (int32 i = 0; i < e_time_type_max && i < msg.data_ary_size(); i++)
		{
			m_role_time.data_ary[i] = msg.data_ary(i);
		}
		get_welfare_mgr().reset_first_recharge_welfare();
		send_time_all();
	}
	bool player::load_role_time_by_db_lua(const char* data_ptr, int32 data_len)
	{
		ZoneScoped;
		//if (data_ptr == nullptr || !data_len)
		//{
		//	return false;
		//}
		//faith::cs2dp_proto::role_time_db msg;
		//bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		//if (!is_sucess)
		//{
		//	return false;
		//}

		//s_time_info db_data;
		//for (int32 i = 0; i < msg.data_ary_size(); i++)
		//{
		//	db_data.data_ary[i] = msg.data_ary(i);
		//}
		//load_role_time(db_data);

		return true;


	}
	void player::save_role_time(int32 save_type_ex)
	{
		ZoneScoped;
		character_s2s_sl_time_data msg;
		for (int32 i = 0; i < e_time_type_max; i++)
		{
			msg.add_data_ary(m_role_time.data_ary[i]);
		}
		send_message_to_dp(&msg, e_msgindex_dp2cs_save_char_data, e_data_flag_time, save_type_ex);
	}
	i64 player::get_time_data(e_time_type time_type)
	{
		ZoneScoped;
		if (time_type >= e_time_type_max)
		{
			return 0;
		}
		return m_role_time.data_ary[time_type];
	}
	void player::set_time_data(e_time_type time_type, i64 time_num)
	{
		ZoneScoped;
		if (time_type >= e_time_type_max)
		{
			return;
		}
		m_role_time.data_ary[time_type] = time_num;
	}

	int32 player::get_time_data_sec(e_time_type time_type)
	{
		ZoneScoped;
		i64  ret_time = get_time_data(time_type);
		int32 ret_sec = ret_time / second_tick_time;
		return ret_sec;
	}


	void player::send_time_all()
	{
		ZoneScoped;
		character_proto_character_time_all time_msg;
		for (int32 i = 0; i < e_time_type_max; ++i)
		{
			time_msg.add_time_value(m_role_time.data_ary[i]);
		}

		send_message_to_self(&time_msg, e_msgindex_s2c_character_time_all);
	}

	void player::send_time_one(e_time_type time_type)
	{
		ZoneScoped;
		if (time_type >= e_time_type_max)
		{
			return;
		}
		character_proto_character_time_one time_msg;
		time_msg.set_time_id(time_type);
		time_msg.set_time_value(m_role_time.data_ary[time_type]);

		send_message_to_self(&time_msg, e_msgindex_s2c_character_time_one);
	}

	void player::send_open_server_time_to_self()
	{
		ZoneScoped;
		character_proto_open_server_time open_server;
		open_server.set_open_server_time(globle_data::get_instance().get_server_open_tm_str());
		open_server.set_open_server_activity_end_time(globle_data::get_instance().get_server_open_state_end_tm_str());
		open_server.set_interval_days(globle_data::get_instance().get_server_on_days());
		send_message_to_self(&open_server, e_msgindex_s2c_open_server_time);
	}
	void player::load_role_logic(const character_s2s_sl_logic_data& msg)
	{
		ZoneScoped;
		for (int32 i = 0; i < e_role_logic_info_max && i < msg.data_ary_size(); i++)
		{
			m_role_logic.data_ary[i] = msg.data_ary(i);
		}
		if (m_role_logic.data_ary[e_role_logic_info_zero_gs_up_start_value] > 0 && m_role_info.data_i64_ary[e_role_i64_info_history_best_gs_value] <= 0)
		{
			m_role_info.data_i64_ary[e_role_i64_info_history_best_gs_value] = m_role_logic.data_ary[e_role_logic_info_zero_gs_up_start_value];
		}
		if (m_role_logic.data_ary[e_role_logic_info_zero_mount_up_start_value] > 0 && m_role_info.data_i64_ary[e_role_i64_info_zero_mount_up_start_value] <= 0)
		{
			m_role_info.data_i64_ary[e_role_i64_info_zero_mount_up_start_value] = m_role_logic.data_ary[e_role_logic_info_zero_mount_up_start_value];
		}
		if (m_role_logic.data_ary[e_role_logic_info_zero_wing_up_start_value] > 0 && m_role_info.data_i64_ary[e_role_i64_info_zero_wing_up_start_value] <= 0)
		{
			m_role_info.data_i64_ary[e_role_i64_info_zero_wing_up_start_value] = m_role_logic.data_ary[e_role_logic_info_zero_wing_up_start_value];
		}
		if (m_role_logic.data_ary[e_role_logic_info_zero_equip_up_start_value] > 0 && m_role_info.data_i64_ary[e_role_i64_info_zero_equip_up_start_value] <= 0)
		{
			m_role_info.data_i64_ary[e_role_i64_info_zero_equip_up_start_value] = m_role_logic.data_ary[e_role_logic_info_zero_equip_up_start_value];
		}
		if (m_role_logic.data_ary[e_role_i64_info_zero_spirit_up_start_value] > 0 && m_role_info.data_i64_ary[e_role_i64_info_history_best_gs_value] <= 0)
		{
			m_role_info.data_i64_ary[e_role_i64_info_history_best_gs_value] = m_role_logic.data_ary[e_role_i64_info_zero_spirit_up_start_value];
		}

		send_logic_all();
	}
	bool player::load_role_logic_by_db_lua(const char* data_ptr, int32 data_len)
	{
		ZoneScoped;
		//if (data_ptr == nullptr || !data_len)
		//{
		//	return false;
		//}
		//faith::cs2dp_proto::role_logic_db msg;
		//bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		//if (!is_sucess)
		//{
		//	return false;
		//}

		//s_logic_info db_data;
		//for (int32 i = 0; i < msg.data_ary_size(); i++)
		//{
		//	db_data.data_ary[i] = msg.data_ary(i);
		//}

		//load_role_logic(db_data);

		return true;

	}
	void player::save_role_logic(int32 save_type_ex)
	{
		ZoneScoped;
		character_s2s_sl_logic_data msg;
		for (int32 i = 0; i < e_role_logic_info_max; i++)
		{
			msg.add_data_ary(m_role_logic.data_ary[i]);
		}
		send_message_to_dp(&msg, e_msgindex_dp2cs_save_char_data, e_data_flag_logic_info, save_type_ex);
	}
	int32 player::get_logic_data(e_role_logic_info logic_type)
	{
		ZoneScoped;
		if (logic_type >= e_role_logic_info_max)
		{
			return 0;
		}
		return m_role_logic.data_ary[logic_type];
	}
	void player::set_logic_data(e_role_logic_info logic_type, int32 logic_num)
	{
		ZoneScoped;
		if (logic_type >= e_role_logic_info_max)
		{
			return;
		}
		m_role_logic.data_ary[logic_type] = logic_num;
	}
	void player::send_logic_all()
	{
		ZoneScoped;
		character_proto_character_logic_all logic_msg;
		for (int32 i = 0; i < e_role_logic_info_max; ++i)
		{
			logic_msg.add_logic_value(m_role_logic.data_ary[i]);
		}

		send_message_to_self(&logic_msg, e_msgindex_s2c_character_logic_all);
	}

	void player::send_logic_one(e_role_logic_info logic_type)
	{
		ZoneScoped;
		if (logic_type >= e_role_logic_info_max)
		{
			return;
		}
		character_proto_character_logic_one logic_msg;
		logic_msg.set_logic_id(logic_type);
		logic_msg.set_logic_value(m_role_logic.data_ary[logic_type]);

		send_message_to_self(&logic_msg, e_msgindex_s2c_character_logic_one);
	}

	int32 player::get_max_level_by_func(int32 func_type)
	{
		ZoneScoped;
		int32 cur_level = get_unit_info(e_role_info_exp_level);
		int32 cur_mission_id = 0;
		int32 world_level = cell_server::getInstance().get_world_level_last();
		cmission* mission_ptr = get_mission_mgr().find_mission_by_index(e_mission_slot_main);
		if (mission_ptr != nullptr && mission_ptr->get_mission_template_ptr() != nullptr)
		{
			cur_mission_id = mission_ptr->get_mission_template_ptr()->attribute_id;
		}
		int32 max_level = 0;
		faith::template_manager::template_type* template_list = template_manager::get_instance().get_templates(e_DevelopOpenTemplate);
		if (nullptr == template_list)
		{
			return 0;
		}
		template_manager::template_type::iterator it;
		for (it = template_list->begin(); it != template_list->end(); ++it)//��������ÿһ��
		{
			DevelopOpenTemplate* template_ptr = (DevelopOpenTemplate*)it->second;
			if (template_ptr == nullptr)
			{
				continue;
			}
			if (template_ptr->Type != func_type)
			{
				continue;
			}
			if (cur_level < template_ptr->NeedRoleLevel)
			{
				continue;
			}
			if (world_level < template_ptr->NeedWorldLevel)
			{
				continue;
			}
			if (cur_mission_id <= template_ptr->NeedMission)
			{
				continue;
			}
			if (max_level < template_ptr->OpenLevel)
			{
				max_level = template_ptr->OpenLevel;
			}
		}
		return max_level;
	}

	void player::set_money_all(const s_money_info& money_info)
	{
		ZoneScoped;
		m_money_info = money_info;
		////�����ô��룬Ϊ��ÿ�μ�Ǯ�����
		//for (int32 i = 1; i < e_money_type_max; ++i)
		//{
		//	if (m_money_info.cur_data_ary[i] < 100000)
		//	{
		//		m_money_info.cur_data_ary[i] = 10000000 + i;
		//	}
		//}
	}

	void player::set_money_data(e_money_type money_type, int64 money_value)
	{
		ZoneScoped;
		if (isnt_money_type(money_type))
		{
			return;
		}
		m_money_info.cur_data_ary[money_type] = money_value;
	}
	int64 player::get_money_data(e_money_type money_type)
	{
		ZoneScoped;
		if (isnt_money_type(money_type))
		{
			return 0;
		}
		return m_money_info.cur_data_ary[money_type];
	}
	int32 player::get_money_data_32(e_money_type money_type)
	{
		ZoneScoped;
		return get_money_data(money_type);
	}
	int64 player::get_all_money_data(e_money_type money_type)
	{
		ZoneScoped;
		if (isnt_money_type(money_type))
		{
			return 0;
		}
		return m_money_info.all_data_ary[money_type];
	}
	bool player::can_cut_money(e_money_type money_type, int64 money_value)
	{
		ZoneScoped;
		if (isnt_money_type(money_type) || money_value < 0)
		{
			return false;
		}
		//if (money_type == e_money_type_silver_bind)
		//{
		//	if (m_money_info.cur_data_ary[money_type] + m_money_info.cur_data_ary[e_money_type_silver] >= money_value)
		//	{
		//		return true;
		//	}
		//}
		if (money_type == e_money_type_jewel_bind)
		{
			if (m_money_info.cur_data_ary[money_type] + m_money_info.cur_data_ary[e_money_type_jewel] >= money_value)
			{
				return true;
			}
		}
		else
		{
			if (m_money_info.cur_data_ary[money_type] >= money_value)
			{
				return true;
			}
		}
		return false;
	}

	bool player::can_cut_money_by_template_two_tuple_arr(const std::vector<int32>& money_arr, int32 extra_times)
	{
		ZoneScoped;
		int32 price_arr_len = money_arr.size();
		if (price_arr_len < 2)
		{
			return true;
		}

		int64 cost_money_id = 0;
		int64 cost_money_num = 1;
		for (int32 i = 0; i < price_arr_len - e_money_tuple_num; i += e_money_tuple_max)
		{
			cost_money_id = money_arr[i + e_money_tuple_id];
			cost_money_num = money_arr[i + e_money_tuple_num] * extra_times;

			if (can_cut_money((e_money_type)cost_money_id, cost_money_num) == false)
			{
				return false;
			}
		}

		return true;
	}
	void player::cut_money(e_money_type money_type, int64 money_value, e_server_log_cut_money cut_type, int32 param, int32 param2)
	{
		ZoneScoped;
		int64 old_money_num = m_money_info.cur_data_ary[money_type];
		m_money_info.cur_data_ary[money_type] -= money_value;
		m_money_info.cost_data_ary[money_type] += money_value;
		int32 other_money_type = e_money_type_exp;
		int64 other_money_num = 0;
		int64 other_money_old = 0;
		if (m_money_info.cur_data_ary[money_type] < 0)
		{
			if (money_type == e_money_type_jewel_bind)
			{
				other_money_type = e_money_type_jewel;
				other_money_num = m_money_info.cur_data_ary[money_type] * -1;
				other_money_old = m_money_info.cur_data_ary[e_money_type_jewel];
				m_money_info.cur_data_ary[e_money_type_jewel] += m_money_info.cur_data_ary[money_type];
				send_money_one(e_money_type_jewel);
			}
			m_money_info.cur_data_ary[money_type] = 0;
		}

		send_money_one(money_type);
		get_achievement_mgr().player_changed(eat_current_money_check);

		if ((money_type == e_money_type_jewel || other_money_type == e_money_type_jewel)
			&& cut_type != e_server_log_cut_money_auction_buy
			&& cut_type != e_server_log_cut_money_auction_bid)
		{
			//�������������Ϸ���߼��õ�
			int64 total_cost_diamond_num = get_unit_info(e_role_info_total_diamond_cost_num);
			int64 daily_cost = get_unit_info(e_role_info_daily_cost);
			if (money_type == e_money_type_jewel)
			{
				total_cost_diamond_num += money_value;
				daily_cost += money_value;
			}
			if (other_money_type == e_money_type_jewel)
			{
				total_cost_diamond_num += other_money_num;
				daily_cost += other_money_num;
			}
			set_unit_info(e_role_info_total_diamond_cost_num, total_cost_diamond_num);
			send_info_one(e_role_info_total_diamond_cost_num);
			set_unit_info(e_role_info_daily_cost, daily_cost);
			send_info_one(e_role_info_daily_cost);

			int64 fund_pride_cost_num = get_unit_info(e_role_info_fund_pride_cost_value);
			if (money_type == e_money_type_jewel)
			{
				fund_pride_cost_num += money_value;
			}
			if (other_money_type == e_money_type_jewel)
			{
				fund_pride_cost_num += other_money_num;
			}
			set_unit_info(e_role_info_fund_pride_cost_value, fund_pride_cost_num);
			send_info_one(e_role_info_fund_pride_cost_value);

			//��ʱ��ۼ����ı�ʯ��ֵ����
			if (money_type == e_money_type_jewel)
			{
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_cost, money_value);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_cost_jewel, money_value);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_1, money_value);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_2, money_value);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_3, money_value);
			}
			if (other_money_type == e_money_type_jewel)
			{
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_cost, other_money_num);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_cost_jewel, other_money_num);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_1, other_money_num);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_2, other_money_num);
				get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_cost_jewel_3, other_money_num);
			}
			m_ranking_mgr.set_and_sync_ranking_data(e_RankingIndex_time_limit_activity_total_cost, get_time_limit_activity_mgr().get_activity_schedule(e_time_limit_activity_type_total_cost));
			if (money_type == e_money_type_jewel)
			{
				server_log::jewel_consume_log(money_value, cut_type, get_unit_info(e_role_info_total_diamond_cost_num));
			}
			if (other_money_type == e_money_type_jewel)
			{
				server_log::jewel_consume_log(other_money_num, cut_type, get_unit_info(e_role_info_total_diamond_cost_num));
			}
		}

		if ((money_type == e_money_type_jewel || other_money_type == e_money_type_jewel)
			&& cut_type != e_server_log_cut_money_auction_buy
			&& cut_type != e_server_log_cut_money_auction_bid)
		{
			//������������ڲ�ͳ���õ�
			int32 total_cost_diamond_num = get_logic_data(e_role_logic_info_jewel_total_consume);
			if (money_type == e_money_type_jewel)
			{
				total_cost_diamond_num += money_value;
			}
			if (other_money_type == e_money_type_jewel)
			{
				total_cost_diamond_num += other_money_num;
			}
			set_logic_data(e_role_logic_info_jewel_total_consume, total_cost_diamond_num);
			send_logic_one(e_role_logic_info_jewel_total_consume);
		}

		if (get_write_log())
		{
			server_log::money_role_log(m_login_third_data, m_role_info, -1, money_type, money_value - other_money_num, m_money_info.cur_data_ary[money_type], cut_type, param, old_money_num, m_money_info.cur_data_ary[money_type], param, param2);

			set_log_var(log_head);

			get_log_common_head_info(log_head);

			if ((money_value - other_money_num) != 0)
			{
				server_log::serverCurrencyChange(log_head, init_unit::change_i32_to_string(money_type), 0, 0, cut_type, money_value - other_money_num, m_money_info.cur_data_ary[money_type], param);
			}

			if (other_money_num > 0)
			{
				server_log::serverCurrencyChange(log_head, init_unit::change_i32_to_string(other_money_type), 0, 0, cut_type, other_money_num, m_money_info.cur_data_ary[other_money_type], param);
				server_log::money_role_log(m_login_third_data, m_role_info, -1, other_money_type, other_money_num, m_money_info.cur_data_ary[other_money_type], cut_type, param, other_money_old, m_money_info.cur_data_ary[other_money_type], param, param2);
			}
		}
		if (money_type == e_money_type_jewel_bind)
		{
			buy_message_tip(e_buy_tip_type_money);
		}
	}

	bool player::can_add_money(e_money_type money_type, int64 money_value)
	{
		ZoneScoped;
		if (isnt_money_type(money_type) || money_value < 0)
		{
			return false;
		}
		return true;
	}

	bool player::add_money_or_exp(e_money_type money_type, int64 money_value, e_server_log_add_money add_type, int32 param)
	{
		ZoneScoped;
		if (money_type == e_money_type_exp)
		{
			money_value = add_exp(money_value, true);
			return true;
		}
		else
		{
			return add_money(money_type, money_value, add_type, param, false);
		}
	}

	bool player::lua_add_money_or_exp(e_money_type money_type, xstring money_value, e_server_log_add_money add_type, int32 param)
	{
		ZoneScoped;
		int64 value = init_unit::change_string_to_i64(money_value);
		return add_money_or_exp(money_type, value, add_type, param);
	}

	bool player::add_money(e_money_type money_type, int64 money_value, e_server_log_add_money add_type, int32 param, bool is_need_show_toptip)
	{
		ZoneScoped;
		int64 old_money_num = 0;
		if (money_value == 0)
		{
			//+0��Ǯ����return false��������֮ǰ�������߼�
			return true;
		}
		if (can_add_money(money_type, money_value))
		{
			old_money_num = m_money_info.cur_data_ary[money_type];
			m_money_info.cur_data_ary[money_type] += money_value;
			m_money_info.all_data_ary[money_type] += money_value;
			if (money_type != e_money_type_exp)
			{
				send_money_one(money_type);
			}
			get_achievement_mgr().player_changed(eat_current_money_check);
			get_achievement_mgr().player_changed(eat_all_money_count);

			if (money_type == e_money_type_silver_bind)
			{
				//��ҡ����Ҫ��ʾ
				character_proto_show_money_get show_msg;
				show_msg.set_money_percent(-10);
				show_msg.set_money_type(money_type);
				show_msg.set_money_num(money_value);
				show_msg.set_is_need_show_toptip(is_need_show_toptip);
				send_message_to_self(&show_msg, e_msgindex_s2c_show_money_get);
			}

			if (money_type == e_money_type_battle_achievement)
			{
				int64 current_battle_achievement_value = get_highest_record(e_role_history_highest_record_battle_achievement);

				if (current_battle_achievement_value < 0)
				{
					current_battle_achievement_value = 0;
				}
				set_highest_record(e_role_history_highest_record_battle_achievement, current_battle_achievement_value + money_value);
			}
			if (money_type == e_money_type_pound)
			{
				set_pound_num(money_value + m_cur_pound_num);
				character_proto_show_money_get show_msg;
				show_msg.set_money_percent(-10);
				show_msg.set_money_type(money_type);
				show_msg.set_money_num(money_value);
				show_msg.set_is_need_show_toptip(is_need_show_toptip);
				send_message_to_self(&show_msg, e_msgindex_s2c_show_money_get);
			}

			if (money_type == e_money_type_cross_server_money)
			{
				//�����Ҫ��ʾ
				character_proto_show_money_get show_msg;
				show_msg.set_money_percent(-10);
				show_msg.set_money_type(money_type);
				show_msg.set_money_num(money_value);
				show_msg.set_is_need_show_toptip(is_need_show_toptip);
				send_message_to_self(&show_msg, e_msgindex_s2c_show_money_get);
			}

			if (money_type == e_money_type_jewel
				&& add_type == e_server_log_add_money_recharge)
			{
				int32 old_total_recharge_value = get_logic_data(e_role_logic_info_jewel_total_recharge);
				set_logic_data(e_role_logic_info_jewel_total_recharge, old_total_recharge_value + money_value);
				send_logic_one(e_role_logic_info_jewel_total_recharge);
			}

			if (money_type == e_money_type_season_point)
			{
				int32 add_percent = get_competition_mgr().get_competition_info().point_add_percent;
				int32 add_value = money_value * add_percent / 100;
				if (add_value > 0)
				{
					m_money_info.cur_data_ary[money_type] += add_value;
					m_money_info.all_data_ary[money_type] += add_value;
					send_money_one(money_type);
					get_achievement_mgr().player_changed(eat_current_money_check);
					get_achievement_mgr().player_changed(eat_all_money_count);
				}
				get_competition_mgr().add_competition_exp(money_value + add_value);
			}
			if (money_type == e_money_type_element_point)
			{
				int32 add_percent = get_element_competition_mgr().get_competition_info().point_add_percent;
				int32 add_value = money_value * add_percent / 100;
				if (add_value > 0)
				{
					m_money_info.cur_data_ary[money_type] += add_value;
					m_money_info.all_data_ary[money_type] += add_value;
					send_money_one(money_type);
					get_achievement_mgr().player_changed(eat_current_money_check);
					get_achievement_mgr().player_changed(eat_all_money_count);
				}
				get_element_competition_mgr().add_competition_exp(money_value + add_value);
			}
			if (get_write_log())
			{
				server_log::money_role_log(m_login_third_data, m_role_info, 1, money_type, money_value, m_money_info.cur_data_ary[money_type], add_type, param, old_money_num, m_money_info.cur_data_ary[money_type]);

				set_log_var(log_head);

				get_log_common_head_info(log_head);

				int32 is_rechange = 0;
				if (add_type == e_server_log_add_money_recharge)
				{
					is_rechange = 1;
				}

				server_log::serverCurrencyChange(log_head, init_unit::change_i32_to_string(money_type), 1, is_rechange, add_type, money_value, m_money_info.cur_data_ary[money_type], param);


				if (money_type == e_money_type_pound)
				{
					server_log::element_recovery_log(money_value);
				}
			}
			return true;
		}
		return false;
	}

	void player::send_money_all()
	{
		ZoneScoped;
		character_proto_character_money_all money_msg;

		money_msg.set_role_guid(get_unit_guid().server_64);

		for (int32 i = 0; i < e_money_type_max; ++i)
		{
			money_msg.add_money_value(m_money_info.cur_data_ary[i]);
		}

		send_message_to_self(&money_msg, e_msgindex_s2c_character_money_all);
	}

	void player::send_money_one(e_money_type money_type)
	{
		ZoneScoped;
		if (isnt_money_type(money_type))
		{
			return;
		}
		character_proto_character_money_one money_msg;

		money_msg.set_role_guid(get_unit_guid().server_64);
		money_msg.set_money_id(money_type);
		money_msg.set_money_value(m_money_info.cur_data_ary[money_type]);

		send_message_to_self(&money_msg, e_msgindex_s2c_character_money_one);
	}

	void player::load_money(const s_money_info& money_info)
	{
		ZoneScoped;
		set_money_all(money_info);
		send_money_all();
	}

	bool player::load_money_by_db_lua(const char* data_ptr, int32 data_len)
	{
		ZoneScoped;
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_money_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_money_info db_data;
		for (int32 i = 0; i < msg.cur_data_ary_size(); i++)
		{
			db_data.cur_data_ary[i] = msg.cur_data_ary(i);
		}
		for (int32 i = 0; i < msg.all_data_ary_size(); i++)
		{
			db_data.all_data_ary[i] = msg.all_data_ary(i);
		}
		for (int32 i = 0; i < msg.cost_data_ary_size(); i++)
		{
			db_data.cost_data_ary[i] = msg.cost_data_ary(i);
		}

		load_money(db_data);

		return true;


	}

	void player::save_money(int32 save_type_ex)
	{
		ZoneScoped;
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_money);
		if (is_use == false)
		{
			cs2dp_save_money request;
			request.role_guid = get_unit_guid();
			request.unit_array_index = m_array_index;
			request.save_type_ex = save_type_ex;
			memcpy(&(request.money_info), &m_money_info, sizeof(request.money_info));
			send_message_to_dp(&request, sizeof(request));
		}
		else
		{
			faith::cs2dp_proto::save_role_money msg;
			msg.set_role_guid(get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_type_ex);
			faith::cs2dp_proto::role_money_db* db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_money_type_max; i++)
			{
				db_data->add_cur_data_ary(m_money_info.cur_data_ary[i]);
			}
			for (int32 i = 0; i < e_money_type_max; i++)
			{
				db_data->add_all_data_ary(m_money_info.all_data_ary[i]);
			}
			for (int32 i = 0; i < e_money_type_max; i++)
			{
				db_data->add_cost_data_ary(m_money_info.cost_data_ary[i]);
			}
			send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_money);
		}
	}


	void  player::worship_by_type(int32 worship_type, bool is_cost_diamond)
	{
		ZoneScoped;
		character_proto_worship_end msg;
		msg.set_worship_type(worship_type);
		msg.add_role_guid(get_unit_guid().A);
		msg.add_role_guid(get_unit_guid().B);

		//------------------------------�����п�-------------------------------------------------------
		int32 player_upgrade_template_id = get_unit_info(e_role_info_upgrade_id);
		PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, player_upgrade_template_id);
		if (worship_type < 0 || worship_type >= e_big_player_type_max || upgrade_template_ptr == nullptr)
		{
			msg.set_worship_result(e_item_string_unkown);
			send_message_to_self(&msg, e_msgindex_s2c_worship_end);
			return;
		}
		if (GAMECONFIG->LordOfWarWorshipCost.size() != 4
			|| GAMECONFIG->LordOfCityWorshipCost.size() != 4
			|| GAMECONFIG->LordOfSanctuaryWorshipCost.size() != 4
			|| GAMECONFIG->LordOfGodWorshipCost.size() != 4
			|| GAMECONFIG->LordOfWarStaticRewardArray.size() != 4
			|| GAMECONFIG->LordOfCityStaticRewardArray.size() != 4
			|| GAMECONFIG->LordOfSanctuaryStaticRewardArray.size() != 4
			|| GAMECONFIG->LordOfGodStaticRewardArray.size() != 4
			|| GAMECONFIG->LordOfCityWorshipEXPCoefficient.size() != 2
			|| GAMECONFIG->LordOfWarWorshipEXPCoefficient.size() != 2
			|| GAMECONFIG->LordOfSanctuaryWorshipEXPCoefficient.size() != 2
			|| GAMECONFIG->LordOfGodWorshipEXPCoefficient.size() != 2)
		{
			msg.set_worship_result(e_item_string_unkown);
			send_message_to_self(&msg, e_msgindex_s2c_worship_end);
			return;
		}
		//------------------------------�����пս���-------------------------------------------------------


		int32 cur_worship_time = 0;
		int32 max_worship_time = 0;
		int32 money_cost_id = 0;
		int32 money_cost_num = 0;
		int32 this_worship_get_sign_index = -1;
		std::vector<int32> money_reward_array;
		float exp_reward_coefficient = 0;

		float base_exp_reward = upgrade_template_ptr->BaseExp;
		switch (worship_type)
		{
		case e_big_player_type_lord_of_city:
		{
			this_worship_get_sign_index = e_role_info_lord_of_city_worship_time;
			cur_worship_time = get_unit_info(e_role_info_lord_of_city_worship_time);
			max_worship_time = GAMECONFIG->LordOfCityWorshipTime;
			if (is_cost_diamond)
			{
				money_cost_id = GAMECONFIG->LordOfCityWorshipCost[2];
				money_cost_num = GAMECONFIG->LordOfCityWorshipCost[3];
				exp_reward_coefficient = GAMECONFIG->LordOfCityWorshipEXPCoefficient[1];
			}
			else
			{
				money_cost_id = GAMECONFIG->LordOfCityWorshipCost[0];
				money_cost_num = GAMECONFIG->LordOfCityWorshipCost[1];
				exp_reward_coefficient = GAMECONFIG->LordOfCityWorshipEXPCoefficient[0];
			}
			money_reward_array = GAMECONFIG->LordOfCityStaticRewardArray;
		}
		break;
		case e_big_player_type_lord_or_war:
		{
			this_worship_get_sign_index = e_role_info_lord_of_war_worship_time;
			cur_worship_time = get_unit_info(e_role_info_lord_of_war_worship_time);
			max_worship_time = GAMECONFIG->LordOfWarWorshipTime;
			if (is_cost_diamond)
			{
				money_cost_id = GAMECONFIG->LordOfWarWorshipCost[2];
				money_cost_num = GAMECONFIG->LordOfWarWorshipCost[3];
				exp_reward_coefficient = GAMECONFIG->LordOfWarWorshipEXPCoefficient[1];
			}
			else
			{
				money_cost_id = GAMECONFIG->LordOfWarWorshipCost[0];
				money_cost_num = GAMECONFIG->LordOfWarWorshipCost[1];
				exp_reward_coefficient = GAMECONFIG->LordOfWarWorshipEXPCoefficient[0];
			}
			money_reward_array = GAMECONFIG->LordOfWarStaticRewardArray;
		}
		break;
		case e_big_player_type_lord_or_sanctuary:
		{
			this_worship_get_sign_index = e_role_info_lord_of_sanctuary_worship_time;
			cur_worship_time = get_unit_info(e_role_info_lord_of_sanctuary_worship_time);
			max_worship_time = GAMECONFIG->LordOfSanctuaryWorshipTime;
			if (is_cost_diamond)
			{
				money_cost_id = GAMECONFIG->LordOfSanctuaryWorshipCost[2];
				money_cost_num = GAMECONFIG->LordOfSanctuaryWorshipCost[3];
				exp_reward_coefficient = GAMECONFIG->LordOfSanctuaryWorshipEXPCoefficient[1];
			}
			else
			{
				money_cost_id = GAMECONFIG->LordOfSanctuaryWorshipCost[0];
				money_cost_num = GAMECONFIG->LordOfSanctuaryWorshipCost[1];
				exp_reward_coefficient = GAMECONFIG->LordOfSanctuaryWorshipEXPCoefficient[0];
			}
			money_reward_array = GAMECONFIG->LordOfSanctuaryStaticRewardArray;
		}
		break;
		case e_big_player_type_lord_or_god:
		{
			this_worship_get_sign_index = e_role_info_lord_of_god_worship_time;
			cur_worship_time = get_unit_info(e_role_info_lord_of_god_worship_time);
			max_worship_time = GAMECONFIG->LordOfGodWorshipTime;
			if (is_cost_diamond)
			{
				money_cost_id = GAMECONFIG->LordOfGodWorshipCost[2];
				money_cost_num = GAMECONFIG->LordOfGodWorshipCost[3];
				exp_reward_coefficient = GAMECONFIG->LordOfGodWorshipEXPCoefficient[1];
			}
			else
			{
				money_cost_id = GAMECONFIG->LordOfGodWorshipCost[0];
				money_cost_num = GAMECONFIG->LordOfGodWorshipCost[1];
				exp_reward_coefficient = GAMECONFIG->LordOfGodWorshipEXPCoefficient[0];
			}
			money_reward_array = GAMECONFIG->LordOfGodStaticRewardArray;
		}
		break;
		default:
			msg.set_worship_result(e_item_string_unkown);
			send_message_to_self(&msg, e_msgindex_s2c_worship_end);
			return;
			break;
		}

		if (cur_worship_time >= max_worship_time)
		{
			msg.set_worship_result(e_worship_has_worship);
			send_message_to_self(&msg, e_msgindex_s2c_worship_end);
			return;
		}
		if (!can_cut_money(e_money_type(money_cost_id), money_cost_num))
		{
			if (is_cost_diamond)
			{
				msg.set_worship_result(e_worship_diamond_not_enough);
			}
			else
			{
				msg.set_worship_result(e_worship_gold_not_enough);
			}
			send_message_to_self(&msg, e_msgindex_s2c_worship_end);
			return;
		}
		int64 final_exp_reward = base_exp_reward * exp_reward_coefficient;
		final_exp_reward = add_exp(final_exp_reward);
		msg.add_money_get_id_array(e_money_type_exp);
		msg.add_money_get_num_array(final_exp_reward);

		if (is_cost_diamond)
		{
			int64 real_money_num = money_reward_array[3];
			add_money_or_exp(e_money_type(money_reward_array[2]), real_money_num, e_server_log_add_money_worship);
			msg.add_money_get_id_array(money_reward_array[2]);
			msg.add_money_get_num_array(real_money_num);
		}
		else
		{
			int64 real_money_num = money_reward_array[1];
			add_money_or_exp(e_money_type(money_reward_array[0]), real_money_num, e_server_log_add_money_worship);
			msg.add_money_get_id_array(money_reward_array[0]);
			msg.add_money_get_num_array(real_money_num);
		}
		set_unit_info(e_role_info(this_worship_get_sign_index), cur_worship_time + 1);
		send_info_one(e_role_info(this_worship_get_sign_index));

		msg.set_worship_result(e_worship_success);
		send_message_to_self(&msg, e_msgindex_s2c_worship_end);
	}

	bool player::add_money_by_template_tuple(const std::vector<int32>& money_two_tuple_arr, bool pop_msg, float fac)
	{
		ZoneScoped;
		int32 len = money_two_tuple_arr.size();
		if (len <= 0 || fac <= 0)
		{
			return true;
		}
		if (len % e_money_tuple_max != 0)
		{
			return false;
		}

		int32 money_id = 0;
		int64 money_num = 0;
		for (int32 i = 0; i < len; i += e_money_tuple_max)
		{
			money_id = money_two_tuple_arr[i + e_money_tuple_id];
			money_num = money_two_tuple_arr[i + e_money_tuple_num] * fac;

			if (money_num <= 0)
			{
				continue;
			}

			// �������Ҫ������ҵȼ�����߶������ͽ�������Ǯ��������ҵĵȼ�
			//if (map_template_ptr->AwardMoneyFactor > 0)
			//{
			//	int32 factor = 1;
			//	switch ((e_money_type)money_id)
			//	{
			//	case e_money_type_exp:
			//	case e_money_type_silver:
			//	case e_money_type_silver_bind:
			//	{
			//		int32 upgrade_template_id = player_ref.get_unit_info(e_role_info_upgrade_id);
			//		PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);
			//		if (nullptr != upgrade_template_ptr)
			//		{
			//			factor = (e_money_type_exp == money_id ? upgrade_template_ptr->BaseExp : upgrade_template_ptr->BaseCoin);
			//			money_num *= factor;
			//		}
			//	}
			//	break;
			//	}
			//}

			add_money_or_exp((e_money_type)money_id, money_num, e_server_log_add_money_daily_legion_reward);
			if (pop_msg)
			{
				send_add_money_msg(money_id, money_num);
			}
		}

		return true;
	}

	// ����ͨ��ģ�������Ʒ��������������Ʒ�Ĺ������ն�������ӿ�
	bool player::add_item_by_template_tuple(e_server_log_add_item add_type, int32 param, const std::vector<int32>& item_two_tuple_arr, int32 is_locked, bool pop_msg, float fac, bool check_career, bool auto_to_money)
	{
		ZoneScoped;
		int32 len = item_two_tuple_arr.size();
		if (len <= 0)
		{
			return true;
		}
		if (len % e_item_tuple_max != 0)
		{
			return false;
		}

		int32 item_id = 0;
		int32 item_num = 0;
		int32 money_arr[e_money_type_max] = { 0 };
		std::vector<citem*> item_array;
		for (int32 i = 0; i < len; i += e_item_tuple_max)
		{
			item_id = item_two_tuple_arr[i + e_item_tuple_id];
			item_num = item_two_tuple_arr[i + e_item_tuple_num];

			ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
			if (nullptr == item_template_ptr)
			{
				continue;
			}

			// ������ɵ���Ʒ��ǰְҵ����ʹ�þͲ�����
			if (check_career)
			{
				int32 cur_player_career = get_unit_info(e_role_info_class_type);
				if (init_unit::can_career_use_item(cur_player_career, item_template_ptr->class_type) == false)
				{
					continue;
				}
			}

			if (auto_to_money && init_unit::is_equip_type(item_template_ptr->item_type))
			{
				s_money_tuple earn_money = get_item_set().get_item_recovery_earn_money(item_id);
				money_arr[earn_money.money_id] += earn_money.money_num;
			}
			else
			{
				citem* new_item = get_item_set().create_item_by_template(add_type, param, item_id, item_num, is_locked);
				if (nullptr == new_item)
				{
					continue;
				}
				item_array.push_back(new_item);
			}
		}
		get_item_set().put_in_bag(item_array);
		if (pop_msg && auto_to_money)
		{
			for (int32 i = 0; i < e_money_type_max; ++i)
			{
				if (money_arr[i] > 0)
				{
					send_add_money_msg(i, money_arr[i]);
				}
			}
		}

		return true;
	}

	bool player::add_item_by_template_tuple(e_server_log_add_item add_type, int32 param, const std::vector<int32>& item_id_arr, const std::vector<int32>& item_num_arr, int32 is_locked, bool pop_msg, float fac, bool check_career, bool auto_to_money)
	{
		ZoneScoped;
		int32 len = item_id_arr.size();
		if (len != item_num_arr.size())
		{
			return false;
		}
		if (len <= 0 || fac <= 0)
		{
			return true;
		}

		std::vector<int32> item_arr(len * 2);
		for (int32 i = 0; i < len; ++i)
		{
			item_arr.push_back(item_id_arr[i]);
			item_arr.push_back(item_num_arr[i]);
		}

		int32 check_len = item_arr.size();
		if (check_len <= 0 || check_len % e_item_tuple_max != 0)
		{
			return false;
		}

		return add_item_by_template_tuple(add_type, param, item_arr, is_locked, pop_msg, fac, check_career, auto_to_money);
	}

	bool player::add_item_by_drop_items_list(const std::vector<s_item_template_info>& drop_item_list, bool pop_msg, float fac, bool check_career, bool auto_to_money)
	{
		ZoneScoped;
		int32 len = drop_item_list.size();
		if (len <= 0)
		{
			return true;
		}

		s_item_template_info item;
		std::vector<int32> items_arr(len * 2);
		for (int32 i = 0; i < len; ++i)
		{
			item = drop_item_list[i];
			items_arr.push_back(item.m_item_id);
			items_arr.push_back(item.m_item_num);
		}

		int32 check_len = items_arr.size();
		if (check_len <= 0 || check_len % e_item_tuple_max != 0)
		{
			return false;
		}

		return add_item_by_template_tuple(e_server_log_add_item_by_dtop_items, item.m_item_num, items_arr, 0, pop_msg, fac, check_career, auto_to_money);
	}

	void player::send_add_money_msg(int32 money_id, int32 money_num)
	{
		ZoneScoped;
		item_proto_add_money add_money_msg;
		add_money_msg.set_money_id(money_id);
		add_money_msg.set_money_num(money_num);
		send_message_to_self(&add_money_msg, e_msgindex_s2c_add_money);
	}
	void player::send_add_item_msg(int32 item_id, int32 item_num, bool send_by_mail)
	{
		ZoneScoped;
		item_proto_add_item add_item_msg;
		add_item_msg.set_item_id(item_id);
		add_item_msg.set_item_num(item_num);
		add_item_msg.set_send_by_mail(send_by_mail);
		send_message_to_self(&add_item_msg, e_msgindex_s2c_add_item);
	}

	void player::send_promp_msg_to_client(const std::vector<s_item_template_info>& items_array, const std::vector<s_item_template_info>& money_array)
	{
		ZoneScoped;
		if (items_array.empty() && money_array.empty())
		{
			CONSOLE_ERROR("items_array and money_array is empty");
			return;
		}
		const int32 item_template_info_array_size = items_array.size();
		std::vector<s_item_template_info> merge_array;

		for (auto& it : items_array)
		{
			const int32 item_id = it.m_item_id;
			const int32 item_num = it.m_item_num;
			const int32 item_is_lock = it.m_lock;
			bool is_merge = false;
			for (auto& merge_it : merge_array)
			{
				const int32 temp_item_id = merge_it.m_item_id;
				const int32 temp_item_is_lock = merge_it.m_lock;
				if (temp_item_id == item_id && temp_item_is_lock == item_is_lock)
				{
					merge_it.m_item_num += item_num;
					is_merge = true;
					break;
				}
			}
			if (is_merge == false)
			{
				merge_array.push_back(it);
			}
		}
		for (auto& it : money_array)
		{
			const int32 item_id = it.m_item_id;
			const int32 item_num = it.m_item_num;
			bool is_merge = false;
			for (auto& merge_it : merge_array)
			{
				const int32 temp_item_id = merge_it.m_item_id;
				if (temp_item_id == item_id)
				{
					merge_it.m_item_num += item_num;
					is_merge = true;
					break;
				}
			}
			if (is_merge == false)
			{
				merge_array.push_back(it);
			}
		}
		faith::item_proto_item_get_item_msg client_pak;
		client_pak.add_role_guid(get_unit_guid().A);
		client_pak.add_role_guid(get_unit_guid().B);

		for (auto& it : merge_array)
		{
			client_pak.add_item_data(it.m_item_id);
			client_pak.add_item_data(it.m_item_num);
			client_pak.add_item_data(it.m_lock);
		}

		send_message(&client_pak, e_msgindex_s2c_get_item);
	}

	void player::send_message_to_dp(const void* data, uint32 date_len)
	{
		ZoneScoped;
		connection_mgr::getInstance().send_to_dp(data, date_len, get_unit_info(e_role_info_server_id));
	}
	void player::send_message_to_dp_lua(const char* msg, int32 msg_len, uint32 header)
	{
		ZoneScoped;
		connection_mgr::getInstance().send_to_dp_lua(msg, msg_len, header, get_unit_info(e_role_info_server_id));
	}

	void player::send_message_to_dp_lua(google::protobuf::Message* net_pro, uint32 heade)
	{
		ZoneScoped;
		packet_s2s* data = parse_msg::getInstance().serialze_server(net_pro, heade, guid_64(), 0);
		if (nullptr == data)
		{
			return;
		}

		connection_mgr::getInstance().send_to_dp(data, data->get_packet_len(), get_unit_info(e_role_info_server_id));


	}
	void player::send_message_to_dp(google::protobuf::Message* net_pro, uint32 heade, int32 save_type_ex)
	{
		ZoneScoped;
		packet_s2s* data = parse_msg::getInstance().serialze_server(net_pro, heade, get_unit_guid(), get_array_index(), save_type_ex);
		if (nullptr == data)
		{
			return;
		}
		connection_mgr::getInstance().send_to_dp(data, data->get_packet_len(), get_unit_info(e_role_info_server_id));
	}
	void player::send_message_to_dp(google::protobuf::Message* net_pro, uint32 heade, int64 save_flag, int32 save_type_ex)
	{
		ZoneScoped;
		packet_s2s* data = parse_msg::getInstance().serialze_server(net_pro, heade, save_flag, get_unit_guid(), get_array_index(), save_type_ex);
		if (nullptr == data)
		{
			return;
		}
		connection_mgr::getInstance().send_to_dp(data, data->get_packet_len(), get_unit_info(e_role_info_server_id));
	}
	int32 player::send_message(const void* data, uint32 data_len)
	{
		ZoneScoped;
		if (nullptr == data || data_len <= 0 || m_array_index < 0)
		{
			CONSOLE_ERROR("send_message error data is nullptr or data_len is 0 or m_array_index is 0");
			return 0;
		}

		cell_server::getInstance().send_to_fep(data, data_len, m_client_uid.fepserver_uid);
		return 1;
	}

	int32 player::send_message(google::protobuf::Message* net_pro, uint32 header)
	{
		ZoneScoped;
		if (nullptr == net_pro || m_array_index < 0)
		{
			return false;
		}
		packet_c2s_s2c* data_ptr = parse_msg::getInstance().serialze_client(m_client_uid, net_pro, header);
		if (nullptr == data_ptr)
		{
			return false;
		}
		send_message(data_ptr, data_ptr->get_packet_len());
		return true;
	}

	void player::get_aoi_msg(faith::aoi_proto_unit_aoi_all& msg)
	{
		ZoneScoped;
		msg.set_unit_guid(get_unit_guid().server_64);
		msg.set_role_name(get_name());
		msg.set_role_appearance(get_appearance());
		msg.set_runtime_id(get_array_index());
		msg.set_n_type(get_unit_type());
		msg.set_template_id(get_unit_info(e_role_info_template_id));
		msg.clear_location();
		msg.add_location(get_new_map_pos().unit_location.x);
		msg.add_location(get_new_map_pos().unit_location.y);
		msg.add_location(get_new_map_pos().unit_location.z);
		msg.set_yaw(get_new_map_pos().unit_rotation.yaw);
		msg.set_exp_level(get_unit_info(e_role_info_exp_level));
		msg.set_upgrade_id(get_unit_info(e_role_info_upgrade_id));
		msg.set_show_fashion(get_unit_info(e_role_info_show_fashion));
		msg.set_head_frame_id(get_unit_info(e_role_info_head_frame));
		msg.set_server_id(get_unit_info(e_role_info_server_id));
		if (get_team_guid().is_valid())
		{
			aoi_proto_player_team_info* player_team_info = msg.mutable_team_info();
			if (nullptr == player_team_info)
			{
				return;
			}

			player_team_info->set_team_guid(get_team_guid().server_64);
			player_team_info->set_captain_guid(get_team_info().captain_guid.server_64);
			player_team_info->set_member_num(get_team_info().member_num);
		}
		if (get_legion_guid().is_valid())
		{
			aoi_proto_player_legion_info* player_legion_info = msg.mutable_legion_info();
			if (nullptr == player_legion_info)
			{
				return;
			}
			player_legion_info->set_legion_guid(get_legion_guid().server_64);
			player_legion_info->set_legion_name(get_legion_info().legion_name);
			player_legion_info->set_job_title(get_legion_info().job_title);
		}
		msg.set_branch_type(get_unit_info(e_role_info_class_branch));
		msg.set_vip_level(get_vip_level(true));
		msg.set_wing_template_id(get_unit_info(e_role_info_wing_showd_template_id));
		msg.set_mount_template_id(get_unit_info(e_role_info_mount_showd_template_id));
		for (int32 i = e_unit_attack_att_none; i < e_unit_attack_att_max; ++i)
		{
			msg.add_att_attack(get_pawn_att().get_attack_att_value(i));
		}
		for (int32 j = e_unit_game_att_movement; j < e_unit_game_att_max; ++j)
		{
			msg.add_att_info(get_pawn_att().get_game_att(j));
		}
		for (int32 j = e_unit_attack_state_dead; j < e_unit_attack_state_max; ++j)
		{
			msg.add_att_state(get_pawn_att().get_state_att(j));
		}
		for (int32 j = e_base_att_info_hp_cur; j < e_base_att_info_max; ++j)
		{
			msg.add_att_base(m_pawn_att.get_unit_base_att(j));
		}
		for (int32 j = 0; j < e_special_name_type_max; j = j + 2)
		{
			msg.add_special_name(m_special_name_mgr.get_name_level_by_type(j));
			msg.add_special_star(m_special_name_mgr.get_value_by_id(j + 1));
		}

		item_set& item_set_ref = item_manager::get_item_set(get_array_index());
		citem* weapon_ptr = item_set_ref.get_equip_item_by_slot(e_role_equip_slot_weapon_1);
		if (weapon_ptr)
		{
			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(weapon_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_add_on; ++i)
			{
				item_info_ptr->add_item_data(weapon_ptr->get_data_info((e_item_info)i));
			}
		}
		citem* mount_ptr = item_set_ref.get_showed_mount();
		if (mount_ptr)
		{
			citem* mount_illusion_ptr = item_set_ref.get_illusion_range_mount_or_wing(mount_ptr);
			if (mount_illusion_ptr)
			{
				item_proto_item_info* item_mount_illusion_info_ptr = msg.add_item_list();
				item_mount_illusion_info_ptr->set_item_guid(mount_illusion_ptr->get_item_guid().server_64);
				for (int32 i = 0; i < e_item_info_add_on; ++i)
				{
					item_mount_illusion_info_ptr->add_item_data(mount_illusion_ptr->get_data_info((e_item_info)i));
				}
			}

			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(mount_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_add_on; ++i)
			{
				item_info_ptr->add_item_data(mount_ptr->get_data_info((e_item_info)i));
			}
		}
		citem* wing_ptr = item_set_ref.get_showed_wing();
		if (wing_ptr)
		{
			citem* wing_illusion_ptr = item_set_ref.get_illusion_range_mount_or_wing(wing_ptr);
			if (wing_illusion_ptr)
			{
				item_proto_item_info* item_wing_illusion_info_ptr = msg.add_item_list();
				item_wing_illusion_info_ptr->set_item_guid(wing_illusion_ptr->get_item_guid().server_64);
				for (int32 i = 0; i < e_item_info_add_on; ++i)
				{
					item_wing_illusion_info_ptr->add_item_data(wing_illusion_ptr->get_data_info((e_item_info)i));
				}
			}

			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(wing_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_add_on; ++i)
			{
				item_info_ptr->add_item_data(wing_ptr->get_data_info((e_item_info)i));
			}
		}
		citem* fashion_weapon_ptr = item_set_ref.get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon);
		if (fashion_weapon_ptr)
		{
			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(fashion_weapon_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
			{
				item_info_ptr->add_item_data(fashion_weapon_ptr->get_data_info((e_item_info)i));
			}
		}

		citem* fashion_body_ptr = item_set_ref.get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_cloth);
		if (fashion_body_ptr)
		{
			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(fashion_body_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
			{
				item_info_ptr->add_item_data(fashion_body_ptr->get_data_info((e_item_info)i));
			}
		}


		cpatron_saint_mgr& patron_saint_mgr = get_patron_saint_mgr();
		msg.set_tinder_template_id(patron_saint_mgr.get_tinder_info(e_tinder_info_current_level_template_id));
		for (int32 belief_type = 0; belief_type < e_belief_series_type_max; belief_type++)
		{
			cbelief* temp_belief = m_belief_mgr.getbelief(belief_type);
			if (temp_belief == nullptr)
			{
				for (int32 i = 0; i < e_belief_info_max; ++i)
				{
					msg.add_belief_info(0);
				}
			}
			else
			{
				for (int32 i = 0; i < e_belief_info_max; ++i)
				{
					msg.add_belief_info(temp_belief->get_belief_ins_data(i));
				}
			}
		}

		if (had_couple())
		{
			msg.set_couple_name(get_couple_name());
		}
		msg.set_interaction_object_guid(m_interaction_mgr.get_interaction_guid().server_64);
		msg.set_interaction_type(m_interaction_mgr.get_real_interaction_type());
	}

	void player::send_message_to_aoi(google::protobuf::Message* net_pro, uint32 header, bool to_self, bool use_length)
	{
		ZoneScoped;
		if (m_array_index < 0)
		{
			return;
		}
		m_player_aoi_watch.send_message_to_aoi(net_pro, header, to_self, use_length);
	}

	void player::send_message_to_self(void* data, uint32 date_len)
	{
		ZoneScoped;
		if (data && date_len > 0)
		{
			send_message(data, date_len);
		}
	}

	void player::send_message_to_self(google::protobuf::Message* net_pro, uint32 header)
	{
		ZoneScoped;
		if (net_pro == nullptr)
		{
			return;
		}
		unit_man::log_msg_info(header, net_pro->ByteSize());
		send_message(net_pro, header);
	}

	void player::send_message_to_self_lua(const char* msg, int32 msg_len, uint32 header)
	{
		ZoneScoped;
		if (msg_len > MAX_C2S_S2C_PACKAGE_SIZE || header <= 0)
		{
			return;
		}
		//m_msg=packet_c2s_s2c();
		m_msg.wheader = header;
		m_msg.client_uid = get_client_uid();
		memcpy(m_msg.google_data, msg, msg_len);
		m_msg.google_data_len = msg_len;
		send_message((void*)&m_msg, m_msg.get_packet_len());
	}
	void  player::send_message_to_aoi_lua(const char* msg, int32 msg_len, uint32 header, bool to_self, bool use_length)
	{
		ZoneScoped;
		if (m_array_index < 0)
		{
			return;
		}
		m_player_aoi_watch.send_message_to_aoi(msg, msg_len, header, to_self, use_length);
	}

	void player::send_message_to_ws_lua(const void* data_ptr, int32 data_len, int32 server_id)
	{
		ZoneScoped;
		connection_mgr::getInstance().send_to_ws(data_ptr, data_len, server_id);
	}

	void player::refresh_heart_beat()
	{
	}

	/************************************************************************/
	/*								                                        */
	/************************************************************************/
	void player::tick_2_sec(const int64& new_time)
	{
		ZoneScoped;
		//��������
		ActivityCommonConfigTemplate* ActivityCommonConfigTemplatePtr = GET_TEMPLATE(ActivityCommonConfigTemplate, belief_cloister_attributeid_in_activity_tbl);
		if (ActivityCommonConfigTemplatePtr != nullptr && m_is_in_belief_cloister_battlefield)
		{
			int32 player_belief_stage_point = get_belief_rune_mgr().get_stage_point();
			if (player_belief_stage_point < ActivityCommonConfigTemplatePtr->ParamInt1)
			{
				get_belief_rune_mgr().add_stage_point(ActivityCommonConfigTemplatePtr->ParamInt3);
				faith::belief_rune_proto_belief_cloister_point_update msg;
				msg.set_point_value(get_belief_rune_mgr().get_stage_point());
				send_message_to_self(&msg, e_msgindex_s2c_belief_cloister_point_update);
			}
			else
			{
				transfer_by_template(get_unit_info(faith::e_role_info_main_map_id), 0, get_unit_info(e_role_info_server_id), guid_64(), 0);
			}
		}
		if (m_notice_id != 0)
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(m_notice_id);
			send_notice(notice_str);
			m_notice_id = 0;
		}
		m_recycle_mgr.tick();
		if (m_is_in_exp_raid)
		{
			//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(m_map_guid);
			//if (map_object_ptr != nullptr)
			//{
			//	int32 life_time = map_object_ptr->get_map_life_time() / second_tick_time;
			//	int32 map_begin_time = 0;
			//	MapTemplate* map_temp_ptr = map_object_ptr->get_map_template_ptr();
			//	if (nullptr != map_temp_ptr)
			//	{
			//		map_begin_time = map_temp_ptr->TimeBegin;
			//	}
			//	if (life_time >= map_begin_time && m_is_begin_exp_raid)
			//	{
			//		send_time_one(e_time_type_exp_raid_join_time);
			//	}
			//}
		}
		m_legion_cs_mgr.change_legion_terr_buff();
	}

	void player::heart_tick(const int64& new_time, const int32& tick_time)
	{
		ZoneScoped;
		if (e_session_status_in_gaming != m_status)
		{
			return;
		}
		time_info temp = time_helper::get_cur_time_new();

		if (get_loading_flag_all_finish() && m_teleport_map_id != -1)
		{
			if (true == m_teleport_map_pos.is_zero())
			{
				m_teleport_control.teleport_by_template(m_teleport_map_id, m_teleport_line_id, m_teleport_group_id, m_teleport_map_guid, m_teleport_war_index);
			}
			else
			{
				m_teleport_control.teleport_by_template_pos(m_teleport_map_id, m_teleport_line_id, m_teleport_map_pos, m_teleport_map_guid);
			}
			clear_trans_cache();
			return;
		}

		int64 diff_time = new_time - m_old_tick_time;
		m_old_tick_time = new_time;
		int64 login_time_all = get_time_data(e_time_type_login_time_all) + tick_time;
		set_time_data(e_time_type_login_time_all, login_time_all);
		int64 cur_level_time = get_time_data(e_time_type_cur_level_time) + tick_time;
		set_time_data(e_time_type_cur_level_time, cur_level_time);

		if (is_need_refresh_daily_info_at_zero_hour_no_cross())
		{
			if (is_need_refresh_week_info())
			{
				refresh_at_weekly();
			}
			refresh_at_zero_hour_no_cross(true);
		}
		if (is_need_refresh_daily_info_at_zero_hour_cross())
		{
			//ˢ�¿���
			refresh_at_zero_hour_cross(true);
		}

		if (is_need_refresh_gs_up_start_value_time())
		{
			for (int32 i = e_role_history_highest_record_gs_value; i <= e_role_history_highest_record_spirit_value; i++)
			{
				int64 record_value = get_highest_record(i);
				CONSOLE_INFO("player::heart_tick refresh_gs_up_start highest_record_type:{} value:{}", i, record_value);
				set_unit_i64_info_data((e_role_i64_info)(i - e_role_history_highest_record_gs_value + e_role_i64_info_history_best_gs_value), record_value);
			}

			int32 cur_time = time_helper::get_cur_time_new().second;
			set_time_data(e_time_type_last_refresh_gs_up_start_value_time, cur_time);
		}

		if (is_need_clear_harry_info_at_harry_time())
		{
			clear_harry_info_at_harry_time();
		}

		unit::heart_tick(new_time, tick_time);
		if (new_time >= m_min_tick)
		{
			m_min_tick = new_time + second_tick_time * (rand() % 30 + 30);
			m_convert_mgr.heart_tick(new_time);
			m_mail_mgr.heart_tick(new_time);

			//�κ���ˢ���йصĲ�����Ҫ��������ж�ǰ��
			refresh_daily_info(true);
			//map_record_mgr::get_instance().heart_tick(get_array_index(), new_time);

			if (is_need_refresh_month_info())
			{
				refresh_month_info();
			}
			check_sync_gs_to_ws();
		}

		if (new_time >= m_2sec_tick)
		{
			m_2sec_tick = new_time + second_tick_time * 2;
			tick_2_sec(new_time);
		}

		if (new_time >= m_hour_1_tick)
		{
			m_hour_1_tick = new_time + hour_tick_time;

			int64 test_local_time = time_helper::get_local_cur_time_stamp();//����ʱ��
			character_proto_syn_tick_time syn_time_msg;
			syn_time_msg.set_cur_server_time(test_local_time / 1000);
			syn_time_msg.set_cur_server_green_time(m_old_tick_time / 1000);
			send_message_to_self(&syn_time_msg, e_msgindex_s2c_syn_tick_time);

			send_open_server_time_to_self();
		}

		if (new_time >= m_sec_tick)
		{
			int64 second_diff_time = new_time - m_sec_tick + second_tick_time;
			if (m_sec_tick != 0)
			{
				open_bag(second_diff_time);
				storage_open(second_diff_time);
			}
			m_sec_tick = new_time + second_tick_time;
			m_arena_mgr.heart_tick(new_time);
			m_welfare_mgr.heart_tick(new_time);
			m_title_mgr.heart_tick(new_time);
			m_interaction_mgr.heart_tick(new_time);
			m_meditation_mgr.heart_tick(new_time);
			m_belief_mgr.heart_tick(new_time);
			m_belief_rune_mgr.heart_tick(new_time);
			m_time_limit_activity_mgr.heart_tick(new_time);
			m_role_competition_mgr.heart_tick(new_time);
			m_element_competition_mgr.heart_tick(new_time);
			m_lucky_card_mgr.heart_tick(new_time);
			m_star_trip_mgr.heart_tick(new_time);
			m_time_feed_back_mgr.heart_tick(new_time);
			m_time_limit_gift_mgr.heart_tick(new_time);
			update_team_member_pos_info_tick();//�����Աλ�ø���
			check_vip_experience(new_time);
			check_boss_challenge_restore_times(new_time / second_tick_time);
			m_auction_mgr.person_sec_tick();
			check_subscribe_daily();
			//if (new_time - get_time_data(e_time_type_treasure_jewel_free) > 0)
			//{
			//	set_logic_data(e_role_logic_info_jewel_free, 1);
			//	send_logic_one(e_role_logic_info_jewel_free);
			//	set_time_data(e_time_type_treasure_jewel_free, new_time + 10 * day_tick_time);
			//}
			if (new_time - get_time_data(e_time_type_treasure_element_free) > 0)
			{
				set_logic_data(e_role_logic_info_element_free, 1);
				send_logic_one(e_role_logic_info_element_free);
				set_time_data(e_time_type_treasure_element_free, new_time + 10 * day_tick_time);
			}
		}
		if (new_time >= m_half_sec_tick)
		{
			m_half_sec_tick = new_time + (half_second_tick_time / 2);
			m_map_record_mgr->heart_tick(new_time);
			auto_use_hp_pot();
		}
		m_worship_target.fast_heart_tick(new_time);
		m_mission_mgr.heart_tick(new_time);
		//����������CD�ܶ� ����һ�� ��Ҫÿ֡�� tick
		m_auction_mgr.heart_tick(new_time);
		online_award_system::heart_tick(this, new_time, tick_time);
		map_search_system::heart_tick(get_array_index(), new_time, tick_time);
		item_manager::heart_tick(get_array_index(), new_time);

		get_ranking_mgr().tick(new_time);

		//���鸱��
		if (m_is_in_exp_raid)
		{
			/*auto map_object_ptr = world_cs::get_map_by_guid<map_object>(m_map_guid);
			if (map_object_ptr != nullptr)
			{
				int32 life_time = map_object_ptr->get_map_life_time() / second_tick_time;
				int32 map_begin_time = 0;
				MapTemplate* map_temp_ptr = map_object_ptr->get_map_template_ptr();
				if (nullptr != map_temp_ptr)
				{
					map_begin_time = map_temp_ptr->TimeBegin;
				}
				if (m_is_begin_exp_raid == false)
				{
					if (m_is_need_add_exp_buff == false)
					{
						for (int32 i = 0; i < map_temp_ptr->MapBuffIdArray.size(); i += 2)
						{
							if (i + 1 < map_temp_ptr->MapBuffIdArray.size())
							{
								if (map_temp_ptr->MapBuffIdArray[i + 1] > e_class_type_none && map_temp_ptr->MapBuffIdArray[i + 1] != get_unit_info(e_role_info_class_type))
								{
									continue;
								}
							}
							buff_man::del_buff_by_template_id(get_array_index(), get_array_index(), map_temp_ptr->MapBuffIdArray[i]);
							m_is_need_add_exp_buff = true;
						}
					}
				}
				else
				{
					if (m_is_need_add_exp_buff)
					{
						for (int32 i = 0; i < map_temp_ptr->MapBuffIdArray.size(); i += 2)
						{
							if (i + 1 < map_temp_ptr->MapBuffIdArray.size())
							{
								if (map_temp_ptr->MapBuffIdArray[i + 1] > e_class_type_none && map_temp_ptr->MapBuffIdArray[i + 1] != get_unit_info(e_role_info_class_type))
								{
									continue;
								}
							}
							buff_man::add_buff_inst(get_array_index(), get_array_index(), map_temp_ptr->MapBuffIdArray[i]);
							m_is_need_add_exp_buff = false;
						}
					}
				}
				if (life_time >= map_begin_time && m_is_begin_exp_raid)
				{
					int64 last_exp_raid_time = get_time_data(e_time_type_exp_raid_join_time);
					int64 add_exp_raid_time = get_time_data(e_time_type_exp_raid_extra_time);
					VIPTemplate* vip_temp_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level(false));
					if (nullptr != vip_temp_ptr)
					{
						int64 max_exp_raid_time = get_daily_exp_raid_time() * second_tick_time + (add_exp_raid_time * second_tick_time);
						if (last_exp_raid_time >= max_exp_raid_time)
						{
							transfer_by_template(get_unit_info(faith::e_role_info_main_map_id), 0, 0, guid_64(), 0);
						}
						else if (last_exp_raid_time < max_exp_raid_time)
						{
							last_exp_raid_time += diff_time;
							if (last_exp_raid_time >= max_exp_raid_time)
							{
								last_exp_raid_time = max_exp_raid_time;
							}
							set_time_data(e_time_type_exp_raid_join_time, last_exp_raid_time);

							int32 cur_time = ((get_time_data(e_time_type_exp_raid_join_time) / second_tick_time) - add_exp_raid_time) / vip_temp_ptr->LimitConvertExpTime;
							if (cur_time < 0)
							{
								cur_time = 0;
							}
							bool need_send = cur_time != get_must_do_count(e_daily_must_do_typ_raid_exp);
							if (need_send)
							{
								set_must_do_count(e_daily_must_do_typ_raid_exp, cur_time);
								set_daily_active_degree_info(e_daily_must_do_typ_raid_exp);
								send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_exp);
							}
						}
					}
				}
			}*/
		}

		//�Ϲ��ż�
		int64 last_battlefield_time = get_time_data(e_time_type_ancient_battelfield);
		if (m_is_in_ancient_battlefield && last_battlefield_time <= 0)
		{
			transfer_by_template(get_unit_info(faith::e_role_info_main_map_id), 0, 0, guid_64(), 0);
		}
		else if (m_is_in_ancient_battlefield && last_battlefield_time > 0)
		{
			last_battlefield_time -= diff_time;
			if (last_battlefield_time < 0)
			{
				last_battlefield_time = 0;
			}
			set_time_data(e_time_type_ancient_battelfield, last_battlefield_time);
			if (m_add_ancient_activity_degree == false)
			{
				int cur_time = GAMECONFIG->AncientBattlefieldTime / 60 - get_time_data(e_time_type_ancient_battelfield) / minute_tick_time;
				bool need_send = cur_time != get_must_do_count(e_daily_must_do_typ_raid_ancient_battlefield);
				if (need_send)
				{
					set_must_do_count(e_daily_must_do_typ_raid_ancient_battlefield, cur_time);
					set_daily_active_degree_info(e_daily_must_do_typ_raid_ancient_battlefield);
					send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_ancient_battlefield);
				}
			}
		}
		//��������


		interact_with_npc_tick(new_time);
		set_random_num(new_time);

		if (new_time >= m_time_save_db && m_update_db_flag == false && m_client_uid.is_valid())
		{
			m_time_save_db = new_time + role_data_save_time;
			m_update_db_flag = true;
			clear_saving_flag(e_save_data_type_update);
			save_role_all_data(e_save_data_type_update);
		}
		if (new_time >= m_leave_time)
		{
			m_leave_time = new_time + minute_tick_time;
			if (get_client_uid().is_valid())
			{
				cs2fep_in_game msg;
				memcpy(msg.account, get_account(), max_account_length);
				msg.array_index = get_array_index();
				msg.client_uid = get_client_uid();
				msg.role_guid = get_unit_guid();
				send_message(&msg, sizeof(msg));
			}
			else
			{
				m_leave_num += 1;
				if (m_leave_num >= diconnect_repeat_num)
				{
					unit_man::logout_player(get_unit_guid(), get_array_index(), false, e_logout_result_time_out);
				}
			}
		}
		if (m_summon_tick && m_summon_tick < new_time)
			summon_all_pet();
	}

	bool player::is_need_refresh_week_info()
	{
		ZoneScoped;
		if (!is_self_server())
		{
			return false;
		}
		int64 last_refresh = get_time_data(e_time_type_last_refresh_by_zero_no_cross);
		if (last_refresh <= 0)
		{
			return true;
		}
		time_info last_refresh_time_info = time_helper::get_time_by_stamp_new(last_refresh);
		time_info cur_time_info = time_helper::get_cur_time_new();

		if (last_refresh_time_info.year != cur_time_info.year
			|| last_refresh_time_info.week_in_year != cur_time_info.week_in_year)
		{
			return true;
		}
		return false;
	}

	void player::check_vip_experience(const int64& new_time)
	{
		ZoneScoped;
		if (get_unit_info(e_role_info_vip_experience_level) > 0
			&& new_time > get_time_data(e_time_type_vip_experience_level_time))
		{
			VIPTemplate* old_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
			if (old_vip_template_ptr == nullptr)
			{
				return;
			}

			int32 old_experience_level = get_unit_info(e_role_info_vip_experience_level);
			set_unit_info(e_role_info_vip_experience_level, 0);
			send_info_one(e_role_info_vip_experience_level);

			//set_time_data(e_time_type_vip_experience_level_time, 0);
			//send_time_one(e_time_type_vip_experience_level_time);

			refresh_vip_title();

			vip_buff_change(old_experience_level, false);
			vip_buff_change(get_vip_level(), true);

			VIPTemplate* cur_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
			if (cur_vip_template_ptr == nullptr)
			{
				return;
			}
			if (old_vip_template_ptr->ExtraJewelSlotNum > cur_vip_template_ptr->ExtraJewelSlotNum)
			{
				item_set& self_item_set = get_item_set();
				for (int32 i = e_role_equip_slot_weapon_1; i < e_role_equip_slot_amulet; ++i)
				{
					citem* temp_item_ptr = self_item_set.get_item_by_slot(e_bag_type_equip, i);
					if (temp_item_ptr == nullptr)
					{
						continue;;
					}
					for (int32 j = e_item_info_jewel_vip_slot_0; j <= e_item_info_jewel_vip_slot_1; ++j)
					{
						int32 jewel_req_num = e_item_info_jewel_vip_slot_1 - j + 1;
						if (cur_vip_template_ptr->ExtraJewelSlotNum < jewel_req_num)
						{
							self_item_set.set_jewel_off(temp_item_ptr->get_item_guid(), j);
						}
					}
				}
			}
		}
	}

	void player::open_bag(const int64& diff_time)
	{
		ZoneScoped;
		// ����Ҫ��������������Ҫ��ʱ��
		int32 old_bag_num = get_logic_data(e_role_logic_info_bag_open);
		if (old_bag_num < MAX_ITEM_BAG_NUM) // ���if��ʾ������ȫ���������ʱ�������ܽ��Զ��ر�
		{
			set_bag_slot_open_num(MAX_ITEM_BAG_NUM);
			send_open_bag_info();
			//m_role_time.data_ary[e_time_type_bag_time] -= diff_time;
			//if (m_role_time.data_ary[e_time_type_bag_time] <= 0)
			//{
			//	int32 new_bag_num = old_bag_num + 1;
			//	for (int32 i = old_bag_num; i < MAX_ITEM_BAG_NUM; ++i)
			//	{
			//		int32 new_bag_slot_open_time = 0;
			//		BagExtendTemplate* bag_extend_template_ptr = GET_TEMPLATE(BagExtendTemplate, new_bag_num + 1 + bag_extend_template_first_id);
			//		if (nullptr != bag_extend_template_ptr)
			//		{
			//			new_bag_slot_open_time = bag_extend_template_ptr->Time * 1000;
			//		}
			//		if (new_bag_slot_open_time <= 0)
			//		{
			//			new_bag_num++;
			//		}
			//		else
			//		{
			//			m_role_time.data_ary[e_time_type_bag_time] = new_bag_slot_open_time;
			//			fix_bag_slot_open_time_by_vip(0, get_vip_level());
			//			break;
			//		}
			//	}
			//	new_bag_num = new_bag_num > MAX_ITEM_BAG_NUM ? MAX_ITEM_BAG_NUM : new_bag_num;
			//	set_bag_slot_open_num(new_bag_num);
			//	send_open_bag_info();
			//}
		}
	}

	void player::storage_open(const int64& diff_time)
	{
		ZoneScoped;
		int32 old_storage_num = get_logic_data(e_role_logic_info_storage_open);
		if (old_storage_num < max_item_storage_num) // ���if��ʾ������ȫ���������ʱ�������ܽ��Զ��ر�
		{
			set_storage_slot_open_num(max_item_storage_num);
			send_open_bag_info();
			//m_role_time.data_ary[e_time_type_storage_time] -= diff_time;
			//if (m_role_time.data_ary[e_time_type_storage_time] <= 0)
			//{
			//	int32 new_storage_num = old_storage_num + 1;
			//	for (int32 i = old_storage_num; i < max_item_storage_num; ++i)
			//	{
			//		int32 new_storage_slot_open_time = 0;
			//		BagExtendTemplate* storage_extend_template_ptr = GET_TEMPLATE(BagExtendTemplate, new_storage_num + 1 + template_storage_id);
			//		if (nullptr != storage_extend_template_ptr)
			//		{
			//			new_storage_slot_open_time = storage_extend_template_ptr->Time * 1000;
			//		}
			//		if (new_storage_slot_open_time <= 0)
			//		{
			//			new_storage_num++;
			//		}
			//		else
			//		{
			//			m_role_time.data_ary[e_time_type_storage_time] = new_storage_slot_open_time;
			//			fix_storage_slot_open_time_by_vip(0, get_vip_level());
			//			break;
			//		}
			//	}
			//	new_storage_num = new_storage_num > max_item_storage_num ? max_item_storage_num : new_storage_num;
			//	set_storage_slot_open_num(new_storage_num);
			//	send_open_bag_info();
			//}
		}
	}

	void player::send_open_bag_info()
	{
		ZoneScoped;
		int32 bag_open_num = get_logic_data(e_role_logic_info_bag_open);
		int32 bag_slot_open_time = m_role_time.data_ary[e_time_type_bag_time];
		int32 storage_open_num = get_logic_data(e_role_logic_info_storage_open);
		int32 storage_slot_open_time = m_role_time.data_ary[e_time_type_storage_time];

		character_proto_bag_slot_open_info bag_slot_info_msg;
		bag_slot_info_msg.set_role_guid(get_unit_guid().server_64);
		bag_slot_info_msg.set_bag_slot_open_num(bag_open_num);
		bag_slot_info_msg.set_open_next_need_time(bag_slot_open_time);
		bag_slot_info_msg.set_storage_slot_open_num(storage_open_num);
		bag_slot_info_msg.set_open_storage_next_need_time(storage_slot_open_time);
		send_message_to_self(&bag_slot_info_msg, e_msgindex_s2c_bag_open_slot_num);
	}

	void player::set_loading_flag(e_data_flag flag)
	{
		ZoneScoped;
		m_loading_flag.erase(flag);
		CONSOLE_INFO("flag:{} loading_flag_size:{} time:{}", int32(flag), m_loading_flag.size(), utility::get_tick_count());
		if (m_loading_flag.empty() == false)
		{
			return;
		}
		if (e_session_status_dp_loading != m_status)
		{
			return;
		}
		m_status = e_session_status_client_loading;
		init_by_inst_data_over();

		CONSOLE_INFO("player enter game end guid:{}, map guid:{}", get_unit_guid().server_64, get_map_guid().server_64);

		world_cs::player_load_data_over(get_map_ent(), this);
		cs2ws_enter_game rep;
		rep.client_uid = m_client_uid;
		rep.eResult = cs2ws_enter_game::e_success;
		rep.char_info_to_ws.role_guid = get_unit_guid();
		rep.char_info_to_ws.scene_uid = get_unit_info(e_role_info_move_map_id);
		rep.char_info_to_ws.line_uid = get_line_id();
		rep.char_info_to_ws.map_pos = get_new_map_pos();
		rep.char_info_to_ws.array_index = get_array_index();
		rep.map_guid = get_map_guid();
		rep.map_template_id = base_map_system::get_map_template_id(m_map_ent);
		connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));

		ws2fep_enter_game rep_fep;
		rep_fep.client_uid = get_client_uid();
		rep_fep.e_result = e_error_code_success;
		rep_fep.cs_array_index = get_array_index();
		rep_fep.server_type = e_server_type_cs;
		memcpy(rep_fep.account, get_account(), sizeof(rep_fep.account));
		send_message_to_self(&rep_fep, sizeof(rep_fep));




		if (is_already_login() == false)
		{
			set_is_already_login(true);
			m_login_time = utility::get_tick_count();
			if (get_write_log())
			{
				int64 login_time = get_login_time();
				int64 online_time = utility::get_tick_count() - login_time;
				int64 create_time = get_unit_i64_info_data(e_role_i64_info_create_time);
				server_log::enter_game_role_log(m_login_third_data, m_role_info, m_money_info, online_time, create_time, m_login_type);
				set_log_var(log_head)
					get_log_common_head_info(log_head);
				server_log::serverRoleLogin(log_head);

			}
			get_achievement_mgr().player_changed(eat_login_all_count);
		}
	}

	void player::clear_saving_flag(int32 type)
	{
		ZoneScoped;
		if (type >= e_save_data_type_max || type < 0)
		{
			return;
		}
		m_saving_flag[type].clear();

	}
	void player::set_saving_flag(int32 type, e_data_flag flag)
	{
		ZoneScoped;
		if (type >= e_save_data_type_max || type < 0)
		{
			return;
		}

		m_saving_flag[type].erase(flag);
	}
	void player::save_role_base_info(int32 save_type_ex)
	{
		ZoneScoped;
		int64 now_time = utility::get_tick_count();
		set_time_data(e_time_type_last_save_data_time, now_time);
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_character);
		if (is_use == false)
		{
			cs2dp_save_character request;
			request.unit_array_index = m_array_index;
			request.save_type_ex = save_type_ex;
			memcpy(&(request.role_info), &m_role_info, sizeof(request.role_info));
			send_message_to_dp(&request, sizeof(request));
		}
		else
		{
			faith::cs2dp_proto::save_role_info msg;
			msg.set_role_guid(get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_type_ex);
			faith::cs2dp_proto::role_info_db* db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			db_data->set_role_guid(m_role_info.role_guid.server_64);
			db_data->set_account(m_role_info.account);
			db_data->set_role_name(m_role_info.role_name);
			db_data->set_role_appearance(m_role_info.role_appearance);
			db_data->set_role_first_kill_ary(m_role_info.role_first_kill_npc_id_arr);
			db_data->set_map_guid(m_role_info.map_guid.server_64);
			for (int32 i = 0; i < e_role_i64_info_max; i++)
			{
				db_data->add_data_i64_ary(m_role_info.data_i64_ary[i]);
			}
			for (int32 i = 0; i < e_role_info_max; i++)
			{
				db_data->add_data_ary(m_role_info.data_ary[i]);
			}
			send_message_to_dp(&msg, e_msgindex_cs2dp_save_character);
		}
	}
	void player::load_role_data_online(const guid_64& up_role_guid)
	{
		ZoneScoped;
		cs2dp_load_data_online reqloading;
		reqloading.role_guid = get_unit_guid();
		reqloading.up_role_guid = up_role_guid;
		reqloading.unit_array_index = get_array_index();
		send_message_to_dp(&reqloading, sizeof(cs2dp_load_data_online));
	}
	void player::send_role_data_online()
	{

	}
	void player::save_role_all_data(e_save_role_data_type eType)
	{
		ZoneScoped;
		// return; /////////////////// TODO
		if (get_loading_flag_all_finish())
		{
			int64 now_tick = utility::get_tick_count();
			if (m_saving_flag[eType].empty() == false)
			{
				return;
			}
			m_role_info.map_guid = get_map_guid(); //Ϊ��ȷ��һ��
			offline_award_system::shut_down(this);
			switch (eType)
			{
			case e_save_data_type_exit_game:
			{
				set_time_data(e_time_type_logout_time, time_helper::get_cur_time_new().second);
				get_pawn_att().stop_tick();
				m_status = e_session_status_create_player;
				send_role_info_to_gm();
				world_cs::player_leave_scene(get_map_ent(), this);
				get_special_name_mgr().sync_military_rank_to_ws();
				check_sync_gs_to_ws();
				server_log::jewel_count_log(get_unit_info_inst(), get_logic_info_inst(), get_third_info(), get_money_data_inst(), get_login_type());
				set_log_var(log_head)
					get_log_common_head_info(log_head);

				int32 onlineTime = m_welfare_mgr.get_active_degree_info(e_daily_active_degree_type_online_30_minutes);

				server_log::serverEarlyWarning(log_head, onlineTime);
			}
			break;
			case e_save_data_type_leave_scene:
			{
				get_pawn_att().stop_tick();
				m_status = e_session_status_create_player;
				world_cs::player_leave_scene(get_map_ent(), this);
				get_special_name_mgr().sync_military_rank_to_ws();
				check_sync_gs_to_ws();
			}
			break;
			case e_save_data_type_update:
			{
			}
			break;
			case e_save_data_type_cross:
			{
				m_role_info.map_guid = m_move_map_guid;//�������ʱ��map_guidȡҪ����ĵ�ͼguid
				get_pawn_att().stop_tick();
				m_status = e_session_status_create_player;
				world_cs::player_leave_scene(get_map_ent(), this);
				get_special_name_mgr().sync_military_rank_to_ws();
				check_sync_gs_to_ws();
			}
			break;
			}

			m_saving_flag[eType].insert(e_data_flag_info);
			m_saving_flag[eType].insert(e_data_flag_att);
			m_saving_flag[eType].insert(e_data_flag_skill);
			m_saving_flag[eType].insert(e_data_flag_money);
			m_saving_flag[eType].insert(e_data_flag_buff);
			m_saving_flag[eType].insert(e_data_flag_time_activity);
			//m_saving_flag[eType].insert(e_data_flag_sprite_fetter);
			m_saving_flag[eType].insert(e_data_flag_mission);
			m_saving_flag[eType].insert(e_data_flag_goods);
			m_saving_flag[eType].insert(e_data_flag_map);
			m_saving_flag[eType].insert(e_data_flag_item_upgrade);
			m_saving_flag[eType].insert(e_data_flag_special_name);
			m_saving_flag[eType].insert(e_data_flag_achievement);
			m_saving_flag[eType].insert(e_data_flag_pokedex);
			m_saving_flag[eType].insert(e_data_flag_tinder);
			m_saving_flag[eType].insert(e_data_flag_worship);
			m_saving_flag[eType].insert(e_data_flag_yesterday_must_do_remain);
			m_saving_flag[eType].insert(e_data_flag_convert);
			m_saving_flag[eType].insert(e_data_flag_time);
			m_saving_flag[eType].insert(e_data_flag_welfare);
			m_saving_flag[eType].insert(e_data_flag_active_degree);
			m_saving_flag[eType].insert(e_data_flag_title);
			m_saving_flag[eType].insert(e_data_flag_talent);
			m_saving_flag[eType].insert(e_data_flag_highest_record);
			m_saving_flag[eType].insert(e_data_flag_service_goal);
			m_saving_flag[eType].insert(e_data_flag_daily_must_do_count);
			m_saving_flag[eType].insert(e_data_flag_service_rank);
			m_saving_flag[eType].insert(e_data_flag_logic_info);
			m_saving_flag[eType].insert(e_data_flag_gain_treasure);
			m_saving_flag[eType].insert(e_data_flag_time_limit_activity);
			m_saving_flag[eType].insert(e_data_flag_boss_island);
			m_saving_flag[eType].insert(e_data_flag_seven_day_goal);
			m_saving_flag[eType].insert(e_data_flag_phantom);
			m_saving_flag[eType].insert(e_data_flag_chat_record);
			m_saving_flag[eType].insert(e_data_flag_player_information);
			m_saving_flag[eType].insert(e_data_flag_recycle);
			m_saving_flag[eType].insert(e_data_flag_recycle_task);
			m_saving_flag[eType].insert(e_data_flag_player_competition);
			m_saving_flag[eType].insert(e_data_flag_dragontrip);
			m_saving_flag[eType].insert(e_data_flag_legion_skill);
			m_saving_flag[eType].insert(e_data_flag_starark);
			m_saving_flag[eType].insert(e_data_flag_element_competition);
			m_saving_flag[eType].insert(e_data_flag_subscribe_daily_info);
			for (int32 i = e_bag_type_none + 1; i < e_bag_type_max; ++i)
			{
				m_saving_flag[eType].insert(e_data_flag_item + i);
			}


			save_role_base_info(eType);//��������һ��
			m_treasure_mgr.save_treasure_to_db(eType);
			save_money(eType);
			save_role_time(eType);
			save_role_logic(eType);
			m_passive_skill.save_passive_skill(eType);
			m_legion_skill.save_legion_skill(eType);
			m_pawn_att.save_base_att_to_db(eType);
			m_talent_mgr.save_talent_to_db(eType);
			m_pokedex_mgr.save_pokedex_to_db(eType);
			m_mission_mgr.save_mission_to_db(eType);
			m_store_helper.save_purchase_record_to_db(eType);
			m_special_name_mgr.save_data_to_db(eType);
			m_achievement_mgr.save_achievement_to_db(eType);
			m_worship_target.save_worship_target_to_db(eType);
			m_ranking_mgr.save_ranking_to_db();
			m_convert_mgr.save_data_to_db(eType);
			m_welfare_mgr.save_welfare_to_db(eType);
			m_welfare_mgr.save_active_degree_to_db(eType);
			m_welfare_mgr.save_random_get_to_db(eType);
			m_title_mgr.save_title_to_db(eType);
			m_patron_saint_mgr.save_tinder_info_to_db(eType);
			save_daily_must_do_count(eType);
			save_yesterday_must_do_remain(eType);
			save_history_highest_record(eType);
			m_service_goal_mgr.save_service_goal_to_db(eType);
			m_seven_day_goal_mgr.save_seven_day_goal_to_db(eType);
			//get_item_set().save_item_to_db(eType);
			item_system::save_data_to_db(this, eType);
			get_item_set().save_show_info_to_db(eType);
			item_upgrade_system::save_data_to_db(this, eType);
			time_activity_system::save_data_to_db(this, eType);
			skill_manager::save_skill(get_array_index(), eType);
			buff_man::save(get_array_index(), eType);
			m_map_record_mgr->save_map_record_to_db(eType);
			m_guide_mgr.logout_save_guide_record();
			m_service_rank_mgr.save_recevice_rewards_state_to_db(eType);
			m_gain_treasure_mgr.save_gain_treasure_info_to_db(eType);
			m_lucky_card_mgr.set_limit_activiy_info();
			m_star_trip_mgr.set_limit_activiy_info();
			m_time_limit_activity_mgr.save_data_to_db(get_array_index(), eType);
			m_boss_island_mgr.save_boss_island_to_db(eType);
			m_server_harry_mgr.save_role_harry_info_to_db(eType);
			m_phantom_mgr.save_phantom_to_db(eType);

			m_chat_mgr.save_chat_record(eType);

			m_recycle_mgr.save_recycle_to_db(eType);

			m_recycle_mgr.save_recycle_task_to_db(eType);

			m_oracle_trial_mgr.save_oracle_trial_to_db(eType);

			m_person_infor_mgr.save_to_db(eType);
			m_role_competition_mgr.save_to_db(eType);
			m_dragontrip_mgr.save_dragontrip_to_db(eType);
			m_starark_mgr.save_starark_to_db(eType);
			m_element_competition_mgr.save_to_db(eType);
			m_time_feed_back_mgr.save_info(eType);
			m_time_limit_gift_mgr.save_info(eType);
			save_subscribe_daily_info(eType);
			save_obj_lua(eType);
		}
		else
		{
			switch (eType)
			{
			case e_save_data_type_exit_game:
			{
				unit_man::logout_player(m_role_info.role_guid, get_array_index(), false, e_logout_result_load_data_fail);
			}
			break;
			case e_save_data_type_leave_scene:
			case e_save_data_type_cross:
			{
				m_teleport_control.m_teleporting = false;
			}
			break;
			case e_save_data_type_update:
			{
				;
			}
			break;
			}
		}
	}

	void player::map_in_out_hp_process()
	{
		ZoneScoped;
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, get_unit_info(e_role_info_move_map_id));
		if (nullptr == map_template_ptr)
		{
			return;
		}
		if (map_template_ptr->HpOperateWhenOut)
		{
			double now_hp = get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
			double main_hp = get_pawn_att().get_unit_base_att(e_base_att_info_hp_main);
			change_hp(now_hp - main_hp, get_array_index(), false, 0);
		}
	}

	void player::push_all_info_to_client()
	{
		ZoneScoped;
		send_unit_begin_info();
		buff_man::sync_buff_set_info(get_array_index(), get_array_index());
		skill_manager::send_skill_all(get_array_index());
		m_map_record_mgr->send_map_record_all();
		get_pawn_att().syn_all_att();
		get_pawn_att().send_base_att_all();
		get_item_set().send_item_all();
		send_money_all();
		send_time_all();
		send_logic_all();
		get_belief_mgr().send_belief_all();
		get_talent_mgr().send_talent_all();
		get_pokedex_mgr().send_pokedex_all();
		get_patron_saint_mgr().send_tinder_info_all();
		get_worship_target().send_worhip_record();
		get_welfare_mgr().send_welfare_all();
		get_welfare_mgr().send_active_degree();
		get_title_mgr().push_msg_own_title_all();
		get_convert_mgr().send_info_all();
		get_mission_mgr().send_all_mission();
		get_goods_helper_mgr().send_store_goods_record_all();
		//map_record_set_ref.send_demons_tower_info();
		get_special_name_mgr().send_special_name_all();
		get_achievement_mgr().send_all_achievement();
		get_guide_mgr().send_all_guide_record();
		get_first_time_do_mgr().send_all_first_time_do_records();
		send_history_highest_record_all();
		get_auction_mgr().send_selling_list_all();
		get_service_goal_mgr().send_all_service_goal();
		update_three_day_must_do_remain();
		send_daily_must_do_count_info_all();
		get_gain_treasure_mgr().send_gain_treasure_info_all();
		get_boss_island_mgr().send_player_boss_island_info_all();
		get_seven_day_goal_mgr().send_all_seven_day_goal();
		reconnect_send_red_package_info();
		get_time_limit_activity_mgr().send_open_activity_all();
		get_time_limit_activity_mgr().send_open_activity_branch_info_all();
		get_phantom_mgr().sync_all_message_to_client();
		get_chat_mgr().sync_all_chat_record_to_client();
		get_recycle_mgr().sync_all_message_to_client();
		get_recycle_mgr().sync_all_invited_list();
		get_person_infor_mgr().sync_to_client();
		get_competition_mgr().sync_competition_info();
		get_element_competition_mgr().sync_competition_info();
		get_welfare_mgr().get_first_recharge_welfare_by_id();
	}

	void player::on_player_disconnect()
	{
		ZoneScoped;
		s_client_uid client_uid(0);
		set_client_uid(client_uid);

		world_cs::player_leave_scene(get_map_ent(), this);

		m_status = e_session_status_logout_connect;
		//��ֹ����
		get_marry_mgr().confirm_marry_operate(1, 0, 0);
	}

	void player::on_player_reconnect(const s_client_uid& client_uid, int32 client_map_id, int32 client_line_id)
	{
		ZoneScoped;
		if (m_status != e_session_status_logout_connect)
		{
			return;
		}
		set_client_uid(client_uid);

		auto base_map_cp = m_map_ent->get_component<base_map_component>();
		if (false == base_map_cp.isValid())
		{
			CONSOLE_ERROR("player::on_player_reconnect base_map_cp is nullptr");
			return;
		}
		if (client_map_id != base_map_cp->m_map_id || client_line_id != base_map_cp->m_line_id)
		{
			m_teleport_control.re_teleport_to_local();
			return;
		}
		m_status = e_session_status_in_gaming;
		//if (e_session_status_in_gaming == m_status)
		{
			// ���push��Ҫ��ǰ��
			push_all_info_to_client();
		}
		get_team_cs_mgr().on_player_reconnect();
		world_cs::player_reconnect(get_map_ent(), this);
		summon_all_pet();
		int64 test_local_time = time_helper::get_local_cur_time_stamp();//����ʱͬ��һ��ʱ��
		character_proto_syn_tick_time syn_time_msg;
		syn_time_msg.set_cur_server_time(test_local_time / 1000);
		syn_time_msg.set_cur_server_green_time(m_old_tick_time / 1000);
		send_message_to_self(&syn_time_msg, e_msgindex_s2c_syn_tick_time);
		game_proto_reconnect_enter_scene_end msg_reconnect_enter_end;
		msg_reconnect_enter_end.set_result(e_error_code_success);
		send_message_to_self(&msg_reconnect_enter_end, e_msgindex_s2c_reconnect_enter_scene);

		cmission* mission_ptr = get_mission_mgr().find_mission_by_index(e_mission_slot_main);
		if (nullptr != mission_ptr)
		{
			mission_ptr->check_self_see_npc();
		}
		send_open_server_time_to_self();
	}

	void player::save_yesterday_must_do_remain(int32 save_typ)
	{
		ZoneScoped;
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_yesterday_must_do);
		if (is_use == false)
		{
			cs2dp_save_character_yesterday_remain_must_do req;

			req.save_type_ex = save_typ;
			req.unit_array_index = m_array_index;
			req.role_guid = get_unit_guid();
			for (int32 i = 0; i < resource_find_back_day; i++)
			{
				req.data_info[i] = m_three_day_must_do_remain_ary[i];
			}
			send_message_to_dp(&req, sizeof(cs2dp_save_character_yesterday_remain_must_do));
		}
		else
		{
			faith::cs2dp_proto::save_role_yesterday_must_do_remain msg;
			msg.set_role_guid(get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_typ);

			faith::cs2dp_proto::role_yesterday_must_do_remain_db* db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}

			int32 row_count = 0;
			for (int32 i = 0; i < resource_find_back_day; i++)
			{
				faith::cs2dp_proto::role_yesterday_must_do_remain_row* db_row = db_data->add_row_data();
				if (db_row == nullptr)
				{
					return;
				}
				db_row->set_role_guid(m_three_day_must_do_remain_ary[i].role_guid.server_64);
				db_row->set_role_exp_level_yesterday(m_three_day_must_do_remain_ary[i].role_exp_level_yesterday);
				db_row->set_save_date(m_three_day_must_do_remain_ary[i].save_date);
				db_row->set_role_vip_level_yesterday(m_three_day_must_do_remain_ary[i].role_vip_level_yesterday);
				db_row->set_distance_today_day_count(m_three_day_must_do_remain_ary[i].distance_today_day_count);
				db_row->set_role_arena_rank(m_three_day_must_do_remain_ary[i].role_arena_rank);

				for (int32 j = 0; j < e_daily_must_do_typ_max; j++)
				{
					db_row->add_data_ary(m_three_day_must_do_remain_ary[i].data_ary[j]);
				}
				row_count++;
			}
			db_data->set_row_count(row_count);
			if (row_count == 0)
			{
				db_data->add_row_data();
			}
			send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_yesterday_must_do);
		}
	}

	DailyActivitiesTemplate* player::get_find_back_template_ptr(int32 daily_activities_type, int32 exp_level)
	{
		ZoneScoped;
		DailyActivitiesTemplate* return_ptr = nullptr;
		switch (daily_activities_type)
		{
		case e_daily_must_do_typ_raid_exp:
		case e_daily_must_do_typ_raid_daily:
		case e_daily_must_do_typ_raid_ancient_battlefield:
		case e_daily_must_do_typ_raid_siliver:
		case e_daily_must_do_typ_angel_temple:
		case e_daily_must_do_typ_pk_king:
		case e_daily_must_do_typ_arena:
		case e_daily_must_do_typ_crystal_illusion:
		case e_daily_must_do_typ_crusade_mission:
		case e_daily_must_do_typ_belief_cloister:
		case e_daily_must_do_typ_boss_home:
		case e_daily_must_do_typ_single_boss:
		case e_daily_must_do_typ_boss_island:
		case e_daily_must_do_typ_gold_army:
		case e_daily_must_do_typ_vip_boss_home:
		{
			return_ptr = template_manager::get_instance().get_template_by_daily_activity_type(daily_activities_type, exp_level);
		}
		break;
		case e_daily_must_do_typ_raid_devil_square:
		case e_daily_must_do_typ_bloody_castle:
		case e_daily_must_do_typ_raid_multiple_princess_guard_war:
		case e_daily_must_do_typ_raid_multiple_lava_hellfire:
		case e_daily_must_do_typ_raid_multiple_abyss_of_fear:
		case e_daily_must_do_typ_mingmie_xukong:
		{
			int32 higest_index = -1;
			std::vector<DailyActivitiesTemplate*> temp_template_array = template_manager::get_instance().get_template_array_by_daily_activity_type(daily_activities_type, exp_level);
			int32 temp_array_size = temp_template_array.size();
			for (int32 array_index = 0; array_index < temp_array_size; array_index++)
			{
				if (nullptr == temp_template_array[array_index])
				{
					continue;
				}
				MapTemplate* new_map_ptr = GET_TEMPLATE(MapTemplate, temp_template_array[array_index]->MapTemplateId);
				if (new_map_ptr == nullptr /*|| temp_record.is_valid() == false*/)
				{
					continue;
				}

				if (!m_func_unlock_mgr.is_func_unlock(temp_template_array[array_index]->FuncUnlockName))
				{
					continue;
				}

				if (new_map_ptr->EnterLevel > exp_level)
				{
					continue;
				}

				//if (temp_record.get_data_info(e_map_record_info_fastest_time) > 0)
				//{
				//	if (higest_index == -1)
				//	{
				//		higest_index = array_index;
				//	}
				//	else
				//	{
				//		MapTemplate* old_map_ptr = GET_TEMPLATE(MapTemplate, temp_template_array[higest_index]->MapTemplateId);
				//		if (old_map_ptr != nullptr)
				//		{
				//			if (new_map_ptr->Difficulty > old_map_ptr->Difficulty)
				//			{
				//				higest_index = array_index;
				//			}
				//		}
				//	}
				//}
				//else
				//{
					//û��ͨ�ؼ�¼����򵥵��һ�
				if (-1 == higest_index)
				{
					if (0 == new_map_ptr->Difficulty)
					{
						higest_index = array_index;
					}
				}
				//}
			}

			if (higest_index >= 0 && higest_index < temp_array_size)
			{
				return_ptr = temp_template_array[higest_index];
			}
		}
		break;
		default:
			break;
		}
		return return_ptr;
	}
	int32 player::get_find_back_template_id(int32 daily_activities_type, int32 exp_level)
	{
		ZoneScoped;
		DailyActivitiesTemplate* temp = get_find_back_template_ptr(daily_activities_type, exp_level);
		if (temp == nullptr)
		{
			return 0;
		}
		return temp->attribute_id;
	}

	void player::refresh_three_day_must_do_remain_info(bool bOnline)
	{
		ZoneScoped;
		if (get_time_data(e_time_type_last_refresh_time) == 0)
		{//����0�����ºţ���Ҫ�����
			return;
		}

		int32 pass_day = 0;
		bool is_first = true;
		//��ʹ���ֻ��һ�죬cur_time - last_refresh_time Ҳ���ܴ���24Сʱ �� �����賿1�����ߵ������賿2������ ���25Сʱ
		//�� ���� ��ȥ24Сʱ һ��������
		//time_info yesterday_time_info = time_helper::get_time_by_stamp_new(m_three_day_must_do_remain_ary[resource_find_back_day - 2].save_date);
		int32 last_refresh_time = get_time_data(e_time_type_last_refresh_time);
		int32 cur_time = time_helper::get_cur_time_new().second;
		int32 delta_time = (cur_time - last_refresh_time) / 3600;
		time_info yesterday_time_info = time_helper::get_time_by_stamp_new(cur_time - day_time_second);

		if (delta_time > 24 && delta_time <= 24 * 2)
		{
			pass_day = 1;
		}
		if (delta_time > 24 * 2 && delta_time <= 24 * 3)
		{
			pass_day = 2;
		}
		if (delta_time > 24 * 3)
		{
			pass_day = 3;
		}
		pass_day++;
		int32 all_day = pass_day - 1;
		while (pass_day--)
		{
			tidy_three_day_mustdo_remain_ary();
			int32 cur_role_level = get_unit_info(e_role_info_exp_level);
			m_three_day_must_do_remain_ary[resource_find_back_day - 1].clear_data();
			m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_exp_level_yesterday = cur_role_level;
			m_three_day_must_do_remain_ary[resource_find_back_day - 1].save_date = time_helper::get_cur_time_new().second;
			m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_guid = get_unit_guid();
			m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_vip_level_yesterday = get_vip_level();
			m_three_day_must_do_remain_ary[resource_find_back_day - 1].distance_today_day_count = 3;
			m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_arena_rank = -1;
			if (bOnline)
			{
				m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_arena_rank = get_arena_mgr().current_rank();
			}

			for (int32 must_do_type = 0; must_do_type < e_daily_must_do_typ_max; ++must_do_type)
			{
				int32 temp_value = 0;
				DailyActivitiesTemplate* daily_must_do_template_ptr = get_find_back_template_ptr(must_do_type, get_unit_info(e_role_info_exp_level));
				if (daily_must_do_template_ptr == nullptr)
				{
					continue;
				}
				if (0 == daily_must_do_template_ptr->IsCanFindBack)
				{
					continue;
				}
				if (daily_must_do_template_ptr->ActivityOrder > 120000000)
				{
					ActivityCommonConfigTemplate* activity_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, daily_must_do_template_ptr->ActivityOrder);
					std::vector<int32> open_data = cell_server::getInstance().get_activity_open_data((daily_must_do_template_ptr->ActivityOrder) - 120000000);
					if (activity_template_ptr != nullptr
						&& open_data.size() > 0)
					{
						bool is_yesterday_has_this_activity = false;
						int32 this_day = -1;
						if (yesterday_time_info.day_in_week >= (all_day - pass_day))
						{
							this_day = yesterday_time_info.day_in_week - (all_day - pass_day);
						}
						else
						{
							this_day = all_day_in_week + yesterday_time_info.day_in_week - (all_day - pass_day);
						}
						for (int32 i = 0; i < open_data.size(); ++i)
						{
							if (this_day == open_data[i])
							{
								is_yesterday_has_this_activity = true;
								break;
							}
						}

						if (!is_yesterday_has_this_activity)
						{
							continue;
						}
					}
				}
				bool is_unlock = m_func_unlock_mgr.is_func_unlock(daily_must_do_template_ptr->FuncUnlockName);
				if (is_unlock)
				{
					switch (must_do_type)
					{
					case e_daily_must_do_typ_raid_devil_square:
					case e_daily_must_do_typ_raid_siliver:
					case e_daily_must_do_typ_bloody_castle:
					case e_daily_must_do_typ_raid_multiple_princess_guard_war:
					case e_daily_must_do_typ_raid_multiple_lava_hellfire:
					case e_daily_must_do_typ_raid_multiple_abyss_of_fear:
					case e_daily_must_do_typ_angel_temple:
					case e_daily_must_do_typ_mingmie_xukong:
					case e_daily_must_do_typ_belief_cloister:
					{
						MapTemplate* temp_map_ptr = GET_TEMPLATE(MapTemplate, daily_must_do_template_ptr->MapTemplateId);
						if (temp_map_ptr != nullptr && temp_map_ptr->EnterLevel <= cur_role_level)
						{
							if (is_first)
							{
								temp_value = temp_map_ptr->EnterCount - get_must_do_count((e_daily_must_do_typ)must_do_type) + get_buy_count_with_map_id(daily_must_do_template_ptr->MapTemplateId);
							}
							else
							{
								temp_value = temp_map_ptr->EnterCount + get_buy_count_with_map_id(daily_must_do_template_ptr->MapTemplateId);
							}
						}
					}
					break;
					case e_daily_must_do_typ_pk_king:
					{
						MapTemplate* temp_map_ptr = GET_TEMPLATE(MapTemplate, daily_must_do_template_ptr->MapTemplateId);
						if (temp_map_ptr != nullptr && temp_map_ptr->EnterLevel <= cur_role_level)
						{
							if (is_first)
							{
								temp_value = temp_map_ptr->EnterCount - get_must_do_count((e_daily_must_do_typ)must_do_type);

							}
							else
							{
								temp_value = temp_map_ptr->EnterCount;
							}
						}
					}
					break;
					case e_daily_must_do_typ_raid_daily:
					{
						if (get_mission_mgr().find_mission_by_index(e_mission_slot_daily) != nullptr)
						{
							if (is_first)
							{
								temp_value = DAILY_MISSION_MAX - get_unit_info(e_role_info_daily_done_num);
							}
							else
							{
								temp_value = DAILY_MISSION_MAX;
							}
						}
					}
					break;
					case e_daily_must_do_typ_raid_ancient_battlefield:
					{
						MapTemplate* temp_map_ptr = GET_TEMPLATE(MapTemplate, daily_must_do_template_ptr->MapTemplateId);
						if (temp_map_ptr != nullptr && temp_map_ptr->EnterLevel <= cur_role_level)
						{
							if (is_first)
							{
								temp_value = get_time_data(e_time_type_ancient_battelfield) / minute_tick_time;
							}
							else
							{
								temp_value = GAMECONFIG->AncientBattlefieldTime / 60;
							}
						}
					}
					break;
					case e_daily_must_do_typ_raid_exp:
					{
						MapTemplate* temp_map_ptr = GET_TEMPLATE(MapTemplate, daily_must_do_template_ptr->MapTemplateId);
						VIPTemplate* vip_temp_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level(false));
						if (temp_map_ptr != nullptr && temp_map_ptr->EnterLevel <= cur_role_level && nullptr != vip_temp_ptr)
						{
							if (is_first)
							{
								int64 left_time_sec = get_max_daily_exp_raid_time() * second_tick_time + get_time_data(e_time_type_exp_raid_extra_time) * second_tick_time - get_time_data(e_time_type_exp_raid_join_time);
								left_time_sec = left_time_sec > 0 ? left_time_sec : 0;
								temp_value = left_time_sec / minute_tick_time;
							}
							else
							{
								temp_value = get_max_daily_exp_raid_time() / 60;
							}
						}
					}
					break;
					case e_daily_must_do_typ_arena:
					{
						if (is_first)
						{
							temp_value = 1 - get_must_do_count((e_daily_must_do_typ)must_do_type);
							//temp_value = GAMECONFIG->ArenaTimes - get_unit_info(e_role_info_arena_times);
						}
						else
						{
							temp_value = 1;// GAMECONFIG->ArenaTimes;
						}
					}
					break;
					case e_daily_must_do_typ_crusade_mission:
					{
						if (get_mission_mgr().find_mission_by_index(e_mission_slot_against) != nullptr)
						{
							if (is_first)
							{
								temp_value = AGAINST_MISSION_MAX - get_unit_info(e_role_info_against_done_num);
							}
							else
							{
								temp_value = AGAINST_MISSION_MAX;
							}

						}
					}
					break;
					case e_daily_must_do_typ_crystal_illusion:
					{
						if (is_first)
						{
							temp_value = GAMECONFIG->MaxGatherCrystalCount - get_unit_info(e_role_info_crystal_dreamland_chests);
						}
						else
						{
							temp_value = GAMECONFIG->MaxGatherCrystalCount;
						}
					}
					break;
					case e_daily_must_do_typ_boss_home:
					{
						ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
						if (nullptr == act_common_config_ptr)
						{
							return;
						}
						int32 max_count = act_common_config_ptr->ParamInt2;
						if (is_first)
						{

							temp_value = max_count - this->get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
						}
						else
						{
							temp_value = max_count;
						}
					}
					break;
					case e_daily_must_do_typ_vip_boss_home:
					{
						ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_home);
						if (nullptr == act_common_config_ptr)
						{
							return;
						}
						int32 max_count = act_common_config_ptr->ParamInt2;
						if (is_first)
						{

							temp_value = max_count - this->get_logic_data(e_role_logic_info_boss_home_cur_used_times);
						}
						else
						{
							temp_value = max_count;
						}
					}
					break;
					case e_daily_must_do_typ_single_boss:
					{
						ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_single);
						if (nullptr == act_common_config_ptr)
						{
							return;
						}
						int32 max_count = act_common_config_ptr->ParamInt3;
						if (is_first)
						{

							temp_value = max_count - this->get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_simple_boss);
						}
						else
						{
							temp_value = max_count;
						}
					}
					break;
					case e_daily_must_do_typ_boss_island:
					{
						if (is_first)
						{
							temp_value = GAMECONFIG->BossIslandMaxKillNum - this->get_boss_island_mgr().get_boss_island_info(e_island_record_killed_island_boss);
						}
						else
						{
							temp_value = GAMECONFIG->BossIslandMaxKillNum;
						}
					}
					break;
					case e_daily_must_do_typ_gold_army:
					{
						ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
						if (nullptr == act_common_config_ptr)
						{
							continue;
						}
						std::vector<int32> open_time = cell_server::getInstance().get_activity_open_time(e_activity_type_world_boss);
						int32 active_max_num = open_time.size() / once_activity_open_time_array_size;
						if (cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss))
						{
							active_max_num -= 1;
						}
						if (is_first)
						{
							temp_value = active_max_num - get_must_do_count((e_daily_must_do_typ)must_do_type);
						}
						else
						{
							temp_value = active_max_num;
						}
					}
					break;
					default:
						break;
					}
				}
				if (temp_value < 0)
				{
					temp_value = 0;
				}
				m_three_day_must_do_remain_ary[resource_find_back_day - 1].data_ary[must_do_type] = temp_value;
			}
			is_first = false;
		}


		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			if (!m_three_day_must_do_remain_ary[i].role_guid.is_valid())
			{
				continue;
			}
			m_three_day_must_do_remain_ary[i].distance_today_day_count = i + 1;
		}
		return;
	}

	void player::update_three_day_arean_rank(int32 arena_rank)
	{
		ZoneScoped;
		if (arena_rank == -1)
		{
			return;
		}
		bool is_change = false;
		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			if (!m_three_day_must_do_remain_ary[i].role_guid.is_valid())
			{
				continue;
			}
			if (m_three_day_must_do_remain_ary[i].role_arena_rank == -1)
			{
				m_three_day_must_do_remain_ary[i].role_arena_rank = arena_rank;

				is_change = true;

			}
			if (m_three_day_must_do_remain_ary[i].data_ary[e_daily_must_do_typ_arena] > 1)
			{
				m_three_day_must_do_remain_ary[i].data_ary[e_daily_must_do_typ_arena] = 1;
				is_change = true;
			}
		}
		if (is_change)
		{
			update_three_day_must_do_remain();
		}
	}
	void player::tidy_three_day_mustdo_remain_ary()
	{
		ZoneScoped;
		for (int32 i = 0; i < resource_find_back_day - 1; i++)
		{
			memcpy(&m_three_day_must_do_remain_ary[i], &m_three_day_must_do_remain_ary[i + 1], sizeof(s_unit_yesterday_must_do_remain));
		}
	}
	void player::update_three_day_exp()
	{
		ZoneScoped;
		for (int32 i = 0; i < resource_find_back_day - 1; i++)
		{
			m_three_day_must_do_remain_ary[i].role_exp_level_yesterday = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_exp_level_yesterday;
			m_three_day_must_do_remain_ary[i].role_vip_level_yesterday = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_vip_level_yesterday;
			m_three_day_must_do_remain_ary[i].role_arena_rank = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_arena_rank;
		}
	}
	uint32 player::get_off_line_time(void)
	{

		return 1;

	}

	void player::update_character_attribute()
	{
	}

	bool player::get_loading_flag_all_finish(void)
	{
		ZoneScoped;
		return m_loading_flag.empty();
	}

	bool player::get_saveing_flag_all_finish(void)
	{
		ZoneScoped;
		for (int32 i = 0; i < e_save_data_type_max; ++i)
		{
			if (m_saving_flag[i].empty() == false)
			{
				return false;
			}
		}
		return true;
	}

	void player::transfer_local_pos(const s_map_pos& map_pos, bool send_client)
	{
		ZoneScoped;
		m_teleport_control.teleport_to_local(map_pos, send_client);
	}
	void player::transfer_born()
	{
		ZoneScoped;
		auto base_map_cp = m_map_ent->get_component<base_map_component>();
		s_map_pos map_pos;
		map_pos.unit_location.x = base_map_cp->m_map_template->BornLocation[0];
		map_pos.unit_location.y = base_map_cp->m_map_template->BornLocation[1];
		map_pos.unit_location.z = base_map_cp->m_map_template->BornLocation[2];
		map_pos.unit_rotation.roll = base_map_cp->m_map_template->BornRotation[0];
		map_pos.unit_rotation.pitch = base_map_cp->m_map_template->BornRotation[1];
		map_pos.unit_rotation.yaw = base_map_cp->m_map_template->BornRotation[2];
		transfer_local_pos(map_pos);
	}

	bool player::is_in_cross_pk_map()
	{
		ZoneScoped;
		if (m_status != e_session_status_in_gaming)
		{
			return false;
		}
		auto map_type = base_map_system::get_map_type(get_map_ent());
		auto map_sub_type = base_map_system::get_map_sub_type(get_map_ent());
		return (map_type == e_map_type_cross_server_pk && map_sub_type != 4);
	}
	bool player::is_in_copy_map()
	{
		ZoneScoped;
		auto map_type = base_map_system::get_map_type(get_map_ent());
		return (map_type > e_map_type_main && map_type != e_map_type_big_map);
	}

	bool player::is_can_transfer_new_map()
	{
		ZoneScoped;
		int32 mission_id = m_mission_mgr.get_mission_id(e_mission_slot_main);
		if (mission_id < new_map_lock_id)
		{
			game_proto_send_transfer_new_map_error msg;
			send_message_to_self(&msg, e_mgsindex_s2c_transfer_new_map_error);
			return false;
		}
		return true;
	}

	bool player::is_lock_map_mission(int32 map_id)
	{
		ZoneScoped;
		MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, map_id);
		if (nullptr == map_ptr)
		{
			return false;
		}
		int32 lock_id = map_ptr->UnLockMissionID;
		if (lock_id <= 0)
		{
			return true;
		}
		int32 mission_id = m_mission_mgr.get_mission_id(e_mission_slot_main);
		if (lock_id >= mission_id)
		{
			game_proto_send_transfer_new_map_error msg;
			send_message_to_self(&msg, e_mgsindex_s2c_transfer_new_map_error);
			return false;
		}
		return true;
	}

	void player::get_pk_king_bron_pos(guid_64 map_guid, s_map_pos& temp_pos)
	{
		ZoneScoped;
		auto map_type = base_map_system::get_map_type(map_guid);
		if (map_type != e_map_type_king_of_pk)
		{
			return;
		}

		//temp_pos = map_ptr->get_born_pos(get_array_index());
	}

	void player::reset_client_camera()
	{
		ZoneScoped;
		game_proto_reset_camera msg;
		send_message_to_self(&msg, e_msgindex_s2c_reset_camera);
	}

	void player::transfer_by_template_pos(int32 map_template_id, int32 line_id, s_map_pos map_pos, guid_64 map_guid)
	{
		ZoneScoped;
		if (m_status != e_session_status_in_gaming)
		{
			return;
		}
		if (is_lock_map_mission(map_template_id) == false)
		{
			return;
		}
		//gate����ֹ���͵��ǿ����ͼ
		if (init_unit::is_tansfer_server(map_template_id, cell_server::getInstance().get_is_self_gate_server(), cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity)) == false && cell_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) && false == init_unit::is_map_all_server(map_template_id))
		{
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90305139);
			send_notice(notice_str);
			return;
		}

		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		character_proto_showed_message msg;
		if (map_template_ptr == nullptr)
		{
			msg.set_message_id(e_item_string_unkown);
			send_message_to_self(&msg, e_msgindex_s2c_show_message);
			return;
		}
		if (get_unit_info(e_role_info_exp_level) < map_template_ptr->EnterLevel)
		{
			msg.set_message_id(e_map_exp_level_cant);
			send_message_to_self(&msg, e_msgindex_s2c_show_message);
			return;
		}

		//if (map_template_ptr->Type == e_map_type_field && get_time_data(e_time_type_ancient_battelfield) <= 0)
		//{
		//	msg.set_message_id(e_map_ancient_map_no_time);
		//	send_message_to_self(&msg, e_msgindex_s2c_show_message);
		//	return;
		//}

		m_teleport_map_id = map_template_id;
		m_teleport_line_id = line_id;
		m_teleport_map_pos = map_pos;
		m_teleport_map_guid = map_guid;
		m_teleport_war_index = 0;
		m_teleport_group_id = 0;
	}
	void player::transfer_by_template(int32 map_template_id, int32 line_id, int32 group_id, const guid_64& map_guid, int32 war_index)
	{
		ZoneScoped;
		if (m_status != e_session_status_in_gaming)
		{
			return;
		}
		if (is_lock_map_mission(map_template_id) == false)
		{
			return;
		}
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		character_proto_showed_message msg;
		msg.add_role_guid(get_unit_guid().A);
		msg.add_role_guid(get_unit_guid().B);

		if (map_template_ptr == nullptr)
		{
			msg.set_message_id(e_item_string_unkown);
			send_message_to_self(&msg, e_msgindex_s2c_show_message);
			return;
		}

		if (get_unit_info(e_role_info_exp_level) < map_template_ptr->EnterLevel)
		{
			msg.set_message_id(e_map_exp_level_cant);
			send_message_to_self(&msg, e_msgindex_s2c_show_message);
			return;
		}

		if (is_dead())
		{
			set_player_relive(0.5, false);
		}

		m_teleport_map_id = map_template_id;
		m_teleport_line_id = line_id;
		m_teleport_group_id = group_id;
		m_teleport_map_guid = map_guid;
		m_teleport_war_index = war_index;

		if (get_must_do_count(e_daily_must_do_typ_demon_tower) < 1 && map_template_ptr->Type == e_map_type_demon_tower_fuben)
		{
			set_must_do_count(e_daily_must_do_typ_demon_tower, get_must_do_count(e_daily_must_do_typ_demon_tower) + 1);
			send_daily_must_do_count_info_one(e_daily_must_do_typ_demon_tower);
			set_daily_active_degree_info(e_daily_must_do_typ_demon_tower);
		}
	}

	void player::gm_order(const int32& command_type, const int32& sub_command, std::vector<int32> arg_int, std::vector<std::string> arg_string)
	{
		ZoneScoped;
		switch (command_type)
		{
		case egot_role_vip_point:
			if (arg_int.size() != 1)
			{
				return;
			}
			gm_set_vip_point(arg_int[0]);
			break;
		case egot_role_career:
			if (arg_int.size() != 1)
			{
				return;
			}
			gm_set_career(arg_int[0]);
			break;
		case egot_god:
			get_pawn_att().set_state_att(e_unit_attack_state_no_control, true);
			get_pawn_att().set_state_att(e_unit_attack_state_no_hurt, true);
			break;
		case egot_no_god:
			get_pawn_att().set_state_att(e_unit_attack_state_no_control, false);
			get_pawn_att().set_state_att(e_unit_attack_state_no_hurt, false);
			break;
		case egot_close_speed:
			get_pawn_att().set_close_speed(arg_int[0]);
			break;
		case egot_skill_level:
			if (arg_int.size() != 1)
			{
				return;
			}
			skill_manager::get_skill_set(get_array_index()).gm_set_all_career_skill_level(arg_int[0]);
			break;
		case egot_reset_cd:
			skill_manager::get_skill_set(get_array_index()).gm_reset_all_career_skill_cd();
			break;
		case egot_add_legion_coin://���Ӿ�����Դ
		{
			if (arg_int.size() != 2)
			{
				return;
			}
			guid_64 legion_guid = get_legion_guid();
			if (!legion_guid.is_valid())
			{
				return;
			}
			int32 add_coin_type = arg_int[0];
			int64 add_coin_num = arg_int[1];
			if (add_coin_num < 0)
			{
				add_coin_num = 0;
			}
			switch ((e_legion_gm_order)add_coin_type)
			{
			case e_legion_gm_order_money:
			{
				cs2ws_donate_legion_money legion_money_msg;
				legion_money_msg.client_uid = get_client_uid();
				legion_money_msg.money_num = add_coin_num;
				legion_money_msg.battle_achievement = 0;
				connection_mgr::getInstance().send_to_ws(&legion_money_msg, sizeof(legion_money_msg));
			}
			break;
			case e_legion_gm_order_achievement:
			{
				add_money_or_exp(e_money_type_battle_achievement, add_coin_num, e_server_log_add_money_gm_order);
				send_money_one(e_money_type_battle_achievement);
			}
			break;
			case e_legion_gm_order_emblem:
			{
				cs2ws_legion_add_glory_glow add_glory_glow_msg;
				add_glory_glow_msg.role_guid = get_unit_guid();
				add_glory_glow_msg.add_gg_value = add_coin_num;
				connection_mgr::getInstance().send_to_ws(&add_glory_glow_msg, sizeof(add_glory_glow_msg));
			}
			break;
			case e_legion_gm_order_item1:
			case e_legion_gm_order_item2:
			case e_legion_gm_order_item3:
			case e_legion_gm_order_item4:
			{
				cs2ws_donate_legion_item legion_item_msg;
				legion_item_msg.client_uid = get_client_uid();
				legion_item_msg.item_index = add_coin_type - 3;
				legion_item_msg.item_num = add_coin_num;
				legion_item_msg.money_num = 0;
				legion_item_msg.battle_achievement = 0;
				connection_mgr::getInstance().send_to_ws(&legion_item_msg, sizeof(legion_item_msg));
			}
			break;
			}
		}
		break;
		case egot_transport:
		{
			if (arg_int.size() != 4)
			{
				return;
			}
			s_map_pos temp_pos;
			temp_pos.set_location(arg_int[0], arg_int[1], arg_int[2]);
			//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(m_map_guid);
			auto map_object_ptr = world_cs::get_map_by_template_id<map_object>(arg_int[3]);
			if (map_object_ptr)
			{
				m_teleport_control.teleport_by_template_pos(map_object_ptr->get_map_template_id(), get_line_id(), temp_pos, guid_64());
			}
		}
		break;
		case egot_unlock_all_raid: // ����ȫ�����鸱��
		{
			//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(get_array_index());
			//if (map_record_set_ref.is_valid() == true)
			//{
			//	faith::template_manager::template_type* map_table_ptr = template_manager::get_instance().get_templates(e_MapTemplate);
			//	if (nullptr == map_table_ptr)
			//	{
			//		return;
			//	}
			//	faith::template_manager::template_type::iterator ite;
			//	MapTemplate* map_template_ptr = nullptr;
			//	for (ite = map_table_ptr->begin(); ite != map_table_ptr->end(); ++ite)
			//	{
			//		map_template_ptr = (MapTemplate*)(ite->second);
			//		if (nullptr == map_template_ptr)
			//		{
			//			continue;
			//		}
			//		int32 map_template_id = map_template_ptr->attribute_id;
			//		s_map_record_info record_info;
			//		record_info.data_ary[e_map_record_info_map_template_id] = map_template_id;
			//		record_info.data_ary[e_map_record_info_unlock] = 1;
			//		record_info.data_ary[e_map_record_info_enter_count] = 0;
			//		record_info.data_ary[e_map_record_info_fastest_time] = 5;
			//		record_info.data_ary[e_map_record_info_extra_enter_count] = 0;
			//		record_info.data_ary[e_map_record_info_score_record] = 0;
			//		map_record_set_ref.set_map_record(map_template_id, record_info);
			//	}
			//	map_record_set_ref.send_map_record_all();
			//}
			break;
		}
		case egot_dump_npc_info:
		{
			if (arg_int.size() != 1)
			{
				return;
			}
			unit_man::dump_npc_info_by_template_id(arg_int[0]);
			break;
		}
		case egot_dump_aoi_info:
		{
			const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(get_map_ent(), get_old_map_pos().unit_location);
			server_log::dump_location_watch(aoi_tower_watch);
			break;
		}
		case egot_reset_highest_record:
		{
			m_history_high_record.clear_data();
			break;
		}
		case egot_control_robot_chat:
		{
			int32 result = arg_int[0];
			if (result < 0 || result >1)
			{
				break;
			}
			m_is_receive_robot_chat = result;
			break;
		}
		case egot_set_self_hp_point:
		{
			get_pawn_att().set_unit_base_att(e_base_att_info_hp_cur, arg_int[0]);
			break;
		}
		case egot_clear_arena:
		{
			set_unit_info(e_role_info_arena_times, 10);
			set_unit_info(e_role_info_arena_vip_times, 0);
		}
		break;
		case egot_open_mobile_gm_web_require:
		{
			faith::character_proto_character_gmorder_message gm_msg;

			gm_msg.set_command_type(egot_open_mobile_gm_web_response);
			gm_msg.set_sub_command_type(0);
			gm_msg.add_arg_int(0);
			gm_msg.add_arg_string("");

			send_message_to_self(&gm_msg, e_msgindex_s2c_character_gmorder_message);
		}
		break;
		case egot_grade_up:
		{
			if (arg_int.size() != 2)
			{
				return;
			}
			gm_set_grade_level(arg_int[0], arg_int[1]);

		}
		break;
		}

	}


	int32 player::get_map_drop_box_notice_id(const int32& map_type, const int32& item_type, const int32& item_color, const int32& map_difficult)
	{
		ZoneScoped;
		int32 notice_id = 0;
		switch (map_type)
		{
		case e_map_type_battle:
		{
			switch (item_color)
			{
			case e_item_color_purple:
				if (e_item_type_weapon == item_type || e_item_type_armor == item_type)
				{
					notice_id = 93000148;
				}
				break;
			case e_item_color_orange:
				if (e_item_type_weapon == item_type || e_item_type_armor == item_type)
				{
					notice_id = 93000147;
				}
				break;
			default:
				break;
			}
		}
		break;
		case e_map_type_princess_guard_war:
		case e_map_type_lava_hellfire:
		case e_map_type_sea_of_hate:
		{
			switch (item_color)
			{
			case e_item_color_orange:
			{
				if (e_item_type_weapon == item_type || e_item_type_armor == item_type)
				{
					switch (map_difficult)
					{
					case 1:
						notice_id = 93000180;
						break;
					case 2:
						notice_id = 93000181;
						break;
					case 3:
						notice_id = 93000182;
						break;
					case 4:
						notice_id = 93000183;
						break;
					default:
						break;
					}
				}
			}
			break;
			case e_map_type_broken_sky:

				break;
			default:
				break;
			}
		}
		break;
		case e_map_type_broken_sky:
		{
			notice_id = 93000184;
		}
		break;
		default:
			break;
		}
		return notice_id;
	}

	void player::send_get_pick_special_drop_box_notice(const std::vector<s_item_info>& item_data)
	{
		ZoneScoped;
		auto map_template = base_map_system::get_map_template(get_map_ent());
		if (map_template == nullptr)
		{
			CONSOLE_ERROR("map_template is nullptr");
			return;
		}
		//��������Ӧ����ֻ��һ����Ʒ
		if (item_data.size() <= 0)
		{
			CONSOLE_ERROR("item_data is zero");
			return;
		}
		ItemTemplate* item_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(item_data[0].data_ary[e_item_info_info_id]);
		if (item_ptr == nullptr)
		{
			CONSOLE_ERROR("item_ptr is nullptr");
			return;
		}

		int32 notice_id = 0;
		notice_id = get_map_drop_box_notice_id(map_template->Type, item_ptr->item_type, item_ptr->item_color, map_template->Difficulty);
		if (notice_id <= 0)
		{
			//CONSOLE_ERROR("notice_id is zero");
			return;
		}
		s_chat_notice_info notice_data;
		notice_data.item_data = item_data;
		get_chat_mgr().send_notice_new(notice_id, notice_data);
	}


	void player::on_req_pick_drop_box(const s_unit_identifier& unit_identifier)
	{
		ZoneScoped;
		//�ҵ�npc
		item_proto_pick_drop_box msg;
		msg.add_role_guid(get_unit_guid().A);
		msg.add_role_guid(get_unit_guid().B);

		npc& npc_ref = unit_man::get_npc(unit_identifier);
		if (npc_ref.is_valid() == false)
		{
			return;
		}
		int32 item_template_id = npc_ref.get_item_id();
		int32 item_lock_state = npc_ref.get_item_lock();

		std::vector<s_item_template_info> item_array;
		item_array.push_back({ item_template_id, 1, item_lock_state });
		if (false == get_item_set().can_put_items(item_array))
		{
			msg.set_pick_state(-1);
			msg.set_item_template_id(0);
			msg.set_pick_fail_msg_id(90029513);
			send_message_to_self(&msg, e_msgindex_s2c_character_pick_drop_box);
			return;
		}

		e_dropbox_owner_type owner_type = npc_ref.get_owner_type();
		const guid_64& owner_guid = npc_ref.get_owner().unit_guid;
		const guid_64& me_guid = get_unit_guid();
		const int32 drop_temp_npc_id = npc_ref.get_drop_npc_temp_id();

		if (e_dropbox_owner_player == owner_type)
		{
			if (owner_guid != me_guid)
			{
				//�����Լ��ĵ����, ����ʰȡ;
				return;
			}
		}
		else //�˴��ж��Ƿ���ӹ���,������������,Ŀǰ�߼�û��ʵ��, todo
		{
		}

		unit_man::kill_unit(get_array_index(), unit_identifier.runtime_id);//��ˢ,��ɾnpc,�������
		//TODO: Ӧ��У��һ��,ȷ��npc�Ѿ���ɾ����

		//һ�������ֻ��һ����Ʒ
		citem* item_ptr = get_item_set().create_item_by_template(e_server_log_add_item_pick_up_drop_box, get_unit_info(e_role_info_move_map_id), item_template_id, 1, item_lock_state);
		if (nullptr != item_ptr)
		{
			guid_64 item_guid = item_ptr->get_item_guid();
			std::vector<s_item_info>  item_data;
			item_data.push_back(item_ptr->get_item_inst());
			get_item_set().put_in_bag(item_ptr);
			{
				msg.set_pick_state(1);
				msg.set_item_template_id(item_template_id);
				msg.set_item_guid(item_guid.server_64);
				send_message_to_self(&msg, e_msgindex_s2c_character_pick_drop_box);
				drop_item_send_notice(item_data, drop_temp_npc_id);
				send_get_pick_special_drop_box_notice(item_data);
				return;
			}
		}
		else
		{
			msg.set_pick_state(-1);
			msg.set_item_template_id(0);
			send_message_to_self(&msg, e_msgindex_s2c_character_pick_drop_box);
		}

	}

	void player::game_over()
	{
		ZoneScoped;
		faith::character_proto_s2c_game_over game_over_msg;
		game_over_msg.set_game_result(1);
		send_message_to_self(&game_over_msg, e_msgindex_s2c_game_over);
	}

	void player::fuben_game_over(s_fuben_settlement& settlement, int32 fuben_template_id, bool finished, float award_fac)
	{
		ZoneScoped;
		//���¸�������
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, fuben_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}

		if (init_unit::get_map_public(map_template_ptr->Type))
		{
			return;
		}

		int32 assist_state = get_team_cs_mgr().is_assist_in_raid();
		if (finished)
		{
			assist_state = give_assist_reward();
		}

		//�����ͻ��˸�����������
		faith::character_proto_map_fuben_over fuben_over_msg;
		fuben_over_msg.set_fuben_template_id(fuben_template_id);
		fuben_over_msg.set_pass_time(settlement.pass_time);
		fuben_over_msg.set_dead_count(settlement.dead_count);
		fuben_over_msg.set_settle_level(settlement.fuben_level);
		fuben_over_msg.set_assist_state(assist_state);

		if (finished && map_template_ptr->SettleLevelType.size() > 0)
		{
			buy_message_tip(e_buy_tip_type_fuben, settlement.fuben_level);
		}

		if (false == finished)
		{
			send_message_to_self(&fuben_over_msg, e_msgindex_s2c_map_fuben_over);
			return;
		}
		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(get_array_index());
		//if (false == map_record_set_ref.is_valid())
		//{
		//	return;
		//}
		int32 move_in_map_id = get_unit_info(e_role_info_move_map_id);
		//int32 enter_times = map_record_set_ref.get_enter_count(move_in_map_id);
		//int32 buy_times = map_record_set_ref.get_buy_count(move_in_map_id);
		//int32 add_count = map_record_set_ref.get_add_count(move_in_map_id);
		if (e_map_type_boss_single != map_template_ptr->Type && e_map_type_demon_tower_fuben != map_template_ptr->Type && e_consume_enter_count_cond_enter != map_template_ptr->ConsumeEnterCounterCond)// && get_team_cs_mgr().get_is_not_drop())
		{
			//	if ((map_template_ptr->EnterCount + buy_times + add_count) <= enter_times)//����������ڵ��ڿɽ������+�ѹ������
			//	{
			//		//û�д�����������
			//		return;
			//	}
		}
		if (e_map_type_boss_single != map_template_ptr->Type && e_consume_enter_count_cond_enter == map_template_ptr->ConsumeEnterCounterCond && get_team_cs_mgr().get_is_not_drop() && e_map_type_demon_tower_fuben != map_template_ptr->Type)
		{
			//if ((map_template_ptr->EnterCount + buy_times + add_count) < enter_times)//����������ڵ��ڿɽ������+�ѹ������
			//{
			//	//û�д�����������
			//	return;
			//}
		}

		// �Ѷ�Ӧ�����ľ���ͽ�Ǯ�������Ÿ����
		std::vector<int32>* award_money_arr = nullptr;
		if (e_map_type_demon_tower_fuben == map_template_ptr->Type)
		{
			award_money_arr = &(map_template_ptr->FirstWinAwardMoney);
		}
		else
		{
			award_money_arr = &(map_template_ptr->AwardMoney);
		}
		int32 arr_len = award_money_arr->size();
		if (arr_len > 0 && arr_len % 2 == 0)
		{
			int32 money_id = 0;
			int64 money_num = 0;

			int32 team_mem_num = get_team_info().member_num;
			float exp_extra_fix_team = 0.f;
			float exp_extra_fix_legion = 0.f;
			float raid_fix_team = 0.f;
			init_unit::get_team_share_exp_ratio(team_mem_num, 0, exp_extra_fix_team, exp_extra_fix_legion, raid_fix_team);
			float raid_fix_settle_level = 0.f;
			if (GAMECONFIG->RaidFinishSettlementRatio.size() > settlement.fuben_level)
			{
				raid_fix_settle_level = GAMECONFIG->RaidFinishSettlementRatio[settlement.fuben_level];
			}

			for (int32 i = 0; i < arr_len; i += 2)
			{
				money_id = (*award_money_arr)[i];
				money_num = (int32)((*award_money_arr)[i + 1] * award_fac);

				// �������Ҫ������ҵȼ�����߶������ͽ�������Ǯ��������ҵĵȼ�
				if (map_template_ptr->AwardMoneyFactor > 0)
				{
					int32 factor = 1;
					switch ((e_money_type)money_id)
					{
					case e_money_type_exp:
						//case e_money_type_silver:
					case e_money_type_silver_bind:
					{
						int32 upgrade_template_id = get_unit_info(e_role_info_upgrade_id);
						PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);
						if (nullptr != upgrade_template_ptr)
						{
							factor = e_money_type_exp == money_id ? upgrade_template_ptr->BaseExp : upgrade_template_ptr->BaseCoin;
							money_num *= factor;
						}
					}
					break;
					}
				}

				money_num = money_num * (1 + raid_fix_team + raid_fix_settle_level);

				fuben_over_msg.add_reward_moneys(money_id);
				fuben_over_msg.add_reward_moneys(money_num);

				add_money_or_exp((e_money_type)money_id, money_num, e_server_log_add_money_raid_over_reward, fuben_template_id);
			}
		}

		// �Ѷ�Ӧ�����Ĺ̶���Ʒ�������Ÿ����
		std::vector<int32>* award_item_arr = nullptr;
		std::vector<int32>* award_item_num_arr = nullptr;
		if (e_map_type_demon_tower_fuben == map_template_ptr->Type)
		{
			award_item_arr = &(map_template_ptr->FirstWinAwardItems);
			award_item_num_arr = &(map_template_ptr->FirstWinAwardItemsNum);
			m_mission_mgr.target_check(e_mission_end_type_clear_demon_tower);
		}
		else
		{
			award_item_arr = &(map_template_ptr->AwardItems);
			award_item_num_arr = &(map_template_ptr->AwardItemsNum);
		}
		if (award_item_arr->size() == award_item_num_arr->size())
		{
			int32 arr_len = award_item_arr->size();
			if (arr_len > 0)
			{
				item_set& temp_item_set = get_item_set();
				for (int32 i = 0; i < arr_len; ++i)
				{
					int32 award_template_id = (*award_item_arr)[i];
					int32 award_num = (int32)((*award_item_num_arr)[i] * award_fac);
					citem* award_item = temp_item_set.create_item_by_template(e_server_log_add_item_raid_reward, fuben_template_id, award_template_id, award_num, 1);
					if (nullptr == award_item)
					{
						return;
					}
					std::vector<citem*> item_list;
					item_list.push_back(award_item);
					if (temp_item_set.can_put_items(item_list) == false)
					{
						std::string title = "";
						title += template_manager::get_instance().get_str_by_string_template_id(map_template_ptr->MapName);
						title += template_manager::get_instance().get_str_by_string_template_id(90090511);
						get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), title, "", 0, 0, 0, 0, item_list);
					}
					else
					{
						temp_item_set.put_in_bag(item_list);
					}
				}
			}
		}

		//��������� ���ݶ�Ӧ���������ڹؿ�bossλ�ô��������
		if (map_template_ptr->SettleLevelDropIdArr.size() > settlement.fuben_level && map_template_ptr->Type != e_map_type_oracle_trial_map)
		{
			int32 drop_id = map_template_ptr->SettleLevelDropIdArr[settlement.fuben_level];
			int32 settle_boss_id = map_template_ptr->BossID;
			s_map_pos settle_boss_pos = s_map_pos();
			NpcTemplate* npc_temp_ptr = GET_TEMPLATE(NpcTemplate, settle_boss_id);
			if (nullptr != npc_temp_ptr && npc_temp_ptr->PathFindLocation.size() >= 3)
			{
				settle_boss_pos.unit_location.x = npc_temp_ptr->PathFindLocation[0];
				settle_boss_pos.unit_location.y = npc_temp_ptr->PathFindLocation[1];
				settle_boss_pos.unit_location.z = npc_temp_ptr->PathFindLocation[2];
			}
			else
			{
				settle_boss_pos = get_new_map_pos();
			}
			drop_manager::create_drop_box(drop_id, this, settle_boss_pos, true);
			if (map_template_ptr->FirstWinAwardDropId > 0)
			{
				//if (map_record_set_ref.is_have_first_arawd(move_in_map_id))
				//{
				//	map_record_set_ref.get_first_award(move_in_map_id);
				//	drop_manager::create_drop_box(map_template_ptr->FirstWinAwardDropId, *this, settle_boss_pos, true);
				//}
			}
		}

		send_message_to_self(&fuben_over_msg, e_msgindex_s2c_map_fuben_over);

		if (finished && map_template_ptr->Type == e_map_type_battle && map_template_ptr->ResultDialogType == e_map_result_dialog_type_arena)
		{
			faith::character_proto_break_map_over break_map_over_msg;
			send_message_to_self(&break_map_over_msg, e_msgindex_s2c_break_map_over);
		}
	}
	int32 player::get_assist_chest()
	{
		ZoneScoped;
		int32 cur_assist_count = get_unit_info(e_role_info_assist_chests);
		return (GAMECONFIG->MaxAssistChests - cur_assist_count);
	}
	int32 player::give_assist_reward()
	{
		ZoneScoped;
		bool is_assistant = get_team_cs_mgr().is_assist_in_raid();
		//bool is_not_drop = get_team_cs_mgr().get_is_not_drop();
		if (false == is_assistant)
		{
			return 0;
		}

		MapTemplate* map_cfg = base_map_system::get_map_template(get_map_ent());
		if (nullptr == map_cfg)
		{
			return 1;
		}

		//bool is_assist_other = map_object_ptr->have_be_assisted_mem(get_team_guid());
		//if (false == is_assist_other && map_cfg->Type != e_map_type_single_assist_boss)
		//{
		//	return 1;
		//}
		s_player_team_info& player_team_info = get_team_cs_mgr().get_unit_team_info();
		if (false == player_team_info.is_valid() && map_cfg->Type != e_map_type_single_assist_boss)
		{
			return 1;
		}

		if (player_team_info.captain_guid == get_unit_guid())
		{
			//�ӳ�û����ս�߱���
			return 1;
		}
		guid_64 my_guid = get_unit_guid();
		if (!get_team_cs_mgr().is_in_same_legion(my_guid, player_team_info.captain_guid) && map_cfg->Type != e_map_type_single_assist_boss)
		{
			return 1;
		}

		get_mission_mgr().target_check(e_mission_end_type_assis_fight_num);

		//if (get_team_cs_mgr().get_is_not_drop() == false)
		//{
		//	return 1;
		//}
		//if (player_team_info.member_num < 3)
		//{
		//	return 1;
		//}


		//int32 cur_assist_count = get_unit_info(e_role_info_assist_chests);
		//if (get_assist_chest() <= 0)
		//{
		//	return 3;
		//}
		if (check_can_get_helper_reward())
		{
			std::vector<int32> rwd_list = map_cfg->AssistAwardItems;
			int32 len = rwd_list.size();
			if (len > 0 && len % 2 == 0)
			{
				int32 money_type = e_money_type_assist_fighting;
				int32 money_num = get_help_value(rwd_list[1]);

				add_money_or_exp((e_money_type)money_type, money_num, e_server_log_add_money_assist_reward);
				//this->delete_get_be_helped_reward_times(money_num);
				delete_get_helper_reward_times(money_num);

				vector<s_item_template_info> promp_item_data;
				promp_item_data.push_back({ rwd_list[0] ,money_num , 1 });
				this->get_item_set().get_item_send_promp_msg_to_client(promp_item_data);

				return 2;
			}
		}
		return 1;
	}

	void player::send_gift_add_friend_values(guid_64& addreessee_guid, int32 gift_id, int32 gift_count, std::string chat_message)
	{
		ZoneScoped;
		if (addreessee_guid.is_valid() == false || gift_id <= 0 || gift_count <= 0)
		{
			return;
		}

		//������Ʒid��������ӵ��Ѻö�val��ȥ����
		ItemTemplate* gift_ptr = GET_TEMPLATE(ItemTemplate, gift_id);
		if (nullptr == gift_ptr)
		{
			return;
		}
		PropBasicAttributeTemplate* gift_prop_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, gift_ptr->logic_id);
		if (nullptr == gift_prop_ptr)
		{
			return;
		}
		if (2 != gift_prop_ptr->OtherRecvItemID.size())
		{
			return;
		}
		//������ĵ������Ƿ��㹻
		if (item_system::can_cost_item(this, e_bag_type_bag, gift_id, gift_count) == false)
		{
			return;
		}
		item_system::cost_item_from_bag(this, e_bag_type_bag, gift_id, gift_count);
		//cs2ws ����sender�Ѻö�
		cs2ws_add_friendliness_value cs_pak;
		cs_pak.sender_guid = get_unit_guid();
		cs_pak.addreessee_guid = addreessee_guid;
		cs_pak.gift_id = gift_id;
		cs_pak.gift_count = gift_count;
		connection_mgr::getInstance().send_to_ws(&cs_pak, sizeof(cs_pak));
		//��addrreessee��ҷ����ʼ�
		int32 receive_item_id = gift_prop_ptr->OtherRecvItemID[1];
		int32 total_gift_num = gift_prop_ptr->OtherRecvItemID[0] * gift_count;
		int32 add_friendliness_value = gift_count * gift_prop_ptr->FriendlinessAdd;
		std::string mail_title;
		std::string mail_context;
		ItemTemplate* receive_gift_ptr = GET_TEMPLATE(ItemTemplate, receive_item_id);
		if (nullptr == receive_gift_ptr)
		{
			return;
		}
		trie_filter::get_instance().find_forbidden(chat_message);
		append_gift_mail_system_string(mail_title, mail_context, get_name(), gift_ptr->ItemName, gift_count, add_friendliness_value, chat_message, receive_item_id);
		bool is_clear = false;
		s_item_info gift_item_array[faith::max_item_per_mail];
		int32 item_pos = 0;
		for (int32 create_count = 0; total_gift_num > 0;)
		{
			if (total_gift_num > receive_gift_ptr->max_pile_num)
			{
				create_count = receive_gift_ptr->max_pile_num;
				total_gift_num = total_gift_num - receive_gift_ptr->max_pile_num;

			}
			else
			{
				create_count = total_gift_num;
				total_gift_num = 0;
			}
			//�ȷ�һ��
			if (item_pos >= faith::max_item_per_mail)
			{
				get_mail_mgr().send_mail_to_another_player_by_system(addreessee_guid, 0, mail_title, mail_context, 0, 0, 0, 0, gift_item_array, item_pos);
				item_pos = 0;
				for (int32 index = 0; index < faith::max_item_per_mail; index++)
				{
					gift_item_array[index].clear_data();
				}
				is_clear = true;
			}
			bool OK = faith::init_unit::init_item_data(gift_item_array[item_pos++], receive_item_id, -1, create_count, e_bag_type_bag, 1);
			if (OK == false)
			{
				return;
			}
			is_clear = false;
		}
		if (!is_clear)
		{
			get_mail_mgr().send_mail_to_another_player_by_system(addreessee_guid, 0, mail_title, mail_context, 0, 0, 0, 0, gift_item_array, item_pos);
		}

	}


	void player::append_gift_mail_system_string(std::string& title, std::string& context, std::string sender_name, int32 gift_id, int32 gift_count, int32 add_friendliness, const std::string& chat_info, int32 incidental_gift_id)
	{
		ZoneScoped;
		const std::vector<std::string>& temp_array_list_title = template_manager::get_instance().get_string_list(90090684);
		const std::vector<std::string>& temp_array_list_content = template_manager::get_instance().get_string_list(90090685);
		const std::vector<std::string>& temp_array_list_tail = template_manager::get_instance().get_string_list(90090686);
		const std::vector<std::string>& temp_array_list_gift = template_manager::get_instance().get_string_list(gift_id);
		if (temp_array_list_title.size() < 1 || temp_array_list_content.size() < 5 || temp_array_list_gift.size() < 1)
		{
			return;
		}
		char buf[10];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", gift_count);
		std::string gift_count_str = buf;
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", add_friendliness);
		std::string friendliness_str = buf;

		title = temp_array_list_title[0];
		context = sender_name
			+ temp_array_list_content[0]
			+ temp_array_list_gift[0]
			+ temp_array_list_content[1]
			+ gift_count_str
			+ temp_array_list_content[2]
			+ friendliness_str
			+ temp_array_list_content[3];
		if (chat_info != "")
		{
			context += temp_array_list_content[4];
			context += chat_info;
		}
		if (incidental_gift_id > 0)
		{
			ItemTemplate* incidental_gift_ptr = GET_TEMPLATE(ItemTemplate, incidental_gift_id);
			if (nullptr == incidental_gift_ptr)
			{
				return;
			}
			const std::vector<std::string>& temp_array_list = template_manager::get_instance().get_string_list(incidental_gift_ptr->ItemName);
			if (temp_array_list.size() < 1 || temp_array_list_tail.size() < 2)
			{
				return;
			}
			context += temp_array_list_tail[0];
			context += temp_array_list[0];
			context += temp_array_list_tail[1];
		}
	}


	void player::get_first_kill_world_boss_prize_func(int32 first_kill_welfare_template_id)
	{
		ZoneScoped;
		//BOSS��ɱ��������ȡ����Ҫʱ��������
		//int32 SevenDays = 7;
		//int32 open_server_over_days = get_server_on_days();
		//if (open_server_over_days > SevenDays)
		//{
		//	return;
		//}

		WelfareTemplate* welfare_template_ptr = GET_TEMPLATE(WelfareTemplate, first_kill_welfare_template_id);
		if (nullptr == welfare_template_ptr)
		{
			return;
		}

		//������һ����itemid һ�����������ж��Ƿ���������
		if (welfare_template_ptr->RewardItemArray.size() % 2 != 0)
		{
			return;
		}

		std::vector<citem*> prize_item_vector;
		for (int32 index = 0; index < welfare_template_ptr->RewardItemArray.size(); index += 2)
		{
			ItemTemplate* prize_tmp = template_manager::get_instance().get_item_template_ptr_by_template_id(welfare_template_ptr->RewardItemArray[index]);
			if (nullptr == prize_tmp)
			{
				return;
			}
			if (prize_tmp->class_type.size() <= 0)
			{
				citem* prize_item_ptr = get_item_set().create_item_by_template(e_server_log_add_item_first_kill_boss_welfare, first_kill_welfare_template_id, welfare_template_ptr->RewardItemArray[index], welfare_template_ptr->RewardItemArray[index + 1]);
				if (nullptr == prize_item_ptr)
				{
					return;
				}
				prize_item_vector.push_back(prize_item_ptr);
			}
			else
			{
				for (int32 class_index = 0; class_index < prize_tmp->class_type.size(); class_index++)
				{
					if (m_role_info.data_ary[e_role_info_class_type] == prize_tmp->class_type[class_index])
					{
						citem* prize_item_ptr = get_item_set().create_item_by_template(e_server_log_add_item_first_kill_boss_welfare, first_kill_welfare_template_id, welfare_template_ptr->RewardItemArray[index], welfare_template_ptr->RewardItemArray[index + 1]);
						if (nullptr == prize_item_ptr)
						{
							return;
						}
						prize_item_vector.push_back(prize_item_ptr);
					}
				}
			}
		}

		cs2ws_has_get_first_kill_prize send_pak;
		std::string item_name_array_string = "";
		send_pak.first_kill_welfare_template_id = welfare_template_ptr->attribute_id;
		if (prize_item_vector.size() <= get_item_set().get_empty_slot_num(e_bag_type_bag))
		{
			std::vector<s_item_template_info> item_data;
			int32 reward_size = 0;
			for (int count = 0; count < prize_item_vector.size(); count++)
			{
				if (prize_item_vector[count] == nullptr)
				{
					continue;
				}

				ItemTemplate* item_template_ptr = prize_item_vector[count]->get_item_info_ptr();
				if (item_template_ptr == nullptr)
				{
					continue;
				}
				s_item_info temp_item_info = prize_item_vector[count]->get_item_inst();

				if (reward_size < max_world_boss_reward_num)
				{
					send_pak.reward_array[reward_size] = temp_item_info;
					reward_size++;
				}

				item_data.push_back({ temp_item_info.data_ary[e_item_info_info_id] ,temp_item_info.data_ary[e_item_info_stack_count],temp_item_info.data_ary[e_item_info_locked] });
			}
			send_pak.is_get_prize = e_first_kill_prize_type_was_get;
			get_item_set().get_item_send_promp_msg_to_client(item_data);
		}
		else
		{
			send_pak.is_get_prize = e_first_kill_prize_type_not_get;
		}
		get_item_set().put_in_bag(prize_item_vector);
		connection_mgr::getInstance().send_to_ws(&send_pak, sizeof(send_pak));
	}


	void player::lucky_draw(int32 fuben_template_id)
	{
		ZoneScoped;
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, fuben_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}
		std::vector<s_item_template_info> item_drop_list_with_arr;
		cdrop::gen_drop_list_by_drop_id(map_template_ptr->RandomAwardItems, item_drop_list_with_arr, get_unit_info(e_role_info_class_type), get_unit_info(e_role_info_exp_level));

		if (item_drop_list_with_arr.empty())
		{
			return;
		}

		std::vector<s_item_template_info> award_items_with_num_list;
		cdrop::merge_drop_bag(item_drop_list_with_arr, award_items_with_num_list);

		item_set& temp_item_set = get_item_set();
		int32 ret = 0;

		for (auto& award_item_with_num : award_items_with_num_list)
		{
			citem* award_item = temp_item_set.create_item_by_template(e_server_log_add_item_raid_reward, fuben_template_id, award_item_with_num.m_item_id, award_item_with_num.m_item_num, 1, 0, faith::e_bag_type_none, 0);
			if (nullptr == award_item)
			{
				return;
			}

			character_proto_lucky_draw_end lucky_draw_res;
			lucky_draw_res.set_item_template_id(award_item_with_num.m_item_id);
			lucky_draw_res.set_item_num(award_item_with_num.m_item_num);
			std::vector<citem*> item_list;
			item_list.push_back(award_item);
			if (temp_item_set.can_put_items(item_list))
			{
				temp_item_set.put_in_bag(item_list);
				lucky_draw_res.set_send_mail(false);
				send_message_to_self(&lucky_draw_res, e_msgindex_s2c_lucky_draw);
			}
			else
			{
				std::string title = "90090506";
				title += template_manager::get_instance().get_str_by_string_template_id(map_template_ptr->MapName);
				title += template_manager::get_instance().get_str_by_string_template_id(90090506);
				get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), title, "", 0, 0, 0, 0, item_list);
				lucky_draw_res.set_send_mail(true);
				send_message_to_self(&lucky_draw_res, e_msgindex_s2c_lucky_draw);
			}
		}
	}
	void player::recovery_for_item(const DropTemplate* drop_template_ptr, std::vector< int32 >& item_ids, std::vector<int32>& item_num, std::vector<int32>& item_array)
	{
		ZoneScoped;
		// ��Ϊ��
		if (drop_template_ptr == nullptr)
		{
			return;
		}
		//id����Ϊ0
		if (drop_template_ptr->attribute_id == 0)
		{
			return;
		}
		std::vector<s_item_template_info> item_drop_list_with_att;
		int32 drop_template_id = drop_template_ptr->attribute_id;
		cdrop::gen_drop_list_by_drop_id(drop_template_id, item_drop_list_with_att, get_unit_info(e_role_info_class_type), get_unit_info(e_role_info_exp_level));
		//Ϊ��˵��û�õ���Ʒ
		if (item_drop_list_with_att.empty())
		{
			return;
		}
		//0 ������� ���ܵ�������Ʒ��ÿ���������=Ȩ��/1000000 
		//1 ��һ���� ���ֻ�����һ����Ʒ
		if (drop_template_ptr->DropType == 0)
		{
			for (int32 i = 0; i < item_drop_list_with_att.size(); i++)
			{
				int32 j = 0;
				for (j = 0; j < item_ids.size(); j++)
				{
					if (item_drop_list_with_att[i].m_item_id == item_ids[j])
					{
						item_num[j]++;
						break;
					}
				}
				if (j == item_ids.size())
				{
					item_ids.push_back(item_drop_list_with_att[i].m_item_id);
					item_num.push_back(1);
				}
			}
		}
		else
		{
			item_ids.push_back(item_drop_list_with_att[0].m_item_id);
			item_num.push_back(item_drop_list_with_att.size());
		}

		int32 list_size = item_drop_list_with_att.size();

		for (int32 x = 0; x < list_size; x++)
		{
			int32 temp_id = item_drop_list_with_att[x].m_item_id;
			if (temp_id <= 0)
			{
				continue;
			}

			if (item_array.size() >= 2)
			{
				int32 y = 0;
				for (; y < item_array.size(); y += 2)
				{
					if (item_array[y] == temp_id)
					{
						item_array[y + 1]++;
						break;
					}
				}

				if (y == item_array.size())
				{
					item_array.push_back(temp_id);
					item_array.push_back(1);
				}

			}
			else
			{
				item_array.push_back(temp_id);
				item_array.push_back(1);
			}

		}

	}

	int32 player::get_highest_mount_level()
	{
		ZoneScoped;
		item_set& temp_item_set = get_item_set();
		return temp_item_set.get_highest_mount_level();
	}

	void player::fix_bag_slot_open_time_by_vip(int32 old_vip_level, int32 new_vip_level)
	{
		ZoneScoped;
		if (get_logic_data(e_role_logic_info_bag_open) >= MAX_ITEM_BAG_NUM)
		{
			return;
		}

		VIPTemplate* old_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(old_vip_level);
		VIPTemplate* new_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(new_vip_level);
		if (old_vip_template_ptr == nullptr
			|| new_vip_template_ptr == nullptr)
		{
			return;
		}
		float old_bag_open_speed = old_vip_template_ptr->BagSlotOpenRate;
		float new_bag_open_speed = new_vip_template_ptr->BagSlotOpenRate;
		if (FLOAT_IS_ZERO(old_bag_open_speed))
		{
			old_bag_open_speed = 1.0f;
		}
		if (FLOAT_IS_ZERO(new_bag_open_speed))
		{
			new_bag_open_speed = 1.0f;
		}
		int32 time_to_next_open = m_role_time.data_ary[e_time_type_bag_time];
		int32 new_time = time_to_next_open * old_bag_open_speed / new_bag_open_speed;
		m_role_time.data_ary[e_time_type_bag_time] = new_time;
	}

	void player::fix_storage_slot_open_time_by_vip(int32 old_vip_level, int32 new_vip_level)
	{
		ZoneScoped;
		if (get_logic_data(e_role_logic_info_storage_open) >= max_item_storage_num)
		{
			return;
		}

		VIPTemplate* old_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(old_vip_level);
		VIPTemplate* new_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(new_vip_level);
		if (old_vip_template_ptr == nullptr
			|| new_vip_template_ptr == nullptr)
		{
			return;
		}
		float old_bag_open_speed = old_vip_template_ptr->BagSlotOpenRate;
		float new_bag_open_speed = new_vip_template_ptr->BagSlotOpenRate;
		if (FLOAT_IS_ZERO(old_bag_open_speed))
		{
			old_bag_open_speed = 1.0f;
		}
		if (FLOAT_IS_ZERO(new_bag_open_speed))
		{
			new_bag_open_speed = 1.0f;
		}
		int32 time_to_next_open = m_role_time.data_ary[e_time_type_storage_time];
		int32 new_time = time_to_next_open * old_bag_open_speed / new_bag_open_speed;
		m_role_time.data_ary[e_time_type_storage_time] = new_time;
	}

	void player::send_fuben_info_to_client(s_fuben_info_to_client& info)
	{
		ZoneScoped;
		faith::character_proto_map_fuben_info_to_client msg;
		msg.set_wave_id(info.wave_id);
		msg.set_cur_npc(info.cur_npc);
		msg.set_cur_wave(info.cur_wave);
		msg.set_total_npc(info.total_npc);
		msg.set_total_wave(info.total_wave);
		msg.set_cur_map_state(info.cur_map_state);
		msg.set_cur_player_num(info.cur_player_num);
		msg.set_kill_monster_num(info.kill_monster_num);
		send_message_to_self(&msg, e_msgindex_s2c_fuben_info_to_client);
	}

	bool player::can_become_target()
	{
		ZoneScoped;
		return true;
	}

	void player::send_dead_notice_with_cross_server(int32 server_id, std::string killer_name)
	{
		ZoneScoped;
		std::string server_name = "";
		server_name = cell_server::getInstance().get_server_name(server_id);
		std::string cur_server_name = "";
		cur_server_name = cell_server::getInstance().get_server_name(get_unit_info(e_role_info_server_id));
		int32 notice_id = notice_id_cross_server_kill_player;
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(server_name);
		vec_notice_str.push_back(killer_name);
		vec_notice_str.push_back(cur_server_name);
		vec_notice_str.push_back(get_name());
		std::string notice_str = init_unit::implode(vec_notice_str);
		get_chat_mgr().send_notice(notice_id, notice_str);
	}

	int32 player::get_daily_exp_raid_time()
	{
		ZoneScoped;
		VIPTemplate* vip_temp_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level(false));
		if (nullptr == vip_temp_ptr)
		{
			return 0;
		}
		int32 cur_acyivity = get_unit_info(e_role_info_daily_activity_degree); //��ǰ��Ծֵ
		return cur_acyivity * vip_temp_ptr->LimitConvertExpTime;
	}

	int32 player::get_max_daily_exp_raid_time()
	{
		ZoneScoped;
		VIPTemplate* vip_temp_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level(false));
		if (nullptr == vip_temp_ptr)
		{
			return 0;
		}
		return vip_temp_ptr->DailyMustMax * vip_temp_ptr->LimitConvertExpTime;
	}

	void player::be_dead(bool is_auto_put_into_bag)
	{
		ZoneScoped;
		get_pawn_att().send_unit_dead(true);
		m_dead_stamp = utility::get_tick_count();
		get_war_state_mgr().clear_war_state();
		world_cs::player_dead(get_map_ent(), this);
		unit& temp_send_unit = unit_man::get_unit(m_kill_array_index);

		if (m_server_harry_mgr.is_have_item())
		{
			m_server_harry_mgr.clear_server_item_with_player_dead();
			change_harry_buff();
		}

		if (temp_send_unit.is_valid()
			&& temp_send_unit.get_unit_type() == e_unit_type_player
			&& temp_send_unit.get_unit_guid() != get_unit_guid())
		{
			//��������ɱ��,������pkֵ���������ɱ�ɾ͡������Ź����
			s_map_pos cur_sender_map_pos = temp_send_unit.get_old_map_pos();
			s_map_pos new_sender_map_pos = temp_send_unit.get_new_map_pos();
			if (temp_send_unit.get_pk_community_mgr().is_in_not_peace_and_kindevil()
				&& !temp_send_unit.get_pk_community_mgr().is_in_legal_pk_area())
			{
				//��ɱ�ѷ���Ӫ����pkֵ
				int32 sender_community_type = temp_send_unit.get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
				int32 target_community_type = get_pawn_att().get_unit_base_att(e_base_att_info_community_type);
				if (globle_data::get_instance().get_community_group_relation_by_type(sender_community_type, target_community_type) > 0)
				{
					if (temp_send_unit.get_unit_info(e_role_info_server_id) == cell_server::getInstance().get_server_id() && true == is_self_server())//�������PKֵ
					{
						temp_send_unit.get_pk_community_mgr().add_pk_value(KILL_PLAYER_PER_PK_VALUE);
					}
				}
			}
			bool is_end = m_kill_num_now >= GAMECONFIG->BeEndKillNeedNum;
			player& temp_send_player = unit_man::get_player(m_kill_array_index);
			if (temp_send_player.is_valid())
			{
				set_last_kill_me_player_guid(temp_send_player.get_unit_guid());
				temp_send_player.set_last_kill_player_guid(get_unit_guid());
				temp_send_player.send_kill_prompt(m_array_index, is_end);
				MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, get_unit_info(e_role_info_move_map_id));
				if (map_template_ptr && map_template_ptr->Type == faith::e_map_type_big_map && temp_send_unit.get_unit_guid() != get_unit_guid())
				{
					//����ɱ����
					int32 kill_player_num = temp_send_player.get_unit_info(faith::e_role_info_kill_player_num);
					temp_send_player.set_unit_info(faith::e_role_info_kill_player_num, ++kill_player_num);
					temp_send_player.send_info_one(faith::e_role_info_kill_player_num);
					//����ɱ�˳ƺ�
					temp_send_player.get_title_mgr().add_title_by_type_and_value(faith::e_title_type_kill_player_num, kill_player_num);
				}
				MapTemplate* send_player_map_ptr = temp_send_player.m_cur_map_template_ptr;
				if (nullptr == send_player_map_ptr)
				{
					return;
				}
				if ((false == is_self_server() || false == temp_send_player.is_self_server()) && send_player_map_ptr->Type != e_map_type_cross_server_pk
					&& send_player_map_ptr->Type != e_map_type_king_of_pk && send_player_map_ptr->Type != e_map_type_cross_ladder)
				{
					send_dead_notice_with_cross_server(temp_send_player.get_unit_info(e_role_info_server_id), temp_send_player.get_name());
				}
				if (send_player_map_ptr->Type == e_map_type_cross_server_harry)
				{
					temp_send_player.get_server_harry_mgr().kill_player_with_harry(m_array_index);
				}
			}
			auto map_template = base_map_system::get_map_template(get_map_ent());
			if (nullptr == map_template)
			{
				return;
			}
			//pk֮����ɱ���ӳ���
			if (true == init_unit::get_map_need_add_hate(map_template->Type) && temp_send_unit.get_unit_info(e_role_info_server_id) == get_unit_info(e_role_info_server_id))
			{
				//�������Ϊ���˵ķ�����Ϣ��ws
				guid_64 hate_guid = temp_send_unit.get_unit_guid();
				sync_data_to_ws(e_sync_cs2ws_data_dead_to_addhate, (int64)hate_guid.server_64);
				send_chief_dead_notice();
			}
		}

		m_continuity_kill_end_time_stamp = 0;
		m_kill_num_now = 0;

		kill_all_pet();

		m_attacker_index = m_kill_array_index;

		if (get_write_log())
		{
			int32 map_template_id = base_map_system::get_map_template_id(get_map_ent());
			server_log::player_dead_role_log(m_login_third_data, temp_send_unit.get_unit_info_inst(), m_role_info, get_new_map_pos().unit_location, temp_send_unit.get_pawn_att().get_unit_base_att(e_base_att_info_pk_mode), get_pawn_att().get_unit_base_att(e_base_att_info_pk_mode), map_template_id, temp_send_unit.get_unit_type());

		}
	}

	void player::send_kill_prompt(int32 kill_player_array_index, bool is_end)
	{
		ZoneScoped;
		unit& killed_player = unit_man::get_unit(kill_player_array_index);
		if (!killed_player.is_valid())
		{
			return;
		}
		if (m_continuity_kill_end_time_stamp < time_helper::get_cur_time_new().millisecond)
		{
			m_kill_num_now = 1;
		}
		else
		{
			m_kill_num_now++;
		}
		m_continuity_kill_end_time_stamp = init_unit::get_end_time(GAMECONFIG->ContinuityKillTIme);

		character_proto_kill_prompt_info msg;
		msg.set_kill_num_now(m_kill_num_now);
		msg.set_is_end(is_end);
		msg.set_kill_player_name(killed_player.get_name());
		msg.set_kill_player_class_type(killed_player.get_unit_info(e_role_info_class_type));
		send_message_to_self(&msg, e_msgindex_s2c_kill_prompt);
	}

	void player::send_chief_dead_notice()
	{
		ZoneScoped;
		//// ��������Ҹ����Ĺ���
// modify by wangsonghao : ����ɱ�������ĳ�����ŵľ��ų����߸����ų���ʱ����ϵͳƵ������һ������
		MapTemplate* map_template_ptr = base_map_system::get_map_template(get_map_ent());
		if (nullptr == map_template_ptr)
		{
			return;
		}
		if (e_map_type_legion_station == map_template_ptr->Type
			|| e_map_type_city_war == map_template_ptr->Type
			|| e_map_type_king_of_pk == map_template_ptr->Type
			|| e_map_type_arena == map_template_ptr->Type
			|| e_map_type_cross_ladder == map_template_ptr->Type
			|| e_map_type_attack_city == map_template_ptr->Type)
		{
			return;
		}
		if (get_legion_guid().is_valid() == false)
		{
			return;
		}
		int32 notice_id = 0;
		if (get_legion_info().job_title == e_legion_job_title_chief)
		{
			notice_id = 93000030;
		}
		else if (get_legion_info().job_title == e_legion_job_title_assistant_chief)
		{
			notice_id = 93000254;
		}
		if (notice_id <= 0)
		{
			return;
		}
		player& temp_send_player = unit_man::get_player(m_kill_array_index);
		std::string killer_name = std::string(temp_send_player.get_name());
		std::string my_name = std::string(get_name());
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
		vec_notice_str.push_back(killer_name);
		vec_notice_str.push_back(std::string(get_legion_info().legion_name));
		vec_notice_str.push_back(my_name);
		std::string notice_str = init_unit::implode(vec_notice_str);

		std::vector<int32> param_arr;
		param_arr.reserve(5);
		fvector player_pos = get_new_map_pos().unit_location;
		int32 line_id = get_line_id();

		param_arr.push_back(line_id);
		param_arr.push_back(map_template_ptr->attribute_id);
		param_arr.push_back(player_pos.x);
		param_arr.push_back(player_pos.y);
		param_arr.push_back(player_pos.z);

		world_cs::send_notice_with_param(get_unit_guid(), "", notice_str, param_arr, e_notice_param_go_to_location, e_chat_type_system);
		//////////////////////////////////////////////////////////////////////////
	}

	int32 player::get_npc_position()
	{
		ZoneScoped;
		for (int32 i = 0; i < npc_position_max; ++i)
		{
			if (false == m_npc_position[i])
			{
				m_npc_position[i] = true;
				return i;
			}
		}
		return -1;
	}
	void player::set_npc_position(int32 npc_pos)
	{
		ZoneScoped;
		if (npc_pos < 0 || npc_pos >= npc_position_max)
		{
			return;
		}
		m_npc_position[npc_pos] = false;
	}
	void player::send_grade_up_notice(int32 new_player_template_id)
	{
		ZoneScoped;
		PlayerInitTemplate* new_player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, new_player_template_id);
		if (new_player_template_ptr == nullptr)
		{
			return;
		}
		if (new_player_template_ptr->NoticeId > 0)
		{
			int32 notice_id = new_player_template_ptr->NoticeId;
			std::vector<std::string> vec_notice_str;
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(get_name());
			const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(new_player_template_ptr->CareerName);
			vec_notice_str.push_back(temp_string);
			xstring notice_str = init_unit::implode(vec_notice_str);
			get_chat_mgr().send_notice(notice_id, notice_str);
		}
	}
	int64 player::get_cur_level_max_talent_num()
	{
		ZoneScoped;
		int32 cur_level = get_unit_info(e_role_info_exp_level);
		int64 all_talent_num = 0;
		int32 begin_upgrade_id = player_upgrade_cfg_first_id;
		int32 end_upgrade_id = player_upgrade_cfg_first_id + cur_level;
		for (int32 i = begin_upgrade_id; i <= end_upgrade_id; i++)
		{
			PlayerUpgradeTemplate* TempPtr = GET_TEMPLATE(PlayerUpgradeTemplate, i);
			if (nullptr == TempPtr)
			{
				break;
			}
			all_talent_num += TempPtr->TalentNum;
		}
		return all_talent_num;
	}
	int64 player::get_cur_level_max_attribute_talent_num()
	{
		ZoneScoped;
		int64 all_attribute_talent = 0;

		int32 cur_player_template_id = get_unit_info(e_role_info_template_id);
		PlayerInitTemplate* cur_player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, cur_player_template_id);
		if (nullptr != cur_player_template_ptr)
		{
			if (cur_player_template_ptr->GradeUpTalent.size() >= 2
				&& cur_player_template_ptr->GradeUpTalent[0] == e_money_type_attribute_talent)
			{
				all_attribute_talent += cur_player_template_ptr->GradeUpTalent[1];
			}
			int32 cur_big_grade_size = cur_player_template_ptr->GradeBranch.size();
			int32 hood_index = get_logic_data(e_role_logic_info_god_hood_grade) % (cur_big_grade_size + 1);
			for (int32 i = 0; i < hood_index; ++i)
			{
				GradeGodHoodTemplate* grade_god_hood_template_ptr = GET_TEMPLATE(GradeGodHoodTemplate, cur_player_template_ptr->GradeBranch[i]);
				if (grade_god_hood_template_ptr)
				{
					if (grade_god_hood_template_ptr->GradeUpTalent.size() >= 2
						&& grade_god_hood_template_ptr->GradeUpTalent[0] == e_money_type_attribute_talent)
					{
						all_attribute_talent += grade_god_hood_template_ptr->GradeUpTalent[1];
					}
				}
			}
			for (int32 i = 1; i <= init_unit::get_max_grade_level(); ++i)
			{
				cur_player_template_id -= 1;
				PlayerInitTemplate* player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, cur_player_template_id);
				if (nullptr == player_template_ptr)
				{
					break;
				}
				if (player_template_ptr->GradeUpTalent.size() >= 2
					&& player_template_ptr->GradeUpTalent[0] == e_money_type_attribute_talent)
				{
					all_attribute_talent += player_template_ptr->GradeUpTalent[1];
				}
				for (int32 j = 0; j < player_template_ptr->GradeBranch.size(); ++j)
				{
					GradeGodHoodTemplate* grade_god_hood_template_ptr = GET_TEMPLATE(GradeGodHoodTemplate, player_template_ptr->GradeBranch[j]);
					if (grade_god_hood_template_ptr)
					{
						if (grade_god_hood_template_ptr->GradeUpTalent.size() >= 2
							&& grade_god_hood_template_ptr->GradeUpTalent[0] == e_money_type_attribute_talent)
						{
							all_attribute_talent += cur_player_template_ptr->GradeUpTalent[1];
						}
					}
				}
			}
		}
		return all_attribute_talent;
	}

	void player::add_att_by_load()
	{
		ZoneScoped;
		int32 cur_player_template_id = get_unit_info(e_role_info_template_id);
		PlayerInitTemplate* cur_player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, cur_player_template_id);
		if (nullptr == cur_player_template_ptr)
		{
			return;
		}
		get_pawn_att().apply_att_change_by_array(cur_player_template_ptr->GradeUpProperty, true);

		item_system::load_data_over(this);

		int32 cur_big_grade_size = cur_player_template_ptr->GradeBranch.size();
		int32 hood_index = get_logic_data(e_role_logic_info_god_hood_grade) % (cur_big_grade_size + 1);
		for (int32 i = 0; i < hood_index; ++i)
		{
			GradeGodHoodTemplate* grade_god_hood_template_ptr = GET_TEMPLATE(GradeGodHoodTemplate, cur_player_template_ptr->GradeBranch[i]);
			if (grade_god_hood_template_ptr)
			{
				get_pawn_att().apply_att_change_by_array(grade_god_hood_template_ptr->GradeUpProperty, true);
			}
		}
		for (int32 i = 1; i <= init_unit::get_max_grade_level(); ++i)
		{
			cur_player_template_id -= 1;
			PlayerInitTemplate* player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, cur_player_template_id);
			if (nullptr == player_template_ptr)
			{
				break;
			}
			get_pawn_att().apply_att_change_by_array(player_template_ptr->GradeUpProperty, true);
			for (int32 j = 0; j < player_template_ptr->GradeBranch.size(); ++j)
			{
				GradeGodHoodTemplate* grade_god_hood_template_ptr = GET_TEMPLATE(GradeGodHoodTemplate, player_template_ptr->GradeBranch[j]);
				if (grade_god_hood_template_ptr)
				{
					get_pawn_att().apply_att_change_by_array(grade_god_hood_template_ptr->GradeUpProperty, true);
				}
			}
		}
	}
	void player::grade_god_hood(int32 hood_index, int32 skill_template_id)
	{
		ZoneScoped;
		// ��ǰ��ת���Ľ�ɫģ��
		int32 cur_player_template_id = get_unit_info(e_role_info_template_id);
		PlayerInitTemplate* cur_player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, cur_player_template_id);
		if (nullptr == cur_player_template_ptr)
		{
			return;
		}

		if (hood_index > cur_player_template_ptr->GradeBranch.size() || hood_index <= 0)
		{
			return;
		}

		GradeGodHoodTemplate* grade_god_hood_template_ptr = GET_TEMPLATE(GradeGodHoodTemplate, cur_player_template_ptr->GradeBranch[hood_index - 1]);
		if (nullptr == grade_god_hood_template_ptr)
		{
			return;
		}

		//���ȼ�
		int32 cur_level = get_unit_info(e_role_info_exp_level);
		if (cur_level < grade_god_hood_template_ptr->LevelMax)
		{
			return;
		}

		//���תְ���ĵ���Ʒ
		int32 item_need_arr_len = grade_god_hood_template_ptr->GradeUpCostItem.size();
		if (item_need_arr_len > 0 && item_need_arr_len % 2 == 0)
		{
			int32 item_template_id = 0;
			int32 item_num = 0;

			for (int32 i = 0; i < item_need_arr_len; i += 2)
			{
				item_template_id = grade_god_hood_template_ptr->GradeUpCostItem[i];
				item_num = grade_god_hood_template_ptr->GradeUpCostItem[i + 1];
			}

			item_set& item_ref = get_item_set();
			int32 have_item_num = item_ref.get_item_count(e_bag_type_bag, item_template_id);
			if (have_item_num < item_num)
			{
				return;
			}

		}

		//תְ��Ҫ��ɵ�����

		int32 cur_grade_num = get_logic_data(e_role_logic_info_god_hood_grade);
		int32 temp_num = cur_grade_num / faith::player_grade_up_one_mission_num;
		if (temp_num == 0)
		{
			int32 mission_flag = 1 << (cur_grade_num + 1);
			if ((mission_flag & get_logic_data(e_role_logic_info_done_grade_mission)) == 0 && grade_god_hood_template_ptr->GradeUpMission != 0)
			{
				return;
			}
		}
		else
		{
			int32 mission_flag = 1 << ((cur_grade_num + 1) % faith::player_grade_up_one_mission_num);
			if ((mission_flag & get_logic_data((e_role_logic_info)(e_role_logic_info_done_grade_mission_1 + temp_num - 1))) == 0 && grade_god_hood_template_ptr->GradeUpMission != 0)
			{
				return;
			}
		}

		//���תְ���ĵ���Ʒ
		if (item_need_arr_len > 0 && item_need_arr_len % 2 == 0)
		{
			int32 item_template_id = 0;
			int32 item_num = 0;

			for (int32 i = 0; i < item_need_arr_len; i += 2)
			{
				item_template_id = grade_god_hood_template_ptr->GradeUpCostItem[i];
				item_num = grade_god_hood_template_ptr->GradeUpCostItem[i + 1];
			}

			//��Ʒ�Ƿ��㹻
			item_set& item_ref = get_item_set();
			int32 have_item_num = item_ref.get_item_count(e_bag_type_bag, item_template_id);
			if (have_item_num < item_num)
			{
				return;
			}
			item_ref.cost_item_by_id_with_lock_states(e_server_log_del_item_grade_up, 0, item_template_id, item_num, 1);
		}

		//�����
		//�����֧����
		if (skill_template_id != 0)
		{
			skill_set& temp_skill_set = skill_manager::get_skill_set(m_array_index);
			for (int32 i = 0; i < grade_god_hood_template_ptr->GradeUpSkills.size(); ++i)
			{
				if (skill_template_id == grade_god_hood_template_ptr->GradeUpSkills[i])
				{
					temp_skill_set.activate_skill(skill_template_id);
				}
			}
		}

		//��������
		get_pawn_att().apply_att_change_by_array(grade_god_hood_template_ptr->GradeUpProperty, true);

		set_logic_data(e_role_logic_info_god_hood_grade, get_logic_data(e_role_logic_info_god_hood_grade) + 1);

		send_logic_one(e_role_logic_info_god_hood_grade);

		refresh_service_goal(e_service_goal_type_god_hood_grade);

		//���������츳
		if (grade_god_hood_template_ptr->GradeUpTalent.size() >= 2)
		{
			e_money_type money_type = (e_money_type)grade_god_hood_template_ptr->GradeUpTalent[0];
			int64 money_num = grade_god_hood_template_ptr->GradeUpTalent[1];
			add_money(money_type, money_num, e_server_log_add_money_exp_level_up, grade_god_hood_template_ptr->attribute_id);
		}

		int32 old_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);
		int32 new_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);

		send_grade_up_response((int32)e_grade_up_result_success, old_equip_buff_id, new_equip_buff_id, skill_template_id);
		return;

	}

	bool player::change_role_class(int32 class_change)
	{
		ZoneScoped;
		if (class_change < e_branch_type_common || class_change >= e_branch_type_max)
		{
			return false;
		}

		if (get_unit_info(e_role_info_class_branch) == class_change || e_branch_type_common == class_change || get_unit_info(e_role_info_class_branch) == e_branch_type_common)
		{
			return false;
		}

		guid_64 need_item_guid = get_item_set().get_item_by_type(e_item_type_expendable, e_prop_sub_type_change_class_card);
		if (!need_item_guid.is_valid())
		{
			return false;
		}
		citem* item_ptr = get_item_set().get_item(need_item_guid);
		if (nullptr == item_ptr)
		{
			return false;
		}
		if (item_ptr->get_data_info(e_item_info_stack_count) < 1)
		{
			return false;
		}

		int32 cur_player_template_id = get_unit_info(e_role_info_template_id);

		PlayerInitTemplate* cur_player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, cur_player_template_id);
		if (nullptr == cur_player_template_ptr)
		{
			return false;
		}

		// 		int32 new_player_template_id = cur_player_template_id - /*(cur_player_template_ptr->Branch - class_change) **/ role_branch_id_diff;
		// 
		// 		PlayerInitTemplate* new_player_template_ptr = GET_TEMPLATE(PlayerInitTemplate, new_player_template_id);
		// 		if (nullptr == new_player_template_ptr)
		// 		{
		// 			return false;
		// 		}

		get_item_set().del_item(e_server_log_del_item_change_class, class_change, item_ptr, 1);

		//ȥ��װ����ʾ��Чbuff
		int32 old_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);
		get_item_set().equip_off_upgrade_show_buff();

		//		set_unit_info(e_role_info_template_id, new_player_template_id);
		set_unit_info(e_role_info_class_branch, class_change);
		broadcast_info_one(e_role_info_template_id);
		broadcast_info_one(e_role_info_class_branch);

		//�����츳
		m_talent_mgr.talent_reset();

		sync_data_to_ws(e_sync_cs2ws_data_player_template, cur_player_template_id);

		//����װ����Чbuff
		int32 new_equip_buff_id = get_item_set().get_equipment_upgrade_buff_id(false);
		send_grade_up_response((int32)e_grade_up_result_success, old_equip_buff_id, new_equip_buff_id);
		get_item_set().equip_on_upgrade_show_buff();
		return true;
	}

	int32 player::get_grade_num()
	{
		ZoneScoped;
		int32 player_template_id = get_unit_info(e_role_info_template_id);
		int32 class_type = get_unit_info(e_role_info_class_type);
		return unit_man::get_grade_num(player_template_id, class_type);
	}

	void player::check_can_done_grade_mission(const int32& mission_template_id)
	{
		ZoneScoped;
		int32 player_class = get_unit_info(e_role_info_class_type);
		if (player_class <= 0 || player_class > GAMECONFIG->InitRoleAry.size())
		{
			return;
		}

		int32 init_template_id = GAMECONFIG->InitRoleAry[player_class - 1];
		template_manager::template_type* player_init_cfgs = template_manager::get_instance().get_templates(e_PlayerInitTemplate);
		if (nullptr == player_init_cfgs)
		{
			return;
		}

		for (auto iter = player_init_cfgs->begin(); iter != player_init_cfgs->end(); ++iter)
		{
			PlayerInitTemplate* init_config_ptr = (PlayerInitTemplate*)(iter->second);
			if (nullptr == init_config_ptr)
			{
				continue;
			}

			if (init_config_ptr->attribute_id < init_template_id)
			{
				continue;
			}

			if (init_config_ptr->attribute_id > init_template_id + init_unit::get_max_grade_level())
			{
				continue;
			}

			if (init_config_ptr->Class == player_class)
			{

				int32 index = init_config_ptr->attribute_id - init_template_id;
				int32 miss_done_index = -1;
				if (init_config_ptr->GradeUpMission == mission_template_id)
				{
					miss_done_index = (index + 1) * faith::player_grade_up_stage_nums;
				}
				else
				{
					for (int32 i = 0; i < init_config_ptr->GradeBranch.size(); ++i)
					{
						GradeGodHoodTemplate* grade_god_hod_ptr = GET_TEMPLATE(GradeGodHoodTemplate, init_config_ptr->GradeBranch[i]);
						if (nullptr == grade_god_hod_ptr)
						{
							continue;
						}

						if (mission_template_id == grade_god_hod_ptr->GradeUpMission)
						{
							miss_done_index = index * faith::player_grade_up_stage_nums + i + 1;
							break;
						}
					}
				}

				if (miss_done_index == -1)
				{
					continue;
				}

				int32 temp_num = (miss_done_index - 1) / faith::player_grade_up_one_mission_num;
				int32 flag_index = e_role_logic_info_done_grade_mission;
				int32 miss_done_flag = 0;
				if (temp_num == 0)
				{
					miss_done_flag = 1 << miss_done_index;
				}
				else
				{
					miss_done_flag = 1 << (miss_done_index % faith::player_grade_up_one_mission_num);
					flag_index = e_role_logic_info_done_grade_mission_1 + temp_num - 1;
				}

				set_logic_data((e_role_logic_info)flag_index, get_logic_data((e_role_logic_info)flag_index) | miss_done_flag);
				send_logic_one((e_role_logic_info)flag_index);
				continue;
			}
		}
	}
	void player::buy_goods(int32 goods_id, int32 goods_num, int32 store_id, bool is_auto_buy, int32 back_string)
	{
		ZoneScoped;
		int32 end_state = 0;
		if (back_string != 0)
		{
			end_state = back_string;
		}
		else
		{
			end_state = m_store_helper.buy_goods(goods_id, goods_num, store_id, is_auto_buy);
		}
		faith::goods_proto_goods_operate_end msg;
		msg.set_goods_id(goods_id);
		msg.set_goods_num(goods_num);
		msg.set_store_id(store_id);
		msg.set_operation_end_state(end_state);
		msg.set_is_auto_buy(is_auto_buy);
		send_message_to_self(&msg, e_msgindex_s2c_goods_operate);
	}

	void player::set_auto_use_hp_pot(float hp_percent, bool is_auto_buy_pot)
	{
		ZoneScoped;
		m_is_auto_buy_pot = is_auto_buy_pot;
		m_hp_percent = hp_percent;
	}

	void player::auto_use_hp_pot()
	{//�Զ�ʹ��Ѫƿ��ʱ����
		//ZoneScoped;
		//if (get_session_state() != e_session_status_in_gaming)
		//{
		//	return;
		//}
		//float cur_hp = get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
		//float max_hp = get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
		//if (cur_hp / max_hp < m_hp_percent && cur_hp > 0)
		//{
		//	citem* item_ptr = get_item_set().get_can_auto_use_hp_item();
		//	if (nullptr == item_ptr)
		//	{
		//		return;
		//	}
		//	int32 use_num = 1;
		//	e_error_code check_result = get_item_set().item_use_check(*item_ptr, use_num);
		//	if (e_error_code_success != check_result)
		//	{
		//		return;
		//	}
		//	citem* end_item = nullptr;
		//	get_item_set().item_use(item_ptr->get_item_guid(), end_item);
		//}
	}

	void player::get_fuhuopos(s_map_pos& map_pos, int32 map_template_id, s_map_pos& cur_pos)
	{
		ZoneScoped;
		//����Ǵ����粢��δ��������,��ص�������
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (map_template_ptr == nullptr)
		{
			return;
		}
		if ((map_template_ptr->Type == faith::e_map_type_big_map) && (m_func_unlock_mgr.is_func_unlock("Fly") == false))
		{
			if (map_template_ptr->BornLocation.size() < 3 || map_template_ptr->BornRotation.size() < 3)
			{
				return;
			}
			map_pos.unit_location.x = map_template_ptr->BornLocation[0];
			map_pos.unit_location.y = map_template_ptr->BornLocation[1];
			map_pos.unit_location.z = map_template_ptr->BornLocation[2];
			map_pos.unit_rotation.roll = map_template_ptr->BornRotation[0];
			map_pos.unit_rotation.pitch = map_template_ptr->BornRotation[1];
			map_pos.unit_rotation.yaw = map_template_ptr->BornRotation[2];
			return;
		}


		//����,�������ĸ����
		map_pos.unit_location.x = 0;
		map_pos.unit_location.y = 0;
		map_pos.unit_location.z = 0;

		int32 len = map_template_ptr->RelivePoint.size();
		if (len == 0 || len % e_relivepoint_read_max != 0)
		{
			return;
		}
		int32 dis = -1;
		for (int32 i = 0; i < len; i += e_relivepoint_read_max)
		{
			int32 point_x = map_template_ptr->RelivePoint[e_relivepoint_read_x + i];
			int32 point_y = map_template_ptr->RelivePoint[e_relivepoint_read_y + i];
			int32 point_z = map_template_ptr->RelivePoint[e_relivepoint_read_z + i];
			fvector location(point_x, point_y, point_z);
			int32 temp_dis = (cur_pos.unit_location - location).length_2d();
			if (-1 == dis || temp_dis <= dis)
			{
				dis = temp_dis;
				map_pos.unit_location.x = point_x;
				map_pos.unit_location.y = point_y;
				map_pos.unit_location.z = point_z;
				map_pos.unit_rotation.roll = map_template_ptr->RelivePoint[e_relivepoint_read_roll + i];
				map_pos.unit_rotation.pitch = map_template_ptr->RelivePoint[e_relivepoint_read_pitch + i];
				map_pos.unit_rotation.yaw = map_template_ptr->RelivePoint[e_relivepoint_read_yaw + i];
			}
		}
	}

	void player::change_hp_and_del_fuhuo_buff(float hp_percent)
	{
		ZoneScoped;
		if (is_dead())
		{
			return;
		}
		double hp = get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max) * hp_percent;
		change_hp(-hp, get_array_index(), false, 0);
		buff_man::del_buff_by_template_id(get_array_index(), get_array_index(), GAMECONFIG->FuHuoBuffId);
	}

	void player::set_player_relive(float hp_percent, bool summon_pet)
	{
		ZoneScoped;
		get_pawn_att().set_state_att(e_unit_attack_state_dead, false);
		double hp = get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max) * hp_percent;
		change_hp(-hp, get_array_index(), false, 0);
		//buff_man::add_buff_inst(get_array_index(), get_array_index(), GAMECONFIG->FuHuoBuffId);

		m_dead_stamp = 0;
		if (summon_pet)
			summon_all_pet();
	}

	void player::back_to_life_with_map_end()
	{
		ZoneScoped;
		if (false == is_dead())
		{
			return;
		}

		set_player_relive(1, false);
		character_proto_character_fuhuo_s2c	msg;
		msg.set_mode(0);
		send_message_to_self(&msg, e_msgindex_s2c_character_fuhuo);
	}

	void player::back_to_life(e_revive_type revive_mode, bool is_free)
	{
		ZoneScoped;
		if (!is_dead())
		{
			character_proto_character_fuhuo_s2c	msg;
			msg.set_mode(revive_mode);
			send_message_to_self(&msg, e_msgindex_s2c_character_fuhuo);
			return;
		}
		set_player_relive(1, false);

		transfer_born();

		character_proto_character_fuhuo_s2c	msg;
		msg.set_mode(revive_mode);
		send_message_to_self(&msg, e_msgindex_s2c_character_fuhuo);
	}

	void player::on_player_revive_suc(e_revive_type revive_mode, int64 dead_duration)
	{
		ZoneScoped;

		if (e_revive_type_same_pos != revive_mode)
		{
			//ֻ����ԭ�ظ���
			return;
		}
		auto map_type = base_map_system::get_map_type(m_map_ent);
		if (init_unit::is_map_gain_treasure(map_type) && init_unit::is_map_boss_island(map_type))
		{
			//Ŀǰֻ�ڴ�����Ҳ�����㵺����
			return;
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(get_map_ent(), get_old_map_pos().unit_location);
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			int32 unit_index = it->first;
			npc& aoi_npc_ref = unit_man::get_npc(unit_index);
			if (aoi_npc_ref.is_valid() == false)
			{
				continue;
			}
			aoi_npc_ref.restore_cache_hate_val(get_array_index(), dead_duration);
		}
	}

	void player::set_bag_slot_open_num(int32 num)
	{
		ZoneScoped;
		if (num < 0)
		{
			return;
		}
		else if (num > MAX_ITEM_BAG_NUM)
		{
			return;
		}
		else
		{
			set_logic_data(e_role_logic_info_bag_open, num);
			if (get_write_log())
			{
				server_log::bag_total_num_log(m_login_third_data, m_role_info, num);

			}
		}
	}

	void player::set_storage_slot_open_num(int32 num)
	{
		ZoneScoped;
		if (num < 0)
		{
			return;
		}
		else if (num > max_item_storage_num)
		{
			return;
		}
		else
		{
			set_logic_data(e_role_logic_info_storage_open, num);
		}
	}

	bool player::unlock_bag_slot(int32 unlock_slot_index, int32 unlock_storage_slot_index)
	{
		ZoneScoped;
		if (unlock_slot_index == 0)
		{
			return 	unlock_storage_slot(unlock_slot_index, unlock_storage_slot_index);
		}
		if (unlock_slot_index < 0 || unlock_slot_index > MAX_ITEM_BAG_NUM)
		{
			return false;
		}
		// �Ѿ��򿪵���Ʒ���Ӳ����ٴ�
		if (unlock_slot_index + 1 <= get_logic_data(e_role_logic_info_bag_open))
		{
			return false;
		}

		VIPTemplate* cur_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
		if (cur_vip_template_ptr == nullptr)
		{
			return false;
		}
		float open_rate = cur_vip_template_ptr->BagSlotOpenRate;
		if (FLOAT_IS_ZERO(open_rate))
		{
			open_rate = 1.0f;
		}

		BagExtendTemplate* bag_extend_template_ptr = nullptr;

		// �������㿪��һ����Ʒ������Ҫ�ĸ��ֻ��ҵ��ܺͣ��Ա�������ʱ�������
		int32 money_need[e_money_type_max] = { 0 };
		int32 bag_extend_template_id = 0;
		for (int32 slot_index = get_logic_data(e_role_logic_info_bag_open); slot_index <= unlock_slot_index; ++slot_index)
		{
			bag_extend_template_id = slot_index + 1 + bag_extend_template_first_id;
			bag_extend_template_ptr = GET_TEMPLATE(BagExtendTemplate, bag_extend_template_id);
			if (nullptr == bag_extend_template_ptr
				|| bag_extend_template_ptr->MoneyNeed.size() < 2)
			{
				return false;
			}

			// 2016-12-23: ��������,������ǰ���ӵļ۸����ݸ��ӽ���ʱ������Ŷ���������
			// ��ǰ���ڿ����ĸ���ʵʱ���� = �����ø������軨��*(ʣ��ʱ��/��������ʱ��)
			// ����ֵ����ȡ��int,�������õ�����Ϊ9.11,��ȡ���󻨷�Ϊ10
			//
			// ���������Ҫ��������ע�͵����if��֧����
			if (slot_index == get_logic_data(e_role_logic_info_bag_open))
			{
				double left_time = (double)get_time_data(e_time_type_bag_time);

				double need_time = (double)((bag_extend_template_ptr->Time / open_rate) * 1000);
				if (need_time != 0)
				{
					double discount = left_time / need_time;
					double _money_need = (double)bag_extend_template_ptr->MoneyNeed[1] * discount;
					int32 ret = (int32)(_money_need + 0.5f); // ����ȡ��
					money_need[bag_extend_template_ptr->MoneyNeed[0]] += ret;
				}
				continue;
			}

			money_need[bag_extend_template_ptr->MoneyNeed[0]] += bag_extend_template_ptr->MoneyNeed[1];
		}


		// �ж���һ����Ƿ��㹻
		for (int32 i = e_money_type_treasure_pic; i < e_money_type_max; ++i)
		{
			if (money_need[i] != 0)
			{
				if (!can_cut_money((e_money_type)i, money_need[i]))
				{
					return false; // ���Ҳ���ᵼ�¿�����ֹ
				}
			}
		}
		// �۵��������Ӧ��ҽ�ɫ��Ǯ��
		for (int32 i = e_money_type_treasure_pic; i < e_money_type_max; ++i)
		{
			if (money_need[i] != 0)
			{
				cut_money((e_money_type)i, money_need[i], e_server_log_cut_money_unlock_bag_slot);
			}
		}
		// �����µ��Ѿ������ĸ��ӵ���Ŀ
		set_bag_slot_open_num(unlock_slot_index + 1);
		if (unlock_slot_index + 2 <= MAX_ITEM_BAG_NUM)
		{
			bag_extend_template_ptr = GET_TEMPLATE(BagExtendTemplate, unlock_slot_index + 2 + bag_extend_template_first_id);
			if (nullptr == bag_extend_template_ptr)
			{
				return false;
			}
			m_role_time.data_ary[e_time_type_bag_time] = bag_extend_template_ptr->Time * 1000;
			fix_bag_slot_open_time_by_vip(0, get_vip_level());
		}
		send_open_bag_info();
		return true;
	}

	bool player::unlock_storage_slot(int32 unlock_slot_index, int32 unlock_storage_slot_index)
	{
		ZoneScoped;
		if (unlock_storage_slot_index < 0 || unlock_storage_slot_index > max_item_storage_num)
		{
			return false;
		}
		// �Ѿ��򿪵���Ʒ���Ӳ����ٴ�
		if (unlock_storage_slot_index + 1 <= get_logic_data(e_role_logic_info_storage_open))
		{
			return false;
		}

		VIPTemplate* cur_vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
		if (cur_vip_template_ptr == nullptr)
		{
			return false;
		}
		float open_rate = cur_vip_template_ptr->BagSlotOpenRate;
		if (FLOAT_IS_ZERO(open_rate))
		{
			open_rate = 1.0f;
		}

		BagExtendTemplate* bag_extend_template_ptr = nullptr;

		// �������㿪��һ����Ʒ������Ҫ�ĸ��ֻ��ҵ��ܺͣ��Ա�������ʱ�������
		int32 money_need[e_money_type_max] = { 0 };
		int32 bag_extend_template_id = 0;
		for (int32 slot_index = get_logic_data(e_role_logic_info_storage_open); slot_index <= unlock_storage_slot_index; ++slot_index)
		{
			bag_extend_template_id = slot_index + 1 + template_storage_id;
			bag_extend_template_ptr = GET_TEMPLATE(BagExtendTemplate, bag_extend_template_id);
			if (nullptr == bag_extend_template_ptr
				|| bag_extend_template_ptr->MoneyNeed.size() < 2)
			{
				return false;
			}

			// 2016-12-23: ��������,������ǰ���ӵļ۸����ݸ��ӽ���ʱ������Ŷ���������
			// ��ǰ���ڿ����ĸ���ʵʱ���� = �����ø������軨��*(ʣ��ʱ��/��������ʱ��)
			// ����ֵ����ȡ��int,�������õ�����Ϊ9.11,��ȡ���󻨷�Ϊ10
			//
			// ���������Ҫ��������ע�͵����if��֧����
			if (slot_index == get_logic_data(e_role_logic_info_storage_open))
			{
				double left_time = (double)get_time_data(e_time_type_storage_time);

				double need_time = (double)((bag_extend_template_ptr->Time / open_rate) * 1000);
				if (need_time != 0)
				{
					double discount = left_time / need_time;
					double _money_need = (double)bag_extend_template_ptr->MoneyNeed[1] * discount;
					int32 ret = (int32)(_money_need + 0.5f); // ����ȡ��
					money_need[bag_extend_template_ptr->MoneyNeed[0]] += ret;
				}
				continue;
			}

			money_need[bag_extend_template_ptr->MoneyNeed[0]] += bag_extend_template_ptr->MoneyNeed[1];
		}
		// �۵��������Ӧ��ҽ�ɫ��Ǯ��
		for (int32 i = e_money_type_treasure_pic; i < e_money_type_max; ++i)
		{
			if (money_need[i] != 0)
			{
				if (!can_cut_money((e_money_type)i, money_need[i]))
				{
					return false; // ���Ҳ���ᵼ�¿�����ֹ
				}
			}
		}
		// �۵��������Ӧ��ҽ�ɫ��Ǯ��
		for (int32 i = e_money_type_treasure_pic; i < e_money_type_max; ++i)
		{
			if (money_need[i] != 0)
			{
				cut_money((e_money_type)i, money_need[i], e_server_log_cut_money_unlock_storage_slot);
			}
		}
		// �����µ��Ѿ������ĸ��ӵ���Ŀ
		set_storage_slot_open_num(unlock_storage_slot_index + 1);
		if (unlock_storage_slot_index + 2 <= max_item_storage_num)
		{
			bag_extend_template_ptr = GET_TEMPLATE(BagExtendTemplate, unlock_storage_slot_index + 2 + template_storage_id);
			if (nullptr == bag_extend_template_ptr)
			{
				return false;
			}
			m_role_time.data_ary[e_time_type_storage_time] = bag_extend_template_ptr->Time * 1000;
			fix_storage_slot_open_time_by_vip(0, get_vip_level());
		}

		send_open_bag_info();
		return true;
	}

	void player::send_grade_up_response(int32 result_code, int32 old_equip_buff_id, int32 new_equip_buff_id, int32 skill_template_id)
	{
		ZoneScoped;
		character_proto_character_grade_up_end grade_up_response;

		guid_64 player_guid = get_unit_guid();
		grade_up_response.set_role_guid(player_guid.server_64);
		grade_up_response.set_result_code(result_code);
		grade_up_response.set_old_equip_buff_id(old_equip_buff_id);
		grade_up_response.set_new_equip_buff_id(new_equip_buff_id);
		grade_up_response.set_skill_template_id(skill_template_id);


		send_message_to_aoi(&grade_up_response, e_msgindex_s2c_character_grade_up);
	}

	void player::send_character_error(int32 error)
	{
		ZoneScoped;
		character_proto_character_error character_err_msg;

		character_err_msg.add_role_guid(get_unit_guid().A);
		character_err_msg.add_role_guid(get_unit_guid().B);
		character_err_msg.set_error(error);

		send_message_to_self(&character_err_msg, e_msgindex_s2c_character_error);
	}

	void player::update_team_member_pos_info_tick()	// ˢ�¶�Աλ����Ϣ
	{
		ZoneScoped;
		if (!is_player_in_team())
		{
			return;
		}
		cs2ws_update_team_member_pos_info_one update_team_member_pos_info_one_msg;
		update_team_member_pos_info_one_msg.team_guid = get_team_guid();
		update_team_member_pos_info_one_msg.role_guid = get_unit_guid();
		update_team_member_pos_info_one_msg.pos_x = get_unit_info(faith::e_role_info_move_pos_x);
		update_team_member_pos_info_one_msg.pos_y = get_unit_info(faith::e_role_info_move_pos_y);
		update_team_member_pos_info_one_msg.pos_z = get_unit_info(faith::e_role_info_move_pos_z);
		connection_mgr::getInstance().send_to_ws(&update_team_member_pos_info_one_msg, sizeof(update_team_member_pos_info_one_msg));
	}

	bool player::is_player_in_the_team(guid_64 team_guid)
	{
		ZoneScoped;
		if (!team_guid.is_valid())
		{
			return false;
		}

		return get_team_info().team_guid == team_guid;
	}
	bool player::is_player_in_the_same_team(player* m_player)
	{
		ZoneScoped;
		return get_team_info().team_guid == m_player->get_team_guid();
	}

	void player::get_func_unlock_award(int32 func_unlock_id)
	{
		ZoneScoped;
		int32 cur_preview_func_unlock_id = get_unit_info(e_role_info_get_func_unlock_award_record);

		int32 cur_need_mission_id = 0;
		int32 next_need_mission_id = 0;
		FuncUnlockTemplate* func_unlock_tempalte_ptr = GET_TEMPLATE(FuncUnlockTemplate, cur_preview_func_unlock_id);
		if (nullptr != func_unlock_tempalte_ptr)
		{
			cur_need_mission_id = func_unlock_tempalte_ptr->UnlockNeedMissionID;
		}
		FuncUnlockTemplate* unlock_func_unlock_tempalte_ptr = GET_TEMPLATE(FuncUnlockTemplate, func_unlock_id);
		if (nullptr == unlock_func_unlock_tempalte_ptr)
		{
			return;
		}
		if (m_func_unlock_mgr.is_func_unlock(unlock_func_unlock_tempalte_ptr->FuncName) == false)
		{
			return;
		}
		next_need_mission_id = unlock_func_unlock_tempalte_ptr->UnlockNeedMissionID;

		if (next_need_mission_id <= cur_need_mission_id)
		{
			return;
		}

		int32 len = unlock_func_unlock_tempalte_ptr->ItemAwardArr.size();
		if (len <= 0)
		{
			return;
		}
		if (len % e_item_tuple_max != 0)
		{
			return;
		}
		//��ý���
		int32 locked_stated = 1;
		if (true == add_item_by_template_tuple(e_server_log_add_item_unlock_award, len, unlock_func_unlock_tempalte_ptr->ItemAwardArr, locked_stated, true, 1.0, true, false))
		{
			//�����¹��ܻ�ý�Ʒ�����ʾ
			int32 cur_player_career = get_unit_info(e_role_info_class_type);
			std::vector<s_item_template_info> item_data;
			for (int32 item_id = 0; item_id < unlock_func_unlock_tempalte_ptr->ItemAwardArr.size(); item_id += e_item_tuple_max)
			{
				ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, unlock_func_unlock_tempalte_ptr->ItemAwardArr[item_id]);
				if (nullptr == item_template_ptr)
				{
					continue;
				}
				if (init_unit::can_career_use_item(cur_player_career, item_template_ptr->class_type) == false)
				{
					continue;
				}
				item_data.push_back({ unlock_func_unlock_tempalte_ptr->ItemAwardArr[item_id], unlock_func_unlock_tempalte_ptr->ItemAwardArr[item_id + e_item_tuple_num] , 1 });
			}
			get_item_set().get_item_send_promp_msg_to_client(item_data);
		}
		set_unit_info(e_role_info_get_func_unlock_award_record, func_unlock_id);
		character_proto_get_func_unlock_award_end get_func_unlock_award_end_msg;
		get_func_unlock_award_end_msg.set_func_unlock_template_id(func_unlock_id);
		send_message_to_self(&get_func_unlock_award_end_msg, e_msgindex_s2c_get_func_unlock_award_end);
	}
	int32 player::get_random_num(int32 random_index)
	{
		ZoneScoped;
		if (random_index < 0 || random_index >= random_seed_num)
		{
			return -1;
		}
		return m_random_array[random_index];
	}
	void player::set_random_num(const int64& new_time)
	{
		ZoneScoped;
		if (new_time < m_random_array_time)
		{
			return;
		}
		m_random_array_time = new_time + thrity_second_tick_time;
		game_proto_random_array random_array;
		for (int32 i = 0; i < random_seed_num; ++i)
		{
			int32 random_num = random_gen::get_random(10000, 1000000);
			random_array.add_random_num(random_num);
			m_random_array[i] = random_num;
		}
		send_message_to_self(&random_array, e_msgindex_s2c_random_array);
	}
	//////////////////////////////////////////////////////////////////////////
	// ������
	//////////////////////////////////////////////////////////////////////////
	void player::set_last_auction_time(int64 time)
	{
		ZoneScoped;
		m_last_auction_time = time;
	}
	int64 player::get_last_auction_time()
	{
		ZoneScoped;
		return m_last_auction_time;
	}

	void player::load_yesterday_must_do_remain_end(const s_unit_yesterday_must_do_remain* data_info)
	{
		ZoneScoped;
		memcpy(m_three_day_must_do_remain_ary, data_info, sizeof(s_unit_yesterday_must_do_remain) * resource_find_back_day);
		int32 cur_time = time_helper::get_cur_time_new().second;
		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			int32 interval_time = cur_time - m_three_day_must_do_remain_ary[i].save_date;
			if (interval_time / 3600 > 24 * 3)//���ʱ�䳬������
			{
				m_three_day_must_do_remain_ary[i].clear_data();
			}
		}

		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			if (m_three_day_must_do_remain_ary[i].distance_today_day_count != i + 1)
			{
				int32 temp = m_three_day_must_do_remain_ary[i].distance_today_day_count - 1;
				temp = temp < 0 ? 0 : temp;
				swap(m_three_day_must_do_remain_ary[i], m_three_day_must_do_remain_ary[temp]);
			}
		}
		update_three_day_must_do_remain();
		set_loading_flag(e_data_flag_yesterday_must_do_remain);
	}

	bool player::load_yesterday_must_do_remain_by_db_lua(const char* data_ptr, int32 data_len)
	{
		ZoneScoped;
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_yesterday_must_do_remain_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar* p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_unit_yesterday_must_do_remain));
		if (p_data == nullptr)
		{
			return false;
		}
		s_unit_yesterday_must_do_remain* p_row = (s_unit_yesterday_must_do_remain*)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::cs2dp_proto::role_yesterday_must_do_remain_row db_row = msg.row_data(i);
			p_row->role_guid.server_64 = db_row.role_guid();
			p_row->role_exp_level_yesterday = db_row.role_exp_level_yesterday();
			p_row->save_date = db_row.save_date();
			p_row->role_vip_level_yesterday = db_row.role_vip_level_yesterday();
			p_row->distance_today_day_count = db_row.distance_today_day_count();
			p_row->role_arena_rank = db_row.role_arena_rank();
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			p_row++;
		}
		load_yesterday_must_do_remain_end((const s_unit_yesterday_must_do_remain*)p_data);
		return true;


	}

	void player::find_back_must_do_resource_process(int32 must_do_typ, bool is_diamond_find)
	{
		ZoneScoped;
		character_proto_find_back_must_do_resource_end msg;
		msg.set_must_do_typ(must_do_typ);
		msg.set_is_diamond_find(is_diamond_find);

		float find_back_coefficient = 0;
		if (is_diamond_find)
		{
			find_back_coefficient = GAMECONFIG->DiamondFindBackPercentage / 100.0f;
		}
		else
		{
			find_back_coefficient = GAMECONFIG->SiliverFindBackPercentage / 100.0f;
		}

		int32 yesterday_exp_level = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_exp_level_yesterday;
		int32 yesterday_vip_level = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_vip_level_yesterday;
		int32 yesterday_arena_level = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_arena_rank;
		int32 upgrade_template_id = get_unit_info(e_role_info_upgrade_id);

		PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);
		if (upgrade_template_ptr == nullptr)
		{
			msg.set_operate_stat(e_item_string_unkown);
			send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
			return;
		}
		if (upgrade_template_ptr->Level != yesterday_exp_level)
		{
			//�������еĵȼ���֮ǰ���ǰһ��ĵȼ���ƥ�䣬Ҫ�����ҵ���ȷ����һ��
			upgrade_template_id -= upgrade_template_ptr->Level - yesterday_exp_level;
		}
		upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);

		std::vector<int32> item_list;
		item_list.reserve(20);
		std::vector<int64> money_list;
		money_list.reserve(50);
		int32 find_back_cost = 0;
		int32 find_back_cost_typ = -1;
		int32 find_back_num = 0;
		if (is_diamond_find)
		{
			find_back_cost_typ = e_money_type_jewel_bind;
		}
		else
		{
			find_back_cost_typ = e_money_type_silver_bind;
		}

		//��α�,����Ǯ��
		for (int32 i = 0; i < e_daily_must_do_typ_max; ++i)
		{
			if (must_do_typ != -1)
			{
				if (must_do_typ != i)
				{
					continue;
				}
			}
			int32 remain_num = 0;
			for (int32 day = 0; day < resource_find_back_day; day++)
			{
				remain_num += m_three_day_must_do_remain_ary[day].data_ary[i];
			}


			DailyActivitiesTemplate* template_ptr = get_find_back_template_ptr(i, m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_exp_level_yesterday);
			if (template_ptr == nullptr)
			{
				continue;
			}
			if (0 == template_ptr->IsCanFindBack)
			{
				continue;
			}
			bool is_unlock = m_func_unlock_mgr.is_func_unlock(template_ptr->FuncUnlockName);
			if (!is_unlock)
			{
				continue;
			}
			int32 this_time_money = 0;
			if (is_diamond_find)
			{
				this_time_money = template_ptr->DiamondBackCost * remain_num;
			}
			else
			{
				this_time_money = template_ptr->SiliverBackCost * remain_num;
			}
			find_back_cost += this_time_money;
			find_back_num = remain_num;
		}
		if (!can_cut_money(e_money_type(find_back_cost_typ), find_back_cost))
		{
			if (is_diamond_find)
			{
				msg.set_operate_stat(e_item_string_jewel);
			}
			else
			{
				msg.set_operate_stat(e_item_string_money);
			}
			send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
			return;
		}

		for (int32 i = 0; i < e_daily_must_do_typ_max; ++i)
		{
			if (must_do_typ != -1)
			{
				if (must_do_typ != i)
				{
					continue;
				}
			}
			int32 remain_num = 0;
			for (int32 day = 0; day < resource_find_back_day; day++)
			{
				remain_num += m_three_day_must_do_remain_ary[day].data_ary[i];
			}

			if (remain_num == 0)
			{
				continue;
			}
			DailyActivitiesTemplate* template_ptr = get_find_back_template_ptr(i, m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_exp_level_yesterday);
			if (nullptr == template_ptr)
			{
				continue;
			}
			if (0 == template_ptr->IsCanFindBack)
			{
				continue;
			}
			bool is_unlock = m_func_unlock_mgr.is_func_unlock(template_ptr->FuncUnlockName);
			if (!is_unlock)
			{
				continue;
			}

			int32 item_back_Array_num = template_ptr->ItemFindBackArray.size();
			int32 dynamic_back_array_num = template_ptr->DynamicFindBackInfo.size();
			int32 static_back_array_num = template_ptr->StaticFindBackInfo.size();
			if (item_back_Array_num % 2 != 0 || dynamic_back_array_num % 2 != 0 || static_back_array_num % 2 != 0)
			{
				continue;
			}

			int32 dynamic_money_num = dynamic_back_array_num / 2;
			for (int32 money_index = 0; money_index < dynamic_money_num; ++money_index)
			{
				if (i == e_daily_must_do_typ_arena)
				{
					break;
				}
				//��̬�仯��������ҵȼ�����Ǯ��Ҫ��ϵ��*��׼ֵ
				int32 money_typ = template_ptr->DynamicFindBackInfo[money_index * 2];
				int32 money_coefficient = template_ptr->DynamicFindBackInfo[money_index * 2 + 1];
				int64 money_num = 0;
				if (money_typ == e_money_type_exp)
				{
					money_num = money_coefficient * upgrade_template_ptr->BaseExp;
				}
				else if (money_typ == e_money_type_silver_bind)
				{
					money_num = money_coefficient * upgrade_template_ptr->BaseCoin;
				}
				else if (money_typ == e_money_type_red_diamond)
				{
					money_num = money_coefficient * upgrade_template_ptr->BaseCoin;
				}
				float real_money_num = (float)money_num * (float)remain_num * (float)find_back_coefficient;

				money_list.push_back(money_typ);
				money_list.push_back(real_money_num);
				msg.add_money_reward(money_typ);
				msg.add_money_reward(int64(real_money_num));
			}

			int32 static_money_num = static_back_array_num / 2;
			for (int32 money_index = 0; money_index < static_money_num; ++money_index)
			{
				if (i == e_daily_must_do_typ_arena)
				{
					break;
				}
				//��̬Ǯֱ�Ӽ�
				int32 money_typ = template_ptr->StaticFindBackInfo[money_index * 2];
				int32 money_num = template_ptr->StaticFindBackInfo[money_index * 2 + 1];
				float real_money_num = money_num * remain_num * find_back_coefficient;
				money_list.push_back(money_typ);
				money_list.push_back(real_money_num);
				msg.add_money_reward(money_typ);
				msg.add_money_reward(int32(real_money_num));

			}

			int32 item_find_back_num = item_back_Array_num / 2;
			for (int32 item_index = 0; item_index < item_find_back_num; ++item_index)
			{
				if (i == e_daily_must_do_typ_arena)
				{
					break;
				}
				int32 item_id = template_ptr->ItemFindBackArray[item_index * 2];
				int32 item_num = template_ptr->ItemFindBackArray[item_index * 2 + 1] * remain_num * find_back_coefficient;
				ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
				if (temp_template_ptr == nullptr)
				{
					continue;
				}
				if (item_num <= temp_template_ptr->max_pile_num)
				{
					item_list.push_back(item_id);
					item_list.push_back(item_num);
				}
				else
				{
					int32 max_pile_num = 0;
					if (temp_template_ptr->max_pile_num <= 0)
					{
						max_pile_num = 1;
					}
					else
					{
						max_pile_num = temp_template_ptr->max_pile_num;
					}
					int32 item_piles_count = item_num / max_pile_num;
					if (item_num % max_pile_num != 0)
					{
						item_piles_count += 1;
					}

					for (int32 piles_index = 0; piles_index < item_piles_count; ++piles_index)
					{
						if (item_num > max_pile_num)
						{
							item_list.push_back(item_id);
							item_list.push_back(max_pile_num);
							item_num -= max_pile_num;
						}
						else
						{
							item_list.push_back(item_id);
							item_list.push_back(item_num);
						}
					}
				}
			}
			if (i == e_daily_must_do_typ_arena)
			{
				const ArenaRankRewardTemplate* reward_template_ptr = template_manager::get_instance().get_arena_reward_by_arena_pos(yesterday_arena_level);
				if (reward_template_ptr == nullptr
					|| reward_template_ptr->ItemRewardArray.size() % 2 != 0
					|| reward_template_ptr->MoneyRewardArray.size() % 2 != 0)
				{
					return;
				}
				int32 item_reward_num = reward_template_ptr->ItemRewardArray.size() / 2;

				for (int32 j = 0; j < item_reward_num; ++j)
				{
					int32 item_id = reward_template_ptr->ItemRewardArray[j * 2];
					int32 item_num = reward_template_ptr->ItemRewardArray[j * 2 + 1] * remain_num * find_back_coefficient;

					ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
					if (temp_template_ptr == nullptr)
					{
						continue;
					}
					if (item_num <= temp_template_ptr->max_pile_num)
					{
						item_list.push_back(item_id);
						item_list.push_back(item_num);
					}
					else
					{
						int32 max_pile_num = 0;
						if (temp_template_ptr->max_pile_num <= 0)
						{
							max_pile_num = 1;
						}
						else
						{
							max_pile_num = temp_template_ptr->max_pile_num;
						}
						int32 item_piles_count = item_num / max_pile_num;
						if (item_num % max_pile_num != 0)
						{
							item_piles_count += 1;
						}

						for (int32 piles_index = 0; piles_index < item_piles_count; ++piles_index)
						{
							if (item_num > max_pile_num)
							{
								item_list.push_back(item_id);
								item_list.push_back(max_pile_num);
								item_num -= max_pile_num;
							}
							else
							{
								item_list.push_back(item_id);
								item_list.push_back(item_num);
							}
						}
					}
				}
				int32 money_reward_num = reward_template_ptr->MoneyRewardArray.size() / 2;

				for (int32 k = 0; k < money_reward_num; ++k)
				{
					int32 money_typ = reward_template_ptr->MoneyRewardArray[k * 2];
					int64 money_num = reward_template_ptr->MoneyRewardArray[k * 2 + 1];

					float real_money_num = money_num * remain_num * find_back_coefficient;
					money_list.push_back(money_typ);
					money_list.push_back(real_money_num);
					msg.add_money_reward(money_typ);
					msg.add_money_reward(int32(real_money_num));
				}
			}
			server_log::resource_find_back_log(get_third_info(), m_role_info, i, find_back_cost_typ, remain_num);

		}

		if (must_do_typ != -1)
		{
			server_log::resource_find_back_log(get_third_info(), m_role_info, must_do_typ, find_back_cost_typ, find_back_num);

		}

		int32 money_list_size = money_list.size();
		if (0 != money_list_size % 2)
		{
			return;
		}
		if (true == get_item_set().put_in_bag(e_server_log_add_item_daily_must_do_arena, yesterday_arena_level, item_list))
		{
			cut_money(e_money_type(find_back_cost_typ), find_back_cost, e_server_log_cut_money_resource_back);


			for (int32 i = 0; i < money_list_size; i += 2)
			{
				int32 money_typ = money_list[i];
				int64 real_money_num = money_list[i + 1];
				add_money_or_exp(e_money_type(money_typ), real_money_num, e_server_log_add_money_resource_back);
			}

			if (-1 == must_do_typ)
			{
				for (int32 i = 0; i < resource_find_back_day; i++)
				{
					memset(m_three_day_must_do_remain_ary[i].data_ary, 0, sizeof(int32) * e_daily_must_do_typ_max);
				}
			}
			else
			{
				for (int32 i = 0; i < resource_find_back_day; i++)
				{
					m_three_day_must_do_remain_ary[i].data_ary[must_do_typ] = 0;
				}
			}

		}
		else
		{
			msg.set_operate_stat(e_item_string_bag_full);
			send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
			return;
		}

		update_three_day_must_do_remain();
		msg.set_operate_stat(e_item_string_succeed);
		send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
	}


	void player::find_back_serveral_resource_process(const int32& must_do_typ, bool is_diamond_find, const int32& find_back_count)
	{
		ZoneScoped;
		if (must_do_typ < 0 || must_do_typ >= e_daily_must_do_typ_max)
		{
			return;
		}
		int32 final_find_back_count = 0;
		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			final_find_back_count += m_three_day_must_do_remain_ary[i].data_ary[must_do_typ];
		}
		if (find_back_count <= 0 || find_back_count > final_find_back_count)
		{
			return;
		}

		character_proto_find_back_must_do_resource_end msg;
		msg.set_must_do_typ(must_do_typ);
		msg.set_is_diamond_find(is_diamond_find);

		float find_back_coefficient = 0;
		if (is_diamond_find)
		{
			find_back_coefficient = GAMECONFIG->DiamondFindBackPercentage / 100.0f;
		}
		else
		{
			find_back_coefficient = GAMECONFIG->SiliverFindBackPercentage / 100.0f;
		}

		int32 yesterday_exp_level = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_exp_level_yesterday;
		int32 yesterday_vip_level = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_vip_level_yesterday;
		int32 yesterday_arena_level = m_three_day_must_do_remain_ary[resource_find_back_day - 1].role_arena_rank;
		int32 upgrade_template_id = get_unit_info(e_role_info_upgrade_id);

		PlayerUpgradeTemplate* upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);
		if (upgrade_template_ptr == nullptr)
		{
			msg.set_operate_stat(e_item_string_unkown);
			send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
			return;
		}
		if (upgrade_template_ptr->Level != yesterday_exp_level)
		{
			//�������еĵȼ���֮ǰ���ǰһ��ĵȼ���ƥ�䣬Ҫ�����ҵ���ȷ����һ��
			upgrade_template_id -= upgrade_template_ptr->Level - yesterday_exp_level;
		}
		upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);

		std::vector<int32> item_list;
		item_list.reserve(20);
		std::vector<int64> money_list;
		money_list.reserve(50);
		int32 find_back_cost = 0;
		int32 find_back_cost_typ = -1;
		if (is_diamond_find)
		{
			find_back_cost_typ = e_money_type_jewel_bind;
		}
		else
		{
			find_back_cost_typ = e_money_type_silver_bind;
		}
		m_yesterday_back_type = find_back_cost_typ;
		DailyActivitiesTemplate* template_ptr = get_find_back_template_ptr(must_do_typ, yesterday_exp_level);
		if (template_ptr == nullptr)
		{
			return;
		}
		if (0 == template_ptr->IsCanFindBack)
		{
			return;
		}
		bool is_unlock = m_func_unlock_mgr.is_func_unlock(template_ptr->FuncUnlockName);
		if (!is_unlock)
		{
			return;
		}
		if (is_diamond_find)
		{
			find_back_cost = template_ptr->DiamondBackCost * find_back_count;
		}
		else
		{
			find_back_cost = template_ptr->SiliverBackCost * find_back_count;
		}

		if (!can_cut_money(e_money_type(find_back_cost_typ), find_back_cost))
		{
			if (is_diamond_find)
			{
				msg.set_operate_stat(e_item_string_jewel);
			}
			else
			{
				msg.set_operate_stat(e_item_string_money);
			}
			send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
			return;
		}

		int32 item_back_Array_num = template_ptr->ItemFindBackArray.size();
		int32 dynamic_back_array_num = template_ptr->DynamicFindBackInfo.size();
		int32 static_back_array_num = template_ptr->StaticFindBackInfo.size();
		if (item_back_Array_num % 2 != 0 || dynamic_back_array_num % 2 != 0 || static_back_array_num % 2 != 0)
		{
			return;
		}

		int32 dynamic_money_num = dynamic_back_array_num / 2;
		for (int32 money_index = 0; money_index < dynamic_money_num; ++money_index)
		{
			if (must_do_typ == e_daily_must_do_typ_arena)
			{
				break;
			}
			//��̬�仯��������ҵȼ�����Ǯ��Ҫ��ϵ��*��׼ֵ
			int32 money_typ = template_ptr->DynamicFindBackInfo[money_index * 2];
			int64 money_coefficient = template_ptr->DynamicFindBackInfo[money_index * 2 + 1];
			int64 money_num = 0;
			if (money_typ == e_money_type_exp)
			{
				money_num = money_coefficient * (int64)upgrade_template_ptr->BaseExp;
			}
			else if (money_typ == e_money_type_silver_bind)
			{
				money_num = money_coefficient * (int64)upgrade_template_ptr->BaseCoin;
			}
			else if (money_typ == e_money_type_red_diamond)
			{
				money_num = money_coefficient * (int64)upgrade_template_ptr->BaseCoin;
			}
			float real_money_num = (float)money_num * (float)find_back_count * find_back_coefficient;

			money_list.push_back(money_typ);
			money_list.push_back(real_money_num);
			msg.add_money_reward(money_typ);
			msg.add_money_reward(int64(real_money_num));
		}

		int32 static_money_num = static_back_array_num / 2;
		for (int32 money_index = 0; money_index < static_money_num; ++money_index)
		{
			if (must_do_typ == e_daily_must_do_typ_arena)
			{
				break;
			}
			//��̬Ǯֱ�Ӽ�
			int32 money_typ = template_ptr->StaticFindBackInfo[money_index * 2];
			int32 money_num = template_ptr->StaticFindBackInfo[money_index * 2 + 1];
			float real_money_num = money_num * find_back_count * find_back_coefficient;
			money_list.push_back(money_typ);
			money_list.push_back(real_money_num);
			msg.add_money_reward(money_typ);
			msg.add_money_reward(int32(real_money_num));

		}

		int32 item_find_back_num = item_back_Array_num / 2;
		for (int32 item_index = 0; item_index < item_find_back_num; ++item_index)
		{
			if (must_do_typ == e_daily_must_do_typ_arena)
			{
				break;
			}
			int32 item_id = template_ptr->ItemFindBackArray[item_index * 2];
			int32 item_num = template_ptr->ItemFindBackArray[item_index * 2 + 1] * find_back_count * find_back_coefficient;
			ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
			if (temp_template_ptr == nullptr)
			{
				continue;
			}
			if (item_num <= temp_template_ptr->max_pile_num)
			{
				item_list.push_back(item_id);
				item_list.push_back(item_num);
			}
			else
			{
				int32 max_pile_num = 0;
				if (temp_template_ptr->max_pile_num <= 0)
				{
					max_pile_num = 1;
				}
				else
				{
					max_pile_num = temp_template_ptr->max_pile_num;
				}
				int32 item_piles_count = item_num / max_pile_num;
				if (item_num % max_pile_num != 0)
				{
					item_piles_count += 1;
				}

				for (int32 piles_index = 0; piles_index < item_piles_count; ++piles_index)
				{
					if (item_num > max_pile_num)
					{
						item_list.push_back(item_id);
						item_list.push_back(max_pile_num);
						item_num -= max_pile_num;
					}
					else
					{
						item_list.push_back(item_id);
						item_list.push_back(item_num);
					}
				}
			}
		}
		if (must_do_typ == e_daily_must_do_typ_arena)
		{
			const ArenaRankRewardTemplate* reward_template_ptr = template_manager::get_instance().get_arena_reward_by_arena_pos(yesterday_arena_level);
			if (reward_template_ptr == nullptr
				|| reward_template_ptr->ItemRewardArray.size() % 2 != 0
				|| reward_template_ptr->MoneyRewardArray.size() % 2 != 0)
			{
				return;
			}
			int32 item_reward_num = reward_template_ptr->ItemRewardArray.size() / 2;

			for (int32 i = 0; i < item_reward_num; ++i)
			{
				int32 item_id = reward_template_ptr->ItemRewardArray[i * 2];
				int32 item_num = reward_template_ptr->ItemRewardArray[i * 2 + 1] * find_back_count * find_back_coefficient;

				ItemTemplate* temp_template_ptr = GET_TEMPLATE(ItemTemplate, item_id);
				if (temp_template_ptr == nullptr)
				{
					continue;
				}
				if (item_num <= temp_template_ptr->max_pile_num)
				{
					item_list.push_back(item_id);
					item_list.push_back(item_num);
				}
				else
				{
					int32 max_pile_num = 0;
					if (temp_template_ptr->max_pile_num <= 0)
					{
						max_pile_num = 1;
					}
					else
					{
						max_pile_num = temp_template_ptr->max_pile_num;
					}
					int32 item_piles_count = item_num / max_pile_num;
					if (item_num % max_pile_num != 0)
					{
						item_piles_count += 1;
					}

					for (int32 piles_index = 0; piles_index < item_piles_count; ++piles_index)
					{
						if (item_num > max_pile_num)
						{
							item_list.push_back(item_id);
							item_list.push_back(max_pile_num);
							item_num -= max_pile_num;
						}
						else
						{
							item_list.push_back(item_id);
							item_list.push_back(item_num);
						}
					}
				}
			}
			int32 money_reward_num = reward_template_ptr->MoneyRewardArray.size() / 2;

			for (int32 i = 0; i < money_reward_num; ++i)
			{
				int32 money_typ = reward_template_ptr->MoneyRewardArray[i * 2];
				int64 money_num = reward_template_ptr->MoneyRewardArray[i * 2 + 1];

				float real_money_num = money_num * find_back_count * find_back_coefficient;
				money_list.push_back(money_typ);
				money_list.push_back(real_money_num);
				msg.add_money_reward(money_typ);
				msg.add_money_reward(int32(real_money_num));
			}
		}

		int32 money_list_size = money_list.size();
		if (0 != money_list_size % 2)
		{
			return;
		}
		if (true == get_item_set().put_in_bag(e_server_log_add_item_daily_must_do_arena, yesterday_arena_level, item_list))
		{
			cut_money(e_money_type(find_back_cost_typ), find_back_cost, e_server_log_cut_money_resource_back);
			for (int32 i = 0; i < money_list_size; i += 2)
			{
				int32 money_typ = money_list[i];
				int64 real_money_num = money_list[i + 1];
				add_money_or_exp(e_money_type(money_typ), real_money_num, e_server_log_add_money_resource_back);
			}
			int32 temp_find_back_count = find_back_count;
			for (int32 i = 0; i < resource_find_back_day; i++)
			{
				if (!m_three_day_must_do_remain_ary[i].role_guid.is_valid())
				{
					continue;
				}
				if (m_three_day_must_do_remain_ary[i].data_ary[must_do_typ] < temp_find_back_count)
				{
					temp_find_back_count -= m_three_day_must_do_remain_ary[i].data_ary[must_do_typ];
					m_three_day_must_do_remain_ary[i].data_ary[must_do_typ] = 0;
				}
				else
				{
					m_three_day_must_do_remain_ary[i].data_ary[must_do_typ] -= temp_find_back_count;
					temp_find_back_count = 0;
					break;
				}
			}

			m_already_must_do_remain_data_ary[must_do_typ] += find_back_count;
		}
		else
		{
			msg.set_operate_stat(e_item_string_bag_full);
			send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
			return;
		}
		server_log::resource_find_back_log(get_third_info(), m_role_info, must_do_typ, find_back_cost_typ, find_back_count);


		update_three_day_must_do_remain();
		msg.set_operate_stat(e_item_string_succeed);
		send_message_to_self(&msg, e_msgindex_s2c_find_back_must_do_resource_end);
	}

	void player::update_find_back_count(int32 must_do_type, int32 find_back_count)
	{
		ZoneScoped;
		int32 temp_find_back_count = find_back_count;
		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			if (!m_three_day_must_do_remain_ary[i].role_guid.is_valid())
			{
				continue;
			}
			if (m_three_day_must_do_remain_ary[i].data_ary[must_do_type] < temp_find_back_count)
			{
				temp_find_back_count -= m_three_day_must_do_remain_ary[i].data_ary[must_do_type];
				m_three_day_must_do_remain_ary[i].data_ary[must_do_type] = 0;
			}
			else
			{
				m_three_day_must_do_remain_ary[i].data_ary[must_do_type] -= temp_find_back_count;
				temp_find_back_count = 0;
				break;
			}
		}
		m_already_must_do_remain_data_ary[must_do_type] += find_back_count;
	}
	s_unit_yesterday_must_do_remain& player::get_find_back_info()
	{
		ZoneScoped;
		return m_three_day_must_do_remain_ary[resource_find_back_day - 1];
	}

	int32 player::get_find_count_by_type(int32 must_do_type)
	{
		ZoneScoped;
		if (must_do_type < 0 || must_do_type >= e_daily_must_do_typ_max)
		{
			return 0;
		}
		int32 final_find_back_count = 0;
		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			final_find_back_count += m_three_day_must_do_remain_ary[i].data_ary[must_do_type];
		}
		return final_find_back_count;
	}

	void player::update_three_day_must_do_remain()
	{
		ZoneScoped;
		character_proto_update_three_day_must_do_remain msg;
		msg.add_role_guid(get_unit_guid().A);
		msg.add_role_guid(get_unit_guid().B);
		for (int32 i = 0; i < resource_find_back_day; i++)
		{
			if (!m_three_day_must_do_remain_ary[i].role_guid.is_valid())
			{
				continue;
			}
			character_proto_daily_must_do_remain_info* remain_info = msg.add_remain_three_day_must_do();
			remain_info->add_role_guid(get_unit_guid().A);
			remain_info->add_role_guid(get_unit_guid().B);
			remain_info->set_role_level(m_three_day_must_do_remain_ary[i].role_exp_level_yesterday);
			remain_info->set_save_time(m_three_day_must_do_remain_ary[i].save_date);
			remain_info->set_vip_level(m_three_day_must_do_remain_ary[i].role_vip_level_yesterday);
			remain_info->set_arena_rank(m_three_day_must_do_remain_ary[i].role_arena_rank);
			for (int32 must_do_typ = 0; must_do_typ < e_daily_must_do_typ_max; must_do_typ++)
			{
				remain_info->add_data_array(m_three_day_must_do_remain_ary[i].data_ary[must_do_typ]);
			}
		}
		send_message_to_self(&msg, e_msgindex_s2c_update_three_day_must_do_remain);
	}

	void player::recharge_by_template_id(int32 goods_id, const xstring& order_id, int64 order_num, int32 payment_type, float pay_price, int32 recharge_time_count)
	{
		ZoneScoped;
		if (recharge_time_count <= 0)
		{
			recharge_time_count = 1;
		}
		cs2ws_recharge_end send_msg;
		//if (order_id == nullptr)
		//{
		//	send_msg.result = e_recharge_error_recharge_by_template_id_order_id_is_null;
		//	connection_mgr::getInstance().send_to_all_ws( &send_msg, sizeof(cs2ws_recharge_end));
		//	server_log::recharge_error_log("", get_unit_guid(), goods_id, e_recharge_error_recharge_by_template_id_order_id_is_null);
		//	return;
		//}
		send_msg.result = e_recharge_error_none;
		send_msg.order_num = order_num;
		send_msg.payment_type = payment_type;
		send_msg.role_guid = get_unit_guid();
		send_msg.goods_id = goods_id;

		int32 order_id_len = order_id.length();
		if (order_id_len > max_recharge_order_length)
		{
			send_msg.result = e_recharge_error_db_save_recharge_order_id_len_bigger;
			connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(cs2ws_recharge_end));
			server_log::recharge_error_log("", get_unit_guid(), goods_id, e_recharge_error_db_save_recharge_order_id_len_bigger);
			return;
		}
		memcpy(send_msg.order_id, order_id.c_str(), order_id_len);

		server_log::recharge_step_log(order_id, get_unit_guid(), goods_id, e_recharge_step_cs_recharge_by_template_id, get_third_info(), m_login_type, &m_role_info);

		RechargeTemplate* recharge_template_ptr = GET_TEMPLATE(RechargeTemplate, goods_id);
		if (recharge_template_ptr == nullptr
			|| recharge_template_ptr->MoneyGetArray.size() % 2 != 0)
		{
			send_msg.result = e_recharge_error_recharge_by_template_id_recharge_template_ptr_is_null;
			connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(cs2ws_recharge_end));
			return;
		}
		int32 recharge_typ = recharge_template_ptr->Type;
		guid_64 role_guid = get_unit_guid();

		//RechargeTemplate* recharge_template_ptr = template_manager::get_instance().get_template_by_recharge_type(recharge_typ);
		//if ((recharge_template_ptr == nullptr) || (recharge_template_ptr->MoneyGetArray.size() % 2 != 0))
		//{
		//	send_msg.result = e_recharge_error_recharge_by_template_id_MoneyGetArray_error;
		//	connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(cs2ws_recharge_end));
		//	return;
		//}

		//������
		if (recharge_template_ptr->MonthCardAddTime < 0)
		{
			if (get_unit_info(e_role_info_exclusive_flag) != 1)
			{
				set_unit_info(e_role_info_exclusive_flag, 1);
				set_time_data(e_time_type_exclusive_card_activity_time, 1);
				send_time_one(e_time_type_exclusive_card_activity_time);
				send_info_one(e_role_info_exclusive_flag);
			}
		}

		int32 month_card_add_day = recharge_template_ptr->MonthCardAddTime * recharge_time_count;
		if (month_card_add_day > 0)
		{
			int32 month_card_cur_day = get_time_data(e_time_type_month_card_activity_time);
			int32 month_card_remain_day = get_time_data(e_time_type_month_card_remain_time);
			if (month_card_cur_day == 0)
			{
				set_time_data(e_time_type_month_card_activity_time, 1);
			}
			if (get_time_data(e_time_type_month_card_remain_time) < 0)
			{
				set_time_data(e_time_type_month_card_remain_time, 0);
			}
			set_time_data(e_time_type_month_card_remain_time, month_card_remain_day + month_card_add_day);
			send_time_one(e_time_type_month_card_remain_time);
			send_time_one(e_time_type_month_card_activity_time);
		}

		int32 cur_vip_level = get_vip_level(false);

		std::vector<int32> money_reward_array;
		money_reward_array.reserve(recharge_template_ptr->MoneyGetArray.size());
		for (int32 i = 0; i < recharge_template_ptr->MoneyGetArray.size(); i += 2)
		{
			int32 temp_money_id = recharge_template_ptr->MoneyGetArray[i];
			int32 temp_money_num = recharge_template_ptr->MoneyGetArray[i + 1] * recharge_time_count;
			money_reward_array.push_back(temp_money_id);
			money_reward_array.push_back(temp_money_num);
		}
		do_add_vip(money_reward_array, recharge_template_ptr->VipPointBonuse * recharge_time_count, goods_id, order_id, pay_price, payment_type);

		int32 new_vip_level = get_vip_level(false);

		//�����״γ�ֵ�õ�λ,���ö��⽱��
		bool if_is_first_recharge = m_welfare_mgr.get_welfare_by_first_recharge(goods_id);
		send_msg.is_first_recharge = if_is_first_recharge;
		get_ranking_mgr().sync_vip_level_to_ws_ranking();

		send_msg.result = e_recharge_error_none;
		connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(cs2ws_recharge_end));

		//�жϲ���ȡС���ֵ����
		get_time_limit_activity_mgr().apply_small_charge_award(goods_id);
		//�жϲ���ȡÿ��ֱ���������
		get_time_limit_activity_mgr().apply_daily_gift_charge_award(goods_id);
		//�ж���Ԫ������
		get_time_feed_back_mgr().apply_recharge(goods_id);
		//�ж���ʱ�������
		get_time_limit_gift_mgr().apply_recharge(goods_id);
		get_competition_mgr().check_is_manual(goods_id);

		get_element_competition_mgr().check_is_manual(goods_id);

		get_star_trip_mgr().target_mission(e_star_trip_target_type_recharge);
		server_log::recharge_step_log(order_id, get_unit_guid(), goods_id, e_recharge_step_cs_recharge_by_template_id_end, get_third_info(), m_login_type, &m_role_info);

		if (m_is_dead_recharge)
		{
			dead_widget_recharge_auto_get_welfare();
			set_recharget_widget_type(false);
		}

		//�³�ֵ����
		if (get_login_type() == faith::e_login_type_new_account_and_password || get_login_type() == faith::e_login_type_new_zw)
		{
			cs2fep_month_recharge_num msg;
			memcpy(msg.account, get_account(), max_account_length);
			msg.array_index = get_array_index();
			msg.client_uid = get_client_uid();
			msg.role_guid = get_unit_guid();
			msg.recharge_num = recharge_template_ptr->Price;
			send_message(&msg, sizeof(msg));
		}
	}

	void player::set_recharget_widget_type(bool is_dead_recharge)
	{
		ZoneScoped;
		m_is_dead_recharge = is_dead_recharge;
	}

	void player::dead_widget_recharge_auto_get_welfare()
	{
		ZoneScoped;
		//���������ֵ �Զ���ȡ���� װ������ ����ʱװ װ��ʱװ ��Ѹ���
		faith::template_manager::template_type* table = template_manager::get_instance().get_templates(e_WelfareTemplate);
		if (nullptr == table)
		{
			return;
		}
		faith::template_manager::template_type::iterator ite;
		for (ite = table->begin(); ite != table->end(); ++ite)
		{
			WelfareTemplate* welfare_template_ptr = (WelfareTemplate*)(ite->second);
			if (welfare_template_ptr == nullptr)
			{
				continue;
			}
			if (welfare_template_ptr->Type == faith::e_welfare_type_first_recharge_reward)
			{
				get_welfare_mgr().get_welfare_by_id(welfare_template_ptr->attribute_id);

				for (int32 i = 0; i < welfare_template_ptr->RewardItemArray.size(); i++)
				{
					ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, welfare_template_ptr->RewardItemArray[i]);
					if (item_template_ptr == nullptr)
					{
						continue;
					}
					PropBasicAttributeTemplate* item_logic_template_ptr = GET_TEMPLATE(PropBasicAttributeTemplate, item_template_ptr->logic_id);
					if (item_logic_template_ptr == nullptr)
					{
						continue;
					}
					DropTemplate* drop_template_ptr = GET_TEMPLATE(DropTemplate, item_logic_template_ptr->ItemGet);
					if (drop_template_ptr == nullptr)
					{
						continue;
					}
					for (int32 j = 0; j < drop_template_ptr->IDs.size(); j++)
					{
						citem* temp_item_ptr = get_item_set().get_item_by_template_id(drop_template_ptr->IDs[j]);
						if (temp_item_ptr == nullptr)
						{
							continue;
						}
						if (get_item_set().is_class_enable(temp_item_ptr) == false)
						{
							continue;
						}
						ItemTemplate* item_template_ptr = temp_item_ptr->get_item_info_ptr();
						if (item_template_ptr == nullptr)
						{
							continue;
						}
						if (item_template_ptr->item_type == faith::e_item_type_weapon)
						{
							int32 temp_item_slot = faith::e_role_equip_slot_weapon_1;
							get_item_set().equip_on(temp_item_ptr->get_item_guid(), temp_item_slot);
						}
						break;
					}
					break;
				}

				for (int32 i = 0; i < welfare_template_ptr->RewardItemArray.size(); i++)
				{
					ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, welfare_template_ptr->RewardItemArray[i]);
					if (item_template_ptr == nullptr)
					{
						continue;
					}
					citem* temp_item_ptr = get_item_set().get_item_by_template_id(item_template_ptr->FashionId);
					if (temp_item_ptr == nullptr)
					{
						continue;
					}
					get_item_set().fashion_upgrade(temp_item_ptr->get_item_guid(), 0);

					ItemTemplate* fashion_item_template_ptr = temp_item_ptr->get_item_info_ptr();
					if (fashion_item_template_ptr == nullptr)
					{
						continue;
					}

					citem* cur_equip_fashion_ptr = get_item_set().get_item_by_slot(faith::e_bag_type_equip_fasion, fashion_item_template_ptr->sub_type);
					if (cur_equip_fashion_ptr == nullptr)
					{
						get_item_set().equip_on(temp_item_ptr->get_item_guid(), fashion_item_template_ptr->sub_type);
					}
					break;
				}

				back_to_life(faith::e_revive_type_same_pos, true);
			}
		}
		character_proto_dead_recharge_end msg;
		msg.add_role_guid(get_unit_guid().A);
		msg.add_role_guid(get_unit_guid().B);
		send_message_to_self(&msg, e_msgindex_s2c_dead_recharge_end);
	}

	void player::do_add_vip(const std::vector<int32>& money_array, int32 vip_point, int32 goods_id, std::string order_id_str, float pay_price, int32 fixed_payment_type)
	{
		ZoneScoped;
		if (money_array.size() % 2 != 0)
		{
			return;
		}
		int32 money_reward_num = money_array.size() / 2;
		int32 jewel_add_value = 0;

		for (int32 i = 0; i < money_reward_num; ++i)
		{
			int32 add_money_id = money_array[i * 2];
			int64 add_money_value = money_array[i * 2 + 1];
			if (add_money_id < 0 || add_money_id >= e_money_type_max)
			{
				continue;
			}

			add_money_or_exp(e_money_type(add_money_id), add_money_value, e_server_log_add_money_recharge, goods_id);
			if (add_money_id == e_money_type_jewel)
			{
				jewel_add_value += add_money_value;
			}
			send_add_money_msg(add_money_id, add_money_value);
		}

		//����VIP����
		int32 cur_vip_level = get_vip_level(false);
		int32 cur_vip_buff_level = get_vip_level(true);
		int32 add_vip_point = vip_point;

		int32 old_recharge_num = get_unit_info(e_role_info_recharge_num);

		set_unit_info(e_role_info_recharge_num, old_recharge_num + add_vip_point);
		int32 cur_real_recharge_jewel_num = get_unit_info(e_role_info_real_recharge_jewel_num);
		set_unit_info(e_role_info_real_recharge_jewel_num, cur_real_recharge_jewel_num + add_vip_point);
		if (cur_real_recharge_jewel_num == 0)
		{
			int32 cur_time = time_helper::get_today_zero_time_info().second;
			if (cur_time > 0)
			{
				set_time_data(e_time_type_frist_recharge_time, cur_time);
				get_welfare_mgr().get_first_recharge_welfare_by_id();
			}
		}
		//��ʱ��ۼƳ�ֵ��������
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_charge, add_vip_point);
		get_time_limit_activity_mgr().reset_ranking_value(e_time_limit_activity_type_total_charge);

		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_cross_total_charge, add_vip_point);
		get_time_limit_activity_mgr().reset_ranking_value(e_time_limit_activity_type_cross_total_charge);

		//��ʱ�ÿ�ճ�ֵ��������
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge2, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge3, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge4, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge5, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_daily_charge6, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_total_charge2, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_charge_1, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_charge_2, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_new_total_charge_3, add_vip_point);

		//ȫ��۱���ֵ��������
		//���
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_1, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_2, add_vip_point);
		//����
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_3, add_vip_point);
		get_time_limit_activity_mgr().add_activity_schedule(e_time_limit_activity_type_national_treasure_4, add_vip_point);

		//ͬ����ֵ���а���Ϣ
		m_ranking_mgr.set_and_sync_ranking_data(e_RankingIndex_recharge_rmb, get_unit_info(e_role_info_real_recharge_jewel_num));

		//ͬ����ʱ���ֵ��Ϊ
		get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_recharge);

		int32 cur_daily_recharge_num = get_unit_info(e_role_info_daily_recharge_num);
		set_unit_info(e_role_info_daily_recharge_num, cur_daily_recharge_num + add_vip_point);

		calculate_vip_level();
		int32 new_vip_level = get_vip_level(false);
		if (new_vip_level != cur_vip_level)
		{
			if (new_vip_level > cur_vip_buff_level)
			{
				vip_buff_change(cur_vip_buff_level, false);
				vip_buff_change(new_vip_level, true);
			}
			refresh_vip_title();
			broadcast_info_one(e_role_info_vip_level);

			//ͬ��vip�ȼ���session
			sync_data_to_ws(e_sync_cs2ws_data_vip_level, new_vip_level);
		}

		send_info_one(e_role_info_recharge_num);
		send_info_one(e_role_info_daily_recharge_num);
		send_info_one(e_role_info_real_recharge_jewel_num);

		if (get_write_log())
		{
			if (old_recharge_num == 0)
			{
				server_log::first_recharge_log(get_unit_guid().server_64, true);
			}

			xstring cur_time = time_helper::get_cur_time();
			server_log::month_card_log(cur_time, 1);
			server_log::recharge_log(get_third_info(), get_unit_info_inst(), get_money_data(e_money_type_jewel), order_id_str, pay_price, jewel_add_value, goods_id, old_recharge_num == 0, fixed_payment_type, m_login_type);
			server_log::today_recharge_log(vip_point, get_unit_info(e_role_info_daily_recharge_num), get_unit_info(e_role_info_vip_level));

			int32 is_first_recharge = 0;
			if (old_recharge_num == 0)
			{
				is_first_recharge = 1;
			}

			set_log_var(new_log_head)
				get_log_common_head_info(new_log_head);
			server_log::serverRechargeSuccess(new_log_head, get_unit_info(e_role_info_vip_level), "", order_id_str, pay_price, 0, 0, 0,
				m_login_type, (fixed_payment_type > 0 ? init_unit::change_i32_to_string(fixed_payment_type) : m_login_third_data.param14),
				jewel_add_value, get_money_data(e_money_type_jewel), 1, goods_id, is_first_recharge, m_login_third_data.param8);



			if (new_vip_level != cur_vip_level)
			{
				bool is_get_old_welfare = false;
				int32 vip_item_get_flag = get_unit_info(e_role_info_vip_item_geted_level);
				int32 target_flag = 1 << cur_vip_level;
				if (!(vip_item_get_flag & target_flag))
				{
					is_get_old_welfare = true;
				}
				server_log::vip_log(cur_vip_level, is_get_old_welfare, get_unit_info(e_role_info_vip_level));

				set_log_var(log_head);
				//set_log_common_head_part1(log_head, SERVERCONFIG->app_key,m_login_third_data.param7, m_login_third_data.param4, m_login_third_data.param5);
				//set_log_common_head_part2(log_head, m_login_third_data.param2, 0);
				//set_log_common_head_part3(log_head, m_role_info.account, init_unit::change_i32_to_string(m_role_info.data_ary[e_role_info_server_id]), utility::get_tick_count())
				//set_log_common_head_part4(log_head, init_unit::change_i64_to_string(m_role_info.role_guid.server_64), m_role_info.role_name, m_role_info.data_ary[e_role_info_exp_level], m_role_info.data_ary[e_role_info_gender])
				//set_log_common_head_part5(log_head, m_role_info.data_i64_ary[e_role_i64_info_gs_value], init_unit::change_i32_to_string(m_role_info.data_ary[e_role_info_class_type]), m_role_info.data_ary[e_role_info_vip_level])
				get_log_common_head_info(log_head);
				server_log::serverVipUpgrade(log_head, get_unit_info(e_role_info_vip_level), 0);
			}
		}

		save_role_all_data(e_save_data_type_update);

	}

	void player::calculate_vip_level()
	{
		ZoneScoped;
		int32 cur_vip_level = get_vip_level(false);
		int32 cur_vip_point_num = get_unit_info(e_role_info_recharge_num);//VIP�����Ļس�ֵ��ʯ���

		int32 new_vip_level = 0;
		for (; new_vip_level < GAMECONFIG->MaxVIPLevel; ++new_vip_level)
		{
			VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(new_vip_level);
			if (vip_template_ptr == nullptr)
			{
				break;
			}
			if (cur_vip_point_num < vip_template_ptr->LevelUpRequest)
			{
				break;
			}
		}
		set_unit_info(e_role_info_vip_level, new_vip_level);

		if (cur_vip_level != new_vip_level)
		{
			fix_bag_slot_open_time_by_vip(cur_vip_level, new_vip_level);
			send_open_bag_info();
		}
	}

	void player::vip_buff_change(int32 vip_level, bool is_add)
	{
		ZoneScoped;
		VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
		if (vip_template_ptr == nullptr)
		{
			return;
		}
		int32 role_array_index = get_array_index();
		buff_ex_env_param penv;
		penv.buff_level = 1;

		if (is_add)
		{
			buff_man::add_buff_inst(role_array_index, role_array_index, vip_template_ptr->PropertyBonuseBuffID, &penv);
			buff_man::add_buff_inst(role_array_index, role_array_index, vip_template_ptr->KIllMonsterBUffID, &penv);
			get_convert_mgr().reset_convert_data(false);
		}
		else
		{
			buff_man::del_buff_by_template_id(role_array_index, role_array_index, vip_template_ptr->PropertyBonuseBuffID);
			buff_man::del_buff_by_template_id(role_array_index, role_array_index, vip_template_ptr->KIllMonsterBUffID);
		}
	}

	void player::refresh_vip_title()
	{
		ZoneScoped;

		int32 cur_vip_level = get_vip_level();

		for (int32 vip_level = 0; vip_level <= GAMECONFIG->MaxVIPLevel; ++vip_level)
		{
			VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(vip_level);
			if (nullptr == vip_template_ptr
				|| vip_template_ptr->TitleId <= 0)
			{
				continue;
			}
			if (cur_vip_level < vip_level)
			{
				m_title_mgr.remove_title_by_template_id(vip_template_ptr->TitleId);
			}
			else
			{
				m_title_mgr.add_title_by_template_id(vip_template_ptr->TitleId);
			}
		}
	}

	bool player::is_need_refresh_daily_info_at_zero_hour()
	{
		ZoneScoped;
		i64 last_refresh = get_time_data(e_time_type_last_daily_info_refresh_time);

		if (last_refresh <= 0)
		{
			return true;
		}

		time_info last_refresh_time_info = time_helper::get_time_by_stamp_new(last_refresh);
		time_info cur_time_info = time_helper::get_cur_time_new();

		if (last_refresh_time_info.year != cur_time_info.year
			|| last_refresh_time_info.month_in_year != cur_time_info.month_in_year
			|| last_refresh_time_info.day_in_month != cur_time_info.day_in_month)
		{
			return true;
		}
		return false;
	}

	void player::clear_harry_info_at_harry_time()
	{
		ZoneScoped;
		m_server_harry_mgr.clear_harry_info_all();

		int64 next_refresh_time = -1;
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		int32 cur_day = time_helper::get_cur_time_new().day_in_week;
		int32 refresh_day = act_com_cfg->ParamInt1;
		int64 stamp_by_hour_min = time_helper::get_stamp_by_hour_min(0, 0);
		int32 need_day = 0;
		if (refresh_day > cur_day)
		{
			need_day = refresh_day - cur_day;
		}
		else if (refresh_day == cur_day)
		{
			need_day = all_day_in_week;
		}
		else
		{
			need_day = all_day_in_week - cur_day + refresh_day;
		}
		next_refresh_time = stamp_by_hour_min + need_day * day_time_second;
		set_time_data(e_time_type_last_harry_clear_time, next_refresh_time);
	}

	bool player::is_need_clear_harry_info_at_harry_time()
	{
		ZoneScoped;
		ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_cfg_ptr)
		{
			return false;
		}
		int32 clear_day = act_cfg_ptr->ParamInt1;
		int64 next_refresh = get_time_data(e_time_type_last_harry_clear_time);
		if (next_refresh <= 0)
		{
			return true;
		}
		int64 cur_time_sec = time_helper::get_cur_time_new().second;
		if (cur_time_sec >= next_refresh)
		{
			return true;
		}
		return false;
	}

	bool player::is_need_refresh_daily_info_at_zero_hour_no_cross()
	{
		ZoneScoped;
		if (!is_self_server())
		{
			return false;
		}
		int64 last_refresh = get_time_data(e_time_type_last_refresh_by_zero_no_cross);
		if (last_refresh <= 0)
		{
			return true;
		}
		time_info last_refresh_time_info = time_helper::get_time_by_stamp_new(last_refresh);
		time_info cur_time_info = time_helper::get_cur_time_new();

		if (last_refresh_time_info.year != cur_time_info.year
			|| last_refresh_time_info.month_in_year != cur_time_info.month_in_year
			|| last_refresh_time_info.day_in_month != cur_time_info.day_in_month)
		{
			return true;
		}
		return false;
	}
	bool player::is_need_refresh_daily_info_at_zero_hour_cross()
	{
		ZoneScoped;
		if (is_self_server())
		{
			return false;
		}
		int64 last_refresh = get_time_data(e_time_type_last_refresh_by_zero_cross);
		if (last_refresh <= 0)
		{
			return true;
		}
		time_info last_refresh_time_info = time_helper::get_time_by_stamp_new(last_refresh);
		time_info cur_time_info = time_helper::get_cur_time_new();

		if (last_refresh_time_info.year != cur_time_info.year
			|| last_refresh_time_info.month_in_year != cur_time_info.month_in_year
			|| last_refresh_time_info.day_in_month != cur_time_info.day_in_month)
		{
			return true;
		}
		return false;
	}

	bool	player::is_need_refresh_daily_info_at_specify_hour()
	{
		ZoneScoped;
		i64 last_save_refresh = get_time_data(e_time_type_last_refresh_time);
		if (last_save_refresh <= 0)
		{
			return true;
		}

		return time_helper::is_over_refresh_time(last_save_refresh);
	}


	bool player::is_need_refresh_gs_up_start_value_time()
	{
		ZoneScoped;
		i64 last_refresh = get_time_data(e_time_type_last_refresh_gs_up_start_value_time);

		if (last_refresh <= 0)
		{
			return true;
		}

		time_info last_refresh_time_info = time_helper::get_time_by_stamp_new(last_refresh);
		time_info cur_time_info = time_helper::get_cur_time_new();

		if (last_refresh_time_info.year != cur_time_info.year
			|| last_refresh_time_info.month_in_year != cur_time_info.month_in_year
			|| last_refresh_time_info.day_in_month != cur_time_info.day_in_month)
		{
			return true;
		}
		return false;
	}

	void player::refresh_daily_info(bool bOnline)
	{
		ZoneScoped;
		bool is_refresh_by_zero = false;
		bool is_refresh_by_special_time = false;
		//ÿ�����ˢ��
		if (is_need_refresh_daily_info_at_zero_hour())
		{
			send_mail_daily_legion_no_receive_rewards();//�����û��ȡ�ľ��Ÿ������ȷ����ʼ�,��ˢ��
			refresh_at_zero_hour(bOnline);
			is_refresh_by_zero = true;
		}

		//ÿ�����ˢ�°�
		if (is_need_refresh_daily_info_at_specify_hour())
		{
			send_mail_daily_activity_no_receive_rewards();//�����û��ȡ�Ļ�Ծ�Ƚ������ȷ����ʼ�,��ˢ��
			refresh_at_specify_hour(bOnline);
			is_refresh_by_special_time = true;
		}

		if (is_refresh_by_zero
			|| is_refresh_by_special_time)
		{
			m_welfare_mgr.send_random_get_all();
			m_welfare_mgr.send_welfare_all();
			m_welfare_mgr.send_active_degree();
			send_info_all(is_refresh_by_zero, is_refresh_by_special_time);
			send_time_all();
			send_logic_all();
			send_money_all();
		}
	}

	void player::refresh_at_zero_hour(bool bOnline)
	{
		ZoneScoped;
		int32 cur_time = time_helper::get_cur_time_new().second;
		int32 last_refresh_time = get_time_data(e_time_type_last_daily_info_refresh_time);

		set_unit_info(e_role_info_vip_free_revive_time, 0);
		set_unit_info(e_role_info_element_heart_refine_time, 0);
		set_unit_info(e_role_info_daily_recharge_num, 0);
		set_unit_info(e_role_info_daily_cost, 0);

		set_unit_info(e_role_info_lord_of_city_worship_time, 0);
		set_unit_info(e_role_info_lord_of_sanctuary_worship_time, 0);
		set_unit_info(e_role_info_lord_of_war_worship_time, 0);
		set_unit_info(e_role_info_lord_of_god_worship_time, 0);

		set_unit_info(e_role_info_legion_donate_earn_by_money, 0);
		set_unit_info(e_role_info_legion_donate_earn_by_item, 0);

		set_unit_info(e_role_info_red_package_geted_times, 0);
		set_unit_info(e_role_info_free_red_package_send_times, 0);
		set_unit_info(e_role_info_legion_dance_exp_num, 0);

		int32 contiune_logine_time = get_unit_info(e_role_info_continue_login_time);

		//��ʹ���ֻ��һ�죬cur_time - last_refresh_time Ҳ���ܴ���24Сʱ���� �����賿1�����ߣ������賿2�����ߣ����25Сʱ
		//�� �����족��ȥ24Сʱ һ��������
		int32 cur_yesterday = cur_time - 24 * 3600;

		time_info last_refresh_time_info = time_helper::get_time_by_stamp_new(last_refresh_time);
		time_info yesterday_time_info = time_helper::get_time_by_stamp_new(cur_yesterday);

		time_info today_time_info = time_helper::get_time_by_stamp_new(cur_time);
		time_info today_zero_time_info = time_helper::get_time_by_stamp_new(today_time_info.second - today_time_info.second_in_day);
		time_info last_refresh_zero_time_info = time_helper::get_time_by_stamp_new(last_refresh_time_info.second - last_refresh_time_info.second_in_day);
		int32 diff_day_num = (today_zero_time_info.second - last_refresh_zero_time_info.second) / (24 * 3600);
		if (diff_day_num < 0)
		{
			diff_day_num = 0;
		}

		if (last_refresh_time_info.year != yesterday_time_info.year
			|| last_refresh_time_info.month_in_year != yesterday_time_info.month_in_year
			|| last_refresh_time_info.day_in_month != yesterday_time_info.day_in_month)
		{
			set_unit_info(e_role_info_continue_login_time, 1);
		}
		else
		{
			set_unit_info(e_role_info_continue_login_time, contiune_logine_time + 1);
		}

		i32 cur_total_login_num = get_unit_info(e_role_info_total_login_days);
		set_unit_info(e_role_info_total_login_days, cur_total_login_num + 1);

		//�ۼƵ�¼�������Ӳ鿴�Ƿ��пɻ�óƺ�
		m_title_mgr.add_title_by_type_and_value(faith::e_title_type_login_day_num, cur_total_login_num + 1);

		//�������
		m_welfare_mgr.refresh_daily_info();
		//ˢ���ۼ�ǩ��
		m_welfare_mgr.refresh_grand_total_check_in_num();

		m_star_trip_mgr.refresh_today_mission();
		//����
		if (0 != get_unit_info(e_role_info_exclusive_flag))
		{
			int32 exclusive_card_times = get_time_data(e_time_type_exclusive_card_activity_time);
			set_time_data(e_time_type_exclusive_card_activity_time, exclusive_card_times + diff_day_num);
			if (31 < get_time_data(e_time_type_exclusive_card_activity_time))
			{
				set_time_data(e_time_type_exclusive_card_activity_time, 1);
				m_welfare_mgr.set_welfare_info(e_welfare_type_exclusive_check_in, 0);
			}
		}

		//�¿�
		int32 month_card_cur_day = get_time_data(e_time_type_month_card_activity_time);
		int32 month_card_remain_day = get_time_data(e_time_type_month_card_remain_time);
		if (month_card_remain_day > 0)
		{
			set_time_data(e_time_type_month_card_activity_time, month_card_cur_day + diff_day_num);
			set_time_data(e_time_type_month_card_remain_time, month_card_remain_day - diff_day_num);

			if (get_time_data(e_time_type_month_card_activity_time) > 30)
			{
				//30�����������Ϊ1
				set_time_data(e_time_type_month_card_activity_time, 1);
				m_welfare_mgr.set_welfare_info(e_welfare_type_monthcard_reward, 0);
			}
			if (get_time_data(e_time_type_month_card_remain_time) < 0)
			{
				set_time_data(e_time_type_month_card_remain_time, 0);
			}
		}
		else
		{
			set_time_data(e_time_type_month_card_activity_time, 0);
			set_time_data(e_time_type_month_card_remain_time, 0);
			m_welfare_mgr.set_welfare_info(e_welfare_type_monthcard_reward, 0);
		}
		set_time_data(e_time_type_last_daily_info_refresh_time, cur_time);
		set_time_data(e_time_type_login_time, cur_time);//����Ҫ���õ�¼ʱ��
		// ���ŵ�ÿ���콱��� ����ÿ���콱ʱ����
		set_time_data(e_time_type_legion_daily_award_flags, 0);
		if (get_write_log())
		{
			server_log::legion_info_log(get_legion_info().legion_guid.server_64, get_legion_info().legion_name, get_legion_info().legion_level, get_legion_info().legion_role_num);

			//set_log_var(log_head)
			//get_log_common_head_info(log_head);
			//server_log::serverLegionLog(log_head, get_legion_info().legion_name, get_legion_info().legion_level, get_legion_info().legion_power, get_legion_info().legion_role_num);
		}
		send_open_server_time_to_self();

		if (bOnline)
		{
			int64 login_time = get_login_time();
			int64 online_time = utility::get_tick_count() - login_time;
			int64 create_time = get_unit_i64_info_data(e_role_i64_info_create_time);
			server_log::across_day_log(m_login_third_data, m_role_info, online_time, create_time, m_login_type);
		}
		m_service_rank_mgr.check_receive_reward_end_time();

		refresh_kill_legion_boss_get_award_today();//����boss��ɱˢ��

		m_server_harry_mgr.refresh_day_harry_info();//����Ӷ�ÿ�����

		set_logic_data(e_role_logic_info_cross_ladder_join_num, 0);
		set_logic_data(e_role_logic_info_cross_ladder_win_num, 0);
		set_logic_data(e_role_logic_info_cross_ladder_reward_tag, 0);
		set_logic_data(e_role_logic_info_daily_lucky_times, 0);

		set_logic_data(e_role_logic_info_element_war_buy_ticket, 0);
		set_logic_data(e_role_logic_info_element_war_mission_tag, 0);

		get_guide_mgr().send_all_guide_record();

		m_dragontrip_mgr.day_refresh();





		server_log::jewel_count_log(get_unit_info_inst(), get_logic_info_inst(), get_third_info(), get_money_data_inst(), get_login_type());
	}


	void player::refresh_at_zero_hour_no_cross(bool bOnline)
	{
		ZoneScoped;
		int32 cur_time = time_helper::get_cur_time_new().second;
		int64 cur_time_big = time_helper::get_cur_time_new().millisecond;


		m_time_limit_activity_mgr.refresh_by_zero(cur_time_big);
		m_store_helper.update_record(true, e_goods_purchase_limit_type_per_day);


		set_time_data(e_time_type_last_refresh_by_zero_no_cross, cur_time);
	}

	void player::refresh_at_weekly()
	{
		ZoneScoped;
		m_store_helper.update_record(true, e_goods_purchase_limit_type_per_week);
	}


	void player::refresh_at_zero_hour_cross(bool bOnline)
	{
		ZoneScoped;
		int32 cur_time = time_helper::get_cur_time_new().second;
		int64 cur_time_big = time_helper::get_cur_time_new().millisecond;

		//m_time_limit_activity_mgr.refresh_by_zero_cross(cur_time_big);
		set_time_data(e_time_type_last_refresh_by_zero_cross, cur_time);
	}
	void player::refresh_at_specify_hour(bool bOnline)
	{
		ZoneScoped;

		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr
			|| region_template_ptr->RegionCode >= GAMECONFIG->SpecialRefreshTimeHour.size())
		{
			return;
		}

		refresh_three_day_must_do_remain_info(bOnline);
		update_three_day_exp();
		update_three_day_must_do_remain();
		updata_daily_must_do_count();
		m_welfare_mgr.refresh_at_specify_hour();
		m_worship_target.fresh_data();
		m_gain_treasure_mgr.refresh_gain_treasure_info();//��������
		m_boss_island_mgr.refresh_all_boss_island_info();
		get_convert_mgr().reset_convert_data(true);

		set_unit_info(e_role_info_crystal_dreamland_chests, 0);
		set_unit_info(e_role_info_world_boss_chests, 0);
		set_unit_info(e_role_info_assist_chests, 0);

		set_unit_info(e_role_info_daily_activity_degree, 0);
		set_unit_info(e_role_info_daily_active_degree_get_flag, 0);
		set_unit_info(e_role_info_arena_times, 0);
		set_unit_info(e_role_info_arena_vip_times, 0);
		set_unit_info(e_role_info_arena_vip_buy_times, 0);
		set_time_data(e_time_type_legion_bonus_award_flags, 0);//�ֺ�������

		set_unit_info(e_role_info_relic_exp_num, 0);
		set_unit_info(e_role_info_relic_kill_num, 0);//�Ϲ��ż���ɱ���뾭��ֵ����
		clear_relic_exp();

		//set_logic_data(e_role_logic_info_cross_ladder_join_num, 0);
		//set_logic_data(e_role_logic_info_cross_ladder_win_num, 0);
		//set_logic_data(e_role_logic_info_cross_ladder_reward_tag, 0);

		set_logic_data(e_role_logic_info_helper_reward_send_times, 0);
		set_logic_data(e_role_logic_info_gain_treasure_restore_num, 0);
		set_logic_data(e_role_logic_info_gain_treasure_restore_time, 0);
		set_logic_data(e_role_logic_info_sky_land_boss_restore_num, 0);
		set_logic_data(e_role_logic_info_sky_land_boss_restore_time, 0);
		set_logic_data(e_role_logic_info_help_reward_send_times, 0);
		set_logic_data(e_role_logic_info_exp_raid_add_damage_gold, 0);
		set_logic_data(e_role_logic_info_exp_raid_add_damage_diamond, 0);

		refresh_boss_challenge_times();

		set_time_data(e_time_type_ancient_battelfield, GAMECONFIG->AncientBattlefieldTime * second_tick_time);
		set_time_data(e_time_type_exp_raid_join_time, 0);
		set_time_data(e_time_type_exp_raid_extra_time, 0);
		set_time_data(e_time_type_exp_raid_skill_cd, 0);
		//set_must_do_count(e_daily_must_do_typ_raid_ancient_battlefield, GAMECONFIG->AncientBattlefieldTime * second_tick_time);

		get_mission_mgr().init_against_mission();
		get_mission_mgr().init_daily_mission();
		get_mission_mgr().init_marry_mission();
		get_mission_mgr().send_all_mission();
		get_marry_mgr().reset_marry_dati();
		get_marry_mgr().sync_dati_marry(0);
		get_lucky_card_mgr().refresh_free_num();

		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(get_array_index());
		//if (map_record_set_ref.is_valid())
		//{
		//	map_record_set_ref.refresh_all_map_record_enter_count();
		//}

		int64 next_refresh_time = time_helper::get_next_refresh_time_stamp(-1, GAMECONFIG->SpecialRefreshTimeHour[region_template_ptr->RegionCode]);
		set_time_data(e_time_type_last_refresh_time, next_refresh_time);

		m_subscribe_daily_list.clear();
		send_subscribe_daily_info();

		for (int32 i = 0; i < GAMECONFIG->DailyPlayCount.size(); i = i + 2)
		{
			e_money_type money_type = e_money_type(GAMECONFIG->DailyPlayCount[i]);
			int32 money_count = GAMECONFIG->DailyPlayCount[i + 1];
			auto cur_money = get_money_data(money_type);
			if (cur_money >= money_count)
			{
				continue;
			}
			set_money_data(money_type, money_count);
		}
	}

	void player::send_mail_daily_activity_no_receive_rewards()
	{
		ZoneScoped;
		int32 cur_acyivity = get_unit_info(e_role_info_daily_activity_degree); //��ǰ��Ծֵ
		if (cur_acyivity > 0)
		{
			std::vector<int32> item_array;
			s_item_info mail_item_array[max_item_per_mail];
			int32 mail_item_array_size = 0;
			RegionTemplate* region_ptr = globle_data::get_instance().get_region_template_ptr();
			if (nullptr == region_ptr)
			{
				return;
			}
			std::vector<WelfareTemplate*> temp_template_array = template_manager::get_instance().get_welfare_template_ptr_array_by_type(e_welfare_type_daily_activity_reward, cur_acyivity);
			if (temp_template_array.size() > 0)
			{
				for (int32 i = 0; i < temp_template_array.size(); i++)
				{
					if (!m_welfare_mgr.is_welfare_geted(e_welfare_type_daily_activity_reward, i))
					{
						if (temp_template_array[i]->RewardItemArray.size() / 3 > 0 && temp_template_array[i]->RewardItemArray.size() % 3 == 0)
						{
							int32 j = 0;
							for (; j < temp_template_array[i]->RewardItemArray.size() / 3; j++)
							{
								if (region_ptr->RegionCode == temp_template_array[i]->RewardItemArray[j * 3])
								{
									item_array.push_back(temp_template_array[i]->RewardItemArray[j * 3 + 1]);
									item_array.push_back(temp_template_array[i]->RewardItemArray[j * 3 + 2]);
								}
							}
						}
					}
				}
			}

			i64 last_refresh_time = get_time_data(e_time_type_last_refresh_time);
			if (last_refresh_time <= 0)
			{
				return;
			}

			for (int32 index = 0; index < item_array.size(); index += 2)
			{
				citem* item_ptr = get_item_set().create_item_by_template(e_server_log_add_item_daily_activity_reward, 0, item_array[index], item_array[index + 1], 1);
				if (item_ptr != nullptr)
				{
					mail_item_array[mail_item_array_size] = item_ptr->get_item_inst();
					mail_item_array_size++;
					if (mail_item_array_size >= max_item_per_mail)
					{
						get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), "90303044", "90303045", 0, 0, 0, 0, mail_item_array, mail_item_array_size, last_refresh_time);
						memset(mail_item_array, 0, max_item_per_mail * sizeof(s_item_info));
						mail_item_array_size = 0;
					}
				}
			}

			if (mail_item_array_size > 0)
			{
				get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), "90303044", "90303045", 0, 0, 0, 0, mail_item_array, mail_item_array_size, last_refresh_time);
			}

		}
	}

	void player::send_mail_daily_legion_no_receive_rewards()
	{
		ZoneScoped;
		if (false == get_legion_guid().is_valid())
		{
			return;
		}
		if (get_time_data(e_time_type_login_time) <= get_time_data(e_time_type_logout_time) && time_helper::is_diff_day(get_time_data(e_time_type_logout_time), time_helper::get_cur_time_new().second - day_time_second))
		{
			return;
		}
		int32 award_level = get_legion_info().legion_construction_level[ELegionInfo_construction_level_3];
		int32 legion_template_id = first_legion_template_id + award_level - 1;
		LegionConstructionsTemplate* legion_template_ptr = GET_TEMPLATE(LegionConstructionsTemplate, legion_template_id);
		if (nullptr == legion_template_ptr)
		{
			return;
		}

		int64 legion_daily_rwd_flag_data = get_time_data(e_time_type_legion_daily_award_flags);
		int32 is_got_rwd = init_unit::get_flag_data(legion_daily_rwd_flag_data, e_legion_daily_rwd_flag_daily_everyone);
		if (1 == is_got_rwd)
		{
			return;
		}

		std::vector<int32> item_array = legion_template_ptr->MemDailyAwards;
		s_item_info mail_item_array[max_item_per_mail];
		int32 mail_item_array_size = 0;

		int64 last_refresh = get_time_data(e_time_type_last_refresh_time);
		if (last_refresh <= 0)
		{
			return;
		}
		int64  cur_second = last_refresh - sec_five_hour_tick_time;

		for (int32 i = 0; i < item_array.size(); i += 2)
		{
			citem* item_ptr = get_item_set().create_item_by_template(e_server_log_add_item_get_welfare, legion_template_id, item_array[i], item_array[i + 1], 0);
			if (item_ptr != nullptr)
			{
				mail_item_array[mail_item_array_size] = item_ptr->get_item_inst();
				mail_item_array_size++;
				if (mail_item_array_size >= max_item_per_mail)
				{
					get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), "90303042", "90303043", 0, 0, 0, 0, mail_item_array, mail_item_array_size, cur_second);
					memset(mail_item_array, 0, max_item_per_mail * sizeof(s_item_info));
					mail_item_array_size = 0;
				}
			}
		}

		if (mail_item_array_size > 0)
		{
			get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), "90303042", "90303043", 0, 0, 0, 0, mail_item_array, mail_item_array_size, cur_second);
		}

	}

	bool player::is_need_refresh_month_info()
	{
		ZoneScoped;
		int32 last_refresh = get_time_data(e_time_type_last_month_info_refresh_time);
		if (last_refresh <= 0)
		{
			return true;
		}
		time_info last_refresh_time_info = time_helper::get_time_by_stamp_new(last_refresh);
		time_info cur_time_info = time_helper::get_cur_time_new();

		if (last_refresh_time_info.year != cur_time_info.year
			|| last_refresh_time_info.month_in_year != cur_time_info.month_in_year)
		{
			return true;
		}
		return false;
	}

	void player::refresh_month_info()
	{
		ZoneScoped;
		// ��������ˢ��
		m_welfare_mgr.refresh_month_info();

		int32 cur_time = time_helper::get_cur_time_new().second;
		set_time_data(e_time_type_last_month_info_refresh_time, cur_time);
	}

	int32 player::get_patron_saint_activated_num()
	{
		ZoneScoped;
		return m_patron_saint_mgr.get_mark_activated_num();
	}

	int32 player::get_patron_saint_level_num()
	{
		ZoneScoped;
		return m_patron_saint_mgr.get_mark_level_sum();
	}

	bool player::is_grow_up_fund_purchased(int32 fund_type)
	{
		ZoneScoped;
		int32 temp_flag = 1 << fund_type;
		int32 cur_payment_flag = get_unit_info(e_role_info_fund_payment_flag);
		if (temp_flag & cur_payment_flag)
		{
			return true;
		}
		return false;
	}
	bool player::is_grow_up_fund_geted(int32 fund_type, int32 index)
	{
		ZoneScoped;
		int32 temp_flag = 1 << index;
		int32 cur_flag = 0;
		switch (fund_type)
		{
		case faith::e_grow_up_fund_type_reborn:
		{
			cur_flag = get_unit_info(faith::e_role_info_fund_reborn_get_flag);
			if (cur_flag & temp_flag)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
		case faith::e_grow_up_fund_type_time:
		{
			cur_flag = get_unit_info(faith::e_role_info_fund_time_get_flag);
			if (cur_flag & temp_flag)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
		case faith::e_grow_up_fund_type_pride:
		{
			cur_flag = get_unit_info(faith::e_role_info_fund_pride_get_flag);
			if (cur_flag & temp_flag)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
		default:
			break;
		}
		return true;
	}
	bool player::is_grow_up_fund_enable(int32 fund_template_id)
	{
		ZoneScoped;
		GrowUpFundTemplate* grow_up_fund_template_ptr = GET_TEMPLATE(GrowUpFundTemplate, fund_template_id);
		if (grow_up_fund_template_ptr == nullptr)
		{
			return false;
		}
		int32 fund_type = grow_up_fund_template_ptr->Type;
		switch (fund_type)
		{
		case faith::e_grow_up_fund_type_reborn:
		{
			int32 cur_value = get_unit_info(faith::e_role_info_exp_level);
			if (cur_value >= grow_up_fund_template_ptr->TargetValue)
			{
				return true;
			}
		}
		break;
		case faith::e_grow_up_fund_type_time:
		{
			int32 cur_value = get_unit_info(faith::e_role_info_total_login_days);
			if (cur_value >= grow_up_fund_template_ptr->TargetValue)
			{
				return true;
			}
		}
		break;
		case faith::e_grow_up_fund_type_pride:
		{
			int64 cost_value = get_unit_i64_info_data(faith::e_role_i64_info_gs_value);
			if (cost_value >= grow_up_fund_template_ptr->TargetValue)
			{
				return true;
			}
		}
		break;
		default:
			break;
		}
		return false;
	}

	void player::change_harry_buff()
	{
		ZoneScoped;
		if (get_server_harry_mgr().is_have_item())
		{
			buff_man::add_buff_inst(m_array_index, m_array_index, harry_special_buff);
		}
		else
		{
			buff_man::del_buff_by_template_id(m_array_index, m_array_index, harry_special_buff);
		}
	}

	void player::find_other_player_info(guid_64 target_guid, int32 server_id)
	{
		ZoneScoped;
		//�Ȳ鿴������Ƭ��Ϣ
		get_person_infor_mgr().get_other_person_information_to_dp(target_guid, true);

		faith::cs2dp_get_other_player_info get_other_player_info_msg;
		get_other_player_info_msg.role_guid = get_unit_guid();
		get_other_player_info_msg.target_guid = target_guid;
		connection_mgr::getInstance().send_to_dp(&get_other_player_info_msg, sizeof(get_other_player_info_msg), server_id);
	}

	void player::find_other_player_info_base_end(s_unit_info other_player_info)
	{
		ZoneScoped;
		if (other_player_info.data_i64_ary[e_role_i64_info_gs_value] <= 0 && other_player_info.data_ary[e_role_i64_info_gs_value] > 0)
		{
			other_player_info.data_i64_ary[e_role_i64_info_gs_value] = other_player_info.data_ary[e_role_i64_info_gs_value];
		}
		faith::character_proto_find_other_unit_base_end find_other_unit_base_end_msg;
		find_other_unit_base_end_msg.set_target_guid(other_player_info.role_guid.server_64);
		find_other_unit_base_end_msg.set_account(other_player_info.account);
		find_other_unit_base_end_msg.set_role_name(other_player_info.role_name);
		find_other_unit_base_end_msg.set_role_appearance(other_player_info.role_appearance);
		for (int32 index = 0; index < e_role_info_max; index++)
		{
			find_other_unit_base_end_msg.add_data_array(other_player_info.data_ary[index]);
		}
		guid_64 team_guid = guid_64();
		player& player_ref = unit_man::get_player(other_player_info.role_guid);
		if (player_ref.is_valid())
		{
			team_guid = player_ref.get_team_guid();
		}
		if (team_guid.is_valid())
		{
			find_other_unit_base_end_msg.set_team_guid(team_guid.server_64);
		}
		for (int32 i = 0; i < e_role_i64_info_max; i++)
		{
			find_other_unit_base_end_msg.add_data_str_array(to_string(other_player_info.data_i64_ary[i]));
		}
		send_message_to_self(&find_other_unit_base_end_msg, e_msgindex_s2c_find_other_unit_base_end);
	}
	void player::find_other_player_info_equiping_end(const s_item_info_db* other_player_info)
	{
		ZoneScoped;
		faith::character_proto_find_other_unit_equiping_end find_other_unit_equiping_end_msg;
		find_other_unit_equiping_end_msg.set_target_guid(other_player_info[0].role_guid.server_64);
		find_other_unit_equiping_end_msg.set_is_finish(false);
		for (int32 index = 0; index < faith::role_show_item_num; index++)
		{
			if (!other_player_info[index].data_info.item_guid.is_valid())
			{
				break;
			}

			faith::character_proto_find_other_unit_item_one* find_other_unit_equiping_one_msg = find_other_unit_equiping_end_msg.add_equiping_info();
			find_other_unit_equiping_one_msg->set_item_guid(other_player_info[index].data_info.item_guid.server_64);
			for (int32 i = 0; i < e_item_info_max; i++)
			{
				find_other_unit_equiping_one_msg->add_data_array(other_player_info[index].data_info.data_ary[i]);
			}

			int32 pak_size = find_other_unit_equiping_end_msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				send_message_to_self(&find_other_unit_equiping_end_msg, e_msgindex_s2c_find_other_unit_equiping_end);
				find_other_unit_equiping_end_msg.clear_equiping_info();
			}
		}
		find_other_unit_equiping_end_msg.set_is_finish(true);
		send_message_to_self(&find_other_unit_equiping_end_msg, e_msgindex_s2c_find_other_unit_equiping_end);

		set_other_player_info_buff(other_player_info);
		return;
	}

	void player::set_other_player_info_buff(const s_item_info_db* other_player_info)	//ͨ��װ��end��ȡ����buff��Ϣ
	{
		ZoneScoped;
		if (other_player_info == nullptr)
		{
			return;
		}
		faith::character_proto_find_other_unit_buff_equip_end find_other_unit_buff_end_msg;
		find_other_unit_buff_end_msg.set_target_guid(other_player_info[0].role_guid.server_64);

		//����ǿ����׿Խbuff��ģ��id
		//ǿ��buff
		int32 current_buff_id = -1;
		faith::template_manager::template_type* addition_buff_tmpl_table = template_manager::get_instance().get_templates(e_AdditionBuffTemplate);
		if (nullptr == addition_buff_tmpl_table)
		{
			return;
		}

		faith::template_manager::template_type::iterator ite;
		int32 max_id = 0;          // ��ǰ����Ч�����Ч����BUFFģ���ID
		int32 max_equip_num = 0;   // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��װ������
		int32 max_upgrade_num = 0; // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��ǿ������
		for (ite = addition_buff_tmpl_table->begin(); ite != addition_buff_tmpl_table->end(); ++ite)
		{
			AdditionBuffTemplate* addition_tmpl_ptr = (AdditionBuffTemplate*)(ite->second);
			if (nullptr == addition_tmpl_ptr || addition_tmpl_ptr->AdditionBuffType != e_addition_buff_upgrade + 1)
			{
				continue;
			}
			int32 equip_num = 0;

			for (int32 i = e_role_equip_slot_body; i < e_role_equip_slot_max; i++)
			{
				if (!other_player_info[i].role_guid.is_valid())
				{
					continue;
				}
				if ((other_player_info[i].data_info.data_ary[faith::e_item_info_slot] < e_role_equip_slot_amulet) && (other_player_info[i].data_info.data_ary[faith::e_item_info_upgrade_count] >= addition_tmpl_ptr->UpgradeNum))
				{
					equip_num++;
				}
			}
			// ��������������if����˵����ǰ�ĸ�װ��ǿ�������������BUFF�������ٱȽ����BUFF�Ƿ���ĿǰΪֹЧ����ѵ�BUFF
			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (addition_tmpl_ptr->EquipNum > max_equip_num || addition_tmpl_ptr->UpgradeNum > max_upgrade_num)
				{
					max_id = addition_tmpl_ptr->attribute_id;
					max_equip_num = addition_tmpl_ptr->EquipNum;
					max_upgrade_num = addition_tmpl_ptr->UpgradeNum;
				}
			}
		}
		find_other_unit_buff_end_msg.add_buff_id(max_id);
		//׿Խbuff

		max_id = 0;          // ��ǰ����Ч�����Ч����BUFFģ���IDmax_equip_num
		max_equip_num = 0;   // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��װ������
		int32 max_quality = 0; // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��ǿ������
		for (ite = addition_buff_tmpl_table->begin(); ite != addition_buff_tmpl_table->end(); ++ite)
		{
			AdditionBuffTemplate* addition_tmpl_ptr = (AdditionBuffTemplate*)(ite->second);
			if (nullptr == addition_tmpl_ptr || addition_tmpl_ptr->AdditionBuffType != e_addition_buff_quality + 1)
			{
				continue;
			}
			int32 equip_num = 0;

			for (int32 i = e_role_equip_slot_body; i < e_role_equip_slot_max; i++)
			{
				if (!other_player_info[i].role_guid.is_valid())
				{
					continue;
				}
				ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, other_player_info[i].data_info.data_ary[faith::e_item_info_info_id]);
				if (item_template_ptr == nullptr)
				{
					continue;
				}
				if ((other_player_info[i].data_info.data_ary[faith::e_item_info_slot] < e_role_equip_slot_amulet) && item_template_ptr->item_color >= addition_tmpl_ptr->EquipQuality)
				{
					equip_num++;
				}
			}
			// ��������������if����˵����ǰ�ĸ�װ��ǿ�������������BUFF�������ٱȽ����BUFF�Ƿ���ĿǰΪֹЧ����ѵ�BUFF
			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (addition_tmpl_ptr->EquipNum > max_equip_num || addition_tmpl_ptr->EquipQuality > max_quality)
				{
					max_id = addition_tmpl_ptr->attribute_id;
					max_equip_num = addition_tmpl_ptr->EquipNum;
					max_quality = addition_tmpl_ptr->EquipQuality;
				}
			}
		}
		find_other_unit_buff_end_msg.add_buff_id(max_id);

		//׷��
		max_id = 0;          // ��ǰ����Ч�����Ч����BUFFģ���IDmax_equip_num
		max_equip_num = 0;   // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��װ������
		int32 max_addon = 0; // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��ǿ������
		for (ite = addition_buff_tmpl_table->begin(); ite != addition_buff_tmpl_table->end(); ++ite)
		{
			AdditionBuffTemplate* addition_tmpl_ptr = (AdditionBuffTemplate*)(ite->second);
			if (nullptr == addition_tmpl_ptr || addition_tmpl_ptr->AdditionBuffType != e_addition_buff_addon + 1)
			{
				continue;
			}
			int32 equip_num = 0;

			for (int32 i = e_role_equip_slot_body; i < e_role_equip_slot_max; i++)
			{
				if (!other_player_info[i].role_guid.is_valid())
				{
					continue;
				}
				if ((other_player_info[i].data_info.data_ary[faith::e_item_info_slot] < e_role_equip_slot_amulet) && (other_player_info[i].data_info.data_ary[faith::e_item_info_add_on] >= addition_tmpl_ptr->AddOnLevel))
				{
					equip_num++;
				}
			}
			// ��������������if����˵����ǰ�ĸ�װ��ǿ�������������BUFF�������ٱȽ����BUFF�Ƿ���ĿǰΪֹЧ����ѵ�BUFF
			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (addition_tmpl_ptr->EquipNum > max_equip_num || addition_tmpl_ptr->AddOnLevel > max_addon)
				{
					max_id = addition_tmpl_ptr->attribute_id;
					max_equip_num = addition_tmpl_ptr->EquipNum;
					max_addon = addition_tmpl_ptr->AddOnLevel;
				}
			}
		}
		find_other_unit_buff_end_msg.add_buff_id(max_id);

		//ϴ��
		max_id = 0;          // ��ǰ����Ч�����Ч����BUFFģ���IDmax_equip_num
		max_equip_num = 0;   // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��װ������
		int32 max_succinct = 0; // ��ǰ��Ч�����Ч����BUFFģ���Ҫ��ǿ������
		for (ite = addition_buff_tmpl_table->begin(); ite != addition_buff_tmpl_table->end(); ++ite)
		{
			AdditionBuffTemplate* addition_tmpl_ptr = (AdditionBuffTemplate*)(ite->second);
			if (nullptr == addition_tmpl_ptr || addition_tmpl_ptr->AdditionBuffType != e_addition_buff_succinct + 1)
			{
				continue;
			}
			int32 equip_num = 0;

			for (int32 i = e_role_equip_slot_body; i < e_role_equip_slot_max; i++)
			{
				if (!other_player_info[i].role_guid.is_valid())
				{
					continue;
				}
				if ((other_player_info[i].data_info.data_ary[faith::e_item_info_slot] < e_role_equip_slot_amulet) && (other_player_info[i].data_info.data_ary[faith::e_item_info_succinct_level] >= addition_tmpl_ptr->SuccinctFullQualityLevel))
				{
					equip_num++;
				}
			}
			// ��������������if����˵����ǰ�ĸ�װ��ǿ�������������BUFF�������ٱȽ����BUFF�Ƿ���ĿǰΪֹЧ����ѵ�BUFF
			if (equip_num >= addition_tmpl_ptr->EquipNum)
			{
				if (addition_tmpl_ptr->EquipNum > max_equip_num || addition_tmpl_ptr->SuccinctFullQualityLevel > max_succinct)
				{
					max_id = addition_tmpl_ptr->attribute_id;
					max_equip_num = addition_tmpl_ptr->EquipNum;
					max_succinct = addition_tmpl_ptr->SuccinctFullQualityLevel;
				}
			}
		}
		find_other_unit_buff_end_msg.add_buff_id(max_id);

		send_message_to_self(&find_other_unit_buff_end_msg, e_msgindex_s2c_find_other_unit_buff_equip_end);
	}
	void player::find_other_player_info_special_name_end(const s_special_name_info_db other_player_info)
	{
		ZoneScoped;
		faith::character_proto_find_other_unit_special_name_end find_other_unit_special_name_end_msg;
		find_other_unit_special_name_end_msg.set_target_guid(other_player_info.role_guid.server_64);
		for (int32 i = 0; i < faith::e_special_name_type_max; i++)
		{
			find_other_unit_special_name_end_msg.add_special_name_level(other_player_info.data_info.data_ary[i]);
		}
		send_message_to_self(&find_other_unit_special_name_end_msg, e_msgindex_s2c_find_other_unit_special_name_end);
	}

	void player::find_other_player_info_base_group_end(int32 group_job, std::string group_name, guid_64 target_guid)
	{
		ZoneScoped;
		faith::character_proto_find_other_unit_base_group_end find_other_unit_base_group_end_msg;
		find_other_unit_base_group_end_msg.set_target_guid(target_guid.server_64);
		find_other_unit_base_group_end_msg.set_group_job(group_job);
		find_other_unit_base_group_end_msg.set_group_name(group_name);
		send_message_to_self(&find_other_unit_base_group_end_msg, e_msgindex_s2c_find_other_unit_base_group_end);
	}

	void player::find_other_player_info_spirit_end(const s_item_info_db other_player_info)
	{
		ZoneScoped;
		faith::character_proto_find_other_unit_spirit_end find_other_unit_spirit_end_msg;
		find_other_unit_spirit_end_msg.set_target_guid(other_player_info.role_guid.server_64);
		find_other_unit_spirit_end_msg.set_sprite_guid(other_player_info.data_info.item_guid.server_64);
		//s_spirit_info temp_spirit_info = get_spirit_mgr().make_fake_spirit_info(other_player_info.data_info);
		//for (int index = 0; index < e_spirit_info_max; index++)
		//{
		//	find_other_unit_spirit_end_msg.add_data_array(temp_spirit_info.data_ary[index]);
		//}
		send_message_to_self(&find_other_unit_spirit_end_msg, e_msgindex_s2c_find_other_unit_spirit_end);
	}
	void player::find_other_player_info_wing_or_mount_end(const s_item_info_db* other_player_info_array, bool is_wing)
	{
		ZoneScoped;
		faith::character_proto_find_other_unit_wing_or_mount_end find_other_unit_wing_or_mount_end_msg;

		find_other_unit_wing_or_mount_end_msg.set_target_guid(other_player_info_array[0].role_guid.server_64);
		find_other_unit_wing_or_mount_end_msg.set_is_wing(is_wing);
		find_other_unit_wing_or_mount_end_msg.set_is_finish(false);

		for (int32 i = 0; i < faith::role_show_item_num; i++)
		{
			if (!other_player_info_array[i].data_info.item_guid.is_valid())
			{
				break;
			}

			faith::character_proto_find_other_unit_item_one* find_other_unit_wing_or_mount_one_msg = find_other_unit_wing_or_mount_end_msg.add_mount_or_wing_info();
			find_other_unit_wing_or_mount_one_msg->set_item_guid(other_player_info_array[i].data_info.item_guid.server_64);
			for (int32 index = 0; index < e_item_info_max; index++)
			{
				find_other_unit_wing_or_mount_one_msg->add_data_array(other_player_info_array[i].data_info.data_ary[index]);
			}

			int32 pak_size = find_other_unit_wing_or_mount_end_msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				send_message_to_self(&find_other_unit_wing_or_mount_end_msg, e_msgindex_s2c_find_other_unit_wing_or_mount_one_end);
				find_other_unit_wing_or_mount_end_msg.clear_mount_or_wing_info();
			}
		}
		find_other_unit_wing_or_mount_end_msg.set_is_finish(true);
		send_message_to_self(&find_other_unit_wing_or_mount_end_msg, e_msgindex_s2c_find_other_unit_wing_or_mount_one_end);
	}

	void player::find_other_player_info_buff_end(guid_64 target_guid, const int32* buff_info, const int32& data_num)
	{
		ZoneScoped;
		if (data_num % e_enum_buff_info_max != 0)
		{
			return;
		}
		int32 data_group_num = data_num / e_enum_buff_info_max;
		int32 max_group_num = max_enum_buff_data_num / e_enum_buff_info_max;

		faith::character_proto_find_other_unit_buff_end find_other_unit_buff_end_msg;
		find_other_unit_buff_end_msg.set_target_guid(target_guid.server_64);

		for (int32 index = 0; index < data_group_num; index++)
		{
			if (index >= max_group_num)
			{
				break;
			}
			find_other_unit_buff_end_msg.add_buff_info(buff_info[index * e_enum_buff_info_max + e_enum_buff_info_id]);
			find_other_unit_buff_end_msg.add_buff_info(buff_info[index * e_enum_buff_info_max + e_enum_buff_info_effect_index]);
		}
		send_message_to_self(&find_other_unit_buff_end_msg, e_msgindex_s2c_find_other_unit_buff_end);
	}
	void player::find_other_player_info_feather_end(const s_item_info_db* feather_info)
	{
		ZoneScoped;
		faith::character_proto_find_other_unit_equiping_end find_other_unit_feather_end_msg;
		find_other_unit_feather_end_msg.set_target_guid(feather_info[0].role_guid.server_64);
		find_other_unit_feather_end_msg.set_is_finish(false);
		for (int32 index = 0; index < faith::e_feather_sub_type_feather4; index++)
		{
			if (!feather_info[index].data_info.item_guid.is_valid())
			{
				break;
			}

			faith::character_proto_find_other_unit_item_one* find_other_unit_feather_one_msg = find_other_unit_feather_end_msg.add_equiping_info();
			find_other_unit_feather_one_msg->set_item_guid(feather_info[index].data_info.item_guid.server_64);
			for (int32 i = 0; i < e_item_info_max; i++)
			{
				find_other_unit_feather_one_msg->add_data_array(feather_info[index].data_info.data_ary[i]);
			}

			int32 pak_size = find_other_unit_feather_end_msg.ByteSize();
			if (pak_size > packege_length_max)
			{
				send_message_to_self(&find_other_unit_feather_end_msg, e_msgindex_s2c_find_other_unit_feather_end);
				find_other_unit_feather_end_msg.clear_equiping_info();
			}
		}
		find_other_unit_feather_end_msg.set_is_finish(true);
		send_message_to_self(&find_other_unit_feather_end_msg, e_msgindex_s2c_find_other_unit_feather_end);
	}


	void player::set_role_right(e_role_right role_right_type, int32 type_value)
	{
		ZoneScoped;
		if (role_right_type >= e_role_right_max)
		{
			return;
		}
		m_role_right.data_ary[role_right_type] = type_value;
	}
	int32 player::get_role_right(e_role_right role_right_type)
	{
		ZoneScoped;
		if (role_right_type >= e_role_right_max)
		{
			return 0;
		}
		return m_role_right.data_ary[role_right_type];
	}

	void player::aoi_change_show_player(int32 show_num)
	{
		ZoneScoped;
		if (show_num <0 || show_num > role_watch_unit_num)
		{
			return;
		}
		set_unit_info(e_role_info_watch_count, show_num);
	}

	void player::reconnect_send_red_package_info()
	{
		ZoneScoped;
		cs2ws_reconnect_send_red_package_info msg;
		msg.role_guid = get_unit_guid();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_reconnect_send_red_package_info));
	}

	void player::send_red_package(int32 template_id, xstring send_text, guid_64 item_guid)
	{
		ZoneScoped;
		//ֻҪ���ɹ���ֱ�ӷ��ؾ��У���Ϊ�ж϶��ڿͻ���������

		character_proto_send_red_package_end msg;
		msg.set_red_package_id(template_id);
		RedPackageTemplate* red_template_ptr = GET_TEMPLATE(RedPackageTemplate, template_id);
		if (red_template_ptr == nullptr)
		{
			return;
		}
		if (strlen(send_text.c_str()) <= 0)
		{
			send_text = template_manager::get_instance().get_str_by_string_template_id(GAMECONFIG->RedPackageDefaultTextId[red_template_ptr->Type]);
		}
		if (strlen(send_text.c_str()) > red_package_max_text_length)
		{
			xstring temp_text = send_text.substr(0, red_package_max_text_length);
			send_text = temp_text;
		}
		bool is_free = false;
		if (red_template_ptr->CostMoneyArray.empty())
		{
			is_free = true;
		}
		else if (red_template_ptr->CostMoneyArray.size() < 2)
		{
			return;
		}
		if (!is_free)
		{
			int32 cost_money_type = red_template_ptr->CostMoneyArray[0];
			int32 cost_money_value = red_template_ptr->CostMoneyArray[1];
			if (cost_money_value == 0 && false == item_guid.is_valid())
			{
				return;
			}
			else if (item_guid.is_valid())
			{
				if (cost_money_value > 0)
				{
					return;
				}
				citem* item_ptr = get_item_set().get_item(item_guid);
				if (nullptr == item_ptr)
				{
					return;
				}
				int32 item_id = item_ptr->get_item_template_id();
				if (red_template_ptr->LogicID != item_id)
				{
					return;
				}
				int32 item_slot = item_ptr->get_data_info(e_item_info_slot);
				get_item_set().del_item(e_server_log_del_item_red_package, template_id, item_guid, 1);
				item_proto_item_operation_end item_msg;

				item_msg.set_operation_state(1);
				item_msg.set_operation_type(e_item_operation_use);
				item_msg.set_item_slot(item_slot);

				item_msg.add_role_guid(get_unit_guid().A);
				item_msg.add_role_guid(get_unit_guid().B);
				item_msg.add_item_guid(item_guid.A);
				item_msg.add_item_guid(item_guid.B);
				send_message_to_self(&item_msg, e_msgindex_s2c_item_operation);
			}
			if (!can_cut_money(e_money_type(cost_money_type), cost_money_value))
			{
				return;
			}
			//�˴���Ǯ�����ws�ж�ʧ�ܣ������ʼ����ӻ���
			cut_money(e_money_type(cost_money_type), cost_money_value, e_server_log_cut_money_red_package);
		}
		else
		{
			int32 sender_vip_level = get_vip_level();
			int32 cur_free_num = get_unit_info(e_role_info_free_red_package_send_times);
			VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(sender_vip_level);
			if (vip_template_ptr == nullptr)
			{
				return;
			}
			if (cur_free_num >= vip_template_ptr->FreeRedPackageNum)
			{
				return;
			}

			set_unit_info(e_role_info_free_red_package_send_times, cur_free_num + 1);
			send_info_one(e_role_info_free_red_package_send_times);
		}
		cs2ws_send_red_package send_msg;
		send_msg.sender_guid = get_unit_guid();
		memcpy(send_msg.send_text, send_text.c_str(), send_text.size());
		memcpy(send_msg.sender_name, get_name(), max_name_size);
		send_msg.red_package_template_id = template_id;

		bool is_use_lua = proto_by_lua(e_msgindex_cs2ws_send_red_pack);
		if (!is_use_lua)
		{
			connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(cs2ws_send_red_package));
		}
		else
		{
			faith::cs2ws_proto::send_red_package pro_msg;

			send_msg.to_proto(pro_msg);

			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msgindex_cs2ws_send_red_pack, 0);
		}




		//�˴�������ʾ�ɹ�����ΪWS�˻����ܳ������⣬�����ܺ��������
	}

	void player::send_red_package_end_proc(int32 template_id, int32 send_result)
	{
		ZoneScoped;
		RedPackageTemplate* template_ptr = GET_TEMPLATE(RedPackageTemplate, template_id);
		if (template_ptr == nullptr)
		{
			return;
		}
		if (send_result != e_red_bag_send_success &&
			send_result != e_red_bag_get_globel_success &&
			send_result != e_red_bag_get_legion_success &&
			send_result != e_red_bag_get_vip_success &&
			template_ptr->CostMoneyArray.size() == 0)
		{//���ɹ� ��ѵ� �ӻ���
			int32 cur_free_num = get_unit_info(e_role_info_free_red_package_send_times);
			if (cur_free_num > 0)
			{
				set_unit_info(e_role_info_free_red_package_send_times, cur_free_num - 1);
				send_info_one(e_role_info_free_red_package_send_times);
			}
		}
		character_proto_send_red_package_end end_msg;
		end_msg.set_red_package_id(template_id);
		end_msg.set_send_result(send_result);
		send_message_to_self(&end_msg, e_msgindex_s2c_send_red_package_end);
	}

	void player::get_red_package(guid_64 red_package_guid)
	{
		ZoneScoped;
		if (!red_package_guid.is_valid())
		{
			return;
		}
		VIPTemplate* template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
		if (template_ptr == nullptr)
		{
			return;
		}
		if (template_ptr->RedPackageGetTimes >= 0)
		{
			int32 cur_get_times = get_unit_info(e_role_info_red_package_geted_times);
			if (cur_get_times >= template_ptr->RedPackageGetTimes)
			{
				return;
			}
		}

		cs2ws_get_red_package msg;
		msg.red_package_guid = red_package_guid;
		msg.role_guid = get_unit_guid();
		memcpy(msg.role_name, get_name(), max_name_size);

		bool is_use_lua = proto_by_lua(e_msgindex_cs2ws_get_red_pack);
		if (!is_use_lua)
		{
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_get_red_package));
		}
		else
		{
			faith::cs2ws_proto::get_red_package pro_msg;

			msg.to_proto(pro_msg);

			connection_mgr::getInstance().send_to_ws_lua(&pro_msg, e_msgindex_cs2ws_get_red_pack, 0);
		}

	}

	void player::get_red_pack_end(guid_64 red_package_guid, int32 get_result, int32 money_type, int32 money_num)
	{
		ZoneScoped;
		int64 money_real_num = money_num;
		if (get_result == e_red_bag_get_success)
		{
			add_money_or_exp(e_money_type(money_type), money_real_num, e_server_log_add_money_red_package);

			int32 cur_geted_times = get_unit_info(e_role_info_red_package_geted_times);
			set_unit_info(e_role_info_red_package_geted_times, cur_geted_times + 1);
			send_info_one(e_role_info_red_package_geted_times);
		}
		character_proto_get_red_package_end msg;
		msg.add_red_pack_guid(red_package_guid.A);
		msg.add_red_pack_guid(red_package_guid.B);
		msg.set_get_result(get_result);
		msg.set_money_type(money_type);
		msg.set_money_num(money_real_num);
		send_message_to_self(&msg, e_msgindex_s2c_get_red_package_end);
	}

	void player::transfer_buff(int32 operate_type)
	{
		ZoneScoped;
		if (operate_type == 1)
		{
			//���Ӵ�����Ч
			buff_man::add_buff_inst(m_array_index, m_array_index, transfer_buff_id);
		}
		else if (operate_type == 2)
		{
			//ɾ��������Ч
			buff_man::del_buff_by_template_id(m_array_index, m_array_index, transfer_buff_id);
		}
	}

	void player::send_role_info_to_gm()
	{
		ZoneScoped;
		faith::cs2ws_send_role_info_to_gm ret;
		ret.client_uid = get_client_uid();
		ret.role_info = get_unit_info_inst();
		connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
	}

	void player::send_stat_num_award(int32 drop_id)
	{
		ZoneScoped;
		std::vector<s_item_template_info> drop_item_list;
		cdrop::gen_drop_list_by_drop_id(drop_id, drop_item_list, get_unit_info(e_role_info_class_type), get_unit_info(e_role_info_exp_level));
		if (drop_item_list.size() <= 0)
		{
			return;
		}
		std::vector<int32> item_arr;
		for (int32 i = 0; i < drop_item_list.size(); ++i)
		{
			item_arr.push_back(drop_item_list[i].m_item_id);
			item_arr.push_back(drop_item_list[i].m_item_num);
		}
		get_item_set().put_in_bag(e_server_log_add_item_stat_num, drop_id, item_arr);
	}

	void player::send_oracle_trial_fuben_over_award(int32 map_id, int32 drop_id, int32 rank_num)
	{
		ZoneScoped;
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_id);
		if (map_template_ptr == nullptr)
		{
			return;
		}
		if (rank_num > 3)
		{
			return;
		}
		int32 map_name_id = map_template_ptr->MapName;
		std::vector<s_item_template_info> drop_item_list;
		cdrop::gen_drop_list_by_drop_id(drop_id, drop_item_list, get_unit_info(e_role_info_class_type), get_unit_info(e_role_info_exp_level));
		std::vector<std::string> content_params_title;
		content_params_title.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_oracle_trial_funben_over_title));
		std::string title = init_unit::implode(content_params_title);

		std::vector<std::string> content_params_contenttext;
		content_params_contenttext.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text(e_mail_common_text_oracle_trial_funben_over_content_one + rank_num - 1)));
		content_params_contenttext.push_back(template_manager::get_instance().int_to_string(map_name_id));
		std::string contenttext = init_unit::implode(content_params_contenttext);

		globle_data::get_instance().send_mail_with_item(get_unit_guid(), get_unit_info(e_role_info_server_id), drop_item_list, title, contenttext);
	}

	void player::set_oracle_trial_info(int32 tier_num, int32 customs_num, int32 customs_state)
	{
		ZoneScoped;
		m_oracle_trial_mgr.set_play_oracle_trial_info(tier_num, customs_num, customs_state);
	}

	void player::lua_oracle_trial_map_game_over(int32 map_template_id, int32 customs_state)
	{
		ZoneScoped;
		script_mgr::get_instance().call_func("oracle_trial_mgr", "oracle_trial_map_game_over", 0, false, "%d%d%d", m_array_index, map_template_id, customs_state);
	}

	bool player::aoi_watch_all_data_in(int32 unit_array_index)
	{
		ZoneScoped;
		if (unit_array_index >= npc_arrary_index_begin)
		{
			return true;
		}
		if (m_player_aoi_watch.can_join_aoi(unit_array_index))
		{
			m_player_aoi_watch.add_aoi_index(unit_array_index);
			return true;
		}
		return false;
	}
	void player::aoi_watch_all_data_out(int32 unit_array_index)
	{
		ZoneScoped;
		if (m_interaction_mgr.get_interaction_guid().is_valid())
		{
			player& target_player_ref = unit_man::get_player(unit_array_index);
			if (m_interaction_mgr.get_interaction_guid() == target_player_ref.get_unit_guid())
			{
				m_interaction_mgr.stop_interaction();
			}
		}

		if (unit_array_index <= 0 || unit_array_index >= init_player_max)
		{
			return;
		}
		m_player_aoi_watch.del_aoi_index(unit_array_index);
	}
	bool player::aoi_is_watch_all_data(int32 unit_array_index)
	{
		ZoneScoped;
		if (unit_array_index <= 0 || unit_array_index >= init_player_max)
		{
			npc& npc_ref = unit_man::get_npc(unit_array_index);
			if (npc_ref.get_unit_type() == e_unit_type_drop_bag && npc_ref.get_owner() != get_identifier())
			{//���˵ĵ�������Լ����ÿ���
				return false;
			}
			return true;
		}
		return m_player_aoi_watch.is_in_aoi(unit_array_index);
	}
	bool player::is_recharge_by_type(e_recharge_type recharge_type)
	{
		ZoneScoped;
		bool result = false;
		switch (recharge_type)
		{
		case faith::e_recharge_type_none:
			break;
		case faith::e_recharge_type_vip:
			if (0 < get_vip_level())
			{
				result = true;
			}
			break;
		case faith::e_recharge_type_month_card:
			if (0 < get_time_data(e_time_type_month_card_activity_time))
			{
				result = true;
			}
			break;
		case faith::e_recharge_type_exclusive:
			if (0 < get_unit_info(e_role_info_exclusive_flag))
			{
				result = true;
			}
			break;
		case faith::e_recharge_type_max:
			break;
		default:
			break;
		}
		return result;
	}

	int32 player::get_month_card_current_activity_time()
	{
		ZoneScoped;
		return get_time_data(e_time_type_month_card_activity_time);
	}

	int32 player::get_month_card_remain_time()
	{
		ZoneScoped;
		return get_time_data(e_time_type_month_card_remain_time);
	}

	int32 player::get_exclusive_card_current_activity_time()
	{
		ZoneScoped;
		return get_time_data(e_time_type_exclusive_card_activity_time);
	}

	int32 player::get_vip_extra_raid_enter(int32 map_template_id)
	{
		ZoneScoped;
		const VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level());
		if (nullptr == vip_template_ptr)
		{
			return 0;
		}

		const MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr || map_template_ptr->HaveExtraEnterWay <= 0)
		{
			return 0;
		}

		int32 ret_extra_cnt = 0;
		const std::vector<int32>& extra_raid_vip_cfg = vip_template_ptr->RaidMapEnter;
		int32 cfg_size = extra_raid_vip_cfg.size();
		int32 group_map_temp_id = 0;
		group_map_temp_id = init_unit::get_map_id_by_diff_level(map_template_id, map_template_ptr->Type, map_template_ptr->GroupType);
		for (int32 cfg_idx = 0; cfg_idx < cfg_size - 1; cfg_idx += 2)
		{
			int32 vip_extra_map_id = extra_raid_vip_cfg[cfg_idx];
			int32 vip_extra_cnt = extra_raid_vip_cfg[cfg_idx + 1];
			if (vip_extra_map_id == group_map_temp_id)
			{
				ret_extra_cnt += vip_extra_cnt;
				break;
			}
		}

		if (map_template_ptr->ExtraEnterCount > 0)
		{
			ret_extra_cnt += map_template_ptr->ExtraEnterCount;
		}

		if (ret_extra_cnt > max_extra_enter_map_count)
		{
			ret_extra_cnt = max_extra_enter_map_count;
		}

		//map_record_set& player_map_record_set = map_record_mgr::get_instance().get_map_record_set(get_array_index());
		//if (player_map_record_set.is_valid() == false)
		//{
		//	return 0;
		//}
		//if (map_template_ptr->Type == e_map_type_battle && map_template_ptr->SubType == 1)
		//{
		//	int32 drama_ex_common_times = player_map_record_set.get_drama_raid_common_extra_times();
		//	ret_extra_cnt += vip_template_ptr->DramaRaidCommonEnter;
		//	ret_extra_cnt = ret_extra_cnt > max_extra_enter_map_count ? max_extra_enter_map_count : ret_extra_cnt;
		//	ret_extra_cnt -= drama_ex_common_times;
		//}
		//else
		//{
		//	int32 extra_enter_times = player_map_record_set.get_extra_enter_times(map_template_id);
		//	ret_extra_cnt -= extra_enter_times;
		//}
		return ret_extra_cnt;
	}

	int32 player::get_vip_level(bool is_check_experience_level)
	{
		ZoneScoped;
		int32 return_value = get_unit_info(e_role_info_vip_level);
		if (is_check_experience_level
			&& get_unit_info(e_role_info_vip_experience_level) > return_value
			&& get_time_data(e_time_type_vip_experience_level_time) > time_helper::get_cur_time_new().second)
		{
			return_value = get_unit_info(e_role_info_vip_experience_level);
		}
		return return_value;
	}

	void player::buy_message_tip(int32 type, int32 level /* = 0 */)
	{
		ZoneScoped;
		if (type == e_buy_tip_type_money) //�����Ͳ���Ҫ
		{
			return;
		}
		if (type == e_buy_tip_type_fuben && level == 2)
		{
			return;
		}
		if (type == e_buy_tip_type_money && get_money_data(e_money_type_jewel_bind) >= 200)
		{
			return;
		}
		int64 create_time = get_unit_i64_info_data(e_role_i64_info_create_time);

		if (time_is_same_day(create_time))
		{
			return;
		}
		int64 last_time = get_time_data(e_time_type_fuben_level_buy_tip_time);
		if (type == e_buy_tip_type_money)
		{
			last_time = get_time_data(e_time_type_money_buy_tip_time);
		}
		if (time_is_same_day(last_time))
		{
			return;
		}
		int32 tip_type = -1;
		if (type == e_buy_tip_type_fuben)
		{
			if (get_unit_info(e_role_info_real_recharge_jewel_num) == 0)
			{
				tip_type = e_login_promptbox_type_firstflush;

			}
			else if (get_time_data(e_time_type_month_card_activity_time) == 0)
			{
				tip_type = e_login_promptbox_type_monthlycard;
			}
			else if (get_unit_info(e_role_info_exclusive_flag) != 1)
			{
				tip_type = e_login_promptbox_type_exclusive;

			}
			else if (get_unit_info(e_role_info_fund_payment_flag) == 0)
			{
				tip_type = e_login_promptbox_type_fund;
			}
			set_time_data(e_time_type_fuben_level_buy_tip_time, time_helper::get_cur_time_new().second);
		}
		else
		{
			if (get_time_data(e_time_type_month_card_activity_time) == 0)
			{
				tip_type = e_login_promptbox_type_monthlycard;
			}
			else if (get_unit_info(e_role_info_exclusive_flag) != 1)
			{
				tip_type = e_login_promptbox_type_exclusive;

			}
			else if (get_unit_info(e_role_info_fund_payment_flag) == 0)
			{
				tip_type = e_login_promptbox_type_fund;
			}
			set_time_data(e_time_type_money_buy_tip_time, time_helper::get_cur_time_new().second);

		}
		if (tip_type == -1)
		{
			return;
		}
		character_proto_buy_tip msg;

		msg.set_tip_type(tip_type);

		send_message_to_self(&msg, e_msgindex_s2c_buy_tip);

	}

	bool player::time_is_same_day(int64 in_time_stamp)
	{
		ZoneScoped;
		time_info in_time_info = time_helper::get_time_by_stamp_new(in_time_stamp);
		time_info cur_time_info = time_helper::get_cur_time_new();
		if (in_time_info.year == cur_time_info.year
			&& in_time_info.month_in_year == cur_time_info.month_in_year
			&& in_time_info.day_in_month == cur_time_info.day_in_month)
		{
			return true;
		}
		return false;

	}

	void player::req_hit_and_get_npc_proc(guid_64 npc_guid)
	{
		ZoneScoped;
		npc& target_npc = unit_man::get_npc(npc_guid);
		if (!target_npc.is_valid())
		{
			return;
		}

		fvector target_pos = target_npc.get_new_map_pos().unit_location;
		fvector self_pos = get_new_map_pos().unit_location;

		int64 cur_dis = init_unit::get_distance(self_pos, target_pos);
		if (cur_dis > max_hit_and_get_npc_dis_power)
		{//�������500 ����̫��ȷ
			return;
		}
		finish_interact_with_npc(&target_npc);
	}

	void player::record_server_crystal_log()
	{
		ZoneScoped;
		//auto map_obj_ptr = world_cs::get_map_by_guid<map_cs>(get_map_guid());
		//if (nullptr == map_obj_ptr)
		//{
		//	return;
		//}
		//map_obj_ptr->record_server_crystal_log(get_array_index());
	}

	int32 player::get_pokedex_finished_num()
	{
		ZoneScoped;
		return get_pokedex_mgr().get_active_pokedex_num();
	}

	void player::load_history_highest_record(const s_role_history_high_record_info& highest_record_info)
	{
		ZoneScoped;
		m_history_high_record = highest_record_info;
		send_history_highest_record_all();
	}
	bool player::load_history_highest_record_by_db_lua(const char* data_ptr, int32 data_len)
	{
		ZoneScoped;
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_highest_record_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_role_history_high_record_info db_data;
		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}

		load_history_highest_record(db_data);

		return true;


	}
	void player::send_history_highest_record_all()
	{
		ZoneScoped;
		//�����ֹ�ͻ���loading����ס ֻ���Ͳ�����ʷ��߼�¼ ʣ�µĵ���
		character_proto_update_highest_record_all msg;
		for (int32 i = 0; i < e_role_history_highest_record_max; ++i)
		{
			msg.add_info_array(init_unit::change_i64_to_string(m_history_high_record.data_ary[i]));
		}
		send_message_to_self(&msg, e_msgindex_s2c_send_highest_record_all);
		msg.Clear();
		for (int32 i = 0; i < e_role_history_highest_record_gs_value; ++i)
		{
			msg.add_info_array(init_unit::change_i64_to_string(m_history_high_record.data_ary[i]));
		}
		send_message_to_self(&msg, e_msgindex_s2c_send_highest_record_all);

		for (int32 i = e_role_history_highest_record_gs_value; i < e_role_history_highest_record_max; i++)
		{
			send_history_highest_record_one(i);
		}
	}

	void player::send_history_highest_record_one(int32 info_index)
	{
		ZoneScoped;
		if (info_index < 0 || info_index >= e_role_history_highest_record_max)
		{
			return;
		}
		character_proto_update_highest_record_one msg;
		msg.set_info_index(info_index);
		msg.set_info_value(init_unit::change_i64_to_string(m_history_high_record.data_ary[info_index]));
		send_message_to_self(&msg, e_msgindex_s2c_send_highest_record_one);
	}

	void player::save_history_highest_record(int32 save_type_ex)
	{
		ZoneScoped;
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_hightest_record);
		if (is_use == false)
		{
			cs2dp_save_hightest_record req;
			req.role_guid = get_unit_guid();
			req.unit_array_index = get_array_index();
			req.save_type_ex = save_type_ex;
			req.highest_info = m_history_high_record;
			send_message_to_dp(&req, sizeof(cs2dp_save_hightest_record));
		}
		else
		{
			faith::cs2dp_proto::save_role_highest_record msg;
			msg.set_role_guid(get_unit_guid().server_64);
			msg.set_unit_array_index(get_array_index());
			msg.set_save_type_ex(save_type_ex);

			faith::cs2dp_proto::role_highest_record_db* db_data = msg.mutable_db_data();

			for (int32 i = 0; i < e_role_history_highest_record_max; i++)
			{
				db_data->add_data_ary(m_history_high_record.data_ary[i]);
			}
			send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_hightest_record);
		}
	}

	int64 player::get_highest_record(int32 index)
	{
		ZoneScoped;
		if (index < 0 || index >= e_role_history_highest_record_max)
		{
			return 0;
		}
		return m_history_high_record.data_ary[index];
	}

	void player::set_highest_record(int32 index, int64 value)
	{
		ZoneScoped;
		if (index < 0 || index >= e_role_history_highest_record_max)
		{
			return;
		}
		m_history_high_record.data_ary[index] = value;
	}

	bool player::check_and_set_highest_record(e_role_history_highest_record record_type, int64 single_type_target_level, int64 single_type_target_color)
	{
		ZoneScoped;
		if (record_type >= e_role_history_highest_record_max)
		{
			return false;
		}
		int64 old_value = m_history_high_record.data_ary[record_type];
		int64 new_value = get_new_highest_value(record_type, single_type_target_level, single_type_target_color);
		if (old_value < new_value)
		{
			set_highest_record(record_type, new_value);
			send_history_highest_record_one(record_type);
			return true;
		}
		return false;
	}

	void player::upgrade_highest_record(e_role_history_highest_record record_type, int64 new_value)
	{
		ZoneScoped;
		int64 old_value = get_highest_record((int32)record_type);
		if (new_value > old_value)
		{
			set_highest_record(record_type, new_value);
			send_history_highest_record_one(record_type);
			if (e_role_history_highest_record_higest_equip_upgrade_buff == record_type && new_value == first_upgrade_buff_need_level)
			{
				game_proto_pop_plot_tip pop_plot_tip;
				pop_plot_tip.set_str_id_type(1);
				pop_plot_tip.set_str_id(first_upgrade_buff_guide_id);
				send_message_to_self(&pop_plot_tip, e_msgindex_s2c_pop_plot_tip);
			}
		}
	}

	int64 player::get_new_highest_value(e_role_history_highest_record record_type, int64 single_type_target_level, int64 single_type_target_color)
	{
		ZoneScoped;
		int64 return_value = 0;
		if (record_type >= e_role_history_highest_record_max)
		{
			return return_value;
		}
		switch (record_type)
		{
		case faith::e_role_history_highest_record_single_equip_upgrade:
			return_value = single_type_target_level;
			break;
		case faith::e_role_history_highest_record_total_equip_upgrade_num:
		{
			for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index < e_role_equip_slot_amulet; ++temp_equip_index)
			{
				citem* temp_equip_ptr = get_item_set().get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
				if (temp_equip_ptr != nullptr)
				{
					return_value += temp_equip_ptr->get_data_info(e_item_info_upgrade_count);
				}
			}
		}
		break;
		case faith::e_role_history_highest_record_total_equip_addon_num:
		{
			for (int32 temp_equip_index = e_role_equip_slot_weapon_1; temp_equip_index < e_role_equip_slot_amulet; ++temp_equip_index)
			{
				citem* temp_equip_ptr = get_item_set().get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
				if (temp_equip_ptr != nullptr)
				{
					return_value += temp_equip_ptr->get_data_info(e_item_info_add_on);
				}
			}
		}
		break;
		case faith::e_role_history_highest_record_weapon_full_succinct_grade:
		{

			citem* temp_equip_ptr = get_item_set().get_equip_item_by_slot((e_role_equip_slot)e_role_equip_slot_weapon_1);
			if (temp_equip_ptr != nullptr)
			{
				//��ȡװ�����ϴ���ȼ�
				return_value = temp_equip_ptr->get_item_succinct_current_buff_level();
			}

		}
		break;
		case faith::e_role_history_highest_record_equip_full_succinct_grade:
		{
			int32 array_size = GAMECONFIG->SuccinctEquipBuffIdArray.size();
			if (array_size < 0)
			{
				return return_value;
			}
			//ȫ��װ��ϴ���ȼ� ���ﵽ
			int32 addon_lowest_level = 9999;				//�ȸ�һ���ܴ��ֵ
			for (int32 temp_equip_index = e_role_equip_slot_hat; temp_equip_index < e_role_equip_slot_amulet; ++temp_equip_index)
			{
				citem* temp_equip_ptr = get_item_set().get_equip_item_by_slot((e_role_equip_slot)temp_equip_index);
				if (temp_equip_ptr != nullptr)
				{
					if (addon_lowest_level >= temp_equip_ptr->get_data_info(e_item_info_succinct_level))
					{
						addon_lowest_level = temp_equip_ptr->get_data_info(e_item_info_succinct_level);
					}
				}
				else
				{
					addon_lowest_level = 0;
					break;
				}
			}
			return_value = addon_lowest_level;
		}
		break;
		case faith::e_role_history_highest_record_item_succinct_reach_mark_level:
		{
			for (int32 i = 0; i < succinct_mark_req_type_num; ++i)
			{
				if (single_type_target_level >= succinct_mark_req_grade[i]
					&& single_type_target_color >= succinct_mark_req_color[i])
				{//�������������������11��Ӧ��
					return_value++;
				}
				else
				{
					break;
				}
			}
		}
		break;
		case faith::e_role_history_highest_record_total_sprite_equip_level:
		{
			for (int32 i = 0; i < sprite_bag_num; ++i)
			{
				citem* equiping_spirit_ptr = get_item_set().get_item_by_slot(e_bag_type_equip_spirit, i);
				if (equiping_spirit_ptr != nullptr)
				{
					return_value += equiping_spirit_ptr->get_data_info(e_item_info_upgrade_count);
				}
			}
		}
		break;
		case faith::e_role_history_highest_record_total_element_heart_equip_level:
		{
			for (int32 i = 0; i < element_heart_equip_num; ++i)
			{
				citem* temp_element_heart = get_item_set().get_item_by_slot(e_bag_type_equip_elemenet, i);
				if (temp_element_heart != nullptr)
				{
					return_value += temp_element_heart->get_data_info(e_item_info_upgrade_count) + 1;
				}
			}
		}
		break;
		case faith::e_role_history_highest_record_demon_tower_highest_level:
		{
			return_value = single_type_target_level;
		}
		break;
		//case faith::e_role_history_highest_record_higest_weapon_upgrade:
		//{
		//	return_value = single_type_target_level;
		//}
		//break;

		case faith::e_role_history_highest_record_addon_equip:
		{
			return_value = init_unit::get_addon_phase_level(get_item_set().get_equip_smallest_addon_level());
		}
		break;
		case faith::e_role_history_highest_record_addon_weapon:
		{
			int32 array_size = GAMECONFIG->AddonBuffLevelArray.size();
			if (array_size < 0)
			{
				return return_value;
			}
			citem* temp_equip_ptr = get_item_set().get_equip_item_by_slot(e_role_equip_slot_weapon_1);
			if (temp_equip_ptr != nullptr)
			{
				for (int32 index = 0; index < array_size; index++)
				{
					if (temp_equip_ptr->get_data_info(e_item_info_add_on) >= GAMECONFIG->AddonBuffLevelArray[index])
					{
						return_value = GAMECONFIG->AddonBuffLevelArray[index];
					}
				}
			}
		}
		break;
		case faith::e_role_history_highest_record_rune_total_level:
		{
			int32 array_size = GAMECONFIG->RuneBuffLevelArray.size();
			if (array_size < 0)
			{
				return return_value;
			}
			//int32 rune_total_level = m_element_heart_mgr.get_rune_total_level();
			//for (int32 index = 0; index < array_size; index++)
			//{
			//	if (rune_total_level >= GAMECONFIG->RuneBuffLevelArray[index])
			//	{
			//		return_value = GAMECONFIG->RuneBuffLevelArray[index];
			//	}
			//}
		}
		break;
		case faith::e_role_history_highest_record_belief_spring:
		case faith::e_role_history_highest_record_belief_summer:
		case faith::e_role_history_highest_record_belief_autumn:
		case faith::e_role_history_highest_record_belief_winter:
		{
			for (int32 index = 0; index < e_belief_buff_level_max; index++)
			{
				if (single_type_target_level >= belief_buff_req_array[index])
				{
					return_value = belief_buff_req_array[index];
				}
			}
		}

		break;
		case faith::e_role_history_highest_record_gs_value:
		case faith::e_role_history_highest_record_mount_value:
		case faith::e_role_history_highest_record_wing_value:
		case faith::e_role_history_highest_record_equip_value:
		case faith::e_role_history_highest_record_spirit_value:
		{
			return_value = single_type_target_level;
		}
		break;
		case faith::e_role_history_highest_record_max_awaken_level:
		{
			return_value = get_item_set().get_awaken_fetter_max_num();
		}
		break;
		case faith::e_role_history_highest_record_max_forge_level:
		{
			return_value = get_item_set().get_equip_smallest_forge_level() - 1; //��ɫ�ǴӰ�=1��ʼ��
		}
		break;
		case faith::e_role_history_highest_record_max_equip_enchant_level:
		{
			return_value = get_item_set().get_equip_smallest_enchant_level();
		}
		break;
		case faith::e_role_history_highest_record_max_jewelry_enchant_level:
		{
			return_value = get_item_set().get_equip_smallest_enchant_level(true);
		}
		break;
		default:
			break;
		}

		return return_value;
	}

	void player::check_can_marry(const guid_64& sub_guid, int32 marry_item_id, bool is_cost)
	{
		ZoneScoped;
		faith::cs2ws_operate_result_end ret;
		ret.sub_guid = sub_guid;
		ret.is_cost_item = is_cost;
		ret.item_id = marry_item_id;
		ret.is_divorce = false;
		ret.main_guid = get_unit_guid();
		player& sub_player_ref = unit_man::get_player(sub_guid);
		if (false == sub_player_ref.is_valid())
		{
			ret.result_type = e_marry_operate_type_one_offline;
			connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
			return;
		}

		if (!(GAMECONFIG->MarryUnlockID < get_mission_id(e_mission_slot_main))
			|| !(GAMECONFIG->MarryUnlockID < sub_player_ref.get_mission_id(e_mission_slot_main)))
		{
			ret.result_type = e_marry_operate_type_mary_func_unlock;
			connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
			return;
		}
		citem* marry_item_ptr = get_item_set().get_item_by_template_id(marry_item_id);
		if (nullptr == marry_item_ptr)
		{
			ret.result_type = e_marry_operate_type_lack_marry_item;
			connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
			return;
		}
		else if (marry_item_ptr->get_data_info(e_item_info_stack_count) <= 0)
		{
			ret.result_type = e_marry_operate_type_lack_marry_item;
			connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
			return;
		}

		if (is_cost)
		{
			get_item_set().del_item(e_server_log_del_item_marry, 0, marry_item_ptr, 1);
		}
		ret.result_type = e_marry_operate_type_success;
		connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
		return;
	}

	void  player::check_can_divorce(bool is_cost)
	{
		ZoneScoped;
		faith::cs2ws_operate_result_end ret;
		ret.is_cost_item = is_cost;
		ret.is_divorce = true;

		if (GAMECONFIG->DivorceCostMoney.size() <= 0)
		{
			return;
		}

		int32 divorce_moeny_type = GAMECONFIG->DivorceCostMoney[0];
		int32 money_count = GAMECONFIG->DivorceCostMoney[1];
		if (is_cost)
		{
			if (!can_cut_money((e_money_type)divorce_moeny_type, money_count))
			{
				ret.result_type = e_marry_operate_type_divorce_lack_money;
			}
			else
			{
				ret.result_type = e_marry_operate_type_success;
				cut_money((e_money_type)divorce_moeny_type, money_count, e_server_log_cut_money_divorce);
			}
		}
		ret.item_id = divorce_moeny_type;
		ret.main_guid = get_unit_guid();
		connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
	}

	void player::clear_couple_info()
	{
		ZoneScoped;
		memset(m_couple_name, 0, sizeof(m_couple_name));
		m_couple_guid.clear_data();
	}

	void player::set_couple_name(const xchar* couple_name)
	{
		ZoneScoped;
		if (couple_name == nullptr)
		{
			return;
		}
		memset(m_couple_name, 0, sizeof(m_couple_name));
		memcpy(m_couple_name, couple_name, max_name_size);
	}


	void  player::get_new_married_reward_end(int32 old_marry_state)
	{
		ZoneScoped;
		ws2cs_marry_get_reward_end ret;
		ret.sender_guid = get_unit_guid();
		ret.old_marry_state = old_marry_state;

		int32 reward_id = GAMECONFIG->MarryRewardID;

		std::vector<int32> tmp_item;
		tmp_item.push_back(reward_id);
		tmp_item.push_back(1);
		if (true == get_item_set().put_in_bag(e_server_log_add_item_married_reward, old_marry_state, tmp_item))
		{
			ret.get_result = e_marry_operate_type_success;
		}
		else
		{
			ret.get_result = e_marry_operate_type_unknow_error;
		}

		connection_mgr::getInstance().send_to_ws(&ret, sizeof(ret));
	}

	void player::sync_married_info_guid_to_aoi(const guid_64& role_guid, const xchar* couple_name, bool is_marry)
	{
		ZoneScoped;
		faith::aoi_proto_marry_unit pak;
		pak.set_is_marry(is_marry);
		pak.set_unit_guid(role_guid.server_64);
		pak.set_couple_name(couple_name);
		send_message_to_aoi(&pak, e_msgindex_s2c_aoi_marry_unit);
	}

	void  player::handle_marry_result_func(const guid_64& couple_guid, const  xchar* couple_name, bool is_marry, int32 marry_item_id)
	{
		ZoneScoped;
		if (is_marry)
		{
			set_couple_guid(couple_guid);
			set_couple_name(couple_name);
			m_marry_mgr.activate_wedding_ring(marry_item_id);
		}
		else
		{
			clear_couple_info();
			m_marry_mgr.close_wedding_ring();
			get_mission_mgr().clear_marry_mission();
		}
		sync_married_info_guid_to_aoi(get_unit_guid(), couple_name, is_marry);
	}
	void player::sync_data_to_ws_lua(e_sync_cs2ws_data_type type, int32 data, int32 sub_data)
	{
		ZoneScoped;
		sync_data_to_ws(type, data, sub_data);
	}
	void player::sync_data_to_ws(e_sync_cs2ws_data_type type, int64 data, int32 sub_data)
	{
		ZoneScoped;
		if (type >= e_sync_cs2ws_data_max)
		{
			return;
		}
		if (is_valid() == false)
		{
			return;
		}

		cs2ws_sync_char_data req;
		req.role_guid = get_unit_guid();
		req.data_value = data;
		req.data_type = type;
		req.sub_data = sub_data;

		connection_mgr::getInstance().send_to_ws(&req, sizeof(cs2ws_sync_char_data));
	}

	void player::check_sync_gs_to_ws()
	{
		ZoneScoped;
		int64 new_gs = get_role_gs();
		if (new_gs == m_gs_last_sync_ws)
		{
			return;
		}
		m_gs_last_sync_ws = new_gs;
		sync_data_to_ws(e_sync_cs2ws_data_gs_value, new_gs, get_arena_mgr().current_rank());
	}

	int64 player::get_role_gs()
	{
		ZoneScoped;
		return get_unit_gs_value();
	}

	void player::refresh_service_goal(const e_service_goal_type service_goal_type)
	{
		ZoneScoped;
		m_service_goal_mgr.refresh_service_goal(service_goal_type);
		m_seven_day_goal_mgr.refresh_seven_day_goal(service_goal_type);
	}

	s_fake_player_info player::get_play_fake_player_info()
	{
		ZoneScoped;
		s_fake_player_info play_fake_info;
		play_fake_info.guid = guid_gen::make_guid();

		//��ʼ��������Ϣ
		play_fake_info.role_info = get_unit_info_inst();
		play_fake_info.att_info = get_pawn_att().get_unit_base_att_inst();

		//��ʼ��װ����Ϣ
		skill_set& temp_skill_set = skill_manager::get_skill_set(m_array_index);
		play_fake_info.skill_num = temp_skill_set.get_skill_num();
		temp_skill_set.set_fake_skill_info(play_fake_info.skill_data, MAX_SKILL_NUM);

		//��ʼ��װ����Ϣ
		play_fake_info.item_num = get_item_set().init_fake_player_item_info(play_fake_info.item_data, fake_item_num);

		//��ʼ��������Ϣ
		//citem* temp_spirit_info = get_spirit_mgr().get_fighting_spirit();
		//if (nullptr != temp_spirit_info)
		//{
		//	play_fake_info.spirit_data = get_spirit_mgr().make_fake_spirit_info(temp_spirit_info);
		//	play_fake_info.spirit_data.spirit_guid = guid_gen::make_guid();
		//}

		//��ʼ��Ů����Ϣ
		play_fake_info.belief_num = 0;
		cpatron_saint_mgr& patron_saint_mgr = get_patron_saint_mgr();
		for (int32 belief_type = 0; belief_type < e_belief_series_type_max; belief_type++)
		{
			cbelief* temp_belief = m_belief_mgr.getbelief(belief_type);
			if (temp_belief == nullptr)
			{
				continue;
			}
			play_fake_info.belief_data[belief_type] = temp_belief->get_belief_inst();
			play_fake_info.belief_num++;
		}

		//��ʼ��ս������
		play_fake_info.m_fight_att = get_pawn_att().get_unit_fight_att_inst();

		//��ʼ��buff��Ϣ

		int32 array_index = get_array_index();
		buff_set& buff_set_ref = buff_man::get_buff_set_by_index(array_index);
		play_fake_info.buff_num = buff_set_ref.init_fake_buff_info(play_fake_info.buff_data, MAN_UNIT_BUFF_INST);

		play_fake_info.all_data_block_masked();
		return play_fake_info;
	}

	void player::grade_up_level_ani_end(int32 old_buff_id, int32 new_buff_id)
	{
		ZoneScoped;
		character_proto_grade_up_level_ani_finish msg;
		msg.set_role_guid(get_unit_guid().server_64);
		msg.set_old_equip_buff_id(old_buff_id);
		msg.set_new_equip_buff_id(new_buff_id);
		send_message_to_aoi(&msg, e_msgindex_s2c_grade_up_level_ani_finish);
		//unit_arra
		//buff_man::del_buff_by_template_id(m_array_index, m_array_index, old_buff_id);
		buff_man::add_buff_inst(m_array_index, m_array_index, new_buff_id);
		//get_item_set().equip_off_show_buff();
	}

	int32 player::get_base_exp_of_dead_npc(npc& dead_npc_ref)
	{
		ZoneScoped;
		NpcTemplate* dead_npc_template = dead_npc_ref.get_npc_template();
		if (nullptr == dead_npc_template)
		{
			return 0;
		}
		int32 npc_type = dead_npc_template->NpcType;
		if (npc_type == e_unit_type_summoned)
		{
			unit& unit_ref = unit_man::get_unit(dead_npc_ref.get_owner());
			if (unit_ref.is_valid())
			{
				npc_type = unit_ref.get_unit_type();
			}
		}
		if (npc_type != e_unit_type_monster)
		{
			return 0;
		}

		int32 upgrade_template_id = dead_npc_ref.get_unit_info(e_role_info_upgrade_id);
		if (dead_npc_template->DeadExpMode == e_kill_npc_exp_mode_player_lv)
		{
			upgrade_template_id = get_unit_info(e_role_info_upgrade_id);
		}

		PlayerUpgradeTemplate* player_upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);
		if (nullptr == player_upgrade_template_ptr)
		{
			return 0;
		}
		int64 base_exp = player_upgrade_template_ptr->BaseExp;
		if (FLOAT_IS_ZERO(dead_npc_template->DeadExp) == false)
		{
			base_exp *= dead_npc_template->DeadExp;
		}
		base_exp += dead_npc_template->DeadExpPlus;
		return base_exp;
	}

	void player::save_daily_must_do_count(int32 save_typ)
	{
		ZoneScoped;
		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_daily_must_do_count);
		if (is_use == false)
		{
			cs2dp_save_character_daily_must_do_count req;
			req.save_type_ex = save_typ;
			req.data_info = m_daily_must_do_count;
			req.role_guid = get_unit_guid();
			req.unit_array_index = m_array_index;
			send_message_to_dp(&req, sizeof(cs2dp_save_character_daily_must_do_count));
		}
		else
		{
			faith::cs2dp_proto::save_role_daily_must_do_count msg;
			msg.set_role_guid(get_unit_guid().server_64);
			msg.set_unit_array_index(m_array_index);
			msg.set_save_type_ex(save_typ);

			faith::cs2dp_proto::role_daily_must_do_count_db* db_data = msg.mutable_db_data();

			for (int32 i = 0; i < e_daily_must_do_typ_max; i++)
			{
				db_data->add_data_ary(m_daily_must_do_count.data_ary[i]);
			}
			send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_daily_must_do_count);
		}
	}

	void player::load_daily_must_do_count(s_role_daily_must_do_count data_info)
	{
		ZoneScoped;
		m_daily_must_do_count = data_info;

		send_daily_must_do_count_info_all();
		set_loading_flag(e_data_flag_daily_must_do_count);
	}
	bool player::load_daily_must_do_count_by_db_lua(int64 role_id, const char* data_ptr, int32 data_len)
	{
		ZoneScoped;
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_daily_must_do_count_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}


		s_role_daily_must_do_count db_data;
		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}
		db_data.role_guid.server_64 = role_id;
		load_daily_must_do_count(db_data);

		return true;

	}

	void player::updata_daily_must_do_count()
	{
		ZoneScoped;
		for (int i = 0; i < e_daily_must_do_typ_max; i++)
		{
			m_daily_must_do_count.data_ary[i] = 0;
		}
		unit::send_daily_must_do_count_info_all();
	}

	void player::drop_item_send_notice(const std::vector<s_item_info>& item_data, int32 drop_npc_temp_id)
	{
		ZoneScoped;
		ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, item_data[0].data_ary[e_item_info_info_id]);
		if (nullptr == item_template_ptr)
		{
			return;
		}

		if (item_template_ptr->item_color < faith::e_item_color_orange)
		{
			return;
		}
		//if (item_template_ptr->item_color < faith::e_item_color_orange && item_template_ptr->item_type != 1 && item_template_ptr->item_type != 2)
		//{
		//	return;
		//}

		NpcTemplate* npc_template_ptr = GET_TEMPLATE(NpcTemplate, drop_npc_temp_id);
		if (npc_template_ptr == nullptr)
		{
			return;
		}
		std::string npc_name = "";				// npc����
		if (!template_manager::get_instance().get_str_by_attribute_id(npc_name, npc_template_ptr->NpcName))
		{
			return;
		}
		int32 notice_id = npc_template_ptr->Noticeld;
		s_item_info item_data_swp = item_data[0];  //������Ʒ��Ϣ�ṹ
		std::vector<std::string> vec_notice_str;
		if (notice_id <= 0)
		{
			return;
		}
		if (notice_id_boss == notice_id) //��ɱBOSS �� ��Ӣ�����ʰȡ�Ĺ���
		{
			if (get_legion_guid().is_valid())
			{
				notice_id = notice_id_boss_add_legion;
			}
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(npc_name);
			if (get_legion_guid().is_valid())
			{
				vec_notice_str.push_back(get_legion_info().legion_name);
			}
			vec_notice_str.push_back(get_name());
			vec_notice_str.push_back("%d");
		}
		else if (notice_id_curse == notice_id)			// ����������
		{
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(get_name());
			vec_notice_str.push_back("%d");
		}
		else if (notice_id_legion_boss == notice_id)	//	��Ծ���boss
		{
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(npc_name);
			vec_notice_str.push_back(get_name());
			vec_notice_str.push_back("%d");
		}
		else if (notice_id_gain_treasure_boss_set_top == notice_id)
		{
			if (item_template_ptr->DropRecordModel == e_drop_record_model_type_record)
			{
				notice_id = notice_id_gain_treasure_boss_record;
			}
			else if (item_template_ptr->DropRecordModel == e_drop_record_model_type_set_top)
			{
				//����Ҫ��id
			}
			else
			{
				//����¼Ҳ������
				//return;
			}
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(get_name());
			vec_notice_str.push_back(npc_name);
			vec_notice_str.push_back("%d");
		}
		else if (notice_id_boss_island_record == notice_id)
		{
			xchar* server_name = cell_server::getInstance().get_server_name(get_unit_info(e_role_info_server_id));
			if (server_name == nullptr)
			{
				return;
			}
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(server_name);
			vec_notice_str.push_back(get_name());
			vec_notice_str.push_back(npc_name);
			vec_notice_str.push_back("%d");
		}
		else if (notice_id_cross_boss_award_id_not_legion == notice_id)
		{
			if (get_legion_guid().is_valid())
			{
				notice_id = cross_boss_be_kill_have_legion_notice_id;
			}
			xchar* server_name = cell_server::getInstance().get_server_name(get_unit_info(e_role_info_server_id));
			if (server_name == nullptr)
			{
				return;
			}
			vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
			vec_notice_str.push_back(server_name);
			if (get_legion_guid().is_valid())
			{
				vec_notice_str.push_back(get_legion_info().legion_name);
			}
			vec_notice_str.push_back(get_name());
			vec_notice_str.push_back(npc_name);
			vec_notice_str.push_back("%d");
			std::string notice_str = init_unit::implode(vec_notice_str);
			cs2ws_send_kill_boss_notic msg;
			msg.item_info_ptr.notice_id = notice_id;
			msg.item_info_ptr.item_num = 0;
			msg.item_info_ptr.boss_template = drop_npc_temp_id;
			msg.item_info_ptr.data_ary[0] = item_data[0];
			memcpy(msg.item_info_ptr.notice_string, notice_str.c_str(), notice_str.length());
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
			return;
		}
		std::string notice_str = init_unit::implode(vec_notice_str);
		get_chat_mgr().send_notice(notice_id, notice_str, &item_data_swp, 0);
	}
	void player::treasure_prize(int32 treasure_type, int32 prize_type, int32 cost_money_type, int32 cost_money_value, int32 prize_score, int32 prize_luck, int32 item_id, int32 item_num, int32 prize_list[treasure_prize_num])
	{
		ZoneScoped;
		m_treasure_mgr.treasure_prize(treasure_type, prize_type, cost_money_type, cost_money_value, prize_score, prize_luck, item_id, item_num, prize_list);
	}

	bool player::buy_count_with_map_id(int32 map_temp_id, int32 buy_count)
	{
		ZoneScoped;
		int32 remain_count = get_remain_buy_count_with_map_id(map_temp_id);
		if (remain_count <= 0)
		{
			return false;
		}
		int32 cur_buy_count = get_buy_count_with_map_id(map_temp_id);
		int32 player_vip_level = get_vip_level();
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);
		if (nullptr == map_temp_ptr)
		{
			return false;
		}
		//if (false == init_unit::get_map_public(map_template_ptr->Type))//�����в������������
		//{
		//	send_notice("90300078");
		//	return false;
		//}
		std::vector<int32> vip_buy_count_arr = map_temp_ptr->ExtraBuyCount;
		int32 vip_buy_count_arr_size = vip_buy_count_arr.size();
		if (vip_buy_count_arr_size < (player_vip_level + 1))
		{
			return false;
		}
		if (vip_buy_count_arr[player_vip_level] < cur_buy_count + buy_count)
		{
			return false;
		}

		for (int32 i = cur_buy_count; i < cur_buy_count + buy_count; i++)
		{
			std::vector<int32>vip_buy_count_need_money_arr = globle_data::get_instance().get_extra_buy_money_arr(map_temp_id);	//map_temp_ptr->ExtraBuyCountNeedMoney;
			if (vip_buy_count_need_money_arr.size() < (2 * (vip_buy_count_arr[vip_buy_count_arr_size - 1])))
			{
				return false;
			}
			if (vip_buy_count_need_money_arr.size() <= (i * 2 + 1))
			{
				return false;
			}
			if (false == can_cut_money((e_money_type)(vip_buy_count_need_money_arr[i * 2]), vip_buy_count_need_money_arr[i * 2 + 1]))
			{
				return false;
			}
		}

		for (int32 i = cur_buy_count; i < cur_buy_count + buy_count; i++)
		{
			std::vector<int32>vip_buy_count_need_money_arr = globle_data::get_instance().get_extra_buy_money_arr(map_temp_id); //map_temp_ptr->ExtraBuyCountNeedMoney;
			if (vip_buy_count_need_money_arr.size() < (2 * (vip_buy_count_arr[vip_buy_count_arr_size - 1])))
			{
				return false;
			}
			if (vip_buy_count_need_money_arr.size() <= (i * 2 + 1))
			{
				return false;
			}

			cut_money((e_money_type)(vip_buy_count_need_money_arr[i * 2]), vip_buy_count_need_money_arr[i * 2 + 1], e_server_log_cut_money_teleport, map_temp_ptr->attribute_id);
		}

		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(get_array_index());
		//if (false == map_record_set_ref.is_valid())
		//{
		//	return false;
		//}

		//if (0 == map_temp_ptr->HaveExtraEnterWay)
		//{
		//	return false;
		//}
		//bool buy_result = map_record_set_ref.buy_count(map_temp_id, buy_count);
		//if (false == buy_result)
		//{
		//	return false;
		//}

		std::string notice_str_id = "90091285";
		int32 count = buy_count;
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(count));
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		send_notice(notice_str);

		get_team_cs_mgr().check_one_stop_finish_flags();//����Ƕ����У��ͷ���һ�ζ��鵱ǰ״̬��Ӱ����ս״̬

		return true;
	}

	int32 player::get_max_buy_count_with_map_id(int32 map_temp_id)
	{
		ZoneScoped;
		int32 player_vip_level = get_vip_level();
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);
		if (nullptr == map_temp_ptr)
		{
			return -1;
		}
		std::vector<int32> vip_buy_count_arr = map_temp_ptr->ExtraBuyCount;
		if (vip_buy_count_arr.size() < (player_vip_level + 1))
		{
			return -1;
		}
		return vip_buy_count_arr[player_vip_level];
	}

	int32 player::get_remain_buy_count_with_map_id(int32 map_temp_id)
	{
		ZoneScoped;
		int32 max_buy_count = get_max_buy_count_with_map_id(map_temp_id);
		int32 cur_buy_count = get_buy_count_with_map_id(map_temp_id);
		if (max_buy_count < 0 || cur_buy_count < 0)
		{
			return -1;
		}
		return (max_buy_count - cur_buy_count);
	}

	int32 player::get_buy_count_with_map_id(int32 map_temp_id)
	{
		ZoneScoped;
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);
		if (nullptr == map_temp_ptr)
		{
			return -1;
		}
		return 0;
	}

	int32 player::get_use_things_add_map_count(int32 map_temp_id)
	{
		ZoneScoped;
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id);
		if (nullptr == map_temp_ptr)
		{
			return -1;
		}
		return 0;
	}

	bool player::add_map_time_with_things(std::vector<int32> map_time_arr, int32 item_num)
	{
		ZoneScoped;
		if (map_time_arr.size() <= 0 || map_time_arr.size() % 2 != 0)
		{
			return false;
		}
		int32 map_type = map_time_arr[0];
		int32 add_time = map_time_arr[1] * item_num;
		if (map_type == e_map_type_exp_fuben)
		{
			int32 cur_add_time = get_time_data(e_time_type_exp_raid_extra_time);
			cur_add_time += add_time;
			set_time_data(e_time_type_exp_raid_extra_time, cur_add_time);
			send_time_one(e_time_type_exp_raid_extra_time);
			VIPTemplate* vip_temp_ptr = template_manager::get_instance().get_template_by_vip_level(get_vip_level(false));
			if (nullptr != vip_temp_ptr)
			{
				int32 cur_time = ((get_time_data(e_time_type_exp_raid_join_time) / second_tick_time) - cur_add_time) / vip_temp_ptr->LimitConvertExpTime;
				if (cur_time < 0)
				{
					cur_time = 0;
				}
				bool need_send = cur_time != get_must_do_count(e_daily_must_do_typ_raid_exp);
				if (need_send)
				{
					set_must_do_count(e_daily_must_do_typ_raid_exp, cur_time);
					set_daily_active_degree_info(e_daily_must_do_typ_raid_exp);
					send_daily_must_do_count_info_one(e_daily_must_do_typ_raid_exp);
				}
			}
		}
		else
		{
			return true;
		}
		return true;
	}

	void player::fuben_vip_mopping(int32 fuben_template_id, int32 settlement)
	{
		ZoneScoped;
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, fuben_template_id);
		if (nullptr == map_template_ptr || map_template_ptr->VipMoppingConfig.size() < 2)
		{
			return;
		}

		//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(get_array_index());
		//if (false == map_record_set_ref.is_valid())
		//{
		//	return;
		//}

		//int32 enter_times = map_record_set_ref.get_enter_count(fuben_template_id);
		//int32 buy_times = map_record_set_ref.get_buy_count(fuben_template_id);
		//int32 add_count = map_record_set_ref.get_add_count(fuben_template_id);

		if (map_template_ptr->Type == e_map_type_boss_home)
		{
			int32 cur_times = get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
			ActivityCommonConfigTemplate* temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
			if (temp_ptr == nullptr || cur_times >= temp_ptr->ParamInt2)
			{
				return;
			}
			settlement = get_gain_treasure_mgr().get_map_rank() - 1;
		}
		else if (map_template_ptr->Type == e_map_type_boss_single)
		{
			if (false == m_gain_treasure_mgr.can_entry_to_boss_single_map())
			{
				return;
			}
		}
		else
		{
			//// ������
			//if ((map_template_ptr->EnterCount + buy_times + add_count) < enter_times)
			//{
			//	return;
			//}
		}

		// ��������Ʒ
		if (map_template_ptr->EnterRaidCostItem.size() >= 2)
		{
			int32 item_id = map_template_ptr->EnterRaidCostItem[0];
			int32 item_need_num = map_template_ptr->EnterRaidCostItem[1];
			item_set& item_ref = get_item_set();
			int32 have_item_num = item_ref.get_item_count(e_bag_type_bag, item_id);
			if (have_item_num < item_need_num)
			{
				return;
			}
			else
			{
				item_ref.cost_item_by_id_with_lock_states(e_server_log_del_item_teleport, fuben_template_id, item_id, item_need_num, 1);
			}
		}


		//��������� ���ݶ�Ӧ���������ڹؿ�bossλ�ô��������
		if (map_template_ptr->VipMoppingDrop.size() > settlement)
		{
			int32 drop_id = map_template_ptr->VipMoppingDrop[settlement];
			int32 settle_boss_id = map_template_ptr->BossID;
			s_map_pos settle_boss_pos = s_map_pos();
			NpcTemplate* npc_temp_ptr = GET_TEMPLATE(NpcTemplate, settle_boss_id);
			if (nullptr != npc_temp_ptr && npc_temp_ptr->PathFindLocation.size() >= 3)
			{
				settle_boss_pos.unit_location.x = npc_temp_ptr->PathFindLocation[0];
				settle_boss_pos.unit_location.y = npc_temp_ptr->PathFindLocation[1];
				settle_boss_pos.unit_location.z = npc_temp_ptr->PathFindLocation[2];
			}
			else
			{
				settle_boss_pos = get_new_map_pos();
			}

			drop_manager::create_drop_box(drop_id, this, settle_boss_pos, true);

			// ÿ��ɨ����Ϊ���һ�� ���ӻ�Ծ��
			fuben_map_enter_activity();

			if (map_template_ptr->Type == e_map_type_boss_home)
			{
				gain_treasure_mgr& gain_treasure_ref = get_gain_treasure_mgr();
				int32 gain_treasure_value = gain_treasure_ref.get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
				gain_treasure_value += 1;
				gain_treasure_ref.set_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss, gain_treasure_value);
				gain_treasure_ref.send_gain_treasure_info_one(e_gain_treasure_info_type_killed_field_boss);
				set_must_do_count(e_daily_must_do_typ_boss_home, get_must_do_count(e_daily_must_do_typ_boss_home) + 1);
				send_daily_must_do_count_info_one(e_daily_must_do_typ_boss_home);
				set_daily_active_degree_info(e_daily_must_do_typ_boss_home);
				get_mission_mgr().target_check(e_mission_end_type_boss_field, 0);
				get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_kill_boss);
				set_boss_challenge_restore_times();
			}
			else if (map_template_ptr->Type == e_map_type_boss_single)
			{
				gain_treasure_mgr& gain_treasure_ref = get_gain_treasure_mgr();
				int32 gain_treasure_value = gain_treasure_ref.get_gain_treasure_info(e_gain_treasure_info_type_killed_simple_boss);
				gain_treasure_value += 1;
				gain_treasure_ref.set_gain_treasure_info(e_gain_treasure_info_type_killed_simple_boss, gain_treasure_value);
				gain_treasure_ref.send_gain_treasure_info_one(e_gain_treasure_info_type_killed_simple_boss);
				set_must_do_count(e_daily_must_do_typ_single_boss, get_must_do_count(e_daily_must_do_typ_single_boss) + 1);
				send_daily_must_do_count_info_one(e_daily_must_do_typ_single_boss);
				set_daily_active_degree_info(e_daily_must_do_typ_single_boss);
				get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_kill_personal_boss);
			}

			//map_record_set_ref.increase_enter_count(fuben_template_id, false);


			//��ʱ��Ϊ��������
			if (map_template_ptr->Order == e_map_order_type_daily_raid)
			{
				switch (map_template_ptr->Type)
				{
				case e_map_type_exp_fuben:	//���鸱��
					get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_exp_stage);
					break;
				case e_map_type_money_fuben: //��Ҹ���
					get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_coin_stage);
					break;
				case e_map_type_abyss_of_fear: //�û�����
					get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_illusion_stage);
					break;
				case e_map_type_fallen_fantasy: //��������
					get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_belief_stage);
					break;
				case e_map_type_star_test: //���鸱��
					get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_sprite_stage);
					break;
				default:
					break;
				}
			}
		}
	}

	bool player::add_map_count_with_things(std::vector<int32> map_temp_id, int32 add_count)
	{
		ZoneScoped;
		if (map_temp_id.size() < 2)
		{
			return false;
		}
		switch (map_temp_id[0])
		{
		case e_map_count_map_id:
		{
			MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, map_temp_id[1]);
			if (nullptr == map_temp_ptr)
			{
				return false;
			}
			//map_record_set& map_record_set_ref = map_record_mgr::get_instance().get_map_record_set(get_array_index());
			//if (false == map_record_set_ref.is_valid())
			//{
			//	return false;
			//}
			//map_record_set_ref.add_count_with_item(map_temp_id[1], add_count);
		}
		break;
		case e_map_count_gain_treasure_type:
		{
			// ����Boss��ع�Bossһ��ֻ��ʹ��һ����Ʒ
			if (add_count > 1)
			{
				return false;
			}
			int32 cur_num = get_gain_treasure_mgr().get_gain_treasure_info(map_temp_id[1]);
			// ����1�β�����
			if (cur_num <= 0)
			{
				return false;
			}
			get_gain_treasure_mgr().set_gain_treasure_info(map_temp_id[1], cur_num - 1);
			get_gain_treasure_mgr().send_gain_treasure_info_one(e_gain_treasure_info_type_buy_simple_boss);
		}
		break;
		default:
			break;
		}

		std::string notice_str_id = "90091290";
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(add_count));
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		send_notice(notice_str);

		get_team_cs_mgr().check_one_stop_finish_flags();//����Ƕ����У��ͷ���һ�ζ��鵱ǰ״̬��Ӱ����ս״̬

		return true;
	}

	void player::change_name(xstring role_name)
	{
		ZoneScoped;
		set_name(role_name);
		send_info_all();
	}

	void player::sub_rename_card(uint32 rename_card_id)
	{
		ZoneScoped;
		int32 ret = e_change_role_success;
		citem* item_ptr = get_item_set().get_item_by_template_id(rename_card_id);
		if (nullptr == item_ptr)
		{
			ret = e_change_failed_role_item_not_enough;
		}
		else if (item_ptr->get_data_info(e_item_info_stack_count) < 1)
		{
			ret = e_change_failed_role_item_not_enough;
		}

		//ɾ����Ʒ
		if (ret == e_change_role_success)
		{
			get_item_set().del_item(e_server_log_del_item_rename_cost, 0, item_ptr, 1);
		}

		cs2ws_sub_player_rename_item_end resp;
		resp.role_guid = get_unit_guid();
		resp.result = ret;
		connection_mgr::getInstance().send_to_ws(&resp, sizeof(resp));
	}

	void player::sub_re_legion_name(uint32 re_legion_name_card_id)
	{
		ZoneScoped;
		cs2ws_confirm_change_legion_name msg;
		msg.role_guid = get_unit_guid();

		if (item_system::can_cost_item(this, e_bag_type_bag, re_legion_name_card_id, 1) == false)
		{
			msg.m_legion_error_type = e_legion_name_error_item_not_enough;
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
			return;
		}
		item_system::cost_item_from_bag(this, e_bag_type_bag, re_legion_name_card_id, 1);

		msg.m_legion_error_type = e_legion_name_error_change_name_succeed;
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
	}
	bool player::is_get_kill_legion_boss_award_today()
	{
		ZoneScoped;
		return get_logic_data(e_role_logic_info_is_get_kill_legion_boss_award) > 0;
	}

	void player::mark_kill_legion_boss_get_award_today()
	{
		ZoneScoped;
		set_logic_data(e_role_logic_info_is_get_kill_legion_boss_award, 1);
	}

	void player::refresh_kill_legion_boss_get_award_today()
	{
		ZoneScoped;
		set_logic_data(e_role_logic_info_is_get_kill_legion_boss_award, 0);
	}

	void player::set_first_npc_kill_id_arr()
	{
		ZoneScoped;
		m_first_npc_kill_id_arr.clear();
		xstring first_npc_kill_id_str = m_role_info.role_first_kill_npc_id_arr;
		init_unit::parse_char_to_vector(m_first_npc_kill_id_arr, first_npc_kill_id_str);
	}

	bool player::get_is_have_cur_first_kill_npc_id(int32 npc_id)
	{
		ZoneScoped;
		for (int32 i = 0; i < m_first_npc_kill_id_arr.size(); i++)
		{
			if (npc_id == m_first_npc_kill_id_arr[i])
			{
				if (npc_id == 76000281)
				{
					CONSOLE_INFO("have same first kill npc id = {}", npc_id);
				}
				return true;
			}
		}
		return false;
	}

	void player::insert_first_npc_kill_id_str(int32 first_npc_id)
	{
		ZoneScoped;
		if (m_first_npc_kill_id_arr.size() >= max_first_kill_npc_id_num)
		{
			return;
		}
		if (get_is_have_cur_first_kill_npc_id(first_npc_id))
		{
			return;
		}
		m_first_npc_kill_id_arr.push_back(first_npc_id);
		xstring first_npc_kill_id_str = "";
		init_unit::parse_vector_to_char(first_npc_kill_id_str, m_first_npc_kill_id_arr);
		m_role_info.set_first_kill_npc_id_arr(first_npc_kill_id_str);
	}

	void player::set_last_kill_player_guid(guid_64 player_guid)
	{
		ZoneScoped;
		int64 cur_time_sec = time_helper::get_cur_time_new().second;
		ActivityCommonConfigTemplate* act_com_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg_ptr)
		{
			return;
		}
		int32 interval_time = act_com_cfg_ptr->ParamInt5;
		if (player_guid == m_last_kill_player_guid)
		{
			if (m_last_kill_player_time + interval_time < cur_time_sec)
			{
				m_last_kill_player_time = cur_time_sec;
			}
		}
		else
		{
			m_last_kill_player_guid = player_guid;
			m_last_kill_player_time = cur_time_sec;
		}
	}

	void player::set_last_kill_me_player_guid(guid_64 player_guid)
	{
		ZoneScoped;
		int64 cur_time_sec = time_helper::get_cur_time_new().second;
		ActivityCommonConfigTemplate* act_com_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg_ptr)
		{
			return;
		}
		int32 interval_time = act_com_cfg_ptr->ParamInt5;
		if (player_guid == m_last_kill_me_player_guid)
		{
			if (m_last_kill_me_player_time + interval_time < cur_time_sec)
			{
				m_last_kill_me_player_time = cur_time_sec;
			}
		}
		else
		{
			m_last_kill_me_player_guid = player_guid;
			m_last_kill_me_player_time = cur_time_sec;
		}
	}

	void player::summon_all_pet()
	{
		ZoneScoped;
		if (m_summon_tick == 0)
		{
			m_summon_tick = time_helper::get_cur_time_new().millisecond + second_tick_time;
			return;
		}
		m_summon_tick = 0;
		m_goddess_mgr.enter_reload_goddess();
		//summon_pet(76990301);
	}

	void player::kill_all_pet()
	{
		ZoneScoped;
		m_summon_tick = 0;
		for (auto i = 1; i < e_summon_pet_type_max; ++i)
		{
			kill_pet_by_type((e_summon_pet_type)i);
		}
	}

	npc* player::summon_pet(int32 template_id)
	{
		ZoneScoped;
		auto npc_template_ptr = GET_TEMPLATE(NpcTemplate, template_id);
		if (nullptr == npc_template_ptr || npc_template_ptr->NpcType != e_unit_type_summoned)
			return nullptr;
		if (npc_template_ptr->SubType <= e_summon_pet_type_normal_summon || npc_template_ptr->SubType >= e_summon_pet_type_max)
			return nullptr;

		//s_map_pos new_map_pos;
		//new_map_pos.clear_data();
		//new_map_pos.unit_location = get_new_map_pos().unit_location;
		//new_map_pos.set_rotation(get_new_map_pos().unit_rotation);
		//new_map_pos.unit_location.z = get_new_map_pos().unit_location.z + get_unit_half_height() / 2;
		//new_map_pos.unit_location.x += rand() % 10;
		//new_map_pos.unit_location.y += rand() % 10;

		kill_pet_by_type(e_summon_pet_type(npc_template_ptr->SubType));

		npc* temp_ptr = world_cs::spawn_npc(template_id, get_back_pos(), get_identifier(), get_map_ent());
		//npc* temp_ptr = world_cs::spawn_npc(get_map_guid(), new_map_pos, template_id, get_identifier());
		if (nullptr == temp_ptr || false == temp_ptr->is_valid())
			return nullptr;
		//auto map_object_ptr = world_cs::get_map_by_guid<map_object>(get_map_guid());
		//if (map_object_ptr && map_object_ptr->get_map_type() != e_map_type_big_map
		//	&& map_object_ptr->get_map_type() != e_map_type_princess_guard_war
		//	&& map_object_ptr->get_map_type() != e_map_type_boss_island)
		//{
		//	temp_ptr->get_pawn_att().set_game_att(e_unit_game_att_movement, e_move_ment_wing_walk, true);
		//}
		m_summon_pet_array[e_summon_pet_type(npc_template_ptr->SubType)] = temp_ptr->get_array_index();
		return temp_ptr;
	}

	void player::kill_pet_by_type(e_summon_pet_type type)
	{
		ZoneScoped;
		if (m_summon_pet_array[type])
		{
			npc& npc_ref = unit_man::get_npc(m_summon_pet_array[type]);
			if (npc_ref.is_valid() && npc_ref.is_summon_pet())
			{
				unit_man::remove_npc(m_summon_pet_array[type], false);
			}
			m_summon_pet_array[type] = 0;
		}
	}

	npc* player::get_pet_ptr_by_type(e_summon_pet_type type)
	{
		ZoneScoped;
		if (m_summon_pet_array[type])
		{
			npc& npc_ref = unit_man::get_npc(m_summon_pet_array[type]);
			if (npc_ref.is_valid() && npc_ref.is_summon_pet())
				return &npc_ref;
			else
				m_summon_pet_array[type] = 0;
		}
		return nullptr;
	}

	void player::change_summon_pet_movement()
	{
		ZoneScoped;
		//for (auto i = 1; i < e_summon_pet_type_max; ++i)
		//{
		//	if (m_summon_pet_array[i])
		//	{
		//		npc& npc_ref = unit_man::get_npc(m_summon_pet_array[i]);
		//		if (npc_ref.is_valid() && npc_ref.is_summon_pet())
		//		{
		//			npc_ref.get_pawn_att().set_game_att(e_unit_game_att_movement, get_pawn_att().get_game_att(e_unit_game_att_movement), true);
		//		}
		//	}
		//}
	}

	s_map_pos player::get_back_pos(bool isLeft)
	{
		ZoneScoped;
		const fvector& effect_orig_location = get_new_map_pos().unit_location;
		const frotator& effect_orig_rotation = get_new_map_pos().unit_rotation;
		fvector offset;
		if (isLeft)
		{
			offset.x = 140 * cos((effect_orig_rotation.yaw + 50) * RADIAN) * -1;
			offset.y = 140 * sin((effect_orig_rotation.yaw + 50) * RADIAN) * -1;
			offset.z = get_unit_half_height() / 2;
			offset += effect_orig_location;
		}
		else
		{
			offset.x = 140 * cos((effect_orig_rotation.yaw - 50) * RADIAN) * -1;
			offset.y = 140 * sin((effect_orig_rotation.yaw - 50) * RADIAN) * -1;
			offset.z = get_unit_half_height() / 2;
			offset += effect_orig_location;
		}

		s_map_pos new_map_pos;
		new_map_pos.clear_data();
		new_map_pos.unit_location = offset;
		new_map_pos.set_rotation(effect_orig_rotation);
		return new_map_pos;
	}

	void player::sync_wedding_ring_level_to_ws()
	{
		ZoneScoped;
		cs2ws_update_wedding_ring_level msg;
		msg.role_guid = get_unit_guid();
		msg.ring_level = m_marry_mgr.get_ring_level();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(cs2ws_update_wedding_ring_level));
	}

	void player::add_heart_value(int32 heart_value, int32 send_type)
	{
		ZoneScoped;
		get_marry_mgr().add_heart_value(heart_value, send_type);
	}

	void player::get_marry_dati_last_reward()
	{
		ZoneScoped;

		std::vector<int32> m_ItemRewardArray = GAMECONFIG->MarryDaTiEndReward;
		if (m_ItemRewardArray.size() < 2)
		{
			return;
		}
		//��Ʒ����
		std::vector<citem*> item_array;
		vector<s_item_template_info> promp_item_data;
		for (int32 i = 0; i * 2 + 1 < m_ItemRewardArray.size(); i++)
		{
			int32 item_template_id = m_ItemRewardArray[i * 2];
			int32 item_num = m_ItemRewardArray[i * 2 + 1];

			ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_template_id);
			if (nullptr == ItemTemplatePtr)
			{
				continue;
			}
			citem* temp_item = get_item_set().create_item_by_template(e_server_log_add_item_marry_dati, 0, item_template_id, item_num, 1);
			if (nullptr != temp_item)
			{
				item_array.push_back(temp_item);
				promp_item_data.push_back({ item_template_id ,item_num ,temp_item->get_data_info(e_item_info_locked) });
			}
		}
		get_item_set().put_in_bag(item_array);
		if (promp_item_data.size() > 0)
		{
			get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
		}
	}

	void player::get_marry_dati_reward(int32 dati_id, bool is_true)
	{
		ZoneScoped;
		QuestionMarryTemplate* question_marry_template = GET_TEMPLATE(QuestionMarryTemplate, dati_id);
		if (nullptr == question_marry_template)
		{
			return;
		}
		std::vector<int32> m_ItemRewardArray;
		std::vector<int32> m_BattleRewardArray;

		if (is_true)
		{
			m_ItemRewardArray = question_marry_template->ItemRewardArray;
			m_BattleRewardArray = question_marry_template->BattleRewardArray;
		}
		else
		{
			m_ItemRewardArray = question_marry_template->ItemRewardArray1;
			m_BattleRewardArray = question_marry_template->BattleRewardArray1;
		}
		//�ӽ��
		int32 reward_len = m_ItemRewardArray.size();
		if (reward_len > 0 && reward_len % 2 == 0)
		{
			//��Ʒ����
			std::vector<citem*> item_array;
			vector<s_item_template_info> promp_item_data;
			for (int32 i = 0; i * 2 + 1 < m_ItemRewardArray.size(); i++)
			{
				int32 item_template_id = m_ItemRewardArray[i * 2];
				int32 item_num = m_ItemRewardArray[i * 2 + 1];

				ItemTemplate* ItemTemplatePtr = GET_TEMPLATE(ItemTemplate, item_template_id);
				if (nullptr == ItemTemplatePtr)
				{
					continue;
				}
				citem* temp_item = get_item_set().create_item_by_template(e_server_log_add_item_marry_dati, 0, item_template_id, item_num, 1);
				if (nullptr != temp_item)
				{
					item_array.push_back(temp_item);
					promp_item_data.push_back({ item_template_id,item_num ,temp_item->get_data_info(e_item_info_locked) });
				}
			}
			get_item_set().put_in_bag(item_array);
			if (promp_item_data.size() > 0)
			{
				get_item_set().get_item_send_promp_msg_to_client(promp_item_data);
			}
		}
		//�ӽ��
		int32 len = m_BattleRewardArray.size();
		if (len > 0 && len % 2 == 0)
		{
			add_money_by_template_tuple(m_BattleRewardArray);
		}
	}
	void player::player_changed(e_achievement_type operate_type)
	{
		ZoneScoped;
		get_achievement_mgr().player_changed(operate_type);
	}

	void player::add_title_by_type_and_value(e_title_type title_type, int32 value1, int32 value2)
	{
		ZoneScoped;
		m_title_mgr.add_title_by_type_and_value(title_type, value1, value2);
	}

	void player::req_cross_ladder_last_score()
	{
		ZoneScoped;
		cs2ws_cross_ladder_req_last_score msg;
		msg.role_guid = get_unit_guid();
		msg.server_id = cell_server::getInstance().get_server_id();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg), GATECONFIG->gate_id);
	}

	void player::receive_cross_ladder_last_score(int32 last_score)
	{
		ZoneScoped;
		LadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_ladder_score(last_score);
		if (nullptr == ladder_template_ptr)
			return;
		int32 new_title_id = ladder_template_ptr->TitleReward;
		if (new_title_id == 0)
		{
			//������������������û�õ��ƺ�
			delete_cross_ladder_title();
			return;
		}
		for (int32 iter = e_title_type_cross_ladder_one; iter <= e_title_type_cross_ladder_six; ++iter)
		{
			int32 title_id = get_title_mgr().get_title_template_id_by_type((e_title_type)iter);
			if (get_title_mgr().is_have_title(title_id) == false)
				continue;
			//��������������ֳƺ���ͬ
			if (title_id == new_title_id)
				return;
			else
			{
				//�ƺŲ�ͬ
				int32 show_title = get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
				get_title_mgr().remove_title_by_template_id(title_id);
				if (show_title == title_id)
				{
					show_title = get_title_mgr().get_first_active_title_template_id();
					get_title_mgr().equip_on_title(show_title);
				}
			}
		}
		get_title_mgr().add_title_by_template_id(new_title_id);
	}

	void player::receive_cross_ladder_last_ranking(int32 last_ranking)
	{
		ZoneScoped;
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_ladder);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		int32 new_title_id = 0;
		if (last_ranking > 0 && last_ranking <= 100)
			new_title_id = act_com_cfg->ParamInt6;

		if (new_title_id == 0)
		{
			//������������������û�õ��ƺ�
			int32 title_id = get_title_mgr().get_title_template_id_by_type(e_title_type_cross_ladder_ranking);
			if (get_title_mgr().is_have_title(title_id))
			{
				int32 show_title = get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
				get_title_mgr().remove_title_by_template_id(title_id);
				if (show_title == title_id)
				{
					show_title = get_title_mgr().get_first_active_title_template_id();
					get_title_mgr().equip_on_title(show_title);
				}
			}
			return;
		}

		if (get_title_mgr().is_have_title(new_title_id) == true)
			return;
		get_title_mgr().add_title_by_template_id(new_title_id);
	}

	void player::delete_cross_ladder_title()
	{
		ZoneScoped;
		for (int32 iter = e_title_type_cross_ladder_one; iter <= e_title_type_cross_ladder_six; ++iter)
		{
			int32 title_id = get_title_mgr().get_title_template_id_by_type((e_title_type)iter);
			if (get_title_mgr().is_have_title(title_id))
			{
				int32 show_title = get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
				get_title_mgr().remove_title_by_template_id(title_id);
				if (show_title == title_id)
				{
					show_title = get_title_mgr().get_first_active_title_template_id();
					get_title_mgr().equip_on_title(show_title);
				}
			}
		}
	}

	void player::req_element_ladder_last_score()
	{
		ZoneScoped;
		cs2ws_element_ladder_req_last_score msg;
		msg.role_guid = get_unit_guid();
		msg.server_id = cell_server::getInstance().get_server_id();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg), GATECONFIG->gate_id);
	}

	void player::receive_element_ladder_last_score(int32 last_score)
	{
		ZoneScoped;
		ElementLadderTemplate* ladder_template_ptr = template_manager::get_instance().get_template_by_element_score(last_score);
		if (nullptr == ladder_template_ptr)
		{
			return;
		}
		int32 new_title_id = ladder_template_ptr->TitleReward;
		if (new_title_id == 0)
		{
			//������������������û�õ��ƺ�
			delete_element_ladder_title();
			return;
		}
		for (int32 iter = e_title_type_element_war_one; iter <= e_title_type_element_war_three; ++iter)
		{
			int32 title_id = get_title_mgr().get_title_template_id_by_type((e_title_type)iter);
			if (get_title_mgr().is_have_title(title_id) == false)
				continue;
			//��������������ֳƺ���ͬ
			if (title_id == new_title_id)
				return;
			else
			{
				//�ƺŲ�ͬ
				int32 show_title = get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
				get_title_mgr().remove_title_by_template_id(title_id);
				if (show_title == title_id)
				{
					show_title = get_title_mgr().get_first_active_title_template_id();
					get_title_mgr().equip_on_title(show_title);
				}
			}
		}
		get_title_mgr().add_title_by_template_id(new_title_id);
	}

	void player::receive_element_ladder_last_ranking(int32 last_ranking)
	{
		ZoneScoped;
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_element_war);
		if (nullptr == act_com_cfg)
		{
			return;
		}
		int32 new_title_id = 0;
		if (last_ranking > 0 && last_ranking <= 100)
		{
			new_title_id = act_com_cfg->ParamInt6;
		}

		if (new_title_id == 0)
		{
			//������������������û�õ��ƺ�
			int32 title_id = get_title_mgr().get_title_template_id_by_type(e_title_type_element_war_ranking);
			if (get_title_mgr().is_have_title(title_id))
			{
				int32 show_title = get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
				get_title_mgr().remove_title_by_template_id(title_id);
				if (show_title == title_id)
				{
					show_title = get_title_mgr().get_first_active_title_template_id();
					get_title_mgr().equip_on_title(show_title);
				}
			}
			return;
		}

		if (get_title_mgr().is_have_title(new_title_id) == true)
			return;
		get_title_mgr().add_title_by_template_id(new_title_id);
	}

	void player::delete_element_ladder_title()
	{
		ZoneScoped;
		for (int32 iter = e_title_type_element_war_one; iter <= e_title_type_element_war_three; ++iter)
		{
			int32 title_id = get_title_mgr().get_title_template_id_by_type((e_title_type)iter);
			if (get_title_mgr().is_have_title(title_id))
			{
				int32 show_title = get_pawn_att().get_unit_base_att(faith::e_base_att_info_equip_title_id);
				get_title_mgr().remove_title_by_template_id(title_id);
				if (show_title == title_id)
				{
					show_title = get_title_mgr().get_first_active_title_template_id();
					get_title_mgr().equip_on_title(show_title);
				}
			}
		}
	}

	void player::send_gm_add_ticket()
	{
		ZoneScoped;
		cs2ws_element_war_gm_send_add_ticket msg;
		msg.role_guid = get_unit_guid();
		connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg), GATECONFIG->gate_id);
	}

	void player::transfer_to_assist_fight(int32 map_template_id, guid_64 map_guid, int32 group_id, s_map_pos map_pos)
	{
		ZoneScoped;
		set_logic_data(e_role_logic_info_main_pk_mode, e_pk_mode_peace);
		//ǿ���˶�
		//get_team_cs_mgr().send_leave_team_to_ws();
		//������ս��Ϣ
		m_teleport_map_id = map_template_id;
		m_teleport_map_pos = map_pos;
		m_teleport_line_id = 0;
		m_teleport_group_id = group_id;
		m_teleport_map_guid = map_guid;
		m_teleport_war_index = 0;
	}

	bool player::check_can_get_helper_reward()
	{
		ZoneScoped;
		int32 cur_times = get_logic_data(e_role_logic_info_helper_reward_send_times);
		if (cur_times >= GAMECONFIG->MaxHelperRewardSendTimes + get_extra_helper_reward_times())
			return false;
		return true;
	}

	int32 player::get_extra_helper_reward_times()
	{
		ZoneScoped;
		int32 legion_job_title = get_player_legion_job_title();
		if (GAMECONFIG->LegionJobExtraHelperRewardTimes.size() == 0)
			return 0;
		if (legion_job_title < 0 || legion_job_title >= GAMECONFIG->LegionJobExtraHelperRewardTimes.size())
			return 0;
		return GAMECONFIG->LegionJobExtraHelperRewardTimes[legion_job_title];
	}

	bool player::check_can_get_be_helped_reward()
	{
		ZoneScoped;
		return check_can_get_helper_reward();
	}

	int32 player::delete_get_helper_reward_times(int32 point)
	{
		ZoneScoped;
		int32 cur_times = get_logic_data(e_role_logic_info_helper_reward_send_times);
		set_logic_data(e_role_logic_info_helper_reward_send_times, cur_times + point);
		send_logic_one(e_role_logic_info_helper_reward_send_times);

		//set_logic_data(e_role_logic_info_help_reward_send_times, cur_times + point);
		//send_logic_one(e_role_logic_info_help_reward_send_times); 
		return 0;

	}

	int32 player::get_help_value(int32 point)
	{
		ZoneScoped;
		int32 cur_times = get_logic_data(e_role_logic_info_helper_reward_send_times);

		int32 up_limit = GAMECONFIG->MaxHelperRewardSendTimes + get_extra_helper_reward_times();

		if (cur_times + point > up_limit)
		{
			return up_limit - cur_times;
		}
		return point;
	}
	int32 player::delete_get_be_helped_reward_times(int32 point)
	{
		ZoneScoped;
		/*int32 cur_times = get_logic_data(e_role_logic_info_help_reward_send_times);
		set_logic_data(e_role_logic_info_help_reward_send_times, cur_times + 1);
		send_logic_one(e_role_logic_info_help_reward_send_times);*/
		return delete_get_helper_reward_times(point);
	}

	void player::send_attacker_info(unit& unit_ref)
	{
		ZoneScoped;
		if (false == unit_ref.is_valid() || e_unit_type_player != unit_ref.get_unit_type() || unit_ref.get_unit_guid() == get_unit_guid())
			return;

		if (m_send_attacker_info_cd < time_helper::get_cur_time_new().second)
		{
			m_send_attacker_info_cd = time_helper::get_cur_time_new().second + 5;
			game_proto_send_the_attacker_info msg;
			msg.set_role_name(unit_ref.get_name());
			msg.set_role_guid_a(unit_ref.get_unit_guid().A);
			msg.set_role_guid_b(unit_ref.get_unit_guid().B);
			msg.set_template_id(unit_ref.get_unit_info(e_role_info_template_id));
			msg.set_role_level(unit_ref.get_unit_info(e_role_info_exp_level));
			msg.set_is_npc(false);
			send_message_to_self(&msg, e_msgindex_s2c_send_the_attacker_info);
		}
	}

	void player::send_attacker_info_by_robot_player(npc& unit_ref)
	{
		ZoneScoped;
		if (false == unit_ref.is_valid() || unit_ref.get_unit_guid() == get_unit_guid())
			return;
		if (m_send_attacker_info_cd < time_helper::get_cur_time_new().second)
		{
			m_send_attacker_info_cd = time_helper::get_cur_time_new().second + 5;
			game_proto_send_the_attacker_info msg;
			msg.set_role_name(unit_ref.get_name());
			msg.set_role_guid_a(unit_ref.get_unit_guid().A);
			msg.set_role_guid_b(unit_ref.get_unit_guid().B);
			msg.set_template_id(unit_ref.get_npc_template_id());
			msg.set_is_npc(true);
			send_message_to_self(&msg, e_msgindex_s2c_send_the_attacker_info);
		}
	}

	bool player::check_can_receive_summon_damage()
	{
		ZoneScoped;
		if (m_assist_fight_mgr.is_assist_helper() && m_assist_fight_mgr.get_assist_fight_guid().is_valid())
			return true;
		auto map_type = base_map_system::get_map_type(m_map_ent);

		switch (map_type)
		{
		case e_map_type_boss_vip_home:
		{
			ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_home);
			if (nullptr == act_common_config_ptr)
				return false;

			int32 max_count = act_common_config_ptr->ParamInt2;
			int32 cur_count = get_logic_data(e_role_logic_info_boss_home_cur_used_times);
			if (cur_count >= max_count)
				return false;
			return true;
		}
		break;
		case e_map_type_boss_home:
		{
			ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
			if (nullptr == act_common_config_ptr)
			{
				return false;
			}

			int32 max_count = act_common_config_ptr->ParamInt2;
			int32 cur_count = get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
			if (cur_count >= max_count)
				return false;

			return true;
		}
		break;
		case e_map_type_boss_island:
		{
			if (get_boss_island_mgr().get_left_boss_island_kill() == 0)
				return false;
			return true;
		}
		break;
		default:
			break;
		}
		return true;
	}


	bool player::check_can_get_boss_drop(npc* npc_ptr)
	{
		ZoneScoped;
		if (nullptr == npc_ptr)
			return false;
		auto map_type = base_map_system::get_map_type(m_map_ent);

		switch (map_type)
		{
		case e_map_type_boss_vip_home:
		{
			ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_home);
			if (nullptr == act_common_config_ptr)
				return false;

			int32 max_count = act_common_config_ptr->ParamInt2;
			int32 cur_count = get_logic_data(e_role_logic_info_boss_home_cur_used_times);
			if (cur_count >= max_count)
				return false;
			return true;
		}
		break;
		case e_map_type_boss_home:
		{
			ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
			if (nullptr == act_common_config_ptr)
			{
				return false;
			}

			int32 max_count = act_common_config_ptr->ParamInt2;
			int32 cur_count = get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
			if (cur_count >= max_count)
				return false;
			return true;
		}
		break;
		case e_map_type_boss_island:
		{
			if (get_boss_island_mgr().get_left_boss_island_kill() == 0)
				return false;
			return true;
		}
		break;
		default:
			break;
		}
		return true;
	}

	void  player::delete_boss_challenge_times_after_drop()
	{
		ZoneScoped;

		auto map_type = base_map_system::get_map_type(m_map_ent);
		switch (map_type)
		{
		case e_map_type_boss_vip_home:
		{
			ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_home);
			if (nullptr == act_common_config_ptr)
				break;
			int32 max_count = act_common_config_ptr->ParamInt2;
			int32 cur_kill_num = get_logic_data(e_role_logic_info_boss_home_cur_used_times);
			cur_kill_num++;
			if (cur_kill_num > max_count)
			{
				cur_kill_num = max_count;
			}
			set_logic_data(e_role_logic_info_boss_home_cur_used_times, cur_kill_num);
			send_logic_one(e_role_logic_info_boss_home_cur_used_times);

			set_must_do_count(e_daily_must_do_typ_vip_boss_home, get_must_do_count(e_daily_must_do_typ_vip_boss_home) + 1);
			send_daily_must_do_count_info_one(e_daily_must_do_typ_vip_boss_home);
			set_daily_active_degree_info(e_daily_must_do_typ_vip_boss_home);
		}
		break;
		case e_map_type_boss_home:
		{
			ActivityCommonConfigTemplate* act_common_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
			if (nullptr == act_common_config_ptr)
				break;
			int32 max_count = act_common_config_ptr->ParamInt2;
			int32 cur_kill_num = get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
			cur_kill_num++;
			if (cur_kill_num > max_count)
			{
				cur_kill_num = max_count;
			}
			get_gain_treasure_mgr().set_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss, cur_kill_num);
			get_gain_treasure_mgr().send_gain_treasure_info_one(e_gain_treasure_info_type_killed_field_boss);
			set_must_do_count(e_daily_must_do_typ_boss_home, get_must_do_count(e_daily_must_do_typ_boss_home) + 1);
			send_daily_must_do_count_info_one(e_daily_must_do_typ_boss_home);
			set_daily_active_degree_info(e_daily_must_do_typ_boss_home);
			get_mission_mgr().target_check(e_mission_end_type_boss_field, 0);
			get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_kill_boss);
			get_recycle_mgr().on_event(e_recycle_task_type_kill_boss_9);
		}
		break;
		case e_map_type_boss_island:
		{
			int32 max_count = GAMECONFIG->BossIslandMaxKillNum;
			int32 cur_kill_num = get_boss_island_mgr().get_boss_island_info(e_island_record_killed_island_boss);
			cur_kill_num++;
			if (cur_kill_num > max_count)
			{
				cur_kill_num = max_count;
			}
			get_boss_island_mgr().set_boss_island_info(e_island_record_killed_island_boss, cur_kill_num);
			get_boss_island_mgr().send_player_boss_island_info_one(e_island_record_killed_island_boss);
			set_must_do_count(e_daily_must_do_typ_boss_island, get_must_do_count(e_daily_must_do_typ_boss_island) + 1);
			send_daily_must_do_count_info_one(e_daily_must_do_typ_boss_island);
			set_daily_active_degree_info(e_daily_must_do_typ_boss_island);
		}
		break;
		default:
			return;
		}
		set_boss_challenge_restore_times();
	}

	void player::refresh_boss_challenge_times()
	{
		ZoneScoped;
		set_logic_data(e_role_logic_info_boss_home_cur_used_times, 0);
	}

	bool player::check_can_enter_gain_treasure_map(e_map_type type)
	{
		ZoneScoped;
		//�ع�boss�뵥�˵ع�bossͨ��һ����ͼ
		if (type == e_map_type_boss_home || type == e_map_type_single_land_boss)
		{
			int32 cur_times = get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
			ActivityCommonConfigTemplate* temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_field);
			if (temp_ptr == nullptr || cur_times >= temp_ptr->ParamInt2)
			{
				return false;
			}
			return true;
		}
		if (type == e_map_type_boss_vip_home)
		{
			int32 cur_times = get_logic_data(e_role_logic_info_boss_home_cur_used_times);
			ActivityCommonConfigTemplate* temp_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_boss_home);
			if (temp_ptr == nullptr || cur_times >= temp_ptr->ParamInt2)
			{
				return false;
			}
			return true;
		}
		return false;
	}

	void  player::set_boss_challenge_restore_times()
	{
		ZoneScoped;

		auto map_type = base_map_system::get_map_type(m_map_ent);
		switch (map_type)
		{
		case e_map_type_boss_home:
		case e_map_type_single_land_boss:
		{
			int32 cur_kill_num = get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
			if (cur_kill_num > 0)
			{
				if (get_logic_data(e_role_logic_info_gain_treasure_restore_time) == 0)
				{
					int32 cur_times = get_logic_data(e_role_logic_info_gain_treasure_restore_num);
					if (cur_times >= GAMECONFIG->BossHomeMaxRestoreNum)
						break;
					set_logic_data(e_role_logic_info_gain_treasure_restore_time, time_helper::get_cur_time_new().second + GAMECONFIG->BossHomeRestoreNeedTime);
					send_logic_one(e_role_logic_info_gain_treasure_restore_time);
				}
			}
		}
		break;
		case e_map_type_boss_island:
		{
			int32 cur_kill_num = get_boss_island_mgr().get_boss_island_info(e_island_record_killed_island_boss);
			if (cur_kill_num > 0)
			{
				if (get_logic_data(e_role_logic_info_sky_land_boss_restore_time) == 0)
				{
					int32 cur_times = get_logic_data(e_role_logic_info_sky_land_boss_restore_num);
					if (cur_times >= GAMECONFIG->SkyIsLandMaxRestoreNum)
						break;
					set_logic_data(e_role_logic_info_sky_land_boss_restore_time, time_helper::get_cur_time_new().second + GAMECONFIG->SkyIsLandRestoreNeedTime);
					send_logic_one(e_role_logic_info_sky_land_boss_restore_time);
				}
			}
		}
		break;
		default:
			break;
		}
	}
	void player::check_boss_challenge_restore_times(int32 new_time_second)
	{
		ZoneScoped;
		if (get_logic_data(e_role_logic_info_gain_treasure_restore_time) && new_time_second >= get_logic_data(e_role_logic_info_gain_treasure_restore_time))
		{
			int32 old_time = get_logic_data(e_role_logic_info_gain_treasure_restore_time);
			set_logic_data(e_role_logic_info_gain_treasure_restore_time, 0);
			int32 cur_kill_num = get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss);
			if (cur_kill_num > 0)
			{
				get_gain_treasure_mgr().set_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss, cur_kill_num - 1);
				get_gain_treasure_mgr().send_gain_treasure_info_one(e_gain_treasure_info_type_killed_field_boss);

				int32 cur_times = get_logic_data(e_role_logic_info_gain_treasure_restore_num);
				set_logic_data(e_role_logic_info_gain_treasure_restore_num, cur_times + 1);
				send_logic_one(e_role_logic_info_gain_treasure_restore_num);
				if ((cur_times + 1) < GAMECONFIG->BossHomeMaxRestoreNum && cur_kill_num - 1 > 0)
				{
					set_logic_data(e_role_logic_info_gain_treasure_restore_time, old_time + GAMECONFIG->BossHomeRestoreNeedTime);
				}
			}
			send_logic_one(e_role_logic_info_gain_treasure_restore_time);
		}
		if (get_logic_data(e_role_logic_info_sky_land_boss_restore_time) && new_time_second >= get_logic_data(e_role_logic_info_sky_land_boss_restore_time))
		{
			int32 old_time = get_logic_data(e_role_logic_info_sky_land_boss_restore_time);
			set_logic_data(e_role_logic_info_sky_land_boss_restore_time, 0);
			int32 cur_kill_num = get_boss_island_mgr().get_boss_island_info(e_island_record_killed_island_boss);
			if (cur_kill_num > 0)
			{
				get_boss_island_mgr().set_boss_island_info(e_island_record_killed_island_boss, cur_kill_num - 1);
				get_boss_island_mgr().send_player_boss_island_info_one(e_island_record_killed_island_boss);

				int32 cur_times = get_logic_data(e_role_logic_info_sky_land_boss_restore_num);
				set_logic_data(e_role_logic_info_sky_land_boss_restore_num, cur_times + 1);
				send_logic_one(e_role_logic_info_sky_land_boss_restore_num);
				if ((cur_times + 1) < GAMECONFIG->SkyIsLandMaxRestoreNum && cur_kill_num - 1 > 0)
				{
					set_logic_data(e_role_logic_info_sky_land_boss_restore_time, old_time + GAMECONFIG->SkyIsLandRestoreNeedTime);
				}
			}
			send_logic_one(e_role_logic_info_sky_land_boss_restore_time);
		}
	}

	void player::add_legion_dance_daily_must()
	{
		ZoneScoped;
		set_must_do_count(e_daily_must_do_typ_legion_dance, get_must_do_count(e_daily_must_do_typ_legion_dance) + 1);
		send_daily_must_do_count_info_one(e_daily_must_do_typ_legion_dance);
		set_daily_active_degree_info(e_daily_must_do_typ_legion_dance);
		get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_legion_dance);  //����ʱ��Ϊ���Ź������
	}

	bool player::add_money_or_exp_with_string(e_money_type money_type, std::string money_value, e_server_log_add_money add_type, int32 param)
	{
		ZoneScoped;
		int64 tmp_money_value = init_unit::change_string_to_i64(money_value);
		return add_money_or_exp(money_type, tmp_money_value, add_type, param);
	}

	void player::send_upgrade_wing_notify_with_item(int32 NoticeId, int32 WingNameId, int32 UpgradeNow, citem* item_ptr)
	{
		ZoneScoped;
		s_item_info item_data[chat_max_item];
		int32 item_num = 0;

		int32 notice_id = NoticeId;

		std::string notice_str_id = template_manager::get_instance().get_str_id_by_notice_id(notice_id);
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		notice_str_params_vec.push_back(get_name());
		notice_str_params_vec.push_back("%d");
		notice_str_params_vec.push_back(template_manager::get_instance().int_to_string(UpgradeNow));
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		item_data[item_num] = item_ptr->get_item_inst();
		get_chat_mgr().send_notice(notice_id, notice_str, item_data, item_num);
	}

	void player::skill_cast_by_skill_id(int32 skill_id)
	{
		ZoneScoped;
		use_skill_param param;
		param.sender_unit_idf = get_identifier();
		param.target_unit_idf = get_identifier();
		param.skill_template_id = skill_id;
		param.pos = get_new_map_pos().unit_location;
		param.yaw = get_new_map_pos().unit_location.y;
		param.skill_order = 1;
		skill_manager::skill_cast(param);
	}


	void player::get_log_common_head_info(s_log_common_head& log_head)
	{
		ZoneScoped;
		set_log_common_head_part1(log_head, SERVERCONFIG->app_key, m_login_third_data.param7, m_login_third_data.param4, m_login_third_data.param5);
		set_log_common_head_part2(log_head, m_login_third_data.param2, 0);
		set_log_common_head_part3(log_head, m_role_info.account, m_role_info.data_ary[e_role_info_server_id], utility::get_tick_count())
			set_log_common_head_part4(log_head, m_role_info.role_guid, m_role_info.role_name, m_role_info.data_ary[e_role_info_exp_level], m_role_info.data_ary[e_role_info_gender])
			set_log_common_head_part5(log_head, m_role_info.data_i64_ary[e_role_i64_info_gs_value], m_role_info.data_ary[e_role_info_class_type], m_role_info.data_ary[e_role_info_vip_level])
	}

	s_log_common_head& player::get_share_log_head()
	{
		ZoneScoped;
		static s_log_common_head share_log_head;
		set_log_common_head_part1(share_log_head, SERVERCONFIG->app_key, m_login_third_data.param7, m_login_third_data.param4, m_login_third_data.param5);
		set_log_common_head_part2(share_log_head, m_login_third_data.param2, 0);
		set_log_common_head_part3(share_log_head, m_role_info.account, m_role_info.data_ary[e_role_info_server_id], utility::get_tick_count())
			set_log_common_head_part4(share_log_head, m_role_info.role_guid, m_role_info.role_name, m_role_info.data_ary[e_role_info_exp_level], m_role_info.data_ary[e_role_info_gender])
			set_log_common_head_part5(share_log_head, m_role_info.data_i64_ary[e_role_i64_info_gs_value], m_role_info.data_ary[e_role_info_class_type], m_role_info.data_ary[e_role_info_vip_level])
			return share_log_head;
	}

	void player::set_role_account(xstring temp_account)
	{
		ZoneScoped;
		m_role_info.set_role_account(temp_account);
	};

	fvector player::get_position_by_chase(f32 skill_distance, fvector npc_position)
	{
		ZoneScoped;
		fvector player_location = get_new_map_pos().unit_location;
		fvector ret_location;

		int32 random_index = random_gen::get_random(0, NPC_NUM_AROUND_PLAYER / 4 - 1);
		int32 sub_distance_min = skill_distance * 0.2 * -1;
		int32 sub_distance_max = skill_distance * 0.1;
		int32 random_distance = random_gen::get_random(sub_distance_min, sub_distance_max);

		skill_distance = skill_distance * 0.7;

		fvector temp_fv = player_location - npc_position;
		if (temp_fv.x < 0)
		{
			if (temp_fv.y < 0)
			{
				random_index += NPC_NUM_AROUND_PLAYER / 4 * 3;	// ������
			}
		}
		else
		{
			if (temp_fv.y < 0)
			{
				random_index += NPC_NUM_AROUND_PLAYER / 4 * 2;	// ������
			}
			else
			{
				random_index += NPC_NUM_AROUND_PLAYER / 4;		// ������
			}
		}

		skill_distance += random_distance;
		ret_location.z = npc_position.z;
		ret_location.x = player_location.x + skill_distance * cos(2 * FAITH_PI * random_index / NPC_NUM_AROUND_PLAYER);
		ret_location.y = player_location.y - skill_distance * sin(2 * FAITH_PI * random_index / NPC_NUM_AROUND_PLAYER);

		return ret_location;
	};

	void player::show_vip(int32 is_show_vip)
	{
		ZoneScoped;
		if (is_show_vip < 0 || is_show_vip > 1)
		{
			set_unit_info(faith::e_role_info_is_show_vip, 1);
		}
		else
		{
			set_unit_info(faith::e_role_info_is_show_vip, is_show_vip);
		}
		send_info_one(faith::e_role_info_is_show_vip, true);
		sync_data_to_ws(e_sync_cs2ws_data_is_show_vip, is_show_vip);
	}
	void player::enchant_show_type(int32 show_type)
	{
		ZoneScoped;
		if (show_type <= 0)
		{
			return;
		}
		get_item_set().equip_off_enchant_show_buff();
		set_unit_info(faith::e_role_info_enchant_show_type, show_type);
		get_item_set().equip_on_enchant_show_buff();
		send_info_one(e_role_info_enchant_show_type);
	}
	void player::subscribe_daily(int32 must_do_type)
	{
		ZoneScoped;
		for (int32 i = 0; i < m_subscribe_daily_list.size(); ++i)
		{
			if (m_subscribe_daily_list[i].must_do_type == must_do_type)
			{
				send_subscribe_daily_end(e_subscribe_daily_end_type_have);
				return;
			}
		}

		DailyActivitiesTemplate* template_ptr = template_manager::get_instance().get_template_by_daily_active_degree_type(must_do_type);
		if (template_ptr == nullptr)
		{
			send_subscribe_daily_end(e_subscribe_daily_end_type_have);
			return;
		}

		if (template_ptr->SubscribeRewardId <= 0 || template_ptr->InMapList.size() <= 0)
		{
			send_subscribe_daily_end(e_subscribe_daily_end_type_have);
			return;
		}
		// ��������
		s_subscribe_daily_info new_info;
		new_info.must_do_type = must_do_type;
		new_info.info_state = e_subscribe_daily_info_state_open;
		// ���ӱ���
		m_subscribe_daily_list.push_back(new_info);
		//���ͱ������

		send_subscribe_daily_info();
		send_subscribe_daily_end(e_subscribe_daily_end_type_succeed);

		ActivityCommonConfigTemplate* activity_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, template_ptr->ActivityOrder);
		if (activity_ptr == nullptr)
		{
			return;
		}

		std::string notice_str_id = "90308043";
		std::vector<std::string> notice_str_params_vec;
		notice_str_params_vec.push_back(notice_str_id);
		notice_str_params_vec.push_back(template_manager::get_instance().get_str_by_string_template_id(activity_ptr->ActivityName));
		std::string notice_str = init_unit::implode(notice_str_params_vec);
		send_notice(notice_str);
	}
	void player::check_subscribe_daily()
	{
		ZoneScoped;
		int32 cur_map_id = get_unit_info(e_role_info_move_map_id);
		for (int32 i = 0; i < m_subscribe_daily_list.size(); ++i)
		{
			s_subscribe_daily_info& tem_info = m_subscribe_daily_list[i];
			if (tem_info.info_state >= e_subscribe_daily_info_state_end)
			{
				continue;
			}
			DailyActivitiesTemplate* template_ptr = template_manager::get_instance().get_template_by_daily_active_degree_type(tem_info.must_do_type);
			if (template_ptr == nullptr)
			{
				continue;
			}
			ActivityCommonConfigTemplate* activity_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, template_ptr->ActivityOrder);
			if (activity_ptr == nullptr)
			{
				continue;
			}
			if (cell_server::getInstance().get_activity_sec_left(activity_ptr->ActivityType, e_activity_time_get_all) > 0)
			{
				for (int32 j = 0; j < template_ptr->InMapList.size(); ++j)
				{
					if (cur_map_id == template_ptr->InMapList[j])
					{
						tem_info.info_state = e_subscribe_daily_info_state_end;
						item_set& temp_item_set = get_item_set();
						std::vector<citem*> item_list;
						std::vector<int32> item_list_arr = init_unit::reward_regional_diff(template_ptr->SubscribeRewardId);
						if (item_list_arr.size() / 2 > 0)
						{
							for (int32 y = 0; y < item_list_arr.size() / 2; y++)
							{
								int32 award_template_id = item_list_arr[y * 2];
								int32 award_num = item_list_arr[y * 2 + 1];
								citem* award_item = temp_item_set.create_item_by_template(e_server_log_add_item_mail, 0, award_template_id, award_num, 1);
								if (nullptr != award_item)
								{
									item_list.push_back(award_item);
								}
							}
						}

						std::vector<std::string> content_params;

						content_params.push_back("90308039");
						content_params.push_back(template_manager::get_instance().get_str_by_string_template_id(activity_ptr->ActivityName));
						std::string contenttext = init_unit::implode(content_params);

						std::string title = "90308038";
						get_mail_mgr().send_mail_to_another_player_by_system(get_unit_guid(), get_unit_info(e_role_info_server_id), title, contenttext, 0, 0, 0, 0, item_list);
						send_subscribe_daily_info();
						break;
					}
				}
			}


		}
	}
	void player::load_subscribe_daily_info_end(const s_subscribe_daily_db_info* dp_info, int32 data_num)
	{
		ZoneScoped;
		if (nullptr == dp_info)
		{
			return;
		}
		// ���������Ϣ
		m_subscribe_daily_list.clear();
		for (int32 i = 0; i < data_num; ++i)
		{
			m_subscribe_daily_list.push_back(dp_info[i]._info);
		}
		send_subscribe_daily_info();
	}
	void player::save_subscribe_daily_info(e_save_role_data_type eType)
	{
		ZoneScoped;
		guid_64 role_guid = get_unit_guid();
		cs2dp_save_subscribe_daily_info_to_db msg;
		msg.role_guid = role_guid;
		msg.save_type_ex = eType;
		msg.unit_array_index = m_array_index;
		msg.date_num = 0;
		int32 data_num = 0;
		for (int32 i = 0; i < m_subscribe_daily_list.size() && i < time_limit_gift_db_num; ++i)
		{
			msg.info_list[i].role_guid = role_guid;
			msg.info_list[i]._info = m_subscribe_daily_list[i];
			++data_num;
		}
		msg.date_num = data_num;
		send_message_to_dp(&msg, e_msgindex_cs2dp_save_subscribe_daily_info_to_db);
	}
	void player::send_subscribe_daily_info()
	{
		ZoneScoped;
		chat_proto_send_subscribe_daily_info msg;
		for (int32 i = 0; i < m_subscribe_daily_list.size(); ++i)
		{
			s_subscribe_daily_info& _info = m_subscribe_daily_list[i];
			chat_proto_subscribe_daily_info* tem_info = msg.add_info_list();
			if (nullptr == tem_info)
			{
				continue;
			}
			tem_info->set_dailt_type(_info.must_do_type);
			tem_info->set_info_state(_info.info_state);
		}
		send_message_to_self(&msg, e_msgindex_s2c_send_subscribe_daily_info);
	}
	void player::send_subscribe_daily_end(int32 end_type)
	{
		ZoneScoped;
		chat_proto_send_subscribe_daily_end info_ptr;
		info_ptr.set_result(end_type);
		send_message_to_self(&info_ptr, e_msgindex_s2c_send_subscribe_daily_end);
	}
	void player::create_obj_lua()
	{
		ZoneScoped;
#ifdef  OPEN_LUA_DATA

		if (is_valid() == false)
		{
			return;
		}
		script_mgr::get_instance().call_func(nullptr, "create_player", 0, false, "%d%l", get_array_index(), get_unit_guid().server_64);
#endif

	}
	void player::del_obj_lua()
	{
		ZoneScoped;
#ifdef OPEN_LUA_DATA

		if (is_valid() == false)
		{
			return;
		}
		script_mgr::get_instance().call_func(nullptr, "del_player", 0, false, "%d", get_array_index());
#endif

	}

	void player::save_obj_lua(int32 eType)
	{
		ZoneScoped;
#ifdef  OPEN_LUA_DATA
		if (is_valid() == false)
		{
			return;
		}
		script_mgr::get_instance().call_func(nullptr, "save_player", 0, false, "%d%d", get_array_index(), eType);
#endif
	}
}
