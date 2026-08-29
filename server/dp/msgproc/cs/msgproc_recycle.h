#ifndef DP_MSGPROC_RECYCLE_H
#define DP_MSGPROC_RECYCLE_H

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace faith
{

	void cs2dp_req_load_char_recycle(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_recycle_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	bool cs2dp_save_role_recycle_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
	void cs2dp_req_save_char_recycle(uint32 connindex, const void* data_ptr, size_t data_len);

	void cs2dp_req_load_char_recycle_task(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_recycle_task_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	bool cs2dp_save_role_recycle_task_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
	void cs2dp_req_save_char_recycle_task(uint32 connindex, const void* data_ptr, size_t data_len);

	void cs2dp_req_load_char_recycle_invited(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_recycle_invited_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);


	void cs2dp_req_get_inviter_player_info(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_get_inviter_player_info_end(db_result_type result,uint32 connindex, const guid_64& role_guid, const guid_64& target_guid);
	void cs2dp_req_save_inviter_player_add(uint32 connindex, const void* data_ptr, size_t data_len);

	




}














#endif
