/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2019-07-20
	
	File Name :	game_mgr.h
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

#ifndef _GAME_MGR_H_
#define _GAME_MGR_H_

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include <common/singleton.hpp>
#include <net/net_server_mgr.hpp>
#include "cross_mgr.h"

using namespace faith::net;

//////////////////////////////////////////////////////////////////////////
//	Class Declare
//////////////////////////////////////////////////////////////////////////
namespace faith
{
	typedef std::map<int32, cross_mgr*> cross_mgr_map;
	typedef cross_mgr_map::iterator cross_mgr_map_it;
	class game_mgr : public faith::singleton<game_mgr>
	{
		friend class faith::singleton<game_mgr>;
		game_mgr();
	public:
		~game_mgr();

	public:
		bool init();
		bool start();
		void stop();
		void server_loop(uint32 timer_index);
		void send_to_server(const void *data_ptr, size_t data_len, int32 server_id, e_server_type server_type, int32 server_index = 0);
		void send_to_cross(const void *data_ptr, size_t data_len, int32 cross_id);
		void broad_cast(const void *data_ptr, size_t data_len, int32 server_id = 0, e_server_type server_type = e_server_type_ws);
	private:
		void on_conn_closed(const net_server* faith_server_ptr);
		void on_req_login(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_req_stop(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_server_register(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_data_received(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_server_change_cross_id(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_server_flag_load_end(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_server_act_hide_end(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_server_need_reload_csv(unsigned int connindex, const void *data_ptr, size_t data_len);
		void on_server_reload_csv_one_server_end(unsigned int connindex, const void *data_ptr, size_t data_len);
		void last_server_reload_csv_all();
		bool is_all_server_flag_load_end();
		void del_server_flag_with_server_close(int32 server_id);
		void send_all_server_end_refresh_list();
	public:
		cross_mgr* get_cross_server(int32 cross_id);
		cross_mgr* add_cross_server(int32 cross_id);
		void del_cross_server(int32 cross_id);
		game_server* get_game_server(int32 server_id);
		game_server* get_game_server_by_conn(uint32 conn_index);
		game_server* add_game_server(const s_game_info& game_info, e_server_type server_type, int32 server_index, uint32 conn_index);
		void del_game_server(uint32 conn_index);
		void set_all_server_id(int32 server_id);
	public:
		void HandlerDaemonOnrecv(const void* data_ptr, size_t data_len);
		void close_server(const void* data_ptr, size_t data_len);
	private:
		uint32 m_timer_index_game_loop;
		cross_mgr_map m_cross_mgr_map;
		game_server_map	m_game_server_map;
		game_server_map	m_conn_server_map;
		std::map<int32, int32> m_all_server_flag_end_map;
		std::set<int32> m_all_server_id_arr;

		std::map<std::string, std::vector<int32>> m_all_server_msg_with_ip;//根据IP信息划分不同服务器，用于处理reloadcsv依次加载
		std::map<std::string, std::vector<int32>> m_cur_server_msg_with_ip;//当次剩余服务器
		int64	m_begin_reload_csv_time;

		int32	m_gate_server_id;
	};

}
#endif