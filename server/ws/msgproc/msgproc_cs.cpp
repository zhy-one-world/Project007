/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   15:02
	file base:	msgproc_cs
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "chat.pb.h"
#include "components/scene/cs_map_component.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "cross_group/gate_proxy.hpp"
#include "cross_group/gate_proxy.hpp"
#include "cross_ladder.pb.h"
#include "http/http_send.hpp"
#include "invalid_word/invalid_ansi_word.h"
#include "lua/script_mgr.h"
#include "msgproc_cs.hpp"
#include "net.pb.h"
#include "server/activity/boss_island_ws_mgr.h"
#include "server/activity/broken_sky_mgr.h"
#include "server/activity/cloud_shop_mgr.h"
#include "server/activity/cross_ladder_ws_mgr.h"
#include "server/activity/cross_server_harry_ws_mgr.h"
#include "server/activity/cross_server_pk_ws_mgr.h"
#include "server/activity/cross_server_world_boss_ws_mgr.h"
#include "server/activity/element_war_ws_mgr.h"
#include "server/activity/gain_treasure_ws_mgr.h"
#include "server/activity/pk_king_mgr.h"
#include "server/activity/time_limit_activity_ws_mgr.h"
#include "server/arena/arena_mgr_ws.h"
#include "server/assist_fight/assist_fight_mgr.h"
#include "server/auction/auction_mgr_ws.h"
#include "server/best_record/best_record_mgr.h"
#include "server/big_player_ws_mgr.h"
#include "server/chat/chat_mgr_ws.h"
#include "server/client_session_mgr.hpp"
#include "server/gm/gm_order_proc_ws.h"
#include "server/legion/attack_city_ws_mgr.h"
#include "server/legion/legion_ws_answer.h"
#include "server/legion/legion_ws_mgr.h"
#include "server/lucky_draw_record_ws_mgr.h"
#include "server/mail/event_ws_mgr.h"
#include "server/mail/mail_event_ws.h"
#include "server/person_information/person_infor_ws_mgr.h"
#include "server/ranking/ranking_mgr_ws.h"
#include "server/recharge_mgr_ws.hpp"
#include "server/red_package/red_package_ws_mgr.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "server/rename/rename_mgr.h"
#include "server/role_pk/role_pk_manager.h"
#include "server/team/team_ws_mgr.h"
#include "server/world_boss/world_boss_ws_mgr.h"
#include "server_log.hpp"
#include "system/scene/cs_map_mgr_system.h"
#include "system/scene/cs_map_system.h"
#include "template/StringConst_S.h"
#include "template/template_manager.h"
#include "utility/cs_date.hpp"
#include "utility/guid_gen.h"
#include "utility/init_unit.h"
#include "utility/parse_msg.h"
#include "world_server.hpp"
#include "ws_client.hpp"
#include <boss_island_msg.hpp>
#include <chat_msg.hpp> 
#include <cloud_shop_msg.hpp>
#include <cross_ladder_msg.hpp>
#include <cross_server_harry_msg.hpp>
#include <cross_server_pk_msg.hpp>
#include <cross_server_world_boss_msg.hpp>
#include <event_msg.hpp> 
#include <gain_treasure_msg.hpp>
#include <legion_station_msg.hpp>
#include <login_msg.hpp>
#include <lucky_draw_record_msg.hpp>
#include <mail_msg.hpp>
#include <marry_msg.hpp> 
#include <world_boss_msg.hpp> 
#include <world_server_msg.hpp>
#include "game.pb.h"
#include "character.pb.h"

