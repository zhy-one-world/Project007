/********************************************************************
created: 2020年6月17日
file base: pk_king_msg
file ext: hpp
author: zhaoyuming

purpose:
*********************************************************************/

#ifndef _PK_KING_MSG_HPP_
#define _PK_KING_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/pk_king_def.hpp"
#include "core.hpp"
#include "utility/score_indicator.h"
#include "Logic/chat_def.hpp"
namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_cs2ws_create_pk_king_combat_map = faith::e_msg_base_pk_king,				//发送创建地图请求
		e_msgindex_ws2cs_create_pk_king_combat_map_end,										//返回创建地图结果并传回地图Id
		e_msgindex_cs2ws_send_is_pk_king,													//发送成为Pk之王消息
		e_msgindex_ws2ws_send_is_pk_king,
		e_msgindex_cs2ws_send_break_line,
		e_msgindex_ws2ws_break_login_transfer_map,
		e_msgindex_ws2ws_break_login_transfer_map_end,
	};


	struct cs2ws_create_pk_king_combat_map : public faith::packet_base
	{
		guid_64 map_guid;
		int32	map_num;
		cs2ws_create_pk_king_combat_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_create_pk_king_combat_map;
		}
	};

	struct ws2cs_create_pk_king_combat_map_end : public faith::packet_base
	{
		guid_64 map_guid;
		guid_64	map_array[pk_king_max_map_num];
		int32 map_num;
		ws2cs_create_pk_king_combat_map_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_create_pk_king_combat_map_end;
		}
		void to_proto(faith::ws2cs_proto::create_pk_king_combat_map_end& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			for (int32 i = 0 ; i < map_num && i < pk_king_max_map_num; i++)
			{
				msg.add_map_array(map_array[i]);
			}
			msg.set_map_num(map_num);

		}
		void from_proto(const faith::ws2cs_proto::create_pk_king_combat_map_end& msg)
		{
			map_guid.server_64 = msg.map_guid();
			map_num = msg.map_num();
			for (int32 i = 0; i < map_num && i < pk_king_max_map_num; i++)
			{
				map_array[i] = msg.map_array(i);
			}

		}
	};

	struct cs2ws_send_is_pk_king : public faith::packet_base
	{
		guid_64 role_guid;
		int32 server_id;
		cs2ws_send_is_pk_king()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_is_pk_king;
		}
	};
	struct ws2ws_send_is_pk_king : public faith::packet_base
	{
		guid_64 role_guid;
		int32 server_id;
		ws2ws_send_is_pk_king()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_is_pk_king;
		}
	};
	struct cs2ws_send_break_line : public faith::packet_base
	{
		s_break_line_login_info	break_info;
		cs2ws_send_break_line()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_break_line;
		}
	};
	struct ws2ws_break_login_transfer_map : public faith::packet_base
	{
		guid_64						role_guid;
		guid_64						map_guid;
		int32						server_id;
		int32						activity_type;
		ws2ws_break_login_transfer_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_break_login_transfer_map;
		}
	};
	struct ws2ws_break_login_transfer_map_result : public faith::packet_base
	{
		int32						result;
		guid_64						role_guid;
		guid_64						map_guid;
		int32						war_idex;
		int32						activity_type;
		int32						server_id;
		int32						map_template_id;
		ws2ws_break_login_transfer_map_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_break_login_transfer_map_end;
		}
	};

#pragma pack(pop)
}
#endif