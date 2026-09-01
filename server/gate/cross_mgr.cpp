/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2019-07-20
	
	File Name :	cross_mgr.cpp
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include "cross_mgr.h"
#include "server_log.hpp"

//////////////////////////////////////////////////////////////////////////
//	Class Implement
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	cross_mgr::cross_mgr() 
	{
		clear_data();
	}

	cross_mgr::~cross_mgr()
	{
	}
	void cross_mgr::clear_data()
	{
		m_cross_id = 0;
		m_game_server_map.clear();
	}
	game_server* cross_mgr::get_game_server(int32 server_id)
	{
		game_server_map_it it = m_game_server_map.find(server_id);
		if (it == m_game_server_map.end())
		{
			return nullptr;
		}
		return it->second;
	}
	void cross_mgr::get_this_cross_all_server(s_game_info* server_info_arr, int32& num)
	{
		num = 0;
		game_server_map_it it = m_game_server_map.begin();
		for (it; it != m_game_server_map.end(); it++)
		{
			if (nullptr != it->second)
			{
				server_info_arr[num] = it->second->get_game_info();
				num++;
			}
		}
	}
	void cross_mgr::add_game_server(game_server* game_server_ptr)
	{
		if (nullptr == game_server_ptr)
		{
			return;
		}
		m_game_server_map[game_server_ptr->get_server_id()] = game_server_ptr;
	}
	void cross_mgr::del_game_server(int32 server_id)
	{
		game_server_map_it it = m_game_server_map.find(server_id);
		if (it != m_game_server_map.end())
		{
			m_game_server_map.erase(it);
		}
	}
	void cross_mgr::broad_cast(const void *data_ptr, size_t data_len, int32 out_server_id)
	{
		for (game_server_map_it it = m_game_server_map.begin(); it != m_game_server_map.end(); ++it)
		{
			if (it->second->get_server_id() != out_server_id)
			{
				it->second->send_message(data_ptr, data_len, e_server_type_ws);
			}
		}
	}
}