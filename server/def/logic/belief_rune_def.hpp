/********************************************************************
created: 2018/09/20
created: 21:4:2016 15:40
file base: belief_rune_def
file ext: hpp
author: shangxiaoliang

*********************************************************************/
#ifndef _BELIEF_RUNE_DEF_HPP_
#define _BELIEF_RUNE_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "game_enum_def_s.h"
#include "template/template_base.h"

namespace faith
{
#pragma pack(push,1)

	const int32	belief_rune_unlock_template_base_id = 143000000;
	const int32 belief_cloister_attributeid_in_activity_tbl = 120000000 + e_activity_type_belief_cloister;	//信仰回廊活动在活动表中的id
	const int32 belief_cloister_leave_map_wait_seconds = 15;	//信仰回廊副本离开倒计时时间（秒）

	enum e_belief_rune_operate_type
	{
		e_belief_rune_operate_type_equip_on = 1,
		e_belief_rune_operate_type_level_up,
		e_belief_rune_operate_type_decompose
	};

	enum e_belief_rune_error_code_item
	{
		e_belief_rune_error_code_item_none,							//没错误
		e_belief_rune_error_code_item_player_error,					//玩家不否存在
		e_belief_rune_error_code_item_param_slot_error,				//参数错误，装备槽的索引越界
		e_belief_rune_error_code_item_param_slot_is_unlock,			//装备槽未解锁
		e_belief_rune_error_code_item_item_not_exist,				//道具不存在
		e_belief_rune_error_code_item_item_is_not_belief_rune,		//类型不是信仰符文
		e_belief_rune_error_code_item_template_not_found,			//数据表找不到数据
		e_belief_rune_error_code_item_item_container_error,			//容器类型不合法
		e_belief_rune_error_code_item_is_max_level,					//已是最大等级
		e_belief_rune_error_code_item_template_data_error,			//数据表格式错误
		e_belief_rune_error_code_item_levelup_money_not_enough,		//升级消耗钱不够
		e_belief_rune_error_code_item_param_empty,					//参数错误，分解列表为空
		e_belief_rune_error_code_item_decompose_money_not_enough,	//保值分解消耗钱不够
		e_belief_rune_error_code_item_equip_on_one_attr,             //同类属性只能穿戴一个
		e_belief_rune_error_code_item_item_is_not_sub_belief_rune    //道具的子类型不合法
	};
	
#pragma pack(pop)
}

#endif
