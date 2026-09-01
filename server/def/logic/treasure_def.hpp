/********************************************************************
created: 2017-5-26 16:31:46
file base: treasure_def
file ext: hpp
author: zhy
*********************************************************************/
#ifndef _TREASURE_DEF_HPP_
#define _TREASURE_DEF_HPP_

#include "logic/type_def.hpp"

namespace faith
{
#pragma pack(push,1)

	const int32 treasure_prize_num = 30;						//礼品数量
	const int32 treasure_prize_record_num = 50;					//记录数量
	const int32 treasure_prize_record_string_num = 1000;		//记录数量
	const int32 treasure_royal_best_notice_id = 93000236;		//皇家宝藏极品公告
	const int32 treasure_royal_better_notice_id = 93000237;		//皇家宝藏稀有公告
	const int32 treasure_element_best_notice_id = 93000238;		//符文宝藏极品公告
	const int32 treasure_element_better_notice_id = 93000239;	//符文宝藏稀有公告
	const int32 spirit_element_best_notice_id = 93000260;		//精灵宝藏极品公告
	const int32 spirit_element_better_notice_id = 93000261;		//精灵宝藏稀有公告
	const int32 time_limit_best_notice_id = 93000437;		//精灵宝藏极品公告
	const int32 stime_limit_better_notice_id = 93000438;		//精灵宝藏稀有公告

	struct s_treasure_record
	{
		s_treasure_record()
		{
			memset(this, 0, sizeof(*this));
		}
		int32 data_ary[e_treasure_record_max];
	};
	struct s_treasure_info_db
	{
		guid_64 role_guid;
		s_treasure_record data_info;
		s_treasure_info_db()
		{
			memset(this, 0, sizeof(*this));
		}
	};
#pragma pack(pop)
}

#endif
