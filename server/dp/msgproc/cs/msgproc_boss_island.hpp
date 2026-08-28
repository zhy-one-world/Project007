/********************************************************************
created: 2018Äê9ÔÂ6ÈÕ
file base: msgproc_boss_island
file ext: h
author: zhangshuo

purpose: ÓÀºãµº
*********************************************************************/

#ifndef _DP_MSGPROC_MSGPROC_ISLAND_HPP_
#define _DP_MSGPROC_MSGPROC_ISLAND_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace hld
{
	void cs2dp_req_load_boss_island_info(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_boss_island_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_boss_island_info(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_boss_island_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
