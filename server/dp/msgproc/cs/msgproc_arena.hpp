/********************************************************************
created:	2016年8月25日
file base : msgproc_arena
file ext : hpp
author : zhangminghai

purpose :
*********************************************************************/
#ifndef _DP_MSGPROC_ARENA_HPP_
#define _DP_MSGPROC_ARENA_HPP_

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace hld
{
	//竞技场
	void cs2dp_req_add_arena_log(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_add_arena_log_end(db_result_type result);

	void cs2dp_req_load_arena_log(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_load_arena_log_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_index);

	void cs2dp_req_load_arena_char_role_info(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_role_info_end(db_result_type result, uint32 connindex, const guid_64& map_guid);

	void cs2dp_req_load_arena_char_att(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_att_end(db_result_type result, uint32 connindex, const guid_64& map_guid);

	void cs2dp_req_load_arena_char_skill(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_skill_end(db_result_type result, uint32 connindex, const guid_64& map_guid);

	void cs2dp_req_load_arena_char_item(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_item_end(db_result_type result, uint32 connindex, const guid_64& map_guid);

	void cs2dp_req_load_arena_char_spirit(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_spirit_end(db_result_type result, uint32 connindex, const guid_64& map_guid);

	void cs2dp_req_load_arena_char_fight_att(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_fight_att_end(db_result_type result, uint32 connindex, const guid_64& map_guid);

	void cs2dp_req_load_arena_char_buff(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_buff_end(db_result_type result, uint32 connindex, const guid_64& map_guid);

	void cs2dp_req_load_arena_char_belief(uint32 connindex, const guid_64& role_guid, const guid_64& map_guid);
	void cs2dp_req_load_arena_char_belief_end(db_result_type result, uint32 connindex, const guid_64& map_guid);
}

#endif