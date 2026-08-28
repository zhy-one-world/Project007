/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   14:59
	file base:	msgproc_fep
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WS_MSGPROC_FEP_H_
#define _WS_MSGPROC_FEP_H_

// #include "../server/client_session.hpp"
 #include "logic/type_def.hpp"

namespace hld
{
	void fep2ls_request_client_login(uint32 conn_index, const void* data_ptr, size_t data_len);
	void fep2ls_enum_character_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void fep2ls_create_character_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void fep2ls_delete_character_func(uint32 conn_index, const void* data_ptr, size_t data_len);

	void fep2ws_client_reconnect_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void fep2ws_client_loginned(uint32 conn_index, const void* data_ptr, size_t data_len);
	void fep2ws_client_leave_queued(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_enter_game(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_enter_scene(uint32 conn_index, const void* data_ptr, size_t data_len);
	void fep2ws_req_logout_client(uint32 conn_index, const void* data_ptr, size_t data_len);

	void c2ws_send_recruit_proc(uint32 conn_index, const void* data_ptr, size_t data_len);

	void c2ws_recharge_success_by_sdk_proc(uint32 conn_index, const void* data_ptr, size_t data_len);
}


#endif
