/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 21:14
  file base: cell_server
  file ext: cpp
  author: lidongyang

  purpose:
*********************************************************************/
#include "app/app_server.hpp"
#include "base/ecs_world.h"
#include "cell_server.hpp"
#include "connection/daemon_client.hpp"
#include "cross_server_harry_def.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_access_mgr.hpp"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "Lua/script_mgr.h"
#include "net/net_client_mgr.hpp"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "system/play/time_activity_system.h"
#include "tracy/Tracy.hpp"
#include "utility/parse_msg.h"
#include <net/scheduler.hpp>
#include <server_data/server_data.hpp>
#include <time.hpp>
#include "internal/internal.hpp"
#include "utility/globle_data.h"
#include "internet/eye_proto.pb.h"
#include "internet/net.pb.h"
#include "logic/time_limit_activity_temp_mgr.hpp"
namespace faith
{
	using namespace faith::net;
	using namespace faith::utility;

	cell_server::cell_server()
	{
		m_server_id = SERVERCONFIG->game_id;
		m_gm_state = SERVERCONFIG->gm_state;
		m_timer_index_game_loop = scheduler::scheduler_invalid_timer_index;
		m_server_avg_level = 0;
		m_world_level_cur = 0;
		m_world_level_last = 0;
		m_reload_csv = false;
		m_empty_harry_msg.clear_data();
		for (int32 i = e_need_server_cross_none; i < e_need_server_cross_max; i++)
		{
			m_need_begin_cross_server_config[i] = 0;
		}
		memset(m_fep_conn_index, 0, sizeof(m_fep_conn_index));

		for (int32 i = e_server_info_type_begin_cross_server_time; i < e_server_info_type_max; i++)
		{
			m_server_info_arr[i] = 0;
		}
		m_is_close_cs = false;
	}

	cell_server::~cell_server()
	{

	}
	bool cell_server::init()
	{
		xstring ip = CSCONFIG->internal_host;
		int32 port = CSCONFIG->internal_port;

		if (false == net_server_mgr::getInstance().init(e_server_type_cs, ip, port,
			CS_SERVER_SEND_BUFF_SIZE,
			CS_SERVER_RECV_BUFF_SIZE, 
			INTERNAL_SERVER_MAX_PACKET_SIZE, 
			CS_NEED_SERVER_COUNT, 
			CS_NEED_SERVER_COUNT,
			[](const net_server* faith_server_ptr) {
				if (nullptr == faith_server_ptr)
				{
					return;
				}
				switch (faith_server_ptr->get_server_type())
				{
				case e_server_type_fep:
				{
					CONSOLE_INFO("on_conn_closed e_server_type_fep!");
					unit_man::save_all_player(e_logout_result_connect_dis);
					unit_man::remove_all_player();
				}
				break;
				default:
					break;
				}
			}))
		{
			return false;
		}
		return true;
	}
	void cell_server::start()
	{
		net_server_mgr::getInstance().start();
		m_timer_index_game_loop = scheduler::getInstance().add_timer(33, boost::bind(&cell_server::server_loop, this, _1));
	}

	void cell_server::stop()
	{
		if (scheduler::scheduler_invalid_timer_index != m_timer_index_game_loop)
		{
			scheduler::getInstance().remove_timer(m_timer_index_game_loop);
		}
		net_server_mgr::getInstance().stop();
	}

