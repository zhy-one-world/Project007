/********************************************************************
created: 2019年7月22日
file base: oracle_trial_def
file ext: hpp
author: zhaoyuming
purpose:
*********************************************************************/
#ifndef _ORACLE_TRIAL_DEF_HPP_
#define _ORACLE_TRIAL_DEF_HPP_

#include "char_def.hpp"
#include "game_enum_def_s.h"

namespace faith
{
#pragma pack(push,1)	
	const int32							max_tier_num = 30;			//天启试炼最大层数
	const int32							max_customs_num = 6;		//天启试炼每层最大关卡数
	const int32							max_reward_num = 6;			//每层最大奖励数量
	const int32							max_commingup_item_id = 10;	//扫荡最大物品类型数量
	const int32							complete_state = 333333;
	const int32							complete_reward_state = 111111;
	const int32							oracle_trial_unlock_id = 117000139;	//天启试炼解锁表id

	enum e_oracle_trial_customs_state
	{
		e_oracle_trial_customs_state_begin,							//已经开启关卡却没有通关过
		e_oracle_trial_customs_state_one_stat,						//通关了关卡获得1颗星
		e_oracle_trial_customs_state_two_stat,						//通关了关卡获得2颗星
		e_oracle_trial_customs_state_three_stat,					//通关了关卡获得3颗星
	};

	enum e_oracle_trial_error
	{
		e_oracle_trial_error_get_award_succed = 1,					//领取奖励成功了
		e_oracle_trial_error_get_award_not_get_num,					//这个奖励已经领取过了
		e_oracle_trial_error_get_award_no_have_stat,				//星数不足
		e_oracle_trial_error_max,									//未知错误
		e_oracle_trial_error_commping_up_succed,					//扫荡成功
		e_oracle_trial_error_commping_up_no_have_num,				//扫荡失败 今日已经扫荡过了
	};
	
	struct oracle_trial_customs_state
	{
		int32	customs_state;										//当前层数的关卡状态		
		int32	reward_stat;										//当前层数的奖励领取状态
		oracle_trial_customs_state()
		{
			clear_data();
		}
		void clear_data()	
		{
			memset(this, 0, sizeof(oracle_trial_customs_state));
		}
	};

	struct oracle_trial_play_info
	{
		oracle_trial_customs_state	tier_num[max_tier_num];			//天启试炼的层数
		oracle_trial_play_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(oracle_trial_play_info));
		}
		int32 get_hight_rank()
		{
			int32	hight_rank = 10;
			for (int32 i = 1; i < max_tier_num; ++i)
			{
				if (tier_num[i - 1].customs_state > 0 && tier_num[i].customs_state <= 0)
				{
					int32 _num = tier_num[i - 1].customs_state;
					hight_rank = i * 10;
					while (1)
					{
						if (_num > 10)
						{
							++hight_rank;
							_num /= 10;
						}
						else
						{
							++hight_rank;
							break;
						}
					}
				}
				else if (tier_num[i].customs_state > 0 && i == max_tier_num - 1)
				{
					int32 _num = tier_num[i].customs_state;
					hight_rank = (i + 1) * 10;
					while (1)
					{
						if (_num > 10)
						{
							++hight_rank;
							_num /= 10;
						}
						else
						{
							++hight_rank;
							break;
						}
					}
				}				
			}
			return hight_rank;
		}
		int32 get_all_stat_num()
		{
			int32 num = 0;
			for (int32 i = 0; i < max_tier_num; ++i)
			{
				if (tier_num[i].customs_state > 0)
				{
					int32 _num = tier_num[i].customs_state;
					while (1)
					{
						if (_num > 10)
						{
							num += _num % 10;
							_num /= 10;
						}
						else
						{
							num += _num % 10;
							break;
						}
					}
				}

			}
			return num;
		}
	};

	struct s_oracle_trial_info_db
	{
		guid_64 role_guid;
		int32   tier_num;
		int32	customs_state;											
		int32	reward_stat;
		s_oracle_trial_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_oracle_trial_info_db));
		}

	};

	struct item_num_info
	{
		int32						item_id[10];
		int32						item_num[10];
		int32						num;
		item_num_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(item_num_info));
		}
	};
#pragma pack(pop)
}
#endif