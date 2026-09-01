/********************************************************************
	created:	2016年9月3日03:06:47
	file base:	re_login_mgr
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WS_RE_LOGIN_MGR_H_
#define _WS_RE_LOGIN_MGR_H_

#include "client_session.hpp"
#include <singleton.hpp>
#include <unordered_map>
#include <login_msg.hpp>

namespace faith
{
	struct s_re_login_info
	{
		xchar					m_account[max_account_length + 1];
		fep2ws_client_logined	m_login_data;
		s_re_login_info()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	typedef std::unordered_map<std::string, s_re_login_info> re_login_info_map;
	typedef re_login_info_map::iterator re_login_info_map_it;

	class re_login_mgr : public singleton<re_login_mgr>
	{
		friend class singleton<re_login_mgr>;
	public:
		re_login_mgr(void);
		~re_login_mgr(void);
		void tick(int32 dt);
	public:
		void remove_re_login(const std::string& account);
		void add_re_login(const std::string& account, s_re_login_info& re_info);
	private:
		re_login_info_map m_re_login_info_map;//需要重新登录的玩家列表
	};

}

#endif