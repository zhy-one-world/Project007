/********************************************************************
	created:	2015年7月24日14:18:14
	file base:	msgproc_goods
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_GOODS_HPP_
#define _DP_MSGPROC_GOODS_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace hld
{
	//角色购买商品记录
	void cs2dp_req_load_char_goods(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_goods_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_goods(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_goods_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
}

#endif
