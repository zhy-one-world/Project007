/********************************************************************
  created: 2014/08/09
  created: 9:8:2014 11:48
  file base: http_access
  file ext: hpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#pragma once
#include "logic/type_def.hpp"
#include "server_def.hpp"
#include <net/http_accessor.hpp>

namespace faith
{
	class http_access
	{
	public:
		http_access();
		~http_access();

	public:
		void clear_data();
		int64 async_request(xstring& sdk_url, std::vector<xstring>& head_list, const xstring& params, http_accessor::result_handler_type handler, e_http_request_type request_type);
		void on_http_access_result(
			int http_error_code,		// 由 HttpAccessorCode中 枚举值定义
			const std::string& http_error_info,
			const std::string& http_result			// html源文件，相当于ascii编码串。（unicode/ansi版本）
		);
	public:
		ui64									m_uid;
		int32									m_list_num;
		bool									m_is_use;
		xstring									m_url;
		http_accessor::result_handler_type		m_handler;
	};
}