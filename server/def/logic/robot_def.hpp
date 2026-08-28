/********************************************************************
created: 2016/05/05
file base: robot_def
file ext: hpp
author: dengronggen

purpose:
*********************************************************************/
#ifndef _ROBOT_DEF_HPP_
#define _ROBOT_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "time_def.hpp"
#include "template/template_base.h"

namespace hld
{
#pragma pack(push,1)

	//充值
	const int32  first_charge_mission_id  = 15000640;
	const int32  first_charge_template_id_arry[4] = { 104000002, 104000003, 104000004, 104000006 }; //蓝砖充值 60 300 980 1980
	const int32  first_charge_reward_id   = 107010001;
	const int32  auto_charge_template_id  = 104000003;

	const int32 levelup_world_avg_level_time_hour = 5;				//自动gm命令追赶服务器等级时间
	const int32 levelup_world_days = 15;							//开服前7天自动追赶服务器等级
	const int32 probability_levelup_world_days = 9;					//第9天之后概率提升等级
	const int32 levelup_probability = 80;							//提升及等级概率 80%

	//物品
	const int32 auto_upgrade_item_interval_time = 30000;			//自动强化装备时间间隔
	const int32 team_robot_max = 1;									//队伍内机器人上限 

	//军团
	const int32 auto_opera_legion_time = 2 * hour_tick_time;			//自动加入军团事件120分钟
	const int32 auto_join_legion_time_have_jioned = 1 * day_tick_time; //加入过军团后退出就1天申请一次

	const int32 auto_quit_legion_robot_num = 5;						//军团超过5个机器人退出军团
	const int32 auto_quit_legion_leader_out_online_time = 3 * 24 * 60 * 60;	//军团长三天不在线退出军团
	const int32 request_join_legion_times_max = 5;					//机器人申请一个军团的次数上限

	//地图
	const int32 godness_trial_map_template_id = 71030001;			//女神的试炼副本id
	const int32 lord_trials_map_template_id_arry[4] = { 71040003, 71040012, 71040013, 71040014 };

	//好友
	const int32 request_add_friend_count = 5;					   //机器人最多加5次好友

	//活动
	const int32 join_pk_king_num_max = 15;						  //pk之王活动地图最多进图的机器人数量
	const int32 join_hollow_break_num_max = 10;					  //进入虚空破碎地图最多的人数

	const int32 pk_king_fight_pos_x1 = 3901;							//pk之王进图之后打架的位置
	const int32 pk_king_fight_pos_y1 = 3663;
	const int32 pk_king_fight_pos_z1 = 135;

	const int32 pk_king_fight_pos_x2 = 3895;
	const int32 pk_king_fight_pos_y2 = 12035;
	const int32 pk_king_fight_pos_z2 = 135;
	
	const int32 pk_king_fight_pos_x3 = 12466;
	const int32 pk_king_fight_pos_y3 = 12041;
	const int32 pk_king_fight_pos_z3 = 135;
	
	const int32 pk_king_fight_pos_x4 = 12326;
	const int32 pk_king_fight_pos_y4 = 3572;
	const int32 pk_king_fight_pos_z4 = 135;

	//图鉴
	const int32 commit_pokedex_time = 300000;				//图鉴自动提交时间间隔
	const int32 resolve_pokedex_time = 1800000;				//分解无用图鉴时间间隔
	const int32 max_pokedex_out_broad_num = 5;				//去图鉴挂机的人数上限


	//需要激活的物品 冰霜龙 先驱之翼
	const std::vector<int32> need_renew_item_template_id_arry = { 31030012, 31040041 };
#pragma pack(pop)
}
#endif