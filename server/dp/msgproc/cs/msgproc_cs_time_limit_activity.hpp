/********************************************************************
	created:	2018-7-28 15:15:40
	file base:	msgproc_cs_time_limit_activity
	file ext:	hpp
	author:		shangxiaoliang
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_CS_TIME_LIMIT_ACTIVITY_HPP_
#define _DP_MSGPROC_CS_TIME_LIMIT_ACTIVITY_HPP_

#include "db_manager.hpp"

namespace hld
{
	//限时活动
	void cs2dp_req_load_char_time_limit_activity_info(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_time_limit_activity_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_time_limit_activity_info(uint32 connindex, const void* data_ptr, size_t data_len);

	void ws2dp_req_load_time_limit_temp_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_load_time_limit_temp_info_end(db_result_type result, uint32 connindex, int32 cur_num);
	void ws2dp_req_load_time_limit_branch_temp_info(uint32 connindex);
	void ws2dp_req_load_time_limit_branch_temp_info_end(db_result_type result, uint32 connindex, int32 cur_num);

	void ws2dp_req_save_time_limit_temp_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_save_time_limit_temp_info_end(db_result_type result, uint32 connindex);
	void ws2dp_req_save_time_limit_branch_temp_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_save_time_limit_branch_temp_info_end(db_result_type result, uint32 connindex);

	void ws2dp_req_update_role_time_limit_info_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_update_role_time_limit_info_func_end(db_result_type result, uint32 connindex);

	void ws2dp_req_update_delete_time_limit_temp(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_update_delete_time_limit_temp_end(db_result_type result, uint32 connindex);

	void ws2dp_req_update_delete_time_limit_branch_temp(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_update_delete_time_limit_branch_temp_end(db_result_type result, uint32 connindex);

	void ws2dp_req_refresh_role_time_limit_guid(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_refresh_role_time_limit_guid_end(db_result_type result, uint32 connindex);


	bool cs2dp_save_role_time_limit_activity_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