namespace hld
{
	void cs2ws_rep_reconnect_game(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_reconnect_game* packet = static_cast<const cs2ws_reconnect_game*>(data_ptr);
		if (packet == NULL)
		{
			string str = " the packet is null";
			server_log::reconnect_game_log("ws", __FUNCTION__, str.c_str());
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == client_session_ptr)
		{
			ws2cs_client_logout	req_logout;
			//	周期性广播，通知所有 CS LOGOUT
			req_logout.role_guid = packet->role_guid;
			req_logout.client_uid = packet->client_uid;


			bool is_use = proto_by_lua(e_msg_index_ws2cs_client_logout);
			if (is_use == false)
			{
				world_server::getInstance().broadcast(&req_logout, sizeof(req_logout), e_server_type_cs);
			}
			else
			{
				hld::ws2cs_proto::client_logout pro_msg;
				pro_msg.set_role_guid(packet->role_guid.server_64);
				pro_msg.set_client_uid(packet->client_uid.fep_uid_64);
				pro_msg.set_need_send_save_end(false);
				world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_client_logout, e_server_type_cs);
			}

			
			world_server::getInstance().broadcast(&req_logout, sizeof(req_logout), e_server_type_cs);

			string str = " the client_session_ptr is null";
			server_log::reconnect_game_log("ws", __FUNCTION__, str.c_str());

			return;
		}
		if (packet->reconnect_res == e_reconnect_result_win)
		{
			client_session_ptr->m_logout_begin = false;
			client_session_ptr->m_logout_time = 0;
			marry_mgr_ws::get_instance().session_online_marry_handle(client_session_ptr->get_role_guid());
			team_ws_mgr::get_instance().on_player_online(client_session_ptr);
			legion_ws_mgr::get_instance().on_player_reconnect(client_session_ptr);
			world_boss_ws_mgr::get_instance().send_world_boss_all(client_session_ptr);
			world_boss_ws_mgr::get_instance().send_all_boss_hp_per(client_session_ptr);

			ws2fep_reconnect_game fep_msg;
			fep_msg.client_uid = packet->client_uid;
			fep_msg.cellserver_id = client_session_ptr->get_cs_conn_index();
			fep_msg.array_index = client_session_ptr->get_cs_array_index();
			fep_msg.reconnect_res = e_reconnect_result_win;
			memcpy(fep_msg.account, client_session_ptr->m_account, sizeof(fep_msg.account));
			client_session_ptr->send_to_fep(&fep_msg, sizeof(fep_msg));
		}
		else
		{
			ws2fep_reconnect_game fep_msg;
			fep_msg.client_uid = packet->client_uid;
			fep_msg.reconnect_res = e_reconnect_result_cs_no_find;
			memcpy(fep_msg.account, client_session_ptr->m_account, sizeof(fep_msg.account));
			client_session_ptr->send_to_fep(&fep_msg, sizeof(fep_msg));

			client_session_mgr::getInstance().logout_client(client_session_ptr);
		}
	}

	void cs2ws_rep_enter_game(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_enter_game* packet = static_cast<const cs2ws_enter_game*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}

		client_session* session = client_session_mgr::getInstance().get_session(packet->char_info_to_ws.role_guid);
		if (session == NULL)
		{
			ws2cs_client_logout	req_logout;
			req_logout.client_uid = packet->client_uid;
			req_logout.role_guid = packet->char_info_to_ws.role_guid;

			bool is_use = proto_by_lua(e_msg_index_ws2cs_client_logout);
			if (is_use == false)
			{
				world_server::getInstance().send_by_uid(conn_index, &req_logout, sizeof(req_logout));
			}
			else
			{
				hld::ws2cs_proto::client_logout pro_msg;
				pro_msg.set_role_guid(packet->char_info_to_ws.role_guid.server_64);
				pro_msg.set_client_uid(packet->client_uid.fep_uid_64);
				pro_msg.set_need_send_save_end(false);
				world_server::getInstance().send_by_uid_lua(conn_index, &pro_msg, e_msg_index_ws2cs_client_logout);
			}

			return;
		}
		switch (packet->eResult)
		{
		case cs2ws_enter_game::e_success:
		{
			session->m_status = client_session::e_ss_ingame;
			session->set_cs_array_index(packet->char_info_to_ws.array_index);
			session->m_step_num = client_session::e_session_step_cs_enter_game;
			ws2fep_enter_game rep_fep;
			rep_fep.client_uid = session->get_client_uid();
			rep_fep.e_result = e_error_code_success;
			rep_fep.cs_array_index = packet->char_info_to_ws.array_index;
			rep_fep.server_type = e_server_type_ws;
			memcpy(rep_fep.account, session->m_account, sizeof(rep_fep.account));
			session->send_to_fep(&rep_fep, sizeof(rep_fep));
		}
			break;
		case cs2ws_enter_game::e_failed_repeat_player_id:
		default:
		{
			client_session_mgr::getInstance().kickout_account(session->m_account, e_logout_result_enter_scene_failed);
		}
			break;
		}
	}

	void cs2ws_rep_enter_scene(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_enter_scene* packet = static_cast<const cs2ws_enter_scene*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (session == NULL)
		{
			return;
		}

		switch (packet->eResult)
		{
		case cs2ws_enter_scene::e_failed_unknow_error:
		case cs2ws_enter_scene::e_enter_ret_player_err:
		{
			client_session_mgr::getInstance().kickout_account(session->m_account, e_logout_result_enter_scene_failed);
		}
		break;
		case cs2ws_enter_scene::e_enter_ret_already_in:
		{
			game_proto_enter_scene_end enter_scene_end;
			enter_scene_end.set_result(hld::e_error_code_repeated);
			session->send_to_client(&enter_scene_end, e_msgindex_s2c_enter_scene);
		}
		break;
		case cs2ws_enter_scene::e_enter_ret_success:
		{
			game_proto_enter_scene_end enter_scene_end;
			enter_scene_end.set_result(hld::e_error_code_success);
			session->send_to_client(&enter_scene_end, e_msgindex_s2c_enter_scene);
			session->enter_scene_logic_proc();
		}
		break;
		}
	}

	void cs2ws_rep_logout(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_client_logout* packet = static_cast<const cs2ws_client_logout*>(data_ptr);
		if (packet == NULL)
			return;

		client_session* session = client_session_mgr::getInstance().get_session(packet->player_guid);
		if (session == NULL)
			return;
		session->set_cs_conn_index(e_invalid_server_uid);

		if (session->m_status != client_session::e_ss_logout)
		{
			client_session_mgr::getInstance().kickout_account(session->m_account, packet->logout_result);
		}
		else
		{
			client_session_mgr::getInstance().logout_complete(session);
		}
	}
	void cs2ws_rep_role_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const hld::cs2ws_role_info* packet = static_cast<const cs2ws_role_info*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (session == NULL)
		{
			return;
		}
		if (packet->role_index >= e_role_info_max)
		{
			return;
		}
		session->m_role_info.data_ary[packet->role_index] = packet->role_value;
	}

	void cs2ws_rep_map_state(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const hld::cs2ws_map_state* packet = static_cast<const cs2ws_map_state*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}
		auto map_ent = get_entity(packet->map_guid);
		cs_map_system::set_map_state(map_ent, packet->map_state);
	}
	void cs2ws_req_transfer_player(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const hld::cs2ws_tansfer_player* packet = static_cast<const cs2ws_tansfer_player*>(data_ptr);
		if (packet == NULL)
		{
			CONSOLE_ERROR("cs2ws_req_transfer_player packet is null");
			return;
		}

		client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (session == NULL)
		{
			CONSOLE_ERROR("cs2ws_req_transfer_player session is null role_guid:{}", packet->role_guid.server_64);
			return;
		}

		guid_64 now_map_guid = session->get_map_guid();
		session->set_last_map_guid(now_map_guid);
		int32 unit_num_after_rm = cs_map_mgr_system::remove_unit_from_map(now_map_guid, session->get_role_guid());
		team_ws_mgr::get_instance().on_unit_leave_map(session, unit_num_after_rm);

		if (packet->dest_group_id != 0 && gate_proxy::getInstance().is_gate_run() && packet->dest_group_id != world_server::getInstance().get_server_id())
		{
			cross::transfer_player(session, packet->dest_group_id, packet->transfer_info);
			return;
		}
		session->set_transfer_info(packet->transfer_info);
		session->m_status = client_session::e_ss_map_transfer;
		session->set_cs_conn_index(-1);
		memcpy(&session->m_role_info, &packet->role_unit_info, sizeof(session->m_role_info));

		session->login_try_enter_scene();
	}

	void cs2ws_req_pk_king_begin(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_pk_king_begin* packet = static_cast<const cs2ws_pk_king_begin*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_pk_king_begin))
		{
			return;
		}

		//client_session* temp_all_session = client_session_mgr::getInstance().get_all_session();
		//int32 session_used_num = client_session_mgr::getInstance().get_session_array_used();
		//for (int32 i = 0; i <= session_used_num; ++i)
		//{
		//	client_session& client_session_ref = temp_all_session[i];
		//	if (client_session_ref.is_vaild())
		//	{
		//		ws2cs_clear_pk_king_title msg;
		//		msg.role_guid = client_session_ref.get_role_guid();
		//		client_session_ref.send_to_cs(&msg, sizeof(msg));
		//	}
		//}
		guid_64 winner_guid;
		winner_guid.clear_data();
		big_player_ws_mgr::get_instance().set_big_player_guid(e_big_player_type_lord_or_war, winner_guid);
		if (init_unit::is_send_clear_title_with_clear_big_player(e_big_player_type_lord_or_war))
		{
			big_player_ws_mgr::get_instance().clear_title_with_big_player_clear(e_big_player_type_lord_or_war);
		}
		big_player_ws_mgr::get_instance().send_big_player_msg_to_other_server(false);
	}

	void cs2ws_receive_role_info_gm(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_role_info_to_gm* packet = static_cast<const cs2ws_send_role_info_to_gm*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_role_info_to_gm))
		{
			return;
		}
		http_send::get_instance().send_role_info(packet->role_info);
	}

	void cs2ws_req_set_big_player_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_set_big_player* packet = static_cast<const cs2ws_set_big_player*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_set_big_player))
		{
			return;
		}

		guid_64 winner_guid = packet->winner_guid;
		client_session* session = client_session_mgr::getInstance().get_session(winner_guid);
		if (nullptr == session)
		{
			winner_guid = guid_64();
		}
		int32 big_player_type = packet->big_player_type;
		if (big_player_type == e_big_player_type_lord_or_war)
		{
			pk_king_mgr& pk_king_mgr_ref = pk_king_mgr::get_instance();
			pk_king_mgr_ref.set_pk_king_player(winner_guid);
		}
		big_player_ws_mgr::get_instance().set_big_player_guid((e_big_player_type)big_player_type, winner_guid);		
		big_player_ws_mgr::get_instance().send_big_player_msg_to_other_server(false);
		if (false == winner_guid.is_valid() && init_unit::is_send_clear_title_with_clear_big_player(big_player_type))
		{
			big_player_ws_mgr::get_instance().clear_title_with_big_player_clear(big_player_type);
		}
	}

	void cs2ws_req_broken_sky_creat_boss(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_broken_sky_creat_boss* packet = static_cast<const cs2ws_broken_sky_creat_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_broken_sky_creat_boss))
		{
			return;
		}
		broken_sky_mgr& broken_sky_mgr_ref = broken_sky_mgr::get_instance();
		broken_sky_mgr_ref.sync_creat_boss_to_cs(packet->from_map_guid);
	}

	void cs2ws_req_broken_sky_info_sync(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_broken_sky_info_sync* packet = static_cast<const cs2ws_broken_sky_info_sync*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		broken_sky_mgr& broken_sky_mgr_ref = broken_sky_mgr::get_instance();
		broken_sky_mgr_ref.sync_game_info_to_all_map_inst(packet->map_template_id, packet->broken_sky_inst_guid, packet->damage, packet->top_records, packet->record_num);
	}

	void cs2ws_req_broken_sky_kill_boss(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_broken_sky_kill_boss* packet = static_cast<const cs2ws_broken_sky_kill_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_broken_sky_kill_boss))
		{
			return;
		}

		broken_sky_mgr& broken_sky_mgr_ref = broken_sky_mgr::get_instance();
		broken_sky_mgr_ref.sync_kill_boss_to_all_map_inst(packet->map_template_id, packet->killer_unit_index, packet->killer_guid);
	}

	void cs2ws_rep_get_map_info_from_cs_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_get_map_info_from_cs_end* packet = static_cast<const cs2ws_get_map_info_from_cs_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_get_map_info_from_cs_end))
		{
			return;
		}
		int64 map_info[map_info_arr_max_num];
		for (int32 i = 0; i < map_info_arr_max_num; i++)
		{
			map_info[i] = packet->map_info[i];
		}
		cs_map_mgr_system::send_one_map_info_to_client(packet->role_guid, packet->map_template_id, map_info);
	}

	//////////////////////////////////////////// team /////////////////////////////////////////////

	void cs2ws_req_update_team_member_info_one(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_update_team_member_info_one* packet = static_cast<const cs2ws_update_team_member_info_one*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_update_team_member_info_one))
		{
			return;
		}

		guid_64 role_guid = packet->member_guid;
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}

		int32 info_index = packet->info_index;
		int32 info_value = packet->info_value;

		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(role_guid);
		if (nullptr != team_ws_ptr)
		{
			s_team_member_info* member_info = team_ws_ptr->get_member(role_guid);
			if (nullptr != member_info)
			{
				member_info->data_ary[info_index] = info_value;
				team_ws_ptr->send_team_member_info_one(role_guid, info_index);
			}
		}
	}

	void cs2ws_req_update_team_member_pos_info_one(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_update_team_member_pos_info_one* packet = static_cast<const cs2ws_update_team_member_pos_info_one*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_update_team_member_pos_info_one))
		{
			return;
		}
		guid_64	team_guid = packet->team_guid;
		client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == session)
		{
			return;
		}
		team_ws* team_ws_ptr = nullptr;
		team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
		if (nullptr != team_ws_ptr)
		{
			team_ws_ptr->update_team_member_position(packet->role_guid, packet->pos_x, packet->pos_y, packet->pos_z);
		}
	}

	void cs2ws_req_create_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_create_team* packet = static_cast<const cs2ws_create_team*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_create_team))
		{
			return;
		}

		guid_64 role_guid = packet->captain_info.role_guid;
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (nullptr == session)
		{
			return;
		}
		
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();

		// 已经有了队伍就不能再创建队伍了
		if (team_ws_mgr_ref.get_unit_team(role_guid))
		{
			return;
		}
		
		e_team_type team_type = packet->team_type;
		int32 team_sub_type_id = packet->team_sub_type_id;
		int32 required_fighting_power = packet->required_fighting_power;
		int32 required_level = packet->required_level;
		bool auto_matching_member = packet->auto_accept_new_member;
		int32 one_stop_ = packet->team_one_stop_flags;
		const s_team_member_info& captain_info = packet->captain_info;

		if (team_type >= e_team_type_max)
		{
			return;
		}
		
		if (team_sub_type_id <= 0)
		{
			team_sub_type_id = -1;
		}
		if (team_type == e_team_type_none)
		{
			// modify by wangsonghgao : 之前的逻辑是队伍目标为"全部"的时候是不能开启自动匹配的
			// 现在根据需求修改为队伍目标为"全部"的也可以开启自动匹配
			// auto_matching_member = false;
			team_sub_type_id = -1;
		}

		// 加入队伍所需的战斗力不能是负数
		if (required_fighting_power < 0)
		{
			return;
		}

		if (false == team_ws_mgr_ref.is_team_type_valid(team_type, team_sub_type_id))
		{
			return;
		}

		team_ws_mgr_ref.create_new_team(session, captain_info, team_type, team_sub_type_id, auto_matching_member, one_stop_, required_fighting_power, required_level);
	}

	void cs2ws_req_change_team_type(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_change_team_type* packet = static_cast<const cs2ws_change_team_type*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_change_team_type))
		{
			return;
		}
		guid_64 player_guid = packet->player_guid;
		guid_64 team_guid = packet->team_guid;
		client_session* session = client_session_mgr::getInstance().get_session(player_guid);
		if (nullptr == session)
		{
			return;
		}
		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}
		for (int32 i = 0; i < max_team_member_num; i++)
		{
			guid_64 team_member_guid = packet->team_member_info[i].role_guid;
			client_session* team_mem_session = client_session_mgr::getInstance().get_session(team_member_guid);
			if (nullptr == team_mem_session)
			{
				continue;
			}
			team_ws_ptr->update_member(team_mem_session, packet->team_member_info[i]);
		}
		//team_ws_ptr->set_one_stop_flags(packet->one_stop_);
		team_ws_ptr->send_team_info_to_all_member();
		team_ws_ptr->change_team_type(*packet);

	}

	void cs2ws_req_join_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_join_team* packet = static_cast<const cs2ws_join_team*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_join_team))
		{
			return;
		}

		guid_64 team_guid = packet->team_guid;
		s_team_member_info member_info = packet->member_info;
		guid_64 new_member_guid = member_info.role_guid;

		client_session* session = client_session_mgr::getInstance().get_session(new_member_guid);
		if (nullptr == session)
		{
			return;
		}

		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			// 客户端处理的时候可以顺便删了
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_team_already_dissolve);
			return;
		}

		if (team_ws_ptr->get_member_num() >= max_team_member_num)
		{
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_team_member_is_full);
			return;
		}

		client_session* captain_session = client_session_mgr::getInstance().get_session(team_ws_ptr->get_captain_guid());
		if (nullptr == captain_session)
		{
			return;
		}
		
		if (team_ws_mgr_ref.get_unit_team(new_member_guid) != nullptr)
		{
			team_ws_mgr_ref.send_team_error_to_session(captain_session, e_team_error_player_already_join_other_team);
			return;
		}

		if (false == team_ws_ptr->is_have_invite_guid(new_member_guid))//检测是否存在于邀请列表中
		{
			return;
		}
		team_ws_ptr->del_invite_guid(new_member_guid);
		if (team_ws_ptr->add_member(session, member_info, false) == false)
		{
			return;
		}

		team_ws_ptr->send_add_team_member_message(member_info);
	}
	void cs2ws_join_team_assist_fight(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_join_team* packet = static_cast<const cs2ws_join_team*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_join_team))
		{
			return;
		}

		guid_64 team_guid = packet->team_guid;
		s_team_member_info member_info = packet->member_info;
		guid_64 new_member_guid = member_info.role_guid;

		client_session* session = client_session_mgr::getInstance().get_session(new_member_guid);
		if (nullptr == session)
		{
			return;
		}

		team_ws_mgr& team_ws_mgr_ref = team_ws_mgr::get_instance();
		team_ws* team_ws_ptr = team_ws_mgr_ref.get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			// 客户端处理的时候可以顺便删了
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_team_already_dissolve);
			return;
		}

		if (team_ws_ptr->get_member_num() >= max_team_member_num)
		{
			team_ws_mgr_ref.send_team_error_to_session(session, e_team_error_team_member_is_full);
			return;
		}

		client_session* captain_session = client_session_mgr::getInstance().get_session(team_ws_ptr->get_captain_guid());
		if (nullptr == captain_session)
		{
			return;
		}

		if (team_ws_mgr_ref.get_unit_team(new_member_guid) != nullptr)
		{
			team_ws_mgr_ref.send_team_error_to_session(captain_session, e_team_error_player_already_join_other_team);
			return;
		}
		if (team_ws_ptr->add_member(session, member_info, false) == false)
		{
			return;
		}

		team_ws_ptr->send_add_team_member_message(member_info);
	}
	void cs2ws_req_apply_to_join_team(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_apply_to_join_team* packet = static_cast<const cs2ws_apply_to_join_team*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_apply_to_join_team))
		{
			return;
		}

		guid_64 team_guid = packet->team_guid;
		s_team_member_info applicant_info = packet->applicant_info;

		guid_64 applicant_guid = applicant_info.role_guid;
		client_session* applicant_session = client_session_mgr::getInstance().get_session(applicant_guid);
		if (nullptr == applicant_session)
		{
			return;
		}
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

