/********************************************************************
created: 2018Äê4ÔÂ12ÈÕ11:42
file base: msgproc_service_rank
file ext: hpp
author: wangdi

purpose: about service rank
*********************************************************************/
#ifndef _DP_MSGPROC_SERVICE_RANK_HPP_
#define _DP_MSGPROC_SERVICE_RANK_HPP_

#include "db_manager.hpp"

namespace hld
{
	void cs2dp_req_load_service_rank_reward_state(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_service_rank_reward_state_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_service_rank_reward_state(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_service_rank_reward_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
