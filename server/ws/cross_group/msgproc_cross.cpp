/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-5-25 16:35
	
	File Name :	msgproc_gate.cpp
	
	Author : zhangming 
	
	Description : 
	
	Change List :


@@*/

//////////////////////////////////////////////////////////////////////////
//	File Include
//////////////////////////////////////////////////////////////////////////
#include "msgproc_cross.hpp"
#include <gate_msg.hpp>
#include <char_msg.hpp>
#include <login_msg.hpp>
#include <mail_msg.hpp>
#include <role_pk_msg.hpp>
#include <core.hpp>
#include <cross_server_harry_msg.hpp>
#include <cross_ladder_msg.hpp>
#include <cross_server_world_boss_msg.hpp>
#include <legion_msg.hpp>
#include <cloud_shop_msg.hpp>
#include <ranking_msg.hpp>
#include <world_server_msg.hpp>
#include <chat_msg.hpp>
#include "gate_proxy.hpp"
#include "server_log.hpp"
#include "net/net_server_mgr.hpp"
#include "connection/daemon_client.hpp"
#include "world_server.hpp"
#include "app/app_server.hpp"
#include "ws_client.hpp"
#include "../server/client_session_mgr.hpp"
#include "../server/role_pk/role_pk_manager.h"
#include "../server/queued_login_mgr.h"
#include "game_cfg/servers_config.h"
#include "../server/ranking/ranking_mgr_ws.h"
#include "../server/activity/time_limit_activity_ws_mgr.h"
#include "../server/activity/time_limit_activity_temp_ws_mgr.h"
#include "../server/activity/boss_island_ws_mgr.h"
#include "../server/activity/cross_server_pk_ws_mgr.h"
#include "../server/activity/cross_server_harry_ws_mgr.h"
#include "../server/legion/city_war_territory_mgr.h"
#include "../server/activity/cross_ladder_ws_mgr.h"
#include "../server/activity/cross_server_world_boss_ws_mgr.h"
#include "../server/activity/pk_king_mgr.h"
#include "../server/activity/crystal_fairyland_mgr.h"
#include "../server/activity/cross_server_city_war_ws_mgr.h"
#include "../server/activity/cloud_shop_mgr.h"
#include "../server/activity/pk_king_mgr.h"
#include "../server/activity/element_war_ws_mgr.h"
#include "../server/legion/legion_ws_mgr.h"
#include "../server/legion/legion_ws_city_war.h"
#include "cross_transfer_logic.hpp"
#include <Utility/parse_msg.h>
#include "utility/cs_date.hpp"
#include "utility/guid_gen.h"
#include "../server/big_player_ws_mgr.h"
#include "utility/init_unit.h"
#include "../server/activity/cross_server_city_war_ws_mgr.h"
#include "../server/mail/mail_event_ws.h"
#include "../server/mail/event_ws_mgr.h"
#include "../server/legion/attack_city_ws_mgr.h"
#include "cross_ladder.pb.h"
#include "ranking.pb.h"
#include "cross.pb.h"
#include "net.pb.h"

namespace faith
{
	void gate2ws_rep_register(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		CONSOLE_INFO("data_len : {}", data_len);
		if (data_len != sizeof(server2gate_req_register))
		{
			return;
		}
		const server2gate_req_register* packet = (const server2gate_req_register*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}

		world_server::getInstance().set_gate_server_config(packet->game_info);
		if (packet->recv_server_id <= 0)
		{
			ws_client::getInstance().send_cross_msg(packet->game_info.server_id);
		}

		if (packet->game_info.cross_id <= 0 || world_server::getInstance().get_cross_id() <= 0)
		{
			world_server::getInstance().remove_server_config(packet->game_info.server_id);
			world_server::getInstance().send_server_config_to_all_cs(packet->game_info, false);
			return;
		}
		if (packet->game_info.cross_id != world_server::getInstance().get_cross_id())
		{
			world_server::getInstance().remove_server_config(packet->game_info.server_id);
			world_server::getInstance().send_server_config_to_all_cs(packet->game_info, false);
			return;
		}
		world_server::getInstance().set_server_config(packet->game_info);
		world_server::getInstance().send_server_config_to_all_cs(packet->game_info, true);
		world_server::getInstance().func_when_add_other_server(packet->game_info.server_id);
		world_server::getInstance().send_server_config_all_to_client(nullptr);

		if (packet->game_info.server_id == world_server::getInstance().get_cross_id())
		{
			world_server::getInstance().refresh_server_cross_time();
			world_server::getInstance().send_msg_when_gate_register(e_ws_flag_server_cross_time);
		}
	}
	void gate2ws_rep_off_line(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		CONSOLE_INFO("data_len : {}", data_len);
		if (data_len != sizeof(gate2server_req_off_line))
		{
			return;
		}
		const gate2server_req_off_line* packet = (const gate2server_req_off_line*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		if (daemon_client::getInstance().get_server_close())
		{
			return;
		}
		s_game_info*  game_info_ptr = world_server::getInstance().get_server_config_by_server_id(packet->server_id);
		if (game_info_ptr)
		{
			world_server::getInstance().send_server_config_to_all_cs(*game_info_ptr, false);
			world_server::getInstance().func_when_remove_other_server(packet->server_id);
			world_server::getInstance().remove_server_config(packet->server_id);
			world_server::getInstance().send_server_config_all_to_client(nullptr);
		}
	}
	void gate2ws_rep_gate_new(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		CONSOLE_INFO("data_len : {}", data_len);
		if (data_len != sizeof(gate2gate_req_new))
		{
			return;
		}
		const gate2gate_req_new* packet = (const gate2gate_req_new*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		ws_client::getInstance().start_gate_new(packet->server_info);
	}

	void gate2ws_reload_csv(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2server_reload_csv* packet = static_cast<const gate2server_reload_csv*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		world_server::getInstance().reload_csv_func();
	}

	void gate2ws_set_server_act_hide_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_server_act_hide* packet = static_cast<const gate2ws_server_act_hide*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		world_server::getInstance().set_server_act_hidden(packet->is_need_hide, true);
	}

	void gate2ws_end_refresh_server_list_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_end_refresh_list* packet = static_cast<const gate2ws_end_refresh_list*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		world_server::getInstance().end_refresh_server_list();
	}

