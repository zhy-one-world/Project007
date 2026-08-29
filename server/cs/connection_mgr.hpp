/********************************************************************
  created: 2014/07/14
  created: 14:7:2014 14:14
  file base: connection_mgr
  file ext: hpp
  author: lidongyang
  
  purpose: 
*********************************************************************/
#ifndef _CONNECTION_MGR_HPP_
#define _CONNECTION_MGR_HPP_
/************************************************************************/
/*                           File Include                               */
/************************************************************************/

#include <singleton.hpp>
#include "server_def.hpp"
#include <Logic/transfer_def.hpp>
#include <net/net_client_mgr.hpp>
#include "utility/serialize_msg.h"
namespace faith
{
	/************************************************************************/
	/*							  Class Declare                             */
	/************************************************************************/
	class connection_mgr:public singleton<connection_mgr>
	{
		friend class singleton<connection_mgr>;
	private:
		connection_mgr(void);
	public:	
		~connection_mgr(void);
	public:
	public:
		/************************************************************************/
		/*	CS Server															*/
		/************************************************************************/
		bool start();
		bool start_gate();
		bool start_gate_new(const s_server_info& gate_info);
		void stop();
		void send_to_dp(const void* data_ptr, size_t data_len, int32 server_id = 0);
		void send_to_dp_lua(const char* msg, int32 msg_len, uint32 header, int32 server_id);
		void send_to_dp_lua(google::protobuf::Message* net_pro, uint32 header, int32 server_id);
		void send_to_ws(const void* data_ptr, size_t data_len, int32 server_id = 0);
		void send_to_ws_lua(const char* msg, int32 msg_len, uint32 header, int32 server_id);
		void send_to_ws_lua(google::protobuf::Message* net_pro, uint32 header, int32 server_id = 0);
		/************************************************************************/
		/*	                           CS to WS Client                          */
		/************************************************************************/
		void internal_rep_login(uint32 connindex, const void* data_ptr, size_t data_len);
		void internal_gate_msg(uint32 connindex, const void* data_ptr, size_t data_len);
		void gate2ws_rep_gate_new(uint32 conn_index, const void* data_ptr, size_t data_len);
		bool broadcast_pak(const void* data_ptr, size_t data_len);
		bool broadcast_pak(const void* data_ptr, size_t data_len, e_server_type server_type);
	private:
		void on_conn_status(const net_client* faith_client_ptr);
		void on_conn_closed(const net_client* faith_client_ptr);
		uint32 get_gate_index();
	private:
		s_server_info m_ws_info;
		uint32 m_ws_conn_index;
		s_server_info m_dp_info;
		uint32 m_dp_conn_index;
		s_server_info m_gate_info;
		int32 m_gate_index;
		std::vector<uint32> m_gate_conn_array;

		packet_s2s		m_s2s_msg;
	};

}

#endif
