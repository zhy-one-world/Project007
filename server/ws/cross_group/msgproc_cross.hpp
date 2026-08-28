/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-5-25 16:34
	
	File Name :	msgproc_gate.hpp
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

#ifndef _MSGPROC_GATE_HPP_
#define _MSGPROC_GATE_HPP_
#include "logic/type_def.hpp"
namespace hld
{
	// gate msg
	void gate2ws_rep_register(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_rep_off_line(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_rep_gate_new(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_rep_from_ws(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_change_server_id_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_this_cross_all_server_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_end_refresh_server_list_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_set_server_act_hide_func(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_reload_csv(uint32 conn_index, const void* data_ptr, size_t data_len);
	//gate
	void gate2ws_rep_sign_up(uint32 conn_index, const void* data_ptr, size_t data_len);
	void gate2ws_rep_join_war(uint32 conn_index, const void* data_ptr, size_t data_len);
	//cs
	void cs2ws_req_role_pk_result(uint32 conn_index, const void* data_ptr, size_t data_len);

	//client
	void c2ws_req_cross_info(uint32 conn_index, const void* data_ptr, size_t data_len);

	void c2ws_req_sign_up(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_req_role_pk_log(uint32 conn_index, const void* data_ptr, size_t data_len);
	//dp
	void dp2ws_req_role_pk_top(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_req_role_pk_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void dp2ws_req_role_pk_log(uint32 conn_index, const void* data_ptr, size_t data_len);

	//cross_ladder
	void c2ws_cross_ladder_req_role_info(uint32 conn_index, const void* data_ptr, size_t data_len);
	void c2ws_cross_ladder_req_sign_up(uint32 conn_index, const void* data_ptr, size_t data_len);

	void c2ws_element_war_req_role_info(uint32 conn_index, const void* data_ptr, size_t data_len);
}

#endif