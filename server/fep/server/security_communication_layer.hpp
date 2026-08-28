/********************************************************************
	created:	2014/07/30
	created:	30:7:2014   18:12
	file base:	security_communication_layer
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _SECURITY_COMMUNICATION_LAYER_H_
#define _SECURITY_COMMUNICATION_LAYER_H_

#include <singleton.hpp>
#include "logic/type_def.hpp"
#include "base.hpp"

namespace hld
{
	//	基于RDE方法的“通信安全层”（SCL）实现
	//	实现 FEP 和 Client 之间的通信安全层
	//	主要由 proxy_service_cli 使用
	class security_communication_layer : public singleton<security_communication_layer>
	{
	public:
		friend class singleton<security_communication_layer>;
		typedef boost::function<int32(uint32,const void*,size_t)>	sender_handler_type;
		typedef boost::function<void(uint32,const void*,size_t)>	recved_handler_type;

	public:
		security_communication_layer(void);
		~security_communication_layer(void);
	public:
		bool							add_scl_session(uint32 conn_index, sender_handler_type& sender,recved_handler_type& recved_handler);
		int32							send_to_session(uint32 conn_index, const void* data_ptr, size_t data_len);
		void							send_to_session(uint32 conn_index, google::protobuf::Message* proto_ptr, int32 header);
		void							send_to_session(uint32 conn_index, int32 header, const void* data_ptr, size_t data_len);
		void							on_encrypted_data_recved(uint32 conn_index,const void* data_ptr,size_t data_len);
		int64							get_send_byte();
		void							print_send_byte();
	private:
		recved_handler_type				m_scl_cli_recver;
		sender_handler_type				m_scl_cli_sender;
		packet_c2s_s2c					m_cs_msg;
		int64							m_send_byte;
		std::map<int32, int64>			m_send_message_map;
	};
}

#endif//#define __SECURITY_COMMUNICATION_LAYER_H_
