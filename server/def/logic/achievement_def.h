#pragma once
/********************************************************************
created: 2016/07/14
file base: achievement_def
file ext: hpp
author: 张宇翔

purpose:
*********************************************************************/
#ifndef _ACHIEVEMENT_DEF_HPP_
#define _ACHIEVEMENT_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace faith
{
#pragma pack(push,1)

	const int32			MAX_ACHIEVEMENT_NUM = 320;											//成就项目最大数量
//	const int32			MAX_ACHIEVEMENT_PAGE_NUM = 8;										//分组页签数(从1开始)

	enum e_achievement_operate_type
	{
		eaot_none,//缺省
		eaot_finish,//领取

	};

	enum e_achievement_type
	{
		eat_npc_kill_count,//杀怪统计
		eat_boss_kill_count,//boss击杀统计
		eat_compose_count,//合成统计
		eat_item_upgrade_count,//强化次数统计
		eat_item_addon_count,//追加次数统计
		eat_item_upgrade_state_check,//装备强化某星判断
		eat_item_addon_state_check,//装备追加某级判断
		eat_role_level_count,//等级判断
		eat_role_transmigration_level_count,//转生等级
		eat_skill_level_check,//技能等级达到几级判断
		eat_login_continuity_count,//连续登陆统计,-------------------------废弃
		eat_login_all_count,//累计登陆统计
		eat_current_money_check,//当前金钱判断
		eat_all_money_count,//累计金钱判断
		eat_easy_fuben_count,//简单副本通关统计
		eat_medium_fuben_count,//普通副本通关统计
		eat_hard_fuben_count,//困难副本通关统计
		eat_military_rank_check,//军衔等级判断
		eat_main_mission_check,//主线任务判断
		eat_add_friend_count,//加好友统计
		eat_add_or_create_group_team,//加入或创建队伍统计
		eat_add_or_create_group_legion,//加入或创建战盟统计
		eat_item_inherit_count,//传承统计----------------------------------废弃
		eat_all_skill_level_count,//全部技能等级统计

		eat_lingyu_grade_count,					// 翎羽总阶----------------废弃
		eat_wing_grade_count,					// 羽翼等阶统计

		eat_mount_grade_count,					// 坐骑最高阶统计
		eat_mount_type,							// 坐骑类型
		eat_mount_illusion_count,				// 坐骑幻化总次数统计
		
		eat_spirit_num_count,					// 拥有精灵数量统计
		eat_spirit_level_count,					// 精灵总等级统计

		eat_belief_grade_count,					// 信仰总等阶统计
		
		eat_patron_saint_activation_num_count,	// 守护神激活数量统计
		eat_patron_saint_grade_num_count,		// 守护神总等级统计

		eat_illustration_finish_num_count,		// 图鉴完成数量统计

		eat_legion_boss,						// 军团boss

		eat_run_equip_num_count,				// 符文最高装备槽数 符文 = 元素之心
		eat_runc_level_count,					// 同时装备符文之和总等级

		eat_multiple_fuben_has_finished_count,			// 多人副本完成数量

		eat_role_marry,							// 角色结婚

		eat_max,
	};

	enum e_achievement_state
	{
		eas_accept,//未完成
		eas_can_finish,//已完成
		eas_done,//已领取
	};
	enum e_achievement_inst_data
	{
		eaid_id,//模板id
		eaid_current_num,//当前进度
		eaid_current_state,//当前状态
		eaid_max,
	};
	enum e_achievement_error_type
	{
		eaet_system_error,
		eaet_success,
		eaet_operation_illegal,

	};
	struct s_achievement_info
	{
		int32 data_ary[eaid_max];
		s_achievement_info()
		{
			reset();
		}
		void reset()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
		bool  operator==(const s_achievement_info &a1) const 
		{
			for (int32 i = 0; i < eaid_max; i++)
			{
				if (a1.data_ary[i] != data_ary[i])
				{
					return false;
				}
			}
			return true;
		}
	};
	struct s_achievement_info_db
	{
		guid_64 role_guid;
		s_achievement_info data_info;
	};
#pragma pack(pop)
}
#endif
