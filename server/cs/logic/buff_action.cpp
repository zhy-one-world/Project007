/********************************************************************
	created:	2010/03/02
	created:	2:3:2010   17:41
	filename: 	d:\worksite\skill\lowserver\octopus\app\cs\low\new_buff\buff_action.cpp
	file path:	d:\worksite\skill\lowserver\octopus\app\cs\low\new_buff
	file base:	buff_action
	file ext:	cpp
	author:		zhangjianyu
	
	purpose:	
*********************************************************************/

#include "base/ecs_world.h"
#include "buff_action.h"
#include "cpp_damage_counter.h"
#include "logic/buff_item.h"
#include "logic/buff_man.h"
#include "logic/skill_manager.h"
#include "logic/skill_set.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "send_skill.h"
#include "internet/skill.pb.h"
#include "system/scene/base_map_system.h"
#include "template/template_manager.h"
#include "utility/random.h"
#include "internet/net.pb.h"
#include "utility/vector3d.h"
#include "npc.hpp"

namespace faith
{
	/************************************************************************/
	/* tool function                                                        */
	/************************************************************************/
	bool buff_param_error(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (nullptr == buff_action_template_ptr
			|| buff_action_template_ptr->ActionParams.size() < 10 /*定死的长度不管用没用*/)
		{
			return true;
		}

		if (buff_action_template_ptr->ActionParams[1] < random_gen::get_random(0, 100))
		{
			if (env.apply)
			{
				return true;
			}
		}
		if (nullptr == env.pbuff)
		{
			return true;
		}
		if (env.pbuff->get_cur_pile_count() <= 0)
		{
			return true;
		}
		return false;
	}
	void parse_buff_src_and_tar(const buff_env_param& env, int32 target_type, int32& sender_index, int32& target_index)
	{
		if (env.nevent == 0)
		{
			if (target_type == 0)
			{
				sender_index = env.pbuff->get_sender_index();
				target_index = env.pbuff->get_owner_index();
			}
			else if (target_type == 1)
			{
				sender_index = env.pbuff->get_owner_index();
				target_index = env.pbuff->get_sender_index();
			}
			else if (target_type == 2)
			{
				sender_index = env.pbuff->get_sender_index();
				target_index = env.pbuff->get_sender_index();
			}
			else
			{
				sender_index = env.pbuff->get_owner_index();
				target_index = env.pbuff->get_owner_index();
			}

		}
		else
		{
			if (target_type == 0)
			{
				sender_index = env.neventsender;
				target_index = env.neventrecever;
			}
			else if (target_type == 1)
			{
				sender_index = env.neventrecever;
				target_index = env.neventsender;
			}
			else if (target_type == 2)
			{
				sender_index = env.neventsender;
				target_index = env.neventsender;
			}
			else
			{
				sender_index = env.neventrecever;
				target_index = env.neventrecever;
			}
		}
	}
	bool buff_use_map(unit& unit_ref, BuffActionTemplate* buff_action_template_ptr)
	{
		if (unit_ref.is_valid())
		{
			return true;
		}
		return false;
	}
	void buff_event_param(const buff_env_param& env, BuffActionTemplate* buff_action_template_ptr, int32 owner)
	{
		return;
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bool buff_none(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (nullptr == buff_action_template_ptr)
		{
			return false;
		}
		buff_event_param(env, buff_action_template_ptr, 0);
		return true;
	}

	bool buff_mod_att(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().apply_att_change_by_array(buff_action_template_ptr->AttArray, env.apply, env.pbuff->get_cur_pile_count());
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_exp_only(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}
		if (env.pbuff == nullptr)
		{
			return false;
		}
		int32 add_num = buff_action_template_ptr->ActionParams[2];
		int32 add_type = buff_action_template_ptr->ActionParams[3];
		int32 buff_race = env.pbuff->get_race();
		int32 buff_level = env.pbuff->get_level();
		int32 cut_num = 0;
		std::vector<float> AttArray;
		AttArray.push_back(4);
		AttArray.push_back(add_type);
		AttArray.push_back(0);
		AttArray.push_back(0);
		AttArray.push_back(1);
		bool is_change = target_ptr.get_pawn_att().exp_add_change(buff_race, buff_level, add_num, env.apply, cut_num);
		if (env.apply)
		{
			if (is_change)
			{
				AttArray[e_att_one_value] = cut_num / 100.f;
				target_ptr.get_pawn_att().apply_att_change_by_array(AttArray, false, env.pbuff->get_cur_pile_count());
				AttArray[e_att_one_value] = add_num / 100.f;
				target_ptr.get_pawn_att().apply_att_change_by_array(AttArray, true, env.pbuff->get_cur_pile_count());
			}
		}
		else
		{
			AttArray[e_att_one_value] = cut_num / 100.f;
			target_ptr.get_pawn_att().apply_att_change_by_array(AttArray, false, env.pbuff->get_cur_pile_count());
		}
		return true;
	}
	bool buff_mod_back_hp(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		f32 change_percent = buff_action_template_ptr->ActionParams[2] * env.pbuff->get_cur_pile_count() / 100.f;
		f32 change_value = buff_action_template_ptr->ActionParams[3] * env.pbuff->get_cur_pile_count();
		target_ptr.get_pawn_att().set_game_att(e_unit_game_att_back_hp_percent, change_percent, env.apply);
		target_ptr.get_pawn_att().set_game_att(e_unit_game_att_back_hp_value, change_value, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_speed_up(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().apply_att_change_by_array(buff_action_template_ptr->AttArray, env.apply, env.pbuff->get_cur_pile_count());
		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_speed_up, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_speed_down(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (env.apply)
		{
			float un_speed = target_ptr.get_pawn_att().get_attack_att_value(e_unit_attack_att_un_move_speed);
			float buff_time = env.pbuff->get_buff_data_template()->DuringTime;
			if (buff_time > 0)
			{
				buff_time = buff_time - buff_time * un_speed / 100.f;
				int64 end_time = init_unit::get_end_time(buff_time);
				env.pbuff->set_buff_time(end_time);
			}
		}

		target_ptr.get_pawn_att().apply_att_change_by_array(buff_action_template_ptr->AttArray, env.apply, env.pbuff->get_cur_pile_count());
		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_speed_down, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_no_skill(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (env.apply)
		{
			float no_skill = target_ptr.get_pawn_att().get_attack_att_value(e_unit_attack_att_un_no_skill);
			float buff_time = env.pbuff->get_buff_data_template()->DuringTime;
			if (buff_time > 0)
			{
				buff_time = buff_time - buff_time * no_skill / 100.f;
				int64 end_time = init_unit::get_end_time(buff_time);
				env.pbuff->set_buff_time(end_time);
			}
		}
		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_no_skill, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_adversity_aid(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_adversity_aid, env.apply);
		target_ptr.get_pawn_att().adversity_aid_buff_id(buff_action_template_ptr->ActionParams[2], buff_action_template_ptr->ActionParams[3], env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_hp(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (target_ptr.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur) < 0//当前血量过低，估计已经死亡了 
			|| src_ptr.is_dead())
		{
			return false;
		}

		e_skill_hurt_type damage_style = (e_skill_hurt_type)buff_action_template_ptr->ActionParams[4];//伤害类型

		f32 base_damage						= 10.0f;
		if (buff_action_template_ptr->ActionParams[2] <= buff_action_template_ptr->ActionParams[3])
		{
			base_damage = random_gen::get_random(buff_action_template_ptr->ActionParams[2], buff_action_template_ptr->ActionParams[3]);			//从最小值到最大值随即
		}
		else
		{
			base_damage = random_gen::get_random(buff_action_template_ptr->ActionParams[3], buff_action_template_ptr->ActionParams[2]);			//从最小值到最大值随即
		}				
		double damage = base_damage* env.pbuff->get_cur_pile_count();	//伤害*层数

		if (target_ptr.get_pawn_att().get_state_att(e_unit_attack_state_no_hurt) && damage >= 0)
		{
			return true;
		}

		use_buff_param ub_param;
		ub_param.attacker_array_index = src_ptr.get_array_index();
		ub_param.target_array_index = target_ptr.get_array_index();
		ub_param.damage_base_value = damage;
		ub_param.damage_style = damage_style;
		ub_param.damage_num = env.exenv.damage_num;
		ub_param.buff_percent = env.exenv.buff_percent;
		ub_param.be_hit = env.exenv.be_hit;
		ub_param.be_critical = env.exenv.be_critical;
		ub_param.hit_random = env.exenv.hit_random;
		ub_param.server_use_client = true;
		if (env.nevent == using_item)
		{
			ub_param.skill_info_id = env.exenv.skill_template_id;
		}
		else
		{
			ub_param.skill_info_id = env.skillid;
		}
		if (ub_param.skill_info_id <= 0)
		{
			ub_param.skill_info_id = buff_action_template_ptr->attribute_id;
		}
		if (buff_action_template_ptr->ActionParams[5] > 0)
		{//是否使用某个特别状态
			if (buff_action_template_ptr->ActionParams[8] > 0)
			{
				if (target_ptr.get_pawn_att().get_state_att(buff_action_template_ptr->ActionParams[5]))
				{
					buff_man::add_buff_inst(send_index, target_index, buff_action_template_ptr->ActionParams[8]);

					if (env.pbuff->is_zlzz())
					{
						env.pbuff->send_show();
					}
				}
			
				return true;
			}
			else
			{
				if (target_ptr.get_pawn_att().get_state_att(buff_action_template_ptr->ActionParams[5]))
				{
					if (buff_action_template_ptr->ActionParams[7] == 0 || target_ptr.get_pawn_att().get_state_trigger(buff_action_template_ptr->ActionParams[5]))
					{//不用标记就使用 || 标记已经拥有了
						ub_param.add_percent = buff_action_template_ptr->ActionParams[6] / 100.f;
					}
				}
			}
		}
		if (env.exenv.by_client)
		{
			cpp_damage_counter::buff_damage_by_server(ub_param, false);
		}
		else
		{
			cpp_damage_counter::buff_damage_by_server(ub_param, true);
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_mp(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	/************************************************************************/
	/* condition function                                                   */
	/************************************************************************/

	bool buff_condition_no_buff(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 buff_template_id = buff_action_template_ptr->ActionParams[1];
		bool same_sender = buff_action_template_ptr->ActionParams[4] > 0;

		return !buff_man::is_have_buff_by_template_id(src_ptr.get_array_index(), target_ptr.get_array_index(), buff_template_id, same_sender);
	}
	
	bool buff_mod_movement(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		for (int32 i = 0; i < buff_action_template_ptr->ReplaceSkill.size(); i += 2)
		{
			skill_manager::replace_skill_id(target_index, buff_action_template_ptr->ReplaceSkill[i], buff_action_template_ptr->ReplaceSkill[i + 1], env.apply, true);
		}
		if (env.apply)
		{
			int32 new_movement = 0;
			int32 old_movement = target_ptr.get_pawn_att().get_game_att(e_unit_game_att_movement);
			float wing_walk_percent = 0;
			float wing_fly_percent = 0;
			float mount_walk_percent = 0;
			float mount_fly_percent = 0;
			ItemTemplate*	wing_item_template_ptr = GET_TEMPLATE(ItemTemplate, target_ptr.get_unit_info(e_role_info_wing_showd_template_id));
			if (wing_item_template_ptr)
			{
				WingTemplate*	wing_template_ptr = GET_TEMPLATE(WingTemplate, wing_item_template_ptr->logic_id);
				if (wing_template_ptr)
				{
					wing_walk_percent = wing_template_ptr->MaxWalkSpeed;
					wing_fly_percent = wing_template_ptr->MaxFlySpeed;
				}
			}

			ItemTemplate*	mount_item_template_ptr = GET_TEMPLATE(ItemTemplate, target_ptr.get_unit_info(e_role_info_mount_showd_template_id));
			if (mount_item_template_ptr)
			{
				MountTemplate*	mount_template_ptr = GET_TEMPLATE(MountTemplate, mount_item_template_ptr->logic_id);
				if (mount_template_ptr)
				{
					mount_walk_percent = mount_template_ptr->MaxWalkSpeed;
					mount_fly_percent = mount_template_ptr->MaxFlySpeed;
				}
			}
	
			switch (old_movement)
			{
			case e_move_ment_mount_walk:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, mount_walk_percent, send_gs_to_client, false);
			}
			break;
			case e_move_ment_wing_walk:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, wing_walk_percent, send_gs_to_client, false);
			}
			break;
			case e_move_ment_mount_fly:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, mount_fly_percent, send_gs_to_client, false);
			}
			break;
			case e_move_ment_wing_fly:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, wing_fly_percent, send_gs_to_client, false);
			}
			break;
			default:
				break;
			}  

			if (buff_action_template_ptr->ActionParams.size() < 2)
			{
				return false;
			}
			new_movement = buff_action_template_ptr->ActionParams[2];
			switch (new_movement)
			{
			case e_move_ment_mount_walk:
			case e_move_ment_wing_fly:
			case e_move_ment_mount_fly:
				break;
			case e_move_ment_wing_walk:
			case e_move_ment_walk:
			{
				if (target_ptr.get_unit_info(e_role_info_wing_showd_template_id) > 0)
				{
					new_movement = e_move_ment_wing_walk;
				}
				else
				{
					new_movement = e_move_ment_walk;
				}
			} 
				break;
			}
 
			target_ptr.get_pawn_att().set_game_att(e_unit_game_att_movement, new_movement, env.apply);
			target_ptr.get_war_state_mgr().clear_war_state();
			switch (new_movement)
			{
			case e_move_ment_mount_walk:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, mount_walk_percent, send_gs_to_client, env.apply);

			}
			break;
			case e_move_ment_wing_walk:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, wing_walk_percent, send_gs_to_client, env.apply);
			}
			break;
			case e_move_ment_mount_fly:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, mount_fly_percent, send_gs_to_client, env.apply);
			}
			break;
			case e_move_ment_wing_fly:
			{
				target_ptr.get_pawn_att().apply_att_change_effect(4, e_unit_attack_att_move_speed, 0, wing_fly_percent, send_gs_to_client, env.apply);
			}
				break;
			default:
				break;
			}

		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_dizzy(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (target_ptr.get_pawn_att().get_state_att(e_unit_attack_state_no_control) && env.apply)
		{
			return false;
		}

		if (target_ptr.get_unit_type() == e_unit_type_monster)
		{
			if ((init_unit::is_boss(target_ptr.get_unit_sub_type()) && target_ptr.is_control_boss() == 0) || target_ptr.get_unit_sub_type() == e_monster_type_legion_bonfire)
			{
				return false;
			}
		}
		if (env.apply)
		{
			float no_dizzy = target_ptr.get_pawn_att().get_attack_att_value(e_unit_attack_att_un_dizzy);
			float buff_time = env.pbuff->get_buff_data_template()->DuringTime;
			if (buff_time > 0)
			{
				if (buff_action_template_ptr->ActionParams[2] == 0)
				{
					buff_time = buff_time - buff_time * no_dizzy / 100.f;
				}
				int64 end_time = init_unit::get_end_time(buff_time);
				env.pbuff->set_buff_time(end_time);
			}
		}
		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_dizzy, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_state_trigger(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_trigger(buff_action_template_ptr->ActionParams[2], env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_frozen(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (target_ptr.get_pawn_att().get_state_att(e_unit_attack_state_no_control) && env.apply)
		{
			return false;
		}
		if (target_ptr.get_unit_type() == e_unit_type_monster)
		{
			if ((init_unit::is_boss(target_ptr.get_unit_sub_type()) && target_ptr.is_control_boss() == 0) || target_ptr.get_unit_sub_type() == e_monster_type_legion_bonfire)
			{
				return false;
			}
		}
		if (env.apply)
		{
			float no_frozen = target_ptr.get_pawn_att().get_attack_att_value(e_unit_attack_att_un_frozen);
			float buff_time = env.pbuff->get_buff_data_template()->DuringTime;
			if (buff_time > 0)
			{
				buff_time = buff_time - buff_time * no_frozen / 100.f;
				int64 end_time = init_unit::get_end_time(buff_time);
				env.pbuff->set_buff_time(end_time);
			}
		}
		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_frozen, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_stealth(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_stealth, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_adsorbent(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_adsorbent, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_bleeding(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_bleeding, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_burn(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_burn, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_lock_cd(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_lock_cd, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_game_state(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 change_type = buff_action_template_ptr->ActionParams[2];
		f32 change_percent = buff_action_template_ptr->ActionParams[3];
		target_ptr.get_pawn_att().set_state_percent(change_type, change_percent, env.apply);
		target_ptr.get_pawn_att().set_state_att(change_type, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_game_att(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 change_type = buff_action_template_ptr->ActionParams[2];
		double change_value =(double)(buff_action_template_ptr->ActionParams[3]) / 100;
		target_ptr.get_pawn_att().set_game_att(change_type, change_value, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_crazy(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_crazy, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_stealth_show_me(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_stealth_show_me, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_back(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (target_ptr.get_unit_type() == e_unit_type_monster)
		{
			if ((init_unit::is_boss(target_ptr.get_unit_sub_type()) && target_ptr.is_control_boss() == 0) || target_ptr.get_unit_sub_type() == e_monster_type_legion_bonfire)
			{
				return false;
			}
		}
		if (target_ptr.get_unit_type() == e_unit_type_summoned && target_ptr.get_unit_sub_type() == e_summon_pet_type_boss_pet)
		{
			return false;
		}

		if (env.apply)
		{
			/*s_map_pos temp_pos;
			temp_pos.posX = buff_action_template_ptr->ActionParams[2];
			temp_pos.posY = buff_action_template_ptr->ActionParams[3];
			target_ptr.set_pos_move(temp_pos);*/
		}
		if (target_ptr.get_pawn_att().get_state_att(e_unit_attack_state_frozen) && env.apply)
		{
			return false;
		}
		if (target_ptr.get_pawn_att().get_state_att(e_unit_attack_state_no_control) && env.apply)
		{
			return false;
		}
		if (target_ptr.get_unit_type() != e_unit_type_player)
		{
			target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_attack_back, env.apply);
		}

		if (env.apply && env.pbuff)
		{
			BuffDataTemplate const* buff_data_template = env.pbuff->get_buff_data_template();
			if (buff_data_template)
			{
				BuffResourceTemplate const* buff_res_template = GET_TEMPLATE(BuffResourceTemplate, buff_data_template->ResourceID);
				if (buff_res_template)
				{	
					//if (target_ptr.get_unit_type() == e_unit_type_player && buff_res_template->MoveLength > 0)
					//{
					//	return true;
					//}

					if (buff_res_template->MoveLength < 0) //拉回
					{
						s_map_pos src_pos = src_ptr.get_new_map_pos();


						fvector offset;
						offset.x = skill_back_distance * cos(src_pos.unit_rotation.Yaw * RADIAN);
						offset.y = skill_back_distance * sin(src_pos.unit_rotation.Yaw * RADIAN);

					    fvector aim_pos = src_pos.unit_location + offset;

						target_ptr.get_move_mgr().start(aim_pos, buff_res_template->MoveTime);
					}
					else //击退
					{
						s_map_pos target_pos = target_ptr.get_new_map_pos();
						s_map_pos src_pos = src_ptr.get_new_map_pos();
						fvector sub_pos = src_pos.unit_location - target_pos.unit_location;
						target_pos.unit_rotation.yaw = sub_pos.rotator().Yaw;
						target_ptr.set_new_map_pos(target_pos);
						
						target_ptr.get_move_mgr().start(-buff_res_template->MoveLength, buff_res_template->MoveTime, target_pos.unit_rotation.yaw);
					}
				}
			}
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_return(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		skill_set& skill_set_ref = skill_manager::get_skill_set(target_index);
		if (env.apply)
		{
			fvector now_location = target_ptr.get_new_map_pos().unit_location;
			skill_set_ref.set_return_location(now_location);
		}
		else
		{
			fvector old_location = skill_set_ref.get_return_location();
			target_ptr.get_move_mgr().send_unit_end_location(old_location);
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_no_control(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_no_control, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_no_hurt(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_no_hurt, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}	
	bool buff_mod_no_dead(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_no_dead, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_event_trigger(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (env.apply)
		{
			target_ptr.trigger_unit_event(buff_action_template_ptr->ActionParams[2], buff_action_template_ptr->ActionParams[3]);
		}
		return true;
	}
	bool buff_mod_del_buff(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (false == env.apply)
		{
			return true;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}
		int32 buff_template_id = buff_action_template_ptr->ActionParams[2];
		if (buff_template_id <= 0 )
		{
			return true;
		}
		buff_man::del_buff_by_type(send_index, target_index, buff_template_id);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_dispel_debuff(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (false == env.apply)
		{
			return true;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 debuff_state = buff_action_template_ptr->ActionParams[2];
		if (-1 == debuff_state)
		{
			target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_dizzy, false); //1
			target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_frozen, false); //2
			target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_no_skill, false); //8
			target_ptr.get_pawn_att().set_state_att(e_unit_attack_state_speed_down, false); //10
		}
		else
		{
			target_ptr.get_pawn_att().set_state_att(debuff_state, false);
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_add_time(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 buff_template_id = buff_action_template_ptr->ActionParams[2];
		buff_item* buff_item_ptr = buff_man::get_buff_item_by_template_id(src_ptr.get_array_index(), target_ptr.get_array_index(), buff_template_id);
		if (NULL == buff_item_ptr)
		{
			return false;
		}
		float buff_add_time = buff_action_template_ptr->ActionParams[3] / 1000.f;
		if (env.apply)
		{
			buff_item_ptr->add_life_time(buff_add_time);
		}
		else
		{
			buff_item_ptr->add_life_time(-buff_add_time);
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_use_skill(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 skill_id = buff_action_template_ptr->ActionParams[2];		
		send_skill::send_skill_spell_cast(s_unit_identifier(target_ptr.get_unit_guid(), target_index),
			skill_id,
			0,
			0,
			s_unit_identifier(),
			target_ptr.get_new_map_pos(),
			fvector());
		player& playe_ptr = unit_man::get_player(target_index);
		if (playe_ptr.is_valid())
		{
			int32 passive_skill_id = playe_ptr.get_passive_skill().get_passive_skill_id_by_buff_action_id(buff_action_template_ptr->attribute_id);
			if (passive_skill_id > 0)
			{
				skill_proto_trigger_passive_skill item_msg;
				item_msg.set_skill_template_id(passive_skill_id);
				playe_ptr.send_message_to_self(&item_msg, e_msgindex_s2c_trigger_passive_skill);
			}		
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_call_npc(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}
		if (buff_action_template_ptr->ActionParams.size() < 20)
		{
			return false;
		}
		int32 npc_template_id = buff_action_template_ptr->ActionParams[2];
		int32 npc_num = 0;
		for (int32 i = 2; i < 10; ++i)
		{
			if (buff_action_template_ptr->ActionParams[i] > 0)
			{
				npc_num++;
			}
		}
		if (npc_num > 1)
		{
			int32 rand_num = random_gen::get_random(0, npc_num - 1);
			npc_template_id = buff_action_template_ptr->ActionParams[2 + rand_num];
		}
		if (env.exenv.skill_target_id == -1 || env.exenv.skill_target_id == 0 || env.exenv.skill_target_id  == target_index)
		{
			if (env.exenv.skill_template_id == sepc_skill_snow_id || env.exenv.skill_template_id == sepc_skill_fire_id)
			{
			    skill* skl_ptr = skill_manager::get_skill_set(target_ptr.get_identifier().runtime_id).get_skill_inst_by_id(env.exenv.skill_template_id);
				if (skl_ptr)
				{
					fvector aim_pos = skl_ptr->get_enemy_pos(buff_action_template_ptr->ActionParams[10]);
					if (aim_pos.is_valid())
					{
						world_cs::spawn_npc(npc_template_id, aim_pos, src_ptr.get_identifier(), target_ptr.get_map_ent());
						buff_event_param(env, buff_action_template_ptr, target_index);
						return true;
					}
				}
			}
			int32 show_wing_id = 0;
			if (env.exenv.skill_template_id == sepc_skill_muti_id || env.exenv.skill_template_id == sepc_skill_mirror_id)
			{
				player& playe_ref = unit_man::get_player(target_index);
				if (playe_ref.is_valid())
				{
					show_wing_id = playe_ref.get_unit_info(e_role_info_wing_showd_template_id);
				}
			}
			const fvector& effect_orig_location = target_ptr.get_new_map_pos().unit_location;
			const frotator& effect_orig_rotation = target_ptr.get_new_map_pos().unit_rotation;
			fvector offset;
			offset.x = buff_action_template_ptr->ActionParams[10] * cos(effect_orig_rotation.yaw * RADIAN);
			offset.y = buff_action_template_ptr->ActionParams[11] * sin(effect_orig_rotation.yaw * RADIAN);
			offset.z = buff_action_template_ptr->ActionParams[12];
			offset += effect_orig_location;
			int32 rand_radius = std::max(buff_action_template_ptr->ActionParams[13], 1);
			int32 count = std::min(buff_action_template_ptr->ActionParams[14], 16);

			fvector effect_location;
			for (int32 i = 0; i < count; i++)
			{
				int32 effect_radius = rand() % rand_radius;
				int32 effect_angle = rand() % 360;

				fvector effect_offset(rand_radius* cos(effect_angle*faith::math::DEGTORAD),
					rand_radius* sin(effect_angle*faith::math::DEGTORAD));

				effect_location = offset + effect_offset;
				s_map_pos effect_born_pos(effect_location, target_ptr.get_new_map_pos().unit_rotation);
			    world_cs::spawn_npc(npc_template_id, effect_born_pos, src_ptr.get_identifier(), target_ptr.get_map_ent(),  nullptr, show_wing_id);
			}
		}
		else
		{
			unit& create_unit_ptr = unit_man::get_unit(env.exenv.skill_target_id);
			world_cs::spawn_npc(npc_template_id, create_unit_ptr.get_new_map_pos(), src_ptr.get_identifier(), create_unit_ptr.get_map_ent());

		}
			
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_use_buff(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 change_type = buff_action_template_ptr->ActionParams[2];
		int32 buff_id = buff_action_template_ptr->ActionParams[3];
		if (change_type == 0)
		{
			if (env.apply)
			{
				buff_ex_env_param penv;
				penv.skill_order = env.exenv.skill_order;
				penv.skill_template_id = env.exenv.skill_template_id;
				penv.pos = env.exenv.pos;
				penv.dir = env.exenv.dir;
				penv.buff_level = 1;
				buff_man::add_buff_inst(src_ptr.get_array_index(), target_ptr.get_array_index(), buff_id, &penv);
			}
			else
			{
				buff_man::del_buff_by_template_id(target_ptr.get_array_index(), target_ptr.get_array_index(), buff_id);
			}
		}
		else
		{
			buff_man::del_buff_by_template_id(target_ptr.get_array_index(), target_ptr.get_array_index(), buff_id);
		}
		//player* playe_ptr = &unit_man::get_player(target_index);
		//if (playe_ptr != nullptr)
		//{
		//	int32 passive_skill_id = playe_ptr->get_passive_skill().get_passive_skill_id_by_buff_action_id(buff_action_template_ptr->attribute_id);
		//	if (passive_skill_id > 0)
		//	{
		//		skill_proto_trigger_passive_skill item_msg;
		//		item_msg.set_skill_template_id(passive_skill_id);
		//		playe_ptr->send_message_to_self(&item_msg, e_msgindex_s2c_trigger_passive_skill);
		//	}
		//}	
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_again_buff(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (env.apply == false)
		{
			return true;
		}
		int32 len = buff_action_template_ptr->ActionParams.size();
		for (int32 i =2; i < len; ++i)
		{
			int32 buff_series = buff_action_template_ptr->ActionParams[i];
			if (buff_series <= 0)
			{
				return true;
			}
			buff_item* buff_item_ptr = buff_man::get_buff_item_by_series_id(target_index, target_index, buff_series);
			if (nullptr == buff_item_ptr)
			{
				continue;
			}
			buff_item_ptr->begin_effect(nullptr);
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_mount_change(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (env.apply == false)
		{
			return true;
		}
		//citem* mount_ptr = target_ptr.get_item_set().get_item(env.exenv.item_guid);
		//if (mount_ptr == nullptr)
		//{
		//	return false;
		//}
		//target_ptr.get_item_set().show_this_mount(mount_ptr, mount_ptr->get_data_info(e_item_info_showing_illusion_idex), false);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_pk_mode(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index); 

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 pk_mode = buff_action_template_ptr->ActionParams[2];
		if (env.apply == false)
		{
			pk_mode = e_pk_mode_peace;
		}
		target_ptr.get_pk_community_mgr().set_pk_mode(pk_mode);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_money(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		player& target_ptr = unit_man::get_player(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (env.apply == false || target_ptr.is_valid() == false)
		{
			return true;
		}
		int32 money_type = buff_action_template_ptr->ActionParams[2];
		int64 money_ratio = buff_action_template_ptr->ActionParams[3];

		int64 money_num = 0;

		int64 base_exp = 0;

		int32 upgrade_template_id = target_ptr.get_unit_info(e_role_info_upgrade_id);
		int32 cur_player_level = target_ptr.get_unit_info(e_role_info_exp_level);
		int32 rule_level = GAMECONFIG->ExpServerEndLevel;
		PlayerUpgradeTemplate* player_upgrade_template_ptr = GET_TEMPLATE(PlayerUpgradeTemplate, upgrade_template_id);
		if (player_upgrade_template_ptr)
		{
			base_exp = player_upgrade_template_ptr->BaseExp;
			/*	money_num = player_upgrade_template_ptr->BaseExp * money_ratio / 100.f;
				money_num = money_num + (money_num * env.exenv.buff_percent);*/
		}

		if (buff_action_template_ptr->ActionParams[9] && cur_player_level > rule_level)
		{
			base_exp = target_ptr.get_cur_player_world_level_exp();
		}

		money_num = base_exp * money_ratio / 100.f;
		money_num = money_num + (money_num * env.exenv.buff_percent);

		target_ptr.add_money_or_exp(e_money_type(money_type), money_num, e_server_log_add_money_buff, env.pbuff->get_buff_template_id());
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_damage_reflect(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		f32 change_value = buff_action_template_ptr->ActionParams[2] / 100.f * env.pbuff->get_cur_pile_count();
		target_ptr.get_pawn_att().set_game_att(e_unit_game_att_damage_reflect, change_value, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_absorb_hurt(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 change_type = buff_action_template_ptr->ActionParams[2];
		double change_value = buff_action_template_ptr->ActionParams[3] * env.pbuff->get_cur_pile_count();
		int32 skill_template_id = buff_action_template_ptr->ActionParams[4];
		double damage_percent =(double)(buff_action_template_ptr->ActionParams[5]) * (double)(target_ptr.get_unit_info(e_role_info_exp_level)) / 100 ;
		if (change_type == 0)
		{
			target_ptr.get_pawn_att().damage_absorb_change(env.pbuff->get_buff_template_id(), skill_template_id, change_value + damage_percent, env.apply);
		}
		else
		{
			change_value /= 100.f;
			double max_hp = target_ptr.get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
			change_value = change_value * max_hp;
			target_ptr.get_pawn_att().damage_absorb_change(env.pbuff->get_buff_template_id(), skill_template_id, change_value + damage_percent, env.apply);
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_damage_sub(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		float change_value = buff_action_template_ptr->ActionParams[2] / 100.f;
		target_ptr.get_pawn_att().set_game_att(e_unit_game_att_damage_sub, change_value, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_suck_blood(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		f32 change_value = buff_action_template_ptr->ActionParams[2] / 100.f * env.pbuff->get_cur_pile_count();
		target_ptr.get_pawn_att().set_game_att(e_unit_game_att_suck_blood, change_value, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_skill_add_buff(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 series_type = buff_action_template_ptr->ActionParams[2];
		int32 buff_template_id = buff_action_template_ptr->ActionParams[3];
		int32 is_self = buff_action_template_ptr->ActionParams[4];
		skill_set& skill_set_ref = skill_manager::get_skill_set(target_ptr.get_array_index());
		skill_set_ref.skill_add_buff(series_type, buff_template_id, is_self, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_skill_cd(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 series_type = buff_action_template_ptr->ActionParams[2];
		int32 sub_cd = buff_action_template_ptr->ActionParams[3] * env.pbuff->get_cur_pile_count();;
		skill_set& skill_set_ref = skill_manager::get_skill_set(target_ptr.get_array_index());
		skill_set_ref.change_skill_fix_value(series_type, e_fix_skill_sub_cd_value, sub_cd, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_skill_replace(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		switch (buff_action_template_ptr->ActionParams[2])
		{
		case e_game_condition_type_movement:
		{
			int32 movement = target_ptr.get_pawn_att().get_game_att(e_unit_game_att_movement);
			if (movement == buff_action_template_ptr->ActionParams[3])
			{
				for (int32 i = 0; i < buff_action_template_ptr->ReplaceSkill.size(); i += 2)
				{
					if (buff_action_template_ptr->ReplaceSkill[i] > 0)
					{
						skill_manager::replace_skill_id(target_index, buff_action_template_ptr->ReplaceSkill[i], buff_action_template_ptr->ReplaceSkill[i + 1], env.apply);
					}
				}
			}
		}
			break;
		default:
			for (int32 i = 0; i < buff_action_template_ptr->ReplaceSkill.size(); i += 2)
			{
				if (buff_action_template_ptr->ReplaceSkill[i] > 0)
				{
					skill_manager::replace_skill_id(target_index, buff_action_template_ptr->ReplaceSkill[i], buff_action_template_ptr->ReplaceSkill[i + 1], env.apply);
				}
			}
			break;
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_skill_all_cd(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 change_type = buff_action_template_ptr->ActionParams[2];
		int32 sub_cd = buff_action_template_ptr->ActionParams[3] * env.pbuff->get_cur_pile_count();
		skill_set& skill_set_ref = skill_manager::get_skill_set(target_ptr.get_array_index());
		if (change_type == 0)
		{
			skill_set_ref.change_skill_sub_cd_all(sub_cd, env.apply);
		}
		else
		{
			skill_set_ref.change_skill_percent_cd_all(sub_cd / 100.f, env.apply);
		}
		target_ptr.get_pawn_att().syn_gs_change();
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	bool buff_mod_skill_learn(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 skill_template_id = buff_action_template_ptr->ActionParams[2];
		if (skill_template_id > 0)
		{
			if (env.apply)
			{
				skill_manager::resp_master_skill_by_id(target_ptr.get_array_index(), skill_template_id);
			}
			else
			{
				skill_manager::remove_unit_skill_by_id(target_ptr.get_array_index(), skill_template_id);
			}
		}
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}
	
	bool buff_mod_skill_damage_ratio(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		//unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		int32 series_type = buff_action_template_ptr->ActionParams[2];
		float fix_val = (buff_action_template_ptr->ActionParams[3] * env.pbuff->get_cur_pile_count()) / 100.f;
		skill_set& skill_set_ref = skill_manager::get_skill_set(target_ptr.get_array_index());
		skill_set_ref.change_skill_fix_value(series_type, e_fix_skill_damage_ratio, fix_val, env.apply);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_mod_relife(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		//unit& src_ptr = unit_man::get_unit(send_index);
		player& target_ptr = unit_man::get_player(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		{//跨服天梯地图禁止非服务器复活

			if (base_map_system::get_map_type(target_ptr.get_map_ent()) == e_map_type_cross_ladder)
			{
				return false;
			}
		}

		if (target_ptr.is_valid())
		{
			target_ptr.set_player_relive();
		}
		return true;
	}

	bool buff_mod_kill_once(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}
		if (target_ptr.get_pawn_att().get_state_att(e_unit_attack_state_no_hurt))
		{
			return false;
		}
		if (target_ptr.get_unit_type() == e_unit_type_monster
			&& (target_ptr.get_unit_sub_type() == e_monster_type_elite
				|| init_unit::is_boss(target_ptr.get_unit_sub_type()) || target_ptr.get_unit_sub_type() == e_monster_type_legion_bonfire))
		{
			return true;
		}
		if (target_ptr.get_unit_type() == e_unit_type_summoned && target_ptr.get_unit_sub_type() == e_summon_pet_type_boss_pet)
		{
			return true;
		}
		if (src_ptr.get_pk_community_mgr().is_skill_target(target_ptr.get_array_index(), eskilltarget_enemy) == false)
		{
			return true;
		}
		double hp = target_ptr.get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
		buff_proto_buff_damage buff_damage;
		buff_damage.set_damage_count(hp);
		buff_damage.set_damage_type(edt_kill_once);
		buff_damage.set_sender(src_ptr.get_unit_guid().server_64);
		if (src_ptr.get_unit_guid().server_64 != target_ptr.get_unit_guid().server_64)
		{
			buff_damage.set_target(target_ptr.get_unit_guid().server_64);
		}
		buff_damage.set_is_dead(1);
		buff_damage.set_end_hp(0);
		if (src_ptr.get_pawn_att().get_use_owner_att() && src_ptr.get_unit_type() != e_unit_type_player)
		{
			npc* npc_ptr = npc::cast(&src_ptr);
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
		target_ptr.send_message_to_self(&buff_damage, e_msgindex_s2c_buff_damage);
		src_ptr.send_message_to_self(&buff_damage, e_msgindex_s2c_buff_damage);
		target_ptr.change_hp(hp, send_index, true);
		return true;
	}

	bool buff_mod_energy(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		if (target_ptr.get_pawn_att().can_cur_energy(buff_action_template_ptr->ActionParams[2]) == false || src_ptr.is_dead())
		{
			return false;
		}
		target_ptr.get_pawn_att().change_energy(buff_action_template_ptr->ActionParams[2]);
		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_spirit_att(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}

		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& src_ptr = unit_man::get_unit(send_index);
		player& target_ptr = unit_man::get_player(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}
		
		int32 jiban_id = buff_action_template_ptr->ActionParams[2];
		float percent_num = (float)buff_action_template_ptr->ActionParams[3] / (float)1000;
		if (true == env.apply)
		{
			bool is_change = false;
			//std::vector<int32> jiban_trigger_list = target_ptr.get_spirit_mgr().get_jiban_trigger_list();
			//for (int32 i = 0; i < jiban_trigger_list.size(); ++i)
			//{
			//	if (jiban_trigger_list[i] == jiban_id)
			//	{
			//		SpiritJiBanTemplate* TempTemplatePtr = GET_TEMPLATE(SpiritJiBanTemplate, jiban_id);
			//		if (TempTemplatePtr != nullptr)
			//		{
			//			is_change = true;
			//			target_ptr.get_pawn_att().apply_att_change_by_array(TempTemplatePtr->Property, true, percent_num);
			//		}
			//	}
			//}
			if (false == is_change)
			{
				return false;
			}
		}
		else
		{
			SpiritJiBanTemplate* TempTemplatePtr = GET_TEMPLATE(SpiritJiBanTemplate, jiban_id);
			if (TempTemplatePtr != nullptr)
			{
				target_ptr.get_pawn_att().apply_att_change_by_array(TempTemplatePtr->Property, false, percent_num);
			}
		}

		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	bool buff_change_bush(buff_env_param& env, BuffActionTemplate* buff_action_template_ptr)
	{
		if (buff_param_error(env, buff_action_template_ptr))
		{
			return false;
		}
		if (env.apply == false)
		{
			return false;
		}
		int32 send_index = 0;
		int32 target_index = 0;

		parse_buff_src_and_tar(env, buff_action_template_ptr->ActionParams[0], send_index, target_index);

		unit& target_ptr = unit_man::get_unit(target_index);

		if (buff_use_map(target_ptr, buff_action_template_ptr) == false)
		{
			return false;
		}

		ChangeShapeTemplate * tem_ptr = GET_TEMPLATE(ChangeShapeTemplate, buff_action_template_ptr->ActionParams[3]);
		if (tem_ptr == nullptr)
		{
			return false;
		}
		

		if (buff_action_template_ptr->ActionParams[2] == 1)
		{
			for (int32 i = 0; i < tem_ptr->BeginBuffList.size(); ++i)
			{
				buff_man::add_buff_inst(target_index, target_index, tem_ptr->BeginBuffList[i]);
			}
			target_ptr.get_pawn_att().set_change_bush_id(buff_action_template_ptr->ActionParams[3]);
			if (target_ptr.get_cur_map_type() == e_map_type_attack_city)
			{
				if (target_ptr.get_pawn_att().get_unit_base_att(e_base_att_info_community_type) == e_community_type_attack_player)
				{
					target_ptr.get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_attack_car);
					target_ptr.get_pawn_att().send_base_att_one(e_base_att_info_community_type);
				}
				else if (target_ptr.get_pawn_att().get_unit_base_att(e_base_att_info_community_type) == e_community_type_guard_player)
				{
					target_ptr.get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_guard_car);
					target_ptr.get_pawn_att().send_base_att_one(e_base_att_info_community_type);
				}
			}
		}
		else
		{
			for (int32 i = 0; i < tem_ptr->BeginBuffList.size(); ++i)
			{
				buff_man::del_buff_by_template_id(target_index, target_index, tem_ptr->BeginBuffList[i]);
			}
			for (int32 i = 0; i < tem_ptr->EndBuffList.size(); ++i)
			{
				buff_man::add_buff_inst(target_index, target_index, tem_ptr->EndBuffList[i]);
			}
			target_ptr.get_pawn_att().set_change_bush_id(buff_action_template_ptr->ActionParams[0]);

			if (target_ptr.get_cur_map_type() == e_map_type_attack_city)
			{
				if (target_ptr.get_pawn_att().get_unit_base_att(e_base_att_info_community_type) == e_community_type_attack_car)
				{
					target_ptr.get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_attack_player);
					target_ptr.get_pawn_att().send_base_att_one(e_base_att_info_community_type);
				}
				else if (target_ptr.get_pawn_att().get_unit_base_att(e_base_att_info_community_type) == e_community_type_guard_car)
				{
					target_ptr.get_pawn_att().set_unit_base_att(e_base_att_info_community_type, e_community_type_guard_player);
					target_ptr.get_pawn_att().send_base_att_one(e_base_att_info_community_type);
				}
			}
		}

		buff_event_param(env, buff_action_template_ptr, target_index);
		return true;
	}

	
	buff_action_func	g_buffaction[] =
	{
		{"none",				buff_none},	
		/*
		 *	condition
		 */
		{"nobuff",				buff_condition_no_buff},

		/*
		 *	action
		 */
		//改变属性状态
		{ "modhp",				buff_mod_hp},
		{ "modmp",				buff_mod_mp},
		{ "modatt",				buff_mod_att},
		{ "moddizzy",			buff_mod_dizzy },
		{ "modmovement",		buff_mod_movement },
		{ "modback",			buff_mod_back },
		{ "modreturn",			buff_mod_return },
		{ "modaddtime",			buff_mod_add_time },
		{ "moduseskill",		buff_mod_use_skill },
		{ "modusebuff",			buff_mod_use_buff },
		{ "modagainbuff",		buff_mod_again_buff },
		{ "modnocontrol",		buff_mod_no_control },
		{ "modnohurt",			buff_mod_no_hurt },
		{ "modnoskill",			buff_mod_no_skill },
		{ "modeventtrigger",	buff_mod_event_trigger },
		{ "modskilladdbuff",	buff_mod_skill_add_buff},
		{ "modadversityaid",	buff_mod_adversity_aid },
		{ "modstealth",			buff_mod_stealth },
		{ "modstealthshowme",	buff_mod_stealth_show_me },
		{ "modbleeding",		buff_mod_bleeding },
		{ "modburn",			buff_mod_burn },
		{ "modlockcd",			buff_mod_lock_cd },
		{ "modgamestate",		buff_mod_game_state },
		{ "modgameatt",			buff_mod_game_att },
		{ "modcrazy",			buff_mod_crazy },
		{ "modadsorbent",		buff_mod_adsorbent },
		{ "modspeeddown",		buff_mod_speed_down },
		{ "modspeedup",			buff_mod_speed_up },
		{ "modstatetrigger",	buff_mod_state_trigger },
		{ "modfrozen",			buff_mod_frozen},
		{ "modcallnpc",			buff_mod_call_npc },
		{ "modbackhp",			buff_mod_back_hp },
		{ "modexponly",			buff_mod_exp_only },
		{ "moddamagereflect",	buff_mod_damage_reflect },
		{ "modmountchange",		buff_mod_mount_change },
		{ "modnodead",			buff_mod_no_dead },
		{ "modrelife",			buff_mod_relife },
		{ "modkillonce",		buff_mod_kill_once },
		{ "moddispeldebuff",	buff_mod_dispel_debuff },
		{ "moddelbuff",			buff_mod_del_buff },
		{ "modpk",				buff_mod_pk_mode },
		{ "modmoney",			buff_mod_money },
		{ "modabsorbhurt",		buff_mod_absorb_hurt },
		{ "modsuckblood",		buff_mod_suck_blood },
		{ "modskillcd",			buff_mod_skill_cd },
		{ "moddamagesub",		buff_mod_damage_sub },
		{ "modskillallcd",		buff_mod_skill_all_cd },
		{ "modskillreplace",	buff_mod_skill_replace },
		{ "modskilllearn",		buff_mod_skill_learn },
		{ "modskilldamageratio",buff_mod_skill_damage_ratio },
		{ "modenergy",			buff_mod_energy },
		{ "modspiritatt",		buff_spirit_att },
		{ "modchangebush",		buff_change_bush },
	};
	int32 g_buffaction_num = sizeof(g_buffaction) / sizeof(buff_action_func);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void buff_str::clear_char( 
		char* szaction, 
		char cone, 
		char ctwo )
	{
		int32 npos = 0;
		int32 nscanpos = 0;

		while( szaction[nscanpos] )
		{
			szaction[npos] = szaction[nscanpos];

			if( szaction[npos] != cone &&
				szaction[npos] != ctwo )
				npos++;

			nscanpos++;
		}

		szaction[npos] = 0;
	}

	int32	buff_str::find_delimter( 
		char* szaction, 
		char cone, 
		char ctwo )
	{
		int32 npos = 0;

		while( szaction[npos] )
		{
			if( szaction[npos] == cone ||
				szaction[npos] == ctwo )
				return npos;

			npos++;
		}

		return 0;
	}

	void buff_str::conv_lower_case( 
		char* szaction )
	{
		int32 nloopcount = 0;

		while( szaction[nloopcount] )
		{
			szaction[nloopcount] = tolower( szaction[nloopcount] );
			nloopcount++;
		}
	}

	int32	buff_str::check_act( 
		char* szaction )
	{
		int32 nret	= true;
		int32 npos	= 0;
		int32 nclose	= 0;

		while( szaction[npos] )
		{
			if( szaction[npos] < 0x21 ||
				szaction[npos] > 0x7e )
			{
				nret = 0;
				break;
			}

			if( szaction[npos] == FUNL_DELIMITERC )
			{
				nclose++;
				if( szaction[npos+1] == FUNP_DELIMITERC )
				{
					nret = 0;
					break;
				}
			}

			if( szaction[npos] == FUN_RDELIMITERC )
				nclose--;

			if( szaction[npos] == FUNP_DELIMITERC &&
				szaction[npos+1] == FUN_RDELIMITERC)
			{
				nret = 0;
				break;
			}

			npos++;
		}

		if( npos && 
			szaction[npos-1] != FUN_RDELIMITERC)
			nret = 0;

		if( nret && nclose )
			nret = false;

#ifdef _debug
		if( !nret )
			printf( "buff check action syntax error : %s\n", szaction );
#endif

		return nret;
	}

	pbuffaction	buff_str::find_act(const std::string& szaction)
	{
		for( int32 nloopcount = 0; nloopcount < g_buffaction_num; nloopcount++ )
		{
			if(szaction == g_buffaction[nloopcount].szname)
				return g_buffaction[nloopcount].buffaction;
		}
		return nullptr;
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	void buff_action::parse_action(const std::string& szaction)
	{
		m_bauto = true;
		m_action.func_ptr = buff_str::find_act(szaction);
	}
	bool buff_action::_condition::parse(char* szcondition, uint32 len)
	{
		m_strcondition.assign(szcondition);
		if (m_strcondition.empty())
			return false;

		//trim();

		while (!parse_over())
		{
			if (m_strcondition.find_first_of("^|", m_pos) == m_pos)
			{
				if (!parse_operator())
					return false;
			}
			else
			{
				if (!parse_func())
					return false;
			}
		}

		return valid();
	}

	void buff_action::_condition::trim()
	{
		int32 pos = m_strcondition.find_first_of(' ');
		while (pos != std::string::npos)
		{
			m_strcondition.erase(pos);
		}
	}

	bool buff_action::_condition::parse_over()
	{
		return m_strcondition.length() <= m_pos;
	}

	bool buff_action::_condition::parse_func()
	{
		int32 param_begin = m_strcondition.find('(', m_pos);
		int32 param_end = m_strcondition.find(')', m_pos);

		if (param_begin==std::string::npos || param_end==std::string::npos || param_begin>=param_end)
			return false;

		std::string strcondition = m_strcondition.substr(m_pos, param_begin-m_pos);

		_function condition;
		condition.func_ptr = buff_str::find_act( (char*)strcondition.c_str() );
		if (!condition.func_ptr)
			return false;

		int32 param_len = param_end - param_begin - 1;
		if (param_len > 0)
		{
			std::string strparam = m_strcondition.substr(param_begin+1, param_len);
			if (!parse_param(strparam))
				return false;
		}

		m_functions.push_back(condition);
		m_pos = param_end + 1;
		return true;
	}

	bool buff_action::_condition::parse_operator()
	{
		if (parse_over())
			return false;

		char c = m_strcondition[m_pos];
		_cond_op op;
		switch (c)
		{
		case '^':
			op = op_and;
			break;
		case '|':
			op = op_or;
			break;
		//case '!':
		//	op = op_neg:
		//	break;
		default:
			return false;
		};

		m_operators.push_back(op);
		m_pos++;
		return true;
	}

	bool buff_action::_condition::parse_param(std::string& strparam)
	{
		if (strparam.empty()) return false;

		uint32 start_pos = 0;
		int32 comma_pos = strparam.find(',');
		int32 param_index = 0;
		std::string strparamitem;

//		try
		{
			while (comma_pos != std::string::npos)
			{
				strparamitem = strparam.substr(start_pos, comma_pos-start_pos);
				if (!strparamitem.empty())
				{
					ANSICHAR* temp_char = (ANSICHAR*)strparamitem.c_str();
//					buff_action_template_ptr->ActionParams[param_index++] = (int32)(temp_char);
				}


				start_pos = comma_pos+1;
				comma_pos = strparam.find(',', start_pos);
			}

			if (start_pos < strparam.length())
			{
				strparamitem = strparam.substr(start_pos);
				if (!strparamitem.empty())
				{
					ANSICHAR* temp_char = (ANSICHAR*)strparamitem.c_str();
//					buff_action_template_ptr->ActionParams[param_index] = (int32)(temp_char);
				}
			}
		}
// 		catch (boost::bad_lexical_cast e)
// 		{
// 			return false;	
// 		}

		return true;
	}

	bool buff_action::_condition::is_condition_right(buff_env_param& env, BuffActionTemplate* action_template_ptr)
	{
		std::vector<_function>::iterator iter_func = m_functions.begin();
		std::vector<_cond_op>::iterator iter_op = m_operators.begin();
		bool first = true;
		bool final_result = true;
		_cond_op op = op_none;

		while (iter_func != m_functions.end())
		{
			bool step_result = true;
			_function& func = *iter_func;

			if (func.func_ptr && func.func_ptr(env, action_template_ptr) == false)
			{
				step_result = false;
			}
				
			if (first)
			{
				final_result = step_result;
				first = false;
			}
			else
			{
				switch (op)
				{
				case op_and: final_result = final_result && step_result; break;
				case op_or: final_result = final_result || step_result; break;
				case op_none: return false;
				};
			}

			if (iter_op != m_operators.end())
			{
				op = *iter_op;
				++iter_op;
			}
			else
			{
				op = op_none;
			}

			++iter_func;
		}

		return final_result;
	}

	void buff_action::set_template(BuffActionTemplate* action_template_ptr)
	{
		if (nullptr == action_template_ptr)
		{
			return;
		}
		clear_data();
		m_buff_action_template = action_template_ptr;
		parse_action(m_buff_action_template->Action);
	}

	bool buff_action::is_condition_right( buff_env_param& env )
	{
		if (m_condition_exp.valid())
		{
			return m_condition_exp.is_condition_right(env, m_buff_action_template);
		}
		else if (m_condition.func_ptr && 
			m_condition.func_ptr(env, m_buff_action_template) == false)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool buff_action::operator( )( buff_env_param& env )
	{
		bool nret = false;
		if (m_action.func_ptr)
		{
			nret = m_action.func_ptr( env, m_buff_action_template);
		}			
			 
		return nret;
	}
}