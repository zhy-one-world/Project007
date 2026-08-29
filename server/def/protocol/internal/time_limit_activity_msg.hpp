/********************************************************************
created: 2018/07/28
file base: time limit activity
file ext: hpp
author: shangxiaoliang

purpose:
*********************************************************************/
#ifndef _FAITH_TIME_LIMIT_ACTIVITY_HPP_
#define _FAITH_TIME_LIMIT_ACTIVITY_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"   
#include "Logic/time_limit_activity_def.hpp"
#include "internet/ws2cs.pb.h"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_cs2dp_load_time_limit_activity_info = faith::e_msg_base_time_limit_activity,
		e_msgindex_dp2cs_load_time_limit_activity_info_end,
		e_msgindex_cs2dp_save_time_limit_activity_info,
		e_msgindex_ws2dp_load_time_limit_temp,
		e_msgindex_dp2ws_load_time_limit_temp_end,
		e_msgindex_dp2cs_load_time_limit_branch_temp_end,
		e_msgindex_ws2dp_save_time_limit_temp,
		e_msgindex_ws2dp_save_time_limit_branch_temp,
		e_msgindex_ws2cs_time_limit_temp,
		e_msgindex_ws2cs_time_limit_branch_temp,
		e_msgindex_ws2cs_end_act_type,
		e_msgindex_ws2dp_load_time_limit_info,
		e_msgindex_dp2ws_load_time_limit_info_end,
		e_msgindex_ws2dp_save_time_limit_info,
		e_msgindex_ws2dp_update_role_time_limit,
		e_msgindex_ws2dp_delete_time_limit_temp,
		e_msgindex_ws2dp_delete_time_limit_branch_temp,
		e_msgindex_ws2dp_refresh_role_time_limit_guid,
		e_msgindex_ws2fep_time_limit_temp,
		e_msgindex_ws2fep_time_limit_branch_temp,
	};

	struct ws2dp_load_time_limit_ws_info : public packet_base
	{
		ws2dp_load_time_limit_ws_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_time_limit_info;
		}
	};

	struct ws2dp_save_time_limit_ws_info : public packet_base
	{
		s_time_limit_ws_info			act_info[e_time_limit_activity_type_max];
		int32							data_num;
		ws2dp_save_time_limit_ws_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_time_limit_info;
		}
	};

	struct dp2ws_load_time_limit_ws_info_end : public packet_base
	{
		s_time_limit_ws_info			act_info[time_limit_activity_dp_send_size];
		int32							data_num;
		int32							is_end;
		dp2ws_load_time_limit_ws_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_time_limit_info_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&act_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_time_limit_ws_info));
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_time_limit_info_end;
		}
	};

	struct cs2dp_save_time_limit_activity_info : public packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		int32 save_type_ex;
		bool  is_self_server;
		int32 data_num;
		s_time_limit_activity_info_db activity_data_array[max_save_num_with_one_player];

		cs2dp_save_time_limit_activity_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_time_limit_activity_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&activity_data_array - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_time_limit_activity_info_db));
		}
	};

	struct dp2cs_load_time_limit_activity_info_end : public packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		int32 data_num;
		s_time_limit_activity_info_db	data_list[max_save_num_with_one_player];
		dp2cs_load_time_limit_activity_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_time_limit_activity_info_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_time_limit_activity_info_db));
		}
	};

	struct ws2cs_act_limit_temp : public packet_base
	{
		s_time_limit_activity_temp_db temp_db_info[max_send_cs2ws_once_num];
		bool						is_begin;
		int32						data_num;
		ws2cs_act_limit_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_time_limit_temp;
		}
		void clear_data()
		{
			is_begin = false;
			data_num = 0;
			for (int32 i = 0; i < max_send_cs2ws_once_num; i++)
			{
				temp_db_info[i].clear_data();
			}
		}
		bool to_proto(faith::ws2cs_proto::act_limit_temp& msg)
		{
			for (int32 i = 0; i < data_num && i < max_send_cs2ws_once_num; i++)
			{
				faith::st_proto::st_time_limit_activity_temp_db *st_act_ptr = msg.add_temp_db_info();
				if (st_act_ptr == nullptr)
				{
					return false;
				}
				temp_db_info[i].to_proto(st_act_ptr);
			}
			msg.set_is_begin(is_begin);
			msg.set_data_num(data_num);
			return true;
		}
		void from_proto(const faith::ws2cs_proto::act_limit_temp& msg)
		{
			data_num = msg.data_num();
			is_begin = msg.is_begin();
			for (int32 i = 0; i < data_num && i < max_send_cs2ws_once_num; i++)
			{
				temp_db_info[i].from_proto(msg.temp_db_info(i));	
			}
		}
	};

	struct ws2cs_act_limit_branch_temp : public packet_base
	{
		s_time_limit_activity_branch_temp_db temp_db_info[max_send_cs2ws_once_num];
		bool						is_end;
		int32						data_num;
		ws2cs_act_limit_branch_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_time_limit_branch_temp;
		}
		void clear_data()
		{
			is_end = false;
			data_num = 0;
			for (int32 i = 0; i < max_send_cs2ws_once_num; i++)
			{
				temp_db_info[i].clear_data();
			}
		}
		bool to_proto(faith::ws2cs_proto::act_limit_branch_temp& msg)
		{
			 for (int32 i = 0; i < data_num && i < max_send_cs2ws_once_num; i++)
			 {
				 faith::st_proto::st_time_limit_activity_branch_temp_db *st_act_branch_ptr = msg.add_temp_db_info();
				 if (st_act_branch_ptr == nullptr)
				 {
					 return false;
				 }
				 temp_db_info[i].to_proto(st_act_branch_ptr);		 
			 }
			 msg.set_is_end(is_end);
			 msg.set_data_num(data_num);
			 return true;
		}

		void from_proto(const faith::ws2cs_proto::act_limit_branch_temp& msg)
		{
			is_end = msg.is_end();
			data_num = msg.data_num();

			for (int32 i = 0; i < data_num && i < max_send_cs2ws_once_num; i++)
			{
				temp_db_info[i].from_proto(msg.temp_db_info(i));
			}
		}
	};

	struct ws2fep_act_limit_temp : public packet_base
	{
		s_time_limit_activity_temp_db temp_db_info[max_send_cs2ws_once_num];
		bool						is_begin;
		int32						data_num;
		ws2fep_act_limit_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2fep_time_limit_temp;
		}
		void clear_data()
		{
			is_begin = false;
			data_num = 0;
			for (int32 i = 0; i < max_send_cs2ws_once_num; i++)
			{
				temp_db_info[i].clear_data();
			}
		}
	};

	struct ws2fep_act_limit_branch_temp : public packet_base
	{
		s_time_limit_activity_branch_temp_db temp_db_info[max_send_cs2ws_once_num];
		bool						is_end;
		int32						data_num;
		ws2fep_act_limit_branch_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2fep_time_limit_branch_temp;
		}
		void clear_data()
		{
			is_end = false;
			data_num = 0;
			for (int32 i = 0; i < max_send_cs2ws_once_num; i++)
			{
				temp_db_info[i].clear_data();
			}
		}
	};

	struct ws2cs_end_act_type : public packet_base
	{
		int32 act_type;
		bool is_need_send_to_client;
		ws2cs_end_act_type()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_end_act_type;
		}
		void to_proto(faith::ws2cs_proto::end_act_type& msg)
		{
			msg.set_act_type(act_type);
			msg.set_is_need_send_to_client(is_need_send_to_client);
		}
		void from_proto(const faith::ws2cs_proto::end_act_type& msg)
		{
			act_type = msg.act_type();
			is_need_send_to_client = msg.is_need_send_to_client();
		}
	};

	struct ws2dp_save_time_limit_temp : public packet_base
	{
		s_time_limit_activity_temp_db temp_db_info;
		ws2dp_save_time_limit_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_time_limit_temp;
		}
	};

	struct ws2dp_update_role_time_limit : public packet_base
	{
		int32 act_guid;
		int32 act_type;
		ws2dp_update_role_time_limit()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_update_role_time_limit;
		}
	};

	struct ws2dp_save_time_limit_branch_temp : public packet_base
	{
		s_time_limit_activity_branch_temp_db temp_db_info;
		ws2dp_save_time_limit_branch_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_time_limit_branch_temp;
		}
	};

	struct ws2dp_load_time_limit_temp : public packet_base
	{
		ws2dp_load_time_limit_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_time_limit_temp;
		}
	};

	struct dp2ws_load_time_limit_temp_end : public packet_base
	{
		bool							is_load_all;
		int32							data_num;
		s_time_limit_activity_temp_db	temp_info[max_load_time_limit_temp_num];
		dp2ws_load_time_limit_temp_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_time_limit_temp_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&temp_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_time_limit_activity_temp_db));
		}
		void clear_data()
		{
			is_load_all = false;
			data_num = 0;
			memset(temp_info, 0, sizeof(temp_info));
		}
	};

	struct dp2ws_load_time_limit_branch_temp_end : public packet_base
	{
		bool									is_load_all;
		int32									data_num;
		s_time_limit_activity_branch_temp_db	temp_info[max_load_time_limit_temp_num];
		dp2ws_load_time_limit_branch_temp_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_time_limit_branch_temp_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&temp_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_time_limit_activity_temp_db));
		}
		void clear_data()
		{
			is_load_all = false;
			data_num = 0;
			memset(temp_info, 0, sizeof(temp_info));
		}
	};

	struct ws2dp_delete_time_limit_temp : public packet_base
	{
		int32 act_guid;
		ws2dp_delete_time_limit_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_delete_time_limit_temp;
		}
	};

	struct ws2dp_delete_time_limit_branch_temp : public packet_base
	{
		int32 act_guid;
		ws2dp_delete_time_limit_branch_temp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_delete_time_limit_branch_temp;
		}
	};

	struct ws2dp_refresh_role_time_limit_guid : public packet_base
	{
		int32 old_act_guid;
		int32 new_act_guid;
		ws2dp_refresh_role_time_limit_guid()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_refresh_role_time_limit_guid;
		}
	};

#pragma pack(pop)
}
#endif