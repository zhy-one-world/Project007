/********************************************************************
created: 2018年9月6日
file base: boss_island_mgr
file ext: h
author: zhangshuo

purpose: 永恒岛
*********************************************************************/
#ifndef _BOSS_ISLAND_MGR_H_
#define _BOSS_ISLAND_MGR_H_

#include "boss_island_def.hpp"
#include "gain_treasure_def.hpp"

namespace faith
{
	class player;
	class boss_island_mgr
	{
	public:
		boss_island_mgr();
		~boss_island_mgr();
	public:
		void									clear_data();
		void									set_player_ptr(player* player_ptr);

		void									load_boss_island_info_from_db(const s_boss_island_info& boss_island_info);
		bool                                    load_boss_island_by_db_lua(const char *data_ptr, int32 data_len);
		void									save_boss_island_to_db(int32 type);
		bool									is_finish_today_boss_island();

		void									set_boss_island_info(int32 type, int32 value);
		int32									get_boss_island_info(int32 type);
		int32									get_left_boss_island_chest();//剩余采集次数
		int32									get_left_cross_pk_chest();//跨服JJC剩余采集次数
		bool									can_have_item_in_island();
		int32									get_left_boss_island_kill();//剩余BOSS击杀数
		static void								send_boss_island_info_to_ws(int32 boss_spawn_point_template_id, guid_64 boss_guid, int32 next_refresh_stamp, const std::vector<s_gain_treasure_player_info>& player_info_arr);

		void									refresh_all_boss_island_info();
		void									send_player_boss_island_info_all();
		void									send_player_boss_island_info_one(int32 type);
		

	private:
		player*									m_player_ptr;
		int32									m_boss_island_info[e_island_record_max];
	};
}

#endif