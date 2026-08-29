/********************************************************************
created: 2016年8月9日13:56:28
file base: pk_def
file ext: hpp
author: zhangminghai

purpose: about pk and community system's store_mgr
*********************************************************************/

#ifndef _PK_DEF_HPP_
#define _PK_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace faith
{
#pragma pack(push,1)
#define PER_MIN_CLEAR_PK_VALUE		10
#define	FIRST_HURT_ADDED_PK_VALUE   10
#define	KILL_PLAYER_PER_PK_VALUE    100
	
	const int32 auto_fight_back_sec = 10;
	const int32 auto_fight_max_num = 10;

	enum e_community_type
	{
		e_community_type_npc_pet,			//玩家宠物
		e_community_type_player_group1,		//玩家阵营1
		e_community_type_player_group2,		//玩家阵营2
		e_community_type_player_group3,		//玩家阵营3
		e_community_type_player_group4,		//玩家阵营4
		e_community_type_player_group5,		//玩家阵营5
		e_community_type_player_group6,		//玩家阵营6
		e_community_type_player_yuliu1,		//阵营预留1
		e_community_type_player_yuliu2,		//阵营预留2
		e_community_type_common_monster,	//通用怪物阵营
		e_community_type_common_friend,		//通用友方阵营
		e_community_type_friend_npc_group1,	//友方npc阵营1
		e_community_type_friend_npc_group2,	//友方npc阵营2
		e_community_type_attack_player,		//进攻方玩家专用阵营
		e_community_type_attack_car,		//进攻方战车专用阵营
		e_community_type_guard_player,		//防守方玩家专用阵营
		e_community_type_guard_car,			//防守方战车专用阵营
		e_community_type_guard_boss,		//防守方boss专用阵营
		e_community_type_max,
	};
	
	enum e_community_state
	{
		e_community_state_out = 0,			//不在阵营中			
		e_community_state_in,				//在阵营中
	};
	
	struct s_community_info
	{
		int32 all_community[e_community_type_max];//1表示在这个阵营中0表示不在此阵营
		s_community_info()
		{
			reset();
		}
		void reset()
		{
			memset(all_community, 0, sizeof(all_community));
		}
	};

	struct s_community_info_db
	{
		guid_64 role_guid;
		s_community_info data_info;
		s_community_info_db()
		{
			role_guid.clear_data();
			data_info.reset();
		}
	};

	struct s_community_group_relation
	{
		int32 data_ary[e_community_type_max][e_community_type_max];	//0表示对立	1表示友好
		s_community_group_relation()
		{
			reset();
		}
		void reset()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	
	struct s_is_skill_target_param
	{
		int32		skill_target_type;
		int32		region_type;
		int32		map_pk_type;
		int32		sender_unit_type;
		int32		target_unit_type;
		int32		sender_line_id;
		int32		target_line_id;
		int32		sender_community_type;
		int32		target_community_type;
		int32		sender_pk_mode;
		int32		target_pk_mode;
		int32		sender_move_state;
		int32		target_move_state;
		int32		target_pk_value;
		int32		sender_sever_id;
		int32		target_sever_id;
		int32		cur_server_id;
		bool		is_in_same_team;
		bool		is_in_same_legion;
		bool		is_self;
		bool		is_self_pet;
		bool		is_self_owner;
		bool		is_boss;
		bool		is_fight_back_state;
		bool		is_team_protect;
		bool		is_legion_protect;
		s_is_skill_target_param()
		{
			reset();
		}
		void reset()
		{
			is_in_same_team = false;
			is_in_same_legion = false;
			is_self = false;
			is_self_pet = false;
			is_self_owner = false;
			is_boss = false;
			is_fight_back_state = false;
			is_team_protect = false;
			is_legion_protect = false;
		}
	};
	
	enum e_pk_cfg_type
	{
		e_pk_cfg_type_pk_mode_end = 4, //1-4位都是pk_mode
		e_pk_cfg_type_is_fight_back = 5,
		e_pk_cfg_type_team_protect = 6,
		e_pk_cfg_type_legion_protect = 7,
	};

#pragma pack(pop)
}
#endif