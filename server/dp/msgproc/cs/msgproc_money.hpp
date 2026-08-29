/********************************************************************
	created:	2015年7月24日14:00:15
	file base:	msgproc_money
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_MONEY_HPP_
#define _DP_MSGPROC_MONEY_HPP_

#include "db_manager.hpp"

namespace faith
{
	//角色技能
	void cs2dp_req_load_char_money(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_money_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_money(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_money_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
