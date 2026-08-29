/********************************************************************
created: 2018年5月5日
file base: gain_treasure_mgr
file ext: h
author: wangdi

purpose: 打宝相关
*********************************************************************/
#ifndef _GAIN_TREASURE_MGR_H_
#define _GAIN_TREASURE_MGR_H_

#include "gain_treasure_def.hpp"

namespace faith
{
	class player;
	class gain_treasure_mgr
	{
	public:
		gain_treasure_mgr();
		~gain_treasure_mgr();
	public:
		void									clear_data();
		void									set_player_ptr(player* player_ptr);

		bool									can_entry_to_boss_single_map();
		bool									can_have_item(int32 npc_level);
		bool									is_finish_today_field_boss();

		void									save_gain_treasure_info_to_db(int32 save_type);
		void									load_gain_treasure_info_from_db(const s_gain_treasure_info& info_data);
		bool                                    load_role_gain_treasure_by_db_lua(const char *data_ptr, int32 data_len);
		void									send_gain_treasure_info_one(int32 info_type);
		void									send_gain_treasure_info_all();
		void									set_gain_treasure_info(int32 info_type, int32 info_value);
		void									refresh_gain_treasure_info();
		int32									get_gain_treasure_info(int32 info_type);
		static void								send_gain_treasure_record_to_ws(int32 boss_spawn_point_template_id, int32 killed_stamp, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr);
		static void								send_gain_treasure_boss_info_to_ws(int32 boss_spawn_point_template_id, guid_64 boss_guid, int32 next_refresh_stamp, const std::vector<s_gain_treasure_player_info>& player_info_arr);

		void									buy_single_boss_num();

		int32									get_map_rank();
		void									set_map_rank(int32 _rank);
	private:
		player*									m_player_ptr;
		int32									m_gain_treasure_info[e_gain_treasure_info_type_max];
		int32									m_map_rank;
	};
}

#endif