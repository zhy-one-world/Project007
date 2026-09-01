#pragma once
/********************************************************************
created: 2019年2月13日
file base: cseven_day_goal
file ext: h
author: wangdi
purpose: 七日目标头文件
*********************************************************************/
#ifndef _seven_day_goal_HPP_
#define _seven_day_goal_HPP_

#include "Logic/seven_day_goal_def.hpp"
#include "template/template_manager.h"

namespace faith
{
	class player;
	class cseven_day_goal
	{
	public:
		cseven_day_goal();
		~cseven_day_goal();

		void									clear_data();
		bool									is_valid() { return m_seven_day_goal_info.data_ary[e_seven_day_goal_info_id] > 0; }
		void									init_seven_day_goal(s_seven_day_goal_info seven_day_goal_info);
		const s_seven_day_goal_info&			get_seven_day_goal_info();
		e_service_goal_type						get_seven_day_goal_condition_type();
		ServiceGoalTemplate*					get_service_goal_template_ptr();
		int32									get_seven_day_goal_active_data();
		e_service_goal_active_type				get_seven_day_goal_active_type();
		bool									cal_seven_day_goal_state(const int32 target_value, const int32 current_value);
		int32									can_received_seven_day_goal();
	
		int32									get_data_info(e_seven_day_goal_info data_index);
		void									set_data_info(e_seven_day_goal_info data_index, int32 data_value);
		int32									cal_target_value(int32 grade_num, int32 level_num);

	private:
		s_seven_day_goal_info					m_seven_day_goal_info;
	};
}

#endif