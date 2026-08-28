/********************************************************************
created: 2016年8月16日
file base: mail_mgr
file ext: cpp
author: luoxingyu

purpose: about role's mail_mgr
*********************************************************************/

#include "connection_mgr.hpp"
#include "utility/init_unit.h"
#include "internal/event_msg.hpp"
#include "internet/item.pb.h"
#include "internet/mail.pb.h"
#include "internet/net.pb.h"
#include "item.h"
#include "item_def.hpp"
#include "logic/player.hpp"
#include "mail_mgr.h"
#include "server_log.hpp"
#include "server_log_def.hpp"
#include "system/item/item_system.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "utility/cs_date.hpp"
#include "utility/parse_msg.h"
#include "internal/core.hpp"
#include "internal/mail_msg.hpp"

namespace hld
{
	cmail_mgr::cmail_mgr()
	{
		m_player_ptr = nullptr;
		clear_data();
	}

	cmail_mgr::~cmail_mgr(void)
	{

	}

	void cmail_mgr::clear_data()
	{
		memset(m_globel_mail_sign, 0, sizeof(m_globel_mail_sign));
		for (int32 i = 0; i < MAX_MAIL_NUM; ++i)
		{
			m_mail_list[i].clear_data();
		}
		m_mail_item_map.clear();
		//for (int32 i = 0; i < max_mail_item_num; ++i)
		//{
		//	m_mail_item_list[i].clear_data();
		//}

		m_auto_load_timer = -1;
		m_mail_list_num = 0;
		//m_globel_mail_next_check_time = 0;
		m_mail_load_lock = 0;
		clear_del_array();
	}

	void cmail_mgr::clear_date_by_index(int32 target_index)
	{
		if (target_index < 0
			|| target_index >= MAX_MAIL_NUM)
		{
			return;
		}
		m_mail_list[target_index].clear_data();
		if (target_index == m_mail_list_num - 1)
		{
			m_mail_list_num = target_index;
		}
	}

