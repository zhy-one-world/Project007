
#include "time.hpp"
#include "server_log.hpp"
#include "logic/skill_set.h"
#include "logic/item_set.h"
#include "template/template_manager.h"
#include "logic/unit_man.h"
#include "cache_mgr.hpp"
#include "logic/unit.hpp"
#include "logic/player.hpp"
#include "event_processer.h"
#include "system/item/item_system.h"
#include "internet/skill.pb.h"
#include "utility/init_unit.h"
#include "internet/net.pb.h"
#include "internal/skill_msg.hpp"
#include "skill_effect.h"
#include "template/SkillTemplate_S.h"

namespace faith
{
    skill_set::~skill_set()
	{
		m_unit_idf.unit_guid.clear_data();
		clear_data();
	}
	void skill_set::clear_data()
	{
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			cache_mgr::get_instance().back_skill_ptr(it->second);
		}
		m_skill_map.clear();
		for (skill_effect_map_it it = m_effect_map.begin(); it != m_effect_map.end(); ++it)
		{
			cache_mgr::get_instance().back_skill_effect_ptr(it->second);
		}
		m_effect_map.clear();
		m_skill_tick_map.clear();
		m_return_location.clear_data();
		m_sub_cd_all = 0;
		m_sub_percent_all = 0;
		m_public_time = 0;
		m_break_skill_array.clear();
	}
	void skill_set::heart_tick(const int64& new_time, const int32& tick_time)
	{
		if (m_effect_map.empty() == false)
		{
			for (skill_effect_map_it it = m_effect_map.begin(); it != m_effect_map.end();)
			{
				skill_effect_map_it it_temp = it++;
				skill_effect* effect_ref = it_temp->second;
				if (effect_ref)
				{
					effect_ref->heart_tick(new_time);
					if (new_time >= effect_ref->get_life_time())
					{

						//CONSOLE_INFO("  skill_order = " << effect_ref->get_skill_order() << " effect_index =" << effect_ref->get_effect_index());
						cache_mgr::get_instance().back_skill_effect_ptr(effect_ref);
						m_effect_map.erase(it_temp);
					}
				}
			}
		}
		for (skill_ptr_map_it it = m_skill_tick_map.begin(); it != m_skill_tick_map.end();)
		{
			skill_ptr_map_it temp_it = it++;
			skill* skill_ptr = *temp_it;
			if (skill_ptr)
			{
				skill_ptr->heart_tick(new_time, tick_time);
				if (skill_ptr->is_remove_tick())
				{
					m_skill_tick_map.erase(temp_it);
				}
			}
			else
			{
				m_skill_tick_map.erase(temp_it);
			}
		}
	}
	skill_effect* skill_set::get_skill_effect(int32 skill_order, int32 effect_index)
	{
		guid_64 effect_guid(skill_order, effect_index);
		skill_effect_map_it it = m_effect_map.find(effect_guid.server_64);
		if (it == m_effect_map.end())
		{
			return nullptr;
		}
		else
		{
			//CONSOLE_INFO("  skill_order = "<< skill_order <<" effect_index =" << effect_index);
		}
		return it->second;
	}
	void skill_set::add_skill_effect(skill_effect* skill_effect_ptr)
	{
		if (nullptr == skill_effect_ptr)
		{
			return;
		}
		guid_64 effect_guid(skill_effect_ptr->get_skill_order(), skill_effect_ptr->get_effect_index());
		//CONSOLE_INFO("  skill_order = " << skill_effect_ptr->get_skill_order() << " effect_index =" << skill_effect_ptr->get_effect_index());
		m_effect_map[effect_guid.server_64] = skill_effect_ptr;
	}
	void skill_set::skill_hurt(const use_skill_param& param)
	{
		unit& sender_unit = unit_man::get_unit(m_unit_idf);
		if (sender_unit.get_pawn_att().is_can_move() == false)
		{
			SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, param.skill_template_id);
			if(skill_template_ptr == nullptr || skill_template_ptr->CanHurtNoMove == 0)
				return;
		}
		unit& target_unit = unit_man::get_unit(param.target_unit_idf);
		if (target_unit.get_session_state() != e_session_status_in_gaming)
		{
			return;
		}
		skill_effect* skill_effect_ptr = get_skill_effect(param.skill_order, param.effect_index);
		if (skill_effect_ptr)
		{
			skill_effect_ptr->skill_hurt(param);
		}
		else
		{
			//CONSOLE_INFO("  skill_order = " << param.skill_order << " effect_index =" << param.effect_index);
		}
	}
	void skill_set::load_skill_by_db(const skill_proto_skill_save_load& load_msg)
	{
		int32 data_num = load_msg.data_ary_size() / e_skill_info_max;
		for (int32 i = 0; i < data_num; i++)
		{
			s_skill_info skill_info;
			for (int32 j =0; j < e_skill_info_max; ++j)
			{
				skill_info.data_ary[j] = load_msg.data_ary(i*e_skill_info_max + j);
			}
			if (skill_info.data_ary[e_skill_info_template_id])
			{
				skill* skill_ptr = add_skill_inst_by_info(skill_info, false);
				if (skill_ptr && skill_ptr->get_data_info(e_skill_info_use_pos) >= 0)
				{
					skill_ptr->set_passive_buff(true);
				}
			}
		}
		player& player_ref = unit_man::get_player(m_unit_idf);
		if (player_ref.is_valid())
		{
			PlayerInitTemplate* player_init_template_ptr = GET_TEMPLATE(PlayerInitTemplate, player_ref.get_unit_info(e_role_info_template_id));
			if (player_init_template_ptr)
			{
				//for (int32 i = 0; i < player_init_template_ptr->InstSkills.size(); i++)
				//{
				//	skill* skill_ptr = get_skill_inst_by_id(player_init_template_ptr->InstSkills[i]);
				//	if (nullptr == skill_ptr)
				//	{
				//		s_skill_info skill_info;
				//		if (init_unit::init_skill_data(skill_info, player_init_template_ptr->InstSkills[i], 1))
				//		{
				//			add_skill_inst_by_info(skill_info, false);
				//		}
				//	}
				//}
				for (int32 i = 0; i < player_init_template_ptr->AssistSkills.size(); i++)
				{
					skill* skill_ptr = get_skill_inst_by_id(player_init_template_ptr->AssistSkills[i]);
					if (nullptr == skill_ptr)
					{
						s_skill_info skill_info;
						if (init_unit::init_skill_data(skill_info, player_init_template_ptr->AssistSkills[i], 1))
						{
							add_skill_inst_by_info(skill_info, false);
						}
					}
				}
				for (int32 i = 0; i < GAMECONFIG->PatronSaintSkillStartId.size(); i++)
				{
					skill* skill_ptr = get_skill_inst_by_id(GAMECONFIG->PatronSaintSkillStartId[i]);
					if (nullptr == skill_ptr)
					{
						s_skill_info skill_info;
						if (init_unit::init_skill_data(skill_info, GAMECONFIG->PatronSaintSkillStartId[i], 0))
						{
							add_skill_inst_by_info(skill_info, false);
						}
					}
				}
			}
			send_skill_all();
		}
	}

	bool skill_set::save_skill_to_db(int32 save_type_ex)
	{
		player& player_ref = unit_man::get_player(m_unit_idf);
		skill_proto_skill_save_load request_save;
		int64 now_time = utility::get_tick_count();
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			it->second->save_skill_info(request_save, now_time);
		}
		player_ref.send_message_to_dp(&request_save, e_msgindex_cs2dp_save_char_skill, save_type_ex);
		return true;
	}
	void skill_set::exp_level_up(int32 cur_level) 
	{
		//已经废弃了
		//for (auto skill_iter = m_skill_insts.begin(); skill_iter != m_skill_insts.end(); ++skill_iter)
		//{
		//	auto skill_id = skill_iter->get_data_info(e_skill_info_template_id);
		//	SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_id);
		//	if (cur_level >= skill_template_ptr->LearnConditionLevel)
		//	{
		//		if ((*skill_iter).get_data_info(e_skill_info_activated) != 1)
		//		{
		//			(*skill_iter).set_data_info(e_skill_info_activated, 1);
		//			send_skill_by_inst((*skill_iter), e_skill_operation_unlock);
		//		}
		//	}
		//}
	}
	void skill_set::send_skill_one(const skill* skill_inst)
	{
		if (nullptr == skill_inst)
		{
			return;
		}
		unit& temp_unit = unit_man::get_unit(m_unit_idf);
		skill_proto_skill_info msg;
		msg.set_unit_guid(temp_unit.get_unit_guid().server_64);
		for (int32 i = 0; i < e_skill_info_max; ++i)
		{
			if (i == e_skill_info_cd_cur_time)
			{
				msg.add_data_ary(skill_inst->get_skill_cd());
			}
			else
			{
				msg.add_data_ary(skill_inst->get_data_info((e_skill_info)i));
			}
		}
		temp_unit.send_message_to_self(&msg, e_msgindex_s2c_skill_info_one);
	}
	void skill_set::send_skill_all()
	{
		unit& temp_unit = unit_man::get_unit(m_unit_idf);
		if (m_skill_map.empty())
		{
			return;
		}
		skill_proto_skill_info_all msg;
		msg.set_unit_guid(temp_unit.get_unit_guid().server_64);
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			skill* skill_inst_ref = it->second;
			skill_proto_skill_info* skill_info_msg = msg.add_skill_list();
			for (int32 j = 0; j < e_skill_info_max; ++j)
			{
				if (j == e_skill_info_cd_cur_time)
				{
					skill_info_msg->add_data_ary(skill_inst_ref->get_skill_cd());
				}
				else
				{
					skill_info_msg->add_data_ary(skill_inst_ref->get_data_info((e_skill_info)j));
				}
			}
			if (msg.ByteSize() >= packege_length_max)
			{
				temp_unit.send_message_to_self(&msg, e_msgindex_s2c_skill_info_all);
				msg.clear_skill_list();
			}
		}
		if (msg.ByteSize() > 0)
		{
			temp_unit.send_message_to_self(&msg, e_msgindex_s2c_skill_info_all);
		}
	}
	void skill_set::skill_change_msg(int32 skill_template_id, int32 pos_index)
	{
		skill* old_skill_ptr = get_skill_inst_by_pos(pos_index);
		skill* new_skill_ptr = get_skill_inst_by_id(skill_template_id);
		if (old_skill_ptr && new_skill_ptr)
		{
			old_skill_ptr->set_data_info(e_skill_info_use_pos, -1);
			old_skill_ptr->set_passive_buff(false);
		}
		if (new_skill_ptr)
		{
			new_skill_ptr->set_data_info(e_skill_info_use_pos, pos_index);
			new_skill_ptr->set_passive_buff(true);

			auto& temp_player = unit_man::get_player(m_unit_idf);
			if (temp_player.is_valid())
			{
				temp_player.get_mission_mgr().target_check(e_mission_end_type_active_skill_count);
			}
		}
	}
	void skill_set::change_skill_sub_cd_all(int32 sub_cd, bool apply)
	{
		if (apply)
		{
			m_sub_cd_all += sub_cd;
		}
		else
		{
			m_sub_cd_all -= sub_cd;
		}			
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			it->second->change_skill_fix_value(e_fix_skill_sub_cd_value, sub_cd, apply);
		}
	}
	void skill_set::change_skill_percent_cd_all(float percent_cd, bool apply)
	{
		if (apply)
		{
			m_sub_percent_all += percent_cd;
		}
		else
		{
			m_sub_percent_all -= percent_cd;
		}
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			it->second->change_skill_fix_value(e_fix_skill_sub_cd_percent, percent_cd, apply);
		}
	}
	float skill_set::get_fix_skill_value(int32 series_type, e_fix_skill fix_type)
	{
		skill* skill_ptr = get_skill_inst_by_series(series_type);
		if (skill_ptr)
		{
			 return skill_ptr->get_fix_skill_value(fix_type);
		}
		return 0;
	}
	void skill_set::change_skill_fix_value(int32 series_type, e_fix_skill fix_type, float fix_val, bool apply)
	{
		skill* skill_ptr = get_skill_inst_by_series(series_type);
		if (skill_ptr)
		{
			skill_ptr->change_skill_fix_value(fix_type, fix_val, apply);
		}
	}
	void skill_set::change_skill(int32 series_type, int32 skill_template_id, bool apply)
	{
		skill* skill_ptr = get_skill_inst_by_series(series_type);
		if (skill_ptr)
		{
			skill_ptr->change_skill(skill_template_id, apply);
		}
	}

	void skill_set::skill_add_buff(int32 series_type, int32 buff_template_id, int32 is_self, bool apply)
	{
		if (0 == series_type)
		{
			for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
			{
				it->second->skill_add_buff(buff_template_id, is_self, apply);
			}
		}
		else
		{
			skill* skill_ptr = get_skill_inst_by_series(series_type);
			if (skill_ptr)
			{
				skill_ptr->skill_add_buff(buff_template_id, is_self, apply);
			}

		}
	}
	skill* skill_set::add_skill_inst_by_id(int32 skill_template_id, bool activated)
	{
		s_skill_info skill_info;
		if (init_unit::init_skill_data(skill_info, skill_template_id, activated))
		{
			return add_skill_inst_by_info(skill_info, true);
		}
		return nullptr;
	}
	skill* skill_set::add_skill_inst_by_info(const s_skill_info& skill_data, bool is_send)
	{
		int32 skill_template_id = skill_data.data_ary[e_skill_info_template_id];
		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (nullptr == skill_template_ptr)
		{
			return nullptr;
		}

		if (skill_template_ptr->ClassType > 0)
		{
			unit& unit_ref = unit_man::get_unit(m_unit_idf.runtime_id);
			if (!unit_ref.is_valid())
			{
				return nullptr;
			}

			if (unit_ref.get_unit_type() == e_unit_type_player)
			{
				int32 class_type = unit_ref.get_unit_info(e_role_info_class_type);
				if (class_type != skill_template_ptr->ClassType)
				{
					return nullptr;
				}
			}
		}

		skill* serial_skill = get_skill_inst_by_series(skill_template_ptr->Series);
		if (serial_skill)
		{
			serial_skill->change_skill(skill_template_id, true);
			return serial_skill;
		}
		s_skill_info skill_info;
		for (int32 i = 0; i < e_skill_info_max; ++i)
		{
			skill_info.data_ary[i] = skill_data.data_ary[i];
		}
		if (skill_info.data_ary[e_skill_info_cd_cur_time] > skill_info.data_ary[e_skill_info_cd_max_time])
		{
			skill_info.data_ary[e_skill_info_cd_cur_time] = skill_info.data_ary[e_skill_info_cd_max_time];
		}
		skill* skill_inst_ref = cache_mgr::get_instance().get_skill_ptr();
		skill_inst_ref->set_owner(m_unit_idf);
		skill_inst_ref->set_skill_info(skill_info);
		skill_inst_ref->change_skill(skill_template_id, true);
		skill_inst_ref->change_skill_fix_value(e_fix_skill_sub_cd_value, m_sub_cd_all, true);
		skill_inst_ref->change_skill_fix_value(e_fix_skill_sub_cd_percent, m_sub_percent_all, true);
		m_skill_map[skill_template_ptr->Series] = skill_inst_ref;
		if (is_send)
		{
			send_skill_one(skill_inst_ref);
		}
		return m_skill_map[skill_template_ptr->Series];
	}
	skill* skill_set::get_skill_inst_by_series(int32 series)
	{
		skill_map_it it = m_skill_map.find(series);
		if (it != m_skill_map.end())
		{
			return (it->second);
		}
		return nullptr;
	}
	void skill_set::remove_skill_inst_by_series(int32 series)
	{
		skill_map_it it = m_skill_map.find(series);
		if (it != m_skill_map.end())
		{
			it->second->deactivate();
			m_skill_map.erase(it);
		}
	}
	skill*	skill_set::get_skill_inst_by_type(e_skill_type skill_type)
	{
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			if (it->second->get_data_info(e_skill_info_skill_type) == skill_type)
			{
				return it->second;
			}
		}
		return nullptr;
	}
	skill*	skill_set::get_skill_inst_by_pos(int32 skill_pos)
	{
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			if (it->second->get_data_info(e_skill_info_use_pos) == skill_pos)
			{
				return it->second;
			}
		}
		return nullptr;
	}
	skill* skill_set::get_skill_inst_by_id(const int32& skill_template_id)
	{
		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (skill_template_ptr)
		{
			return get_skill_inst_by_series(skill_template_ptr->Series);
		}
		return nullptr;
	}
	void skill_set::remove_skill_inst_by_id(int32 skill_template_id)
	{
		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (skill_template_ptr)
		{
			remove_skill_inst_by_series(skill_template_ptr->Series);
		}
	}
	void skill_set::resp_spell_cast(use_skill_param& param)
	{
		unit& sender_unit = unit_man::get_unit(m_unit_idf);
		if (sender_unit.get_pawn_att().is_can_move() == false || sender_unit.get_pawn_att().get_state_att(e_unit_attack_state_no_skill))
		{
			SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, param.skill_template_id);
			if (skill_template_ptr == nullptr || skill_template_ptr->CanUseNoMove == 0)
				return;
		}
		int32 msg_real_skill_id = param.skill_template_id;

		skill* skill_inst_ptr = get_skill_inst_by_id(msg_real_skill_id);
		if (NULL == skill_inst_ptr)
		{
			skill_inst_ptr = add_skill_inst_by_id(msg_real_skill_id, true);
		}
		if (nullptr == skill_inst_ptr)
		{
			return;
		}
		SkillTemplate* skill_template_ptr = skill_inst_ptr->get_skill_template_ptr();
		if (nullptr == skill_template_ptr)
		{
			return;
		}
		if (skill_template_ptr->ConsumeMagic > 0)
		{
			if (sender_unit.get_pawn_att().can_cut_magic(skill_template_ptr->ConsumeMagic))
			{
				sender_unit.get_pawn_att().change_magic(skill_template_ptr->ConsumeMagic);
			}
			else
			{
				return;
			}
		}
		if (skill_template_ptr->ConsumeEnergy > 0)
		{
			if (sender_unit.get_pawn_att().can_cur_energy(skill_template_ptr->ConsumeEnergy))
			{
				sender_unit.get_pawn_att().change_energy(skill_template_ptr->ConsumeEnergy);
			}
			else
			{
				return;
			}
		}
		if (sender_unit.m_cur_map_template_ptr)
		{
			if (skill_template_ptr->InvalidMapTypeArray.empty() == false)
			{
				for (int32 i = 0; i < skill_template_ptr->InvalidMapTypeArray.size(); ++i)
				{
					if (skill_template_ptr->InvalidMapTypeArray[i] == sender_unit.m_cur_map_template_ptr->Type)
					{
						return;
					}
				}
			}
			if (skill_template_ptr->ValidMapTypeArray.empty() == false)
			{
				bool is_in = false;
				for (int32 i = 0; i < skill_template_ptr->ValidMapTypeArray.size(); ++i)
				{
					if (skill_template_ptr->ValidMapTypeArray[i] == sender_unit.m_cur_map_template_ptr->Type)
					{
						is_in = true;
						break;
					}
				}
				if (is_in == false)
				{
					return;
				}
			}
		}
		if (skill_template_ptr->ActiveType == esat_zhudong)
		{
			int64 now_time = utility::get_tick_count();
			//部分非攻击技能服务器不判断攻冷 解决网络卡顿时 多个技能包同时推向服务器 导致例如上下马技能使用失败 客户端就出现了寻路卡住和骑马打仗的问题
			if((is_can_break_skill(skill_template_ptr->attribute_id) == false) && skill_template_ptr->Type != est_patron_saint && skill_template_ptr->CastType != estype_sprint
				&& skill_template_ptr->Type != est_floor_call_mount
				&& skill_template_ptr->Type != est_ride_fly_up
				&& skill_template_ptr->Type != est_ride_fly_down
				&& skill_template_ptr->Type != est_ride_call_wing
				&& skill_template_ptr->Type != est_ride_change_mount_no_anim
				&& skill_template_ptr->Type != est_ride_call_wing_no_anim
				&& skill_template_ptr->Type != est_world_sky_fly_up
				&& skill_template_ptr->Type != est_world_sky_fly_down
				&& skill_template_ptr->Type != est_world_sky_sky_to_floor
				&& skill_template_ptr->Type != est_world_wingfly_to_mountfly
				&& skill_template_ptr->Type != est_world_mountfly_to_wingfly
				&& skill_template_ptr->Type != est_world_wingfly_to_mountfly_no_anim
				&& skill_template_ptr->Type != est_bigworld_wing_fly_down
				&& skill_template_ptr->Type != est_bigworld_wing_fly_up
				)
			{
				return;
			}
			if (skill_template_ptr->Type == est_patron_saint)
			{
				skill_proto_skill_patron_saint_effect skill_effect_msg;
				skill_effect_msg.set_unit_guid(sender_unit.get_unit_guid());
				skill_effect_msg.set_skill_template_id(msg_real_skill_id);
				sender_unit.send_message_to_self(&skill_effect_msg, e_msgindex_s2c_skill_patron_saint_effect);
			}
			if (skill_template_ptr->Type == est_world_sky_sky_to_floor && sender_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) != faith::e_move_ment_mount_fly)
			{
				return;
			}

			if (skill_inst_ptr->get_skill_cd() > 0)
			{
				return;
			}
			if (skill_inst_ptr->get_next_cd() > 0 && skill_inst_ptr->get_cur_skill_id() != msg_real_skill_id)
			{
				return;
			}
			if (skill_template_ptr->NotChangePublicTime <= 0)
			{
				if (m_unit_idf.runtime_id < npc_arrary_index_begin)
				{
					m_public_time = init_unit::get_end_time(skill_template_ptr->PublicTime - 0.05f);//降低点公冷 以应对网络状况
					set_break_skill(skill_template_ptr->BreakSkillArray);
				}
			}
		}
		m_skill_tick_map.push_back(skill_inst_ptr);
		//use_skill_param param;
		param.sender_unit_idf = m_unit_idf;
		param.skill_template_id = msg_real_skill_id;
		float skill_move_length = 0.f;
		
		for (int32 i = 0; i < skill_template_ptr->MoveLength.size(); ++i)
		{
			skill_move_length += skill_template_ptr->MoveLength[i];
		}
		if (skill_move_length > 0 && !param.dest_pos.is_zero())
		{
			if (param.dest_pos.squared_distance(param.pos) - k_skill_move_square_length_error_range > skill_move_length * skill_move_length)
			{
				return;
			}
		}
		player* send_ptr = player::cast(&sender_unit);
		if (send_ptr && param.is_server == false)
		{
			send_ptr->get_meditation_mgr().break_meditation();
			send_ptr->stop_interact_with_npc();
		}
		sender_unit.get_war_state_mgr().add_in_war(param.target_unit_idf.runtime_id, skill_template_ptr->InWarTime);
		if ((skill_template_ptr->Type >= est_floor_common && skill_template_ptr->Type <= est_floor_skill_7) || skill_template_ptr->Type == est_patron_saint)
		{
			sender_unit.trigger_unit_event(e_game_event_type_unit_skill, skill_template_ptr->Series, skill_template_ptr->Type, skill_template_ptr->Series);
		}
		sender_unit.trigger_unit_event(e_game_event_type_unit_move);
		s_map_pos position = sender_unit.get_new_map_pos();
		position.unit_location.x = param.pos.x;
		position.unit_location.y = param.pos.y;
		sender_unit.set_new_map_pos(position);
		skill_inst_ptr->do_skill(param);
		skill_inst_ptr->add_maturity_num(1);
		int32 skill_cd = skill_inst_ptr->get_skill_cd();
		if (skill_cd > 0)
		{
			bool send_flag = false;
			if (send_ptr)
			{
				send_flag = true;
			}
			set_skill_cd_by_type((e_skill_type)skill_template_ptr->Type, skill_cd, send_flag);
		}
	}
	void skill_set::resp_spell_hurt(s_unit_identifier const& target_unit_idf, use_skill_param& param)
	{
		unit& sender_unit = unit_man::get_unit(m_unit_idf);
		if (sender_unit.get_pawn_att().is_can_move() == false)
		{
			CONSOLE_INFO("unit no move");
			return;
		}
		skill* skill_inst_ptr = get_skill_inst_by_id(param.skill_template_id);
		if (NULL == skill_inst_ptr)
		{
			return;
		}
		SkillTemplate* skill_template_ptr = skill_inst_ptr->get_skill_template_ptr();
		if (nullptr == skill_template_ptr)
		{
			return;
		}
		if (skill_template_ptr->ActiveType == esat_zhudong)
		{
			int64 now_time = utility::get_tick_count();
			if (is_can_break_skill(skill_template_ptr->attribute_id) == false)
			{
				return;
			}
		}
		param.sender_unit_idf = m_unit_idf;
		skill_hurt(param);
	}
	void skill_set::resp_cancel_skill(const skill_proto_skill_operation& req)
    {
//         auto iter = std::find_if(m_use_skills.begin(), m_use_skills.end(), [info_id](skill& _skill)
//         {
// 			return _skill.get_info_id() == req.info_id();
//         });
//         if (iter != m_use_skills.end())
//         {
// 			use_skill_param param;
// 			param.attacker_uid.A = req.src_unit_id(0);
// 			param.attacker_uid.B = req.src_unit_id(1);
// 			param.target_uid.A = req.target_id(0);
// 			param.target_uid.B = req.target_id(1);
// 			param.info_id = req.info_id();
// 			param.skill_guid.A = iter->id(0);
// 			param.skill_guid.B = iter->id(1);
// 			if (req.postion_size() == 3)
// 			{
// 				param.pos.x = req.postion(0);
// 				param.pos.y = req.postion(1);
// 				param.pos.z = req.postion(2);
// 			}
// 			if (req.dir_size() == 3)
// 			{
// 				param.dir.roll = req.dir(0);
// 				param.dir.pitch = req.dir(1);
// 				param.dir.yaw = req.dir(2);
// 			}
// 			param.time_stamp = req.use_skill_time_stamp();
// 			param.use_skill_flag = req.use_skill_flag();
// 			param.is_force = req.bforce();
// 			iter->cancel(param);
//             m_use_skills.erase(iter);
//         }
    }

	void skill_set::resp_learn_skill(const skill_proto_skill_operation& req)
	{
		ZoneScoped;
		
		int32 skill_template_id = req.skill_template_id();
		int32 up_level = 1;

		auto skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (nullptr == skill_template_ptr)
		{
			CONSOLE_ERROR("skill_template_ptr is null, skill_template_id:{}", skill_template_id);
			return;
		}
		skill* up_skill_inst_ptr = get_skill_inst_by_id(skill_template_id);
		if (nullptr != up_skill_inst_ptr)
		{
			CONSOLE_ERROR("up_skill_inst_ptr is have, skill_template_id:{}", skill_template_id);
			return;
		}

		player& player_ref = unit_man::get_player(m_unit_idf.runtime_id);
		if (player_ref.is_valid() == false)
		{
			CONSOLE_ERROR("player_ref is null, skill_template_id:{}", skill_template_id);
			return;
		}
		if (player_ref.get_unit_info(e_role_info_exp_level) < skill_template_ptr->LearnConditionLevel)
		{//等级不足
			CONSOLE_ERROR("skill_set::resp_learn_skill, level not enough, skill_template_id:{} player_level:{} need_level:{}", skill_template_id, player_ref.get_unit_info(e_role_info_exp_level), skill_template_ptr->LearnConditionLevel);
			return;
		}

		//这个版本去除熟练度升级的条件,以后应该要回复
		//if (skill_template_ptr->MaxMasteryNum > skill_inst_ptr->get_data_info(e_skill_info_maturity))
		//{//熟练度不满
		if (skill_template_ptr->LearnConfitionMoney.size() % 2 != 0)
		{
			CONSOLE_ERROR("LearnConfitionMoney error, skill_template_id:{} size:{}", skill_template_id, skill_template_ptr->LearnConfitionMoney.size());
			return;
		}
		if (skill_template_ptr->LearnConfitionItem.size() % 2 != 0)
		{
			CONSOLE_ERROR("LearnConfitionItem error, skill_template_id:{} size:{}", skill_template_id, skill_template_ptr->LearnConfitionItem.size());
			return;
		}
		for (int32 i = 0; i < skill_template_ptr->LearnConfitionMoney.size(); i += 2)
		{//钱不足
			int32 need_money_id = skill_template_ptr->LearnConfitionMoney[i];
			int32 need_money_num = skill_template_ptr->LearnConfitionMoney[i + 1];
			if (player_ref.can_cut_money((e_money_type)need_money_id, need_money_num) == false)
			{
				CONSOLE_ERROR("money not enough, skill_template_id:{} need_money_id:{} need_money_num:{}", skill_template_id, need_money_id, need_money_num);
				return;
			}
		}
		for (int32 i = 0; i < skill_template_ptr->LearnConfitionItem.size(); i += 2)
		{//物品不足
			int32 need_item_id = skill_template_ptr->LearnConfitionItem[i];
			int32 need_item_num = skill_template_ptr->LearnConfitionItem[i + 1];
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, need_item_id, need_item_num) == false)
			{//物品不足
				CONSOLE_ERROR("item not enough, skill_template_id:{} need_item_id:{} need_item_num:{}", skill_template_id, need_item_id, need_item_num);
				return;
			}
		}
		
		for (int32 i = 0; i < skill_template_ptr->LearnConfitionMoney.size(); i += 2)
		{
			player_ref.cut_money((e_money_type)skill_template_ptr->LearnConfitionMoney[i], skill_template_ptr->LearnConfitionMoney[i + 1], e_server_log_cut_money_learn_skill);
		}
		for (int32 i = 0; i < skill_template_ptr->LearnConfitionItem.size(); i += 2)
		{
			int32 need_item_id = skill_template_ptr->LearnConfitionItem[i];
			int32 need_item_num = skill_template_ptr->LearnConfitionItem[i + 1];
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, need_item_id, need_item_num);
		}

		unlock_skill(skill_template_id, up_level);
		return;
	}
	void skill_set::unlock_skill(const int32& skill_template_id, const int32& up_level)
	{
		ZoneScoped;

		if (skill_template_id <= 0 || up_level <= 0)
		{
			CONSOLE_ERROR("skill_set::unlock_skill, skill_template_id:{} up_level:{}", skill_template_id, up_level);
			return;
		}
		skill* skill_inst_ptr = get_skill_inst_by_id(skill_template_id);
		if (NULL == skill_inst_ptr)
		{
			skill_inst_ptr = add_skill_inst_by_id(skill_template_id, true);
		}
		skill_inst_ptr->set_data_info(e_skill_info_level, up_level);
		send_skill_one(skill_inst_ptr);
	}

	void skill_set::resp_upgrade_skill(const skill_proto_skill_operation& req)
	{
		ZoneScoped;
		player& player_ref = unit_man::get_player(m_unit_idf.runtime_id);
		if (player_ref.is_valid() == false)
		{
			CONSOLE_ERROR("player_ref is null");
			return;
		}
		auto skill_level = player_ref.get_logic_data(e_role_logic_info_skill_level);
		if (skill_level >= GAMECONFIG->UpLevelSkillCost.size() - 1)
		{
			CONSOLE_ERROR("skill_level is max skill_level:{} max:{}", skill_level, GAMECONFIG->UpLevelSkillCost.size() - 1);
			return;
		}
		auto need_money_id = GAMECONFIG->UpLevelSkillCost[0];
		auto need_money_num = GAMECONFIG->UpLevelSkillCost[skill_level + 1];
		if (player_ref.can_cut_money((e_money_type)need_money_id, need_money_num) == false)
		{
			CONSOLE_ERROR("money not enough, skill_level:{} need_money_id:{} need_money_num:{}", skill_level, need_money_id, need_money_num);
			return;
		}
		player_ref.cut_money((e_money_type)need_money_id, need_money_num, e_server_log_cut_money_upgrade_skill);
		player_ref.set_logic_data(e_role_logic_info_skill_level, skill_level + 1);
		player_ref.send_logic_one(e_role_logic_info_skill_level);
		player_ref.get_mission_mgr().target_check(e_mission_end_type_active_skill_level);
	}
	void skill_set::upgrade_skill_achievement()
	{
		player& player_ref = unit_man::get_player(m_unit_idf);

		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			player_ref.get_achievement_mgr().skill_level_up(it->second);
		}
	}

	void	skill_set::all_skill_level_count_to_achievement()
	{
		int32 skill_count_num = 0;
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			skill_count_num += it->second->get_data_info(e_skill_info_level);
		}
		player& player_ref = unit_man::get_player(m_unit_idf);
		player_ref.get_achievement_mgr().skill_level_count(skill_count_num);
	}

	void skill_set::activate_skill(int32 skill_template_id, bool is_activate)
	{
		skill* skill_ptr = get_skill_inst_by_id(skill_template_id);
		if (nullptr == skill_ptr)
		{
			return;
		}
		int32 old_state = skill_ptr->get_data_info(e_skill_info_activated);
		if (is_activate)
		{
			if (old_state == 0)
			{
				skill_ptr->set_data_info(e_skill_info_activated, 1);
				send_skill_one(skill_ptr);
				player& player_ref = unit_man::get_player(m_unit_idf);
				if (player_ref.is_valid())
				{
					player_ref.get_mission_mgr().target_check(e_mission_end_type_learning_skill);
				}
			}
		}
		else
		{
			if (old_state == 1)
			{
				skill_ptr->set_data_info(e_skill_info_activated, 0);
				skill_ptr->deactivate();
				send_skill_one(skill_ptr);
			}
		}
	}

	void skill_set::relieve_stop_time(const int32& skill_template_id)
	{
		skill* skill_ptr = get_skill_inst_by_id(skill_template_id);
		if (nullptr == skill_ptr)
		{
			return;
		}
		skill_ptr->relieve_stop_time();
	}
	bool skill_set::gm_set_all_career_skill_level( int32 new_level)
	{
		player& player_ref = unit_man::get_player(m_unit_idf);
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			int32 skill_template_id = it->second->get_skill_template_id();
			unlock_skill(skill_template_id, new_level);
			player_ref.get_achievement_mgr().skill_level_up(it->second);
		}
		all_skill_level_count_to_achievement();
		return true;
	}

	bool skill_set::gm_reset_all_career_skill_cd()
	{
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			it->second->set_skill_cd(0);
		}
		send_skill_all();
		return true;
	}

	bool skill_set::set_skill_cd_by_type(e_skill_type skill_type, int32 skill_cd, bool is_send)
	{
		if (skill_type < est_floor_common || skill_type >= e_skill_type_max)
		{
			return false;
		}
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			if (it->second->get_data_info(e_skill_info_skill_type) == skill_type)
			{
				it->second->set_skill_cd(skill_cd);
				if (is_send)
				{
					send_skill_one(it->second);
				}
			}
		}
		return true;
	}

	bool skill_set::is_can_break_skill(int32 skill_template_id)
	{
		int64 now_time = utility::get_tick_count();
		if (m_public_time < now_time)
		{
			return true;
		}
		for (int32 i = 0; i < m_break_skill_array.size(); ++i)
		{
			if (m_break_skill_array[i] == skill_template_id)
			{
				//走到这里是被主动技能中断的 所以清除不可打断时间
				m_public_time = 0;
				m_break_skill_array.clear();
				return true;
			}
		}
		return false;
	}

	void skill_set::set_break_skill(std::vector<int32> skill_array)
	{
		m_break_skill_array.clear();
		m_break_skill_array = skill_array;
	}

	bool skill_set::activate_all_skill()
	{
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			skill* skill_inst_ref = it->second;
			skill_inst_ref->set_data_info(e_skill_info_activated, 1);
		}			
		player& player_ref = unit_man::get_player(m_unit_idf);
		if (player_ref.is_valid())
		{
			player_ref.get_mission_mgr().target_check(e_mission_end_type_learning_skill);
		}
		send_skill_all();
		return true;
	}

	void skill_set::set_fake_skill_info(s_skill_info * play_skill_info, int32 max_num)
	{
		int32 set_index = 0;
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			if (set_index < max_num)
			{
				skill* skill_inst_ref = it->second;
				if (nullptr == skill_inst_ref)
				{
					continue;
				}
				play_skill_info[set_index] = skill_inst_ref->get_data_all();
				set_index++;
			}
		}
	}

	int32 skill_set::get_skill_equip_num()
	{
		int32 num = 0;
		for (skill_map_it it = m_skill_map.begin(); it != m_skill_map.end(); ++it)
		{
			skill* skill_inst_ref = it->second;
			if (nullptr == skill_inst_ref)
			{
				continue;
			}
			if (skill_inst_ref->get_data_info(e_skill_info_use_pos) > 0 && skill_inst_ref->get_data_info(e_skill_info_use_pos) <= est_floor_skill_7)
			{
				num++;
			}
		}
		return num;
	}
}

