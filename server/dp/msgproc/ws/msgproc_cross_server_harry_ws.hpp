/********************************************************************
	created:	2019Äê4ÔÂ4ÈÕ12:59:26
	file base:	msgproc_cross_server_harry_ws
	file ext:	hpp
	author:		zhangshuo
	
	purpose:	
*********************************************************************/
#ifndef _MSGPROC_CROSS_SERVER_HARRY_WS_H_
#define _MSGPROC_CROSS_SERVER_HARRY_WS_H_

#include "db_manager.hpp"

namespace hld
{
	void save_cross_server_harry_ws_info_lua(uint32 connindex, int32 server_id, int32 server_money, int32 special_money, int64 first_player_guid, int64 second_player_guid, int64 third_player_guid, xstring first_player_name);
	void save_cross_server_harry_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_cross_server_harry_ws_info_end(db_result_type result, uint32 connindex);

	void load_cross_server_harry_ws_info_lua(uint32 connindex);
	void load_cross_server_harry_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_cross_server_harry_ws_info_end(db_result_type result, uint32 connindex);

	void load_harry_player_detail_lua(uint32 connindex, int32 player_idex, int64 role_guid, int32 server_id);
	void load_harry_player_detail(uint32 connindex, const void *data_ptr, size_t data_len);

	void del_one_server_msg_func_lua(uint32 connindex, int32 server_id);
	void del_one_server_msg_func(uint32 connindex, const void *data_ptr, size_t data_len);
	void del_one_server_msg_func_end(db_result_type result, uint32 connindex);

	void ws2dp_req_load_harry_player_base_info(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id = 0);
	void ws2dp_req_load_harry_player_base_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_harry_player_att(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id = 0);
	void ws2dp_req_load_harry_player_att_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_harry_player_skill(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id = 0);
	void ws2dp_req_load_harry_player_skill_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_harry_player_item(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id = 0);
	void ws2dp_req_load_harry_player_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_harry_player_buff(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id = 0);
	void ws2dp_req_load_harry_player_buff_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_harry_player_belief(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id = 0);
	void ws2dp_req_load_harry_player_belief_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_harry_player_fight_att(uint32 connindex, const guid_64& role_guid, int32 big_type, int32 server_id = 0);
	void ws2dp_req_load_harry_player_fight_att_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

}

#endif