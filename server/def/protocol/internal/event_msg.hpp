/********************************************************************
created: 2016/08/25
file base: event
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_EVENT_HPP_
#define _FAITH_EVENT_HPP_

#include "base.hpp"
#include "internet/cs2ws.pb.h"
#include "internet/ws2cs.pb.h"
#include "Logic/char_def.hpp"
#include "Logic/event_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/mail_def.hpp"
#include "logic/type_def.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_cs2ws_check_player_globel_mail_event = faith::e_msg_base_event,
		e_msgindex_dp2cs_load_player_had_globel_mail_end,
		e_msgindex_ws2dp_save_mail_event,
		e_msgindex_ws2dp_save_mail_event_item,
		e_msgindex_ws2dp_load_mail_event,
		e_msgindex_ws2dp_load_mail_event_item,
		e_msgindex_dp2ws_load_mail_event_end,
		e_msgindex_dp2ws_load_mail_event_item_end,
		e_msgindex_ws2cs_had_new_globel_mail,
		e_msgindex_ws2dp_save_new_globel_mail_had_info,

		e_msgindex_cs2ws_reconnect_send_red_pack_info,
		e_msgindex_cs2ws_send_red_pack,
		e_msgindex_ws2cs_send_red_pack_end,
		e_msgindex_cs2ws_get_red_pack,
		e_msgindex_ws2cs_get_red_pack_end,
		e_msgindex_ws2dp_save_red_pack,
		e_msgindex_ws2dp_load_red_pack,
		e_msgindex_ws2dp_load_red_pack_receiver,
		e_msgindex_ws2dp_add_red_pack_to_sql,
		e_msgindex_ws2dp_add_red_pack_receiver_to_sql,
		e_msgindex_ws2dp_del_red_pack_by_sql,

		e_msgindex_dp2ws_load_red_pack_end,
		e_msgindex_dp2ws_load_red_pack_receiver_end,
	};

	struct dp2cs_load_player_had_globel_mail_end : public faith::packet_base
	{
		int32								unit_index;
		guid_64								role_guid;
		guid_64								data_info[max_globel_mail_in_same_time];

		dp2cs_load_player_had_globel_mail_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_player_had_globel_mail_end;
		}
	};

	struct ws2cs_had_new_globel_mail : public faith::packet_base
	{
		guid_64								role_guid;
		int32								data_num;
		guid_64								data_info[max_globel_mail_in_same_time];
		ws2cs_had_new_globel_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_had_new_globel_mail;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num* sizeof(guid_64));
		}
		void to_proto(faith::ws2cs_proto::had_new_globel_mail& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_data_num(data_num);
			for (int32 i = 0; i < max_globel_mail_in_same_time; i++)
			{
				msg.add_data_info(data_info[i]);
			}
		}
		void from_proto(const faith::ws2cs_proto::had_new_globel_mail& msg)
		{
			role_guid.server_64 = msg.role_guid();
			data_num = msg.data_num();
			for (int32 i = 0; i < max_globel_mail_in_same_time && i < msg.data_info_size(); i++)
			{
				data_info[i] = msg.data_info(i);
			}
		}
	};

	struct ws2dp_save_new_globel_mail_had_info : public faith::packet_base
	{
		guid_64								role_guid;
		guid_64								data_info[max_globel_mail_in_same_time];
		ws2dp_save_new_globel_mail_had_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_new_globel_mail_had_info;
		}
	};

	struct cs2ws_check_player_globel_mail_event : public faith::packet_base
	{
		guid_64								role_guid;
		int64								role_creat_time;
		int32								data_num;
		guid_64								data_info[max_globel_mail_in_same_time];

		cs2ws_check_player_globel_mail_event()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_check_player_globel_mail_event;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num *  sizeof(guid_64));
		}
	};

	struct ws2dp_save_mail_event_to_db : public faith::packet_base
	{
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32									mail_num;
		s_mail_info							mail_info[max_globel_mail_in_same_time];

		ws2dp_save_mail_event_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_mail_event;
		}
		uint32 get_pak_length() const
		{

			const int32 basic_len = (ULONG_PTR)&mail_info - (ULONG_PTR)&wheader;
			return (basic_len + mail_num * sizeof(s_mail_info));
		}
	};

	struct ws2dp_save_mail_event_item_to_db : public faith::packet_base
	{
		guid_64								mail_guid;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								item_num;
		s_item_info							item_list[max_globel_mail_item_in_same_time];

		ws2dp_save_mail_event_item_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_mail_event_item;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&item_list - (ULONG_PTR)&wheader;
			return (basic_len + item_num * sizeof(s_item_info));
		}
	};

	struct ws2dp_load_mail_event : public faith::packet_base
	{
		//这里什么都不用传，其实就是个load使能，load整张表
		bool								is_over;
		ws2dp_load_mail_event()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_mail_event;
		}
	};

	struct ws2dp_load_mail_event_item : public faith::packet_base
	{
		//这里什么都不用传，其实就是个load使能，load整张表
		bool								is_over;
		ws2dp_load_mail_event_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_mail_event_item;
		}
	};



	struct dp2ws_load_mail_event_end : public faith::packet_base
	{
		guid_64								role_guid;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								mail_num;
		s_mail_info							mail_info[max_globel_mail_in_same_time];

		dp2ws_load_mail_event_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_mail_event_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&mail_info - (ULONG_PTR)&wheader;
			return (basic_len + mail_num * sizeof(s_mail_info));
		}
	};



	struct dp2ws_load_mail_event_item_end : public faith::packet_base
	{
		guid_64								role_guid;
		bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								item_num;
		s_item_info							item_list[max_item_per_mail * max_globel_mail_in_same_time];

		dp2ws_load_mail_event_item_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_mail_event_item_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&item_list - (ULONG_PTR)&wheader;
			return (basic_len + item_num * sizeof(s_item_info));
		}
	};

	struct cs2ws_reconnect_send_red_package_info : public faith::packet_base
	{
		guid_64								role_guid;
		cs2ws_reconnect_send_red_package_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_reconnect_send_red_pack_info;
		}
	};

	struct cs2ws_send_red_package : public faith::packet_base
	{
		guid_64								sender_guid;
		int32								red_package_template_id;
		xchar								send_text[red_package_max_text_length + 1];
		xchar								sender_name[max_name_size + 1];
		cs2ws_send_red_package()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_red_pack;
		}
		void to_proto(faith::cs2ws_proto::send_red_package& msg)
		{
			msg.set_sender_guid(sender_guid.server_64);
			msg.set_red_package_template_id(red_package_template_id);
			msg.set_send_text(send_text);
			msg.set_sender_name(sender_name);

		}
		void my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::cs2ws_proto::send_red_package& msg)
		{
			sender_guid.server_64 = msg.sender_guid();
			red_package_template_id = msg.red_package_template_id();
			my_memcopy_string(send_text, red_package_max_text_length, msg.send_text());
			my_memcopy_string(sender_name, max_name_size, msg.sender_name());
		}
	};

	struct ws2cs_send_red_package_end : public faith::packet_base
	{
		guid_64								sender_guid;
		int32								red_package_template_id;
		int32								send_result;
		ws2cs_send_red_package_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_send_red_pack_end;
		}
		void to_proto(faith::ws2cs_proto::send_red_package_end& msg)
		{
			msg.set_sender_guid(sender_guid.server_64);
			msg.set_red_package_template_id(red_package_template_id);
			msg.set_send_result(send_result);
		}
		void from_proto(const faith::ws2cs_proto::send_red_package_end& msg)
		{
			sender_guid.server_64 = msg.sender_guid();
			red_package_template_id = msg.red_package_template_id();
			send_result = msg.send_result();
		}
	};

	struct cs2ws_get_red_package : public faith::packet_base
	{
		guid_64								role_guid;
		guid_64								red_package_guid;
		xchar								role_name[max_name_size + 1];
		cs2ws_get_red_package()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_get_red_pack;
		}
		void to_proto(faith::cs2ws_proto::get_red_package& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_red_package_guid(red_package_guid.server_64);
			msg.set_role_name(role_name);

		}

		void my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}

		void from_proto(faith::cs2ws_proto::get_red_package& msg)
		{
			role_guid.server_64 = msg.role_guid();
			red_package_guid.server_64 = msg.red_package_guid();
			my_memcopy_string(role_name, max_name_size, msg.role_name());
		}
	};

	struct ws2dp_save_red_package : public faith::packet_base
	{
		int32					data_num;
		s_red_package			red_package_list[red_package_total_num];
		ws2dp_save_red_package()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_red_pack;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&red_package_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_red_package));
		}
	};

	struct ws2dp_load_red_package : public faith::packet_base
	{
		ws2dp_load_red_package()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_red_pack;
		}
	};

	struct ws2dp_load_red_package_receiver : public faith::packet_base
	{
		guid_64				red_package_guid;
		ws2dp_load_red_package_receiver()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_red_pack_receiver;
		}
	};

	struct dp2ws_load_red_package_end : public faith::packet_base
	{
		int32				data_num;
		s_red_package_base	red_package_list[red_package_total_num];
		dp2ws_load_red_package_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_red_pack_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&red_package_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_red_package_base));
		}
		bool to_proto(faith::dp2s_proto::load_red_package_end& msg)
		{
			msg.set_data_num(data_num);
			for (int32 i = 0; i < data_num && i < red_package_total_num; i++)
			{
				faith::dp2s_proto::s_red_package_base* red_base_ptr = msg.add_red_package_list();
				if (red_base_ptr == nullptr)
				{
					return false;
				}
				red_package_list[i].to_proto(red_base_ptr);
			}
			return true;
		}

		void from_proto(const faith::dp2s_proto::load_red_package_end& msg)
		{
			data_num =  msg.data_num();

			for (int32 i = 0; i < data_num && i < red_package_total_num; i++)
			{
				red_package_list[i].from_proto(msg.red_package_list(i));
			}

		}

	};

	struct dp2ws_load_red_package_receiver_end : public faith::packet_base
	{
		int32					data_num;
		s_red_package_receiver	receiver_list[red_package_max_receiver_num];
		dp2ws_load_red_package_receiver_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_red_pack_receiver_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&receiver_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_red_package_receiver));
		}
		bool to_proto(faith::dp2s_proto::load_red_package_receiver_end& msg)
		{
			msg.set_data_num(data_num);

			for (int32 i = 0; i < data_num && i < red_package_max_receiver_num; i++)
			{
				faith::dp2s_proto::s_red_package_receiver* red_rreceiver_ptr = msg.add_receiver_list();
				if (red_rreceiver_ptr == nullptr)
				{
					return false;
				}
				receiver_list[i].to_proto(red_rreceiver_ptr);

			}
			return true;
		}
		void from_proto(faith::dp2s_proto::load_red_package_receiver_end& msg)
		{
			data_num = msg.data_num();
			for (int32 i = 0; i < data_num && i < red_package_max_receiver_num; i++)
			{
				receiver_list[i].from_proto(msg.receiver_list(i));
			}

		}
	};

	struct ws2cs_get_red_package_end : public faith::packet_base
	{
		guid_64	receiver_guid;
		guid_64	red_package_guid;;
		int32	get_result;
		int32	money_type;
		int32	money_num;
		ws2cs_get_red_package_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_get_red_pack_end;
		}
		void to_proto(faith::ws2cs_proto::get_red_package_end& msg)
		{
			msg.set_receiver_guid(receiver_guid.server_64);
			msg.set_red_package_guid(red_package_guid.server_64);
			msg.set_get_result(get_result);
			msg.set_money_type(money_type);
			msg.set_money_num(money_num);
		}
		void from_proto(const faith::ws2cs_proto::get_red_package_end& msg)
		{
			receiver_guid.server_64 = msg.receiver_guid();
			red_package_guid.server_64 = msg.red_package_guid();
			get_result = msg.get_result();
			money_type = msg.money_type();
			money_num = msg.money_num();
		}
	};

	struct ws2dp_add_red_package_to_sql : public faith::packet_base
	{
		s_red_package_base	red_package_info;
		ws2dp_add_red_package_to_sql()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_add_red_pack_to_sql;
		}
	};

	struct ws2dp_add_red_package_receiver_to_sql : public faith::packet_base
	{
		s_red_package_receiver	receiver_info;
		ws2dp_add_red_package_receiver_to_sql()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_add_red_pack_receiver_to_sql;
		}
	};

	struct ws2dp_del_red_package_by_sql : public faith::packet_base
	{
		guid_64	red_package_guid;
		ws2dp_del_red_package_by_sql()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_del_red_pack_by_sql;
		}
	};

#pragma pack(pop)
}
#endif