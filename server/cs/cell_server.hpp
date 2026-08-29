/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 11:47
  file base: cell_server
  file ext: hpp
  author: lidongyang
  
  purpose: 
*********************************************************************/
#ifndef _CELL_SERVER_HPP_
#define _CELL_SERVER_HPP_
/************************************************************************/
/*                           File Include                               */
/************************************************************************/
#include <singleton.hpp>
#include "server_def.hpp"
#include <net/net_server_mgr.hpp>
#include "Logic/char_def.hpp"
#include "Logic/gate_def.hpp"
#include "./connection_mgr.hpp"
#include <cross_server_harry_def.hpp>
#include <activity_def.hpp>
/************************************************************************/
/*                            Maroc Define                              */
/************************************************************************/
namespace faith
{
//#define OPEN_LUA_DATA
//#define OPEN_LUA_TICK
	class cell_server : public faith::singleton<cell_server>
	{
		friend class faith::singleton<cell_server>;
	protected:
		cell_server();
	public:
		virtual ~cell_server();
	public:
		bool init();
		void start();
		void stop();
		void on_req_login(uint32 connindex, const void *data_ptr, size_t data_len);
		void on_req_stop(uint32 connindex, const void *data_ptr, size_t data_len);
		void send_to_fep(const void* data_ptr, size_t data_len, int32 server_index);
		void broadcast(const void* data_ptr, size_t data_len, e_server_type server_type);
	protected:
		void	on_conn_closed(const net_server* faith_server_ptr);
		void	server_loop(uint32 timer_index);
	public:
		int32 get_server_id() { return m_server_id; }
		bool get_gm_state() { return m_gm_state; }
		void set_open_time(int32 open_time);
		bool get_is_cross_sever(int32 server_id);
		bool get_is_self_gate_server();
		bool get_is_self_server(int32 server_id);
		bool is_cross_cs();

		void	set_need_begin_cross_gm_common(e_need_server_cross gm_type, int32 need_cross);
		int32	get_need_begin_cross_gm_common(e_need_server_cross gm_type);

		void	set_server_info_arr(int32 server_info_type, int32 server_info_value);
		int32	get_server_info_arr(int32 server_info_type);
		void	set_server_avg_level(int32 lv) { m_server_avg_level = lv; }
		int32	get_server_avg_level();
		void	set_world_level_cur(int32 lv) { m_world_level_cur = lv; }
		int32	get_world_level_cur() { return m_world_level_cur; }
		void	set_world_level_last(int32 lv) { m_world_level_last = lv; }
		int32	get_world_level_last() { return m_world_level_last; }

		void	set_server_config(const s_game_info& game_info);
		void	remove_server_config(const int32& server_id);
		void	clear_all_server_config();
		xchar*	get_server_name(int32 server_id);

		void	set_reload_csv() { m_reload_csv = true; }
		void	handler_daemon_onrecv(const void* data_ptr, size_t data_len);
		void	close_server(const void* data_ptr, size_t data_len);

	public:
		void	set_cur_server_harry_msg(const s_server_harry_msg harry_msg[max_server_num]);
		s_server_harry_msg& get_cur_server_harry_msg();
		s_server_harry_msg& get_harry_msg_with_server_id(int32 server_id);

		void	set_world_boss_guid(guid_64 npc_guid) { m_cur_world_boss_guid = npc_guid; }
		guid_64	get_world_boss_guid() { return m_cur_world_boss_guid; }

		void	dp_connect_cs_success();//dp连接上cs以后的函数
		int32	get_server_open_time() { return m_open_time; }

		int32 get_activity_sec_left(int32 activity_type, e_activity_time_get time_get, bool is_other_time = false, bool is_cross_city_war = false);
		std::vector<int32> get_activity_open_time(int32 activity_type);	//活动开启时间
		std::vector<int32> get_activity_open_data(int32 activity_type);	//活动开启日期

		void set_ladder_world_level(int64 world_level) { m_ladder_world_level = world_level; }
		int64 get_ladder_world_level() { return m_ladder_world_level; }
	protected:
		uint32	m_timer_index_game_loop;
		bool m_gm_state;
		int32 m_server_id;
		int32 m_open_time;
		int32	m_server_avg_level;
		int32	m_world_level_cur;
		int32	m_world_level_last;
		bool	m_reload_csv;
		int32	m_fep_conn_index[SERVER_FEP_COUNT];
		game_info_map m_game_info_map;
		int64	m_next_daemon_time;

		int64	m_ladder_world_level;

		s_server_harry_msg	m_server_harry_msg[max_server_num];
		s_server_harry_msg	m_empty_harry_msg;

		guid_64 m_cur_world_boss_guid;

		int32	m_need_begin_cross_server_config[e_need_server_cross_max];

		int32	m_server_info_arr[e_server_info_type_max];

		bool	m_is_close_cs;//防止结束进程时多次析构导致的崩溃
	};
}

#endif
