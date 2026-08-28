/********************************************************************
created:	2014/08/01
created:	01:08:2014   12:45
file base: player_def
file ext:	hpp
author:	Locke

purpose:	
*********************************************************************/
#include <cassert>
#ifndef _PLAYER_DEF_HPP_
#define _PLAYER_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/att_def.hpp"

#define  NPC_NUM_AROUND_PLAYER	32	// 围绕玩家一圈的怪物点数

namespace hld
{
	// player 模板数据
	//struct player_template_info : public base_info
	//{
	//	int32		id;
	//	int32		level;
	//	int32		level_up_exp_cost;
	//	int32		max_physics;
	//	int32		max_stamina;
	//	int32		max_hp_param;
	//	int32		max_magic_param;
	//	int32		init_gold;
	//	int32		init_magic;
	//	int32		max_magic;
	//	int32		max_population;
	//	int32		add_magic_speed;
	//	int32		add_hp_speed;
	//	int32		add_gold_speed;

	//	player_template_info():id(0),level(0),level_up_exp_cost(0),
	//		max_physics(0),max_stamina(0),max_hp_param(0),max_magic_param(0),init_gold(0),init_magic(0),max_magic(0),max_population(0),
	//		add_magic_speed(0),add_hp_speed(0),add_gold_speed(0)
	//	{
	//		
	//	}
	//};
}

#endif
