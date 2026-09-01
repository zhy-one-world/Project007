/********************************************************************
created: 2016/05/10
created: 10:5:2016 15:28
file base: char_def
file ext: hpp
author: W.H.B.A.(王黄博昂)

purpose:
*********************************************************************/
#ifndef _CHAT_DEF_HPP_
#define _CHAT_DEF_HPP_

#include <cassert>
#include <string>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "item_def.hpp"
#include "template/template_base.h"

namespace faith
{
#pragma pack(push,1)
	const uint32 globel_message_max_size = 200;											// 全局公告最大字节数 100X2
	const uint32 globel_message_max_show_time = 20;										// 一条全局提示最大显示时间，防止出问题导致再也不显示其他Notice
	const uint32 continued_notice_max_num = 100;										// 最大持续公告数量，讲道理，永远不应该有这么多……
	const uint32 chat_max_item = 4;														// 一条聊天带的最大物品数
	const uint32 chat_show_max = 30;													// 最大显示聊天条数
	const uint32 chat_max_word_num = 100;												// 聊天文本最大字数
	const uint32 utf8_byt_per_chinese_world = 3;										// utf8中文编码每个字字节数
	const uint32 chat_max_chat_size = chat_max_word_num * utf8_byt_per_chinese_world;	// 聊天文本最大尺寸
	const uint32 chat_max_chat_size_with_hyper_link = chat_max_chat_size + 200;			// 聊天文本最大尺寸（含超链接）
	const uint32 chat_content_info_max = 4;												// 单个字符最大长度
	const uint32 chat_mini_show_num = 5;												// 全频道聊天数量和
	const uint32 chat_audio_id_max = 300;												// 语音id最大长度
	const uint32 chat_auto_reply_interval = 30;											// 自动回复CD
	const uint32 chat_offline_num_max = 999;											// 离线消息最多接收999条
	const uint32 add_friend_success_content_id = 90305232;								// 添加好友成功申请方收到的内容id
	
	const uint32 chat_record_max_len = 256;
	const uint32 chat_real_max_length = 135;
	const uint32 chat_record_max_num = 10;
	const int32  chat_error_max_len = -1;
	const int32  chat_error_invalid_word = -2;
	const int32  chat_error_sucess = 0;

	enum ENoticeShowTyp
	{
		e_notice_show_type_none,
		e_notice_show_type_head,
		e_notice_show_type_under,
		e_notice_show_type_system_chat,
		e_notice_show_type_head_and_system_chat,
		e_notice_show_type_toptip,
		e_notice_show_type_max,
	};
	struct s_chat_notice_info
	{
		std::vector<xstring> other_name;
		std::vector<xstring> describe_str;
		std::vector<xstring> item_name;
		std::vector<int32> num_type_data;
		std::vector<s_item_info> item_data;
		void clear_data()
		{
			other_name.clear();
			describe_str.clear();
			item_name.clear();
			num_type_data.clear();
			item_data.clear();
		}
	};

	struct e_chat_content_info
	{
		xchar				common_text[chat_content_info_max + 1];
		int32				item_info[e_item_info_max];
		float				location[3];				//反正就x、y、z三个数……
		int32				line_id;
		int32				map_id;
		int32				template_id;
		xchar				audio_id[chat_audio_id_max + 1]; // 临时写法，需要优化		
		float				duration_time;
		int32				face_finish_img_id;
		e_chat_content_info()
		{
			reset();
		}

		void reset()
		{
			memset(this, 0, sizeof(*this));
		}

		bool is_item() const
		{
			return item_info[e_item_info_info_id] != 0;
		}

		const bool is_loction() const
		{
			return location[0] > 0 || location[1] > 0 || location[2] > 0;
		}

		const bool is_only_text() const
		{
			return (!is_item()) && (!is_loction());
		}
	}; 

	//离线消息
	struct e_chat_offline_info
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

		e_chat_offline_info()
		{
			memset(this, 0, sizeof(*this));
		}

		void set_content_text(xstring content)
		{
			memset(content_text, 0, chat_max_chat_size + 1);

			memcpy(content_text, content.c_str(), content.size() > chat_max_chat_size ? chat_max_chat_size : content.size());
		}
		void set_sender_name(xstring sender_player_name)
		{
			memset(sender_name, 0, max_name_size + 1);
			memcpy(sender_name, sender_player_name.c_str(), sender_player_name.size() > sizeof(sender_name) ? sizeof(sender_name) : sender_player_name.size());
		}
	};
///////////////////////////////
	struct s_chat_record
	{
		int32  pos;
		xchar  record[chat_record_max_len + 1];
		s_chat_record()
		{
			memset(this, 0, sizeof(*this));
		}
			

	};
	struct s_chat_record_db
	{
		guid_64 role_guid;
		s_chat_record chat_record;
		s_chat_record_db()
		{
			memset(this, 0, sizeof(*this));
		}
		
	};


#pragma pack(pop)
}

#endif
