/********************************************************************
	file base:	skill_work
	file ext:	cpp
	author:		wenhongming
	purpose:	
*********************************************************************/

#include "time.hpp"
#include "server_log.hpp"
#include "skill_work.h"
#include <algorithm>
#include "logic/type_def.hpp"
#include "logic/player.hpp"
#include "logic/world_cs.h"
#include "logic/unit_man.h"
#include "pk_community_def.hpp"
#include "template/template_manager.h"
#include "logic/buff_man.h"
#include "guide_def.hpp"
#include "logic/skill_manager.h"
#include "cache_mgr.hpp"
#include "logic/skill_set.h"
#include "internet/skill.pb.h"
#include "internet/net.pb.h"
#include "skill_effect.h"
#include "template/SkillTemplate_S.h"

namespace hld
{
	skill_work::skill_work()
	{
		clear_data();
	}

	skill_work::~skill_work()
	{

	}
	bool skill_work::is_remove_tick()
	{
		if (m_end_time > 0)
		{
			return false;
		}
		return true;
	}
	void skill_work::heart_tick(const int64& new_time, const int32& tick_time)
	{
		skill_move(new_time);
		if (new_time >= m_refresh_time)
		{
			skill_operate();
		}
		if (m_end_time> 0 && new_time > m_end_time)
		{
			m_end_time = 0;
		}
	}
	void skill_work::add_effect(const use_skill_param& param, const unit_index_map& skill_add_buff_target_map, const unit_index_map& skill_add_buff_self_map, bool send_effect, bool is_move_effect)
	{
		if (param.effect_index < 0 || param.effect_index >= skill_effect_max)
		{
			return;
		}
		skill_set& skill_set_ref = skill_manager::get_skill_set(param.sender_unit_idf.runtime_id);
		skill_effect* skill_effect_ptr = skill_set_ref.get_skill_effect(param.skill_order, param.effect_index);
		if (skill_effect_ptr)
		{
			return;
		}

		skill_effect_ptr = cache_mgr::get_instance().get_skill_effect_ptr();
		skill_effect_ptr->set_effect_index(param.effect_index);
		skill_effect_ptr->init_effect_data(param, skill_add_buff_target_map, skill_add_buff_self_map, param.effect_index, is_move_effect);
		if (false == param.is_begin)
		{
			skill_set_ref.add_skill_effect(skill_effect_ptr);
			if (send_effect)
			{
				skill_effect_ptr->send_effect_fly();
			}
		}
		else
		{
			cache_mgr::get_instance().back_skill_effect_ptr(skill_effect_ptr);
		}
	}
	void skill_work::skill_move(const int64& new_time)
	{
		if (m_is_skill_move == false || new_time < m_move_tick_time || (m_unit_idf.runtime_id < npc_arrary_index_begin && m_skill_template_ptr->UseConfig == 0))
		{
			if (m_skill_template_ptr->UseConfig == 0)
			{
				player& player_ref = unit_man::get_player(m_unit_idf);
				if (player_ref.is_valid())
				{
					int32 max_move = skill_move_max;
					if (m_skill_template_ptr->MoveLength.size() > 0)
					{
						max_move = skill_move_max > m_skill_template_ptr->MoveLength[0] ? skill_move_max : m_skill_template_ptr->MoveLength[0];
					}
					int32 cur_speed = player_ref.get_pawn_att().get_move_speed();
					max_move = cur_speed > max_move ? cur_speed : max_move;
					player_ref.get_pawn_att().set_move_speed(max_move);
				}
			}
			return;
		}
		if (m_skill_template_ptr->MoveDuration.size() > 0 && (m_move_index < m_skill_template_ptr->MoveDuration.size()) )
		{
			m_move_tick_time = init_unit::get_end_time(m_skill_template_ptr->MoveDuration[m_move_index]);
		}
		
		if (m_skill_template_ptr->MoveLength.size() > 0 && (m_move_index < m_skill_template_ptr->MoveLength.size()))
		{
			if (m_skill_template_ptr->MoveLength[m_move_index] != 0)
			{
				unit& target_unit_ref = unit_man::get_unit(m_skill_param.target_unit_idf);
				unit& unit_ref = unit_man::get_unit(m_unit_idf);
				if (new_time == 1 && target_unit_ref.is_valid())
				{
					m_move_location = target_unit_ref.get_new_map_pos().unit_location;
				}
				if (m_move_location.is_valid() && m_skill_template_ptr->NeedTarget)
				{
					fvector sub_pos = m_move_location - unit_ref.get_new_map_pos().unit_location;
					float move_length = sub_pos.length_2d();
					/*			float move_yaw = unit_ref.get_new_map_pos().unit_rotation.yaw;*/
					if (move_length > m_skill_template_ptr->LengthCorrection)
					{
						if (move_length > m_skill_template_ptr->MoveLength[m_move_index])
						{
							unit_ref.get_move_mgr().start(m_skill_template_ptr->MoveLength[m_move_index], m_skill_template_ptr->MoveDuration[m_move_index], m_move_yaw, false, m_is_not_sync_rotation, m_is_client_driver);
						}
						else
						{
							unit_ref.get_move_mgr().start(move_length, m_skill_template_ptr->MoveDuration[m_move_index], m_move_yaw, false, m_is_not_sync_rotation, m_is_client_driver);
						}
					}
				}
				else
				{
					unit_ref.get_move_mgr().start(m_skill_template_ptr->MoveLength[m_move_index], m_skill_template_ptr->MoveDuration[m_move_index], m_move_yaw, true, m_is_not_sync_rotation, m_is_client_driver, true, false);
				}
			}
		}
		m_move_index++;
		if (m_move_index >= m_skill_template_ptr->MoveLength.size())
		{
			m_is_skill_move = false;
			return;
		}
	}
	float skill_work::get_skill_cast_start_time()
	{
		float start_time = 0.0f;
		if (m_effect_index >= m_skill_template_ptr->CastTimeAxle.size())
		{
			return start_time;
		}
		start_time = m_skill_template_ptr->CastTimeAxle[m_effect_index];
		start_time -= 0.05f;
		if (start_time < 0)
		{
			start_time = 0.0f;
		}
		return start_time;
	}
	void skill_work::skill_cast()
	{
		if (m_cast_time_axle_len == 0)
		{
			return;
		}
		m_skill_state = e_skill_state::cast;
		m_effect_index = 0;
		m_effect_begin_time = utility::get_tick_count();
		if (m_skill_template_ptr->CastTimeAxle.size() > 0)
		{
			m_refresh_time = init_unit::get_end_time(get_skill_cast_start_time());
			m_effect_index++;
		}
		m_is_skill_move = true;
		m_move_tick_time = 0;
		m_move_location.clear_data();
		skill_move(1);
		skill_operate();
	}
	void skill_work::skill_operate()
	{
		switch (m_skill_state)
		{
		case e_skill_state::spell:
		{
			send_skill_cast(true);
			unit& unit_ref = unit_man::get_unit(m_unit_idf);
			m_move_yaw = unit_ref.get_new_map_pos().unit_rotation.yaw;
			skill_cast();
		}
		break;
		case e_skill_state::cast:
		{
			if (m_skill_param.is_server)
			{
				send_effect();
			}
			else
			{
				for (int32 i = m_effect_index - 1; i < m_cast_time_axle_len; i++)
				{
					send_effect();
				}
			}
		}
		break;
		default:
			break;
		}
	}
	void skill_work::send_skill_do(e_skill_operation_type operation_type, bool send_self)
	{
		unit& owner = unit_man::get_unit(m_unit_idf);
		unit& target_unit = unit_man::get_unit(m_skill_param.target_unit_idf);
		static skill_proto_skill_operation_end resp;
		resp.Clear();
		resp.set_operation_type(operation_type);
		resp.set_attack_unit_guid(owner.get_unit_guid().server_64);
		if (owner.get_unit_guid() != target_unit.get_unit_guid())
		{
			resp.set_target_unit_guid(target_unit.get_unit_guid().server_64);
		}
		resp.set_skill_template_id(m_skill_param.skill_template_id);
		resp.add_postion(m_skill_param.pos.x);
		resp.add_postion(m_skill_param.pos.y);
		resp.add_postion(m_skill_param.pos.z);
		resp.set_yaw(m_skill_param.yaw);
		if (operation_type != e_skill_operation_cast)
		{
			owner.send_message_to_aoi(&resp, e_msgindex_s2c_skill_operation, send_self);
		}
		else
		{
			owner.send_message_to_aoi(&resp, e_msgindex_s2c_skill_operation, true);
		}
	}
	void skill_work::send_skill_spell(bool send_self)
	{
		unit& owner = unit_man::get_unit(m_unit_idf);
		unit& target_unit = unit_man::get_unit(m_skill_param.target_unit_idf);
		static skill_proto_skill_spell_end resp;
		resp.Clear();
		resp.set_attack_unit_guid(owner.get_unit_guid().server_64);
		resp.set_skill_template_id(m_skill_param.skill_template_id);
		resp.set_yaw(m_skill_param.yaw);
		owner.send_message_to_aoi(&resp, e_msgindex_s2c_skill_spell, send_self);
	}
	void skill_work::send_skill_cast(bool send_self)
	{
		if (nullptr == m_skill_template_ptr || m_skill_template_ptr->ResourceId <= 0)
		{
			return;
		}
		unit& owner = unit_man::get_unit(m_unit_idf);
		unit& target_unit = unit_man::get_unit(m_skill_param.target_unit_idf);
		static skill_proto_skill_cast_end resp;
		resp.Clear();
		resp.set_attack_unit_guid(owner.get_unit_guid().server_64);
		if (owner.get_unit_guid() != target_unit.get_unit_guid())
		{
			resp.set_target_unit_guid(target_unit.get_unit_guid().server_64);
		}
		resp.set_skill_template_id(m_skill_param.skill_template_id);
		resp.add_postion(m_skill_param.pos.x);
		resp.add_postion(m_skill_param.pos.y);
		resp.add_postion(m_skill_param.pos.z);
		resp.set_yaw(m_skill_param.yaw);
		owner.send_message_to_aoi(&resp, e_msgindex_s2c_skill_cast, send_self, true);
	}
	void skill_work::send_effect()
	{
		player& player_ref = unit_man::get_player(m_unit_idf);
		bool is_send_effect = false;
		bool is_move_effect = false;
		if (m_skill_template_ptr->ActiveType == esat_buff || player_ref.is_valid() == false || player_ref.get_is_robot())
		{
			is_send_effect = true;
			is_move_effect = true;
		}
		if (m_effect_index < m_cast_time_axle_len)
		{
			m_skill_param.effect_index = m_effect_index - 1;
			add_effect(m_skill_param, m_skill_add_buff_target_map, m_skill_add_buff_self_map, is_send_effect, is_move_effect);
			if (m_skill_template_ptr->CastTimeAxle.size() > 0)
			{
				int32 add_time = m_skill_template_ptr->CastTimeAxle[m_effect_index] * 1000;
				m_refresh_time = add_time + m_effect_begin_time;
				m_effect_index++;
				if (add_time == 0)
				{
					send_effect();
				}
			}
		}
		else if (m_effect_index == m_cast_time_axle_len)
		{
			m_skill_param.effect_index = m_effect_index - 1;
			add_effect(m_skill_param, m_skill_add_buff_target_map, m_skill_add_buff_self_map, is_send_effect, is_move_effect);
			m_effect_index++;
		}
		else
		{
			m_skill_state = e_skill_state::none;
		}
	}
	float skill_work::get_end_time()
	{
		float end_time =  m_skill_template_ptr->SpellTime + m_skill_template_ptr->ChannelTime;
		int32 len = m_skill_template_ptr->CastTimeAxle.size();
		if (len <= 1)
		{
			return end_time;
		}
		float cast_time_max = m_skill_template_ptr->CastTimeAxle[len - 1];
		if (cast_time_max < end_time)
		{
			return end_time;
		}
		return cast_time_max + 0.5f;
	}
	void skill_work::init_skill_data()
	{
		m_skill_state = e_skill_state::none;
		m_effect_index = 0;
		m_effect_begin_time = utility::get_tick_count();
		m_end_time = init_unit::get_end_time(get_end_time());
		//m_end_time = init_unit::get_end_time(m_skill_template_ptr->SpellTime + m_skill_template_ptr->ChannelTime);
		m_refresh_time = 0;
		m_skill_param.clear_data();
		m_move_index = 0;
		m_move_yaw = 0;
		m_is_not_sync_rotation = false;
		m_is_client_driver = false;
	}
	void skill_work::clear_data()
	{
		m_skill_state = e_skill_state::none;
		m_skill_template_id = 0;
		m_cast_time_axle_len = 0;
		m_move_yaw = 0;
		m_is_not_sync_rotation = 0;
		m_is_client_driver = 0;
		m_effect_index = 0;
		m_effect_begin_time = 0;
		m_refresh_time = 0;
		m_unit_idf.clear_data();
		m_skill_param.clear_data();
		m_skill_add_buff_self_map.clear();
		m_skill_add_buff_target_map.clear();
		m_skill_template_ptr = nullptr;
		m_is_skill_move = false;
		m_end_time = 0;
		m_move_tick_time = 0;
		m_move_length = 0;
		m_move_index = 0;
		m_move_location.clear_data();
	}
	void skill_work::do_skill(const use_skill_param& param, const unit_index_map& skill_add_buff_target_map, const unit_index_map& skill_add_buff_self_map)
	{
		m_skill_template_ptr = GET_TEMPLATE(SkillTemplate, param.skill_template_id);
		if (nullptr == m_skill_template_ptr)
		{
			return;
		}
		m_cast_time_axle_len = m_skill_template_ptr->CastTimeAxle.size();
		init_skill_data();
		m_skill_param = param;
		m_skill_add_buff_self_map = skill_add_buff_self_map;
		m_skill_add_buff_target_map = skill_add_buff_target_map;
		if (m_skill_template_ptr->BeginBuff.size() > 0)
		{
			if (m_skill_template_ptr->BeginBuff[0] > 0)
			{
				use_skill_param self_param = m_skill_param;
				self_param.target_unit_idf = self_param.sender_unit_idf;
				self_param.is_begin = true;
				self_param.effect_index = 0;
				m_skill_state = e_skill_state::begin;
				add_effect(self_param, m_skill_add_buff_target_map, m_skill_add_buff_self_map, true, false);
			}
		}
		if (m_skill_template_ptr->SpellTime > 0)
		{
			m_refresh_time = init_unit::get_end_time(m_skill_template_ptr->SpellTime);
			m_skill_state = e_skill_state::spell;
			send_skill_spell(true);
		}
		else
		{
			if (m_skill_template_ptr->ActiveType == esat_zhudong)
			{
				send_skill_cast(false);
			}
			else
			{
				send_skill_cast(true);
			}
			if (m_skill_template_ptr->IsNotSyncRotation == 0)
			{
				m_is_not_sync_rotation = false;
			}
			else
			{
				m_is_not_sync_rotation = true;
			}
			if (m_skill_template_ptr->IsClientDriver == 0)
			{
				m_is_client_driver = false;
			}
			else
			{
				if (m_skill_template_ptr->ActiveType == esat_zhudong)
				{
					m_is_client_driver = false;
				}
				else
				{
					m_is_client_driver = true;
				}
			}
			m_move_yaw = param.yaw;
			skill_cast();
		}
	}
	void skill_work::relieve_stop_time()
	{
		m_refresh_time = init_unit::get_end_time(1);
	}
}



