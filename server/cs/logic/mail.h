/********************************************************************
created: 2016年8月16日
file base: mail
file ext: h
author: lxy

purpose: about mail
*********************************************************************/
#ifndef _MAIL_H_
#define _MAIL_H_

#include <Logic/mail_def.hpp>
#include <Logic/item_def.hpp>
#include "template/ServerTemplateHead.h"

namespace faith
{
	class cmail
	{
	public:
		cmail();
		~cmail();

		void										heart_tick(const int64& new_time);
		void										clear_data();
		void										set_data_use(bool is_use) { m_is_data_use = is_use; }
		bool&										is_valid() { return m_is_data_use; }

		const s_mail_info&							get_mail_inst() const { return m_mail_data; }						//实例	
		guid_64&									get_mail_guid() { return m_mail_data.mail_guid; }
		int32										get_data_info(EMailInfo eIndex);
		void										set_data_info(EMailInfo eIndex, int32 iValue);
		void										set_mail_data(const s_mail_info& mail_data);

		std::string									get_mail_text_contents() { return m_mail_data.text_contents; };
		std::string									get_mail_sender_name() { return m_mail_data.sender_name; };
		std::string									get_mail_title() { return m_mail_data.mail_title; };

		bool										is_has_item_or_money();

		void										set_mail_text_contents(char* text_contents);
		void										set_mail_sender_name(char* sender_name);
		void										set_mail_title(char* mail_title);
		void										set_item_guid_empty();
		void										set_money_empty();
		void										set_target_item_guid_empty(int32 index);
		guid_64										get_target_item_guid(int32 index);
		int32										get_item_num();
	private:
		s_mail_info								m_mail_data;						// 邮件实例数据	
		bool									m_is_data_use;
	};
}
#endif

