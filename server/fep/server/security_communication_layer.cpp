/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:15
	file base:	security_communication_layer
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "time.hpp"
#include "security_communication_layer.hpp"
#include <file_system.hpp>
#include <login_msg.hpp>
#include "proxy_service_cli.hpp"
#include "fep_client.hpp"
#include <Utility/serialize_msg.h>
#include "server_log.hpp"
#include "net.pb.h"

namespace hld
{
	using namespace utility;
	security_communication_layer::security_communication_layer(void)
	{
		m_send_byte = 0;
	}

	security_communication_layer::~security_communication_layer(void)
	{
	}

	bool security_communication_layer::add_scl_session(uint32 conn_index,sender_handler_type& sender,recved_handler_type& recved_handler)
	{
		m_scl_cli_recver = recved_handler;
		m_scl_cli_sender = sender;

		if ( m_scl_cli_sender )
		{
			login_proto_rde_encryption rdeEncryption;
			rdeEncryption.set_is_gm(fep_client::getInstance().get_gm_state());
			send_to_session(conn_index, &rdeEncryption, e_msgindex_s2c_rdeencryption);
		}
		return true;
	}
	int64 security_communication_layer::get_send_byte()
	{
		int64 temp_send_byte = m_send_byte;
		m_send_byte = 0;
		return temp_send_byte;
	}
	void	security_communication_layer::print_send_byte()
	{
		for (std::map<int32, int64>::iterator it = m_send_message_map.begin(); it != m_send_message_map.end(); ++it)
		{
			CONSOLE_INFO("send num:{} header:{}", it->second, it->first);
		}
	}
	//	desc:	加密并发送数据
	int32	security_communication_layer::send_to_session(uint32 conn_index,const void* data_ptr, size_t data_len)
	{
		int32 ret = 0;
		if ( m_scl_cli_sender )
		{
			client_session* client_session_ptr = proxy_service_cli::getInstance().get_session_by_connect(conn_index);
			if (nullptr == client_session_ptr)
			{
				return false;
			}
			m_send_byte += data_len;
			void* msg = const_cast<void*>(data_ptr);
			packet_c2s_s2c* packet_msg = (packet_c2s_s2c*)msg;
			packet_msg->wheader_plus = client_session_ptr->get_msg_index();
			ret = m_scl_cli_sender(conn_index, packet_msg, data_len);
			//m_send_message_map[packet_msg->wheader] += data_len;
		}
		return ret;
	}
	void security_communication_layer::send_to_session(uint32 conn_index, google::protobuf::Message* proto_ptr, int32 header)
	{
		m_cs_msg.clear_data();
		serialize_msg::get_instance().set_serialize_msg_new(m_cs_msg, proto_ptr, header);
		send_to_session(conn_index, &m_cs_msg, m_cs_msg.get_packet_len());

	}
	void security_communication_layer::send_to_session(uint32 conn_index, int32 header, const void* data_ptr, size_t data_len)
	{
		m_cs_msg.clear_data();
		memcpy(m_cs_msg.google_data, data_ptr, data_len);
		m_cs_msg.wheader = header;
		m_cs_msg.google_data_len = data_len;
		send_to_session(conn_index, &m_cs_msg, m_cs_msg.get_packet_len());
	}

	//	desc:	处理接收到的密文数据
	void security_communication_layer::on_encrypted_data_recved(uint32 conn_index,const void* data_ptr,size_t data_len)
	{
		if (m_scl_cli_recver)
		{
			m_scl_cli_recver(conn_index, data_ptr, data_len);
		}
	}

}
