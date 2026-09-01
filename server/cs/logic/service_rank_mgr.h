/********************************************************************
created: 2018年4月12日11:42
file base: service_rank_mgr
file ext: h
author: wangdi

purpose: about service rank
*********************************************************************/
#ifndef _SERVICE_RANK_MGR_H_
#define _SERVICE_RANK_MGR_H_

#include "Logic/ranking_def.hpp"

namespace faith
{
	class player;
	class service_rank_mgr
	{
	public:
		service_rank_mgr();
		~service_rank_mgr();
	public:
		void									clear_data();
		void									set_player_ptr(player* player_ptr);

		void									save_recevice_rewards_state_to_db(int32 save_type);
		void									load_recevice_rewards_state_from_db(const s_service_rank_reward_state* data_list, int32 data_num);
		bool                                    load_recevice_rewards_state_by_db_lua(const char *data_ptr, int32 data_len);
		void									send_service_rank_reward_state_one(int32 info_type);
		void									send_service_rank_reward_state_all();
		void									receive_reward_by_type(int32 rank_type);
		void									receive_reward_by_type_end(int32 rank_type, int32 role_num);
		void									set_receive_reward_state(int32 rank_type, int32 reward_state);
		bool									is_can_set_reward_stete_have(int32 rank_type, const int64& rank_value);
		void									check_receive_reward_end_time();
		void									check_receive_reward_state(e_RankingIndex ranking_index, const int64& rank_value);
		int32									get_service_rank_type_by_ranking_type(e_RankingIndex ranking_index);

	private:
		player*									m_player_ptr;
		int32									m_recevice_rewards_state_arr[e_service_rank_type_max];
	};
}

#endif
