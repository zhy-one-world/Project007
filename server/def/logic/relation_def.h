/********************************************************************
	file name:	relation_def.h
	created:	2017/05/26 23:18
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _RELATION_DEF_H_
#define _RELATION_DEF_H_

#include "char_def.hpp"

namespace faith
{
#pragma pack(push,1)
	//最大好友数量
	//#define MAX_FRIEND_NUM	99
	const int32 MAX_FRIEND_NUM = 99;
	//查找玩家最大数量
	const int32 MAX_FIND_PLAYER_NUM = 30;
	//查找玩家间隔时间（秒）
	const int32 FIND_PLAYER_INTERVAL = 3000;
	//玩家推荐列表人数
	const int32 RECOMMENDED_PLAYER_NUM = 5;
	//最近聊天玩家数量
	const int32 FRIEND_CHAT_NUM = 10;
	
	//玩家更新好友数据间隔时间（毫秒）
	const int32 FRIEND_USERINFO_INTERVAL = 30000;

	const int32 TalkDateItem = 20;
	const int32 TalkWithNum = 10;
	const int32 PUSHRELATIONMINNUM = 1;
	const int32 IntervalReqState = 60;
	const int32 OtherReqAddMin = 5;

	const int32 SearchMax = 20;

	//好友状态
	enum e_relation_state
	{
		e_relation_offline,		//离线
		e_relation_online,		//在线
	};


	//上线提醒类型
	enum e_loginnotice_type
	{
		e_loginnotice_friend,	//好友
		e_loginnotice_marry,	//结婚
		e_loginnotice_master,	//师门
	};
	//////////////////////////////////////////////////////////////////////////
	//About Marrage
	//通信包类型定义
	//S:0申请/1确认/2取消/3答应/4拒绝/5离婚
	//C:0二级确认/1被求/2对方取消
	enum e_marry_packet_type
	{
		e_marry_packet_askmarry = 0,
		e_marry_packet_confirm = 1,
		e_marry_packet_cancel = 2,
		e_marry_packet_accept = 3,
		e_marry_packet_refuse = 4,
		e_marry_packet_divorce = 5,
		e_marry_packet_parade = 6,
	};

	//结婚用到物品
	#define MARRY_ITEM_LUXURY (48)
	#define MARRY_ITEM_NORMAL (59069)
	#define MARRY_ITEM_RESULT (49)
	//游街有限时间
	#define PARADE_VALIDTIME (60*60*2)
	//离婚到再婚时间
	#define MARRY_BREAK_TIME (60*60*24)
	//结婚级别限制
	#define MARRY_MIN_LEVEL (30)

	//拜堂步骤
	enum e_marry_process_step
	{
		e_marry_process_invalid,
		e_marry_process_story = 58,		  //步骤，同时代表故事id
		e_marry_process_action1 = 90103,	  //步骤，同时代表动作id
		e_marry_process_action2 = 90104,    //步骤，同时代表动作id
		e_marry_process_action3 = 90105,	  //步骤，同时代表动作id
		e_marry_process_over = 167002,	  //步骤，同时代表特效buffid
	};
	//烟花ID和位置
	#define MARRY_FIREWORKS_ID (4)
	#define MARRY_FIREWORKS_POS1 (ScenePos(12,19))
	#define MARRY_FIREWORKS_POS2 (ScenePos(15,19))
	#define MARRY_FIREWORKS_POS3 (ScenePos(11,23))
	#define MARRY_FIREWORKS_POS4 (ScenePos(16,23))
	//拜堂开始位置
	#define MARRY_PROCESS_POS1 (ScenePos(13,21))
	#define MARRY_PROCESS_POS2 (ScenePos(14,21))
	//拜堂结束位置
	#define MARRY_PROCESS_OVER_POS1 (ScenePos(14,16.5f))
	#define MARRY_PROCESS_OVER_POS2 (ScenePos(15,16.5f))
	//拜堂第一个走动最长延迟时间
	#define MARRY_PROCESS_MAXTIME (60)
	//拜堂每个步骤最长时间
	#define MARRY_STEP_INTERVAL (5)
	//拜堂最终发放的buff
	#define MARRY_PROCESS_BUFF (248010)
	//每天最大蹭经验数
	#define MAXSHAREMARRYCOINANDEXP 20
	//老公老婆称号
	#define MARRY_HUSBAND_TITLE (4)
	#define MARRY_WIFE_TITLE (5)
	//游街最长时间限制
	#define MARRY_PARADE_MAXTIME (150)
	//游街故事模式
	#define MARRY_PARADE_STORY (60)
	//游街使用NPC
	enum e_marry_parade_npc
	{
		e_marry_parade_frontnpc = 935,
		e_marry_parade_frontway = 85,

		e_marry_parade_busnpc = 934,
		e_marry_parade_busway = 58,

		e_marry_parade_backnpc = 935,
		e_marry_parade_backway = 31,
	};
	//巡逻落线
	#define WUYINGXUNZONG_DATAID 59
	//////////////////////////////////////////////////////////////////////////

	struct s_relation_info
	{
		guid_64	relation_guid;
		xchar	relation_name[max_name_size + 1];
		int64	gs_value;
		int32	data_array[e_relation_data_max];
		s_relation_info()
		{
			clear_data();
		}
		void clear_data()
		{
			relation_guid.clear_data();
			memset(relation_name, 0, sizeof(relation_name));
			gs_value = 0;
			memset(data_array, 0, sizeof(data_array));
		}
		bool  operator==(const s_relation_info &a1) const
		{
			if (relation_guid != a1.relation_guid ||
				strncmp(relation_name, a1.relation_name, max_name_size) != 0 || a1.gs_value != gs_value)
			{
				return false;
			}
			for (int32 i = 0; i < e_relation_data_max; ++i)
			{
				if (data_array[i] != a1.data_array[i])
				{
					return false;
				}
			}
			return true;
		}
	};

	struct s_relation_info_db
	{
		guid_64 role_guid;
		s_relation_info data_info;
	};

#pragma pack(pop)
}

#endif // !_GAMEDEFINE_RELATION_H_
