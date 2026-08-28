/********************************************************************
	file base:	skill
	file ext:	cpp
	author:		wenhongming
	purpose:	
*********************************************************************/

#include "time.hpp"
#include "server_log.hpp"
#include "skill.h"
#include <algorithm>
#include "logic/type_def.hpp"
#include "logic/player.hpp"
#include "logic/world_cs.h"
#include "logic/unit_man.h"
#include "pk_community_def.hpp"
#include "template/template_manager.h"
#include "logic/buff_man.h"
#include "guide_def.hpp"
#include "aoi/aoi_system.h"
#include "internet/skill.pb.h"
#include "internet/net.pb.h"
#include "template/SkillTemplate_S.h"

namespace hld
{
	skill::skill()
	{
		clear_data();
	}

	skill::~skill()
	{

	}
	void skill::heart_tick(const int64& new_time, const int32& tick_time)
	{
		m_skill_work.heart_tick(new_time, tick_time);
		unit& unit_ref = unit_man::get_unit(m_unit_idf);
		if (unit_ref.is_valid() && unit_ref.get_pawn_att().get_state_att(e_unit_attack_state_lock_cd))
		{
			m_skill_cd += tick_time;
		}
		if (m_next_skill_cd > 0 && m_next_skill_cd < new_time)
		{
			m_next_skill_cd = 0;
			if (m_skill_template_ptr)
			{
				SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, m_skill_template_ptr->BeginSkill);
				if (skill_template_ptr)
				{
					m_cur_skill_id = m_skill_template_ptr->BeginSkill;
					m_skill_template_ptr = skill_template_ptr;
					int32 max_time = get_data_info(e_skill_info_cd_max_time);
					int32 sub_time = get_fix_skill_value(e_fix_skill_sub_cd_value);
					float percent_cd = get_fix_skill_value(e_fix_skill_sub_cd_percent);
					int32 end_time = (max_time - sub_time * 1000) * (1 - percent_cd) - 100;//降低点公冷 以应对网络状况
					set_skill_cd(end_time);
				}
			}
		}
	}
	void skill::clear_data()
	{
		m_skill_work.clear_data();
		m_skill_cd = 0;
		m_next_skill_cd = 0;
		m_skill_add_buff_target_map.clear();
		m_skill_add_buff_self_map.clear();
		m_fix_info.clear_data();
		m_back_location.clear_data();
		m_skill_info.clear_data();
		m_unit_idf.clear_data();
		m_skill_template_ptr = nullptr;
		m_change_skill_list.clear();
		m_cur_skill_id = 0;
		m_begin_skill_id = 0;
	}
	float skill::get_fix_skill_value(e_fix_skill fix_type)
	{
		if (fix_type >= e_fix_skill_max)
		{
			return 0;
		}
		return m_fix_info.data[fix_type];
	}
	void skill::change_skill_fix_value(e_fix_skill fix_type, float fix_val, bool apply)
	{
		if (apply)
		{
			m_fix_info.data[fix_type] += fix_val;
		}
		else
		{
			m_fix_info.data[fix_type] -= fix_val;
		}
		send_skill_fixed_param();
	}
	void skill::send_skill_fixed_param()
	{
		player& player_ref = unit_man::get_player(m_unit_idf);
		if (false == player_ref.is_valid())
		{
			return;
		}
		skill_proto_skill_fixed_param skill_fixed_param_msg;
		skill_fixed_param_msg.set_skill_template_id(m_skill_info.data_ary[e_skill_info_template_id]);

		for (int32 idx = 0; idx < e_fix_skill_max; idx++)
		{
			skill_fixed_param_msg.add_fixed_params(m_fix_info.data[idx]);
		}

		player_ref.send_message_to_self(&skill_fixed_param_msg, e_msgindex_s2c_skill_sync_fixed_param);
	}
	void skill::skill_add_buff(int32 buff_template_id, int32 is_self, bool apply)
	{
		if (is_self)
		{
			unit_index_map_it it = m_skill_add_buff_self_map.find(buff_template_id);
			if (apply)
			{
				if (it == m_skill_add_buff_self_map.end())
				{
					m_skill_add_buff_self_map[buff_template_id] = 1;
				}
				else
				{
					it->second += 1;
				}
			}
			else
			{
				m_skill_add_buff_self_map[buff_template_id] -= 1;
				if (it != m_skill_add_buff_self_map.end())
				{
					it->second -= 1;
					if (it->second <= 0)
					{
						m_skill_add_buff_self_map.erase(it);
					}
				}
			}
		}
		else
		{
			unit_index_map_it it = m_skill_add_buff_target_map.find(buff_template_id);
			if (apply)
			{
				if (it == m_skill_add_buff_target_map.end())
				{
					m_skill_add_buff_target_map[buff_template_id] = 1;
				}
				else
				{
					it->second += 1;
				}
			}
			else
			{
				m_skill_add_buff_target_map[buff_template_id] -= 1;
				if (it != m_skill_add_buff_target_map.end())
				{
					it->second -= 1;
					if (it->second <= 0)
					{
						m_skill_add_buff_target_map.erase(it);
					}
				}
			}
		}
	}
	int64 skill::get_skill_cd() const
	{
		return m_skill_cd - utility::get_tick_count();
	}
	void skill::set_skill_cd(const int32& skill_cd)
	{
		m_skill_cd = skill_cd + utility::get_tick_count();
	}
	void skill::set_skill_info(const s_skill_info& skill_info)
	{
		m_skill_info = skill_info;
		m_skill_cd = skill_info.data_ary[e_skill_info_cd_cur_time] + utility::get_tick_count();
	}
	void skill::add_maturity_num(int32 maturity_num)
	{
		if (m_skill_template_ptr)
		{
			int32 old_skill_master = get_data_info(e_skill_info_maturity);
			old_skill_master += maturity_num;
			old_skill_master = old_skill_master > m_skill_template_ptr->MaxMasteryNum ? m_skill_template_ptr->MaxMasteryNum : old_skill_master;
			set_data_info(e_skill_info_maturity, old_skill_master);
		}
	}
	int32 skill::get_skill_series()
	{
		if (nullptr == m_skill_template_ptr)
		{
			return 0;
		}
		return m_skill_template_ptr->Series;
	}
	bool skill::is_remove_tick()
	{
		return m_skill_work.is_remove_tick() && m_next_skill_cd == 0;
	}
	void skill::do_skill(const use_skill_param& param)
	{
		m_skill_work.do_skill(param, m_skill_add_buff_target_map, m_skill_add_buff_self_map);
		if (m_skill_template_ptr)
		{
			if (m_skill_template_ptr->NextSkill)
			{
				SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, m_skill_template_ptr->NextSkill);
				if (skill_template_ptr)
				{
					m_cur_skill_id = m_skill_template_ptr->NextSkill;
					m_skill_template_ptr = skill_template_ptr;
					if (m_skill_template_ptr->NextSkillTime < 0.01f)
					{
						int32 max_time = get_data_info(e_skill_info_cd_max_time);
						int32 sub_time = get_fix_skill_value(e_fix_skill_sub_cd_value);
						float percent_cd = get_fix_skill_value(e_fix_skill_sub_cd_percent);
						int32 end_time = (max_time - sub_time * 1000) * (1 - percent_cd) - 200;//降低点公冷 以应对网络状况
						set_skill_cd(end_time);
						m_next_skill_cd = 0;
					}
					else
					{
						m_next_skill_cd = init_unit::get_end_time(m_skill_template_ptr->NextSkillTime);
					}
				}
			}
			else
			{
				int32 max_time = get_data_info(e_skill_info_cd_max_time);
				int32 sub_time = get_fix_skill_value(e_fix_skill_sub_cd_value);
				float percent_cd = get_fix_skill_value(e_fix_skill_sub_cd_percent);
				int32 end_time = (max_time - sub_time * 1000) * (1 - percent_cd) - 200;//降低点公冷 以应对网络状况
				set_skill_cd(end_time);
				m_next_skill_cd = 0;
			}
		}
	}
	bool skill::save_skill_info(skill_proto_skill_save_load& request_save, int64& now_time)
	{
		if (nullptr == m_skill_template_ptr || m_skill_template_ptr->SaveType <= 0)
		{
			return false;
		}
		for (int32 i =0; i < e_skill_info_max; ++i)
		{
			if (i == e_skill_info_cd_cur_time)
			{
				request_save.add_data_ary(get_skill_cd());
			}
			else
			{
				request_save.add_data_ary(m_skill_info.data_ary[i]);
			}
		}
		return true;
	}
	void skill::set_passive_buff(bool be_add)
	{
		if (nullptr == m_skill_template_ptr || m_skill_template_ptr->PassiveBuff.size() <= 0)
		{
			return;
		}
		for (int32 i = 0; i < m_skill_template_ptr->PassiveBuff.size(); ++ i)
		{
			if (m_skill_template_ptr->PassiveBuff[i] > 0)
			{
				if (be_add)
				{
					buff_man::add_buff_inst(m_unit_idf.runtime_id, m_unit_idf.runtime_id, m_skill_template_ptr->PassiveBuff[i]);
				}
				else
				{
					buff_man::del_buff_by_template_id(m_unit_idf.runtime_id, m_unit_idf.runtime_id, m_skill_template_ptr->PassiveBuff[i], true);
				}
			}
		}
	}
	void skill::deactivate()
	{
		if (nullptr == m_skill_template_ptr)
		{
			return;
		}

		for (int32 i = 0; i < m_skill_template_ptr->TargetBuff.size(); ++i)
		{
			buff_man::del_buff_by_template_id(m_unit_idf.runtime_id, m_unit_idf.runtime_id, m_skill_template_ptr->TargetBuff[i], true);
		}

		for (int32 i = 0; i < m_skill_template_ptr->SelfBuff.size(); ++i)
		{
			buff_man::del_buff_by_template_id(m_unit_idf.runtime_id, m_unit_idf.runtime_id, m_skill_template_ptr->SelfBuff[i], true);
		}

		for (int32 i = 0; i < m_skill_template_ptr->BeginBuff.size(); ++i)
		{
			buff_man::del_buff_by_template_id(m_unit_idf.runtime_id, m_unit_idf.runtime_id, m_skill_template_ptr->BeginBuff[i], true);
		}

		set_passive_buff(false);
	}
	void skill::relieve_stop_time()
	{
		m_skill_work.relieve_stop_time();
	}
	void skill::change_skill(int32 skill_template_id, bool is_add)
	{
		SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, skill_template_id);
		if (nullptr == skill_template_ptr)
		{
			return;
		}
		if (is_add)
		{
			for (unit_index_list_it it = m_change_skill_list.begin(); it != m_change_skill_list.end(); ++it)
			{
				if (*it == skill_template_id)
				{
					return;
				}
				else
				{
					SkillTemplate* old_skill_template_ptr = GET_TEMPLATE(SkillTemplate, *it);
					if (old_skill_template_ptr && old_skill_template_ptr->ChangeLevel == skill_template_ptr->ChangeLevel)
					{
						if (*it == m_begin_skill_id)
						{
							m_skill_template_ptr = skill_template_ptr;
							m_cur_skill_id = skill_template_id;
							m_begin_skill_id = skill_template_id;
							send_change_skill(skill_template_id);
						}
						*it = skill_template_id;
						return;
					}
				}
			}
			m_change_skill_list.push_back(skill_template_id);
			if (nullptr == m_skill_template_ptr || skill_template_ptr->ChangeLevel > m_skill_template_ptr->ChangeLevel)
			{
				m_skill_template_ptr = skill_template_ptr;
				m_cur_skill_id = skill_template_id;
				m_begin_skill_id = skill_template_id;
				send_change_skill(skill_template_id);
			}
		}
		else
		{
			unit_index_list_it temp_it = m_change_skill_list.end();
			for (unit_index_list_it it = m_change_skill_list.begin(); it != m_change_skill_list.end(); ++it)
			{
				if (*it == skill_template_id)
				{
					temp_it = it;
					break;
				}
			}
			if (temp_it != m_change_skill_list.end())
			{
				m_change_skill_list.erase(temp_it);
				if (skill_template_id == m_begin_skill_id)
				{
					int32 max_level = -1;
					int32 max_skill_id = 0;
					SkillTemplate* max_skill_template_ptr = nullptr;
					for (unit_index_list_it it = m_change_skill_list.begin(); it != m_change_skill_list.end(); ++it)
					{
						SkillTemplate* skill_template_ptr = GET_TEMPLATE(SkillTemplate, *it);
						if (skill_template_ptr && skill_template_ptr->ChangeLevel > max_level)
						{
							max_level = skill_template_ptr->ChangeLevel;
							max_skill_id = *it;
							max_skill_template_ptr = skill_template_ptr;
						}
					}
					if (max_skill_id > 0)
					{
						m_skill_template_ptr = max_skill_template_ptr;
						m_cur_skill_id = max_skill_id;
						m_begin_skill_id = max_skill_id;
						send_change_skill(max_skill_id);
					}
					else
					{
						m_skill_template_ptr = nullptr;
						m_cur_skill_id = 0;
						m_begin_skill_id = 0;
						change_skill(get_data_info(e_skill_info_template_id), true);
					}
				}
			}
		}
	}
	void skill::send_change_skill(int32 skill_template_id)
	{
		player& player_ref = unit_man::get_player(m_unit_idf);
		if (false == player_ref.is_valid())
		{
			return;
		}
		skill_proto_change_skill skill_msg;
		skill_msg.set_skill_template_id(skill_template_id);
		player_ref.send_message_to_self(&skill_msg, e_msgindex_s2c_skill_change_skill);
	}

	fvector skill::get_enemy_pos(int32 in_radius)
	{
		unit& src_ptr = unit_man::get_unit(m_unit_idf);
		if (src_ptr.is_valid() == false)
		{
			return fvector();
		}
		const unit_index_map& aoi_tower_watch = aoi_system::get_watch_all(src_ptr.get_map_ent(),src_ptr.get_new_map_pos().unit_location);
		if (aoi_tower_watch.empty())
		{
			return fvector();
		}
		std::vector<int32> tmp_obj_index;

		int32 len = sepc_skill_range;


		for (unit_index_map_const_it it = aoi_tower_watch.begin(); it != aoi_tower_watch.end(); ++it)
		{
			unit& unit_ref = unit_man::get_unit(it->first);
			if (unit_ref.is_mission_see(m_unit_idf.runtime_id) == false)
			{
				continue;
			}
			if (init_unit::is_in_distance(unit_ref.get_new_map_pos().unit_location, src_ptr.get_new_map_pos().unit_location, m_skill_template_ptr->TargetDistance) == false)
			{
				continue;
			}
			if (src_ptr.get_pk_community_mgr().is_skill_target(unit_ref.get_array_index(), m_skill_template_ptr->TargetType) == false)
			{
				continue;
			}
			tmp_obj_index.push_back(it->first);
		}
		if (tmp_obj_index.size() == 0)
		{
			return fvector();
		}
		fvector aim_pos;
		int32 count = 0;
		for (auto it : tmp_obj_index)
		{
			unit& unit_ref = unit_man::get_unit(it);
			

			aim_pos += unit_ref.get_new_map_pos().unit_location;
			count++;
		}
		if (count > 0)
		{
			aim_pos /= count;
		}


		fvector dir = aim_pos - src_ptr.get_new_map_pos().unit_location;

		if (dir.length_2d() <= in_radius)
		{
			return aim_pos;
		}

		fvector offset;
		offset.x = in_radius * cos(dir.rotator().Yaw * RADIAN);
		offset.y = in_radius * sin(dir.rotator().Yaw * RADIAN);

		aim_pos = src_ptr.get_new_map_pos().unit_location + offset;
	
		return aim_pos;
	}
}



