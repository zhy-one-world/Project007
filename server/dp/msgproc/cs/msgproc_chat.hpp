
#ifndef  MSGPROC_CHAT_HPP
#define  MSGPROC_CHAT_HPP

#include "logic/type_def.hpp"
#include "db_manager.hpp"

namespace faith
{

	void cs2dp_req_load_chat_record(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_chat_record_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_req_save_chat_record(uint32 connindex, const void* data_ptr, size_t data_len);
	bool cs2dp_save_role_chat_record_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);


}




#endif