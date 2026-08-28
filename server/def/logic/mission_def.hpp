/********************************************************************
created: 2016/05/05
file base: mission_def
file ext: hpp
author: 张宇翔

purpose:
*********************************************************************/
#ifndef _MISSION_DEF_HPP_
#define _MISSION_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace hld
{
#pragma pack(push,1)
	const int32 MAX_MISSION_LINE_NUM = 4;					// 一共4种任务
	const int32 TEMPLATE_ONE_NPC_OR_ITEM_INFO_LEN = 2;		// 模板中一个目标长度为两位,id,num
	const float MIN_DISTENCE_TO_DONE_MISSION = 300000.0;	// 完成任务最小距离
	const int32 LOCATION_ARRAY_LEN = 3;						// 坐标数组为三位
	const int32 MAX_POOL_NUM = 5;							// 日常,讨伐任务池最大数
	const int32 MISSION_START_ID = 24000001;				// 任务的起始id
	const int32 DAILY_MISSION_MAX = 10;						// 日常任务上限10
	const int32 AGAINST_MISSION_MAX = 5;					// 讨伐任务上限5
	const int32 MARRY_MISSION_MAX = 5;						// 结婚任务上限5
	const int32 MAX_AWARD_ITEM = 3;
	const int32 MAX_DALIY_STAR_NUM = 5;
	const int32 max_mission_money_reward_type = 5;			// 任务完成最多货币奖励类型
	
	const int32 daily_mission_unlock_id = 117000069;
	const int32 against_mission_unlock_id = 117000070;
	const int32 first_recharge_time_mission_id = 15000633;  //设置首充时间任务id

	enum e_mission_state//任务状态
	{
		e_mission_state_none,//无标记
		e_mission_state_cannot_accept,//条件不足无法接取
		e_mission_state_can_accept,//未接取
		e_mission_state_accepted,//接取但是未完成
		e_mission_state_done,//目标达成
		e_mission_state_chat,//对话完成
		e_mission_state_can_finish,//可交付
		e_mission_state_finished,//已交付
		e_mission_state_max,
	};
	enum e_npc_mark
	{
		e_npc_mark_none,//无标记
		e_npc_mark_cannot_accept,//不可接灰色叹号
		e_npc_mark_can_accept,//可接黄色叹号
		e_npc_mark_accepted,//已接未完成灰色问号
		e_npc_mark_can_finish,//完成黄色叹号
	};
	enum e_mission_operate_end_state
	{
		e_mission_operate_end_state_system_wrong,//失败
		e_mission_operate_end_state_succeed,//成功
		e_mission_operate_end_state_bag_full,//包满
		e_mission_operate_end_state_too_far,//距离过远
		e_mission_operate_end_state_not_enough_money,//钱不够
		e_mission_operate_end_state_too_fast//操作太频繁
	};
	enum e_mission_inst_data
	{
		e_mission_inst_data_id,//任务模板id
		e_mission_inst_type,//任务类型
		e_mission_inst_slot,//任务槽位
		e_mission_inst_data_goal1_num,//任务目标1的当前数量
		e_mission_inst_data_goal2_num,//任务目标2的当前数量
		e_mission_inst_data_goal3_num,//任务目标3的当前数量
		e_mission_inst_data_goal4_num,//任务目标4的当前数量
		e_mission_inst_data_star_level,//日常任务的星级
		e_mission_inst_data_mission_state,//任务的状态
		e_mission_inst_data_finish_item_id1,//奖励物品8
		e_mission_inst_data_finish_item_num1,//数量1 9
		e_mission_inst_data_finish_item_id2,//10
		e_mission_inst_data_finish_item_num2,
		e_mission_inst_data_finish_item_id3,
		e_mission_inst_data_finish_item_num3,
		e_mission_inst_data_finish_item_id4,
		e_mission_inst_data_finish_item_num4,
		e_mission_inst_get_time,//接受任务时间
		e_mission_inst_data_max,
	};
	enum e_mission_finish_type
	{
		e_mission_finish_type_normal,//正常完成
		e_mission_finish_type_double,//双倍奖励
		e_mission_finish_type_endall,//一键完成
	};
	enum e_mission_finish_award_type
	{
		e_mission_finish_award_type_none,
		e_mission_finish_award_type_normal,//常规点数奖励
		e_mission_finish_award_type_rate,//倍率奖励
		e_mission_finish_award_type_world,

	};

	enum e_finish_activate_type
	{
		e_finish_activate_type_non,				//无
		e_finish_activate_type_skill,			//技能
		e_finish_activate_type_wing,			//翅膀
		e_finish_activate_type_mount,			//坐骑
		e_finish_activate_type_item,			//物品
		e_finish_activate_type_sprite_jiban,	//精灵羁绊
		e_finish_activate_type_sprite_qiyuan,	//精灵奇缘
		e_finish_activate_type_wing_spirit,		//注灵
		e_finish_activate_type_wing_feather,	//翎羽
		e_finish_activate_type_wing_soul,		//注魂
		e_finish_activate_type_meditation,      //冥想
		e_finish_activate_type_max
	};


	enum e_finish_activate_info
	{
		e_finish_activate_info_type,			//类型，要激活什么，是 技能还是翅膀还是啥
		e_finish_activate_info_barserker,		//战士，与 职业 id完全相同
		e_finish_activate_info_wizard,			//巫师，与 职业 id完全相同
		e_finish_activate_info_guardian,		//守护者，与 职业 id完全相同
		e_finish_activate_info_assassinator,	//刺客，与 职业 id完全相同
		e_finish_activate_info_max
	};

	struct s_mission_info
	{
		int32 data_ary[e_mission_inst_data_max];
		s_mission_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}

		int32 get_mission_id()
		{
			return data_ary[e_mission_inst_data_id];
		}

		bool is_valid()
		{
			if (get_mission_id() <= 0)
			{
				return false;
			}
			return true;
		}
	};

	struct s_mission_info_db
	{
		guid_64 role_guid;
		s_mission_info data_info;
	};

	enum e_mission_goal_tuple
	{
		e_mission_goal_tuple_id,
		e_mission_goal_tuple_num,
		e_mission_goal_tuple_max
	};

#pragma pack(pop)
}
#endif