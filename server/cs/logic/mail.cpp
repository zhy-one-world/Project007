/********************************************************************
created:2016Äê8ÔÂ16ÈÕ
file base: mail
file ext: cpp
author: lxy

purpose: about mail
*********************************************************************/

#include "mail.h"
#include "utility/guid_gen.h"
#include "template/template_manager.h"

namespace hld
{

	cmail::cmail(void)
	{
		clear_data();
	}

	cmail::~cmail(void)
	{
	}

	void cmail::heart_tick(const int64& new_time)
	{
	}

	void cmail::clear_data()
	{
		m_mail_data.clear_data();
		m_is_data_use = false;
	}

	void cmail::set_mail_data(const s_mail_info& mail_data)
	{
		memcpy(&m_mail_data, &mail_data, sizeof(mail_data));
	}

	int32 cmail::get_data_info(EMailInfo eIndex)
	{
		if (eIndex < EMailInfo_max)
		{
			return m_mail_data.data_ary[eIndex];

		}
		return -999;
	}

	void cmail::set_data_info(EMailInfo eIndex, int32 iValue)
	{
		if (eIndex < EMailInfo_max)
		{
			m_mail_data.data_ary[eIndex] = iValue;
		}
	}


	void cmail::set_mail_text_contents(char* text_contents)
	{
		int32 real_size = strlen(text_contents);
		if (max_text_contents_length < real_size)
		{
			real_size = max_text_contents_length;
		}
		memcpy(m_mail_data.text_contents, text_contents, real_size);
	}

	void cmail::set_mail_sender_name(char* sender_name)
	{
		int32 real_size = strlen(sender_name);
		if (max_name_size < real_size)
		{
			real_size = max_text_contents_length;
		}
		memcpy(m_mail_data.text_contents, sender_name, real_size);
	}

	void cmail::set_mail_title(char* mail_title)
	{
		int32 real_size = strlen(mail_title);
		if (max_mail_title_length < real_size)
		{
			real_size = max_text_contents_length;
		}
		memcpy(m_mail_data.text_contents, mail_title, real_size);
	}


	void  cmail::set_item_guid_empty()
	{
		for (int32 i = EMailInfo_ItemGuid11 ; i <= EMailInfo_ItemGuid52 ; ++i)
		{
			m_mail_data.data_ary[i] = 0;
		}
	}

	void cmail::set_money_empty()
	{
		for (int32 i = EMailInfo_MoneyTyp1; i <= EMailInfo_MoneyNum2; ++i)
		{
			m_mail_data.data_ary[i] = 0;
		}
	}

	void cmail::set_target_item_guid_empty(int32 index)
	{
		if (index < 0 || index >= max_item_per_mail)
		{
			return;
		}

		m_mail_data.data_ary[index*2 + EMailInfo_ItemGuid11] = 0;
		m_mail_data.data_ary[index*2 + EMailInfo_ItemGuid12] = 0;
	}
	guid_64 cmail::get_target_item_guid(int32 index)
	{
		if (index < 0 || index >= max_item_per_mail)
		{
			return guid_64(0,0);
		}
		guid_64 temp_guid = guid_64(m_mail_data.data_ary[index*2 + EMailInfo_ItemGuid11], m_mail_data.data_ary[index*2 + EMailInfo_ItemGuid12]);
		return temp_guid;
	}

	int32 cmail::get_item_num()
	{
		guid_64 temp_guid;
		int32 item_count = 0;
		for (int32 i = EMailInfo_ItemGuid11;i <= EMailInfo_ItemGuid51 ; i+=2)
		{
			temp_guid = guid_64(m_mail_data.data_ary[i], m_mail_data.data_ary[i+1]);
			if (temp_guid.is_valid())
			{
				item_count++;
			}
		}
		return item_count;
	}

	bool cmail::is_has_item_or_money()
	{
		return m_mail_data.is_has_item_or_money();
	}
}