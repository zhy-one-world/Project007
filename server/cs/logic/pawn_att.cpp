/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 20:00
  file base: pawn_att
  file ext: cpp
  author: S.L.H.(孙力华)
  
  purpose: 
*********************************************************************/

/************************************************************************/
/*                             File Include                             */
/************************************************************************/
#include "internal/att_msg.hpp"
#include "cpp_damage_counter.h"
#include "internet/cs2dp.pb.h"
#include "logic/buff_man.h"
#include "logic/item_set.h"
#include "logic/player.hpp"
#include "logic/unit_man.h"
#include "pawn_att.hpp"
#include "send_skill.h"
#include "server_log.hpp"
#include "server_log.hpp"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include "utility/parse_msg.h"
#include "internet/character.pb.h"
#include "internet/net.pb.h"
#include "npc.hpp"
#include "utility/init_unit.h"

/************************************************************************/
/*                           Class Implement                            */
/************************************************************************/
namespace faith
{

	pawn_att::pawn_att()
	{
		m_unit_array_index = 0;
		clear_data();
	}

	pawn_att::~pawn_att()
	{
	}
	void pawn_att::clear_data()
	{
		m_unit_base_att.clear_data();
		m_unit_tick_begin = false;
		memset(m_unit_att_all, 0, sizeof(m_unit_att_all));
		memset(m_unit_att_end, 0, sizeof(m_unit_att_end));
		memset(m_game_att, 0, sizeof(m_game_att));
		memset(m_state_att, 0, sizeof(m_state_att));
		memset(m_state_trigger, 0, sizeof(m_state_trigger));
		memset(m_state_att_percent, 0, sizeof(m_state_att_percent));
		clear_att_update_mask();
		m_att_change = false;
		m_is_update_att = false;
		m_be_hurt_type = e_be_hurt_type_nomal;
		m_use_owner_att = 0;
		m_cur_hp_last_sync = 0;
		m_cur_hp_last_sync_ws = 0;
		m_max_hp_last_sync_ws = 0;
		m_back_hp_magic_time = 0;
		m_back_hp_buff_time = 0;
		m_next_3_sec_tick_time = 0;
		m_back_hp_energy_time = 0;
		m_adversity_aid_buff_list.clear();
		m_damage_absorb_list.clear();
		m_last_move_time = 0;
		m_max_move_length = 0;
		m_close_speed = false;
		m_exp_add_map.clear();
		m_is_dead = 0;
	}

	void pawn_att::heart_tick(const int64& new_time)
	{
		if (m_unit_tick_begin)
		{
			syn_att_change();
		}
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid())
		{
			if (get_game_att(e_unit_game_att_war_state) == e_war_state_peace)
			{
				if (new_time >= m_back_hp_magic_time)
				{
					m_back_hp_magic_time = new_time + back_hp_magic_time;
					if (get_unit_base_att(e_base_att_info_hp_cur) < get_attack_att_value(e_unit_attack_att_hp_max))
					{
						use_buff_param ub_param;
						ub_param.attacker_array_index = m_unit_array_index;
						ub_param.target_array_index = m_unit_array_index;
						ub_param.damage_base_value = -GAMECONFIG->BackHpNum;
						ub_param.damage_style = edt_real;
						cpp_damage_counter::buff_damage_by_server(ub_param, true);
					}
					change_magic(-GAMECONFIG->BackMagicNum);
				}
				if (new_time >= m_back_hp_buff_time)
				{
					m_back_hp_buff_time = new_time + second_tick_time;
					double add_value = get_game_att(e_unit_game_att_back_hp_value);
					add_value += get_attack_att_value(e_unit_attack_att_hp_max) * get_game_att(e_unit_game_att_back_hp_percent);
					if (add_value > 0 && get_unit_base_att(e_base_att_info_hp_cur) < get_attack_att_value(e_unit_attack_att_hp_max))
					{
						use_buff_param ub_param;
						ub_param.attacker_array_index = m_unit_array_index;
						ub_param.target_array_index = m_unit_array_index;
						ub_param.damage_base_value = -add_value;
						ub_param.damage_style = edt_real;
						cpp_damage_counter::buff_damage_by_server(ub_param, true);
					}
				}	
			}
			if (new_time >= m_back_hp_energy_time)
			{
				for (int32 i =0; i < GAMECONFIG->SprintbuffIdArray.size(); i++)
				{
					if (buff_man::is_have_buff_by_template_id(m_unit_array_index, m_unit_array_index, GAMECONFIG->SprintbuffIdArray[i], true))
					{
						return;
					}
				}
				m_back_hp_energy_time = new_time + second_tick_time;
				PlayerUpgradeTemplate* player_upgrade_template_ptr = template_manager::get_instance().get_player_upgrade_by_level(player_ref.get_unit_info(e_role_info_class_type), player_ref.get_unit_info(e_role_info_exp_level));
				if (nullptr != player_upgrade_template_ptr)
				{
					change_energy(-player_upgrade_template_ptr->ReplyEnergy);
				}
			}
		}

		int64 cur_hp = get_unit_base_att(e_base_att_info_hp_cur);
		if (m_cur_hp_last_sync != cur_hp)
		{
			send_base_att_one(e_base_att_info_hp_cur);
			m_cur_hp_last_sync = cur_hp;
		}

