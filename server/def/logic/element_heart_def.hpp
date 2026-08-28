#pragma once
/********************************************************************
created: 2016/07/04
file base: element_heart_def
file ext: hpp
author: 张宇翔

purpose:
*********************************************************************/
#ifndef _ELEMENT_HEART_DEF_HPP_
#define _ELEMENT_HEART_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace hld
{
#pragma pack(push,1)
#define			FIRST_ELEMENT_HEART_DROP_GROUP				55000001										//初始元素之心抽取组id
#define         FIRST_ELEMENT_HEART_LUCKY_DRAW				55000100										//元素之心首次抽取必得符文
#define			SUMMON_ELEMNT_HEART_GROUP_LEVEL				3												//召唤抽取组到达的等级

	const int32 elemnt_heart_max_group_level = 4;
	const int32 elemnt_heart_orange_level = 5;	// 橙色元素之心

	//enum e_element_heart_operate_type
	//{
	//	eehot_get_one,//抽取1个
	//	eehot_get_ten,//抽取10个
	//	eehot_add_exp,//元素之心升级
	//	eehot_equip_on,//元素之心装备
	//	eehot_equip_off,//元素之心卸下
	//	eehot_summon,//召唤
	//	eehot_tidy,//整理
	//};
	enum e_element_heart_quality_levels
	{
		e_element_heart_quality_level_0,	//元素残渣
		e_element_heart_quality_level_1,
		e_element_heart_quality_level_2,
		e_element_heart_quality_level_3,
		e_element_heart_quality_level_4,
		e_element_heart_quality_level_5,
		e_element_heart_quality_level_max,
	};
#pragma pack(pop)
}
#endif
