/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   15:02
	file base:	world_server
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WORLD_SERVER_H_
#define _WORLD_SERVER_H_

#include <singleton.hpp>
#include "server_def.hpp"
#include <logic/time_def.hpp>
#include <logic/char_def.hpp>
#include <logic/activity_def.hpp>
#include <net/net_server_mgr.hpp>
#include "net/http_def.hpp"
#include "server/client_session_mgr.hpp"
#include "cross_group/gate_proxy.hpp"

namespace faith
{
	using namespace net;
//#define OPEN_LUA_TICK
	/************************************************************************/
	/*							world server                                */
	/************************************************************************/
	class world_server :	public singleton<world_server>
	{
		friend class singleton<world_server>;
	public:
		enum
		{
			e_gameloop_interval = 50,
		};
	private:
		world_server();
	public:
		virtual ~world_server();
	public:
		bool	init();
		bool	start();
		void	stop();
		void	handler_onclose(const net_server* faith_server_ptr);
		void	set_server_config(const s_game_info& game_info);
		void	set_gate_server_config(const s_game_info& game_info);
		void	remove_server_config(const int32& server_id);
		void	clear_all_server_config();
		void	send_server_config_to_all_cs(int32 conn_index = -1);
		void	send_server_config_to_all_cs(const s_game_info& game_info, bool is_add, int32 conn_index = -1);
		void	send_server_config_all_to_client(client_session* session);
		void	send_server_config_to_db();
		bool	set_begin_time(const int32 begin_time);

		void	set_need_begin_cross_gm_common(const int32 need_cross, e_need_server_cross gm_type);
		int32	get_need_begin_cross_gm_common(e_need_server_cross gm_type);

		int32	get_gate_server_id() { return GATECONFIG->gate_id; };

		void	set_server_info_arr(const int32 server_info_value, int32 server_info_type);
		int32	get_server_info_arr(int32 server_info_type);
		void	send_begin_cross_gm_to_cs(e_need_server_cross gm_type);
		void	send_begin_cross_gm_to_client(e_need_server_cross gm_type);
		void	save_server_info_to_db(int32 server_info_type, s_daemon_callback_info callback_info = s_daemon_callback_info());
		void	send_server_info_arr_to_cs(int32 server_info_type);
		void	send_all_server_info_to_cs();

		void	send_need_begin_cross_pk_to_client(client_session* session);
		bool	is_sky_island_server() { return m_server_id == get_cross_id(); };
		int32	get_server_group_id	(){ return m_server_id % server_group_level_num; }
		int32	get_big_group_id(){ return m_server_id/ server_group_level_num; }
		int32	get_server_id() { return m_server_id; }//服务器的server_id
		xstring&	get_out_ip() { return m_out_ip; }//外网ip
		int32	get_out_port() { return m_out_port; }//外网端口
		xstring&	get_server_name() { return m_server_name; }//服务器的名称
		int32	get_cross_id() {return m_need_begin_cross_config[e_need_server_cross_begin_cross];}
		const int32 get_begin_time() { return m_begin_time; }
		bool	get_gm_state() { return m_gm_state; }
		void	tick(const uint64& tick_time);
		bool	is_need_refresh_ws_daily_info_at_zero_hour();
		bool	is_need_refresh_ws_daily_info_at_week_time();
		bool	is_need_refresh_ws_daily_info_at_week_twenty_three_time();
		
