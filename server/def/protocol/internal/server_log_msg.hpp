/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 11:14
  file base: base_dp
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _LOG_MSG_HPP_
#define _LOG_MSG_HPP_

#include "logic/type_def.hpp"
#include "Logic/char_def.hpp"
#include "Logic/chat_def.hpp"
#include "Logic/legion_def.hpp"
#include "Logic/server_log_def.hpp"
#include "Logic/login_def.hpp"
#include <base.hpp>

using namespace faith;

namespace faith
{
	const int32 log_time_str_len = 32;

	enum 
	{
		e_msgindex_server2dp_save_log = e_msg_base_save_log,
		e_msgindex_server2dp_save_chat_log,
		e_msgindex_server2dp_save_legion_log,
	};

#pragma pack(push,1)

	struct server2dp_save_log : public packet_base
	{
		xchar			sql_table_name[max_name_size + 1];
		xchar			save_string[max_log_char_num + 1];
		server2dp_save_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_server2dp_save_log;
		}

		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};

	struct server2dp_save_chat_log : public packet_base
	{
		int32				server_id;
		xchar				log_time[log_time_str_len + 1];
		guid_64				role_guid;
		xchar				role_account[max_account_length + 1];
		xchar				role_name[max_name_size + 1];
		int32				role_level;
		guid_64				target_guid;
		int32				chat_type;
		int32				login_type;
		login_fixed_data	third_info;
		xchar				chat_content[chat_max_chat_size_with_hyper_link + 1];

		server2dp_save_chat_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_server2dp_save_chat_log;
		}

		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};

	struct server2dp_save_legion_log : public packet_base
	{
		int32				server_id;
		xchar				log_time[log_time_str_len + 1];
		guid_64				legion_guid;
		guid_64				role_guid;
		xchar				legion_name[max_name_size + 1];
		int32				operate_type;
		int32				login_type;
		int32               level;
		int64               power;
		int32               role_num;
		login_fixed_data	third_info;
		xchar				param_string[max_legion_announcement_size + 1];

		server2dp_save_legion_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_server2dp_save_legion_log;
		}
	};

#pragma pack(pop)
}
#endif
