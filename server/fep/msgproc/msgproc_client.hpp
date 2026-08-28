/********************************************************************
  created: 2014/07/12
  created: 12:7:2014 15:55
  file base: msgproc_client
  file ext: hpp
  author: wangpengqing
  
  purpose: 客户端到服务器的请求处理
*********************************************************************/
#ifndef _FEP_MSGPROC_CLIENT_H_
#define _FEP_MSGPROC_CLIENT_H_

#include "../server/client_session.hpp"

namespace hld
{
	void c2fep_logout(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2fep_create_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2fep_enum_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2fep_select_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2fep_del_character(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2fep_ping(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2ls_req_login_reconnect(uint32 array_index, const void *data_ptr, size_t data_len);
	void c2ls_req_login(uint32 array_index,const void *data_ptr,size_t data_len);

//////////////////////////////////////////////////////////////////////////////////////////////////////
	void c2ws_token_login(uint32 array_index, const void *data_ptr, size_t data_len);
}

#endif