	void gate2ws_change_server_id_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_change_server_id* packet = static_cast<const gate2ws_change_server_id*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		bool is_add = packet->is_add;
		if (is_add)
		{
			world_server::getInstance().set_server_config(packet->server_info);
			world_server::getInstance().send_server_config_to_all_cs(packet->server_info, true);
			world_server::getInstance().func_when_add_other_server(packet->server_info.server_id);
			world_server::getInstance().send_server_config_all_to_client(nullptr);
		}
		else
		{
			world_server::getInstance().remove_server_config(packet->server_info.server_id);
			world_server::getInstance().send_server_config_to_all_cs(packet->server_info, false);
			world_server::getInstance().func_when_remove_other_server(packet->server_info.server_id);
			world_server::getInstance().send_server_config_all_to_client(nullptr);
		}
	}

	void gate2ws_this_cross_all_server_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2ws_all_server_arr* packet = static_cast<const gate2ws_all_server_arr*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 server_num = packet->server_num;
		for (int32 i = 0; i < server_num; i++)
		{
			world_server::getInstance().set_server_config(packet->server_info[i]);
			world_server::getInstance().send_server_config_to_all_cs(packet->server_info[i], true);
			world_server::getInstance().func_when_add_other_server(packet->server_info[i].server_id);
		}
		world_server::getInstance().send_msg_when_gate_register(e_ws_flag_server_cross_time);
		world_server::getInstance().send_to_gate_get_all_msg_with_change_gate_server();
		world_server::getInstance().send_server_config_all_to_client(nullptr);
	}

	void cs2ws_req_role_pk_result(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const cs2ws_role_pk_result* packet = static_cast<const cs2ws_role_pk_result*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		role_pk_manager::getInstance().role_pk_result(packet->finish_war_map, packet->win_team_num);
	}

	void gate2ws_rep_from_ws(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const gate2server_to_server* packet = static_cast<const gate2server_to_server*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		switch (packet->dataType)
		{
			case e_msgindex_c2s_cross_cross_info:
			{
				//ws2gate_cross_info* cross_info_ptr = (ws2gate_cross_info*)(packet->data);
				//role_pk_manager::getInstance().add_role_pk_info(cross_info_ptr->role_guid, cross_info_ptr->role_name, cross_info_ptr->template_id, cross_info_ptr->role_gs);
			}
			break;
			case e_msgindex_s2c_cross_cross_info:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_cross_cross_info);
			}
			break;
			case e_msgindex_c2s_cross_sign_up:
			{
				//ws2gate_sign_up* sign_up_ptr = (ws2gate_sign_up*)(packet->data);
				//if (nullptr == sign_up_ptr)
				//{
				//	return;
				//}
				//e_cross_sign_up_result res = e_cross_sign_up_state_error;
				//if (sign_up_ptr->is_join)
				//{
				//	res = role_pk_manager::getInstance().sign_up(sign_up_ptr->role_guid, guid_64());
				//}
				//else
				//{
				//	res = role_pk_manager::getInstance().sign_up_leave(sign_up_ptr->role_guid, guid_64());
				//}
				//cross_proto_sign_up_end msg;
				//msg.set_sign_type(sign_up_ptr->sign_up_type);
				//msg.set_sign_end(res);
				//cross::send_msg_to_ws(packet->role_guid, packet->server_id, e_msgindex_s2c_cross_sign_up, &msg);
			}
			break;
			case e_msgindex_s2c_cross_sign_up:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_cross_sign_up);
			}
			break;
			case e_msgindex_s2c_cross_begin_war:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_cross_begin_war);
			}
			break;
			case e_msg_index_ws2dp_add_mail_to_sql:
			{
				ws_client::getInstance().send_to_dp(packet->data, packet->dataLen);
			}
			break;
			case e_msgindex_ws2ws_send_other_server_mail:
			{
				ws2ws_send_other_server_mail* cross_info_ptr = (ws2ws_send_other_server_mail*)(packet->data);

				mail_ws_mgr::get_instance().recv_other_server_mail(cross_info_ptr->mail_record, cross_info_ptr->mail_info);
			}
			break;
			case e_msgindex_ws2ws_send_other_server_mail_end:
			{
				ws2ws_send_other_server_mail_end* cross_info_ptr = (ws2ws_send_other_server_mail_end*)(packet->data);

				mail_ws_mgr::get_instance().recv_other_server_mail_end(cross_info_ptr->mail_record);
			}
			break;
			case e_msgindex_c2s_cross_war_log:
			{
				server2dp_proto_ws2dp_load_role_pk_log msg;
				msg.set_role_guid(packet->role_guid.server_64);
				ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_role_pk_log, packet->server_id);
				//ws2dp_load_role_pk_log msg;
				//msg.role_guid = packet->role_guid;
				//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), packet->server_id);
			}
			break;
			case e_msgindex_s2c_cross_war_log:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_cross_war_log);
			}
			break;
			case e_msgindex_s2c_pk_state_end:
			{
				ws2ws_transfer_cross_pk_map* cross_info_ptr = (ws2ws_transfer_cross_pk_map*)(packet->data);

				switch (cross_info_ptr->activity_type)
				{
				case e_activity_type_cross_server_pk:
					cross_server_pk_ws_mgr::get_instance().recv_other_ws_transger_gate_map(cross_info_ptr->class_type, cross_info_ptr->role_guid, cross_info_ptr->server_id);
					break;
				case e_activity_type_pk_king:
					pk_king_mgr::get_instance().recv_other_ws_transfer_gate_map(cross_info_ptr->role_guid, cross_info_ptr->server_id);
					break;
				case e_activity_type_crystal_fairyland:
					crystal_fairyland_mgr::get_instance().recv_other_ws_transfer_gate_map(cross_info_ptr->role_guid, cross_info_ptr->server_id);
					break;
				case e_activity_type_cross_server_harry:
					cross_server_harry_ws_mgr::get_instance().recv_other_ws_transfer_gate_map(cross_info_ptr->role_guid, cross_info_ptr->server_id);
					break;
				case e_activity_type_overlord_city_war:
					cross_server_city_war_ws_mgr::get_instance().recv_other_ws_transfer_gate_map(cross_info_ptr->role_guid, cross_info_ptr->server_id, cross_info_ptr->legion_guid);
					break;
				case e_activity_type_city_war_cross_server:
					city_war_territory_mgr::get_instance().recv_other_ws_transfer_gate_map(cross_info_ptr->role_guid, cross_info_ptr->terr_id, cross_info_ptr->legion_guid, cross_info_ptr->server_id);
					break;
				case e_activity_type_cross_server_world_boss:
					cross_server_world_boss_ws_mgr::get_instance().recv_other_ws_transger_gate_map(cross_info_ptr->role_guid, cross_info_ptr->server_id, cross_info_ptr->class_type);
					break;
				case e_activity_type_attack_city:
					attack_city_ws_mgr::get_instance().recv_other_ws_transger_gate_map(cross_info_ptr->role_guid, cross_info_ptr->legion_guid, cross_info_ptr->server_id);
					break;
				default:
					break;
				}
			}
			break;
			case e_msgindex_s2c_pk_map_island_state_msg:
			{
				ws2ws_transfer_cross_pk_map_result* cross_info_ptr = (ws2ws_transfer_cross_pk_map_result*)(packet->data);

				cross_server_pk_ws_mgr::get_instance().transfer_to_map_with_gate_msg(cross_info_ptr->role_guid, cross_info_ptr->result, cross_info_ptr->map_temp_id, cross_info_ptr->map_guid, cross_info_ptr->war_idex, cross_info_ptr->server_id);
			}
			break;
			case e_msgindex_ws2ws_send_cross_server_harry_info:
			{
				ws2ws_send_cross_server_harry_msg_all* cross_info_ptr = (ws2ws_send_cross_server_harry_msg_all*)(packet->data);

				cross_server_harry_ws_mgr::get_instance().recv_gate_harry_msg(cross_info_ptr->harry_msg, cross_info_ptr->data_num, cross_info_ptr->is_need_set_name);

				world_server::getInstance().set_ws_loading_flag(e_ws_flag_harry);
				
			}
			break;
			case e_msgindex_ws2ws_cross_world_boss_map_result:
			{
				ws2ws_transfer_cross_world_boss_map_result* cross_info_ptr = (ws2ws_transfer_cross_world_boss_map_result*)(packet->data);

				cross_server_world_boss_ws_mgr::get_instance().transfer_to_map_with_gate_msg(cross_info_ptr->role_guid, cross_info_ptr->result, cross_info_ptr->map_temp_id, cross_info_ptr->map_guid, cross_info_ptr->war_idex, cross_info_ptr->server_id);
			}
			break;
			case e_msgindex_ws2ws_pk_fake_player_msg:
			{
				ws2ws_pk_harry_player* cross_info_ptr = (ws2ws_pk_harry_player*)(packet->data);
				
				cross_server_pk_ws_mgr::get_instance().set_winner_fake_player_with_gate_msg(cross_info_ptr->data_num, cross_info_ptr->pk_player_msg);

				world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_pk);
				
			}
			break;
			case e_msgindex_ws2ws_send_cross_server_harry_one:
			{
				ws2ws_send_cross_server_harry_msg_one* cross_info_ptr = (ws2ws_send_cross_server_harry_msg_one*)(packet->data);

				cross_server_harry_ws_mgr::get_instance().recv_server_harry_msg(cross_info_ptr->harry_msg, cross_info_ptr->data_num);
			}
			break;
			case e_msgindex_ws2ws_send_cross_server_harry_player_info:
			{
				ws2ws_send_server_harry_player_msg* cross_info_ptr = (ws2ws_send_server_harry_player_msg*)(packet->data);

				cross_server_harry_ws_mgr::get_instance().set_server_harry_best_player(cross_info_ptr->server_id, cross_info_ptr->player_guid_arr[0], cross_info_ptr->player_guid_arr[1], cross_info_ptr->player_guid_arr[2], cross_info_ptr->player_name);
			}
			break;
			case e_msgindex_ws2ws_send_server_harry_rank:
			{
				ws2ws_send_server_harry_award* cross_info_ptr = (ws2ws_send_server_harry_award*)(packet->data);

				cross_server_harry_ws_mgr::get_instance().recv_gate_with_server_money_rank(cross_info_ptr->server_rank);
			}
			break;
			case e_msgindex_ws2ws_server_info_msg:
			{
				ws2ws_send_server_info* cross_info_ptr = (ws2ws_send_server_info*)(packet->data);

				for (int32 i = 0; i < cross_info_ptr->data_num; i++)
				{
					if (cross_info_ptr->data_num > e_need_server_cross_max)
					{
						break;
					}
					world_server::getInstance().set_need_begin_cross_gm_common(cross_info_ptr->gm_commond_arr[i], (e_need_server_cross)i);
					world_server::getInstance().send_begin_cross_gm_to_client((e_need_server_cross)i);
					world_server::getInstance().send_begin_cross_gm_to_cs((e_need_server_cross)i);
				}
				world_server::getInstance().save_gm_common_to_db();
				if (!world_server::getInstance().is_loading_flag_finish(e_ws_flag_gm_common))//防止二次加载导致的问题
				{
					world_server::getInstance().set_ws_loading_flag(e_ws_flag_gm_common);
					legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
					city_war_mgr.set_cross_server_territory_war_state(world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war));
					world_server::getInstance().send_server_config_to_db();
					world_server::getInstance().load_server_attr_val();
				}
			}
			break;
			case e_msgindex_ws2ws_get_gate_msg:
			{
				ws2ws_get_big_player_from_gate* cross_info_ptr = (ws2ws_get_big_player_from_gate*)(packet->data);
				int64 flag = cross_info_ptr->data_flag;
				int32 server_id = cross_info_ptr->server_id;
				if (!world_server::getInstance().is_loading_flag_finish(flag))
				{
					return;
				}
				switch (flag)
				{
				case e_ws_flag_gm_common:
					world_server::getInstance().send_need_server_cross_to_other_server(server_id);
					break;
				case e_ws_flag_big_player:
					big_player_ws_mgr::get_instance().send_big_player_msg_to_other_server(true, server_id);
					break;
				case e_ws_flag_harry:
					cross_server_harry_ws_mgr::get_instance().send_all_harry_msg_to_other_server(server_id, true);
					break;
				case e_ws_flag_cross_pk:
					cross_server_pk_ws_mgr::get_instance().send_winner_fake_player_to_other_server(server_id);
					break;
				case e_ws_flag_overload_war:
					cross_server_city_war_ws_mgr::get_instance().send_city_info_all_to_other_ws(server_id);
					break;
				case e_ws_flag_city_war:
					city_war_territory_mgr::get_instance().send_city_info_all_to_other_ws(server_id);
					break;
				case e_ws_flag_cross_boss:
					cross_server_world_boss_ws_mgr::get_instance().send_cross_boss_info_all_to_other_ws(server_id);
					break;
				case e_ws_flag_cross_server_state:
					world_server::getInstance().send_server_info_by_type_to_other_server(e_server_info_type_server_cross_state, server_id);
					break;
				case e_ws_flag_attack_city:
					attack_city_ws_mgr::get_instance().send_info_all_to_other_ws(server_id);
				default:
					break;
				}
			}
			break;
			case e_msgindex_ws2ws_big_player_info:
			{
				ws2ws_send_big_player_msg* cross_info_ptr = (ws2ws_send_big_player_msg*)(packet->data);
				bool is_load_flag = cross_info_ptr->is_load_flag;
				for (int32 i = 0; i < e_big_player_type_max; i++)
				{
					if (false == init_unit::is_cross_server_big_player(i, world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_activity), world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war)))
					{
						continue;
					}
					guid_64 old_guid = big_player_ws_mgr::get_instance().get_big_player_guid((e_big_player_type)i);
					big_player_ws_mgr::get_instance().set_big_player_guid((e_big_player_type)i, cross_info_ptr->data_info[i].role_guid);
					if (false == cross_info_ptr->data_info[i].role_guid.is_valid())
					{
						big_player_ws_mgr::get_instance().clear_title_with_big_player_clear(i);
						if (i == e_big_player_type_lord_of_city)
						{
							legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
							city_war_mgr.send_del_city_master_stuff(old_guid);
						}
						if (i == e_big_player_type_overload_legion)
						{
							legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
							city_war_mgr.del_overlord_legion(false, true);
						}
					}
					if (true == cross_info_ptr->data_info[i].role_guid.is_valid() && i == e_big_player_type_overload_legion)
					{
						legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
						city_war_mgr.set_overlord_legion(cross_info_ptr->data_info[i].role_guid, false, true);
					}
				}
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_big_player);			
			}
			break;
			case e_msgindex_ws2ws_chat_server_legion:
			{
				client_session_mgr::getInstance().send_message_to_legion_data(packet->role_guid, packet->data, packet->dataLen, e_msgindex_s2c_receive_chat_new);
			}
			break;
			case e_msgindex_ws2ws_chat_server_all_member:
			{
				client_session_mgr::getInstance().send_message_to_all_client_data( packet->data, packet->dataLen, e_msgindex_s2c_receive_chat_new);
			}
			break;
			case e_msgindex_ws2ws_clear_all_pk_msg:
			{
				client_session_mgr::getInstance().send_message_to_all_client_data(packet->data, packet->dataLen, e_msgindex_s2c_clear_pk_msg_all);
			}
			break;
			case e_msgindex_s2c_cross_server_pk_msg_info_all:
			{
				gate2ws_server_id* cross_info_ptr = (gate2ws_server_id*)(packet->data);

				int32 server_id = cross_info_ptr->server_id;
				cross_server_pk_ws_mgr::get_instance().send_ws2ws_all_pk_msg(server_id, packet->role_guid);
			}
			break;
			case e_msgindex_s2c_cross_server_pk_msg_info_one:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				cross_server_pk_ws_mgr::get_instance().send_all_msg_to_client(packet->data, packet->dataLen, session_ptr);
			}
			break;
			case e_msgindex_s2c_clear_pk_msg_all:
			{
				cross_server_pk_ws_mgr::get_instance().send_pk_to_all_player_ws(packet->data, packet->dataLen);
			}
			break;
			case e_msgindex_s2c_gain_treasure_boss_info_all:
			{
				gate2ws_server_id* cross_info_ptr = (gate2ws_server_id*)(packet->data);

				int32 server_id = cross_info_ptr->server_id;
				boss_island_ws_mgr::get_instance().send_ws2ws_all_boss_island_msg(server_id, packet->role_guid);
				//game_proto_gain_treasure_boss_info_all boss_info_all_msg;
				//boss_island_ws_mgr::get_instance().fill_boss_info_all_msg(boss_info_all_msg);
				//cross::send_msg_to_ws(packet->role_guid, server_id, e_msgindex_s2c_gain_treasure_boss_info_one, &boss_info_all_msg);
			}
			break;
			case e_msgindex_s2c_gain_treasure_boss_info_one:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				boss_island_ws_mgr::get_instance().send_all_msg_to_client(packet->data, packet->dataLen, session_ptr);
			}
			break;
			case e_msgindex_ws2ws_send_cross_proto_msg:
			{
				if (packet->role_guid.is_valid())
				{
					client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
					if (nullptr == session_ptr)
					{
						return;
					}
					session_ptr->send_to_client(packet->data, packet->dataLen, packet->header);
				}
				else
				{
					client_session_mgr::getInstance().send_message_to_all_client_data(packet->data, packet->dataLen, packet->header);
				}
			}
			break;
			case e_msgindex_s2c_boss_island_info_one:
			{
				boss_island_ws_mgr::get_instance().send_boss_island_info_to_all_player_ws(packet->data, packet->dataLen);
			}
			break;
			case e_msgindex_cs2ws_sync_ranking_info:
			{
				cs2ws_sync_ranking_info* req_ptr = (cs2ws_sync_ranking_info*)(packet->data);

				ranking_mgr_ws::sync_player_info(req_ptr->role_info);
			}
			break;
			//case e_msgindex_ws2ws_req_gs_ranking:
			//{
			//	gate2ws_gs_ranking * req_ptr = (gate2ws_gs_ranking*)(packet->data);

			//	ranking_mgr_ws::send_gs_ranking(req_ptr->server_index);
			//}
			case e_msgindex_ws2ws_res_gs_ranking:
			{
				ranking_mgr_ws::get_gs_ranking(packet->data,packet->dataLen);
			}
			break;
			case e_msgindex_ws2ws_get_oracle_trial_info:
			{
				ranking_mgr_ws::send_gs_ranking(e_RankingIndex_Oracle_Trial, e_RankingIndex_Oracle_Trial);
			}	
			break;
			case e_msgindex_ws2ws_send_oracle_ranking_notice:
			{
				ws2ws_send_oracle_ranking_notice* cross_info_ptr = (ws2ws_send_oracle_ranking_notice*)(packet->data);

				if (cross_info_ptr->player_guid.is_valid() == false && strlen(cross_info_ptr->play_name) <= 0)
				{
					return;
				}
				if (cross_info_ptr->legion_guid.is_valid() && strlen(cross_info_ptr->legion_name) > 0)
				{
					ranking_mgr_ws::send_oracle_trial_notice(cross_info_ptr->play_name, cross_info_ptr->legion_name, cross_info_ptr->server_id);
				}
				else
				{
					ranking_mgr_ws::send_oracle_trial_notice(cross_info_ptr->play_name, cross_info_ptr->server_id);		
				}
				
			}
			break;
			case e_msgindex_ws2ws_req_all_gs_ranking:
			{
				ranking_mgr_ws::ws2gws_get_ranking_list_func(packet->data, packet->dataLen);
			}
			break;
			case e_msgindex_ws2ws_get_ranking_list_error:
			{
				gate2ws_get_ranking_list_error* req = (gate2ws_get_ranking_list_error*)(packet->data);
				ranking_mgr_ws::send_operate_result_to_client(req->role_guid, req->result);
			}
			break;
			case e_msgindex_ws2ws_res_all_gs_ranking:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_ranking_get_top_end);
			}
			break;
			case e_msgindex_ws2ws_req_get_my_rank_func:
			{				
				ws2gate_my_rank* cross_info_ptr = (ws2gate_my_rank*)(packet->data);

				ranking_mgr_ws::ws2gate_get_my_rank_func(cross_info_ptr->player_guid, cross_info_ptr->server_id, cross_info_ptr->sender_guid, cross_info_ptr->rank_type);
			}
			break;
			case e_msgindex_ws2ws_res_get_my_rank_func:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_ranking_get_my_rank_end);
			}
			break;
			case e_msgindex_ws2ws_req_time_limit_rank_info:
			{
				ws2gate_time_limit_get_rank_info* cross_info_ptr = (ws2gate_time_limit_get_rank_info*)(packet->data);

				time_limit_activity_ws_mgr::get_instance().get_rank_info((e_time_limit_activity_type)cross_info_ptr->activity_type, cross_info_ptr->role_guid, cross_info_ptr->server_id);
			}
			break;
			case e_msgindex_ws2ws_res_time_limit_rank_info:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_time_limit_update_sub_rank);
			}
			break;
			case e_msgindex_gws2ws_legion_rank_with_city_war:
			{
				gws2ws_legion_rank_with_cross_city_war* cross_info_ptr = (gws2ws_legion_rank_with_cross_city_war*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}
				cross_server_city_war_ws_mgr::get_instance().recv_gate_msg_send_legion_reward(cross_info_ptr->legion_guid, cross_info_ptr->rank_idex, cross_info_ptr->member_guid_all, cross_info_ptr->member_num);
			}
			break;
			case e_msgindex_gws2ws_overload_begin_notice:
			{
				gws2ws_overload_city_begin_notice* cross_info_ptr = (gws2ws_overload_city_begin_notice*)(packet->data);
				if (cross_info_ptr->notice_type == e_overload_notice_type_legion_begin)
				{
					guid_64 legion_guid = cross_info_ptr->legion_guid;
					int32 group_num = cross_info_ptr->group_num;
					legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
					if (nullptr != legion_ws_ptr)
					{
						legion_ws_ptr->notice_on_begin_overload_city_war(group_num);
						legion_ws_ptr->mail_on_begin_overload_city_war(group_num);
					}
				}
				else if (cross_info_ptr->notice_type == e_overload_notice_type_world_begin)
				{
					int32 notice_id = 93000408;//跨服世界霸主分组完毕公告
					std::vector<std::string> vec_str_up_item;
					vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
					std::string notice_str = init_unit::implode(vec_str_up_item);
					int32 create_time = time_helper::get_cur_time_new().second;
					event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
				}
				else if (cross_info_ptr->notice_type == e_overload_notice_type_cross_city_war_begin_world)
				{
					int32 notice_id = 93000413;//跨服世界霸主开启公告
					std::vector<std::string> vec_str_up_item;
					vec_str_up_item.push_back(template_manager::get_instance().get_str_id_by_notice_id(notice_id));
					std::string notice_str = init_unit::implode(vec_str_up_item);
					int32 create_time = time_helper::get_cur_time_new().second;
					event_ws_mgr::get_instance().send_notice_to_all(notice_id, create_time, guid_64(), notice_str);
				}
				else if (cross_info_ptr->notice_type == e_overload_notice_type_cross_city_war_begin_legion)
				{
					guid_64 legion_guid = cross_info_ptr->legion_guid;
					int32 group_num = cross_info_ptr->group_num;
					legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
					if (nullptr != legion_ws_ptr)
					{
						legion_ws_ptr->notice_on_join_overload_city_war();
					}
				}
				else
				{
					return;
				}
			}
			break;
			case e_msgindex_gws2ws_cross_city_info_all:
			{
				gws2ws_cross_city_info_all* cross_info_ptr = (gws2ws_cross_city_info_all*)(packet->data);
				cross_server_city_war_ws_mgr::get_instance().recv_gate_msg_city_info_all(cross_info_ptr->city_info_all, cross_info_ptr->data_num);

				world_server::getInstance().set_ws_loading_flag(e_ws_flag_overload_war);
				
			}
			break;
			case e_msgindex_ws2gws_change_cross_city:
			{
				ws2gws_change_cross_city_chief* cross_info_ptr = (ws2gws_change_cross_city_chief*)(packet->data);

				int32 change_type = cross_info_ptr->change_type;
				switch (change_type)
				{
				case e_change_cross_city_type_change_chief:
					cross_server_city_war_ws_mgr::get_instance().refresh_chief_with_legion(cross_info_ptr->leigon_guid, cross_info_ptr->role_guid, cross_info_ptr->role_name);
					break;
				case e_change_cross_city_type_change_legion_name:
					cross_server_city_war_ws_mgr::get_instance().refresh_legion_name(cross_info_ptr->leigon_guid, cross_info_ptr->role_name);
					break;
				case e_change_cross_city_type_del_legion:
					cross_server_city_war_ws_mgr::get_instance().refresh_rank_with_del_legion(cross_info_ptr->leigon_guid);
					break;
				default:
					break;
				}
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_req_role_info:
			{
				ws2ws_cross_ladder_req_role_info* msg_ptr = (ws2ws_cross_ladder_req_role_info*)(packet->data);

				cross_ladder_ws_mgr_new::get_instance().req_cross_ladder_info(packet->role_guid, msg_ptr);
			}
			break;
			case e_msgindex_s2c_cross_ladder_send_role_info:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
					return;
				faith::cross_ladder_send_role_info msg;
				bool success = msg.ParseFromArray(packet->data, packet->dataLen);
				if (success)
				{
					ws2cs_cross_ladder_refresh_ticket_info send;
					send.role_guid = packet->role_guid;
					send.can_buy_ticket_num = msg.data_array(e_cross_ladder_send_role_info_can_buy_num);
					session_ptr->send_to_cs(&send, sizeof(ws2cs_cross_ladder_refresh_ticket_info));
					if (msg.is_online())
					{
						ws2cs_cross_ladder_send_last_score send_last_score;
						send_last_score.role_guid = packet->role_guid;
						send_last_score.last_score = msg.data_array(e_cross_ladder_send_role_info_last_score);
						send_last_score.last_ranking = msg.last_season_ranking();
						session_ptr->send_to_cs(&send_last_score, sizeof(ws2cs_cross_ladder_send_last_score));
					}
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_cross_ladder_send_role_info);
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_send_sign_up:
			{
				ws2ws_cross_ladder_sign_up* msg_ptr = (ws2ws_cross_ladder_sign_up*)(packet->data);

				e_cross_ladder_sign_up_ret ret = e_cross_ladder_sign_up_failed;
				if (msg_ptr->is_join)
					ret = cross_ladder_ws_mgr_new::get_instance().sign_up(packet->role_guid);
				else
					ret = cross_ladder_ws_mgr_new::get_instance().sign_up_leave(packet->role_guid);

				cross_ladder_ws_mgr_new::get_instance().update_server_online_num(msg_ptr->server_id, msg_ptr->online_num);

				if (ret == e_cross_ladder_sign_up_success)
				{
					cross_ladder_ws_mgr_new::get_instance().set_role_server_info(packet->role_guid, msg_ptr->server_ip, msg_ptr->server_port);
				}
				cross_ladder_respond_sign_up msg;
				msg.set_sign_up_result(ret);
				cross::send_msg_to_ws(packet->role_guid, msg_ptr->server_id, e_msgindex_s2c_cross_ladder_respond_sign_up, &msg);
			}
			break;
			case e_msgindex_s2c_cross_ladder_respond_sign_up:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
					return;

				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_cross_ladder_respond_sign_up);
			}
			break;
			case e_msgindex_cs2ws_cross_ladder_req_buy_ticket:
			{
				cs2ws_cross_ladder_req_buy_ticket* msg_ptr = (cs2ws_cross_ladder_req_buy_ticket*)packet->data;

				cross_ladder_ws_mgr_new::get_instance().req_buy_ticket(packet->role_guid);
			}
			break;
			case e_msgindex_s2c_cross_ladder_respond_buy_join_ticket:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
					return;
				faith::cross_ladder_respond_buy_join_ticket msg;
				bool success = msg.ParseFromArray(packet->data, packet->dataLen);
				if (success)
				{
					ws2cs_cross_ladder_refresh_ticket_info send;
					send.role_guid = packet->role_guid;
					send.can_buy_ticket_num = msg.can_buy_num();
					session_ptr->send_to_cs(&send, sizeof(ws2cs_cross_ladder_refresh_ticket_info));
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_cross_ladder_respond_buy_join_ticket);
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_req_create_map:
			{
				ws2ws_cross_ladder_create_map* msg_ptr = (ws2ws_cross_ladder_create_map*)(packet->data);
				if(msg_ptr->match_info.robot_template_id == 0)
					cross_ladder_ws_mgr_new::get_instance().deal_with_create_map_msg(msg_ptr->match_info);
				else
				{
					cross_ladder_ws_mgr_new::get_instance().deal_with_create_robot_map(msg_ptr->match_info);
				}
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_create_map_failed:
			{
				ws2ws_cross_ladder_create_map_failed* msg_ptr = (ws2ws_cross_ladder_create_map_failed*)(packet->data);

				cross_ladder_ws_mgr_new::get_instance().reset_role_state(msg_ptr->first_role_guid);
				cross_ladder_ws_mgr_new::get_instance().reset_role_state(msg_ptr->second_role_guid);
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_send_game_result:
			{
				ws2ws_cross_ladder_send_game_result* msg_ptr = (ws2ws_cross_ladder_send_game_result*)(packet->data);

				cross_ladder_ws_mgr_new::get_instance().deal_with_pk_result_from_ws(msg_ptr->draw, msg_ptr->winner_guid, msg_ptr->loser_guid);
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_send_match_success:
			{
				ws2ws_cross_ladder_send_match_success* cross_ptr = (ws2ws_cross_ladder_send_match_success*)(packet->data);

				cross_ladder_ws_mgr_new::get_instance().deal_with_match_success_msg(packet->role_guid, cross_ptr);
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_sync_game_result:
			{
				ws2ws_cross_ladder_sync_game_result* msg_ptr = (ws2ws_cross_ladder_sync_game_result*)(packet->data);
				cross_ladder_ws_mgr_new::get_instance().deal_with_pk_result_msg(packet->role_guid, msg_ptr);
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_send_last_score:
			{
				ws2ws_cross_ladder_send_last_score* msg_ptr = (ws2ws_cross_ladder_send_last_score*)(packet->data);
				if (nullptr == msg_ptr)
					return;
				client_session* pSession = client_session_mgr::getInstance().get_session(msg_ptr->role_guid);
				if (nullptr != pSession)
				{
					ws2cs_cross_ladder_send_last_score msg;
					msg.role_guid  = msg_ptr->role_guid;
					msg.last_score = msg_ptr->last_score;
					pSession->send_to_cs(&msg, sizeof(ws2cs_cross_ladder_send_last_score));
				}
			}
			break;
			case e_msgindex_ws2ws_cross_ladder_notify_get_season_reward:
			{
				ws2ws_cross_ladder_notify_season_reward* msg_ptr = (ws2ws_cross_ladder_notify_season_reward*)(packet->data);
				if (nullptr == msg_ptr)
					return;
				for (auto index = 0; index < init_session_max; ++index)
				{
					client_session* pSession = client_session_mgr::getInstance().get_session_by_array_index(index);
					if (pSession->is_vaild())
					{
						cross_ladder_ws_mgr_new::get_instance().send_req_cross_ladder_role_info(pSession);
					}
				}
			}
			break;
			case e_msgindex_ws2ws_legion_proto_apply_city_war_bid:
			{
				ws2ws_legion_proto_apply_city_war_bid * msg = (ws2ws_legion_proto_apply_city_war_bid*)(packet->data);

				city_war_territory_mgr::get_instance().check_cross_server_apply_city_war_bid(msg->serverid,msg->legion_guid,msg->terr_id,msg->bid_index, msg->role_guid);
				break;;
			}
			case e_msgindex_ws2ws_legion_proto_apply_city_war_bid_response:
			{
				ws2ws_legion_proto_apply_city_war_bid_response *msg = (ws2ws_legion_proto_apply_city_war_bid_response*)(packet->data);

				city_war_territory_mgr::get_instance().cross_server_apply_city_war_bid_response(msg->terr_id,msg->bid_index,msg->result,msg->needprice,msg->role_guid,msg->next_bid_price,msg->old_legion_guid);
				break;
			}
			case e_msgindex_ws2ws_legion_proto_return_city_war_bid_money:
			{
				ws2ws_legion_proto_return_city_war_bid_money *msg = (ws2ws_legion_proto_return_city_war_bid_money*)(packet->data);

				city_war_territory_mgr::get_instance().cross_server_return_city_war_bid_response(msg->legion_guid, msg->price, msg->terri_id);
				break;
			}
			case e_msgindex_ws2ws_legion_proto_apply_city_war_bid_end:
			{
				ws2ws_legion_proto_apply_city_war_bid_end *msg = (ws2ws_legion_proto_apply_city_war_bid_end*)(packet->data);

				city_war_territory_mgr::get_instance().cross_server_apply_city_war_bid_end(msg->terr_id, msg->bid_index, msg->legion_guid, msg->role_guid, msg->next_bid_price,msg->server_id, msg->legion_name, msg->need_price);
				break;
			}
			case e_msgindex_ws2ws_set_territory_prior_maintain:
			{
				ws2ws_set_territory_prior_maintain *msg = (ws2ws_set_territory_prior_maintain*)(packet->data);

				city_war_territory_mgr::get_instance().req_cross_server_territory_prior_maintain_gate(msg->legin_guid,msg->territory_ids,msg->serverid,msg->role_guid, msg->territory_ids_num);
				break;
			}
			case e_msgindex_ws2ws_set_territory_prior_maintain_response:
			{
				ws2ws_set_territory_prior_maintain_response *msg = (ws2ws_set_territory_prior_maintain_response*)(packet->data);

				city_war_territory_mgr::get_instance().req_cross_server_territory_prior_maintain_end(msg->role_guid, msg->territory_ids, msg->prior_rets, msg->territory_ids_num);
				break;
			}
			case e_msgindex_ws2wsreq_legion_send_territory_info:
			{
				ws2wsreq_legion_send_territory_info *msg = (ws2wsreq_legion_send_territory_info*)(packet->data);

				city_war_territory_mgr::get_instance().cross_server_apply_city_war_bid_end(msg->role_guid, msg->info, msg->need_price, msg->legion_guid);
				break;
			}
			case e_msgindex_ws2ws_legion_proto_push_city_war_info:
			{
				ws2ws_legion_proto_push_city_war_info *msg = (ws2ws_legion_proto_push_city_war_info*)(packet->data);

				city_war_territory_mgr::get_instance().push_city_info_to_legion(msg->legion_guid, msg->city_info);
				break;
			}
			case e_msgindex_ws2wsreq_legion_get_city_war_info:
			{
				ws2wsreq_legion_get_city_war_info *msg = (ws2wsreq_legion_get_city_war_info*)(packet->data);

				city_war_territory_mgr::get_instance().req_cross_server_legion_city_war_info_gate(msg->role_guid, msg->server_id);
				break;
			}
			case e_msgindex_ws2wsreq_legion_get_city_war_info_end:
			{
				ws2wsreq_legion_get_city_war_info_end *msg = (ws2wsreq_legion_get_city_war_info_end*)(packet->data);

				city_war_territory_mgr::get_instance().req_cross_server_legion_city_war_info_end(msg->role_guid, msg->war_info);
				break;
			}
			case e_msgindex_ws2wsreq_get_occupation_daily_award:
			{
				ws2wsreq_get_occupation_daily_award *msg = (ws2wsreq_get_occupation_daily_award*)(packet->data);

				city_war_territory_mgr::get_instance().req_get_occupation_daily_award_gate(msg->role_guid,msg->legion_guid,msg->terr_id,msg->server_id);
				break;
			}
			case e_msgindex_ws2wsreq_get_occupation_daily_award_end:
			{
				ws2wsreq_get_occupation_daily_award_end *msg = (ws2wsreq_get_occupation_daily_award_end*)(packet->data);

				city_war_territory_mgr::get_instance().req_get_occupation_daily_award_end(msg->role_guid, msg->legion_guid, msg->terr_id, msg->notice);
				break;
			}
			case e_msgindex_ws2wssend_cross_war_begin_notice:
			{
				legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
				city_war_mgr.send_cross_server_war_begin_notice();
				break;
			}
			case e_msgindex_ws2ws_maintain_terrories:
			{
				legion_ws_mgr& legion_ws_msg_ref = legion_ws_mgr::get_instance();
				legion_ws_msg_ref.deduct_maintain_territories();
				break;
			}
			case e_msgindex_ws2ws_end_one_territory_to_all_mem:
			{
				ws2ws_end_one_territory_to_all_mem *msg = (ws2ws_end_one_territory_to_all_mem*)(packet->data);

				if (msg->need_send_winner_msg)
				{
					city_war_territory_mgr::get_instance().territory_notice(msg->territory_id, msg->legion_guid);
				}
				else 
				{
					legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(msg->legion_guid);
					if (nullptr != legion_ws_ptr)
					{
						legion_ws_ptr->send_one_territory_to_all_mem(msg->territory_id);	
					}
				}
				break;
			}
			case e_msgindex_ws2ws_end_war_info:
			{
				ws2ws_end_war_info *msg = (ws2ws_end_war_info*)(packet->data);

				city_war_territory_mgr::get_instance().on_cross_server_territory_war_end_info(msg->legion_guid, msg->territory_id, msg->is_winner, msg->is_defense_win);
				break;
			}
			case e_msgindex_ws2ws_refresh_reward_info:
			{
				ws2ws_refresh_reward_info *msg = (ws2ws_refresh_reward_info*)(packet->data);

				city_war_territory_mgr::get_instance().ws2ws_refresh_award_state_end(msg->legion_guid);
				break;
			}
			case e_msgindex_ws2ws_get_cross_server_level:
			{
				ws2ws_get_cross_server_level* req = (ws2ws_get_cross_server_level*)(packet->data);
				if (req->server_id <= 0)
				{
					return;
				}
				cross_server_world_boss_ws_mgr::get_instance().send_server_level(req->server_id);
			}
			break;
			case e_msgindex_ws2ws_get_cross_server_level_end:
			{
				ws2ws_get_cross_server_level_end* req = (ws2ws_get_cross_server_level_end*)(packet->data);
				if (req->server_id <= 0)
				{
					return;
				}
				cross_server_world_boss_ws_mgr::get_instance().server_level_func(req->true_server_level, req->false_server_level, req->server_id);
			}
			break;
			case e_msgindex_ws2ws_make_cross_server_world_legion_award:
			{
				ws2ws_make_cross_server_world_legion_award* req = (ws2ws_make_cross_server_world_legion_award*)(packet->data);

				cross_server_world_boss_ws_mgr::get_instance().make_cross_world_boss_legion_award(req->legion_guid, req->boss_tem_id, req->score_rank, req->play_guid_list, req->play_num);
			}
			break;
			case e_msgindex_ws2ws_grant_kill_boss_legion_award:
			{
				ws2ws_grant_kill_boss_legion_award* req = (ws2ws_grant_kill_boss_legion_award*)(packet->data);

				cross_server_world_boss_ws_mgr::get_instance().make_kill_boss_legion_awaed(req->kill_boss_legion_guid, req->boss_id, req->play_guid_list, req->play_num);
			}
			break;
			case e_msgindex_ws2ws_refresh_kill_boss_show_info:
			{
				ws2ws_cross_refresh_kill_boss_show_info* req = (ws2ws_cross_refresh_kill_boss_show_info*)(packet->data);

				cross_server_world_boss_ws_mgr::get_instance().set_kill_boss_show_info(req->boss_info);
			}
			break;
			case e_msg_ws2ws_get_legion_chief_guid:
			{
				ws2ws_get_legion_chief_guid* req = (ws2ws_get_legion_chief_guid*)(packet->data);
				if (req->legion_guid.is_valid() == false)
				{
					return;
				}
				legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(req->legion_guid);
				if (legion_ptr == nullptr)
				{
					return;
				}
				ws2ws_set_legion_chief_guid msg;
				msg.legion_guid = req->legion_guid;
				msg.rank_type = req->rank_type;
				msg.legion_chief_guid = legion_ptr->get_chief_guid();
				s_legion_member_info* play_info = legion_ptr->get_member(msg.legion_chief_guid);
				msg.legion_chief_vip_level = 0;
				if (play_info != nullptr)
				{
					msg.legion_chief_vip_level = play_info->data_ary[e_legion_member_info_vip_level];
				}				
				msg.set_legion_chief_name(legion_ptr->get_chief_name());
				int32 server_id = world_server::getInstance().get_cross_id();
				if (server_id <= 0)
				{
					return;
				}
				cross::send_msg_to_ws(guid_64(), server_id, e_msg_ws2ws_set_legion_chief_guid, &msg, sizeof(msg));
			}
			break;
			case e_msg_ws2ws_set_legion_chief_guid:
			{
				ws2ws_set_legion_chief_guid* req = (ws2ws_set_legion_chief_guid*)(packet->data);
				if (req->legion_guid.is_valid() == false)
				{
					return;
				}
				ranking_mgr_ws::set_ranking_leigon_chief_guid(req->rank_type, req->legion_guid, req->legion_chief_guid, req->legion_chief_name, req->legion_chief_vip_level);
			}
			break;
			case e_msg_ws2ws_send_legion_welfare_rank:
			{
				ws2ws_send_legion_welfare_rank* req = (ws2ws_send_legion_welfare_rank*)(packet->data);
				if (req->legion_guid.is_valid() == false)
				{
					return;
				}
				legion_ws* legion_ptr = legion_ws_mgr::get_instance().get_legion(req->legion_guid);
				if (nullptr == legion_ptr)
				{
					return;
				}
				legion_ptr->send_legion_welfare_by_activity_and_rank(e_activity_type_cross_server_world_boss, req->rank_index);
			}
			break;
			case e_msgindex_ws2ws_refresh_kill_boss_show_info_all:
			{
				ws2ws_cross_refresh_kill_boss_show_info_all * req = (ws2ws_cross_refresh_kill_boss_show_info_all *)(packet->data);
				cross_server_world_boss_ws_mgr::get_instance().set_kill_boss_show_info_all(req->boss_info, req->data_num);

				world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_boss);
				
			}
			break;
			case e_msgindex_ws2ws_sync_attack_city_legion_info:
			{
				ws2ws_sync_attack_city_legion_info * req = (ws2ws_sync_attack_city_legion_info *)(packet->data);
				if (req == nullptr)
				{
					return;
				}
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_attack_city);
			}
			break;
			case e_msgindex_ws2ws_send_attack_city_group_mail:
			{
				ws2ws_send_attack_city_group_mail * req = (ws2ws_send_attack_city_group_mail *)(packet->data);
				if (req == nullptr)
				{
					return;
				}
				for (int32 i = 0; i < (e_attack_city_group_level_max * attack_city_group_max_num); ++i)
				{
					guid_64 legion_guid = req->legion_guid[i];
					if (legion_guid.is_valid())
					{
						legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(req->legion_guid[i]);
						if (nullptr != legion_ws_ptr)
						{
							legion_ws_ptr->mail_on_attack_city_group_up(i);
						}
					}
				}
			}
			break;
			case e_msgindex_ws2ws_bid_info_to_ws:
			{
				ws2ws_bid_info_to_ws * msg = (ws2ws_bid_info_to_ws*)(packet->data);
				city_war_territory_mgr::get_instance().send_city_info_all_to_other_ws_end(msg->legion_guid,msg->bid_infos,msg->bid_infos_num);

				world_server::getInstance().set_ws_loading_flag(e_ws_flag_city_war);
				
				break;
			}
			case e_msgindex_ws2ws_clear_bid_info_to_gate:
			{
				ws2ws_clear_bid_info_to_gate *msg = (ws2ws_clear_bid_info_to_gate*)(packet->data);

				city_war_territory_mgr::get_instance().clear_territory_legion_bid_info(msg->territory_id, msg->legion_guid);
				break;
			}
			case e_msgindex_ws2ws_kick_player:
			{
				ws2ws_kick_out_player *msg = (ws2ws_kick_out_player*)(packet->data);

				guid_64 role_guid = msg->role_guid;
				bool need_send = msg->need_send_save_end;
				client_session* session_ptr = client_session_mgr::getInstance().get_session(role_guid);
				if (nullptr != session_ptr)
				{
					session_ptr->set_need_send_save_end(need_send);
					client_session_mgr::getInstance().kickout_account(role_guid, e_logout_result_replaced_by_new_login);
				}
			}
			break;
			case e_msgindex_ws2ws_player_legion_info:
			{
				ws2ws_cross_player_legion_info* msg = (ws2ws_cross_player_legion_info*)(packet->data);

				s_cross_player_legion_info& legion_info = msg->legion_info;
				legion_info.save_time_stamp = time_helper::get_cur_time_new().second;
				guid_64 role_guid = msg->role_guid;
				if (role_guid.is_valid())
				{
					client_session_mgr::getInstance().set_cross_player_legion(legion_info, role_guid);
				}
			}
			break;
			case e_msg_gate2ws_send_boss_ranking_list:
			{
				gate2ws_send_boss_ranking_list* msg = (gate2ws_send_boss_ranking_list*)(packet->data);

				if (msg->get_pak_length() != packet->dataLen)
				{
					return;
				}
				cross_server_world_boss_ws_mgr::get_instance().set_cross_boss_ranking_list(msg->boss_id, msg->data_ary, msg->data_num);
			}
			break;
			case e_msg_gate2ws_send_kill_boss_notice:
			{
				gate2ws_send_kill_boss_notic* msg = (gate2ws_send_kill_boss_notic*)(packet->data);

				gate2ws_send_kill_boss_notic resp;
				memcpy(&resp, msg, sizeof(gate2ws_send_kill_boss_notic));
				cross_server_world_boss_ws_mgr::get_instance().send_boss_dead_notice(msg->item_info_ptr.boss_template, msg->item_info_ptr.notice_id, msg->item_info_ptr.item_num, resp.item_info_ptr.data_ary, msg->item_info_ptr.notice_string);
			}
			break;
			case e_msgindex_ws2gws_send_buy_cloud:
			{
				ws2gws_send_bug_cloud* msg = (ws2gws_send_bug_cloud*)(packet->data);

				cloud_shop_mgr::get_instance().recv_other_ws_buy_msg(msg->role_guid, msg->server_id, msg->req_type, msg->role_name, msg->role_class_type, msg->sender_server_id);
			}
			break;
			case e_msgindex_gws2ws_cloud_shop_buy:
			{
				ws2cs_cloud_shop_buy_req* msg = (ws2cs_cloud_shop_buy_req*)(packet->data);

				guid_64 role_guid = msg->role_guid;
				client_session* pSession = client_session_mgr::getInstance().get_session(role_guid);
				ws2cs_cloud_shop_buy_req req;
				req = *msg;
				if (nullptr != pSession)
				{
					bool is_use = proto_by_lua(e_msgindex_ws2cs_cloud_shop_buy_req);
					if (is_use == false)
					{
						pSession->send_to_cs(&req, sizeof(ws2cs_cloud_shop_buy_req));
					}
					else
					{
						faith::ws2cs_proto::cloud_shop_buy_req pro_msg;
						req.to_proto(pro_msg);
						pSession->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_cloud_shop_buy_req);
					}
				}
			}
			break;
			case e_msgindex_ws2gws_cloud_shop_buy_end:
			{
				cs2ws_cloud_shop_buy_resp* msg = (cs2ws_cloud_shop_buy_resp*)(packet->data);

				cloud_shop_mgr::get_instance().send_buy_result_to_player(msg->rold_guid, msg->error_id, msg->buy_type, msg->server_id, msg->sender_server_id);
			}
			break;
			case e_msg_index_ws2ws_time_limit_activity_common_data_sync:
			{
				ws2ws_time_limit_activity_common_data_sync* msg = (ws2ws_time_limit_activity_common_data_sync*)(packet->data);

				vector<int32> condition_list;
				for (int32 idx = 0; idx < msg->condition_num; idx++)
				{
					condition_list.push_back(msg->condition_list[idx]);
				}
				time_limit_activity_ws_mgr::get_instance().update_activity_common_data_on_gate(msg->activity_type, msg->data_num, condition_list);
				break;
			}
			case e_msg_index_ws2ws_time_limit_activity_common_data_sync_end:
			{
				ws2ws_time_limit_activity_common_data_sync_end* msg = (ws2ws_time_limit_activity_common_data_sync_end*)(packet->data);

				vector<int32> data_type_list;
				vector<int32> data_num_list;
				data_type_list.push_back(msg->activity_type);
				data_num_list.push_back(msg->activity_schedule);
				time_limit_activity_ws_mgr::get_instance().send_activity_common_data(data_type_list, data_num_list);
				break;
			}
			case e_msg_index_ws2ws_check_time_limit_activity_common_data:
			{
				ws2ws_check_time_limit_activity_common_data* msg = (ws2ws_check_time_limit_activity_common_data*)(packet->data);
				time_limit_activity_ws_mgr::get_instance().check_common_data_on_gate(msg->server_id, msg->activity_id, msg->user_guid, msg->activity_type, msg->check_data);
				break;
			}
			case e_msg_index_ws2ws_check_time_limit_activity_common_data_end:
			{
				ws2ws_check_time_limit_activity_common_data_end* msg = (ws2ws_check_time_limit_activity_common_data_end*)(packet->data);

				time_limit_activity_ws_mgr::get_instance().check_common_data_end(msg->activity_id, msg->user_guid, msg->is_achieve);
				break;
			}
			case e_msg_index_ws2ws_get_time_limit_activity_common_data_to_gate:
			{
				ws2ws_get_time_limit_activity_common_data_to_gate* msg = (ws2ws_get_time_limit_activity_common_data_to_gate*)(packet->data);

				time_limit_activity_ws_mgr::get_instance().get_activity_common_data_on_gate(msg->server_id, msg->role_guid);
				break;
			}
			case e_msg_index_ws2ws_get_time_limit_activity_common_data_to_ws:
			{
				ws2ws_get_time_limit_activity_common_data_to_ws* msg = (ws2ws_get_time_limit_activity_common_data_to_ws*)(packet->data);

				vector<int32> data_type_list;
				vector<int32> data_num_list;
				data_type_list.push_back(msg->activity_type);
				data_num_list.push_back(msg->activity_schedule);
				time_limit_activity_ws_mgr::get_instance().send_activity_common_data_one(msg->role_guid, data_type_list, data_num_list);
				break;
			}
			case e_msgindex_ws2ws_cross_server_time:
			{
				ws2ws_cross_server_time* msg = (ws2ws_cross_server_time*)(packet->data);

				world_server::getInstance().set_cross_server_open_time(msg->cross_server_time, true);
			}
			break;
			case e_msgindex_ws2gws_get_cloud_shop:
			{
				ws2gws_get_cloud_shop* msg = (ws2gws_get_cloud_shop*)(packet->data);

				cloud_shop_mgr::get_instance().send_open_msg_to_other_server(msg->role_guid, msg->sender_server_id);
			}
			break;
			case e_msgindex_ws2ws_change_gate_legion_name:
			{
				ws2ws_change_gate_legion_name* msg = (ws2ws_change_gate_legion_name*)(packet->data);

				if (!msg->legion_guid.is_valid())
				{
					return;
				}
				if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_boss))
				{
					cross_server_world_boss_ws_mgr::get_instance().change_legion_name(msg->legion_guid, msg->legion_name);
				}
				if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
				{
					city_war_territory_mgr::get_instance().change_legion_name(msg->legion_guid, msg->legion_name);
				}
				if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_attack_city))
				{
					attack_city_ws_mgr::get_instance().change_legion_name(msg->legion_guid, msg->legion_name);
				}

				//改变排行榜军团名
				ranking_mgr_ws::set_gate_ranking_legion_name(msg->legion_guid, msg->legion_name);
				//向cs发送更改军团名消息
				ws2cs_change_gate_legion_name _msg;
				_msg.legion_guid = msg->legion_guid;
				_msg.set_legion_name(msg->legion_name);
				bool is_use = proto_by_lua(e_msg_index_ws2cs_change_gate_legion_name);
				if (is_use == false)
				{
					world_server::getInstance().broadcast(&_msg, sizeof(_msg), e_server_type_cs);
				}
				else
				{
					faith::ws2cs_proto::change_gate_legion_name pro_msg;
					_msg.to_proto(pro_msg);
					world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_change_gate_legion_name, e_server_type_cs);
				}
			}
			break;
			case e_msgindex_ws2ws_sync_oracle_trial_rank:
			{
				ws2ws_sync_oracle_trial_rank* msg = (ws2ws_sync_oracle_trial_rank*)(packet->data);

				guid_64 old_fist_play = -1;
				ranking_list* rank_list = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_Oracle_Trial);
				if (rank_list != nullptr && rank_list->empty() == false)
				{
					ranking_list_ite it = rank_list->begin();
					s_ranking_player_info play_rank_info = *it;
					old_fist_play = play_rank_info.role_guid;
				}

				ranking_mgr_ws::sync_player_info(msg->play_info);
				ranking_mgr_ws::can_send_oracle_trial_notice(old_fist_play, msg->play_info.role_guid);
			}
			break;
			case e_msgindex_ws2ws_get_oracle_trial_rank:
			{
				ws2ws_get_oracle_trial_rank* msg = (ws2ws_get_oracle_trial_rank*)(packet->data);

				ranking_mgr_ws::send_oracle_trial_rank_info(msg->play_guid, msg->server_id);
			}
			break;
			case e_msgindex_ws2ws_get_oracle_trial_rank_end:
			{
				ws2ws_get_oracle_trial_rank_end* msg = (ws2ws_get_oracle_trial_rank_end*)(packet->data);

				client_session* session_ptr = client_session_mgr::getInstance().get_session(msg->play_guid);
				if (session_ptr == nullptr)
				{
					return;
				}
				ranking_proto_get_oracle_trial_rank_end req;
				msg->data_num = msg->data_num > oracle_trial_rank_info ? oracle_trial_rank_info : msg->data_num;
				req.set_data_num(msg->data_num);
				for (int32 i = 0; i < msg->data_num; ++i)
				{
					ranking_proto_ranking_player_info_lua* ranking_player_data = req.add_play_info();
					ranking_player_data->set_player_name(msg->play_info[i].role_name);
					ranking_player_data->set_role_guid_a(msg->play_info[i].role_guid.A);
					ranking_player_data->set_role_guid_b(msg->play_info[i].role_guid.B);
					ranking_player_data->set_ranking_type(msg->play_info[i].ranking_type);
					ranking_player_data->set_ranking_value(msg->play_info[i].ranking_value);
					ranking_player_data->set_supporting_guid_a(msg->play_info[i].supporting_guid.A);
					ranking_player_data->set_supporting_guid_b(msg->play_info[i].supporting_guid.B);
					ranking_player_data->set_supporting_name(msg->play_info[i].supporting_name);
					ranking_player_data->set_vip_level(msg->play_info[i].vip_level);
					ranking_player_data->set_peak(msg->play_info[i].Peak);
					ranking_player_data->set_server_id(msg->play_info[i].server_id);
					ranking_player_data->set_standby_parameter(msg->play_info[i].standby_parameter);
				}
				session_ptr->send_to_client(&req, e_msgindex_s2c_get_oracle_trial_rank_end);
			}
			break;
			case e_msg_gate2ws_server_info:
			{
				gate2ws_server_info* msg = (gate2ws_server_info*)(packet->data);
				if (nullptr == msg)
				{
					return;
				}
				world_server::getInstance().set_server_info_arr(msg->server_info_value, msg->server_info_type);
				world_server::getInstance().send_server_info_arr_to_cs(msg->server_info_type);
				if (msg->server_info_type == e_server_info_type_server_cross_state)
				{
					world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_server_state);
				}
			}
			break;
			case e_msgindex_ws2ws_send_is_pk_king:
			{
				ws2ws_send_is_pk_king* msg = (ws2ws_send_is_pk_king*)(packet->data);
				if (nullptr == msg)
				{
					return;
				}
				pk_king_mgr::get_instance().add_rank_list_data(packet->role_guid);
			}
			break;
			case e_msgindex_ws2ws_break_login_transfer_map:
			{
				ws2ws_break_login_transfer_map* msg = (ws2ws_break_login_transfer_map*)(packet->data);
				if (nullptr == msg)
				{
					return;
				}
				switch (msg->activity_type)
				{
				case e_activity_type_pk_king:
				{
					pk_king_mgr::get_instance().check_can_break_login_map(msg->role_guid, msg->map_guid, msg->server_id);
				}
				break;
				default:
					break;
				}
				
			}
			break;
			case e_msgindex_ws2ws_break_login_transfer_map_end:
			{
				ws2ws_break_login_transfer_map_result* msg = (ws2ws_break_login_transfer_map_result*)(packet->data);
				if (nullptr == msg)
				{
					return;
				}
				pk_king_mgr::get_instance().transfer_break_login_map(msg->role_guid, msg->map_guid, msg->result, msg->map_template_id, msg->war_idex, msg->server_id);
			}
			break;
			case e_msgindex_ws2ws_element_war_req_role_info:
			{
				ws2ws_element_war_req_role_info* msg_ptr = (ws2ws_element_war_req_role_info*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				element_war_ws_mgr::get_instance().req_element_war_info(packet->role_guid, msg_ptr); 
			}
			break;
			case e_msgindex_ws2ws_element_war_send_sign_up:
			{
				ws2ws_element_war_sign_up* msg_ptr = (ws2ws_element_war_sign_up*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				game_proto_element_war_send_sign_up_end msg;
				e_element_war_sign_up_ret ret = e_element_war_sign_up_success;
				if (world_server::getInstance().get_activity_sec_left(e_activity_type_element_war, e_activity_time_get_gaming) <= 0)
				{
					return;
				}
				guid_64 error_guid;
				error_guid.clear_data();
				if (msg_ptr->team_guid.is_valid())
				{
					//多人匹配处理
					std::vector<guid_64> role_guid_list;
					for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
					{
						if (msg_ptr->role_guid_list[i].is_valid())
						{
							role_guid_list.push_back(msg_ptr->role_guid_list[i]);
						}
					}
					ret = element_war_ws_mgr::get_instance().team_sign_up(role_guid_list, msg_ptr->team_guid, error_guid);
					msg.set_ret_index(ret);
					if (error_guid.is_valid())
					{
						msg.set_guid_a(error_guid.A);
						msg.set_guid_b(error_guid.B);
					}
					for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
					{
						if (msg_ptr->role_guid_list[i].is_valid())
						{
							cross::send_msg_to_ws(msg_ptr->role_guid_list[i], msg_ptr->server_id, e_mgsindex_s2c_element_war_send_sign_up_end, &msg);
						}
					}	
				}
				else
				{
					//单人匹配处理
					if (msg_ptr->role_guid_list[0].is_valid())
					{
						ret = element_war_ws_mgr::get_instance().single_sign_up(msg_ptr->role_guid_list[0]);
					}
					
					msg.set_ret_index(ret);
					msg.set_guid_a(msg_ptr->role_guid_list[0].A);
					msg.set_guid_b(msg_ptr->role_guid_list[0].B);
					cross::send_msg_to_ws(msg_ptr->role_guid_list[0], msg_ptr->server_id, e_mgsindex_s2c_element_war_send_sign_up_end, &msg);
				}
				//设置服务器信息
				if (ret == e_element_war_sign_up_success)
				{
					for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
					{
						if (msg_ptr->role_guid_list[i].is_valid())
						{
							element_war_ws_mgr::get_instance().set_role_server_info(msg_ptr->role_guid_list[i], msg_ptr->server_ip, msg_ptr->server_port);
						}
					}
				}
			}
			break;
			case e_mgsindex_s2c_element_war_send_sign_up_end:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				faith::game_proto_element_war_send_sign_up_end msg;
				bool success = msg.ParseFromArray(packet->data, packet->dataLen);
				if (success)
				{
			
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_mgsindex_s2c_element_war_send_sign_up_end);
			}
			break;
			case e_mgsindex_s2c_element_war_send_close_sign_up_end:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				faith::game_proto_element_war_send_close_sign_up_end msg;
				bool success = msg.ParseFromArray(packet->data, packet->dataLen);
				if (success)
				{

				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_mgsindex_s2c_element_war_send_close_sign_up_end);

			}
			break;
			case e_msgindex_ws2ws_element_war_close_sign_up:
			{
				ws2ws_element_war_close_sign_up* msg_ptr = (ws2ws_element_war_close_sign_up*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				element_war_ws_mgr::get_instance().sign_up_leave(msg_ptr->role_guid);
			}
			break;
			case e_msgindex_ws2ws_element_war_req_create_map:
			{
				ws2ws_element_war_create_map* msg_ptr = (ws2ws_element_war_create_map*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				element_war_ws_mgr::get_instance().deal_with_create_map_msg(msg_ptr->match_info);
			}
			break;
			case e_msgindex_ws2ws_element_war_create_map_failed:
			{
				ws2ws_element_war_create_map_failed* msg_ptr = (ws2ws_element_war_create_map_failed*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				for (int32 i = 0; i < ELEMENT_WAR_PLAY_NUM; ++i)
				{
					element_war_ws_mgr::get_instance().reset_role_state(msg_ptr->first_role_guid[i]);
					element_war_ws_mgr::get_instance().reset_role_state(msg_ptr->second_role_guid[i]);
				}		
			}
			break;
			case e_msgindex_ws2ws_element_war_send_match_success:
			{
				ws2ws_element_war_send_match_success* msg_ptr = (ws2ws_element_war_send_match_success*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				element_war_ws_mgr::get_instance().deal_with_match_success_msg(packet->role_guid, msg_ptr);
			}		
			break;
			case e_msgindex_ws2ws_element_war_sync_game_result:
			{
				ws2ws_element_war_sync_game_result* msg_ptr = (ws2ws_element_war_sync_game_result*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				element_war_ws_mgr::get_instance().deal_with_pk_result_msg(packet->role_guid, msg_ptr);
			}
			break;
			case e_msgindex_ws2ws_element_war_check_can_get_mission_reward:
			{
				ws2ws_element_war_check_can_get_mission_reward* msg_ptr = (ws2ws_element_war_check_can_get_mission_reward*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				element_war_ws_mgr::get_instance().check_can_get_mission_reward(packet->role_guid, msg_ptr->mission_index);
			}
			break;
			case e_msgindex_ws2ws_element_war_check_can_get_mission_reward_end:
			{
				ws2ws_element_war_check_can_get_mission_reward_end* msg_ptr = (ws2ws_element_war_check_can_get_mission_reward_end*)(packet->data);
				if (msg_ptr == nullptr)
				{
					return;
				}
				if (msg_ptr->ret_index == e_element_war_get_mission_reward_succeed)
				{
					ws2cs_element_war_check_can_get_mission_reward_end msg;
					msg.role_guid = msg_ptr->role_guid;
					msg.mission_id = msg_ptr->mission_id;
					msg.mission_index = msg_ptr->mission_index;
					msg.ret_index = msg_ptr->ret_index;
					bool is_use = proto_by_lua(e_msgindex_ws2cs_element_war_check_can_get_mission_reward_end);
					if (is_use == false)
					{
						world_server::getInstance().broadcast(&msg, sizeof(msg), e_server_type_cs);
					}
					else
					{
						faith::ws2cs_proto::element_war_check_can_get_mission_reward_end pro_msg;
						msg.to_proto(pro_msg);
						world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2cs_element_war_check_can_get_mission_reward_end, e_server_type_cs);
					}
				}
				else
				{
					client_session* session_ptr = client_session_mgr::getInstance().get_session(msg_ptr->role_guid);
					if (session_ptr == nullptr)
					{
						return;
					}
					game_proto_element_war_get_mission_reward_end msg;
					msg.set_ret_index(msg_ptr->ret_index);
					session_ptr->send_to_client(&msg, e_mgsindex_s2c_element_war_get_mission_reward_end);
				}
			}
			break;
			case  e_mgsindex_s2c_element_war_send_role_info:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				faith::game_proto_element_war_role_info msg;
				bool success = msg.ParseFromArray(packet->data, packet->dataLen);
				if (success)
				{
					ws2cs_element_war_refresh_ticket_info send;
					send.role_guid = packet->role_guid;
					send.can_buy_ticket_num = msg.info_data(e_element_war_role_info_ticket_num);
					session_ptr->send_to_cs(&send, sizeof(ws2cs_element_war_refresh_ticket_info));
					if (msg.is_online())
					{
						ws2cs_element_war_send_last_score send_last_score;
						send_last_score.role_guid = packet->role_guid;
						send_last_score.last_score = msg.info_data(e_element_war_role_info_last_score);
						send_last_score.last_ranking = msg.last_score();
						bool is_use = proto_by_lua(e_msgindex_ws2cs_element_war_send_last_score);
						if (is_use == false)
						{
							session_ptr->send_to_cs(&send_last_score, sizeof(ws2cs_element_war_send_last_score));
						}
						else
						{
							faith::ws2cs_proto::element_war_send_last_score pro_msg;
							send_last_score.to_proto(pro_msg);
							session_ptr->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_element_war_send_last_score);
						}
					}
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_mgsindex_s2c_element_war_send_role_info);
			}
			break;
			case e_msgindex_ws2ws_element_war_notify_get_season_reward:
			{
				ws2ws_element_war_notify_season_reward* msg_ptr = (ws2ws_element_war_notify_season_reward*)(packet->data);
				if (nullptr == msg_ptr)
					return;
				for (auto index = 0; index < init_session_max; ++index)
				{
					client_session* pSession = client_session_mgr::getInstance().get_session_by_array_index(index);
					if (pSession->is_vaild())
					{
						element_war_ws_mgr::get_instance().send_req_element_war_role_info(pSession);
					}
				}
			}
			break;
			case e_msgindex_ws2ws_element_ladder_send_last_score:
			{
				ws2ws_element_ladder_send_last_score* msg_ptr = (ws2ws_element_ladder_send_last_score*)(packet->data);
				if (nullptr == msg_ptr)
					return;
				client_session* pSession = client_session_mgr::getInstance().get_session(msg_ptr->role_guid);
				if (nullptr != pSession)
				{
					ws2cs_element_war_send_last_score msg;
					msg.role_guid = msg_ptr->role_guid;
					msg.last_score = msg_ptr->last_score;
					bool is_use = proto_by_lua(e_msgindex_ws2cs_element_war_send_last_score);
					if (is_use == false)
					{
						pSession->send_to_cs(&msg, sizeof(ws2cs_element_war_send_last_score));
					}
					else
					{
						faith::ws2cs_proto::element_war_send_last_score pro_msg;
						msg.to_proto(pro_msg);
						pSession->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_element_war_send_last_score);
					}
				}
			}
			break;
			case e_msgindex_ws2ws_element_war_send_math_tip:
			{
				ws2ws_element_war_send_math_tip* msg_ptr = (ws2ws_element_war_send_math_tip*)(packet->data);
				if (nullptr == msg_ptr)
					return;
				client_session* pSession = client_session_mgr::getInstance().get_session(msg_ptr->role_guid);
				if (nullptr != pSession)
				{
					pSession->send_notice("90305869");
				}
			}
			break;
			case e_msgindex_ws2ws_transfer_attack_city_map_result:
			{
				ws2ws_transfer_attack_city_map_result* cross_info_ptr = (ws2ws_transfer_attack_city_map_result*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}
				attack_city_ws_mgr::get_instance().transfer_to_map_with_gate_msg(cross_info_ptr->role_guid, cross_info_ptr->result, cross_info_ptr->map_temp_id, cross_info_ptr->map_guid, cross_info_ptr->war_idex, cross_info_ptr->server_id);
			}
			break;
			case e_msgindex_ws2ws_attack_city_legion_check:
			{
				ws2ws_attack_city_legion_check* cross_info_ptr = (ws2ws_attack_city_legion_check*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}
				attack_city_ws_mgr::get_instance().check_legion_is_valid(cross_info_ptr->legion_guid);
			}
			break;
			case e_msgindex_ws2ws_attack_city_legion_check_end:
			{
				ws2ws_attack_city_legion_check_end* cross_info_ptr = (ws2ws_attack_city_legion_check_end*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}
				attack_city_ws_mgr::get_instance().set_legion_is_valid(cross_info_ptr->legion_guid);
			}
			break;
			case e_msgindex_ws2ws_get_attack_city_legion_info:
			{
				ws2ws_get_attack_city_legion_info* cross_info_ptr = (ws2ws_get_attack_city_legion_info*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}

				attack_city_ws_mgr::get_instance().send_attack_city_legion_info(cross_info_ptr->legion_guid, cross_info_ptr->role_guid, cross_info_ptr->server_id);
			}
			break;
			case e_msgindex_ws2ws_send_attack_city_legion_info:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_attack_city_legion_info);
			}
			case e_msgindex_ws2ws_get_attack_city_legion_rank_info:
			{
				ws2ws_get_attack_city_legion_rank_info* cross_info_ptr = (ws2ws_get_attack_city_legion_rank_info*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}

				attack_city_ws_mgr::get_instance().send_attack_city_legion_rank_info(cross_info_ptr->legion_guid, cross_info_ptr->role_guid, cross_info_ptr->server_id);
			}
			break;
			case e_msgindex_ws2ws_send_attack_city_legion_rank_info:
			{
				client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
				if (nullptr == session_ptr)
				{
					return;
				}
				session_ptr->send_to_client(packet->data, packet->dataLen, e_msgindex_s2c_attack_city_legion_rank_info);
			}
			break;
			case e_msgindex_ws2ws_load_offline_other_relation:
			{
				ws2ws_load_offline_other_relation* cross_info_ptr = (ws2ws_load_offline_other_relation*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}
				ws2dp_load_relation_info_other_server msg;
				msg.sender_guid = cross_info_ptr->sender_guid;
				msg.sender_server_id = cross_info_ptr->sender_server_id;
				msg.other_guid = cross_info_ptr->other_guid;
				msg.list_type = cross_info_ptr->list_type;
				ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
			}
			break;
			case e_msgindex_ws2ws_load_offline_other_relation_end:
			{
				ws2ws_load_offline_other_relation_end* cross_info_ptr = (ws2ws_load_offline_other_relation_end*)(packet->data);
				if (nullptr == cross_info_ptr)
				{
					return;
				}


				client_session* session_player = client_session_mgr::getInstance().get_session(cross_info_ptr->sender_guid);
				if (nullptr == session_player)
				{
					return;
				}

				if (cross_info_ptr->other_relation.relation_guid == cross_info_ptr->sender_guid)
				{
					return;
				}
				session_player->get_relation_list_mgr().recv_other_relation_info_from_db(cross_info_ptr->other_relation);
			}
			break;

			default:
			break;
		}
	}
	void c2ws_req_cross_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		cross_proto_cross_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr)
		{
			return;
		}
		if (gate_proxy::getInstance().is_gate_run())
		{
			//ws2gate_cross_info msg;
			//msg.sign_up_type = (e_cross_sign_up_type)req.cross_type();
			//memcpy(msg.role_name, session_ptr->m_role_info.role_name, max_name_size);
			//msg.role_guid = session_ptr->get_role_guid();
			//msg.template_id = session_ptr->get_role_info_data(e_role_info_template_id);
			//msg.role_gs = session_ptr->get_role_info_data(e_role_info_GS_value);

			//cross::send_msg_to_ws(session_ptr->get_role_guid(), session_ptr->get_role_info_data(e_role_info_server_id), e_msgindex_c2s_cross_cross_info, &msg, sizeof(msg));
		}
		else
		{
			role_pk_manager::getInstance().add_role_pk_info(session_ptr->get_role_guid(), session_ptr->m_role_info.role_name, session_ptr->m_role_info.data_ary[e_role_info_template_id], session_ptr->get_role_gs_value());
		}
	}
	void c2ws_req_sign_up(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		cross_proto_sign_up req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr)
		{
			return;
		}
		if (session_ptr->m_role_info.data_ary[e_role_info_exp_level] < SIGLE_CROSS_LADDER_EXP_LEVEL)
		{
			cross_proto_sign_up_end msg;
			msg.set_sign_type(req.sign_type());
			msg.set_sign_end(e_cross_sign_up_exp_level);
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_sign_up);
			return;
		}
		/*if (time_helper::get_hour())
		{
			cross_proto_sign_up_end msg;
			msg.set_sign_type(req.sign_type());
			msg.set_sign_end(e_cross_sign_up_no_in_time);
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_sign_up);
			return;
		}*/
		if (gate_proxy::getInstance().is_gate_run())
		{
			if (false == gate_proxy::getInstance().is_gate_run())
			{
				cross_proto_sign_up_end msg;
				msg.set_sign_type(req.sign_type());
				msg.set_sign_end(e_cross_sign_up_cross_server_fail);
				session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_sign_up);
				return;
			}

			//ws2gate_sign_up msg;
			//msg.role_guid = session_ptr->get_role_guid();
			//msg.is_join = req.is_join();
			//if (req.sign_type() == e_cross_sign_up_type_sigle)
			//{
			//	msg.team_guid = 0;
			//}
			//cross::send_msg_to_ws(session_ptr->get_role_guid(), session_ptr->get_role_info_data(e_role_info_server_id), e_msgindex_c2s_cross_sign_up, &msg, sizeof(msg));
		}
		else
		{
			e_cross_sign_up_result res = e_cross_sign_up_state_error;
			if (req.is_join())
			{
				res = role_pk_manager::getInstance().sign_up(session_ptr->get_role_guid(), empty_guid_64);
			}
			else
			{
				res = role_pk_manager::getInstance().sign_up_leave(session_ptr->get_role_guid(), empty_guid_64);
			}
			cross_proto_sign_up_end msg;
			msg.set_sign_type(req.sign_type());
			msg.set_sign_end(res);
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_sign_up);
		}
	}
	void gate2ws_rep_sign_up(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//gate2ws_sign_up* packet = (gate2ws_sign_up*)data_ptr;
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->account);
		//if (nullptr == session_ptr)
		//{
		//	return;
		//}
		//cross_proto_sign_up_end msg;
		//msg.set_sign_type(packet->sign_up_type);
		//msg.set_sign_end(packet->result);
		//session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_sign_up);
	}
	void gate2ws_rep_join_war(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//gate2ws_join_war* packet = (gate2ws_join_war*)data_ptr;
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->account);
		//if (nullptr == session_ptr)
		//{
		//	return;
		//}
		//cross_proto_join_war_end msg;
		//msg.set_join_end(0);
		//session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_join_war);
	}

	void dp2ws_req_role_pk_top(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2ws_load_role_pk_top* packet = (dp2ws_load_role_pk_top*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		for (int32 i = 0; i < PK_RANKING_TOP_NUM; ++i)
		{
			role_pk_manager::getInstance().dp_role_pk_info(packet->pk_top_array[i], i);
		}
	}
	void dp2ws_req_role_pk_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2ws_load_role_pk_info* packet = (dp2ws_load_role_pk_info*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		role_pk_manager::getInstance().dp_role_pk_info(packet->role_pk_info);
	}
	void c2ws_req_role_pk_log(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);

		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr)
		{
			return;
		}
		if (gate_proxy::getInstance().is_gate_run())
		{
			cross::send_msg_to_ws(session_ptr->get_role_guid(), session_ptr->get_role_info_data(e_role_info_server_id), e_msgindex_c2s_cross_war_log, data_ptr, data_len);
		}
		else
		{
			server2dp_proto_ws2dp_load_role_pk_log msg;
			msg.set_role_guid(session_ptr->get_role_guid().server_64);
			msg.set_server_id(session_ptr->get_role_info_data(e_role_info_server_id));
			ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_role_pk_log, session_ptr->get_role_info_data(e_role_info_server_id));

			//ws2dp_load_role_pk_log msg;
			//msg.role_guid = session_ptr->get_role_guid();
			//msg.server_id = session_ptr->get_role_info_data(e_role_info_server_id);
			//ws_client::getInstance().send_to_dp(&msg, sizeof(msg), session_ptr->get_role_info_data(e_role_info_server_id));
			return;
		}
	}
	void dp2ws_req_role_pk_log(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2ws_load_role_pk_log* packet = (dp2ws_load_role_pk_log*)data_ptr;
		if (nullptr == packet)
		{
			return;
		}
		cross_proto_role_pk_log_array msg;
		for (int32 i = 0; i < packet->data_len; ++i)
		{
			cross_proto_role_pk_log_end* role_pk_log = msg.add_pk_log_array();
			s_cross_war_db& temp_info = packet->data_array[i];
			role_pk_log->set_target_name_1(temp_info.target_name_1);
			role_pk_log->set_target_name_2(temp_info.target_name_2);
			role_pk_log->set_target_name_3(temp_info.target_name_3);
			role_pk_log->set_ladder_score(temp_info.ladder_score);
			role_pk_log->set_cross_honor(temp_info.cross_honor);
			role_pk_log->set_win_result(temp_info.win_result);
		}
		if (packet->server_id == world_server::getInstance().get_server_id())
		{
			client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
			if (nullptr == session_ptr)
			{
				return;
			}
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_war_log);
		}
		else
		{
			cross::send_msg_to_ws(packet->role_guid, packet->server_id, e_msgindex_s2c_cross_war_log, &msg);
		}
	}

	void c2ws_cross_ladder_req_role_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		cross_ladder_request_role_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr || session_ptr->is_self_server() == false)
			return;

		if (cross_ladder_ws_mgr_new::get_instance().send_req_cross_ladder_role_info(session_ptr) == false)
		{
			cross_ladder_send_role_info msg;
			msg.set_cross_result(0);
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_ladder_send_role_info);
		}
	}

	void c2ws_cross_ladder_req_sign_up(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		cross_ladder_request_sign_up req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr || session_ptr->is_self_server() == false || world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_ladder) == false)
			return;
		e_cross_ladder_sign_up_ret	ret = cross_ladder_ws_mgr_new::get_instance().check_can_sign_up(session_ptr);
		if(e_cross_ladder_sign_up_success != ret)
		{
			cross_ladder_respond_sign_up msg;
			msg.set_sign_up_result(ret);
			session_ptr->send_to_client(&msg, e_msgindex_s2c_cross_ladder_respond_sign_up);
		}
		else
		{
			ws2ws_cross_ladder_sign_up msg;
			msg.is_join		= req.is_join();
			msg.server_id   = world_server::getInstance().get_server_id();
			memcpy(msg.server_ip, world_server::getInstance().get_out_ip().c_str(), max_ip_address_length);
			msg.server_port = world_server::getInstance().get_out_port();
			msg.online_num	= client_session_mgr::getInstance().get_game_num();
			cross::send_msg_to_ws(session_ptr->get_role_guid(), cross_ladder_ws_mgr_new::get_instance().get_match_server_id(), e_msgindex_ws2ws_cross_ladder_send_sign_up, &msg, sizeof(msg));
		}
	}

	void c2ws_element_war_req_role_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		game_proto_element_war_request_role_info req;
		parse_msg::getInstance().parse_message_new(&req, data_ptr, data_len);
		s_client_uid client_uid;
		parse_msg::getInstance().parse_clientuid_from_msg_new(data_ptr, client_uid);
		client_session* session_ptr = client_session_mgr::getInstance().get_session(client_uid);
		if (nullptr == session_ptr || session_ptr->is_self_server() == false)
			return;

		if (element_war_ws_mgr::get_instance().send_req_element_war_role_info(session_ptr) == false)
		{
			game_proto_element_war_role_info msg;
			msg.set_cross_result(0);
			session_ptr->send_to_client(&msg, e_mgsindex_s2c_element_war_send_role_info);
		}
	}
}
