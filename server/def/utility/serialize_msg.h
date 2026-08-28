/********************************************************************
  created: 2014/07/04
  created: 4:7:2014 17:58
  file base: serialize_msg
  file ext: h
  author: YU REN
  
  purpose: 服务器封装发送给客户端的googleprotocolbuff类型消息
*********************************************************************/
#ifndef serialize_msg_h
#define serialize_msg_h

#include "base.hpp"

namespace hld
{

	class serialize_msg
	{
	private:
		serialize_msg();
	public:
		~serialize_msg();
		static serialize_msg& get_instance();
	public:
// 		void		set_server_data(net_proto& net_prot,uint32 server_uid,uint32 connect_idx);
// 		void		set_server_data(net_proto& net_prot,s_client_uid client_uid);
// 
// 		void		set_serialize_msg(packet_c2s_s2c& msg, net_proto& net_prot);
// 		void		set_serialize_msg(packet_c2s_s2c& msg, net_proto& net_prot,s_client_uid client_uid);

		// new by Locke.
		void		set_server_data_new(packet_c2s_s2c& msg, s_client_uid client_uid);

		void		set_serialize_msg_new(packet_s2s& msg, google::protobuf::Message* proto_ptr,int32 header);
		void		set_serialize_msg_new(packet_c2s_s2c& msg, google::protobuf::Message* proto_ptr,int32 header);
		void		set_serialize_msg_new(packet_c2s_s2c& msg, google::protobuf::Message* proto_ptr, s_client_uid client_uid, int32 header);
	};

}

#endif
