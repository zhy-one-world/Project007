#pragma once

#include "logic/type_def.hpp"

namespace hld
{
	class map_s2s_sl_map_record;
	class map_record;

	struct s_record_key
	{
		union
		{
			struct
			{
				int16 map_type;
				int16 sub_type;
			};
			uint32 key;
		};
		s_record_key(uint32 _key)
		{
			key = _key;
		}
		s_record_key(int32 _map_type, int32 _sub_type)
		{
			map_type = _map_type;
			sub_type = _sub_type;
		}
		s_record_key(e_map_type _map_type, int32 _sub_type)
		{
			map_type = _map_type;
			sub_type = _sub_type;
		}
	};


	class map_record_set
	{
	public:
		map_record_set();
		void clear_data();
		void set_player_ptr(int32 unit_array_index) { m_unit_array_index = unit_array_index; }
		void heart_tick(const int64& new_time);
	public:
		void save_map_record_to_db(int32 save_type_ex);
		void load_map_record_by_db(const map_s2s_sl_map_record& msg);
		void send_map_record_all();
		void send_map_record_one(int32 record_key);
	public:
		std::shared_ptr<map_record> create_map_record(int32 map_template_id);
		std::shared_ptr<map_record> get_map_record(int32 map_template_id);
		std::shared_ptr<map_record> get_map_record(e_map_type map_type, int32 sub_type = 0);
		void upgrade_map(int32 map_template_id);
	private:
		int32 m_unit_array_index;
		std::unordered_map<uint32, std::shared_ptr<map_record>> m_map_record_map;
	};
}