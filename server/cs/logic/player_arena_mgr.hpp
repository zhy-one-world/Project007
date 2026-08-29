/********************************************************************
created: 2016年8月23日14:56:28
file base: player_arena_mgr
file ext: hpp
author: zhangminghai

purpose: about arena system's arena_mgr
*********************************************************************/
#pragma once

#ifndef _PLAYER_ARENA_MGR_H_
#define _PLAYER_ARENA_MGR_H_

#include "Logic/arena_def.hpp"
#include "logic/type_def.hpp"
namespace faith
{

	class player_arena_mgr
	{
	public:
		player_arena_mgr() { m_unit_index = 0; clear_data(); };
		~player_arena_mgr() {};
	public:
		void							heart_tick(const int64& new_time);//用于发送挑战冷却

		void							set_player_ptr(int32& unit_index);

		void							clear_data();

		int32							current_rank() const {return m_current_rank;}   //lua
		void							set_current_rank(int32 new_rank) { m_current_rank = new_rank; };

		void							get_arena_rank(); //lua

		void							get_arena_rank_end(int32 arena_rank, int32 is_request);

		//请求挑战
		void							req_challenge(const guid_64& target_role_guid, bool is_cost_money, int32 target_cur_pos, int32 self_cur_pos);

		void							do_challenge(guid_64 target_role_guid, int32 robot_id, int32 req_result
													, int32 cost_money_type, int32 cost_money_num
													, int32 target_cur_pos, int32 self_cur_pos);

		void							challenge_over(const guid_64& target_role_guid, int32 end_result, int32 target_rank, int32 self_rank);

		void							add_challeng_log(const guid_64& challenge_role_guid, const guid_64& target_role_guid, bool initiative, int32 end_result,int32 role_rank, int32 target_rank);

		//请求获取挑战记录
		void							get_challenge_logs();

		void							get_challenge_logs_end(std::vector<s_chanllenge_log_info>&  challenge_logs);

		//请求匹配的玩家
		void							req_get_match_player();  //lua

		void							req_match_player_end(const player_arena_rank* target_guids);

		void                            lua_req_match_player_end(const char *data_ptr, int32 data_len);

		void							req_get_first_three_player(); //lua

		const xchar*					get_current_challenge_player_name(const guid_64& target_guid);

		void							get_reward();
		int64                           get_vip_add(int64 value);
		void							mopping_remain_times();
		void							set_is_requiring(bool is_requiring);     //lua
		bool                            is_requiring() { return m_is_requiring; } // lua
		void							sync_gs_value(int64 gs_value);
	public:
		bool							m_is_requiring;
		int64							m_requiring_safty_timer;
	private:
		int32									m_unit_index;
		int32									m_current_rank;
		std::vector<player_arena_rank>			m_match_player;
		int32									m_today_add_time;
	public:
		bool                           lua_req_challenge_to_ws(int32 target_guid_a, int32 target_guid_b, int32 money_cost_type, int32 money_cost_value, int32 target_cur_pos, int32 self_cur_pos);
		bool                           lua_set_reward_time();

	public:
	   typedef std::map<guid_64, xstring>   challenge_name_map;

	   static challenge_name_map   name_map;

	public:
	  static void add_map_element(const guid_64& role_guid, xstring name);

	  static const char* get_name(const guid_64& role_guid);


		



	};
}
#endif