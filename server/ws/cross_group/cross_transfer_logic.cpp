/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-5-25 17:44
	
	File Name :	cross_transfer_logic.cpp
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include "cross_transfer_logic.hpp"
#include "gate_proxy.hpp"
#include "world_server.hpp"
#include "../server/client_session_mgr.hpp"
#include <core.hpp>
#include "../server/team/team_ws_mgr.h"
#include "cross.pb.h"
#include "net.pb.h"

namespace faith
{
	namespace cross
	{

		//------------------------------------------------------------------------
		// Player
		//------------------------------------------------------------------------
		void transfer_player(client_session* pSession, int32 dstGroupID, const s_transfer_info& transfer_info)
		{
			if (!pSession || pSession->m_status!=client_session::e_ss_ingame)
				return;

			if (gate_proxy::getInstance().is_gate_run() == false)
			{
				client_session_mgr::getInstance().logout_complete(pSession);
				return ;
			}

			if (dstGroupID == world_server::getInstance().get_server_id())
			{
				client_session_mgr::getInstance().logout_complete(pSession);
				return ;
			}

			pSession->m_status = client_session::e_ss_cross_transfer;
			if (dstGroupID <= 0)
			{
				dstGroupID = pSession->get_role_info_data(e_role_info_server_id);
			}

			if (pSession->get_team_guid().is_valid())
			{
				team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(pSession->get_team_guid());
				if (nullptr != team_ws_ptr)
				{
					s_team_member_info* member_info_ptr = team_ws_ptr->get_member(pSession->get_role_guid());
					if (member_info_ptr != nullptr && member_info_ptr->data_ary[ETeamMemberInfo_agora_id] != 0)
					{
						team_ws_ptr->leave_voie_channel(pSession->get_role_guid());
					}
					//team_ws_ptr->del_member(pSession->get_role_guid());玩家跨服时不再删除队伍
					team_ws_ptr->set_is_need_save_team(true);
					team_ws_ptr->send_claer_team_member_message(pSession->get_role_guid());
				}
			}

			//WS2GATE_REQ_TRANSFER request;
			//TransferParam& param = request.param;
			//param.objType = EOBJ_TYPE_PLAYER;
			//param.objID = pSession->get_role_guid();
			//param.destGroupID = dstGroupID;
			//param.transfer_info = transfer_info;
			//strncpy(param.logicParam.playerParam.account, pSession->m_role_info.account, max_account_length);
			//strncpy(param.logicParam.playerParam.ipAddr, pSession->m_login_third_data.param6, max_ip_address_length);//该玩家IP
			//if (pSession->is_self_server())
			//{
			//	request.legion_info = pSession->get_legion_info();
			//	request.is_need_set_legion_info = true;
			//}
			//gate_proxy::getInstance().Send(&request, sizeof(request));

			s_game_info dst_server_info;
			s_game_info* game_info_ptr = world_server::getInstance().get_server_config_by_server_id(dstGroupID);
			if (nullptr == game_info_ptr)
			{
				game_info_ptr = world_server::getInstance().get_gate_server_config_by_server_id(dstGroupID);
				/*if (nullptr == game_info_ptr)
				{
					if (cross_ladder_ws_mgr_new::get_instance().get_transfer_game_info(pSession->get_role_guid(), dstGroupID, dst_server_info))
					{
						game_info_ptr = &dst_server_info;
					}
					else if (element_war_ws_mgr::get_instance().get_transfer_game_info(pSession->get_role_guid(), dstGroupID, dst_server_info))
					{
						game_info_ptr = &dst_server_info;
					}
				}*/
			}
			if (nullptr == game_info_ptr)
			{
				if (world_server::getInstance().is_sky_island_server() && dstGroupID == pSession->get_role_info_data(e_role_info_server_id))
				{
					pSession->set_role_info_data(e_role_info_move_server_id, pSession->get_role_info_data(e_role_info_server_id));
					client_session_mgr::getInstance().kickout_account(pSession->m_account, e_logout_result_enter_game_server_maintenance);
					return;
				}
				else
				{
					pSession->set_role_info_data(e_role_info_move_server_id, world_server::getInstance().get_server_id());
					client_session_mgr::getInstance().kickout_account(pSession->m_account, e_logout_result_enter_game_server_maintenance);

					//pSession->send_notice("90096612");
					//pSession->login_try_enter_scene();
				}
				return;
			}
			//发送跨服军团缓存数据
			ws2ws_cross_player_legion_info legion_info_msg;
			legion_info_msg.role_guid = pSession->get_role_guid();
			legion_info_msg.legion_info.legion_info = pSession->get_legion_info();
			cross::send_msg_to_ws(guid_64(), dstGroupID, e_msgindex_ws2ws_player_legion_info, &legion_info_msg, sizeof(legion_info_msg));
			cross_proto_cross_transfer cross_msg;
			cross_msg.set_server_id(pSession->get_role_info_data(e_role_info_server_id));
			for (int32 i = 0; i < TOKEN_LEN; ++i)
			{
				cross_msg.add_token(0);
			}
			cross_msg.set_role_guid(pSession->get_role_guid().server_64);
			cross_msg.set_out_ip(game_info_ptr->out_ip);
			cross_msg.set_out_port(boost::lexical_cast<xstring>(game_info_ptr->out_port));
			pSession->send_to_client(&cross_msg, e_msgindex_s2c_cross_transfer);
			client_session_mgr::getInstance().logout_complete(pSession);
		}
		//------------------------------------------------------------------------
		// Utility
		//------------------------------------------------------------------------
		void send_msg_to_ws(guid_64 role_guid, int32 server_id, uint32 dataType, const void* pdata, uint32 len)
		{
			static gate2server_to_server msg;
			msg.role_guid = role_guid;
			msg.server_id = server_id;
			msg.dataType = dataType;
			msg.dataLen = len;
			memcpy( msg.data, pdata, len );
			ws_client::getInstance().send_to_gate(&msg, msg.get_pak_length(), server_id);
		}

		void send_msg_to_ws(guid_64 role_guid, int32 server_id, uint32 dataType, const google::protobuf::Message* proto_ptr, uint32 header)
		{
			static gate2server_to_server msg;
			msg.role_guid = role_guid;
			msg.server_id = server_id;
			msg.dataType = dataType;
			msg.header = header;
			bool ret = proto_ptr->SerializeToArray((void*)msg.data, sizeof(msg.data));
			if (!ret)
			{
				return;
			}
			msg.dataLen = proto_ptr->ByteSize();
			ws_client::getInstance().send_to_gate(&msg, msg.get_pak_length(), server_id);
		}
	}
}