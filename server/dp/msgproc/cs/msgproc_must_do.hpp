/********************************************************************
	created:	2016年9月22日
	file base:	msgproc_yesterday_must_do_remain
	file ext:	hpp
	author:		lxy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_YESTERDAY_MUST_DO_REMAIN_HPP_
#define _DP_MSGPROC_YESTERDAY_MUST_DO_REMAIN_HPP_

#include "db_manager.hpp"

namespace faith
{
	//昨天剩下没做的每日必做，找回用
	void cs2dp_req_load_char_yesterday_must_do_remain(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_yesterday_must_do_remain_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_yesterday_must_do_remain(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_yesterday_must_do_remain_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
	//每日必做次数加载
	void cs2dp_req_load_char_daily_must_do_count(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_daily_must_do_count_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_daily_must_do_count(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_daily_must_do_count_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