		if (new_time >= m_next_3_sec_tick_time)
		{
			m_next_3_sec_tick_time = new_time + 3 * second_tick_time;
			heart_tick_3_sec(new_time);
		}
	}

	void pawn_att::heart_tick_3_sec(const int64& new_time)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}

		int64 cur_hp = get_unit_base_att(e_base_att_info_hp_cur);
		int64 max_hp = get_attack_att_value(e_unit_attack_att_hp_max);
		if (m_cur_hp_last_sync_ws != cur_hp)
		{
			m_cur_hp_last_sync_ws = cur_hp;
			player_ref.sync_data_to_ws(e_sync_cs2ws_data_cur_hp, cur_hp);
		}
		if (m_max_hp_last_sync_ws != max_hp)
		{
			m_max_hp_last_sync_ws = max_hp;
			player_ref.sync_data_to_ws(e_sync_cs2ws_data_max_hp, max_hp);
		}
	}

	void pawn_att::init_unit_att_over(bool use_owner)
	{
		calcu_unit_att_by_level(use_owner);
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, unit_ref.get_unit_info(e_role_info_move_map_id));
		if (map_template_ptr && map_template_ptr->Type == faith::e_map_type_big_map)
		{
			if (get_game_att(e_unit_game_att_movement) == e_move_ment_walk)
			{
				if (unit_ref.get_unit_info(e_role_info_wing_showd_template_id) > 0)
				{
					set_game_att(e_unit_game_att_movement, e_move_ment_wing_walk, true);
				}
			}
			if (get_game_att(e_unit_game_att_movement) == e_move_ment_wing_fly)
			{
				if (unit_ref.get_unit_info(e_role_info_wing_showd_template_id) <= 0)
				{
					unit_ref.get_item_set().show_this_init_max_wing();
				}
			}
		}
		m_unit_tick_begin = true;
	}
	void pawn_att::calcu_unit_att_by_level(bool use_ower)
	{
		if (false == use_ower)
		{
			unit& unit_ref = unit_man::get_unit(m_unit_array_index);
			update_unit_att_info(unit_ref.get_unit_info(e_role_info_upgrade_id), true);
		}

		if (get_unit_base_att(e_base_att_info_hp_cur) <= 0)
		{
			set_unit_base_att(e_base_att_info_hp_cur, get_attack_att_value(e_unit_attack_att_hp_max));
		}
	}
	s_arena_char_fight_att pawn_att::get_unit_fight_att_inst()
	{
		s_arena_char_fight_att fight_info;
		for (int32 i = 0; i < e_unit_attack_att_max; ++i)
		{
			fight_info.att_value[i] = m_unit_att_end[i];
		}
		return fight_info;
	}
	void pawn_att::load_unit_att_by_db(const s_arena_char_fight_att& unit_att)
	{
		for (int32 i = 0; i < e_unit_attack_att_max; ++i)
		{
			m_unit_att_end[i] = unit_att.att_value[i];
			m_unit_att_all[send_gs_to_client][epai_self_value].att[i] = unit_att.att_value[i];
		}
	}
	void pawn_att::load_base_att_by_db(const s_base_att_info& bast_att)
	{
		m_unit_base_att = bast_att;
		for (int32 i = e_base_att_info_none; i < e_base_att_info_hp_cur; ++ i)
		{
			int64 value = m_unit_base_att.data_ary[i] + m_unit_base_att.data_ary[i + 11];
			apply_att_change_effect(4, i, value, 0, send_gs_to_client, true);
		}
	}
	bool pawn_att::load_base_att_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::cs2dp_proto::role_att_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		s_base_att_info db_data;
		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			db_data.data_ary[i] = msg.data_ary(i);
		}


		load_base_att_by_db(db_data);

		return true;
	}
	void pawn_att::save_base_att_to_db(int32 save_type_ex)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);


		bool is_use = template_manager::get_instance().is_message_use_lua(e_msgindex_cs2dp_save_char_att);
		if (is_use == false)
		{
			cs2dp_save_char_att request;
			request.save_type_ex = save_type_ex;
			request.role_guid = player_ref.get_unit_guid();
			request.unit_array_index = player_ref.get_array_index();
			request.data_info = m_unit_base_att;
			for (int32 i = 0; i < e_unit_attack_att_max; ++i)
			{
				request.attack_att[i] = m_unit_att_end[i];
			}
			player_ref.send_message_to_dp(&request, sizeof(request));
		}
		else
		{
			faith::cs2dp_proto::save_role_att msg;
			msg.set_role_guid(player_ref.get_unit_guid().server_64);
			msg.set_unit_array_index(player_ref.get_array_index());
			msg.set_save_type_ex(save_type_ex);
			faith::cs2dp_proto::role_att_db *db_data = msg.mutable_db_data();
			if (db_data == nullptr)
			{
				return;
			}
			for (int32 i = 0; i < e_base_att_info_max; i++)
			{
				db_data->add_data_ary(m_unit_base_att.data_ary[i]);
			}
			for (int32 j = 0; j < e_unit_attack_att_max; j++)
			{
				db_data->add_attack_att(m_unit_att_end[j]);
			}
			player_ref.send_message_to_dp_lua(&msg, e_msgindex_cs2dp_save_char_att);
		}
	}
	void pawn_att::send_base_att_all()
	{
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);

		character_proto_base_att_all att_msg;
		att_msg.set_unit_guid(unit_ref.get_unit_guid().server_64);
		for (int32 i = 1; i < e_base_att_info_max; ++i)
		{
			att_msg.add_att_ary(m_unit_base_att.data_ary[i]);
		}
		unit_ref.send_message_to_self(&att_msg, e_msgindex_s2c_character_base_att_all);
	}
	void pawn_att::send_base_att_one(int32 att_index)
	{
		if (att_index <= 0 || att_index >= e_base_att_info_max)
		{
			return;
		}
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);

		character_proto_base_att_one att_msg;
		att_msg.set_unit_guid(unit_ref.get_unit_guid().server_64);
		att_msg.set_att_type(att_index);
		att_msg.set_att_value(m_unit_base_att.data_ary[att_index]);
		unit_ref.send_message_to_aoi(&att_msg, e_msgindex_s2c_character_base_att_one);
	}
	void pawn_att::set_unit_base_att(int32 att_index, int64 iValue)
	{
		if (att_index <= 0 || att_index >= e_base_att_info_max)
		{
			return;
		}
		m_unit_base_att.data_ary[att_index] = iValue;

		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, unit_ref.get_unit_info(e_role_info_move_map_id));
		if (map_template_ptr && map_template_ptr->Type == faith::e_map_type_big_map && att_index == e_base_att_info_hp_cur)
		{
			m_unit_base_att.data_ary[e_base_att_info_hp_main] = iValue;
		}
		if (att_index <= e_base_att_info_stamina)
		{
			player& player_ref = unit_man::get_player(m_unit_array_index);
			if (player_ref.is_valid() && player_ref.get_write_log())
			{
				int32 strength = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_strength) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_strength);
				int32 agility = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_agility) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_agility);
				int32 intellect = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_intellect) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_intellect);
				int32 stamina = player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_stamina) + player_ref.get_pawn_att().get_unit_base_att(e_base_att_info_body_stamina);
				server_log::player_mark_log(player_ref.get_third_info(), player_ref.get_unit_info_inst(), strength, agility, intellect, stamina);
				
			}
		}
	}

	int64 pawn_att::get_unit_base_att(int32 att_index)
	{
		if (att_index <= 0 || att_index >= e_base_att_info_max)
		{
			return 0; 
		}
		return m_unit_base_att.data_ary[att_index];
	}

	void pawn_att::exp_levelup()
	{
		int32 map_type = 0;
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid())
		{
			map_type = base_map_system::get_map_type(player_ref.get_map_ent());
		}
		if (true == init_unit::is_levelup_add_hp(map_type))
		{
			set_unit_base_att(e_base_att_info_hp_cur, get_attack_att_value(e_unit_attack_att_hp_max));
		}
		syn_att_change();
		syn_all_att();
	}

	void pawn_att::change_gs_for_att(e_pawn_att_index pawn_att_index, const std::vector<int32>& change_arr, int32 value, int32 is_show)
	{
		if (change_arr.size() < 0 || change_arr.size() % 2 != 0)
		{
			return;
		}
		for (int32 i = 0; i < change_arr.size(); i += 2)
		{
			att_modifier(pawn_att_index, change_arr[i], value * change_arr[i + 1], is_show, true);
		}
	}

	void pawn_att::strength_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show)//力量属性变化增加二级属性
	{
		switch (class_type)
		{

		case e_class_type_barserker://战士
			change_gs_for_att(pawn_att_index, GAMECONFIG->BarserkerSTREffectAttArr, value, is_show);
			break;
		case e_class_type_wizard://法师
			change_gs_for_att(pawn_att_index, GAMECONFIG->WizardSTREffectAttArr, value, is_show);
			break;
		case e_class_type_guardian://骑士
			change_gs_for_att(pawn_att_index, GAMECONFIG->GuardianSTREffectAttArr, value, is_show);
			break;
		case e_class_type_assassinator://刺客
			change_gs_for_att(pawn_att_index, GAMECONFIG->AssassinatorSTREffectAttArr, value, is_show);
			break;
		case  e_class_type_ranger://缺省
			att_modifier(pawn_att_index, e_unit_attack_att_attack_max, value * 0.3, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_armor, value * 1.6, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_hp_max, value * 2.7, is_show, true);
			break;
		default:break;
		}
		return;
	}
	void pawn_att::agility_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show)//敏捷属性变化增加二级属性
	{
		switch (class_type)
		{

		case e_class_type_barserker://战士
			change_gs_for_att(pawn_att_index, GAMECONFIG->BarserkerAGIEffectAttArr, value, is_show);
			break;
		case e_class_type_wizard://法师
			change_gs_for_att(pawn_att_index, GAMECONFIG->WizardAGIEffectAttArr, value, is_show);
			break;
		case e_class_type_guardian://骑士
			change_gs_for_att(pawn_att_index, GAMECONFIG->GuardianAGIEffectAttArr, value, is_show);
			break;
		case e_class_type_assassinator://刺客
			change_gs_for_att(pawn_att_index, GAMECONFIG->AssassinatorAGIEffectAttArr, value, is_show);
			break;
		case e_class_type_ranger://缺省
			att_modifier(pawn_att_index, e_unit_attack_att_attack_min, value * 0.8, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_hit_force, value * 0.3, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_dodge_force, value * 0.3, is_show, true);
			break;
		default:break;
		}
		return;
	}
	void pawn_att::stamina_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show)//耐力属性变化增加二级属性
	{
		switch (class_type)
		{

		case e_class_type_barserker://战士
			change_gs_for_att(pawn_att_index, GAMECONFIG->BarserkerSTAEffectAttArr, value, is_show);
			break;
		case e_class_type_wizard://法师
			change_gs_for_att(pawn_att_index, GAMECONFIG->WizardSTAEffectAttArr, value, is_show);
			break;
		case e_class_type_guardian://骑士
			change_gs_for_att(pawn_att_index, GAMECONFIG->GuardianSTAEffectAttArr, value, is_show);
			break;
		case e_class_type_assassinator://刺客
			change_gs_for_att(pawn_att_index, GAMECONFIG->AssassinatorSTAEffectAttArr, value, is_show);
			break;
		case e_class_type_ranger://缺省
			att_modifier(pawn_att_index, e_unit_attack_att_hp_max, value * 2.3, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_critical_armor_rate, value * 0.2, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_armor, value * 1.4, is_show, true);
			break;
		default:break;
		}
		return;
	}
	void pawn_att::intellect_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show)//智力属性增加二级属性
	{
		switch (class_type)
		{

		case e_class_type_barserker://战士
			change_gs_for_att(pawn_att_index, GAMECONFIG->BarserkerINTEffectAttArr, value, is_show);
			break;
		case e_class_type_wizard://法师
			change_gs_for_att(pawn_att_index, GAMECONFIG->WizardINTEffectAttArr, value, is_show);
			break;
		case e_class_type_guardian://骑士
			change_gs_for_att(pawn_att_index, GAMECONFIG->GuardianINTEffectAttArr, value, is_show);
			break;
		case e_class_type_assassinator://刺客
			change_gs_for_att(pawn_att_index, GAMECONFIG->AssassinatorINTEffectAttArr, value, is_show);
			break;
		case e_class_type_ranger://缺省
			att_modifier(pawn_att_index, e_unit_attack_att_attack_min, value * 0.2, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_attack_max, value * 0.9, is_show, true);
			att_modifier(pawn_att_index, e_unit_attack_att_critical_rate, value * 0.2, is_show, true);
			break;
		default:break;
		}
		return;
	}

	void pawn_att::apply_att_change_by_array(const std::vector<float>& att_array, bool is_add, f32 add_count)
	{
		if (att_array.size() == 0 || att_array.size() % e_att_one_max != 0)
		{
			CONSOLE_ERROR("att_array.size():{}", att_array.size());
			return;
		}
		for (int32 i = 0; i < att_array.size(); i += e_att_one_max)
		{
			int32 att_level = att_array[i + e_att_one_level] + 0.5f;
			int32 att_id = att_array[i + e_att_one_att_id] + 0.5f;
			f32 att_value = att_array[i + e_att_one_value] * add_count;
			f32 att_percent = att_array[i + e_att_one_percent] * add_count;
			int32 is_show = att_array[i + e_att_one_show];
			apply_att_change_effect(att_level, att_id, att_value, att_percent, is_show, is_add);
		}
	}
	void pawn_att::apply_att_change_by_array(const std::vector<std::string>& att_array, bool is_add, f32 add_count)
	{
		if (att_array.size() == 0 || att_array.size() % e_att_one_max != 0)
		{
			CONSOLE_ERROR("att_array.size():{}", att_array.size());
			return;
		}
		for (int32 i = 0; i < att_array.size(); i += e_att_one_max)
		{
			int32 att_level = boost::lexical_cast<float>(att_array[i + e_att_one_level]) + 0.5f;
			int32 att_id = boost::lexical_cast<float>(att_array[i + e_att_one_att_id]) + 0.5f;
			f32 att_value = boost::lexical_cast<float>(att_array[i + e_att_one_value]) * add_count;
			f32 att_percent = boost::lexical_cast<float>(att_array[i + e_att_one_percent]) * add_count;
			int32 is_show = boost::lexical_cast<float>(att_array[i + e_att_one_show]);
			apply_att_change_effect(att_level, att_id, att_value, att_percent, is_show, is_add);
		}
	}

	void pawn_att::apply_att_change_effect(int32 att_level, int32 id, f32 value, f32 percent, int32 is_show, bool apply)
	{
		if (e_unit_attack_att_none > id || e_unit_attack_att_max < id)
		{
			return;
		}
		if (att_level > tier_att_max || att_level < tier_att_min)
		{
			CONSOLE_INFO("att_level:{} id:{} value:{} percent:{} is_show:{} apply:{}", att_level, id, value, percent, is_show, apply);
			return;
		}
		att_modifier(e_pawn_att_index((att_level-2) * 2 - 2), id, value, is_show, apply);
		att_modifier(e_pawn_att_index((att_level-2) * 2 - 1), id, percent, is_show, apply);
	}

	f32	pawn_att::get_unit_att(uint32 att_level, int32 att_id) const
	{
		if (att_id <= e_unit_attack_att_none || att_id >= e_unit_attack_att_max)
		{
			return 0;
		}
		if (att_level < epai_self_value || att_level >= epai_max)
		{
			return 0;
		}
		return m_unit_att_all[send_gs_to_client][att_level].att[att_id];
	}
	double pawn_att::get_attack_att_gs(uint32 att_index) const
	{
		if (e_unit_attack_att_none < att_index && att_index < e_unit_attack_att_max)
		{
			double self_value = m_unit_att_all[send_gs_to_client][epai_self_value].att[att_index] * (1 + m_unit_att_all[send_gs_to_client][epai_self_rate].att[att_index]);
			double equip_value = (self_value + m_unit_att_all[send_gs_to_client][epai_equip_value].att[att_index]) * (1 + m_unit_att_all[send_gs_to_client][epai_equip_rate].att[att_index]);
			return equip_value;
		}
		return 0;
	}

	double pawn_att::get_attack_att_by_att(uint32 att_index) const
	{
		if (e_unit_attack_att_none < att_index && att_index < e_unit_attack_att_max)
		{
			if (att_index == e_unit_attack_att_hp_max || att_index == e_unit_attack_att_move_speed)
			{
				return m_unit_att_end[att_index];
			}
			else
			{
				double self_value = (m_unit_att_all[send_gs_to_client][epai_self_value].att[att_index] + m_unit_att_all[send_att_to_client][epai_self_value].att[att_index]) * (1 + m_unit_att_all[send_gs_to_client][epai_self_rate].att[att_index] + m_unit_att_all[send_att_to_client][epai_self_rate].att[att_index]);
				double equip_value = (self_value + m_unit_att_all[send_gs_to_client][epai_equip_value].att[att_index] + m_unit_att_all[send_att_to_client][epai_equip_value].att[att_index]) * (1 + m_unit_att_all[send_gs_to_client][epai_equip_rate].att[att_index] + m_unit_att_all[send_att_to_client][epai_equip_rate].att[att_index]);
				return equip_value < 0 ? 0 : equip_value;
			}
		}
		return 0;
	}

	double	pawn_att::get_attack_att_by_gs_and_hide(uint32 att_index) const
	{
		if (e_unit_attack_att_none < att_index && att_index < e_unit_attack_att_max)
		{
			double self_value = (m_unit_att_all[send_gs_to_client][epai_self_value].att[att_index] + m_unit_att_all[send_nothing_to_client][epai_self_value].att[att_index] + m_unit_att_all[send_att_to_client][epai_self_value].att[att_index]) * (1 + m_unit_att_all[send_gs_to_client][epai_self_rate].att[att_index] + m_unit_att_all[send_nothing_to_client][epai_self_rate].att[att_index] + m_unit_att_all[send_att_to_client][epai_self_rate].att[att_index]);
			double equip_value = (self_value + m_unit_att_all[send_gs_to_client][epai_equip_value].att[att_index] + m_unit_att_all[send_nothing_to_client][epai_equip_value].att[att_index] + m_unit_att_all[send_att_to_client][epai_equip_value].att[att_index]) * (1 + m_unit_att_all[send_gs_to_client][epai_equip_rate].att[att_index] + m_unit_att_all[send_nothing_to_client][epai_equip_rate].att[att_index] + m_unit_att_all[send_att_to_client][epai_equip_rate].att[att_index]);
			return equip_value;
		}
		return 0;
	}
	double pawn_att::get_attack_att_value(uint32 att_index)
	{
		if (e_unit_attack_att_none < att_index && att_index < e_unit_attack_att_max)
		{
			return m_unit_att_end[att_index];
		}
		return 0;
	}
	void pawn_att::update_unit_att_info(int32 template_id, bool is_add)
	{
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, template_id);
		if (nullptr == template_ptr)
		{
			return;
		}

		if (template_ptr->AttArray.size() % e_att_one_max != 0 || template_ptr->AttArray.empty())
		{
			return;
		}
		for (int32 base_index = 0; base_index < template_ptr->AttArray.size(); base_index += e_att_one_max)
		{
			float att_level = template_ptr->AttArray[base_index + e_att_one_level];
			float att_index = template_ptr->AttArray[base_index + e_att_one_att_id];
			float base_value = template_ptr->AttArray[base_index + e_att_one_value];
			float base_percent = template_ptr->AttArray[base_index + e_att_one_percent];
			if (m_unit_array_index >= npc_arrary_index_begin)
			{
				npc& npc_ref = unit_man::get_npc(m_unit_array_index);
				std::vector<float> const& init_property_ratio = npc_ref.get_init_poroperty_ratio();
				std::vector<float> const& init_property_plus = npc_ref.get_init_poroperty_plus();

				for (uint32 ratio_index = 0; ratio_index < init_property_ratio.size(); ratio_index += e_att_one_max)
				{
					if (att_level == init_property_ratio[ratio_index + e_att_one_level] && att_index == init_property_ratio[ratio_index + e_att_one_att_id])
					{
						//if (FLOAT_IS_ZERO(init_property_ratio[ratio_index + e_att_one_value]) == false)
						{
							base_value *= init_property_ratio[ratio_index + e_att_one_value];
						}
						//if (FLOAT_IS_ZERO(init_property_ratio[ratio_index + e_att_one_percent]) == false)
						{
							base_percent *= init_property_ratio[ratio_index + e_att_one_percent];
						}
					}
				}
				for (int32 plus_index = 0; plus_index < init_property_plus.size(); plus_index += e_att_one_max)
				{
					if (att_level == init_property_plus[plus_index + e_att_one_level] && att_index == init_property_plus[plus_index + e_att_one_att_id])
					{
						base_value += init_property_plus[plus_index + e_att_one_value];
						base_percent += init_property_plus[plus_index + e_att_one_percent];
					}
				}
			}
			apply_att_change_effect(att_level, att_index, base_value, base_percent, template_ptr->AttArray[base_index + e_att_one_show], is_add);
		}
		update_unit_pure_hurt_att_info(template_id, is_add);
	}

	void pawn_att::update_unit_pure_hurt_att_info(int32 template_id, bool is_add)
	{
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, template_id);
		if (nullptr == template_ptr)
		{
			return;
		}

		if (template_ptr->PureHurtAttArray.size() % e_att_one_max != 0 || template_ptr->PureHurtAttArray.empty())
		{
			return;
		}
		for (int32 base_index = 0; base_index < template_ptr->PureHurtAttArray.size(); base_index += e_att_one_max)
		{
			float att_level = template_ptr->PureHurtAttArray[base_index + e_att_one_level];
			float att_index = template_ptr->PureHurtAttArray[base_index + e_att_one_att_id];
			float base_value = template_ptr->PureHurtAttArray[base_index + e_att_one_value];
			float base_percent = template_ptr->PureHurtAttArray[base_index + e_att_one_percent];
			if (m_unit_array_index >= npc_arrary_index_begin)
			{
				npc& npc_ref = unit_man::get_npc(m_unit_array_index);
				std::vector<float> const& init_property_ratio = npc_ref.get_init_poroperty_ratio();
				std::vector<float> const& init_property_plus = npc_ref.get_init_poroperty_plus();

				for (uint32 ratio_index = 0; ratio_index < init_property_ratio.size(); ratio_index += e_att_one_max)
				{
					if (att_level == init_property_ratio[ratio_index + e_att_one_level] && att_index == init_property_ratio[ratio_index + e_att_one_att_id])
					{
						//if (FLOAT_IS_ZERO(init_property_ratio[ratio_index + e_att_one_value]) == false)
						{
							base_value *= init_property_ratio[ratio_index + e_att_one_value];
						}
						//if (FLOAT_IS_ZERO(init_property_ratio[ratio_index + e_att_one_percent]) == false)
						{
							base_percent *= init_property_ratio[ratio_index + e_att_one_percent];
						}
					}
				}
				for (int32 plus_index = 0; plus_index < init_property_plus.size(); plus_index += e_att_one_max)
				{
					if (att_level == init_property_plus[plus_index + e_att_one_level] && att_index == init_property_plus[plus_index + e_att_one_att_id])
					{
						base_value += init_property_plus[plus_index + e_att_one_value];
						base_percent += init_property_plus[plus_index + e_att_one_percent];
					}
				}
			}
			apply_att_change_effect(att_level, att_index, base_value, base_percent, template_ptr->PureHurtAttArray[base_index + e_att_one_show], is_add);
		}
	}

	bool pawn_att::att_modifier(e_pawn_att_index pawn_att_index, int32 ers_index, f32 value, int32 is_show, bool apply)
	{//因为有百分比乘法,相互关联的属性必须走getfinalatt获取结果进行计算
		if (ers_index <= e_unit_attack_att_none ||
			ers_index >= e_unit_attack_att_max ||
			pawn_att_index < 0 ||
			pawn_att_index >= epai_max )
		{
			return false;
		}

		if (true == FLOAT_IS_ZERO(value))
		{
			return true;
		}

		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		if (unit_ref.get_unit_type() == e_unit_type_monster && unit_ref.get_unit_sub_type() == e_monster_type_player_data)
		{
			if(unit_ref.get_cur_map_template_ptr()->Type != e_map_type_cross_ladder && unit_ref.get_cur_map_template_ptr()->Type != e_map_type_arena)
				return false;
		}
		int32 class_type = unit_ref.get_unit_info(e_role_info_class_type);
		double old_value = m_unit_att_all[send_gs_to_client][pawn_att_index].att[ers_index];
		value = apply ? value : -value;
		double old_final_value = m_unit_att_end[ers_index];
		if (is_show == send_gs_to_client)
		{
			m_unit_att_all[send_gs_to_client][pawn_att_index].att[ers_index] += value;
		}
		else if (is_show == send_nothing_to_client)
		{
			m_unit_att_all[send_nothing_to_client][pawn_att_index].att[ers_index] += value;
		}
		else if (is_show == send_att_to_client)
		{
			m_unit_att_all[send_att_to_client][pawn_att_index].att[ers_index] += value;
		}
		m_unit_att_end[ers_index] = get_attack_att_by_gs_and_hide(ers_index);
		switch (ers_index)
		{
		case e_unit_attack_att_strength:
		{
			strength_to_second_att(class_type, pawn_att_index, m_unit_att_end[ers_index] - old_final_value, is_show);
		}
		break;
		case e_unit_attack_att_agility:
		{
			agility_to_second_att(class_type, pawn_att_index, m_unit_att_end[ers_index] - old_final_value, is_show);
		}
		break;
		case e_unit_attack_att_intellect:
		{
			intellect_to_second_att(class_type, pawn_att_index, m_unit_att_end[ers_index] - old_final_value, is_show);
		}
		break;
		case e_unit_attack_att_stamina:
		{
			stamina_to_second_att(class_type, pawn_att_index, m_unit_att_end[ers_index] - old_final_value, is_show);
		}
		break;
		case e_unit_attack_att_hp_max:
		{
			if (m_unit_att_end[ers_index] < 0)
			{
				m_unit_att_end[ers_index] = 1;
			}
			if (m_unit_tick_begin)
			{
				if (old_final_value > 0)
				{
					int64 new_cur_hp = get_unit_base_att(e_base_att_info_hp_cur) / old_final_value * m_unit_att_end[ers_index];
					if (new_cur_hp <= 0)
					{
						new_cur_hp = 1;
					}
					set_unit_base_att(e_base_att_info_hp_cur, new_cur_hp);
				}
				else
				{
					set_unit_base_att(e_base_att_info_hp_cur, m_unit_att_end[ers_index]);
				}
			}
		}
		break;
		case e_unit_attack_att_move_speed:
		{
			if (m_unit_att_end[ers_index] < 0)
			{
				m_unit_att_end[ers_index] = 1;
			}
			if (old_final_value < m_unit_att_end[ers_index])
			{
				m_max_move_length = m_unit_att_end[ers_index] + 200;//放宽一些移动的判定
				//m_max_move_length += (m_max_move_length >> 1);
			}
		}
		break;
		default:
		{
			if (m_unit_att_end[ers_index] < 0)
			{
				m_unit_att_end[ers_index] = 0;
			}
		}
			break;
		}
		if (fabs(old_final_value - m_unit_att_end[ers_index]) > 0.000001)
		{
			set_att_update_mask(ers_index, e_pawn_att_type_attack);
			m_att_change = true;
		}

		return true;
	}

	void pawn_att::syn_all_att()
	{
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		character_proto_character_all_att char_all_data;
		char_all_data.set_unit_guid(unit_ref.get_unit_guid().server_64);
		for (int32 i = e_unit_attack_att_none; i < e_unit_attack_att_max; ++i)
		{
			char_all_data.add_attack_att(get_attack_att_by_att(i));
		}
		for (int32 i = e_unit_game_att_movement; i < e_unit_game_att_max; ++i)
		{
			char_all_data.add_game_att(get_game_att(i));
		}
		for (int32 i = e_unit_attack_state_dead; i < e_unit_attack_state_max; ++i)
		{
			char_all_data.add_attack_state(get_state_att(i));
		}
		unit_ref.send_message_to_self(&char_all_data, e_msgindex_s2c_character_all_att);
	}
	void pawn_att::send_unit_dead(bool is_dead)
	{
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		unit& killer_ref = unit_man::get_unit(unit_ref.get_killer_array_index());
		character_proto_unit_dead msg;
		msg.set_unit_guid(unit_ref.get_unit_guid().server_64);
		msg.set_killer_guid(killer_ref.get_unit_guid().server_64);
		msg.set_skill_template_id(unit_ref.get_killer_skill_template_id());
		msg.set_is_dead(is_dead);
		msg.set_is_critical(unit_ref.get_be_critical());
		unit_ref.send_message_to_aoi(&msg, e_msgindex_s2c_unit_dead, true);
	}
	void pawn_att::syn_att_change()
	{
		if (!m_unit_tick_begin || false == is_att_mask())
		{
			return;
		}
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		if (m_att_change)
		{
			unit_ref.set_unit_gs_change();
			m_att_change = false;
		}
		bool send_other = false;

		static character_proto_character_one_att		char_one_data_other;
		static character_proto_character_one_att		char_one_data;
		char_one_data_other.Clear();
		char_one_data.Clear();
		char_one_data_other.set_unit_guid(unit_ref.get_unit_guid().server_64);
		for (int32 key = e_unit_attack_att_none + 1; key < e_unit_attack_att_max; ++key)
		{
			if (m_update_att_mask[key])
			{
				char_one_data.add_attack_att_index(key);
				char_one_data.add_attack_att_value(get_attack_att_by_att(key));
				if (key == e_unit_attack_att_hp_max || key == e_unit_attack_att_move_speed || key == e_unit_attack_att_armor
					|| key == e_unit_attack_att_hit_force
					|| key == e_unit_attack_att_dodge_force
					|| key == e_unit_attack_att_be_attack_player)
				{
					send_other = true;
					char_one_data_other.add_attack_att_index(key);
					char_one_data_other.add_attack_att_value(get_attack_att_by_att(key));
				}
			}
		}
		for (int32 key = e_unit_game_att_movement; key < e_unit_game_att_max; ++key)
		{
			int32 game_key = key + e_unit_attack_att_max;
			if (m_update_att_mask[game_key])
			{
				send_other = true;
				char_one_data.add_attack_att_index(key + game_att_sync);
				char_one_data.add_attack_att_value(m_game_att[key]);
				char_one_data_other.add_attack_att_index(key + game_att_sync);
				char_one_data_other.add_attack_att_value(m_game_att[key]);

			}
		}
		for (int32 key = e_unit_attack_state_dead; key < e_unit_attack_state_max; ++key)
		{
			int32 state_key = key + e_unit_attack_att_max + e_unit_game_att_max;
			if (m_update_att_mask[state_key])
			{
				send_other = true;
				char_one_data.add_attack_att_index(key + state_att_sync);
				char_one_data.add_attack_att_value(m_state_att[key] > 0);
				char_one_data_other.add_attack_att_index(key + state_att_sync);
				char_one_data_other.add_attack_att_value(m_state_att[key] > 0);

			}
		}
		for (int32 key = e_unit_attack_state_dead; key < e_unit_attack_state_max; ++key)
		{
			int32 state_key = key + e_unit_attack_att_max + e_unit_game_att_max + e_unit_attack_state_max;
			if (m_update_att_mask[state_key])
			{
				send_other = true;
				char_one_data.add_attack_att_index(key + state_trigger_sync);
				char_one_data.add_attack_att_value(m_state_att[key] > 0);
				char_one_data_other.add_attack_att_index(key + state_trigger_sync);
				char_one_data_other.add_attack_att_value(m_state_att[key] > 0);

			}
		}
		clear_att_update_mask();
		unit_ref.send_message_to_self(&char_one_data, e_msgindex_s2c_character_one_att);
		if (send_other)
		{
			unit_ref.send_message_to_aoi(&char_one_data_other, e_msgindex_s2c_character_one_att, false);
		}
	}

	void pawn_att::syn_gs_change()
	{
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		unit_ref.set_unit_gs_change();
	}

	void pawn_att::set_game_att(int32 att_index, float value, bool apply)
	{
		if (att_index < e_unit_game_att_movement || att_index >= e_unit_game_att_max)
			return;
		bool is_change = false;
		bool is_send = false;
		switch (att_index)
		{
		case faith::e_unit_game_att_movement:
		case faith::e_unit_game_att_war_state:
		case faith::e_unit_game_att_fight_lock:
		case faith::e_unit_game_att_jump:
		case faith::e_unit_game_att_interaction:
		{
			if (att_index == e_unit_game_att_movement)
			{
				is_change = true;
				is_send = true;
			}
			if (m_game_att[att_index] != value)
			{
				is_change = true;
				is_send = true;
				m_game_att[att_index] = value;
			}
		}
		break;
		case faith::e_unit_game_att_exp_only:
		{
			if (apply)
			{
				if (m_game_att[att_index] < value)
				{
					is_change = true;
					m_game_att[att_index] = value;
				}
			}
			else
			{
				is_change = true;
				m_game_att[att_index] = 0;
			}
		}
		break;
		default:
		{
			is_change = true;
			if (apply)
			{
				m_game_att[att_index] += value;
			}
			else
			{
				m_game_att[att_index] -= value;
			}
		}
		}
		if (att_index == e_unit_game_att_attack_num)
		{
			unit& unit_ref = unit_man::get_unit(m_unit_array_index);
			if (unit_ref.is_valid())
			{
				unit_ref.trigger_unit_event(e_game_event_type_unit_attack_num, m_game_att[att_index]);
			}
		}
		if (att_index == e_unit_game_att_movement && m_unit_array_index < npc_arrary_index_begin)
		{
			player& player_ref = unit_man::get_player(m_unit_array_index);
			if (player_ref.is_valid())
				player_ref.change_summon_pet_movement();
		}
		if (is_change)
		{
			set_att_update_mask(att_index, e_pawn_att_type_game);
			if (is_send)
			{
				syn_att_change();
			}
		}
	}
	bool pawn_att::is_dead()
	{
		return m_is_dead > 0;
	}

	float pawn_att::get_game_att(uint32 att_index) const
	{
		if(att_index < e_unit_game_att_movement || att_index >= e_unit_game_att_max)
			return 0;
		return m_game_att[att_index];
	}
	bool pawn_att::get_state_att(uint32 att_index) const
	{
		if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
			return false;
		return m_state_att[att_index] > 0;
	}

	void pawn_att::set_state_att(uint32 att_index, bool flag)
	{
		if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
		{
			return;
		}
		if (att_index == e_unit_attack_state_will_dead)
		{
			m_state_att[att_index] = flag ? m_state_att[att_index] + 1 : m_state_att[att_index] - 1;
			set_att_update_mask(att_index, e_pawn_att_type_state);
			use_adversity_aid_buff_id(att_index);
			syn_att_change();
		}
		else
		{

			if (m_state_att[att_index] != flag)
			{
				if (att_index != e_unit_attack_state_dead)
				{
					if (flag == false && get_state_percent(att_index) > 0)
					{
						return;
					}
					m_state_att[att_index] = flag;
					set_att_update_mask(att_index, e_pawn_att_type_state);
					use_adversity_aid_buff_id(att_index);
					syn_att_change();
				}
				else
				{
					m_state_att[att_index] = flag;
					m_is_dead = flag;
					if (!flag)
					{
						send_unit_dead(flag);
					}
				}
			}
		}
	}

	void pawn_att::set_state_percent(uint32 att_index, int32 flag_percent, bool apply)
	{
		if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
		{
			return;
		}
		if (apply)
		{
			m_state_att_percent[att_index] += flag_percent;
		}
		else
		{
			m_state_att_percent[att_index] -= flag_percent;
		}
	}
	int32 pawn_att::get_state_percent(uint32 att_index) const
	{
		if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
		{
			return 0;
		}
		return m_state_att_percent[att_index];
	}
	void pawn_att::set_state_trigger(uint32 att_index, bool flag)
	{
		if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
		{
			return;
		}
		if (m_state_trigger[att_index] != flag)
		{
			m_state_trigger[att_index] = flag;
			set_att_update_mask(att_index, e_pawn_att_type_sign);
		}
	}
	bool pawn_att::get_state_trigger(uint32 att_index) const
	{
		if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
		{
			return false;
		}
		return m_state_trigger[att_index];
	}
	bool pawn_att::is_can_move()
	{
		if (get_state_att(e_unit_attack_state_dead)
			|| get_state_att(e_unit_attack_state_dizzy)
			|| get_state_att(e_unit_attack_state_frozen)
			|| get_state_att(e_unit_attack_state_attack_back)
			|| get_state_att(e_unit_attack_state_adsorbent))
		{
			return false;
		}
		return true;
	}
	bool pawn_att::can_cut_magic(double magic_value)
	{
		if (magic_value <= get_unit_base_att(e_base_att_info_magic_cur))
		{
			return true;
		}
		return false;
	}
	void pawn_att::change_magic(double magic_value)
	{
		double now_magic = get_unit_base_att(e_base_att_info_magic_cur);
		now_magic -= magic_value;
		if (now_magic <= 0)
		{
			now_magic = 0;
		}
		else if (now_magic > get_attack_att_value(e_unit_attack_att_magic))
		{
			now_magic = get_attack_att_value(e_unit_attack_att_magic);
		}
		set_unit_base_att(e_base_att_info_magic_cur, now_magic);
		send_base_att_one(e_base_att_info_magic_cur);
	}

	bool pawn_att::can_cut_body_power(double body_power)
	{
		if (body_power <= get_unit_base_att(e_base_att_info_body_power_cur))
		{
			return true;
		}
		return false;
	}
	void pawn_att::change_body_power(double body_power)
	{
		double now_body_power = get_unit_base_att(e_base_att_info_body_power_cur);
		now_body_power -= body_power;
		if (now_body_power <= 0)
		{
			now_body_power = 0;
		}
		else if (now_body_power > get_attack_att_value(e_unit_attack_att_body_power))
		{
			now_body_power = get_attack_att_value(e_unit_attack_att_body_power);
		}
		set_unit_base_att(e_base_att_info_body_power_cur, now_body_power);
		send_base_att_one(e_base_att_info_body_power_cur);
	}
	bool pawn_att::can_cur_energy(double energy_value)
	{
		if (energy_value <= get_unit_base_att(e_base_att_info_cur_energy))
		{
			return true;
		}
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		unit_ref.trigger_unit_event(e_game_event_type_unit_energy_low, m_unit_array_index);
		return false;
	}
	void pawn_att::change_energy(double energy_value)
	{
		double old_energy = get_unit_base_att(e_base_att_info_cur_energy);
		double now_energy = old_energy - energy_value;
		if (now_energy <= 0)
		{
			now_energy = 0;
		}
		else if (now_energy > get_attack_att_value(e_unit_attack_att_flight_energy))
		{
			now_energy = get_attack_att_value(e_unit_attack_att_flight_energy);
		}
		if (old_energy != now_energy)
		{
			set_unit_base_att(e_base_att_info_cur_energy, now_energy);
			send_base_att_one(e_base_att_info_cur_energy);
		}
	}
	void pawn_att::adversity_aid_buff_id(int32 buff_id, int32 attack_state, bool apply)
	{
		for (std::list<s_adversity_aid>::iterator it = m_adversity_aid_buff_list.begin(); it != m_adversity_aid_buff_list.end();)
		{
			std::list<s_adversity_aid>::iterator temp_it = it++;
			if (temp_it->m_buff_template_id == buff_id)
			{
				if (apply)
				{
					return;
				}
				else
				{
					m_adversity_aid_buff_list.erase(temp_it);
					return;
				}
			}
		}
		if (apply)
		{
			s_adversity_aid adversity_aid_info(buff_id, e_unit_attack_state(attack_state));
			m_adversity_aid_buff_list.push_back(adversity_aid_info);
		}
	}
	void pawn_att::use_adversity_aid_buff_id(int32 attack_state)
	{
		if (attack_state <= 0 || attack_state >= e_unit_attack_state_max)
		{
			return;
		}
		for (std::list<s_adversity_aid>::iterator it = m_adversity_aid_buff_list.begin(); it != m_adversity_aid_buff_list.end();)
		{
			std::list<s_adversity_aid>::iterator temp_it = it++;
			if (temp_it->m_attack_state == attack_state || temp_it->m_attack_state == 0)
			{
				buff_man::add_buff_inst(m_unit_array_index, m_unit_array_index, temp_it->m_buff_template_id);
			}
		}
	}
	void pawn_att::damage_absorb_change(int32 buff_template_id, int32 skill_template_id, int64 absorb_num, bool apply)
	{
		for (std::list<s_damage_absorb>::iterator it = m_damage_absorb_list.begin(); it != m_damage_absorb_list.end(); )
		{
			std::list<s_damage_absorb>::iterator temp_it = it++;
			if (temp_it->m_buff_template_id = buff_template_id)
			{
				if (apply)
				{
					temp_it->m_skill_template_id = skill_template_id;
					temp_it->m_absorb_max_num = absorb_num;
					temp_it->m_absorb_cur_num = absorb_num;
				}
				else
				{
					unit& unit_ref = unit_man::get_unit(m_unit_array_index);
					send_skill::send_skill_spell_cast(s_unit_identifier(unit_ref.get_unit_guid(), m_unit_array_index),
						temp_it->m_skill_template_id,
						0,
						temp_it->m_absorb_max_num,
						s_unit_identifier(),
						unit_ref.get_new_map_pos(),
						fvector());
					m_damage_absorb_list.erase(temp_it);
				}
				return;
			}
		}
		if (apply)
		{
			s_damage_absorb damage_absorb_info;
			damage_absorb_info.m_buff_template_id = buff_template_id;
			damage_absorb_info.m_skill_template_id = skill_template_id;
			damage_absorb_info.m_absorb_max_num = absorb_num;
			damage_absorb_info.m_absorb_cur_num = absorb_num;
			m_damage_absorb_list.push_back(damage_absorb_info);
		}
	}
	int64 pawn_att::damage_absorb_cut(int64 damage_num)
	{
		for (std::list<s_damage_absorb>::iterator it = m_damage_absorb_list.begin(); it != m_damage_absorb_list.end();)
		{
			std::list<s_damage_absorb>::iterator temp_it = it++;
			if (temp_it->m_absorb_cur_num > damage_num)
			{
				temp_it->m_absorb_cur_num -= damage_num;
				damage_num = 0;
			}
			else if (temp_it->m_absorb_cur_num == damage_num)
			{
				buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, temp_it->m_buff_template_id);
				damage_num = 0;
			}
			else
			{
				buff_man::del_buff_by_template_id(m_unit_array_index, m_unit_array_index, temp_it->m_buff_template_id);
				damage_num -= temp_it->m_absorb_cur_num;
			}
		}
		return damage_num;
	}

	bool pawn_att::exp_add_change(int32 buff_race, int32 buff_level, int32 add_num, bool apply, int32& cut_num)
	{
		std::map<int32, s_exp_add>::iterator it = m_exp_add_map.find(buff_race);
		if (apply)
		{
			if (it == m_exp_add_map.end())
			{
				s_exp_add new_add;
				new_add.m_buff_level = buff_level;
				new_add.m_add_num = add_num;
				m_exp_add_map[buff_race] = new_add;
				cut_num = 0;
				return true;
			}
			else
			{
				if (buff_level > it->second.m_buff_level)
				{
					cut_num = it->second.m_add_num;
					it->second.m_buff_level = buff_level;
					it->second.m_add_num = add_num;
					return true;
				}
				else
				{
					cut_num = 0;
					return false;
				}
			}
		}
		else
		{
			if (it != m_exp_add_map.end())
			{
				cut_num = it->second.m_add_num;
				m_exp_add_map.erase(it);
			}
			else
			{
				cut_num = 0;
			}
			return true;
		}

	}
	void pawn_att::change_base_body_att(const std::vector<int32>& base_body_att,bool is_refresh)
	{
		unit& unit_ref = unit_man::get_unit(m_unit_array_index);
		PlayerUpgradeTemplate* template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, unit_ref.get_unit_info(e_role_info_upgrade_id));
		if (nullptr==template_ptr)
		{
			return;
		}
		int32 use_point = 0;
		int32 max_att = 0;
		int32 SumValue = 0;
		for (int32 i = e_base_att_info_body_strength; i <= e_base_att_info_body_stamina; i++)
		{
			use_point += get_unit_base_att(i);
		}

		max_att = template_ptr->AddPoints;

		if (max_att < 0)
		{
			return;
		}
		for (int32 i = 0; i < base_body_att.size(); i++)
		{
			int32 value = base_body_att[i];
			if (value < 0)
			{
				value = 0;
			}
			if (value > max_att)//防止客户端被破解，导致输入一个int_max，在总数计算时无法准确判断
			{
				return;
			}
			if (!is_refresh)
			{
				if (value < get_unit_base_att(i + e_base_att_info_body_strength))
				{
					return;
				}
			}
			
			SumValue += value;
		}
		if (max_att < SumValue || SumValue < 0)
		{
			return;
		}
		for (int32 i = e_base_att_info_body_strength; i <= e_base_att_info_body_stamina; ++i)
		{
			int32 value = get_unit_base_att(i);
			int32 value1 = base_body_att[i - e_base_att_info_body_strength];
			set_unit_base_att(i, value1);
			change_base_att(i, value, false, is_refresh);
			change_base_att(i, value1, true, is_refresh);
		}
		send_base_att_all();
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (player_ref.is_valid())
		{
			player_ref.get_mission_mgr().target_check(e_mission_end_type_intensify_add_gs, e_power_up_prompt_role_add_point);
			player_ref.get_mission_mgr().target_check(e_mission_end_type_add_dot, 0);
		}
	}

	void pawn_att::change_equip_gs(bool is_add, bool is_refresh)
	{
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return;
		}
		item_set& item = player_ref.get_item_set();
		if (false == is_refresh && false == player_ref.get_is_have_cant_equip_item())
		{
			return;
		}
		for (int32 i = e_role_equip_slot_weapon_1; i <= e_role_equip_slot_amulet; i++)
		{
			citem* item_ptr = item.get_equip_item_by_slot((e_role_equip_slot)i);
			if (nullptr == item_ptr)
			{
				continue;
			}
			guid_64 item_guid = item_ptr->get_item_guid();
			if (is_add == item.is_equipment_effective(item_guid))
			{
				if (!is_add == item_ptr->get_add_att())
				{
					item.change_att_for_equip(item_guid, is_add);
					item_ptr->set_add_att(is_add);
					if (false == is_add)
					{
						player_ref.set_is_have_cant_equip_item(true);
					}
				}
			}
		}
	}

	void pawn_att::change_base_att(int32 att_type, int32 value, bool is_add, bool is_refresh )
	{
		change_equip_gs(is_add, is_refresh);	
		int32 base_type = -1;
		switch (att_type)
		{
		case e_base_att_info_strength:
		case e_base_att_info_body_strength:
			base_type = e_unit_attack_att_strength;
			break;
		case e_base_att_info_agility:
		case e_base_att_info_body_agility:
			base_type = e_unit_attack_att_agility;
			break;
		case e_base_att_info_intellect:
		case e_base_att_info_body_intellect:
			base_type = e_unit_attack_att_intellect;
			break;
		case e_base_att_info_stamina:
		case e_base_att_info_body_stamina:
			base_type = e_unit_attack_att_stamina;
			break;
		default:
			break;
		}
		if (base_type < 0)
		{
			return;
		}
		apply_att_change_effect(4, base_type, value, 0, send_gs_to_client, is_add);
	}
	bool pawn_att::move_check(fvector& move_location, int64& time_now)
	{
		if (m_close_speed)
		{
			return true;
		}
		if (time_now < m_last_move_time)
		{
			return true;
		}
		m_last_move_time = time_now + sync_pos_walk_tick_time;
		player& player_ref = unit_man::get_player(m_unit_array_index);
		if (false == player_ref.is_valid())
		{
			return false;
		}
		if (init_unit::is_in_distance(move_location, player_ref.get_old_map_pos().unit_location, m_max_move_length))
		{
			return true;
		}
		return false;
	}
	void pawn_att::set_close_speed(int32 close_type)
	{
		m_close_speed = close_type;
	}
	int32 pawn_att::get_change_gs_value(const std::vector<float>& add_att, const std::vector<float>& del_att)
	{
		int64 gs_value = 0;
		if (add_att.size() == 0 && del_att.size() == 0)
		{
			return gs_value;
		}

		if (del_att.size() % e_att_one_max != 0 || add_att.size() % e_att_one_max != 0)
		{
			return gs_value;
		}

		int64 now_gs = 0;		
		for (int32 ers_index = e_unit_attack_att_strength; ers_index < e_unit_attack_att_max; ers_index++)
		{
			now_gs += globle_data::get_instance().get_att_gs(ers_index) * get_attack_att_gs(ers_index);
		}

		tag_unit_att old_unit_att_all[epai_max];	//保存旧属性
		for (int32 i = epai_self_value; i < epai_max; ++i)
		{
			old_unit_att_all[i] = m_unit_att_all[send_gs_to_client][i];
		}

		//先扣除属性
		for (int32 i = 0; i < del_att.size(); i += e_att_one_max)
		{
			int32 att_level = del_att[i + e_att_one_level] + 0.5f;
			int32 att_id = del_att[i + e_att_one_att_id] + 0.5f;
			f32 att_value = del_att[i + e_att_one_value] * 1;
			f32 att_percent = del_att[i + e_att_one_percent] * 1;

			if (e_unit_attack_att_none > att_id || e_unit_attack_att_max < att_id)
			{
				continue;
			}

			//判断属性层级是否有效
			if (att_level > tier_att_max || att_level < tier_att_min)
			{
				continue;
			}

			//计算修改属性层级
			e_pawn_att_index  value_level = e_pawn_att_index((att_level - 2) * 2 - 2);
			e_pawn_att_index  rate_level = e_pawn_att_index((att_level - 2) * 2 - 1);
		
			//修改属性
			old_unit_att_all[value_level].att[att_id] -= att_value;
			old_unit_att_all[rate_level].att[att_id] -= att_percent;
		}

		//计算增加属性
		for (int32 i = 0; i < add_att.size(); i += e_att_one_max)
		{
			int32 att_level = add_att[i + e_att_one_level] + 0.5f;
			int32 att_id = add_att[i + e_att_one_att_id] + 0.5f;
			f32 att_value = add_att[i + e_att_one_value] * 1;
			f32 att_percent = add_att[i + e_att_one_percent] * 1;
			int32 att_gs = globle_data::get_instance().get_att_gs(att_id);	//属性价值

			//判断属性id是否有效
			if (e_unit_attack_att_none > att_id || e_unit_attack_att_max < att_id)
			{
				continue;
			}

			//判断属性层级是否有效
			if (att_level > tier_att_max || att_level < tier_att_min)
			{
				continue;
			}

			//计算玩家旧属性
			double old_self_value = old_unit_att_all[epai_self_value].att[att_id] * (1 + old_unit_att_all[epai_self_rate].att[att_id]);
			double old_att_value = (old_self_value + old_unit_att_all[epai_equip_value].att[att_id]) * (1 + old_unit_att_all[epai_equip_rate].att[att_id]);

			//计算修改属性层级
			e_pawn_att_index  value_level = e_pawn_att_index((att_level - 2) * 2 - 2);
			e_pawn_att_index  rate_level = e_pawn_att_index((att_level - 2) * 2 - 1);

			//修改属性
			old_unit_att_all[value_level].att[att_id] += att_value;
			old_unit_att_all[rate_level].att[att_id] += att_percent;
		}

		for (int32 ers_index = e_unit_attack_att_strength; ers_index < e_unit_attack_att_max; ers_index++)
		{
			double self_value = old_unit_att_all[epai_self_value].att[ers_index] * (1 + old_unit_att_all[epai_self_rate].att[ers_index]);
			double equip_value = (self_value + old_unit_att_all[epai_equip_value].att[ers_index]) * (1 + old_unit_att_all[epai_equip_rate].att[ers_index]);
			gs_value += globle_data::get_instance().get_att_gs(ers_index) * equip_value;
		}
		return gs_value - now_gs;
	}


	void pawn_att::show_att(int32 att_type)
	{
		switch (att_type)
		{
		case e_pawn_att_type_attack:
		{
			for (int32 i = e_unit_attack_att_none; i < e_unit_attack_att_max; ++i)
			{
				auto att_value = get_attack_att_value(i);
				CONSOLE_INFO("attack_att att_id:{} att_value:{}", i, att_value);
			}
		}
		break;
		case e_pawn_att_type_game:
		{
			for (int32 i = e_unit_game_att_movement; i < e_unit_game_att_max; ++i)
			{
				auto att_value = get_game_att(i);
				CONSOLE_INFO("game_att att_id:{} att_value:{}", i, att_value);
			}
		}
		break;
		case e_pawn_att_type_state:
		{
			for (int32 i = e_unit_attack_state_dead; i < e_unit_attack_state_max; ++i)
			{
				auto att_value = get_state_att(i);
				CONSOLE_INFO("state_att att_id:{} att_value:{}", i, att_value);
			}
		}
		break;
		case e_pawn_att_type_sign:
		{
			for (int32 i = e_unit_attack_state_dead; i < e_unit_attack_state_max; ++i)
			{
				auto att_value = get_state_trigger(i);
				CONSOLE_INFO("state_trigger att_id:{} att_value:{}", i, att_value);
			}
		}
		break;
		default:
			CONSOLE_ERROR("show_att error att_type:%d", att_type);
			break;
		}
	}
}

