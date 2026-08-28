/********************************************************************
created: 2019年03月17日
file base: time_limit_activity_object
file ext: h
author: wangdi

purpose: time limit object
*********************************************************************/

#ifndef _TIME_LIMIT_ACTIVITY_OBJECT_H_
#define _TIME_LIMIT_ACTIVITY_OBJECT_H_

#include "Logic/time_limit_activity_def.hpp"

namespace hld
{
	class player;
	class time_limit_activity_object
	{
	public:
		time_limit_activity_object();
		~time_limit_activity_object();
		void								heart_tick(const int64& new_time);
		void								clear_data();
		void								clear_schedule_and_reward_flag();
		void								set_gs_up_init_value();
		void								set_player_ptr(player* player_ptr);
		void                                init_end();
		void								refresh_by_zero(const int64& time_now);
		void								init_template_id();
		void								init_template_id_no_cross(int32 act_guid = 0);
		void								init_template_id_cross(int32 act_guid = 0);
		bool								is_can_get_reward(int32 reward_template_id, int32 param_int_1 = 0);
		int32								get_cur_value(int32 index = 0);
		int32								get_target_value(int32 reward_template_id, int32 index = 0);
		void								get_reward(int32 reward_template_id);
		void								buy_goods(int32 reward_template_id, int32 goods_num);
		void								send_reward(int32 reward_template_id);
		void								get_reward_by_check_common_data_end(int32 reward_template_id, bool is_achieve);
		bool								is_same_cycle(int32 reward_template_id);
		bool								is_open();
		bool								is_valid();
		void								send_info_to_self();
		s_time_limit_activity_info&			get_activity_info() { return m_activity_info; }
		void								set_activity_info(s_time_limit_activity_info activity_info) { m_activity_info = activity_info; }
		int32								get_data_info(e_time_limit_activity_info info_type);
		void								set_data_info(e_time_limit_activity_info info_type, int32 info_value);
		int32								get_activity_schedule(int32 schedule_index = 0);
		void								set_activity_schedule(int32 schedule_value, int32 schedule_index = 0);
		void								add_activity_schedule(int32 add_value, int32 schedule_index = 0);
		void								set_activity_schedule_str(xstring schedule_str);
		xstring								get_activity_schedule_str();
		bool								is_reward_geted(int64 reward_index = 0);
		void								set_reward_get_flag(int64 reward_index = 0);
		void								set_reward_get_flag_with_id(int32 reward_template_id);

		const s_time_limit_activity_temp&	get_time_limit_activity_temp();
		const s_time_limit_activity_branch_temp&	get_time_limit_activity_branch_temp();

		int32								get_branch_template_id() { return m_time_limit_activity_branch_template_id; }
		int32								get_cycle_num() { return m_cycle_num; }
		int32								get_cycle_index() { return m_cycle_index; }
		void								set_need_sync_to_ranking();
		
		bool								is_affected_by_pause_flag();

		int32								get_act_type();
		int32								get_act_guid() { return m_activity_info.data_ary[e_time_limit_activity_info_guid]; };

		void								set_is_old_act(bool is_old) { m_is_old_act = is_old; };

		void								load_lucky_card_info_end(bool is_init = false);

		void								check_send_new_server_mail();


		//钻石商店
		void								refresh_diamond_shop(int32 refresh_type);
		void								buy_diamond_shop_by_index(int32 buy_index);
	private:
		player*								m_player_ptr;
		s_time_limit_activity_info			m_activity_info;
		int32								m_time_limit_activity_branch_template_id;
		int32								m_cycle_num;
		int32								m_cycle_index;
		bool								m_need_sync_to_ranking = false;
		int64								m_interval_time;	//	慢tick 1分钟 一次

		bool								m_is_old_act;
	};
}


#endif