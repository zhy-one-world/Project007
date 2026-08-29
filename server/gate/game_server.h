/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2019-07-20
	
	File Name :	game_server.h
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

#ifndef _GAME_SERVER_H_
#define _GAME_SERVER_H_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
//#include <db_config.hpp>
#include "server_def.hpp"
#include "Logic/gate_def.hpp"

//////////////////////////////////////////////////////////////////////////
//
//	Class Declare
//
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	class game_server
	{
	public:
		game_server();
		~game_server();
	public:
		void	clear_data();
	public:
		bool is_game_run();
		const int32&	get_server_id() { return m_game_info.server_id; }
		const int32&	get_cross_id() { return m_game_info.cross_id; }
		void set_game_info(const s_game_info& game_info);
		void set_server_index(e_server_type server_type, int32 server_index, int32 conn_index);
		void set_server_close(int32 conn_index);
		const s_game_info& get_game_info() { return m_game_info; };
	public:
		void send_message(const void *data_ptr, size_t data_len, e_server_type server_type, int32 server_index = 0);
		void send_message_all(const void *data_ptr, size_t data_len);
	private:
		int32				m_cs_conn_index[SERVER_CS_COUNT];
		int32				m_dp_conn_index;
		int32				m_ws_conn_index;
		s_game_info			m_game_info;
	};
}


#endif