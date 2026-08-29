/********************************************************************
created: 2022年2月23日
file base: time_feed_back_mgr
file ext: hpp
author: zhaoyuming
*********************************************************************/

#ifndef _TIME_FEED_BACK_MGR_
#define _TIME_FEED_BACK_MGR_

#include "template/ServerTemplateHead.h"
#include "time_limit_activity_mgr.h"

namespace faith
{
	class time_feed_back_mgr
	{
	public:
		time_feed_back_mgr();
		~time_feed_back_mgr();
	public:
		void clear_data();

		// 初始化数据
		void init_data();
		
		// Tick
		void heart_tick(const int64 & new_time);

		// 接收数据结果
		void load_info_end(const s_time_feed_back_db_info * dp_info, int32 data_num);

		// 保存数据
		void save_info(e_save_role_data_type eType);

		// 设置角色索引
		void set_player_ptr(const int32 array_index);

		// 获取几天后的0点时间戳
		int32 get_receive_award_time(int32 day_num);

		// 发送活动数据
		void send_time_feed_back_info();

		// 活动操作
		void time_feed_back_operation(int32 operation_type, int32 mian_id, int32 sub_id);

		// 发送操作结果
		void send_operate_end(e_time_feed_back_operation_end_type end_type);

		// 检查充值
		void apply_recharge(int32 goods_id);
		// 根据活动表获取商品数据
		s_time_feed_back_info& get_info_by_id(int32 main_id, int32 sub_id);
	private:
		int32									m_array_index;				// 玩家索引
		std::vector<s_time_feed_back_info>		m_info_list;				// 活动信息列表
		s_time_feed_back_info					m_empty_info;				// 空信息	
	};

}


#endif