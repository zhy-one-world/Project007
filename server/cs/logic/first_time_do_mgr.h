#ifndef _FIRST_TIME_DO_MGR_H_
#define _FIRST_TIME_DO_MGR_H_

#include "logic/type_def.hpp"
#include "Logic/first_time_do_def.hpp"

namespace faith
{
	class first_time_do_mgr
	{
	public:
		explicit first_time_do_mgr();
		void clear_data();

		void set_unit_index(int32 unit_index) { m_unit_index = unit_index; }
		const int32 get_unit_index() { return m_unit_index; }

		bool is_invalid_do_type(e_first_time_do_type do_type);
		bool exist_do_record(e_first_time_do_type do_type);

		bool is_first_time_do(e_first_time_do_type do_type);
		
		// 有新加的首次类型只需要在适当的位置调用这个接口并把类型传入即可
		bool add_do_record(e_first_time_do_type do_type, bool save_to_db = true);

		void load_first_time_do_records_from_db(const s_first_time_do_record records[e_first_time_do_type_max], int32 record_num);
		bool load_frist_time_do_by_db_lua(const char *data_ptr, int32 data_len);
		void send_all_first_time_do_records();

	private:
		void save_first_time_do_record_to_db(e_first_time_do_type do_type);
		void sync_first_time_do_record_to_client(e_first_time_do_type do_type);

	private:
		int32					m_unit_index;
		s_first_time_do_record	m_first_time_do_record_list[e_first_time_do_type_max];
	};
}

#endif