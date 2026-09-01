/********************************************************************
	file name:	relation.cpp
	created:	2017/05/26 13:49
	author:		zhy
	purpose:	
*********************************************************************/

#include "ws_client.hpp"
#include "relation_set.h"
#include "../client_session.hpp"
#include "../client_session_mgr.hpp"
#include <char_msg.hpp>
#include "template/template_manager.h"
#include "time.hpp"
#include "utility/init_unit.h"
#include "../ranking/ranking_mgr_ws.h"
#include "../mail/event_ws_mgr.h"
#include "relation.pb.h"
#include "net.pb.h"


namespace faith
{
	relation_set::relation_set()
	{
		m_client_session_ptr = nullptr;
		clear_data();
	}

	void	relation_set::clear_data()
	{
		m_relation_type = 0;
		m_relation_array.clear();
	}

	void	relation_set::set_client_session(client_session* client_session_ptr)
	{
		m_client_session_ptr = client_session_ptr;
	}
	void	relation_set::set_relation_type(int32 itype)
	{
		if (itype < 0 || itype >= e_relationlist_type_max)
		{
			return;
		}
		m_relation_type = itype;
	}

	bool	relation_set::is_relation(const guid_64& relation_guid)
	{
		for (relation_list_it it = m_relation_array.begin(); it != m_relation_array.end(); ++it)
		{
			if (relation_guid == (*it).get_data_guid())
			{
				return true;
			}
		}
		return false;
	}

	bool relation_set::add_relation(const s_relation_info& realtion_info, bool send_client)
	{
		relation new_relation;
		new_relation.serialize_from_db(realtion_info);
		m_relation_array.push_front(new_relation);
		if (send_client)
		{
			send_relation_add_msg_to_self(new_relation);
		}
		return true;
	}

	bool	relation_set::del_relation(const guid_64& relation_guid)
	{
		//关系是否合法
		if (!relation_guid.is_valid())
		{
			return false;
		}
		

		for (relation_list_it it = m_relation_array.begin(); it != m_relation_array.end(); ++it)
		{
			if (relation_guid == (*it).get_data_guid())
			{
				m_relation_array.erase(it);
				send_relation_del_msg_to_self(relation_guid);
				return true;
			}
		}
		return false;
	}

	bool	relation_set::is_full()
	{
		return m_relation_array.size() >= MAX_FRIEND_NUM;
	}

	int32	relation_set::get_relation_num()
	{
		return m_relation_array.size();
	}

	relation*	relation_set::get_relation(const guid_64& relation_guid)
	{
		if (!relation_guid.is_valid())
		{
			return nullptr;
		}

		for (relation_list_it it = m_relation_array.begin(); it != m_relation_array.end(); ++it)
		{
			if (relation_guid ==(*it).get_data_guid())
			{
				return &(*it);
			}
		}
		return nullptr;
	}
	int32 relation_set::get_max_firendliness()
	{
		int32 max_value = 0;
		for (relation_list_it it = m_relation_array.begin(); it != m_relation_array.end(); ++it)
		{
			if ((*it).get_data_value(e_relation_data_friendliness) > max_value)
			{
				max_value = (*it).get_data_value(e_relation_data_friendliness);
			}
		}
		return max_value;
	}
	bool	relation_set::serialize_to_db(s_relation_info* data_list, int32& list_count)
	{
		if (nullptr == data_list )
		{
			return false;
		}
		if (list_count >= MAX_FRIEND_NUM*e_relationlist_type_max || list_count < 0)
		{
			return false;
		}
		for (relation_list_it it = m_relation_array.begin(); it != m_relation_array.end(); ++it)
		{
			relation&  relation_ref = *it;
			if (relation_ref.serialize_to_db(data_list[list_count]))
			{
				++list_count;
			}
		}
		return true;
	}
	
	bool	relation_set::serialize_from_db(const s_relation_info& datas)
	{
		if (is_relation(datas.relation_guid))
		{
			return false;
		}
		add_relation(datas, false);
		return true;
	}
	void relation_set::send_relation_add_msg_to_self(relation& relation_ref)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		relation_proto_s2c_add_relation_info msg;
		msg.set_relation_list_type(m_relation_type);
		msg.set_guid(relation_ref.get_data_guid().server_64);
		msg.set_name(relation_ref.get_data_name());
		msg.set_gs_value(init_unit::change_i64_to_string(relation_ref.get_data_gs_value()));
		for (int32 data = 0; data < e_relation_data_max; data++)
		{
			msg.add_data_array(relation_ref.get_data_value((e_relation_data)data));
		}
		m_client_session_ptr->send_to_client(&msg, e_msgindex_s2c_addfriend);
	}

	void	relation_set::send_relation_del_msg_to_self(guid_64 relation_guid)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		relation_proto_s2c_del_relation_info msg;
		msg.set_relation_list_type(m_relation_type);
		msg.set_guid(relation_guid.server_64);
		m_client_session_ptr->send_to_client(&msg, e_msgindex_s2c_delfriend);
	}

	void	relation_set::send_sync_relation_list_to_client(int32 e_msg_relation_type)
	{
		if (nullptr == m_client_session_ptr)
		{
			return;
		}
		relation_proto_s2c_syc_full_relation_list msg;

		for (relation_list_it it = m_relation_array.begin(); it != m_relation_array.end(); ++it)
		{
			relation& relation_ref = *it;
			relation_proto_friend_user_info* friend_user_info = msg.add_full_relation_list_info();
			if (nullptr == friend_user_info)
			{
				return;
			}
			friend_user_info->set_guid(relation_ref.get_data_guid().server_64);
			friend_user_info->set_name(relation_ref.get_data_name());
			friend_user_info->set_gs_value(init_unit::change_i64_to_string(relation_ref.get_data_gs_value()));
			for (int32 data_index = 0; data_index < e_relation_data_max; data_index++)
			{
				friend_user_info->add_data_array(relation_ref.get_data_value((e_relation_data)data_index));
			}
			if (msg.ByteSize() > packege_length_max)
			{
				m_client_session_ptr->send_to_client(&msg, e_msg_relation_type);
				msg.clear_full_relation_list_info();
			}
		}
		if (msg.full_relation_list_info_size() > 0)
		{
			m_client_session_ptr->send_to_client(&msg, e_msg_relation_type);
		}
	}
}