/********************************************************************
created: 2019年03月20日
file base: time_limit_activity_mgr
file ext: h
author: wangdi

purpose: time limit activity
*********************************************************************/
#ifndef _TIME_LIMIT_ACTIVITY_MGR_H_
#define _TIME_LIMIT_ACTIVITY_MGR_H_

#include "Logic/time_limit_activity_def.hpp"
#include "time_limit_activity_object.h"

namespace faith
{
	typedef std::map<int32, time_limit_activity_object>		time_limit_act_map;
	class player;
	class time_limit_activity_mgr
	{
	public:
		time_limit_activity_mgr();
		~time_limit_activity_mgr();
		void													clear_data();
		void													heart_tick(const int64& new_time);
		void													set_player_ptr(player* player_ptr);
		void													refresh_by_zero(const int64& time_now);
		void													send_open_activity_all();
		void													send_open_activity_branch_info_all();
		void													add_new_act_with_new_temp();//根据新表获得新活动
		void													delete_old_act_with_new_temp();
		void													init_template_id_all();
		void													init_template_id_cross();
		void													save_data_to_db(const int32& unit_array_index, int32 save_type_ex);
		void													load_data_from_db(const s_time_limit_activity_info_db *activity_data, int32 data_num);
		bool                                                    load_data_by_db_lua(const char *data_ptr, int32 data_len);
		std::vector<int32>&										get_activity_obj(e_time_limit_activity_type activity_type);
		time_limit_activity_object&								get_act_obj_by_act_guid(int32 act_guid);
		time_limit_activity_object&								get_new_act_obj_by_act_type(e_time_limit_activity_type activity_type);
		bool													is_activity_open(e_time_limit_activity_type activity_type);
		int32													get_activity_circle_num(e_time_limit_activity_type activity_type);
		int32													get_activity_circle_index(e_time_limit_activity_type activity_type);
		int32													get_activity_branch_template_id(e_time_limit_activity_type activity_type);
		int32													get_activity_template_id(e_time_limit_activity_type activity_type);
		int32													get_treasure_lua_award_index(e_time_limit_activity_type activity_type);
		int32													get_activity_schedule(e_time_limit_activity_type activity_type, int32 schedule_index = 0, int32 act_guid = 0);//act_guid表示活动唯一guid，如果不填默认通过类型去找最新的
		void													set_activity_schedule(e_time_limit_activity_type activity_type, int32 schedule_value, int32 schedule_index = 0, int32 act_guid = 0);
		void													add_activity_schedule(e_time_limit_activity_type activity_type, int32 add_value, int32 schedule_index = 0, int32 act_guid = 0);
		void													sync_common_data_after_add_schedule(e_time_limit_activity_type activity_type, int32 add_value);
		void													activity_behavior_done(e_time_limit_behavior_type behavior_type, int32 add_value = 1);;
		void													apply_award(int32 reward_template_id, int32 act_guid, int32 param_int_1 = -1);
		void													reset_ranking_value(e_time_limit_activity_type activity_type);
		void													apply_small_charge_award(int32 recharge_template_id);
		void													sync_time_limit_activity_data_to_ws(int32 activity_type, int32 data_num, std::vector<int32> condition_list);
		void													check_time_limit_activity_common_data_to_ws(int32 activity_id, int32 data_type, int32 check_num);
		void													check_time_limit_activity_common_data_to_ws_resp(int32 activity_id, bool is_achieve);
		void													set_up_rank_activity_schedule(e_time_limit_activity_type activity_type);
		void													check_refresh_charge_value();
		void													init_all_act();
		void													change_added_drop_item();
		void													apply_daily_gift_charge_award(int32 recharge_template_id);
		void													check_send_new_server_mail();

		void													buy_time_limit_item(int32 activity_type, int32 buy_num);
		void													send_buy_time_limit_item_end(int32 activity_type, int32 result_type);

		void													send_limit_time_notice_info();	//发送限时公告数据 优先级较高

	private:
		player*													m_player_ptr;
		time_limit_act_map										m_activity_obj;
		time_limit_activity_object								m_empty_act_obj;
		std::vector<int32>										m_act_obj_vec;
	};
}

#endif
