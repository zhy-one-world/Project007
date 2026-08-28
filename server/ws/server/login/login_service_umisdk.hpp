/********************************************************************
created: 2020/03/04
created: 3:4:2020 15:58
file base: login_service_umisdk
file ext: h
author: wy

purpose: sdk login
*********************************************************************/
#ifndef _LOGIN_SERVICE_umisdk_HPP_
#define _LOGIN_SERVICE_umisdk_HPP_

#include <singleton.hpp>
#include "login_service.hpp"
#include "http/http_access_mgr.hpp"
namespace hld
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Class Declare
	//
	//////////////////////////////////////////////////////////////////////////

	class login_service_umisdk : public login_service, public singleton<login_service_umisdk>
	{
		friend class singleton<login_service_umisdk>;
	public:
		login_service_umisdk() { m_time_count = 0; };
		~login_service_umisdk() {};
		login_service_umisdk(int32 login_type) : m_login_type(login_type) { };
	public:
		virtual void						req_login(const s_client_uid client_uid, const login_proto_login proto_data);
		virtual void						end_login(const dp2ls_client_login* pdata);
		virtual void						tick(float deltaseconds);
		void								on_login_result_handle(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result);

	private:

		bool								read_json_data(const xstring& json_data, const s_client_uid client_uid);

		xstring								create_post_data(const xstring& data);
		xstring								create_sign(i16 param_num, ...);															// 根据第三方服务定制签名,可变参都是xstring类型
		//void								create_http_head_list(std::vector<xstring>& head_list, i16 param_num, ...);				// 根据第三方服务定制请求头,可变参都是xstring类型

		void								save_account(const xstring& account, const xstring& server_msg, const s_client_uid client_uid, int64* ban_role_array, int64* ban_chat_array);
	private:
		int32								m_time_count;
		int32								m_login_type;
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