	void cell_server::on_conn_closed(const net_server* faith_server_ptr)
	{
		if (nullptr == faith_server_ptr)
		{
			return;
		}
		switch (faith_server_ptr->get_server_type())
		{
		case e_server_type_fep:
		{
			CONSOLE_INFO("on_conn_closed e_server_type_fep!");
			unit_man::save_all_player(e_logout_result_connect_dis);
			unit_man::remove_all_player();
		}
		break;
		default:
			break;
		}
	}
	void cell_server::send_to_fep(const void* data_ptr, size_t data_len, int32 server_index)
	{
		if (server_index < 0 || server_index >= SERVER_FEP_COUNT)
		{
			return;
		}
		net_server_mgr::getInstance().send_message(data_ptr, data_len, m_fep_conn_index[server_index], e_server_type_fep);
	}
	void cell_server::broadcast(const void* data_ptr, size_t data_len, e_server_type server_type)
	{
		net_server_mgr::getInstance().send_message(data_ptr, data_len, -1, server_type);
	}
	void cell_server::on_req_login(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//存下这台服务器的信息
		const faith::req_login* msg = static_cast<const faith::req_login*>(data_ptr);
		if (NULL == msg)
		{
			return;
		}
		std::cout << msg->server_info.ip_addr << " " << msg->server_info.port << " " << msg->server_info.server_type << " " << msg->server_info.server_index << std::endl;
		if (msg->server_info.server_type >= e_server_type_max)
		{
			return;
		}
		if (msg->server_info.server_index < 0)
		{
			return;
		}
		switch (msg->server_info.server_type)
		{
		case e_server_type_fep:
		{
			if (msg->server_info.server_index > SERVER_FEP_COUNT)
			{
				return;
			}
			m_fep_conn_index[msg->server_info.server_index] = conn_index;
		}
		break;
		default:
			break;
		}
		net_server* faith_server_ptr = net_server_mgr::getInstance().get_peer_by_conn_index(conn_index);
		if (nullptr == faith_server_ptr)
		{
			return;
		}
		faith_server_ptr->set_server_info(msg->server_info);
	}

	void cell_server::on_req_stop(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const faith::req_stop* msg = static_cast<const faith::req_stop*>(data_ptr);
		if (NULL == msg)
		{
			return;
		}
		if (msg->server_type != e_server_type_cs)
		{
			return;
		}
		CONSOLE_INFO("on_req_stop FaithEye Stop Game!");
		if (m_is_close_cs)
		{
			return;
		}
		m_is_close_cs = true;
		//unit_man::save_all_player(e_logout_result_connect_dis);
		//unit_man::remove_all_player();
		daemon_client::getInstance().set_server_close(true);

	}
	void cell_server::server_loop(uint32 timer_index)
	{
		ZoneScoped;
		if (timer_index != m_timer_index_game_loop)
		{
			return;
		}
		static int32 loop_counter = 0;
		static int64 tick_time = get_tick_count();	// 每次场景tick时间(平均值)
		static int64 last_log_time = 0;
		static int64 sync_cs_data = 0;	// 每5分钟打印一些数据
		static int64 time_old = 0;
		++loop_counter;
		static bool is_cs_ok = false;
		if (is_cs_ok == false)
		{
			tick_time = get_tick_count();
			last_log_time = tick_time;
			time_old = tick_time;
			sync_cs_data = tick_time;
			is_cs_ok = true;
			CONSOLE_INFO("cs start ok");
		}
		if (m_reload_csv)
		{
			m_reload_csv = false;
			template_manager::get_instance().init();
			script_mgr::get_instance().call_func(nullptr, "cs_main", 0, false, "");
			unit_man::reload_csv();
			cs_reload_csv_end msg;
			connection_mgr::getInstance().send_to_ws(&msg, sizeof(msg));
		}
		int64 time_now = get_tick_count();
		int32 tick_diff = time_now - time_old;

		g_ecs->tick(tick_diff);

		item_system::heart_tick(time_now);
		unit_man::heart_tick(time_now, tick_diff);
		world_cs::heart_tick(time_now);
		time_activity_system::heart_tick(time_now);

		time_old = time_now;
		if (tick_diff > 1000 || tick_diff < 0)
		{
			//CONSOLE_INFO("cell_server::server_loop tick_diff = " << tick_diff << "thread id ="<< boost::this_thread::get_id());
		}
		static uint32 ticks = 0;
		++ticks;
		if (is_cs_ok && time_now > last_log_time)
		{
			if (m_gm_state)
			{
				CONSOLE_INFO("==========cs gm server status==========");
			}
			else
			{
				CONSOLE_INFO("==========cs server status==========");
			}
			if (globle_data::get_instance().get_version_template_ptr())
			{
				CONSOLE_INFO("res svn code : {}", globle_data::get_instance().get_version_template_ptr()->Version);
			}
			faith::int32 session_count = unit_man::get_player_num();
			faith::int32 robot_count = unit_man::get_robot_num();
			faith::int32 npc_count = unit_man::get_npc_num();
			faith::int32 ws_num = net_client_mgr::getInstance().get_server_count(e_server_type_ws);
			faith::int32 dp_num = net_client_mgr::getInstance().get_server_count(e_server_type_dp);
			faith::int32 fep_num = net_server_mgr::getInstance().get_server_count(e_server_type_fep);
			faith::int32 gate_num = net_client_mgr::getInstance().get_server_count(e_server_type_gate);
			CONSOLE_INFO("ws {}/{} dp {}/{} fep {}/{} gate {}/{}", ws_num, SERVER_WS_COUNT, dp_num, SERVER_DP_COUNT, fep_num, SERVER_FEP_COUNT, gate_num, SERVER_GATE_COUNT);
			CONSOLE_INFO("session num {} session {}", session_count, SERVERCONFIG->init_player_num);
			CONSOLE_INFO("robot num {} ", robot_count);
			CONSOLE_INFO("npc num {} npc max {}", npc_count, init_npc_max);
			CONSOLE_INFO("tick {}", (time_now - tick_time) / ticks);
			last_log_time = time_now + server_console_time;
			tick_time = time_now;
			ticks = 0;
			server_data::get_instance().heart_tick(time_now, "cs", session_count);
			if (npc_count <= 0)
			{
				daemon_client::getInstance().set_server_error_string("npc num is zero");
			}
		}


		if (daemon_client::getInstance().get_server_close())
		{
			// 输出信息
			CONSOLE_INFO("daemon close, all player offline, please shutdown cs ! ! !");
			unit_man::save_all_player(e_logout_result_connect_dis);
			unit_man::remove_all_player();
			stop();
			app_server::getInstance().stop();
			return;
		}
		if (time_now >= sync_cs_data)
		{
			sync_cs_data = time_now + server_player_to_ws;
			faith::app_server_update	req;
			req.player_count = unit_man::get_player_num() - unit_man::get_robot_num();
			req.max_player_count = init_player_max;
			connection_mgr::getInstance().send_to_ws(&req, sizeof(req));
		}
		daemon_client::getInstance().heart_tick(time_now);
		//http_access_mgr::get_instance().tick(time_now);
#ifdef OPEN_LUA_TICK
		script_mgr::get_instance().call_func(nullptr, "UpdateSingleton", 0, false, "%l", time_now);
#endif

	}