		int64	get_next_week_refresh_time();
		int64	get_next_week_twenty_three_refresh_time();
		bool	is_need_refresh_ws_daily_info_at_clear_city_bid_time();
		int64	get_next_clear_city_bid_time();
		void	set_server_time_zero(int64 server_time)  { m_server_time_zero = server_time; }
		void	set_server_time_five(int64 server_time)  { m_server_time_five = server_time; }
		int64	get_next_refresh_time(srv_refresh_type time_type);
		void	refresh_day_for_zero(const uint64& tick_time);
		void	refresh_day_for_five();
		void	load_server_attr_val();
		void	save_server_attr_val_db(server_attr_val_type _type, int64 _val);
		void	handler_daemon_onrecv(const void* data_ptr, size_t data_len);
		void	close_server(const void* data_ptr, size_t data_len);
		void	gm_order_from_daemon(const void* data_ptr, size_t data_len);
		s_game_info& get_server_config(int32 server_id) { return m_cross_info_map[server_id]; };
		int32	get_server_num() { return m_cross_info_map.size(); };
		s_game_info* get_server_config_by_server_id(int32 server_id);
		s_game_info& get_gate_server_config(int32 server_id) { return m_gate_cross_info_map[server_id]; };
		int32	get_gate_server_num() { return m_gate_cross_info_map.size(); };
		s_game_info* get_gate_server_config_by_server_id(int32 server_id);
		game_info_map& get_server_list() { return m_cross_info_map; };
		void	send_need_begin_cross(int32 conn_index = -1);
		void	get_server_id_arr(std::set<int32>& server_id_arr);

		void	set_clear_bid_time(int64 refresh_time) { m_server_time_clear_city_war_bid = refresh_time; };
		void	set_week_refresh_time(int64 refresh_time) { m_server_time_week_constribution_refresh = refresh_time; };
		void	set_week_twenty_three_refresh_time(int64 refresh_time) { m_server_time_week_legion_welfare_refresh = refresh_time; };
		void	set_refresh_harry_time(int64 refresh_time) { m_server_time_refresh_harry = refresh_time; };
		void	set_ladder_world_level(int64 world_level, bool need_send_to_cs = false, bool need_save = false);
		int64	get_ladder_world_level() { return m_server_ladder_wrold_level; };
		int64	get_next_harry_refresh_time();
		void	refresh_for_harry_time();
	public:
		void	set_cross_server_open_time(int32 cross_time, bool is_from_other_server);
		void	set_cross_server_open_day();
		void	open_act_with_cross_server_open_day();//根据跨服时间解锁活动
		void	save_gm_common_to_db(int64 req_http_id = 0);
		void	reload_csv_func();
		void	internal_req_login(uint32 conn_index, const void* data_ptr, size_t data_len);
		void	internal_appserver_dataupdate(uint32 conn_index, const void* data_ptr, size_t data_len);

		void	set_flag_when_one_min_start();//一分钟后如果flag有问题，则强制执行
		void	end_refresh_server_list();//根据当前server_list处理需要用到server_list的逻辑
		bool	is_have_this_server(int32 server_id);
	public:
		void	send_by_uid(int32 conn_index, const void* data_ptr, size_t data_len);
		void    send_by_uid_lua(int32 conn_index, google::protobuf::Message* net_pro, uint32 header);
		void	send_to_fep(int32 server_index, const void* data_ptr, size_t data_len);
		void	send_to_fep_all(const void* data_ptr, size_t data_len);
		void	send_to_cs(int32 conn_index, const void* data_ptr, size_t data_len);
		void	send_to_cs(const void* data_ptr, size_t data_len);
		void	broadcast(const void* data_ptr, size_t data_len, e_server_type server_type = e_server_type_invalid);
		void    broadcast_lua(google::protobuf::Message* net_pro, uint32 header, e_server_type server_type = e_server_type_invalid);
		void	send_message_out(const void* data_ptr, size_t data_len, int32 conn_index = -1, e_server_type server_type = e_server_type_invalid);

		void	func_when_add_other_server(int32 server_id);//当有其他服务器加入跨服组时执行的函数
		void	func_when_remove_other_server(int32 server_id);//当有其他服务器移除跨服组时执行的函数
		void	func_when_change_gate_server(int32 server_id);//当跨服组重新调整时执行的函数
		void	set_ws_loading_flag(int64 flag);
		bool	is_ws_all_load_end() { return m_ws_loading_flag == 0; };//是否全部加载完
		bool	is_loading_flag_finish(int64 target_loading_flag);//单个类型是否加载完

