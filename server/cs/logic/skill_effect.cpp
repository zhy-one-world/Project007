/********************************************************************
	file base:	skill_effect
	file ext:	cpp
	author:		wenhongming
	purpose:	
*********************************************************************/

#include "aoi/aoi_system.h"
#include "buff_item.h"
#include "guide_def.hpp"
#include "utility/init_unit.h"
#include "logic/buff_man.h"
#include "logic/player.hpp"
#include "logic/type_def.hpp"
#include "logic/unit_man.h"
#include "server_log.hpp"
#include "skill_effect.h"
#include "template/template_manager.h"
#include "time.hpp"
#include "internet/net.pb.h"
#include "internet/skill.pb.h"

namespace faith
{
	void skill_effect::clear_data()
	{
		m_is_data_use = false;
		m_is_move_effect = false;
		m_bullet_template_id = 0;
		m_skill_param.clear_data();
		m_skill_add_buff_self_map.clear();
		m_skill_add_buff_target_map.clear();
		m_skill_template_ptr = nullptr;
		m_skill_bullet_template_ptr = nullptr;
		m_life_time = 0;
		m_move_tick_time = 0;
		m_begin_radius = 0;
		m_add_radius = 0;
		m_speed_vector.clear_data();
		m_speed_time.clear_data();
		m_target_index_map.clear();
	}
	bool& skill_effect::is_valid()
	{
		return m_is_data_use;
	}
	void skill_effect::init_effect_data(const use_skill_param& skill_param, const unit_index_map& skill_add_buff_target_map, const unit_index_map& skill_add_buff_self_map, const int32& effect_index, bool is_move_effect)
	{
		m_skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_param.skill_template_id);
		if (nullptr == m_skill_template_ptr || m_skill_template_ptr->MissileId.size() <= effect_index)
		{
			return;
		}
		m_is_data_use = true;
		m_skill_param = skill_param;
		m_is_move_effect = is_move_effect;
		m_skill_add_buff_self_map = skill_add_buff_self_map;
		m_skill_add_buff_target_map = skill_add_buff_target_map;
		if (skill_param.is_begin)
		{
			trig_buf(m_skill_param.target_unit_idf,
				m_skill_param.pos,
				m_skill_param.skill_template_id,
				m_skill_param.is_begin,
				m_skill_param.effect_index,
				m_skill_param);
			return;
		}
		m_bullet_template_id = m_skill_template_ptr->MissileId[effect_index];
		m_skill_bullet_template_ptr = GET_TEMPLATE(SkillBulletTemplate, m_bullet_template_id);
		if (m_skill_bullet_template_ptr)
		{
			m_life_time = init_unit::get_end_time(m_skill_bullet_template_ptr->Duration + m_skill_template_ptr->ChannelTime);
			fvector random_location(0, 0, 0);
			if (m_skill_bullet_template_ptr->RandomPoint.size() >= e_pos_type_max)
			{
				if (m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_x] > 0)
				{
					random_location.X = rand() % m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_x] - (m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_x] / 2);
				}
				if (m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_y] > 0)
				{
					random_location.Y = rand() % m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_y] - (m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_y] / 2);
				}
				if (m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_z] > 0)
				{
					random_location.Z = rand() % m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_z] - (m_skill_bullet_template_ptr->RandomPoint[e_pos_type_pos_z] / 2);
				}
			}
			unit& unit_ref = unit_man::get_unit(m_skill_param.sender_unit_idf);
			m_skill_param.pos = unit_ref.get_new_map_pos().unit_location;
			//if (effect_index != 0)
			{
				m_skill_param.yaw = unit_ref.get_new_map_pos().unit_rotation.yaw;
			}
			m_skill_param.pos += random_location;
			if (m_skill_bullet_template_ptr->EffectPos.size() >= e_pos_type_max)
			{
				m_skill_param.pos.x += m_skill_bullet_template_ptr->EffectPos[e_pos_type_pos_x] * cos(m_skill_param.yaw * RADIAN);
				m_skill_param.pos.y += m_skill_bullet_template_ptr->EffectPos[e_pos_type_pos_y] * sin(m_skill_param.yaw * RADIAN);
				m_skill_param.pos.z += m_skill_bullet_template_ptr->EffectPos[e_pos_type_pos_z];
			}
			m_begin_radius = m_skill_bullet_template_ptr->BeginRadius;
			if (m_skill_bullet_template_ptr->Duration > 0)
			{
				if (m_skill_bullet_template_ptr->Angle <= 0)
				{
					m_add_radius = (m_skill_bullet_template_ptr->EndRadius - m_skill_bullet_template_ptr->BeginRadius) / m_skill_bullet_template_ptr->Duration;
				}
				else
				{
					m_add_radius = m_skill_bullet_template_ptr->Speed * m_skill_bullet_template_ptr->Duration * sin(m_skill_bullet_template_ptr->Angle) * 2;
					m_add_radius *= m_add_radius;
				}
				m_speed_vector.x = m_skill_bullet_template_ptr->Speed * cos(m_skill_param.yaw * RADIAN);
				m_speed_vector.y = m_skill_bullet_template_ptr->Speed * sin(m_skill_param.yaw * RADIAN);
				m_speed_vector.z = 0;
				m_begin_pos = m_skill_param.pos;
				m_move_tick_time = utility::get_tick_count();
			}
			if (m_skill_bullet_template_ptr->EffectBPName <= 0 || m_is_move_effect)
			{
				if (m_skill_bullet_template_ptr->MoveType == e_skill_bullet_move_stop)
				{
					effect_hurt();
					m_life_time = 0;
				}
			}
		}
	}
	void skill_effect::send_effect_fly()
	{
		if (nullptr == m_skill_bullet_template_ptr)
		{
			return;
		}
		if (m_skill_bullet_template_ptr->EffectBPName <= 0)
		{
			return;
		}
		unit& owner = unit_man::get_unit(m_skill_param.sender_unit_idf);
		unit& target_unit = unit_man::get_unit(m_skill_param.target_unit_idf);

		static skill_proto_skill_fly_end resp;
		resp.Clear();
		resp.set_attack_unit_guid(owner.get_unit_guid().server_64);
		if (owner.get_unit_guid() != target_unit.get_unit_guid())
		{
			resp.set_target_unit_guid(target_unit.get_unit_guid().server_64);
		}
		resp.set_skill_template_id(m_skill_param.skill_template_id);
		resp.set_bullet_template_id(m_bullet_template_id);
		resp.set_effect_index(m_array_index);
		
		if (owner.get_unit_type() != e_unit_type_player)
		{
			owner.send_message_to_aoi(&resp, e_msgindex_s2c_skill_fly, true, false);
		}
		owner.send_message_to_self(&resp, e_msgindex_s2c_skill_fly);
	}

	void skill_effect::heart_tick(const int64& new_time)
	{
		if (m_life_time <= 0)
		{
			return;
		}
		if (nullptr == m_skill_bullet_template_ptr || nullptr == m_skill_template_ptr)
		{
			m_life_time = 0;
			return;
		}
		if (m_skill_bullet_template_ptr->EffectBPName <= 0 || m_is_move_effect)
		{
			effect_move(new_time);
		}
	}
	void skill_effect::effect_move(const int64& new_time)
	{
		int64 diff_time = new_time - m_move_tick_time;
		float f_diff_time = diff_time / 1000.f;
		m_move_tick_time = new_time;
		m_speed_time = m_speed_vector * f_diff_time;
		switch (m_skill_bullet_template_ptr->MoveType)
		{
		case e_skill_bullet_move_line:
		{
			effect_hurt();
			if (init_unit::is_in_distance(m_skill_param.pos, m_begin_pos, m_skill_bullet_template_ptr->MaxLength) == false)
			{
				m_life_time = -1;
			}
			m_skill_param.pos += m_speed_time;
		}
		break;
		case e_skill_bullet_move_follow:
		case e_skill_bullet_move_follow_and_hurt:
		{
			unit& unit_ref = unit_man::get_unit(m_skill_param.target_unit_idf);
			if (m_skill_bullet_template_ptr->MoveType == e_skill_bullet_move_follow_and_hurt)
			{
				effect_hurt();
			}
			else
			{
				if (init_unit::is_in_distance(m_skill_param.pos, unit_ref.get_new_map_pos().unit_location, m_begin_radius))
				{
					effect_hurt();
				}
			}
			m_skill_param.pos += m_speed_time;
			fvector diff_vector = unit_ref.get_new_map_pos().unit_location - m_skill_param.pos;
			float radian_value = diff_vector.rotator().Yaw * RADIAN;
			m_speed_vector.x = m_skill_bullet_template_ptr->Speed * cos(radian_value);
			m_speed_vector.y = m_skill_bullet_template_ptr->Speed * sin(radian_value);
			m_speed_vector.z = 0;
		}
		break;
		case e_skill_bullet_move_radian:
		{
			effect_hurt();
			m_skill_param.pos += m_speed_time;
			if (init_unit::is_in_distance(m_skill_param.pos, m_begin_pos, m_skill_bullet_template_ptr->MaxLength) == false)
			{
				m_life_time = -1;
			}
		}
		break;
		}
		m_begin_radius += m_add_radius * f_diff_time;
	}
	void skill_effect::effect_hurt()
	{
		if (nullptr == m_skill_bullet_template_ptr)
		{
			return;
		}
		unit& send_unit_ref = unit_man::get_unit(m_skill_param.sender_unit_idf);

		if (m_skill_bullet_template_ptr->EffectImmediately == e_skill_bullet_unit_one)
		{
			unit& target_unit_ref = unit_man::get_unit(m_skill_param.target_unit_idf);
			if (m_skill_template_ptr == nullptr)
			{
				return;
			}
			if (send_unit_ref.is_mission_see(m_skill_param.target_unit_idf.runtime_id) == false)
			{
				return;
			}
			if (target_unit_ref.is_mission_see(m_skill_param.sender_unit_idf.runtime_id) == false)
			{
				return;
			}
			float diff_len = m_begin_radius * 1.5;
			if (init_unit::is_in_distance(target_unit_ref.get_new_map_pos().unit_location, m_skill_param.pos, diff_len))
			{
				if (m_skill_template_ptr->ActiveType == esat_buff && send_unit_ref.get_unit_type() == e_unit_type_player)
				{
					int32 max_z = 2 * (send_unit_ref.get_unit_half_height() + target_unit_ref.get_unit_half_height());
					if (fabs(target_unit_ref.get_new_map_pos().unit_location.z - m_skill_param.pos.z) >= max_z)
					{
						return;
					}
				}
				trig_buf(m_skill_param.target_unit_idf,
					m_skill_param.pos,
					m_skill_param.skill_template_id,
					m_skill_param.is_begin,
					m_skill_param.effect_index,
					m_skill_param);
				m_life_time = -1;
			}
		}
		else
		{
			const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(send_unit_ref.get_map_ent(), m_skill_param.pos);
			if (aoi_tower_watch.empty())
			{
				return;
			}
			float diff_len = m_begin_radius * 1.5;
			unit_index_map_const_it it_end = aoi_tower_watch.end();
			for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != it_end; ++it)
			{
				unit& unit_ref = unit_man::get_unit(it->first);
				if (send_unit_ref.is_mission_see(it->first) == false)
				{
					continue;
				}
				if (unit_ref.is_mission_see(m_skill_param.sender_unit_idf.runtime_id) == false)
				{
					continue;
				}
				if (init_unit::is_in_distance(unit_ref.get_new_map_pos().unit_location, m_skill_param.pos, diff_len))
				{
					if (m_skill_template_ptr->ActiveType == esat_buff && send_unit_ref.get_unit_type() == e_unit_type_player)
					{
						int32 max_z = 2 * (send_unit_ref.get_unit_half_height() + unit_ref.get_unit_half_height());
						if (fabs(unit_ref.get_new_map_pos().unit_location.z - m_skill_param.pos.z) >= max_z)
						{
							return;
						}
					}
					unit_guid_map_it it = m_target_index_map.find(unit_ref.get_unit_guid().server_64);
					if (it == m_target_index_map.end() && m_target_index_map.size() < m_skill_bullet_template_ptr->HitNum)
					{
						trig_buf(unit_ref.get_identifier(),
							m_skill_param.pos,
							m_skill_param.skill_template_id,
							m_skill_param.is_begin,
							m_skill_param.effect_index,
							m_skill_param);
					}
				}
			}
		}
	}
	void skill_effect::skill_hurt(const use_skill_param& param)
	{
		if (nullptr == m_skill_bullet_template_ptr)
		{
			return;
		}
		unit& attack_unit = unit_man::get_unit(param.sender_unit_idf);
		unit& target_unit = unit_man::get_unit(param.target_unit_idf);
		int32 max_length = m_skill_bullet_template_ptr->MaxLength;
		if (target_unit.is_mission_see(param.sender_unit_idf.runtime_id) == false)
		{
			return;
		}
		if (attack_unit.is_mission_see(param.target_unit_idf.runtime_id) == false)
		{
			return;
		}
		max_length += attack_unit.get_unit_capsule_width() + target_unit.get_unit_capsule_width();
		max_length += (max_length >> 1);//乘大1.5倍,防止客户端出现打中,服务器判断无效的问题
		if (init_unit::is_in_distance(param.pos, target_unit.get_new_map_pos().unit_location, max_length))
		{
			unit_guid_map_it it = m_target_index_map.find(param.target_unit_idf.unit_guid.server_64);
			if (it == m_target_index_map.end())
			{
				if (m_target_index_map.size() < m_skill_bullet_template_ptr->HitNum)
				{
					trig_buf(param.target_unit_idf, m_skill_param.pos, param.skill_template_id, false, m_skill_param.effect_index, param, true);
				}
			}
			else
			{
				int32 attack_num  = it->second % 100;
				if (attack_num < m_skill_template_ptr->TargetBuff.size() / 5)
				{
					for (unit_guid_map_it begin_it = m_target_index_map.begin(); begin_it != m_target_index_map.end();)
					{
						unit_guid_map_it temp_it = begin_it++;
						int32 runtime_id = temp_it->second / 100;
						s_unit_identifier temp_identifier(it->first, runtime_id);
						unit& attack_unit = unit_man::get_unit(temp_identifier);
						if (attack_unit.is_valid() == false)
						{
							m_target_index_map.erase(temp_it);
						}
					}
					if (m_target_index_map.size() < m_skill_bullet_template_ptr->HitNum)
					{
						trig_buf(param.target_unit_idf, m_skill_param.pos, param.skill_template_id, false, m_skill_param.effect_index, param, true);
					}
				}
			}
		}
		else
		{
			//CONSOLE_INFO("skill_effect::max_length = "<< max_length);
		}
	}
	void skill_effect::end_effect()
	{
		if (nullptr == m_skill_bullet_template_ptr || nullptr == m_skill_template_ptr)
		{
			return;
		}
		switch (m_skill_bullet_template_ptr->AmmoType)
		{
		case e_ammo_type_npc:
		{

		}
		break;
		}
	}
	void skill_effect::trig_buf(
		const s_unit_identifier& target_identifier, 
		fvector& rot, 
		int32 skill_template_id,
		bool is_begin,
		int32 cast_idx, 
		const use_skill_param& skill_param, 
		bool by_client)
    {
		if (nullptr == m_skill_template_ptr)
		{
			//CONSOLE_INFO("skill_effect::trig_buf m_skill_template_ptr is null ");
			return;
		}
		unit& send_unit = unit_man::get_unit(m_skill_param.sender_unit_idf);
		if (send_unit.is_valid() == false)
		{
			//CONSOLE_INFO("skill_effect::trig_buf send_unit is null ");
			return;
		}
		int32 sender_index = m_skill_param.sender_unit_idf.runtime_id;
		int32 target_index = target_identifier.runtime_id;
		float buff_percent = 0;

		for (int32 i = 0; i < m_skill_template_ptr->BuffPrecent.size(); ++i)
		{
			int32& buff_template_id = m_skill_template_ptr->BuffPrecent[i];
			buff_item* buff_item_ptr = buff_man::get_buff_item_by_template_id(sender_index, sender_index, buff_template_id);
			if (buff_item_ptr)
			{
				buff_percent += buff_item_ptr->get_buff_percent();
			}
		}

		if (is_begin)
		{
			for (uint32 i = 0; i < m_skill_template_ptr->BeginBuff.size(); ++i)
			{
				uint32 buff_id = m_skill_template_ptr->BeginBuff[i];
				if (buff_id <= 0)
				{
					continue;
				}
				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.buff_percent = buff_percent;
				penv.buff_level = 1;
				penv.skill_target_id = m_skill_param.target_unit_idf.runtime_id;
				buff_man::add_buff_inst(sender_index, sender_index, buff_id, &penv);
			}
			for (unit_index_map_it it = m_skill_add_buff_self_map.begin(); it != m_skill_add_buff_self_map.end(); ++it)
			{
				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.buff_percent = buff_percent;
				penv.buff_level = 1;
				penv.skill_target_id = m_skill_param.target_unit_idf.runtime_id;
				buff_man::add_buff_inst(sender_index, sender_index, it->first, &penv);
			}
			return;
		}
		unit& target_unit = unit_man::get_unit(target_identifier);
		if (target_unit.is_valid() == false || target_unit.get_map_guid() != send_unit.get_map_guid())
		{
			//CONSOLE_INFO("skill_effect::trig_buf target_unit is null ");
			return;
		}
		if (sender_index == target_index)
		{
			for (uint32 i = cast_idx*k_max_buff_num_in_cast_section, j = 0;
				i < m_skill_template_ptr->SelfBuff.size() && j < k_max_buff_num_in_cast_section;
				++i, ++j)
			{
				uint32 buff_id = m_skill_template_ptr->SelfBuff[i];
				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.buff_percent = buff_percent;
				penv.by_client = by_client;
				penv.be_critical = m_skill_param.be_critical;
				penv.buff_level = 1;
				buff_man::add_buff_inst(sender_index, sender_index, buff_id, &penv);
			}
			for (unit_index_map_it it = m_skill_add_buff_self_map.begin(); it != m_skill_add_buff_self_map.end(); ++it)
			{
				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.buff_percent = buff_percent;
				penv.buff_level = 1;
				buff_man::add_buff_inst(sender_index, sender_index, it->first, &penv);
			}
		}
		if (send_unit.get_pk_community_mgr().is_skill_target(target_unit.get_array_index(), m_skill_template_ptr->TargetType))
		{
			player* target_ptr = player::cast(&target_unit);
			if (nullptr != target_ptr)
			{//打断采集状态
				target_ptr->stop_interact_with_npc();
			}
			send_unit.get_war_state_mgr().add_in_war(target_index);
			if (m_skill_template_ptr->InWar == e_skill_use_target_in_war_type_true)
			{
				target_unit.get_war_state_mgr().add_in_war(sender_index);
			}
			send_unit.trigger_unit_event(e_game_event_type_unit_hit, target_index);
			target_unit.trigger_unit_event(e_game_event_type_unit_be_hit, sender_index); 
			unit_guid_map_it it = m_target_index_map.find(target_unit.get_unit_guid().server_64);
			if (it != m_target_index_map.end())
			{
				it->second += 1;
			}
			else
			{
				m_target_index_map[target_unit.get_unit_guid().server_64] = target_unit.get_array_index() * 100 + 1;
			}

			for (uint32 i = cast_idx*k_max_buff_num_in_cast_section, j = 0;
			i < m_skill_template_ptr->TargetBuff.size() && j < k_max_buff_num_in_cast_section;
				++i, ++j)
			{
				uint32 buff_id = m_skill_template_ptr->TargetBuff[i];
				if (buff_id <= 0)
				{
					break;
				}
				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.damage_num = skill_param.damage_num;
				penv.dest_pos = skill_param.dest_pos;
				penv.buff_percent = buff_percent;
				penv.by_client = by_client;
				penv.be_critical = skill_param.be_critical;
				penv.buff_level = 1;
				penv.hit_random = skill_param.hit_random;
				buff_man::add_buff_inst(sender_index, target_index, buff_id, &penv);
			}
			for (unit_index_map_it it = m_skill_add_buff_target_map.begin(); it != m_skill_add_buff_target_map.end(); ++it)
			{
				BuffDataTemplate* buff_template_ptr = GET_TEMPLATE(BuffDataTemplate, it->first);
				if (buff_template_ptr == nullptr)
				{
					continue;
				}
				if (false == send_unit.get_pk_community_mgr().is_skill_target(target_index, eskilltarget_enemy) && buff_template_ptr->Type == e_buff_type_negative)
				{
					continue;
				}
				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.buff_percent = buff_percent;
				penv.buff_level = 1;
				buff_man::add_buff_inst(sender_index, target_index, it->first, &penv);
			}
			send_unit.trigger_unit_event(e_game_event_type_unit_bullet, target_index);
			send_unit.trigger_unit_event(e_game_event_type_unit_bullet_self, sender_index);
		}
		else
		{
			for (uint32 i = cast_idx*k_max_buff_num_in_cast_section, j = 0;
			i < m_skill_template_ptr->FriendBuff.size() && j < k_max_buff_num_in_cast_section;
				++i, ++j)
			{
				uint32 buff_id = m_skill_template_ptr->FriendBuff[i];

				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.damage_num = -skill_param.damage_num;
				penv.dest_pos = skill_param.dest_pos;
				penv.buff_percent = buff_percent;
				penv.by_client = by_client;
				penv.be_critical = skill_param.be_critical;
				penv.buff_level = 1;
				penv.hit_random = skill_param.hit_random;
				buff_man::add_buff_inst(sender_index, target_index, buff_id, &penv);
			}
			for (unit_index_map_it it = m_skill_add_buff_self_map.begin(); it != m_skill_add_buff_self_map.end(); ++it)
			{
				buff_ex_env_param penv;
				penv.skill_template_id = skill_template_id;
				penv.pos = m_skill_param.pos;
				penv.dir.yaw = m_skill_param.yaw;
				penv.be_hit = m_skill_param.be_hit;
				penv.buff_percent = buff_percent;
				penv.buff_level = 1;
				buff_man::add_buff_inst(sender_index, target_index, it->first, &penv);
			}
		}
	}
}



