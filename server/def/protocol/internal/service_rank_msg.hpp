/********************************************************************
created: 2018Äê4ÔÂ12ÈÕ11:42
file base: service_rank
file ext: hpp
author: wangdi

purpose: about service rank
*********************************************************************/

#ifndef _FAITH_SERVICE_RANK_HPP_
#define _FAITH_SERVICE_RANK_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/ranking_def.hpp"

namespace faith
{
#pragma pack(push,1)
	enum 
	{
		e_msgindex_dp2cs_load_service_rank_reward_state = faith::e_msg_base_service_rank,
		e_msgindex_cs2dp_save_service_rank_reward_state,
	};

	struct dp2cs_load_service_rank_reward_state : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		bool								is_over;
		int32								data_num;
		s_service_rank_reward_state			data_list[e_service_rank_type_max];
		dp2cs_load_service_rank_reward_state()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_service_rank_reward_state;
		}
	};

	struct cs2dp_save_service_rank_reward_state : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_service_rank_reward_state			data_list[e_service_rank_type_max];
		cs2dp_save_service_rank_reward_state()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_service_rank_reward_state;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_service_rank_reward_state));
		}
	};

#pragma pack(pop)
}

#endif
