/********************************************************************
  created: 2014/07/12
  created: 12:7:2014 15:55
  file base: msgproc_client
  file ext: hpp
  author: wangpengqing
  
  purpose: 客户端到服务器的请求处理
*********************************************************************/
#ifndef _GATEWAY_MSGPROC_CLIENT_H_
#define _GATEWAY_MSGPROC_CLIENT_H_

#include "../server/client_session.hpp"

namespace faith
{
	void c2gateway_logout(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2gateway_create_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2gateway_enum_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2gateway_select_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2gateway_del_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2gateway_ping(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2ls_req_login_reconnect(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2ls_req_login(uint32 array_index,const void *data_ptr,size_t data_len);

//////////////////////////////////////////////////////////////////////////////////////////////////////
	void c2ws_token_login(uint32 array_index, const void *data_ptr, size_t data_len);
}

#endif
