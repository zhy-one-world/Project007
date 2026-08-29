/********************************************************************
	created:	2015/03/17
	created:	17:3:2015   12:28
	file base:	parse_msg 
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include "server_log.hpp"
#include "parse_msg.h"

namespace faith
{

	parse_msg::parse_msg()
	{
		memset(&db_cahce, 0, sizeof(db_cahce));
	}

	parse_msg::~parse_msg()
	{

	}

	parse_msg& parse_msg::getInstance()
	{
		static parse_msg g_instance;
		return g_instance;
	}

	bool parse_msg::parse_message_new(google::protobuf::Message* proto_ptr,const void* data_ptr,size_t data_len)
	{
		if (proto_ptr && data_ptr && data_len > 0)
		{
			packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data_ptr;
			return proto_ptr->ParseFromArray(package_ptr->google_data, package_ptr->get_google_data_len());
		}
		else
		{
			return false;
		}
	}
	bool parse_msg::parse_buffer_to_proto(google::protobuf::Message* proto_ptr, const void* data_ptr, size_t data_len, bool is_check_len)
	{
		if (proto_ptr == nullptr || data_ptr == nullptr)
		{
			return false;
		}
		if (is_check_len == true && data_len == 0)
		{
			return false;
		}
	    return proto_ptr->ParseFromArray(data_ptr, data_len);	
	}
	packet_s2s* parse_msg::parse_message_server(google::protobuf::Message* proto_ptr, const void* data_ptr, size_t data_len)
	{
		if (proto_ptr && data_ptr && data_len > 0)
		{
			packet_s2s* package_ptr = (packet_s2s*)data_ptr;
			if (proto_ptr->ParseFromArray(package_ptr->google_data, package_ptr->get_google_data_len()))
			{
				return package_ptr;
			}
			return nullptr;
		}
		else
		{
			return nullptr;
		}
	}
	void parse_msg::parse_clientuid_from_msg_new(const void* data_ptr, s_client_uid& client_uid)
	{	
		if (data_ptr == NULL)
			return;

		packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data_ptr;

		client_uid = package_ptr->client_uid;
	}
	void parse_msg::parse_clientuid_from_msg_new(const void* data_ptr, int32& array_index)
	{
		if (data_ptr == NULL)
		{
			return;
		}
		packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data_ptr;
		array_index = package_ptr->client_uid.fepsession_uid;
	}

	bool parse_msg::is_internal_package(const void* data,size_t data_len)
	{
		if (NULL == data)
		{
			return false;
		}

		packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data;

		if (package_ptr->wheader >= e_msg_c2s_s2c_begin && package_ptr->wheader < e_msg_c2s_s2c_end)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	ui16 parse_msg::get_packet_header(const void* data,size_t data_len)
	{
		ui16 header = 0;
		{
			const packet_base* packet = static_cast<const packet_base*>(data);
			if(packet)
			{
				header = packet->wheader;
			}
		}

		return header;
	}

	s_client_uid parse_msg::get_packet_connect_idx(const void* data,size_t data_len)
	{
		s_client_uid client_uid;
		bool is_internal = is_internal_package(data,data_len);
		if(!is_internal)
		{
			packet_c2s_s2c* package_ptr = (packet_c2s_s2c*)data;
			if (package_ptr)
			{
				client_uid = package_ptr->client_uid;
			}
		}
		return client_uid;
	}
	packet_c2s_s2c* parse_msg::serialze_client(s_client_uid client_uid, google::protobuf::Message* netPro, int32 header)
	{
		if (NULL == netPro || client_uid.is_valid() == false)
		{
			return nullptr;
		}
		m_msg.wheader = header;
		m_msg.client_uid = client_uid;
		if (netPro->SerializeToArray(m_msg.google_data, sizeof(m_msg.google_data)))
		{
			m_msg.google_data_len = netPro->ByteSize();
			return &m_msg;
		}
		else
		{
			CONSOLE_INFO("    header : {}", header);
			return nullptr;
		}
	}
	packet_s2s* parse_msg::serialze_server(google::protobuf::Message* netPro, int32 header, guid_64 logic_guid, int32 logic_index, int32 logic_ex)
	{
		if (NULL == netPro)
		{
			return nullptr;
		}
		m_server_msg.wheader = header;
		m_server_msg.logic_guid = logic_guid;
		m_server_msg.logic_index = logic_index;
		m_server_msg.logic_ex = logic_ex;
		if (netPro->SerializeToArray(m_server_msg.google_data, sizeof(m_server_msg.google_data)))
		{
			m_server_msg.google_data_len = netPro->ByteSize();
			return &m_server_msg;
		}
		else
		{
			CONSOLE_INFO("    header : {}", header);
			return nullptr;
		}
	}
	packet_s2s* parse_msg::serialze_server(google::protobuf::Message* netPro, int32 header, int64 data_flag, guid_64 logic_guid, int32 logic_index, int32 logic_ex)
	{
		if (NULL == netPro)
		{
			return nullptr;
		}
		m_server_msg.wheader = header;
		m_server_msg.logic_guid = logic_guid;
		m_server_msg.logic_index = logic_index;
		m_server_msg.logic_ex = logic_ex;
		m_server_msg.data_flag = data_flag;
		if (netPro->SerializeToArray(m_server_msg.google_data, sizeof(m_server_msg.google_data)))
		{
			m_server_msg.google_data_len = netPro->ByteSize();
			return &m_server_msg;
		}
		else
		{
			CONSOLE_INFO("    header : {}", header);
			return nullptr;
		}
	}
	packet_s2s* parse_msg::serialze_server(const xchar* data_array, int32 data_size, int32 header, guid_64 logic_guid, int32 logic_index, int32 logic_ex)
	{
		m_server_msg.wheader = header;
		m_server_msg.logic_guid = logic_guid;
		m_server_msg.logic_index = logic_index;
		m_server_msg.logic_ex = logic_ex;
		memcpy(m_server_msg.google_data, data_array, data_size);
		m_server_msg.google_data_len = data_size;
		return &m_server_msg;
	}
	packet_s2s * parse_msg::serialze_buffer(google::protobuf::Message* netPro)
	{
		return serialze_server(netPro, 0, guid_64(), 0, 0);

	}

	xchar * parse_msg::get_cache(int32 size)
	{
		if (size > INTERNAL_SERVER_MAX_PACKET_SIZE)
		{
			return nullptr;
		}
		memset(db_cahce, 0, INTERNAL_SERVER_MAX_PACKET_SIZE); 
		return db_cahce;
	}

	void  parse_msg::my_memcopy(void* dst, size_t max_len, void const* src, size_t size)
	{
		size_t len = size;
		if (max_len < size)
		{
			len = max_len;
		}
		memcpy(dst, src, len);
	}
	void  parse_msg::my_memcopy_string(void* dst, size_t max_len, const xstring &str)
	{
		size_t len = str.size();
		if (max_len < len)
		{
			len = max_len;
		}
		memcpy(dst, str.c_str(), len);
	}
	
}

