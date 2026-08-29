/********************************************************************
created: 2019年03月20日
file base: time_limit_activity_ws_mgr
file ext: cpp
author: wangdi

purpose: time limit activity
*********************************************************************/
#ifndef _TIME_LIMIT_ACTIVITY_MGR_WS_H_
#define _TIME_LIMIT_ACTIVITY_MGR_WS_H_

#include "logic/type_def.hpp"

#include "Logic/time_limit_activity_def.hpp"

namespace faith
{
	class client_session;

	class time_limit_activity_ws_mgr
	{
	public:
		static time_limit_activity_ws_mgr& get_instance()
		{
			static time_limit_activity_ws_mgr instance;
			return instance;
		}
		explicit time_limit_activity_ws_mgr();

		void												init_manager();
		void												tick(const int64& now_time);
		void												tick_1_day(const int64& now_time);
		void												refresh_day_for_zero(const uint64& tick_time);
		void												clear_data();
		void												refresh_data(e_time_limit_activity_type activity_type);
		void												send_activity_rank_rewards(e_time_limit_activity_type activity_type);
		int32												get_time_limit_activity_rank_min_config_value(e_time_limit_activity_type activity_type);
		void												get_rank_info(e_time_limit_activity_type activity_type, guid_64 role_uid, int32 source_server_id = 0);			//server_id不等于0 表示从ws发到gate的消息
		void												update_activity_common_data(int32 activity_type, int32 data_num, std::vector<int32> condition_list);
		void												update_activity_common_data_on_gate(int32 activity_id, int32 data_num, std::vector<int32> condition_list);
		void												update_activity_common_data_on_local(int32 activity_type, int32 data_num, std::vector<int32> condition_list);
		void												check_common_data(int32 activity_id, guid_64 user_guid, int32 activity_type, int32 check_num);
		void												check_common_data_on_gate(int32 server_id, int32 activity_id, guid_64 user_guid, int32 activity_type, int32 check_num);
		void												check_common_data_end(int32 activity_id, guid_64 user_guid, bool is_achieve);
		void												send_activity_common_data(std::vector<int32> data_type_list, std::vector<int32> data_num_list);
		void												send_activity_common_data_one(guid_64 role_guid, std::vector<int32> data_type_list, std::vector<int32> data_num_list);
		void												get_activity_common_data_to_gate(client_session* session_ptr);
		void												get_activity_common_data_on_gate(int32 server_id, guid_64 role_guid);
		void												end_act_with_gm(int32 activity_type);
		void												end_act_with_time_limit_activity_type(e_time_limit_activity_type activity_type);
		void												send_load_time_limit_act_to_db();
		void												load_end_time_limit_act(const s_time_limit_ws_info* act_info, int32 data_num, int32 is_end);
		void												save_ws_time_limit_act_to_db(int32 activity_type = -1);
		void												init_all_time_limit_ws();
		bool												is_activity_open(e_time_limit_activity_type activity_type);
		void												send_activity_schedule_to_other_ws(e_time_limit_activity_type activity_type, int32 server_id = 0);
		void												clear_activity_by_type(e_time_limit_activity_type activity_type, bool is_need_save = false);
		s_time_limit_activity_ws_info&						get_cur_cloud_shop_activity();
		s_time_limit_activity_ws_info&						get_activity_info_ref(e_time_limit_activity_type activity_type);
		e_time_limit_activity_type							get_can_begin_cloud_shop_type_in_this_server();
	private:
		s_time_limit_activity_ws_info						m_empty_info;
		std::map<int32, s_time_limit_activity_ws_info>		m_activity_info;
	};
}

#endif
