/********************************************************************
created: 2021年3月24日
file base: lucky_card_def
file ext: h
author: zhaoyuming
*********************************************************************/

#ifndef _LUCKY_CARD_DEF_H
#define _LUCKY_CARD_DEF_H

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "time_limit_activity_def.hpp"

namespace hld
{
#pragma pack(push, 1)

	const int32 lucak_card_max_num = 9;				// 翻牌总数
	const int32 lucak_card_notice_id = 93000434;	// 获奖公告id

	enum e_lucak_card_game_state_type
	{
		e_lucak_card_game_state_type_begin,			// 未支付
		e_lucak_card_game_state_type_first,			// 已支付未开始翻牌	
		e_lucak_card_game_state_type_second,		// 已翻第一次
		e_lucak_card_game_state_type_three,			// 已翻第二次
		e_lucak_card_game_state_type_end,			// 结束
	};

	enum e_lucak_card_use_type
	{
		e_lucak_card_use_type_free,					// 免费
		e_lucak_card_use_type_normal,				// 正常	
		e_lucak_card_use_type_award,				// 保底
	};

	enum e_lucak_card_award_type
	{
		e_lucak_card_award_type_special = 1,		// 特等奖
		e_lucak_card_award_type_first,				// 一等奖
		e_lucak_card_award_type_second,				// 二等奖
		e_lucak_card_award_type_three,				// 三等奖
	};

	enum e_lucak_card_info
	{
		e_lucak_card_info_null ,					// 未开启
		e_lucak_card_info_black,					// 黑色
		e_lucak_card_info_red,						// 红色
	};	

	enum e_lucak_card_set_activity
	{
		e_lucak_card_set_activity_state_info = 6,
		e_lucak_card_set_activity_is_have_free,
		e_lucak_card_set_activity_end_info,
		e_lucak_card_set_activity_end_item_id,
		e_lucak_card_set_activity_no_get_num,
		e_lucak_card_set_activity_lucky_card_info,
	};

	enum e_lucak_card_item_info
	{
		e_lucak_card_item_info_award_type,			// 奖励类型
		e_lucak_card_item_info_item_index,			// 奖励索引
		e_lucak_card_item_info_item_id,				// 物品id
		e_lucak_card_item_info_item_num,			// 物品数量
		e_lucak_card_item_info_basice_weight,		// 奖励基础权重
		e_lucak_card_item_info_change_weight,		// 奖励权重修正
		e_lucak_card_item_info_max,
	};

	enum e_lucak_card_operate_end
	{
		e_lucak_card_operate_end_succeed,			// 成功
		e_lucak_card_operate_end_not_have_item,		// 缺少活动物品
		e_lucak_card_operate_end_not_open,			// 活动未开启
		e_lucak_card_operate_end_have_touch,		// 已经触发过了
	};

	struct s_lucky_card_role_info
	{
		int32 activity_guid;												// 活动Guid
		int32 state_info;													// 进行状态
		int32 is_have_free;													// 是否有免费次数
		int32 end_info;														// 结果类型
		int32 end_item_id;													// 奖励物品id
		int32 no_get_num;													// 连续未获得奖励次数
		int32 lucky_card_info;												// 已翻牌信息	10进制表示 从个位数开始每一位代表一张牌的状态
		xchar get_item_array[time_limit_activity_string_max_size + 1];		// 获取物品次数
		s_lucky_card_role_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		void set_item_array(xstring temp_name)
		{
			memset(get_item_array, 0, sizeof(get_item_array));
			memcpy(get_item_array, temp_name.c_str(), temp_name.size() > time_limit_activity_string_max_size ? time_limit_activity_string_max_size : temp_name.size());
		}

		//获取翻牌信息
		int32 get_lucky_card_info(int32 index)
		{
			int32 card_info = lucky_card_info / pow(10, (index - 1));
			if (card_info <= 0)
			{
				return 0;
			}
			return card_info % 10;
		}

		//设置翻牌信息
		void set_lucky_card_info(int32 index, int32 _info)
		{
			int32 cur_info = get_lucky_card_info(index);
			if (_info == cur_info)
			{
				return;
			}
			lucky_card_info += ((_info - cur_info)  * pow(10, (index - 1)));
		}

		// 获得翻牌颜色数量
		int32 get_lucky_card_color_num(int32 color_index)
		{
			int32 have_num = 0;
			for (int32 i = 1; i <= 9; ++i)
			{
				if (get_lucky_card_info(i) == color_index)
				{
					have_num++;
				}
			}
			return have_num;
		}

		//结束清理
		void end_clear()
		{
			state_info = 0;
			end_info = 0;
			end_item_id = 0;
			lucky_card_info = 0;
		}
	};

	struct s_lucky_card_award_item_info
	{
		int32 data_arr[e_lucak_card_item_info_max];
		int32 get_num;		
		s_lucky_card_award_item_info()
		{
			memset(this, 0, sizeof(*this));
		}

		int32 get_item_info(e_lucak_card_item_info info_type)
		{
			if (info_type < 0 || info_type >= e_lucak_card_item_info_max)
			{
				return 0;
			}
			return data_arr[info_type];
		}

		void set_item_info(e_lucak_card_item_info info_type, int32 _value)
		{
			if (info_type < 0 || info_type >= e_lucak_card_item_info_max)
			{
				return;
			}
			data_arr[info_type] = _value;
		}

		int64 math_weight()
		{
			float basice_weight = get_item_info(e_lucak_card_item_info_basice_weight);
			float change_weight = get_item_info(e_lucak_card_item_info_change_weight);
			if (get_num <= 0)
			{
				return basice_weight * 100;
			}
			//精度取小数点后两位
			int64 end_weight = (basice_weight * pow((change_weight / 100), (float)get_num)) * 100;
			if (end_weight < 0)
			{
				end_weight = 0;
			}
			return end_weight ;	
		}
	};


#pragma pack(pop)
}


#endif