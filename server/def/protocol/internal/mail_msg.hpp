/********************************************************************
created: 2016/08/15
file base: mail
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_MAIL_HPP_
#define _FAITH_MAIL_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/mail_def.hpp"
#include "Logic/item_def.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_dp2cs_load_char_mail = faith::e_msg_base_mail,
		e_msgindex_dp2cs_load_char_mail_item,
		e_msgindex_cs2dp_auto_load_mail_by_db,
		e_msgindex_cs2ws_check_id_mail,
		e_msgindex_ws2dp_check_id_mail,
		e_msgindex_dp2ws_check_id_mail_end,
		e_msg_index_ws2dp_add_mail_to_sql,
		e_msg_index_dp2ws_add_mail_to_sql_failed,
		e_msgindex_cs2dp_del_char_mail,
		e_msgindex_cs2dp_del_char_mail_item,
		e_msgindex_cs2dp_add_char_mail,
		e_msgindex_dp2cs_del_char_mail_end,
		e_msgindex_dp2cs_operate_load_lock_mail,
		e_msgindex_dp2ws_load_other_server_mail_record_end,
		e_msgindex_ws2dp_load_other_server_mail,
		e_msg_index_dp2ws_load_other_server_mail_end,
		e_msgindex_ws2ws_send_other_server_mail,
		e_msgindex_ws2ws_send_other_server_mail_end,
		e_msgindex_ws2dp_del_other_server_mail,
		e_msgindex_ws2dp_save_other_server_mail_record,
	};

	// DP2CS 分批发送玩家的所有物品信息
	struct dp2cs_load_char_mail : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_index;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32									data_num;
		s_mail_info							data_list[MAX_MAIL_NUM];
		dp2cs_load_char_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_mail;
		}
		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_mail_info));
		}
	};

	struct dp2cs_operate_load_lock_mail : public faith::packet_base
	{
		int32								unit_index;
		guid_64								unit_guid;
		int32								operate_value;
		dp2cs_operate_load_lock_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_operate_load_lock_mail;
		}
	};

	struct dp2cs_load_char_mail_item : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								data_num;
		s_item_info							mail_item_data[MAX_MAIL_NUM*max_item_per_mail];
		dp2cs_load_char_mail_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_mail_item;
		}
		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&mail_item_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_item_info));
		}
	};

	struct dp2cs_del_mail_end : public faith::packet_base
	{
		guid_64								unit_guid;
		int32								unit_index;
		int32								del_result;
		dp2cs_del_mail_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_del_char_mail_end;
		}
	};

	struct cs2dp_add_char_mail : public faith::packet_base
	{
		guid_64								role_guid;
		s_mail_info							mail_info;
		cs2dp_add_char_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_add_char_mail;
		}
	};

	struct cs2dp_del_char_mail : public faith::packet_base
	{
		guid_64								unit_guid;
		int32								unit_index;
		int32								data_num;
		guid_64								mail_guid[MAX_MAIL_NUM];
		cs2dp_del_char_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_del_char_mail;
		}
	};

	struct cs2dp_del_char_mail_item : public faith::packet_base
	{
		guid_64								unit_guid;
		guid_64								item_guid;
		cs2dp_del_char_mail_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_del_char_mail_item;
		}
	};

	struct cs2dp_auto_load_mail_by_db : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		cs2dp_auto_load_mail_by_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_auto_load_mail_by_db;
		}
	};

	struct ws2dp_add_mail_to_sql : public faith::packet_base
	{
		int32		address_server_id;
		s_item_info item_list[max_item_per_mail];
		s_mail_info mail_info;
		int32		item_num;

		xchar		role_mark[max_account_length + 1];
		int32		mark_type;
		int32		resend_times;
		ws2dp_add_mail_to_sql() 
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2dp_add_mail_to_sql;
		}
	};

	struct dp2ws_add_mail_to_sql_failed : public faith::packet_base
	{
		s_item_info item_list[max_item_per_mail];
		s_mail_info mail_info;
		int32		item_num;
		dp2ws_add_mail_to_sql_failed() 
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_dp2ws_add_mail_to_sql_failed;
		}
	};

	struct cs2ws_check_id_mail : public faith::packet_base
	{
		guid_64								role_guid;
		int32								cur_time;
		int32								server_id;
		cs2ws_check_id_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_check_id_mail;
		}
	};

	struct ws2dp_check_id_mail : public faith::packet_base
	{
		guid_64								role_guid;
		int32								server_id;
		int32								cur_time;
		ws2dp_check_id_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_check_id_mail;
		}
	};

	struct dp2ws_check_id_mail_end : public faith::packet_base
	{
		guid_64								role_guid;
		int32								server_id;
		int32								data_num;
		s_id_mail_info						data_array[check_id_mail_per_time];
		dp2ws_check_id_mail_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_check_id_mail_end;
		}
	};

	struct dp2ws_load_other_server_mail_record_end : public faith::packet_base
	{
		int32 data_num;
		s_other_server_mail_record mail_record[max_other_server_mail_record_num];

		dp2ws_load_other_server_mail_record_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_other_server_mail_record_end;
		}
		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&mail_record - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_other_server_mail_record));
		}
	};
	struct ws2dp_save_other_server_mail_record : public faith::packet_base
	{
		s_other_server_mail_record mail_record;

		ws2dp_save_other_server_mail_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_other_server_mail_record;
		}
	};

	struct ws2dp_load_other_server_mail : public faith::packet_base
	{
		int32 server_id;
		guid_64 mail_guid;
		ws2dp_load_other_server_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_other_server_mail;
		}
	};

	struct dp2ws_load_other_server_mail_end : public faith::packet_base
	{
		int32 data_num;
		int32 server_id;
		s_other_server_mail_info mail_info[max_other_server_mail_num];
		dp2ws_load_other_server_mail_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_dp2ws_load_other_server_mail_end;
		}
		i64 get_pak_length() const
		{
			const i64 basic_len = (ULONG_PTR)&mail_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_other_server_mail_info));
		}
	};
	struct ws2dp_del_other_server_mail : public faith::packet_base
	{
		s_other_server_mail_record mail_record;
		ws2dp_del_other_server_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_del_other_server_mail;
		}
	};

	struct ws2ws_send_other_server_mail : public faith::packet_base
	{
		s_other_server_mail_record mail_record;
		s_other_server_mail_info mail_info;
		ws2ws_send_other_server_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_other_server_mail;
		}
	};

	struct ws2ws_send_other_server_mail_end : public faith::packet_base
	{
		s_other_server_mail_record mail_record;
		ws2ws_send_other_server_mail_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_other_server_mail_end;
		}
	};

#pragma pack(pop)
}

#endif