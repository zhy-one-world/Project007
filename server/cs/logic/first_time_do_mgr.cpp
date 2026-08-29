
#include "connection_mgr.hpp"
#include "first_time_do_mgr.h"
#include "logic/unit_man.h"
#include "logic/player.hpp"
#include "utility/parse_msg.h"
#include "internal/first_time_do_msg.hpp"
#include "internet/net.pb.h"
#include "internet/first_time_do.pb.h"

namespace faith
{
	first_time_do_mgr::first_time_do_mgr()
	{
		m_unit_index = 0;
		clear_data();
	}

	void first_time_do_mgr::clear_data()
	{
		for (int32 i = 0; i < e_first_time_do_type_max; ++i)
		{
			m_first_time_do_record_list[i].clear_data();
		}
	}

	bool first_time_do_mgr::is_invalid_do_type(e_first_time_do_type do_type)
	{
		if (do_type >= e_first_time_do_type_max)
		{
			return false;
		}
		return true;
	}

	void first_time_do_mgr::save_first_time_do_record_to_db(e_first_time_do_type do_type)
	{
		if (is_invalid_do_type(do_type) == false)
		{
			return;
		}

		player& player_ref = unit_man::get_player(get_unit_index());
		if (false == player_ref.is_valid())
		{
			return;
		}

		cs2dp_save_first_time_do_record save_msg;
		save_msg.role_guid = player_ref.get_unit_guid();
		save_msg.do_type = do_type;
		player_ref.send_message_to_dp( &save_msg, sizeof(save_msg));
	}

	void first_time_do_mgr::sync_first_time_do_record_to_client(e_first_time_do_type do_type)
	{
		if (is_invalid_do_type(do_type) == false)
		{
			return;
		}

		player& player_ref = unit_man::get_player(get_unit_index());
		if (false == player_ref.is_valid())
		{
			return;
		}
		
		first_time_do_proto_sync_first_time_do_record sync_msg;
		first_time_do_proto_first_do_record* record_msg = sync_msg.mutable_record();
		record_msg->set_do_type(do_type);

		player_ref.send_message_to_self(&sync_msg, e_msgindex_s2c_sync_first_time_do_record);
	}

	void first_time_do_mgr::load_first_time_do_records_from_db(const s_first_time_do_record records[e_first_time_do_type_max], int32 record_num)
	{
		if (record_num >= e_first_time_do_type_max)
		{
			return;
		}

		player& player_ref = unit_man::get_player(get_unit_index());
		if (false == player_ref.is_valid())
		{
			return;
		}

		e_first_time_do_type do_type = e_first_time_do_type_none;
		for (int32 i = 0; i < record_num; ++i)
		{
			do_type = (e_first_time_do_type)(records[i].do_type);
			add_do_record(do_type, false);
		}

		send_all_first_time_do_records();
	}
	bool first_time_do_mgr::load_frist_time_do_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		faith::db_proto::role_first_time_do_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_first_time_do_record));
		if (p_data == nullptr)
		{
			return false;
		}
		s_first_time_do_record *p_row = (s_first_time_do_record *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			faith::db_proto::role_first_time_do_row db_row = msg.row_data(i);
			p_row->do_type = db_row.do_type();

			p_row++;
		}
		load_first_time_do_records_from_db((const s_first_time_do_record *)p_data, msg.row_count());
		return true;

	}

	void first_time_do_mgr::send_all_first_time_do_records()
	{
		player& player_ref = unit_man::get_player(get_unit_index());
		if (false == player_ref.is_valid())
		{
			return;
		}

		first_time_do_proto_update_first_time_do_records update_all_records;
		update_all_records.set_records_num(0);

		int32 record_count = 0;
		for (int32 i = 0; i < e_first_time_do_type_max; ++i)
		{
			if (m_first_time_do_record_list[i].is_valid() == true)
			{
				first_time_do_proto_first_do_record* record_msg = update_all_records.add_records();
				if (nullptr == record_msg)
				{
					continue;
				}
				record_msg->set_do_type(m_first_time_do_record_list[i].do_type);
				++record_count;
			}
		}
		update_all_records.set_records_num(record_count);
		player_ref.send_message_to_self(&update_all_records, e_msgindex_s2c_update_first_time_do_records);
	}

	bool first_time_do_mgr::exist_do_record(e_first_time_do_type do_type)
	{
		if (is_invalid_do_type(do_type) == false)
		{
			return false;
		}
		
		for (int32 i = 0; i < e_first_time_do_type_max; ++i)
		{
			if (do_type == m_first_time_do_record_list[i].do_type)
			{
				return true;
			}
		}

		return false;
	}

	bool first_time_do_mgr::is_first_time_do(e_first_time_do_type do_type)
	{
		if (is_invalid_do_type(do_type) == false)
		{
			return false;
		}
		if (exist_do_record(do_type) == false)
		{
			return true;
		}
		return false;
	}

	bool first_time_do_mgr::add_do_record(e_first_time_do_type do_type, bool save_to_db)
	{
		if (is_invalid_do_type(do_type) == false)
		{
			return false;
		}
		if (exist_do_record(do_type) == true)
		{
			return false;
		}

		for (int32 i = 0; i < e_first_time_do_type_max; ++i)
		{
			if (m_first_time_do_record_list[i].is_valid() == false)
			{
				m_first_time_do_record_list[i].do_type = do_type;
				if (save_to_db)
				{
					save_first_time_do_record_to_db(do_type);
					sync_first_time_do_record_to_client(do_type);
				}
				return true;
			}
		}

		return false;
	}
}
