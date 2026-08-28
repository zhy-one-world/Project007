/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   20:26
	file base:	dbproxy_service
	file ext:	hpp
	author:		lucifer~yu
	
	purpose:	
*********************************************************************/
#ifndef _DB_PROXY_SERVICE_H_
#define _DB_PROXY_SERVICE_H_

#include "logic/type_def.hpp"
#include <singleton.hpp>
#include "base.hpp"
#include "server_def.hpp"
#include "net/net_server_mgr.hpp"
#include "data_manager/data_manager.hpp"

namespace hld
{
	//
	//	singleton services class
	//
	class dbproxy_service
	{
		friend class singleton<dbproxy_service>;
	protected:
		dbproxy_service(	);
	public:
		virtual ~dbproxy_service(	);
		static dbproxy_service& getInstance()
		{
			static dbproxy_service s_dbproxy_service_client;
			return s_dbproxy_service_client;
		}
	public:
		bool									init_db_conn();
		bool									start(	);
		void									stop();

		xstring									get_ipaddr();
		uint32									get_port();
		int32									get_server_id() { return m_server_id; }
	private:													// 消息处理定时器
		void									server_loop(uint32 timer_index);
		void									on_conn_closed(const net_server* faith_server_ptr);
	public:
		void									on_req_login(uint32 connindex, const void *data_ptr, size_t data_len);
		void									on_req_stop(uint32 connindex, const void *data_ptr, size_t data_len);
		void									send_message(uint32 connindex, const void *data_ptr, size_t data_len);
		void									send_message(uint32 connindex, google::protobuf::Message* proto_ptr, uint32 header);
		void									send_message_lua(uint32 connindex, const char* msg, int32 msg_len, uint32 header);
		void                                    send_message_lua(uint32 connindex, google::protobuf::Message* net_pro, uint32 header);
		void									handler_daemon_onrecv(const void* data_ptr, size_t data_len);
		void									close_server(const void* data_ptr, size_t data_len);
	protected:
		uint32									m_timerindex_gameloop;
		xstring									m_ipaddr;
		uint32									m_port;
		int32									m_server_id;
		packet_s2s								m_dp2s_msg;
	};
}

#endif
