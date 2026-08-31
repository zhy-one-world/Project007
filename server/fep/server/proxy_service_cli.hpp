/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:02
	file base:	proxy_service_cli
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _PROXY_SERVICE_CLIENT_H_
#define _PROXY_SERVICE_CLIENT_H_

#include <net/tcp_server.hpp>
#include <singleton.hpp>
#include <memory>
#include <mutex>
#include <string>
#include "Logic/count_def.hpp"
#include "client_session.hpp"
#include "security_communication_layer.hpp"
#include <random.hpp>

namespace faith
{
	//
	//	singleton class, connection-proxy services for clients
	//
	class proxy_service_cli : public singleton<proxy_service_cli>
	{
		friend class singleton<proxy_service_cli>;
	protected:
		proxy_service_cli();
	public:
		virtual ~proxy_service_cli();
	public:
		void register_message(ui16 msg_index, const msg_handler_type& handler);
		void on_serverstatus_changed( uint32 status );
		void on_conn_created( uint32 connindex );
		void on_conn_closed( uint32 connindex );	
		void on_data_received( uint32 connindex,const void *data_ptr,size_t data_len );
		bool init();
		bool start();
		void stop();
		bool alloc_session(uint32 connindex);		
		void disconn_session(uint32 connindex, e_logout_result logout_result);
		client_session_ptr get_session_by_connect(uint32 connindex);
		client_session_ptr get_session_by_account(int32 array_index, const xchar* account = nullptr);
		client_session_ptr get_empty_session();
		client_session_ptr get_session_by_id(int32 array_index);
		client_session_ptr get_session_by_fep_uid_64(ui64 uid);
		void set_allow_connection(bool be_allow)	{	m_enable_connect=be_allow;		};
		void broadcast(const void* data_ptr, size_t data_len, int32 header);
		void broadcast(const void* data_ptr, size_t data_len);
		bool is_valid_session(uint32 conn_index);
		uint32 get_port()       {  return m_port;}
		int32& get_session_num() {
			ZoneScoped;
			return m_session_array_num;
		}
		void send_time_limit_activity_template_to_all();
	private:
		void logout(uint32 connindex, e_logout_result logout_result);//发送登出协议,并修改相关状态
		bool free_session(uint32 connindex);
		bool set_netpara_option(uint32 send_buf_size, uint32 recv_buf_size, uint32 max_packet_size);
	protected:
		uint32 m_port;
		bool m_enable_connect;
		net::tcp_server* m_tcpserver;
		security_communication_layer::recved_handler_type	m_scl_cli_recver;
		security_communication_layer::sender_handler_type	m_scl_cli_sender;
		msg_handler_type m_handler_map[e_msg_base_max];
		client_session_ptr m_session_array[init_socket_more];		//	who's key is connection_index to FEP
		mutable std::mutex m_session_mutex;
		int32 m_session_array_num;
	};

}

#endif
