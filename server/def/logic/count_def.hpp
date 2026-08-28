/********************************************************************
  created: 2018年1月10日00:01:41
  file base: count_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _LOW_COUNT_DEF_HPP_
#define _LOW_COUNT_DEF_HPP_

#include "logic/type_def.hpp"

/************************************************************************/
/*        控制玩家登陆的基本数量限制							        */
/************************************************************************/

namespace hld 
{
#pragma pack(push,1)
#ifdef _DEBUG
	const int32 init_normal_max = 100;					//普通玩家进入数量
	const int32 init_vip_max = 10;					//vip+gm账号通道
	const int32 init_player_max = init_normal_max + init_vip_max; //初始化角色数量（服务器最大数量）
	const int32 init_link_max = 10;					//排队数量
	const int32 init_link_more = 10;					//超了上限后给玩家提示
	const int32 init_socket_link = init_player_max + init_link_max;//可以维持连接的数量
	const int32 init_socket_more = init_socket_link + init_link_more;//连接数在角色的基础上多一些
	const int32 init_npc_max = 10000;					//初始化npc数量（服务器最大数量）
	const int32 init_cs_max = 2;						// 总共有多少个cs
	const int32 init_session_max = init_player_max * init_cs_max; // 总共有多少个链接玩家
	const int32 max_save_data_count = init_player_max + 100;//数据缓存的数量
	const int32 queue_player_count = 1;					//排队单次进入玩家数量
#else
	const int32 init_normal_max = 1500;					//普通玩家进入数量
	const int32 init_vip_max = 200;						//vip+gm账号通道
	const int32 init_player_max = init_normal_max + init_vip_max; //初始化角色数量（服务器最大数量）
	const int32 init_link_max = 500;					//排队数量
	const int32 init_link_more = 10;					//超了上限后给玩家提示
	const int32 init_socket_link = init_player_max + init_link_max;//可以维持连接的数量
	const int32 init_socket_more = init_socket_link + init_link_more;//连接数在角色的基础上多一些
	const int32 init_npc_max = 15000;					//初始化npc数量（服务器最大数量）
	const int32 init_cs_max = 3;						// 总共有多少个cs
	const int32 init_session_max = init_player_max * init_cs_max; // 总共有多少个链接玩家
	const int32 max_save_data_count = init_player_max + 1000;//数据缓存的数量
	const int32 queue_player_count = 1;					//排队单次进入玩家数量
#endif

	const int32 npc_arrary_index_begin = 100000;					// npc列表的起始id
	const int32 npc_arrary_index_max = 100000 + init_npc_max;		// npc列表的最大值

#pragma pack(pop)
}

#endif
