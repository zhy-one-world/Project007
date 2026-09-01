
#include "time.hpp"
#include "server_log.hpp"
#include "ai_level_nomal.h"
#include "logic/unit_man.h"
#include "template/template_manager.h"
#include "Logic/fake_player_def.hpp"
#include "system/scene/base_map_system.h"
#include "logic/aoi/aoi_system.h"
#include "logic/buff_man.h"
#include "template/NpcTemplate_S.h"
#include "logic/unit.hpp"
#include "logic/player.hpp"
#include "../npc.hpp"
#include "internet/skill.pb.h"
#include "../skill_manager.h"
#include "../send_skill.h"
#include "internet/aoi.pb.h"
#include "internet/net.pb.h"

namespace faith
{
	void s_npc_skill::clear()
	{
		skill_template_id = 0;
		skill_self = 0;
		skill_cd_cur_time = 0.f;
		skill_cd_max_time = 0.f;
		skill_distance = 0.f;
		skill_distance_max = 0.f;
		skill_weight = 0.f;
		spell_time = 0.f;
		channel_time = 0.f;
		move_length = 0;
		value_high = 0.f;
		value_low = 0.f;
		hurt_offset.clear_data();
		AutoReleasePriority = 0;
		cast_time_axle.clear();
		cast_duration = 0.f;
		cast_trace_target_location = 0;
		cast_trace_target_rotation = 0;
	}

	ai_level_nomal::ai_level_nomal()
	{
		m_hate_vec.clear();
		m_hate_vec_len = 0;
		clear_data();
	}
	void ai_level_nomal::clear_data()
	{
		m_tick_next_time = 0;//下一个tick消耗的时间
		m_move_tick_time = 0;//开始移动的时间点
		m_face_next_time = 0;
		m_ai_state = e_ai_state_born;
		m_ai_state_time = 0;
		m_npc_ptr = nullptr;
		m_npc_template_ptr = nullptr;
		m_born_location.clear_data();
		m_born_rotation.clear_data();
		m_chase_rotation.clear_data();
		m_patrol_location.clear_data();	//巡逻点
		m_chase_location.clear_data();//开始追击的点
		m_view_length = 0;//最远可见距离
		m_pursuit_length = 0;;//最大追击距离
		m_patrol_length = 0;;//最大巡逻距离
		m_speed_vector.clear_data();//移动速度
		m_born_map_ent = nullptr;//出生的地图id
		m_skill_index = 0;;//技能列表的下标
		m_skill_order = 0;
		m_skill_vec.clear();//技能列表
		m_skill_vec_len = 0;//技能列表长度
		m_damage_vec.clear();
		m_damage_vec_len = 0;
		m_first_attack_index = 0;;//首个攻击者
		m_total_hate = 0;
		m_war_time = 0;
		m_back_hp_begin = false;//开始启动返回出生点回血
		m_back_hp_time = 0;//返回出生点回血间隔
		m_has_combo_skill = false;
		m_next_combo_skill = 0;
		m_patrol_type = 0;		//巡逻类型
		m_patrol_direction = 0;	//巡逻方向	1--正向	0--反向
		m_hate_vec.clear();
		m_hate_vec_len = 0;
		m_attack_map.clear();
		m_attack_num = 0;
		m_all_hate_list_gs_value = 0;
		m_follow_role_guid.clear_data();
	}

	void ai_level_nomal::create_ai(npc* unit_ptr, const s_map_pos& born_pos, const s_map_pos& patrol_pos, NpcTemplate* npc_template_ptr, Entity* map_ent)
	{
		if (nullptr == unit_ptr || nullptr == npc_template_ptr)
		{
			return;
		}
		m_npc_ptr = unit_ptr;
		m_npc_template_ptr = npc_template_ptr;
		m_born_map_ent = map_ent;
		m_born_location = born_pos.unit_location;
		m_born_rotation = born_pos.unit_rotation;
		m_patrol_location = patrol_pos.unit_location;
		m_patrol_type = npc_template_ptr->PatrolType;
		m_pursuit_length = m_npc_template_ptr->PursuitLength;
		m_patrol_length = m_npc_template_ptr->PatrolLength;
		m_view_length = m_npc_template_ptr->ViewLength;
		m_follow_length = m_npc_template_ptr->FollowLength;
		m_follow_attack_length = m_npc_template_ptr->FollowAttackLength;
		if (init_unit::is_fake_player(m_npc_ptr->get_unit_type(), m_npc_ptr->get_unit_sub_type()) == false
			|| unit_ptr->get_cur_map_template_ptr()->Type == e_map_type_cross_ladder)
		{
			init_skill_from_template();
		}
		if (FLOAT_IS_ZERO(m_npc_template_ptr->BornTime))
		{
			m_npc_ptr->set_born_anim(0);
			m_ai_state = e_ai_state_stand;
		}
		else
		{
			m_npc_ptr->set_residue_born_time(time_helper::get_cur_time_new().second);
			m_ai_state = e_ai_state_born;
			m_ai_state_time = init_unit::get_end_time(m_npc_template_ptr->BornTime);
		}
	}

	void ai_level_nomal::init_skill_from_template()
	{
		if (nullptr == m_npc_template_ptr || nullptr == m_npc_ptr)
		{
			return;
		}
		if (m_npc_template_ptr->SkillWarTime.size() % 4 != 0)
		{
			return;
		}
		for (int32 i = 0;
			i < m_npc_template_ptr->SkillWarTime.size() && i + npc_use_skill_len <= m_npc_template_ptr->SkillWarTime.size();
			i += npc_use_skill_len)
		{
			if (m_npc_template_ptr->SkillWarTime[i + 2] > 0)
			{
				s_npc_skill time_skill;
				time_skill.npc_skill_type = e_npc_skill_type_war_time;
				time_skill.value_high = m_npc_template_ptr->SkillWarTime[i];
				time_skill.value_low = m_npc_template_ptr->SkillWarTime[i + 1];
				time_skill.skill_template_id = m_npc_template_ptr->SkillWarTime[i + 2];
				time_skill.skill_self = m_npc_template_ptr->SkillWarTime[i + 3];
				m_skill_vec.push_back(time_skill);
			}
		}

		for (int32 i = 0;
			i < m_npc_template_ptr->SkillHate.size() && i + npc_use_skill_len <= m_npc_template_ptr->SkillHate.size();
			i += npc_use_skill_len)
		{
			if (m_npc_template_ptr->SkillHate[i + 2] > 0)
			{
				s_npc_skill hate_skill;
				hate_skill.npc_skill_type = e_npc_skill_type_hate;
				hate_skill.value_high = m_npc_template_ptr->SkillHate[i];
				hate_skill.value_low = m_npc_template_ptr->SkillHate[i + 1];
				hate_skill.skill_template_id = m_npc_template_ptr->SkillHate[i + 2];
				hate_skill.skill_self = m_npc_template_ptr->SkillHate[i + 3];
				m_skill_vec.push_back(hate_skill);
			}
		}

		for (int32 i = 0;
			i < m_npc_template_ptr->SkillHp.size() && i + npc_use_skill_len <= m_npc_template_ptr->SkillHp.size();
			i += npc_use_skill_len)
		{
			if (m_npc_template_ptr->SkillHp[i + 2] > 0)
			{
				s_npc_skill hp_skill;
				hp_skill.npc_skill_type = e_npc_skill_type_hp;
				hp_skill.value_high = m_npc_template_ptr->SkillHp[i];
				hp_skill.value_low = m_npc_template_ptr->SkillHp[i + 1];
				hp_skill.skill_template_id = m_npc_template_ptr->SkillHp[i + 2];
				hp_skill.skill_self = m_npc_template_ptr->SkillHp[i + 3];
				m_skill_vec.push_back(hp_skill);
			}
		}

		for (int32 i = 0;
			i < m_npc_template_ptr->SkillCD.size() && i + npc_use_skill_len <= m_npc_template_ptr->SkillCD.size();
			i += npc_use_skill_len)
		{
			if (m_npc_template_ptr->SkillCD[i + 2] > 0)
			{
				s_npc_skill cd_skill;
				cd_skill.npc_skill_type = e_npc_skill_type_cd;
				cd_skill.value_high = m_npc_template_ptr->SkillCD[i];
				cd_skill.value_low = m_npc_template_ptr->SkillCD[i + 1];
				cd_skill.skill_template_id = m_npc_template_ptr->SkillCD[i + 2];
				cd_skill.skill_self = m_npc_template_ptr->SkillCD[i + 3];
				m_skill_vec.push_back(cd_skill);
			}
		}
		for (int32 i = 0;
			i < m_npc_template_ptr->SkillNormal.size() && i + npc_use_skill_len <= m_npc_template_ptr->SkillNormal.size();
			i += npc_use_skill_len)
		{
			if (m_npc_template_ptr->SkillNormal[i + 2] > 0)
			{
				s_npc_skill normal_skill;
				normal_skill.npc_skill_type = e_npc_skill_type_nomal;
				normal_skill.value_high = 0;
				normal_skill.value_low = 0;
				normal_skill.skill_template_id = m_npc_template_ptr->SkillNormal[i + 2];
				normal_skill.skill_self = 0;
				m_skill_vec.push_back(normal_skill);
			}
		}
		m_skill_vec_len = m_skill_vec.size();

		skill_proto_skill_save_load save_data;
		for (int32 i = 0; i < m_skill_vec_len; ++i)
		{
			s_skill_info skill_info;
			if (init_unit::init_skill_data(skill_info, m_skill_vec[i].skill_template_id))
			{
				for (int32 j = 0; j < e_skill_info_max; ++j)
				{
					save_data.add_data_ary(skill_info.data_ary[j]);
				}
				init_skill_inst(m_skill_vec[i], m_skill_vec[i].skill_template_id);
			}
		}
		skill_manager::skill_set_owner(m_npc_ptr->get_array_index());
		skill_manager::load_skill(m_npc_ptr->get_array_index(), save_data);
	}

