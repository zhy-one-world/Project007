/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2019-07-20
	
	File Name :	game_server.cpp
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include "game_server.h"
#include "net/net_server_mgr.hpp"
#include "game_mgr.h"

//////////////////////////////////////////////////////////////////////////
//	Class Implement
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	game_server::game_server()
	{
		clear_data();
	}

	game_server::~game_server()
	{
	}
	void game_server::clear_data()
	{
		memset(m_cs_conn_index, 0, sizeof(m_cs_conn_index));
		for (int32 i =0; i < SERVER_CS_COUNT; ++i)
		{
			m_cs_conn_index[i] = -1;
		}
		m_dp_conn_index = -1;
		m_ws_conn_index = -1;
		m_game_info.clear_data();
	}
	bool game_server::is_game_run()
	{
		if (m_dp_conn_index < 0)
		{
			return false;
		}
		if (m_ws_conn_index < 0)
		{
			return false;
		}	
		for (int32 i = 0; i < SERVER_CS_COUNT; ++i)
		{
			if (m_cs_conn_index[i] < 0)
			{
				return false;
			}
		}
		return true;
	}
	void game_server::set_game_info(const s_game_info& game_info)
	{
		m_game_info = game_info;
	}
	void game_server::set_server_index(e_server_type server_type, int32 server_index, int32 conn_index)
	{
		switch (server_type)
		{
		case faith::e_server_type_dp:
			m_dp_conn_index = conn_index;
			break;
		case faith::e_server_type_ws:
			m_ws_conn_index = conn_index;
			break;
		case faith::e_server_type_cs:
		{
			if (server_index >= SERVER_CS_COUNT)
			{
				return;
			}
			m_cs_conn_index[server_index] = conn_index;
		}
		break;
		default:
			break;
		}
	}
	void game_server::set_server_close(int32 conn_index)
	{
		if (conn_index == m_dp_conn_index)
		{
			m_dp_conn_index = -1;
			return;
		}
		if (conn_index == m_ws_conn_index)
		{
			m_ws_conn_index = -1;
			return;
		}
		for (int32 i = 0; i < SERVER_CS_COUNT; ++i)
		{
			if (m_cs_conn_index[i] == conn_index)
			{
				m_cs_conn_index[i] = -1;
				return;
			}
		}
	}
	void game_server::send_message(const void *data_ptr, size_t data_len, e_server_type server_type, int32 server_index)
	{
		switch (server_type)
		{
		case faith::e_server_type_dp:
		{
			net_server_mgr::getInstance().send_message(data_ptr, data_len, m_dp_conn_index);
		}
			break;
		case faith::e_server_type_ws:
		{
			net_server_mgr::getInstance().send_message(data_ptr, data_len, m_ws_conn_index);
		}
			break;
		case faith::e_server_type_cs:
		{
			if (server_index >= SERVER_CS_COUNT)
			{
				return;
			}
			net_server_mgr::getInstance().send_message(data_ptr, data_len, m_cs_conn_index[server_index]);
		}
		break;
		case faith::e_server_type_gate:
		{
			if (server_index == 0)
			{
				cross_mgr* cross_mgr_ptr = game_mgr::getInstance().get_cross_server(m_game_info.cross_id);
				if (cross_mgr_ptr)
				{
					cross_mgr_ptr->broad_cast(data_ptr, data_len);
				}
			}
			else
			{
				net_server_mgr::getInstance().send_message(data_ptr, data_len, m_ws_conn_index);
			}
		}
		break;
		default:
			break;
		}
	}
	void game_server::send_message_all(const void *data_ptr, size_t data_len)
	{
		net_server_mgr::getInstance().send_message(data_ptr, data_len, m_dp_conn_index);
		net_server_mgr::getInstance().send_message(data_ptr, data_len, m_ws_conn_index);
		for (int32 i = 0; i < SERVER_CS_COUNT; ++i)
		{
			net_server_mgr::getInstance().send_message(data_ptr, data_len, m_cs_conn_index[i]);
		}
	}
}