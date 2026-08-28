/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   11:53
	file base:	msgproc_ws
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _FEP_MSGPROC_WS_H_
#define _FEP_MSGPROC_WS_H_

#include "logic/type_def.hpp"

namespace hld
{
	void ws2fep_broadcast_msg_fun(uint32 connindex, const void *data_ptr, size_t data_len);
	void ws2fep_client_login_queue_status(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2fep_client_loginned(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2fep_reconnect_game_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2fep_enter_game_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2fep_kickout_account_func(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2fep_recv_act_limit_temp(uint32 connindex, const void* data_ptr, size_t data_len);
	void ws2fep_recv_act_limit_branch_temp(uint32 connindex, const void* data_ptr, size_t data_len);
}

#endif
