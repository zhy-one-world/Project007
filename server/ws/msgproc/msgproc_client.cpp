#include "assist_fight.pb.h"
#include "base/ecs_world.h"
#include "character.pb.h"
#include "chat.pb.h"
#include "cloud_shop.pb.h"
#include "cross_group/gate_proxy.hpp"
#include "game.pb.h"
#include "Logic/cross_server_harry_def.hpp"
#include "lua/script_mgr.h"
#include "msgproc_client.hpp"
#include "net.pb.h"
#include "ranking.pb.h"
#include "relation.pb.h"
#include "server//legion/legion_ws_answer.h"
#include "server/activity/broken_sky_mgr.h"
#include "server/activity/cloud_shop_mgr.h"
#include "server/activity/cross_server_city_war_ws_mgr.h"
#include "server/activity/cross_server_harry_ws_mgr.h"
#include "server/activity/cross_server_pk_ws_mgr.h"
#include "server/activity/cross_server_world_boss_ws_mgr.h"
#include "server/activity/crystal_fairyland_mgr.h"
#include "server/activity/element_war_ws_mgr.h"
#include "server/activity/gain_treasure_ws_mgr.h"
#include "server/activity/pk_king_mgr.h"
#include "server/activity/time_limit_activity_ws_mgr.h"
#include "server/activity/time_limit_activity_ws_mgr.h"
#include "server/appearance_info_mgr.h"
#include "server/assist_fight/assist_fight_mgr.h"
#include "server/best_record/best_record_mgr.h"
#include "server/client_session_mgr.hpp"
#include "server/legion/attack_city_ws_mgr.h"
#include "server/legion/legion_ws_mgr.h"
#include "server/ranking/ranking_mgr_ws.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "server/rename/rename_mgr.h"
#include "server/team/team_ws_mgr.h"
#include "server/web_client.hpp"
#include "server/world_boss/world_boss_ws_mgr.h"
#include "server_log.hpp"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "template/template_manager.h"
#include "time_limit_activity.pb.h"
#include "utility/init_unit.h"
#include "welfare.pb.h"
#include "world_server.hpp"
#include <marry_msg.hpp> 
#include <Utility/parse_msg.h>


namespace faith
{
	/////////////////////////////////////////team/////////////////////////////////////////

