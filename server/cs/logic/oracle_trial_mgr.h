/********************************************************************
created: 2019年7月25日
file base: oracle_trial_mgr
file ext: h
author: zhaoyuming
purpose: 天启试炼
*********************************************************************/
#ifndef _ORACLE_TRIAL_MGR_H
#define _ORACLE_TRIAL_MGR_H

#include "oracle_trial_def.hpp"
#include "gain_treasure_def.hpp"
#include "drop_def.hpp"


namespace faith
{
	class player;
	class oracle_trial_mgr
	{
	public:
		oracle_trial_mgr();
		~oracle_trial_mgr();
	public:
		void clear_data();
		void set_unit_index(int32 array_index) { m_array_index = array_index; }
		void load_oracle_trial_info_from_db(const s_oracle_trial_info_db* oracle_trial_info , int32 data_num);
		bool load_oracle_trial_by_db_lua(const char *data_ptr, int32 data_len);
		void save_oracle_trial_to_db(int32 tier_num);
		void send_player_oracle_trial_info_all();
		void set_play_oracle_trial_info(int32 tier_num, int32 customs_num, int32 customs_state);
		void set_play_oracle_trial_reward_info(int32 tier_num, int32 reward_num, int32 reward_state);
		int32 get_tier_customs_state_info(int32 tier_num);
		int32 get_tier_reward_state_info(int32 tier_num);
		void send_mopping_up_award();
		int32 get_hight_rank_num();
		void get_oracle_trial_stat_num_raward(int32 tier_num, int32 reward_num);
		void send_commingup_item_list(int32 item_id[max_commingup_item_id], int32 item_num[max_commingup_item_id], int32 _item_num);
		void send_fuben_over_info(int32 map_id, int32 old_statr, int32 new_statr);
		oracle_trial_play_info get_m_oracle_trial_info();

		item_num_info get_drop_list_by_id(int32 drop_id);
	//以下函数是直接调用lua的函数
		int32 lua_get_comppingup_item_list();
		int32 lua_get_stat_num_award(int32 tier_num, int32 reward_num);
		void lua_map_game_over(int32 trial_num, int32 customs_num, int32 customs_state);

		bool is_show_prompt();

	private:
		int32									m_array_index;
		oracle_trial_play_info					m_oracle_trial_info;
	};
}

#endif