	void cell_server::handler_daemon_onrecv(const void* data_ptr, size_t data_len)
	{
		int32 msg_herder = parse_msg::getInstance().get_packet_header(data_ptr, data_len);

		switch (msg_herder)
		{
		case e_msgindex_d2s_close_process:
		{
			close_server(data_ptr, data_len);
		}
		break;
		default:
			break;
		}
	}
	void cell_server::close_server(const void* data_ptr, size_t data_len)
	{
		eye_proto_d2s_close_process request;
		if (!parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len))
		{
			return;
		}
		if (request.server_id() != get_server_id()
			|| request.server_type() != e_server_type_cs)
		{
			return;
		}
		CONSOLE_INFO("eye_proto_d2s_close_process FaithEye Stop Game!");
		if (m_is_close_cs)
		{
			return;
		}
		m_is_close_cs = true;
		//unit_man::save_all_player(e_logout_result_connect_dis);
		//unit_man::remove_all_player();
		daemon_client::getInstance().set_server_close(true);

	}

	void cell_server::set_server_config(const s_game_info& game_info)
	{
		m_game_info_map[game_info.server_id] = game_info;
	}
	void cell_server::remove_server_config(const int32& server_id)
	{
		game_info_map_it it = m_game_info_map.find(server_id);
		if (it != m_game_info_map.end())
		{
			m_game_info_map.erase(it);
		}
	}

	void cell_server::clear_all_server_config()
	{
		m_game_info_map.clear();
	}

	xchar* cell_server::get_server_name(int32 server_id)
	{
		game_info_map_it it = m_game_info_map.find(server_id);
		if (it == m_game_info_map.end())
		{
			return empty_char_array;
		}
		return it->second.server_name;
	}

	void cell_server::set_cur_server_harry_msg(const s_server_harry_msg harry_msg[max_server_num])
	{
		memcpy(m_server_harry_msg, harry_msg, sizeof(m_server_harry_msg));
	}

	s_server_harry_msg& cell_server::get_cur_server_harry_msg()
	{
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg[i].server_id == m_server_id)
			{
				return m_server_harry_msg[i];
			}
		}
		return m_empty_harry_msg;
	}

	s_server_harry_msg& cell_server::get_harry_msg_with_server_id(int32 server_id)
	{
		for (int32 i = 0; i < max_server_num; i++)
		{
			if (m_server_harry_msg[i].server_id == server_id)
			{
				return m_server_harry_msg[i];
			}
		}
		return m_empty_harry_msg;
	}

	void cell_server::dp_connect_cs_success()
	{
		time_limit_activity_temp_mgr::get_instance().init_manager();
	}

	int32 cell_server::get_activity_sec_left(int32 activity_type, e_activity_time_get time_get, bool is_other_time, bool is_cross_city_war)
	{
		bool is_cross = false;
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (_activity_cfg_ptr != nullptr && _activity_cfg_ptr->IsOpen > 0)
		{
			if ((_activity_cfg_ptr->CrossActivityOpenTime.size() > 0 || _activity_cfg_ptr->CrossOpenDate.size() > 0) && get_need_begin_cross_gm_common((e_need_server_cross)_activity_cfg_ptr->ActivityCrossType))
			{
				is_cross = true;
			}
		}
		return init_unit::get_activity_sec_left(activity_type, time_get, is_other_time, is_cross_city_war, is_cross);
	}

	std::vector<int32> cell_server::get_activity_open_time(int32 activity_type)
	{
		std::vector<int32> open_time;
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return open_time;
		}

		bool is_cross = false;
		open_time = _activity_cfg_ptr->ActivityOpenTime;

		if ((_activity_cfg_ptr->CrossActivityOpenTime.size() > 0 || _activity_cfg_ptr->CrossOpenDate.size() > 0) && get_need_begin_cross_gm_common((e_need_server_cross)_activity_cfg_ptr->ActivityCrossType))
		{
			is_cross = true;
		}

		if (is_cross)
		{
			open_time = _activity_cfg_ptr->CrossActivityOpenTime;
		}
		return open_time;
	}

	std::vector<int32> cell_server::get_activity_open_data(int32 activity_type)
	{
		std::vector<int32> open_data;
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return open_data;
		}
		bool is_cross = false;

		if (_activity_cfg_ptr->CrossOpenDate.size() > 0 && get_need_begin_cross_gm_common((e_need_server_cross)_activity_cfg_ptr->ActivityCrossType))
		{
			is_cross = true;
		}

		if (is_cross)
		{
			open_data = _activity_cfg_ptr->CrossOpenDate;
		}
		else
		{
			open_data = _activity_cfg_ptr->OpenDate;
		}

		return open_data;
	}

	void cell_server::set_need_begin_cross_gm_common(e_need_server_cross gm_type, int32 need_cross)
	{
		if (gm_type < e_need_server_cross_begin_cross || gm_type >= e_need_server_cross_max)
		{
			return;
		}
		m_need_begin_cross_server_config[gm_type] = need_cross;
	}

	int32 cell_server::get_need_begin_cross_gm_common(e_need_server_cross gm_type)
	{
		if (gm_type < e_need_server_cross_begin_cross || gm_type >= e_need_server_cross_max)
		{
			return false;
		}
		return m_need_begin_cross_server_config[gm_type];
	}
	bool cell_server::get_is_cross_sever(int32 server_id)
	{
		if (get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) > 0 && server_id == get_need_begin_cross_gm_common(e_need_server_cross_begin_cross))
		{
			return true;
		}
		return false;
	}
	bool cell_server::get_is_self_gate_server()
	{
		return get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) == 0 || get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) == m_server_id;
	}
	bool cell_server::get_is_self_server(int32 server_id)
	{
		return server_id == m_server_id;
	}
	bool cell_server::is_cross_cs()
	{
		return get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) == m_server_id;
	}
	void cell_server::set_open_time(int32 open_time)
	{
		CONSOLE_INFO("set_open_time open_time:{}", open_time);
		m_open_time = open_time;
		globle_data::get_instance().init_server_open_time(open_time);
		time_activity_system::start_up();
	}

	void cell_server::set_server_info_arr(int32 server_info_type, int32 server_info_value)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			return;
		}
		m_server_info_arr[server_info_type] = server_info_value;
	}

	int32 cell_server::get_server_info_arr(int32 server_info_type)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			return 0;
		}
		return m_server_info_arr[server_info_type];
	}

	int32 cell_server::get_server_avg_level()
	{
		int32 avg_level = m_server_avg_level >= m_world_level_last ? m_server_avg_level : m_world_level_last;
		return avg_level;
	}
	}
