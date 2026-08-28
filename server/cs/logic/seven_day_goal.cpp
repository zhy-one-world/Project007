/********************************************************************
created: 2019年2月13日
file base: cseven_day_goal
file ext: cpp
author: wangdi
purpose: 七日目标头文件
*********************************************************************/

#include "seven_day_goal.h"
#include "template/StringConst_S.h"
#include "utility/cs_date.hpp"

namespace hld
{
	
	cseven_day_goal::cseven_day_goal()
	{
		clear_data();
	}

	cseven_day_goal::~cseven_day_goal()
	{

	}

	void cseven_day_goal::clear_data()
	{
		m_seven_day_goal_info.clear_data();
	}

	void cseven_day_goal::init_seven_day_goal(s_seven_day_goal_info seven_day_goal_info)
	{
		m_seven_day_goal_info = seven_day_goal_info;
	}

	e_service_goal_type cseven_day_goal::get_seven_day_goal_condition_type()
	{
		ServiceGoalTemplate* temp_template_ptr = get_service_goal_template_ptr();
		if (nullptr == temp_template_ptr)
		{
			return e_service_goal_type_max;
		}
		return (e_service_goal_type)temp_template_ptr->ConditionType;
	}

	ServiceGoalTemplate* cseven_day_goal::get_service_goal_template_ptr()
	{
		if (m_seven_day_goal_info.data_ary[e_seven_day_goal_info_id] <= 0)
		{
			return nullptr;
		}
		return GET_TEMPLATE(ServiceGoalTemplate, m_seven_day_goal_info.data_ary[e_seven_day_goal_info_id]);
	}

	int32 cseven_day_goal::get_seven_day_goal_active_data()
	{
		ServiceGoalTemplate* temp_template_ptr = get_service_goal_template_ptr();
		if (nullptr == temp_template_ptr)
		{
			return 0;
		}
		return temp_template_ptr->TheData;
	}

	e_service_goal_active_type cseven_day_goal::get_seven_day_goal_active_type()
	{
		ServiceGoalTemplate* temp_template_ptr = get_service_goal_template_ptr();
		if (nullptr == temp_template_ptr)
		{
			return e_service_goal_active_type_max;
		}
		return (e_service_goal_active_type)temp_template_ptr->ActiveType;
	}

	int32 cseven_day_goal::cal_target_value(int32 grade_num, int32 level_num)
	{
		return ((grade_num - 1) * 11 + level_num);
	}

	int32 cseven_day_goal::get_data_info(e_seven_day_goal_info data_index)
	{
		if (data_index >= e_seven_day_goal_info_max)
		{
			return 0;
		}
		return m_seven_day_goal_info.data_ary[data_index];
	}

	void cseven_day_goal::set_data_info(e_seven_day_goal_info data_index, int32 data_value)
	{
		if (data_index >= e_seven_day_goal_info_max)
		{
			return;
		}
		m_seven_day_goal_info.data_ary[data_index] = data_value;
	}

	bool cseven_day_goal::cal_seven_day_goal_state(const int32 target_value, const int32 current_value)
	{
		if (current_value >= target_value)
		{
			set_data_info(e_seven_day_goal_info_state, e_service_goal_state_finished);
			return true;
		}
		const int32 current_schedule = get_data_info(e_seven_day_goal_info_schedule);
		if (current_value > current_schedule)
		{
			set_data_info(e_seven_day_goal_info_schedule, current_value);
			return true;
		}
		return false;
	}

	int32 cseven_day_goal::can_received_seven_day_goal()
	{
		int32 time_now = time_helper::get_cur_time_new().second;
		if (time_now >= m_seven_day_goal_info.data_ary[e_seven_day_goal_info_open_time]
			&& e_service_goal_state_finished == m_seven_day_goal_info.data_ary[e_seven_day_goal_info_state])
		{
			return e_item_string_succeed;
		}
		return e_item_string_unkown;
	}

	const s_seven_day_goal_info& cseven_day_goal::get_seven_day_goal_info()
	{
		return m_seven_day_goal_info;
	}
}

