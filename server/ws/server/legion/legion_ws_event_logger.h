#ifndef _WS_LEGION_WS_EVENT_LOGGER_H_
#define _WS_LEGION_WS_EVENT_LOGGER_H_

#include "Logic/legion_def.hpp"
#include "legion.pb.h"

namespace hld
{
	class legion_ws_event_logger
	{
	public:
		legion_ws_event_logger();
		~legion_ws_event_logger();
		void clear_data();
		void init(guid_64 cur_legion);
	public:
		void add_member_join_event(std::string member_name);
		void add_member_leave_event(std::string member_name);
		void add_member_post_change_event(std::string member_name, std::string old_job, std::string new_job);
		void add_donate_money_event(std::string member_name, int32 legion_up_money_num, int32 battle_achievement_num, int32 donate_money_num);
		void add_donate_item_event(std::string member_name, int32 item_id, int32 item_num, int32 battle_achievement_num, int32 donate_item_get_money);
		void add_on_create_bonfire_event();
		void add_legion_construction_level_up_event(ELegionInfo construction_idex, int32 level);
		void add_occupy_city_event(std::string chief_name, int32 territory_id);
		void add_lose_city_by_war_event(int32 territory_id);
		void add_lose_city_by_maintain_money_not_enough_event(int32 territory_id);
		void add_bid_win_event(int32 territory_id);
		void add_bid_fail_event(int32 territory_id);
		void add_win_overlord_city_war_event(std::string chief_name);
		void add_member_change_name_event(const xstring& original_name, const xstring& current_name);
		void clear_event();
		void add_event(std::string& data_str);
		void add_event(s_legion_event& new_event);

		int32 fill_get_event_list_msg(legion_proto_get_legion_event_list_end& get_event_list_end_msg, uint32 happen_time);

		void req_load_all_legion_event_from_db();
		void recv_load_all_legion_event_from_db(const s_legion_event* event_data_arr, int32 arr_len);
		void load_legion_event(const s_legion_event& event_data);
		void save_legion_event_into_db(const s_legion_event& event_data);
		void del_legion_event_from_db(int32 event_idx);
		void clear_legion_event_in_db();

	private:
		guid_64						m_legion_guid;
		std::list<s_legion_event>	m_event_list;
	};
}

#endif