/********************************************************************
created: 2016年8月15日
file base: event
file ext: hpp
author: lxy

purpose: about event
*********************************************************************/


#ifndef _EVENT_DEF_HPP_
#define _EVENT_DEF_HPP_


#include "logic/type_def.hpp"
#include "template/template_base.h"
#include "char_def.hpp"
#include "mail_def.hpp"
#include "internet/dp2s.pb.h"

namespace faith
{
#pragma pack(push,1)
	const uint32 auto_save_time = 5;																		// 自动存储间隔分钟数
	const uint32 max_globel_mail_in_same_time = 30;															// 同一时间最大群发邮件数
	const uint32 max_globel_mail_item_in_same_time = max_globel_mail_in_same_time * max_item_per_mail;		// 同一时间最大群发邮件物品数

	//红包
	const uint32 red_package_max_receiver_num = 50;									// 单个红包最大份数
	const uint32 red_package_max_text_length = 60;									// 红包最大文本长度
	const uint32 red_package_show_num = 5;											// 红包抽取数额显示前X人
	const uint32 red_package_total_num = 50;										// 同一时间最大红包数
	const uint32 total_receiver_num = red_package_total_num * red_package_max_receiver_num;			// 总抽奖者数量
	const uint32 red_package_msg_max_lenght = 5694;									// 单个红包消息包最大长度

	struct s_globel_mail_guid_db
	{
		guid_64 role_guid;
		guid_64 data_info[max_globel_mail_in_same_time];
	};

	struct s_red_package_receiver
	{
		guid_64		red_package_guid;
		guid_64		receiver_guid;
		xchar		role_name[max_name_size + 1];
		int32		money_num;

		s_red_package_receiver()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_role_name(const xstring& _role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _role_name.c_str(), _role_name.size() > max_name_size ? max_name_size : _role_name.size());
		}
		void to_proto(faith::dp2s_proto::s_red_package_receiver* red_receiver_ptr)
		{
			if (red_receiver_ptr == nullptr)
			{
				return;
			}
			red_receiver_ptr->set_red_package_guid(red_package_guid.server_64);
			red_receiver_ptr->set_receiver_guid(receiver_guid.server_64);
			red_receiver_ptr->set_role_name(role_name);
			red_receiver_ptr->set_money_num(money_num);
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

		void from_proto(const faith::dp2s_proto::s_red_package_receiver& red_receiver_ref)
		{
			red_package_guid.server_64 = red_receiver_ref.red_package_guid();
			receiver_guid.server_64 = red_receiver_ref.receiver_guid();
			my_memcopy_string(role_name, max_name_size, red_receiver_ref.role_name());
			money_num = red_receiver_ref.money_num();
		}

	};

	struct s_red_package_base
	{
		guid_64					red_package_guid;
		int32					red_package_template_id;
		guid_64					sender_guid;
		int32					create_time;
		xchar					sender_name[max_name_size + 1];
		xchar					text[red_package_max_text_length + 1];
		int32					sender_vip_level;
		int32					remain_times;
		int32					remain_money_num;

		s_red_package_base()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		void set_sender_name(const xstring& role_name)
		{
			memset(sender_name, 0, sizeof(role_name));
			memcpy(sender_name, role_name.c_str(), role_name.size() > max_name_size ? max_name_size : role_name.size());
		}
		void to_proto(faith::dp2s_proto::s_red_package_base* red_base_ptr)
		{
			if (red_base_ptr == nullptr)
			{
				return;
			}
			red_base_ptr->set_red_package_guid(red_package_guid.server_64);
			red_base_ptr->set_red_package_template_id(red_package_template_id);
			red_base_ptr->set_sender_guid(sender_guid.server_64);
			red_base_ptr->set_create_time(create_time);
			red_base_ptr->set_sender_name(sender_name);
			red_base_ptr->set_text(text);
			red_base_ptr->set_sender_vip_level(sender_vip_level);
			red_base_ptr->set_remain_times(remain_times);
			red_base_ptr->set_remain_money_num(remain_money_num);
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
		void from_proto(const faith::dp2s_proto::s_red_package_base& red_base_ref)
		{
			red_package_guid.server_64 = red_base_ref.red_package_guid();
			red_package_template_id = red_base_ref.red_package_template_id();
			sender_guid.server_64 = red_base_ref.sender_guid();
			create_time = red_base_ref.create_time();
			my_memcopy_string(sender_name, max_name_size, red_base_ref.sender_name());
			my_memcopy_string(text, red_package_max_text_length, red_base_ref.text());
			sender_vip_level = red_base_ref.sender_vip_level();
			remain_times = red_base_ref.remain_times();
			remain_money_num = red_base_ref.remain_money_num();
		}
	};

	struct s_red_package_receiver_db
	{
		guid_64					red_package_guid;
		s_red_package_receiver	info;

		s_red_package_receiver_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_red_package
	{
		s_red_package_base		base_info;
		s_red_package_receiver	receiver_info_list[red_package_max_receiver_num];

		int32 get_empty_receiver_slot() const
		{
			for (int32 i = 0; i < red_package_max_receiver_num; ++i)
			{
				if (!receiver_info_list[i].receiver_guid.is_valid())
				{
					return i;
				}
			}
			return -1;
		}

		int32 get_receiver_num() const
		{//有效数据必定是连续的
			int32 receiver_num = 0;
			for (int32 i = 0; i < red_package_max_receiver_num; ++i)
			{
				if (receiver_info_list[i].receiver_guid.is_valid())
				{
					receiver_num++;
				}
			}
			return receiver_num;
		}

		s_red_package()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

#pragma pack(pop)
}


#endif

