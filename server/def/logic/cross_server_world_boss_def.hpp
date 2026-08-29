/********************************************************************
  created: 2019年5月28日
  file base: cross_server_world_boss_def
  file ext: hpp
  author: zhaoyuming
  purpose: 
*********************************************************************/
#ifndef _CROSS_SERVER_WORLD_BOSS_DEF_HPP_
#define _CROSS_SERVER_WORLD_BOSS_DEF_HPP_

#include "char_def.hpp"
#include "chat_def.hpp"

namespace faith 
{
#pragma pack(push,1)	
	const int32							m_map_max_boss = 3;										//每个地图生成的boss数量
	const int32							map_max = 4;											//创建的活动地图数量
	const int32							m_boss_max_num = m_map_max_boss*map_max;				//生成的总boss数量
	const int32							cross_boss_max_server = 10;								//跨服世界boss活动服务器最大数
	const int32							cross_boss_map_id_tmp = 71211109;						//活动地图模板id
	const int32							legion_act_num = 10;									//最大奖励数量
	const int32							act_num = 100;											//最大军团排名数量
	const int32							cross_boss_activity_notice_id = 93000287;				//跨服世界boss活动公告
	const int32							cross_boss_activity_ready_notice_id = 93000288;			//跨服世界boss活动准备公告
	const int32							cross_boss_activity_begin_notice_id = 93000289;			//跨服世界boss活动开始公告
	const int32							cross_bosss_legion_convene_notice_id = 93000290;		//跨服世界boss军团召集公告
	const int32							cross_boss_be_kill_have_legion_notice_id = 93000278;	//跨服世界boss被击杀公告有军团
	const int32							cross_boss_be_kill_not_have_legion_notice_id = 93000291;//跨服世界boss被击杀公告无军团
	const int32							cross_boss_be_kill_award_notice_id = 93000292;			//跨服世界boss奖励公告
	const int32							ranking_list_leght = 100;								//跨服世界排行榜最大数量
	const int32							cross_boss_map_template_num = 4;						//跨服世界boss获取地图模板id遍历是的一组数据的长度 (1个地图id后面3和bossid)




	struct cross_world_boss_info_to_db
	{
		int32						boss_template_id;
		int32						boss_is_dead;
		int32						kill_boss_time;
		int32						kill_boss_player_server_id;
		guid_64						kill_boss_player_guid;
		xchar						kill_boss_player_name[max_name_size + 1];
		guid_64						kill_boss_legion_guid;
		xchar						kill_boss_legion_name[max_name_size + 1];
		cross_world_boss_info_to_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(cross_world_boss_info_to_db));
		}
	};

	struct cross_boss_info
	{
		int32						boss_template_id;
		bool						is_dead;
		xchar						kill_boss_play_name[max_name_size + 1];
		xchar						kill_boss_play_legion_name[max_name_size + 1];
		int32						boss_be_kill_time;
		int32						kill_boss_server_id;
		guid_64						kill_boss_legion_id;
		guid_64						kill_boss_play_id;
		cross_boss_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(cross_boss_info));
		}
		void set_play_name(const xstring play_name)
		{
			memset(kill_boss_play_name, 0, sizeof(kill_boss_play_name));
			memcpy(kill_boss_play_name, play_name.c_str(), play_name.size() > sizeof(kill_boss_play_name) ?  sizeof(kill_boss_play_name) : play_name.size());
		}
		void set_legion_name(const xstring legion_name)
		{
			memset(kill_boss_play_legion_name, 0, sizeof(kill_boss_play_legion_name));
			memcpy(kill_boss_play_legion_name, legion_name.c_str(), legion_name.size() > sizeof(kill_boss_play_legion_name) ? sizeof(kill_boss_play_legion_name) : legion_name.size());
		}
		void operator = (const cross_boss_info* boss_info)
		{
			memset(this, 0, sizeof(cross_boss_info));
			memcpy(this, boss_info, sizeof(cross_boss_info));
		}
	};
	
	struct cross_boss_notice
	{
		int32			boss_template;
		int32			notice_id;
		int32			item_num;
		s_item_info		data_ary[chat_max_item];
		xchar			notice_string[globel_message_max_size + 1];

		cross_boss_notice()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(cross_boss_notice));
		}
	};

	struct cross_boss_level
	{
		int32						server_id;
		int32						true_boss_level;
		int32						false_boss_level;
		cross_boss_level()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(cross_boss_level));
		}
		bool is_valid()
		{
			return server_id > 0;
		}
	};

	struct activity_legion_play
	{
		guid_64 legion_guid;
		guid_64 play_guid;
		activity_legion_play()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(activity_legion_play));
		}
	};
	typedef std::map<guid_64, activity_legion_play> activity_legion_play_map;
#pragma pack(pop)
}
#endif
