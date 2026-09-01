/********************************************************************
  created: 2019-7-22
  file base: time_limit_activity_temp_mgr
  file ext: hpp
  author: zhangshuo
  
  purpose: 
*********************************************************************/
#ifndef _TIME_LIMIT_ACTIVITY_TEMP_MGR_
#define _TIME_LIMIT_ACTIVITY_TEMP_MGR_

#include "Logic/time_limit_activity_def.hpp"

namespace faith
{
	class time_limit_activity_temp_mgr
	{
	public:
		static time_limit_activity_temp_mgr& get_instance()
		{
			static time_limit_activity_temp_mgr instance;
			return instance;
		}
		void														init_manager();
		void														heart_tick(const int64& time_new);
		void														clear_data();

		const s_time_limit_activity_temp&							get_time_limit_act_with_id(int32 temp_id);//根据表ID获得主表
		const s_time_limit_activity_branch_temp&					get_time_limit_act_branch_with_id(int32 role_id);//根据子表唯一id获得子表

		int32														get_new_limit_act_with_act_type(int32 activity_type);//根据活动类型取得最新主表

		const std::map<int32, s_time_limit_activity_temp>&			get_temp_map() { return m_time_limit_activity_temp_map; };
		const std::map<int32, s_time_limit_activity_branch_temp>&	get_branch_temp_map() { return m_time_limit_activity_branch_temp_map; };

		void														end_act_with_act_type(int32 act_type, bool is_need_send);

		void														recv_cs2ws_limit_act_temp(const s_time_limit_activity_temp_db * temp_db_info, int32 data_num, bool is_begin);
		void														recv_cs2ws_limit_act_branch_temp(const s_time_limit_activity_branch_temp_db * temp_db_info, int32 data_num, bool is_end);

	private:
		explicit time_limit_activity_temp_mgr();

		void														parse_char_temp(s_time_limit_activity_temp& temp_info, const s_time_limit_activity_temp_db& temp_dp_info);//DP表转常用表
		void														parse_char_branch_temp(s_time_limit_activity_branch_temp& temp_info, const s_time_limit_activity_branch_temp_db& temp_dp_info);//DP表转常用表		

		std::map<int32, s_time_limit_activity_temp>					m_time_limit_activity_temp_map;
		std::map<int32, s_time_limit_activity_temp>					m_temporary_time_limit_activity_temp_map;
		std::map<int32, s_time_limit_activity_branch_temp>			m_time_limit_activity_branch_temp_map;
		std::map<int32, s_time_limit_activity_branch_temp>			m_temporary_time_limit_activity_branch_temp_map;

		s_time_limit_activity_temp									m_empty_limit_act_temp;
		s_time_limit_activity_branch_temp							m_empty_limit_act_branch_temp;
	};

}
#endif
