/********************************************************************
  created: 2014/08/09
  created: 9:8:2014 11:48
  file base: http_access_mgr
  file ext: cpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#include "http_access_mgr.hpp"
#include "json/json.h"
#include "server_log.hpp"
#include <rlog.hpp>

namespace faith
{
	http_access_mgr::http_access_mgr()
	{
		ZoneScoped;
		clear();
	}

	http_access_mgr::~http_access_mgr()
	{
		ZoneScoped;
		clear();
	}

	http_access_mgr& http_access_mgr::get_instance()
	{
		ZoneScoped;
		static http_access_mgr g_instance;
		return g_instance;
	}

	void http_access_mgr::init(bool need_listen)
	{
		ZoneScoped;
		clear();
		http_accessor::getInstance().init();
	}

	void http_access_mgr::tick(const int64& new_time)
	{
		ZoneScoped;
		http_accessor::getInstance().run();
	}
	void http_access_mgr::start_listen_http(int32 listen_port, http_server_callback_type call_back)
	{
		_RLOG_(MINFO, ::faith::log_detail::format_message("http listen success port :{}",  listen_port));
		http_accessor::getInstance().start_listen(listen_port, call_back);
	}
	void http_access_mgr::repose_client_req(int64 handle_index, int32 error_code, xstring error_msg)
	{
		ZoneScoped;
		Json::Value  json_value;
		json_value["errorCode"] = error_code;
		json_value["errorMsg"] = error_msg;

		Json::FastWriter json_write;
		repose_client_req(handle_index, json_write.write(json_value));
	}
	void http_access_mgr::repose_client_req(int64 handle_index, const xstring& repose_body)
	{
		ZoneScoped;
		http_accessor::getInstance().repose_client_req(0, handle_index, repose_body);
	}
	void http_access_mgr::new_repose_client_req(int64 handle_index, int32 error_code, xstring error_msg)
	{
		ZoneScoped;
		Json::Value  json_value;
		json_value["code"] = error_code; // error_code;
		json_value["msg"] = error_msg;
		json_value["data"] = "";
		Json::FastWriter json_write;
		repose_client_req(handle_index, json_write.write(json_value));
	}
	uint32 http_access_mgr::async_request(ui64 uid, xstring& sdk_url, std::vector<xstring>& head_list,const xstring& params, http_accessor::result_handler_type handler, e_http_request_type request_type)
	{
		ZoneScoped;
		for (int32 i =0; i < init_player_max; ++i)
		{
			http_access& http_access_ref = m_http_map[i];
			if (http_access_ref.m_is_use == false)
			{
				http_access_ref.m_is_use = true;
				http_access_ref.m_list_num = i;
				http_access_ref.m_uid = uid;
				http_access_ref.async_request(sdk_url, head_list, params, handler, request_type);
				break;
			}
		}
		return 1;
	}


	void http_access_mgr::remove_http(s_client_uid client_uid)
	{
		ZoneScoped;

	}
	void http_access_mgr::remove_http(int32 http_count)
	{
		ZoneScoped;
		if (http_count<0 || http_count>= init_player_max)
		{
			return;
		}
		m_http_map[http_count].clear_data();
	}

	void http_access_mgr::clear()
	{
		ZoneScoped;
		for (int32 i = 0; i < init_player_max; ++i)
		{
			m_http_map[i].clear_data();
		}
	}
	
}
