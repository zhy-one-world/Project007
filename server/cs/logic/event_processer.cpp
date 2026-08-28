/********************************************************************
created:	2018/04/02 14:22:00
file base:  event_processer
file ext:	cpp
author:	    zhy
	
purpose:	
*********************************************************************/

#include "buff_def.hpp"
#include "event_processer.h"
#include "logic/player.hpp"
#include "logic/unit.hpp"
#include "logic/unit_man.h"
#include "internet/skill.pb.h"
#include "time.hpp"
#include "internet/net.pb.h"
#include "utility/random.h"

namespace hld
{
	void game_event_handler::change_event_param(int32 owner_index, int32 skill_template_id, int64 skill_cd, bool apply, const std::vector<int32>& event_condition)
	{
		if (event_condition.size() < 10 || event_condition[0] <= 0)
		{
			return;
		}
		if (apply)
		{
			event_processer* event_processer_prt = event_manager::get_instance().get_event_processer(owner_index);
			if (event_processer_prt)
			{
				game_event game_event_info;
				game_event_info.m_type = event_condition[0];
				game_event_info.m_skill_id = skill_template_id;
				game_event_info.m_dist_ptr = this;
				game_event_info.m_sender_index = owner_index;
				game_event_info.m_last_time = skill_cd;
				game_event_info.m_tick_time = event_condition[1] * 1000;
				if (game_event_info.m_tick_time <= 0)
				{
					game_event_info.m_tick_time = 100;
				}
				game_event_info.m_random_num = event_condition[2];
				game_event_info.m_event_param.m_type = event_condition[0];
				game_event_info.m_event_param.m_owner_index = owner_index;
				game_event_info.m_event_param.m_event_param_1 = event_condition[3];
				game_event_info.m_event_param.m_event_param_2 = event_condition[4];
				event_processer_prt->add_event(game_event_info);
			}
		}
		else
		{
			event_processer* event_processer_prt = event_manager::get_instance().get_event_processer(owner_index);
			if (event_processer_prt)
			{
				event_processer_prt->del_event(event_condition[0], this);
				trigger_event(owner_index, false);
			}
		}
	}
	bool game_event_handler::on_event(int32 owner_index, s_trigger_param& trigger_param, const std::vector<int32>& event_condition)
	{
		if (event_condition.size() < 10 || event_condition[0] <= 0)
		{
			return false;
		}

		if (event_condition[9] != 0 && e_game_event_type_unit_bullet == event_condition[0])
		{
			int32 target_index = trigger_param.m_event_param_1;
			unit& unit_ref = unit_man::get_unit(owner_index);
			if (unit_ref.is_valid() == false)
			{
				return false;
			}

			if (false == unit_ref.get_pk_community_mgr().is_skill_target(target_index, eskilltarget_enemy))
			{
				return false;
			}
		}
		if (event_condition[0] == trigger_param.m_type)
		{
			switch (trigger_param.m_type)
			{
			case e_game_event_type_unit_hp_down:
			{
				if (trigger_param.m_event_param_1 >= event_condition[3] &&
					trigger_param.m_event_param_1 <= event_condition[4] &&
					trigger_param.m_event_param_2 >= event_condition[4])
				{
					trigger_event(owner_index, true);
					return true;
				}
			}
			break;
			case e_game_event_type_unit_hp_up:
			case e_game_event_type_unit_attack_num:
			{
				if (trigger_param.m_event_param_1 <= event_condition[4] &&
					trigger_param.m_event_param_1 >= event_condition[3] &&
					trigger_param.m_event_param_2 <= event_condition[3])
				{
					trigger_event(owner_index, true);
					return true;
				}
				if ((trigger_param.m_event_param_1 <= trigger_param.m_event_param_2)
					&& event_condition[5]
					&& trigger_param.m_type == e_game_event_type_unit_hp_up)
				{
					trigger_event(owner_index, true);
					return true;

				}
			}
			break;
			case e_game_event_type_unit_dispel_self:
			case e_game_event_type_unit_dispel_target:
			{
				return false;
			}
			break;
			case e_game_event_type_unit_bullet:
			{
				trigger_event(trigger_param.m_event_param_1, true);
				return true;
			}
			break;
			default:
				if ((event_condition[3] == 0 || trigger_param.m_event_param_1 == event_condition[3])
					&& (event_condition[4] == 0 || trigger_param.m_event_param_2 == event_condition[4])
					&& (event_condition[5] == 0 || trigger_param.m_event_param_3 != event_condition[5]))
				{
					trigger_event(owner_index, true);
					return true;
				}
				break;
			}
		}
		return false;
	}
	event_processer::event_processer() 
	{ 
	}
	void event_processer::clear_data()
	{
		m_event_map.clear();
		m_param_list.clear();
	}
	void event_processer::add_event(const game_event& event)
	{
		game_event_map_it it = m_event_map.find(event.m_type);
		if (it != m_event_map.end())
		{
			it->second.push_back(event);
		}
		else
		{
			m_event_map[event.m_type].push_back(event);
		}
	}
	void event_processer::del_event(int32 event_type, const game_event_handler* m_dist_ptr)
	{
		game_event_map_it it = m_event_map.find(event_type);
		if (it != m_event_map.end() && it->second.empty() == false)
		{
			for (game_event_list_it list_it = it->second.begin(); list_it != it->second.end(); )
			{
				game_event_list_it temp_it = list_it++;
				if (temp_it->m_dist_ptr == m_dist_ptr)
				{
					it->second.erase(temp_it);
				}
			}
		}
		return;
	}
	void event_processer::heart_tick(const int64& new_time)
	{
		if (m_param_list.empty())
		{
			return;
		}
		for (std::list<s_trigger_param>::iterator it = m_param_list.begin(); it != m_param_list.end(); ++it)
		{
			trigger_event(*it);
		}
		m_param_list.clear();
	}
	void event_processer::add_trigger_param(s_trigger_param& param)
	{
		game_event_map_it it = m_event_map.find(param.m_type);
		if (it != m_event_map.end() && it->second.empty() == false)
		{
			m_param_list.push_back(param);
		}
	}

