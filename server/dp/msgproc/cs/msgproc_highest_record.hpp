/********************************************************************
	file name:	msgproc_highest_record.hpp
	created:	2017/06/09 15:32
	author:		Lxy
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_HIGHEST_RECORD_HPP_
#define _DP_MSGPROC_HIGHEST_RECORD_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace faith
{
	//角色历史最高纪录
	void cs2dp_req_load_player_highest_record(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void db2dp_req_load_player_highest_record_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_player_highest_record(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_highest_record_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
