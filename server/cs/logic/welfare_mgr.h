/********************************************************************
created: 2016年10月8日17:00
file base: welfare_mgr
file ext: h
author: lxy

purpose: about welfare
*********************************************************************/
#ifndef _WELFARE_MGR_H_
#define _WELFARE_MGR_H_

#include "Logic/welfare_def.hpp"
namespace hld
{
	class player;
	class welfare_proto_get_reward_end;
	struct s_item_template_info;
	struct WelfareTemplate;

	class cwelfare_mgr
	{
	public:
		cwelfare_mgr();
		~cwelfare_mgr(void);
		void clear_data();
		void heart_tick(const int64& new_time);
		void set_player_ptr(int32& unit_index) { m_unit_array_index = unit_index; }

		void save_welfare_to_db(int32 save_type);
		void load_welfare_by_db(const s_welfare_info* welfare_data);
		bool load_welfare_by_db_lua(const char *data_ptr, int32 data_len);
		void send_welfare_one(e_welfare_type info_index);
		void send_welfare_all();
		void set_welfare_info(e_welfare_type info_type,int32 info_value);
		int32 get_welfare_info(e_welfare_type info_type);

		void save_active_degree_to_db(int32 save_type);
		void load_active_degree_by_db(const s_active_degree_info* active_degree_data);
		bool load_active_degree_by_db_lua(const char *data_ptr, int32 data_len);
		void send_active_degree();

		void save_random_get_to_db(int32 save_type);
		void load_random_get_by_db(const s_welfare_random_get_info* active_degree_data);
		void send_random_get_all();
		void send_random_get_one(e_welfare_random_item info_index);
		void set_random_get_info(e_welfare_random_item info_index, int32 info_value);
		i32 get_random_get_info(e_welfare_random_item info_index);

		void set_active_degree_info(e_daily_active_degree_type info_type, int32 info_value);
		i32 get_active_degree_info(e_daily_active_degree_type info_type);

		void get_welfare_by_id(int32 welfare_template_id);
		void get_first_recharge_welfare_by_id();
		void reset_first_recharge_welfare();
		void get_welfare_by_id_activatecode(int32 welfare_template_id);
		bool get_welfare_by_first_recharge(int32 recharge_template_id);
		int32 get_cur_value_by_welfare_typ(int32 welfare_typ);
		int32 get_welfare_by_id_welfare_index(int32 welfare_template_id);
		bool is_welfare_geted(int32 welfare_typ, int32 welfare_index);
		void set_welfare_get_flag(int32 welfare_typ, int32 welfare_index);
		void refresh_daily_info();
		void refresh_at_specify_hour();
		int32 get_welfare_has_num(int32 welfare_typ);

		void get_welfare(int32 welfare_template_id, std::vector<s_item_template_info>& item_list, std::vector<s_item_template_info>& money_list);

		void add_welfare_to_vector(int32 id, int32 num, std::vector<s_item_template_info>& res_list);

		bool is_correct_class_type(int32 item_template_id);

		s_welfare_info& get_welfare_info_ref();
		s_active_degree_info& get_active_degree_info_ref();
		s_welfare_random_get_info&	get_random_get_info_ref();
		bool is_recharge_time(int32 recharge_index);

		/************************************************************************/
		/*                         每日签到                                     */
		/************************************************************************/
	public:
		void	check_in(WelfareTemplate* welfare_template_ptr);
		void	month_card_check_in(WelfareTemplate* welfare_template_ptr);
		void	exclusive_card_get_reward(WelfareTemplate* welfare_template_ptr);
		void    refresh_month_info();
		int32   get_unit_array() { return m_unit_array_index; }
		void	refresh_grand_total_check_in_num();
		void	retroactive_all_days();	//补签所有
		int32	get_retroactive_all_days_money_num();
		void	get_cumulative_sign_in_reward(int32 index);//领取累计签到奖励

	private:
		// 将相应的奖励放入msg中
		bool	get_welfare_reward(const e_welfare_type welfare_type, const int32 welfare_target_value, welfare_proto_get_reward_end& msg, std::vector<s_item_template_info>& data_array);
		int32	data_to_target_value(const e_welfare_type welfare_type, const int32 target_value);
		bool	is_today_check_in();
		void	set_today_check_in();
		void    send_notice(const std::vector<int32>& item_array, const std::vector<int32>& money_array, int32 notice_id);
		int32	get_check_in_times();
		void	add_replace_times();
		int32   get_has_replace_times();

	private:
		s_welfare_info				m_welfare_info;
		s_active_degree_info		m_active_degree_info;
		s_welfare_random_get_info	m_random_get_info;
		int32						m_unit_array_index;

		//用来tick往上加的在线时间，在 load 时同步为 存储位“e_daily_active_degree_type_online_30_minutes”的数据，在 save 时赋值给存储位
		//防止 tick 的加值是float值导致的 int + float 会损失数据的问题
		int64						m_online_time;
		int64						m_old_tick_time;
		int64						m_min_tick;
	};
}

#endif