// 		// 如果目标队伍的活动地图和发起请求的不一样就不能申请
// 		int32 need_team_map_id = packet->team_map_id;
// 		if (need_team_map_id > 0 && team_ws_ptr->get_team_map_tempalte_id() != need_team_map_id)
// 		{
// 			return;
// 		}

		if (team_ws_ptr->is_auto_matching() && team_ws_ptr->add_member(applicant_session, applicant_info, true))
		{
			team_ws_ptr->send_add_team_member_message(applicant_info);
		}
		else
		{
			guid_64 captain_guid = team_ws_ptr->get_captain_guid();
			client_session* captain_session = client_session_mgr::getInstance().get_session(captain_guid);
			if (nullptr == captain_session)
			{
				return;
			}
			team_ws_ptr->add_invite_guid(applicant_guid);
			// 给队长发送申请信息
			team_proto_join_team_applicant_info join_team_applicant_info_msg;
			join_team_applicant_info_msg.set_role_guid(applicant_info.role_guid.server_64);
			join_team_applicant_info_msg.set_team_guid(team_ws_ptr->get_team_guid().server_64);
			join_team_applicant_info_msg.set_player_template_id(applicant_info.data_ary[ETeamMemberInfo_player_template_id]);
			join_team_applicant_info_msg.set_role_name(applicant_info.role_name);
			join_team_applicant_info_msg.set_level(applicant_info.data_ary[ETeamMemberInfo_level]);
			captain_session->send_to_client(&join_team_applicant_info_msg, e_msgindex_s2c_join_team_applicant_info);

			// 给申请者发送一个申请反馈
			team_proto_join_team_apply_end join_team_apply_end_msg;
			join_team_apply_end_msg.set_applied_team_guid(team_ws_ptr->get_team_guid().server_64);
			applicant_session->send_to_client(&join_team_apply_end_msg, e_msgindex_s2c_join_team_apply_end);
		}
	}

	void cs2ws_req_del_team_member(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_del_team_member* packet = static_cast<const cs2ws_del_team_member*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_del_team_member))
		{
			return;
		}

		guid_64 team_guid = packet->team_guid;
		guid_64 member_guid = packet->member_guid;

		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		team_ws_ptr->del_member(member_guid);
	}

	void cs2ws_req_team_game_over(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_team_game_over* packet = static_cast<const cs2ws_team_game_over*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_team_game_over))
		{
			return;
		}

		guid_64 map_guid = packet->raid_map_guid;
		auto map_ent = get_entity(map_guid);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("map_ent is nullptr map_guid:{}", map_guid.server_64);
			return;
		}
		auto map_cp = map_ent->get_component<cs_map_component>();

		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_team(map_cp->m_own_team_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}

		team_ws_ptr->on_team_game_over();
	}
	
	void cs2ws_recv_team_common_check_in_cs(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		team_ws_mgr::get_instance().recv_condition_check_from_cs(conn_index, data_ptr, data_len);
	}
	
	void cs2ws_req_team_change_aim_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_team_change_aim_info* packet = static_cast<const cs2ws_team_change_aim_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_team_change_aim_info))
		{
			return;
		}

		guid_64 player_guid = packet->role_guid;

		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(player_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}
		if (team_ws_ptr->get_captain_guid() != player_guid)
		{
			return;
		}
		
		e_team_type team_type_id = packet->team_type_id;
		int32 team_sub_type_id = packet->team_sub_type_id;

		if (false == team_ws_ptr->change_team_aim(team_type_id, team_sub_type_id))
		{
			return;
		}
		
		team_ws_ptr->send_all_member_aoi_team_info();

		//一条龙改变目标的时候 重新开始automatch么
		team_ws_ptr->on_team_attribute_changed();
		
		//team_ws_ptr->sync_team_attribute_to_mems();();
		team_proto_update_team_attribute_end update_team_attr_end_msg;
		update_team_attr_end_msg.set_n_team_type_id(team_ws_ptr->get_team_type());
		update_team_attr_end_msg.set_n_team_sub_type_id(team_ws_ptr->get_team_sub_type_id());
		team_ws_ptr->send_message_to_all_member(&update_team_attr_end_msg, e_msgindex_s2c_update_team_attribute_end);
	}

	void cs2ws_req_after_team_aim_finish_op(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_after_team_aim_finish_op* packet = static_cast<const cs2ws_after_team_aim_finish_op*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_after_team_aim_finish_op))
		{
			return;
		}

		guid_64 player_guid = packet->role_guid;

		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(player_guid);
		if (nullptr == team_ws_ptr)
		{
			return;
		}
		
		// 现在每个成员有了 one_stop_flags 直接就知道助战状态了 这里不算也行
		if (packet->map_check_ret >= e_error_code_success)
		{
			team_ws_ptr->set_member_check_ret(player_guid, (e_error_code)packet->map_check_ret);
		}
		
		int32 op_ret_one_stop_flags = packet->role_one_stop_flags;
		if (op_ret_one_stop_flags >= 0)
		{
			team_ws_ptr->set_member_one_stop_flags(player_guid, op_ret_one_stop_flags);

			if (team_ws_ptr->get_captain_guid() == player_guid && team_ws_ptr->is_one_stop_team()) //说明现在就是一条龙队伍
			{
				if (team_ws_ptr->set_one_stop_flags(op_ret_one_stop_flags))
				{
					team_ws_ptr->send_all_member_aoi_team_info();
					team_ws_ptr->on_team_attribute_changed();
					
					//team_ws_ptr->sync_team_attribute_to_mems();
					team_proto_update_team_attribute_end update_team_attr_end_msg;
					update_team_attr_end_msg.set_n_one_stop_flags(team_ws_ptr->get_one_stop_flags());
					team_ws_ptr->send_message_to_all_member(&update_team_attr_end_msg, e_msgindex_s2c_update_team_attribute_end);;
				}
			}
		}
	}

	// best_record
	void cs2ws_req_set_best_record(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_set_best_record* packet = static_cast<const cs2ws_set_best_record*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_set_best_record))
		{
			return;
		}
		if (best_record_mgr::get_instance().is_use_lua())
		{
			cs2ws_set_best_record * tmp = const_cast<cs2ws_set_best_record *>(packet);
			hld::cs2ws_proto::set_best_record pro_msg;
			tmp->to_proto(pro_msg);
			packet_s2s* p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "best_record_mgr_set_best_record", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}

		best_record_mgr& best_record_mgr_ref = best_record_mgr::get_instance();
		best_record_mgr_ref.set_best_record(packet->map_template_id,packet->record);
	}

	// mail
	void cs2ws_find_and_send_mail_to_player(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_mail* packet = static_cast<const cs2ws_send_mail*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_mail))
		{
			return;
		}

		mail_ws_mgr::get_instance().send_mail(packet->addressee_guid, packet->receiver_server_id, packet->mail_info, packet->item_list, packet->item_num);
	}

	void cs2ws_send_mail_to_all_player_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_mail_to_all_player* packet = static_cast<const cs2ws_send_mail_to_all_player*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_mail_to_all_player))
		{
			return;
		}

		event_ws_mgr::get_instance().add_globel_mail(packet->mail_info, packet->item_list, packet->item_num);
	}

	void cs2ws_check_id_mail_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_check_id_mail* packet = static_cast<const cs2ws_check_id_mail*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_check_id_mail))
		{
			return;
		}
		mail_ws_mgr::get_instance().check_id_mail(packet->role_guid, packet->server_id, packet->cur_time);
	}

	void cs2ws_check_player_globel_mail_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_check_player_globel_mail_event* packet = static_cast<const cs2ws_check_player_globel_mail_event*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}
		event_ws_mgr::get_instance().check_player_globel_mail(packet->data_info, packet->data_num, packet->role_guid, packet->role_creat_time);
	}

	void cs2ws_sync_character_data(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_sync_char_data* packet = static_cast<const cs2ws_sync_char_data*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}

		if (data_len != sizeof(cs2ws_sync_char_data))
		{
			return;
		}
		client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		 

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(packet->role_guid);
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(packet->role_guid);
		
		switch (e_sync_cs2ws_data_type(packet->data_type))
		{
		case e_sync_cs2ws_data_gs_value:
		{
			session_ptr->set_role_gs_value(packet->data_value);
			arena_mgr_ws::get_instance().sync_player_gs(packet->role_guid, packet->data_value, packet->sub_data);
			ranking_mgr_ws::sync_player_arena_gs(packet->role_guid, packet->data_value);
			if (nullptr != legion_ws_ptr)
			{
				legion_ws_ptr->update_member_gs_value(packet->role_guid, packet->data_value);
			}
			if (nullptr != team_ws_ptr)
			{
				s_team_member_info* member_info = team_ws_ptr->get_member(packet->role_guid);
				if (nullptr != member_info)
				{
					member_info->gs_value = packet->data_value;
					team_ws_ptr->send_team_gs_value(packet->role_guid);
				}
			}
		}
		break;
		case e_sync_cs2ws_data_cur_hp:
		{
			session_ptr->m_role_data_ws_ex[e_role_data_ws_ex_cur_hp] = packet->data_value;
			if (nullptr != team_ws_ptr)
			{
				s_team_member_info* member_info = team_ws_ptr->get_member(packet->role_guid);
				if (nullptr != member_info)
				{
					member_info->data_ary[ETeamMemberInfo_cur_hp] = packet->data_value;
					team_ws_ptr->send_team_member_info_one(packet->role_guid, ETeamMemberInfo_cur_hp);
				}
			}
		}
		break;
		case e_sync_cs2ws_data_max_hp:
		{
			session_ptr->m_role_data_ws_ex[e_role_data_ws_ex_max_hp] = packet->data_value;
			if (nullptr != team_ws_ptr)
			{
				s_team_member_info* member_info = team_ws_ptr->get_member(packet->role_guid);
				if (nullptr != member_info)
				{
					member_info->data_ary[ETeamMemberInfo_max_hp] = packet->data_value;
					team_ws_ptr->send_team_member_info_one(packet->role_guid, ETeamMemberInfo_max_hp);
				}
			}
		}
		break;
		case e_sync_cs2ws_data_exp_level:
		{
			session_ptr->m_role_info.data_ary[e_role_info_exp_level] = packet->data_value;
			if (nullptr != legion_ws_ptr)
			{
				legion_ws_ptr->update_member_info_one(packet->role_guid, e_legion_member_info_level, packet->data_value);
			}
			if (nullptr != team_ws_ptr)
			{
				s_team_member_info* member_info = team_ws_ptr->get_member(packet->role_guid);
				if (nullptr != member_info)
				{
					member_info->data_ary[ETeamMemberInfo_level] = packet->data_value;
					team_ws_ptr->send_team_member_info_one(packet->role_guid, ETeamMemberInfo_level);
				}
			}
			marry_mgr_ws::get_instance().update_marry_data(session_ptr->get_role_guid());
		}
			break;
		case e_sync_cs2ws_data_player_template:
		{
			session_ptr->m_role_info.data_ary[e_role_info_template_id] = packet->data_value;
			if (nullptr != legion_ws_ptr)
			{
				legion_ws_ptr->update_member_info_one(packet->role_guid, e_legion_member_info_player_template_id, packet->data_value);
			}
			if (nullptr != team_ws_ptr)
			{
				s_team_member_info* member_info = team_ws_ptr->get_member(packet->role_guid);
				if (nullptr != member_info)
				{
					member_info->data_ary[ETeamMemberInfo_player_template_id] = packet->data_value;
					team_ws_ptr->send_team_member_info_one(packet->role_guid, ETeamMemberInfo_player_template_id);
				}
			}
		}
			break;
		case e_sync_cs2ws_military_rank:
		{
			arena_mgr_ws::get_instance().sync_player_arena_military_rank(packet->role_guid, packet->data_value, packet->sub_data);
		}
			break;
		case e_sync_cs2ws_data_dead_to_addhate:
		{
			client_session* hate_session = client_session_mgr::getInstance().get_session((guid_64)packet->data_value);
			if (nullptr != hate_session)
			{
				for (int32 i = e_relationlist_type_friend; i <= e_relationlist_type_hate; i++)
				{
					bool result = session_ptr->get_relation_list_mgr().is_relation_exist((e_relationlist_type)i, hate_session->get_role_guid());
					if (result)
					{
						return;
					}
				}
				session_ptr->get_relation_list_mgr().add_relation(e_relationlist_type_hate, hate_session->get_role_guid());
			}
		}
			break;
		case e_sync_cs2ws_data_vip_level:
		{
			session_ptr->set_role_info_data(e_role_info_vip_level, packet->data_value);
			if (nullptr != legion_ws_ptr)
			{
				legion_ws_ptr->update_member_info_one(packet->role_guid, e_legion_member_info_vip_level, packet->data_value);
			}
			//同步Vip到Gate
			element_war_ws_mgr::get_instance().send_req_element_war_role_info(session_ptr);
		}
			break;
		case e_sync_cs2ws_data_is_show_vip:
		{
			session_ptr->set_role_info_data(e_role_info_is_show_vip, packet->data_value);
			if (nullptr != legion_ws_ptr)
			{
				legion_ws_ptr->update_member_info_one(packet->role_guid, e_legion_member_info_is_show_vip, packet->data_value);
			}
		}
		break;
		default:
			break;			
		}
	}
		//arena
	void cs2ws_get_arena_rank(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_req_get_arena_rank* packet = static_cast<const cs2ws_req_get_arena_rank*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_req_get_arena_rank))
		{
			return;
		}
		arena_mgr_ws::get_instance().req_get_player_rank(packet->role_guid);
	}

	void lua_cs2ws_get_arena_rank(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_get_arena_rank, req_get_arena_rank);
	}

	void cs2ws_req_challenge(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_req_challenge_msg* packet = static_cast<const cs2ws_req_challenge_msg*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_req_challenge_msg))
		{
			return;
		}
		arena_mgr_ws::get_instance().req_challenge(packet->role_guid, packet->target_guid, packet->cost_money_type, packet->cost_money_num, packet->target_cur_pos, packet->self_cur_pos);
	}

	void lua_cs2ws_req_challenge(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_req_challenge, req_challenge_msg);
	}

	void cs2ws_req_get_choose_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_req_choose_list* packet = static_cast<const cs2ws_req_choose_list*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_req_choose_list))
		{
			return;
		}
		arena_mgr_ws::get_instance().get_choose_list(packet->role_guid, packet->cur_pos);

	}

	void lua_cs2ws_req_get_choose_list(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_req_get_choose_list, req_choose_list);
	}
	
	void cs2ws_req_get_first_three(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_first_three* packet = static_cast<const cs2ws_first_three*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_first_three))
		{
			return;
		}
		arena_mgr_ws::get_instance().get_first_three(packet->role_guid);

	}
	void lua_cs2ws_req_get_first_three(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_req_get_first_three, first_three);
	}
	void cs2ws_chellenge_over_req(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_chellenge_over* packet = static_cast<const cs2ws_chellenge_over*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_chellenge_over))
		{
			return;
		}

		if (packet->chellenge_result == e_arena_end_type_win)
		{
			arena_mgr_ws::get_instance().change_rank(packet->role_info, packet->target_guid, packet->target_rank);
		}
		else
		{
			arena_mgr_ws::get_instance().reset_challenge_state(packet->role_guid, packet->target_guid, packet->target_rank, packet->role_info.rank_pos);
		}
	}

	void lua_cs2ws_chellenge_over_req(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_chellenge_over_req, chellenge_over);
	}

	void cs2ws_arena_sync_gs_value_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_arena_sync_gs_value* packet = static_cast<const cs2ws_arena_sync_gs_value*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_arena_sync_gs_value))
		{
			return;
		}
		arena_mgr_ws::get_instance().sync_gs_value(packet->role_guid, packet->cur_pos, packet->gs_value);
	}

	void lua_cs2ws_arena_sync_gs_value_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_arena_sync_gs_value_proc, arena_sync_gs_value);
	}

	void cs2ws_send_globel_message_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_globel_message* packet = static_cast<const cs2ws_send_globel_message*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_globel_message))
		{
			return;
		}

		chat_proto_chat_text msg;
		msg.set_text(packet->message_text);
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_receive_globel_message);
	}


	void cs2ws_worship_player_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_req_worship_player* packet = static_cast<const cs2ws_req_worship_player*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_req_worship_player))
		{
			return;
		}

		ranking_mgr_ws::cs2ws_req_add_worship_func(e_RankingIndex(packet->ranking_index), packet->worship_type, packet->role_guid,packet->target_guid);
	}

	void cs2ws_sync_ranking_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const hld::cs2ws_sync_ranking_info* packet = static_cast<const cs2ws_sync_ranking_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_sync_ranking_info))
		{
			return;
		}
		int32 cur_ws_server_id = world_server::getInstance().get_server_id();
		if (cur_ws_server_id == packet->server_id)
		{
			ranking_mgr_ws::sync_player_info(packet->role_info);
		}
		else
		{
			cs2ws_sync_ranking_info req;
			req.role_info = packet->role_info;
			req.server_id = packet->server_id;
			cross::send_msg_to_ws(packet->role_info.role_guid, packet->server_id, e_msgindex_cs2ws_sync_ranking_info, &req, sizeof(req));
		}



	}


	void cs2ws_sync_ranking_player_vip_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const hld::cs2ws_sync_vip_level_data* packet = static_cast<const cs2ws_sync_vip_level_data*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_sync_vip_level_data))
		{
			return;
		}
		ranking_mgr_ws::cs2ws_sync_player_vip_level_func(packet->role_guid, packet->vip_level);
	}

	void cs2ws_req_service_rank_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const hld::cs2ws_req_service_rank_info* packet = static_cast<const cs2ws_req_service_rank_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_req_service_rank_info))
		{
			return;
		}
		
		ws2cs_req_service_rank_info_end result;
		result.rank_num = ranking_mgr_ws::get_player_service_rank_index(packet->role_guid, packet->rank_type);
		result.role_guid = packet->role_guid;
		result.rank_type = packet->rank_type;
		client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == session_ptr)
		{
			return;
		}
		bool is_use = proto_by_lua(e_msgindex_ws2cs_req_service_rank_info_end);
		if (is_use == false)
		{
			session_ptr->send_to_cs(&result, sizeof(ws2cs_req_service_rank_info_end));
		}
		else
		{
			hld::ws2cs_proto::req_service_rank_info_end pro_msg;
			result.to_proto(pro_msg);
			session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_req_service_rank_info_end);
		}
	}
	
	void cs2ws_send_notice_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_notice* packet = static_cast<const cs2ws_send_notice*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_notice))
		{
			return;
		}
		int32 create_time = time_helper::get_cur_time_new().second;
		int32 i = 0;
		cs2ws_send_notice resp;
		memcpy(&resp, packet, sizeof(cs2ws_send_notice));
		event_ws_mgr::get_instance().send_notice_to_all(packet->notice_id, create_time, packet->role_guid, packet->notice_string, resp.data_ary,packet->item_num);
	}

	void dp2ws_load_world_boss_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_world_boss_end* packet = static_cast<const dp2ws_load_world_boss_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != (const_cast<dp2ws_load_world_boss_end*>(packet))->get_pak_length())
		{
			return;
		}

		//world_boss_ws_mgr::get_instance().load_world_boss_event_end(packet->data_info, packet->data_num);
	}

	void cs2ws_world_boss_dead_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_world_boss_dead* packet = static_cast<const cs2ws_world_boss_dead*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_world_boss_dead))
		{
			return;
		}
		world_boss_ws_mgr::get_instance().set_world_boss_dead(packet->world_boss_spawn_template_id, packet->killer_name);

				//当前击杀boss不为世界boss 不往下面进行
		int32 cur_boss_spawn_tem_id = packet->world_boss_spawn_template_id;
		NpcSpawnPointTemplate* cur_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, cur_boss_spawn_tem_id);
		if (cur_spawn_point_template_ptr == nullptr)
		{
			return;
		}
		NpcTemplate*cur_npc_template_ptr = GET_TEMPLATE(NpcTemplate, cur_spawn_point_template_ptr->TemplateId);
		if (nullptr == cur_npc_template_ptr || cur_npc_template_ptr->SubType != e_monster_type_gold_army)
		{
			return;
		}
		//if (packet->world_boss_spawn_template_id == world_boss_ws_mgr::get_instance().get_cur_first_boss_spawn_id())
		//{
		//	world_boss_ws_mgr::get_instance().get_world_boss_info(world_boss_ws_mgr::get_instance().get_cur_first_boss_spawn_id())->set_creating(true);
		//}
		world_boss_ws_mgr::get_instance().clear_score();
		//int32 next_boss_spawn_tem_id = cur_spawn_point_template_ptr->NextSpawnNpcId;
		//if (next_boss_spawn_tem_id <= cur_boss_spawn_tem_id)
		//{
		//	return;
		//}
		//NpcSpawnPointTemplate* spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, next_boss_spawn_tem_id);
		//if (spawn_point_template_ptr == nullptr)
		//{
		//	return;
		//}
		//NpcTemplate*npc_template_ptr = GET_TEMPLATE(NpcTemplate, spawn_point_template_ptr->TemplateId);
		//if (npc_template_ptr == nullptr || npc_template_ptr->NpcType != e_unit_type_monster ||npc_template_ptr->SubType != e_monster_type_gold_army)
		//{
		//	world_boss_ws_mgr::get_instance().set_is_all_boss_dead(true);
		//	return;
		//}
		//world_boss_ws_mgr::get_instance().create_world_boss(next_boss_spawn_tem_id);

	}

	void cs2ws_create_world_boss_sucess_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_create_world_boss_success* packet = static_cast<const cs2ws_create_world_boss_success*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_create_world_boss_success))
		{
			return;
		}
		world_boss_ws_mgr::get_instance().create_world_boss_success(packet->world_boss_guid, packet->world_boss_npc_array_index, packet->world_boss_spawn_template_id, packet->world_boss_line_id);
	}

	void cs2ws_first_killer_get_prize(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_has_get_first_kill_prize* packet = static_cast<const cs2ws_has_get_first_kill_prize*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_has_get_first_kill_prize))
		{
			return;
		}

		world_boss_ws_mgr::get_instance().first_killer_get_prize_func(packet->first_kill_welfare_template_id, packet->is_get_prize, packet->reward_array);

	}



	void cs2ws_add_friendliness_value_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		const cs2ws_add_friendliness_value* msg_pak = static_cast<const cs2ws_add_friendliness_value*>(data_ptr);

		client_session* sender_ptr = client_session_mgr::getInstance().get_session(msg_pak->sender_guid);
		if (nullptr == sender_ptr)
		{
			return;
		}

		sender_ptr->get_relation_list_mgr().send_gift_add_friendliness_func(msg_pak->addreessee_guid, msg_pak->gift_id, msg_pak->gift_count);

	}

	void cs2ws_fuben_add_friendliness_value_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		const cs2ws_fuben_add_friendliness_value* msg_pak = static_cast<const cs2ws_fuben_add_friendliness_value*>(data_ptr);

		for (int32 i = 0; i < hld::max_team_member_num; i++)
		{
			client_session* sender_ptr = client_session_mgr::getInstance().get_session(msg_pak->team_member_guid[i]);
			if (nullptr == sender_ptr)
			{
				return;
			}
			sender_ptr->get_relation_list_mgr().fuben_add_friendliness_func(msg_pak->team_member_guid, msg_pak->team_num);
		}
	
	}

	void cs2ws_check_item_enough_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		const cs2ws_operate_result_end* msg_pak = static_cast<const cs2ws_operate_result_end*>(data_ptr);

		if (!msg_pak->is_divorce)
		{
			if (msg_pak->is_cost_item)
			{
				marry_mgr_ws::get_instance().cs2ws_cost_marry_item_end(msg_pak->main_guid, msg_pak->sub_guid, msg_pak->result_type);
			}
			else
			{
				marry_mgr_ws::get_instance().cs2ws_check_marry_end(msg_pak->main_guid, msg_pak->sub_guid, msg_pak->result_type, msg_pak->item_id);
			}
		}
		else
		{
			if (msg_pak->is_cost_item)
			{
				marry_mgr_ws::get_instance().cs2ws_force_divorce_end(msg_pak->main_guid, msg_pak->result_type);
			}
			else
			{
				marry_mgr_ws::get_instance().cs2ws_check_divorce_end(msg_pak->main_guid, msg_pak->result_type);
			} 
		} 
	}

	void cs2ws_ret_get_reward_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const ws2cs_marry_get_reward_end* packet = static_cast<const ws2cs_marry_get_reward_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_marry_get_reward_end))
		{
			return;
		}
		 
		marry_mgr_ws::get_instance().cs2ws_get_reward_end(packet->sender_guid,packet->old_marry_state, packet->get_result);
	}

	void cs2ws_update_wedding_ring_level_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_update_wedding_ring_level* packet = static_cast<const cs2ws_update_wedding_ring_level*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_update_wedding_ring_level))
		{
			return;
		}
		marry_mgr_ws::get_instance().cs_update_wedding_ring_level(packet->role_guid, packet->ring_level);
	}

	void cs2ws_add_heart_value_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_add_heart_value* packet = static_cast<const cs2ws_add_heart_value*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_add_heart_value))
		{
			return;
		}
		if (marry_mgr_ws::get_instance().add_heart_value(packet->role_guid, packet->add_value))
		{
			client_session* session = client_session_mgr::getInstance().get_session(packet->role_guid);
			if (session)
			{
				int32 notice_id = 0;
				if (packet->send_type == 0)
				{
					notice_id = 90096888;
				}
				else if (packet->send_type == 1)
				{
					notice_id = 90096904;
				}

				std::vector<std::string> vec_notice_str;
				const std::string& temp_string = template_manager::get_instance().get_str_by_string_template_id(notice_id);
				vec_notice_str.push_back(temp_string);
				vec_notice_str.push_back(init_unit::change_i32_to_string(packet->add_value));
				xstring notice_str = init_unit::implode(vec_notice_str);
				session->send_notice(notice_str);
			}
		}
	}

	void cs2ws_cost_bless_money_result_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_cost_bless_money_result* packet = static_cast<const cs2ws_cost_bless_money_result*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_cost_bless_money_result))
		{
			return;
		}
		marry_mgr_ws::get_instance().req_bless_one_couple_end(packet->role_name, packet->target_guid);
	}

	void cs2ws_send_lucky_info_to_client_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_send_lucky_info_to_client* packet = static_cast<const cs2ws_send_lucky_info_to_client*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_lucky_info_to_client))
		{
			return;
		}

		person_infor_ws_mgr::get_instance().send_lucky_info_by_type(packet->target_guid, (e_lucky_info_req_type)packet->req_type);
	}

	void cs2ws_reconnect_send_red_package_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_reconnect_send_red_package_info* packet = static_cast<const cs2ws_reconnect_send_red_package_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_reconnect_send_red_package_info))
		{
			return;

		}
		red_package_ws_mgr::get_instance().send_info_to_one(packet->role_guid);
	}

	void cs2ws_send_red_package_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_red_package* packet = static_cast<const cs2ws_send_red_package*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_red_package))
		{
			return;
		}
		std::string send_content = packet->send_text;
		invalid_ansi_word::replace_invalid_ansi_str(send_content);

		int32 add_result = red_package_ws_mgr::get_instance().add_red_package(packet->red_package_template_id, packet->sender_guid, packet->sender_name, send_content);
		if (add_result != e_red_bag_send_success &&
			add_result != e_red_bag_get_globel_success &&
			add_result != e_red_bag_get_legion_success &&
			add_result != e_red_bag_get_vip_success)
		{
			RedPackageTemplate* red_pack_template_ptr = GET_TEMPLATE(RedPackageTemplate, packet->red_package_template_id);
			if (red_pack_template_ptr != nullptr
				&& red_pack_template_ptr->CostMoneyArray.size() >= 2)
			{
				const xstring& mail_context = template_manager::get_instance().get_str_by_string_template_id(e_red_bag_refund_mail_text);
				s_item_info gift_item;
				std::vector<s_item_info> item_list;
				if (red_pack_template_ptr->LogicID != 0)
				{
					bool result = hld::init_unit::init_item_data(gift_item, red_pack_template_ptr->LogicID, -1, 1, e_bag_type_bag, 1);
					if (result)
					{
						item_list.push_back(gift_item);
					}
				}
				std::string mail_title_name = template_manager::get_instance().get_str_by_string_template_id(90303013);
				event_ws_mgr::get_instance().send_mail_to_player_by_system(packet->sender_guid, 0, mail_title_name, "REFUND", mail_context
					, red_pack_template_ptr->CostMoneyArray[0], red_pack_template_ptr->CostMoneyArray[1]
					, 0, 0
					, item_list);
			}
		}

		client_session* session = client_session_mgr::getInstance().get_session(packet->sender_guid);
		if (session && session->get_cs_conn_index() >= 0)
		{
			ws2cs_send_red_package_end end_msg;
			end_msg.sender_guid = packet->sender_guid;
			end_msg.red_package_template_id = packet->red_package_template_id;
			end_msg.send_result = add_result;
			bool is_use = proto_by_lua(e_msgindex_ws2cs_send_red_pack_end);
			if (is_use == false)
			{
				session->send_to_cs(&end_msg, sizeof(ws2cs_send_red_package_end));
			}
			else
			{
				hld::ws2cs_proto::send_red_package_end pro_msg;
				end_msg.to_proto(pro_msg);
				session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_send_red_pack_end);
			}
		}
	}

	void lua_cs2ws_send_red_package_process(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_send_red_package_process, send_red_package);
	}

	void cs2ws_get_red_package_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_get_red_package* packet = static_cast<const cs2ws_get_red_package*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_get_red_package))
		{
			return;
		}
		red_package_ws_mgr::get_instance().get_red_package(packet->red_package_guid, packet->role_guid, packet->role_name);
	}
	void lua_cs2ws_get_red_package_process(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_CS_WS_PACKET(cs2ws_get_red_package_process, get_red_package);
	}
	void cs2ws_send_chat_to_ws_process_new(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//chat比较特殊，不能直接发客户端，因为他有一些 发送失败、成功 的逻辑+回复信息
		const cs2ws_send_chat_to_ws_new* packet = static_cast<const cs2ws_send_chat_to_ws_new*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != (const_cast<cs2ws_send_chat_to_ws_new*>(packet))->get_pak_length())
		{
			return;
		}
		guid_64 sender_guid = packet->sender_guid;

		chat_proto_chat_content msg;
		msg.set_chat_typ(packet->chat_type);
		msg.set_sender_guid(sender_guid.server_64);
		msg.set_sender_guid_a(sender_guid.A);
		msg.set_sender_guid_b(sender_guid.B);
		msg.set_sender_template_id(packet->sender_template_id);
		msg.set_sender_name(packet->sender_name);
		msg.set_relation_template_id(packet->sender_template_id);
		msg.set_relation_level(packet->sender_exp_level);
		guid_64 chat_guid = guid_gen::make_guid();
		msg.set_chat_guid(chat_guid.server_64);
		msg.set_chat_guid_a(chat_guid.A);
		msg.set_chat_guid_b(chat_guid.B);
		msg.set_vip_title_template_id(packet->sender_vip_title_template_id);
		msg.set_head_frame_id(packet->sender_head_frame_id);
		msg.set_server_id(packet->sender_server_id);
		//msg.set sender_head_frame_id(packet->sender_head_frame_id);
		//军团消息
		std::vector<xstring> legion_answer_content_vec;
		if (packet->chat_type == e_chat_type_legion)
		{
			for (int32 i = 0; i < packet->content_info_num; ++i)
			{
				e_chat_content_info temp_info = packet->content_info[i];
				if (temp_info.is_item() == 0 && temp_info.is_loction() == false && temp_info.template_id == 0)
				{
					legion_answer_content_vec.push_back(temp_info.common_text);
				}
			}
		}

		e_chat_content_info		content_info[chat_max_word_num];
		memcpy(content_info, packet->content_info, sizeof(e_chat_content_info) * chat_max_word_num);
		chat_mgr_ws::get_instance().forbidden_word_check(content_info, packet->content_info_num);

		std::vector<xstring> chat_content_vec;
		for (int32 i = 0; i < packet->content_info_num; ++i)
		{
			e_chat_content_info temp_info = content_info[i];
			chat_proto_chat_content_info* temp_info_msg = msg.add_content_info_array();
			if (temp_info.is_item())
			{
				for (int32 j = 0; j < e_item_info_max; ++j)
				{
					temp_info_msg->add_item_data(temp_info.item_info[j]);
				}
			}
			else if (temp_info.is_loction())
			{
				temp_info_msg->add_location(temp_info.location[0]);
				temp_info_msg->add_location(temp_info.location[1]);
				temp_info_msg->add_location(temp_info.location[2]);
				temp_info_msg->set_map_id(temp_info.map_id);
				temp_info_msg->set_line_id(temp_info.line_id);
			}
			else if (temp_info.template_id != 0)
			{
				temp_info_msg->set_template_id(temp_info.template_id);
			}
			else
			{
				temp_info_msg->set_commontext(temp_info.common_text);
				chat_content_vec.push_back(temp_info.common_text);
			}
			temp_info_msg->set_audio_id(temp_info.audio_id);
			temp_info_msg->set_audio_duration_time(temp_info.duration_time);
			temp_info_msg->set_face_finish_img_id(temp_info.face_finish_img_id);
		}
		msg.set_is_finish_send(true);

		switch (packet->chat_type)
		{
		case e_chat_type_server:
		{
			client_session_mgr::getInstance().send_message_to_all_server_client(&msg, e_msgindex_s2c_receive_chat_new, e_msgindex_ws2ws_chat_server_all_member);
		}
		break;
		case e_chat_type_system:
		case e_chat_type_world:
		{
			if (packet->chat_type == e_chat_type_system && packet->addressee_guid.is_valid())
			{
				if (packet->addressee_guid.A)
				{
					client_session_mgr::getInstance().send_message_to_all_area_server_client(&msg, e_msgindex_s2c_receive_chat_new, e_msgindex_ws2ws_chat_server_all_member);
				}
				else
				{
					client_session_mgr::getInstance().send_message_to_all_server_client(&msg, e_msgindex_s2c_receive_chat_new, e_msgindex_ws2ws_chat_server_all_member);

				}
			}
			else
			{
				client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_receive_chat_new);
			}
		}
		break;
		case e_chat_type_legion:
		{			
			legion_ws* temp_legion = legion_ws_mgr::get_instance().get_unit_legion(sender_guid);
			if (temp_legion != nullptr)
			{
				legion_ws_answer::get_instance().answer_legion_question(sender_guid, legion_answer_content_vec);
				msg.add_addressee_guid(temp_legion->get_legion_guid().A);
				msg.add_addressee_guid(temp_legion->get_legion_guid().B);
			}
			client_session_mgr::getInstance().send_message_to_legion(sender_guid, &msg, e_msgindex_s2c_receive_chat_new);
			client_session_mgr::getInstance().send_message_to_cur_server_legion(sender_guid, &msg, e_msgindex_s2c_receive_chat_new);

		}
		break;
		case e_chat_type_team:
		{
			team_ws* temp_team = team_ws_mgr::get_instance().get_unit_team(sender_guid);
			if (temp_team != nullptr)
			{
				temp_team->send_message_to_all_member(&msg, e_msgindex_s2c_receive_chat_new);
			}
		}
		break;
		case e_chat_type_friend:
		case e_chat_type_private:
		{
			guid_64 addressee_guid = packet->addressee_guid;
			client_session* temp_session = client_session_mgr::getInstance().get_session(addressee_guid);
			ws2cs_send_chat_end failed_msg;
			failed_msg.sender_guid = sender_guid;
			failed_msg.chat_type = packet->chat_type;
			if (temp_session == nullptr)
			{
				if (chat_content_vec.size() > 0)
				{
					//目标玩家离线保存到数据库
					ws2dp_save_chat_with_offline offline_msg;
					offline_msg.sender_guid = packet->sender_guid;
					offline_msg.sender_templete_id = packet->sender_template_id;
					offline_msg.chat_type = packet->chat_type;
					offline_msg.address_guid = packet->addressee_guid;
					offline_msg.set_sender_name(packet->sender_name);
					offline_msg.relation_template_id = packet->sender_template_id;
					offline_msg.relation_level = packet->sender_exp_level;
					offline_msg.chat_guid = msg.chat_guid();
					offline_msg.vip_title_template_id = packet->sender_vip_title_template_id;
					offline_msg.head_frame_id = packet->sender_head_frame_id;
					xstring chat_content = init_unit::implode(chat_content_vec);
					offline_msg.set_content_text(chat_content);

					ws_client::getInstance().send_to_dp(&offline_msg, sizeof(offline_msg));
				}
			}
			else if (!temp_session->is_in_game())
			{
				failed_msg.send_result = e_chat_player_state_cant_chat;
				client_session* sender_session = client_session_mgr::getInstance().get_session(sender_guid);
				if (sender_session != nullptr)
				{
					bool is_use = proto_by_lua(e_msgindex_ws2cs_send_chat_end);
					if (is_use == false)
					{
						sender_session->send_to_cs(&failed_msg, sizeof(ws2cs_send_chat_end));
					}
					else
					{
						hld::ws2cs_proto::send_chat_end pro_msg;
						failed_msg.to_proto(pro_msg);
						sender_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_send_chat_end);
					}
				}
			}
			else
			{
				msg.add_addressee_guid(addressee_guid.A);
				msg.add_addressee_guid(addressee_guid.B);
				temp_session->send_to_client(&msg, e_msgindex_s2c_receive_chat_new);

				//这里说明发送成功，再回发给自己，用于显示 自己说了什么
				client_session* self_session = client_session_mgr::getInstance().get_session(sender_guid);
				//这里要附加上“收件人名称”，用于显示“我对XXX说”
				msg.set_addressee_name(temp_session->m_role_info.role_name);

				if (self_session != nullptr)
				{
					temp_session->get_relation_list_mgr().add_chat(self_session);
					temp_session->get_relation_list_mgr().recv_chat(self_session->get_role_guid());
					msg.clear_addressee_guid();
					msg.add_addressee_guid(sender_guid.A);
					msg.add_addressee_guid(sender_guid.B);
					self_session->send_to_client(&msg, e_msgindex_s2c_receive_chat_new);
					self_session->get_relation_list_mgr().add_chat(temp_session);
				}
			}

		}
		break;
		case e_chat_type_radio_host:
		{
			client_session* sender_session = client_session_mgr::getInstance().get_session(sender_guid);
			if (sender_session != nullptr)
			{
				sender_session->send_to_client(&msg, e_msgindex_s2c_receive_chat_new);
			}
		}
		break;
		default:
			break;
		}
	}

	void cs2ws_send_notice_with_param_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_notice_with_param* packet = static_cast<const cs2ws_send_notice_with_param*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_notice_with_param))
		{
			return;
		}
		std::vector<int32> param_array;
		param_array.reserve(e_item_info_max);
		for (int32 i = 0; i < e_item_info_max; ++i)
		{
			if (i >= packet->param_len)
			{
				break;
			}
			param_array.push_back(packet->param_array[i]);
		}

		event_ws_mgr::get_instance().send_notice_with_param(packet->sender_guid, packet->sender_name,
			packet->content_text, param_array,
			packet->param_type, packet->chat_typ, packet->template_id, packet->notice_id);
	}

	void cs2ws_send_chat_result(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_send_chat_end* packet = static_cast<const cs2ws_send_chat_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_chat_end))
		{
			return;
		}
		guid_64 sender_guid = packet->sender_guid;
		ws2cs_send_chat_end msg;
		msg.sender_guid = sender_guid;
		msg.send_result = packet->send_result;

		client_session* sender_session = client_session_mgr::getInstance().get_session(sender_guid);
		if (sender_session == nullptr)
		{
			return;
			//sender_session->send_to_cs(&msg,sizeof(ws2cs_send_chat_end));
		}
		bool is_use = proto_by_lua(e_msgindex_ws2cs_send_chat_end);
		if (is_use == false)
		{
			sender_session->send_to_cs(&msg, sizeof(ws2cs_send_chat_end));
		}
		else
		{
			hld::ws2cs_proto::send_chat_end pro_msg;
			msg.to_proto(pro_msg);
			sender_session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_send_chat_end);
		}

	}

	void cs2ws_create_big_player_sucess_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_create_big_player_success* packet = static_cast<const cs2ws_create_big_player_success*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_create_big_player_success))
		{
			return;
		}
		if (big_player_ws_mgr::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "big_player_mgr_crate_big_player_statue_success", 0, false, "%d%l", packet->big_type, packet->big_player_guid.server_64);
			return;

		}
		big_player_ws_mgr::get_instance().crate_big_player_statue_success(packet->big_type, packet->big_player_guid);
	}

	void cs2ws_add_lucky_draw_record(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_add_new_draw_record* packet = static_cast<const cs2ws_add_new_draw_record*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_add_new_draw_record))
		{
			return;
		}

		lucky_draw_record_ws_mgr::get_instance().add_record(packet->record_info);
	}

	//////////////////////////////////////////////////////////////////////////
	//auction
	void cs2ws_auction_sell_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_add_item_into_db* packet = static_cast<const cs2ws_auction_add_item_into_db*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_add_item_into_db))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			hld::cs2ws_proto::auction_add_item_into_db pro_msg;
			cs2ws_auction_add_item_into_db* tmp = const_cast<cs2ws_auction_add_item_into_db*>(packet);
			tmp->to_proto(pro_msg);
			packet_s2s* p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_add_auction_info_to_db", 0, true, "%s",
				p_s2s->google_data, p_s2s->google_data_len);
			return;

		}

		auction_mgr_ws::get_instance().add_auction_info_to_db(packet->auction_info, packet->old_item_guid, packet->role_info, true, packet->third_info);
	}

	void cs2ws_auction_buy_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_buy* packet = static_cast<const cs2ws_auction_buy*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_buy))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_buy", 0, false, "%l%l",
				packet->role_guid.server_64, packet->item_guid.server_64);

			return;

		}

		auction_mgr_ws::get_instance().buy(packet->role_guid, packet->item_guid);
	}
	void cs2ws_auction_puchase_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_send_sell_success_info* packet = static_cast<const cs2ws_auction_send_sell_success_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_send_sell_success_info))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			cs2ws_auction_send_sell_success_info* tmp = const_cast<cs2ws_auction_send_sell_success_info*>(packet);
			packet_s2s* p_s2s = auction_mgr_ws::get_instance().get_auction_buffer(tmp->auction_info);
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_send_auction_puchase_success_info", 0, true, "%s",
				p_s2s->google_data, p_s2s->google_data_len);
			return;

		}

		auction_mgr_ws::get_instance().send_auction_puchase_success_info(packet->auction_info);
	}

	void cs2ws_auction_cancel_sell_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_cancel_sell* packet = static_cast<const cs2ws_auction_cancel_sell*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_cancel_sell))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_cancel_sell", 0, false, "%l%l",
				packet->role_guid.server_64, packet->item_guid.server_64);
			return;
		}

		auction_mgr_ws::get_instance().cancel_sell(packet->role_guid, packet->item_guid);
	}

	void cs2ws_trade_bid_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_bid_info* packet = static_cast<const cs2ws_auction_bid_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_bid_info))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_trade_bid", 0, false, "%l%l%d%d",
				packet->role_guid.server_64, packet->item_guid.server_64,
				packet->price_money_type, packet->price_money_value);
			return;
		}

		auction_mgr_ws::get_instance().trade_bid(packet->role_guid, packet->item_guid, packet->price_money_type, packet->price_money_value);
	}

	void cs2ws_clear_selling_lock_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_clear_selling_lock* packet = static_cast<const cs2ws_auction_clear_selling_lock*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_clear_selling_lock))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			bool tmp = false;
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_operate_selling_lock", 0, false, "%l%b", packet->item_guid.server_64, tmp);
			return;
		}

		auction_mgr_ws::get_instance().operate_selling_lock(packet->item_guid, false);
	}

	void cs2ws_auction_bid_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_bid_success* packet = static_cast<const cs2ws_auction_bid_success*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_bid_success))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			cs2ws_auction_bid_success* tmp = const_cast<cs2ws_auction_bid_success *>(packet);
			packet_s2s * p_s2s = auction_mgr_ws::get_instance().get_auction_buffer(tmp->auction_info);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_bid_success_proc", 0, true, "%s%l",
				p_s2s->google_data, p_s2s->google_data_len,
				tmp->old_buyer_guid.server_64);
			return;
		}
		auction_mgr_ws::get_instance().bid_success_proc(packet->auction_info, packet->old_buyer_guid);
	}

	void cs2ws_auction_share_sell_reward_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_share_sell_reward* packet = static_cast<const cs2ws_auction_share_sell_reward*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_share_sell_reward))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			cs2ws_auction_share_sell_reward* tmp = const_cast<cs2ws_auction_share_sell_reward *>(packet);
			packet_s2s * p_s2s = auction_mgr_ws::get_instance().get_auction_buffer(tmp->data_info);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_share_reward_to_seller", 0, true, "%s",
				p_s2s->google_data, p_s2s->google_data_len);
			return;
		}
		auction_mgr_ws::get_instance().share_reward_to_seller(packet->data_info);
	}

	void cs2ws_auction_add_trade_record_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_add_purchase_record* packet = static_cast<const cs2ws_auction_add_purchase_record*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_add_purchase_record))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			cs2ws_auction_add_purchase_record* tmp = const_cast<cs2ws_auction_add_purchase_record*>(packet);
			packet_s2s* p_s2s = auction_mgr_ws::get_instance().get_auction_buffer(tmp->auction_record_info.auction_info);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_add_record_to_db", 0, true, "%s%l%l%b",
				p_s2s->google_data, p_s2s->google_data_len,
				packet->auction_record_info.role_guid.server_64,
				packet->auction_record_info.record_guid.server_64,
				packet->auction_record_info.is_sell);

			return;

		}
		auction_mgr_ws::get_instance().add_record_to_db(packet->auction_record_info.auction_info, 
			packet->auction_record_info.role_guid, 
			packet->auction_record_info.record_guid, 
			packet->auction_record_info.is_sell);
	}

	void cs2ws_auction_del_bid_record_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_del_bid_record* packet = static_cast<const cs2ws_auction_del_bid_record*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_del_bid_record))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_del_bid_record", 0, false, "%l", packet->item_guid.server_64);
			return;
		}
		auction_mgr_ws::get_instance().del_bid_record(packet->item_guid);
	}

	void cs2ws_auction_request_person_refresh_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_auction_request_refresh_person* packet = static_cast<const cs2ws_auction_request_refresh_person*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_auction_request_refresh_person))
		{
			return;
		}
		auction_mgr_ws::get_instance().add_person_info(packet);

	}

	void cs2ws_recharge_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (data_len != sizeof(cs2ws_recharge_end))
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_cs2ws_recharge_end_proc_package_len_error, 0, invalid_payment_type, guid_64(), 0, nullptr);
			return;
		}
		const cs2ws_recharge_end* packet = static_cast<const cs2ws_recharge_end*>(data_ptr);
		if (nullptr == packet)
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_cs2ws_recharge_end_proc_package_is_null, 0, invalid_payment_type, guid_64(), 0, nullptr);
			return;
		}

		if (packet->result == e_recharge_error_none)
		{
			recharge_mgr_ws::get_instance().recharge_process_success(packet->order_id, packet->role_guid, packet->goods_id, packet->order_num, packet->payment_type);
		}
		recharge_mgr_ws::get_instance().send_recharge_end_result(packet->result, packet->order_num, packet->payment_type, packet->role_guid, packet->goods_id, packet->order_id, packet->is_first_recharge);
	}

	void cs2ws_recharge_end_send_mail_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_recharge_end_send_mail* packet = static_cast<const cs2ws_recharge_end_send_mail*>(data_ptr);
		if (nullptr == packet)
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_cs2ws_recharge_end_send_mail_proc_package_is_null, 0, invalid_payment_type, guid_64(), 0, nullptr);
			return;
		}
		if (data_len != sizeof(cs2ws_recharge_end_send_mail))
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_cs2ws_recharge_end_send_mail_proc_package_len_error, 0, invalid_payment_type, guid_64(), 0, nullptr);
			return;
		}
		recharge_mgr_ws::get_instance().recharge_send_mail(packet->order_id, packet->role_guid, packet->goods_id, packet->pay_price, packet->order_num, packet->payment_type, packet->direct_diamond);
	}

	void cs2ws_gm_order_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_gm_order* packet = static_cast<const cs2ws_gm_order*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_gm_order))
		{
			return;
		}
		gm_order_proc_ws::proc_gm_order(packet->gm_order_type, packet->order_param, packet->param_num);
	}

	void cs2ws_update_boss_damage_list_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_npc_damage_list* packet = static_cast<const cs2ws_npc_damage_list*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_npc_damage_list))
		{
			return;
		}
		world_boss_ws_mgr::get_instance().set_damage_list(packet->top_records, packet->record_num,packet->boss_type, packet->is_legion_score, packet->npc_id);
		
	}

	void cs2ws_npc_left_hp_per_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_npc_left_hp_per* packet = static_cast<const cs2ws_npc_left_hp_per*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_npc_left_hp_per))
		{
			return;
		}
		
		world_boss_ws* world_boss_ptr = world_boss_ws_mgr::get_instance().get_world_boss_info(packet->npc_spawn_point_template_id);
		if (nullptr != world_boss_ptr)
		{
			world_boss_ptr->set_hp_per(packet->left_hp_per);
		}

		character_proto_npc_left_hp_per msg;
		msg.set_npc_spawn_point_template_id(packet->npc_spawn_point_template_id);
		msg.set_left_hp_per(packet->left_hp_per);
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_npc_left_hp_per);
	}

	void cs2ws_update_gain_treasure_boss_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_update_gain_treasure_boss_info* packet = static_cast<const cs2ws_update_gain_treasure_boss_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_update_gain_treasure_boss_info))
		{
			return;
		}
		gain_treasure_ws_mgr::get_instance().update_gain_treasure_boss_info(packet->boss_info);
	}

	void cs2ws_update_boss_island_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_update_boss_island_info* packet = static_cast<const cs2ws_update_boss_island_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_update_boss_island_info))
		{
			return;
		}
		boss_island_ws_mgr::get_instance().update_boss_island_info(packet->boss_info);
	}

	void cs2ws_add_gain_treasure_record_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_add_gain_treasure_record_info* packet = static_cast<const cs2ws_add_gain_treasure_record_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len == 0 || packet->data_num < 0)
		{
			return;
		}
		int32 data_num = packet->data_num;
		hld::s_gain_treasure_record_info_one record_info;
		record_info.clear_data();
		record_info.boss_spawn_point_template_id = packet->boss_spawn_point_temp_id;
		record_info.killed_stamp = packet->killed_stamp;
		std::vector<s_gain_treasure_record_player_info> new_player_info;
		new_player_info.clear();
		for (int32 i = 0; i < data_num; i++)
		{
			if (i >= gain_treasure_gain_item_max_num)
			{
				break;
			}
			bool is_have_name = false;
			for (int32 j = 0; j < new_player_info.size(); j++)
			{
				std::string new_name = new_player_info[j].role_name;
				std::string old_name = packet->player_info[i].role_name;
				if (new_name == old_name)
				{
					is_have_name = true;
					for (int32 z = 0; z < gain_treasure_gain_item_max_num; z++)
					{
						if (packet->player_info[i].gain_item_template_id == 0)
						{
							break;
						}
						if (new_player_info[j].gain_item_template_id[z] == 0 && packet->player_info[i].gain_item_template_id != 0)
						{
							new_player_info[j].gain_item_template_id[z] = packet->player_info[i].gain_item_template_id;
							break;
						}
					}
				}
			}
			if (new_player_info.size() == 0 || false == is_have_name)
			{
				std::string player_name = packet->player_info[i].role_name;
				s_gain_treasure_record_player_info new_player_info_one;
				new_player_info_one.clear_data();
				memcpy(new_player_info_one.role_name, player_name.c_str(), player_name.size() >= max_name_size ? max_name_size : player_name.size());
				new_player_info_one.gain_item_template_id[0] = packet->player_info[i].gain_item_template_id;
				new_player_info_one.role_guid = packet->player_info[i].role_guid;
				new_player_info.push_back(new_player_info_one);
				continue;
			}
		}
		for (int32 i = 0; i < new_player_info.size(); i++)
		{
			if (i >= max_team_member_num)
			{
				break;
			}
			record_info.player_info_arr[i] = new_player_info[i];
		}
		server_log::boss_home_battle_from_boss_log(record_info);
		if (gain_treasure_ws_mgr::get_instance().is_use_lua())
		{
			hld::st_proto::st_gain_treasure_record_info_one pro_msg;
			record_info.to_proto(&pro_msg);
			packet_s2s* p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_add_gain_treasure_record_and_save", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}
		gain_treasure_ws_mgr::get_instance().add_gain_treasure_record_and_save(record_info);
	}

	//cloud shop
	void cs2ws_cloud_shop_buy_proc_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_cloud_shop_buy_resp* packet = static_cast<const cs2ws_cloud_shop_buy_resp*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		const cs2ws_cloud_shop_buy_resp req = *packet;
		
		if (world_server::getInstance().get_cross_id() > 0)
		{
			cross::send_msg_to_ws(guid_64(), world_server::getInstance().get_cross_id(), e_msgindex_ws2gws_cloud_shop_buy_end, &req, sizeof(req));
		}
		else
		{
			cloud_shop_mgr::get_instance().send_buy_result_to_player(packet->rold_guid, packet->error_id, packet->buy_type);
		}
	}

	void cs2ws_sub_rename_card_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_sub_player_rename_item_end* packet = static_cast<const cs2ws_sub_player_rename_item_end*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}

		client_session* session_player = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == session_player)
		{
			return;
		}

		xstring role_name;
		xstring original_name;

		rename_mgr::get_instance().sub_rename_item_end(packet->result, packet->role_guid, role_name, original_name);

		if (packet->result != e_change_role_success)
		{
			return;
		}

		client_session_mgr::getInstance().change_player_name_func(packet->role_guid, role_name);
		session_player->get_relation_list_mgr().send_change_name_mail_to_all_friend(original_name, role_name);
		ranking_mgr_ws::change_player_name_func(packet->role_guid, role_name);
		legion_ws_mgr::get_instance().change_player_name_func(packet->role_guid, role_name);
		role_pk_manager::getInstance().change_player_name_func(packet->role_guid, role_name);
		team_ws_mgr::get_instance().change_player_name_func(packet->role_guid, role_name);
		marry_mgr_ws::get_instance().change_player_name_func(packet->role_guid, role_name);
		red_package_ws_mgr::get_instance().change_player_name_func(packet->role_guid, role_name);
		lucky_draw_record_ws_mgr::get_instance().change_player_name_func(packet->role_guid, role_name);
		best_record_mgr::get_instance().change_player_name_func(packet->role_guid, role_name);
		arena_mgr_ws::get_instance().change_player_name_func(packet->role_guid, role_name);
		broken_sky_mgr::get_instance().change_player_name_func(packet->role_guid, role_name);
		cloud_shop_mgr::get_instance().change_player_name_func(packet->role_guid, role_name);
		gain_treasure_ws_mgr::get_instance().change_player_name_func(packet->role_guid, role_name, original_name);
		world_boss_ws_mgr::get_instance().change_player_name_func(packet->role_guid, role_name, original_name);
		element_war_ws_mgr::get_instance().send_req_element_war_role_info(session_player);
	}

	void cs2ws_update_cross_server_pk_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_update_one_island_msg* packet = static_cast<const cs2ws_update_one_island_msg*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_update_one_island_msg))
		{
			return;
		}
		cross_server_pk_ws_mgr::get_instance().save_cs2ws_island_msg(packet->island_msg, packet->war_num, packet->winner_guid, packet->is_end_war, packet->is_need_send_to_client, packet->pk_count);
	}

	void cs2ws_update_cross_server_pk_map_info_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_cross_server_pk_map_msg* packet = static_cast<const cs2ws_cross_server_pk_map_msg*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_cross_server_pk_map_msg))
		{
			return;
		}
		cross_server_pk_ws_mgr::get_instance().set_cross_server_pk_map_end(packet->class_type);
	}

	void cs2ws_update_server_change_money(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_send_change_server_money* packet = static_cast<const cs2ws_send_change_server_money*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_change_server_money))
		{
			return;
		}
		cross_server_harry_ws_mgr::get_instance().change_server_money(packet->is_special_harry, packet->change_value, packet->server_id);
	}

	void cs2ws_send_harry_notice_to_all_player_func(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_send_harry_notice_to_all_player* packet = static_cast<const cs2ws_send_harry_notice_to_all_player*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_harry_notice_to_all_player))
		{
			return;
		}
		int32 string_id = packet->string_id;
		xstring	 string_str = template_manager::get_instance().get_str_by_string_template_id(string_id);
		event_ws_mgr::get_instance().send_notice_to_all_only_string(string_str);
	}
	void cs2ws_create_harry_player(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_send_create_harry_player* packet = static_cast<const cs2ws_send_create_harry_player*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_create_harry_player))
		{
			return;
		}
		if (packet->map_type == e_map_type_cross_server_harry)
		{
			cross_server_harry_ws_mgr::get_instance().set_create_need_harry_player(true);
		}
		else if (packet->map_type == e_map_type_big_map)
		{
			if (!cross_server_pk_ws_mgr::get_instance().get_is_create_fake_player())
			{
				cross_server_pk_ws_mgr::get_instance().set_need_create_fake_player(true);
				cross_server_pk_ws_mgr::get_instance().set_is_create_fake_player(true);
			}
			else
			{
				cross_server_pk_ws_mgr::get_instance().set_is_create_fake_player(false);
			}

		}
	}

	void cs2ws_cross_ladder_req_buy_ticket_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_cross_ladder_req_buy_ticket* packet = static_cast<const cs2ws_cross_ladder_req_buy_ticket*>(data_ptr);
		if (nullptr == packet)
			return;
		if (data_len != sizeof(cs2ws_cross_ladder_req_buy_ticket))
			return;

		client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);

		if (false == gate_proxy::getInstance().is_gate_run() || false == cross_ladder_ws_mgr_new::get_instance().is_in_activity_time())
		{
			cross_ladder_ws_mgr_new::get_instance().refound_ticket_cost(packet->role_guid, 0);
			if (nullptr != session_ptr)
			{
				cross_ladder_respond_buy_join_ticket msg;
				msg.set_result(e_cross_ladder_buy_ticket_not_open);
				session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_ladder_respond_buy_join_ticket);
			}
		}
		else
			cross::send_msg_to_ws(packet->role_guid, cross_ladder_ws_mgr_new::get_instance().get_match_server_id(), e_msgindex_cs2ws_cross_ladder_req_buy_ticket, data_ptr, data_len);
	}

	void cs2ws_cross_ladder_send_game_result_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_cross_ladder_send_game_result* packet = static_cast<const cs2ws_cross_ladder_send_game_result*>(data_ptr);
		if (nullptr == packet)
			return;
		if (data_len != sizeof(cs2ws_cross_ladder_send_game_result))
			return;

		cross_ladder_ws_mgr_new::get_instance().deal_with_pk_result_from_cs(packet->map_guid, packet->winner_guid);
	}

	void cs2ws_cross_ladder_req_last_score_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_cross_ladder_req_last_score* packet = static_cast<const cs2ws_cross_ladder_req_last_score*>(data_ptr);
		if (nullptr == packet)
			return;
		if (data_len != sizeof(cs2ws_cross_ladder_req_last_score))
			return;

		cross_ladder_ws_mgr_new::get_instance().req_cross_ladder_last_score(packet->role_guid, packet->server_id);
	}

	void cs2ws_cross_server_kill_world_boss_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_cross_server_kill_world_boss* packet = static_cast<const cs2ws_cross_server_kill_world_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_cross_server_kill_world_boss))
		{
			return;
		}
		cross_server_world_boss_ws_mgr::get_instance().cross_boss_on_dead(packet->killer_guid, packet->be_kill_boss_guid);
	}
	void cs2ws_cross_server_world_boss_creat_boss_begin_func(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_cross_server_world_boss_creat_boss_begin* packek = static_cast<const cs2ws_cross_server_world_boss_creat_boss_begin*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_cross_server_world_boss_creat_boss_begin))
		{
			return;
		}
		cross_server_world_boss_ws_mgr::get_instance().sync_create_boos_to_cs(packek->from_map_guid);
	}
	void cs2ws_make_cross_server_world_legion_award_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_make_cross_server_world_legion_award* packek = static_cast<const cs2ws_make_cross_server_world_legion_award*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_make_cross_server_world_legion_award))
		{
			return;
		}
		cross_server_world_boss_ws_mgr::get_instance().send_legion_act_rank(*packek);
	}

	void cs2ws_cross_boss_map_game_over_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_cross_boss_map_game_over* packek = static_cast<const cs2ws_cross_boss_map_game_over*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_cross_boss_map_game_over))
		{
			return;
		}
		cross_server_world_boss_ws_mgr::get_instance().cross_boss_map_game_over(packek->map_template_id);
	}
	void cs2ws_time_limit_sync_common_data(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_time_limit_activity_common_data_sync* packet = static_cast<const cs2ws_time_limit_activity_common_data_sync*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		vector<int32> condition_list;
		for (int32 idx = 0 ; idx < packet->condition_num; idx++)
		{
			condition_list.push_back(packet->condition_list[idx]);
		}
		if (packet->activity_type == e_time_limit_activity_type_national_treasure_2)
		{
			time_limit_activity_ws_mgr::get_instance().update_activity_common_data(packet->activity_type, packet->data_num, condition_list);
		}
		else
		{
			time_limit_activity_ws_mgr::get_instance().update_activity_common_data_on_local(packet->activity_type, packet->data_num, condition_list);
		}
	}
	void cs2ws_check_time_limit_sync_common_data(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const cs2ws_check_time_limit_activity_common_data* packet = static_cast<const cs2ws_check_time_limit_activity_common_data*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		time_limit_activity_ws_mgr::get_instance().check_common_data(packet->activity_id, packet->user_guid, packet->activity_type, packet->check_data);
	}
	void cs2ws_send_kill_boss_notic_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_send_kill_boss_notic* packek = static_cast<const cs2ws_send_kill_boss_notic*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_kill_boss_notic))
		{
			return;
		}
		cs2ws_send_kill_boss_notic resp;
		memcpy(&resp, packek, sizeof(cs2ws_send_kill_boss_notic));
		cross_server_world_boss_ws_mgr::get_instance().send_boss_dead_notice(packek->item_info_ptr.boss_template, packek->item_info_ptr.notice_id, resp.item_info_ptr.item_num, resp.item_info_ptr.data_ary, packek->item_info_ptr.notice_string);
		gate2ws_send_kill_boss_notic msg;
		msg.item_info_ptr.clear_data();
		memcpy(&msg.item_info_ptr, &packek->item_info_ptr, sizeof(packek->item_info_ptr) > sizeof(msg.item_info_ptr) ? sizeof(msg.item_info_ptr) : sizeof(packek->item_info_ptr));
		cross::send_msg_to_ws(guid_64(), 0, e_msg_gate2ws_send_kill_boss_notice, &msg, sizeof(msg));
	}

	void cs2ws_sync_person_information_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_sync_person_information* packek = static_cast<const cs2ws_sync_person_information*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_sync_person_information))
		{
			return;
		}

		person_infor_ws_mgr::get_instance().sync_person_info(packek->role_guid, packek->person_info);
	}
	void cs2ws_delete_person_information_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_delete_person_information* packek = static_cast<const cs2ws_delete_person_information*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_delete_person_information))
		{
			return;
		}

		person_infor_ws_mgr::get_instance().remove_person_info(packek->role_guid);
	}
	void cs2ws_get_person_couple_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_get_person_couple* packek = static_cast<const cs2ws_get_person_couple*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_get_person_couple))
		{
			return;
		}
		client_session* tmp_session = client_session_mgr::getInstance().get_session(packek->role_guid);
		if (nullptr != tmp_session && tmp_session->is_self_server())
		{
			marry_couple_data couple_data = marry_mgr_ws::get_instance().get_couple_data(packek->target_guid);
			hld::character_proto_sync_other_person_couple_info msg;

			msg.set_role_guid(couple_data.role_guid);
			msg.set_role_name(couple_data.role_name);
			msg.set_marry_time(couple_data.marry_time);

			tmp_session->send_to_client(&msg, e_msgindex_s2c_sync_other_person_couple_data);
		}
	}

	void cs2ws_cross_player_save_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_player_save_end* packek = static_cast<const cs2ws_player_save_end*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_player_save_end))
		{
			return;
		}
		client_session* session = client_session_mgr::getInstance().get_session(packek->role_guid);
		if (nullptr != session)
		{
			session->set_is_cross_server_save_end(true);
		}
	}
	void cs2ws_create_bonfire_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_bonfire_create* packek = static_cast<const cs2ws_bonfire_create*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_bonfire_create))
		{
			return;
		}
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(packek->legion_guid);
		if (nullptr == legion_ptr)
		{
			return;
		}
		legion_ptr->on_create_bonfire_npc();
	}
	void cs2ws_bonfire_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_bonfire_end* packek = static_cast<const cs2ws_bonfire_end*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_bonfire_end))
		{
			return;
		}
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(packek->legion_guid);
		if (nullptr == legion_ptr)
		{
			return;
		}
		legion_ptr->legion_brofire_end();
	}
	void cs2ws_legion_boss_create_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_legion_boss_create* packek = static_cast<const cs2ws_legion_boss_create*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_legion_boss_create))
		{
			return;
		}
		legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(packek->legion_guid);
		if (nullptr == legion_ptr)
		{
			return;
		}
		int64 create_time = time_helper::get_cur_time_new().second;
		legion_ptr->set_legion_boss_create_time(create_time);
	}

	void cs2ws_damage_legion_boss_player_arr_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_legion_boss_damage_player* packek = static_cast<const cs2ws_legion_boss_damage_player*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_legion_boss_damage_player))
		{
			return;
		}
		guid_64 legion_guid = packek->legion_guid;
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->set_legion_boss_damage_player(packek->player_guid_arr, packek->member_num);
		int32 rank_idex = world_boss_ws_mgr::get_instance().get_legion_rank_with_legion_boss(legion_guid) + 1;
		legion_ws_ptr->send_legion_welfare_by_activity_and_rank(e_activity_type_legion_boss, rank_idex);
		//legion_ws_ptr->send_legion_welfare_by_npc(packek->npc_id, rank_idex);
		legion_ws_ptr->send_legion_reward_with_legion_boss_rank(rank_idex);
		legion_ws_ptr->send_legion_boss_notice();
		legion_ws_ptr->send_all_legion_boss_notice();
	}

	void cs2ws_check_can_assist_fight_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_check_can_assist_fight_end* packek = static_cast<const cs2ws_check_can_assist_fight_end*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_check_can_assist_fight_end))
		{
			return;
		}
		assist_fight_mgr::get_instance().req_assist_fight_end(packek->assist_fight_guid, packek->role_guid, packek->check_result);
	}

	void cs2ws_cancel_assist_fight_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_cancel_assist_fight* packek = static_cast<const cs2ws_cancel_assist_fight*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_cancel_assist_fight))
		{
			return;
		}
		assist_fight_mgr::get_instance().cancel_assist_fight(packek->cancel_type, packek->assist_fight_guid, packek->role_guid);
	}
	void cs2ws_start_assist_fight_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_start_assist_fight* packek = static_cast<const cs2ws_start_assist_fight*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_start_assist_fight))
		{
			return;
		}
		assist_fight_mgr::get_instance().start_assist_fight(packek->m_assist_fight_info);
	}

	void cs2ws_end_assist_fight_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_end_assist_fight* packek = static_cast<const cs2ws_end_assist_fight*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_end_assist_fight))
		{
			return;
		}
		std::vector<guid_64> help_list;
		for (int32 i = 0 ; i < assist_fight_max_num;i++)
		{
			if (packek->help_list[i].is_valid())
			{
				help_list.push_back(packek->help_list[i]);
			}
		}
	}

	void cs2ws_create_pk_king_combat_map_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_create_pk_king_combat_map* packek = static_cast<const cs2ws_create_pk_king_combat_map*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_create_pk_king_combat_map))
		{
			return;
		}
		pk_king_mgr& pk_king_mgr_ref = pk_king_mgr::get_instance();
		pk_king_mgr_ref.create_pk_king_combat_map(packek->map_guid, packek->map_num);
	}
	void cs2ws_send_is_pk_king_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_send_is_pk_king* packek = static_cast<const cs2ws_send_is_pk_king*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_is_pk_king))
		{
			return;
		}
		if (world_server::getInstance().get_server_id() == packek->server_id)
		{
			pk_king_mgr::get_instance().add_rank_list_data(packek->role_guid);
		}
		else
		{
			ws2ws_send_is_pk_king msg;
			msg.role_guid = packek->role_guid;
			msg.server_id = packek->server_id;
			cross::send_msg_to_ws(guid_64(), packek->server_id, e_msgindex_ws2ws_send_is_pk_king, &msg, sizeof(msg));
		}
		

	}
	void cs2ws_send_break_line_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_send_break_line* packek = static_cast<const cs2ws_send_break_line*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_break_line))
		{
			return;
		}
		switch (packek->break_info.active_type)
		{
		case e_activity_type_pk_king:
		{
			pk_king_mgr::get_instance().add_break_line_player(packek->break_info);
		}
		break;
		case e_activity_type_element_war:
		{
			element_war_ws_mgr::get_instance().add_break_line_player(packek->break_info);
		}
		break;
		default:
			break;
		}
	}
	void cs2ws_element_war_map_game_end_role_info_msg(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_element_war_map_game_end_role_info* packek = static_cast<const cs2ws_element_war_map_game_end_role_info*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_element_war_map_game_end_role_info))
		{
			return;
		}
		element_war_ws_mgr::get_instance().map_game_end(packek);
	}
	void cs2ws_element_war_element_war_buy_ticket_msg(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_element_war_buy_ticket* packek = static_cast<const cs2ws_element_war_buy_ticket*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_element_war_buy_ticket))
		{
			return;
		}
		element_war_ws_mgr::get_instance().buy_ticket_num(packek->role_guid);
	}
	void cs2ws_element_ladder_req_last_score_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_element_ladder_req_last_score* packet = static_cast<const cs2ws_element_ladder_req_last_score*>(data_ptr);
		if (nullptr == packet)
			return;
		if (data_len != sizeof(cs2ws_element_ladder_req_last_score))
			return;
		element_war_ws_mgr::get_instance().req_element_ladder_last_score(packet->role_guid, packet->server_id);
	}
	void cs2ws_element_war_gm_send_add_ticket_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_element_war_gm_send_add_ticket* packet = static_cast<const cs2ws_element_war_gm_send_add_ticket*>(data_ptr);
		if (nullptr == packet)
			return;
		if (data_len != sizeof(cs2ws_element_war_gm_send_add_ticket))
			return;
		element_war_ws_mgr::get_instance().gm_add_ticket(packet->role_guid);
	}
	void cs2ws_reload_csv_end(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		world_server::getInstance().set_cs_reload_csv_end();
	}
	void cs2ws_send_attack_city_legion_reward_info_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_send_attack_city_legion_reward_info* packet = static_cast<const cs2ws_send_attack_city_legion_reward_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}	
		if (data_len != sizeof(cs2ws_send_attack_city_legion_reward_info))
		{
			return;
		}
		legion_ws* rank_legion = legion_ws_mgr::get_instance().get_legion(packet->legion_guid);
		if (nullptr == rank_legion)
		{
			CONSOLE_INFO("attack_city legion_reward legion_guid = nil");
			return;
		}
		ActivityCommonConfigTemplate* config_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_attack_city);
		if (nullptr == config_ptr)
		{
			return;
		}
		for (int32 i = 0; i < 50; ++i)
		{
			auto& item_w_num = packet->item_list[i];
			if (item_w_num.m_item_id <= 0)
			{
				break;
			}
			rank_legion->give_activity_reward(config_ptr, packet->player_guid, packet->player_num, item_w_num.m_item_id, item_w_num.m_item_num);
		}
		rank_legion->mail_on_attack_city_single_reward(packet->rank_level, packet->player_guid, packet->player_num);
		
	}
	void send_attack_city_legion_end_info_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_send_attack_city_legion_end_info* packet = static_cast<const cs2ws_send_attack_city_legion_end_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_attack_city_legion_end_info))
		{
			return;
		}
		for (int32 i = 0; i < attack_city_group_max_num; ++i)
		{
			s_attack_city_legion_info & legion_info = attack_city_ws_mgr::get_instance().get_legion_info(packet->info_list[i].legion_guid);
			if (false == legion_info.is_valid())
			{
				continue;
			}
			// 设置军团已经参与活动 后续点击进入将无效
			legion_info.is_check = (int32)e_attack_check_type_join_end;
			// S组第一名连胜次数加一 名次不变动
			if (packet->group_level == e_attack_city_group_level_s && packet->info_list[i].group_level == 0)
			{
				legion_info.group_level = 0;
				legion_info.winning_streak_num++;
			}
			// C组最后一名名次也不动
			else if (packet->group_level == e_attack_city_group_level_c && packet->info_list[i].group_level == 3)
			{
				legion_info.group_level = packet->group_level * attack_city_group_max_num + packet->info_list[i].group_level;
				legion_info.winning_streak_num = 0;
			}
			// 每组第一名晋级
			else if (packet->info_list[i].group_level == 0)
			{
				legion_info.group_level = ((packet->group_level * attack_city_group_max_num) - 1);
				legion_info.winning_streak_num = 0;
			}
			// 每组最后一名降级
			else if (packet->info_list[i].group_level == 3)
			{
				legion_info.group_level = ((packet->group_level + 1) * attack_city_group_max_num);
				legion_info.winning_streak_num = 0;
			}
			// 其他人等级不变
			else
			{
				legion_info.group_level = packet->group_level * attack_city_group_max_num + packet->info_list[i].group_level;
				legion_info.winning_streak_num = 0;
			}
			CONSOLE_INFO("add_group_data legion_guid:{} rank:{} end_level:{}", legion_info.legion_guid.server_64, packet->info_list[i].group_level, legion_info.group_level);
			attack_city_ws_mgr::get_instance().save_group_data(legion_info);
		}
	}
	void send_attack_city_rank_end_info_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_send_attack_city_rank_end_info* packet = static_cast<const cs2ws_send_attack_city_rank_end_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_attack_city_rank_end_info))
		{
			return;
		}
		for (int32 i = 0; i < max_legion_member_num; ++i)
		{
			s_attack_city_rank_info rank_info = packet->info_list[i];
			if (false == rank_info.is_valid())
			{
				break;
			}
			attack_city_ws_mgr::get_instance().add_rank_data(rank_info, true);
		}
		attack_city_ws_mgr::get_instance().add_rank_data(packet->legion_info, true);
	}
	void cs2ws_send_attack_city_legion_call_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_send_attack_city_legion_call* packet = static_cast<const cs2ws_send_attack_city_legion_call*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(cs2ws_send_attack_city_legion_call))
		{
			return;
		}

		legion_ws* legion_info = legion_ws_mgr::get_instance().get_legion(packet->legion_guid);
		if (nullptr == legion_info)
		{
			return;
		}
		legion_info->send_legion_attack_city_call(packet->play_guid);
	}
}
