#ifndef _ACTIVITY_ACTIVITY_DEF_HPP_
#define _ACTIVITY_ACTIVITY_DEF_HPP_

#include "game_enum_def_s.h"
#include "logic/type_def.hpp"

namespace faith
{
#pragma pack(push,1)
	
	const int32 first_activity_common_template_id = 120000000;	// 活动通用配置表第一个配置id

	const int32 first_broken_sky_reward_temp_id = 113004001;
	const int32 first_world_boss_reward_temp_id = 113011001;
	const int32 first_legion_boss_reward_temp_id = 113005001;

	const int32 broken_sky_act_open_time = 900 ;	//破碎虚空开启持续时间
	const int32 once_activity_open_time_array_size = 8;

	/************************************************/
	/*                 宝箱降临活动                 */
	/************************************************/

	namespace chest_arrival_activity
	{
		const int32 max_spawn_chest_num = 10000;
		const int32 spawn_chest_begin_template_id = 0;//天降宝箱已弃用
		const int32 chest_live_time = 15 * 60; // 单位秒

		const int32 spawn_chest_time[] = {
			13 * 3600 + 00 * 60 + 00,	// 13:00
			13 * 3600 + 15 * 60 + 00,	// 13:15
			19 * 3600 + 00 * 60 + 00,	// 19:00
			19 * 3600 + 15 * 60 + 00,	// 19:15
		};
		const int32 spawn_chest_time_arr_size = sizeof(spawn_chest_time) / sizeof(int32);
	};
	enum e_map_difficulty_for_multiplayer
	{
		e_map_difficulty_for_multiplayer_easy, //简单
		e_map_difficulty_for_multiplayer_normal, //普通
		e_map_difficulty_for_multiplayer_hard, //困难
		e_map_difficulty_for_multiplayer_heroic,//英雄
		e_map_difficulty_for_multiplayer_hell,//地狱
		e_map_difficulty_for_multiplayer_max
	};

	enum e_activity_time_get
	{
		e_activity_time_get_ready,
		e_activity_time_get_gaming,
		e_activity_time_get_all,
		e_activity_time_get_notice
	};

	enum e_must_do_type
	{
		e_must_do_type_none,
		e_must_do_type_everyday_act,
		e_must_do_type_time_act
	};
#pragma pack(pop)
}

#endif
