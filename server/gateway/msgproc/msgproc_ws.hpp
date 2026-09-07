/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   11:53
	file base:	msgproc_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _GATEWAY_MSGPROC_WS_H_
#define _GATEWAY_MSGPROC_WS_H_

#include "logic/type_def.hpp"

namespace faith
{
	void ws2gateway_broadcast_msg_fun(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2gateway_client_login_queue_status(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2gateway_client_loginned(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2gateway_reconnect_game_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2gateway_enter_game_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2gateway_kickout_account_func(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
