#pragma once
/********************************************************************
created: 2019年2月13日
file base: seven_day_goal_mgr
file ext: h
author: wangdi
purpose: 七日目标头文件
*********************************************************************/
#ifndef _SEVEN_DAY_GOAL_MGR_H_
#define _SEVEN_DAY_GOAL_MGR_H_
#include "Logic/seven_day_goal_def.hpp"
#include "seven_day_goal.h"
namespace hld
{
	class player;
	class service_goal_proto_seven_day_goal_one;

	class cseven_day_goal_mgr
	{
	public:
		cseven_day_goal_mgr();
		~cseven_day_goal_mgr();

	public:
		void						clear_data();
		void						set_player_index(int32 player_index);
		void						load_seven_day_goal_by_db(const s_seven_day_goal_info* seven_day_goal_data, const int32 seven_day_goal_num);
		bool                        load_seven_day_goal_by_db_lua(const char *data_ptr, int32 data_len);
		void						save_seven_day_goal_to_db(const int32 save_type);
		void						send_all_seven_day_goal();
	
		void						refresh_seven_day_goal(const e_service_goal_type seven_day_goal_type);
		void						refresh_seven_day_goal_all();

		int32						get_seven_day_goal_reward_days(int32 reward_data, e_service_goal_active_type active_type);
		int32						get_specify_reward_num();

		int32						get_has_been_received();
		cseven_day_goal&					get_seven_day_goal_obj_by_index(int32 obj_index);
		cseven_day_goal*					get_seven_day_goal_obj_by_index_lua(int32 obj_index);
		cseven_day_goal&					get_seven_day_goal_obj_by_id(int32 obj_id);
		std::vector<cseven_day_goal*>		get_seven_day_goal_obj_by_type(const e_service_goal_type seven_day_goal_type);
		
		void						create_seven_day_goal_obj(s_seven_day_goal_info seven_day_goal_info);
		void						received_seven_day_goal(int32 seven_day_goal_id);
		void						refresh_seven_day_goal_received_reward_days();
		void						add_seven_day_goal_reward_days(const int32 reward_day, const e_service_goal_active_type active_type);
		void						clear_seven_day_goal_reward_days();

		void						send_message_to_self(google::protobuf::Message* net_pro, uint32 header);
		bool						put_reward_into_bag(int32 seven_day_goal_info_id);
		void						refresh_seven_day_goal(cseven_day_goal* seven_day_goal_obj_ptr, bool is_self_add = false);
		void						make_seven_day_goal_one_proto(service_goal_proto_seven_day_goal_one* msg_one, cseven_day_goal& seven_day_goal_obj_ref);
		void						send_seven_day_goal_one_to_self(cseven_day_goal& seven_day_goal_obj_ref);
		void						send_seven_day_goal_operate_result_to_self(int32 operate_type, int32 operate_end_state, cseven_day_goal& seven_day_goal_obj_ref);

		int32						get_seven_day_goal_num(){ return m_seven_day_goal_obj_num; }
	private:
		player&						get_player() const;

	private:
		int32						m_player_index;
		cseven_day_goal				m_seven_day_goal_obj_array[max_seven_day_goal_num];
		cseven_day_goal				m_empty_goal_obj;
		int32						m_seven_day_goal_obj_num;
		int32						m_seven_day_goal_reward_days[max_reward_days + 1][e_service_goal_active_type_max];
	};
}
#endif