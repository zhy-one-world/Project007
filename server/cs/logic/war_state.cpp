/********************************************************************
created: 2016Äê10ÔÂ25ÈÕ12:03:42
file base: war_state
file ext: h
author: zhy

purpose: 
*********************************************************************/

#include "logic/unit.hpp"
#include "logic/unit_man.h"
#include "template/template_manager.h"
#include "war_state.h"
#include "utility/init_unit.h"

namespace hld
{
	c_war_state::c_war_state(void)
	{
		m_unit_array_index = 0;
		clear_data();
	}
	c_war_state::~c_war_state(void)
	{
	}

	void c_war_state::heart_tick(const int64& new_time)
	{
		if (m_unit_war_time_num <= 0)
		{
			return;
		}

		for (int32 i = 0; i < m_unit_war_time_vec_num; ++i)
		{
			s_unit_war_time& war_time_ref = m_unit_war_time_vec[i];
			if (war_time_ref.is_data_use)
			{
				if (new_time >= war_time_ref.war_time)
				{
					war_time_end(war_time_ref.unit_index, true);
				}
			}
		}
	}
	void c_war_state::clear_data()
	{
		m_unit_war_time_vec.clear();
		m_unit_war_time_vec_num = 0;
		m_unit_war_time_num = 0;
	}
	void c_war_state::war_time_end(int32 unit_index, bool is_self)
	{
		for (int32 i = 0; i < m_unit_war_time_vec_num; ++i)
		{
			s_unit_war_time& war_time_ref = m_unit_war_time_vec[i];
			if (war_time_ref.unit_index == unit_index)
			{
				war_time_ref.clear_data();
				--m_unit_war_time_num;
				break;
			}
		}
		if (m_unit_war_time_num <= 0)
		{
			unit& self_unit = unit_man::get_unit(m_unit_array_index);
			self_unit.get_pawn_att().set_game_att(e_unit_game_att_war_state, e_war_state_peace, true);
		}
		if (is_self)
		{
			unit& temp_unit = unit_man::get_unit(unit_index);
			temp_unit.get_war_state_mgr().war_time_end(m_unit_array_index, false);
		}
	}
	void  c_war_state::clear_war_state()
	{
		for (int32 i = 0; i < m_unit_war_time_vec_num; ++i)
		{
			s_unit_war_time& war_time_ref = m_unit_war_time_vec[i];
			if (war_time_ref.is_data_use)
			{
				war_time_end(war_time_ref.unit_index, true);
			}
		}
	}
	void c_war_state::add_in_war(int32 unit_index, int32 war_time)
	{
		if (war_time == 0)
		{
			return;
		}
		if (war_time  == -1)
		{
			war_time = GAMECONFIG->PublicWarTime;
		}
		int32 empty_index = -1;
		for (int32 i = 0; i < m_unit_war_time_vec_num; ++i)
		{
			s_unit_war_time& war_time_ref = m_unit_war_time_vec[i];
			if (war_time_ref.unit_index == unit_index)
			{
				war_time_ref.war_time = init_unit::get_end_time(war_time);
				unit& self_unit = unit_man::get_unit(m_unit_array_index);
				if (self_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) < e_move_ment_mount_walk || self_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_wing_fly)
				{
					self_unit.get_pawn_att().set_game_att(e_unit_game_att_war_state, e_war_state_attack_in, true);
				}
				return;
			}
			if (war_time_ref.is_data_use == false && empty_index == -1)
			{
				empty_index = i;
			}
		}
		if (empty_index == -1)
		{
			s_unit_war_time new_war_time;
			m_unit_war_time_vec.push_back(new_war_time);
			empty_index = m_unit_war_time_vec_num++;
		}
		s_unit_war_time& unit_war_time_ref = m_unit_war_time_vec[empty_index];
		unit_war_time_ref.unit_index = unit_index;
		unit_war_time_ref.is_data_use = true;
		unit_war_time_ref.war_time = init_unit::get_end_time(war_time);
		++m_unit_war_time_num;
		unit& self_unit = unit_man::get_unit(m_unit_array_index);
		if (self_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) < e_move_ment_mount_walk || self_unit.get_pawn_att().get_game_att(e_unit_game_att_movement) == e_move_ment_wing_fly)
		{
			self_unit.get_pawn_att().set_game_att(e_unit_game_att_war_state, e_war_state_attack_in, true);
		}
	}
}
