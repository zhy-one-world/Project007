#pragma once
/********************************************************************
created: 2017年8月10日11:27:00
file base: service_goal
file ext: hpp
author: 韩泽成
purpose: 七日目标头文件
*********************************************************************/
#ifndef _SERVICE_GOAL_HPP_
#define _SERVICE_GOAL_HPP_

#include "template/ServerTemplateHead.h"
#include "Logic/service_goal_def.hpp"
#include "google/protobuf/message.h"

namespace hld
{
	class player;
	class service_goal_proto_service_goal_one;

	class cservice_goal
	{
	public:
		cservice_goal();
		~cservice_goal();

	public:
		void									 set_player_index(int32 player_index);
		void									 clear_data();

	public:
		void									 init_service_goal(ServiceGoalTemplate* template_ptr, const s_service_goal_info service_goal_info);
												 
	public:										 
		const bool								 is_unfinished();
		const bool								 is_finished();
		const bool								 is_received();
		const bool								 is_empty();
		const bool								 is_non_nullptr();

		void									 received_service_goal();
		void									 refresh_service_goal(bool is_self_add = false);
		void									 make_service_goal_one_proto(service_goal_proto_service_goal_one* proto_service_goal_one);
		void									 send_notice_service_goal(const std::vector<int32>& item_datay_array,const int32 item_data_locaked = 0);
		
		const s_service_goal_info&				 get_service_goal_info();
		const int32								 get_service_goal_template_id();
		const e_service_goal_type				 get_service_goal_condition_type();
		const int32								 get_service_goal_active_data();
		const e_service_goal_active_type		 get_service_goal_active_type();
		const int32								 get_notice_id();
		void									 refresh_over_time(const int32& time_now);

	private:									 
		const e_service_goal_state				 get_service_goal_state();
		void									 set_service_goal_state(e_service_goal_state service_goal_state);
		bool									 cal_service_goal_state(const int32 target_value, const int32 current_value);
		const int32								 can_received_service_goal();

		int32									 get_service_goal_schedule();
		void									 set_service_goal_schedule(int32 service_goal_schedule);

		bool									 put_reward_into_bag();
		const int32								 get_service_goal_target_value();

		void									 send_service_goal_one_to_self();
		void									 send_service_goal_operate_result_to_self(const int32 operate_type, const int32 operate_end_state);
		void									 send_message_to_self(google::protobuf::Message* net_pro, uint32 header);

	public:
		int32									 get_data_info(e_service_goal_inst_data data_index);
		void									 set_data_info(e_service_goal_inst_data data_index, int32 data_value);
	private:
		player&									 get_player() const;
		int32									 cal_target_value(int32 grade_num, int32 level_num);

	private:
		ServiceGoalTemplate*					 m_template_ptr;
		s_service_goal_info						 m_service_goal_info;
		int32									 m_player_index;
	};
}

#endif