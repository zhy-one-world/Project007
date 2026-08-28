/********************************************************************
created: 2016年7月14日
file base: achievement
author: 张宇翔
purpose: 单个成就
*********************************************************************/

#include "achievement.hpp"
#include "template/template_manager.h"
#include "logic/player.hpp"

namespace hld
{
	cachievement::cachievement()
	{
		clear_data();
	}
	cachievement::~cachievement()
	{
	}
	void cachievement::tick(float elapse_time)
	{
	}
	void cachievement::init_achievement_mgr(player* player_ptr)
	{
		if (player_ptr == nullptr)
		{
			return;
		}
		clear_data();
		m_player_ptr = player_ptr;
	}
	void cachievement::clear_data()
	{
		m_achievement_info.reset();
		m_player_ptr = nullptr;
		m_achievement_ptr = nullptr;
	}
	bool cachievement::init_achievement_by_template(int32 achievement_id, player* player_ptr)
	{

		AchievementTemplate* achievement_ptr = GET_TEMPLATE(AchievementTemplate, achievement_id);
		if (nullptr == achievement_ptr)
		{
			return false;
		}
		//初始化
		m_achievement_ptr = achievement_ptr;
		m_player_ptr = player_ptr;
		m_achievement_info.reset();
		set_inst_data(eaid_id, achievement_id);
		return true;
	}
	bool cachievement::init_achievement_by_info(s_achievement_info achievement_info, player* player_ptr)
	{

		for (int32 info_index = eaid_id; info_index < eaid_max; info_index++)
		{
			if (set_inst_data(info_index, achievement_info.data_ary[info_index]) == false)
			{
				return false;
			}
		}
		AchievementTemplate* achievement_ptr = GET_TEMPLATE(AchievementTemplate, achievement_info.data_ary[eaid_id]);
		if (nullptr == achievement_ptr)
		{
			return false;
		}
		//初始化
		m_achievement_ptr = achievement_ptr;
		m_player_ptr = player_ptr;
		return true;
	}


	int32 cachievement::finish_achievement(std::vector<s_item_template_info>& get_money_tuple_array)
	{
		if (nullptr == m_achievement_ptr)
		{
			return eaet_system_error;
		}
		if (nullptr == m_player_ptr)
		{
			return eaet_system_error;
		}
		if (get_achievement_state() != eas_can_finish)
		{
			return eaet_operation_illegal;
		}
		int32 money_array_size = m_achievement_ptr->FinishMoney.size();
		if (money_array_size == 0 || money_array_size % e_money_tuple_max != 0)
		{
			return eaet_system_error;
		}
		
		for (int32 money_index = 0; money_index < money_array_size; money_index += e_money_tuple_max)
		{
			int32 money_id = m_achievement_ptr->FinishMoney[money_index + e_money_tuple_id];
			int32 money_num = m_achievement_ptr->FinishMoney[money_index + +e_money_tuple_num];

			m_player_ptr->add_money_or_exp((e_money_type)money_id, money_num, e_server_log_add_money_achievement, get_inst_data(eaid_id));
			get_money_tuple_array.push_back({ money_id, money_num });
		}
		set_achievement_state(eas_done);
		
		return eaet_success;
	}

	int32 cachievement::get_achievement_type()
	{
		if (nullptr == m_achievement_ptr)
		{
			return -1;
		}
		else
		{
			return m_achievement_ptr->AchievementType;
		}
	}
	int32 cachievement::get_inst_data(int32 index) const
	{
		if (index >= eaid_id &&  index < eaid_max)
		{
			return m_achievement_info.data_ary[index];
		}
		else return -1;
	}
	bool cachievement::set_inst_data(int32 index, int32 num)
	{
		if (index >= eaid_id && index < eaid_max)
		{
			m_achievement_info.data_ary[index] = num;
			return true;
		}
		return false;
	}

	bool cachievement::set_achievement_state(e_achievement_state achievement_state)
	{
		if (achievement_state <= eas_done)
		{
			m_achievement_info.data_ary[eaid_current_state] = achievement_state;
			return true;
		}
		return false;
	}

	bool  cachievement::inc_count()
	{
		if (nullptr == m_achievement_ptr)
		{
			return false;
		}
		if (get_achievement_state() >= eas_can_finish)
		{//已可交付
			return false;
		}

		if (m_achievement_ptr->AchievementGoal.size() < e_item_tuple_max)
		{
			return false;
		}

		m_achievement_info.data_ary[eaid_current_num]++;
		check_achievement_state();
		return true;
	}
	bool  cachievement::replace_count(int32 new_value)
	{
		if (nullptr == m_achievement_ptr
			|| m_achievement_ptr->AchievementGoal.size() < e_item_tuple_max)
		{
			return false;
		}
		if (get_achievement_state() >= eas_can_finish)
		{//已可交付
			return false;
		}
		if (new_value > m_achievement_ptr->AchievementGoal[e_item_tuple_num])
		{
			new_value = m_achievement_ptr->AchievementGoal[e_item_tuple_num];
		}
		if (new_value > m_achievement_info.data_ary[eaid_current_num])
		{
			m_achievement_info.data_ary[eaid_current_num] = new_value;
			check_achievement_state();
			return true;
		}
		return false;
	}


	bool cachievement::check_achievement_state()
	{
		if (m_achievement_ptr == nullptr
			|| m_achievement_ptr->AchievementGoal.size() < e_item_tuple_max)
		{
			return false;
		}

		if (get_achievement_state() >= eas_can_finish)
		{//已可交付
			return false;
		}

		if (get_inst_data(eaid_current_num) >= m_achievement_ptr->AchievementGoal[e_item_tuple_num])
		{
			set_inst_data(eaid_current_num, m_achievement_ptr->AchievementGoal[e_item_tuple_num]);
			set_achievement_state(eas_can_finish);
			return true;
		}
		return false;
	}

}

