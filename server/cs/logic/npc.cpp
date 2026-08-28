/********************************************************************
	created:	2015/03/26
	created:	26:3:2015   14:57
	file base:	npc
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "aoi/aoi_system.h"
#include "cell_server.hpp"
#include "components/npc/box_award_component.h"
#include "cross_server_world_boss_def.hpp"
#include "fake_player_def.hpp"
#include "internet/item.pb.h"
#include "logic/buff_man.h"
#include "logic/item_set.h"
#include "logic/npc.hpp"
#include "logic/player.hpp"
#include "logic/skill_manager.h"
#include "logic/skill_set.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "utility/random.h"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "system/npc/box_award_system.h"
#include "system/npc/treasure_pic_system.h"
#include "system/play/wheel_plate_system.h"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include "threat_drop_processor.h"
#include "internet/aoi.pb.h"
#include "internal/world_boss_msg.hpp"
#include "internet/game.pb.h"
#include "internet/net.pb.h"
#include "internet/assist_fight.pb.h"
namespace hld
{
	npc::npc()
	{
	}
	
	npc::~npc()
	{
	}
	void npc::set_owner(const s_unit_identifier& unit_identifier)
	{
		m_owner_unit_identifier = unit_identifier;
		unit& unit_ref = unit_man::get_unit(m_owner_unit_identifier);
		if (unit_ref.is_valid())
		{
			if (!is_summon_pet() && get_unit_sub_type() != e_monster_type_robot_player)
				set_unit_info(e_role_info_upgrade_id, m_npc_template_ptr->UpgradeId + unit_ref.get_unit_info(e_role_info_exp_level));
			else
				set_unit_info(e_role_info_upgrade_id, m_npc_template_ptr->UpgradeId);
			set_unit_info(e_role_info_server_id, unit_ref.get_unit_info(e_role_info_server_id));
			if (!is_summon_pet())
				get_pawn_att().set_game_att(e_unit_game_att_movement, unit_ref.get_pawn_att().get_game_att(e_unit_game_att_movement), true);
			if (m_npc_template_ptr->UseOwnerAtt)
			{
				for (int32 i = e_unit_attack_att_attack_min; i < e_unit_attack_att_max; ++i)
				{
					get_pawn_att().apply_att_change_effect(4, i, unit_ref.get_pawn_att().get_attack_att_value(i), 0, send_gs_to_client, true);
				}
			}
		}
	}

	e_dropbox_owner_type npc::get_owner_type()
	{
		return m_owner_type;
	}

	void npc::set_owner_type(e_dropbox_owner_type owner_type)
	{
		m_owner_type = owner_type;
	}

	npc* npc::cast(unit* unit_ptr)
	{
		npc* ret = nullptr;
		if (unit_ptr && unit_ptr->is_valid())
		{
			if (unit_ptr->get_unit_type() != e_unit_type_player)
			{
				ret = static_cast<npc*>(unit_ptr);
			}
		}
		return ret;
	}

	void npc::heart_tick(const int64& new_time, const int32& tick_time)
	{
		if (m_show_map == false)
		{
			return;
		}
		unit::heart_tick(new_time, tick_time);
		if (get_unit_type() == e_unit_type_drop_bag || is_dead() || m_stop_ai)
		{
		}
		else
		{
			m_ai_unit.ai_tick(new_time);
		}

		if (m_is_life_change)
		{
			if (m_life_time < new_time)
			{
				m_is_life_change = false;
				if (get_unit_type() == e_unit_type_drop_bag)
				{
					std::vector<s_item_template_info> drop_list_vec;
					drop_list_vec.clear();
					const guid_64& player_guid = get_owner().unit_guid;
					player& temp_player = unit_man::get_player(player_guid);
					if (temp_player.is_valid())
					{
						ItemTemplate* item_ptr = GET_TEMPLATE(ItemTemplate, get_item_id());
						if (item_ptr)
						{
							if (item_ptr->item_color >= e_item_color_purple)
							{
								s_item_template_info item_drop_list_with_num = temp_player.get_item_set().get_const_att_item_by_id(get_item_id(), 1, 0, 0, temp_player.get_unit_info(e_role_info_class_type), temp_player.get_unit_info(e_role_info_exp_level));
								globle_data::get_instance().add_to_drop_list(drop_list_vec, item_drop_list_with_num);
							}
						}
					}

					if (drop_list_vec.size() > 0)
					{
						std::vector<std::string> content_params_title;
						content_params_title.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_get_remaining_title));
						std::string title = init_unit::implode(content_params_title);

						std::vector<std::string> content_params_contenttext;
						content_params_contenttext.push_back(globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_get_remaining_context));
						std::string contenttext = init_unit::implode(content_params_contenttext);
						globle_data::get_instance().send_mail_with_item(temp_player.get_unit_guid(), temp_player.get_unit_info(e_role_info_server_id), drop_list_vec, title, contenttext);
					}
				}

				unit_man::remove_npc(get_array_index(), false);
			}
		}
		tick_clamor(new_time);

		if (get_unit_type() == e_unit_type_monster && !is_dead()
			&& init_unit::is_world_boss(get_unit_sub_type()))
		{
			refresh_npc_left_hp_per();
		}

		if (new_time - m_last_sec_stamp >= second_tick_time)
		{
			m_last_sec_stamp = new_time;
			boss_remove_if_crazy();
			send_plot_with_npc_hp();
			check_can_see_npc_state();
		}
		if (new_time - m_last_2sec_stamp >= second_tick_time * 4)
		{
			m_last_2sec_stamp = new_time;
			send_boss_damage_list_to_all_player();
		}
		if (new_time - m_last_3sec_stamp >= second_tick_time * 5)
		{
			m_last_3sec_stamp = new_time;
			send_damage_score_to_ws();
			send_legion_score_to_ws();
		}
		if (m_next_come_back_time > 0 && new_time >= m_next_come_back_time)
		{
			fuhuo_npc();
		}
	}

	void npc::clear_data()
	{
		box_award_system::shut_down(this);
		wheel_plate_system::shut_down(this);
		treasure_pic_system::shut_down(this);
		m_ai_unit.clear_data();
		m_kill_array_index = 0;
		m_spawn_point_id = 0;
		m_life_time = 0;
		m_is_life_change = false;
		m_born_type = 0;
		m_show_map = false;
		m_show_weapon_id = 0;
		m_robot_template_id = 0;
		m_init_poroperty_ratio.clear();
		m_init_property_plus.clear();
		m_owner_unit_identifier.clear_data();
		m_npc_template_ptr = nullptr;
		m_next_clamor_time = 0;
		m_next_clamor_tick = 0;
		battle_begin_time = 0;
		last_hp_scale_flag = 110;
		m_residue_born_time = 0;
		unit::clear_data();
		m_last_sec_stamp = 0;
		m_player_info_arr.clear();
		m_score_rank_indicator.clear_data();
		m_legion_score_rank_indicator.clear_data();
		m_player_got_damage_reward_map.clear();
		m_drop_with_npc_temp_id = 0;
		m_last_2sec_stamp = 0;
		m_last_3sec_stamp = 0;
		m_last_send_plot_hp = 101;
		m_stop_ai = false;
		m_next_come_back_time = 0;
		m_can_see_player_guid.clear_data();
		m_continuity_kill_end_time_stamp = 0;
		m_kill_num_now = 0;
		m_is_player_data_npc = false;
		m_be_kill_num = 0;
		m_assist_fight_guid.clear_data();
		m_is_be_assist_fight_npc = false;
		m_attack_city_npc_type = -1;
	}

	void npc::boss_remove_if_crazy()
	{
		if (init_unit::is_world_boss(get_unit_sub_type()) == false)
		{
			return;
		}
		if (m_ai_unit.is_war_state() == false && true == get_pawn_att().get_state_att(e_unit_attack_state_crazy))
		{
			unit_man::remove_npc(m_array_index, false);
		}
	}
	bool npc::init_cross_ladder_npc(int32 npc_template_id, s_map_pos born_pos, Entity* map_ent, int32 map_template_id, int32 line_id, const s_map_pos& patrol_pos, std::string& name_str, int32 faker_player_template_id, bool need_show_fasion)
	{
		m_npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_template_id);
		if (m_npc_template_ptr == nullptr)
		{
			return false;
		}
		m_unit_type = (e_unit_type)m_npc_template_ptr->NpcType;
		m_unit_sub_type = m_npc_template_ptr->SubType;
		m_status = e_session_status_in_gaming;
		m_life_time = init_unit::get_end_time(m_npc_template_ptr->LifeTime);
		m_map_ent = map_ent;
		m_line_id = line_id;
		if (FLOAT_IS_ZERO(m_npc_template_ptr->LifeTime) == false)
		{
			m_is_life_change = true;
		}
		static s_unit_info role_info;
		role_info.clear_data();
		role_info.role_guid = m_role_info.role_guid;
		role_info.data_ary[e_role_info_template_id] = faker_player_template_id; //假数据
		role_info.data_ary[e_role_info_gender] = m_npc_template_ptr->Sex;
		role_info.data_ary[e_role_info_class_type] = m_npc_template_ptr->Class;
		role_info.data_ary[e_role_info_wing_showd_template_id] = m_npc_template_ptr->BornWing;
		role_info.data_ary[e_role_info_mount_showd_template_id] = m_npc_template_ptr->BornMount;
		role_info.data_ary[e_role_info_move_map_id] = map_template_id;
		role_info.set_role_name(name_str);

		int32 fake_level = faker_player_template_id % 100 + m_npc_template_ptr->Level / 100 * 100;
		if (fake_level < 100)
			fake_level += 100;
		role_info.data_ary[e_role_info_upgrade_id] = m_npc_template_ptr->UpgradeId;
		role_info.data_ary[e_role_info_exp_level] = fake_level;

		m_born_type = m_npc_template_ptr->BornType;
		get_pawn_att().set_game_att(e_unit_game_att_movement, m_npc_template_ptr->IsRide, true);
		get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_peace, false);
		m_pawn_att.set_unit_base_att(e_base_att_info_community_type, m_npc_template_ptr->BornCommunity);
		m_pawn_att.set_be_hurt_type(m_npc_template_ptr->BeHurtType);
		m_pawn_att.set_use_owner_att(m_npc_template_ptr->UseOwnerAtt);
		m_init_poroperty_ratio = m_npc_template_ptr->PropertyRatio;
		m_init_property_plus = m_npc_template_ptr->PropertyPlus;
		set_unit_info_all(role_info);
		if (m_npc_template_ptr->Capsule.size() >= 2)
		{
			m_unit_capsule = m_npc_template_ptr->Capsule[1];
			born_pos.unit_location.z += m_npc_template_ptr->Capsule[0] - 110;//减掉一个用玩家测量时玩家的高度
			m_unit_half_height = m_npc_template_ptr->Capsule[0];
		}
		set_new_map_pos(born_pos);
		set_old_map_pos(born_pos);
		m_ai_unit.init_ai(this, born_pos, patrol_pos, m_npc_template_ptr, get_map_ent());
		m_ai_unit.set_has_combo_skill(true);
		item_proto_item_save_load load_proto;
		static s_item_info item_info;
		for (int32 i = 0; i < m_npc_template_ptr->BornItem.size(); ++i)
		{
			item_info.clear_data();
			if (init_unit::init_item_data(item_info, m_npc_template_ptr->BornItem[i], 0, 1, e_bag_type_equip, 1))
			{
				load_proto.add_item_guid(item_info.item_guid.server_64);
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					load_proto.add_data_ary(item_info.data_ary[j]);
				}
				load_proto.add_param_str(item_info.param_str);
			}
		}
		item_info.clear_data();
		if (init_unit::init_item_data(item_info, m_npc_template_ptr->BornWing, 0, 1, e_bag_type_wing, 1))
		{//初始化翅膀
			load_proto.add_item_guid(item_info.item_guid.server_64);
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				load_proto.add_data_ary(item_info.data_ary[j]);
			}
			load_proto.add_param_str(item_info.param_str);
		}

		if (need_show_fasion)
		{
			PlayerInitTemplate* player_init_ptr = GET_TEMPLATE(PlayerInitTemplate, faker_player_template_id);
			if (player_init_ptr != nullptr && player_init_ptr->InitFashion.empty() == false)
			{
				int32 fake_fashion_id = 0;
				for (auto fashion_iter = player_init_ptr->InitFashion.begin(); fashion_iter != player_init_ptr->InitFashion.end();++fashion_iter)
				{
					ItemTemplate* temp_item_ptr = GET_TEMPLATE(ItemTemplate, *fashion_iter);
					if (temp_item_ptr != nullptr)
					{
						if (temp_item_ptr->sub_type == e_fashion_show_type_cloth)
							fake_fashion_id = *fashion_iter;
					}
				}
				if (fake_fashion_id != 0)
				{
					role_info.data_ary[e_role_info_show_fashion] = fake_fashion_id;

					item_info.clear_data();
					//e_bag_type_equip_fasion, e_fashion_show_type_cloth
					if (init_unit::init_item_data(item_info, fake_fashion_id, e_fashion_show_type_cloth, 1, e_bag_type_equip_fasion, 1))
					{
						item_info.data_ary[e_item_info_slot] = e_fashion_show_type_cloth;
						load_proto.add_item_guid(item_info.item_guid.server_64);
						for (int32 j = 0; j < e_item_info_max; ++j)
						{
							load_proto.add_data_ary(item_info.data_ary[j]);
						}
						load_proto.add_param_str(item_info.param_str);
					}
				}
			}
		}

		item_info.clear_data();
		if (init_unit::init_item_data(item_info, m_npc_template_ptr->BornMount, 0, 1, e_bag_type_mount, 1))
		{//初始化坐骑 显示原型，不幻化
			load_proto.add_item_guid(item_info.item_guid.server_64);
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				load_proto.add_data_ary(item_info.data_ary[j]);
			}
			load_proto.add_param_str(item_info.param_str);
		}
		get_item_set().load_item_by_db(load_proto);
		static s_spirit_info init_spirit[1];
		init_spirit[0].clear_data();
		if (init_unit::init_spirit_data(init_spirit[0], m_npc_template_ptr->BornSpirit, 0, e_spirit_bag_type_equiped, 1))
		{//初始精灵
			//m_spirit_mgr.load_spirit_by_db(init_spirit, 1);
		}

		battle_begin_time = 0;
		return true;
	}

	void npc::record_treasure_log(const int32 map_type)
	{
		damage_info_vector damage_list = m_ai_unit.get_damage_list();
		int32 list_len = damage_list.size();
		for (int32 idx = 0; idx < list_len; idx++)
		{
			const s_damage_info& damage_info = damage_list[idx];

			guid_64 play_guid = damage_info.unit_identifier.unit_guid;
			if (play_guid.is_valid())
			{
				player& player_ref = unit_man::get_player(play_guid);
				if (player_ref.is_valid() && player_ref.get_write_log())
				{
					//助战不记录打宝日志
					if (player_ref.get_assist_fight_mgr().is_assist_helper())
					{
						continue;
					}
					int32 treasure_type = player_ref.get_vip_level();
					int32 kill_field_num = player_ref.get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_field_boss) + player_ref.get_logic_data(e_role_logic_info_gain_treasure_restore_num);;	//地宫boss击杀次数
					int32 kill_simple_num = player_ref.get_gain_treasure_mgr().get_gain_treasure_info(e_gain_treasure_info_type_killed_simple_boss);//个人boss击杀次数
					int32 kill_home_num = player_ref.get_logic_data(e_role_logic_info_boss_home_cur_used_times);									//boss之家击杀次数
					int32 treasure_num = kill_field_num + kill_simple_num + kill_home_num + 1;

					set_log_var(log_head)
					player_ref.get_log_common_head_info(log_head);
					server_log::serverTreasureLog(log_head, treasure_type, get_npc_template_id(), treasure_num);	
				}	
			}
		}	
	}

	void npc::record_world_boss_log()
	{
		damage_info_vector damage_list = m_ai_unit.get_damage_list();
		int32 list_len = damage_list.size();
		for (int32 idx = 0; idx < list_len; idx++)
		{
			const s_damage_info& damage_info = damage_list[idx];
			guid_64 play_guid = damage_info.unit_identifier.unit_guid;
			if (play_guid.is_valid())
			{
				player& player_ref = unit_man::get_player(play_guid);
				if (player_ref.is_valid() && player_ref.get_write_log())
				{
					int32 is_self_server = 0;
					if (false == player_ref.is_self_server())
					{
						is_self_server = 1;
					}
					set_log_var(log_head)
					player_ref.get_log_common_head_info(log_head);
					server_log::serverBeatBoss(log_head, e_server_beat_boss_type_world_boss, is_self_server, get_npc_template_id());
				}
			}
		}
	}

	void npc::record_broken_boss_log()
	{
		damage_info_vector damage_list = m_ai_unit.get_damage_list();
		int32 list_len = damage_list.size();
		for (int32 idx = 0; idx < list_len; idx++)
		{
			const s_damage_info& damage_info = damage_list[idx];
			guid_64 play_guid = damage_info.unit_identifier.unit_guid;
			if (play_guid.is_valid())
			{
				player& player_ref = unit_man::get_player(play_guid);
				if (player_ref.is_valid() && player_ref.get_write_log())
				{
					int32 is_self_server = 0;
					if (false == player_ref.is_self_server())
					{
						is_self_server = 1;
					}
					set_log_var(log_head)
						player_ref.get_log_common_head_info(log_head);
					server_log::serverBeatBoss(log_head, e_server_beat_boss_type_broken_boss, is_self_server, get_npc_template_id());
				}
			}
		}
	}

	void npc::record_legion_boss_log()
	{
		damage_info_vector damage_list = m_ai_unit.get_damage_list();
		int32 list_len = damage_list.size();
		for (int32 idx = 0; idx < list_len; idx++)
		{
			const s_damage_info& damage_info = damage_list[idx];
			guid_64 play_guid = damage_info.unit_identifier.unit_guid;
			if (play_guid.is_valid())
			{
				player& player_ref = unit_man::get_player(play_guid);
				if (player_ref.is_valid() && player_ref.get_write_log())
				{
					int32 is_self_server = 0;
					if (false == player_ref.is_self_server())
					{
						is_self_server = 1;
					}
					set_log_var(log_head)
						player_ref.get_log_common_head_info(log_head);
					server_log::serverBeatBoss(log_head, e_server_beat_boss_type_legion_boss, is_self_server, get_npc_template_id());
				}
			}
		}
	}

	void npc::set_can_see_player_guid(guid_64 unit_guid)
	{
		m_can_see_player_guid = unit_guid;
	}

	guid_64 npc::get_can_see_player_guid()
	{
		return m_can_see_player_guid;
	}

	void npc::check_can_see_npc_state()
	{
		if (false == get_can_see_player_guid().is_valid())
		{
			return;
		}
		//检查如果不是当前任务怪就删除掉
		player& player_ref = unit_man::get_player(get_can_see_player_guid());
		if (player_ref.is_valid())
		{
			cmission* mission_ptr = player_ref.get_mission_mgr().find_mission_by_index(e_mission_slot_main);
			if (nullptr == mission_ptr)
			{
				be_dead();

				return;
			}
			if (false == mission_ptr->is_can_see_npc_guid(get_unit_guid()))
			{
				be_dead();
				return;
			}
		}
		else
		{
			//如果玩家不在线删除 上线后会重新创建
			be_dead();
		}
	}

	xstring npc::load_random_role_name()
	{
		xstring role_name = "Lucy";

		int32 first_name_index = random_gen::get_random(GAMECONFIG->CreateNameArray[0], GAMECONFIG->CreateNameArray[1]);
		int32 family_name_index = random_gen::get_random(GAMECONFIG->CreateNameArray[2], GAMECONFIG->CreateNameArray[3]);
		vector<xstring> first_name_array = template_manager::get_instance().get_string_list(first_name_index);
		vector<xstring> family_name_array = template_manager::get_instance().get_string_list(family_name_index);
		if (first_name_array.size() > 0)
		{
			role_name = first_name_array[0];
		}
		if (family_name_array.size() > 0)
		{
			role_name += family_name_array[0];
		}

		return role_name;
	}

	void npc::robot_be_dead()
	{
		unit& temp_send_unit = unit_man::get_unit(m_kill_array_index);
		if (false == temp_send_unit.is_valid())
		{
			return;
		}

		world_cs::npc_dead(get_map_ent(), this);
		bool is_end = m_kill_num_now >= GAMECONFIG->BeEndKillNeedNum;
		bool is_player = true;
		int32 array_index = m_kill_array_index;
		if (m_kill_array_index >= npc_arrary_index_begin)
		{
			is_player = false;
			npc& npc_ref = unit_man::get_npc(m_kill_array_index);
			if (npc_ref.is_valid())
			{
				player& player_ref = unit_man::get_player(npc_ref.get_owner());
				if (player_ref.is_valid())
				{
					is_player = true;
					array_index = player_ref.get_array_index();
				}
				else
				{
					npc_ref.send_kill_prompt(m_array_index);
				}

			}
		}
		if (is_player)
		{
			player& temp_send_player = unit_man::get_player(array_index);
			if (false == temp_send_player.is_valid())
			{
				return;
			}

			bool is_end = m_kill_num_now >= GAMECONFIG->BeEndKillNeedNum;
			temp_send_player.send_kill_prompt(m_array_index);
			MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, get_unit_info(e_role_info_move_map_id));
			if (map_template_ptr && map_template_ptr->Type == hld::e_map_type_big_map && temp_send_unit.get_unit_guid() != get_unit_guid())
			{
				//增加杀人数
				int32 kill_player_num = temp_send_player.get_unit_info(hld::e_role_info_kill_player_num);
				temp_send_player.set_unit_info(hld::e_role_info_kill_player_num, ++kill_player_num);
				temp_send_player.send_info_one(hld::e_role_info_kill_player_num);
				//增加杀人称号
				temp_send_player.get_title_mgr().add_title_by_type_and_value(hld::e_title_type_kill_player_num, kill_player_num);
			}
		}
	}
	
	void npc::send_kill_prompt(int32 kill_player_array_index, bool is_end)
	{
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
	}

	void npc::fuhuo_npc()
	{
		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(get_map_guid());
		if (nullptr == map_object_ptr || map_object_ptr->get_map_state() >= e_map_state_over)
		{
			m_kill_dead_reason = e_be_dead_self;
			be_dead();
			return;
		}
		s_map_pos fuhuo_pos = map_object_ptr->get_born_pos(get_array_index());
		if (fuhuo_pos.is_zero())
		{
			fuhuo_pos.set_location(m_ai_unit.get_bron_location());
		}

		set_new_map_pos(fuhuo_pos);
		m_ai_unit.clear_bron_data();
		set_ai_state(e_ai_state_stand);
		get_pawn_att().set_state_att(e_unit_attack_state_dead, false);
		full_hp();
		world_cs::add_npc(get_map_ent(), this);
		m_next_come_back_time = 0;
	}

	void npc::clear_bron_data()
	{
		m_ai_unit.clear_bron_data();
	}

	void npc::add_ai_level_next_time(int32 add_time)
	{
		m_ai_unit.add_level_nomal_next_time(add_time);
	}

	void npc::set_ai_unit_born_pos(s_map_pos born_pos)
	{
		m_ai_unit.set_level_nomal_born_pos(born_pos);
	}

	void npc::clear_hate_value()
	{
		m_ai_unit.clear_hate_value();
	}

	void npc::set_follow_guid(guid_64 role_guid)
	{
		m_ai_unit.set_follow_guid(role_guid);
	}

	guid_64 npc::get_follow_guid()
	{
		return m_ai_unit.get_follow_guid();
	}
	bool npc::init_by_template_data(int32 npc_template_id, s_map_pos born_pos, Entity* map_ent, int32 map_template_id, int32 line_id, player* player_ptr, const s_map_pos& patrol_pos, int32 show_wing_id)
	{
		ZoneScoped;
		m_npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_template_id);
		if (m_npc_template_ptr == nullptr)
		{
			CONSOLE_ERROR("npc_template_id is not exist npc_template_id:{}", npc_template_id);
			return false;
		}
		m_unit_type =(e_unit_type)m_npc_template_ptr->NpcType;
		m_unit_sub_type = m_npc_template_ptr->SubType;
		m_status = e_session_status_in_gaming;
		m_life_time = init_unit::get_end_time(m_npc_template_ptr->LifeTime);
		m_map_ent = map_ent;
		m_line_id = line_id;
		if (FLOAT_IS_ZERO(m_npc_template_ptr->LifeTime) == false)
		{
			m_is_life_change = true;
		}
		static s_unit_info role_info;
		role_info.clear_data();
		role_info.role_guid = m_role_info.role_guid;
		role_info.data_ary[e_role_info_template_id] = npc_template_id;
		role_info.data_ary[e_role_info_gender] = m_npc_template_ptr->Sex;
		role_info.data_ary[e_role_info_class_type] = m_npc_template_ptr->Class;
		role_info.data_ary[e_role_info_wing_showd_template_id] = m_npc_template_ptr->BornWing;
		role_info.data_ary[e_role_info_mount_showd_template_id] = m_npc_template_ptr->BornMount;
		role_info.data_ary[e_role_info_move_map_id] = map_template_id;

		if (m_npc_template_ptr->IsRandomRoleName > 0)
		{
			role_info.set_role_name(load_random_role_name());
		}

		
		if (get_unit_info(e_role_info_upgrade_id) == 0)
		{
			role_info.data_ary[e_role_info_upgrade_id] = m_npc_template_ptr->UpgradeId;
			role_info.data_ary[e_role_info_exp_level] = m_npc_template_ptr->Level;
		}
		else
		{
			role_info.data_ary[e_role_info_upgrade_id] = get_unit_info(e_role_info_upgrade_id);
			role_info.data_ary[e_role_info_exp_level] = get_unit_info(e_role_info_exp_level);
		}
		m_born_type = m_npc_template_ptr->BornType;
		get_pawn_att().set_game_att(e_unit_game_att_movement, m_npc_template_ptr->IsRide, true);
		get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_peace, false);
		m_pawn_att.set_unit_base_att(e_base_att_info_community_type, m_npc_template_ptr->BornCommunity);
		m_pawn_att.set_be_hurt_type(m_npc_template_ptr->BeHurtType);
		m_pawn_att.set_use_owner_att(m_npc_template_ptr->UseOwnerAtt);
		m_init_poroperty_ratio = m_npc_template_ptr->PropertyRatio;
		m_init_property_plus = m_npc_template_ptr->PropertyPlus;
		set_unit_info_all(role_info);
		if (m_npc_template_ptr->Capsule.size() >= 2)
		{
			m_unit_capsule = m_npc_template_ptr->Capsule[1];
			born_pos.unit_location.z += m_npc_template_ptr->Capsule[0] - 110;//减掉一个用玩家测量时玩家的高度
			m_unit_half_height = m_npc_template_ptr->Capsule[0];
		}
		set_new_map_pos(born_pos);
		set_old_map_pos(born_pos);
		m_ai_unit.init_ai(this, born_pos, patrol_pos, m_npc_template_ptr, get_map_ent());
		item_proto_item_save_load load_proto;
		static s_item_info item_info;
		for (int32 i = 0; i < m_npc_template_ptr->BornItem.size(); ++i)
		{
			item_info.clear_data();
			if (init_unit::init_item_data(item_info, m_npc_template_ptr->BornItem[i], 0, 1, e_bag_type_equip, 1))
			{
				load_proto.add_item_guid(item_info.item_guid.server_64);
				for (int32 j =0; j < e_item_info_max; ++j)
				{
					load_proto.add_data_ary(item_info.data_ary[j]);
				}
				load_proto.add_param_str(item_info.param_str);
			}
		}
		item_info.clear_data();
		if (init_unit::init_item_data(item_info, m_npc_template_ptr->BornWing, 0, 1, e_bag_type_wing, 1))
		{//初始化翅膀
			load_proto.add_item_guid(item_info.item_guid.server_64);
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				load_proto.add_data_ary(item_info.data_ary[j]);
			}
			load_proto.add_param_str(item_info.param_str);
		}
		if (show_wing_id != 0)
		{
			item_info.clear_data();
			if (init_unit::init_item_data(item_info, show_wing_id, 0, 1, e_bag_type_wing, 1))
			{//初始化翅膀
				load_proto.add_item_guid(item_info.item_guid.server_64);
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					load_proto.add_data_ary(item_info.data_ary[j]);
				}
				load_proto.add_param_str(item_info.param_str);
			}
			set_unit_info(e_role_info_wing_showd_template_id, show_wing_id);
		}

		item_info.clear_data();
		if (init_unit::init_item_data(item_info, m_npc_template_ptr->BornMount, 0, 1, e_bag_type_mount, 1))
		{//初始化坐骑 显示原型，不幻化
			load_proto.add_item_guid(item_info.item_guid.server_64);
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				load_proto.add_data_ary(item_info.data_ary[j]);
			}
			load_proto.add_param_str(item_info.param_str);
		}
		get_item_set().load_item_by_db(load_proto);
		static s_spirit_info init_spirit[1];
		init_spirit[0].clear_data();
		if (init_unit::init_spirit_data(init_spirit[0], m_npc_template_ptr->BornSpirit, 0, e_spirit_bag_type_equiped, 1))
		{//初始精灵
			//m_spirit_mgr.load_spirit_by_db(init_spirit, 1);
		}

		battle_begin_time = 0;

		box_award_system::start_up(player_ptr, this);
		wheel_plate_system::start_up(player_ptr, this);
		return true;
	}

	bool npc::init_fake_player_data(int32 npc_template_id, const s_fake_player_info& fake_data, Entity* map_ent, s_map_pos born_pos, int32 map_template_id, int32 npc_type)
	{
			m_npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_template_id);
			if (m_npc_template_ptr)
			{
				m_unit_type = (e_unit_type)m_npc_template_ptr->NpcType;
				m_unit_sub_type = npc_type;
				m_status = e_session_status_in_gaming;
				m_life_time = init_unit::get_end_time(m_npc_template_ptr->LifeTime);
				m_map_ent = map_ent;

				if (FLOAT_IS_ZERO(m_npc_template_ptr->LifeTime) == false)
				{
					m_is_life_change = true;
				}
				m_born_type = m_npc_template_ptr->BornType;

				s_unit_info role_info = fake_data.role_info;
				role_info.role_guid = m_role_info.role_guid;
				role_info.data_ary[e_role_info_class_type] = m_npc_template_ptr->Class;
				role_info.data_ary[e_role_info_move_map_id] = map_template_id;
				role_info.data_ary[e_role_info_wing_showd_template_id] = fake_data.role_info.data_ary[e_role_info_wing_showd_template_id];
				role_info.data_ary[e_role_info_show_fashion] = fake_data.role_info.data_ary[e_role_info_show_fashion];
				if (m_npc_template_ptr->IsRandomRoleName > 0)
				{
					role_info.set_role_name(load_random_role_name());
				}


				set_unit_info_all(role_info);
				if (m_npc_template_ptr->Capsule.size() >= 2)
				{
					m_unit_capsule = m_npc_template_ptr->Capsule[1];
					born_pos.unit_location.z += m_npc_template_ptr->Capsule[0] - 110;//减掉一个用玩家测量时玩家的高度
					m_unit_half_height = m_npc_template_ptr->Capsule[0];
				}
				set_new_map_pos(born_pos);
				set_old_map_pos(born_pos);

				s_map_pos patrol_pos = s_map_pos();
				m_ai_unit.init_ai(this, born_pos, patrol_pos, m_npc_template_ptr, get_map_ent());
				m_ai_unit.init_fake_player_info(fake_data);
				m_ai_unit.set_has_combo_skill(true);

				item_proto_item_save_load load_proto;
				int32 item_count = 0;

				for (int32 i = 0; i < fake_data.item_num; ++i)
				{
					if (item_count >= fake_item_num)
					{
						break;
					}
					s_item_info const& fake_item_data = fake_data.item_data[i];
					s_item_info item_info;
					if (init_unit::init_item_data(item_info,
						fake_item_data.data_ary[e_item_info_info_id],
						fake_item_data.data_ary[e_item_info_slot],
						1,
						(e_bag_type)fake_item_data.data_ary[e_item_info_container_type],
						fake_item_data.data_ary[e_item_info_activate]))
					{
						if (fake_item_data.data_ary[e_item_info_container_type] == e_bag_type_equip
							&& fake_item_data.data_ary[e_item_info_slot] == e_role_equip_slot_weapon_1)
						{
							ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, fake_item_data.data_ary[e_item_info_info_id]);

							if (item_template_ptr && item_template_ptr->ItemMesh > 0)
							{
								m_show_weapon_id = fake_item_data.data_ary[e_item_info_info_id];
							}
						}
						if (fake_item_data.data_ary[e_item_info_container_type] == e_bag_type_equip_fasion)
						{
							item_info.data_ary[e_item_info_slot] = fake_item_data.data_ary[e_item_info_slot];
						}
						load_proto.add_item_guid(item_info.item_guid.server_64);
						for (int32 j = 0; j < e_item_info_max; ++j)
						{
							load_proto.add_data_ary(item_info.data_ary[j]);
						}
						load_proto.add_param_str(item_info.param_str);
					}
				}
				get_item_set().load_item_by_db(load_proto);
				get_item_set().send_item_all();

				//get_item_set().calcu_addition_with_fake_player(e_addition_buff_upgrade);//强化洗练追加卓越
				//get_item_set().calcu_addition_with_fake_player(e_addition_buff_quality);
				//get_item_set().calcu_addition_with_fake_player(e_addition_buff_addon);
				//get_item_set().calcu_addition_with_fake_player(e_addition_buff_succinct);
				//get_item_set().refresh_element_heart_faker_player_buff();//符文buff
		
				{
					//在 load_base_att_by_db 之后设置假玩家的PK阵营 否则会被覆盖掉
					get_pawn_att().load_base_att_by_db(fake_data.att_info);
					get_pawn_att().load_unit_att_by_db(fake_data.m_fight_att);

				}

				//强制补满血 补血需要放在加buff之前
				m_pawn_att.set_unit_base_att(e_base_att_info_hp_cur, get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max));


				//get_spirit_mgr().load_spirit_by_db(&fake_data.spirit_data, 1);
				for (int32 i = 0; i < fake_data.belief_num; i++)
				{
					cbelief belief_ref;
					belief_ref.clear_data();
					belief_ref.set_belief_data(fake_data.belief_data[i]);
					int32 buff_id = belief_ref.get_buff_id();
					if (buff_id > 0)
					{
						buff_man::add_buff_inst(get_array_index(), get_array_index(), buff_id, nullptr, false);
					}
				}

				for (int32 i = 0; i < fake_data.buff_num; ++i)
				{
					int32 buff_id = fake_data.buff_data[i].data_ary[EBuffInfo_template_id];
					BuffDataTemplate* buff_ptr = GET_TEMPLATE(BuffDataTemplate, buff_id);
					if (buff_ptr && 0 == buff_ptr->NotInvalidFakerPlayer)
					{
						buff_man::add_buff_inst(get_array_index(), get_array_index(), buff_id, nullptr, true);
					}
				}
				get_pawn_att().set_game_att(e_unit_game_att_movement, m_npc_template_ptr->IsRide, true);
				get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_peace, false);
				m_pawn_att.set_unit_base_att(e_base_att_info_community_type, m_npc_template_ptr->BornCommunity);
				m_pawn_att.send_base_att_all();
				return true;
			}
		return false;
	}

	bool npc::init_player_data_robot(int32 npc_template_id, const s_fake_player_info& fake_data, Entity* map_ent, s_map_pos born_pos, int32 map_template_id, int32 npc_type)
	{
		m_npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_template_id);
		if (m_npc_template_ptr)
		{
			m_unit_type = (e_unit_type)m_npc_template_ptr->NpcType;
			m_unit_sub_type = npc_type;
			m_status = e_session_status_in_gaming;
			m_life_time = init_unit::get_end_time(m_npc_template_ptr->LifeTime);
			m_map_ent = map_ent;

			if (FLOAT_IS_ZERO(m_npc_template_ptr->LifeTime) == false)
			{
				m_is_life_change = true;
			}
			m_born_type = m_npc_template_ptr->BornType;

			s_unit_info role_info = fake_data.role_info;
			role_info.role_guid = m_role_info.role_guid;
			role_info.data_ary[e_role_info_template_id] = npc_template_id;
			role_info.data_ary[e_role_info_class_type] = m_npc_template_ptr->Class;
			role_info.data_ary[e_role_info_move_map_id] = map_template_id;
			role_info.data_ary[e_role_info_wing_showd_template_id] = fake_data.role_info.data_ary[e_role_info_wing_showd_template_id];
			role_info.data_ary[e_role_info_show_fashion] = fake_data.role_info.data_ary[e_role_info_show_fashion];
			role_info.data_ary[e_role_info_move_map_id] = map_template_id;
			if (m_npc_template_ptr->IsRandomRoleName > 0)
			{
				role_info.set_role_name(load_random_role_name());
			}


			set_unit_info_all(role_info);
			if (m_npc_template_ptr->Capsule.size() >= 2)
			{
				m_unit_capsule = m_npc_template_ptr->Capsule[1];
				born_pos.unit_location.z += m_npc_template_ptr->Capsule[0] - 110;//减掉一个用玩家测量时玩家的高度
				m_unit_half_height = m_npc_template_ptr->Capsule[0];
			}
			set_new_map_pos(born_pos);
			set_old_map_pos(born_pos);

			s_map_pos patrol_pos = s_map_pos();
			m_ai_unit.init_ai(this, born_pos, patrol_pos, m_npc_template_ptr, get_map_ent());
			m_ai_unit.init_fake_player_info(fake_data);
			m_ai_unit.set_has_combo_skill(true);

			m_is_player_data_npc = true;

			item_proto_item_save_load load_proto;
			int32 item_count = 0;

			for (int32 i = 0; i < fake_data.item_num; ++i)
			{
				if (item_count >= fake_item_num)
				{
					break;
				}
				s_item_info const& fake_item_data = fake_data.item_data[i];
				s_item_info item_info;
				if (init_unit::init_item_data(item_info,
					fake_item_data.data_ary[e_item_info_info_id],
					fake_item_data.data_ary[e_item_info_slot],
					1,
					(e_bag_type)fake_item_data.data_ary[e_item_info_container_type],
					fake_item_data.data_ary[e_item_info_activate]))
				{
					if (fake_item_data.data_ary[e_item_info_container_type] == e_bag_type_equip
						&& fake_item_data.data_ary[e_item_info_slot] == e_role_equip_slot_weapon_1)
					{
						ItemTemplate* item_template_ptr = GET_TEMPLATE(ItemTemplate, fake_item_data.data_ary[e_item_info_info_id]);

						if (item_template_ptr
							&& item_template_ptr->ItemMesh > 0)
						{
							m_show_weapon_id = fake_item_data.data_ary[e_item_info_info_id];
						}
					}
					if (fake_item_data.data_ary[e_item_info_container_type] == e_bag_type_equip_fasion)
					{
						item_info.data_ary[e_item_info_slot] = fake_item_data.data_ary[e_item_info_slot];
					}
					load_proto.add_item_guid(item_info.item_guid.server_64);
					for (int32 j = 0; j < e_item_info_max; ++j)
					{
						load_proto.add_data_ary(item_info.data_ary[j]);
					}
					load_proto.add_param_str(item_info.param_str);
				}
			}
			get_item_set().load_item_by_db(load_proto);
			get_item_set().send_item_all();
			get_pawn_att().load_base_att_by_db(fake_data.att_info);
			get_pawn_att().load_unit_att_by_db(fake_data.m_fight_att);
			//强制补满血 补血需要放在加buff之前
			m_pawn_att.set_unit_base_att(e_base_att_info_hp_cur, get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max));

			//get_spirit_mgr().load_spirit_by_db(&fake_data.spirit_data, 1);
			for (int32 i = 0; i < fake_data.belief_num; i++)
			{
				cbelief belief_ref;
				belief_ref.clear_data();
				belief_ref.set_belief_data(fake_data.belief_data[i]);
				int32 buff_id = belief_ref.get_buff_id();
				if (buff_id > 0)
				{
					buff_man::add_buff_inst(get_array_index(), get_array_index(), buff_id, nullptr, false);
				}
			}

			for (int32 i = 0; i < fake_data.buff_num; ++i)
			{
				int32 buff_id = fake_data.buff_data[i].data_ary[EBuffInfo_template_id];
				BuffDataTemplate* buff_ptr = GET_TEMPLATE(BuffDataTemplate, buff_id);
				if (buff_ptr && 0 == buff_ptr->NotInvalidFakerPlayer)
				{
					buff_man::add_buff_inst(get_array_index(), get_array_index(), buff_id, nullptr, true);
				}
			}
			get_pawn_att().set_game_att(e_unit_game_att_movement, m_npc_template_ptr->IsRide, true);
			get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_peace, false);
			m_pawn_att.set_unit_base_att(e_base_att_info_community_type, m_npc_template_ptr->BornCommunity);
			m_pawn_att.send_base_att_all();
			return true;
		}

		return false;
	}

	bool npc::init_by_robot_data(int32 robot_template_id, int32 npc_template_id, s_map_pos born_pos, Entity* map_ent, int32 map_template_id)
	{
		ArenaRobotTemplate* robot_template_ptr = GET_TEMPLATE(ArenaRobotTemplate, robot_template_id);
		m_npc_template_ptr = GET_TEMPLATE(NpcTemplate, npc_template_id);

		if (robot_template_ptr && m_npc_template_ptr)
		{
			m_robot_template_id = robot_template_id;
			m_unit_type = (e_unit_type)m_npc_template_ptr->NpcType;
			m_unit_sub_type = m_npc_template_ptr->SubType;
			m_status = e_session_status_in_gaming;
			m_life_time = init_unit::get_end_time(m_npc_template_ptr->LifeTime);
			m_map_ent = map_ent;
			if (FLOAT_IS_ZERO(m_npc_template_ptr->LifeTime) == false)
			{
				m_is_life_change = true;
			}
			
			s_unit_info role_info;
			role_info.role_guid = m_role_info.role_guid;
			role_info.data_ary[e_role_info_template_id] = npc_template_id;
			role_info.data_ary[e_role_info_gender] = m_npc_template_ptr->Sex;
			role_info.data_ary[e_role_info_class_type] = m_npc_template_ptr->Class;
			role_info.data_ary[e_role_info_exp_level] = robot_template_ptr->ExpLevel;
			role_info.data_ary[e_role_info_wing_showd_template_id] = robot_template_ptr->Wing;
			role_info.data_ary[e_role_info_move_map_id] = map_template_id;
			if (get_unit_info(e_role_info_upgrade_id) == 0)
			{
				role_info.data_ary[e_role_info_upgrade_id] = robot_template_ptr->UpgradeId;
			}
			else
			{
				role_info.data_ary[e_role_info_upgrade_id] = get_unit_info(e_role_info_upgrade_id);
			}
			if (m_npc_template_ptr->IsRandomRoleName > 0)
			{
				role_info.set_role_name(load_random_role_name());
			}
			m_born_type = m_npc_template_ptr->BornType;
			get_pawn_att().set_game_att(e_unit_game_att_movement, m_npc_template_ptr->IsRide, true);
			get_pk_community_mgr().set_entire_pk_mode(e_pk_mode_peace, false);
			m_pawn_att.set_unit_base_att(e_base_att_info_community_type, m_npc_template_ptr->BornCommunity);

			m_init_poroperty_ratio.clear();
			for (int32 i = 0; i < robot_template_ptr->PropertyRatio.size(); ++i)
			{
				m_init_poroperty_ratio.push_back(robot_template_ptr->PropertyRatio[i]);
			}

			m_init_property_plus.clear();
			for (uint32 i = 0; i < robot_template_ptr->PropertyPlus.size(); ++i)
			{
				m_init_property_plus.push_back(robot_template_ptr->PropertyPlus[i]);
			}

			set_unit_info_all(role_info);
			if (m_npc_template_ptr->Capsule.size() >= 2)
			{
				m_unit_capsule = m_npc_template_ptr->Capsule[1];
				born_pos.unit_location.z += m_npc_template_ptr->Capsule[0] - 110;//减掉一个用玩家测量时玩家的高度
				m_unit_half_height = m_npc_template_ptr->Capsule[0];
			}
			set_new_map_pos(born_pos);
			set_old_map_pos(born_pos);

			s_map_pos patrol_pos = s_map_pos();
			m_ai_unit.init_ai(this, born_pos, patrol_pos, m_npc_template_ptr, get_map_ent());
			m_ai_unit.set_has_combo_skill(true);

			item_proto_item_save_load load_proto;
			s_item_info item_info;
			for (int32 i = 0; i < robot_template_ptr->Items.size(); ++i)
			{
				item_info.clear_data();
				if (init_unit::init_item_data(item_info, robot_template_ptr->Items[i], 0, 1, e_bag_type_equip, 1))
				{
					load_proto.add_item_guid(item_info.item_guid.server_64);
					for (int32 j = 0; j < e_item_info_max; ++j)
					{
						load_proto.add_data_ary(item_info.data_ary[j]);
					}
					load_proto.add_param_str(item_info.param_str);
				}
			}
			item_info.clear_data();
			if (init_unit::init_item_data(item_info, robot_template_ptr->Wing, 0, 1, e_bag_type_wing, 1))
			{
				load_proto.add_item_guid(item_info.item_guid.server_64);
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					load_proto.add_data_ary(item_info.data_ary[j]);
				}
				load_proto.add_param_str(item_info.param_str);
			}
			get_item_set().load_item_by_db(load_proto);
			get_item_set().send_item_all();

			s_spirit_info init_spirit;
			if (robot_template_ptr->Sprite > 0
				&& init_unit::init_spirit_data(init_spirit, robot_template_ptr->Sprite, 0, e_spirit_bag_type_equiped, 1))
			{
				//get_spirit_mgr().load_spirit_by_db(&init_spirit, 1);
			}

			for (int32 i = 0; i < robot_template_ptr->EffectBuffArray.size(); ++i)
			{
				buff_man::add_buff_inst(get_array_index(), get_array_index(), robot_template_ptr->EffectBuffArray[i], nullptr, true);
			}

			return true;
		}
		return false;
	}

	void npc::init_att_over() 
	{
		if (m_npc_template_ptr)
		{
			for (int32 i = 0; i < m_npc_template_ptr->BornBuff.size(); ++i)
			{
				if (m_npc_template_ptr->BornBuff[i] > 0)
				{
					buff_man::add_buff_inst(get_array_index(), get_array_index(), m_npc_template_ptr->BornBuff[i]);
				}
			}
			int32 cur_level = get_unit_info(e_role_info_exp_level);
			if (m_npc_template_ptr->BornBuffLevel.size() > 0 && m_npc_template_ptr->BornBuffLevel.size() % 3 == 0)
			{
				for (int32 i = 0; i < m_npc_template_ptr->BornBuffLevel.size() / 3; i++)
				{
					if (cur_level >= m_npc_template_ptr->BornBuffLevel[i * 3]
						&& cur_level <= m_npc_template_ptr->BornBuffLevel[i * 3 + 1])
					{
						buff_man::add_buff_inst(get_array_index(), get_array_index(), m_npc_template_ptr->BornBuffLevel[i * 3 + 2]);
						break;
					}
				}
			}
		}

		if (init_unit::is_fake_player(m_unit_type, m_unit_sub_type))
		{
			get_item_set().set_equip_skill_buff_all();
			get_item_set().set_equip_att_all();
		}
		//玩家数据怪不需要初始化属性
		if (m_npc_template_ptr != nullptr && m_npc_template_ptr->ComeBackTime > 0)
		{
			if (m_be_kill_num <= 0 && m_is_player_data_npc == false)
			{
				m_pawn_att.init_unit_att_over(m_npc_template_ptr->UseOwnerAtt);
			}
		}
		else
		{
			m_pawn_att.init_unit_att_over(m_npc_template_ptr->UseOwnerAtt);
		}
		
	}

	void npc::set_level_and_upgrade_id(int32 new_level, int32 npc_temp_id)
	{
		NpcTemplate* npc_temp_ptr = GET_TEMPLATE(NpcTemplate, npc_temp_id);
		if (nullptr == npc_temp_ptr)
		{
			return;
		}
		int32 init_id = npc_temp_ptr->UpgradeId;
		int32 dynamic_upgrade_id = 0;
		int32 real_init_id = 0;
		real_init_id = init_id >= new_monster_upgrade_cfg_first_id ? new_monster_upgrade_cfg_first_id : monster_upgrade_cfg_first_id;
		
		dynamic_upgrade_id = real_init_id + new_level - 1;
		if (dynamic_upgrade_id < real_init_id)
		{
			dynamic_upgrade_id = real_init_id;
		}
		else if (dynamic_upgrade_id > (real_init_id + role_max_level - 1))
		{
			dynamic_upgrade_id = real_init_id + role_max_level - 1;
		}

		set_unit_info(e_role_info_exp_level, new_level);
		set_unit_info(e_role_info_upgrade_id, dynamic_upgrade_id);
	}

	std::vector<team_set> npc::get_aoi_team_hate_list()
	{
		std::vector<team_set> vec_team_set;
		hate_info_vector const& hate_list = m_ai_unit.get_hate_list();
		int32 list_size = hate_list.size();
		for (int32 list_index = 0; list_index < list_size; list_index++)
		{
			const s_hate_info& ref_hate_info = hate_list[list_index];
			player& ref_player = unit_man::get_player(ref_hate_info.unit_identifier);
			if (false == ref_player.is_valid())
			{
				continue;
			}
			if (ref_hate_info.hate_value <= 0)
			{
				continue;
			}

			bool has_join_in_team = false;
			const guid_64 team_guid = ref_player.get_team_guid();
			if (true == team_guid.is_valid())
			{
				const int32 vec_size = vec_team_set.size();
				for (int32 i = 0; i < vec_size; i++)
				{
					team_set& temp_team_set = vec_team_set[i];
					if (true == temp_team_set.join_team(&ref_player))
					{
						has_join_in_team = true;
						break;
					}
				}
			}
			if (false == has_join_in_team)
			{
				team_set new_team_set;
				new_team_set.init(team_guid);
				new_team_set.join_team(&ref_player);
				vec_team_set.push_back(new_team_set);
			}

		}

		// 处理aoi中的队友
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(get_map_ent(), get_old_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return vec_team_set;
		}
		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			const int32& unit_index = it->first;
			player& aoi_player_ref = unit_man::get_player(unit_index);
			if (aoi_player_ref.is_valid() == false)
			{
				continue;
			}
			if (false == aoi_player_ref.get_team_guid().is_valid())
			{
				continue;
			}

			const int32 vec_size = vec_team_set.size();
			for (int32 j = 0; j < vec_size; j++)
			{
				team_set& temp_team_set = vec_team_set[j];
				temp_team_set.join_team(&aoi_player_ref);
			}
		}

		return vec_team_set;
	}

	void npc::tick_clamor(const int64& tick_dt)
	{
		if (tick_dt < m_next_clamor_tick)
		{
			return;
		}
		m_next_clamor_tick = tick_dt + minute_tick_time;
		if (is_dead())
		{
			return;
		}
		if (nullptr == m_npc_template_ptr)
		{
			return;
		}
		int32 shouting_size = m_npc_template_ptr->Shouting.size();
		if (shouting_size <= 2)
		{
			return;
		}
		if (m_npc_template_ptr->Shouting[0] <= 0)
		{
			return;
		}

		int64 current_time = time_helper::get_cur_time_new().second;
		if (0 == m_next_clamor_time)
		{
			int64 temp_time = random_gen::get_random(0, 180);
			m_next_clamor_time = current_time + temp_time;
		}
		if (current_time > m_next_clamor_time)
		{
			int64 temp_time = random_gen::get_random(0, m_npc_template_ptr->Shouting[0]);
			m_next_clamor_time = current_time + temp_time;
			int32 random_index = random_gen::get_random(1, shouting_size - 1);
			npc_send_notice_with_location(m_npc_template_ptr->Shouting[random_index],nullptr, 0, m_npc_template_ptr->NpcName);
		}

	}

	void npc::npc_send_notice_with_location(int32 notice_id, std::string* string_param, int32 string_param_num, int32 sender_name_id)
	{
		if (m_npc_template_ptr == nullptr)
		{
			return;
		}
		if (notice_id <= 0)
		{
			return;
		}
		std::vector<std::string> vec_notice_str;
		vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));

		if (string_param != nullptr)
		{
			for (int32 i = 0; i < string_param_num; ++i)
			{
				vec_notice_str.push_back(string_param[i]);
			}
		}

		std::string notice_str = init_unit::implode(vec_notice_str);

		std::vector<int32> param_arr;
		param_arr.reserve(5);
		fvector player_pos = get_new_map_pos().unit_location;
		int32 line_id = get_line_id();

		param_arr.push_back(line_id);
		param_arr.push_back(base_map_system::get_map_template_id(get_map_ent()));
		param_arr.push_back(player_pos.x);
		param_arr.push_back(player_pos.y);
		param_arr.push_back(player_pos.z);

		if (sender_name_id > 0)
		{
			const std::string& npc_name = template_manager::get_instance().get_str_by_string_template_id(sender_name_id);
			world_cs::send_notice_with_param(get_unit_guid(), npc_name, notice_str, param_arr, e_notice_param_go_to_location,
			e_chat_type_world, m_npc_template_ptr->attribute_id, notice_id);
		}

	}

	void npc::send_message_to_aoi(google::protobuf::Message* net_pro, uint32 header, bool to_self, bool use_length)
	{
		if (m_cur_map_template_ptr)
		{
			if (m_cur_map_template_ptr->Type != e_map_type_big_map)
			{
				aoi_system::send_message_aoi_all(get_map_ent(), net_pro, header, get_old_map_pos().unit_location, get_array_index(), false);
			}
			else
			{
				aoi_system::send_message_aoi_all(get_map_ent(), net_pro, header, get_old_map_pos().unit_location, get_array_index(), use_length);
			}
		}
	}

	int64 npc::get_residue_born_time()
	{
		int64 RunningTime = time_helper::get_cur_time_new().second - m_residue_born_time;
		if (nullptr == m_npc_template_ptr)
		{
			return 0;
		}
		if (RunningTime > m_npc_template_ptr->BornTime)
		{
			return m_npc_template_ptr->BornTime;
		}
		return RunningTime;
	}

	void npc::get_aoi_msg(hld::aoi_proto_unit_aoi_all& msg)
	{
		if (m_npc_template_ptr != nullptr)
		{
			msg.set_npc_temp_id(m_npc_template_ptr->attribute_id);
		}
		msg.set_unit_guid(get_unit_guid().server_64);
		msg.set_role_name(get_name());
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
		msg.set_server_id(get_unit_info(e_role_info_server_id));
		msg.set_is_mission_see_player_guid(get_can_see_player_guid());
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
		msg.set_sub_type(m_unit_sub_type);
		msg.set_n_born_anim(m_born_type);
		msg.set_residue_am_time(get_residue_born_time());
		if (m_robot_template_id > 0)
		{
			msg.set_robot_template_id(m_robot_template_id);
		}
		else
		{
			msg.set_robot_template_id(0);
		}
		msg.set_branch_type(get_unit_info(e_role_info_class_branch));
		if (get_unit_info(e_role_info_wing_showd_template_id) > 0)
		{
			msg.set_wing_template_id(get_unit_info(e_role_info_wing_showd_template_id));
		}
		if (get_unit_info(e_role_info_mount_showd_template_id) > 0)
		{
			msg.set_mount_template_id(get_unit_info(e_role_info_mount_showd_template_id));
		}
		if (m_item_id > 0)
		{
			msg.set_n_item_id(m_item_id);
		}
		msg.set_show_weapon_id(m_show_weapon_id);
		if (m_owner_unit_identifier.is_valid())
		{
			msg.set_owner_guid(m_owner_unit_identifier.unit_guid.server_64);
		}
		for (int32 i = e_unit_attack_att_none; i < e_unit_attack_att_max; ++i)
		{
			msg.add_att_attack(get_pawn_att().get_attack_att_value(i));
		}
		for (int32 i = e_unit_game_att_movement; i < e_unit_game_att_max; ++i)
		{
			msg.add_att_info(get_pawn_att().get_game_att(i));
		}
		for (int32 i = e_unit_attack_state_dead; i < e_unit_attack_state_max; ++i)
		{
			msg.add_att_state(get_pawn_att().get_state_att(i));
		}
		for (int32 i = e_base_att_info_hp_cur; i < e_base_att_info_max; ++i)
		{
			msg.add_att_base(m_pawn_att.get_unit_base_att(i));
		}
		item_set& item_set_ref = item_manager::get_item_set(get_array_index());
		citem * weapon_ptr = item_set_ref.get_equip_item_by_slot(e_role_equip_slot_weapon_1);
		if (weapon_ptr)
		{
			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(weapon_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
			{
				item_info_ptr->add_item_data(weapon_ptr->get_data_info((e_item_info)i));
			}
		}			citem * mount_ptr = item_set_ref.get_showed_mount();
		if (mount_ptr)
		{
			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(mount_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
			{
				item_info_ptr->add_item_data(mount_ptr->get_data_info((e_item_info)i));
			}
		}
		citem* wing_ptr = item_set_ref.get_showed_wing();
		if (wing_ptr)
		{
			item_proto_item_info* item_info_ptr = msg.add_item_list();
			item_info_ptr->set_item_guid(wing_ptr->get_item_guid().server_64);
			for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
			{
				item_info_ptr->add_item_data(wing_ptr->get_data_info((e_item_info)i));
			}
		}
		//citem* spirir_item_ptr = m_spirit_mgr.get_fighting_spirit();
		//if (spirir_item_ptr && spirir_item_ptr->get_item_info_ptr())
		//{
		//	character_proto_spirit_info* spirit_info_ptr = msg.mutable_spirit_info();
		//	spirit_info_ptr->set_spirit_guid(spirir_item_ptr->get_item_guid().server_64);
		//	spirit_info_ptr->add_spirit_data(spirir_item_ptr->get_item_info_ptr()->logic_id);
		//	spirit_info_ptr->add_spirit_data(spirir_item_ptr->get_data_info(e_item_info_slot));
		//	spirit_info_ptr->add_spirit_data(spirir_item_ptr->get_data_info(e_item_info_showing_illusion_idex));
		//	spirit_info_ptr->add_spirit_data(spirir_item_ptr->get_data_info(e_item_info_upgrade_count));
		//}
		if (init_unit::is_fake_player(m_unit_type, m_unit_sub_type))
		{
			citem * fashion_weapon_ptr = item_set_ref.get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_weapon);
			if (fashion_weapon_ptr)
			{
				item_proto_item_info* item_info_ptr = msg.add_item_list();
				item_info_ptr->set_item_guid(fashion_weapon_ptr->get_item_guid().server_64);
				for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
				{
					item_info_ptr->add_item_data(fashion_weapon_ptr->get_data_info((e_item_info)i));
				}
			}

			citem * fashion_body_ptr = item_set_ref.get_item_by_slot(e_bag_type_equip_fasion, e_fashion_show_type_cloth);
			if (fashion_body_ptr)
			{
				item_proto_item_info* item_info_ptr = msg.add_item_list();
				item_info_ptr->set_item_guid(fashion_body_ptr->get_item_guid().server_64);
				for (int32 i = 0; i < e_item_info_upgrade_count; ++i)
				{
					item_info_ptr->add_item_data(fashion_body_ptr->get_data_info((e_item_info)i));
				}
			}
		}
		box_award_system::aoi_msg(this, msg);
		wheel_plate_system::aoi_msg(this, msg);
		msg.set_spawn_id(get_spawn_point_id());
	}

	void npc::be_attacked(const int32& attack_index, const double hp)
	{
		if (nullptr == m_npc_template_ptr)
		{
			return;
		}
		m_ai_unit.attack_value(attack_index, hp);
		if (m_npc_template_ptr->IsUseDamageList)
		{	
			if (attack_index >= npc_arrary_index_begin)
			{
				npc& npc_ref = unit_man::get_npc(attack_index);
				if (npc_ref.is_valid())
				{
					player& player_ref = unit_man::get_player(npc_ref.get_owner());
					if (player_ref.is_valid() == false || npc_ref.get_assist_fight_guid().is_valid() || npc_ref.get_is_be_assist_fight_npc())
					{
						add_boss_damage_vaule(&npc_ref, hp);
					}
				}
			}
			else
			{
				player& attacker_ref = unit_man::get_player(attack_index);

				if (attacker_ref.is_valid())
				{
					add_boss_damage_vaule(&attacker_ref, hp);
				}
			}				
		}

		int32 player_index = 0;
		if (attack_index >= npc_arrary_index_begin)
		{
			npc& npc_ref = unit_man::get_npc(attack_index);
			if (npc_ref.is_valid())
			{
				player& player_ref = unit_man::get_player(npc_ref.get_owner());
				if (player_ref.is_valid() == false)
				{
					player_index = player_ref.get_array_index();
				}
			}
		}
		else
		{
			player_index = attack_index;
		}
		// 副本内伤害排行榜
		player& attacker_ref = unit_man::get_player(player_index);
		auto map_type = base_map_system::get_map_type(get_map_ent());
		auto map_sub_type = base_map_system::get_map_sub_type(get_map_ent());
		if (attacker_ref.is_valid())
		{
			if (map_type == e_map_type_legion_station)
			{
				if (m_npc_template_ptr->NpcType == e_unit_type_monster && m_npc_template_ptr->SubType == e_monster_type_boss)
				{
					world_cs::unit_be_damage(get_map_ent(),player_index, get_array_index(), hp);
				}
			}
			else
			{
				world_cs::unit_be_damage(get_map_ent(), player_index, get_array_index(), hp);
			}
			if (m_npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(m_npc_template_ptr->SubType))//世界BOSS
			{
				refresh_npc_damaget_list(player_index, hp, attacker_ref, e_boss_type_world);
			}
			else if ( map_type == e_map_type_cross_server_pk && map_sub_type == 4)//跨服世界BOSS
			{
				refresh_npc_damaget_list(player_index, hp, attacker_ref, e_boss_type_cross_world);
			}
			else if ( map_type == e_map_type_legion_station && m_npc_template_ptr->NpcType == e_unit_type_monster && m_npc_template_ptr->SubType == e_monster_type_boss)//军团BOSS
			{
				refresh_npc_damaget_list(player_index, hp, attacker_ref, e_boss_type_legion);
			}
			else if (map_type == e_map_type_attack_city)
			{
				refresh_npc_damaget_list(player_index, hp, attacker_ref, e_boss_type_attack_city);
			}
		}
		

		if (battle_begin_time <= 0)
		{
			battle_begin_time = time_helper::get_cur_time_new().second;
		}
		if (m_npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(m_npc_template_ptr->SubType))
		{
			player& player_ptr = unit_man::get_player(attack_index);
			if (player_ptr.m_battle_begin_time <= 0)
			{
				player_ptr.m_battle_begin_time = time_helper::get_cur_time_new().second;
			}
		}

		if (m_npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(m_npc_template_ptr->SubType))
		{
			player& player_ptr = unit_man::get_player(attack_index);
			if (player_ptr.m_battle_begin_time <= 0)
			{
				player_ptr.m_battle_begin_time = time_helper::get_cur_time_new().second;
			}
		}
	}

	bool npc::can_become_target()
	{
		if (is_dead())
		{
			return false;
		}
		if (m_npc_template_ptr)
		{
			if (m_npc_template_ptr->SkillTarget > 0)
			{
				return true;
			}
		}
		return false;
	}

	bool npc::can_attack_other(int32 target_unit_idx)
	{
		player& player_ref = unit_man::get_player(m_owner_unit_identifier);
		if (player_ref.is_valid())
		{
			return player_ref.can_attack_other(target_unit_idx);
		}

		if (m_npc_template_ptr)
		{
			// 增加自掉血npc无法攻击时会自动掉血
			if (m_npc_template_ptr->AttackOther > 0 || m_npc_template_ptr->BeHurtType == e_be_hurt_type_self)
			{
				return true;
			}
		}
		return false;
	}

	void npc::be_dead(bool is_auto_put_into_bag)
	{
		get_pawn_att().send_unit_dead(true);
		m_ai_unit.on_dead();
		get_war_state_mgr().war_time_end(m_kill_array_index, true);

		world_cs::npc_dead(get_map_ent(), this);

		if (m_npc_template_ptr->IsUseDamageList == false)
		{
			hate_info_vector const& hate_list = m_ai_unit.get_hate_list();
			drop_manager::create_drop(m_npc_template_ptr,
				get_map_ent(),
				get_new_map_pos(),
				get_first_attacker(),
				m_kill_array_index,
				hate_list,
				m_ai_unit.get_damage_list(),
				m_array_index);
		}

		if (m_npc_template_ptr->NpcType == e_unit_type_award_mark && m_npc_template_ptr->SubType == e_award_mark_type_refrush_monster)
		{//宝箱怪走获取删除
			auto& player_ptr = unit_man::get_player(m_kill_array_index);
			if (player_ptr.is_valid() && m_box_award_component)
			{
				item_system::put_item_into_bag(&player_ptr, m_box_award_component->m_drop_item);
			}
		}
		else
		{
			unit_man::remove_npc(get_array_index(), true);
		}
	}

	void npc::mark_kill_legion_boss_player(const hate_info_vector& hate_list)
	{
		int32 list_size = hate_list.size();
		for (int32 i = 0; i < list_size; i++)
		{
			const s_hate_info& ref_hate_info = hate_list[i];
			player& ref_player = unit_man::get_player(ref_hate_info.unit_identifier);
			if (false == ref_player.is_valid())
			{
				continue;
			}
			if (ref_hate_info.hate_value <= 0)
			{
				continue;
			}
			ref_player.mark_kill_legion_boss_get_award_today();
		}
	}
	void npc::set_spawn_point_id(int32 point_id)
	{
		m_spawn_point_id = point_id;
	}
	void npc::creat_dead_chests()
	{
		ZoneScoped;
		if (m_npc_template_ptr == nullptr || m_npc_template_ptr->DropChests.size() <= 0 || m_npc_template_ptr->DropChestsPos.size() <=0)
		{
			return;
		}

		int32 chest_size = m_npc_template_ptr->DropChests.size();
		if (chest_size % 2 != 0)
		{
			return;
		}

		std::vector<fvector> creat_chests_pos;
		get_drop_chest_pos(creat_chests_pos);

		int32 chest_num = 0;
		for (int32 idx = 0; idx < chest_size; idx += 2)
		{
			chest_num += m_npc_template_ptr->DropChests[idx + 1];
		}

		if (creat_chests_pos.size() < chest_num)
		{
			return;
		}
		creat_chests_pos.resize(chest_num);

		for (int32 i = 0; i < chest_size; i += 2)
		{
			for (int32 j = 0; j < m_npc_template_ptr->DropChests[i + 1]; j++)
			{
				int32 pos_size = creat_chests_pos.size();
				int32 random_idx = random_gen::get_random(0, pos_size - 1);

				s_unit_identifier empty_owner;
				s_map_pos born_pos;
				born_pos.unit_location.x = creat_chests_pos[random_idx].x;
				born_pos.unit_location.y = creat_chests_pos[random_idx].y;
				born_pos.unit_location.z = creat_chests_pos[random_idx].z;
				npc* new_npc_ptr = world_cs::spawn_npc(m_npc_template_ptr->DropChests[i], born_pos, empty_owner, get_map_ent());

				creat_chests_pos.erase(creat_chests_pos.begin() + random_idx);
			}
		}
	}

	void npc::get_drop_chest_pos(std::vector<fvector>& pos)
	{
		if (m_npc_template_ptr == nullptr || m_npc_template_ptr->DropChestsPos.size() <= 0)
		{
			return;
		}
		
		pos.clear();
		int32 template_pos_size = m_npc_template_ptr->DropChestsPos.size();
		if (template_pos_size % 3 != 0)
		{
			return;
		}

		for (int32 idx = 0; idx < template_pos_size; idx += 3)
		{
			fvector temp_pos;
			temp_pos.x = m_npc_template_ptr->DropChestsPos[idx];
			temp_pos.y = m_npc_template_ptr->DropChestsPos[idx + 1];
			temp_pos.z = m_npc_template_ptr->DropChestsPos[idx + 2];
			pos.push_back(temp_pos);
		}

		s_map_pos npc_dead_pos = get_new_map_pos();
		fvector npc_pos = npc_dead_pos.unit_location;

		int32 pos_size = pos.size();
		for (int32 i = 0; i < pos_size; i++)
		{
			int64 first_distance =  init_unit::get_distance(npc_pos, pos[i]);
			for (int32 j = i + 1; j < pos_size; j++)
			{
				int64 second_distance = init_unit::get_distance(npc_pos, pos[j]);
				if (second_distance < first_distance)
				{
					fvector temp;
					temp = pos[j];
					pos[j] = pos[i];
					pos[i] = temp;
				}
			}
		}
	}

	void npc::aoi_watch_all_data_out(int32 unit_index)
	{
		unit& unit_ref = unit_man::get_unit(unit_index);
		m_ai_unit.aoi_out(unit_ref.get_identifier());
		if (unit_ref.get_unit_type() == e_unit_type_player)
		{
			player& player_ref = unit_man::get_player(unit_index);
			if (player_ref.get_assist_fight_mgr().is_in_assist_fight() && player_ref.get_assist_fight_mgr().get_assist_fight_boss_id() == get_unit_guid())
			{
				player_ref.get_assist_fight_mgr().cancel_assist_fight();
			}
			else
			{
				transfer_damage_to_master(player_ref.get_unit_guid(), unit_index);
			}
		}
	}
	bool  npc::restore_cache_hate_val(int32 unit_index, int64 dead_ms)
	{
		unit& unit_ref = unit_man::get_unit(unit_index);
		return m_ai_unit.restore_cache_hate_val(unit_ref.get_identifier(), dead_ms);
	}
	guid_64& npc::get_legion_guid()
	{
		if (get_legion_info().legion_guid.is_valid())
		{
			return get_legion_info().legion_guid;
		}
		
		unit& unit_ref = unit_man::get_unit(m_owner_unit_identifier);
		return unit_ref.get_legion_info().legion_guid;
	}

	guid_64& npc::get_team_guid()
	{
		if (get_team_info().team_guid.is_valid())
		{
			return get_team_info().team_guid;
		}
		
		unit& unit_ref = unit_man::get_unit(m_owner_unit_identifier);
		return unit_ref.get_team_info().team_guid;
	}

	e_unit_type npc::get_owner_unit_type()
	{
		player& player_ref = unit_man::get_player(m_owner_unit_identifier);
		if (player_ref.is_valid())
		{
			return player_ref.get_unit_type();
		}

		return get_unit_type();
	}

	int32 npc::get_owner_unit_level()
	{
		player& player_ref = unit_man::get_player(m_owner_unit_identifier);
		if (player_ref.is_valid())
		{
			return player_ref.get_unit_info(e_role_info_exp_level);
		}

		return get_unit_info(e_role_info_exp_level);
	}

	bool npc::check_can_show_head(guid_64 unit_guid)
	{
		if (m_ai_unit.check_play_in_damage_list(unit_guid))
		{
			return true;
		}
		return false;
	}
	int32 npc::is_control_boss()
	{
		if (m_npc_template_ptr == nullptr)
		{
			return 0;
		}
		return m_npc_template_ptr->IsControlBoss;
	}
	bool npc::is_area_npc()
	{
		if (m_npc_template_ptr == nullptr)
		{
			return false;
		}
		if (m_npc_template_ptr->attribute_id >= area_npc_start_id && m_npc_template_ptr->attribute_id <= area_npc_end_id)
		{
			return true;
		}
		if (m_npc_template_ptr->attribute_id == area_db_npc_id)
		{
			return true;
		}
		return false;


	}

	int32 npc::get_npc_template_id() const
	{
		if (nullptr == m_npc_template_ptr)
		{
			return -1;
		}
		return m_npc_template_ptr->attribute_id;
	}
	void npc::relieve_stop_time(const int32& skill_template_id)
	{
		skill_set& skill_set_ref = skill_manager::get_skill_set(m_array_index);
		skill_set_ref.relieve_stop_time(skill_template_id);
		m_ai_unit.relieve_stop_time(skill_template_id);
	}
	
	void npc::add_damage_value_with_summon(const int32& attack_index, const double hp)
	{
		player& attacker_ref = unit_man::get_player(attack_index);
		auto map_type = base_map_system::get_map_type(get_map_ent());
		auto map_sub_type = base_map_system::get_map_sub_type(get_map_ent());
		if (attacker_ref.is_valid())
		{
			world_cs::unit_be_damage(get_map_ent(), attack_index, get_array_index(), hp);
			if (m_npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(m_npc_template_ptr->SubType))//世界BOSS
			{
				refresh_npc_damaget_list(attack_index, hp, attacker_ref, e_boss_type_world);
			}
			else if (map_type == e_map_type_cross_server_pk && map_sub_type == 4)//跨服世界BOSS
			{
				refresh_npc_damaget_list(attack_index, hp, attacker_ref, e_boss_type_cross_world);
			}
			else if (map_type == e_map_type_legion_station && m_npc_template_ptr->NpcType == e_unit_type_monster && m_npc_template_ptr->SubType == e_monster_type_boss)//军团BOSS
			{
				refresh_npc_damaget_list(attack_index, hp, attacker_ref, e_boss_type_legion);
			}
			else if (map_type == e_map_type_attack_city)
			{
				refresh_npc_damaget_list(attack_index, hp, attacker_ref, e_boss_type_attack_city);
			}
		}

		m_ai_unit.add_damage_value_with_summon(attack_index, hp);
	}

	void npc::check_damage_rewards(int32 unit_index, int64 damage, e_activity_type activity_type)
	{
		player& player_ref = unit_man::get_player(unit_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		RewardSettlementTemplate* reward_template_ptr = template_manager::get_instance().get_template_by_activity_score(activity_type, damage);
		if (nullptr == reward_template_ptr)
		{
			return;
		}

		guid_64 player_guid = player_ref.get_unit_guid();
		int32 cur_reward_id = 0;
		std::unordered_map<ui64, int32>::iterator reward_id_it = m_player_got_damage_reward_map.find(player_guid.server_64);
		if (reward_id_it != m_player_got_damage_reward_map.end())
		{
			cur_reward_id = reward_id_it->second;
		}
		if (cur_reward_id >= reward_template_ptr->attribute_id)
		{
			return;
		}
		if (cur_reward_id <= 0)
		{
			if (activity_type == e_activity_type_world_boss)
			{
				cur_reward_id = first_world_boss_reward_temp_id;
			}
			else if (activity_type == e_activity_type_legion_boss)
			{
				cur_reward_id = first_legion_boss_reward_temp_id;
			}
			else
			{
				cur_reward_id = 0;
			}
		}
		int32 new_reward_id = reward_template_ptr->attribute_id;
		m_player_got_damage_reward_map[player_guid.server_64] = reward_template_ptr->attribute_id;

		for (int32 i = cur_reward_id + 1; i <= new_reward_id; i++)
		{
			RewardSettlementTemplate* cur_reward_template_ptr = GET_TEMPLATE(RewardSettlementTemplate, i);
			if (nullptr == cur_reward_template_ptr)
			{
				return;
			}
			int32 len = cur_reward_template_ptr->ItemRewards.size();
			if (len > 0 && len % 2 == 0)
			{
				int32 item_id = 0;
				int32 item_num = 0;
				std::vector<int32> item_list;
				for (int32 i = 0; i < len; i += 2)
				{
					item_list.clear();
					item_id = cur_reward_template_ptr->ItemRewards[i];
					item_num = cur_reward_template_ptr->ItemRewards[i + 1];
					if (item_num <= 0)
					{
						continue;
					}
					item_list.push_back(item_id);
					item_list.push_back(item_num);
					item_set& item_set_ref = player_ref.get_item_set();

					item_set_ref.put_in_bag(e_server_log_add_item_npc_damage_rewards, i, item_list);
				}
			}
		}
	}

	void npc::refresh_npc_damaget_list(const int32& unit_array_index, double damage_value,player& player_ref, e_boss_type boss_type)
	{
		int64 player_score = m_score_rank_indicator.add_score(player_ref.get_unit_guid(), damage_value);
		if (-1 == player_score)
		{
			m_score_rank_indicator.add_score_record(player_ref.get_unit_guid(), player_ref.get_name(), player_ref.get_unit_info(e_role_info_exp_level));
			m_score_rank_indicator.set_legion_guid(player_ref.get_unit_guid(), player_ref.get_legion_guid());
			player_score = m_score_rank_indicator.add_score(player_ref.get_unit_guid(), damage_value);
			if (boss_type == e_boss_type_world)
			{
				int64 cur_time_stamp = time_helper::get_cur_time_new().second;
				if (cur_time_stamp > player_ref.get_time_data(e_time_type_next_world_boss_can_count))
				{
					ActivityCommonConfigTemplate* activity_cfg_template_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_world_boss);
					std::vector<int32> open_time = cell_server::getInstance().get_activity_open_time(e_activity_type_world_boss);
					if (nullptr != activity_cfg_template_ptr && open_time.size() >= once_activity_open_time_array_size)
					{
						player_ref.get_time_limit_activity_mgr().activity_behavior_done(e_time_limit_behavior_type_kill_world_boss);      //新限时行为世界boss计数
						player_ref.set_must_do_count(e_daily_must_do_typ_gold_army, player_ref.get_must_do_count(e_daily_must_do_typ_gold_army) + 1);
						player_ref.send_daily_must_do_count_info_one(e_daily_must_do_typ_gold_army);
						player_ref.set_daily_active_degree_info(e_daily_must_do_typ_gold_army);
						int32 activity_duration_time = time_helper::get_stamp_by_hour_min(open_time[6], open_time[7])
							- time_helper::get_stamp_by_hour_min(open_time[2], open_time[3]);
						//预留30分钟boss狂暴的时间
						player_ref.set_time_data(e_time_type_next_world_boss_can_count, cur_time_stamp + activity_duration_time + 30 * 60);
					}
				}
			}
		}
		if (boss_type == e_boss_type_world)
		{
			//check_damage_rewards(unit_array_index, player_score, e_activity_type_world_boss);
		}
		else if (boss_type == e_boss_type_legion)
		{
			check_damage_rewards(unit_array_index, player_score, e_activity_type_legion_boss);
		}
		if (false == player_ref.get_legion_guid().is_valid())
		{
			return;
		}
		int64 legion_score = m_legion_score_rank_indicator.add_score(player_ref.get_legion_guid(), damage_value);
		if (-1 == legion_score)
		{
			m_legion_score_rank_indicator.add_score_record(player_ref.get_legion_guid(), player_ref.get_player_legion_name(), player_ref.get_unit_info(e_role_info_exp_level),0, player_ref.get_unit_info(e_role_info_server_id));
			m_legion_score_rank_indicator.add_score(player_ref.get_legion_guid(), damage_value);
		}
	}

	void npc::send_damage_score_to_ws()
	{
		if (m_npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(m_npc_template_ptr->SubType))
		{
			cs2ws_npc_damage_list msg;
			int32 data_idx = 0;
			m_score_rank_indicator.sort();
			int32 data_len = m_score_rank_indicator.get_score_recore_num();
			for (; data_idx < data_len; data_idx++)
			{
				score_record* rank_record_ptr = m_score_rank_indicator.get_score_record_at(data_idx);
				if (nullptr == rank_record_ptr || data_idx >= sync_damage_rank_count)
				{
					break;
				}
				msg.top_records[data_idx] = *rank_record_ptr;
			}
			msg.record_num = data_idx;
			msg.is_legion_score = false;
			msg.npc_id = get_npc_template_id();
			msg.boss_type = e_boss_type_world;
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));

		}

	}

	void npc::send_legion_score_to_ws()
	{
		ZoneScoped;
		auto map_type = base_map_system::get_map_template_id(get_map_ent());

		if (m_npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(m_npc_template_ptr->SubType)
			|| (map_type == e_map_type_legion_station && m_npc_template_ptr->NpcType == e_unit_type_monster && m_npc_template_ptr->SubType == e_monster_type_boss))//世界BOSS
		{
			cs2ws_npc_damage_list msg;
			int32 data_idx = 0;
			m_legion_score_rank_indicator.sort();
			int32 data_len = m_legion_score_rank_indicator.get_score_recore_num();
			for (; data_idx < data_len; data_idx++)
			{
				score_record* rank_record_ptr = m_legion_score_rank_indicator.get_score_record_at(data_idx);
				if (nullptr == rank_record_ptr || data_idx >= sync_damage_rank_count)
				{
					break;
				}
				msg.top_records[data_idx] = *rank_record_ptr;
			}
			msg.record_num = data_idx;
			msg.is_legion_score = true;
			msg.npc_id = get_npc_template_id();
			if (map_type == e_map_type_legion_station)
			{
				msg.boss_type = e_boss_type_legion;
			}
			else
			{
				msg.boss_type = e_boss_type_world;
			}
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
		}
	}

	void npc::send_plot_with_npc_hp()
	{
		float now_hp = get_left_hp_per() * 100;
		if (m_npc_template_ptr == nullptr || m_npc_template_ptr->HpPlotTip.size() <= 0 || m_npc_template_ptr->HpPlotTip.size() % 3 != 0)
		{
			return;
		}
		auto map_object_ptr = world_cs::get_map_by_guid<map_object>(get_map_guid());
		if (nullptr == map_object_ptr)
		{
			return;
		}
		std::vector<int32> hp_plot_arr = m_npc_template_ptr->HpPlotTip;
		for (int32 i = 0; i < hp_plot_arr.size() / 3; i++)
		{
			if (now_hp <= hp_plot_arr[i * 3] && m_last_send_plot_hp > hp_plot_arr[i * 3])
			{
				m_last_send_plot_hp = hp_plot_arr[i * 3];
				game_proto_pop_plot_tip pop_plot_tip;
				pop_plot_tip.set_str_id_type(hp_plot_arr[i * 3 + 1]);
				pop_plot_tip.set_str_id(hp_plot_arr[i * 3 + 2]);
				map_object_ptr->send_message_to_all_player(&pop_plot_tip, e_msgindex_s2c_pop_plot_tip);
				break;
			}
		}
	}

	void npc::refresh_npc_left_hp_per()
	{
		float now_hp = get_left_hp_per() * 100;
		if (last_hp_scale_flag - now_hp > 5 || now_hp - last_hp_scale_flag > 5)
		{

			last_hp_scale_flag = now_hp;
			send_npc_left_hp_per();
		}
	}
	void npc::send_npc_left_hp_per()
	{
		cs2ws_npc_left_hp_per send_pak;
		send_pak.npc_spawn_point_template_id = m_spawn_point_id;
		send_pak.left_hp_per = (int32)(get_left_hp_per() * 100);
		connection_mgr::getInstance().send_to_ws( &send_pak, sizeof(send_pak));
	}

	void npc::send_gain_treasure_record_to_ws(int32 killed_stamp, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr)
	{
		gain_treasure_mgr::send_gain_treasure_record_to_ws(get_spawn_point_id(), killed_stamp, player_info_arr);
	}

	void npc::send_gain_treasure_boss_owner_to_ws()
	{
		if (false == is_gain_treasure_boss() && false == is_island_boss())
		{
			return;
		}
		if (m_npc_template_ptr->IsUseDamageList)
			return;
		hate_info_vector const& hate_list = m_ai_unit.get_hate_list();
		std::vector<team_score_info> team_score_info_array;
		drop_manager::init_team_score_info_ex(hate_list, team_score_info_array);
		std::vector<s_gain_treasure_player_info> player_info_arr;
		player_info_arr.clear();
		if (team_score_info_array.size() > 0)
		{
			team_score_info one_team_info = team_score_info_array[0];
			s_gain_treasure_player_info player_info;
			for (int32 i = 0; i < one_team_info.get_member_num(); i++)
			{
				player* player_ptr = one_team_info.get_member_ptr(i);
				if (nullptr == player_ptr)
				{
					continue;
				}
				player_info.clear_data();
				player_info.player_guid = player_ptr->get_unit_guid();
				std::string player_name = player_ptr->get_name();
				memcpy(player_info.player_name, player_name.c_str(), player_name.size() >= max_name_size ? max_name_size : player_name.size());
				player_info_arr.push_back(player_info);
			}
		}
		bool is_change = false;
		int32 old_len = m_player_info_arr.size();
		int32 new_len = player_info_arr.size();
		if (new_len != old_len)
		{
			is_change = true;
		}
		else
		{
			for (int32 i = 0; i < new_len; i++)
			{
				if (player_info_arr[i].player_guid != m_player_info_arr[i].player_guid)
				{
					is_change = true;
					break;
				}
			}
		}
		if (true == is_change)
		{
			m_player_info_arr = player_info_arr;
			if (true == is_gain_treasure_boss())
			{
				gain_treasure_mgr::send_gain_treasure_boss_info_to_ws(get_spawn_point_id(), get_unit_guid(), 0, player_info_arr);
			}
			else if (true == is_island_boss())
			{
				boss_island_mgr::send_boss_island_info_to_ws(get_spawn_point_id(), get_unit_guid(), 0, player_info_arr);
			}
			else if (true == is_belief_cloister_boss())
			{
				//信仰回廊boss目标转移 由于逻辑相似 暂时和天空之岛逻辑一起处理
				boss_island_mgr::send_boss_island_info_to_ws(get_spawn_point_id(), get_unit_guid(), 0, player_info_arr);
			}
		}
	}

	bool npc::send_gain_treasure_boss_info_when_dead()
	{
		if (false == is_gain_treasure_boss() && false == is_island_boss() && false == is_belief_cloister_boss())
		{
			return false;
		}
		int32 spawn_id = get_spawn_point_id();
		NpcSpawnPointTemplate* spawn_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_id);
		if (nullptr == spawn_template_ptr)
		{
			return false;
		}
		guid_64 null_guid;
		null_guid.clear_data();
		int32 next_refresh_stamp = time_helper::get_cur_time_new().second + spawn_template_ptr->RespawnTime;
		std::vector<s_gain_treasure_player_info> player_info_arr;
		player_info_arr.clear();
		if (true == is_gain_treasure_boss())
		{
			gain_treasure_mgr::send_gain_treasure_boss_info_to_ws(spawn_id, null_guid, next_refresh_stamp, player_info_arr);
		}
		else if (true == is_island_boss())
		{
			boss_island_mgr::send_boss_island_info_to_ws(spawn_id, null_guid, next_refresh_stamp, player_info_arr);
		}
		else if (true == is_belief_cloister_boss())
		{
			//信仰回廊boss被击杀 由于逻辑相似 暂时和天空之岛逻辑一起处理
			boss_island_mgr::send_boss_island_info_to_ws(spawn_id, null_guid, next_refresh_stamp, player_info_arr);
		}
		return true;
	}

	void npc::send_gain_treasure_boss_info_when_spawn()
	{
		if (false == is_gain_treasure_boss() && false == is_island_boss() && false == is_belief_cloister_boss())
		{
			return;
		}
		std::vector<s_gain_treasure_player_info> player_info_arr;
		player_info_arr.clear();
		if (true == is_gain_treasure_boss())
		{
			gain_treasure_mgr::send_gain_treasure_boss_info_to_ws(get_spawn_point_id(), get_unit_guid(), 0, player_info_arr);
		}
		else if (true == is_island_boss())
		{
			boss_island_mgr::send_boss_island_info_to_ws(get_spawn_point_id(), get_unit_guid(), 0, player_info_arr);
		}
		else if (true == is_belief_cloister_boss())
		{
			//信仰回廊boss复活 由于逻辑相似 暂时和天空之岛逻辑一起处理
			boss_island_mgr::send_boss_island_info_to_ws(get_spawn_point_id(), get_unit_guid(), 0, player_info_arr);
		}
	}

	bool npc::is_gain_treasure_boss()
	{
		if (get_unit_type() != e_unit_type_monster || get_unit_sub_type() != e_monster_type_boss)
		{
			return false;
		}

		if (false == init_unit::is_map_gain_treasure(base_map_system::get_map_type(get_map_ent())))
		{
			return false;
		}
		return true;
	}

	bool npc::is_island_boss()
	{
		if ((get_unit_type() != e_unit_type_monster || get_unit_sub_type() != e_monster_type_boss)
			&& (get_unit_type() != e_unit_type_npc || (get_unit_sub_type() != e_npc_type_island_small_chests && get_unit_sub_type() != e_npc_type_island_big_chests)))
		{
			return false;
		}

		if (false == init_unit::is_map_boss_island(base_map_system::get_map_type(get_map_ent())))
		{
			return false;
		}
		return true;
	}
	bool npc::is_belief_cloister_boss()
	{
		if ((get_unit_type() != e_unit_type_monster || get_unit_sub_type() != e_monster_type_boss)
			&& (get_unit_type() != e_unit_type_npc || (get_unit_sub_type() != e_npc_type_island_small_chests && get_unit_sub_type() != e_npc_type_island_big_chests)))
		{
			return false;
		}

		if (false == init_unit::is_map_belief_cloister(base_map_system::get_map_type(get_map_ent())))
		{
			return false;
		}
		return true;
	}
	bool npc::is_boss()
	{
		if (get_unit_type() == e_unit_type_monster && get_unit_sub_type() == e_monster_type_boss)
		{
			return true;
		}
		return false;

	}
	bool npc::is_mission_see(int32 unit_array_index)
	{
		if (nullptr == m_npc_template_ptr)
		{
			return true;
		}
		if (get_unit_type() == e_unit_type_summoned || get_unit_type() == e_unit_type_hide)
		{
			player& player_ref = unit_man::get_player(m_owner_unit_identifier);
			if (player_ref.is_valid() == false)
			{
				return true;
			}
			npc& npc_ref = unit_man::get_npc(unit_array_index);
			if (npc_ref.is_valid() == false)
			{
				return true;
			}
			cmission* mission_ptr = player_ref.get_mission_mgr().find_mission_by_index(e_mission_slot_main);
			if (nullptr == mission_ptr)
			{
				return true;
			}
			int32 now_mission_id = mission_ptr->get_mission_id();
			int32 now_mission_state = mission_ptr->get_mission_state();
			NpcTemplate* npc_template_ptr = npc_ref.get_npc_template();
			if (nullptr == npc_template_ptr)
			{
				return true;
			}
			if ((npc_template_ptr->StartShowMissionId == 0 || now_mission_id >= npc_template_ptr->StartShowMissionId)
				&& (npc_template_ptr->EndShowMissionId == 0 || now_mission_id <= npc_template_ptr->EndShowMissionId)
				&& (npc_template_ptr->StartShowMissionState == 0 || now_mission_state >= npc_template_ptr->StartShowMissionState)
				&& (npc_template_ptr->EndShowMissionState == 0 || now_mission_state < npc_template_ptr->EndShowMissionState)
				&& (false == npc_ref.get_can_see_player_guid().is_valid() || npc_ref.get_can_see_player_guid() == player_ref.get_unit_guid()))
			{
				return true;
			}
		}
		else
		{
			player& player_ref = unit_man::get_player(unit_array_index);
			if (player_ref.is_valid() == false)
			{
				return true;
			}
			cmission* mission_ptr = player_ref.get_mission_mgr().find_mission_by_index(e_mission_slot_main);
			if (nullptr == mission_ptr)
			{
				return true;
			}
			int32 now_mission_id = mission_ptr->get_mission_id();
			int32 now_mission_state = mission_ptr->get_mission_state();
			if ((m_npc_template_ptr->StartShowMissionId == 0 || now_mission_id >= m_npc_template_ptr->StartShowMissionId)
				&& (m_npc_template_ptr->EndShowMissionId == 0 || now_mission_id <= m_npc_template_ptr->EndShowMissionId)
				&& (m_npc_template_ptr->StartShowMissionState == 0 || now_mission_state >= m_npc_template_ptr->StartShowMissionState)
				&& (m_npc_template_ptr->EndShowMissionState == 0 || now_mission_state < m_npc_template_ptr->EndShowMissionState)
				&& (false == get_can_see_player_guid().is_valid() || get_can_see_player_guid() == player_ref.get_unit_guid()))
			{
				return true;
			}
		}
		return false;
	}

	void npc::npc_change_legion_name_func(const guid_64& legion_guid, const xchar* legion_name)
	{
		m_legion_score_rank_indicator.change_legion_name_func(legion_guid, legion_name);
	}
	void npc::init_legion_score_rank(const guid_64 & legion_guid, const xchar * legion_name, int64 role_score, int32 server_id)
	{
		m_legion_score_rank_indicator.add_score_record(legion_guid, legion_name, 0, role_score, server_id);
	}
	bool npc::is_cross_server_boss()
	{
		ActivityCommonConfigTemplate* m_world_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		if (nullptr == m_world_boss_config_ptr)
		{
			return false;
		}
		if (m_world_boss_config_ptr->ParamIntArr4.size()%4 != 0 || m_world_boss_config_ptr->ParamIntArr4.size() / 4 < map_max)
		{
			return false;
		}
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (get_npc_template_id() == m_world_boss_config_ptr->ParamIntArr4[i*4+1] || get_npc_template_id() == m_world_boss_config_ptr->ParamIntArr4[i*4+2] || get_npc_template_id() == m_world_boss_config_ptr->ParamIntArr4[i*4+3])
			{
				return true;
			}		
		}		
		return false;

	}

	void npc::send_boss_god_skill_begion_time()//发送boss开始使用无敌技能直接显示20秒
	{
		if (!is_cross_server_boss())
		{
			return;
		}
		//auto map_object_ptr = world_cs::get_map_by_guid<cross_server_world_boss_map_cs>(get_map_guid());
		//if (nullptr == map_object_ptr || map_type != e_map_type_cross_server_pk || map_object_ptr->get_map_template_ptr()->SubType != 4)
		//{
		//	return;
		//}
		//map_object_ptr->send_boss_god_time_with_map(get_npc_template_id());
	}
	bool npc::is_god_skill(int32 skill_id)
	{
		ActivityCommonConfigTemplate* m_world_boss_config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_world_boss);
		if (nullptr == m_world_boss_config_ptr)
		{
			return false;
		}
		if (m_world_boss_config_ptr->ParamIntArr3.size()%3 != 0 || m_world_boss_config_ptr->ParamIntArr3.size()/3 < m_boss_max_num)
		{
			return false;
		}
		if (skill_id <= 0)
		{
			return false;
		}
		for (int32 i = 0; i < m_boss_max_num; ++i)
		{
			if (get_npc_template_id() == m_world_boss_config_ptr->ParamIntArr3[i*3] && skill_id == m_world_boss_config_ptr->ParamIntArr3[i*3+1])
			{
				return true;
			}
		}

		return false;
	}

	bool npc::is_summon_pet()
	{
		if (get_unit_type() != e_unit_type_summoned)
			return false;
		if (get_unit_sub_type() == e_summon_pet_type_goddess)
			return true;
		return false;
	}
	void npc::cross_boss_send_boss_god_time(guid_64 role_guid)
	{
		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		int32 god_time = int32(time_helper::get_cur_time_new().second - m_boss_god_skill_begin_tiem);
		if (god_time <= 0)
		{
			return;
		}
		game_proto_send_boss_god_time msg;
		msg.set_boss_id(get_npc_template_id());
		msg.set_god_time(god_time);
		player_ref.send_message_to_self(&msg, e_msgindex_s2c_send_boss_god_time);
	}

	/*
	伤害统计规则，排行榜上显示最高层级玩家id
	层级有2种情况：
	队长(有效情况:队长在伤害列表内)-队员-助战者
	玩家-助战者
	*/
	void npc::add_boss_damage_vaule(player* attacker, int64 attack_value)
	{
		if (attacker == nullptr)
			return;

		bool is_new = false;
		auto iter = m_boss_damage_person_map.find(attacker->get_unit_guid());
		if (iter == m_boss_damage_person_map.end())
		{
			is_new = true;
			//对于一个新造成伤害的玩家，先尝试计算其所属小团体的负责人
			//可能是自身，可能是自身的被助战者，也可能是队长
			guid_64 tmp_master_guid;
			if (attacker->get_assist_fight_mgr().is_assist_helper() && attacker->get_assist_fight_mgr().get_assist_fight_guid().is_valid())
			{
				if (m_boss_damage_person_map.find(attacker->get_assist_fight_mgr().get_assist_fight_guid()) == m_boss_damage_person_map.end())
					return;
				tmp_master_guid = attacker->get_assist_fight_mgr().get_assist_fight_guid();
			}
			else if (attacker->is_player_in_team() && attacker->is_player_team_captain() == false)
			{
				if (m_boss_damage_person_map.find(attacker->get_team_info().captain_guid) != m_boss_damage_person_map.end())
					tmp_master_guid = attacker->get_team_info().captain_guid;
				else
					tmp_master_guid = attacker->get_unit_guid();
			}
			else
				tmp_master_guid = attacker->get_unit_guid();

			s_boss_damage_list_person_item new_item;
			new_item.role_guid = attacker->get_unit_guid();
			memcpy(new_item.role_name, attacker->get_name(), max_name_size);
			new_item.damage_value = attack_value;
			new_item.master_guid = tmp_master_guid;

			auto result = m_boss_damage_person_map.insert(make_pair(attacker->get_unit_guid(), new_item));
			if (result.second)
				iter = result.first;
			else
				return;
			attacker->get_assist_fight_mgr().set_boss_id(m_role_info.role_guid);
			send_assist_fight_tip(attacker->get_unit_guid(), new_item.master_guid);
		}
		else
		{
			if (attacker->get_assist_fight_mgr().is_assist_helper() && iter->second.master_guid != attacker->get_assist_fight_mgr().get_assist_fight_guid())
			{
				for (auto iter = m_boss_damage_total_vec.begin(); iter != m_boss_damage_total_vec.end(); ++iter)
				{
					if (iter->role_guid == attacker->get_unit_guid())
					{
						m_boss_damage_total_vec.erase(iter);
						break;
					}
				}
				iter->second.master_guid = attacker->get_assist_fight_mgr().get_assist_fight_guid();
			}
			else
			{
				iter->second.damage_value += attack_value;
			}
		}

		add_boss_damage_total_vaule(iter->second.master_guid, attacker, attack_value);

		//由队长检查队伍合并(针对事先创建好的队伍)
		if (attacker->is_player_in_team() && attacker->is_player_team_captain() == true && iter->second.team_check == false)
		{
			iter->second.team_check = true;
			s_player_team_info& player_team_info = attacker->get_team_cs_mgr().get_unit_team_info();
			for (int32 i = 0; i < max_team_member_num; ++i)
			{
				if (player_team_info.team_mems_guid[i] != attacker->get_unit_guid())
				{
					auto mem_iter = m_boss_damage_person_map.find(player_team_info.team_mems_guid[i]);
					if (mem_iter != m_boss_damage_person_map.end() && mem_iter->second.master_guid != attacker->get_unit_guid())
					{
						role_add_team(player_team_info.team_mems_guid[i], attacker->get_unit_guid());
					}
				}
			}
		}

		if (is_new)
			send_boss_damage_list_to_player(attacker);
	}
	/*
	机器人记录伤害列表
	*/
	void npc::add_boss_damage_vaule(npc * attacker, int64 attack_value)
	{
		if (attacker == nullptr)
		{
			return;
		}
		if (attacker->get_unit_type() != e_unit_type_monster || attacker->get_unit_sub_type() != e_monster_type_robot_player)
		{
			return;
		}
			
		auto iter = m_boss_damage_person_map.find(attacker->get_unit_guid());
		if (iter == m_boss_damage_person_map.end())
		{
			guid_64 tmp_master_guid = attacker->get_unit_guid();
			if (attacker->get_assist_fight_guid().is_valid())
			{
				tmp_master_guid = attacker->get_assist_fight_guid();
			}
			s_boss_damage_list_person_item new_item;
			new_item.role_guid = attacker->get_unit_guid();
			memcpy(new_item.role_name, attacker->get_name(), max_name_size);
			new_item.damage_value = attack_value;
			new_item.master_guid = tmp_master_guid;

			auto result = m_boss_damage_person_map.insert(make_pair(attacker->get_unit_guid(), new_item));
			if (result.second)
			{
				iter = result.first;
			}	
			else
			{
				return;
			}
			send_assist_fight_tip(attacker->get_unit_guid(), new_item.master_guid);
		}
		else
		{
			iter->second.damage_value += attack_value;
		}
		add_boss_damage_total_vaule(iter->second.master_guid, attacker, attack_value);
	}
	void npc::add_boss_damage_total_vaule(guid_64 master_guid, player* attacker, int64 attack_value)
	{
		if (attacker == nullptr)
			return;
		unit& master = unit_man::get_unit(master_guid);
		if (master.is_valid() == false)
		{
			return;
		}
		
		guid_64 high_master_guid = master_guid;
		auto iter = m_boss_damage_person_map.find(master_guid);
		if (iter == m_boss_damage_person_map.end())
				return;

		//找到最高层次的负责人
		if (iter->second.master_guid != master_guid)
			high_master_guid = iter->second.master_guid;

		if (attacker->get_assist_fight_mgr().is_assist_helper() && master_guid == attacker->get_assist_fight_mgr().get_assist_fight_guid())
		{
			//填充助战关系
			int32 tmp_index = -1;
			for (auto i = 0; i < 5; ++i)
			{
				if (iter->second.helper_guid[i] == attacker->get_unit_guid())
				{
					tmp_index = -1;
					break;
				}
				else if (iter->second.helper_guid[i].is_valid() == false)
				{
					if (tmp_index == -1)
					{
						tmp_index = i;
					}
				}
			}
			if (tmp_index != -1)
				iter->second.helper_guid[tmp_index] = attacker->get_unit_guid();
		}

		//维护total_vec
		bool has_in = false;
		for (auto v_iter = m_boss_damage_total_vec.begin(); v_iter != m_boss_damage_total_vec.end(); ++v_iter)
		{
			if (v_iter->role_guid == high_master_guid)
			{
				v_iter->damage_value += attack_value;
				has_in = true;
				break;
			}
		}

		if (has_in == false)
		{
			unit& high_master = unit_man::get_unit(high_master_guid);
			if (high_master.is_valid() == false)
				return;
			s_boss_damage_list_total_item new_item;
			new_item.role_guid = high_master.get_unit_guid();
			memcpy(new_item.role_name, high_master.get_name(), max_name_size);
			new_item.damage_value = attack_value;
			m_boss_damage_total_vec.push_back(new_item);
		}
	}

	void npc::add_boss_damage_total_vaule(guid_64 master_guid, npc * attacker, int64 attack_value)
	{
		if (attacker == nullptr)
		{
			return;
		}
			
		unit& master = unit_man::get_unit(master_guid);
		if (master.is_valid() == false)
		{
			return;
		}

		guid_64 high_master_guid = master_guid;
		auto iter = m_boss_damage_person_map.find(master_guid);
		if (iter == m_boss_damage_person_map.end())
		{
			return;
		}

		//找到最高层次的负责人
		if (iter->second.master_guid != master_guid)
		{
			high_master_guid = iter->second.master_guid;
		}
			
		if (attacker->get_assist_fight_guid().is_valid())
		{
			//填充助战关系
			int32 tmp_index = -1;
			for (auto i = 0; i < 5; ++i)
			{
				if (iter->second.helper_guid[i] == attacker->get_unit_guid())
				{
					tmp_index = -1;
					break;
				}
				else if (iter->second.helper_guid[i].is_valid() == false)
				{
					if (tmp_index == -1)
					{
						tmp_index = i;
					}
				}
			}
			if (tmp_index != -1)
			{
				iter->second.helper_guid[tmp_index] = attacker->get_unit_guid();
			}	
		}

		//维护total_vec
		bool has_in = false;
		for (auto v_iter = m_boss_damage_total_vec.begin(); v_iter != m_boss_damage_total_vec.end(); ++v_iter)
		{
			if (v_iter->role_guid == high_master_guid)
			{
				v_iter->damage_value += attack_value;
				has_in = true;
				break;
			}
		}

		if (has_in == false)
		{
			unit& high_master = unit_man::get_unit(high_master_guid);
			if (high_master.is_valid() == false)
			{
				return;
			}	
			s_boss_damage_list_total_item new_item;
			new_item.role_guid = high_master.get_unit_guid();
			memcpy(new_item.role_name, high_master.get_name(), max_name_size);
			new_item.damage_value = attack_value;
			m_boss_damage_total_vec.push_back(new_item);
		}
	}

	void npc::send_boss_damage_list_to_player(player* attacker)
	{
		if (attacker == nullptr)
			return;

		int32 temp_len = m_boss_damage_total_vec.size();
		if (temp_len == 0)
			return;

		s_boss_damage_list_total_item temp_record;
		for (int32 i = 0; i < temp_len - 1; ++i)
		{
			bool changed = false;
			for (int32 j = 0; j < temp_len - i - 1; ++j)
			{
				if (m_boss_damage_total_vec[j].damage_value < m_boss_damage_total_vec[j + 1].damage_value)
				{
					temp_record = m_boss_damage_total_vec[j];
					m_boss_damage_total_vec[j] = m_boss_damage_total_vec[j + 1];
					m_boss_damage_total_vec[j + 1] = temp_record;
					changed = true;
				}
			}
			if (false == changed)
				break;
		}

		game_proto_send_boss_damage_list_msg msg;
		for (int32 i = 0; i < temp_len && i < 5; ++i)
		{
			game_proto_boss_damage_list_item* info_ptr = msg.add_ranking_list();
			if (info_ptr)
			{
				info_ptr->set_role_name(m_boss_damage_total_vec[i].role_name);
				info_ptr->set_role_guid_a(m_boss_damage_total_vec[i].role_guid.A);
				info_ptr->set_role_guid_b(m_boss_damage_total_vec[i].role_guid.B);
				//int32 damage_value = 0;
				//int32 damage_rate = 0;
				//transfer_damage_to_int32(m_boss_damage_total_vec[i].damage_value, damage_value, damage_rate);
				info_ptr->set_damage_value(init_unit::change_i64_to_string(m_boss_damage_total_vec[i].damage_value));
				//info_ptr->set_damage_rate(damage_rate);
				auto& role_ref = unit_man::get_player(m_boss_damage_total_vec[i].role_guid);
				if (role_ref.is_valid())
				{
					info_ptr->set_is_team(role_ref.is_player_team_captain());
					info_ptr->set_legion_guid_a(role_ref.get_legion_guid().A);
					info_ptr->set_legion_guid_b(role_ref.get_legion_guid().B);
				}
			}
		}

		auto iter = m_boss_damage_person_map.find(attacker->get_unit_guid());
		if (iter != m_boss_damage_person_map.end())
		{
			//int32 damage_value = 0;
			//int32 damage_rate = 0;
			//transfer_damage_to_int32(iter->second.damage_value, damage_value, damage_rate);
			msg.set_my_damage(init_unit::change_i64_to_string(iter->second.damage_value));
			//msg.set_damage_rate(damage_rate);
			msg.set_boss_template_id(get_npc_template_id());

			if (is_dead())
				msg.set_is_end(2);
			else if(get_left_hp_per() * 100 <= GAMECONFIG->BossDamageListNeedHPPercent)
				msg.set_is_end(1);

			for (auto i = 0; i < 5; ++i)
			{
				auto helper_iter = m_boss_damage_person_map.find(iter->second.helper_guid[i]);
				if (helper_iter != m_boss_damage_person_map.end())
				{
					game_proto_boss_damage_list_item* info_ptr = msg.add_helper_list();
					if (info_ptr)
					{
						info_ptr->set_role_name(helper_iter->second.role_name);
						//int32 damage_value = 0;
						//int32 damage_rate = 0;
						//transfer_damage_to_int32(helper_iter->second.damage_value, damage_value, damage_rate);
						info_ptr->set_damage_value(init_unit::change_i64_to_string(helper_iter->second.damage_value));
						//info_ptr->set_damage_rate(damage_rate);
					}
				}
			}
			attacker->send_message_to_self(&msg, e_mgsindex_s2c_send_boss_damage_list_msg);
		}
	}

	void npc::send_boss_damage_list_to_all_player()
	{
		int32 temp_len = m_boss_damage_total_vec.size();
		if (temp_len == 0)
			return;

		s_boss_damage_list_total_item temp_record;
		for (int32 i = 0; i < temp_len - 1; ++i)
		{
			bool changed = false;
			for (int32 j = 0; j < temp_len - i - 1; ++j)
			{
				if (m_boss_damage_total_vec[j].damage_value < m_boss_damage_total_vec[j + 1].damage_value)
				{
					temp_record = m_boss_damage_total_vec[j];
					m_boss_damage_total_vec[j] = m_boss_damage_total_vec[j + 1];
					m_boss_damage_total_vec[j + 1] = temp_record;
					changed = true;
				}
			}
			if (false == changed)
				break;
		}

		game_proto_send_boss_damage_list_msg msg;
		for (int32 i = 0; i < temp_len && i < 5; ++i)
		{
			game_proto_boss_damage_list_item* info_ptr = msg.add_ranking_list();
			if (info_ptr)
			{
				info_ptr->set_role_name(m_boss_damage_total_vec[i].role_name);
				info_ptr->set_role_guid_a(m_boss_damage_total_vec[i].role_guid.A);
				info_ptr->set_role_guid_b(m_boss_damage_total_vec[i].role_guid.B);
				//int32 damage_value = 0;
				//int32 damage_rate = 0;
				//transfer_damage_to_int32(m_boss_damage_total_vec[i].damage_value, damage_value, damage_rate);
				info_ptr->set_damage_value(init_unit::change_i64_to_string(m_boss_damage_total_vec[i].damage_value));
				//info_ptr->set_damage_rate(damage_rate);
				auto& role_ref = unit_man::get_player(m_boss_damage_total_vec[i].role_guid);
				if (role_ref.is_valid())
				{
					info_ptr->set_is_team(role_ref.is_player_team_captain());
					info_ptr->set_legion_guid_a(role_ref.get_legion_guid().A);
					info_ptr->set_legion_guid_b(role_ref.get_legion_guid().B);
				}
			}
		}

		msg.set_boss_template_id(get_npc_template_id());

		if (is_dead())
			msg.set_is_end(2);
		else if (get_left_hp_per() * 100 <= GAMECONFIG->BossDamageListNeedHPPercent)
			msg.set_is_end(1);

		for (auto iter = m_boss_damage_person_map.begin(); iter != m_boss_damage_person_map.end(); ++iter)
		{
			player& player_ref = unit_man::get_player(iter->second.role_guid);
			if (player_ref.is_valid() && player_ref.get_map_guid() == get_map_guid())
			{
				//int32 damage_value = 0;
				//int32 damage_rate = 0;
				//transfer_damage_to_int32(iter->second.damage_value, damage_value, damage_rate);
				msg.set_my_damage(init_unit::change_i64_to_string(iter->second.damage_value));
				//msg.set_damage_rate(damage_rate);
				msg.clear_helper_list();
				for (auto i = 0; i < 5; ++i)
				{
					auto helper_iter = m_boss_damage_person_map.find(iter->second.helper_guid[i]);
					if (helper_iter != m_boss_damage_person_map.end())
					{
						game_proto_boss_damage_list_item* info_ptr = msg.add_helper_list();
						if (info_ptr)
						{
							info_ptr->set_role_name(helper_iter->second.role_name);
							//int32 damage_value = 0;
							//int32 damage_rate = 0;
							//transfer_damage_to_int32(helper_iter->second.damage_value, damage_value, damage_rate);
							info_ptr->set_damage_value(init_unit::change_i64_to_string(helper_iter->second.damage_value));
							//info_ptr->set_damage_rate(damage_rate);
						}
					}
				}
				player_ref.send_message_to_self(&msg, e_mgsindex_s2c_send_boss_damage_list_msg);
			}
		}
	}

	void npc::send_leave_boss_damage_to_player(player* player_ptr)
	{
		if (player_ptr == nullptr)
			return;
		player_ptr->get_assist_fight_mgr().clear_assist_fight_boss_id();
		game_proto_send_leave_boss_damage_list_msg msg;
		player_ptr->send_message_to_self(&msg, e_msgindex_s2c_send_leave_boss_damage_list_msg);
	}

	void npc::transfer_damage_to_master(guid_64 role_guid, int32 array_index)
	{
		//离开AOI会清仇恨，同时清空伤害
		player& player_ref = unit_man::get_player(array_index);
		if (player_ref.is_valid() == false || player_ref.get_unit_guid() != role_guid)
		{
			return;
		}

		auto iter = m_boss_damage_person_map.find(role_guid);
		if (iter == m_boss_damage_person_map.end())
			return;

		if (role_guid == iter->second.master_guid)
		{
			clear_all_damage(role_guid, true);
			return;
		}

		guid_64 master_guid = iter->second.master_guid;
		auto master_iter = m_boss_damage_person_map.find(master_guid);
		if (master_iter == m_boss_damage_person_map.end())
			return;

		master_iter->second.damage_value += iter->second.damage_value;
		if(player_ref.get_team_info().captain_guid == master_guid)
		{
			//队员离开会顺便取消他召唤的助战者
			for (auto i = 0; i < 5; ++i)
			{
				if (iter->second.helper_guid[i].is_valid())
				{
					auto helper_iter = m_boss_damage_person_map.find(iter->second.helper_guid[i]);
					if (helper_iter != m_boss_damage_person_map.end())
					{
						master_iter->second.damage_value += helper_iter->second.damage_value;
						m_boss_damage_person_map.erase(helper_iter);
					}
					player& helper_ref = unit_man::get_player(iter->second.helper_guid[i]);
					if (helper_ref.is_valid())
					{
						send_leave_boss_damage_to_player(&helper_ref);
					}
				}
			}
			m_boss_damage_person_map.erase(iter);
			player_ref.get_assist_fight_mgr().cancel_assist_fight();
			send_leave_boss_damage_to_player(&player_ref);
		}
		else
		{
			//助战者退出助战
			for (auto i = 0; i < 5; ++i)
			{
				if (master_iter->second.helper_guid[i] == role_guid)
				{
					master_iter->second.helper_guid[i].clear_data();
					m_boss_damage_person_map.erase(iter);
					send_leave_boss_damage_to_player(&player_ref);
					player_ref.get_assist_fight_mgr().cancel_assist_fight();
					break;
				}
			}
		}
	}


	void npc::clear_all_damage(guid_64 role_guid, bool is_master)
	{
		//队长离开伤害列表
		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid() == false)
			return;

		auto iter = m_boss_damage_person_map.find(role_guid);
		if (iter == m_boss_damage_person_map.end())
			return;

		if (player_ref.is_player_in_team() && player_ref.is_player_team_captain())
		{
			s_player_team_info& player_team_info = player_ref.get_team_cs_mgr().get_unit_team_info();
			for (int32 i = 0; i < max_team_member_num; ++i)
			{
				if (player_team_info.team_mems_guid[i] != role_guid)
				{
					clear_all_damage(player_team_info.team_mems_guid[i], false);
				}
			}
		}


		for (int32 i = 0; i < 5; ++i)
		{
			if (iter->second.helper_guid[i].is_valid())
			{
				auto helper_iter = m_boss_damage_person_map.find(iter->second.helper_guid[i]);
				if (helper_iter != m_boss_damage_person_map.end())
				{
					player& helper_ref = unit_man::get_player(iter->second.helper_guid[i]);
					if (helper_ref.is_valid())
					{
						send_leave_boss_damage_to_player(&helper_ref);
					}
					m_boss_damage_person_map.erase(helper_iter);
				}
			}
		}
		m_boss_damage_person_map.erase(iter);
		send_leave_boss_damage_to_player(&player_ref);

		if (!is_master)
			return;
		for (auto total_iter = m_boss_damage_total_vec.begin(); total_iter != m_boss_damage_total_vec.end(); ++total_iter)
		{
			if (total_iter->role_guid == role_guid)
			{
				m_boss_damage_total_vec.erase(total_iter);
				return;
			}
		}
	}

	void npc::transfer_damage_to_killer(guid_64 role_guid, guid_64 killer_guid, int32 kill_index)
	{
		auto iter = m_boss_damage_person_map.find(role_guid);
		if (iter == m_boss_damage_person_map.end())
			return;

		int64 transfer_damage = iter->second.damage_value / 5;
		//先做伤害扣除
		iter->second.damage_value -= transfer_damage;
		if (iter->second.master_guid != iter->second.role_guid)
		{
			iter = m_boss_damage_person_map.find(iter->second.master_guid);
		}
		if (iter == m_boss_damage_person_map.end())
			return;

		for (auto total_iter = m_boss_damage_total_vec.begin(); total_iter != m_boss_damage_total_vec.end(); ++total_iter)
		{
			if (total_iter->role_guid == iter->second.role_guid)
			{
				total_iter->damage_value -= transfer_damage;
				break;
			}
		}

		player& killer = unit_man::get_player(kill_index);
		if (killer.is_valid() == false || killer.get_unit_guid() != killer_guid)
		{
			return;
		}
		auto killer_iter = m_boss_damage_person_map.find(killer_guid);
		if (killer_iter == m_boss_damage_person_map.end())
			return;
		//在做伤害增加
		add_boss_damage_vaule(&killer, transfer_damage);
	}

	void npc::send_boss_damage_helper_reward(guid_64 role_guid, int32 ranking_num, const std::vector< s_item_template_info >& drop_id_list)
	{
		player& master_ref = unit_man::get_player(role_guid);
		if (master_ref.is_valid() == false)
			return;
		auto iter = m_boss_damage_person_map.find(role_guid);
		if (iter == m_boss_damage_person_map.end())
			return;
		bool has_helper = false;
		xstring first_name = m_boss_damage_total_vec.size() >= 1 ? m_boss_damage_total_vec[0].role_name : "";
		std::vector<guid_64> helper_vec;
		std::vector<int32>  money_vec;
		{

			std::string title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_help_reward_title);
			std::string context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_help_reward_content);

			std::vector<s_item_template_info> item_list;
			//s_item_template_info reward_item;
			//reward_item.item_template_id = m_npc_template_ptr->HelperReward;
			//reward_item.item_num = 1;
			//reward_item.item_lock = 1;
			//item_list.push_back(reward_item);

			int32 notice_id = 93000424;
			std::vector<std::string> vec_notice_str;

			int32 money_type = e_money_type_assist_fighting;
			int32 money_num = 0;


			for (int32 i = 0; i < 5; ++i)
			{
				if (iter->second.helper_guid[i].is_valid())
				{
					vec_notice_str.clear();
					auto helper_iter = m_boss_damage_person_map.find(iter->second.helper_guid[i]);
					if (helper_iter != m_boss_damage_person_map.end())
					{
						has_helper = true;
						money_num = 0;
						player& helper_ref = unit_man::get_player(iter->second.helper_guid[i]);
						if (helper_ref.is_valid() && helper_ref.check_can_get_helper_reward())
						{
							int32 money_type = e_money_type_assist_fighting;
							money_num = helper_ref.get_help_value(m_npc_template_ptr->HelperReward);
							helper_ref.delete_get_helper_reward_times(money_num);

							helper_ref.add_money(e_money_type_assist_fighting, money_num, e_server_log_add_money_assist_reward, -1, true);
						}

						helper_ref.get_mission_mgr().target_check(e_mission_end_type_assis_fight_num);
						vec_notice_str.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
						vec_notice_str.push_back(helper_ref.get_name());
						vec_notice_str.push_back(master_ref.get_name());
						
						if (m_npc_template_ptr != nullptr)
						{
							vec_notice_str.push_back(template_manager::get_instance().get_str_by_string_template_id(m_npc_template_ptr->NpcName));
							vec_notice_str.push_back(init_unit::change_i32_to_string(m_npc_template_ptr->Level));
						}
						xstring notice_str = init_unit::implode(vec_notice_str);
						helper_ref.get_chat_mgr().send_notice(notice_id, notice_str, helper_ref.get_legion_guid());

						helper_vec.push_back(iter->second.helper_guid[i]);
						money_vec.push_back(money_num);
					}
				}
			}
		}
		if (has_helper)
		{
			if (master_ref.check_can_get_be_helped_reward())
			{
				std::string title = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_helper_reward_title);
				std::string context = globle_data::get_instance().get_mail_common_text_id(e_mail_common_text_helper_reward_content);

				std::vector<s_item_template_info> item_list;
				//s_item_template_info reward_item;
				//reward_item.item_template_id = m_npc_template_ptr->BeHelpedReward;
				//reward_item.item_num = 1;
				//reward_item.item_lock = 1;
				//item_list.push_back(reward_item);
				int32 money_type = e_money_type_assist_fighting;
				int32 money_num = master_ref.get_help_value(m_npc_template_ptr->BeHelpedReward); 
			
				master_ref.add_money(e_money_type_assist_fighting, money_num, e_server_log_add_money_assist_reward, -1, true);
				master_ref.get_assist_fight_mgr().assist_fight_end_to_show_reward(helper_vec, money_num, money_vec, drop_id_list, first_name);
				master_ref.delete_get_be_helped_reward_times(money_num);
				//master_ref.get_mission_mgr().target_check(e_mission_end_type_by_assis_fight_num);
			}
			else
			{
				master_ref.get_assist_fight_mgr().assist_fight_end_to_show_reward(helper_vec, 0, money_vec, drop_id_list, first_name);
			}
		}
		else
		{
			master_ref.get_assist_fight_mgr().assist_fight_end_to_show_reward(helper_vec, 0, money_vec, drop_id_list, first_name);
		}
	}

	void npc::create_gain_treasure_drop_record(player& player_ref, const std::vector< s_item_template_info >& item_list, std::vector<s_gain_treasure_record_player_info_one_things>& player_info)
	{
		for (auto& iter : item_list)
		{
			ItemTemplate* item_temp_ptr = GET_TEMPLATE(ItemTemplate, iter.m_item_id);
			if (item_temp_ptr == nullptr)
			{
				continue;
			}
			if (item_temp_ptr->DropRecordModel <= e_drop_record_model_type_no)
			{
				continue;
			}
			s_gain_treasure_record_player_info_one_things tmp_one;
			tmp_one.role_guid = player_ref.get_unit_guid();
			memcpy(tmp_one.role_name, player_ref.get_name(), max_name_size);
			tmp_one.gain_item_template_id = iter.m_item_id;
			player_info.push_back(tmp_one);
		}
	}

	void npc::change_team_captain(guid_64 old_captain, guid_64 new_captain)
	{
		auto old_iter = m_boss_damage_person_map.find(old_captain);
		auto new_iter = m_boss_damage_person_map.find(new_captain);

		if (old_iter != m_boss_damage_person_map.end() && new_iter != m_boss_damage_person_map.end())
		{
			if (old_iter->second.master_guid == new_captain)
				return;
			player& new_captain_ref = unit_man::get_player(new_captain);
			if (new_captain_ref.is_valid() == false)
				return;
			s_player_team_info& player_team_info = new_captain_ref.get_team_cs_mgr().get_unit_team_info();
			for (int32 i = 0; i < max_team_member_num; ++i)
			{
				if (player_team_info.team_mems_guid[i].is_valid())
				{
					auto mem_iter = m_boss_damage_person_map.find(player_team_info.team_mems_guid[i]);
					if (mem_iter != m_boss_damage_person_map.end())
						mem_iter->second.master_guid = new_captain;
				}
			}
			for (auto total_iter = m_boss_damage_total_vec.begin(); total_iter != m_boss_damage_total_vec.end(); ++total_iter)
			{
				if (total_iter->role_guid == old_captain)
				{
					total_iter->role_guid = new_captain_ref.get_unit_guid();
					memcpy(total_iter->role_name, new_captain_ref.get_name(), max_name_size);
					break;
				}
			}
		}
		else if(old_iter == m_boss_damage_person_map.end() && new_iter != m_boss_damage_person_map.end())
		{
			player& new_captain_ref = unit_man::get_player(new_captain);
			if (new_captain_ref.is_valid() == false)
				return;
			s_player_team_info& player_team_info = new_captain_ref.get_team_cs_mgr().get_unit_team_info();
			for (int32 i = 0; i < max_team_member_num; ++i)
			{
				if (player_team_info.team_mems_guid[i].is_valid())
				{
					auto mem_iter = m_boss_damage_person_map.find(player_team_info.team_mems_guid[i]);
					if (mem_iter != m_boss_damage_person_map.end() && mem_iter->second.master_guid != new_captain)
					{
						role_add_team(player_team_info.team_mems_guid[i], new_captain);
					}
				}
			}
		}
		else if (old_iter != m_boss_damage_person_map.end() && new_iter == m_boss_damage_person_map.end())
		{
			//防止新队长不在线 使用就队长的角色引用
			player& old_captain_ref = unit_man::get_player(old_captain);
			if (old_captain_ref.is_valid() == false)
				return;
			int64 team_mem_damage_value = 0;
			s_player_team_info& player_team_info = old_captain_ref.get_team_cs_mgr().get_unit_team_info();
			for (int32 i = 0; i < max_team_member_num; ++i)
			{
				if (player_team_info.team_mems_guid[i].is_valid())
				{
					auto mem_iter = m_boss_damage_person_map.find(player_team_info.team_mems_guid[i]);
					if (mem_iter != m_boss_damage_person_map.end() && mem_iter->second.master_guid != old_captain)
					{
						mem_iter->second.master_guid = mem_iter->second.role_guid;
						team_mem_damage_value += mem_iter->second.damage_value;
						for (auto helper_index = 0; helper_index < 5; ++helper_index)
						{
							if (mem_iter->second.helper_guid[helper_index].is_valid())
							{
								auto helper_iter = m_boss_damage_person_map.find(mem_iter->second.helper_guid[helper_index]);
								if (helper_iter != m_boss_damage_person_map.end())
								{
									team_mem_damage_value += helper_iter->second.damage_value;
								}
							}
						}
						s_boss_damage_list_total_item new_item;
						new_item.role_guid = mem_iter->second.role_guid;
						memcpy(new_item.role_name, mem_iter->second.role_name, max_name_size);
						new_item.damage_value = team_mem_damage_value;
						m_boss_damage_total_vec.push_back(new_item);
					}
				}
			}
			if (team_mem_damage_value)
			{
				for (auto total_iter = m_boss_damage_total_vec.begin(); total_iter != m_boss_damage_total_vec.end(); ++total_iter)
				{
					if (total_iter->role_guid == old_captain)
					{
						total_iter->damage_value -= team_mem_damage_value;
						break;
					}
				}
			}
		}
	}

	void npc::role_leave_team(guid_64 role_guid, guid_64 captain_guid)
	{
		if (role_guid == captain_guid)
			return;
		auto role_iter = m_boss_damage_person_map.find(role_guid);
		auto capt_iter = m_boss_damage_person_map.find(captain_guid);

		if (role_iter == m_boss_damage_person_map.end() || capt_iter == m_boss_damage_person_map.end())
			return;
		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid())
		{
			player_ref.get_assist_fight_mgr().cancel_assist_fight();
		}
	}

	void npc::role_add_team(guid_64 role_guid, guid_64 captain_guid)
	{
		if (role_guid == captain_guid)
			return;
		auto role_iter = m_boss_damage_person_map.find(role_guid);
		auto capt_iter = m_boss_damage_person_map.find(captain_guid);

		if (role_iter == m_boss_damage_person_map.end() || capt_iter == m_boss_damage_person_map.end())
			return;

		if (role_iter->second.master_guid == role_guid)
		{
			role_iter->second.master_guid = captain_guid;
			int64 role_damage = 0;
			for (auto iter = m_boss_damage_total_vec.begin(); iter != m_boss_damage_total_vec.end(); ++iter)
			{
				if (iter->role_guid == role_guid)
				{
					role_damage = iter->damage_value;
					m_boss_damage_total_vec.erase(iter);
					break;
				}
			}
			for (auto iter = m_boss_damage_total_vec.begin(); iter != m_boss_damage_total_vec.end(); ++iter)
			{
				if (iter->role_guid == captain_guid)
				{
					iter->damage_value += role_damage;
					break;
				}
			}
		}
	}

	void npc::transfer_damage_to_int32(int64 real_damage_value, int32& damage_value, int32& damage_rate)
	{
		if (real_damage_value > 10000000)
		{
			damage_value = real_damage_value / 10000000;
			damage_rate = 3;
		}
		else if (real_damage_value > 10000)
		{
			damage_value = real_damage_value / 10000;
			damage_rate = 2;
		}
		else
		{
			damage_value = real_damage_value;
			damage_rate = 1;
		}
	}

	void npc::send_boss_damage_ranking_reward()
	{
		send_boss_damage_list_to_all_player();

		int32 max_ranking = m_boss_damage_total_vec.size();
		for (int32 i = 1; i <= max_ranking; ++i)
		{
			std::vector<int32>& rwd_data = m_npc_template_ptr->RankRewards;
			
			auto& tmp_item = m_boss_damage_total_vec[i - 1];
			player& master_ref = unit_man::get_player(tmp_item.role_guid);
			if (master_ref.is_valid() == false)
				continue;
			int32 drop_bag_id = globle_data::get_instance().get_drop_bag_by_rank_rwd(i, rwd_data);
			if (m_npc_template_ptr->FirstKillDropId.size() > 0)
			{
				if (!master_ref.get_is_have_cur_first_kill_npc_id(m_npc_template_ptr->attribute_id))
				{
					drop_bag_id = m_npc_template_ptr->FirstKillDropId[0];
					master_ref.insert_first_npc_kill_id_str(m_npc_template_ptr->attribute_id);
				}
			}
			if (drop_bag_id == 0)
				break;

			std::vector<guid_64> team_member;
			team_member.clear();
			if (master_ref.is_player_in_team() && master_ref.is_player_team_captain())
			{
				s_player_team_info& player_team_info = master_ref.get_team_cs_mgr().get_unit_team_info();
				for (int32 team_mem_index = 0; team_mem_index < max_team_member_num; ++team_mem_index)
				{
					auto tmp_team_member_guid = player_team_info.team_mems_guid[team_mem_index];
					if (tmp_team_member_guid.is_valid() && m_boss_damage_person_map.find(tmp_team_member_guid) != m_boss_damage_person_map.end())
					{
						player& member_ref = unit_man::get_player(tmp_team_member_guid);
						if (member_ref.is_valid() && member_ref.check_can_get_boss_drop(this))
							team_member.push_back(tmp_team_member_guid);
					}
				}
			}
			else
			{
				if (master_ref.is_valid() && master_ref.check_can_get_boss_drop(this))
				{
					team_member.push_back(master_ref.get_unit_guid());
				}
			}
			drop_manager::create_drop_for_boss_damage_ranking(*this, i, drop_bag_id, team_member);
		}

		std::vector<guid_64>  tmp_vec;
		for (auto iter = m_boss_damage_person_map.begin(); iter != m_boss_damage_person_map.end(); ++iter)
		{
			tmp_vec.push_back(iter->second.role_guid);
		}

		for (int32 i = 0; i < tmp_vec.size(); i++)
		{
			player& master_ref = unit_man::get_player(tmp_vec[i]);
			if (master_ref.is_valid())
			{
				master_ref.get_assist_fight_mgr().clear_assist_fight_boss_id();
				master_ref.get_assist_fight_mgr().cancel_assist_fight();
			}

		}



		

		//for (auto iter = m_boss_damage_person_map.begin(); iter != m_boss_damage_person_map.end(); ++iter)
		//{
		//	player& master_ref = unit_man::get_player(iter->second.role_guid);
		//	if (master_ref.is_valid())
		//	{
		//		master_ref.get_assist_fight_mgr().clear_assist_fight_boss_id();
		//		master_ref.get_assist_fight_mgr().cancel_assist_fight();
		//	}
		//}

		m_boss_damage_person_map.clear();
		m_boss_damage_total_vec.clear();
	}
	void npc::send_assist_fight_tip(guid_64 assist_guid, guid_64 team_guid)
	{
		if ( false == assist_guid.is_valid() || false == team_guid.is_valid())
		{
			return;
		}
		if (assist_guid == team_guid)
		{
			return;
		}
		unit& assist_ref = unit_man::get_unit(assist_guid);
		if (false == assist_ref.is_valid())
		{
			return;
		}
		unit& master_ref = unit_man::get_unit(team_guid);
		if (false == master_ref.is_valid())
		{
			return;
		}

		assist_fight_proto_send_be_assist_fight_tip master_info;
		master_info.set_role_name(master_ref.get_name());
		master_info.set_head_icon(master_ref.get_unit_info(e_role_info_class_type));
		master_info.set_is_master(true);
		if (assist_ref.get_map_guid() == get_map_guid() && assist_ref.get_session_state() == e_session_status_in_gaming)
		{
			assist_ref.send_message_to_self(&master_info, e_msgindex_s2c_send_be_assist_fight_tip);
		}
		
		assist_fight_proto_send_be_assist_fight_tip help_info;
		help_info.set_role_name(assist_ref.get_name());
		help_info.set_head_icon(assist_ref.get_unit_info(e_role_info_class_type));
		help_info.set_is_master(false);
		if (master_ref.get_map_guid() == get_map_guid() && master_ref.get_session_state() == e_session_status_in_gaming)
		{
			master_ref.send_message_to_self(&help_info, e_msgindex_s2c_send_be_assist_fight_tip);
		}
	}
	void npc::on_mission_event()
	{
		if (get_unit_type() != e_unit_type_monster || get_unit_sub_type() != e_monster_type_boss)
		{
			return;

		}

		int32 map_type = base_map_system::get_map_type(get_map_ent());
		if (map_type != e_map_type_boss_single && map_type != e_map_type_boss_home && map_type != e_map_type_single_land_boss)
		{
			return;
		}
		std::vector<team_set> vec_team_set = get_aoi_team_hate_list();
		const int32 vec_team_set_size = vec_team_set.size();
		for (int i = 0; i < vec_team_set_size; i++)
		{
			team_set& temp_team_set = vec_team_set[i];
			const int32 team_member_num = temp_team_set.get_team_member_num();

			for (int32 j = 0; j < team_member_num; j++)
			{
				player* temp_player_ptr = temp_team_set.get_player_ptr(j);
				if (nullptr == temp_player_ptr)
				{
					continue;
				}
				if (map_type == e_map_type_boss_single)
				{
					temp_player_ptr->get_mission_mgr().target_check(e_mission_end_type_kill_any_single_boss, 1);
				}
				else
				{
					temp_player_ptr->get_mission_mgr().target_check(e_mission_end_type_kill_any_home_boss, 1);
				}	
			}
		}
		//sync_boss_rank_result();
	}

	void npc::sync_boss_rank_result()
	{
		if (m_npc_template_ptr->NpcType == e_unit_type_monster && init_unit::is_world_boss(m_npc_template_ptr->SubType))
		{

			m_score_rank_indicator.sort();

			int32 record_size = m_score_rank_indicator.get_score_record_list_all().size();
			for (int32 i = 0; i < record_size; ++i)
			{
				player& player_ref = unit_man::get_player(m_score_rank_indicator.get_score_record_list_all()[i].role_guid);
				if (player_ref.is_valid())
				{
					sync_single_boss_rank_result(player_ref);
				}
			}
		}
	}
	void npc::sync_single_boss_rank_result(player & player_ref)
	{
		game_proto_boss_result_rank_list_end result_msg;

		int32 top_num = 0;

		for (int32 i = 0; i < 3; i++)
		{
			score_record * tmp = m_score_rank_indicator.get_score_record_at(i);
			if (tmp == nullptr)
			{
				break;
			}
			game_proto_boss_rank_item *item = result_msg.add_top_list();
			if (item == nullptr)
			{
				break;
			}
			item->set_role_name(tmp->role_name);
			item->set_role_guid_a(tmp->role_guid.A);
			item->set_role_guid_b(tmp->role_guid.B);
			item->set_damage_value(init_unit::change_i64_to_string(tmp->score));
			item->set_rank(i + 1);
		}

		result_msg.set_top_num(top_num);

		game_proto_boss_rank_item *my_item = result_msg.mutable_my_rank();
		if (my_item != nullptr)
		{
			my_item->set_role_name("");
			my_item->set_role_guid_a(0);
			my_item->set_role_guid_b(0);
			my_item->set_damage_value("");
			my_item->set_rank(0);
		}
		int32  my_rank_index = 0;
		score_record * tmp = m_score_rank_indicator.get_score_record_and_idx(player_ref.get_unit_guid(), my_rank_index);
		if (tmp != nullptr && my_item != nullptr)
		{
			my_item->set_role_name(tmp->role_name);
			my_item->set_role_guid_a(tmp->role_guid.A);
			my_item->set_role_guid_b(tmp->role_guid.B);
			my_item->set_damage_value(init_unit::change_i64_to_string(tmp->score));
			my_item->set_rank(my_rank_index);
		}
		result_msg.set_boss_template_id(get_npc_template_id());

		player_ref.send_message_to_self(&result_msg, e_msgindex_s2c_boss_result_rank_end);

	}

}