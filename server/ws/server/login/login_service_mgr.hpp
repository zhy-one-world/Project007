/********************************************************************
	created:	2014/06/03
	created:	3:6:2014   13:23
	file base:	login_service_mgr
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _LOGIN_SERVICE_MGR_H_
#define _LOGIN_SERVICE_MGR_H_

#include "server_def.hpp"
#include <Logic/login_def.hpp>
#include <singleton.hpp>
#include "login_service.hpp"
#include <login_msg.hpp>
#include "login.pb.h"

namespace hld
{

	struct login_keep_info
	{
		int64				invalid_time;
		login_proto_login	data;
	};

	class login_service_mgr : public singleton<login_service_mgr>
	{
		friend class singleton<login_service_mgr>;

		login_service_mgr();
		~login_service_mgr();
	public:
		void								tick(int64 time_now);
		void								req_login(const void *data_ptr, size_t data_len);
		void								end_login(const void *data_ptr, size_t data_len);
		const login_proto_login*			get_login_info(const int64 client_uid);

	private:
		login_service*						m_login_service_array[e_login_type_new_max];
		int64								m_next_tick_time;
		std::map<int64, login_keep_info>	m_login_info_keep_map;
	};

}

#endif