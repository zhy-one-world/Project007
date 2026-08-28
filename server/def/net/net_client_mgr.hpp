/********************************************************************
  created: 2019/07/03
  file base: net_client_mgr
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#pragma once
/************************************************************************/
/*                           File Include                               */
/************************************************************************/

#include <singleton.hpp>
#include "base.hpp"
#include "net_client.hpp"
/************************************************************************/
/*							 Function Define                            */
/************************************************************************/
namespace hld
{
	using namespace net;
	/************************************************************************/
	/*							  Class Declare                             */
	/************************************************************************/
	typedef boost::function<void(uint32, const void*, size_t)>	on_recv_handler_type;

	class net_client_mgr:
		public singleton<net_client_mgr>
	{
		friend class singleton<net_client_mgr>;

	private:
		net_client_mgr(void);
	public:	
		~net_client_mgr(void);
	public:
		e_server_type get_server_type() { return m_server_type; }
		void set_server_type(e_server_type server_type) { m_server_type = server_type; }
		int32 get_server_count(e_server_type server_type);
		net_client* get_client_by_index(uint32 conn_index);
	public:
		bool set_netpara_option(uint32 send_buf_size, uint32 recv_buf_size, uint32 max_packet_size, uint32 client_num);
		bool start(const s_server_info& server_info,
			client_on_connect_handler_type onconnect_handler,
			client_on_closed_handler_type onclose_handler);
		void stop();
		void stop(uint32 conn_index);
		void send_message(uint32 conn_index, const void* data_ptr, size_t data_len);
		void broadcast_pak(const void* data_ptr, size_t data_len);
		void broadcast_pak(const void* data_ptr, size_t data_len, e_server_type server_type);
		void broadcast_pak_out(const void* data_ptr, size_t data_len, e_server_type server_type);
		void broadcast_pak_out(const void* data_ptr, size_t data_len, int32 conn_index);
	private:
		net_client* get_empty_client();
		void on_data_received(const net_client* faith_client_ptr, const void *data_ptr, size_t data_len);
	private:
		net_client* m_client_connmap;
		int32 m_client_num;
		e_server_type m_server_type;
	};

}
