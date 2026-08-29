/********************************************************************
created: 2022年3月7日
file base: time_limit_gift
file ext: hpp
author: zhaoyuming
*********************************************************************/

#ifndef _TIME_LIMIT_GIFT_MGR_
#define _TIME_LIMIT_GIFT_MGR_

#include "template/ServerTemplateHead.h"
#include "Logic/char_def.hpp"

namespace faith
{
	class time_limit_gift_mgr
	{
	public:
		time_limit_gift_mgr();
		~time_limit_gift_mgr();
	public:
		void clear_data();

		// 初始化数据
		void init_data();
		
		// Tick
		void heart_tick(const int64 & new_time);

		// 接收数据结果
		void load_info_end(const s_time_limit_gift_db_info * dp_info, int32 data_num);

		// 保存数据
		void save_info(e_save_role_data_type eType);

		// 设置角色索引
		void set_player_ptr(const int32 array_index);

		// 获取几天后的0点时间戳
		int32 get_receive_award_time(int32 day_num);

		// 发送活动数据
		void update_all_info();

		// 活动操作
		void operation_begin(int32 operation_type, int32 template_id);

		// 发送操作结果
		void send_operate_end(e_time_limit_gift_operation_end_type end_type, int32 template_id = 0);

		// 根据活动表获取商品数据
		s_time_limit_gift_info& get_info(int32 template_id);

		// 检查充值
		void apply_recharge(int32 goods_id);

		// 检查完成地图
		void check_target_map(int32 map_template_id);
	private:
		int32									m_array_index;				// 玩家索引
		std::vector<s_time_limit_gift_info>		m_info_list;				// 活动信息列表
		s_time_limit_gift_info					m_empty_info;				// 空信息	
	};

}


#endif