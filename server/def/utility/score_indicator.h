#pragma once

#include "logic/type_def.hpp"
#include "Logic/map_def.hpp"

namespace faith
{
	static const int32 sync_record_count_max = 50;
	class game_proto_score_record_list;

	class score_indicator
	{
	public:
		void				clear_data();

		bool				exist_score_record(const guid_64& role_guid);
		void				add_score_record(const guid_64& role_guid, const xchar* role_name, int32 role_level, int64 role_score = 0, int32 server_id = 0, guid_64 legion_guid = guid_64());
		void				del_score_record(const guid_64& role_guid);

		int64				set_score_by_info(const guid_64& role_guid, const xchar* role_name, int32 role_level, int64 role_score = 0, int32 server_id = 0, guid_64 legion_guid = guid_64());
		int64				add_score_by_info(const guid_64& role_guid, const xchar* role_name, int32 role_level, int64 add_score, int32 server_id = 0, guid_64 legion_guid = guid_64());

		score_record*		get_score_record_at(int32 score_list_index);
		score_record*		get_score_record(const guid_64& role_guid);
		score_record*		get_score_record_and_idx(const guid_64& role_guid, int32& score_idx);
		score_record*		get_highest_score_record();
		int32				get_score_record_idx(const guid_64& role_guid);
		int32				get_score_recore_num();
		void				set_legion_guid(const guid_64& role_guid, const guid_64& legion_guid);
		int64				add_score(const guid_64& role_guid, double add_score);
		bool				set_score(const guid_64& role_guid, int64 score);
		void				sort();

		bool				fill_score_record_list_msg(game_proto_score_record_list& score_record_list_msg, int32 max_record_count = -1);
		void				fill_extra_score_record_msg(game_proto_score_record_list& exrea_score_record_list_msg, guid_64 need_extra_guid);
		void				change_legion_name_func(const guid_64& legion_guid, const xchar* legion_name);
		std::vector<score_record>& get_score_record_list_all() { return m_score_record_list; }

		int64				add_score_record_by_list(const score_record& score_list);
		int64				set_score_record_by_list(const score_record& score_list);
		void				calculate_other_score(const guid_64& role_guid, int32 kill_boss_sum_time);
	private:
		std::vector<score_record> m_score_record_list;
	};
}
