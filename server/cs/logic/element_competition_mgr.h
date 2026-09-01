/********************************************************************
created: 2021年6月16日
file base: element_competition_mgr
file ext: hpp
author: zhaoyuming
purpose:
*********************************************************************/

#ifndef _ELEMENT_COMPETITION_MGR_H_
#define _ELEMENT_COMPETITION_MGR_H_

#include "item.h"

namespace faith
{
	class element_competition_mgr
	{
	public:
		element_competition_mgr();
		~element_competition_mgr();


	public:
		// 数据清理
		void									clear_data();
		// 设置角色索引
		void									set_unit_index(int32 array_index) { m_array_index = array_index; };
		// 获取手册信息引用
		s_role_competition_info&				get_competition_info() { return m_cur_competition_info; };
		// Tick
		void									heart_tick(const int64& new_time);
		// 检查活动状态
		void									check_state(const int64& new_time);
		// 初始化数据结束
		void									load_competition_info_end(s_role_competition_info info);
		// lua发来的加载数据结果
		bool                                    load_element_competition_by_db_lua(const char *data_ptr, int32 data_len);
		// 初始化手册数据
		void									reset_competition_data();
		// 保存数据
		void									save_to_db(e_save_role_data_type eType);
		// 向客户端同步信息
		void									sync_competition_info();
		// 领取奖励
		void									get_reward(int32 id, int32 type);
		// 领取全部奖励
		void									get_reward_all(int32 get_type);
		// 增加手册经验
		void									add_competition_exp(int64 point_num);
		// 向客户端同步手册经验
		void									sync_lv_exp();
		// 发放奖励
		void									send_reward(int32 m_id, int32 m_type);
		// 时间装换函数
		int64									transfer_date_to_stamp(int64 date);
		// 购买等级
		void									buy_competition_level(int32 level);
		// 充值购买
		void									check_is_manual(int32 goods_id);
		// 购买手册
		void									buy_speical_manual(int32 manual_type);
	private:
		// 初始化数据(赛季时间数据)
		void									init_data();
		// 激活手册
		void									activati_manual(int32 manual_type);
		// 激活手册奖励
		void									activati_manual_reward_by_type(int32 active_type, int32 active_ext0, int32 active_ext1);
		// 修改手册等级
		void									change_point();
		// 赛季结算
		void									calcu_cur_competition();
	private:
		int32									m_array_index;
		s_role_competition_info					m_cur_competition_info;
		std::vector<s_competition_time>			m_all_competition;
		e_competition_manual_state				cur_state;
		bool									can_sync;
	};
}
#endif
