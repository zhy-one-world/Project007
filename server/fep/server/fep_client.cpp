/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:31
	file base:	fep_client
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "fep_client.hpp"
#include <net/scheduler.hpp>
#include <internal.hpp>
#include "proxy_service_cli.hpp"
#include "game_cfg/servers_config.h"
#include "app/app_server.hpp"
#include "server_log.hpp"
#include "Logic/time_def.hpp"
#include "connection/daemon_client.hpp"
#include "utility/parse_msg.h"
#include "http/http_access_mgr.hpp"
#include "csv_synchronization_fep_mgr.h"
#include "net.pb.h"
#include "eye_proto.pb.h"

namespace faith
{
	using namespace net;


	fep_client::fep_client()
	{
		m_ws_ci = tcp_client::invalid_conn_index;
		m_cs_ci = tcp_client::invalid_conn_index;
		m_timerindex_gameloop = scheduler::scheduler_invalid_timer_index;

		xstring ws_ip = WSCONFIG->internal_host;
		memcpy(m_ws_info.ip_addr, ws_ip.c_str(), ws_ip.size());
		m_ws_info.port = WSCONFIG->internal_port;
		m_ws_info.server_type = e_server_type_ws;

		xstring cs_ip = CSCONFIG->internal_host;
		memcpy(m_cs_info.ip_addr, cs_ip.c_str(), cs_ip.size());
		m_cs_info.port = CSCONFIG->internal_port;
		m_cs_info.server_type = e_server_type_cs;

		m_gm_state = SERVERCONFIG->gm_state;
		m_cross_server_id = 0;
		m_open_time = 0;
	}

	fep_client::~fep_client()
	{

	}

	void fep_client::start()
	{
		net_client_mgr::getInstance().start(m_ws_info,
			boost::bind(&fep_client::on_conn_status, this, _1),
			boost::bind(&fep_client::on_conn_closed, this, _1)
		);
		net_client_mgr::getInstance().start(m_cs_info,
			boost::bind(&fep_client::on_conn_status, this, _1),
			boost::bind(&fep_client::on_conn_closed, this, _1)
		);
	}
	void fep_client::stop()
	{
		net_client_mgr::getInstance().stop();
	}

	void fep_client::on_conn_status(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch(faith_client_ptr->get_server_type())
		{
		case e_server_type_ws:
		{
			m_ws_ci = faith_client_ptr->get_array_index();
			start_loop();
		}
		break;
		case e_server_type_cs:
		{
			m_cs_ci = faith_client_ptr->get_array_index();
			start_loop();
		}
		default:
			break;
		}
	}

	void fep_client::on_conn_closed(const net_client* faith_client_ptr)
	{
		if (nullptr == faith_client_ptr)
		{
			return;
		}
		switch (faith_client_ptr->get_server_type())
		{
		case e_server_type_ws:
		{
			m_ws_ci = tcp_client::invalid_conn_index;
		}
		break;
		case e_server_type_cs:
		{
			m_cs_ci = tcp_client::invalid_conn_index;
		}
		default:
			break;
		}
	}
	void fep_client::start_loop()
	{
		if (m_cs_ci != tcp_client::invalid_conn_index && m_ws_ci != tcp_client::invalid_conn_index)
		{
			if (!proxy_service_cli::getInstance().start())
			{
				CONSOLE_INFO("proxy_service_cli::getInstance().start error");
				return;
			}
			m_timerindex_gameloop = scheduler::getInstance().add_timer(500, boost::bind(&fep_client::server_loop, this, _1));
		}
	}
	void fep_client::server_loop(uint32 timer_index)
	{
		ZoneScoped;
		static int64 game_time	= faith::utility::get_tick_count();
		int64 time_now = faith::utility::get_tick_count();
		static int32 loop_counter = 0;
		static int64 loop_time = 0;
		loop_counter++;
		loop_time += time_now - game_time;
		game_time = time_now;

		proxy_service_cli::getInstance().update();

		static int64 last_log_time = 0;
		if (time_now > last_log_time)
		{
			last_log_time = time_now + server_console_time;

			if (m_gm_state)
			{
				CONSOLE_INFO("==========fep gm server status==========");
			}
			else
			{
				CONSOLE_INFO("==========fep server status==========");
			}

			CONSOLE_INFO("ws {}/{} cs {}/{}", net_client_mgr::getInstance().get_server_count(e_server_type_ws), SERVER_WS_COUNT, net_client_mgr::getInstance().get_server_count(e_server_type_cs), SERVER_CS_COUNT);

			// 输出信息
			int32 session_count = proxy_service_cli::getInstance().get_session_num();
			CONSOLE_INFO("session num:{} session max:{}", session_count, init_socket_more);
			if (loop_counter > 0)
			{
				CONSOLE_INFO("tick:{}", loop_time / loop_counter);
				loop_time = 0;
				loop_counter = 0;
			}
			if (daemon_client::getInstance().get_server_close())
			{
				CONSOLE_INFO("daemon close, please shutdown fep ! ! !");
				app_server::getInstance().stop();
				return;
			}
		}
		daemon_client::getInstance().heart_tick(time_now);
		http_access_mgr::get_instance().tick(time_now);
		csv_synchronization_fep_mgr::get_instance().heart_tick(time_now);

		static uint64 sync_fep_data = 0;	// fep同步数据计时器
		if(time_now >= sync_fep_data)
		{
			sync_fep_data = time_now + server_player_to_ws;
			faith::app_server_update	req;
			req.player_count = proxy_service_cli::getInstance().get_session_num();
			req.max_player_count = init_socket_more;

			tcp_client::get_instance().send(m_ws_ci, &req, sizeof(req));
		}

	}

	void fep_client::internal_rep_login(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const rep_login* pdata = static_cast<const rep_login*>(data_ptr);
		if ( NULL == pdata )
		{
			return;
		}
		m_cross_server_id = pdata->cross_server_id;
		m_open_time = pdata->open_time;
	}
	void fep_client::internal_rep_stop(uint32 connindex, const void* data_ptr, size_t data_len)
	{
		const faith::req_stop * msg = static_cast<const faith::req_stop*>(data_ptr);
		if (NULL == msg)
		{
			return;
		}
		if (msg->server_type != e_server_type_fep)
		{
			return;
		}
		daemon_client::getInstance().set_server_close(true);
		CONSOLE_INFO("FaithEye Stop Game!");
	}

	void fep_client::handler_daemon_onrecv(const void* data_ptr, size_t data_len)
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

	void fep_client::close_server(const void* data_ptr, size_t data_len)
	{
		eye_proto_d2s_close_process request;
		if (!parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len))
		{
			return;
		}
		if (request.server_id() != SERVERCONFIG->game_id
			|| request.server_type() != e_server_type_fep)
		{
			return;
		}
		daemon_client::getInstance().set_server_close(true);
		CONSOLE_INFO("FaithEye Stop Game!");
	}
	void fep_client::send_message_to_ws(const void* data_ptr, size_t data_len)
	{
		net_client_mgr::getInstance().send_message(m_ws_ci, data_ptr, data_len);
	}
	void fep_client::send_message_to_cs(const void* data_ptr, size_t data_len, uint32 connindex)
	{
		net_client_mgr::getInstance().send_message(m_cs_ci, data_ptr, data_len);
	}
}
