#ifndef _FAITH_ACTIVITY_MSG_HPP_
#define _FAITH_ACTIVITY_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/best_record_def.hpp"
#include "dp2s.pb.h"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_load_all_best_record = faith::e_msg_base_best_record,
		e_msgindex_dp2ws_load_all_best_record,
		e_msgindex_ws2dp_save_best_record
	};

	struct ws2dp_load_all_best_record : public packet_base
	{
		ws2dp_load_all_best_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_all_best_record;
		}
	};

	struct s_db_best_record
	{
		int32	map_template_id;
		guid_64 role_guid;
		xchar	role_name[max_name_size_one_team + 1];
		int32	value;
		s_db_best_record()
		{
			memset(this, 0, sizeof(*this));
		}
		void to_proto(faith::st_proto::st_db_best_record * st_db_best_ptr)
		{
			if (st_db_best_ptr == nullptr)
			{
				return;
			}
			st_db_best_ptr->set_map_template_id(map_template_id);
			st_db_best_ptr->set_role_guid(role_guid.server_64);
			st_db_best_ptr->set_role_name(role_name);
			st_db_best_ptr->set_value(value);
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::st_proto::st_db_best_record& st_db_best_ref)
		{
			map_template_id = st_db_best_ref.map_template_id();
			role_guid.server_64 = st_db_best_ref.role_guid();
			my_memcopy_string(role_name, max_name_size, st_db_best_ref.role_name());
			value = st_db_best_ref.value();
		}
	};

	struct dp2ws_load_all_best_record : public packet_base
	{
		int32					db_record_num;
		s_db_best_record		db_record_list[faith::max_best_record_num];
		dp2ws_load_all_best_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_best_record;
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_best_record;
		}
		bool to_proto(faith::dp2s_proto::load_all_best_record& msg)
		{
			msg.set_db_record_num(db_record_num);
			for (int32 i = 0;  i < db_record_num && i < max_best_record_num; i++)
			{
				faith::st_proto::st_db_best_record *st_db_best_ptr = msg.add_db_record_list();
				if (st_db_best_ptr == nullptr)
				{
					return false;
				}
				db_record_list[i].to_proto(st_db_best_ptr);
			}
			return true;
		}
		void from_proto(const faith::dp2s_proto::load_all_best_record& msg)
		{
			db_record_num = msg.db_record_num();
			for (int32 i = 0; i < max_best_record_num && i < msg.db_record_list_size(); i++)
			{
				db_record_list[i].from_proto(msg.db_record_list(i));
			}
		}

	};

	struct ws2dp_save_best_record : public packet_base
	{
		s_best_record record_info;

		ws2dp_save_best_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_best_record;
		}
	};

#pragma pack(pop)
}

#endif