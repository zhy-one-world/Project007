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
#include <rlog.hpp>
#include "net.pb.h"
#include "eye_proto.pb.h"

namespace faith
{
	using namespace net;


	fep_client::fep_client()
	{
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
		_RLOG_(MINFO, "fep client starting external proxy service");
		if (!proxy_service_cli::getInstance().start())
		{
			_RLOG_(MERROR, "proxy_service_cli::getInstance().start error");
		}
		else
		{
			_RLOG_(MINFO, "external proxy service started");
		}
		m_timerindex_gameloop = scheduler::getInstance().add_timer(
			500,
			FEP_NETWORK_SCHEDULER_THREAD_ID,
			boost::bind(&fep_client::server_loop, this, _1));
		_RLOG_(MINFO, "fep server loop timer initialized, timer index="
			<< m_timerindex_gameloop << " thread=" << FEP_NETWORK_SCHEDULER_THREAD_ID);

		scheduler::getInstance().post(
			boost::bind(&fep_client::start_on_session_thread, this),
			FEP_NETWORK_SCHEDULER_THREAD_ID);
		_RLOG_(MINFO, "internal WS/CS startup task posted, thread="
			<< FEP_NETWORK_SCHEDULER_THREAD_ID);
	}

	void fep_client::start_on_session_thread()
	{
		_RLOG_(MINFO, "starting internal WS client, endpoint="
			<< m_ws_info.ip_addr << ":" << m_ws_info.port);
		const bool ws_start_result = net_client_mgr::getInstance().start(m_ws_info,
			boost::bind(&fep_client::on_conn_status, this, _1),
			boost::bind(&fep_client::on_conn_closed, this, _1)
		);
		_RLOG_(MINFO, "internal WS client start result=" << ws_start_result);
		_RLOG_(MINFO, "starting internal CS client, endpoint="
			<< m_cs_info.ip_addr << ":" << m_cs_info.port);
		const bool cs_start_result = net_client_mgr::getInstance().start(m_cs_info,
			boost::bind(&fep_client::on_conn_status, this, _1),
			boost::bind(&fep_client::on_conn_closed, this, _1)
		);
		_RLOG_(MINFO, "internal CS client start result=" << cs_start_result);
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
			m_ws_session = faith_client_ptr->get_session();
			_RLOG_(MINFO, "internal WS connected, session=" << m_ws_session.get());
		}
		break;
		case e_server_type_cs:
		{
			m_cs_session = faith_client_ptr->get_session();
			_RLOG_(MINFO, "internal CS connected, session=" << m_cs_session.get());
		}
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
			m_ws_session.reset();
			_RLOG_(MWARN, "internal WS connection closed");
		}
		break;
		case e_server_type_cs:
		{
			m_cs_session.reset();
			_RLOG_(MWARN, "internal CS connection closed");
		}
		break;
		default:
			break;
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

		static int64 last_log_time = 0;
		if (time_now > last_log_time)
		{
			last_log_time = time_now + server_console_time;

			if (m_gm_state)
			{
				_RLOG_(MINFO, "==========fep gm server status==========");
			}
			else
			{
				_RLOG_(MINFO, "==========fep server status==========");
			}

			_RLOG_(MINFO, "ws " << net_client_mgr::getInstance().get_server_count(e_server_type_ws)
				<< "/" << SERVER_WS_COUNT << " cs "
				<< net_client_mgr::getInstance().get_server_count(e_server_type_cs)
				<< "/" << SERVER_CS_COUNT);


			int32 session_count = proxy_service_cli::getInstance().get_session_num();
			_RLOG_(MINFO, "session num:" << session_count
				<< " session max:" << init_socket_more);
			if (loop_counter > 0)
			{
				_RLOG_(MINFO, "tick:" << loop_time / loop_counter);
				loop_time = 0;
				loop_counter = 0;
			}
			if (daemon_client::getInstance().get_server_close())
			{
				_RLOG_(MWARN, "daemon close, please shutdown fep ! ! !");
				app_server::getInstance().stop();
				return;
			}
		}
		daemon_client::getInstance().heart_tick(time_now);
		http_access_mgr::get_instance().tick(time_now);

		static uint64 sync_fep_data = 0;	// fepͬ�����ݼ�ʱ��
		if(time_now >= sync_fep_data)
		{
			sync_fep_data = time_now + server_player_to_ws;
			faith::app_server_update	req;
			req.player_count = proxy_service_cli::getInstance().get_session_num();
			req.max_player_count = init_socket_more;

			if (m_ws_session)
			{
				tcp_client::get_instance().send(m_ws_session, &req, sizeof(req));
			}
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
		_RLOG_(MWARN, "FaithEye Stop Game!");
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
		_RLOG_(MWARN, "FaithEye Stop Game!");
	}
	void fep_client::send_message_to_ws(const void* data_ptr, size_t data_len)
	{
		if (m_ws_session)
		{
			tcp_client::get_instance().send(m_ws_session, data_ptr, data_len);
		}
	}
	void fep_client::send_message_to_cs(const void* data_ptr, size_t data_len, uint32 connindex)
	{
		(void)connindex;
		if (m_cs_session)
		{
			tcp_client::get_instance().send(m_cs_session, data_ptr, data_len);
		}
	}
}
