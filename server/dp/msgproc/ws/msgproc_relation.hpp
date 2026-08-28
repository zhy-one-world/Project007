/********************************************************************
	file name:	msgproc_relation.hpp
	created:	2017/06/07 23:32
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _DP_MSGPROC_RELATION_HPP_
#define _DP_MSGPROC_RELATION_HPP_

#include "db_manager.hpp"

namespace hld
{
	//½ÇÉ«¹ØÏµ
	void ws2dp_req_load_player_relation(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_load_player_relation_end(db_result_type result, uint32 connindex, const guid_64& role_guid);
	void ws2dp_req_save_player_relation(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_save_player_relation_end(db_result_type result, uint32 connindex, const guid_64& role_guid);
	void ws2dp_req_load_offline_other_relation(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_load_offline_other_relation_other_server(uint32 connindex, const void* data_ptr, size_t data_len);
	
	void ws2dp_req_load_offline_other_relation_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& relation_type);
	void ws2dp_req_load_offline_other_relation_other_server_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& server_id, const int32& relation_type);
	void ws2dp_req_add_relation(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2dp_req_add_relation_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const int32 relation_type);
	void ws2dp_req_del_relation(uint32 connindex, const void* data_ptr, size_t data_len);

	void ws2dp_req_load_offline_other_relation_lua(uint32 connindex, int64 sender_guid, int32 list_type, int64 other_guid);
	void ws2dp_req_load_player_relation_lua(uint32 connindex, int64 sender_guid);
	void ws2dp_req_add_relation_lua(uint32 connindex, int64 role_guid, int64 relation_guid, xstring relation_name, int64 relation_gs_value, int32 data_array[e_relation_data_max]);
	void ws2dp_req_del_relation_lua(uint32 connindex, int64 role_guid, int64 target_role_guid, int32 relation_type);
}

#endif
