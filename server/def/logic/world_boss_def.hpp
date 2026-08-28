/********************************************************************
created: 2016年10月17日
file base: world boss
file ext: hpp
author: lxy

purpose: about world boss
*********************************************************************/


#ifndef _WORLD_BOSS_DEF_HPP_
#define _WORLD_BOSS_DEF_HPP_


//#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace hld
{
#pragma pack(push,1)
	const int32 max_world_boss_and_gold_army_num = 30;
	const int32 max_world_boss_reward_num = 5;
	const int32	world_boss_line = 1;									//世界boss所在线路
	const int32 max_world_boss_damage_list_num = 50;					//世界boss最大记录伤害列表
	const int32 max_world_boss_arr_num = 4;								//低中高世界boss数组最大数量  
	const int32	world_boss_first_stage_notice_id = 93000431;			//世界boss活动第一阶段开启公告
	const int32	world_boss_second_stage_notice_id = 93000432;			//世界boss活动第二阶段开启公告
	const int32	world_boss_third_stage_notice_id = 93000433;			//世界boss活动第三阶段开启公告
	const int32 world_boss_add_other_score_power = 3;					//每次加额外积分的倍率
	
	enum e_world_boss_level
	{
		e_world_boss_level_1,
		e_world_boss_level_2,
		e_world_boss_level_3,
		e_world_boss_level_max,
	};

	enum e_world_boss_respawn_unit
	{
		e_world_boss_respawn_unit_0,
		e_world_boss_respawn_unit_1,
		e_world_boss_respawn_unit_2,
		e_world_boss_respawn_unit_3,
		e_world_boss_respawn_unit_4,
		e_world_boss_respawn_unit_5,
		e_world_boss_respawn_unit_max,
	};

	struct s_world_boss_info
	{
		int32					world_boss_npc_array_index;
		int32					npc_respawn_point_template_id;
		int32					is_alive;
		int32					line_id;
		int64					last_respawn_time;
		int32					respawn_phase;
		xchar					killer_name[max_name_size + 1];
		xchar					first_killer_name[max_name_size + 1];
		int32					first_killed_time;
		int32					is_get_prize;
		int32					boss_daily_refresh;
		s_world_boss_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_world_boss_ready_info
	{
		guid_64					npc_guid;
		int32					npc_respawn_point_template_id;
		int32					line_id;
		int32					arr_index;
		s_world_boss_ready_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};


#pragma pack(pop)
}

#endif