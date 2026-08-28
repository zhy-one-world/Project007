/********************************************************************
  created: 2014/08/09
  created: 9:8:2014 11:48
  file base: http_access_mgr
  file ext: hpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#ifndef _HTTP_ACCESSOR_MGR_H_
#define _HTTP_ACCESSOR_MGR_H_

#include <Utility/guid_gen.h>
#include "http_access.hpp"
#include "Logic/count_def.hpp"
#include "Logic/world_def.hpp"
#include "base.hpp"

namespace hld
{
	class http_access_mgr
	{
	public:
		http_access_mgr();
		~http_access_mgr();

	public:
		static http_access_mgr&	get_instance();
		void					init(bool need_listen);
		void					tick(const int64& new_time);
		void					start_listen_http(int32 listen_port, http_server_callback_type call_back);
		void					repose_client_req(int64 handle_index, int32 error_code, xstring error_msg);
		void					repose_client_req(int64 handle_index, const xstring& repose_body);
		void					new_repose_client_req(int64 handle_index, int32 error_code, xstring error_msg);
		uint32					async_request(ui64 uid, xstring& sdk_url, std::vector<xstring>& head_list, const xstring& params, http_accessor::result_handler_type handler, e_http_request_type request_type);
		
		void					remove_http(s_client_uid client_uid);
		void					remove_http(int32 http_count);
	private:
		void					clear();
	private:
		http_access				m_http_map[init_player_max];

	};
}
#endif