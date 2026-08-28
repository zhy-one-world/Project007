/********************************************************************
created:	2016Äê10ÔÂ27ÈÕ
	file base : msgproc_big_player
	file ext : hpp
	author : lxy

	purpose :
*********************************************************************/
#ifndef _DP_MSGPROC_BIG_PLAYER_HPP_
#define _DP_MSGPROC_BIG_PLAYER_HPP_

#include "db_manager.hpp"


namespace hld
{
	//big_player
	void ws2dp_req_load_big_player_base_info(uint32 connindex, const guid_64& role_guid, int32 big_type);
	void ws2dp_req_load_big_player_base_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_big_player_att(uint32 connindex, const guid_64& role_guid, int32 big_type);
	void ws2dp_req_load_big_player_att_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_big_player_skill(uint32 connindex, const guid_64& role_guid, int32 big_type);
	void ws2dp_req_load_big_player_skill_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_big_player_item(uint32 connindex, const guid_64& role_guid, int32 big_type);
	void ws2dp_req_load_big_player_item_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);

	void ws2dp_req_load_big_player_buff(uint32 connindex, const guid_64& role_guid, int32 big_type);
	void ws2dp_req_load_big_player_buff_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 big_type);
}

#endif