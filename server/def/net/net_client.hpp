/********************************************************************
  created: 2019/07/03
  file base: faith_client
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#pragma once

#include "server_def.hpp"

namespace hld
{
	using namespace net;
	class net_client;
	typedef boost::function<void(const net_client*)>										client_on_connect_handler_type;
	typedef boost::function<void(const net_client*, const void*, size_t)>					client_on_recved_handler_type;
	typedef boost::function<void(const net_client*)>										client_on_closed_handler_type;

	class net_client
	{
	public:
		net_client(void);
		~net_client(void);
		void clear_data();
	public:
		e_server_status get_server_status() { return m_server_status; }
		const e_server_type get_server_type()const { return m_server_info.server_type; }
		void set_array_index(uint32 conn_index) { m_conn_index = conn_index; }
		const uint32 get_array_index()const { return m_conn_index; }
		const s_server_info& get_server_info() { return m_server_info; }
	public:
		bool start(const s_server_info& server_info, 
			client_on_connect_handler_type onconnect_handler,
			client_on_closed_handler_type onclose_handler, 
			client_on_recved_handler_type onrecved_handler);
		void stop();
		uint32 connect_to();
		int32 send_message(const void* data_ptr, size_t data_len);
		void on_data_received(uint32 connindex, const void *data_ptr, size_t data_len);
	private:
		void on_conn_status(uint32 connindex, tcp_client::e_connect_info status, xstring info);
		void on_conn_closed(uint32 connindex);	
	private:
		void retry_connect(uint32 timer_index);
	private:
		uint32 m_timerindex_connect;
		e_server_status m_server_status;
		uint32 m_conn_index;
		s_server_info m_server_info;
		client_on_connect_handler_type m_onconnect_handler;
		client_on_recved_handler_type m_onrecved_handler;
		client_on_closed_handler_type m_onclose_handler;
	};

}
