/********************************************************************
created: 2017年8月10日11:27:00
file base: service_goal_def
file ext: hpp
author: 韩泽成
purpose: 七日目标消息逻辑文件
*********************************************************************/
#ifndef _DP_MSGPROC_SERVICE_GOAL_HPP_
#define _DP_MSGPROC_SERVICE_GOAL_HPP_

#include "db_manager.hpp"

namespace faith
{
	void cs2dp_req_load_char_service_goal_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_service_goal_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_service_goal_proc(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_service_goal_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
