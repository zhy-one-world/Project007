/********************************************************************
  created: 2018/09/08
  created: 8:9:2018 18:48
  file base: login_service_facebook
  file ext: h
  author: lxy
  
  purpose: 
*********************************************************************/
#ifndef _LOGIN_SERVICE_FACEBOOK_HPP_
#define _LOGIN_SERVICE_FACEBOOK_HPP_

#include <singleton.hpp>
#include "server_def.hpp"
#include "login_service.hpp"
#include "http/http_access_mgr.hpp"
#include <login_msg.hpp>

namespace faith
{
	class login_service_facebook : public login_service , public singleton<login_service_facebook>
	{
		friend class singleton<login_service_facebook>;
	public:
		login_service_facebook() { m_time_count = 0; };
		~login_service_facebook()	{};

	public:
		virtual void						req_login(const s_client_uid client_uid, const login_proto_login proto_data);
		virtual void						end_login(const dp2ls_client_login* pdata);
		virtual void						tick(float deltaseconds);
		void								on_login_result_handle(ui64 http_id,uint32 http_error_code,const xstring& http_error_info,const xstring& http_result);

	private:
		xstring								write_json_data();
		bool								read_json_data(const xstring& json_data,const s_client_uid client_uid);
		
		xstring								create_post_data(const xstring& data, int32 server_id, const xstring& game_id, const xstring& channel_id);
		xstring								create_sign(i16 param_num,...);															// 根据第三方服务定制签名,可变参都是xstring类型
		void								create_http_head_list(std::vector<xstring>& head_list,i16 param_num,...);				// 根据第三方服务定制请求头,可变参都是xstring类型

		void								save_account(const xstring& account, const xstring& server_msg, const s_client_uid client_uid, int64* ban_role_array, int64* ban_chat_array);
	private:
		int32								m_time_count;

		//登录结束后再清理的临时数据，现阶段，log用
		//client_data_map						m_temp_datas;

		/************************************************************************/
		/*							  发送log						       	    */
		/************************************************************************/
	public:
		void									ls2dp_save_server_log(const xstring& log_sql_name, const xstring& sql_param_string);
	};
}


#endif
