/********************************************************************
created: 2019年5月24日
file base: cross_server_world_boss_msg
file ext: hpp
author: zhaoyuming

purpose: 
*********************************************************************/

#ifndef _FAITH_CROSS_SERVER_WORLD_BOSS_MSG_HPP_
#define _FAITH_CROSS_SERVER_WORLD_BOSS_MSG_HPP_

#include "logic/type_def.hpp"
#include "logic/map_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/cross_server_world_boss_def.hpp"
#include "legion_def.hpp"
#include "internet/ws2cs.pb.h"
namespace faith
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_cs2ws_cross_server_world_boss_creat_boss_begin = faith::e_msg_base_cross_server_world_boss, 
		e_msgindex_ws2cs_cross_server_world_boss_creat_boss,		
		e_msgindex_ws2ws_get_cross_server_level,					//获得服务器组中的服务器等级
		e_msgindex_ws2ws_get_cross_server_level_end,				//服务器组中的服务器返回自己的服务器等级,				
		e_msgindex_ws2ws_cross_world_boss_map_result,
		e_msgindex_cs2ws_cross_server_kill_world_boss,				//cs向ws发送boss死亡
		e_msgindex_ws2cs_cross_server_kill_boss,					
		e_msgindex_cs2ws_make_cross_server_world_legion_award,		//发送活动奖励
		e_msgindex_ws2ws_make_cross_server_world_legion_award,
		e_msgindex_ws2dp_save_cross_server_world_boss_info,			//ws发送保存boss信息
		e_msgindex_ws2dp_load_cross_server_world_boss_info,			//ws发送请求获得boss信息
		e_msgindex_dp2ws_load_cross_server_world_boss_info,			//dp发送给ws的boss信息
		e_msgindex_cs2ws_cross_boss_map_game_over,					//通知ws这个地图游戏结束可
		e_msgindex_ws2ws_refresh_kill_boss_show_info,				//一次发一个boss的数据
		e_msgindex_ws2ws_refresh_kill_boss_show_info_all,			//一次发所用boss的数据
		e_msgindex_ws2ws_grant_kill_boss_legion_award,				//发放击杀boss军团的奖励
		e_msg_gate2ws_send_boss_ranking_list,						//跨服世界boss排行榜
		e_msg_cs2ws_send_kill_boss_notic,
		e_msg_gate2ws_send_kill_boss_notice,
		e_msg_ws2ws_get_legion_chief_guid,
		e_msg_ws2ws_set_legion_chief_guid,
		e_msg_ws2ws_send_legion_welfare_rank,
	};	

	struct ws2ws_transfer_cross_world_boss_map_result : public faith::packet_base
	{
		int32						result;
		guid_64						role_guid;
		int32						map_temp_id;
		guid_64						map_guid;
		int32						war_idex;
		int32						activity_type;
		int32						server_id;
		ws2ws_transfer_cross_world_boss_map_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_world_boss_map_result;
		}
	};

	struct ws2dp_save_cross_server_world_boss_msg_all : public faith::packet_base
	{
		int32						boss_template_id;
		int32						boss_is_dead;
		int32						kill_boss_time;
		int32						kill_boss_player_server_id;
		xchar						kill_boss_player_name[max_name_size + 1];
		guid_64						kill_boss_player_guid;
		xchar						kill_boss_legion_name[max_name_size + 1];
		guid_64						kill_boss_legion_guid;
		ws2dp_save_cross_server_world_boss_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_cross_server_world_boss_info;
		}
		void set_role_name(xstring role_name)
		{
			memset(kill_boss_player_name, 0, sizeof(kill_boss_player_name));
			memcpy(kill_boss_player_name, role_name.c_str(), sizeof(kill_boss_player_name) > role_name.size() ? role_name.size() : sizeof(kill_boss_player_name));
		}
		void set_legion_name(xstring role_name)
		{
			memset(kill_boss_legion_name, 0, sizeof(kill_boss_legion_name));
			memcpy(kill_boss_legion_name, role_name.c_str(), sizeof(kill_boss_legion_name) > role_name.size() ? role_name.size() : sizeof(kill_boss_legion_name));
		}
	};
	

	struct ws2dp_load_cross_server_world_boss_msg_all : public faith::packet_base
	{
		ws2dp_load_cross_server_world_boss_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_cross_server_world_boss_info;
		}
	};


	struct dp2ws_load_cross_server_world_boss_msg_all : public faith::packet_base
	{
		int32	data_num;
		cross_world_boss_info_to_db dp_info[m_boss_max_num];
		dp2ws_load_cross_server_world_boss_msg_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_cross_server_world_boss_info;
		}
	};

	struct cs2ws_cross_server_kill_world_boss : public faith::packet_base
	{
		int32 be_kill_boss_guid;
		guid_64 killer_guid;
		cs2ws_cross_server_kill_world_boss()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cross_server_kill_world_boss;
		}
	};
	struct ws2cs_cross_server_world_boss_creat_boss : public faith::packet_base
	{
		int32	true_boss_level;
		int32	false_boss_level;
		guid_64 from_map_guid;
		ws2cs_cross_server_world_boss_creat_boss()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_cross_server_world_boss_creat_boss;
		}
		void to_proto(faith::ws2cs_proto::cross_server_world_boss_creat_boss& msg)
		{
			msg.set_true_boss_level(true_boss_level);
			msg.set_false_boss_level(false_boss_level);
			msg.set_from_map_guid(from_map_guid.server_64);

		}
		void from_proto(const faith::ws2cs_proto::cross_server_world_boss_creat_boss& msg)
		{
			true_boss_level = msg.true_boss_level();
			false_boss_level = msg.false_boss_level();
			from_map_guid.server_64 = msg.from_map_guid();
		}
	};

	struct cs2ws_cross_server_world_boss_creat_boss_begin : public packet_base //发送开始创建活动boss
	{
		guid_64 from_map_guid;

		cs2ws_cross_server_world_boss_creat_boss_begin()
		{
			memset(this, 0, sizeof(cs2ws_cross_server_world_boss_creat_boss_begin));
			wheader = e_msgindex_cs2ws_cross_server_world_boss_creat_boss_begin;
		}
	};

	//获取服务器等级
	struct ws2ws_get_cross_server_level : public faith::packet_base
	{
		int32	server_id;	//发送请求获得服务器等级的服务器id
		ws2ws_get_cross_server_level()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_cross_server_level;
		}
	};
	struct ws2ws_get_cross_server_level_end : public faith::packet_base
	{
		int32	true_server_level;	//服务器等级真boss
		int32	false_server_level;	//服务器等级假boss
		int32	server_id;		//服务器等级对应的服务器id
		ws2ws_get_cross_server_level_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_cross_server_level_end;
		}
	};
	struct cs2ws_make_cross_server_world_legion_award : public faith::packet_base
	{
		int32 boss_id;
		guid_64 kill_boss_play_guid;
		int32 score_num;
		score_record legion_score_rank[act_num];
		int32 member_num;
		activity_legion_play	member_scores[max_hurt_monster_player_num];
		cs2ws_make_cross_server_world_legion_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_make_cross_server_world_legion_award;
		}
	};
	
	struct ws2ws_grant_kill_boss_legion_award : public faith::packet_base
	{
		int32 boss_id;
		guid_64 kill_boss_legion_guid;
		int32	play_num;										//参与数量
		guid_64	play_guid_list[max_hurt_monster_player_num];	//参与活动的玩家
		ws2ws_grant_kill_boss_legion_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_grant_kill_boss_legion_award;
		}
	};
	

	struct ws2ws_make_cross_server_world_legion_award : public faith::packet_base
	{
		guid_64 legion_guid;
		int32	boss_tem_id;
		int32	score_rank;
		int32	play_num;										//参与数量
		guid_64	play_guid_list[max_hurt_monster_player_num];	//参与活动的玩家
		ws2ws_make_cross_server_world_legion_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_make_cross_server_world_legion_award;
		}
	};


	struct cs2ws_cross_boss_map_game_over : public faith::packet_base
	{
		int32		map_template_id;
		cs2ws_cross_boss_map_game_over()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cross_boss_map_game_over;
		}
	};
	struct ws2ws_cross_refresh_kill_boss_show_info : public faith::packet_base
	{
		cross_boss_info	boss_info;
		ws2ws_cross_refresh_kill_boss_show_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_refresh_kill_boss_show_info;
		}
	};
	
	struct ws2ws_cross_refresh_kill_boss_show_info_all : public faith::packet_base
	{
		int32 data_num;
		cross_boss_info	boss_info[m_boss_max_num];
		ws2ws_cross_refresh_kill_boss_show_info_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_refresh_kill_boss_show_info_all;
		}
	};

	struct gate2ws_send_boss_ranking_list : public packet_base
	{
		int32							boss_id;
		int32							data_num;
		s_ranking_player_info			data_ary[ranking_list_leght];
		gate2ws_send_boss_ranking_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_send_boss_ranking_list;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_ary - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_ranking_player_info));
		}
	};

	struct cs2ws_send_kill_boss_notic : public packet_base
	{
		cross_boss_notice item_info_ptr;
		cs2ws_send_kill_boss_notic()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_cs2ws_send_kill_boss_notic;
		}
	};


	struct gate2ws_send_kill_boss_notic : public packet_base
	{
		cross_boss_notice item_info_ptr;
		gate2ws_send_kill_boss_notic()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_gate2ws_send_kill_boss_notice;
		}
	};

	struct ws2ws_get_legion_chief_guid : public packet_base
	{
		guid_64 legion_guid;
		int32 rank_type;
		ws2ws_get_legion_chief_guid()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2ws_get_legion_chief_guid;
		}
	};

	struct ws2ws_set_legion_chief_guid : public packet_base
	{
		guid_64 legion_guid;
		guid_64 legion_chief_guid;
		xchar legion_chief_name[max_name_size + 1];
		int32 legion_chief_vip_level;
		int32 rank_type;
		ws2ws_set_legion_chief_guid()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2ws_set_legion_chief_guid;
		}
		void set_legion_chief_name(xstring role_name)
		{
			memset(legion_chief_name, 0, sizeof(legion_chief_name));
			memcpy(legion_chief_name, role_name.c_str(), sizeof(legion_chief_name) > role_name.size() ? role_name.size() : sizeof(legion_chief_name));
		}
	};

	struct ws2ws_send_legion_welfare_rank : public packet_base
	{
		guid_64 legion_guid;
		int32  rank_index;
		ws2ws_send_legion_welfare_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_ws2ws_send_legion_welfare_rank;
		}
	};
#pragma pack(pop)
}
#endif