	bool event_processer::is_tip_skill(int32 skill_id)
	{
		if (skill_id == passive_skill_zlzz_id)
		{
			return false;
		}
		return true;
	}

	void event_processer::trigger_event(s_trigger_param& param)
	{
		game_event_map_it it = m_event_map.find(param.m_type);
		if (it != m_event_map.end() && it->second.empty() == false)
		{
			for (game_event_list_it list_it = it->second.begin(); list_it != it->second.end();)
			{
				game_event_list_it temp_it = list_it++;
				if (temp_it->m_event_param == param)
				{
					int64 now_time = utility::get_tick_count();
					if (now_time < temp_it->m_last_time)
					{
						continue;
					}
					int32 random_num = random_gen::get_random(0, 100);
					if (temp_it->m_random_num == 0 || random_num <= temp_it->m_random_num)
					{
						if (temp_it->m_dist_ptr->on_event(param))
						{
							temp_it->m_last_time = now_time + temp_it->m_tick_time;
							player& player_ref = unit_man::get_player(temp_it->m_sender_index);
							if (player_ref.is_valid())
							{
								player_ref.get_passive_skill().set_data_info(temp_it->m_skill_id, e_passive_skill_info_cd, temp_it->m_last_time / 1000);
								if (is_tip_skill(temp_it->m_skill_id))
								{
									static skill_proto_trigger_passive_skill item_msg;
									item_msg.set_skill_template_id(temp_it->m_skill_id);
									player_ref.send_message_to_self(&item_msg, e_msgindex_s2c_trigger_passive_skill);
								}
							}
						}
					}
				}
			}
		}
	}
	
	void event_processer::kill_all_event()
	{
        m_event_map.clear();
	}

	event_processer* event_manager::get_event_processer(int32 unit_index)
	{
		if (unit_index >= npc_arrary_index_begin && unit_index < npc_arrary_index_max)
		{
			return &(m_npc_array[unit_index - npc_arrary_index_begin]);
		}
		else if (unit_index > 0 && unit_index < init_player_max)
		{
			return &(m_player_array[unit_index]);
		}
		return nullptr;
	}
}
