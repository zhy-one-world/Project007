/********************************************************************
created: 2014/07/14
created: 14:7:2014 20:25
file base: msgproc_ws
file ext: cpp
author: lidongyang

purpose: 
*********************************************************************/

#include "cell_server.hpp"
#include "internal/char_msg.hpp"
#include "connection_mgr.hpp"
#include "game_cfg/servers_config.h"
#include "Logic/activity_def.hpp"
#include "logic/auction_mgr.h"
#include "logic/element_heart_def.hpp"
#include "logic/item_set.h"
#include "logic/legion_skill.h"
#include "logic/player.hpp"
#include "logic/scene/map_cs.h"
#include "logic/skill_manager.h"
#include "logic/unit_man.h"
#include "logic/world_cs.h"
#include "msgproc_ws.hpp"
#include "server_log.hpp"
#include "system/item/item_system.h"
#include "template/template_manager.h"
#include "title_def.hpp"
#include "utility/globle_data.h"
#include "utility/random.h"
#include "internal/core.hpp"
#include "utility/parse_msg.h"
#include "internal/login_msg.hpp"
#include "logic/npc.hpp"
#include "internal/marry_msg.hpp"
#include "internal/event_msg.hpp"
#include "internal/world_boss_msg.hpp"
#include "internal/chat_msg.hpp"
#include "internal/big_player_msg.hpp"
#include "internal/cloud_shop_msg.hpp"
#include "cloud_shop_def.hpp"
#include "internal/cross_server_harry_msg.hpp"
#include "internal/cross_ladder_msg.hpp"
#include "internal/cross_server_world_boss_msg.hpp"
#include "internal/time_limit_activity_msg.hpp"
#include "logic/time_limit_activity_temp_mgr.hpp"
#include "internal/legion_station_msg.hpp"
#include "internal/pk_king_msg.hpp"
#include "internal/element_war_msg.hpp"
#include "internet/game.pb.h"
#include "element_war_def.hpp"
#include "internet/net.pb.h"
#include "internal/attack_city_msg.hpp"

