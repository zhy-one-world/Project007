#pragma once
/********************************************************************
created: 2016/11/3
file base: prompt_identification
file ext: hpp
author: wucun

purpose:
*********************************************************************/

#ifndef _PROMPT_IDENTIFICATION_DEF_HPP_
#define _PROMPT_IDENTIFICATION_DEF_HPP_


#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"
namespace hld
{
#pragma pack(push,1)
	const int32 prompt_skill_base_num = 3;
	const int32 prompt_skill_all_num = 15;
	const int32 prompt_map_diff_value = 100;		//同一类型地图上下相邻的AttributeId最多差10,这里用100判断
	const int32 prompt_main_frame_cal_flag_count = 4;	//主界面每帧计算多少红点
	const int32 prompt_sub_cal_flag_frame_count = 5;	//子界面多少帧计算一次
	const int32	prompt_raid_frame_cal_flag_count = 2;	//副本每帧计算多少个
	const int32 prompt_box_template_id = 146000000;		//PromptBoxTemplateId
	const int32 prompt_skill_only_one_star_id = 11;		//技能提示起始枚举值
	const int32 prompt_skill_only_one_brach_num = 5;	//技能提示每系分支技能数量
#pragma pack(pop)
}
#endif
