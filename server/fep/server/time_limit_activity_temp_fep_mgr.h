/********************************************************************
  created: 2020-1-7
  file base: time_limit_activity_temp_fep_mgr
  file ext: h
  author: wangdi
  
  purpose: 
*********************************************************************/
#ifndef _TIME_LIMIT_ACTIVITY_TEMP_FEP_MGR_
#define _TIME_LIMIT_ACTIVITY_TEMP_FEP_MGR_

#include "server_def.hpp"
#include "Logic/time_limit_activity_def.hpp"
#include "game.pb.h"

namespace hld
{
	class time_limit_activity_temp_fep_mgr
	{
	public:
		static time_limit_activity_temp_fep_mgr& get_instance()
		{
			static time_limit_activity_temp_fep_mgr instance;
			return instance;
		}
		void														init_manager();
		void														heart_tick(const int64& time_new);
		void														clear_data();
		const std::map<int32, s_time_limit_activity_temp>&			get_temp_map() { return m_time_limit_activity_temp_map; };
		const std::map<int32, s_time_limit_activity_branch_temp>&	get_branch_temp_map() { return m_time_limit_activity_branch_temp_map; };
		void														recv_ws2fep_limit_act_temp(const s_time_limit_activity_temp_db * temp_db_info, int32 data_num, bool is_begin);
		void														recv_ws2fep_limit_act_branch_temp(const s_time_limit_activity_branch_temp_db * temp_db_info, int32 data_num, bool is_end);
		void														fill_time_limit_activity_template_one_msg(const s_time_limit_activity_temp * template_line, game_proto_template_table_data * activity_template_data);
		void														fill_time_limit_activity_branch_template_one_msg(const s_time_limit_activity_branch_temp* template_line, game_proto_template_table_data * activity_template_data);

	private:
		explicit time_limit_activity_temp_fep_mgr();

		void														parse_char_temp(s_time_limit_activity_temp& temp_info, const s_time_limit_activity_temp_db& temp_dp_info);//DP表转常用表
		void														parse_char_branch_temp(s_time_limit_activity_branch_temp& temp_info, const s_time_limit_activity_branch_temp_db& temp_dp_info);//DP表转常用表		

		std::map<int32, s_time_limit_activity_temp>					m_time_limit_activity_temp_map;
		std::map<int32, s_time_limit_activity_branch_temp>			m_time_limit_activity_branch_temp_map;
	};

}
#endif