	void ai_level_nomal::init_fake_player_info(const s_fake_player_info& fake_player_ptr)
	{
		if (!m_npc_ptr) return;

		if (fake_player_ptr.skill_num > 0 && fake_player_ptr.skill_num <= MAX_SKILL_NUM)
		{
			for (int32 i = 0; i < fake_player_ptr.skill_num; ++i)
			{//先加CD技能
				s_skill_info const& skill_info = fake_player_ptr.skill_data[i];
				int32 skill_template_id = skill_info.data_ary[e_skill_info_template_id];
				int32 skill_type = skill_info.data_ary[e_skill_info_skill_type];
				if (skill_template_id > 0 && skill_info.data_ary[e_skill_info_activated] > 0 && skill_info.data_ary[e_skill_info_use_pos] > -1)
				{
					if (est_floor_skill_1 == skill_type ||
						est_floor_skill_2 == skill_type ||
						est_floor_skill_3 == skill_type)
					{
						add_fake_player_cd_skill(skill_template_id);
					}
				}
			}

			for (int32 i = 0; i < fake_player_ptr.skill_num; ++i)
			{//再加普通技能
				s_skill_info const& skill_info = fake_player_ptr.skill_data[i];
				int32 skill_template_id = skill_info.data_ary[e_skill_info_template_id];
				int32 skill_type = skill_info.data_ary[e_skill_info_skill_type];
				if (skill_template_id > 0 && skill_info.data_ary[e_skill_info_activated] > 0 && skill_info.data_ary[e_skill_info_use_pos] > -1)
				{
					if (est_floor_common == skill_type)
					{
						add_fake_player_normal_skill(skill_template_id);
					}
				}
			}

			m_skill_vec_len = m_skill_vec.size();

			skill_proto_skill_save_load save_data;
			int32 skill_num = 0;
			for (int32 i = 0; i < m_skill_vec_len; ++i)
			{
				s_skill_info skill_info;
				if (init_unit::init_skill_data(skill_info, m_skill_vec[i].skill_template_id))
				{
					for (int32 j = 0; j < e_skill_info_max; ++j)
					{
						save_data.add_data_ary(skill_info.data_ary[j]);
					}
				}
			}
			skill_manager::skill_set_owner(m_npc_ptr->get_array_index());
			skill_manager::load_skill(m_npc_ptr->get_array_index(), save_data);
		}
	}

