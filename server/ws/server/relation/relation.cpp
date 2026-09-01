/********************************************************************
	file name:	relation.cpp
	created:	2017/05/26 13:49
	author:		zhy
	purpose:	
*********************************************************************/

#include "ws_client.hpp"
#include "relation_mgr.h"
#include "../client_session.hpp"
#include "../client_session_mgr.hpp"
#include <char_msg.hpp>
#include "template/template_manager.h"
#include "time.hpp"
#include "utility/init_unit.h"
#include "../ranking/ranking_mgr_ws.h"
#include "../mail/event_ws_mgr.h"


namespace faith
{
	relation::relation()
	{
		clear_data();
	}

	bool relation::build_relation(client_session* puser)
	{
		if (!puser)
		{
			return false;
		}

		//初始化player的relation属性
		m_relation_info.relation_guid = puser->get_role_guid();
		this->set_data_name(puser->m_role_info.role_name);
		m_relation_info.gs_value = puser->get_role_gs_value();
		m_relation_info.data_array[e_relation_data_type] = 0;
		m_relation_info.data_array[e_relation_data_level] = puser->m_role_info.data_ary[e_role_info_exp_level];
		m_relation_info.data_array[e_relation_data_template_id] = puser->m_role_info.data_ary[e_role_info_template_id];
		m_relation_info.data_array[e_relation_data_relation_state] = e_relation_online;
		m_relation_info.data_array[e_relation_data_last_login_time] = puser->m_logintime;
		m_relation_info.data_array[e_relation_data_friendliness] = 0;
		m_relation_info.data_array[e_relation_data_vip_title_template_id] = 0;

		return true;
	}

	bool relation::is_valid()
	{
		return m_is_use;
	}

	bool relation::serialize_to_db(s_relation_info& datas)
	{
		datas.relation_guid = m_relation_info.relation_guid.server_64;
		memset(datas.relation_name, 0, sizeof(datas.relation_name));
		datas.gs_value = m_relation_info.gs_value;
		snprintf(datas.relation_name, sizeof(datas.relation_name), "%s", m_relation_info.relation_name);
		for (int32 data_index = 0; data_index < e_relation_data_max; data_index++)
		{
			datas.data_array[data_index] = m_relation_info.data_array[data_index];
		}

		return true;
	}

	bool relation::serialize_from_db(const s_relation_info& datas)
	{
		m_is_use = true;
		m_relation_info.relation_guid = datas.relation_guid;
		m_relation_info.gs_value = datas.gs_value;
		strncpy(m_relation_info.relation_name, datas.relation_name, max_name_size);
		for (int32 data_index = 0; data_index < e_relation_data_max; data_index++)
		{
			m_relation_info.data_array[data_index] = datas.data_array[data_index];
		}
		return true;

	}

	/********************************************************************/
	/* author::allen	code here: relation 							*/
	/********************************************************************/

	void	relation::set_data_name(const xchar* role_name)
	{
		memset(m_relation_info.relation_name, 0, sizeof(m_relation_info.relation_name));
		int32 count = 0;
		count = sizeof(m_relation_info.relation_name) > strlen(role_name) ? strlen(role_name) : sizeof(m_relation_info.relation_name);
		strncpy(m_relation_info.relation_name, role_name, count);
	}

	int32	relation::get_data_value(e_relation_data infotype)
	{
		if (infotype >= e_relation_data_max)
		{
			return -1;
		}

		return m_relation_info.data_array[infotype];
	}

	bool	relation::set_data_value(e_relation_data infotype, int32 value)
	{
		if (infotype >= e_relation_data_max )
		{
			return false;
		}

		m_relation_info.data_array[infotype] = value;
		return true;
	}

	void relation::set_data_gs_value(int64 gs_value)
	{
		m_relation_info.gs_value = gs_value;
	}

	void	relation::clear_data()
	{
		m_is_use = false;
		m_relation_info.clear_data();
	}
}