namespace faith
{
	using namespace net;
	void ws2cs_req_reload_csv(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		cell_server::getInstance().set_reload_csv();
	}
	void ws2cs_req_reload_csv_lua()
	{
		ZoneScoped;
		cell_server::getInstance().set_reload_csv();
	}

	
	void ws2cs_req_del_unit(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2dp_gm_del_cache* packet = static_cast<const ws2dp_gm_del_cache*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2dp_gm_del_cache NULL == packet");
			return;
		}
		ws2cs_req_del_unit_lua(packet->role_guid.server_64);
	}
	void ws2cs_req_del_unit_lua(int64 role_id)
	{
		ZoneScoped;
		guid_64 role_guid;
		role_guid.server_64 = role_id;
		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid())
		{
			CONSOLE_ERROR("player is in game");
			player_ref.clear_saving_flag(e_save_data_type_exit_game);
			player_ref.save_role_all_data(e_save_data_type_exit_game);
			cs2ws_enter_game	rep;
			rep.client_uid = player_ref.get_client_uid();
			rep.char_info_to_ws.role_guid = role_guid;
			rep.map_guid = player_ref.get_map_guid();
			rep.eResult = cs2ws_enter_game::e_failed_repeat_player_id;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
		}
		unit_man::logout_player(role_guid, player_ref.get_array_index(), false, e_logout_result_kick_by_rpc);
	}
	void ws2cs_req_del_unit_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_del_unit_item* packet = static_cast<const ws2cs_del_unit_item*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_req_time_out NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->cs_unit_index);
		if (player_ref.is_valid())
		{
			for (auto i = packet->bag_slot_begin; i < packet->bag_slot_end; ++i)
			{
				item_system::clear_bag(&player_ref, e_bag_type(i));
			}
		}
	}
	
	void ws2cs_req_change_unit_vip(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_change_unit_vip* packet = static_cast<const ws2cs_change_unit_vip*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_ws2cs_req_change_unit_vip NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->cs_unit_index);
		if (player_ref.is_valid())
		{
			int32 new_value = player_ref.get_unit_info(e_role_info_recharge_num) + packet->vip_value;
			if (new_value < 0)
			{
				new_value = 0;
			}
			player_ref.gm_set_vip_point(new_value);
		}
	}
	void ws2cs_req_del_unit_specified_item(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_del_unit_specified_item* packet = static_cast<const ws2cs_del_unit_specified_item*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_req_time_out NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->cs_unit_index);
		if (player_ref.is_valid())
		{
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, packet->bag_type, packet->item_id);
		}
	}
	void ws2cs_req_add_guide_record(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_add_guide_record* packet = static_cast<const ws2cs_add_guide_record*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_req_time_out NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->cs_unit_index);
		if (player_ref.is_valid())
		{
			s_record_info record_info;
			record_info.guide_trigger_id = packet->guide_trigger_id;
			record_info.state = 1;
			player_ref.get_guide_mgr().save_guide_record(record_info);
		}
	}

	void ws2cs_req_time_out(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_time_out* packet = static_cast<const ws2cs_time_out*>(data_ptr);
		if (NULL == packet)
		{
			string str = " the packet is null";
			server_log::reconnect_game_log("cs", __FUNCTION__, str.c_str());
			return;
		}
		player& player_ref = unit_man::get_player(packet->cs_array_index);
		if (player_ref.is_valid())
		{
			player_ref.on_player_disconnect();
		}
	}
	void ws2cs_req_reconnect_game(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_reconnect_game* packet = static_cast<const ws2cs_reconnect_game*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_req_reconnect_game NULL == packet");
			return;
		}
		ws2cs_req_reconnect_game_lua(packet->client_uid.fep_uid_64, packet->role_guid.server_64, packet->cur_map_id, packet->cur_map_line_id);
		//cs2ws_reconnect_game rep;
		//rep.client_uid = packet->client_uid;
		//rep.role_guid = packet->role_guid;
		//player& player_ref = unit_man::get_player(packet->role_guid);
		//if (player_ref.is_valid())
		//{
		//	player_ref.on_player_reconnect(packet->client_uid, packet->cur_map_id, packet->cur_map_line_id);
		//	rep.reconnect_res = e_reconnect_result_win;
		//}
		//else
		//{
		//	rep.reconnect_res = e_reconnect_result_cs_no_find;
		//}
		//connection_mgr::getInstance().send_to_ws( &rep, sizeof(rep));
	}

	void ws2cs_req_reconnect_game_lua(int64 client_id, int64 role_id,int32 cur_map_id, int32 cur_map_line_id)
	{
		ZoneScoped;

		guid_64 role_guid(role_id);
		s_client_uid client_uid(client_id);

		cs2ws_reconnect_game rep;
		rep.client_uid = client_uid;
		rep.role_guid = role_guid;


		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid())
		{
			player_ref.on_player_reconnect(client_uid, cur_map_id, cur_map_line_id);
			rep.reconnect_res = e_reconnect_result_win;
		}
		else
		{
			rep.reconnect_res = e_reconnect_result_cs_no_find;
		}
		connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));

	}
	
	void ws2cs_req_enter_game(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_enter_game* packet = static_cast<const ws2cs_enter_game*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("NULL == packet");
			return;
		}
		if (init_unit::is_robot_account("hy", packet->account))
		{
			CONSOLE_INFO("robot time = {}", utility::get_tick_count());
		}

		CONSOLE_INFO("player guid:{}, map guid:{}",packet->role_guid.server_64, packet->map_guid.server_64);

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid())
		{
			CONSOLE_ERROR("ws2cs_req_enter_game repeat enter game guid = {} state = {}", packet->role_guid.server_64, (int32)player_ref.get_session_state());
			unit_man::logout_player(packet->role_guid, player_ref.get_array_index(), false, e_logout_result_save_data_fail);
			cs2ws_enter_game	rep;
			rep.client_uid = packet->client_uid;
			rep.char_info_to_ws.role_guid = packet->role_guid;
			rep.map_guid = packet->map_guid;
			rep.eResult = cs2ws_enter_game::e_failed_repeat_player_id;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			return;
		}
		auto map_ent = get_entity(packet->map_guid);
		if (map_ent == nullptr)
		{
			CONSOLE_ERROR("ws2cs_req_enter_game map_ent == NULL");
			cs2ws_enter_game	rep;
			rep.client_uid = packet->client_uid;
			rep.char_info_to_ws.role_guid = packet->role_guid;
			rep.map_guid = packet->map_guid;
			rep.eResult = cs2ws_enter_game::e_failed_join_map;
			connection_mgr::getInstance().send_to_ws(&rep, sizeof(rep));
			return;
		}
		player& empty_player = unit_man::get_empty_player(packet->role_guid);
		if (empty_player.is_valid() == false)
		{
			CONSOLE_ERROR("ws2cs_req_enter_game NULL == empty_player");
			cs2ws_enter_game	rep;
			rep.client_uid = packet->client_uid;
			rep.char_info_to_ws.role_guid = packet->role_guid;
			rep.map_guid = packet->map_guid;
			rep.eResult = cs2ws_enter_game::e_failed_repeat_player_id;
			connection_mgr::getInstance().send_to_ws( &rep, sizeof(rep));
			return;
		}
		empty_player.set_unit_info(e_role_info_server_id, packet->server_id);
		empty_player.init_enter_data(packet->login_type, packet->client_uid, packet->fixed_data);
		empty_player.set_map_ent(map_ent);
		empty_player.set_line_id(packet->line_id);
		empty_player.set_war_index(packet->war_index);
		empty_player.set_role_right(e_role_right_allow_chat, packet->is_send_chat);
		empty_player.set_couple_guid(packet->couple_guid);
		empty_player.set_couple_name(packet->couple_name);
		empty_player.get_legion_cs_mgr().set_legion_info(packet->legion_info);
		empty_player.get_team_cs_mgr().set_player_team_info(packet->team_info, true);
		empty_player.get_legion_cs_mgr().set_is_city_master(packet->bflag_loading_title[faith::e_title_type_legion_city_win_chief]);
		empty_player.get_legion_cs_mgr().set_is_win_city_war_member(packet->bflag_loading_title[faith::e_title_type_legion_city_win_member]);
		empty_player.set_is_already_login(packet->is_already_login);
		// 如果是PK之王就获得PK之王的称号,如果不是就剥夺PK之王的称号s 
		//如果是军团战胜利方成员,添加称号  
		//排行类称号   
		empty_player.get_title_mgr().add_loading_ranking_finish_title_type(packet->bflag_loading_title);

		skill_manager::skill_set_owner(empty_player.get_array_index());
		empty_player.set_login_time(packet->login_time);
		
		// 在加载role数据之前先设置account，否则加载错误时删除数据影响机器人计数
		empty_player.set_role_account(packet->account);

		empty_player.create_obj_lua();
		//上线加载数据
		empty_player.load_role_data_online(packet->role_guid);

		
	}

	void lua_ws2cs_req_enter_game(uint32 conn_index, const char *data_ptr, int32 data_len)
	{
		ZoneScoped;
		faith::ws2cs_proto::enter_game msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_enter_game packet;

		packet.from_proto(msg);
	

		ws2cs_req_enter_game(conn_index, &packet, sizeof(ws2cs_enter_game));
	}

	void ws2cs_req_clear_server_config(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_server_clear_all_config* packet = static_cast<const ws2cs_server_clear_all_config*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_req_enter_scene packet == NULL");
			return;
		}
		cell_server::getInstance().clear_all_server_config();
	}


	void ws2cs_req_server_config(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_server_config* packet = static_cast<const ws2cs_server_config*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_req_enter_scene packet == NULL");
			return;
		}
		if (packet->is_add)
		{
			cell_server::getInstance().set_server_config(packet->game_info);
		}
		else
		{
			cell_server::getInstance().remove_server_config(packet->game_info.server_id);
		}
	}

	void lua_ws2cs_req_server_config(uint32 conn_index, const char* data_ptr, int32 data_len)
	{
		ZoneScoped;
		faith::ws2cs_proto::server_config msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_server_config packet;

		packet.from_proto(msg);


		ws2cs_req_enter_game(conn_index, &packet, sizeof(ws2cs_server_config));
	}

	void ws2cs_req_enter_scene(uint32 conn_index,const void* data_ptr,size_t data_len)
	{
		ZoneScoped;
		const ws2cs_enter_scene* packet	= static_cast<const ws2cs_enter_scene*>(data_ptr);
		if ( NULL == packet )
		{
			CONSOLE_ERROR("ws2cs_req_enter_scene packet == NULL");
			return;
		}

		guid_64 role_guid = packet->role_guid;	
		player& player_ref = unit_man::get_player(role_guid);
		cs2ws_enter_scene rep2ws;
		rep2ws.role_guid = role_guid;
		if (player_ref.is_valid() == false)
		{
			CONSOLE_ERROR("ws2cs_req_enter_scene player_ref == NULL role_guid = {}", role_guid.server_64);
			rep2ws.eResult = cs2ws_enter_scene::e_enter_ret_player_err;
			connection_mgr::getInstance().send_to_ws(&rep2ws, sizeof(rep2ws));
			return;
		}

		rep2ws.eResult = (cs2ws_enter_scene::e_enter_ret)player_ref.put_in_scene();
		connection_mgr::getInstance().send_to_ws(&rep2ws, sizeof(rep2ws));
		if (cs2ws_enter_scene::e_enter_ret_success != rep2ws.eResult)
		{
			CONSOLE_ERROR("ws2cs_req_enter_scene put_in_scene fail. Ret = {}", (int32)rep2ws.eResult);
			return;
		}
		CONSOLE_INFO("map_guid:{} role_guid:{}", player_ref.get_map_guid().server_64, role_guid.server_64);

		world_cs::player_enter_scene(player_ref.get_map_ent(), &player_ref);
	}

	void ws2cs_req_logout_client(uint32 conn_index,const void* data_ptr,size_t data_len)
	{
		ZoneScoped;
		const ws2cs_client_logout* packet = static_cast<const ws2cs_client_logout*>(data_ptr);
		if ( NULL == packet )
		{
			return;
		}
		ws2cs_req_logout_client_lua(packet->role_guid.server_64, packet->need_send_save_end);
		//player& player_ref = unit_man::get_player(packet->role_guid);
		//if (player_ref.is_valid())
		//{
		//	player_ref.set_send_gate_save_end(packet->need_send_save_end);
		//	if (player_ref.is_dead())
		//	{
		//		player_ref.set_player_relive(0.5, false);
		//		s_map_pos temp_pos;
		//		s_map_pos cur_pos(player_ref.get_unit_info(e_role_info_main_pos_x), player_ref.get_unit_info(e_role_info_main_pos_y), player_ref.get_unit_info(e_role_info_main_pos_z), 0, 0, 0);
		//		player_ref.get_fuhuopos(temp_pos, player_ref.get_unit_info(e_role_info_main_map_id), cur_pos);
		//		player_ref.set_unit_info(e_role_info_move_map_id, player_ref.get_unit_info(e_role_info_main_map_id));
		//		player_ref.set_unit_info(e_role_info_move_pos_x, temp_pos.unit_location.x);
		//		player_ref.set_unit_info(e_role_info_move_pos_y, temp_pos.unit_location.y);
		//		player_ref.set_unit_info(e_role_info_move_pos_z, temp_pos.unit_location.z);
		//		player_ref.set_unit_info(e_role_info_move_pos_yaw, temp_pos.unit_rotation.yaw);
		//	}
		//	if (cell_server::getInstance().get_is_self_server(player_ref.get_unit_info(e_role_info_server_id)))
		//	{
		//		player_ref.save_role_all_data(e_save_data_type_exit_game);
		//	}
		//	else
		//	{
		//		player_ref.save_role_all_data(e_save_data_type_cross);
		//	}
		//}
		//else
		//{
		//	//unit_man::logout_player(packet->client_uid, packet->role_guid, false, e_logout_result_no_find_player);
		//}
	}
	void ws2cs_req_logout_client_lua(int64 role_id, bool need_send_save_end)
	{
		ZoneScoped;
		guid_64 role_guid(role_id);
		player& player_ref = unit_man::get_player(role_guid);
		if (player_ref.is_valid())
		{
			player_ref.set_send_gate_save_end(need_send_save_end);
			if (player_ref.is_dead())
			{
				player_ref.set_player_relive(0.5, false);
				//s_map_pos temp_pos;
				//s_map_pos cur_pos(player_ref.get_unit_info(e_role_info_main_pos_x), player_ref.get_unit_info(e_role_info_main_pos_y), player_ref.get_unit_info(e_role_info_main_pos_z), 0, 0, 0);
				//player_ref.get_fuhuopos(temp_pos, player_ref.get_unit_info(e_role_info_main_map_id), cur_pos);
				//player_ref.set_unit_info(e_role_info_move_map_id, player_ref.get_unit_info(e_role_info_main_map_id));
				//player_ref.set_unit_info(e_role_info_move_pos_x, player_ref.get_unit_info(e_role_info_main_pos_x));
				//player_ref.set_unit_info(e_role_info_move_pos_y, player_ref.get_unit_info(e_role_info_main_pos_y));
				//player_ref.set_unit_info(e_role_info_move_pos_z, player_ref.get_unit_info(e_role_info_main_pos_z));
				//player_ref.set_unit_info(e_role_info_move_pos_yaw, player_ref.get_unit_info(e_role_info_main_pos_yaw));
			}
			if (cell_server::getInstance().get_is_self_server(player_ref.get_unit_info(e_role_info_server_id)))
			{
				player_ref.save_role_all_data(e_save_data_type_exit_game);
				unit_man::logout_player(role_guid, player_ref.get_array_index(), false, e_logout_result_new_login);
			}
			else
			{
				player_ref.save_role_all_data(e_save_data_type_cross);
			}
		}
		else
		{
			//unit_man::logout_player(packet->client_uid, packet->role_guid, false, e_logout_result_no_find_player);
		}

	}
	void ws2cs_map_create_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_map_create* packet = static_cast<const ws2cs_map_create*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}

		if (data_len != sizeof(ws2cs_map_create))
		{
			return;
		}
		world_cs::create_map_by_template(packet->map_guid, packet->map_template_id, packet->line_id, packet->map_all_time);
	}
	void ws2cs_map_delete_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_map_delete* packet = static_cast<const ws2cs_map_delete*>(data_ptr);
		if (packet == NULL)
		{
			return;
		}

		if (data_len != sizeof(ws2cs_map_delete))
		{
			return;
		}
		world_cs::destroy_map(packet->map_guid);
	}

	void ws2cs_transfer_to_map_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_transfer_to_map* packet = static_cast<const ws2cs_transfer_to_map*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_transfer_to_map))
		{
			return;
		}

		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}

		int32 server_id = packet->server_id;
		int32 map_template_id = packet->map_template_id;
		guid_64 map_guid = packet->map_guid;
		int32 war_idx = packet->war_idx;
		player_ref.transfer_by_template(map_template_id, 0, server_id, map_guid, war_idx);
	}

	void ws2cs_get_map_info_from_cs_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		world_cs::ws2cs_get_map_info_from_cs_fun(conn_index, data_ptr, data_len);
	}
	void lua_ws2cs_get_map_info_from_cs_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		faith::ws2cs_proto::get_map_info_from_cs msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_get_map_info_from_cs packet;

		packet.from_proto(msg);

		ws2cs_get_map_info_from_cs_func(conn_index, &packet, sizeof(ws2cs_get_map_info_from_cs));
		 
	}

	void ws2cs_gm_begin_cross(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_server_need_cross* packet = static_cast<const ws2cs_server_need_cross*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_server_need_cross))
		{
			return;
		}
		cell_server::getInstance().set_need_begin_cross_gm_common(packet->gm_type, packet->need_begin_cross);
	}

	void ws2cs_server_info_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_server_info_arr* packet = static_cast<const ws2cs_server_info_arr*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_server_info_arr))
		{
			return;
		}
		cell_server::getInstance().set_server_info_arr(packet->server_info_type, packet->server_info_value);
	}

	void ws2cs_server_info_all_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_server_info_all* packet = static_cast<const ws2cs_server_info_all*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		for (int32 i = 0; i < e_server_info_type_max; i++)
		{
			cell_server::getInstance().set_server_info_arr(i, packet->server_info_arr[i]);
		}
	}

	void ws2cs_server_refresh_daily_info_at_zero_hour(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_server_is_need_refresh_cs_daily_info_at_zero_hour* packet = static_cast<const ws2cs_server_is_need_refresh_cs_daily_info_at_zero_hour*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_server_is_need_refresh_cs_daily_info_at_zero_hour))
		{
			return;
		}
		unit_man::refresh_kill_num_at_zero_hour();
	}

	void ws2cs_gm_open_time(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2dp_gm_open_time* packet = static_cast<const ws2dp_gm_open_time*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2dp_gm_open_time))
		{
			return;
		}
		cell_server::getInstance().set_open_time(packet->open_time);
	}

	//void ws2cs_gm_open_time_lua(int32 open_time)
	//{
	//	cell_server::getInstance().set_open_time(open_time);
	//}
	////////////////////////////////////////////////////////// activity ////////////////////////////////////////////////////////////

	void ws2cs_broken_sky_sync_creat_boss_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_broken_sky_creat_boss* packet = static_cast<const ws2cs_broken_sky_creat_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_broken_sky_creat_boss))
		{
			return;
		}

		ws2cs_broken_sky_sync_creat_boss_func_lua(packet->boss_level, packet->all_damage, packet->from_map_guid.server_64);
		/*int32	boss_level = packet->boss_level;
		int64	all_damage = packet->all_damage;
		guid_64 from_map_guid = packet->from_map_guid;



		auto map_ptr = world_cs::get_map_by_guid<map_object>(from_map_guid);

		if (nullptr == map_ptr || map_ptr->get_map_type() != e_map_type_broken_sky)
		{
			return;
		}
		broken_sky_map_cs* broken_sky_map_cs_ptr = broken_sky_map_cs::cast(map_ptr);
		if (nullptr == broken_sky_map_cs_ptr)
		{
			return;
		}
		broken_sky_map_cs_ptr->sync_creat_boss_end(boss_level, all_damage);*/
	}

	void ws2cs_broken_sky_sync_creat_boss_func_lua(int32 in_boss_level, int64 in_all_damage, int64 in_from_map_guid)
	{
		ZoneScoped;
		int32	boss_level = in_boss_level;
		int64	all_damage = in_all_damage;
		guid_64 from_map_guid = in_from_map_guid;

		//auto map_ptr = world_cs::get_map_by_guid<broken_sky_map_cs>(from_map_guid);

		//if (nullptr == map_ptr || map_ptr->get_map_type() != e_map_type_broken_sky)
		//{
		//	return;
		//}
		//map_ptr->sync_creat_boss_end(boss_level, all_damage);

	}

	void ws2cs_broken_sky_sync_game_info_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_broken_sky_info_sync* packet = static_cast<const ws2cs_broken_sky_info_sync*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_broken_sky_info_sync))
		{
			return;
		}
		ws2cs_broken_sky_sync_game_info_func_lua(packet->map_template_id, packet->broken_sky_inst_guid.server_64, packet->damage);
		/*int32 map_template_id = packet->map_template_id;
		guid_64 from_map_guid = packet->broken_sky_inst_guid;
		int64 damage = packet->damage;

		std::vector<map_object*> map_set;
		world_cs::get_all_map_by_template_id(map_set, map_template_id);
		for (map_object* map_ptr : map_set)
		{
			if (nullptr == map_ptr || map_ptr->get_map_type() != e_map_type_broken_sky)
			{
				continue;
			}
			broken_sky_map_cs* broken_sky_map_cs_ptr = broken_sky_map_cs::cast(map_ptr);
			if (nullptr == broken_sky_map_cs_ptr)
			{
				continue;
			}
			broken_sky_map_cs_ptr->sync_damage_from_other_map_inst(map_template_id, from_map_guid, damage);
		}*/
	}

	void ws2cs_broken_sky_sync_game_info_func_lua(int32 in_map_template_id, int64 in_from_map_guid, int64 in_damage)
	{
		ZoneScoped;
	

		int32 map_template_id = in_map_template_id;
		guid_64 from_map_guid = in_from_map_guid;
		int64 damage = in_damage;

		//std::vector<broken_sky_map_cs*> map_set;
		//world_cs::get_all_map_by_template_id<broken_sky_map_cs>(map_set, map_template_id);
		//for (auto map_ptr : map_set)
		//{
		//	map_ptr->sync_damage_from_other_map_inst(map_template_id, from_map_guid, damage);
		//}
	}

	void ws2cs_broken_sky_sync_kill_boss_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_broken_sky_kill_boss* packet = static_cast<const ws2cs_broken_sky_kill_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_broken_sky_kill_boss))
		{
			return;
		}

		ws2cs_broken_sky_sync_kill_boss_func_lua(packet->map_template_id, packet->killer_guid.server_64, packet->killer_unit_index);
	}

	void ws2cs_broken_sky_sync_kill_boss_func_lua(int32 in_map_template_id,int64 in_killer_guid, int32 in_killer_unit_index)
	{
		ZoneScoped;

		int32 map_template_id = in_map_template_id;
		guid_64 killer_guid = in_killer_guid;
		int32 killer_unit_index = in_killer_unit_index;

		//std::vector<broken_sky_map_cs*> map_set;
		//world_cs::get_all_map_by_template_id<broken_sky_map_cs>(map_set, map_template_id);
		//for (auto map_ptr : map_set)
		//{
		//	map_ptr->sync_boss_dead_from_ws(map_template_id, killer_unit_index, killer_guid);
		//}
	}

	void ws2cs_clear_big_player_title_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_clear_big_player_title* packet = static_cast<const ws2cs_clear_big_player_title*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_clear_big_player_title))
		{
			return;
		}
		guid_64 role_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.change_big_player_title(packet->title_type);
	}

	void ws2cs_chest_arrival_spawn_chests_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_chest_arrivai_spawn_chest_wave* packet = static_cast<const ws2cs_chest_arrivai_spawn_chest_wave*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_chest_arrivai_spawn_chest_wave))
		{
			return;
		}
		ws2cs_chest_arrival_spawn_chests_func_lua(packet->chest_disappear_time);

		/*	const int32 chest_disappear_time = packet->chest_disappear_time;
			const int32 cur_time = time_helper::get_today_time_in_sec();
			const int32 left_time = chest_disappear_time - cur_time;
			if (left_time <= 0)
			{
				return;
			}
			int64 disappear_time = init_unit::get_end_time(left_time);

			std::vector<map_object*> map_cs_arr;
			world_cs::get_all_map_by_type(map_cs_arr, faith::e_map_type_big_map);
			for (const map_object* map_obj_ptr : map_cs_arr)
			{
				if (nullptr == map_obj_ptr)
				{
					continue;
				}
				map_cs* map_cs_ptr = (map_cs*)map_obj_ptr;

				int32 spawn_id = chest_arrival_activity::spawn_chest_begin_template_id;
				int32 next_spawn_id = 0;
				while (true)
				{
					NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_id);
					if (nullptr == npc_spawn_point_template_ptr)
					{
						break;
					}
					next_spawn_id = npc_spawn_point_template_ptr->NextSpawnNpcId;
					npc* spawn_npc_ptr = map_cs_ptr->spawn_npc(spawn_id);
					if (nullptr != spawn_npc_ptr)
					{
						spawn_npc_ptr->set_live_time(disappear_time);
					}
					if (next_spawn_id <= spawn_id)
					{
						break;
					}
					spawn_id = next_spawn_id;
				}
			}*/
	}


	void ws2cs_chest_arrival_spawn_chests_func_lua(int32 in_chest_disappear_time)
	{
		ZoneScoped;

		const int32 chest_disappear_time = in_chest_disappear_time;
		const int32 cur_time = time_helper::get_today_time_in_sec();
		const int32 left_time = chest_disappear_time - cur_time;
		if (left_time <= 0)
		{
			return;
		}
		int64 disappear_time = init_unit::get_end_time(left_time);

		std::vector<map_cs*> map_cs_arr;
		world_cs::get_all_map_by_type<map_cs>(map_cs_arr, faith::e_map_type_big_map);
		for (auto map_cs_ptr : map_cs_arr)
		{
			int32 spawn_id = chest_arrival_activity::spawn_chest_begin_template_id;
			int32 next_spawn_id = 0;
			while (true)
			{
				NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, spawn_id);
				if (nullptr == npc_spawn_point_template_ptr)
				{
					break;
				}
				next_spawn_id = npc_spawn_point_template_ptr->NextSpawnNpcId;
				npc* spawn_npc_ptr = map_cs_ptr->spawn_npc(spawn_id);
				if (nullptr != spawn_npc_ptr)
				{
					spawn_npc_ptr->set_live_time(disappear_time);
				}
				if (next_spawn_id <= spawn_id)
				{
					break;
				}
				spawn_id = next_spawn_id;
			}
		}
	}

	////////////////////////////////////////////////////////// team ////////////////////////////////////////////////////////////
	
	void ws2cs_apply_to_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		team_cs_mgr::ws2cs_apply_to_join_team_func(conn_index, data_ptr, data_len);
	}

	void ws2cs_set_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		team_cs_mgr::ws2cs_set_player_team_info_func(conn_index, data_ptr, data_len);
	}

	void ws2cs_clear_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		team_cs_mgr::ws2cs_clear_player_team_info_func(conn_index, data_ptr, data_len);
	}

	void ws2cs_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
	}

	void ws2cs_transfer_team_member_to_scene_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		team_cs_mgr::ws2cs_transfer_team_member_to_scene_func(conn_index, data_ptr, data_len);
	}
	
	void ws2cs_team_common_check_in_cs_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		team_cs_mgr::ws2cs_team_common_check_in_cs_func(conn_index, data_ptr, data_len);
	}
	
	void ws2cs_sync_team_start_aim_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		team_cs_mgr::ws2cs_sync_team_start_aim_func(conn_index, data_ptr, data_len);
	}

	////////////////////////////////////////////////////////// legion ////////////////////////////////////////////////////////////

	void ws2cs_set_player_legion_guid_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_set_player_legion_info* packet = static_cast<const ws2cs_set_player_legion_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_set_player_legion_info))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.get_legion_cs_mgr().ws2cs_set_legion_info(data_ptr);
		if (true == packet->is_new_join_legion)
		{
			player_ref.get_achievement_mgr().create_legion();
			player_ref.get_mission_mgr().target_check(e_mission_end_type_join_legion);
			if (player_ref.get_logic_data(e_role_logic_info_first_join_legion_award) <= 0)
			{
				player_ref.set_logic_data(e_role_logic_info_first_join_legion_award, 1);
				ActivityCommonConfigTemplate* act_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_legion_boss);
				if (nullptr == act_cfg_ptr)
				{
					return;
				}
				std::vector<int32> item_arr = act_cfg_ptr->ParamIntArr2;
				if (item_arr.size() <= 0 || item_arr.size() % 2 != 0)
				{
					return;
				}
				std::vector<s_item_template_info> item_list;
				item_list.clear();
				for (int32 i = 0; i < item_arr.size() / 2; i++)
				{
					item_list.push_back({ item_arr[i] ,item_arr[i + 1] });
				}
				globle_data::get_instance().send_mail_with_item(packet->role_guid, 0, item_list, "90303046", "90303047");
			}
		}
	}
	void lua_ws2cs_set_player_legion_guid_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		faith::ws2cs_proto::set_player_legion_info msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		ws2cs_set_player_legion_info packet;

		packet.from_proto(msg);

		ws2cs_set_player_legion_guid_func(conn_index, &packet, sizeof(ws2cs_set_player_legion_info));
		
	}

	void ws2cs_clear_player_legion_info_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_clear_player_legion_info* packet = static_cast<const ws2cs_clear_player_legion_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_clear_player_legion_info))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		
		player_ref.get_legion_cs_mgr().ws2cs_clear_legion_info();
	}

	void ws2cs_create_legion_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_create_legion_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_legion_donate_money_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_legion_donate_money_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_legion_donate_item_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_legion_donate_item_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_enter_legion_boss_map_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_enter_legion_boss_map_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_get_legion_boss_award_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_get_legion_boss_award_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_set_get_legion_boss_award_log_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_set_get_legion_boss_award_log_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_fire_bonfire_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_fire_bonfire_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_add_fuel_to_bonfire_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_add_fuel_to_bonfire_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_legion_rm_recruit_cd_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_legion_rm_recruit_cd_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_draw_bonfire_daily_rwd_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_draw_bonfire_daily_rwd_fun(conn_index, data_ptr, data_len);
	}


	void ws2cs_level_up_legion_skill_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_level_up_legion_skill* packet = static_cast<const ws2cs_level_up_legion_skill*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_level_up_legion_skill))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
	
		player_ref.get_legion_skill().level_up_legion_skill_func(data_ptr);
	}

	void ws2cs_legion_warehouse_operation(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_operation_legion_warehouse_item(conn_index, data_ptr, data_len);
	}

	void ws2cs_confirm_change_legion_name_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_confirm_change_legion_name_func(conn_index, data_ptr, data_len);
	}

	void ws2cs_rem_item_change_legion_name_care_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_rem_item_change_legion_name_care_func(conn_index, data_ptr, data_len);
	}
	void ws2cs_change_gate_legion_name_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_change_gate_legion_name_func(conn_index, data_ptr, data_len);
	}
	void ws2cs_begin_city_war_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_begin_city_war_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_enter_city_war_map_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_enter_city_war_map_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_city_war_operate_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_city_war_operate_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_get_construction_buff_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_get_construction_buff_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_get_occupation_daily_award_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_get_occupation_daily_award_fun(conn_index, data_ptr, data_len);
	}
	void ws2cs_get_bonus_award_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_get_bonus_award_fun(conn_index, data_ptr, data_len);
	}
	void ws2cs_get_city_war_master_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		legion_cs_mgr::ws2cs_get_city_war_master_stuff_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_del_city_war_master_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_del_city_war_master_stuff_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_add_city_war_winner_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_add_city_war_winner_stuff_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_del_city_war_winner_stuff_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_del_city_war_winner_stuff_fun(conn_index, data_ptr, data_len);
	}

	void ws2cs_answer_question_right(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		legion_cs_mgr::ws2cs_answer_question_right_func(conn_index, data_ptr, data_len);
	}


	void ws2cs_receive_mail(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_send_mail* packet = static_cast<const ws2cs_send_mail*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_send_mail))
		{
			return;
		}

		player& receive_player = unit_man::get_player(packet->addressee_guid);
		if (false == receive_player.is_valid())
		{
			return;
		}
		std::vector<s_item_info> temp_item_list;
		for (int32 i = 0; i < packet->item_num; ++i)
		{
			temp_item_list.push_back(packet->item_list[i]);
		}

		//此处只是为了让可以即时收到邮件的玩家收件（及操作），当定时更新时，还是以数据库为准
		receive_player.get_mail_mgr().receive_mail(packet->mail_info, temp_item_list);
	}

	void lua_ws2cs_receive_mail(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_receive_mail, send_mail)
	}

	void ws2cs_receive_notice_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_receive_notice* packet = static_cast<const ws2cs_receive_notice*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_receive_notice))
		{
			return;
		}
		player& receive_player = unit_man::get_player(packet->addressee_guid);
		guid_64 sender_guid = packet->sender_guid;
		int32 crate_time = packet->create_time;
		receive_player.get_chat_mgr().receive_notice(packet->notice_id, sender_guid, crate_time);
	}
	
	void ws2cs_rep_get_rank(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_rep_get_arena_rank* packet = static_cast<const ws2cs_rep_get_arena_rank*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_rep_get_arena_rank))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_arena_mgr().get_arena_rank_end(packet->arena_rank, packet->is_request);
	}
	void ws2cs_res_challenge(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_req_challenge_msg* packet = static_cast<const ws2cs_req_challenge_msg*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_req_challenge_msg))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_arena_mgr().do_challenge(packet->target_guid, packet->robot_id, packet->req_result
												, packet->cost_money_type, packet->cost_money_value
												, packet->target_cur_pos, packet->self_cur_pos);
	}

	void ws2cs_get_choose_ist(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_req_choose_list* packet = static_cast<const ws2cs_req_choose_list*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_req_choose_list))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_arena_mgr().req_match_player_end(packet->target_player);
	}
	
	void ws2cs_resp_fresh_challenge_time(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_fresh_challenge_time* packet = static_cast<const ws2cs_fresh_challenge_time*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_fresh_challenge_time))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
	}
	 

	void 	ws2cs_worship_player_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_req_worship_player_end* packet = static_cast<const ws2cs_req_worship_player_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_req_worship_player_end))
		{
			return;
		}

		//更新被崇拜玩家的崇拜次数
		player& target_player_ref = unit_man::get_player(packet->target_guid);
		if (true == target_player_ref.is_valid())
		{
			target_player_ref.get_ranking_mgr().set_worship_value(packet->target_worship_value);
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_worship_target().add_new_record(packet->target_guid, packet->ranking_type);
		player_ref.get_worship_target().req_worship_player_end(e_RankingIndex(packet->ranking_type), packet->worship_type, packet->target_guid, packet->target_worship_value, packet->worship_result);
	}

	void ws2cs_req_sync_first_rank_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_sync_first_rank_end* packet = static_cast<const ws2cs_sync_first_rank_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_sync_first_rank_end))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid() || false == player_ref.is_self_server())
		{
			return;
		}
		player_ref.get_title_mgr().ranking_first_changed(packet->ranking_type, packet->is_first, packet->is_notice);
	}

	void ws2cs_req_sync_server_avg_lv(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_sync_server_avg_lv* packet = static_cast<const ws2cs_sync_server_avg_lv*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_sync_server_avg_lv))
		{
			return;
		}
		if (packet->srv_avg_lv >= 0)
		{
			cell_server::getInstance().set_server_avg_level(packet->srv_avg_lv);
		}
		if (packet->world_lv_cur >= 0)
		{
			cell_server::getInstance().set_world_level_cur(packet->world_lv_cur);
		}
		if (packet->world_lv_last >= 0)
		{
			cell_server::getInstance().set_world_level_last(packet->world_lv_last);
		}
	}

	void ws2cs_req_service_rank_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_req_service_rank_info_end* packet = static_cast<const ws2cs_req_service_rank_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_req_service_rank_info_end))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->role_guid);
		if (false == main_player_ref.is_valid())
		{
			return;
		}
		main_player_ref.get_service_rank_mgr().receive_reward_by_type_end(packet->rank_type, packet->rank_num);
	}
	
	void ws2cs_marry_check_item_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_check_item_enough* packet = static_cast<const ws2cs_check_item_enough*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_check_item_enough))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->main_guid);
		if (false == main_player_ref.is_valid())
		{
			return;
		}
		if (!packet->is_divorce)
		{
			main_player_ref.check_can_marry(packet->sub_guid, packet->item_id, packet->is_cost_item);
		}
		else
		{
			main_player_ref.check_can_divorce( /*packet->item_id,*/ packet->is_cost_item);
		}
	}

	void ws2cs_marry_result_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_marry_result_end* packet = static_cast<const ws2cs_marry_result_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_marry_result_end))
		{
			return;
		} 

		if (e_marry_operate_type_marry_success == packet->result_type)
		{
			player& main_player_ref = unit_man::get_player(packet->main_guid);
			if (main_player_ref.is_valid())
			{
				main_player_ref.handle_marry_result_func(packet->sub_guid, packet->sub_name ,packet->is_marry, packet->marry_item_id);
			}
			player& sub_player_ref = unit_man::get_player(packet->sub_guid);
			if (sub_player_ref.is_valid())
			{
				sub_player_ref.handle_marry_result_func(packet->main_guid, packet->main_name,packet->is_marry, packet->marry_item_id);
			}
			if (packet->is_marry)
			{
				world_cs::spawn_npc(GAMECONFIG->MarryDisplayNpcId, main_player_ref.get_new_map_pos(), main_player_ref.get_map_ent());
			}	
		}
	}

	void lua_ws2cs_marry_result_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_marry_result_end_proc, marry_result_end);
	}

	void ws2cs_marry_get_reward_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_marry_get_reward* packet = static_cast<const ws2cs_marry_get_reward*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_marry_get_reward))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->sender_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		main_player_ref.get_new_married_reward_end(packet->old_marry_state);
	}
	void ws2cs_marry_get_marry_task_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_marry_get_marry_task* packet = static_cast<const ws2cs_marry_get_marry_task*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->role_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		bool is_receive = main_player_ref.get_mission_mgr().random_marry_mission(true);
		if (is_receive)
		{
			//接取成功提示
			const std::string& notice_str = template_manager::get_instance().get_str_by_string_template_id(90096815);
			main_player_ref.send_notice(notice_str);
		}
	}
	void ws2cs_marry_sync_marry_task(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_sync_marry_task* packet = static_cast<const ws2cs_sync_marry_task*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->role_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		if (!packet->is_marry)
		{
			main_player_ref.get_mission_mgr().clear_marry_mission();
		}
	}
	void ws2cs_update_wedding_ring_state_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_update_wedding_ring_state* packet = static_cast<const ws2cs_update_wedding_ring_state*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_update_wedding_ring_state))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->role_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		main_player_ref.get_marry_mgr().refresh_wedding_ring_info(packet->is_marry, packet->marry_item_id, packet->mate_ring_level, packet->heart_value);
	}
	void ws2cs_update_wedding_ring_level_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_update_wedding_ring_level* packet = static_cast<const ws2cs_update_wedding_ring_level*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_update_wedding_ring_level))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->role_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		main_player_ref.get_marry_mgr().refresh_mate_wedding_ring_level(packet->mate_ring_level);
	}

	void ws2cs_refresh_heart_value_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_refresh_heart_value* packet = static_cast<const ws2cs_refresh_heart_value*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_refresh_heart_value))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->role_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		main_player_ref.get_marry_mgr().refresh_heart_value(packet->heart_value);
	}

	void ws2cs_req_cost_bless_money_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_req_cost_bless_money* packet = static_cast<const ws2cs_req_cost_bless_money*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_req_cost_bless_money))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->role_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		main_player_ref.get_marry_mgr().cost_bless_money(packet->target_guid);
	}

	void ws2cs_return_special_item_times_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_return_special_item_times* packet = static_cast<const ws2cs_return_special_item_times*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_return_special_item_times))
		{
			return;
		}
		player& main_player_ref = unit_man::get_player(packet->target_guid);
		if (main_player_ref.is_valid() == false)
		{
			return;
		}
		int32 has_use_time = main_player_ref.get_logic_data(e_role_logic_info_daily_lucky_times);
		if (has_use_time == 0)
			return;
		main_player_ref.set_logic_data(e_role_logic_info_daily_lucky_times, has_use_time - 1);
		main_player_ref.send_logic_one(e_role_logic_info_daily_lucky_times);
	}

	void ws2cs_refresh_globel_mail_sign_state(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_had_new_globel_mail* packet = static_cast<const ws2cs_had_new_globel_mail*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_had_new_globel_mail))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_mail_mgr().load_had_globel_mail_guid_end(packet->data_info);
	}

	void lua_ws2cs_refresh_globel_mail_sign_state(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_refresh_globel_mail_sign_state, had_new_globel_mail)
	}

	void ws2cs_send_chat_end_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_send_chat_end* packet = static_cast<const ws2cs_send_chat_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_send_chat_end))
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->sender_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_chat_mgr().send_failed_process(packet->send_result, packet->chat_type);

	}

	void ws2cs_create_world_boss_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_create_world_boss* packet = static_cast<const ws2cs_create_world_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_create_world_boss))
		{
			return;
		}

		if (GAMECONFIG->WorldBossRandomPosArr.size() < 54)
		{
			return;
		}

		int32 npc_spawn_template_id = packet->world_boss_spawn_template_id;
		int32 world_boss_pos_id = packet->world_boss_random_pos_arr_id;
		if (world_boss_pos_id * 6 + 5 >= 54)
		{
			return;
		}

		fvector location(GAMECONFIG->WorldBossRandomPosArr[world_boss_pos_id * 6], GAMECONFIG->WorldBossRandomPosArr[world_boss_pos_id * 6 + 1], GAMECONFIG->WorldBossRandomPosArr[world_boss_pos_id * 6 + 2]);
		frotator rotation(GAMECONFIG->WorldBossRandomPosArr[world_boss_pos_id * 6 + 3], GAMECONFIG->WorldBossRandomPosArr[world_boss_pos_id * 6 + 4], GAMECONFIG->WorldBossRandomPosArr[world_boss_pos_id * 6 + 5]);
		s_map_pos bornPos(location, rotation);

		NpcSpawnPointTemplate* npc_spawn_point_template_ptr = GET_TEMPLATE(NpcSpawnPointTemplate, npc_spawn_template_id);
		if (npc_spawn_point_template_ptr == nullptr)
		{
			return;
		}
		int32 map_template_id = npc_spawn_point_template_ptr->MapId;
		auto map_obj_ptr = world_cs::get_map_by_template_id<map_object>(map_template_id);
		if (map_obj_ptr == nullptr)
		{
			return;
		}
		int32 dynamic_lv = world_cs::get_npc_init_level(npc_spawn_point_template_ptr->TemplateId, *map_obj_ptr);

		npc* temp_npc = world_cs::spawn_npc(npc_spawn_point_template_ptr->TemplateId, bornPos, get_entity(map_obj_ptr->get_map_guid()), nullptr, npc_spawn_template_id);
		if (temp_npc != nullptr)
		{
			cell_server::getInstance().set_world_boss_guid(temp_npc->get_unit_guid());
			cs2ws_create_world_boss_success success_msg;
			success_msg.world_boss_spawn_template_id = npc_spawn_template_id;
			success_msg.world_boss_npc_array_index = temp_npc->get_array_index();
			success_msg.world_boss_line_id = temp_npc->get_line_id();
			success_msg.world_boss_guid = temp_npc->get_unit_guid();
			connection_mgr::getInstance().send_to_ws(&success_msg, sizeof(cs2ws_create_world_boss_success));
			//if (temp_npc->get_npc_template()->NpcType!=3)
			//{
				//return;
			//}//不为世界boss的话就return
			if(temp_npc->get_npc_template() != nullptr
				&& temp_npc->get_npc_template()->CreatingNotice.size() > 0)
			{
				int32 size_CreatingNotice = temp_npc->get_npc_template()->CreatingNotice.size();
				int32 notice_index = random_gen::get_random(0, size_CreatingNotice - 1);
				if (notice_index >= 0  	&& notice_index < temp_npc->get_npc_template()->CreatingNotice.size())
				{
					std::string sub_param_array[2];
					std::stringstream temp_stream;
					temp_stream << temp_npc->get_npc_template()->NpcName;
					temp_stream >> sub_param_array[0];
					temp_stream.clear();
					temp_stream << temp_npc->get_line_id();
					temp_stream >> sub_param_array[1];
					temp_npc->npc_send_notice_with_location(temp_npc->get_npc_template()->CreatingNotice[notice_index], sub_param_array, 2, temp_npc->get_npc_template()->NpcName);
				}
			}
		}
	}
	void lua_ws2cs_create_world_boss_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_create_world_boss_proc, create_world_boss);
	}
	void ws2cs_remove_world_boss_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_remove_world_boss* packet = static_cast<const ws2cs_remove_world_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_remove_world_boss))
		{
			return;
		}
		int32 world_boss_array_index = packet->world_boss_npc_array_index;
		npc& world_boss_npc = unit_man::get_npc(world_boss_array_index);
		if (false == world_boss_npc.is_valid())
		{
			return;
		}
		if (world_boss_npc.get_unit_guid() != packet->world_boss_guid)
		{
			return;
		}	
		//if (world_boss_npc.get_spawn_point_id() >= first_boss_ready_npc_id&&world_boss_npc.get_spawn_point_id() <= end_boss_ready_npc_id)
		//{
		//	unit_man::remove_npc(world_boss_array_index, true);
		//	return;
		//}
		/*
		if (true == world_boss_npc.get_ai_unit().is_war_state())
		{
			buff_man::add_buff_inst(world_boss_npc.m_array_index, world_boss_npc.m_array_index, boss_rage_buff_id);
			return;
		}
		*/
		cell_server::getInstance().set_world_boss_guid(guid_64());
		unit_man::remove_npc(world_boss_array_index, false);
	}

	void lua_ws2cs_remove_world_boss_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_remove_world_boss_proc, remove_world_boss);
	}
	
	void ws2cs_create_first_kill_world_boss_prize(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_create_first_kill_prize* packet = static_cast<const ws2cs_create_first_kill_prize*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_create_first_kill_prize))
		{
			return;
		}
		player& self_player = unit_man::get_player(packet->first_killer_guid);
		if (false == self_player.is_valid())
		{
			return;
		}

		self_player.get_first_kill_world_boss_prize_func(packet->first_kill_welfare_template_id);
	}

	void ws2cs_create_big_player_statue_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_create_big_player_statue* packet = static_cast<const ws2cs_create_big_player_statue*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_create_big_player_statue))
		{
			return;
		}

		world_cs::create_big_player_statue(packet->big_player_info, packet->big_type, packet->map_guid);
	}

	void ws2cs_role_info_change_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_role_info_change* packet = static_cast<const ws2cs_role_info_change*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_role_info_change))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		e_role_info role_type = (e_role_info)packet->role_type;
		int32 role_value = packet->role_value;

		player_ref.set_unit_info(role_type, role_value);
	}

	void ws2cs_role_right_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_role_right* packet = static_cast<const ws2cs_role_right*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_role_right))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.set_role_right(packet->right_type, packet->right_value);
	}
	void ws2cs_recharge_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_recharge* packet = static_cast<const ws2cs_recharge*>(data_ptr);
		cs2ws_recharge_end msg;
		if (nullptr == packet)
		{
			msg.result = e_recharge_error_ws2cs_recharge_proc_packet_is_null;
			connection_mgr::getInstance().send_to_ws( &msg, sizeof(cs2ws_recharge_end));
			return;
		}
		if (data_len != sizeof(ws2cs_recharge))
		{
			msg.result = e_recharge_error_ws2cs_recharge_proc_data_len_error;
			connection_mgr::getInstance().send_to_ws( &msg, sizeof(cs2ws_recharge_end));
			return;
		}
		msg.order_num = packet->order_num;
		xstring order_id_str = xstring(packet->order_id);

		player& cur_player = unit_man::get_player(packet->role_guid);
		server_log::recharge_step_log(order_id_str, packet->role_guid, packet->goods_id, e_recharge_step_ws_to_cs_send_recharge_end, cur_player.get_third_info(), cur_player.get_login_type(), &cur_player.m_role_info);

		if (cur_player.is_valid() == false)
		{
			cs2ws_recharge_end_send_mail send_mail_msg;
			memcpy(send_mail_msg.order_id, order_id_str.c_str(), order_id_str.size());
			send_mail_msg.role_guid = packet->role_guid;
			send_mail_msg.goods_id = packet->goods_id;
			send_mail_msg.pay_price = packet->pay_price;
			send_mail_msg.direct_diamond = packet->direct_diamond;
			send_mail_msg.order_num = packet->order_num;
			send_mail_msg.payment_type = packet->payment_type;
			connection_mgr::getInstance().send_to_ws( &send_mail_msg, sizeof(cs2ws_recharge_end_send_mail));

			//msg.result = e_recharge_error_ws2cs_recharge_proc_player_not_valid;
			//connection_mgr::getInstance().send_to_all_ws( &msg, sizeof(cs2ws_recharge_end));
			//server_log::recharge_error_log(packet->order_id, packet->role_guid, packet->goods_id, e_recharge_error_ws2cs_recharge_proc_player_not_valid);
			return;
		}

		if (packet->goods_id > 0)
		{
			cur_player.recharge_by_template_id(packet->goods_id, packet->order_id, packet->order_num, packet->payment_type, packet->pay_price);
		}
		else if(packet->direct_diamond > 0)
		{
			std::vector<int32> money_array;
			money_array.push_back(e_money_type_jewel);
			money_array.push_back(packet->direct_diamond);
			cur_player.do_add_vip(money_array, packet->direct_diamond, 0, packet->order_id, packet->pay_price);
		}
		else
		{
			xstring order_id_str = packet->order_id;

			cs2ws_recharge_end send_msg;
			int32 order_id_len = order_id_str.length();
			if (order_id_len > max_recharge_order_length)
			{
				send_msg.result = e_recharge_error_db_save_recharge_order_id_len_bigger;
				connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(cs2ws_recharge_end));
				server_log::recharge_error_log("", packet->role_guid, packet->goods_id, e_recharge_error_db_save_recharge_order_id_len_bigger);
				return;
			}
			memcpy(send_msg.order_id, order_id_str.c_str(), order_id_len);
			send_msg.order_num = packet->order_num;
			send_msg.payment_type = packet->payment_type;
			send_msg.role_guid = packet->role_guid;
			send_msg.goods_id = packet->goods_id;
			send_msg.result = e_recharge_error_recharge_by_template_id_recharge_template_ptr_is_null;
			connection_mgr::getInstance().send_to_ws(&send_msg, sizeof(cs2ws_recharge_end));
		}
	}

	void lua_ws2cs_recharge_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_recharge_proc, recharge);
	}

	void ws2cs_role_buy_good(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_buy_good* packet = static_cast<const ws2cs_buy_good*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_buy_good))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.buy_goods(packet->goods_id, packet->goods_num, packet->store_id, packet->is_auto_buy, packet->back_string);
	}

	void ws2cs_get_red_pack_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_get_red_package_end* packet = static_cast<const ws2cs_get_red_package_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_get_red_package_end))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->receiver_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.get_red_pack_end(packet->red_package_guid, packet->get_result, packet->money_type, packet->money_num);
	}

	void ws2cs_send_red_pack_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_send_red_package_end* packet = static_cast<const ws2cs_send_red_package_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_send_red_package_end))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->sender_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.send_red_package_end_proc(packet->red_package_template_id, packet->send_result);
	}

	void ws2cs_full_friend_guid_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_full_friend_guid* packet = static_cast<const ws2cs_full_friend_guid*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->player_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		int32 friend_num = packet->friend_num;
		player_ref.upgrade_highest_record(e_role_history_highest_record_higest_friend_num, friend_num);
		player_ref.upgrade_highest_record(e_role_history_highest_record_higest_friendliness_value, packet->highest_firendliness_value);
		player_ref.refresh_service_goal(e_service_goal_type_friendliness);
		player_ref.get_achievement_mgr().player_changed(eat_add_friend_count);
	}

	void ws2cs_req_relation_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_req_relation_end* packet = static_cast<const ws2cs_req_relation_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->self_index);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.req_relation_end(packet->relation_type, packet->target_guid);
	}

	void ws2cs_auction_purchase_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_auction_purchase_success* packet = static_cast<const ws2cs_auction_purchase_success*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			cs2ws_auction_add_item_into_db sell_msg;
			sell_msg.auction_info = packet->auction_info;
			connection_mgr::getInstance().send_to_ws( &sell_msg, sizeof(cs2ws_auction_add_item_into_db));
			return;
		}
		player_ref.get_auction_mgr().buy_item_end(packet->auction_info);
	}

	void lua_ws2cs_auction_purchase_success_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_auction_purchase_success_proc, auction_purchase_success);
	}


	void ws2cs_auction_sell_success_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_auction_send_sell_success_info* packet = static_cast<const ws2cs_auction_send_sell_success_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->auction_info.seller_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.get_auction_mgr().sell_item_success(packet->auction_info);
	}

	void lua_ws2cs_auction_sell_success_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_auction_sell_success_proc, auction_send_sell_success_info);
	}

	void ws2cs_auction_cancel_sell_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_auction_cancel_sell* packet = static_cast<const ws2cs_auction_cancel_sell*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.get_auction_mgr().cancel_sell_item_end(packet->auction_info);
	}
	void lua_ws2cs_auction_cancel_sell_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_auction_cancel_sell_end_proc, auction_cancel_sell);
	}
	void ws2cs_bid_replace_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_auction_bid_replace_info *packet = static_cast<const ws2cs_auction_bid_replace_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		//player_ref.get_auction_mgr().sync_bid_record_failure(packet->auction_replace_info);

	}
	void ws2cs_time_out_del_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_auction_time_out_del *packet = static_cast<const ws2cs_auction_time_out_del *>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.get_auction_mgr().time_out_del(packet->auction_info);

	}

	void lua_ws2cs_time_out_del_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_time_out_del_proc, auction_time_out_del);
	}

	void ws2cs_auction_sell_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_auction_sell_end* packet = static_cast<const ws2cs_auction_sell_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->info.seller_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}
		player_ref.get_auction_mgr().sell_item_end(packet->info, packet->operate_result);
	}

	void lua_ws2cs_auction_sell_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_auction_sell_end_proc, auction_sell_end);
	}

	void ws2cs_auction_find_bid_target_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_auction_find_bid_target_end* packet = static_cast<const ws2cs_auction_find_bid_target_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (false == player_ref.is_valid())
		{
			return;
		}
		player_ref.get_auction_mgr().find_bid_target_end(packet->auction_info);
	}

	void lua_ws2cs_auction_find_bid_target_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_auction_find_bid_target_end_proc, auction_find_bid_target_end);
	}



	void ws2cs_cloud_shop_buy_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		ZoneScoped;
		cs2ws_cloud_shop_buy_resp resp;
		resp.error_id = 0;

		const ws2cs_cloud_shop_buy_req* packet = static_cast<const ws2cs_cloud_shop_buy_req*>(data_ptr);
		if (nullptr == packet)
		{
			resp.error_id = e_cloud_shop_communicate_error_define_data_error;
			connection_mgr::getInstance().send_to_ws( &resp, sizeof(resp));
			return;
		}

		resp.buy_type = packet->buy_type;
		resp.rold_guid = packet->role_guid;
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			resp.error_id = e_cloud_shop_communicate_error_define_player_offline;
			connection_mgr::getInstance().send_to_ws( &resp, sizeof(resp));
			return;
		}
		resp.server_id = player_ref.get_unit_info(e_role_info_server_id);
		resp.sender_server_id = cell_server::getInstance().get_server_id();
		assert(packet->buy_type >= 0 && packet->buy_type <= 1);
		if (packet->buy_type < 0 || packet->buy_type > 1)
		{
			resp.error_id = e_cloud_shop_communicate_error_define_req_type_error;
			connection_mgr::getInstance().send_to_ws( &resp, sizeof(resp));
			return;
		}

		if (packet->buy_type == 0)
		{
			VIPTemplate* vip_template_ptr = template_manager::get_instance().get_template_by_vip_level(player_ref.get_vip_level());
			assert(vip_template_ptr != nullptr);
			if (vip_template_ptr == nullptr)
			{
				resp.error_id = e_cloud_shop_communicate_error_define_vip_data_error;
				connection_mgr::getInstance().send_to_ws( &resp, sizeof(resp));
				return;
			}
			if (vip_template_ptr->CloudShopLimitCount <= packet->have_buy_count)
			{
				resp.error_id = e_cloud_shop_communicate_error_define_not_enough_vip_buy_times;
				connection_mgr::getInstance().send_to_ws( &resp, sizeof(resp));
				return;
			}
		}
		// 大于10000说明是物品走物品逻辑
		if (packet->need_money_type > 10000)
		{
			if (item_system::can_cost_item(&player_ref, e_bag_type_bag, packet->need_money_type, packet->need_money_count) == false)
			{
				resp.error_id = e_cloud_shop_communicate_error_define_not_enough_money;
				connection_mgr::getInstance().send_to_ws(&resp, sizeof(resp));
				return;
			}
			item_system::cost_item_from_bag(&player_ref, e_bag_type_bag, packet->need_money_type, packet->need_money_count);
		}
		else
		{
			if (player_ref.can_cut_money((e_money_type)(packet->need_money_type), packet->need_money_count) == false)
			{
				resp.error_id = e_cloud_shop_communicate_error_define_not_enough_money;
				connection_mgr::getInstance().send_to_ws(&resp, sizeof(resp));
				return;
			}
			player_ref.cut_money((e_money_type)(packet->need_money_type), packet->need_money_count, e_server_log_cut_money_cloud_shop, packet->buy_type);
		}

		connection_mgr::getInstance().send_to_ws( &resp, sizeof(resp));
	}

	void lua_ws2cs_cloud_shop_buy_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_cloud_shop_buy_proc, cloud_shop_buy_req)
	}

	void ws2cs_sub_player_item_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_sub_player_rename_item* packet = static_cast<const ws2cs_sub_player_rename_item*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		player_ref.sub_rename_card(packet->item_templete_id);

	}

	void ws2cs_change_player_name_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_change_player_name* packet = static_cast<const ws2cs_change_player_name*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid() == false)
		{
			return;
		}

		if (strlen(packet->role_name) <= 0)
		{
			return;
		}

		player_ref.change_name(packet->role_name);

		guid_64 couple_guid = player_ref.get_couple_guid();
		player& player_couple_ref = unit_man::get_player(couple_guid);
		if (player_couple_ref.is_valid() == false)
		{
			return;
		}

		if (strlen(packet->role_name) <= 0)
		{
			return;
		}

		player_couple_ref.set_couple_name(packet->role_name);
	}

	void ws2cs_set_legion_average_lv_to_map_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_set_legion_average_lv_to_map* packet = static_cast<const ws2cs_set_legion_average_lv_to_map*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (packet->legion_average_lv <= 0)
		{
			return;
		}
		auto map_ptr = world_cs::get_map_by_guid<map_object>(packet->map_guid);
		if (nullptr == map_ptr)
		{
			return;
		}
		map_ptr->set_legion_average_lv(packet->legion_average_lv);
	}

	void ws2cs_gm_spawn_npc_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_spawn_npc* packet = static_cast<const ws2cs_spawn_npc*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_spawn_npc))
		{
			return;
		}
		auto map_ent = get_entity(packet->map_guid);
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("ws2cs_gm_spawn_npc_func map_ent is nullptr");
			return;
		}

		s_unit_identifier empty_owner;
		world_cs::spawn_npc(packet->npc_template_id, packet->pos, empty_owner, map_ent);
	}

	void lua_ws2cs_gm_spawn_npc_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_gm_spawn_npc_func, spawn_npc);
	}

	void ws2cs_gm_modify_money_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_modify_money* packet = static_cast<const ws2cs_modify_money*>(data_ptr);
		if (NULL == packet)
		{
			CONSOLE_ERROR("ws2cs_req_time_out NULL == packet");
			return;
		}
		player& player_ref = unit_man::get_player(packet->cs_unit_index);
		if (player_ref.is_valid())
		{
			if (packet->is_add_money)
			{
				player_ref.add_money(packet->money_type, packet->money_value, faith::e_server_log_add_money_gm_order, -1, false);
			}
			else
			{
				player_ref.cut_money(packet->money_type, packet->money_value,faith::e_server_log_cut_money_gm_order);
			}
		}
	}

	void ws2cs_set_cur_server_harry_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_send_cur_server_harry_msg* packet = static_cast<const ws2cs_send_cur_server_harry_msg*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_ERROR("ws2cs_send_cur_server_harry_msg NULL == packet");
			return;
		}
		if (data_len != sizeof(ws2cs_send_cur_server_harry_msg))
		{
			return;
		}
		cell_server::getInstance().set_cur_server_harry_msg(packet->server_harry_msg);
	}

	void lua_ws2cs_set_cur_server_harry_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_set_cur_server_harry_func, send_cur_server_harry_msg);
	}

	void ws2cs_send_create_normal_player_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_send_create_normal_player* packet = static_cast<const ws2cs_send_create_normal_player*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_ERROR("ws2cs_send_create_normal_player NULL == packet");
			return;
		}
		if (data_len != sizeof(ws2cs_send_create_normal_player))
		{
			return;
		}
		int32 num = packet->crate_num;
		auto map_cs_ptr = world_cs::get_map_by_template_id<map_cs>(harry_map_id);
		if (nullptr == map_cs_ptr)
		{
			return;
		}

		map_cs_ptr->create_harry_map_normal_npc(num);
	}

	void ws2cs_create_harry_player_statue_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_create_harry_player_statue* packet = static_cast<const ws2cs_create_harry_player_statue*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_create_harry_player_statue))
		{
			return;
		}

		world_cs::create_harry_player_statue(packet->big_player_info, packet->big_type, packet->map_guid);
	}

	void lua_ws2cs_create_harry_player_statue_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_create_harry_player_statue_proc, create_harry_player_statue);
	}

	void ws2cs_clear_harry_player_statue_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_claer_all_harry_player* packet = static_cast<const ws2cs_claer_all_harry_player*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_ERROR("ws2cs_claer_all_harry_player NULL == packet");
			return;
		}
		if (data_len != sizeof(ws2cs_claer_all_harry_player))
		{
			return;
		}
		auto map_obj_ptr = world_cs::get_map_by_template_id<map_cs>(harry_map_id);
		if (nullptr == map_obj_ptr)
		{
			return;
		}
		map_obj_ptr->remove_all_monster();
	}

	void ws2cs_cross_ladder_refresh_ticket_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_cross_ladder_refresh_ticket_info* packet = static_cast<const ws2cs_cross_ladder_refresh_ticket_info*>(data_ptr);
		if (nullptr == packet)
			return;
		if (data_len != sizeof(ws2cs_cross_ladder_refresh_ticket_info))
			return;
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid())
			player_ref.set_cross_ladder_can_buy_ticket_num(packet->can_buy_ticket_num);
	}

	void ws2cs_cross_ladder_send_last_score_proc(uint32 connindex, const void *data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_cross_ladder_send_last_score* packet = static_cast<const ws2cs_cross_ladder_send_last_score*>(data_ptr);
		if (nullptr == packet)
			return;
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid())
		{
			player_ref.receive_cross_ladder_last_score(packet->last_score);
			player_ref.receive_cross_ladder_last_ranking(packet->last_ranking);
		}
	}

	void ws2cs_cross_server_world_boss_creat_boss_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_cross_server_world_boss_creat_boss* packet = static_cast<const ws2cs_cross_server_world_boss_creat_boss*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_cross_server_world_boss_creat_boss))
		{
			return;
		}
		int32	true_boss_level = packet->true_boss_level;
		int32	false_boss_level = packet->false_boss_level;
		guid_64 from_map_guid = packet->from_map_guid;

		//auto map_ptr = world_cs::get_map_by_guid<cross_server_world_boss_map_cs>(from_map_guid);

		//if (nullptr == map_ptr || map_ptr->get_map_type() != e_map_type_cross_server_pk || map_ptr->get_map_template_ptr()->SubType != 4)
		//{
		//	return;
		//}
		//map_ptr->sync_creat_boss_end(true_boss_level, false_boss_level);
	}

	void lua_ws2cs_cross_server_world_boss_creat_boss_func(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_cross_server_world_boss_creat_boss_func, cross_server_world_boss_creat_boss);
	}

	void ws2cs_check_time_limit_activity_common_data_resp(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_check_time_limit_activity_common_data* packet = static_cast<const ws2cs_check_time_limit_activity_common_data*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_check_time_limit_activity_common_data))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->user_guid);
		if (player_ref.is_valid())
		{
			player_ref.get_time_limit_activity_mgr().check_time_limit_activity_common_data_to_ws_resp(packet->activity_id, packet->is_achieve);
		}
	}
	void ws2cs_gm_set_time_limit_activity_pause_flag_resp(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_set_time_limit_activity_pause_flag* packet = static_cast<const ws2cs_set_time_limit_activity_pause_flag*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_set_time_limit_activity_pause_flag))
		{
			return;
		}
		//time_limit_activity_object::set_pause_flag(packet->pause_flag);
	}
	void ws2cs_end_act_type_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_end_act_type* packet = static_cast<const ws2cs_end_act_type*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_end_act_type))
		{
			return;
		}
		int32 act_type = packet->act_type;
		bool is_need_refresh = packet->is_need_send_to_client;
		time_limit_activity_temp_mgr::get_instance().end_act_with_act_type(act_type, is_need_refresh);
	}

	void ws2cs_time_limit_act_temp(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_act_limit_temp* packek = static_cast<const ws2cs_act_limit_temp*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_act_limit_temp))
		{
			return;
		}
		time_limit_activity_temp_mgr::get_instance().recv_cs2ws_limit_act_temp(packek->temp_db_info, packek->data_num, packek->is_begin);
	}

	void lua_ws2cs_time_limit_act_temp(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		faith::ws2cs_proto::act_limit_temp msg;          
	    bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len); 
	    if (is_sucess == false)   
		{                        
		  return;               
	    }                          
	    ws2cs_act_limit_temp packet; 
	    packet.from_proto(msg);          
		ws2cs_time_limit_act_temp(conn_index, &packet, sizeof(ws2cs_act_limit_temp)); 
		
	}

	void ws2cs_time_limit_act_branch_temp(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_act_limit_branch_temp* packek = static_cast<const ws2cs_act_limit_branch_temp*>(data_ptr);
		if (nullptr == packek)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_act_limit_branch_temp))
		{
			return;
		}
		time_limit_activity_temp_mgr::get_instance().recv_cs2ws_limit_act_branch_temp(packek->temp_db_info, packek->data_num, packek->is_end);
	}

	void lua_ws2cs_time_limit_act_branch_temp(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_time_limit_act_branch_temp, act_limit_branch_temp);
	}

	void ws2cs_send_bonfire_level_func(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_send_bonfire_level* packet = static_cast<const ws2cs_send_bonfire_level*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_send_bonfire_level))
		{
			return;
		}
		//auto map_ptr = world_cs::get_map_by_guid<legion_station_map_cs>(packet->map_guid);

		//if (nullptr == map_ptr || map_ptr->get_map_type() != e_map_type_legion_station)
		//{
		//	return;
		//}
		//map_ptr->set_bonfire_level(packet->bonfire_level);
	}
	void ws2cs_set_map_own_legion(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_set_legion_guid_to_map* packet = static_cast<const ws2cs_set_legion_guid_to_map*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_set_legion_guid_to_map))
		{
			return;
		}
		auto map_ptr = world_cs::get_map_by_guid<map_object>(packet->map_guid);

		if (nullptr == map_ptr)
		{
			return;
		}
		map_ptr->set_map_own_legion(packet->legion_guid);
	}
	void ws2cs_transfer_all_player_this_map(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_transfer_this_map_all_player* packet = static_cast<const ws2cs_transfer_this_map_all_player*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_transfer_this_map_all_player))
		{
			return;
		}
		auto map_ptr = world_cs::get_map_by_guid<map_object>(packet->map_guid);

		if (nullptr == map_ptr)
		{
			return;
		}
		map_ptr->transfer_all_player();
		map_ptr->remove_all_monster();
	}

	void ws2cs_sync_assist_fight_info_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_sync_assist_fight_info* packet = static_cast<const ws2cs_sync_assist_fight_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_sync_assist_fight_info))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid())
		{
			player_ref.get_assist_fight_mgr().update_assist_fight_info(packet->m_assist_fight_info);
			if (player_ref.get_write_log())
			{
				MapTemplate* map_ptr = GET_TEMPLATE(MapTemplate, packet->m_assist_fight_info.map_id);
				if (nullptr != map_ptr)
				{
					set_log_var(log_head)
					player_ref.get_log_common_head_info(log_head);
					server_log::serverAssistantLog(log_head, packet->m_assist_fight_info.map_id);
				}	
			}
			player_ref.get_assist_fight_mgr().go_to_assist();
		}
	}
	void lua_ws2cs_sync_assist_fight_info_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_sync_assist_fight_info_proc, sync_assist_fight_info);
	}
	void ws2cs_create_pk_king_combat_map_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_create_pk_king_combat_map_end* packet = static_cast<const ws2cs_create_pk_king_combat_map_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_create_pk_king_combat_map_end))
		{
			return;
		}
		guid_64 from_map_guid = packet->map_guid;

		//auto map_ptr = world_cs::get_map_by_guid<pk_king_map_cs>(from_map_guid);

		//if (nullptr == map_ptr || map_ptr->get_map_type() != e_map_type_king_of_pk)
		//{
		//	return;
		//}
		//map_ptr->allot_all_play_enter_map(packet->map_array, packet->map_num);
	}
	void lua_ws2cs_create_pk_king_combat_map_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_create_pk_king_combat_map_end_proc, create_pk_king_combat_map_end);
	}
	void ws2cs_element_war_map_init_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_element_war_send_create_robot* packet = static_cast<const ws2cs_element_war_send_create_robot*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_element_war_send_create_robot))
		{
			return;
		}
		guid_64 from_map_guid = packet->map_guid;

		//auto map_ptr = world_cs::get_map_by_guid<element_war_map_cs>(from_map_guid);

		//if (nullptr == map_ptr)
		//{
		//	return;
		//}

		//map_ptr->init_map_unit_data(packet->match_info);
	}
	void lua_ws2cs_element_war_map_init_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		ZoneScoped;
		PROTO_FUNC_TO_PACKET(ws2cs_element_war_map_init_proc, element_war_send_create_robot);

	}
	void ws2cs_element_war_get_mission_reward(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_element_war_check_can_get_mission_reward_end* packet = static_cast<const ws2cs_element_war_check_can_get_mission_reward_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(ws2cs_element_war_check_can_get_mission_reward_end))
		{
			return;
		}
		player& player_ref = unit_man::get_player(packet->role_guid);
		if(false == player_ref.is_valid() || player_ref.get_session_state() != e_session_status_in_gaming || false == player_ref.is_self_server())
		{
			return;
		}

		ElementLadderTaskTemplate* template_ptr = GET_TEMPLATE(ElementLadderTaskTemplate, packet->mission_id);
		if (nullptr == template_ptr || template_ptr->Reward.size() < 2)
		{
			return;
		}
		int32 mission_reward_id = template_ptr->Reward[0];
		int32 mission_reward_num = template_ptr->Reward[1];
		int32 reward_tag = player_ref.get_logic_data(e_role_logic_info_element_war_mission_tag);
		game_proto_element_war_get_mission_reward_end ret;
		ret.set_ret_index(e_element_war_get_mission_reward_succeed);
		if ((reward_tag & (1 << packet->mission_index)) == 0)
		{
			std::vector<int32> item_arr;
			item_arr.push_back(mission_reward_id);
			item_arr.push_back(mission_reward_num);
			
			player_ref.get_item_set().put_in_bag(e_server_log_add_item_element_mission, packet->mission_id, item_arr);
			player_ref.set_logic_data(e_role_logic_info_element_war_mission_tag, (reward_tag | (1 << packet->mission_index)));
			player_ref.send_logic_one(e_role_logic_info_element_war_mission_tag);
		}
		else
		{
			ret.set_ret_index(e_element_war_get_mission_reward_have_reward);
		}
		player_ref.send_message(&ret, e_mgsindex_s2c_element_war_get_mission_reward_end);
	}
	void lua_ws2cs_element_war_get_mission_reward(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		PROTO_FUNC_TO_PACKET(ws2cs_element_war_get_mission_reward, element_war_check_can_get_mission_reward_end);
	}
	void ws2cs_element_war_send_last_score_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_element_war_send_last_score* packet = static_cast<const ws2cs_element_war_send_last_score*>(data_ptr);
		if (nullptr == packet)
			return;
		player& player_ref = unit_man::get_player(packet->role_guid);
		if (player_ref.is_valid())
		{
			player_ref.receive_element_ladder_last_score(packet->last_score);
			player_ref.receive_element_ladder_last_ranking(packet->last_ranking);
		}
	}
	void ws2cs_set_ladder_world_level_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_set_ladder_world_level* packet = static_cast<const ws2cs_set_ladder_world_level*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		cell_server::getInstance().set_ladder_world_level(packet->world_level);
	}
	void ws2cs_attack_city_init_data_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		ZoneScoped;
		const ws2cs_attack_city_init_data* packet = static_cast<const ws2cs_attack_city_init_data*>(data_ptr);
		if (nullptr == packet)
		{
			CONSOLE_ERROR("ws2cs_attack_city_init_data_proc");
			return;
		}
		if (data_len != sizeof(ws2cs_attack_city_init_data))
		{
			CONSOLE_ERROR("ws2cs_attack_city_init_data_proc");
			return;
		}
		//auto map_ptr = world_cs::get_map_by_guid<attack_city_map_cs>(packet->map_guid);

		//if (nullptr == map_ptr || map_ptr->get_map_type() != e_map_type_attack_city)
		//{
		//	CONSOLE_ERROR("ws2cs_attack_city_init_data_proc");
		//	return;
		//}

		//map_ptr->init_data(*packet);
	}
}