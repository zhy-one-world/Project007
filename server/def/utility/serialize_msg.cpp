/********************************************************************
	created:	2014/07/29
	created:	29:7:2014   16:23
	file base:	serialize_msg
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include "serialize_msg.h"


namespace hld
{
	serialize_msg::serialize_msg()
	{

	}

	serialize_msg::~serialize_msg()
	{

	}

	serialize_msg& serialize_msg::get_instance()
	{
		static serialize_msg g_instance;
		return g_instance;
	}

	void serialize_msg::set_server_data_new(packet_c2s_s2c& msg, s_client_uid client_uid)
	{
		msg.client_uid = client_uid;
	}

	void serialize_msg::set_serialize_msg_new(packet_s2s& msg, google::protobuf::Message* proto_ptr, int32 header)
	{
		if (NULL == proto_ptr)
			return;

		bool ret = proto_ptr->SerializeToArray((void*)msg.google_data, sizeof(msg.google_data));
		if (!ret)
		{
			return;
		}
		msg.wheader = header;
		msg.google_data_len = proto_ptr->ByteSize();
	}

	void serialize_msg::set_serialize_msg_new(packet_c2s_s2c& msg, google::protobuf::Message* proto_ptr,int32 header)
	{
		if (NULL == proto_ptr)
			return;

		bool ret = proto_ptr->SerializeToArray((void*)msg.google_data, sizeof(msg.google_data));
		if ( !ret )
		{
			return;
		}
		msg.wheader = header;
		msg.google_data_len = proto_ptr->ByteSize();
	}

	void serialize_msg::set_serialize_msg_new(packet_c2s_s2c& msg, google::protobuf::Message* proto_ptr, s_client_uid client_uid, int32 header)
	{
		if (NULL == proto_ptr)
			return;

		set_server_data_new(msg, client_uid);

		bool ret = proto_ptr->SerializeToArray((void*)msg.google_data, sizeof(msg.google_data));
		if ( !ret )
		{
			return;
		}
		msg.wheader = header;
		msg.google_data_len = proto_ptr->ByteSize();
	}
	
}
