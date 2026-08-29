/********************************************************************
	created:	2016Äê9ÔÂ3ÈÕ03:07:50
	file base:	re_login_mgr
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "re_login_mgr.h"
#include <login_msg.hpp>
#include "ws_client.hpp"
#include "client_session_mgr.hpp"

namespace faith
{
	re_login_mgr::re_login_mgr(void)
	{
		m_re_login_info_map.clear();
	}
	re_login_mgr::~re_login_mgr(void)
	{
		m_re_login_info_map.clear();
	}

	void re_login_mgr::tick(int32 dt)
	{
		for (re_login_info_map_it it = m_re_login_info_map.begin(); it != m_re_login_info_map.end(); )
		{
			re_login_info_map_it temp_it = it++;
			s_re_login_info& re_login_info = temp_it->second;
			client_session* old_session_ptr = client_session_mgr::getInstance().get_session(re_login_info.m_account);
			if (nullptr == old_session_ptr)
			{
				client_session_mgr::getInstance().login_client(&(re_login_info.m_login_data));
				m_re_login_info_map.erase(temp_it);
			}
		}
	}
	void re_login_mgr::remove_re_login(const std::string& account)
	{
		re_login_info_map_it it = m_re_login_info_map.find(account);
		if (it != m_re_login_info_map.end())
		{
			m_re_login_info_map.erase(it);
		}
	}
	void re_login_mgr::add_re_login(const std::string& account, s_re_login_info& re_info)
	{
		m_re_login_info_map[account] = re_info;
	}
}
