/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: login_service_google
  file ext: h
  author: lxy
  
  purpose: sdk login
*********************************************************************/
#ifndef _LOGIN_SERVICE_TW_HPP_
#define _LOGIN_SERVICE_TW_HPP_

#include <singleton.hpp>
#include "server_def.hpp"
#include "login_service.hpp"
#include "http/http_access_mgr.hpp"
#include <login_msg.hpp>
#include <net.pb.h>

namespace faith
{
	class login_service_tw : public login_service , public singleton<login_service_tw>
	{
		friend class singleton<login_service_tw>;
	public:
		login_service_tw();
		~login_service_tw()	{};

	public:
		virtual void						req_login(const s_client_uid client_uid, const login_proto_login proto_data);
		virtual void						end_login(const dp2ls_client_login* pdata);
		void								on_login_result_handle(ui64 http_id,uint32 http_error_code,const xstring& http_error_info,const xstring& http_result);

	private:
		xstring								write_json_data();
		bool								read_json_data(const xstring& json_data,const s_client_uid client_uid);
		
		xstring								create_post_data(const xstring& data, int32 server_id, const xstring& game_id, const xstring& channel_id, const xstring& custom_data);
		xstring								create_sign(i16 param_num,...);															// 根据第三方服务定制签名,可变参都是xstring类型
		void								create_http_head_list(std::vector<xstring>& head_list,i16 param_num,...);				// 根据第三方服务定制请求头,可变参都是xstring类型

		void								save_account(const xstring& account, const xstring& server_msg, const s_client_uid client_uid, int64* ban_role_array, int64* ban_chat_array);
		xstring								m_sdk_url;
	};
}


#endif
