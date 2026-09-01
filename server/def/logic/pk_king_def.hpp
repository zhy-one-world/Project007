/********************************************************************
created: 2019年6月17日
file base: pk_king_def
file ext: hpp
author: zhaoyuming
purpose:
*********************************************************************/
#ifndef _PK_KING_DEF_HPP_
#define _PK_KING_DEF_HPP_

#include "char_def.hpp"
#include "game_enum_def_s.h"

namespace faith
{
#pragma pack(push,1)

	const int32							pk_king_max_map_num = 100;								//Pk之王战斗地图最大数量	
	const int32							pk_king_max_play_num = 1200;							//Pk之王待机地图最大玩家数量
	const int32							pk_king_transfer_buff_id = 10990011;					//Pk之王传送特效BuffId
	const int32							pk_king_transfer_god_buff_id = 10992300;				//Pk之王传送无敌BuffId
	const int32							pk_king_weaken_player_buff_id = 1010000;				//Pk之王削弱角色属性BuffId
	const int32							pk_king_transfer_tick_time = 3;							//Pk之王阶段传送秒数
	const int32							pk_king_robot_template_id = 76991113;					//Pk之王玩家模板Id

	enum e_pk_king_time_type
	{	
		e_pk_king_time_type_first,				//第一阶段
		e_pk_king_time_type_second,				//第二阶段
		e_pk_king_time_type_thirdly,			//第三阶段
		e_pk_king_time_type_end,				//死斗阶段
		e_pk_king_time_type_max
	};

	enum e_pk_king_time_state_type
	{
		e_pk_king_time_state_type_setout,		//准备阶段
		e_pk_king_time_state_type_begin,		//开始阶段
	};

	enum e_pk_king_map_sub_type
	{
		e_pk_king_map_sub_type_wait,			//待机地图
		e_pk_king_map_sub_type_combat			//战斗地图
	};

	enum e_pk_king_add_score_type
	{
		e_pk_king_add_score_type_time,			//刷新时间
		e_pk_king_add_score_type_score,			//普通积分
		e_pk_king_add_score_type_hight_score,	//高倍积分
		e_pk_king_add_score_type_max
	};

	struct s_pk_king_player_info
	{
		guid_64						role_guid;						//角色Guid
		xchar						role_name[max_name_size + 1];	//角色名称
		int32						server_id;						//角色服务器Id
		int32						class_type;						//职业
		int32						role_level;						//等级
		int32						score_num;						//角色分数
		int32						kill_num;						//击杀总数
		int32						high_kill_num;					//最高连杀数
		int32						cur_kill_num;					//当前连杀数
		int32						next_send_kill_num;				//上次发送连杀数(已弃用)
		s_map_pos					bron_pos;						//复活点配置
		s_map_pos					tansfer_pos;					//传送点配置
		bool						is_dead;						//是否在死斗中死亡
		bool						is_robot;						//是否是机器人
		bool						is_send_reward;					//是否发放过奖励
		bool						is_send_break_line;				//是否发送过重连信息
		s_pk_king_player_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_king_player_info));
		}
		bool is_valid()
		{
			return role_guid.is_valid();
		}
		void set_role_name(const xstring play_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, play_name.c_str(), play_name.size() > sizeof(role_name) ? sizeof(role_name) : play_name.size());
		}
		bool operator < (const s_pk_king_player_info& other_info) const
		{
			return score_num < other_info.score_num;
		}

		bool operator > (const s_pk_king_player_info& other_info) const
		{
			return score_num > other_info.score_num;
		}
	};

	struct s_pk_king_prop_npc_info									//Pk之王道具Npc信息
	{
		guid_64						role_guid;						//NpcGuid
		int32						npc_tem_id;						//Npc模板Id
		s_map_pos					bron_pos;						//刷新点
		bool						is_dead;						//是否死亡
		s_pk_king_prop_npc_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_king_prop_npc_info));
		}
	};

	struct s_pk_king_play_rank_info									//用于战力排名
	{
		guid_64						role_guid;						//玩家Guid
		int64						role_gs;						//战力值

		s_pk_king_play_rank_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_king_play_rank_info));
		}

		bool operator < (const s_pk_king_play_rank_info& other_info) const
		{
			return role_gs < other_info.role_gs;
		}

		bool operator > (const s_pk_king_play_rank_info& other_info) const
		{
			return role_gs > other_info.role_gs;
		}
	};

	struct s_pk_king_map_info
	{
		guid_64						map_guid;						//地图Guid
		int32						map_state;						//地图阶段
	
		s_pk_king_map_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_pk_king_map_info));
		}
	};

	struct s_break_line_login_info
	{
		guid_64	role_guid;
		guid_64	map_guid;
		int32	server_id;
		int32	active_type;
		int32	map_template_id;
		int64	expiry_time;
		int64	remove_time;
		bool	is_send_msg;
		s_break_line_login_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_break_line_login_info));
		}
	};

#pragma pack(pop)
}
#endif