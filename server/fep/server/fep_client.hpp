/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:29
	file base:	fep_client
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _FEP_SERVER_H_
#define _FEP_SERVER_H_

#include <singleton.hpp>
#include "server_def.hpp"
#include <net/net_client_mgr.hpp>

namespace faith
{
	//	FEP server is a client of ws
	class fep_client : public singleton<fep_client>
	{
		friend class singleton<fep_client>;

	protected:
		fep_client();
	public:
		virtual ~fep_client();
	public:
		void start();
		void stop();
		bool get_gm_state() { return m_gm_state; }
		void internal_rep_login(uint32 connindex, const void* data_ptr, size_t data_len);
		void internal_rep_stop(uint32 connindex, const void* data_ptr, size_t data_len);
		void handler_daemon_onrecv(const void* data_ptr, size_t data_len);
		void close_server(const void* data_ptr, size_t data_len);
		void send_message_to_ws(const void* data_ptr, size_t data_len);
		void send_message_to_cs(const void* data_ptr, size_t data_len, uint32 connindex);
	private:
		void start_on_session_thread();
		void on_conn_status(const net_client* faith_client_ptr);
		void on_conn_closed(const net_client* faith_client_ptr);
		void server_loop(uint32 timer_index);
	protected:
		faith::net::tcp_client_session_ptr m_ws_session;
		faith::net::tcp_client_session_ptr m_cs_session;
		uint32 m_timerindex_gameloop;
		int32 m_cross_server_id;
		int32 m_open_time;
		s_server_info m_ws_info;
		s_server_info m_cs_info;
		bool		m_gm_state;
	};
}

#endif//#define __FEP_SERVER_H_
