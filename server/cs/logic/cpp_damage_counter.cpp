/*@@

	copyright (c) beijing second laboratory game studio. all rights reserved.

	created_datetime : 	2011-1-5 21:33

	file name :	cpp_damage_counter.cpp

	author : zhangjianyu (zhangjianyu@163.com)

	description :

	change list :


@@*/

//////////////////////////////////////////////////////////////////////////
//
//	file include
//
//////////////////////////////////////////////////////////////////////////

#include "cpp_damage_counter.h"
#include "logic/unit_man.h"
#include "logic/skill_manager.h"
#include "template/template_manager.h"
#include "server_log.hpp"
#include "components/scene/base_map_component.h"
#include "base/ecs_world.h"
#include "system/scene/base_map_system.h"
#include "logic/skill_set.h"
#include "logic/unit.hpp"
#include "event_processer.h"
#include "logic/npc.hpp"
#include "internet/net.pb.h"
#include "internet/buff.pb.h"
#include "utility/init_unit.h"
#include "utility/random.h"

namespace hld
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	class implement
	//
	//////////////////////////////////////////////////////////////////////////
	void cpp_damage_counter::buff_damage_by_server(use_buff_param& param, bool be_server)
	{
		unit& attacker = unit_man::get_unit(param.attacker_array_index);
		unit& target = unit_man::get_unit(param.target_array_index);
		unit* owner_attack = nullptr;
		if (false == attacker.is_valid() || false == target.is_valid())
		{
			return;
		}
		if (be_server == false)
		{
			int32 random_num = attacker.get_random_num(param.hit_random);
			if (random_num > 0)
			{
				random_gen::skill_seed(random_num);
			}
		}
		skill_set& attacker_skill_set_ref = skill_manager::get_skill_set(param.attacker_array_index);
		double base_value = param.damage_base_value;
		buff_proto_buff_damage buff_damage;
		bool is_dead = false;
		if (base_value >= 0)
		{
			if (false == attacker.can_attack_other(target.get_array_index()))
			{
				return;
			}
			if (param.damage_style == edt_skill)
			{
				double skill_damage_ratio = 1;
				int32 skill_plus_value = 0;
				SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, param.skill_info_id);
				if (skill_template_ptr)
				{
					skill_damage_ratio = skill_template_ptr->Ratio + skill_template_ptr->Ratio * attacker_skill_set_ref.get_fix_skill_value(skill_template_ptr->Series, e_fix_skill_damage_ratio);
					skill_plus_value = skill_template_ptr->PlusValue;
				}
				if (GAMECONFIG->HitRateParams.size() != 4 || GAMECONFIG->CriticalRateParams.size() != 3)
				{
					return;
				}
				auto map_entity = attacker.get_map_ent();
				if (nullptr == map_entity)
				{
					CONSOLE_ERROR("map_entity is nullptr");
					return;
				}
				auto base_map_cp = map_entity->get_component<base_map_component>();
				if (false == base_map_cp.isValid())
				{
					CONSOLE_ERROR("base_map_cp is nullptr");
					return;
				}
				const double* attack_att_array = attacker.get_pawn_att().get_attack_att_all();
				const double* target_att_array = target.get_pawn_att().get_attack_att_all();
				if (nullptr == attack_att_array || nullptr == target_att_array)
				{
					return;
				}
				s_hit_critical_calcu_param attacker_param;
				attacker_param.att_hit = attack_att_array[e_unit_attack_att_hit_force];
				attacker_param.att_dodge = attack_att_array[e_unit_attack_att_dodge_force];
				attacker_param.att_hit_prob = attack_att_array[e_unit_attack_att_hit_prob];
				attacker_param.att_un_hit_prob = attack_att_array[e_unit_attack_att_un_hit_prob];
				attacker_param.att_critical = attack_att_array[e_unit_attack_att_critical_rate];
				attacker_param.att_critical_armor = attack_att_array[e_unit_attack_att_critical_armor_rate];
				attacker_param.att_crit_prob = attack_att_array[e_unit_attack_att_crit_prob];
				attacker_param.att_un_crit_prob = attack_att_array[e_unit_attack_att_un_crit_prob];
				attacker_param.unit_type = attacker.get_owner_unit_type();
				attacker_param.branch_type = attacker.get_unit_info(e_role_info_class_branch);
				attacker_param.unit_level = attacker.get_owner_unit_level();
				attacker_param.unit_upgrade_id = attacker.get_unit_info(e_role_info_upgrade_id);
				attacker_param.map_type = base_map_cp->m_map_template->Type;
				attacker_param.map_order = base_map_cp->m_map_template->Order;
				attacker_param.move_type = attacker.get_pawn_att().get_game_att(e_unit_game_att_movement);

				if (attacker.m_array_index >= npc_arrary_index_begin)
				{
					npc& attack_npc_ref = unit_man::get_npc(attacker.m_array_index);
					attacker_param.att_npc_temp_id = attack_npc_ref.get_npc_template_id();
				}
				s_hit_critical_calcu_param target_param;
				target_param.att_hit = target_att_array[e_unit_attack_att_hit_force];
				target_param.att_dodge = target_att_array[e_unit_attack_att_dodge_force];
				target_param.att_hit_prob = target_att_array[e_unit_attack_att_hit_prob];
				target_param.att_un_hit_prob = target_att_array[e_unit_attack_att_un_hit_prob];
				target_param.att_critical = target_att_array[e_unit_attack_att_critical_rate];
				target_param.att_critical_armor = target_att_array[e_unit_attack_att_critical_armor_rate];
				target_param.att_crit_prob = target_att_array[e_unit_attack_att_crit_prob];
				target_param.att_un_crit_prob = target_att_array[e_unit_attack_att_un_crit_prob];
				target_param.unit_type = target.get_owner_unit_type();
				target_param.branch_type = target.get_unit_info(e_role_info_class_branch);
				target_param.unit_level = target.get_owner_unit_level();
				target_param.unit_upgrade_id = target.get_unit_info(e_role_info_upgrade_id);
				target_param.move_type = target.get_pawn_att().get_game_att(e_unit_game_att_movement);

				if (target.m_array_index >= npc_arrary_index_begin)
				{
					npc& target_npc_ref = unit_man::get_npc(target.m_array_index);
					target_param.att_npc_temp_id = target_npc_ref.get_npc_template_id();
				}
				if (attacker.get_array_index() == target.get_array_index())
				{
					param.be_hit = true;
					param.be_critical = false;
				}
				else
				{
					init_unit::hit_and_critical(attacker_param, target_param, GAMECONFIG->HitRateParams, GAMECONFIG->CriticalRateParams, param.be_hit, param.be_critical);
				}
				double damage_sub = target.get_pawn_att().get_game_att(e_unit_game_att_damage_sub);
				double person_attack_change = 1;
				if (attacker_param.unit_type == e_unit_type_player && target_param.unit_type == e_unit_type_player)
				{
					person_attack_change = GAMECONFIG->PersonToPersonAttack;
				}


				s_damage_count_all damage_all = init_unit::skill_damage_end(attacker_param,
					target_param,
					attack_att_array,
					target_att_array,
					param,
					GAMECONFIG->ElementDamageRatio_0,
					GAMECONFIG->ElementDamageRatio_1,
					skill_damage_ratio,
					skill_plus_value,
					damage_sub,
					person_attack_change,
					GAMECONFIG->SkyAttackGround
				);
				double end_damage = damage_all.damage_total;
				buff_damage.set_is_double(param.be_critical);
				damage_reflect(attacker, target, end_damage, param);
				suck_blood(attacker, target, end_damage, param);
				end_damage = target.get_pawn_att().damage_absorb_cut(end_damage);
				is_dead = target.change_hp(end_damage, attacker.get_array_index(), param.be_critical, param.skill_info_id);
				double element_damage = damage_all.damage_elem;
				buff_damage.set_damage_count(end_damage);
				buff_damage.set_elem_damage_count(element_damage);
			}
			else if (param.damage_style == edt_real)
			{
				//ÕæÊµÉËº¦
				base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_pure_attack);
				base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * base_value;
				buff_damage.set_damage_count(base_value);
				damage_reflect(attacker, target, base_value, param);
				suck_blood(attacker, target, base_value, param);
				base_value = target.get_pawn_att().damage_absorb_cut(base_value);
				is_dead = target.change_hp(base_value, attacker.get_array_index(), false);
			}
			else if (param.damage_style == edt_percent)
			{
				double max_hp = target.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
				double add_hp = max_hp * base_value / 100;
				if (attacker.get_unit_guid().server_64 != target.get_unit_guid().server_64)
				{
					add_hp += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * add_hp;
				}
				buff_damage.set_damage_count(add_hp);
				damage_reflect(attacker, target, add_hp, param);
				suck_blood(attacker, target, add_hp, param);
				add_hp = target.get_pawn_att().damage_absorb_cut(add_hp);
				is_dead = target.change_hp(add_hp, attacker.get_array_index(), false);
			}
			else if (param.damage_style == edt_reflect)
			{
				base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * base_value;
				buff_damage.set_damage_count(base_value);
				is_dead = target.change_hp(base_value, attacker.get_array_index(), false);
			}
			else if (param.damage_style == edt_damage_num)
			{
				param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
				is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
				buff_damage.set_damage_count(param.damage_num);
			}
			else if (param.damage_style == edt_hp_percent)
			{
				double cur_hp = attacker.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
				param.damage_num = cur_hp * base_value / 100;
				param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
				is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
				buff_damage.set_damage_count(param.damage_num);
			}
			else if (param.damage_style == edt_hp_percent_target)
			{
				double cur_hp = target.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
				param.damage_num = cur_hp * base_value / 100;
				param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
				is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
				buff_damage.set_damage_count(param.damage_num);
			}
			else if (param.damage_style == edt_hp_percent_precise)
			{
				double cur_hp = attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
				double rate = 100000.0;
				param.damage_num = cur_hp / rate * base_value;
				param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
				is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
				buff_damage.set_damage_count(param.damage_num);
			}
			else if (param.damage_style == edt_hp_percent_precise_wound)
			{
				double cur_hp = attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
				double rate = 100000.0;
				param.damage_num = cur_hp / rate * base_value;
				param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
				param.damage_num -= attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_effect_damage_relief) * param.damage_num;
				is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
				buff_damage.set_damage_count(param.damage_num);
			}


			attacker.get_pawn_att().set_game_att(e_unit_game_att_attack_num, 1, true);
		}
		else
		{
			if (param.damage_style == edt_percent)
			{
				double max_hp = target.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
				double add_hp = max_hp * base_value / 100;
				add_hp += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * add_hp;
				is_dead = target.change_hp(add_hp, attacker.get_array_index(), false);
				buff_damage.set_damage_count(add_hp);
			}
			else
			{
				base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * base_value;
				is_dead = target.change_hp(base_value, attacker.get_array_index(), false);
				buff_damage.set_damage_count(base_value);
			}
		}

		buff_damage.set_damage_type(param.damage_style);
		buff_damage.set_sender(attacker.get_unit_guid().server_64);
		if (attacker.get_unit_guid().server_64 != target.get_unit_guid().server_64)
		{
			buff_damage.set_target(target.get_unit_guid().server_64);
		}
		if (param.skill_info_id > 0)
		{
			buff_damage.set_skill_template_id(param.skill_info_id);
		}
		if (is_dead)
		{
			buff_damage.set_is_dead(1);
		}
		buff_damage.set_end_hp(target.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur));
		if (attacker.get_unit_type() == e_unit_type_summoned || attacker.get_unit_type() == e_unit_type_hide)
		{
			npc* npc_ptr = npc::cast(&attacker);
			if (npc_ptr)
			{
				unit& unit_ref = unit_man::get_unit(npc_ptr->get_owner());
				if (unit_ref.is_valid())
				{
					buff_damage.set_sender(unit_ref.get_unit_guid().server_64);
					unit_ref.send_message_to_self(&buff_damage, e_msgindex_s2c_buff_damage);
				}
			}
		}
		target.send_message_to_self(&buff_damage, e_msgindex_s2c_buff_damage);
		if (attacker.get_unit_type() == e_unit_type_player && param.attacker_array_index != param.target_array_index)
		{
			if (base_value < 0 || be_server)
			{
				if (target.get_unit_type() == e_unit_type_drop_bag || target.get_unit_type() == e_unit_type_award_mark)
				{

				}
				else
				{
					attacker.send_message_to_self(&buff_damage, e_msgindex_s2c_buff_damage);
				}
			}
		}
		if (param.be_critical)
		{
			attacker.trigger_unit_event(e_game_event_type_unit_critical);
			target.trigger_unit_event(e_game_event_type_unit_critical_be);
		}
		if (is_dead)
		{
			attacker.trigger_unit_event(e_game_event_type_unit_kill);
			if (target.get_unit_type() == e_unit_type_player)
			{
				attacker.trigger_unit_event(e_game_event_type_unit_kill_player);
				if (param.attacker_array_index == param.target_array_index)
				{
					CONSOLE_INFO("Kill Self Log Skill Id = {} Damage Style = {}", param.skill_info_id, (int32)param.damage_style);
				}
			}
			else
			{
				attacker.trigger_unit_event(e_game_event_type_unit_kill_npc);
			}
		}
	}

	void cpp_damage_counter::buff_damage_by_client(use_buff_param& param)
	{
		unit& attacker = unit_man::get_unit(param.attacker_array_index);
		unit& target = unit_man::get_unit(param.target_array_index);
		if (false == attacker.is_valid() || false == target.is_valid())
		{
			return;
		}

		skill_set& attacker_skill_set_ref = skill_manager::get_skill_set(param.attacker_array_index);
		double base_value = param.damage_base_value;
		buff_proto_buff_damage buff_damage;
		buff_damage.set_is_double(param.be_critical);
		bool is_dead = false;
		if (param.be_hit == true)
		{
			if (base_value >= 0)
			{
				if (false == attacker.can_attack_other(target.get_array_index()))
				{
					return;
				}
				if (param.damage_style == edt_skill)
				{
					SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, param.skill_info_id);
					if (NULL == skill_template_ptr)
					{
						return;
					}
					double ratio = skill_template_ptr->Ratio + skill_template_ptr->Ratio * attacker_skill_set_ref.get_fix_skill_value(skill_template_ptr->Series, e_fix_skill_damage_ratio);
					double plus_value = skill_template_ptr->PlusValue;
					if (attacker.get_array_index() == target.get_array_index())
					{
						ratio = 0;
						plus_value = 0;
					}
					double damage_sub = target.get_pawn_att().get_game_att(e_unit_game_att_damage_sub);
					s_hit_critical_calcu_param attacker_param;
					attacker_param.unit_type = attacker.get_owner_unit_type();
					attacker_param.branch_type = attacker.get_unit_info(e_role_info_class_branch);
					attacker_param.unit_level = attacker.get_owner_unit_level();
					attacker_param.unit_upgrade_id = attacker.get_unit_info(e_role_info_upgrade_id);
					attacker_param.map_type = base_map_system::get_map_type(attacker.get_map_ent());
					attacker_param.move_type = attacker.get_pawn_att().get_game_att(e_unit_game_att_movement);
					s_hit_critical_calcu_param target_param;
					target_param.unit_type = target.get_owner_unit_type();
					target_param.branch_type = target.get_unit_info(e_role_info_class_branch);
					target_param.unit_level = target.get_owner_unit_level();
					target_param.unit_upgrade_id = target.get_unit_info(e_role_info_upgrade_id);
					target_param.move_type = target.get_pawn_att().get_game_att(e_unit_game_att_movement);
					double person_attack_change = 1;
					if (attacker_param.unit_type == e_unit_type_player && target_param.unit_type == e_unit_type_player)
					{
						person_attack_change = GAMECONFIG->PersonToPersonAttack;
					}
					s_damage_count_all damage_all = init_unit::skill_damage_end(attacker_param,
						target_param,
						attacker.get_pawn_att().get_attack_att_all(),
						target.get_pawn_att().get_attack_att_all(),
						param,
						GAMECONFIG->ElementDamageRatio_0,
						GAMECONFIG->ElementDamageRatio_1,
						ratio,
						plus_value,
						damage_sub,
						person_attack_change,
						GAMECONFIG->SkyAttackGround
					);
					double end_damage = damage_all.damage_total;
					damage_reflect(attacker, target, end_damage, param);
					suck_blood(attacker, target, end_damage, param);
					end_damage = target.get_pawn_att().damage_absorb_cut(end_damage);
					is_dead = target.change_hp(end_damage, attacker.get_array_index(), param.be_critical, param.skill_info_id);
					double element_damage = damage_all.damage_elem;
					buff_damage.set_damage_count(end_damage);
					buff_damage.set_elem_damage_count(element_damage);
				}
				else if (param.damage_style == edt_real)
				{
					//ÕæÊµÉËº¦
					base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_pure_attack);
					base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * base_value;
					damage_reflect(attacker, target, base_value, param);
					suck_blood(attacker, target, base_value, param);
					base_value = target.get_pawn_att().damage_absorb_cut(base_value);
					is_dead = target.change_hp(base_value, attacker.get_array_index(), false);
					buff_damage.set_damage_count(base_value);
				}
				else if (param.damage_style == edt_percent)
				{
					double max_hp = target.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
					double add_hp = max_hp * base_value / 100;
					if (attacker.get_unit_guid().server_64 != target.get_unit_guid().server_64)
					{
						add_hp += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * add_hp;
					}
					damage_reflect(attacker, target, add_hp, param);
					suck_blood(attacker, target, add_hp, param);
					add_hp = target.get_pawn_att().damage_absorb_cut(add_hp);
					is_dead = target.change_hp(add_hp, attacker.get_array_index(), false);
					buff_damage.set_damage_count(add_hp);
				}
				else if (param.damage_style == edt_reflect)
				{
					base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * base_value;
					is_dead = target.change_hp(base_value, attacker.get_array_index(), false);
					buff_damage.set_damage_count(base_value);
				}
				else if (param.damage_style == edt_damage_num)
				{
					param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
					is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
					buff_damage.set_damage_count(param.damage_num);
				}
				else if (param.damage_style == edt_hp_percent)
				{
					double cur_hp = attacker.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
					param.damage_num = cur_hp * base_value / 100;
					param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
					is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
					buff_damage.set_damage_count(param.damage_num);
				}
				else if (param.damage_style == edt_hp_percent_precise)
				{
					double cur_hp = attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
					double rate = 100000.0;
					param.damage_num = cur_hp / rate * base_value;
					param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
					is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
					buff_damage.set_damage_count(param.damage_num);
				}
				else if (param.damage_style == edt_hp_percent_precise_wound)
				{
					double cur_hp = attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
					double rate = 100000.0;
					param.damage_num = cur_hp / rate * base_value;
					param.damage_num += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * param.damage_num;
					param.damage_num -= attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_effect_damage_relief) * param.damage_num;
					is_dead = target.change_hp(param.damage_num, attacker.get_array_index(), false);
					buff_damage.set_damage_count(param.damage_num);
				}
				attacker.get_pawn_att().set_game_att(e_unit_game_att_attack_num, 1, true);
			}
			else
			{
				if (param.damage_style == edt_percent)
				{
					double max_hp = target.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
					double add_hp = max_hp * base_value / 100;
					add_hp += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * add_hp;
					is_dead = target.change_hp(add_hp, attacker.get_array_index(), false);
					buff_damage.set_damage_count(add_hp);
				}
				else
				{
					base_value += attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) * base_value;
					is_dead = target.change_hp(base_value, attacker.get_array_index(), false);
					buff_damage.set_damage_count(base_value);
				}
			}
		}
		buff_damage.set_damage_type(param.damage_style);
		buff_damage.set_sender(attacker.get_unit_guid().server_64);
		if (attacker.get_unit_guid().server_64 != target.get_unit_guid().server_64)
		{
			buff_damage.set_target(target.get_unit_guid().server_64);
		}
		if (param.skill_info_id > 0)
		{
			buff_damage.set_skill_template_id(param.skill_info_id);
		}
		if (is_dead)
		{
			buff_damage.set_is_dead(1);
		}
		buff_damage.set_end_hp(target.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur));
		if (base_value < 0 || attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_attack_multiple) > 0)
		{
			attacker.send_message_to_self(&buff_damage, e_msgindex_s2c_buff_damage);
		}
		target.send_message_to_self(&buff_damage, e_msgindex_s2c_buff_damage);
		if (param.be_critical)
		{
			attacker.trigger_unit_event(e_game_event_type_unit_critical);
			target.trigger_unit_event(e_game_event_type_unit_critical_be);
		}
		if (is_dead)
		{
			attacker.trigger_unit_event(e_game_event_type_unit_kill);
			if (target.get_unit_type() == e_unit_type_player)
			{
				attacker.trigger_unit_event(e_game_event_type_unit_kill_player);
			}
			else
			{
				attacker.trigger_unit_event(e_game_event_type_unit_kill_npc);
			}
		}
	}
	void cpp_damage_counter::damage_reflect(unit& attacker, unit& targeter, double end_damage, use_buff_param& param)
	{
		if (targeter.get_pawn_att().get_game_att(e_unit_game_att_damage_reflect) > 0 && param.damage_style != edt_reflect)
		{ //ÉËº¦·´Éä
			use_buff_param ub_param;
			ub_param.attacker_array_index = targeter.get_array_index();
			ub_param.target_array_index = attacker.get_array_index();
			int64 cur_hp = targeter.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
			end_damage = end_damage > cur_hp ? cur_hp : end_damage;
			ub_param.damage_base_value = end_damage * targeter.get_pawn_att().get_game_att(e_unit_game_att_damage_reflect);
			ub_param.damage_style = edt_reflect;

			if (attacker.get_pawn_att().get_state_att(e_unit_attack_state_no_hurt) == false)
			{
				cpp_damage_counter::buff_damage_by_server(ub_param, true);
			}
		}
	}
	void cpp_damage_counter::suck_blood(unit& attacker, unit& targeter, double end_damage, use_buff_param& param)
	{
		if (attacker.get_pawn_att().get_game_att(e_unit_game_att_suck_blood) > 0 && param.damage_style != edt_reflect)
		{
			double old_hp = attacker.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
			double all_hp = attacker.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
			double change_hp = all_hp - old_hp;
			if (change_hp < 1.0)
			{
				return;
			} //ÎüÑª
			double suck_blood = end_damage * attacker.get_pawn_att().get_game_att(e_unit_game_att_suck_blood);
			attacker.change_hp(-suck_blood, attacker.get_array_index(), false);

			if (change_hp < suck_blood)
			{
				suck_blood = change_hp;
			}
			attacker.send_suck_blood_value(suck_blood);
		}
	}
}
