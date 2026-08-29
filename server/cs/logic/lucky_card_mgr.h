/********************************************************************
created: 2021年3月24日
file base: lucky_card_mgr
file ext: hpp
author: zhaoyuming
*********************************************************************/

#ifndef _LUCKY_CARD_MGR_HPP
#define _LUCKY_CARD_MGR_HPP


#include "Logic/lucky_card_def.h"
#include "template/ServerTemplateHead.h"


namespace faith
{
	class lucky_card_mgr
	{
	public:
		lucky_card_mgr();
		~lucky_card_mgr();
	public:
		void clear_data();

		// 初始化数据
		void init_data();
		
		// Tick
		void heart_tick(const int64 & new_time);

		// 设置角色索引
		void set_player_ptr(const int32 array_index);

		// Load结束设置角色信息
		void load_lucky_card_end(s_time_limit_activity_info activity_info, bool is_init = false);
		
		// 设置数据库存储数据
		void set_limit_activiy_info();

		// 周期刷新 清理数据
		void refresh_recycle();

		// 进行下一个状态
		void next_state(int32 touch_index, int32 is_jump);

		// 进行开始
		void game_begin();

		// 跳过
		void jump_game();
		
		// 通过点击进行下一步
		void game_next(int32 touch_index);

		// 获取是否初始化结束
		bool get_init_end() { return  m_init_end; };

		// 增加奖励获取次数
		void add_award_get_num(int32 item_index, int32 add_num);

		// 检查奖励获取次数如果都大于1就全减去1
		void check_award_get_num(int32 award_type);

		// 获取奖励获取次数
		int32 get_award_get_num(int32 item_index);

		// 获取奖励数组
		std::vector<s_lucky_card_award_item_info> get_item_array_by_award_type(int32 award_type);

		// 根据奖励数组随机奖励
		s_lucky_card_award_item_info get_award_by_item_list(std::vector<s_lucky_card_award_item_info>& item_list);

		// 根据奖励数组随机奖励
		s_lucky_card_award_item_info get_award_by_item_index(int32 item_index);

		// 获取奖励类型
		int32 get_award_type_by_use_type(int32 use_type);

		// 刷新免费次数
		void refresh_free_num();

		// 获取下一个点击结果
		int32 get_next_touch_result();

		// 发送奖励公告
		void send_reward_notice(int32 item_id);

		/**************************发消息到客户端**************************************/
		// 发送数据信息
		void send_lucky_card_to_client();

		// 发送点击结果
		void send_operate_end(e_lucak_card_operate_end error_index);

		// 发送刷新卡牌状态
		void send_lucky_card_info(int32 card_index);

	private:
		int32						m_array_index;				// 玩家索引
		bool						m_init_end;					// 是否初始化结束
		s_lucky_card_role_info		m_lucky_card_info;			// 角色活动信息
		int64						m_clear_time;				// 清理计时


	};

}


#endif