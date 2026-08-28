/*@@

Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved.

Created_datetime : 	2019/07/03

File Name :	net_server_mgr.hpp

Author : zhy

Description :

Change List :


@@*/
//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <singleton.hpp>
#include "server_def.hpp"
#include "base.hpp"
#include "net_server.hpp"

namespace hld
{
	using namespace net;

	//////////////////////////////////////////////////////////////////////////
	//
	//	Class connection_server Declare
	//
	//////////////////////////////////////////////////////////////////////////
	typedef boost::function<void(const net_server*)>										server_on_closed_handler_type;
	class net_server_mgr : public singleton<net_server_mgr>
	{
		friend class singleton<net_server_mgr>;

		net_server_mgr(void);
	public:
		~net_server_mgr(void);
	private:
		/************************************************************************/
		/* ws tcp server                                                        */
		/************************************************************************/
		void handler_serverstatus(tcp_server::e_server_status_type status);
		void handler_onconnected(uint32 conn_index);
		void handler_onclose(uint32 conn_index);
		void handler_onrecv(uint32 conn_index, const void* data_ptr, size_t data_len);;
		bool send_message_by_index(const void* data_ptr, size_t data_len, int32 conn_index);
		void send_message_by_type(const void* data_ptr, size_t data_len, e_server_type server_type);
	public:
		/************************************************************************/
		/*tcp server                                                        */
		/************************************************************************/
		bool init(e_server_type server_type, xstring server_ip, int32 server_port,
			uint32 send_buf_size, uint32 recv_buf_size, uint32 max_packet_size, uint32 server_num, uint32 init_num,
			server_on_closed_handler_type onclosed_handler);
		bool start();
		void stop() { if (m_tcpserver_ptr) m_tcpserver_ptr->stop(); }
	public:
		void set_server_type(e_server_type server_type) { m_server_type = server_type; }
		e_server_type get_server_type() { return m_server_type; }
		void set_server_index(int32 server_index) { m_server_index = server_index; }
		int32 get_server_index() { return m_server_index; }
		xstring get_server_ip() { return m_server_ip; }
		int32 get_server_port() { return m_server_port; }
	public:
		std::vector<uint32> get_server_conn_array(e_server_type server_type);
		int32 get_server_count(e_server_type server_type);
		net_server* get_peer_by_conn_index(uint32 conn_index);
		net_server* get_peer_by_type(e_server_type server_type);
		bool send_by_gate(const void* data_ptr, size_t data_len);
		void send_message(const void* data_ptr, size_t data_len, int32 conn_index = -1, e_server_type server_type = e_server_type_invalid);
		void send_message_out(const void* data_ptr, size_t data_len, int32 conn_index = -1, e_server_type server_type = e_server_type_invalid);
	private:
		/************************************************************************/
		/* tcp server															*/
		/************************************************************************/
		e_server_type									m_server_type;
		int32											m_server_index;
		tcp_server*										m_tcpserver_ptr;//	server object
		xstring											m_server_ip;
		int32											m_server_port;
		net_server*									m_conn_map;	//	client
		int32											m_conn_num;
		std::vector<e_server_type>						m_gate_array;
		int32											m_gate_count;
		server_on_closed_handler_type					m_external_onclose_handler;
	};

}

