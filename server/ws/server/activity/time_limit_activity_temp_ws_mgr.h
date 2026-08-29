/********************************************************************
  created: 2019-7-22
  file base: time_limit_activity_temp_ws_mgr
  file ext: hpp
  author: zhangshuo
  
  purpose: 
*********************************************************************/
#ifndef _time_limit_activity_temp_ws_mgr_
#define _time_limit_activity_temp_ws_mgr_

#include "server_def.hpp"
#include "Logic/time_limit_activity_def.hpp"
#include "template/template_manager.h"
#include "game.pb.h"

namespace faith
{
	class time_limit_activity_temp_ws_mgr
	{
	public:
		static time_limit_activity_temp_ws_mgr& get_instance()
		{
			static time_limit_activity_temp_ws_mgr instance;
			return instance;
		}
		void														init_manager();
		void														heart_tick(const int64& time_new);
		void														clear_data();
		void														send_to_dp_load_temp();//去dp load当前表的数据
		void														load_temp_end(const s_time_limit_activity_temp_db* temp_dp_info, bool is_load_all, int32 data_num);
		void														load_branch_temp_end(const s_time_limit_activity_branch_temp_db* temp_dp_info, bool is_load_all, int32 data_num);
		void														save_temp_to_db(const s_time_limit_activity_temp& temp_info);
		void														save_branch_temp_to_db(const s_time_limit_activity_branch_temp& temp_info);
		void														delete_temp(int32 act_guid);
		void														delete_cycle_first_branch_temp_arr(std::vector<int32> cycle_first_branch_temp_arr, int32 activity_type);
		void														delete_branch_temp(int32 act_guid);
		void														load_new_temp(bool is_empty, bool is_need_refresh_guid);
		void														update_role_time_limit_act_guid(int32 act_type, int32 act_guid);
		const s_time_limit_activity_temp&							get_time_limit_act_with_id(int32 temp_id);//根据表ID获得主表
		const s_time_limit_activity_temp&							get_new_limit_act_with_type(int32 activity_type);
		const s_time_limit_activity_branch_temp&					get_time_limit_act_branch_with_id(int32 role_id);//根据子表唯一id获得子表
		const std::map<int32, s_time_limit_activity_temp>&			get_temp_map() { return m_time_limit_activity_temp_map; };
		const std::map<int32, s_time_limit_activity_branch_temp>&	get_branch_temp_map() { return m_time_limit_activity_branch_temp_map; };
		void														send_to_cs_end_act_type(int32 act_type, int32 is_need_send_to_client);
		void														end_act_with_act_type(int32 act_type, int32 is_need_send_to_client);

		void														set_is_load_empty_from_db(bool is_load_empty) { m_is_load_empty_from_db = is_load_empty; }
		bool														get_is_load_empty_from_db() { return m_is_load_empty_from_db; }

		void														fill_time_limit_activity_template_one_msg(const s_time_limit_activity_temp* template_line, game_proto_template_table_data* activity_template_data);
		void														fill_time_limit_activity_branch_template_one_msg(const s_time_limit_activity_branch_temp* template_line, game_proto_template_table_data* activity_template_data);		

		void														parse_char_temp(s_time_limit_activity_temp& temp_info, const s_time_limit_activity_temp_db& temp_dp_info);//DP表转常用表
		void														parse_char_branch_temp(s_time_limit_activity_branch_temp& temp_info, const s_time_limit_activity_branch_temp_db& temp_dp_info);//DP表转常用表		
		void														package_char_temp(s_time_limit_activity_temp_db& temp_dp_info, const s_time_limit_activity_temp& temp_info);//常用表转DP表
		void														package_char_branch_temp(s_time_limit_activity_branch_temp_db& temp_dp_info, const s_time_limit_activity_branch_temp& temp_info);//常用表转DP表
		void														load_limit_act_temp(bool is_need_update, bool is_need_refresh_guid);//读表
		void														load_limit_act_temp(int32 mark_time, bool is_need_update, std::set<int32>& add_id_set, bool is_need_refresh_guid, e_template template_name);
		int32														get_new_limit_act_temp_with_type(int32 activity_type);//获取上次最新的主表数据
		int32														compare_limit_act_temp(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id, bool is_need_refresh_guid);
		int32														compare_cycle_first_branch_temp_arr(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id);
		int32														compare_limit_act_branch_temp(const TimeLimitActivityBranchTemplate* limit_act_temp, int32 act_temp_id);
		std::vector<int32>											get_first_cycle_branch_temp_arr_by_template(int32 first_branch_temp_id, int32 next_first_branch_temp_id, int32 activity_type);
		std::vector<int32>											get_first_cycle_branch_struct_arr_by_template(int32 first_branch_temp_id, int32 next_first_branch_temp_id, int32 activity_type);
		int32														make_new_limit_act_temp_with_temp(const TimeLimitActivityTemplate* limit_act_temp, int32 mark_time, bool is_need_update, int32 template_from_type);
		void														change_limit_act_temp(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id, int32 mark_time, int32 template_from_type);
		std::vector<int32>											make_new_limit_act_branch_temp_with_temp_arr(const std::vector<int32>& cycle_first_id_arr, std::vector<int32>& original_cycle_first_id_arr, int32 mark_time, int32 activity_type);
		void														make_new_limit_act_branch_temp_with_temp(const TimeLimitActivityBranchTemplate* limit_act_temp, int32 mark_time);
		void														change_limit_act_branch_temp(const TimeLimitActivityBranchTemplate* limit_act_temp, int32 act_temp_id);
		void														change_limit_act_branch_temp_all(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id);
		void														send_to_cs_template();
		void														send_to_fep_template();
		bool														is_self_server_temp_line(e_time_limit_activity_type activity_type, const std::vector<int32>& param_int_arr);//该行是否为本服所需
		void														check_and_update_branch_temp_info(const TimeLimitActivityTemplate* limit_act_temp, int32 act_temp_id, int32 mark_time, int32 template_from_type);
		void														delete_temp_info_when_time_over();
		int32														get_cur_mark_time();
		std::vector<int32>											get_activity_id_arr_by_type(int32 activity_type);
		s_time_limit_activity_temp									exchange_to_struct_by_template_ptr(const TimeLimitActivityTemplate* template_ptr);
		void														delete_time_limit_activity_by_type(int32 time_limit_activity_type);
		void														syn_template_info_to_cs_and_client_and_fep();
		bool														is_can_load_new_temp();
		void														load_new_temp_on_server_open();
		void														refresh_activity_guid(int32 old_act_guid, int32 new_act_guid);
		bool														is_self_region_act(const std::vector<int32>& param_int_arr);
		void														check_and_clear_branch_template_no_use();
	private:
		explicit time_limit_activity_temp_ws_mgr();
		std::map<int32, s_time_limit_activity_temp>					m_time_limit_activity_temp_map;
		std::map<int32, s_time_limit_activity_branch_temp>			m_time_limit_activity_branch_temp_map;

		s_time_limit_activity_temp									m_empty_limit_act_temp;
		s_time_limit_activity_branch_temp							m_empty_limit_act_branch_temp;

		bool														m_is_load_empty_from_db;
	};

}
#endif
