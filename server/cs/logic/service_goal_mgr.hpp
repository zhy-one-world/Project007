#pragma once
/********************************************************************
created: 2017年8月10日11:27:00
file base: service_goal_mgr
file ext: hpp
author: 韩泽成
purpose: 七日目标头文件
*********************************************************************/
#ifndef _SERVICE_GOAL_MGR_HPP_
#define _SERVICE_GOAL_MGR_HPP_

#include "Logic/service_goal_def.hpp"
#include "service_goal.hpp"

namespace hld
{
	class player;
	class cservice_goal_mgr
	{
	public:
		cservice_goal_mgr();
		~cservice_goal_mgr();

	public:
		void						clear_data();
		void						set_player_index(int32 player_index);
		void						load_service_goal_by_db(const s_service_goal_info* service_goal_data, const int32 service_goal_num);
		bool                        load_service_goal_by_db_lua(const char *data_ptr, int32 data_len);
		void						save_service_goal_to_db(const int32 save_type);
		void						send_all_service_goal();
	
	public:
		void						service_goal_operate(const int32 operate_type, const int32 service_goal_id);
		void						refresh_service_goal(const e_service_goal_type service_goal_type);
		void						refresh_service_goal_all();
		void						refresh_over_time_all();

		int32						get_service_goal_reward_days(const int32 reward_data, const e_service_goal_active_type active_type);
		int32						get_specify_reward_num();
		bool						check_in_time();
		bool						check_over();

	private:
		cservice_goal&				get_service_goal_obj_by_index(const int32 obj_index);
		cservice_goal&				get_service_goal_obj_by_id(const int32 obj_id);
		std::vector<cservice_goal*>		get_service_goal_obj_by_type(const e_service_goal_type service_goal_type);
		
		void						create_service_goal_obj( ServiceGoalTemplate* template_ptr, const s_service_goal_info service_goal_info);
		void						receive_service_goal(int32 service_goal_id);
		void						refresh_service_goal_received_reward_days();
		void						add_service_goal_reward_days(const int32 reward_day, const e_service_goal_active_type active_type);
		void						clear_service_goal_reward_days();

		void						send_message_to_self(google::protobuf::Message* net_pro, uint32 header);


	private:
		player&						get_player() const;

	private:
		int32						m_player_index;
		cservice_goal				m_service_goal_obj_array[MAX_SERVICE_GOAL_NUM];
		cservice_goal				m_empty_goal_obj;
		int32						m_service_goal_obj_num;
		int32						m_service_goal_reward_days[MAX_REWARD_DAYS + 1][e_service_goal_active_type_max];
		int32						m_service_goal_max_over_time;
	};
}
#endif