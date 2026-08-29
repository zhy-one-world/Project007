/********************************************************************
created: 2022年8月24日
file base: star_trip_def
file ext: h
author: zhaoyuming
*********************************************************************/

#ifndef _STAR_TRIP_DEF_H
#define _STAR_TRIP_DEF_H

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "time_limit_activity_def.hpp"

namespace faith
{
#pragma pack(push, 1)

	enum e_star_trip_operate_end
	{
		e_star_trip_operate_end_buy_finish,					// 兑换成功
		e_star_trip_operate_end_target_mission_finish,		// 领取任务成功
		e_star_trip_operate_end_set_tip_finish,				// 设置提醒成功
		e_star_trip_operate_end_error1,						// 系统错误
		e_star_trip_operate_end_template_error,				// 表格错误
		e_star_trip_operate_end_no_money,					// 积分不足
		e_star_trip_operate_end_no_buy_num,					// 没有购买次数
		e_star_trip_operate_end_no_item,					// 物品创建失败
		e_star_trip_operate_end_mission_error,				// 任务错误
		e_star_trip_operate_end_is_get,						// 已领取奖励
		e_star_trip_operate_end_no_target,					// 完成条件不满足
		e_star_trip_operate_end_no_time,					// 过了任务领奖时间
	};

	enum s_star_trip_operate_type
	{
		s_star_trip_operate_type_buy,
		s_star_trip_operate_type_mission,
		s_star_trip_operate_type_set_tip,
	};

	struct s_star_trip_goods_info
	{
		int32 item_id;								// 物品Id
		int32 item_num;								// 物品数量
		int32 is_lock;								// 是否绑定
		int32 need_money;							// 需要积分数量
		int32 max_num;								// 最大购买次数
		int32 buy_num;								// 已购买次数
		s_star_trip_goods_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		bool can_buy()
		{
			if (max_num < 0 || max_num > buy_num)
			{
				return true;
			}
			return false;
		}

		void add_buy()
		{
			buy_num += 1;
		}
	};

	struct s_star_trip_mission_info
	{
		int32 mission_type;							// 任务类型 1是每日任务, 2是周期任务
		int32 target_type;							// 触发类型
		int32 target_param;							// 触发参数
		int32 target_num;							// 触发参数
		int32 finish_num;							// 完成次数 
		int32 award_num;							// 奖励数量
		int32 is_get;								// 是否领取奖励
		s_star_trip_mission_info()
		{
			memset(this, 0, sizeof(*this));
		}

		bool is_finish()
		{
			// 触发参数大于完成参数视为完成
			return target_num >= finish_num;
		}

		void add_target_num()
		{
			target_num += 1;
		}
	};


#pragma pack(pop)
}


#endif