	//const int32 first_kill_welfare_type = 14;
	void c2ws_req_let_role_join_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_let_role_join_team req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		guid_64 team_guid(req.team_guid());
		ws2cs_join_team join_team_msg;
		join_team_msg.team_guid = team_guid;
		join_team_msg.role_guid = session->get_role_guid();
		session->send_to_cs(&join_team_msg, sizeof(join_team_msg));
	}

	void c2ws_req_leave_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_leave_team req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		guid_64 team_guid(req.team_guid());
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		s_team_member_info* member_info_ptr = team_ws_ptr->get_member(session->get_role_guid());
		if (member_info_ptr != nullptr && member_info_ptr->data_ary[ETeamMemberInfo_agora_id] != 0)
		{
			team_ws_ptr->leave_voie_channel(session->get_role_guid());
		}

		team_ws_ptr->del_member(session->get_role_guid());
	}

	void c2ws_req_kickout_team_member(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_kickout_team_member req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		// 获得要T的角色所在的队伍
		guid_64 team_guid(req.team_guid());
		if (team_guid.is_valid() == false)
		{
			return;
		}
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		// 如果当前要发起T人请求的角色不是队长就不能T人
		if (team_ws_ptr->get_captain_guid() != session->get_role_guid())
		{
			return;
		}

		guid_64 leaver_guid(req.leaver_guid());
		if (leaver_guid.is_valid() == false)
		{
			return;
		}
		//如果要T的人处于多人副本中，则不允许踢出副本
		client_session* leave_player_session = client_session_mgr::getInstance().get_session(leaver_guid);
		if (nullptr != leave_player_session)
		{
			auto map_template = cs_map_system::get_map_template(leave_player_session->get_map_guid());
			if (map_template && map_template->Order == e_map_order_type_multiplayer_raid)
			{
				session->send_notice("90096105");
				return;
			}
		}
		if (team_ws_ptr->del_member(leaver_guid) == true)
		{
			client_session* leaver_session = client_session_mgr::getInstance().get_session(leaver_guid);
			if (nullptr != leaver_session)
			{
				team_proto_kickout_team_member_end kickout_team_member_end_msg;
				leaver_session->send_to_client(&kickout_team_member_end_msg, e_msgindex_s2c_kickout_team_member_end);
			}
		}
	}

	void c2ws_req_set_team_captain(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_set_captain req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		guid_64 team_guid(req.team_guid());
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		// 如果不是队长就不能换队长
		if (team_ws_ptr->get_captain_guid() != session->get_role_guid())
		{
			return;
		}

		guid_64 new_captain_guid(req.new_captain_guid());
		team_ws_ptr->set_captain(new_captain_guid);
	}

	void c2ws_req_invite_player_join_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_invite_player req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		c2ws_req_invite_player_join_team_logic(session, req);

		//guid_64 team_guid(req.team_guid());
		//team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		//team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		//if (nullptr == team_ws_ptr)
		//{
		//	return;
		//}

		//// 如果不是队长就不能邀请
		///*if (team_ws_ptr->get_captain_guid() != session->get_role_guid())
		//{
		//	return;
		//}*/

		//guid_64 invitee_guid(req.invitee_guid());
		//if (invitee_guid.is_valid() == false)
		//{
		//	return;
		//}
		////邀请的玩家跟自己不是同服则禁止
		////if (false == session->is_self_server(invitee_guid, false))
		////{
		////	session->send_notice("90305139");
		////	return;
		////}
		////被邀请的玩家不在线
		//client_session* invitee_session = client_session_mgr::getInstance().get_session(invitee_guid);
		//if (nullptr == invitee_session)
		//{

		//	team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_player_already_offline);
		//	return;
		//}


		//// 被邀请的玩家已经在邀请者的队伍中
		//if (team_ws_ptr->get_member(invitee_guid) != nullptr)
		//{
		//	team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_player_already_join_team);
		//	return;
		//}

		//// 被邀请的玩家已经在其他队伍中
		//if (team_ws_mgr_ref.get_unit_team(invitee_guid) != nullptr)
		//{
		//	team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_player_already_join_other_team);
		//	return;
		//}

		//// 如果是队长邀请就加入邀请列表中
		//if (team_ws_ptr->get_captain_guid() == session->get_role_guid())
		//{
		//	team_ws_ptr->add_invite_guid(invitee_guid);
		//}

		//team_proto_invite_player_end invite_player_end_msg;
		//team_proto_team_brief_info* team_brief_info = invite_player_end_msg.add_inviter_team_info();
		//invite_player_end_msg.set_sender_guid(session->get_role_guid().server_64);
		//team_ws_ptr->get_team_brief_info_pkg(team_brief_info);
		//invitee_session->send_to_client(&invite_player_end_msg, e_msgindex_s2c_invite_player_end);

		//team_proto_invite_player_success invite_player_success_msg;
		//invite_player_success_msg.set_invitee_guid(invitee_guid.server_64);
		//session->send_to_client(&invite_player_success_msg, e_msgindex_s2c_invite_player_success);
	}

	void c2ws_req_invite_player_join_team_logic(client_session * session, team_proto_invite_player & req)
	{
		if (nullptr == session)
		{
			return;
		}

		guid_64 team_guid(req.team_guid());
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		// 如果不是队长就不能邀请
		/*if (team_ws_ptr->get_captain_guid() != session->get_role_guid())
		{
		return;
		}*/

		guid_64 invitee_guid(req.invitee_guid());
		if (invitee_guid.is_valid() == false)
		{
			return;
		}
		//邀请的玩家跟自己不是同服则禁止
		//if (false == session->is_self_server(invitee_guid, false))
		//{
		//	session->send_notice("90305139");
		//	return;
		//}
		//被邀请的玩家不在线
		client_session* invitee_session = client_session_mgr::getInstance().get_session(invitee_guid);
		if (nullptr == invitee_session)
		{

			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_player_already_offline);
			return;
		}


		// 被邀请的玩家已经在邀请者的队伍中
		if (team_ws_ptr->get_member(invitee_guid) != nullptr)
		{
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_player_already_join_team);
			return;
		}

		// 被邀请的玩家已经在其他队伍中
		if (team_ws_mgr_ref.get_unit_team(invitee_guid) != nullptr)
		{
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_player_already_join_other_team);
			return;
		}

		// 如果是队长邀请就加入邀请列表中
		if (team_ws_ptr->get_captain_guid() == session->get_role_guid())
		{
			team_ws_ptr->add_invite_guid(invitee_guid);
		}

		team_proto_invite_player_end invite_player_end_msg;
		team_proto_team_brief_info* team_brief_info = invite_player_end_msg.add_inviter_team_info();
		invite_player_end_msg.set_sender_guid(session->get_role_guid().server_64);
		team_ws_ptr->get_team_brief_info_pkg(team_brief_info);
		invitee_session->send_to_client(&invite_player_end_msg, e_msgindex_s2c_invite_player_end);

		team_proto_invite_player_success invite_player_success_msg;
		invite_player_success_msg.set_invitee_guid(invitee_guid.server_64);
		session->send_to_client(&invite_player_success_msg, e_msgindex_s2c_invite_player_success);
	}

	void c2ws_req_invite_player_join_team_lua(client_session * session, const char * msg, int32 msg_len)
	{

		 PROTO_C_WS_COMMON(team_proto_invite_player, c2ws_req_invite_player_join_team)
	}

	void c2ws_req_transfer_team_to_scene(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_transfer_team_to_scene req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();

		guid_64 team_guid(req.team_guid());
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		// 只有队长才能进行传送
		if (session->get_role_guid() != team_ws_ptr->get_captain_guid())
		{
			return;
		}

		e_team_error ret = team_ws_ptr->transfer_all_member_to_scene(req.map_template_id());
		if (e_team_error_none != ret)
		{
			team_ws_mgr_ref.send_team_error_to_session(session, ret);
		}
	}

	void c2ws_req_ready(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_ready req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		c2ws_req_ready_logic(session, req);

		//team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		//guid_64 team_guid(req.team_guid());
		//team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		//if (nullptr == team_ws_ptr)
		//{
		//	return;
		//}

		//e_team_type team_type = team_ws_ptr->get_team_type();
		//int32 sub_type_id = team_ws_ptr->get_team_sub_type_id();

		//s_team_member_info* member_info = team_ws_ptr->get_member(session->get_role_guid());
		//if (nullptr == member_info)
		//{
		//	return;
		//}

		////监测队伍中是否有跨服玩家
		//const std::list<s_team_member_info>& team_member_list = team_ws_ptr->get_member_list();
		//if (session->get_role_guid() == team_ws_ptr->get_captain_guid())
		//{
		//	bool is_have_other_server_player = false;
		//	for (const s_team_member_info& member_info : team_member_list)
		//	{
		//		client_session* session = client_session_mgr::getInstance().get_session(member_info.role_guid);
		//		if (nullptr == session)
		//		{
		//			continue;
		//		}
		//		if (false == session->is_self_server())
		//		{
		//			is_have_other_server_player = true;
		//			break;
		//		}
		//	}
		//	if (is_have_other_server_player)
		//	{
		//		for (const s_team_member_info& member_info : team_member_list)
		//		{
		//			client_session* session = client_session_mgr::getInstance().get_session(member_info.role_guid);
		//			if (nullptr == session)
		//			{
		//				continue;
		//			}
		//			session->send_notice("90096425");
		//		}
		//		return;
		//	}
		//}

		//if (team_ws_ptr->get_team_type() == e_team_type_one_dragon || team_ws_ptr->get_team_type()==e_team_type_none)
		//{
		//	return;
		//}
		////MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, team_ws_ptr->get_team_sub_type_id());
		////if (nullptr != map_template_ptr && map_template_ptr->Order == e_map_order_type_multiplayer_raid)
		////{
		////	//多人副本进入判断
		////	bool m_result = team_ws_ptr->check_all_commmon_legion();
		////	if (!m_result)
		////	{
		////		return;
		////	}
		////}

		//bool is_ready = (req.is_ready() > 0 ? true : false);
		//if (is_ready && team_ws_mgr_ref.is_team_type_raid(team_ws_ptr->get_team_type()))
		//{
		//	//目前这个检测只有raid类型的队伍用
		//	team_ws_mgr_ref.req_condition_check_to_cs(session, e_team_common_check_invoker_mem_ready, team_type, sub_type_id, team_guid);
		//}
		//else
		//{
		//	team_ws_ptr->set_member_ready_status(session->get_role_guid(), is_ready);
		//}
	}

	void c2ws_req_ready_logic(client_session * session, team_proto_ready & req)
	{
		if (nullptr == session)
		{
			return;
		}

		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		guid_64 team_guid(req.team_guid());
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		e_team_type team_type = team_ws_ptr->get_team_type();
		int32 sub_type_id = team_ws_ptr->get_team_sub_type_id();

		s_team_member_info* member_info = team_ws_ptr->get_member(session->get_role_guid());
		if (nullptr == member_info)
		{
			return;
		}

		//监测队伍中是否有跨服玩家
		const std::list<s_team_member_info>& team_member_list = team_ws_ptr->get_member_list();
		if (session->get_role_guid() == team_ws_ptr->get_captain_guid())
		{
			bool is_have_other_server_player = false;
			for (const s_team_member_info& member_info : team_member_list)
			{
				client_session* session = client_session_mgr::getInstance().get_session(member_info.role_guid);
				if (nullptr == session)
				{
					continue;
				}
				if (false == session->is_self_server())
				{
					is_have_other_server_player = true;
					break;
				}
			}
			if (is_have_other_server_player)
			{
				for (const s_team_member_info& member_info : team_member_list)
				{
					client_session* session = client_session_mgr::getInstance().get_session(member_info.role_guid);
					if (nullptr == session)
					{
						continue;
					}
					session->send_notice("90096425");
				}
				return;
			}
		}

		if (team_ws_ptr->get_team_type() == e_team_type_one_dragon || team_ws_ptr->get_team_type() == e_team_type_none)
		{
			return;
		}
		//MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, team_ws_ptr->get_team_sub_type_id());
		//if (nullptr != map_template_ptr && map_template_ptr->Order == e_map_order_type_multiplayer_raid)
		//{
		//	//多人副本进入判断
		//	bool m_result = team_ws_ptr->check_all_commmon_legion();
		//	if (!m_result)
		//	{
		//		return;
		//	}
		//}

		bool is_ready = (req.is_ready() > 0 ? true : false);
		if (is_ready && team_ws_mgr_ref.is_team_type_raid(team_ws_ptr->get_team_type()))
		{
			//目前这个检测只有raid类型的队伍用
			team_ws_mgr_ref.req_condition_check_to_cs(session, e_team_common_check_invoker_mem_ready, team_type, sub_type_id, team_guid);
		}
		else
		{
			team_ws_ptr->set_member_ready_status(session->get_role_guid(), is_ready);
		}
	}

	void c2ws_req_ready_lua(client_session * session, const char * msg, int32 msg_len)
	{
		 PROTO_C_WS_COMMON(team_proto_ready, c2ws_req_ready)
	}
	
	void c2ws_req_ready_transfer(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
//		s_client_uid client_uid;
//		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
// 		team_proto_ready_to_transfer_team req;
// 		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
// 
// 		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
// 		if (nullptr == session)
// 		{
// 			return;
// 		}
// 
// 		guid_64 team_guid(req.team_guid());
// 		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
// 		if (nullptr == team_ws_ptr)
// 		{
// 			return;
// 		}
// 
// 		// 只有队长才能开始准备传送
// 		if (session->get_role_guid() != team_ws_ptr->get_captain_guid())
// 		{
// 			return;
// 		}
// 
// 		team_ws_ptr->ready_transfer_all_member_to_scene();
	}

	void c2ws_req_search_team_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_search_team_by_map_id req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		c2ws_req_search_team_list_logic(session, req);

		//int32 map_template_id = req.map_template_id();

		//MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		//if (nullptr == map_template_ptr)
		//{
		//	return;
		//}

		//team_ws* cur_team = team_ws_mgr::get_instance().get_unit_team(session->get_role_guid());
		//std::vector<team_ws*> team_ws_list;
		//team_ws_mgr::get_instance().get_team_ws_list_by_map_id(map_template_id, team_ws_list);

		//team_proto_search_team_by_map_id_end search_team_end_msg;
		//search_team_end_msg.set_map_template_id(map_template_id);
		//search_team_end_msg.set_team_list_len(0);
		//int32 team_list_len = 0;
		//for (team_ws* team_ws_ptr : team_ws_list)
		//{
		//	// 不返回玩家所在的队伍
		//	if (nullptr != cur_team)
		//	{
		//		if (team_ws_ptr != nullptr && cur_team->get_team_guid() == team_ws_ptr->get_team_guid())
		//		{
		//			continue;
		//		}
		//	}

		//	// 战斗力不足
		//	int32 required_fighting_power = team_ws_ptr->get_required_fighting_power();
		//	if (session->get_role_gs_value() < required_fighting_power)
		//	{
		//		continue;
		//	}
		//	int32 required_level = team_ws_ptr->get_required_level();
		//	if (session->m_role_info.data_ary[e_role_info_exp_level] < required_level)
		//	{
		//		continue;
		//	}

		//	team_proto_team_brief_info* team_brief_info_msg = search_team_end_msg.add_team_list();
		//	team_ws_ptr->get_team_brief_info_pkg(team_brief_info_msg);

		//	++team_list_len;
		//}
		//search_team_end_msg.set_team_list_len(team_list_len);

		//session->send_to_client(&search_team_end_msg, e_msgindex_s2c_search_team_by_map_id_end);
	}

	void c2ws_req_search_team_list_logic(client_session * session, team_proto_search_team_by_map_id & req)
	{
		if (nullptr == session)
		{
			return;
		}

		int32 map_template_id = req.map_template_id();

		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, map_template_id);
		if (nullptr == map_template_ptr)
		{
			return;
		}

		team_ws* cur_team = team_ws_mgr::get_instance().get_unit_team(session->get_role_guid());
		std::vector<team_ws*> team_ws_list;
		team_ws_mgr::get_instance().get_team_ws_list_by_map_id(map_template_id, team_ws_list);

		team_proto_search_team_by_map_id_end search_team_end_msg;
		search_team_end_msg.set_map_template_id(map_template_id);
		search_team_end_msg.set_team_list_len(0);
		int32 team_list_len = 0;
		for (team_ws* team_ws_ptr : team_ws_list)
		{
			// 不返回玩家所在的队伍
			if (nullptr != cur_team)
			{
				if (team_ws_ptr != nullptr && cur_team->get_team_guid() == team_ws_ptr->get_team_guid())
				{
					continue;
				}
			}

			// 战斗力不足
			int32 required_fighting_power = team_ws_ptr->get_required_fighting_power();
			if (session->get_role_gs_value() < required_fighting_power)
			{
				continue;
			}
			int32 required_level = team_ws_ptr->get_required_level();
			if (session->m_role_info.data_ary[e_role_info_exp_level] < required_level)
			{
				continue;
			}

			team_proto_team_brief_info* team_brief_info_msg = search_team_end_msg.add_team_list();
			team_ws_ptr->get_team_brief_info_pkg(team_brief_info_msg);

			++team_list_len;
		}
		search_team_end_msg.set_team_list_len(team_list_len);

		session->send_to_client(&search_team_end_msg, e_msgindex_s2c_search_team_by_map_id_end);
	}

	void c2ws_req_search_team_list_lua(client_session * session, const char * msg, int32 msg_len)
	{
		PROTO_C_WS_COMMON(team_proto_search_team_by_map_id, c2ws_req_search_team_list)
	}

	void c2ws_req_get_no_team_legionmate_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_get_no_team_mate_list req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		team_ws_mgr::get_instance().req_no_team_mate_list(session, (e_team_req_mate_type)req.mate_type());
	}

	void c2ws_refused_join_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_refused_to_join_team req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == req.has_tar_player_guid())
		{
			return;
		}
		guid_64 tar_guid = req.tar_player_guid();
		client_session* tar_session = client_session_mgr::getInstance().get_session(tar_guid);
		if (nullptr == tar_session)
		{
			return;
		}
		team_proto_refused_to_join_team_end end_msg;
		end_msg.set_tar_player_guid(session->get_role_guid().server_64);
		tar_session->send_to_client(&end_msg, e_msgindex_s2c_refused_join_team_end);
	}

	void c2ws_req_apply_to_join_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_join_team_apply req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (session->m_is_in_assist)
		{
			session->send_notice("90204219");
			return;
		}
		e_join_team_way join_team_way = (e_join_team_way)req.join_team_way();
		guid_64 given_guid = req.given_guid();
		guid_64 captain_guid = guid_64();
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = nullptr;
		switch (join_team_way)
		{
			// 通过队伍的ID来获取队伍
		case e_join_team_way_by_team_guid:
			team_ws_ptr = team_ws_mgr_ref.get_team(given_guid);
			if (nullptr == team_ws_ptr)
			{
				team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_team_already_dissolve);
				return;
			}
			captain_guid = team_ws_ptr->get_captain_guid();
			break;

			// 通过队长的ID来获取队伍
		case e_join_team_way_by_captain_guid:
			team_ws_ptr = team_ws_mgr_ref.get_unit_team(given_guid);
			if (nullptr == team_ws_ptr)
			{
				team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_target_player_not_in_team);
				return;
			}
			if (team_ws_ptr->get_captain_guid() != given_guid)
			{
				team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_target_player_is_not_captain);
				return;
			}
			captain_guid = given_guid;
			break;

		default:
			return;
		}

		// 申请者已经在某个队伍中
		if (team_ws_mgr_ref.get_unit_team(session->get_role_guid()) != nullptr)
		{
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_player_already_join_other_team);
			return;
		}

		// 当队伍已经进入多人本等副本的时候，无法申请入队
		if (team_ws_ptr->get_team_state() == e_team_state_game && init_unit::is_team_type_raid_map(team_ws_ptr->get_team_type()))
		{
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_raidmulti_ingame);
			return;
		}
		//if (false == session->is_self_server(captain_guid, false))
		//{
		//	session->send_notice("90305139");
		//	return;
		//}
		//由于ETeamMemberInfo_check_ret需要去cs上检测 这里的是不确定的 这个逻辑暂时也没用 先屏蔽掉
		//if (marry_mgr_ws::get_instance().is_couple(session->get_role_guid(), team_ws_ptr->get_captain_guid()))
		//{
		//	s_team_member_info new_mem_info;
		//	team_ws_mgr_ref.gen_player_team_mem_info_ws(session, new_mem_info);
		//	if (team_ws_ptr->add_member(new_mem_info))
		//	{
		//		team_ws_ptr->send_add_team_member_message(new_mem_info);
		//	}
		//}

		team_ws_ptr->req_add_member_to_cs(session);
	}

	void c2ws_req_get_team_list_by_aim(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_get_team_list_by_aim req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		c2ws_req_get_team_list_by_aim_logic(session, req);

		//e_team_type team_type_id = (e_team_type)req.team_type_id();
		////if (init_unit::is_team_type_raid_map(team_type_id) && false == session->is_self_server())
		////{
		////	session->send_notice("90305139");
		////	return;
		////}
		//team_proto_get_team_list_by_aim_end get_team_list_end_msg;

		//if (req.team_sub_type_id_size() <= 0)
		//{
		//	return;
		//}
		//if (team_type_id > e_team_type_none)
		//{
		//	int32 sub_type_id = req.team_sub_type_id(0);
		//	team_ws_mgr::get_instance().fill_team_list_msg_by_aim(get_team_list_end_msg, team_type_id, sub_type_id, session);
		//}
		//else
		//{
		//	unit_index_map sub_ids;
		//	for (int32 i = 0; i < req.team_sub_type_id_size(); i++)
		//	{
		//		sub_ids.insert({ req.team_sub_type_id(i) , i});
		//	}
		//	team_ws_mgr::get_instance().fill_team_list_msg_all(get_team_list_end_msg, sub_ids, session);
		//}
		//session->send_to_client(&get_team_list_end_msg, e_msgindex_s2c_get_aim_team_list_end);
	}

	void c2ws_req_get_team_list_by_aim_logic(client_session * session, team_proto_get_team_list_by_aim & req)
	{
		if (nullptr == session)
		{
			return;
		}

		e_team_type team_type_id = (e_team_type)req.team_type_id();
		//if (init_unit::is_team_type_raid_map(team_type_id) && false == session->is_self_server())
		//{
		//	session->send_notice("90305139");
		//	return;
		//}
		team_proto_get_team_list_by_aim_end get_team_list_end_msg;

		if (req.team_sub_type_id_size() <= 0)
		{
			return;
		}
		if (team_type_id > e_team_type_none)
		{
			int32 sub_type_id = req.team_sub_type_id(0);
			team_ws_mgr::get_instance().fill_team_list_msg_by_aim(get_team_list_end_msg, team_type_id, sub_type_id, session);
		}
		else
		{
			unit_index_map sub_ids;
			for (int32 i = 0; i < req.team_sub_type_id_size(); i++)
			{
				sub_ids.insert({ req.team_sub_type_id(i) , i });
			}
			team_ws_mgr::get_instance().fill_team_list_msg_all(get_team_list_end_msg, sub_ids, session);
		}
		session->send_to_client(&get_team_list_end_msg, e_msgindex_s2c_get_aim_team_list_end);
	}

	void c2ws_req_get_team_list_by_aim_lua(client_session * session, const char * msg, int32 msg_len)
	{
		 PROTO_C_WS_COMMON(team_proto_get_team_list_by_aim, c2ws_req_get_team_list_by_aim)
	}

	void c2ws_req_auto_match_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_auto_match_team req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		e_team_type team_type_id = (e_team_type)req.team_type_id();
		int32 sub_type_id = req.team_sub_type_id();
		bool is_begin_match = req.match_state();

		if (team_type_id <= e_team_type_none || team_type_id >= e_team_type_max)
		{
			return;
		}
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		if (false == team_ws_mgr_ref.is_team_type_valid(team_type_id, sub_type_id))
		{
			return;
		}

		int32 match_ret = e_player_match_team_none;
		if (is_begin_match)
		{
			//所有类型的自动匹配都需要先去cs上检查一下
			//if (team_ws_mgr_ref.is_team_type_raid(team_type_id) || e_team_type_one_dragon == team_type_id)
			//{
			team_ws_mgr_ref.req_condition_check_to_cs(session, e_team_common_check_invoker_auto_match_team, team_type_id, sub_type_id, guid_64());
			//	return;
			//}
			//match_ret = team_ws_mgr_ref.player_auto_match_team(team_type_id, sub_type_id, session);
		}
		else
		{
			team_ws_mgr_ref.player_stop_auto_match_team(session->get_role_guid());
			match_ret = e_player_match_team_none;
		}
		team_proto_auto_match_team_end match_team_end_msg;
		match_team_end_msg.set_match_ret(match_ret);
		session->send_to_client(&match_team_end_msg, e_msgindex_s2c_auto_match_team_end);
	}

	void c2ws_req_update_team_attribute(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_update_team_attribute req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_unit_team(session->get_role_guid());
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		if (session->get_role_guid() != team_ws_ptr->get_captain_guid())
		{
			return;
		}

		e_team_type team_type_id = (e_team_type)req.team_type_id();
		int32 team_sub_type_id = req.team_sub_type_id();
		bool is_auto_match = req.is_auto_match();
		int32 limit_fight_power = req.limit_fight_power();
		int32 limit_level = req.limit_level();
		
		team_proto_update_team_attribute_end update_team_attr_end_msg;

		if (false == req.has_team_sub_type_id())
		{
			// sub_type_id放前面 因为后面有可能改
			team_sub_type_id = team_ws_ptr->get_team_sub_type_id();
		}
		
		bool aoi_info_changed = false;
		if (req.has_team_type_id())
		{
			if (e_team_type_one_dragon == team_type_id)
			{
				if (team_ws_ptr->is_one_stop_team())
				{
					//一条龙不可以手动改目标
					return;
				}
				else
				{
					int32 cap_os_flags = team_ws_ptr->get_member_one_stop_flags(team_ws_ptr->get_captain_guid());
					team_ws_ptr->set_one_stop_flags(cap_os_flags);
					team_ws_mgr_ref.get_team_aims_by_os_flags(cap_os_flags, team_type_id, team_sub_type_id);
				}
			}
			else
			{
				team_ws_ptr->set_one_stop_flags(-1); //一条龙修改目标后就不是一条龙了
			}

			update_team_attr_end_msg.set_n_one_stop_flags(team_ws_ptr->get_one_stop_flags());
			aoi_info_changed = true;
		}
		else
		{
			team_type_id = team_ws_ptr->get_team_type();
		}
		
		bool attr_changed = team_ws_ptr->change_team_aim(team_type_id, team_sub_type_id);
		if (attr_changed)
		{
			update_team_attr_end_msg.set_n_team_type_id(team_ws_ptr->get_team_type());
			update_team_attr_end_msg.set_n_team_sub_type_id(team_ws_ptr->get_team_sub_type_id());
			aoi_info_changed = true;
		}
		
		if (false == req.has_is_auto_match())
		{
			is_auto_match = team_ws_ptr->is_auto_matching();
		}
		if (team_ws_ptr->get_team_type() == e_team_type_none)
		{
			is_auto_match = false;
		}
		if (is_auto_match != team_ws_ptr->is_auto_matching())
		{
			team_ws_ptr->set_auto_matching(is_auto_match);
			update_team_attr_end_msg.set_n_is_auto_match(team_ws_ptr->is_auto_matching());
			attr_changed = true;
		}

		if (req.has_limit_fight_power() && limit_fight_power != team_ws_ptr->get_required_fighting_power())
		{
			team_ws_ptr->set_required_fighting_power(limit_fight_power);
			update_team_attr_end_msg.set_n_limit_fight_power(team_ws_ptr->get_required_fighting_power());
			attr_changed = true;
		}

		if (req.has_limit_level() && limit_level != team_ws_ptr->get_required_level())
		{
			team_ws_ptr->set_required_level(limit_level);
			update_team_attr_end_msg.set_n_limit_level(team_ws_ptr->get_required_level());
			attr_changed = true;
		}

		if (attr_changed)
		{
			team_ws_ptr->on_team_attribute_changed();
			team_ws_mgr_ref.notice_all_player_team_created(*team_ws_ptr);
		}
		if (aoi_info_changed)
		{
			team_ws_ptr->send_all_member_aoi_team_info();
		}
		
		//team_ws_ptr->sync_team_attribute_to_mems();
		team_ws_ptr->send_message_to_all_member(&update_team_attr_end_msg, e_msgindex_s2c_update_team_attribute_end);
	}
	
	void c2ws_req_team_mem_operate(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		team_proto_team_mem_operate req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		c2ws_req_team_mem_operate_logic(session, req);

		//guid_64 team_mem_guid(req.role_guid());

		//team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		//team_ws* team_ws_ptr = team_ws_mgr_ref.get_unit_team(team_mem_guid);
		//if (nullptr == team_ws_ptr)
		//{
		//	return;
		//}

		//e_team_operate operate_type = (e_team_operate)req.operate_type();
		//
		//team_proto_team_mem_operate_end team_mem_operate_end_msg;
		//team_mem_operate_end_msg.set_req_role_guid(team_mem_guid.server_64);
		//team_mem_operate_end_msg.set_operate_type(operate_type);

		//int32 paramas_size = req.operate_params_size();

		//for (int32 i = 0; i < paramas_size; i++)
		//{
		//	team_mem_operate_end_msg.add_operate_params(req.operate_params(i));
		//}
		//switch (operate_type)
		//{
		//case faith::e_team_operate_follow_cap:
		//{
		//	s_team_member_info* team_member = team_ws_ptr->get_member(team_mem_guid);
		//	if (nullptr == team_member)
		//	{
		//		return;
		//	}
		//	if (paramas_size == 0)
		//	{
		//		return;
		//	}
		//	team_member->data_ary[ETeamMemberInfo_follow_state] = req.operate_params(0);
		//	break;
		//}
		//case faith::e_team_operate_urge_cap:
		//	break;
		//case faith::e_team_operate_urge_together:
		//	break;
		//case faith::e_team_operate_call_together:
		//	break;		
		//case faith::e_team_operate_max:
		//	break;
		//default:
		//	break;
		//}
		//team_ws_ptr->send_message_to_all_member(&team_mem_operate_end_msg, e_msgindex_s2c_update_team_mem_operate_end);
	}

	void c2ws_req_team_mem_operate_logic(client_session * session, team_proto_team_mem_operate & req)
	{
		if (nullptr == session)
		{
			return;
		}

		guid_64 team_mem_guid(req.role_guid());

		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_unit_team(team_mem_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		e_team_operate operate_type = (e_team_operate)req.operate_type();

		team_proto_team_mem_operate_end team_mem_operate_end_msg;
		team_mem_operate_end_msg.set_req_role_guid(team_mem_guid.server_64);
		team_mem_operate_end_msg.set_operate_type(operate_type);

		int32 paramas_size = req.operate_params_size();

		for (int32 i = 0; i < paramas_size; i++)
		{
			team_mem_operate_end_msg.add_operate_params(req.operate_params(i));
		}
		switch (operate_type)
		{
		case faith::e_team_operate_follow_cap:
		{
			s_team_member_info* team_member = team_ws_ptr->get_member(team_mem_guid);
			if (nullptr == team_member)
			{
				return;
			}
			if (paramas_size == 0)
			{
				return;
			}
			team_member->data_ary[ETeamMemberInfo_follow_state] = req.operate_params(0);
			break;
		}
		case faith::e_team_operate_urge_cap:
			break;
		case faith::e_team_operate_urge_together:
			break;
		case faith::e_team_operate_call_together:
			break;
		case faith::e_team_operate_max:
			break;
		default:
			break;
		}
		team_ws_ptr->send_message_to_all_member(&team_mem_operate_end_msg, e_msgindex_s2c_update_team_mem_operate_end);
	}

	void c2ws_req_team_mem_operate_lua(client_session * session, const char * msg, int32 msg_len)
	{
		 PROTO_C_WS_COMMON(team_proto_team_mem_operate, c2ws_req_team_mem_operate)
	}


	///////////////////////////////////////传送/////////////////////////////////////////////////////////////////////////////

	void c2ws_req_get_best_record(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_get_best_record req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		c2ws_req_get_best_record_logic(session, req);

		//int32 map_template_id = req.map_template_id();
		//if (map_template_id <= 0)
		//{
		//	return;
		//}

		//game_proto_get_best_record_end get_best_record_end_msg;

		//s_best_record* record = best_record_mgr::get_instance().get_record_by_template_id(map_template_id);
		//if (nullptr == record)
		//{
		//	get_best_record_end_msg.set_map_template_id(0);
		//}
		//else
		//{
		//	get_best_record_end_msg.set_map_template_id(record->map_template_id);
		//	for (int32 i = 0; i < faith::single_map_record_num; i++)
		//	{
		//		if (record->single_map_best_record[i].role_guid.is_valid() == false)
		//		{
		//			break;
		//		}
		//		get_best_record_end_msg.add_best_record_role_name(record->single_map_best_record[i].role_name);
		//		get_best_record_end_msg.add_best_record_value(record->single_map_best_record[i].value);
		//	}
		//}

		//session->send_to_client(&get_best_record_end_msg, e_msgindex_s2c_get_best_record_end);
	}

	void c2ws_req_get_best_record_logic(client_session * session, game_proto_get_best_record & req)
	{

		if (nullptr == session)
		{
			return;
		}

		int32 map_template_id = req.map_template_id();
		if (map_template_id <= 0)
		{
			return;
		}

		game_proto_get_best_record_end get_best_record_end_msg;

		s_best_record* record = best_record_mgr::get_instance().get_record_by_template_id(map_template_id);
		if (nullptr == record)
		{
			get_best_record_end_msg.set_map_template_id(0);
		}
		else
		{
			get_best_record_end_msg.set_map_template_id(record->map_template_id);
			for (int32 i = 0; i < faith::single_map_record_num; i++)
			{
				if (record->single_map_best_record[i].role_guid.is_valid() == false)
				{
					break;
				}
				get_best_record_end_msg.add_best_record_role_name(record->single_map_best_record[i].role_name);
				get_best_record_end_msg.add_best_record_value(record->single_map_best_record[i].value);
			}
		}

		session->send_to_client(&get_best_record_end_msg, e_msgindex_s2c_get_best_record_end);
	}

	void c2ws_req_get_best_record_lua(client_session * session, const char * msg, int32 msg_len)
	{
		PROTO_C_WS_COMMON(game_proto_get_best_record, c2ws_req_get_best_record)
	}

	/////////////////////////////////////////map/////////////////////////////////////////

	void c2ws_req_transfer_to_shui_jing_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_transfer_to_pk_king_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_self_server() && false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity))
		{
			return;
		}
		crystal_fairyland_mgr& crystal_mgr_ref = crystal_fairyland_mgr::get_instance();
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity))//本服水晶幻境
		{
			auto ret = crystal_mgr_ref.is_can_join();
			if (e_error_code_success != ret)
			{
				cs_map_mgr_system::send_map_error_to_client(session, ret);
				return;
			}

			int32 war_idx = 0;
			guid_64 shui_jing_map_guid = crystal_mgr_ref.get_valid_shui_jing_map(war_idx);
			if (false == shui_jing_map_guid.is_valid())
			{
				cs_map_mgr_system::send_map_error_to_client(session, e_error_code_map_init_map_err);
				return;
			}
			cs_map_mgr_system::transfer_to_map(session, shui_jing_map_tempplate_id, shui_jing_map_guid, war_idx);
		}
		else if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity))//跨服水晶幻境逻辑
		{
			if (crystal_mgr_ref.is_activity_open() == false)
			{
				session->send_notice("90090360");
				return;
			}
			cross_server_pk_ws_mgr::get_instance().send_to_gate_transfer_map(0, session->get_role_guid(), (int32)e_activity_type_crystal_fairyland);
		}
	}

	void c2ws_req_transfer_to_pk_king_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_transfer_to_pk_king_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_self_server() && false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity))
		{
			return;
		}
		MapTemplate* map_temp_ptr = GET_TEMPLATE(MapTemplate, pk_king_map_template_id);
		if (nullptr == map_temp_ptr)
		{
			return;
		}
		if (session->get_role_info_data(e_role_info_exp_level) < map_temp_ptr->EnterLevel)
		{
			session->send_notice("90090228");
			return;
		}
		pk_king_mgr& pk_king_mgr_ref = pk_king_mgr::get_instance();
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity))//本服PK之王逻辑
		{
			e_error_code ret = pk_king_mgr_ref.is_can_join(session->get_role_guid());
			if (e_error_code_success != ret)
			{
				cs_map_mgr_system::send_map_error_to_client(session, ret);
				if (e_error_code_map_pk_king_ready_not_join_time == ret)
				{
					session->send_notice("90305000");
				}
				return;
			}

			int32 war_idx = 0;
			guid_64 pk_map_guid;
			pk_king_mgr_ref.get_valid_pkking_map(war_idx, pk_map_guid);
			if (false == pk_map_guid.is_valid())
			{
				cs_map_mgr_system::send_map_error_to_client(session, e_error_code_map_init_map_err);
				return;
			}
			cs_map_mgr_system::transfer_to_map(session, pk_king_map_template_id, pk_map_guid, war_idx);
		}
		else if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity))//跨服PK之王逻辑
		{
			if (pk_king_mgr_ref.is_in_notice_time())
			{
				session->send_notice("90305000");
				return;
			}
			
			if (pk_king_mgr_ref.is_in_ready_time() == false)
			{
				cs_map_mgr_system::send_map_error_to_client(session, e_error_code_map_pk_king_ready_end);
				return;
			}
			cross_server_pk_ws_mgr::get_instance().send_to_gate_transfer_map(0, session->get_role_guid(), (int32)e_activity_type_pk_king);
		}
	}
	void c2ws_get_world_boss_damage_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_get_world_boss_damage req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		world_boss_ws_mgr::get_instance().send_damage_list(session,req.boss_id());
	}
	void c2ws_req_transfer_to_cross_pk_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_cross_server_pk_map_transfer req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		int32 class_type = session->get_role_info_data(e_role_info_class_type);
		cross_server_pk_ws_mgr& pk_mgr_ref = cross_server_pk_ws_mgr::get_instance();
		if (false == gate_proxy::getInstance().is_gate_run()
			|| (true == gate_proxy::getInstance().is_gate_run() && world_server::getInstance().is_sky_island_server()))
		{
			int32 war_idex = 0;
			e_error_code ret = pk_mgr_ref.is_can_join(session->get_role_guid());
			int32 map_temp_id = pk_mgr_ref.get_map_temp_id_with_class_type(class_type);
			if (e_error_code_success == ret)
			{
				guid_64 map_guid;
				pk_mgr_ref.get_pk_king_map_guid(class_type, map_guid, war_idex);
				cs_map_mgr_system::transfer_to_map(session, map_temp_id, map_guid, war_idex);//, world_server::getInstance().get_cross_id()
			}
		}
		if (true == gate_proxy::getInstance().is_gate_run())
		{
			e_error_code ret = pk_mgr_ref.is_can_join_with_cross(session->get_role_guid());
			if (e_error_code_success == ret)
			{
				pk_mgr_ref.send_to_gate_transfer_map(class_type, session->get_role_guid(), (int32)e_activity_type_cross_server_pk);
			}
		}

		//else
		//{
		//	cs_map_mgr_system::send_map_error_to_client(session, ret);
		//}
	}

	void c2ws_req_transfer_to_harry_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_transfer_to_harry_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry))
		{
			session->send_notice("90090360");
			return;
		}
		int32 server_id = req.server_id();
		if (true == gate_proxy::getInstance().is_gate_run()
			&& world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry)
			&& server_id == world_server::getInstance().get_server_id())
		{
			bool can_join = cross_server_harry_ws_mgr::get_instance().is_can_join_harry_map(session);
			if (!can_join)
			{
				return;
			}
			guid_64 map_guid;
			int32 war_idex;
			cross_server_harry_ws_mgr::get_instance().get_valid_harry_map(map_guid, war_idex);
			if (!map_guid.is_valid())
			{
				cross_server_harry_ws_mgr::get_instance().init_harry_map();
				cross_server_harry_ws_mgr::get_instance().get_valid_harry_map(map_guid, war_idex);
			}
			if (!map_guid.is_valid())
			{
				session->send_notice("90090579");
				return;
			}
			cs_map_mgr_system::transfer_to_map(session, harry_map_id, map_guid, war_idex, server_id);
		}
		else if (true == gate_proxy::getInstance().is_gate_run()
			&& world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry)
			&& server_id != world_server::getInstance().get_server_id()
			&& server_id != world_server::getInstance().get_cross_id())
		{
			bool can_join = cross_server_harry_ws_mgr::get_instance().is_can_join_harry_map(session);
			if (!can_join)
			{
				return;
			}
			cross_server_pk_ws_mgr::get_instance().send_to_gate_transfer_map(0, session->get_role_guid(), (int32)e_activity_type_cross_server_harry, server_id);
		}
	}

	void c2ws_req_transfer_to_broken_sky_map(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_transfer_to_broken_sky_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_self_server())
		{
			return;
		}

		int32 map_template_id = req.map_id();
		broken_sky_mgr& broken_sky_mgr_ref = broken_sky_mgr::get_instance();
		e_error_code ret = broken_sky_mgr_ref.is_can_join();
		if (e_error_code_success == ret)
		{
			cs_map_mgr_system::transfer_to_map(session, map_template_id, guid_64());
		}
		else
		{
			cs_map_mgr_system::send_map_error_to_client(session, ret);
		}
	}

	void c2ws_req_get_one_map_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_get_one_map_info req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_vaild())
		{
			return;
		}

		guid_64 role_guid = session->get_role_guid();

		MapTemplate* map_template_ptr = GET_TEMPLATE(MapTemplate, req.map_template_id());
		if (nullptr == map_template_ptr)
		{
			return;
		}
		e_map_type operate_map_type = (e_map_type)map_template_ptr->Type;
		switch (operate_map_type)
		{
		case faith::e_map_type_legion_boss:
		case faith::e_map_type_legion_station:
		{
			legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
			legion_ws* legion_ws_ptr = legion_ws_mgr_ref.get_unit_legion(role_guid);
			if (nullptr == legion_ws_ptr)
			{
				return;
			}
			legion_ws_ptr->send_legion_boss_map_info(role_guid);
			break;
		}
		case faith::e_map_type_king_of_pk:
		{
			pk_king_mgr& pk_king_mgr_ref = pk_king_mgr::get_instance();
			pk_king_mgr_ref.send_player_num_in_map_to_client(role_guid);
			break;
		}
		default:
			break;
		}
	}

	void c2ws_get_welfare_activatecode(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		welfare_proto_get_reward_activatecode req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		int64 now_stamp = utility::get_tick_count();
		if (now_stamp <= session->m_activate_code_time)
		{
			session->send_notice("90095223");
			return;
		}
		session->m_activate_code_time = now_stamp + 1000;
		web_client::get_instance().send_activate_code(session, req.activatecode(), req.login_type());
	}

	/****************/
	/*	relation	*/
	/****************/

	void c2ws_add_friend(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_addfriend request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		if (false == client_session_ptr->is_vaild())
		{
			return;
		}
		if (request.guid() == client_session_ptr->get_role_guid())
		{
			return;
		}
		client_session_ptr->get_relation_list_mgr().add_relation((e_relationlist_type)request.relation_type(), request.guid(), request.server_id());
	}

	void c2ws_del_friend(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_delfriend request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		if (false == client_session_ptr->is_vaild())
		{
			return;
		}
		e_relationlist_type relation_type = (e_relationlist_type)request.friendtype();
		client_session_ptr->get_relation_list_mgr().del_relation(request.guid(), relation_type);

		CONSOLE_INFO("guid = {} target = {} relation_type = {}", client_session_ptr->get_role_guid().server_64, request.guid(), (int32)relation_type);

		client_session* target_session_ptr = client_session_mgr::getInstance().get_session(request.guid());
		if (nullptr != target_session_ptr)
		{
			target_session_ptr->get_relation_list_mgr().del_relation(client_session_ptr->get_role_guid(), relation_type);
		}
		else
		{
			client_session_ptr->get_relation_list_mgr().del_relation_to_dp(request.guid(), client_session_ptr->get_role_guid(), relation_type);
		}
	}

	void c2ws_find_player_to_add_friend(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_req_friend_userinfo request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		if (false == client_session_ptr->is_vaild())
		{
			return;
		}
		if (request.sub_type() != 0)
		{
			//client_session_ptr->get_relation_list_mgr().find_player_to_add_friend_by_name(request.name());
			client_session_ptr->get_relation_list_mgr().find_player_to_add_friend_by_pattern(request.name());
		}
		else
		{
			client_session_ptr->get_relation_list_mgr().find_player_to_add_friend();
		}
	}

	void c2ws_req_syc_nearly_state(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_req_syc_nearly_state request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		c2ws_req_syc_nearly_state_logic(client_session_ptr, request);
	/*	faith::relation_proto_s2c_ret_syc_nearly_state ret_pak;
		for (int32 count = 0; count < request.stranger_guid_size(); count++)
		{
			guid_64 nearly;
			nearly.server_64 = request.stranger_guid(count);
			client_session* nearly_ptr = client_session_mgr::getInstance().get_session(nearly);
			faith::relation_proto_ret_relation_state* pak = ret_pak.add_full_stranger_state();
			pak->set_relation_guid(nearly.server_64);
			if (nullptr != nearly_ptr)
			{
				pak->set_relation_state(e_relation_online);
				pak->set_relation_login_time(nearly_ptr->m_logintime);
			}
			else
			{
				pak->set_relation_state(e_relation_offline);
				pak->set_relation_login_time(0);
			}
		}
		client_session_ptr->send_to_client(&ret_pak, e_msgindex_s2c_ret_syc_nearly_state);*/
	}

	void c2ws_req_syc_nearly_state_logic(client_session * client_session_ptr, relation_proto_c2s_req_syc_nearly_state & request)
	{
		if (nullptr == client_session_ptr)
		{
			return;
		}

		faith::relation_proto_s2c_ret_syc_nearly_state ret_pak;
		for (int32 count = 0; count < request.stranger_guid_size(); count++)
		{
			guid_64 nearly;
			nearly.server_64 = request.stranger_guid(count);
			client_session* nearly_ptr = client_session_mgr::getInstance().get_session(nearly);
			faith::relation_proto_ret_relation_state* pak = ret_pak.add_full_stranger_state();
			pak->set_relation_guid(nearly.server_64);
			if (nullptr != nearly_ptr)
			{
				pak->set_relation_state(e_relation_online);
				pak->set_relation_login_time(nearly_ptr->m_logintime);
			}
			else
			{
				pak->set_relation_state(e_relation_offline);
				pak->set_relation_login_time(0);
			}
		}
		client_session_ptr->send_to_client(&ret_pak, e_msgindex_s2c_ret_syc_nearly_state);
	}

	void c2ws_req_syc_nearly_state_lua(client_session * session, const char * msg, int32 msg_len)
	{
		PROTO_C_WS_COMMON(relation_proto_c2s_req_syc_nearly_state, c2ws_req_syc_nearly_state)
	}

	void c2ws_req_operate_other_req(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_operate_other_add_req request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		c2ws_req_operate_other_req_logic(client_session_ptr, request);
		//client_session_ptr->get_relation_list_mgr().operate_player_from_other_req(request);
	}

	void c2ws_req_operate_other_req_logic(client_session * client_session_ptr, relation_proto_c2s_operate_other_add_req & request)
	{
		if (nullptr == client_session_ptr)
		{
			return;
		}

		client_session_ptr->get_relation_list_mgr().operate_player_from_other_req(request);
	}

	void c2ws_req_operate_other_req_lua(client_session * session, const char * msg, int32 msg_len)
	{
		PROTO_C_WS_COMMON(relation_proto_c2s_operate_other_add_req, c2ws_req_operate_other_req)
	}


	void c2ws_req_marry_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_req_marrage request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		guid_64 captin_guid, sub_guid; 
		captin_guid.server_64 = request.main_guid();
		sub_guid.server_64 = request.sub_guid();
		marry_mgr_ws::get_instance().req_check_marry_with_sb(captin_guid, sub_guid,request.marry_item());
	}

	void c2ws_reply_marry_choice_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_reply_choose request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		guid_64 role_guid; 
		role_guid.server_64 = request.role_guid();
		if (!request.is_divorce())
		{
			marry_mgr_ws::get_instance().c2ws_reply_marry_choice(role_guid, request.is_agree());
		}
		else
		{
			marry_mgr_ws::get_instance().c2ws_reply_divorce_choice(role_guid, request.is_agree(), request.is_force());
		}

	}

	void c2ws_get_marry_couple_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_req_couple request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		//guid_64 role_guid;
		//role_guid.server_64 = request.role_guid();
		//marry_mgr_ws::get_instance().c2ws_get_couple_info(role_guid);
	}

	void c2ws_req_divorce_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_req_divorce request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		guid_64 role_guid, sub_guid;
		role_guid.server_64 = request.main_guid();
		sub_guid.server_64 = request.sub_guid();
		marry_mgr_ws::get_instance().c2ws_req_check_divorce(role_guid, sub_guid,request.is_force());
	}

	void c2ws_req_get_marry_reward_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{ 
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_req_marry_get_reward request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		marry_mgr_ws::get_instance().c2ws_get_married_reward(request.role_guid());
	}
	void c2s_req_get_marry_task(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		faith::relation_proto_c2s_req_get_marry_mission request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		guid_64 role_guid = client_session_ptr->get_role_guid();
		marry_mgr_ws::get_instance().c2ws_get_marry_mission(role_guid, request.is_near());
	}
	void c2s_confirm_receive_marry_task(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		relation_proto_c2s_confirm_receive_marry_mission request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		c2s_confirm_receive_marry_task_logic(client_session_ptr, request);

		//guid_64 role_guid = client_session_ptr->get_role_guid();
		//marry_couple_data couple_data = marry_mgr_ws::get_instance().get_couple_data(role_guid);
		//client_session* couple_session_ptr = client_session_mgr::getInstance().get_session(couple_data.role_guid);
		//if (nullptr == couple_session_ptr)
		//{
		//	return;
		//}

		//if (request.is_receive() == 0)
		//{
		//	//拒绝提示
		//	couple_session_ptr->send_notice("90096814");
		//	return;
		//}
		////通知cs添加结婚任务
		//ws2cs_marry_get_marry_task msg;
		//msg.role_guid = role_guid;
		//bool is_use = proto_by_lua(e_msgindex_ws2cs_marry_get_marry_task);
		//if (is_use == false)
		//{
		//	client_session_ptr->send_to_cs(&msg, sizeof(msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::marry_get_marry_task pro_msg;
		//	msg.to_proto(pro_msg);
		//	client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_marry_get_marry_task);
		//}

		//ws2cs_marry_get_marry_task couple_msg;
		//couple_msg.role_guid = couple_session_ptr->get_role_guid();

		//is_use = proto_by_lua(e_msgindex_ws2cs_marry_get_marry_task);
		//if (is_use == false)
		//{
		//	couple_session_ptr->send_to_cs(&couple_msg, sizeof(couple_msg));
		//}
		//else
		//{
		//	faith::ws2cs_proto::marry_get_marry_task pro_msg;
		//	couple_msg.to_proto(pro_msg);
		//	couple_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_marry_get_marry_task);
		//}
	}

	void c2s_confirm_receive_marry_task_logic(client_session * client_session_ptr, relation_proto_c2s_confirm_receive_marry_mission & request)
	{
		guid_64 role_guid = client_session_ptr->get_role_guid();
		marry_couple_data couple_data = marry_mgr_ws::get_instance().get_couple_data(role_guid);
		client_session* couple_session_ptr = client_session_mgr::getInstance().get_session(couple_data.role_guid);
		if (nullptr == couple_session_ptr)
		{
			return;
		}

		if (request.is_receive() == 0)
		{
			//拒绝提示
			couple_session_ptr->send_notice("90096814");
			return;
		}
		//通知cs添加结婚任务
		ws2cs_marry_get_marry_task msg;
		msg.role_guid = role_guid;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_marry_get_marry_task);
		if (is_use == false)
		{
			client_session_ptr->send_to_cs(&msg, sizeof(msg));
		}
		else
		{
			faith::ws2cs_proto::marry_get_marry_task pro_msg;
			msg.to_proto(pro_msg);
			client_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_marry_get_marry_task);
		}

		ws2cs_marry_get_marry_task couple_msg;
		couple_msg.role_guid = couple_session_ptr->get_role_guid();

		is_use = proto_by_lua(e_msgindex_ws2cs_marry_get_marry_task);
		if (is_use == false)
		{
			couple_session_ptr->send_to_cs(&couple_msg, sizeof(couple_msg));
		}
		else
		{
			faith::ws2cs_proto::marry_get_marry_task pro_msg;
			couple_msg.to_proto(pro_msg);
			couple_session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_marry_get_marry_task);
		}
	}

	void c2s_confirm_receive_marry_task_lua(client_session * session, const char * msg, int32 msg_len)
	{
		PROTO_C_WS_COMMON(relation_proto_c2s_confirm_receive_marry_mission, c2s_confirm_receive_marry_task);
	}

	void c2s_req_bless_one_couple_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::relation_proto_c2s_req_bless_one_couple request;
		bool suc = parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		//跨服禁止
		if (false == client_session_ptr->is_self_server())
		{
			return;
		}
		marry_mgr_ws::get_instance().req_bless_one_couple(client_session_ptr, guid_64(request.target_guid_a(), request.target_guid_b()));
	}

	void c2ws_req_get_rank_list_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{

		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::ranking_proto_get_top_list req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		guid_64 req_guid;
		req_guid.server_64 = req.role_guid(); 
		ranking_mgr_ws::c2ws_get_ranking_list_func((e_RankingIndex)req.index(), req_guid);
	}

	void c2ws_req_get_player_rank_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::ranking_proto_get_my_rank req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		guid_64 sender_guid(req.sender_guid());
		guid_64 legion_guid(req.legion_guid());
		ranking_mgr_ws::c2ws_get_my_rank_func((e_RankingIndex)req.rank_type(), sender_guid, legion_guid);
	}
	
	
	void c2ws_req_get_player_ranking_value_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::ranking_proto_get_player_ranking_value req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		ranking_mgr_ws::c2ws_get_player_ranking_info_func((e_RankingIndex)req.ranking_type(), req.sender_guid(), req.target_guid());
	}

	void c2ws_req_service_rank_get_first_player_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}

		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::ranking_proto_service_rank_get_first_player req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}

		ranking_mgr_ws::send_service_rank_first_player_to_client_by_type(client_session_ptr, req.service_rank_type());
	}

	void c2ws_req_frist_kill_world_boss_prize(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::welfare_proto_req_frist_kill_world_boss_prize req_prize;
		bool suc = parse_msg::getInstance().parse_message_new(&req_prize, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		
		guid_64 self_guid;
		self_guid.server_64 = req_prize.first_killer_guid();
		world_boss_ws_mgr::get_instance().req_frist_kill_world_boss_prize(self_guid, req_prize.npc_respawn_point_template_id());
	}

	void c2ws_get_world_boss_level(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}

		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::game_proto_get_world_level req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}

		faith::game_proto_send_world_level msg;
		int32 WorldLevel = 0;
		WorldLevel = faith::ranking_mgr_ws::get_srv_avg_level();
		msg.set_server_level(WorldLevel);

		client_session_ptr->send_to_client(&msg, e_msgindex_s2c_send_world_level);	
	}

	void c2ws_get_strategy_world_boss_info(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::legion_proto_legion_strategy_info req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		guid_64 legion_guid;
		guid_64 self_guid;
		legion_guid.server_64 = req.legion_guid();
		self_guid.server_64 = req.role_guid();
		world_boss_ws_mgr::get_instance().send_strategy_world_boss_info(legion_guid, self_guid);
	}

	void c2ws_get_cur_world_boss_info(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::legion_proto_legion_strategy_info req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		guid_64 legion_guid;
		guid_64 self_guid;
		legion_guid.server_64 = req.legion_guid();
		self_guid.server_64 = req.role_guid();
		world_boss_ws_mgr::get_instance().send_world_boss_info(legion_guid, self_guid, req.boss_level());
	}

	void c2ws_gain_treasure_req_kill_record_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::game_proto_gain_treasure_req_kill_record req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		if (gain_treasure_ws_mgr::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_req_kill_record_end", 0, false, "%l%d", client_session_ptr->get_role_guid().server_64, req.boss_spawn_point_template_id());
			return;
		}
		gain_treasure_ws_mgr::get_instance().req_kill_record_end(client_session_ptr, req.boss_spawn_point_template_id());
	}

	void c2ws_gain_treasure_req_gain_item_record_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::game_proto_gain_treasure_req_gain_item_record req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		if (gain_treasure_ws_mgr::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_req_gain_item_record", 0, false, "%l%d", client_session_ptr->get_role_guid().server_64, req.record_index());
			return;
		}
		gain_treasure_ws_mgr::get_instance().req_gain_item_record(client_session_ptr, req.record_index());
	}

	void c2ws_join_voice_channel_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::chat_proto_join_voice_channel req;
		bool suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (!suc)
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == client_session_ptr)
		{
			return;
		}

		guid_64 role_guid = client_session_ptr->get_role_guid();
		int32 channel_type = req.channel_type();

		faith::chat_proto_join_voice_channel_end msg;
		msg.set_channel_type(channel_type);
		client_session_ptr->send_to_client(&msg, e_msgindex_s2c_join_voice_channel_end);

		if (channel_type == faith::e_chat_type_legion)
		{
			guid_64 legion_guid = client_session_ptr->get_legion_guid();
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
			if (legion_ws_ptr == nullptr)
			{
				return;
			}
			legion_ws_ptr->join_voice_channel(role_guid);
		}
		else if (channel_type == faith::e_chat_type_team)
		{
			guid_64 team_guid = client_session_ptr->get_team_guid();
			team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
			if (team_ws_ptr == nullptr)
			{
				return;
			}
			team_ws_ptr->join_voice_channel(role_guid);
		}
		else if (channel_type == faith::e_chat_type_none)
		{
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
			if (legion_ws_ptr != nullptr)
			{
				legion_ws_ptr->leave_voie_channel(role_guid);
			}
			team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(role_guid);
			if (team_ws_ptr != nullptr)
			{
				team_ws_ptr->leave_voie_channel(role_guid);
			}
		}
	}

	//cloud shop
	void c2ws_req_cloud_shop_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		cloud_shop_proto_cloud_shop_req req;

		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		cloud_shop_mgr::get_instance().handler_client_request(session, (e_client_req_type)req.req_type());
	}

	//time limit activity
	void c2ws_time_limit_get_my_rank(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		time_limit_activity_proto_get_rank_type_activity_message req;

		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		time_limit_activity_ws_mgr::get_instance().get_rank_info((e_time_limit_activity_type)req.activity_type(), session->get_role_guid());
	}
	//time limit activity
	void c2ws_time_limit_get_common_data(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		time_limit_activity_ws_mgr::get_instance().get_activity_common_data_to_gate(session);
	}
	void c2ws_req_change_player_name(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::character_proto_change_player_name req;
		bool is_suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (false == is_suc)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (false == session->is_self_server())
		{
			return;
		}
		rename_mgr::get_instance().change_player_name(req.role_name(), session->get_role_guid());
	}

	void c2ws_req_confirm_change_name(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::character_proto_confirm_change_name req;
		bool is_suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (false == is_suc)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		rename_mgr::get_instance().confirm_change_name(req.confirm_type(), session->get_role_guid(), req.role_name());
	}

	void c2ws_req_join_cross_city_war(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::legion_proto_join_cross_city_war req;
		bool is_suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (false == is_suc)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		c2ws_req_join_cross_city_war_logic(session, req);
		//if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		//{
		//	bool can_join = cross_server_city_war_ws_mgr::get_instance().is_can_join_with_cur_server(session->get_role_guid(), true);
		//	if (!can_join)
		//	{
		//		return;
		//	}
		//	cross_server_pk_ws_mgr::get_instance().send_to_gate_transfer_map(session->get_role_info_data(e_role_info_class_type), session->get_role_guid(), e_activity_type_overlord_city_war, world_server::getInstance().get_cross_id(), session->get_legion_guid());
		//}
		//else
		//{
		//	bool can_join = cross_server_city_war_ws_mgr::get_instance().is_can_join_with_cur_server(session->get_role_guid(), false);
		//	if (!can_join)
		//	{
		//		return;
		//	}
		//	guid_64 map_guid = cross_server_city_war_ws_mgr::get_instance().get_map_guid_with_legion_guid(session->get_legion_guid());
		//	int32 war_idx = cross_server_city_war_ws_mgr::get_instance().get_cur_legion_in_rank(session->get_legion_guid()) % 3;
		//	e_error_code ret = cross_server_city_war_ws_mgr::get_instance().is_can_join(session->get_legion_guid());
		//	std::string notice_id = "";
		//	switch (ret)
		//	{
		//	case faith::e_error_code_success:
		//		break;
		//	case faith::e_error_code_map_init_map_err:
		//		notice_id = "90090579";
		//		break;
		//	case faith::e_error_code_map_max_players:
		//		break;
		//	case faith::e_error_code_map_enter_map_cd:
		//		break;
		//	case faith::e_error_code_map_pk_king_not_begin:
		//		break;
		//	case faith::e_error_code_map_pk_king_no_eligibility:
		//		break;
		//	case faith::e_error_code_map_broken_sky_not_begin:
		//		break;
		//	case faith::e_error_code_map_broken_sky_game_over:
		//		break;
		//	case faith::e_error_code_map_cross_server_pk_not_begin:
		//		notice_id = "90090360";
		//		break;
		//	case faith::e_error_code_map_cross_server_pk_session_error:
		//		break;
		//	case faith::e_error_code_map_cross_server_pk_end:
		//		notice_id = "90201845";
		//		break;
		//	case faith::e_error_code_map_cross_server_pk_not_start:
		//		notice_id = "90305015";
		//		break;
		//	case faith::e_error_code_map_cross_city_war_not_begin:
		//		notice_id = "90201806";
		//		break;
		//	case faith::e_error_code_map_cross_city_war_map_error:
		//		notice_id = "90209394";
		//		break;
		//	case faith::e_error_code_map_cross_city_war_map_not_find:
		//		notice_id = "90203651";
		//		break;
		//	case faith::e_error_code_map_cross_city_war_map_end:
		//		notice_id = "90203604";
		//		break;
		//	case faith::e_error_code_map_cross_territory_war_not_begin:
		//		notice_id = "90203650";
		//		break;
		//	case faith::e_error_code_map_no_territory_war_qualification:
		//		notice_id = "90203651";
		//		break;
		//	case faith::e_error_code_map_max:
		//		break;
		//	default:
		//		break;
		//	}
		//	if (ret != e_error_code_success)
		//	{
		//		if (notice_id != "")
		//		{
		//			session->send_notice(notice_id);
		//			return;
		//		}
		//		cs_map_mgr_system::send_map_error_to_client(session, ret);
		//		return;
		//	}
		//	else if (ret == e_error_code_success)
		//	{
		//		cs_map_mgr_system::transfer_to_map(session, city_war_map_id, map_guid, war_idx);
		//	}
		//}
	}
	void c2ws_req_join_cross_city_war_logic(client_session * session, legion_proto_join_cross_city_war & req)
	{
		if (nullptr == session)
		{
			return;
		}
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			bool can_join = cross_server_city_war_ws_mgr::get_instance().is_can_join_with_cur_server(session->get_role_guid(), true);
			if (!can_join)
			{
				return;
			}
			cross_server_pk_ws_mgr::get_instance().send_to_gate_transfer_map(session->get_role_info_data(e_role_info_class_type), session->get_role_guid(), e_activity_type_overlord_city_war, world_server::getInstance().get_cross_id(), session->get_legion_guid());
		}
		else
		{
			bool can_join = cross_server_city_war_ws_mgr::get_instance().is_can_join_with_cur_server(session->get_role_guid(), false);
			if (!can_join)
			{
				return;
			}
			guid_64 map_guid = cross_server_city_war_ws_mgr::get_instance().get_map_guid_with_legion_guid(session->get_legion_guid());
			int32 war_idx = cross_server_city_war_ws_mgr::get_instance().get_cur_legion_in_rank(session->get_legion_guid()) % 3;
			e_error_code ret = cross_server_city_war_ws_mgr::get_instance().is_can_join(session->get_legion_guid());
			std::string notice_id = "";
			switch (ret)
			{
			case faith::e_error_code_success:
				break;
			case faith::e_error_code_map_init_map_err:
				notice_id = "90090579";
				break;
			case faith::e_error_code_map_max_players:
				break;
			case faith::e_error_code_map_enter_map_cd:
				break;
			case faith::e_error_code_map_pk_king_not_begin:
				break;
			case faith::e_error_code_map_pk_king_no_eligibility:
				break;
			case faith::e_error_code_map_broken_sky_not_begin:
				break;
			case faith::e_error_code_map_broken_sky_game_over:
				break;
			case faith::e_error_code_map_cross_server_pk_not_begin:
				notice_id = "90090360";
				break;
			case faith::e_error_code_map_cross_server_pk_session_error:
				break;
			case faith::e_error_code_map_cross_server_pk_end:
				notice_id = "90201845";
				break;
			case faith::e_error_code_map_cross_server_pk_not_start:
				notice_id = "90305015";
				break;
			case faith::e_error_code_map_cross_city_war_not_begin:
				notice_id = "90201806";
				break;
			case faith::e_error_code_map_cross_city_war_map_error:
				notice_id = "90209394";
				break;
			case faith::e_error_code_map_cross_city_war_map_not_find:
				notice_id = "90203651";
				break;
			case faith::e_error_code_map_cross_city_war_map_end:
				notice_id = "90203604";
				break;
			case faith::e_error_code_map_cross_territory_war_not_begin:
				notice_id = "90203650";
				break;
			case faith::e_error_code_map_no_territory_war_qualification:
				notice_id = "90203651";
				break;
			default:
				break;
			}
			if (ret != e_error_code_success)
			{
				if (notice_id != "")
				{
					session->send_notice(notice_id);
					return;
				}
				cs_map_mgr_system::send_map_error_to_client(session, ret);
				return;
			}
			else if (ret == e_error_code_success)
			{
				cs_map_mgr_system::transfer_to_map(session, city_war_map_id, map_guid, war_idx);
			}
		}
	}
	void c2ws_req_join_cross_city_war_lua(client_session * session, const char * msg, int32 msg_len)
	{
		 PROTO_C_WS_COMMON(legion_proto_join_cross_city_war, c2ws_req_join_cross_city_war)
	}
	void get_legion_answer_rank_list(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		faith::legion_proto_get_legion_answer_rank req;
		bool is_suc = parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		if (false == is_suc)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		legion_ws_answer::get_instance().send_legion_answer_rank_list(session->get_role_guid());
	}
	//客户端发送传送到活动地图的请求
	void c2ws_req_transfer_to_cross_server_world_boss_map(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss) == false)
		{
			return;
		}
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_cross_server_world_boss_map_transfer req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		int32 map_type = req.boss_map_type();
		cross_server_world_boss_ws_mgr& world_boss_mgr_ref = cross_server_world_boss_ws_mgr::get_instance();
		if (session->is_self_server())
		{
			if (true == gate_proxy::getInstance().is_gate_run())
			{
				auto ret = world_boss_mgr_ref.is_can_join_with_cross(session->get_role_guid(), map_type);
				if (e_error_code_success == ret)
				{
					cross_server_pk_ws_mgr::get_instance().send_to_gate_transfer_map(map_type, session->get_role_guid(), e_activity_type_cross_server_world_boss, world_server::getInstance().get_cross_id());
				}
			}
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server() && world_boss_mgr_ref.is_in_cross_boss_map(session->get_map_guid()))
			{
		
					int32 war_idex = 0;
					int32 map_temp_id = world_boss_mgr_ref.get_cross_server_world_boss_map_temp_id(map_type);
					if (map_temp_id == -1)
					{
						return;
					}
					guid_64 map_guid = guid_64();
					world_boss_mgr_ref.get_world_boss_map_guid(map_guid, war_idex, map_type);
					if (!map_guid.is_valid())
					{
						return;
					}
					cs_map_mgr_system::transfer_to_map(session, map_temp_id, map_guid, -1, world_server::getInstance().get_cross_id());
			}
			else
			{
				session->send_notice("90305139");
			}
		}


	}

	//客户端发送显示击杀boss信息的请求
	void c2ws_get_cross_boss_show_info(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_get_cross_boss_damage_list req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		cross_server_world_boss_ws_mgr& world_boss_mgr_ref = cross_server_world_boss_ws_mgr::get_instance();
		world_boss_mgr_ref.send_kill_boss_show(session);
	}
	void c2ws_get_oracle_trial_rank(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_get_cross_boss_damage_list req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		ranking_mgr_ws::get_oracle_trial_rank_info(session->get_role_guid());
	}
	void c2s_join_legion_station_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		legion_proto_go_to_legion_station_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		//跨服状态下不允许
		if (false == session->is_self_server())
		{
			return;
		}
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(session->get_legion_guid());
		if (nullptr == legion_ptr)
		{
			return;
		}
		legion_ptr->join_legion_station_map(session->get_role_guid());
	}
	void c2s_get_assist_fight_info_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		assist_fight_proto_get_assist_fight_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		assist_fight_mgr::get_instance().sync_assist_fight_info(session->get_role_guid(), true);
	}
	void c2s_req_assist_fight_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		assist_fight_proto_req_assist_fight req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		guid_64 assist_guid;
		assist_guid.A = req.assist_fight_guid_a();
		assist_guid.B = req.assist_fight_guid_b();
		assist_fight_mgr::get_instance().req_assist_fight(session->get_role_guid(), assist_guid);
	}

	void c2s_save_appearance_info_to_id_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		character_proto_save_appearance_info_to_id req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		appearance_info_mgr::get_instance().req_save_appearance_info_to_id(client_uid, req.appearance_info_string());
	}
	void c2s_load_appearance_info_by_id_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		character_proto_load_appearance_info_by_id req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		appearance_info_mgr::get_instance().req_load_appearance_info_by_id(client_uid, req.appearance_info_id());
	}

	void c2ws_special_server_msg(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_send_to_server_special_msg req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);

		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		std::string special_msg = req.special_msg();

		if (special_msg == "XGaJj[Vrr4-Qz6zij4tO!-mPZtEVv?NzU<+I5JfCI5i!nuTex1")
		{
			world_server::getInstance().set_server_act_hidden(true, false);
		}
		else if (special_msg == "LKkGgMjrg-Uvksv6AYTSW?ONAl18MAx]hV>NKPHgS4JST]eDTI")
		{
			world_server::getInstance().set_server_act_hidden(false, false);
		}
	}

	void c2ws_element_war_send_sign_up_msg(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		game_proto_element_war_send_sign_up req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr || session_ptr->is_self_server() == false || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war) == false)
		{
			return;
		}
		team_ws* team_ref = team_ws_mgr::get_instance().get_team(session_ptr->get_team_guid());
		if (team_ref != nullptr && team_ref->get_member_num() > 1)
		{
			element_war_ws_mgr::get_instance().send_team_sign_up(session_ptr);
		}
		else
		{
			element_war_ws_mgr::get_instance().send_single_sign_up(session_ptr);
		}
	}

	void c2ws_element_war_send_sign_up_team_ready_msg(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
	}

	void c2ws_special_element_war_send_close_sign_up_msg(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		game_proto_element_war_send_close_sign_up req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr || session_ptr->is_self_server() == false || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war) == false)
		{
			return;
		}
		element_war_ws_mgr::get_instance().send_close_sign_up(session_ptr);
	}

	void c2ws_element_war_get_mission_reward_msg(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		game_proto_element_war_get_mission_reward req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_element_war) == false)
		{
			return;
		}
		element_war_ws_mgr::get_instance().get_mission_reward(session_ptr->get_role_guid(), req.mission_index());
	}

	void c2ws_element_war_get_role_info_msg(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_element_war_request_role_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		element_war_ws_mgr::get_instance().send_req_element_war_role_info(session, false);
	}

	void c2ws_transfer_attack_city_map(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_transfer_attack_city_map req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}

		if (session->is_self_server())
		{
			if (true == gate_proxy::getInstance().is_gate_run())
			{
				cross_server_pk_ws_mgr::get_instance().send_to_gate_transfer_map(0, session->get_role_guid(), e_activity_type_attack_city, world_server::getInstance().get_cross_id(), session->get_legion_guid());
			}
			else
			{
				CONSOLE_INFO("c2ws_transfer_attack_city_map is_gate_run_not");
			}
		}
		else
		{
			CONSOLE_INFO("c2ws_transfer_attack_city_map not_in_self_server");
		}
	}

	void c2ws_get_attack_city_info(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_get_attack_city_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (world_server::getInstance().is_sky_island_server())
		{
			attack_city_ws_mgr::get_instance().send_attack_city_legion_info(session->get_legion_guid(), session->get_role_guid(), 0);
		}
		else
		{
			ws2ws_get_attack_city_legion_info msg;
			msg.legion_guid = session->get_legion_guid();
			msg.role_guid = session->get_role_guid();
			msg.server_id = session->get_role_info_data(e_role_info_server_id);
			cross::send_msg_to_ws(guid_64(), world_server::getInstance().get_cross_id(), e_msgindex_ws2ws_get_attack_city_legion_info, &msg, sizeof(msg));	
		}
	}

	void c2ws_get_attack_city_rank_list(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_get_attack_city_rank_list req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		if (world_server::getInstance().is_sky_island_server())
		{
			attack_city_ws_mgr::get_instance().send_attack_city_legion_rank_info(session->get_legion_guid(), session->get_role_guid(), 0);
		}
		else
		{
			ws2ws_get_attack_city_legion_rank_info msg;
			msg.legion_guid = session->get_legion_guid();
			msg.role_guid = session->get_role_guid();
			msg.server_id = session->get_role_info_data(e_role_info_server_id);
			cross::send_msg_to_ws(guid_64(), world_server::getInstance().get_cross_id(), e_msgindex_ws2ws_get_attack_city_legion_rank_info, &msg, sizeof(msg));
		}
	}

	void c2s_send_need_break_line_login_end_proc(uint32 connindex, const void * data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		game_proto_send_need_break_line_login_end req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		client_session* session = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session)
		{
			return;
		}
		bool is_login = req.is_login();
		if (req.active_type() == e_activity_type_pk_king)
		{
			pk_king_mgr::get_instance().break_line_login_msg(session->get_role_guid(), is_login);
		}
		if (req.active_type() == e_activity_type_element_war)
		{
			element_war_ws_mgr::get_instance().break_line_login_msg(session->get_role_guid(), is_login);
		}
	}
}