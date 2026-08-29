/********************************************************************
created: 2015年9月26日
file base: legion_station_msg_hpp
file ext: hpp
author: zhaoyuming

purpose:
*********************************************************************/
#ifndef _LEGION_STATION_MSG_HPP
#define _LEGION_STATION_MSG_HPP

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/legion_station_def.hpp"
#include "core.hpp"
#include "utility/score_indicator.h"
#include "Logic/chat_def.hpp"

namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_cs2ws_bonfire_create = faith::e_msg_legion_station,				//发送创建篝火成功到cs
		e_msgindex_cs2ws_bonfire_end,
		e_msgindex_ws2cs_send_bonfire_level,
		e_msgindex_cs2ws_legion_boss_create,
		e_msgindex_cs2ws_sync_boss_damage_list,
	};


	struct cs2ws_bonfire_create : public faith::packet_base
	{
		guid_64		legion_guid;
		cs2ws_bonfire_create()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_bonfire_create;
		}
	};

	struct cs2ws_bonfire_end : public faith::packet_base
	{
		guid_64		legion_guid;
		cs2ws_bonfire_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_bonfire_end;
		}
	};

	struct ws2cs_send_bonfire_level : public faith::packet_base
	{
		guid_64		map_guid;
		int32		bonfire_level;
		ws2cs_send_bonfire_level()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_send_bonfire_level;
		}
		void to_proto(faith::ws2cs_proto::send_bonfire_level& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			msg.set_bonfire_level(bonfire_level);
		}
		void from_proto(const faith::ws2cs_proto::send_bonfire_level& msg)
		{
			map_guid.server_64 = msg.map_guid();
			bonfire_level = msg.bonfire_level();

		}
	};

	struct cs2ws_legion_boss_create : public faith::packet_base
	{
		guid_64		legion_guid;
		int32		boss_template_id;
		cs2ws_legion_boss_create()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_legion_boss_create;
		}
	};
	
#pragma pack(pop)
}

#endif