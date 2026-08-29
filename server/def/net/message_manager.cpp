/********************************************************************
	created:	2014/07/31
	created:	31:7:2014   15:20
	file base:	message_manager
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "message_manager.hpp"
#include <base.hpp>
#include "template/template_manager.h"
#include "utility/parse_msg.h"
#include "server_log.hpp"


namespace faith
{
	void none_msg_fuction(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		return;
	}
	message_manager::message_manager() 
	{
		for (int32 i = 0; i < e_msg_base_max;++i)
		{
			register_handler(i, none_msg_fuction);
		}
		m_server_type = e_server_type_invalid;
	}

	message_manager::~message_manager() 
	{
	}
	bool message_manager::register_handler(ui16 packet_id, const on_recv_handler_type& recv_handler)
	{
		m_handler_map[packet_id] = recv_handler;
		return true;
	}
	void message_manager::on_data_received(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		ZoneScoped;
		const packet_base* pPacket = static_cast<const packet_base*>(data_ptr);
		if (NULL == pPacket)
		{
			return;
		}
		if (pPacket->wheader < 0 || pPacket->wheader >= e_msg_base_max)
		{
			return;
		}
		m_handler_map[pPacket->wheader](conn_index, data_ptr, data_len);
	}
}