	void cmail_mgr::send_mail_one(cmail& mail_ptr)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}

		mail_proto_update_mail mail_msg;

		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().B);
		mail_msg.set_is_over(true);
		mail_msg.set_is_need_clear(false);
		get_mail_msg(mail_msg, mail_ptr);

		m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_update);

	}

	void cmail_mgr::get_mail_msg(mail_proto_update_mail& mail_msg, cmail& mail_ptr)
	{
		mail_proto_mail_info* mail_info_ptr = mail_msg.add_mail_list();
		mail_info_ptr->add_mail_guid(mail_ptr.get_mail_guid().A);
		mail_info_ptr->add_mail_guid(mail_ptr.get_mail_guid().B);

		cmail temp_mail = mail_ptr;
		int32 tem_info = 0;
		for (int32 i = 0; i < EMailInfo_max; ++i)
		{
			tem_info = temp_mail.get_data_info((EMailInfo)i);
			mail_info_ptr->add_mail_data(tem_info);
		}
		mail_info_ptr->set_text_contents(temp_mail.get_mail_text_contents());
		mail_info_ptr->set_mail_title(temp_mail.get_mail_title());
		mail_info_ptr->set_sender_name(temp_mail.get_mail_sender_name());
	}

	void cmail_mgr::get_mail_item_msg(item_proto_update_character_item& item_msg, const s_item_info& item_info)
	{
		item_proto_item_info* mail_item_info_ptr = item_msg.add_item_list();
		mail_item_info_ptr->set_item_guid(item_info.item_guid.server_64);

		//cmail temp_mail = mail_ptr;
		int32 tem_info = 0;
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			tem_info = item_info.data_ary[i];
			mail_item_info_ptr->add_item_data(tem_info);
		}
	}

	void cmail_mgr::send_mail_all()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}

		bool is_need_clear = true;
		//全部邮件更新时，需要将客户端的数据先清除

		mail_proto_update_mail mail_msg;
		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().B);
		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			int32 temp_size = mail_msg.ByteSize();

			//包大小大于4800就不能发了，所以判断大于4000就先发一波
			if (temp_size > packege_length_max)
			{
				mail_msg.set_is_over(false);
				mail_msg.set_is_need_clear(is_need_clear);
				m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_update);
				mail_msg.clear_mail_list();
				is_need_clear = false;
			}
			if (m_mail_list[i].is_valid())
			{
				get_mail_msg(mail_msg, m_mail_list[i]);
			}
		}

		mail_msg.set_is_over(true);
		mail_msg.set_is_need_clear(is_need_clear);
		m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_update);
	}
	void cmail_mgr::send_mail_item_all()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}

		item_proto_update_character_item item_msg;
		item_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		item_msg.add_role_guid(m_player_ptr->get_unit_guid().B);

		for (unit_mail_item_map_it it = m_mail_item_map.begin();it != m_mail_item_map.end(); it ++)
		{
			//包大小大于4800就不能发了，所以判断大于4000就先发一波
			if (item_msg.item_list_size() > send_item_max_num)
			{
				m_player_ptr->send_message_to_self(&item_msg, e_msgindex_s2c_mail_item_update);
				item_msg.clear_item_list();
			}
			get_mail_item_msg(item_msg, it->second);
		}
		m_player_ptr->send_message_to_self(&item_msg, e_msgindex_s2c_mail_item_update);
	}

	void cmail_mgr::heart_tick(const int64& new_time)
	{
		int32 cur_time = time_helper::get_cur_time_new().second;
		int32 cur_living_time = 0;
		bool is_need_delete = false;
		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			if (!m_mail_list[i].is_valid())
			{
				continue;
			}
			int32 temp_send_time = m_mail_list[i].get_data_info(EMailInfo_SendTime);
			cur_living_time = cur_time - temp_send_time;
			if (m_mail_list[i].get_data_info(EMailInfo_IsNeedDelete) > 1)
			{
				if (cur_time > m_mail_list[i].get_data_info(EMailInfo_IsNeedDelete))
				{
					is_need_delete = true;
				}
			}
			if ((cur_living_time > GAMECONFIG->MailLiveTime || is_need_delete)
				&& !m_mail_list[i].is_has_item_or_money())
			{
				//邮件到时，将删除标志位置1
				m_mail_list[i].set_data_info(EMailInfo_IsNeedDelete, 1);

				add_to_del_array(m_mail_list[i].get_mail_guid());

				//删除附件物品
				for (int32 item_index = 0; item_index < max_item_per_mail; ++item_index)
				{
					guid_64 temp_item_guid = m_mail_list[i].get_target_item_guid(item_index);
					m_mail_item_map.erase(temp_item_guid);

					//int32 temp_item_index = get_item_info_index(temp_item_guid);
					//if (temp_item_index >= 0 && temp_item_index < max_mail_item_num)
					//{
					//	m_mail_item_list[temp_item_index].clear_data();
					//}
				}
				clear_date_by_index(i);
			}
		}

		if (del_array_effect_num > 0)
		{
			mail_proto_mail_del_by_live_time_info mail_msg;

			for (int32 i = 0; i < del_array_effect_num; ++i)
			{
				mail_msg.add_mail_guid(m_del_mail_array[i].A);
				mail_msg.add_mail_guid(m_del_mail_array[i].B);
			}
			mail_msg.set_mail_num(del_array_effect_num);
			m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_delete_mail_by_live_time);

			//删除邮件本身
			del_by_del_array();
		}

		//if (new_time > m_globel_mail_next_check_time)
		//{
		//	check_mail_event_state();
		//}

		if (m_auto_load_timer >= 0
			&& new_time > m_auto_load_timer)
		{
			auto_load_mail();
		}
	}

	void cmail_mgr::load_had_globel_mail_guid_end(const guid_64* guid_array)
	{
		if (guid_array == nullptr)
		{
			return;
		}
		memset(m_globel_mail_sign, 0, sizeof(m_globel_mail_sign));
		int32 effect_data_index = 0;
		for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
		{
			if (guid_array[i].is_valid())
			{
				m_globel_mail_sign[effect_data_index] = guid_array[i];
				effect_data_index++;
			}
		}
	}
	bool cmail_mgr::load_had_globel_mail_guid_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::db_proto::role_event_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		guid_64 guid_ary[max_globel_mail_in_same_time];

		for (int32 i = 0; i < msg.data_ary_size(); i++)
		{
			guid_ary[i].server_64 = msg.data_ary(i);
		}
	
		load_had_globel_mail_guid_end(guid_ary);

		return true;



	}

	void cmail_mgr::add_event_guid(guid_64 temp_guid)
	{

	}

	void cmail_mgr::check_mail_event_state()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (!m_player_ptr->is_self_server())
		{
			return;
		}
		
		cs2ws_check_player_globel_mail_event msg;
		msg.role_guid = m_player_ptr->get_unit_guid();
		msg.role_creat_time = m_player_ptr->get_unit_i64_info_data(e_role_i64_info_create_time);
		int32 effect_index = 0;
		for (int32 i = 0; i < max_globel_mail_in_same_time; ++i)
		{
			if (m_globel_mail_sign[i].is_valid())
			{
				msg.data_info[effect_index] = m_globel_mail_sign[i];
				effect_index++;
			}
		}
		msg.data_num = effect_index;
		connection_mgr::getInstance().send_to_ws(&msg, msg.get_pak_length());

		//m_globel_mail_next_check_time = init_unit::get_end_time(globel_mail_check_time);

	}
	void cmail_mgr::load_mail_by_db(const s_mail_info* mail_data, int32 mail_num)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}

		if (nullptr == mail_data)
		{
			return;
		}

		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			clear_date_by_index(i);
		}

		s_mail_info temp;
		for (int32 i = 0; i < mail_num; i++)
		{
			if (i >= MAX_MAIL_NUM)
			{
				break;
			}

			int32 temp_mail_send_time = mail_data[i].data_ary[EMailInfo_SendTime];
			if (time_helper::get_cur_time_new().second - temp_mail_send_time > GAMECONFIG->MailLiveTime
				&& !mail_data[i].is_has_item_or_money())
			{
				add_to_del_array(mail_data[i].mail_guid);
			}
			else
			{
				temp = mail_data[i];
				create_mail_by_info(temp);
			}
		}

		if (del_array_effect_num > 0)
		{
			mail_proto_mail_del_by_live_time_info mail_msg;
			mail_msg.add_mail_guid(0);
			mail_msg.add_mail_guid(0);
			mail_msg.set_mail_num(del_array_effect_num);
			m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_delete_mail_by_live_time);

			del_by_del_array();
		}
		operate_load_lock(-1);
	}
	bool  cmail_mgr::load_mail_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::db_proto::role_mail_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_mail_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_mail_info *p_row = (s_mail_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::db_proto::role_mail_row db_row = msg.row_data(i);
			p_row->mail_guid.server_64 = db_row.mail_guid();
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			parse_msg::getInstance().my_memcopy_string(p_row->text_contents, max_text_contents_length, db_row.text_contents());
			parse_msg::getInstance().my_memcopy_string(p_row->sender_name, max_name_size, db_row.sender_name());
			parse_msg::getInstance().my_memcopy_string(p_row->mail_title, max_mail_title_length, db_row.mail_title());
			p_row++;
		}
		load_mail_by_db((const s_mail_info *)p_data, msg.row_count());
		return true;
		
	}

	int32 cmail_mgr::mail_operate(const std::vector<guid_64>& mail_guid, int32 operation_type)
	{
		if (m_player_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		if (mail_guid.empty())
		{
			return e_item_string_unkown;
		}

		int32 operate_result = 0;
		switch (operation_type)
		{
		case EMailOperationTyp_Read:
			operate_result = read_mail(mail_guid);
			break;
		case EMailOperationTyp_GetAndDel:
			operate_result = get_and_del(mail_guid[0]);
			break;
		case EMailOperationTyp_Delete:
			operate_result = del_mail(mail_guid[0]);
			break;
		case EMailOperationTyp_Force_Delete:
			operate_result = force_delete_one(mail_guid[0]);
			break;
		case EMailOperationTyp_AutoGet:
			operate_result = auto_get();
			break;
		case EMailOperationTyp_DelReaded:
			operate_result = del_readed();
			break;
		case EMailOperationTyp_ForceDelAllReaded:
			operate_result = force_delete_all_readed();
		default:
			break;
		}

		mail_proto_mail_operation_end mail_msg;

		mail_msg.set_operation_state(operate_result);
		mail_msg.set_operation_typ(operation_type);

		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().B);

		for (int32 i = 0; i < mail_guid.size(); ++i)
		{
			mail_msg.add_mail_guid(mail_guid[i].A);
			mail_msg.add_mail_guid(mail_guid[i].B);
		}
		m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_operation);

		return 1;
	}


	cmail* cmail_mgr::create_mail_by_info(s_mail_info& temp_info)
	{
		if (nullptr == m_player_ptr)
		{
			return nullptr;
		}

		guid_64 mail_guid = temp_info.mail_guid;
		if (!mail_guid.is_valid())
		{
			mail_guid = guid_gen::make_guid(m_player_ptr->get_unit_guid());
		}
		int32 empty_index = -1;
		for (int32 i = 0; i < MAX_MAIL_NUM; ++i)
		{
			if (m_mail_list[i].is_valid() == false)
			{
				empty_index = i;
				break;
			}
		}
		if (empty_index == -1)
		{
			return nullptr;
		}
		cmail& mail_ref = m_mail_list[empty_index];
		mail_ref.set_mail_data(temp_info);
		mail_ref.set_data_use(true);
		if (empty_index >= m_mail_list_num)
		{
			m_mail_list_num = empty_index + 1;
		}
		return &mail_ref;
	}

	cmail* cmail_mgr::get_mail(const guid_64& mail_guid)
	{
		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			if (m_mail_list[i].get_mail_guid() == mail_guid)
			{
				return &m_mail_list[i];
			}
		}
		return nullptr;
	}

	int32 cmail_mgr::get_mail_index(const guid_64& mail_guid)
	{
		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			if (m_mail_list[i].get_mail_guid() == mail_guid)
			{
				return i;
			}
		}
		return -1;
	}

	int32 cmail_mgr::read_mail(std::vector<guid_64>  mail_guid)
	{
		guid_64 temp_guid;
		temp_guid.clear_data();
		for (int32 i = 0 ; i < mail_guid.size() ; ++i)
		{
			temp_guid = mail_guid[i];
			if (temp_guid.is_valid())
			{
				cmail* temp_mail = get_mail(temp_guid);
				if (temp_mail != nullptr)
				{
					temp_mail->set_data_info(EMailInfo_ReadState, 1);

					//防止刷新，不主动发，反正就是1、0变换，在OperateEnd时设置为1
					//send_mail_one(*temp_mail);
					add_mail_to_db(temp_mail->get_mail_inst());
					return e_item_string_succeed;
				}
				else
				{
					return e_mail_not_exist;
				}
			}
		}
		return e_item_string_unkown;
	}

	int32 cmail_mgr::auto_get()
	{
		int32 result = e_mail_get_success;
		int mail_list_empty_num = 0;
		if (m_player_ptr == nullptr)
		{
			return e_item_string_unkown;
		}

		if (m_mail_load_lock > 0)
		{
			return e_operate_too_fast;
		}

		std::vector<int32> all_money_get_array;
		std::vector<int32> all_item_get_array;

		std::vector<cmail> send_mail_vector;
		
		for (int32 i = 0; i <m_mail_list_num; ++i)
		{
			if (m_mail_list[i].is_valid())
			{
				int32 item_num = m_mail_list[i].get_item_num();

				guid_64 mail_guid = m_mail_list[i].get_mail_guid();
				cmail* temp_mail = get_mail(mail_guid);
				if (temp_mail == nullptr)
				{
					continue;
				}

				bool has_item_add_bag_failed = false;
				if (temp_mail->is_has_item_or_money())
				{
					result = get_one_content(m_mail_list[i].get_mail_guid(), all_money_get_array, all_item_get_array, has_item_add_bag_failed);
				}
				else
				{
					mail_list_empty_num++;
				}
				if (mail_list_empty_num == m_mail_list_num)
				{
					return e_mail_no_item_or_money;
				}
			
				if (result == e_mail_get_success)
				{
					m_mail_list[i].set_data_info(EMailInfo_ReadState, 1);
					send_mail_vector.push_back(m_mail_list[i]);
					add_mail_to_db(m_mail_list[i].get_mail_inst());
				}
				if (has_item_add_bag_failed)
				{

					break;
				}
			}
		}
		send_mail_item_all();
		send_mail_all();

		//增加了物品，或者 加了钱且钱的ID和数量对应
		if ((all_item_get_array.size() > 0) || (all_money_get_array.size() > 0))
		{
			mail_proto_mail_contents_get mail_get_msg;
			for (int32 i = 0; i < all_item_get_array.size(); ++i)
			{
				mail_get_msg.add_item_get_info(all_item_get_array[i]);
			}

			for (int32 i = 0; i < all_money_get_array.size(); ++i)
			{
				mail_get_msg.add_money_get_info(all_money_get_array[i]);
			}

			m_player_ptr->send_message_to_self(&mail_get_msg, e_msgindex_s2c_mail_contents_get);
		}
		else if (result == e_mail_get_success)
		{
			result = e_mail_no_item_or_money;
		}
		return result;
	}
	int32 cmail_mgr::del_readed()
	{
		int32 result = e_mail_delete_sucess;
		if (m_player_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		int32 readed_count = 0;
		std::vector<cmail> send_mail_vector;
		send_mail_vector.reserve(m_mail_list_num);

		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			if (m_mail_list[i].is_valid())
			{
				if (m_mail_list[i].get_data_info(EMailInfo_ReadState) > 0)
				{
					if (m_mail_list[i].is_has_item_or_money())
					{
						result = e_mail_force_delete_confirm;
					}
					else
					{
						m_mail_list[i].set_data_info(EMailInfo_IsNeedDelete, 1);
						send_mail_vector.push_back(m_mail_list[i]);
						add_to_del_array(m_mail_list[i].get_mail_guid());
						clear_date_by_index(i);
					}
					readed_count++;
				}
			}
		}
		if (readed_count == 0)
		{
			return e_mail_has_no_readed_mail;
		}
		send_mail_batch(send_mail_vector);
		del_by_del_array();
		return result;
	}
	int32 cmail_mgr::get_and_del(guid_64 mail_guid)
	{
		int32 mail_index = get_mail_index(mail_guid);
		if (mail_index < 0
			|| mail_index>= m_mail_list_num)
		{
			return e_item_string_unkown;
		}
		cmail* temp_mail = &m_mail_list[mail_index];
		std::vector<int32> all_money_get_array;
		std::vector<int32> all_item_get_array;
		if (temp_mail == nullptr
			|| m_player_ptr ==nullptr)
		{
			return e_item_string_unkown;
		}
		bool has_item_add_bag_failed = false;
		const int32 result = get_one_content(temp_mail->get_mail_guid(), all_money_get_array, all_item_get_array, has_item_add_bag_failed);
		send_mail_one(*temp_mail);
		if (e_mail_get_success != result)
		{
			return result;
		}
		//temp_mail->set_data_info(EMailInfo_IsNeedDelete, 1);
		//delete_mail_in_db(temp_mail->get_mail_guid());
		//send_mail_one(*temp_mail);
		//clear_date_by_index(mail_index);

		if ((all_item_get_array.size() > 0) || (all_money_get_array.size() > 0))
		{
			mail_proto_mail_contents_get mail_get_msg;
			for (int32 i = 0; i < all_item_get_array.size(); ++i)
			{
				mail_get_msg.add_item_get_info(all_item_get_array[i]);
			}

			for (int32 i = 0; i < all_money_get_array.size(); ++i)
			{
				mail_get_msg.add_money_get_info(all_money_get_array[i]);
			}
			m_player_ptr->send_message_to_self(&mail_get_msg, e_msgindex_s2c_mail_contents_get);
		}
		del_mail(mail_guid);
		return e_mail_get_success;
	}
	int32 cmail_mgr::del_mail(guid_64 mail_guid)
	{
		int32 mail_index = get_mail_index(mail_guid);
		if (mail_index < 0
			|| mail_index >= m_mail_list_num)
		{
			return e_item_string_unkown;
		}
		if (m_mail_list[mail_index].is_has_item_or_money())
		{
			//有附件的讲道理是不能进入这个函数的
			return e_item_string_unkown;
		}
		m_mail_list[mail_index].set_data_info(EMailInfo_IsNeedDelete, 1);
		send_mail_one(m_mail_list[mail_index]);
		delete_mail_in_db(m_mail_list[mail_index].get_mail_guid());
		clear_date_by_index(mail_index);
		return e_mail_delete_sucess;
	}
	int32 cmail_mgr::get_one_content(guid_64 mail_guid,
		std::vector<int32>& all_money_get_array,
		std::vector<int32>& all_item_get_array, bool& has_item_add_bag_failed)
	{
		if (m_player_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		if (m_mail_load_lock > 0)
		{
			return e_operate_too_fast;
		}
		cmail* temp_mail = get_mail(mail_guid);
		int32 item_num = 0;
		if (temp_mail == nullptr)
		{
			return e_item_string_unkown;
		}
		if (!temp_mail->is_has_item_or_money())
		{
			return e_item_string_unkown;
		}
		item_num = temp_mail->get_item_num();

		bool all_success = true;
		for (int32 item_index = 0; item_index < max_item_per_mail; ++item_index)
		{
			guid_64 temp_item_guid = temp_mail->get_target_item_guid(item_index);
			if (!temp_item_guid.is_valid())
			{
				continue;
			}
			unit_mail_item_map_it temp_item_it = m_mail_item_map.find(temp_item_guid);
			if (temp_item_it == m_mail_item_map.end())
			{
				continue;
			}

			s_item_info temp_item_info = temp_item_it->second;
			const int32 item_template_id = temp_item_info.data_ary[e_item_info_info_id];
			ItemTemplate* item_template_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(item_template_id);
			if (nullptr == item_template_ptr)
			{
				return e_item_string_unkown;
			}
			temp_item_info.data_ary[e_item_info_container_type] = 0;
			temp_item_info.data_ary[e_item_info_slot] = 0;
			temp_item_info.item_guid = guid_gen::make_guid(m_player_ptr->get_unit_guid());
			if (temp_item_info.data_ary[e_item_info_prototype_id] > 0)
			{
				ItemTemplate* prototype_item_template_ptr = template_manager::get_instance().get_item_template_ptr_by_template_id(temp_item_info.data_ary[e_item_info_prototype_id]);
				if (nullptr != prototype_item_template_ptr)
				{
					temp_item_info.data_ary[e_item_info_over_time] = time_helper::get_cur_time_new().second + prototype_item_template_ptr->EffectiveTime;
				}
			}
			//citem* temp_item = m_player_ptr->get_item_set().create_item_by_info(temp_item_info, e_server_log_add_item_mail, 0, true);
			//if (temp_item == nullptr)
			//{
			//	return e_mail_bag_full_cant_get;
			//}
			//int32 temp_item_id = temp_item->get_data_info(e_item_info_info_id);
			//int32 temp_item_num = temp_item->get_data_info(e_item_info_stack_count);
			//if (all_item_get_array.size() % 2 == 0)
			//{
			//	int32 item_info_num = all_item_get_array.size() / 2;
			//	bool is_has_this_item = false;
			//	for (int32 i = 0; i < item_info_num; ++i)
			//	{
			//		int32 id_index = i * 2;
			//		if (all_item_get_array[i] == temp_item_id)
			//		{
			//			all_item_get_array[i + 1] += temp_item_num;
			//			is_has_this_item = true;
			//			break;
			//		}
			//	}
			//	if (!is_has_this_item)
			//	{
			//		all_item_get_array.push_back(temp_item_id);
			//		all_item_get_array.push_back(temp_item_num);
			//	}
			//}
			//if(m_player_ptr->get_item_set().put_in_bag(temp_item) == false)
			//		all_success = false;

			////成功领取物品，清空该存储位
			//temp_mail->set_target_item_guid_empty(item_index);
			//delete_mail_item_in_db(temp_item_guid);
			//m_mail_item_map.erase(temp_item_it);
			//m_mail_item_list[temp_item_index].clear_data();
			//server_log::mail_get_content_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), temp_mail->get_mail_guid(), temp_item_id, temp_item_num, temp_mail->get_mail_title(), temp_mail->get_mail_text_contents(), m_player_ptr->get_login_type());

			//set_log_var(log_head);

			//m_player_ptr->get_log_common_head_info(log_head);

			//server_log::serverGetMailContent(log_head, temp_item_id, temp_item_num, temp_item_guid.server_64, temp_mail->get_mail_title());
		}

		int32 temp_money_get_typ1 = temp_mail->get_data_info(EMailInfo_MoneyTyp1);
		int64 temp_money_get_num1 = temp_mail->get_data_info(EMailInfo_MoneyNum1);
		int64 log_money_get_num1 = temp_money_get_num1;
		int32 temp_money_get_typ2 = temp_mail->get_data_info(EMailInfo_MoneyTyp2);
		int64 temp_money_get_num2 = temp_mail->get_data_info(EMailInfo_MoneyNum2);
		int64 log_money_get_num2 = temp_money_get_num2;

		//计算本次提取，货币增加量
		//第一种货币
		if(temp_money_get_num1 > 0)
		{
			bool add_money_1_success = false;
			if (temp_money_get_typ1 == e_money_type_exp)
			{
				temp_money_get_num1 = m_player_ptr->add_exp(temp_money_get_num1);
				add_money_1_success = true;
			}
			else
			{
				add_money_1_success = m_player_ptr->add_money_or_exp(e_money_type(temp_money_get_typ1), temp_money_get_num1, e_server_log_add_money_get_mail);
			}
			if (add_money_1_success)
			{
				server_log::mail_get_content_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), temp_mail->get_mail_guid(), temp_money_get_typ1, log_money_get_num1, temp_mail->get_mail_title(), temp_mail->get_mail_text_contents(), m_player_ptr->get_login_type());
				set_log_var(log_head);
				m_player_ptr->get_log_common_head_info(log_head);
				server_log::serverGetMailContent(log_head, temp_money_get_typ1, log_money_get_num1, temp_mail->get_mail_guid().server_64, temp_mail->get_mail_title());

				if (all_money_get_array.size() % 2 == 0)
				{
					int32 money_num = all_money_get_array.size() / 2;
					bool is_has_this_money = false;
					for (int32 i = 0; i < money_num; ++i)
					{
						int32 id_index = i * 2;
						if (all_money_get_array[i] == temp_money_get_typ1)
						{
							all_money_get_array[i + 1] += temp_money_get_num1;
							is_has_this_money = true;
							break;
						}
					}
					if (!is_has_this_money)
					{
						all_money_get_array.push_back(temp_money_get_typ1);
						all_money_get_array.push_back(temp_money_get_num1);
					}
				}
			}
		}

		//第二种货币
		if(temp_money_get_num2 > 0)
		{
			bool add_money_2_success = false;
			if (temp_money_get_typ1 == e_money_type_exp)
			{
				temp_money_get_num2 = m_player_ptr->add_exp(temp_money_get_num2);
				add_money_2_success = true;
			}
			else
			{
				add_money_2_success = m_player_ptr->add_money_or_exp(e_money_type(temp_money_get_typ2), temp_money_get_num2, e_server_log_add_money_get_mail);
			}

			if (add_money_2_success)
			{
				server_log::mail_get_content_log(m_player_ptr->get_unit_info_inst(), m_player_ptr->get_third_info(), temp_mail->get_mail_guid(), temp_money_get_typ2, log_money_get_num2, temp_mail->get_mail_title(), temp_mail->get_mail_text_contents(), m_player_ptr->get_login_type());

				set_log_var(log_head);
				m_player_ptr->get_log_common_head_info(log_head);
				server_log::serverGetMailContent(log_head, temp_money_get_typ2, log_money_get_num2, temp_mail->get_mail_guid().server_64, temp_mail->get_mail_title());


				if (all_money_get_array.size() % 2 == 0)
				{
					int32 money_num = all_money_get_array.size() / 2;
					bool is_has_this_money = false;
					for (int32 i = 0; i < money_num; ++i)
					{
						int32 id_index = i * 2;
						if (all_money_get_array[i] == temp_money_get_typ2)
						{
							all_money_get_array[i + 1] += temp_money_get_num2;
							is_has_this_money = true;
							break;
						}
					}
					if (!is_has_this_money)
					{
						all_money_get_array.push_back(temp_money_get_typ2);
						all_money_get_array.push_back(temp_money_get_num2);
					}
				}
			}
		}

		temp_mail->set_item_guid_empty();
		temp_mail->set_money_empty();

		temp_mail->set_data_info(EMailInfo_ReadState, 1);
		add_mail_to_db(temp_mail->get_mail_inst());

		if (all_success == false)
			has_item_add_bag_failed = true;
		return e_mail_get_success;
	}
	int32 cmail_mgr::force_delete_one(guid_64 mail_guid)
	{
		if (m_player_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		int32 mail_index = get_mail_index(mail_guid);
		if (mail_index < 0
			|| mail_index >= m_mail_list_num)
		{
			return e_item_string_unkown;
		}

		//邮件有效性判断完毕，开始正常逻辑
		for (int32 item_guid_index = EMailInfo_ItemGuid11; item_guid_index <= EMailInfo_ItemGuid51; item_guid_index += 2)
		{
			//在此循环中处理所有五个物品
			guid_64 temp_item_guid(m_mail_list[mail_index].get_data_info((EMailInfo)item_guid_index), m_mail_list[mail_index].get_data_info((EMailInfo)(item_guid_index + 1)));
			if (!temp_item_guid.is_valid())
			{
				continue;
			}
			m_mail_item_map.erase(temp_item_guid);
			//if (temp_item_guid.is_valid())
			//{
			//	int32 item_index = get_item_info_index(temp_item_guid);
			//	if (item_index >=0 && item_index < max_mail_item_num)
			//	{
			//		m_mail_item_list[item_index].clear_data();
			//	}
			//}
		}
		send_mail_item_all();
		//temp_mail->set_data_info(EMailInfo_IsNeedDelete, 1);
		delete_mail_in_db(mail_guid);
		clear_date_by_index(mail_index);
		return e_mail_delete_sucess;
	}
	int32 cmail_mgr::force_delete_all_readed()
	{
		std::vector<cmail> del_mail_list;
		del_mail_list.reserve(m_mail_list_num);

		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			if (m_mail_list[i].get_data_info(EMailInfo_ReadState) > 0)
			{
				m_mail_list[i].set_data_info(EMailInfo_IsNeedDelete, 1);
				del_mail_list.push_back(m_mail_list[i]);

				for (int32 item_guid_index = EMailInfo_ItemGuid11; item_guid_index <= EMailInfo_ItemGuid51; item_guid_index += 2)
				{
					//在此循环中处理所有五个物品
					guid_64 temp_item_guid(m_mail_list[i].get_data_info((EMailInfo)item_guid_index), m_mail_list[i].get_data_info((EMailInfo)(item_guid_index + 1)));
					if (!temp_item_guid.is_valid())
					{
						continue;
					}
					m_mail_item_map.erase(temp_item_guid);

					//if (temp_item_guid.is_valid())
					//{
					//	int32 item_index = get_item_info_index(temp_item_guid);
					//	if (item_index >= 0 && item_index < max_mail_item_num)
					//	{
					//		delete_mail_item_in_db(temp_item_guid);
					//		m_mail_item_list[item_index].clear_data();
					//	}
					//}
				}
				add_to_del_array(m_mail_list[i].get_mail_guid());
				clear_date_by_index(i);
			}
		}

		send_mail_item_all();
		send_mail_batch(del_mail_list);
		del_by_del_array();
		return e_mail_delete_sucess;
	}

	int32 cmail_mgr::get_contents_and_del(std::vector<guid_64>  mail_guid)
	{
		return e_item_string_unkown;
	}

	int32 cmail_mgr::force_delete_mail(std::vector<guid_64>  mail_guid)
	{
		if (m_player_ptr == nullptr)
		{
			return e_item_string_unkown;
		}
		guid_64 temp_guid;
		temp_guid.clear_data();
		//std::vector<cmail> update_list;
		mail_proto_mail_delete_these_mail msg;
		for (int32 i = 0; i < mail_guid.size(); ++i)
		{
			temp_guid = mail_guid[i];
			if (temp_guid.is_valid())
			{
				int32 mail_slot = get_mail_index(temp_guid);
				if (mail_slot >= 0
					&& mail_slot < m_mail_list_num)
				{
					//邮件有效性判断完毕，开始正常逻辑
					for (int32 item_guid_index = EMailInfo_ItemGuid11; item_guid_index <= EMailInfo_ItemGuid51; item_guid_index += 2)
					{
						//在此循环中处理所有五个物品
						guid_64 temp_item_guid(m_mail_list[mail_slot].get_data_info((EMailInfo)item_guid_index), m_mail_list[mail_slot].get_data_info((EMailInfo)(item_guid_index + 1)));
						if (!temp_item_guid.is_valid())
						{
							continue;
						}
						m_mail_item_map.erase(temp_item_guid);
						//int32 temp_item_index = get_item_info_index(temp_item_guid);
						//if (temp_item_index >= 0 && temp_item_index < max_mail_item_num)
						//{
						//	m_mail_item_list[temp_item_index].clear_data();
						//}
					}
					msg.add_mail_guid(temp_guid.A);
					msg.add_mail_guid(temp_guid.B);
					delete_mail_in_db(temp_guid);
					clear_date_by_index(mail_slot);
				}
			}
		}

		if (msg.mail_guid_size()>1)
		{
			m_player_ptr->send_message_to_self(&msg, e_msgindex_s2c_mail_delete_these_mail);
		}

		return e_mail_delete_sucess;
	}

	void cmail_mgr::send_mail_batch(std::vector<cmail> mail_list)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		mail_proto_update_mail mail_msg;
		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		mail_msg.add_role_guid(m_player_ptr->get_unit_guid().B);
		mail_msg.set_is_need_clear(false);
		mail_msg.set_is_over(false);
		std::vector<cmail>::iterator ite;
		for (ite = mail_list.begin(); ite != mail_list.end(); ++ite)
		{
			int32 temp_size = mail_msg.ByteSize();

			//包大小大于4800就不能发了，所以判断大于4000就先发一波
			if (temp_size > packege_length_max)
			{
				m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_update);
				mail_msg.clear_mail_list();
			}
			if (ite->is_valid())
			{
				get_mail_msg(mail_msg, *ite);
			}
		}
		mail_msg.set_is_over(true);
		m_player_ptr->send_message_to_self(&mail_msg, e_msgindex_s2c_mail_update);
	}

	void cmail_mgr::send_mail_to_another_player(guid_64 TargetPlayerGuid,
		int32 target_server_id,
		std::string title, 
		std::string contentText, 
		int32 money_typ1, 
		int32 money_num1,
		int32 money_typ2,
		int32 money_num2,
		std::vector<guid_64>& item_guid_list)
	{
		//单机版没有收发邮件

		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (!TargetPlayerGuid.is_valid())
		{
			return;
		}
		if (item_guid_list.size() > max_item_per_mail)
		{
			return;
		}

		//检查物品是否存在
		std::vector<Entity*> item_list;
		for (uint32 i = 0 ; i < item_guid_list.size() ; ++i)
		{
			auto item_ent = get_entity(item_guid_list[i]);
			if (item_ent)
			{
				item_list.push_back(item_ent);
			}
		}

		//检查钱够不够
		if(!m_player_ptr->can_cut_money((e_money_type)money_typ1, money_num1) ||
		!m_player_ptr->can_cut_money((e_money_type)money_typ2, money_num2))
		{
			return;
		}

		cs2ws_send_mail send_mail_info;
		send_mail_info.receiver_server_id = target_server_id;

		init_unit::init_mail_data_array(send_mail_info.mail_info, item_guid_list, money_typ1, money_num1, money_typ2, money_num2, m_player_ptr->get_unit_guid());

		int32 string_len = 0;
		if (contentText.length() > max_text_contents_length)
		{
			string_len = max_text_contents_length;
		}
		else
		{
			string_len = contentText.length();
		}
		memcpy(send_mail_info.mail_info.text_contents, contentText.c_str(), string_len);

		if (title.length() > max_mail_title_length)
		{
			string_len = max_mail_title_length;
		}
		else
		{
			string_len = title.length();
		}
		memcpy(send_mail_info.mail_info.mail_title, title.c_str(), string_len);

		memcpy(send_mail_info.mail_info.sender_name, m_player_ptr->get_name(), max_name_size);



		//物品数据
		for (int32 i = 0; i < item_list.size(); ++i)
		{
			send_mail_info.item_list[i].item_guid = item_list[i]->getEntityId();
			for (int32 j = 0 ; j < e_item_info_max ; ++j)
			{
				//send_mail_info.item_list[i].data_ary[j] = item_list[i]->get_data_info((e_item_info)j);

				//设置为 邮件包/-1格子
				if (j == e_item_info_container_type)
				{
					send_mail_info.item_list[i].data_ary[j] = e_bag_type_mail;
				}
				else if(j == e_item_info_slot)
				{
					send_mail_info.item_list[i].data_ary[j] = -1;
				}
			}
			send_mail_info.item_num++;
		}

		for (int32 i = 0 ; i < item_list.size(); ++i)
		{
			//删除本地物品
			item_system::cost_item_by_ent(item_list[i]);
		}

		//扣钱
		m_player_ptr->cut_money((e_money_type)money_typ1, money_num1, e_server_log_cut_money_send_mail_content);
		m_player_ptr->cut_money((e_money_type)money_typ2, money_num2, e_server_log_cut_money_send_mail_content);

		send_mail_info.addressee_guid = TargetPlayerGuid;
		connection_mgr::getInstance().send_to_ws(&send_mail_info, sizeof(send_mail_info));
	}

	void cmail_mgr::send_mail_to_another_player_by_system(guid_64 TargetPlayerGuid,
		int32 target_server_id,
		std::string title,
		std::string contentText,
		int32 money_typ1,
		int32 money_num1,
		int32 money_typ2,
		int32 money_num2,
		const std::vector<citem*>& item_list)
	{
		//单机版没有收发邮件

		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (!TargetPlayerGuid.is_valid())
		{
			return;
		}
		if (item_list.size() > max_item_per_mail)
		{
			return;
		}

		cs2ws_send_mail send_mail_info;
		send_mail_info.receiver_server_id = target_server_id;
		std::vector<guid_64> temp_item_guid_array;
		for (int32 i = 0; i < item_list.size(); ++i)
		{
			temp_item_guid_array.push_back(item_list[i]->get_item_guid());
		}

		init_unit::init_mail_data_array(send_mail_info.mail_info, temp_item_guid_array, money_typ1, money_num1, money_typ2, money_num2, m_player_ptr->get_unit_guid());

		int32 string_len = 0;
		if (contentText.length() > max_text_contents_length)
		{
			string_len = max_text_contents_length;
		}
		else
		{
			string_len = contentText.length();
		}
		memcpy(send_mail_info.mail_info.text_contents, contentText.c_str(), string_len);

		if (title.length() > max_mail_title_length)
		{
			string_len = max_mail_title_length;
		}
		else
		{
			string_len = title.length();
		}
		memcpy(send_mail_info.mail_info.mail_title, title.c_str(), string_len);

		//物品数据
		for (int32 i = 0; i < item_list.size(); ++i)
		{
			send_mail_info.item_list[i].item_guid = item_list[i]->get_item_guid();
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				send_mail_info.item_list[i].data_ary[j] = item_list[i]->get_data_info((e_item_info)j);

				//设置为 邮件包/-1格子
				if (j == e_item_info_container_type)
				{
					send_mail_info.item_list[i].data_ary[j] = e_bag_type_mail;
				}
				else if (j == e_item_info_slot)
				{
					send_mail_info.item_list[i].data_ary[j] = -1;
				}
			}
			send_mail_info.item_num++;
		}

		send_mail_info.addressee_guid = TargetPlayerGuid;
		connection_mgr::getInstance().send_to_ws(&send_mail_info, sizeof(send_mail_info));
	}

		void cmail_mgr::send_mail_to_another_player_by_system(guid_64 TargetPlayerGuid,
			int32 target_server_id,
			std::string title,
			std::string contentText,
			int32 money_typ1,
			int32 money_num1,
			int32 money_typ2,
			int32 money_num2,
			s_item_info* item_list,
			int32 item_num,
			int64 cur_time_sec)
		{
			//单机版没有收发邮件

			if (m_player_ptr == nullptr)
			{
				return;
			}
			if (!TargetPlayerGuid.is_valid())
			{
				return;
			}
			if (item_num > max_item_per_mail)
			{
				return;
			}

			cs2ws_send_mail send_mail_info;
			send_mail_info.receiver_server_id = target_server_id;
			std::vector<guid_64> temp_item_guid_array;
			for (int32 i = 0; i < item_num; ++i)
			{
				if (item_list[i].item_guid.is_valid())
				{
					temp_item_guid_array.push_back(item_list[i].item_guid);
				}
			}

			init_unit::init_mail_data_array(send_mail_info.mail_info, temp_item_guid_array, money_typ1, money_num1, money_typ2, money_num2, m_player_ptr->get_unit_guid(), cur_time_sec);

			int32 string_len = 0;
			if (contentText.length() > max_text_contents_length)
			{
				string_len = max_text_contents_length;
			}
			else
			{
				string_len = contentText.length();
			}
			memcpy(send_mail_info.mail_info.text_contents, contentText.c_str(), string_len);

			if (title.length() > max_mail_title_length)
			{
				string_len = max_mail_title_length;
			}
			else
			{
				string_len = title.length();
			}
			memcpy(send_mail_info.mail_info.mail_title, title.c_str(), string_len);

			//物品数据
			for (int32 i = 0; i < item_num; ++i)
			{
				if (!item_list[i].item_guid.is_valid())
				{
					continue;
				}
				send_mail_info.item_list[i].item_guid = item_list[i].item_guid;
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					send_mail_info.item_list[i].data_ary[j] = item_list[i].data_ary[j];

					//设置为 邮件包/-1格子
					if (j == e_item_info_container_type)
					{
						send_mail_info.item_list[i].data_ary[j] = e_bag_type_mail;
					}
					else if (j == e_item_info_slot)
					{
						send_mail_info.item_list[i].data_ary[j] = -1;
					}
				}
				send_mail_info.item_num++;
			}

			send_mail_info.addressee_guid = TargetPlayerGuid;
			connection_mgr::getInstance().send_to_ws(&send_mail_info, sizeof(send_mail_info));
		}


	void cmail_mgr::send_mail_to_all_player(
		std::string title,
		std::string contentText,
		int32 money_typ1,
		int32 money_num1,
		int32 money_typ2,
		int32 money_num2,
		std::vector<guid_64>& item_guid_list)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (item_guid_list.size() > max_item_per_mail)
		{
			return;
		}

		cs2ws_send_mail_to_all_player send_mail_info;
		std::vector<s_item_info> temp_item_info_array;
		for (int32 i = 0; i < item_guid_list.size(); ++i)
		{
			//citem* temp_item_ptr = m_player_ptr->get_item_set().get_item(item_guid_list[i]);
			//if (temp_item_ptr != nullptr)
			//{
			//	temp_item_info_array.push_back(temp_item_ptr->get_item_inst());

			//	//使用新的guid
			//	temp_item_info_array[i].item_guid = guid_gen::make_guid(m_player_ptr->get_unit_guid());
			//	item_guid_list[i] = temp_item_info_array[i].item_guid;
			//}
		}
		init_unit::init_mail_data_array(send_mail_info.mail_info, item_guid_list, money_typ1, money_num1, money_typ2, money_num2, m_player_ptr->get_unit_guid());
		
		int32 string_len = 0;
		if (contentText.length() + 1 > max_text_contents_length)
		{
			string_len = max_text_contents_length;
		}
		else
		{
			string_len = contentText.length() + 1;
		}
		memcpy(send_mail_info.mail_info.text_contents, contentText.c_str(), string_len);

		if (title.length() + 1 > max_mail_title_length)
		{
			string_len = max_mail_title_length;
		}
		else
		{
			string_len = title.length() + 1;
		}
		memcpy(send_mail_info.mail_info.mail_title, title.c_str(), string_len);
		memcpy(send_mail_info.mail_info.sender_name, m_player_ptr->get_name(), max_name_size);



		//物品数据
		for (int32 i = 0; i < temp_item_info_array.size(); ++i)
		{
			send_mail_info.item_list[i].item_guid = temp_item_info_array[i].item_guid;
			for (int32 j = 0; j < e_item_info_max; ++j)
			{
				send_mail_info.item_list[i].data_ary[j] = temp_item_info_array[i].data_ary[j];

				//设置为 邮件包/-1格子
				if (j == e_item_info_container_type)
				{
					send_mail_info.item_list[i].data_ary[j] = e_bag_type_mail;
				}
				else if (j == e_item_info_slot)
				{
					send_mail_info.item_list[i].data_ary[j] = -1;
				}
			}
			send_mail_info.item_num++;
		}


		connection_mgr::getInstance().send_to_ws(&send_mail_info, sizeof(send_mail_info));
	}



	cmail* cmail_mgr::receive_mail(s_mail_info mail_info, std::vector<s_item_info>& item_list)
	{
		
		for (int32 _count = 0;; ++_count)
		{
			//删除旧邮件，做循环是如果日后邮件总数变小，收件时将会把多出来的邮件全清掉

			if (_count == MAX_MAIL_NUM)
			{
				//防错，删除多与邮件的数量等于全部邮件数，应该就是有问题了
				return nullptr;
			}

			//正常情况，第二次循环就应该 break 了
			if (!can_receive_new_mail())
			{
				del_oldest_mail();
			}
			else
			{
				break;
			}
		}
		//s_item_info temp_mail_item_list[max_item_per_mail];
		//for (int32 i = 0; i < item_list.size(); ++i)
		//{
		//	int32 temp_item_empty_slot = get_empty_item_slot();
		//	if (temp_item_empty_slot < 0 || temp_item_empty_slot >= max_mail_item_num)
		//	{
		//		return nullptr;
		//	}
		//	temp_mail_item_list[i] = item_list[i];	
		//}

		//add_mail_to_db(mail_info);
		//add_mail_item_to_db(temp_mail_item_list, item_list.size());

		if (!can_receive_new_mail())
		{
			return nullptr;            	
		}

		for (int32 i = 0; i < item_list.size(); ++i)
		{
			m_mail_item_map.insert( {item_list[i].item_guid, item_list[i]} );

			//int32 temp_item_empty_slot = get_empty_item_slot();
			//if (temp_item_empty_slot < 0 || temp_item_empty_slot >= max_mail_item_num)
			//{
			//	return nullptr;
			//}
			//m_mail_item_list[temp_item_empty_slot] = item_list[i];
		}

		cmail* temp_mail = m_player_ptr->get_mail_mgr().create_mail_by_info(mail_info);
		if (temp_mail != nullptr)
		{
			//send_mail_item_all();
			m_player_ptr->get_mail_mgr().send_mail_one(*temp_mail);
			send_mail_one_item(*temp_mail);
		}

		return temp_mail;
	}

	void cmail_mgr::send_mail_one_item(cmail&  new_mail)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		if (new_mail.get_item_num() <= 0)
		{
			return;
		}
		item_proto_update_character_item item_msg;
		item_msg.add_role_guid(m_player_ptr->get_unit_guid().A);
		item_msg.add_role_guid(m_player_ptr->get_unit_guid().B);


		for (int32 i = 0; i < max_item_per_mail; i++)
		{
			guid_64 temp_guid = new_mail.get_target_item_guid(i);
			if (!temp_guid.is_valid())
			{
				continue;
			}
			unit_mail_item_map_it it = m_mail_item_map.find(temp_guid);
			if (it == m_mail_item_map.end())
			{
				continue;
			}
			get_mail_item_msg(item_msg, it->second);
		}

		m_player_ptr->send_message_to_self(&item_msg, e_msgindex_s2c_mail_item_update);
	}

	void cmail_mgr::add_mail_to_db(s_mail_info mail_data)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		cs2dp_add_char_mail msg;
		msg.role_guid = m_player_ptr->get_unit_guid();
		memcpy(&msg.mail_info, &mail_data, sizeof(s_mail_info));
		m_player_ptr->send_message_to_dp( &msg, sizeof(cs2dp_add_char_mail));
	}
	void cmail_mgr::delete_mail_in_db(guid_64* mail_guid, int32 guid_num)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		cs2dp_del_char_mail del_msg;
		for (int32 i = 0; i < guid_num; ++i)
		{
			if (del_msg.data_num >= MAX_MAIL_NUM)
			{
				break;
			}
			if (mail_guid[i].is_valid())
			{
				del_msg.mail_guid[del_msg.data_num] = mail_guid[i];
				del_msg.data_num++;
			}
		}
		del_msg.unit_index = m_player_ptr->get_array_index();
		del_msg.unit_guid = m_player_ptr->get_unit_guid();
		m_player_ptr->send_message_to_dp( &del_msg, sizeof(cs2dp_del_char_mail));
	}
	void cmail_mgr::delete_mail_in_db(const guid_64& mail_guid)
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}
		cs2dp_del_char_mail del_msg;
		del_msg.data_num = 1;
		del_msg.mail_guid[0] = mail_guid;
		del_msg.unit_index = m_player_ptr->get_array_index();
		del_msg.unit_guid = m_player_ptr->get_unit_guid();
		m_player_ptr->send_message_to_dp( &del_msg, sizeof(cs2dp_del_char_mail));
	}

	void cmail_mgr::delete_mail_item_in_db(guid_64 mail_item_guid)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}
		cs2dp_del_char_mail_item del_msg;
		del_msg.unit_guid = m_player_ptr->get_unit_guid();
		del_msg.item_guid = mail_item_guid;
		m_player_ptr->send_message_to_dp( &del_msg, sizeof(cs2dp_del_char_mail_item));
	}

	void cmail_mgr::load_mail_item_by_db(const s_item_info* item_data, int32 item_num)
	{
		if (nullptr == m_player_ptr)
		{
			return;
		}

		if (nullptr == item_data)
		{
			return;
		}

		//for (int32 i = 0; i < max_mail_item_num; ++i)
		//{
		//	m_mail_item_list[i].clear_data();
		//}
		m_mail_item_map.clear();
		s_item_info temp;
		for (int32 i = 0; i < item_num; i++)
		{
			m_mail_item_map.insert({ item_data[i].item_guid, item_data[i] });
			//int32 empty_slot = get_empty_item_slot();
			//if (empty_slot < 0 || empty_slot >= max_mail_item_num)
			//{
			//	break;
			//}
			//m_mail_item_list[empty_slot] = item_data[i];
		}
		send_mail_all();
		send_mail_item_all();
		operate_load_lock(-1);
		check_mail_event_state();
	}

	bool cmail_mgr::load_mail_item_by_db_lua(const char *data_ptr, int32 data_len)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return false;
		}
		hld::db_proto::role_mail_item_db msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return false;
		}

		xchar *p_data = parse_msg::getInstance().get_cache(msg.row_count() * sizeof(s_item_info));
		if (p_data == nullptr)
		{
			return false;
		}
		s_item_info *p_row = (s_item_info *)p_data;
		for (int32 i = 0; i < msg.row_count(); i++)
		{
			hld::db_proto::item_info_db db_row = msg.row_data(i);
			p_row->item_guid.server_64 = db_row.item_guid();
			for (int32 j = 0; j < db_row.data_ary_size(); j++)
			{
				p_row->data_ary[j] = db_row.data_ary(j);
			}
			parse_msg::getInstance().my_memcopy_string(p_row->param_str, max_recharge_order_length, db_row.param_str());
			p_row++;
		}
		load_mail_item_by_db((const s_item_info *)p_data, msg.row_count());
		return true;

	}


	void cmail_mgr::del_mail_end()
	{
		if (get_mail_num() == 0)
		{
			auto_load_mail();
		}
	}

	int32 cmail_mgr::get_mail_num()
	{
		int32 mail_num = 0;
		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			if (m_mail_list[i].is_valid())
			{
				mail_num++;
			}
		}
		return mail_num;
	}

	int32 cmail_mgr::get_empty_mail_slot()
	{
		for (int32 i = 0; i < MAX_MAIL_NUM; ++i)
		{
			if (!m_mail_list[i].is_valid())
			{
				return i;
			}
		}
		return -1;
	}

	bool cmail_mgr::can_receive_new_mail()
	{
		if (get_mail_num() >= MAX_MAIL_NUM)
		{
			return false;
		}
		return true;
	}
	void cmail_mgr::del_oldest_mail()
	{
		int32 last_mail_index = -1;
		int32 this_mail_send_time = time_helper::get_cur_time_new().second;
		for (int32 i = 0; i < m_mail_list_num; ++i)
		{
			if (m_mail_list[i].is_valid()
				&& m_mail_list[i].get_data_info(EMailInfo_SendTime) < this_mail_send_time)
			{
				last_mail_index = i;
				this_mail_send_time = m_mail_list[last_mail_index].get_data_info(EMailInfo_SendTime);
			}
		}
		if (last_mail_index < 0
			|| last_mail_index >= m_mail_list_num)
		{
			return;
		}
		//删除附件物品
		for (int32 i = 0 ; i < max_item_per_mail; ++i)
		{
			guid_64 temp_guid = m_mail_list[last_mail_index].get_target_item_guid(i);
			if (!temp_guid.is_valid())
			{
				continue;
			}
			m_mail_item_map.erase(temp_guid);

			//int32 temp_item_index = get_item_info_index(temp_guid);
			//if (temp_item_index >=0 && temp_item_index < max_mail_item_num)
			//{
			//	m_mail_item_list[temp_item_index].clear_data();
			//}
		}

		m_mail_list[last_mail_index].set_data_info(EMailInfo_IsNeedDelete, 1);
		send_mail_one(m_mail_list[last_mail_index]);

		//不清数据库只清理内存 数据库中的还要呢
		//delete_mail_in_db(m_mail_list[last_mail_index].get_mail_guid());

		clear_date_by_index(last_mail_index);
	}

	void cmail_mgr::clear_del_array()
	{
		for (int32 i = 0; i < MAX_MAIL_NUM; ++i)
		{
			m_del_mail_array[i].clear_data();
		}
		del_array_effect_num = 0;
	}

	void cmail_mgr::add_to_del_array(const guid_64& target_guid)
	{
		if (del_array_effect_num >=0
			&& del_array_effect_num < MAX_MAIL_NUM)
		{
			m_del_mail_array[del_array_effect_num] = target_guid;
			del_array_effect_num++;
		}
		else
		{
			del_by_del_array();
			if (del_array_effect_num >= 0
				&& del_array_effect_num < MAX_MAIL_NUM)
			{
				m_del_mail_array[del_array_effect_num] = target_guid;
				del_array_effect_num++;
			}
		}
	}

	void cmail_mgr::del_by_del_array()
	{
		delete_mail_in_db(m_del_mail_array, del_array_effect_num);
		clear_del_array();
	}

	void cmail_mgr::auto_load_mail()
	{
		if (m_player_ptr == nullptr)
		{
			return;
		}

		cs2dp_auto_load_mail_by_db auto_load_msg;
		auto_load_msg.role_guid = m_player_ptr->get_unit_guid();
		auto_load_msg.unit_array_index = m_player_ptr->get_array_index();
		m_player_ptr->send_message_to_dp( &auto_load_msg, sizeof(cs2dp_auto_load_mail_by_db));

		if (m_player_ptr->is_self_server())
		{
			cs2ws_check_id_mail check_id_mail_msg;
			check_id_mail_msg.role_guid = m_player_ptr->get_unit_guid();
			check_id_mail_msg.server_id = m_player_ptr->get_unit_info(e_role_info_server_id);
			check_id_mail_msg.cur_time = time_helper::get_cur_time_new().second;
			connection_mgr::getInstance().send_to_ws(&check_id_mail_msg, sizeof(cs2ws_check_id_mail));
		}


		m_auto_load_timer = init_unit::get_end_time(auto_load_mail_time);

		//邮件本体1次，邮件物品1次
		operate_load_lock(2);
	}

	void cmail_mgr::operate_load_lock(int32 change_num)
	{
		m_mail_load_lock += change_num;

		if (m_mail_load_lock < 0)
		{
			m_mail_load_lock = 0;
		}
	}
}