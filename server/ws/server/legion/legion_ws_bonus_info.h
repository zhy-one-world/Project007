#ifndef _WS_LEGION_WS_BONUS_INFO_H_
#define _WS_LEGION_WS_BONUS_INFO_H_

#include "Logic/legion_def.hpp"
#include "legion.pb.h"

namespace hld
{
	class legion_ws_bonus_info
	{
	public:
		legion_ws_bonus_info();
		~legion_ws_bonus_info();
		void clear_data();
		void init(guid_64 cur_legion);
		void add_legion_bonus_info_map(const s_legion_bonus_info& bonus_info);
		void clear_legion_bonus_info_map();	
		void fill_legion_bonus_info_list_all(legion_proto_get_legion_bonus_info_end& get_legion_bonus_info_end);
		void fill_legion_bonus_info_list(legion_proto_legion_bonus_info& legion_bonus_info, int32 mission_id, int32 finish_count);
		s_legion_bonus_info& get_bonus_one(int32 mission_id);

	public:
		void recv_load_all_legion_bonus_info_from_db(const s_legion_bonus_info& bonus_info);
		void save_legion_bonus_info_into_db();
		void clear_legion_bonus_info_in_db();

	private:
		guid_64						m_legion_guid;
		std::unordered_map<int32, s_legion_bonus_info>	m_bonus_info_map;
		bool						m_is_need_save_to_db;
	};
}

#endif