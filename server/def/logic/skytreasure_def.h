#ifndef SKYTREASURE_DEF_H
#define SKYTREASURE_DEF_H
#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace faith
{
#pragma pack(push, 1)

	const int32			max_skytreasure_num = 400;                   //天空宝库最大数量
	const int32         max_skytreasure_reward_num = 100;            //最大奖励数量  
	const int32         skytreasure_notice_id = 93000430;            //天空宝库大奖公告

	const int32			skytreasure_operate_sucess = 0;              //操作成功
	const int32			skytreasure_operate_error_not_item = -1;	 //材料不够	
	const int32			skytreasure_operate_error_not_money = -2;	 //钱不够	
	const int32			skytreasure_operate_error_other = -3;        //其它错误


	enum e_skytreasure_operate_type
	{
		e_skytreasure_get_reward,         //翻牌奖励    
		e_skytreasure_buy_key,            //购买钥匙 
		e_skytreasure_refresh_recycle     //刷新周期
	};

	struct s_skytreasure_info
	{
		int32 skytreasure_layer_num;        //层数
		int32 skytreasure_pos;              //在格子中的位置 
		int32 skytreasure_pos_in_array;     //在奖励数组中的位置	
		s_skytreasure_info()
		{
			clear_data();
		}
		void clear_data()
		{
			skytreasure_layer_num = -1;
			skytreasure_pos = -1;
			skytreasure_pos_in_array = -1;
		}

	};

#pragma pack(pop)
}


#endif