	void ai_level_nomal::add_fake_player_normal_skill(int32 start_skill_template_id)
	{
		s_npc_skill temp_npc_skill;
		temp_npc_skill.npc_skill_type = e_npc_skill_type_nomal;
		temp_npc_skill.value_high = 0;
		temp_npc_skill.value_low = 0;
		temp_npc_skill.skill_template_id = start_skill_template_id;
		temp_npc_skill.skill_self = 0;
		init_skill_inst(temp_npc_skill, temp_npc_skill.skill_template_id);
		m_skill_vec.push_back(temp_npc_skill);

		int32 max_normal_skill_number = 4;
		int32 test_loop_count = 0;
		int32 next_skill_id = -1;

		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, temp_npc_skill.skill_template_id);
		if (skill_template_ptr)
		{
			next_skill_id = skill_template_ptr->BeginSkill;
			while (next_skill_id > 0 &&
				next_skill_id != start_skill_template_id &&
				test_loop_count < max_normal_skill_number)
			{
				temp_npc_skill.clear();
				temp_npc_skill.npc_skill_type = e_npc_skill_type_nomal;
				temp_npc_skill.value_high = 0;
				temp_npc_skill.value_low = 0;
				temp_npc_skill.skill_template_id = next_skill_id;
				temp_npc_skill.skill_self = 0;
				init_skill_inst(temp_npc_skill, temp_npc_skill.skill_template_id);
				m_skill_vec.push_back(temp_npc_skill);

				SkillTemplate* nex_skill_template_ptr = GET_TEMPLATE(SkillTemplate, next_skill_id);
				if (nex_skill_template_ptr)
				{
					next_skill_id = nex_skill_template_ptr->BeginSkill;
					test_loop_count++;
				}
				else
				{
					next_skill_id = -1;
				}
			}
		}
	}

	void ai_level_nomal::add_fake_player_cd_skill(int32 skill_template_id)
	{
		s_npc_skill temp_skill;
		temp_skill.npc_skill_type = e_npc_skill_type_cd;
		temp_skill.value_high = 0;
		temp_skill.value_low = 0;
		temp_skill.skill_template_id = skill_template_id;
		temp_skill.skill_self = 0;
		init_skill_inst(temp_skill, skill_template_id);

		m_skill_vec.push_back(temp_skill);
	}

	void ai_level_nomal::init_skill_inst(s_npc_skill& skill_inst, int32 skill_template_id)
	{
		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (skill_template_ptr)
		{
			skill_inst.skill_template_id = skill_template_id;
			skill_inst.skill_type = skill_template_ptr->Type;
			skill_inst.skill_cd_cur_time = 0;
			skill_inst.skill_cd_max_time = skill_template_ptr->SelfCd;
			skill_inst.skill_distance = skill_template_ptr->MaxDistance;
			skill_inst.skill_distance_max = skill_template_ptr->MaxDistance + skill_template_ptr->AddDistance;
			skill_inst.skill_weight = skill_template_ptr->TargetAngle;
			skill_inst.spell_time = skill_template_ptr->SpellTime;
			skill_inst.channel_time = skill_template_ptr->ChannelTime;
			skill_inst.AutoReleasePriority = skill_template_ptr->AutoReleasePriority;
			if (skill_template_ptr->CastTimeAxle.size() > 0)
			{
				skill_inst.cast_time_axle = skill_template_ptr->CastTimeAxle;
			}
			else
			{
				skill_inst.cast_time_axle.clear();
			}
			float move_length = 0.f;
			float cast_time = 0.f;

			for (int32 j = 0;
				j < skill_template_ptr->MoveLength.size() && j < skill_template_ptr->CastTimeAxle.size();
				j++)
			{
				move_length += skill_template_ptr->MoveLength[j];
				cast_time += skill_template_ptr->CastTimeAxle[j];
			}

			skill_inst.move_length = move_length;
			skill_inst.cast_duration = cast_time;
			skill_inst.cast_trace_target_location = skill_template_ptr->CastTraceTargetLocation;
			skill_inst.cast_trace_target_rotation = skill_template_ptr->CastTraceTargetRotation;
		}
	}

	void ai_level_nomal::ai_tick(const int64& new_time)
	{
		if (new_time < m_tick_next_time)
		{
			return;
		}
		if (nullptr == m_npc_template_ptr || nullptr == m_npc_ptr)
		{
			return;
		}

		face_unit(new_time);
		if (m_back_hp_begin && new_time > m_back_hp_time)
		{
			m_back_hp_begin = false;
			if (m_npc_ptr->get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur) < m_npc_ptr->get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max))
			{
				buff_man::add_buff_inst(m_npc_ptr->get_array_index(), m_npc_ptr->get_array_index(), GAMECONFIG->MonsterAddHp);
			}
		}
		if (m_npc_ptr->get_pawn_att().is_can_move() == false)
		{
			return;
		}
		if (m_npc_ptr->get_owner().is_valid())
		{
			if (m_npc_ptr->get_owner().runtime_id >= npc_arrary_index_begin)
			{
				npc& master_npc_ref = unit_man::get_npc(m_npc_ptr->get_owner());
				if (master_npc_ref.is_valid() && master_npc_ref.get_map_guid() == m_npc_ptr->get_map_guid())
				{

				}
				else
				{
					unit_man::remove_npc(m_npc_ptr->get_array_index(), false);
					return;//归属不存在就删,只判断npc
				}
			}
		}

		if (get_follow_guid().is_valid())
		{
			player& master_ref = unit_man::get_player(get_follow_guid());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				m_patrol_point_location = master_ref.get_new_map_pos().unit_location;
			}
			else
			{
				m_patrol_point_location = m_born_location;
			}
		}
		else
		{
			m_patrol_point_location = m_born_location;
		}
		switch (m_ai_state)
		{
		case e_ai_state_born:
		{
			born_anim_time(new_time);
		}
		break;
		case e_ai_state_stand:
		{
			state_stand(new_time);
		}
		break;
		case e_ai_state_move:
		{
			state_move(new_time);
		}
		break;
		case e_ai_state_chase:
		{
			state_chase(new_time);
		}
		break;
		case e_ai_state_move_to_born:
		{
			state_move_to_born(new_time);
		}
		break;
		case e_ai_state_skill:
		{
			state_skill(new_time);
		}
		break;
		case e_ai_state_follow:
		{
			state_follow(new_time);
		}
		break;
		default:
			break;
		}
		is_need_gather(new_time);
	}

	void ai_level_nomal::add_damage_value_with_summon(const int32& attack_index, const int64 hp)
	{
		if (!m_npc_template_ptr || attack_index <= 0)
		{
			return;
		}

		player* real_attarker = nullptr;
		player& attacker = unit_man::get_player(attack_index);
		if (attacker.is_valid())
		{
			real_attarker = &attacker;
		}
		else
		{
			npc& attacker_npc = unit_man::get_npc(attack_index);
			if (attacker_npc.is_valid() && attacker_npc.get_owner().is_valid())
			{
				player& attacker_owner = unit_man::get_player(attacker_npc.get_owner());
				real_attarker = &attacker_owner;
			}
		}
		if (nullptr == real_attarker || false == real_attarker->is_valid())
		{
			return;
		}
		add_damage_value(real_attarker->get_identifier(), real_attarker->get_legion_guid(), hp);
	}

	void ai_level_nomal::attack_value(const int32& attack_index, const int64 hp)
	{
		if (!m_npc_template_ptr || attack_index <= 0)
		{
			return;
		}

		//if (m_npc_template_ptr->AttackOther == 0)
		//{
		//	return;
		//}

		unit* real_attarker = nullptr;
		player& attacker = unit_man::get_player(attack_index);
		if (attacker.is_valid())
		{
			real_attarker = &attacker;
		}
		else
		{
			npc& attacker_npc = unit_man::get_npc(attack_index);
			if (attacker_npc.is_valid() && attacker_npc.get_owner().is_valid())
			{
				player& attacker_owner = unit_man::get_player(attacker_npc.get_owner());
				real_attarker = &attacker_owner;
			}
			else if (attacker_npc.is_valid() && attacker_npc.get_unit_type() == e_unit_type_monster && attacker_npc.get_unit_sub_type() == e_monster_type_robot_player)
			{
				real_attarker = &attacker_npc;
			}
		}
		if (nullptr == real_attarker || false == real_attarker->is_valid())
		{
			return;
		}

		if (m_first_attack_index <= 0)
		{//设置首个攻击者ID，并同时将 未受攻击 标志位置0 放到这里是因为召唤物有归属者 不能取本身的index
			m_first_attack_index = real_attarker->get_array_index();
		}

		add_hate_value(real_attarker->get_identifier(), hp);
		add_damage_value(real_attarker->get_identifier(), real_attarker->get_legion_guid(), hp);

		//如果收到攻击的npc为召唤物，则把召唤物受到的伤害加给归属npc
		if (m_npc_ptr != nullptr && m_npc_template_ptr != nullptr && m_npc_template_ptr->DamageSummonWithSelf == 1)
		{
			s_unit_identifier cur_npc_owner = m_npc_ptr->get_owner();
			if (cur_npc_owner.is_valid() && cur_npc_owner != m_npc_ptr->get_identifier())
			{
				npc& real_owner_npc = unit_man::get_npc(cur_npc_owner);
				if (real_owner_npc.is_valid())
				{
					real_owner_npc.add_damage_value_with_summon(attack_index, hp);
				}
			}
		}

		if (m_npc_template_ptr->AttackOther > 0 && (m_ai_state == e_ai_state_stand || m_ai_state == e_ai_state_move))
		{
			change_state_to_chase();

			m_move_tick_time = 0;
			m_tick_next_time = 0;
			m_chase_location = m_npc_ptr->get_new_map_pos().unit_location;
			on_hate_list_may_change();
		}
	}
	void ai_level_nomal::born_anim_time(const int64& new_time)
	{
		if (m_ai_state_time <= new_time)
		{
			born_anim_end();
		}
	}
	void ai_level_nomal::born_anim_end()
	{
		m_npc_ptr->set_born_anim(0);
		m_ai_state = e_ai_state_stand;
	}
	void ai_level_nomal::state_stand(const int64& new_time)
	{
		/*if (get_follow_guid().is_valid())
		{
			player& master_ref = unit_man::get_player(get_follow_guid());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				//在主人一定范围内，进入技能状态, 否则进入move状态
				if (check_need_follow_master(get_follow_guid()))
				{
					set_next_time(new_time, ai_attack_tick_time);
					m_ai_state = e_ai_state_follow;
					return;
				}
			}
		}*/
		if (m_npc_ptr->is_summon_pet())
		{
			player& master_ref = unit_man::get_player(m_npc_ptr->get_owner());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				//在主人一定范围内，进入技能状态, 否则进入move状态
				if (false == check_need_follow_master(m_npc_ptr->get_owner().unit_guid))
				{
					if (master_ref.get_pawn_att().get_game_att(e_unit_game_att_war_state) == e_war_state_attack_in)
					{
						set_next_time(new_time, ai_next_attack_tick_time);
						m_ai_state = e_ai_state_skill;
					}
					else
					{
						set_next_time(new_time, ai_next_attack_tick_time);
						m_ai_state = e_ai_state_stand;
					}
				}
				else
				{
					m_ai_state = e_ai_state_move;
				}
			}
			else
			{
				//TODO 自我毁灭？
				unit_man::remove_npc(m_npc_ptr->get_array_index(), false);
				return;
			}
		}
		else
		{
			s_unit_identifier target_index = get_target_by_hate();
			if (false == target_index.is_valid())
			{
				target_index = check_enemy();
			}
			if (false == target_index.is_valid())
			{
				set_next_time(new_time, ai_move_tick_time);
				if (m_npc_template_ptr->PatrolType > 0 && m_npc_template_ptr->CanMove > 0)
				{
					if (m_npc_template_ptr->PatrolType == e_ai_patrol_type_random)
					{
						int64 next_tick = ai_move_tick_time + rand() % 1000;
						set_next_time(new_time, ai_move_tick_time);
						if (init_unit::is_in_distance(m_patrol_point_location, m_npc_ptr->get_new_map_pos().unit_location, m_patrol_length))
						{
							fvector move_pos;
							move_pos.x = rand() % m_patrol_length - m_patrol_length / 2;
							move_pos.y = rand() % m_patrol_length - m_patrol_length / 2;

							m_npc_ptr->get_move_mgr().start(m_patrol_point_location + move_pos, next_tick / 1000.f, true, false);
							m_npc_ptr->get_move_mgr().send_unit_end_location(m_patrol_point_location + move_pos);
						}
						else
						{
							m_npc_ptr->get_move_mgr().start(m_patrol_point_location, next_tick / 1000.f, false, false);
							m_npc_ptr->get_move_mgr().send_unit_end_location(m_patrol_point_location);
						}
						m_ai_state = e_ai_state_move;
					}
					else if (m_npc_template_ptr->PatrolType == e_ai_patrol_type_line)
					{
						int64 next_tick = ai_move_tick_time * 3;
						set_next_time(new_time, next_tick);
						if (init_unit::is_in_distance(m_patrol_location, m_npc_ptr->get_new_map_pos().unit_location, 100))
						{
							m_npc_ptr->get_move_mgr().start(m_born_location, next_tick / 1000.f, false, false);
							m_npc_ptr->get_move_mgr().send_unit_end_location(m_born_location);
						}
						else if (init_unit::is_in_distance(m_born_location, m_npc_ptr->get_new_map_pos().unit_location, 100))
						{
							m_npc_ptr->get_move_mgr().start(m_patrol_location, next_tick / 1000.f, false, false);
							m_npc_ptr->get_move_mgr().send_unit_end_location(m_patrol_location);
						}
						m_ai_state = e_ai_state_move;

					}
				}
				else
				{
					m_speed_vector.clear_data();
				}
			}
			else
			{
				add_hate_value(target_index, 0);
				change_state_to_chase();
				m_tick_next_time = 0;
				m_move_tick_time = 0;
				m_war_time = new_time;
				m_chase_location = m_npc_ptr->get_new_map_pos().unit_location;
				m_chase_rotation = m_npc_ptr->get_new_map_pos().unit_rotation;
			}
		}
	}
	void ai_level_nomal::state_move(const int64& new_time)
	{
		/*if (get_follow_guid().is_valid())
		{
			player& master_ref = unit_man::get_player(get_follow_guid());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				if (check_need_follow_master(get_follow_guid()))
				{
					set_next_time(new_time, ai_attack_tick_time);
					m_ai_state = e_ai_state_follow;
					return;
				}
			}
		}*/
		if (m_npc_ptr->is_summon_pet())
		{
			player& master_ref = unit_man::get_player(m_npc_ptr->get_owner());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				//在主人一定范围内，则停止进入stand状态，否则继续
				if (false == check_need_follow_master(m_npc_ptr->get_owner().unit_guid))
				{
					set_next_time(new_time, ai_move_tick_time);
					if (m_npc_ptr->get_move_mgr().is_runing())
						m_npc_ptr->get_move_mgr().stop(true);
					m_ai_state = e_ai_state_stand;
				}
				else
				{
					s_map_pos follow_map_pos;
					follow_map_pos.clear_data();
					get_map_pos_need_master(follow_map_pos, m_npc_ptr->get_owner().unit_guid);

					//先处理z轴
					if (m_npc_ptr->get_new_map_pos().unit_location.z != follow_map_pos.unit_location.z)
					{
						s_map_pos new_map_pos;
						new_map_pos.clear_data();
						new_map_pos.unit_location = m_npc_ptr->get_new_map_pos().unit_location;
						new_map_pos.set_rotation(m_npc_ptr->get_new_map_pos().unit_rotation);
						new_map_pos.unit_location.z = follow_map_pos.unit_location.z;
						m_npc_ptr->set_new_map_pos(new_map_pos);
					}
					double move_speed = m_npc_ptr->get_pawn_att().get_attack_att_value(e_unit_attack_att_move_speed);
					float next_tick = follow_map_pos.unit_location.length_2d() / move_speed;
					set_next_time(new_time, next_tick);
					m_npc_ptr->get_move_mgr().start(follow_map_pos.unit_location, next_tick / 1000.f, true, false);
					m_npc_ptr->get_move_mgr().send_unit_end_location(follow_map_pos.unit_location);
					m_ai_state = e_ai_state_move;
				}
			}
			else
			{
				//TODO 自我毁灭？
				unit_man::remove_npc(m_npc_ptr->get_array_index(), false);
				return;
			}
		}
		else
		{
			s_unit_identifier target_index = get_target_by_hate();
			if (false == target_index.is_valid())
			{
				target_index = check_enemy();
			}
			if (false == target_index.is_valid())
			{
				set_next_time(new_time, ai_move_tick_time);
				if (m_npc_ptr->get_move_mgr().is_runing() == false)
				{
					m_ai_state = e_ai_state_stand;
				}
			}
			else
			{
				add_hate_value(target_index, 0);
				change_state_to_chase();

				m_tick_next_time = 0;
				m_move_tick_time = new_time;
				m_war_time = new_time;
				m_chase_location = m_npc_ptr->get_new_map_pos().unit_location;
				m_chase_rotation = m_npc_ptr->get_new_map_pos().unit_rotation;
			}
		}
	}
	void ai_level_nomal::state_chase(const int64& new_time)
	{
		/*if (get_follow_guid().is_valid())
		{
			player& master_ref = unit_man::get_player(get_follow_guid());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				//在主人一定范围内，进入技能状态, 否则进入move状态
				if (check_need_follow_master(get_follow_guid()))
				{
					set_next_time(new_time, ai_attack_tick_time);
					m_ai_state = e_ai_state_follow;
					return;
				}
			}
		}*/
		if (m_npc_ptr->is_summon_pet())
		{
			player& master_ref = unit_man::get_player(m_npc_ptr->get_owner());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
			}
			else
			{
			}
		}
		else
		{
			s_unit_identifier target_index = get_target_by_hate();
			if (false == target_index.is_valid())
			{
				m_chase_location = m_patrol_point_location;
				m_chase_rotation = m_born_rotation;
				back_chase(new_time);
				return;
			}

			unit& unit_ref = unit_man::get_unit(target_index);
			if (unit_ref.is_valid() == false || unit_ref.get_map_guid() != m_npc_ptr->get_map_guid())
			{
				del_hate_value(target_index);
				return;
			}
			if (unit_ref.is_dead())
			{
				reduce_hate_value(target_index);
				return;
			}

			if ((init_unit::is_in_distance(m_chase_location, m_npc_ptr->get_new_map_pos().unit_location, m_pursuit_length) == false)
				|| (init_unit::is_in_distance_with_high(unit_ref.get_new_map_pos().unit_location, m_npc_ptr->get_new_map_pos().unit_location, m_view_length, 2.0f, true) == false))
			{
				if (init_unit::is_in_distance(m_patrol_point_location, m_npc_ptr->get_new_map_pos().unit_location, m_patrol_length) == false)
				{
					m_chase_location = m_patrol_point_location;
					m_chase_rotation = m_born_rotation;
				}
				back_chase(new_time);
				return;
			}

			s_npc_skill* npc_skill_ptr = find_current_skill(new_time);
			if (nullptr == npc_skill_ptr)
			{
				return;
			}
			m_back_hp_begin = false;//如果在恢复时间内被打了，则打断回血
			fvector cur_npc_location = m_npc_ptr->get_new_map_pos().unit_location;
			fvector unit_location = unit_ref.get_new_map_pos().unit_location;

			bool move_all_flag = false;

			bool is_monster_chase_player_flag = false;
			if (unit_ref.get_unit_type() == e_unit_type_player
				&& m_npc_ptr->get_unit_type() == e_unit_type_monster
				&& m_npc_ptr->get_unit_sub_type() == e_monster_type_nomal)
			{
				is_monster_chase_player_flag = true;
			}

			if (init_unit::is_in_distance_with_high(unit_location, cur_npc_location, npc_skill_ptr->skill_distance, 5.0f, false))
			{

				if (is_monster_chase_player_flag)
				{
					fvector temp_ = m_destination_location - cur_npc_location;
					temp_.z = 0;
					if (m_destination_location.is_zero() || m_target_location.is_zero() || temp_.is_zero())
					{
						m_npc_ptr->get_move_mgr().stop(true);
						m_ai_state = e_ai_state_skill;
						m_destination_location.clear_data();
						m_target_location.clear_data();
						return;
					}
					if (temp_.length_2d() < 10)
					{
						m_npc_ptr->get_move_mgr().start(m_destination_location, 0.01, true, false);
						m_npc_ptr->get_move_mgr().send_unit_end_location(m_destination_location);
						set_next_time(new_time, second_tick_time);
						return;
					}
					move_all_flag = true;
				}
				else
				{
					if (!m_npc_ptr->is_area_npc())  //竞技场NPC 让移动
					{
						m_npc_ptr->get_move_mgr().stop(true);
					}
					m_ai_state = e_ai_state_skill;
					m_target_location.clear_data();
					m_destination_location.clear_data();
					return;
				}
			}

			if (m_npc_ptr->get_move_mgr().is_runing() == false && m_npc_template_ptr->CanMove > 0)
			{
				fvector destination_location;
				destination_location.clear_data();
				if (is_monster_chase_player_flag)
				{
					fvector temp_ = m_target_location - unit_location;
					temp_.z = 0;
					if (m_destination_location.is_zero() || m_target_location.is_zero() || !temp_.is_zero())
					{
						player* p_temp = (player*)&unit_ref;
						m_target_location = unit_location;
						m_destination_location = p_temp->get_position_by_chase(npc_skill_ptr->skill_distance, cur_npc_location);
					}
					destination_location = m_destination_location;
				}
				else
				{
					destination_location = unit_location;
				}

				fvector middle_end_location;
				middle_end_location.clear_data();
				f32 skill_distance_decimal = 0.3;
				if (move_all_flag)
				{
					skill_distance_decimal = 0;
				}
				middle_end_location.x = cur_npc_location.x + (destination_location.x - cur_npc_location.x) / 3;//取原路程的三分之一为当前目标的最终点
				middle_end_location.y = cur_npc_location.y + (destination_location.y - cur_npc_location.y) / 3;
				middle_end_location.z = cur_npc_location.z;
				fvector middle_diff_vector = middle_end_location - cur_npc_location;
				fvector diff_vector = destination_location - cur_npc_location;
				float diff_yaw = diff_vector.rotator().Yaw;
				float radian_value = diff_yaw * RADIAN;
				fvector move_vector;
				move_vector.x = (npc_skill_ptr->skill_distance - npc_skill_ptr->skill_distance * skill_distance_decimal) * cos(radian_value);
				move_vector.y = (npc_skill_ptr->skill_distance - npc_skill_ptr->skill_distance * skill_distance_decimal) * sin(radian_value);
				if ((move_vector.length_2d() * 2) >= middle_diff_vector.length_2d())//如果走到最远技能距离的length大于中间值的lengh，则执行原逻辑
				{
					double move_speed = m_npc_ptr->get_pawn_att().get_attack_att_value(e_unit_attack_att_move_speed);
					if (move_speed <= 100)
					{
						set_next_time(new_time, second_tick_time);
					}
					else
					{
						fvector target_location = destination_location - move_vector;;
						target_location.z = destination_location.z + m_npc_ptr->get_unit_half_height() - unit_ref.get_unit_half_height();
						float next_tick = move_vector.length_2d() / move_speed;
						if (is_monster_chase_player_flag)
						{
							target_location = destination_location;
							next_tick = (target_location - cur_npc_location).length_2d() / move_speed;
						}
						m_npc_ptr->get_move_mgr().start(target_location, next_tick, true, false);
						m_npc_ptr->get_move_mgr().send_unit_end_location(target_location);
						if (m_npc_ptr->is_area_npc())
						{
							set_next_time(new_time, next_tick);
						}
						else
						{
							set_next_time(new_time, next_tick + second_tick_time * 0.6);
						}
					}
				}
				else//否则执行新逻辑，先走到三分之一处再判断目标位置
				{
					double move_speed = m_npc_ptr->get_pawn_att().get_attack_att_value(e_unit_attack_att_move_speed);
					if (move_speed <= 100)
					{
						set_next_time(new_time, second_tick_time);
					}
					else
					{
						middle_end_location.z = destination_location.z + m_npc_ptr->get_unit_half_height() - unit_ref.get_unit_half_height();
						float next_tick = middle_diff_vector.length_2d() / move_speed;
						m_npc_ptr->get_move_mgr().start(middle_end_location, next_tick, true, false);
						m_npc_ptr->get_move_mgr().send_unit_end_location(middle_end_location);
						if (m_npc_ptr->is_area_npc())
						{
							set_next_time(new_time, next_tick);
						}
						else
						{
							set_next_time(new_time, next_tick + second_tick_time * 0.6);
						}
					}
				}
			}
		}
	}

	void ai_level_nomal::state_skill(const int64& new_time)
	{
		if (nullptr == m_npc_ptr)
		{
			return;
		}
		/*if (get_follow_guid().is_valid())
		{
			player& master_ref = unit_man::get_player(get_follow_guid());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				//在主人一定范围内，进入技能状态, 否则进入move状态
				if (check_need_follow_master(get_follow_guid()))
				{
					set_next_time(new_time, ai_attack_tick_time);
					m_ai_state = e_ai_state_follow;
					return;
				}
			}
		}*/
		if (m_npc_ptr->is_summon_pet())
		{
			player& master_ref = unit_man::get_player(m_npc_ptr->get_owner());
			if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
			{
				//优先移动到主人一定范围内，如果在，则优先选择目标施放技能，找不到则进入stand状态
				if (check_need_follow_master(m_npc_ptr->get_owner().unit_guid))
				{
					m_ai_state = e_ai_state_move;
					return;
				}
				m_ai_state = e_ai_state_stand;
				set_next_time(new_time, ai_attack_tick_time);
				s_unit_identifier target_index = check_enemy();
				if (false == target_index.is_valid())
				{
					return;
				}
				s_npc_skill* npc_skill_ptr = find_current_skill(new_time);
				if (nullptr == npc_skill_ptr)
				{
					return;
				}
				SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, npc_skill_ptr->skill_template_id);
				if (skill_template_ptr == nullptr)
				{
					return;
				}
				unit& unit_target = unit_man::get_unit(target_index);
				if (unit_target.get_pawn_att().get_state_att(e_unit_attack_state_stealth_show_me)
					|| unit_target.get_pawn_att().get_state_att(e_unit_attack_state_stealth))
				{
					return;
				}
				if (m_npc_template_ptr->AttackNum > 0 && m_attack_num >= m_npc_template_ptr->AttackNum)
				{
					return;
				}
				//if (can_sand_skill_by_face(npc_skill_ptr->skill_template_id) == false)
				//{
				//	return;
				//}
				fvector target_location = unit_target.get_new_map_pos().unit_location;
				s_map_pos npc_pos = m_npc_ptr->get_new_map_pos();
				if (init_unit::is_in_distance(target_location, npc_pos.unit_location, npc_skill_ptr->skill_distance_max) == false)
				{
					return;
				}
				//调整方向
				{
					fvector  cur_location = m_npc_ptr->get_new_map_pos().unit_location;
					frotator cur_rotation = m_npc_ptr->get_new_map_pos().unit_rotation;
					fvector  targer_location = unit_target.get_new_map_pos().unit_location;
					fvector  diff_vector = targer_location - cur_location;
					float need_yaw = diff_vector.rotator().yaw;
					if (cur_rotation.yaw != need_yaw)
					{
						m_npc_ptr->set_new_map_angle(need_yaw);
						send_npc_face_unit();
					}
				}
				const s_unit_identifier& attacker_idf = m_npc_ptr->get_identifier();
				int32 skill_template_id = npc_skill_ptr->skill_template_id;
				const s_unit_identifier& target_idf = unit_target.get_identifier();
				fvector dest_pos;
				send_skill::send_skill_spell_cast(attacker_idf, skill_template_id, m_skill_order++, 0, target_idf, npc_pos, dest_pos);
				npc_skill_ptr->skill_cd_cur_time = init_unit::get_end_time(npc_skill_ptr->skill_cd_max_time);
				int32 fight_skill_time_len = skill_template_ptr->ChannelTime * second_tick_time + skill_template_ptr->SpellTime * second_tick_time + 100;
				check_and_operate_combo_skill(npc_skill_ptr);
				m_ai_state = e_ai_state_skill;
				set_next_time(new_time, fight_skill_time_len);
				if (m_npc_template_ptr->AttackOne)
				{
					m_attack_map.clear();
					m_attack_map[target_idf.runtime_id] = 1;
					m_attack_num++;
				}
				else if (m_npc_template_ptr->AttackAllOne)
				{
					m_attack_map[target_idf.runtime_id] = 1;
					m_attack_num++;
				}
			}
			else
			{
				//TODO 自我毁灭？
				unit_man::remove_npc(m_npc_ptr->get_array_index(), false);
				return;
			}
		}
		else
		{
			s_unit_identifier target_index = get_target_by_hate();
			if (false == target_index.is_valid())
			{
				m_chase_location = m_patrol_point_location;
				m_chase_rotation = m_born_rotation;
				back_chase(new_time);
				return;
			}
			set_next_time(new_time, ai_attack_tick_time);
			s_npc_skill* npc_skill_ptr = find_current_skill(new_time);
			if (nullptr == npc_skill_ptr)
			{
				return;
			}
			SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, npc_skill_ptr->skill_template_id);
			if (skill_template_ptr == nullptr)
			{
				return;
			}
			unit& unit_target = unit_man::get_unit(target_index);
			//if (unit_target.is_valid() == false 
			//	|| unit_target.is_dead()
			//	|| unit_target.get_map_guid() != m_npc_ptr->get_map_guid()
			//	|| (unit_target.get_pawn_att().get_game_att(e_unit_game_att_movement) >= e_move_ment_mount_fly && m_npc_ptr->get_pawn_att().get_game_att(e_unit_game_att_movement) < e_move_ment_mount_fly))
			//{
			//	del_hate_value(target_index);
			//	return;
			//}
			if (unit_target.get_pawn_att().get_state_att(e_unit_attack_state_stealth_show_me)
				|| unit_target.get_pawn_att().get_state_att(e_unit_attack_state_stealth))
			{
				return;
			}
			if (m_npc_template_ptr->AttackNum > 0 && m_attack_num >= m_npc_template_ptr->AttackNum)
			{
				return;
			}
			if (can_sand_skill_by_face(npc_skill_ptr->skill_template_id) == false && skill_template_ptr->IsAttack == 0)
			{
				return;
			}
			fvector target_location = unit_target.get_new_map_pos().unit_location;
			s_map_pos npc_pos = m_npc_ptr->get_new_map_pos();
			if (init_unit::is_in_distance_with_high(target_location, npc_pos.unit_location, npc_skill_ptr->skill_distance, 5.0f, false) == false)
			{
				change_state_to_chase();

				m_move_tick_time = 0;
				return;
			}
			if (skill_template_ptr->IsAttack != 0)
			{

				fvector  cur_location = m_npc_ptr->get_new_map_pos().unit_location;
				frotator cur_rotation = m_npc_ptr->get_new_map_pos().unit_rotation;
				fvector  targer_location = unit_target.get_new_map_pos().unit_location;
				fvector  diff_vector = targer_location - cur_location;
				float need_yaw = diff_vector.rotator().yaw;
				if (cur_rotation.yaw != need_yaw)
				{
					m_npc_ptr->set_new_map_angle(need_yaw);
					send_npc_face_unit();
				}

			}
			const s_unit_identifier& attacker_idf = m_npc_ptr->get_identifier();
			int32 skill_template_id = npc_skill_ptr->skill_template_id;
			const s_unit_identifier& target_idf = unit_target.get_identifier();
			fvector dest_pos;
			send_skill::send_skill_spell_cast(attacker_idf, skill_template_id, m_skill_order++, 0, target_idf, npc_pos, dest_pos);
			npc_skill_ptr->skill_cd_cur_time = init_unit::get_end_time(npc_skill_ptr->skill_cd_max_time);
			int32 fight_skill_time_len = skill_template_ptr->ChannelTime * second_tick_time + skill_template_ptr->SpellTime * second_tick_time + 100;
			if (skill_template_ptr->IsAttack != 0)
			{
				fight_skill_time_len -= skill_template_ptr->DecTime * second_tick_time;
			}
			set_next_time(new_time, fight_skill_time_len);
			check_and_operate_combo_skill(npc_skill_ptr);
			//if (skill_template_id == warning_guide_skill_template_id)
			//{
			//	player& player_ref = unit_man::get_player(target_idf);
			//	if (player_ref.is_valid() && player_ref.get_guide_mgr().if_guide_triggered(skill_template_ptr->TriggerGuide))
			//	{
			//		m_npc_ptr->relieve_stop_time(skill_template_id);
			//	}
			//}

			//判断是不是跨服世界boss的无敌技能如果是就发送技能开始时间到客户端{
			if (m_npc_ptr->is_valid() && m_npc_ptr->is_god_skill(npc_skill_ptr->skill_template_id))
			{
				m_npc_ptr->set_boss_god_skill_begin_time(int32(new_time / 1000));
				m_npc_ptr->send_boss_god_skill_begion_time();
			}



			if (m_npc_template_ptr->AttackOne)
			{
				m_attack_map.clear();
				m_attack_map[target_idf.runtime_id] = 1;
				m_attack_num++;
			}
			else if (m_npc_template_ptr->AttackAllOne)
			{
				m_attack_map[target_idf.runtime_id] = 1;
				m_attack_num++;
			}
		}
	}
	bool ai_level_nomal::can_sand_skill_by_face(int32 skill_template_id)
	{
		s_unit_identifier target_index = get_target_by_hate();
		if (false == target_index.is_valid())
		{
			return false;
		}
		unit& target_unit_ref = unit_man::get_unit(target_index);
		if (target_unit_ref.is_valid() == false)
		{
			return false;
		}
		fvector cur_location = m_npc_ptr->get_new_map_pos().unit_location;
		frotator cur_rotation = m_npc_ptr->get_new_map_pos().unit_rotation;
		fvector targer_location = target_unit_ref.get_new_map_pos().unit_location;
		fvector diff_vector = targer_location - cur_location;
		int32 need_yaw = diff_vector.rotator().yaw;
		int32 cur_yaw = cur_rotation.yaw;

		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (skill_template_ptr == nullptr)
		{
			return false;
		}
		if (cur_yaw < 0)
		{
			cur_yaw += 360;
		}
		if (need_yaw < 0)
		{
			need_yaw += 360;
		}
		int32 diff_yaw = abs(cur_yaw - need_yaw);
		if (diff_yaw > 180)
		{
			diff_yaw = 360 - diff_yaw;
		}
		if (diff_yaw <= skill_template_ptr->TargetAngle)
		{
			return true;
		}
		return false;
	}
	void ai_level_nomal::face_unit(const int64& new_time)
	{
		if (m_ai_state == e_ai_state_chase || m_npc_ptr->get_pawn_att().is_can_move() == false)
		{
			return;
		}
		if (m_npc_ptr->is_summon_pet())
			return;
		if (new_time < m_face_next_time)
		{
			return;
		}
		m_face_next_time = new_time + second_tick_time;
		s_unit_identifier target_index = get_target_by_hate();

		if (false == target_index.is_valid())
		{
			target_index = check_enemy();
		}

		if (false == target_index.is_valid())
		{
			return;
		}

		unit& target_unit_ref = unit_man::get_unit(target_index);
		if (target_unit_ref.is_valid() == false)
		{
			return;
		}
		fvector cur_location = m_npc_ptr->get_new_map_pos().unit_location;
		frotator cur_rotation = m_npc_ptr->get_new_map_pos().unit_rotation;
		fvector targer_location = target_unit_ref.get_new_map_pos().unit_location;
		fvector diff_vector = targer_location - cur_location;
		float need_yaw = diff_vector.rotator().yaw;
		if (cur_rotation.yaw != need_yaw)
		{
			m_npc_ptr->set_new_map_angle(need_yaw);
			send_npc_face_unit();
		}
	}
	void ai_level_nomal::send_npc_face_unit()
	{
		if (nullptr == m_npc_ptr)
		{
			return;
		}
		aoi_proto_face_unit msg;
		msg.set_unit_guid(m_npc_ptr->get_unit_guid().server_64);
		msg.set_move_yaw(m_npc_ptr->get_new_map_pos().unit_rotation.yaw);
		m_npc_ptr->send_message_to_aoi(&msg, e_msgindex_s2c_aoi_face_unit);
	}
	void ai_level_nomal::back_chase(const int64& new_time)
	{
		int64 next_tick = ai_move_tick_time + rand() % 1000;
		m_npc_ptr->get_move_mgr().start(m_chase_location, next_tick / 1000.f, false, false);
		m_npc_ptr->get_move_mgr().send_unit_end_location(m_chase_location);
		m_ai_state = e_ai_state_move_to_born;
		if (m_npc_template_ptr->ExitBattleHp > 0)
		{
			m_back_hp_time = init_unit::get_end_time(m_npc_template_ptr->ExitBattleHp);
			m_back_hp_begin = true;
		}
		clear_hate_value();
		clear_damage_value();
		on_hate_list_may_change();
		m_war_time = 0;
		m_total_hate = 0;
		m_tick_next_time = 0;
		m_first_attack_index = 0;
	}
	void ai_level_nomal::state_follow(const int64& new_time)
	{
		player& master_ref = unit_man::get_player(get_follow_guid());
		if (master_ref.is_valid() && master_ref.get_map_guid() == m_npc_ptr->get_map_guid())
		{
			if (false == check_need_follow_attack_master(get_follow_guid()))
			{
				if (m_npc_ptr->get_move_mgr().is_runing())
				{
					m_npc_ptr->get_move_mgr().stop(true);
				}
				set_next_time(new_time, ai_next_attack_tick_time);
				m_ai_state = e_ai_state_move;
			}
			else
			{
				s_map_pos follow_map_pos;
				follow_map_pos.clear_data();
				get_map_pos_need_master(follow_map_pos, get_follow_guid());

				//先处理z轴
				if (m_npc_ptr->get_new_map_pos().unit_location.z != follow_map_pos.unit_location.z)
				{
					s_map_pos new_map_pos;
					new_map_pos.clear_data();
					new_map_pos.unit_location = m_npc_ptr->get_new_map_pos().unit_location;
					new_map_pos.set_rotation(m_npc_ptr->get_new_map_pos().unit_rotation);
					new_map_pos.unit_location.z = follow_map_pos.unit_location.z;
					m_npc_ptr->set_new_map_pos(new_map_pos);
				}
				double move_speed = m_npc_ptr->get_pawn_att().get_attack_att_value(e_unit_attack_att_move_speed);
				float next_tick = follow_map_pos.unit_location.length_2d() / move_speed;
				set_next_time(new_time, next_tick);
				m_npc_ptr->get_move_mgr().start(follow_map_pos.unit_location, next_tick / 1000.f, true, false);
				m_npc_ptr->get_move_mgr().send_unit_end_location(follow_map_pos.unit_location);
				m_ai_state = e_ai_state_follow;
			}
		}
		else
		{
			m_ai_state = e_ai_state_move_to_born;
			set_next_time(new_time, ai_move_tick_time);
		}
	}

	void ai_level_nomal::set_next_time(const int64& new_time, int32 add_time)
	{
		m_tick_next_time = new_time + add_time;
	}
	void ai_level_nomal::npc_bron_clear()
	{
		clear_hate_value();
		clear_damage_value();
		m_war_time = 0;
		m_total_hate = 0;
		m_tick_next_time = 0;
		m_first_attack_index = 0;
	}
	bool const ai_level_nomal::is_boss_war_state() const
	{
		if (nullptr == m_npc_ptr)
		{
			return false;
		}
		if (e_ai_state_chase == m_ai_state || e_ai_state_skill == m_ai_state)
		{
			return true;
		}
		return false;
	}

	void ai_level_nomal::set_born_pos(s_map_pos born_pos)
	{
		m_born_location = born_pos.unit_location;
		m_born_rotation = born_pos.unit_rotation;
	}

	void ai_level_nomal::state_move_to_born(const int64& new_time)
	{
		set_next_time(new_time, ai_attack_tick_time);
		m_npc_ptr->set_new_map_angle(m_chase_rotation.Yaw);
		send_npc_face_unit();
		if (m_npc_ptr->get_move_mgr().is_runing() == false)
		{
			m_ai_state = e_ai_state_stand;
			return;
		}
		m_ai_state = e_ai_state_stand;
		set_next_time(new_time, ai_move_tick_time + rand() % 1000);
	}
	s_npc_skill* ai_level_nomal::get_current_skill()
	{
		if (m_skill_index >= 0 && m_skill_index < m_skill_vec.size())
		{
			if (m_has_combo_skill
				&& m_skill_vec[m_skill_index].skill_type == e_npc_skill_type_nomal
				&& m_next_combo_skill > 0)
			{//能连击+找到的技能时普攻+普通的下一段技能已确定时
				return find_skill_by_template_id(m_next_combo_skill);
			}
			else
			{
				return &m_skill_vec[m_skill_index];
			}
		}
		return nullptr;
	}

	void ai_level_nomal::check_and_operate_combo_skill(s_npc_skill* cur_use_skill)
	{
		if (cur_use_skill == nullptr)
		{
			return;
		}
		if (m_has_combo_skill
			&& cur_use_skill->npc_skill_type == e_npc_skill_type_nomal)
		{
			SkillTemplate* TemplatePtr = GET_TEMPLATE(SkillTemplate, cur_use_skill->skill_template_id);
			if (TemplatePtr == nullptr)
			{
				return;
			}
			m_next_combo_skill = TemplatePtr->NextSkill;
		}
		else
		{
			m_next_combo_skill = 0;
		}
	}

	s_npc_skill* ai_level_nomal::find_current_skill(const int64& new_time)
	{
		double hp_cur = m_npc_ptr->get_pawn_att().get_unit_base_att(e_base_att_info_hp_cur);
		double hp_max = m_npc_ptr->get_pawn_att().get_attack_att_value(e_unit_attack_att_hp_max);
		if (FLOAT_IS_ZERO(hp_max))
		{
			return get_current_skill();
		}
		hp_cur = hp_cur / hp_max * 100;
		int32 skill_first = -1;
		m_skill_index = -1;
		for (int32 i = 0; i < m_skill_vec_len; ++i)
		{
			s_npc_skill& temp_skill = m_skill_vec[i];
			if (temp_skill.skill_cd_cur_time > new_time || temp_skill.skill_template_id == 0)
			{
				continue;
			}
			if ((m_skill_index >= 0 && m_skill_index < m_skill_vec.size())
				&& temp_skill.npc_skill_type != m_skill_vec[m_skill_index].npc_skill_type)
			{//当已经有候选技能时（即已经循环出最高优先级的技能类型，如 CD类技能）
			//就不要再考虑其他类型的技能了
				continue;
			}

			switch (temp_skill.npc_skill_type)
			{
			case e_npc_skill_type_cd:
			{
				if (temp_skill.AutoReleasePriority > skill_first)
				{
					skill_first = temp_skill.AutoReleasePriority;
					m_skill_index = i;
				}
			}
			break;
			case e_npc_skill_type_hp:
			{
				if (hp_cur >= temp_skill.value_low && hp_cur <= temp_skill.value_high)
				{
					if (temp_skill.AutoReleasePriority > skill_first)
					{
						skill_first = temp_skill.AutoReleasePriority;
						m_skill_index = i;
					}
				}
			}
			break;
			case e_npc_skill_type_hate:
			{
				if (m_total_hate >= temp_skill.value_low && m_total_hate <= temp_skill.value_high)
				{
					if (temp_skill.AutoReleasePriority > skill_first)
					{
						skill_first = temp_skill.AutoReleasePriority;
						m_skill_index = i;
					}
				}
			}
			break;
			case e_npc_skill_type_war_time:
			{
				float diff_time = 0;
				if (m_war_time > 0)
				{
					diff_time = utility::get_tick_count() - m_war_time;
				}
				if (diff_time >= temp_skill.value_low * 1000 && diff_time <= temp_skill.value_high * 1000)
				{
					if (temp_skill.AutoReleasePriority > skill_first)
					{
						skill_first = temp_skill.AutoReleasePriority;
						m_skill_index = i;
					}
				}
			}
			break;
			case e_npc_skill_type_nomal:
			{
				if (temp_skill.AutoReleasePriority > skill_first)
				{
					skill_first = temp_skill.AutoReleasePriority;
					m_skill_index = i;
				}
			}
			break;
			default:
				break;
			}
		}
		return get_current_skill();
	}

	s_npc_skill* ai_level_nomal::find_skill_by_template_id(const int32& skill_template_id)
	{
		for (int32 i = 0; i < m_skill_vec_len; ++i)
		{
			if (m_skill_vec[i].skill_template_id == skill_template_id)
			{
				return &m_skill_vec[i];
			}
		}
		return nullptr;
	}

	s_unit_identifier ai_level_nomal::check_enemy()
	{
		s_unit_identifier target_player;
		s_unit_identifier target_npc;
		s_unit_identifier target_boss;
		if (m_npc_template_ptr->PassiveOrActive > 0 && m_npc_template_ptr->AttackOther > 0)
		{
			const fvector& cur_position = m_npc_ptr->get_new_map_pos().unit_location;
			const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(m_born_map_ent, cur_position);

			if (aoi_tower_watch.size() > 0)
			{
				int32 temp_attack_index = 0;
				int32 max_hate = 0;
				int32 min_len = 0;
				int64 min_player = 0;
				int64 min_npc = 0;
				unit_index_map_const_it it_end = aoi_tower_watch.end();
				for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != it_end; ++it)
				{
					const int32& unit_index = it->first;
					if (unit_index == m_npc_ptr->get_array_index())
					{
						continue;
					}
					unit_index_map_it attack_it = m_attack_map.find(unit_index);
					if (attack_it != m_attack_map.end())
					{
						continue;
					}
					unit& unit_ref = unit_man::get_unit(unit_index);
					const fvector& unit_position = unit_ref.get_new_map_pos().unit_location;
					if (init_unit::is_in_distance_with_high(cur_position, unit_position, m_view_length, 2.0f, false) == false)
					{//看不见
						continue;
					}
					int64 cur_dis = init_unit::get_distance(cur_position, unit_position);
					if (/*(unit_ref.get_pawn_att().get_game_att(e_unit_game_att_movement) >= e_move_ment_mount_fly && m_npc_ptr->get_pawn_att().get_game_att(e_unit_game_att_movement) < e_move_ment_mount_fly)
						||*/ unit_ref.is_dead()
						|| unit_ref.get_pawn_att().get_state_att(e_unit_attack_state_stealth_show_me)
						|| unit_ref.get_pawn_att().get_state_att(e_unit_attack_state_stealth)
						|| m_npc_ptr->is_mission_see(unit_index) == false
						|| m_npc_ptr->get_pk_community_mgr().is_skill_target(unit_ref.get_array_index()) == false
						)
					{//角色处于不可攻击的状态
						continue;
					}
					if (unit_ref.get_unit_type() == e_unit_type_player)
					{
						//选取最近的那个
						if (min_player == 0)
						{
							min_player = cur_dis;
							target_player = unit_ref.get_identifier();
						}
						else
						{
							if (cur_dis < min_player)
							{
								min_player = cur_dis;
								target_player = unit_ref.get_identifier();
							}
						}
					}
					else
					{
						//选取最近的那个
						if (min_npc == 0)
						{
							min_npc = cur_dis;
							target_npc = unit_ref.get_identifier();
							if (unit_ref.get_unit_sub_type() == e_monster_type_boss)
							{
								target_boss = unit_ref.get_identifier();
							}
						}
						else
						{
							if (cur_dis < min_npc)
							{
								min_npc = cur_dis;
								target_npc = unit_ref.get_identifier();
								if (unit_ref.get_unit_sub_type() == e_monster_type_boss)
								{
									target_boss = unit_ref.get_identifier();
								}
							}
						}
					}
				}
			}
		}
		if (m_npc_template_ptr->AttackNpc == 1)
		{
			add_hate_value(target_npc, 0);
			return target_npc;
		}
		else if (m_npc_template_ptr->AttackNpc == 2)
		{
			add_hate_value(target_boss, 0);
			return target_npc;
		}
		else
		{
			if (target_player.is_valid())
			{
				add_hate_value(target_player, 0);
				return target_player;
			}
			add_hate_value(target_npc, 0);
			return target_npc;
		}
	}

	void ai_level_nomal::on_dead()
	{
		m_ai_state = e_ai_state_dead;
	}

	bool ai_level_nomal::aoi_out(const s_unit_identifier& unit_identifier)
	{
		if (unit_identifier.runtime_id == m_first_attack_index)
		{
			m_first_attack_index = 0;
		}
		return del_hate_value(unit_identifier);
	}

	bool ai_level_nomal::restore_cache_hate_val(const s_unit_identifier& unit_identifier, int64 dead_duration)
	{
		if (m_dead_hate_cache.empty())
		{
			return false;
		}
		if (nullptr == m_npc_template_ptr || m_npc_template_ptr->HateListParam.size() < 2)
		{
			return false;
		}
		player& player_ref = unit_man::get_player(unit_identifier);
		if (false == player_ref.is_valid())
		{
			return false;
		}

		std::map<uint64, int32>::iterator ite = m_dead_hate_cache.find(player_ref.get_unit_guid().server_64);
		if (ite == m_dead_hate_cache.end())
		{
			return false;
		}

		int64 limit_revive_millsec = m_npc_template_ptr->HateListParam[0] * second_tick_time;
		float revive_restore_ratio = m_npc_template_ptr->HateListParam[1];
		if (limit_revive_millsec < dead_duration)
		{
			return false;
		}

		int32 revive_restore_hate = revive_restore_ratio * ite->second;
		add_hate_value(unit_identifier, revive_restore_hate);
		on_hate_list_may_change();

		return true;
	}

	void ai_level_nomal::add_hate_value(const s_unit_identifier& unit_identifier, int64 hate_value)
	{
		if (unit_identifier.is_valid() == false)
		{
			return;
		}
		if (m_npc_template_ptr->NpcType == e_unit_type_summoned || m_npc_template_ptr->NpcType == e_unit_type_hide || m_npc_template_ptr->NpcType == e_unit_type_trap)
		{
			if (m_hate_vec_len > 0)
			{
				m_hate_vec[0].unit_identifier = unit_identifier;
				m_hate_vec[0].hate_value = hate_value;
				return;
			}
		}
		int32 empty_index = -1;
		for (int32 i = 0; i < m_hate_vec_len; ++i)
		{
			s_hate_info& hate_info_ref = m_hate_vec[i];
			if (hate_info_ref.unit_identifier.unit_guid == unit_identifier.unit_guid)
			{
				hate_info_ref.unit_identifier.runtime_id = unit_identifier.runtime_id;
				hate_info_ref.hate_value += hate_value;
				return;
			}
			else if (hate_info_ref.unit_identifier.is_valid() == false && empty_index == -1)
			{
				empty_index = i;
			}
		}
		if (empty_index == -1)
		{
			s_hate_info new_info;
			m_hate_vec.push_back(new_info);
			empty_index = m_hate_vec_len++;
		}
		s_hate_info& npc_hate_info = m_hate_vec[empty_index];
		npc_hate_info.unit_identifier = unit_identifier;
		npc_hate_info.hate_value = hate_value;
		unit& unit_ref = unit_man::get_unit(unit_identifier);
		if (unit_ref.is_valid())
		{
			npc_hate_info.gs_value = unit_ref.get_unit_gs_value();
			m_all_hate_list_gs_value += unit_ref.get_unit_gs_value();
		}
	}

	bool ai_level_nomal::del_hate_value(const s_unit_identifier& unit_identifier)
	{
		if (unit_identifier.is_valid() == false)
		{
			return true;
		}
		for (int32 i = 0; i < m_hate_vec_len; ++i)
		{
			s_hate_info& hate_info_ref = m_hate_vec[i];
			if (hate_info_ref.unit_identifier == unit_identifier)
			{
				hate_info_ref.unit_identifier.clear_data();
				hate_info_ref.hate_value = 0;

				m_all_hate_list_gs_value -= hate_info_ref.gs_value;
				hate_info_ref.gs_value = 0;
				on_hate_list_may_change();
				return true;
			}
		}
		return false;
	}

	bool ai_level_nomal::reduce_hate_value(const s_unit_identifier& unit_identifier)
	{
		auto map_type = base_map_system::get_map_type(m_npc_ptr->get_map_ent());
		if (e_map_type_boss_home != map_type
			&& e_map_type_boss_vip_home != map_type
			&& e_map_type_boss_island != map_type)
		{
			return del_hate_value(unit_identifier);
		}

		for (int32 i = 0; i < m_hate_vec_len; ++i)
		{
			s_hate_info& hate_info_ref = m_hate_vec[i];
			if (hate_info_ref.unit_identifier == unit_identifier)
			{
				hate_info_ref.hate_value *= 0.99f;//检测如果玩家是死亡状态且处于打宝/永恒岛地图，则仇恨值缓慢减少而不是清零
				on_hate_list_may_change();
				return true;
			}
		}
		return false;
	}

	void ai_level_nomal::clear_hate_value()
	{
		for (int32 i = 0; i < m_hate_vec_len; ++i)
		{
			s_hate_info& hate_info_ref = m_hate_vec[i];
			hate_info_ref.unit_identifier.clear_data();
			hate_info_ref.hate_value = 0;
			hate_info_ref.gs_value = 0;
		}
	}

	s_unit_identifier ai_level_nomal::get_hate_max()
	{
		s_unit_identifier max_unit_identifier;
		if (m_npc_template_ptr->AttackOther == 0)
		{
			return max_unit_identifier;
		}
		int32 max_hate_value = -1;
		for (int32 i = 0; i < m_hate_vec_len; ++i)
		{
			s_hate_info& hate_info_ref = m_hate_vec[i];
			if (hate_info_ref.hate_value <= max_hate_value)
			{
				continue;
			}
			if (m_npc_template_ptr->AttackNpc > 0)
			{
				if (hate_info_ref.unit_identifier.runtime_id >= npc_arrary_index_begin)
				{
					max_unit_identifier = hate_info_ref.unit_identifier;
					max_hate_value = hate_info_ref.hate_value;
				}
			}
			else
			{
				max_unit_identifier = hate_info_ref.unit_identifier;
				max_hate_value = hate_info_ref.hate_value;
			}
		}
		return max_unit_identifier;
	}

	s_unit_identifier ai_level_nomal::get_target_by_hate()
	{
		s_unit_identifier max_unit_identifier;
		if (m_npc_template_ptr->AttackOther == 0)
		{
			return max_unit_identifier;
		}
		int32 max_hate_value = -1;
		for (int32 i = 0; i < m_hate_vec_len; ++i)
		{
			s_hate_info& hate_info_ref = m_hate_vec[i];
			e_ai_target_check_ret check_ret = check_can_be_ai_target(hate_info_ref.unit_identifier, hate_info_ref.hate_value);
			if (e_ai_target_check_ok != check_ret)
			{
				if (e_ai_target_check_del == check_ret)
				{
					hate_info_ref.unit_identifier.clear_data();
					hate_info_ref.hate_value = 0;
					on_hate_list_may_change();
				}
				else if (e_ai_target_check_dead == check_ret)
				{
					reduce_hate_value(hate_info_ref.unit_identifier);
				}
				continue;
			}
			if (hate_info_ref.hate_value <= max_hate_value)
			{
				continue;
			}
			if (m_npc_template_ptr->AttackNpc > 0)
			{
				if (hate_info_ref.unit_identifier.runtime_id >= npc_arrary_index_begin)
				{
					max_unit_identifier = hate_info_ref.unit_identifier;
					max_hate_value = hate_info_ref.hate_value;
				}
			}
			else
			{
				max_unit_identifier = hate_info_ref.unit_identifier;
				max_hate_value = hate_info_ref.hate_value;
			}
		}
		return max_unit_identifier;
	}

	e_ai_target_check_ret ai_level_nomal::check_can_be_ai_target(const s_unit_identifier& unit_identifier, int32 hate_val)
	{
		unit& unit_target = unit_man::get_unit(unit_identifier);
		if (unit_target.is_valid() == false)
		{
			return e_ai_target_check_del;
		}
		if (unit_target.get_map_guid() != m_npc_ptr->get_map_guid())
		{
			return e_ai_target_check_invalid;
		}

		//if (unit_target.get_pawn_att().get_game_att(e_unit_game_att_movement) >= e_move_ment_mount_fly && m_npc_ptr->get_pawn_att().get_game_att(e_unit_game_att_movement) < e_move_ment_mount_fly)
		//{
		//	return e_ai_target_check_invalid;
		//}

		if (unit_target.is_dead())
		{
			if (unit_target.get_unit_type() == e_unit_type_player && m_npc_template_ptr->HateListParam.size() >= 2)
			{
				m_dead_hate_cache[unit_target.get_unit_guid().server_64] = hate_val;
			}
			return e_ai_target_check_dead;
		}

		return e_ai_target_check_ok;
	}

	void ai_level_nomal::on_hate_list_may_change()
	{
		m_npc_ptr->send_gain_treasure_boss_owner_to_ws();
	}

	void ai_level_nomal::add_damage_value(const s_unit_identifier& unit_identifier, const guid_64& legion_guid, const int64& damage_value)
	{
		if (unit_identifier.is_valid() == false)
		{
			return;
		}
		for (int32 i = 0; i < m_damage_vec_len; ++i)
		{
			s_damage_info& damage_info_ref = m_damage_vec[i];
			if (damage_info_ref.unit_identifier.unit_guid == unit_identifier.unit_guid)
			{
				damage_info_ref.unit_identifier.runtime_id = unit_identifier.runtime_id;
				damage_info_ref.damage_value += damage_value;
				if (damage_info_ref.unit_legion_guid.is_valid() == false && legion_guid.is_valid() == true)
				{
					damage_info_ref.unit_legion_guid = legion_guid;
				}
				if (damage_info_ref.unit_legion_guid.is_valid() == true && legion_guid.is_valid() == false)
				{
					damage_info_ref.unit_legion_guid.clear_data();
				}
				return;
			}
		}
		s_damage_info new_info;
		new_info.unit_identifier = unit_identifier;
		new_info.unit_legion_guid = legion_guid;
		new_info.damage_value = damage_value;
		m_damage_vec.push_back(new_info);
		m_damage_vec_len++;
		player& player_ref = unit_man::get_player(unit_identifier.unit_guid);
		if (player_ref.is_valid())
		{
			aoi_proto_update_can_show_head msg;
			msg.set_unit_guid(m_npc_ptr->get_unit_guid().server_64);
			msg.set_can_show(true);
			player_ref.send_message_to_self(&msg, e_mgsindex_s2c_update_can_show_head);
		}

	}
	void ai_level_nomal::clear_damage_value()
	{
		m_damage_vec.clear();
		m_damage_vec_len = 0;
	}

	bool ai_level_nomal::check_near_patrol_point_by_direction(int32 direction)
	{
		if (direction > 0)
		{
			int64 length_diff = init_unit::get_distance(m_patrol_location, m_npc_ptr->get_new_map_pos().unit_location);
			return (length_diff < faith::m_patrol_reach_judge_distance);
		}
		else
		{
			int64 length_diff = init_unit::get_distance(m_born_location, m_npc_ptr->get_new_map_pos().unit_location);
			return (length_diff < faith::m_patrol_reach_judge_distance);
		}
	}
	void ai_level_nomal::is_need_gather(const int64& new_time)
	{
		if (nullptr == m_npc_ptr || m_npc_ptr->get_unit_type() != e_unit_type_monster || m_npc_ptr->get_unit_sub_type() != e_monster_type_robot_player)
		{
			return;
		}
		//自己的位置
		const fvector& cur_position = m_npc_ptr->get_new_map_pos().unit_location;
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(m_npc_ptr->get_map_ent(), cur_position);
		if (aoi_tower_watch.size() > 0)
		{
			unit_index_map_const_it it_end = aoi_tower_watch.end();
			for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != it_end;)
			{
				const int32& unit_index = it->first;
				it++;
				if (unit_index == m_npc_ptr->get_array_index())
				{
					continue;
				}
				unit_index_map_it attack_it = m_attack_map.find(unit_index);
				if (attack_it != m_attack_map.end())
				{
					continue;
				}
				npc& unit_ref = unit_man::get_npc(unit_index);
				if (e_unit_type_npc != unit_ref.get_unit_type() || (e_npc_type_hit_and_get_npc != unit_ref.get_unit_sub_type()))
				{
					continue;
				}
				const fvector& unit_position = unit_ref.get_new_map_pos().unit_location;

				int64 cur_dis = init_unit::get_distance(cur_position, unit_position);
				if (cur_dis > max_hit_and_get_npc_dis_power)
				{
					continue;
				}
				if (unit_ref.get_npc_template() != nullptr)//死亡给击杀者加buff
				{
					std::vector<int32> buff_id_arr = unit_ref.get_npc_template()->BuffIdWithKill;
					if (buff_id_arr.size() > 0)
					{
						for (int32 i = 0; i < buff_id_arr.size(); i++)
						{
							buff_man::add_buff_inst(m_npc_ptr->get_array_index(), m_npc_ptr->get_array_index(), buff_id_arr[i]);
						}
					}
				}
				unit_ref.be_dead();
				break;
			}
		}
	}
	fvector ai_level_nomal::get_patrol_target_location_by_direction(int32 direction)
	{
		if (direction > 0)
		{
			return m_patrol_location;
		}
		else
		{
			return m_born_location;
		}
	}
	void ai_level_nomal::relieve_stop_time(const int32& skill_template_id)
	{
		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (skill_template_ptr == nullptr)
		{
			return;
		}
		m_tick_next_time = init_unit::get_end_time(skill_template_ptr->ChannelTime + 1);
	}

	bool ai_level_nomal::check_need_follow_master(guid_64 role_guid)
	{
		//尝试计算三维坐标点
		player& master_ref = unit_man::get_player(role_guid);
		if (master_ref.is_valid())
		{
			const auto& npc_location = m_npc_ptr->get_new_map_pos().unit_location;
			const auto& master_location = master_ref.get_new_map_pos().unit_location;
			double x_diff = fabs(npc_location.x - master_location.x);
			double y_diff = fabs(npc_location.y - master_location.y);
			double z_diff = fabs(npc_location.z - master_location.z);
			auto distance = sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
			if (m_follow_attack_length > 0)
			{
				if (m_follow_attack_length <= distance)
				{
					return true;
				}
			}
			else
			{
				if (m_pursuit_length <= distance)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool ai_level_nomal::check_need_follow_attack_master(guid_64 role_guid)
	{
		//尝试计算三维坐标点
		player& master_ref = unit_man::get_player(role_guid);
		if (master_ref.is_valid())
		{
			const auto& npc_location = m_npc_ptr->get_new_map_pos().unit_location;
			const auto& master_location = master_ref.get_new_map_pos().unit_location;
			double x_diff = fabs(npc_location.x - master_location.x);
			double y_diff = fabs(npc_location.y - master_location.y);
			double z_diff = fabs(npc_location.z - master_location.z);
			auto distance = sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
			if (m_follow_length <= distance)
				return true;
		}
		return false;
	}

	void ai_level_nomal::get_map_pos_need_master(s_map_pos& map_pos, guid_64 role_guid)
	{
		player& master_ref = unit_man::get_player(role_guid);
		if (master_ref.is_valid())
		{
			map_pos = master_ref.get_back_pos();
			//map_pos.unit_location = master_ref.get_new_map_pos().unit_location;
			//map_pos.set_rotation(master_ref.get_new_map_pos().unit_rotation);
			//map_pos.unit_location.z = master_ref.get_new_map_pos().unit_location.z + master_ref.get_unit_half_height() / 2.5;
		}
	}

	fvector ai_level_nomal::get_bron_location()
	{
		return m_born_location;
	}

	void ai_level_nomal::set_follow_guid(guid_64 role_guid)
	{
		m_follow_role_guid = role_guid;
	}

	guid_64 ai_level_nomal::get_follow_guid()
	{
		return m_follow_role_guid;
	}

	void ai_level_nomal::change_state_to_chase()
	{
		m_ai_state = e_ai_state_chase;
		m_destination_location.clear_data();
		m_target_location.clear_data();
	}
}
