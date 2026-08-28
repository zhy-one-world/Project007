/********************************************************************
created: 2015年9月26日
file base: legion_station_def_hpp
file ext: hpp
author: zhaoyuming

purpose:
*********************************************************************/
#ifndef _LEGION_STATION_DEF_HPP
#define _LEGION_STATION_DEF_HPP

#include "char_def.hpp"
#include "utility/cs_date.hpp"
#include "game_enum_def_s.h"

namespace hld
{
#pragma pack(push,1)
	const int32							legion_friend_spawn_id = 82072065;//军团BOSS友方NPC

	enum e_legion_station_state_type
	{
		e_legion_station_state_type_usual,				//普通状态
		e_legion_station_state_type_legion_boss,		//军团boss状态
		e_legion_station_state_type_bonfire,			//军团篝火状态
		e_legion_station_state_type_max,
	};

	struct s_legion_big_player_add
	{
		int32 class_type;
		int32 add_count;
		xchar player_name[max_name_size + 1];
		s_legion_big_player_add()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_role_name(xstring role_name)
		{
			memset(player_name, 0, sizeof(player_name));
			memcpy(player_name, role_name.c_str(), role_name.size() > sizeof(player_name) ? sizeof(player_name) : role_name.size());
		}
	};	
#pragma pack(pop)
}

#endif