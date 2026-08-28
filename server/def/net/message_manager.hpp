/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   15:01
	file base:	message_manager
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MESSAGE_MANAGER_HPP_
#define _MESSAGE_MANAGER_HPP_

#include <singleton.hpp>
#include "logic/type_def.hpp"
#include <base.hpp>

namespace hld
{
	void none_msg_fuction(uint32 conn_index, const void* data_ptr, size_t data_len);
	typedef boost::function<void(uint32, const void*, size_t)>	on_recv_handler_type;
	class message_manager : public singleton<message_manager>
	{
	private:
		friend class singleton<message_manager>;

	protected:
		message_manager();
	public:
		~message_manager(); 
	public:
		void set_server_type(e_server_type server_type) {m_server_type = server_type;}
		bool register_handler(ui16 packet_id, const on_recv_handler_type& recv_handler);
		void on_data_received(uint32 conn_index, const void *data_ptr, size_t data_len);
	private:
		on_recv_handler_type	m_handler_map[e_msg_base_max];
		e_server_type			m_server_type;
	};

}


#endif // __message_manager_HPP__