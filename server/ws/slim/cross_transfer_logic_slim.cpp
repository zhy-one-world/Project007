/* slim stub for excluded cross_transfer_logic.cpp */
#include "cross_group/cross_transfer_logic.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"

namespace faith
{
	namespace cross
	{
		void transfer_player(client_session* pSession, int32 dstGroupID, const s_transfer_info& transfer_info)
		{
			(void)dstGroupID;
			(void)transfer_info;
			(void)pSession;
			// slim: cross-server transfer disabled
		}

		void send_msg_to_ws(guid_64 role_guid, int32 server_id, uint32 dataType, const void* pdata, uint32 len)
		{
			(void)role_guid;
			(void)server_id;
			(void)dataType;
			(void)pdata;
			(void)len;
		}

		void send_msg_to_ws(guid_64 role_guid, int32 server_id, uint32 dataType, const google::protobuf::Message* proto_ptr, uint32 header)
		{
			(void)role_guid;
			(void)server_id;
			(void)dataType;
			(void)proto_ptr;
			(void)header;
		}
	}
}