		void	send_msg_to_cs_with_load_end();//load结束发cs信息
		void	send_to_gate_get_msg(int64 flag);//请求gate数据
		void	send_to_gate_get_all_msg_with_change_gate_server();

		void	check_send_get_gate_msg(const uint64& tick_time);
		void	refresh_server_cross_time();

		void	send_msg_when_gate_register(int64 flag);

		void	send_need_server_cross_to_other_server(int32 server_id = 0);
		void	send_server_info_by_type_to_other_server(e_server_info_type server_info_type, int32 server_id = 0);
		void	gate_server_func_when_game_info_load_end();

		void	set_last_server_list_from_db(const s_last_server_list_db& last_server_list);
		void	refresh_last_server_list_with_now();
		void	save_last_sever_list_to_db();
		bool	is_add_new_server_id();
		void	load_last_server_list_from_db();
		void	send_to_gate_with_flag_load_end();
		bool	is_same_server_list();

		void	send_server_act_hidden_to_client(client_session* session);
		void	set_server_act_hidden(bool is_need_hidden, bool is_other_server);

		int32 get_activity_sec_left(int32 activity_type, e_activity_time_get time_get, bool is_other_time = false, bool is_cross_city_war = false);
		std::vector<int32> get_activity_open_time(int32 activity_type);	//活动开启时间
		std::vector<int32> get_activity_open_data(int32 activity_type);	//活动开启日期

		void	set_cs_reload_csv_end() { m_is_begin_cs_reload_csv = false; };
	private:
		void	server_loop				( uint32 timer_index );
		void	on_open_time_changed();
		void	send_refresh_to_cs();
	private:
		bool			m_gm_state;
		int32			m_server_id;
		xstring			m_out_ip;
		int32			m_out_port;
		xstring			m_ws_ip;
		int32			m_ws_port;
		xstring			m_server_name;
		game_info_map	m_cross_info_map;
		game_info_map	m_gate_cross_info_map;
		bool			m_world_server_start;
		uint32			m_timerindex_gameloop;
		int32			m_begin_time;
		int64			m_server_time_five;
		int64			m_server_time_zero;
		int64			m_server_time_refresh_harry;//跨服掠夺刷新
		int64			m_server_time_week_constribution_refresh;//军团周贡献刷新时间戳,周1  0点时间戳，可以通用 (暂时没有使用)
		int64			m_server_time_week_legion_welfare_refresh;//军团周贡献刷新时间戳,军福利发放时间戳 周日  23点时间戳，可以通用
		int64			m_server_time_clear_city_war_bid;//城战清零时间戳
		int64			m_server_ladder_wrold_level;
		int32			m_fep_conn_index[SERVER_FEP_COUNT];
		bool			m_is_load_cross_act_end;
		int32			m_need_begin_cross_config[e_need_server_cross_max];//是否开启跨服对应配置

		int32			m_server_info_arr[e_server_info_type_max];//服务器需求数据

		int32			m_open_cross_server_days;//开启跨服具体天数
		bool			m_is_set_cross_time;//是否设置过开跨服时间
		int64			m_ws_loading_flag;					//ws数据加载标志位
		bool			m_need_get_big_player_gate_msg;		//需要加载gate服名人堂数据
		std::set<int64>	m_need_send_gate_flag_arr;		//需要补发往gate的数据
		std::vector<int32> m_last_server_list_server_id;//上次服务器拥有哪些ID
		time_cout_map	m_channel_count;
		bool			m_server_act_hidden;//特殊逻辑
		bool			m_is_begin_reload_csv;//是否在热更，热更时设置true，下一个tick改为false并触发逻辑
		bool			m_is_begin_cs_reload_csv;//是否在热更cs的csv
	};
}

#endif
