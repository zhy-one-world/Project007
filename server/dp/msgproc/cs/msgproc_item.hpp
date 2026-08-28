/********************************************************************
	created:	2015年7月24日14:18:14
	file base:	msgproc_item
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_ITEM_HPP_
#define _DP_MSGPROC_ITEM_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace hld
{
	//角色装备
	void cs2dp_req_load_char_item(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index, int32 bag_type);
	void cs2dp_req_load_char_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index, int32 bag_type);
	void cs2dp_req_save_char_item(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
