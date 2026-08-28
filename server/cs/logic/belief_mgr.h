/********************************************************************
created: 2015年8月28日13:56:28
file base: belief_mgr
file ext: h
author: W.H.B.A.

purpose: about role's belief_mgr
*********************************************************************/
#ifndef _BELIEF_MGR_H_
#define _BELIEF_MGR_H_

#include "belief.h"

namespace hld
{
	class player;
	class cbelief_mgr
	{
	public:
		cbelief_mgr();
		~cbelief_mgr(void);
	public:
		void heart_tick(const int64& new_time);
		void set_player_ptr(player* player_ptr);
		void save_belief_to_db(int32 save_type);
		void load_belief_by_db(const s_belief_info* belief_data, int32 belief_num);
		bool load_belief_by_db_lua(const char *data_ptr, int32 data_len);
		void send_belief_all();
		void send_belief_one(int32 belief_type);
		void clear_data();
		void set_belief_by_info(const s_belief_info& belief_data, int32 belief_number);
		int32 get_total_rank();		// get total rank of all belief
		int32 get_belief_level(int32 belief_type);
		BeliefTemplate* get_cur_max_rank();
		cbelief* getbelief(int32 belief_type);
		int32 get_cur_max_rank_value(int32 target_star);
	public:
		xstring get_belief_name(e_belief_series_type belief_type);
		xstring get_sub_belief_buff_name(const cbelief& belief_ref);
		void send_belief_notice(int32& belief_type);
		void sync_ranking_belief_data();

		std::vector<int32> get_belief_buff_id_array();
	public:
		void belief_change_att(std::vector<f32> const & att_array, int32 add_count, bool is_add);
		int32 get_equip_num_by_belief_level(int32 rank_num, int32 star_num);
	private:
		int32 get_belief_record_type(int32 belief_type);

	private:
		cbelief										m_belief_ary[e_belief_series_type_max];		// 所有信仰
		player*										m_player_ptr;							// 信仰所属角色名称
		int32										m_buff_index;
		int64										m_buff_change_time;
		std::vector<int32>							m_cur_buff_array;
	};
}

#endif
