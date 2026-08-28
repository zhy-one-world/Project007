/********************************************************************
  created: 2014/07/04
  created: 4:7:2014 17:57
  file base: parse_msg
  file ext: h
  author: YU REN
  
  purpose: 服务器解析客户端传递的googleprotocolbuff类型消息
*********************************************************************/
#ifndef parse_msg_h
#define parse_msg_h

#include "logic/type_def.hpp"
#include "base.hpp"

namespace hld
{
#pragma pack(push,1)
	const int32 role_db_data_len = 256 * 1024 * 1024;
	struct s_db_data_array
	{
		guid_64 role_guid;
		char data_array[role_db_data_len];

		s_db_data_array()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
#pragma pack(pop)
	class parse_msg
	{
	private:
		parse_msg();
	public:
		~parse_msg();
		static parse_msg& getInstance();
	public:
		bool parse_message_new(google::protobuf::Message* proto_ptr, const void* data_ptr, size_t data_len);
		bool parse_buffer_to_proto(google::protobuf::Message* proto_ptr, const void* data_ptr, size_t data_len, bool is_check_len = true);
		packet_s2s* parse_message_server(google::protobuf::Message* proto_ptr, const void* data_ptr, size_t data_len);
		void parse_clientuid_from_msg_new(const void* data_ptr, s_client_uid& client_uid);
		void parse_clientuid_from_msg_new(const void* data_ptr, int32& array_index);

		bool  is_internal_package(const void* data,size_t data_len);
		ui16  get_packet_header(const void* data,size_t data_len);
		s_client_uid  get_packet_connect_idx(const void* data, size_t data_len);
	public:
		packet_c2s_s2c* serialze_client(s_client_uid client_uid, google::protobuf::Message* netPro, int32 header);
		packet_s2s* serialze_server(google::protobuf::Message* netPro, int32 header, guid_64 logic_guid, int32 logic_index, int32 logic_ex = 0);
		packet_s2s* serialze_server(google::protobuf::Message* netPro, int32 header, int64 data_flag, guid_64 logic_guid, int32 logic_index, int32 logic_ex = 0);
		packet_s2s* serialze_server(const xchar* data_array, int32 data_size, int32 header, guid_64 logic_guid, int32 logic_index, int32 logic_ex = 0);
		packet_s2s* serialze_buffer(google::protobuf::Message* netPro);
		packet_s2s* get_server_msg() { return &m_server_msg; }

	public:
		xchar * get_cache(int32 size);
		void   my_memcopy(void* dst, size_t max_len, void const* src, size_t size);
		void   my_memcopy_string(void* dst, size_t max_len, const xstring &str);

	private:
		packet_c2s_s2c m_msg;
		packet_s2s m_server_msg;
		xchar db_cahce[INTERNAL_SERVER_MAX_PACKET_SIZE];

	};

#define PROTO_FUNC_TO_PACKET(FUNC, NAME)                \
	hld::ws2cs_proto::##NAME msg;          \
	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len,false); \
	if (is_sucess == false)   \
	{                        \
		return;               \
	}                          \
	ws2cs_##NAME packet; \
	packet.from_proto(msg);          \
	FUNC(conn_index, &packet, sizeof(ws2cs_##NAME)); \

#define PROTO_C_TO_S_FUNC(proto_name, func_name)  \
	proto_name  req;          \
	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&req, msg, msg_len, false); \
	if (is_sucess == false)  \
	{                        \
		return;               \
	}                         \
	func_name##_logic(player_ref, req); \


#define PROTO_C_WS_COMMON(PROTO, FUNC)  \
	PROTO req;                      \
	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&req, msg, msg_len, false); \
	if (is_sucess == false)  \
	{                        \
		return;              \
	}                         \
	FUNC##_logic(session, req);       \


#define PROTO_FUNC_CS_WS_PACKET(FUNC, NAME)                \
	hld::cs2ws_proto::##NAME msg;          \
	bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len, false); \
	if (is_sucess == false)   \
	{                        \
		return;               \
	}                          \
	cs2ws_##NAME packet; \
	packet.from_proto(msg);          \
	FUNC(conn_index, &packet, sizeof(cs2ws_##NAME)); \




	

}

#endif