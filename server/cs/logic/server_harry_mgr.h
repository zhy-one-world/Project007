/********************************************************************
created: 2019年4月24日
file base: server_harry_mgr
file ext: h
author: zhangshuo

purpose: 跨服掠夺
*********************************************************************/
#ifndef _SERVER_HARRY_MGR_H_
#define _SERVER_HARRY_MGR_H_

#include "cross_server_harry_def.hpp"

namespace hld
{
	class player;
	class server_harry_mgr
	{
	public:
		server_harry_mgr();
		~server_harry_mgr();
	public:
		void									clear_data();
		void									set_player_ptr(player* player_ptr);

		bool									is_in_harry_game_time();
		bool									is_in_harry_special_game_time();

		void									save_role_harry_info_to_db(int32 type);
		void									load_role_harry_info_from_db(const s_db_role_harry_info& harry_info);
		bool                                    load_role_harry_record_by_db_lua(const char *data_ptr, int32 data_len);

		void									refresh_day_harry_info();//日常清理
		void									clear_harry_info_all();//全部清理

		void									set_harry_data_info(int32 idex, int32 value);
		int32									get_harry_data_info(int32 idex);

		bool									is_special_collect();//是否为特殊采集
		bool									can_harry_item();
		bool									can_harry_item_with_server_money();//根据繁荣度判断是否能采集
		bool									is_have_special_harry_item_count();
		bool									is_have_item();//是否已经拥有采集物品
		void									clear_server_item_with_player_dead();//清理掉已拥有的物品当玩家死亡

		bool									can_have_item_with_harry_type(int32 harry_type);//是否能获得物品通过该掠夺类型
		bool									can_have_money_with_harry_type(int32 harry_Type);//是否能获得金钱通过该掠夺类型

		void									finish_collect_set_item_server_id(int32 server_id, bool is_special_item);//采集成功，设置一下对应的标记
		void									add_defend_value(int32 value);
		void									add_harry_value(int32 value);
		void									add_defend_or_harry_value_with_ratio(float ratio, bool is_defend_value);//根据倍率增加掠夺值或者守护值

		void									finish_gather_item();//采集成功以后调用的函数

		void									send_ws_change_server_money(bool is_special_harry, int32 change_value, int32 server_id);

		void									give_server_item_to_npc();//提交物品给NPC
		void									kill_player_with_harry(int32 other_player_index);//杀人

		float									get_award_ratio_with_server_money(float server_money);//根据繁荣度获取收益比例
		float									get_award_ratio_with_kill_player_level(float other_player_level);//根据等级差获取收益比例

		void									get_award_item_and_money_with_harry_type(std::vector<int32>& item_arr, std::vector<int32>& money_arr, int32 harry_type);

		void									send_role_harry_info_one(int32 harry_record_type);
		void									send_role_harry_info_all();
		void									sync_defend_data_ranking_data(); //同步跨服掠夺守护值排行榜

		void									add_cross_rob_must_daily_count();
	private:
		player*									m_player_ptr;
		s_role_harry_info						m_harry_info;
	};
}

#endif