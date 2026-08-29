/********************************************************************
created: 2019年2月13日
file base: msgproc_seven_day_goal
file ext: hpp
author: wangdi
purpose: 七日目标消息逻辑文件
*********************************************************************/
#ifndef _DP_MSGPROC_SEVEN_DAY_GOAL_HPP_
#define _DP_MSGPROC_SEVEN_DAY_GOAL_HPP_

#include "db_manager.hpp"

namespace faith
{
	void cs2dp_req_load_char_seven_day_goal_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_seven_day_goal_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_seven_day_goal_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_seven_day_goal_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
