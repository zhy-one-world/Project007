/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2019-07-20
	
	File Name :	cross_mgr.h
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

#ifndef _CROSS_MGR_H_
#define _CROSS_MGR_H_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "server_def.hpp"
#include "game_server.h"

//////////////////////////////////////////////////////////////////////////
//
//	Class Declare
//
//////////////////////////////////////////////////////////////////////////
namespace hld
{
	typedef std::map<int32, game_server*> game_server_map;
	typedef game_server_map::iterator game_server_map_it;
	class cross_mgr
	{
	public:
		cross_mgr();
		~cross_mgr();
	public:
		void	clear_data();
	public:
		void set_cross_id(int32 cross_id) { m_cross_id = cross_id; }
		int32 get_cross_id() { return m_cross_id; }
		game_server* get_game_server(int32 server_id);
		void add_game_server(game_server* game_server_ptr);
		void del_game_server(int32 server_id);
		void get_this_cross_all_server(s_game_info* server_info_arr, int32& num);
	public:
		void broad_cast(const void *data_ptr, size_t data_len, int32 out_server_id = 0);
	private:
		int32				m_cross_id;
		game_server_map		m_game_server_map;
	};
}


#endif