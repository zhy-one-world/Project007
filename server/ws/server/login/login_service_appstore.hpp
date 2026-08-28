/********************************************************************
  created: 2020/07/29
  file base: login_service_appstore
  file ext: cpp
  author: wangdi
  
  purpose: sdk login
*********************************************************************/
#ifndef _LOGIN_SERVICE_APPSTORE_HPP_
#define _LOGIN_SERVICE_APPSTORE_HPP_

#include <singleton.hpp>
#include "server_def.hpp"
#include "login_service.hpp"
#include "http/http_access_mgr.hpp"
#include <login_msg.hpp>

namespace hld
{
	class login_service_appstore : public login_service , public singleton<login_service_appstore>
	{
		friend class singleton<login_service_appstore>;
	public:
		login_service_appstore();
		~login_service_appstore()	{};

	public:
		virtual void						req_login(const s_client_uid client_uid, const login_proto_login proto_data);
		virtual void						end_login(const dp2ls_client_login* pdata);
		virtual void						tick(float deltaseconds);
		void								on_login_result_handle(ui64 http_id,uint32 http_error_code,const xstring& http_error_info,const xstring& http_result);

	private:
		const xstring&						get_client_secret();
		void								refresh_client_secret();
		bool								read_json_data(const xstring& json_data,const s_client_uid client_uid);
		void								save_account(const xstring& account, const xstring& server_msg, const s_client_uid client_uid, int64* ban_role_array, int64* ban_chat_array);
	private:
		int32								m_time_count;
		int32								m_last_client_secret_time;
		xstring								m_client_secret;
	};
}


#endif
