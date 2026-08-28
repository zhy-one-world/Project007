/********************************************************************
created: 2022年8月22日
file base: star_trip_mgr
file ext: hpp
author: zhaoyuming
*********************************************************************/

#ifndef _STAR_TRIP_MGR_HPP
#define _STAR_TRIP_MGR_HPP

#include "template/ServerTemplateHead.h"
#include "Logic/star_trip_def.h"

namespace hld
{
	class star_trip_mgr
	{
	public:
		star_trip_mgr();
		~star_trip_mgr();
	public:
		void clear_data();

		// 初始化数据
		void init_data();
		
		// Tick
		void heart_tick(const int64 & new_time);

		// 设置角色索引
		void set_player_ptr(const int32 array_index);

		// 刷新每日任务
		void refresh_today_mission();

		// 设置数据库存储数据
		void set_limit_activiy_info();

		// 初始化活动数据
		void init_activiy_info();

		// 发送所有活动数据
		void send_all_activity_info_to_client();

		// operate
		void activity_operate(int32 operate_type, int32 parame1, int32 parame2);

		// 检查任务
		void target_mission(int32 mission_type, int32 parame1 = 0);

		// 检查任务
		void target_mission_by_mission_type(e_mission_end_type mission_type, int32 parame1 = 0, int32 parame2 = 0);

		// 兑换物品
		int32 buy_item(int32 item_index);

		// 领取任务奖励
		int32 finish_mission(int32 missione_index);

		bool get_is_init();
	private:
		int32										m_array_index;				// 玩家索引
		int32										m_target_mission_time;		// 任务完成时间
		int32										m_need_tip;					// 使用有兑换提示
		bool										m_init;
		std::vector<s_star_trip_goods_info>			m_goods_list;				// 商品兑换列表
		std::vector<s_star_trip_mission_info>		m_mission_list;				// 任务列表
	};

}


#endif