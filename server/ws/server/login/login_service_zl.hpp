/********************************************************************
  created: 2014/08/05
  created: 5:8:2014 18:48
  file base: login_service_zl
  file ext: hpp
  author: YU REN
  
  purpose: sdk login
*********************************************************************/
#ifndef _login_service_zl_HPP_
#define _login_service_zl_HPP_

#include <singleton.hpp>
#include "server_def.hpp"
#include "login_service.hpp"
#include "http/http_access_mgr.hpp"
#include <login_msg.hpp>

namespace faith
{
	class login_service_zl : public login_service , public singleton<login_service_zl>
	{

		friend class singleton<login_service_zl>;
	public:
		login_service_zl() { m_time_count = 0; };
		~login_service_zl()	{};

	public:
		virtual void						req_login(const s_client_uid client_uid, const login_proto_login proto_data);
		virtual void						end_login(const dp2ls_client_login* pdata);
		virtual void						tick(float deltaseconds);
		void								on_login_result_handle(ui64 http_id,uint32 http_error_code,const xstring& http_error_info,const xstring& http_result);

	private:
		xstring								write_json_data();
		bool								read_json_data(const xstring& json_data,const s_client_uid client_uid);
		
		xstring								create_post_data(const xstring& data);
		xstring								create_sign(i16 param_num,...);															// 根据第三方服务定制签名,可变参都是xstring类型
		void								create_http_head_list(std::vector<xstring>& head_list,i16 param_num,...);				// 根据第三方服务定制请求头,可变参都是xstring类型

		void								save_account(const xstring& account, const s_client_uid client_uid);
	private:
		int32								m_time_count;

		//登录结束后再清理的临时数据，现阶段，log用
		//client_data_map						m_temp_datas;
	};
}


#endif
