/********************************************************************
created: 2019年9月27日
file base: world_server
file ext: hpp
author: zhangshuo

purpose: WSloading链用数据包
*********************************************************************/

#ifndef _FAITH_WORLD_SERVER_MSG_HPP_
#define _FAITH_WORLD_SERVER_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/world_def.hpp"

namespace hld
{
#pragma pack(push,1)
	enum 
	{
		e_msgindex_ws2ws_get_gate_msg = hld::e_msg_base_world_server,
		e_msgindex_cs2ws_cross_player_save_end,
		e_msgindex_ws2ws_server_info_msg,
	};

	struct ws2ws_get_big_player_from_gate : public hld::packet_base
	{
		int32 server_id;
		int64 data_flag;
		ws2ws_get_big_player_from_gate()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_gate_msg;
		}
	};

	struct cs2ws_player_save_end : public hld::packet_base
	{
		guid_64	role_guid;
		cs2ws_player_save_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cross_player_save_end;
		}
	};

	struct ws2ws_send_server_info : public hld::packet_base
	{
		int32 gm_commond_arr[e_need_server_cross_max];
		int32 data_num;
		ws2ws_send_server_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_server_info_msg;
		}
	};


#pragma pack(pop)
}

#endif
