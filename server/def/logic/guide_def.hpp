#pragma once
/********************************************************************
created: 2017/7/15
file base: guide_def
file ext: hpp
author: wucun

purpose:
*********************************************************************/

#ifndef _GUIDE_DEF_HPP_
#define _GUIDE_DEF_HPP_


#include <cassert>
#include "logic/type_def.hpp"

namespace faith
{
#pragma pack(push,1)
	
	const int32			max_guide_record_num = 400;
	const int32			guide_interval = 100;										//不同触发条件的引导间隔

	const int32			warning_guide_skill_template_id = 13500162;					//预警引导boss技能id
	const int32			first_raid_auto_battke_guide_trigger_id = 118013700;		//第一次进入魔龙苏醒自动战斗引导触发id
	const int32         mount_unlock_guide_trigger_id = 0;							//坐骑解锁引导id
	const int32			welfare_type_total_login_guide_trigger_id = 118012000;		//累积登陆引导触发id
	const int32			welfare_type_total_login_max_index = 9;						//累积登陆福利最大索引值
	const int32			daily_prompt_guide_trigger_id = 118016500;					//日常提示引导ID
	const int32			new_tinder_guide_trigger_id = 118016600;					//火种升级引导
	const int32			tinder_unlock_guide_trigger_id = 118011500;					//火种功能开启引导
	const int32			tinder_function_unlock_id = 117000060;						//火种解锁id
	const int32         first_recharge_guide_trigger_id = 118017500;				//首充引导触发id
	const int32         update_mount_guide_trigger_id = 118016800;					//坐骑升级引导
	const int32			service_rank_guide_trigger_id = 118018000;					//开服庆典引导
	const int32         first_exp_fuben_guide_trigger_id = 118018300;               //第一次进入经验副本引导 
	const int32         fly_sprint_guide_id = 118026800;							//飞行冲刺引导ID
	const int32			first_upgrade_buff_guide_id = 118029200;					//首次触发强化Buff引导Id
	const int32			first_upgrade_buff_need_level = 3;							//首次触发强化引导Id需要等级

	struct s_record_info
	{
		int32	guide_trigger_id;
		int32	state;
		s_record_info()
		{
			clear_data();
		}
		void clear_data()
		{
			guide_trigger_id = 0;
			state = 0;
		}
	};

	enum need_mission_tuple
	{
		need_mission_tuple_type,
		need_mission_tuple_id,
		need_mission_tuple_state,
		need_mission_tuple_max
	};

	enum mission_trigge_type
	{
		mission_trigge_type_mission_id,
		mission_trigge_type_mission_start,
		mission_trigge_type_trigge_guide_id,
		mission_trigge_type_max
	};

	enum map_firt_hit_trigge_type
	{
		map_firt_hit_trigge_type_map_type,
		map_firt_hit_trigge_type_guide_id,
		map_firt_hit_trigge_type_max
	};
#pragma pack(pop)
}
#endif
