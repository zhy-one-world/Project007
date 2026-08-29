/********************************************************************
  created: 2014/08/09
  created: 9:8:2014 11:48
  file base: http_send
  file ext: hpp
  author: YU REN
  
  purpose: 
*********************************************************************/
#ifndef _HTTP_SEND_
#define _HTTP_SEND_
#include "server_def.hpp"
namespace faith
{
	struct server_info_gm;
	struct s_unit_info;

	class http_send
	{
	public:
		static http_send&	get_instance();
	public:
		void send_http_handle(ui64 http_id, uint32 http_error_code, const xstring& http_error_info, const xstring& http_result);
		void send_role_count(xstring app_key, int32 server_id, time_cout_map& time_count);
		void send_role_level(xstring app_key, xstring channel_id, xstring lang, xstring account, int32 server_id, guid_64 role_guid, int32 exp_level);
		void send_server_state(const server_info_gm& srv_info, int32 player_num);
		void send_role_info(const s_unit_info& role_info);

	};
}
#endif