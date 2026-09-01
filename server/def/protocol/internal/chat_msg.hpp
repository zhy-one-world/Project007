/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 21:41
  file base: chat
  file ext: hpp
  author: lxy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_CHAT_HPP_
#define _FAITH_CHAT_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/chat_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/relation_def.h"
#include "internet/ws2cs.pb.h"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_cs2ws_send_chat_to_ws = faith::e_msg_base_chat,
		e_msgindex_ws2cs_send_chat_end,
		e_msgindex_ws2cs_receive_chat,
		e_msgindex_cs2ws_send_chat_end,
		e_msgindex_cs2ws_send_chat_to_ws_new,
		e_msgindex_ws2cs_receive_chat_new,
		e_msgindex_cs2ws_send_quick_chat_to_ws,
		e_msgindex_ws2cs_receive_quick_chat,
		e_msgindex_cs2ws_send_notice_with_param,
		e_msgindex_ws2cs_receive_notice_with_param,
		e_msgindex_ws2dp_save_chat_with_offline,
		e_msgindex_ws2dp_load_chat_with_offline,
		e_msgindex_dp2ws_load_chat_with_offline,
		e_msgindex_ws2dp_eliminate_chat_with_offline,
		e_msgindex_cs2dp_save_chat_record,
		e_msgindex_dp2cs_load_chat_record,
		e_msgindex_ws2ws_load_offline_other_relation,
		e_msgindex_ws2ws_load_offline_other_relation_end,
	};

	struct cs2ws_send_chat_to_ws : public faith::packet_base
	{
		guid_64		sender_guid;
		guid_64		addressee_guid;
		int32			chat_typ;
		xchar		sender_name[max_name_size + 1];
		xchar		content_text[chat_max_chat_size + 1];
		int32			item_num;
		s_item_info item_list[chat_max_item];
		cs2ws_send_chat_to_ws()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_chat_to_ws;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&item_list - (ULONG_PTR)&wheader;
			return (basic_len + item_num * sizeof(s_item_info));
		}
	};

	struct cs2ws_send_chat_to_ws_new : public faith::packet_base
	{
		guid_64					sender_guid;
		int32					sender_template_id;
		int32					sender_exp_level;
		int32					sender_vip_title_template_id;
		int32					sender_head_frame_id;
		guid_64					addressee_guid;
		int32					chat_type;
		xchar					sender_name[max_name_size + 1];
		int32					sender_server_id;
		int32					final_img_id;
		int32					content_info_num;
		e_chat_content_info		content_info[chat_max_word_num]; 

		cs2ws_send_chat_to_ws_new()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_chat_to_ws_new;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&content_info - (ULONG_PTR)&wheader;
			return (basic_len + content_info_num * sizeof(e_chat_content_info));
		}
	};
	
	struct ws2cs_send_chat_end : public faith::packet_base
	{
		guid_64 sender_guid;
		int32 send_result;
		int32 chat_type;
		ws2cs_send_chat_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_send_chat_end;
		}
		void to_proto(faith::ws2cs_proto::send_chat_end& msg)
		{
			msg.set_sender_guid(sender_guid.server_64);
			msg.set_send_result(send_result);
			msg.set_chat_type(chat_type);

		}
		void from_proto(const faith::ws2cs_proto::send_chat_end& msg)
		{
			sender_guid.server_64 = msg.sender_guid();
			send_result = msg.send_result();
			chat_type = msg.chat_type();
		}
	};
	struct cs2ws_send_chat_end : public faith::packet_base
	{
		guid_64 sender_guid;
		int32 send_result;
		cs2ws_send_chat_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_chat_end;
		}
	};

	struct ws2cs_receive_chat : public faith::packet_base
	{
		guid_64		sender_guid;
		guid_64		addressee_guid;
		int32			chat_typ;
		xchar		sender_name[max_name_size + 1];
		xchar		addressee_name[max_name_size + 1];
		xchar		content_text[chat_max_chat_size + 1];
		int32			item_num;
		s_item_info item_list[chat_max_item];
		ws2cs_receive_chat()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_receive_chat;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&item_list - (ULONG_PTR)&wheader;
			return (basic_len + item_num * sizeof(s_item_info));
		}
	};

	struct ws2cs_receive_chat_new : public faith::packet_base
	{
		guid_64					sender_guid;
		int32					sender_template_id;
		guid_64					addressee_guid;
		int32					chat_typ;
		xchar					sender_name[max_name_size + 1];
		xchar					addressee_name[max_name_size + 1];
		int32					content_info_num;
		e_chat_content_info		content_info[chat_max_word_num];
		ws2cs_receive_chat_new()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_receive_chat_new;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&content_info - (ULONG_PTR)&wheader;
			return (basic_len + content_info_num * sizeof(e_chat_content_info));
		}
	};

	struct cs2ws_send_quick_chat_to_ws : public faith::packet_base
	{
		guid_64					sender_guid;
		guid_64					addressee_guid;
		int32					chat_typ;
		xchar					sender_name[max_name_size + 1];
		int32					quick_chat_template;

		cs2ws_send_quick_chat_to_ws()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_quick_chat_to_ws;
		}
		
	};

	struct ws2cs_receive_quick_chat : public faith::packet_base
	{
		guid_64					sender_guid;
		guid_64					addressee_guid;
		int32					chat_typ;
		xchar					sender_name[max_name_size + 1];
		xchar					addressee_name[max_name_size + 1];
		int32					quick_chat_template;

		ws2cs_receive_quick_chat()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_receive_quick_chat;
		}
		
	};

	struct cs2ws_send_notice_with_param : public faith::packet_base
	{
		guid_64					sender_guid;
		int32					chat_typ;
		int32					param_type;
		int32					template_id;
		xchar					sender_name[max_name_size + 1];
		xchar					content_text[chat_max_chat_size + 1];
		int32					notice_id;

		//定义一个最长的类型
		int32					param_array[e_item_info_max];
		int32					param_len;

		cs2ws_send_notice_with_param()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_notice_with_param;
		}
	};

	struct ws2dp_save_chat_with_offline : public faith::packet_base
	{
		int64					chat_guid;
		guid_64					address_guid;
		guid_64					sender_guid;
		xchar					sender_name[max_name_size + 1];
		int32					chat_type;
		xchar					content_text[chat_max_chat_size + 1];
		int32					sender_templete_id;
		int32					relation_template_id;
		int32					relation_level;
		int32					vip_title_template_id;
		int32					head_frame_id;
		ws2dp_save_chat_with_offline()
		{
			memset(this, 0, sizeof(*this));

			wheader = e_msgindex_ws2dp_save_chat_with_offline;
		}

		void set_content_text(xstring content)
		{
			memset(content_text, 0, chat_max_chat_size + 1);

			memcpy(content_text, content.c_str(), content.size() > chat_max_chat_size ? chat_max_chat_size : content.size());
		}

		void set_sender_name(xstring sender_player_name)
		{
			memset(sender_name, 0, sizeof(sender_name));
			memcpy(sender_name, sender_player_name.c_str(), sender_player_name.size() > sizeof(sender_name) ? sizeof(sender_name) : sender_player_name.size());
		}

	};

	struct ws2dp_load_chat_with_offline : public faith::packet_base
	{
		guid_64				role_guid;

		ws2dp_load_chat_with_offline()
		{
			memset(this, 0, sizeof(ws2dp_load_chat_with_offline));
			wheader = e_msgindex_ws2dp_load_chat_with_offline;
		}
	};

	struct dp2ws_load_chat_with_offline : public faith::packet_base
	{
		guid_64				role_guid;
		int32				data_num;
		e_chat_offline_info chat_info_list[chat_offline_num_max];

		dp2ws_load_chat_with_offline()
		{
			memset(this, 0, sizeof(dp2ws_load_chat_with_offline));
			wheader = e_msgindex_dp2ws_load_chat_with_offline;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&chat_info_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(e_chat_offline_info));
		}
	};

	struct ws2dp_eliminate_chat_with_offline : public packet_base
	{
		guid_64 role_guid;

		ws2dp_eliminate_chat_with_offline()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_eliminate_chat_with_offline;
		}
	};


	struct dp2cs_load_chat_record : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		//bool								is_over;		// 标识玩家物品是否发送完毕, true表示发送成功完毕，false表示没有发送完，errro通过另一条消息传递
		int32								data_num;
		s_chat_record					    data_list[chat_record_max_num];
		dp2cs_load_chat_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_chat_record;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_chat_record));
		}
	};

	struct cs2dp_save_chat_record : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		int32								data_num;
		s_chat_record					    data_list[chat_record_max_num];

		cs2dp_save_chat_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_chat_record;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_chat_record));
		}


	};

	struct ws2ws_load_offline_other_relation : public packet_base
	{
		guid_64					sender_guid;
		guid_64					sender_server_id;
		int32					list_type;
		guid_64					other_guid;
		ws2ws_load_offline_other_relation()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_load_offline_other_relation;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};

	struct  ws2ws_load_offline_other_relation_end : public packet_base
	{
		guid_64					sender_guid;
		s_relation_info			other_relation;
		ws2ws_load_offline_other_relation_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_load_offline_other_relation_end;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};
#pragma pack(pop)
}

#endif
