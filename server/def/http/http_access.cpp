/********************************************************************
  created: 2014/08/09
  created: 9:8:2014 11:49
  file base: http_access
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#include "logic/type_def.hpp"
#include "http_access.hpp"
#include <net/scheduler.hpp>
#include <time.hpp>
#include "http_access_mgr.hpp"
#include "server_log.hpp"
#include <rlog.hpp>

namespace faith
{	
	http_access::http_access() 
	{
		clear_data();
	}
	http_access::~http_access()
	{
		clear_data();
	}
	int64 http_access::async_request(xstring& sdk_url, std::vector<xstring>& head_list, const xstring& params, http_accessor::result_handler_type handler, e_http_request_type request_type)
	{
		m_url = sdk_url;
		m_handler = handler;
		int64 id = http_accessor::getInstance().async_request(
			sdk_url,
			head_list,
			params,
			boost::bind(&http_access::on_http_access_result, this, _1, _2, _3),
			request_type
		);
		return id;
	}

	void http_access::on_http_access_result(
		int http_error_code,					// 由 HttpAccessorCode中 枚举值定义
		const std::string& http_error_info,
		const std::string& http_result			// html源文件，相当于ascii编码串。（unicode/ansi版本）
	)
	{
		if (http_error_code > 0)
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("http fail url : {}",  m_url));
		}
		if(m_handler.empty() == false)
		{
			m_handler(m_uid, http_error_code, http_error_info, http_result);
		}
		http_access_mgr::get_instance().remove_http(m_list_num);			
	}
	void http_access::clear_data()
	{
		m_list_num = 0;
		m_uid = 0;
		m_is_use = false;
		m_handler.clear();
	}
}