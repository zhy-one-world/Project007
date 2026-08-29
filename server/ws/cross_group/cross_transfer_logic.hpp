/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-5-25 17:41
	
	File Name :	cross_transfer_logic.hpp
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

#ifndef _CROSS_TRANSFER_LOGIC_HPP_
#define _CROSS_TRANSFER_LOGIC_HPP_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include <gate_msg.hpp>
#include "../server/client_session.hpp"

namespace faith
{
	namespace cross
	{
		class guild;

		// player
		void transfer_player(client_session* pSession, int32 dstGroupID, const s_transfer_info& transfer_info); //, const uint32 callbackVal[]
		// utility
		void send_msg_to_ws(guid_64 role_guid, int32 server_id, uint32 dataType, const void* pdata, uint32 len);
		void send_msg_to_ws(guid_64 role_guid, int32 server_id, uint32 dataType, const google::protobuf::Message* proto_ptr, uint32 header = 0);
	}
}

#endif