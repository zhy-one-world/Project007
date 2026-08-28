/********************************************************************
created: 2019年10月15日
file base: legion_dance_def
file ext: hpp
author: zhaoyuming
purpose:
*********************************************************************/
#ifndef _LEGION_DANCE_DEF_HPP_
#define _LEGION_DANCE_DEF_HPP_

#include "char_def.hpp"
#include "game_enum_def_s.h"

namespace hld
{
#pragma pack(push,1)	

	enum e_legion_dance_invite_error
	{
		e_legion_dance_invite_error_player_info_error,				//玩家信息错误
		e_legion_dance_invite_error_not_in_map,						//玩家不再该地图中
		e_legion_dance_invite_error_not_in_game,					//玩家处于离线状态下
		e_legion_dance_invite_error_in_war,							//被邀请玩家在战斗中
		e_legion_dance_invite_error_my_in_war,						//自己在战斗中
		e_legion_dance_invite_error_target_refuse,					//被拒绝
	};

	enum e_legion_dance_accept_error
	{
		e_legion_dance_accept_error_player_info_error,				//玩家信息错误
		e_legion_dance_accept_error_not_in_map,						//玩家不再该地图中
		e_legion_dance_accept_error_not_in_game,					//玩家处于离线状态下
		e_legion_dance_accept_error_in_war,							//邀请玩家在战斗中
		e_legion_dance_accept_error_my_in_war,						//自己在战斗中
		e_legion_dance_accept_error_in_interactive,					//玩家正在交互状态下						
	};

#pragma pack(pop)
}
#endif