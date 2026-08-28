#pragma once
/********************************************************************
created: 2016/10/17
file base: title_def
file ext: hpp
author: wucun

purpose:
*********************************************************************/

#ifndef _TITLE_DEF_HPP_
#define _TITLE_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace hld
{
#pragma pack(push,1)

	const int32			max_attribute_num = 30;			//属性最大数量
	const int32			MAX_TITLE_NUM = 200;			//称号最大数量
	const int32			pk_king_title_id = 39000010; // PK之王的称号ID
	const int32			pk_king_kill_num_title_id = 39000077; // PK之王击杀的称号ID


	enum e_title_type
	{
		e_title_type_title_card,				//使用称号卡
		e_title_type_kingofpk,					//pk之王
		e_title_type_kingofgods,				//诸神之王
		e_title_type_legion_city_win_chief,		//所领导军团为军团争霸胜方
		e_title_type_legion_city_win_member,	//所在军团为军团争霸胜方
		e_title_type_capture_point_chief,		//所领导军团占领10级据点
		e_title_type_capture_point_member,		//所在军团占领10级据点
		e_title_type_login_day_num,				//登陆天数
		e_title_type_level,						//角色等级
		e_title_type_kill_player_num,			//击杀玩家数量
		e_title_type_arena_rank_first,			//竞技场排名第一
		e_title_type_worship_rank_first,		//威望排名第一
		e_title_type_gs_rank_first,				//战力排名第一
		e_title_type_money_rank_first,			//财富排名第一
		e_title_type_first_clearance_heard_map, //首次通关炼狱副本
		e_title_type_gs_ranking_barserker,		//战士战力榜
		e_title_type_gs_ranking_wizard,			//法师
		e_title_type_gs_ranking_guardian,		//骑士
		e_title_type_gs_ranking_assassinator,	//刺客
		e_title_type_vip,						//VIP
		e_title_type_internal_test,			    //内测先锋
		e_title_type_best_barserker,			//最强战士
		e_title_type_best_wizard,				//最强法师
		e_title_type_best_guardian,				//最强骑士
		e_title_type_best_assassinator,			//最强刺客
		e_title_type_marry_times,				//结婚称号
		e_title_type_marry_divorce_times,		//离婚称号
		e_title_type_cross_ladder_one,
		e_title_type_cross_ladder_two,
		e_title_type_cross_ladder_three,
		e_title_type_cross_ladder_four,
		e_title_type_cross_ladder_five,
		e_title_type_cross_ladder_six,
		e_title_type_cross_ladder_ranking,
		e_title_type_pk_king_kill_num,			//Pk之王连杀
		e_title_type_element_war_one,			//元素争霸白金称号
		e_title_type_element_war_two,			//元素争霸钻石称号
		e_title_type_element_war_three,			//元素争霸王者称号
		e_title_type_element_war_ranking,		//元素争霸百强称号
		e_title_type_max	
	};
	
	struct s_title_info
	{
		int32						data_ary[e_title_info_max];
		s_title_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_title_info_db
	{
		guid_64 role_guid;
		int32 data_info[hld::e_title_info_max];
	};

	enum e_title_equip_id
	{
		e_title_equip_template_id,					//当前装备称号模板id
		e_title_equip_index_id,						//当前装备称号数组id
		e_title_equip_max,
	};

	struct s_title_equip_id
	{
		int32						data_ary[e_title_equip_max];
		s_title_equip_id()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	enum e_title_add_result_type
	{
		e_title_add_result_type_none,					//未知错误
		e_title_add_result_type_normal,					//正常使用成功
		e_title_add_result_type_add_time,				//追加时间
		e_title_add_result_type_cannot_use_again,		//不能重复使用
		e_title_add_result_type_max,
	};
	enum e_title_operate_type
	{
		e_title_operate_type_equip_on,					//装备
		e_title_operate_type_equip_off,					//卸下
		e_title_operate_type_max,
	};

	enum e_title_end_time_type
	{
		e_title_end_time_type_forever,
		e_title_end_time_type_time,
		e_title_end_time_type_condition1,
		e_title_end_time_type_condition2
	};

	enum e_title_special_handle
	{
		e_title_special_handle_none,
		e_title_special_handle_add,
		e_title_special_handle_remove,
	};

#pragma pack(pop)
}
#endif
