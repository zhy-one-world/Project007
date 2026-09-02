/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   15:01
	file base:	msgproc_dp
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "../Utility/cs_date.hpp"
#include "../Utility/init_unit.h"
#include "character.pb.h"
#include "chat.pb.h"
#include "cross_group/cross_transfer_logic.hpp"
#include "game_cfg/servers_config.h"
#include "http/http_send.hpp"

#include "msgproc_dp.hpp"
#include "net.pb.h"
#include "relation.pb.h"
#include "server/activity/broken_sky_mgr.h"
#include "server/activity/cloud_shop_mgr.h"
#include "server/activity/cross_ladder_ws_mgr.h"
#include "server/activity/cross_server_city_war_ws_mgr.h"
#include "server/activity/cross_server_harry_ws_mgr.h"
#include "server/activity/cross_server_pk_ws_mgr.h"
#include "server/activity/cross_server_world_boss_ws_mgr.h"
#include "server/activity/element_war_ws_mgr.h"
#include "server/activity/gain_treasure_ws_mgr.h"
#include "server/activity/time_limit_activity_temp_ws_mgr.h"
#include "server/activity/time_limit_activity_ws_mgr.h"
#include "server/activity/time_limit_activity_ws_mgr.h"
#include "server/appearance_info_mgr.h"
#include "server/arena/arena_mgr_ws.h"
#include "server/auction/auction_mgr_ws.h" 
#include "server/best_record/best_record_mgr.h"
#include "server/big_player_ws_mgr.h"
#include "server/client_session.hpp"
#include "server/client_session_mgr.hpp"
#include "server/legion/attack_city_ws_mgr.h"
#include "server/legion/city_war_territory_mgr.h"
#include "server/legion/legion_ws_mgr.h"
#include "server/login/login_service_mgr.hpp"
#include "server/login_role/login_role_mgr.h"
#include "server/lucky_draw_record_ws_mgr.h"
#include "server/mail/event_ws_mgr.h"
#include "server/mail/mail_event_ws.h"
#include "server/ranking/ranking_mgr_ws.h"
#include "server/recharge_mgr_ws.hpp"
#include "server/red_package/red_package_ws_mgr.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "server/rename/rename_mgr.h"
#include "server/web_client.hpp"
#include "server_log.hpp"
#include "server_log.hpp"
#include "template/template_manager.h"
#include "utility/parse_msg.h"
#include "utility/serialize_msg.h"
#include "world_server.hpp"
#include "ws_client.hpp"
#include <activity_msg.hpp>
#include <appearance_info_msg.hpp>
#include <attack_city_msg.hpp>
#include <auction_msg.hpp>
#include <char_msg.hpp>
#include <chat_msg.hpp>
#include <cloud_shop_msg.hpp>
#include <core.hpp>
#include <cross_ladder_msg.hpp>
#include <cross_server_harry_msg.hpp>
#include <cross_server_pk_msg.hpp>
#include <cross_server_world_boss_msg.hpp>
#include <event_msg.hpp>
#include <gain_treasure_msg.hpp>
#include <item_msg.hpp>
#include <legion_msg.hpp>
#include <lucky_draw_record_msg.hpp>
#include <mail_msg.hpp>
#include <marry_msg.hpp>
#include <ranking_msg.hpp>
#include <time_limit_activity_msg.hpp>
#include <rlog.hpp>

namespace faith
{
	void dp2ls_reply_client_login(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}

		login_service_mgr::getInstance().end_login(data_ptr, data_len);
	}

	void dp2ls_reply_character_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ls_enum_character* db_reponse = static_cast<const dp2ls_enum_character*>(data_ptr);
		if (db_reponse == NULL)
			return;
		if (db_reponse->char_count == 0)
		{
			character_proto_enum_character_end_info char_info;
			char_info.set_last_role(1);
			char_info.set_is_only_create_role(world_server::getInstance().get_server_info_arr(e_server_info_type_only_create_role));
			packet_c2s_s2c msg;
			serialize_msg::get_instance().set_serialize_msg_new(msg, &char_info, db_reponse->client_uid, e_msgindex_s2c_enum_char);
			world_server::getInstance().send_to_fep(db_reponse->client_uid.fepserver_uid, &msg, msg.get_packet_len());
		}
		else
		{
			login_role_mgr::getInstance().set_account_info(db_reponse->char_data[0].account, db_reponse->char_count);
			for (int32 i = 0; i < db_reponse->char_count; ++i)
			{
				login_role_mgr::getInstance().set_role_info(db_reponse->char_data[i], db_reponse->client_uid);
			}
		}
	}

	void dp2ls_reply_create_character(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ls_create_character* pdata = static_cast<const dp2ls_create_character*>(data_ptr);
		if (!pdata || data_len != sizeof(dp2ls_create_character))
		{
			return;
		}
		if (pdata->eresult == e_create_role_success)
		{
			login_proto_login login_proto_msg;
			login_proto_msg.ParseFromArray(pdata->login_proto_msg, pdata->login_msg_len);
			if (login_proto_msg.login_type() != e_login_type_new_robot)
			{
				server_log::create_role_log(world_server::getInstance().get_server_id(),
					login_proto_msg.sdk_data().device_id(),
					login_proto_msg.sdk_data().app_key(),
					login_proto_msg.client_version(),
					login_proto_msg.sdk_data().channel_id(),
					login_proto_msg.sdk_data().app_secret(),
					login_proto_msg.sdk_data().model(),
					login_proto_msg.sdk_data().idfa(),
					login_proto_msg.sdk_data().imei(),
					login_proto_msg.sdk_data().android_id(),
					login_proto_msg.sdk_data().mac(),
					login_proto_msg.sdk_data().sn(),
					pdata->role_info,
					login_proto_msg.ip(),
					login_proto_msg.login_type());
				set_log_var(log_head);
				set_log_common_head_part1(log_head, SERVERCONFIG->app_key, login_proto_msg.sdk_data().channel_id(), "0", login_proto_msg.sdk_data().device_id());
				set_log_common_head_part2(log_head, login_proto_msg.client_version(), init_unit::change_string_to_i32(login_proto_msg.custom_info()));
				set_log_common_head_part3(log_head, pdata->role_info.account, world_server::getInstance().get_server_id(), utility::get_tick_count())
				set_log_common_head_part4(log_head, pdata->role_info.role_guid, pdata->role_info.role_name, pdata->role_info.data_ary[e_role_info_exp_level], pdata->role_info.data_ary[e_role_info_gender])
				set_log_common_head_part5(log_head, pdata->role_info.data_i64_ary[e_role_i64_info_gs_value], pdata->role_info.data_ary[e_role_info_class_type], pdata->role_info.data_ary[e_role_info_vip_level]);

				server_log::serverRoleCreate(log_head);



			}
		}
		ls2fep_create_role msg;
		msg.client_uid = pdata->client_uid;
		msg.eresult = pdata->eresult;
		msg.role_guid = pdata->role_guid;
		msg.role_info = pdata->role_info;
		world_server::getInstance().send_to_fep(pdata->client_uid.fepserver_uid, &msg, sizeof(msg));
	}


	void dp2ls_reply_delete_character(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ls_delete_character_end* pdata = static_cast<const dp2ls_delete_character_end*>(data_ptr);
		if (!pdata || data_len != sizeof(dp2ls_delete_character_end))
		{
			return;
		}
		character_proto_delete_character_end del_msg;
		del_msg.set_role_guid(pdata->role_guid.server_64);
		del_msg.set_del_time(pdata->del_time);
		del_msg.set_del_type(pdata->del_type);
		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, &del_msg, pdata->client_uid, e_msgindex_s2c_del_char);
		world_server::getInstance().send_to_fep(pdata->client_uid.fepserver_uid, &msg, msg.get_packet_len());
	}

	void dp2ls_load_enum_item_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}

		const dp2ls_load_enum_item* pdata = static_cast<const dp2ls_load_enum_item*>(data_ptr);

		login_role_mgr::getInstance().set_item_data(pdata->role_guid, pdata->item_data);
	}
	void dp2ls_load_enum_buff_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}

		const dp2ls_load_enum_buff* pdata = static_cast<const dp2ls_load_enum_buff*>(data_ptr);

		login_role_mgr::getInstance().set_buff_data(pdata->role_guid, pdata->buff_data);
	}
	void dp2ls_load_enum_spirit_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}

		const dp2ls_load_enum_spirit* pdata = static_cast<const dp2ls_load_enum_spirit*>(data_ptr);

		login_role_mgr::getInstance().set_spirit_data(pdata->role_guid, pdata->temp_sprite_info);
	}
	void dp2ls_load_enum_time_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}

		const dp2ls_load_enum_time* pdata = static_cast<const dp2ls_load_enum_time*>(data_ptr);

		login_role_mgr::getInstance().set_time_data(pdata->role_guid, pdata->login_out_time);
	}

	void dp2ws_rep_load_last_server_list_lua(uint32 conn_index, int32 server_id, xstring last_server_list)
	{
		s_last_server_list_db server_info;
		server_info.server_id = server_id;
		server_info.set_last_server_list(last_server_list);
		world_server::getInstance().set_last_server_list_from_db(server_info);
	}

	void dp2ws_rep_load_last_server_list(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//const dp2ws_load_last_server_list* packet = static_cast<const dp2ws_load_last_server_list*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	return;
		//}
		//world_server::getInstance().set_last_server_list_from_db(packet->last_server_list);

		dp2s_proto_dp2ws_load_last_server_list request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		dp2ws_rep_load_last_server_list_lua(conn_index, request.server_id(), request.last_server_list());
	}

	void dp2ws_rep_get_game_info_ws_lua(uint32 conn_index, int32 server_id, int32 begin_time, int32 server_info_arr[e_server_info_type_max], int32 gm_commond_arr[e_need_server_cross_max - 1])
	{
				//	init global world_server obj.
		if (!world_server::getInstance().set_begin_time(begin_time))
		{
			_RLOG_(MINFO, "world_server::getInstance().init");
			return;
		}
		int32 cross_server_id = gm_commond_arr[e_need_server_cross_begin_cross - 1];
		world_server::getInstance().set_need_begin_cross_gm_common(cross_server_id, e_need_server_cross_begin_cross);
		s_game_info game_info;
		xstring& out_ip = world_server::getInstance().get_out_ip();
		memcpy(game_info.out_ip, out_ip.c_str(), out_ip.size());
		game_info.out_port = world_server::getInstance().get_out_port();
		xstring& server_name = world_server::getInstance().get_server_name();
		memcpy(game_info.server_name, server_name.c_str(), server_name.size());
		game_info.server_id = world_server::getInstance().get_server_id();
		game_info.cross_id = cross_server_id;
		world_server::getInstance().set_server_config(game_info);

		for (int32 i = e_server_info_type_begin_cross_server_time; i < e_server_info_type_max; i++)
		{
			if (i == e_server_info_type_begin_cross_server_time)
			{
				world_server::getInstance().set_cross_server_open_time(server_info_arr[i], false);
				continue;
			}
			int32 server_value = server_info_arr[i];
			if (i == e_server_info_type_server_cross_state && !world_server::getInstance().is_sky_island_server())
			{
				continue;
			}
			world_server::getInstance().set_server_info_arr(server_value, i);
		}
		world_server::getInstance().refresh_server_cross_time();
		world_server::getInstance().start();
		ws_client::getInstance().start_gate();

		_RLOG_(MINFO, "main(): scheduler started");
		_RLOG_(MINFO, "main(): world_server started");
		_RLOG_(MINFO, "main(): main-thread enter loop");

		if (cross_server_id <= 0)
		{
			for (int32 i = e_need_server_cross_begin_cross; i < e_need_server_cross_max; i++)
			{
				world_server::getInstance().set_need_begin_cross_gm_common(gm_commond_arr[i - 1], (e_need_server_cross)i);
			}
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_gm_common);
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_server_state);

			legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
			city_war_mgr.set_cross_server_territory_war_state(world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war));
			world_server::getInstance().send_server_config_to_db();
			world_server::getInstance().load_server_attr_val();
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				for (int32 i = e_need_server_cross_begin_cross; i < e_need_server_cross_max; i++)
				{
					world_server::getInstance().set_need_begin_cross_gm_common(gm_commond_arr[i - 1], (e_need_server_cross)i);
				}
				world_server::getInstance().load_last_server_list_from_db();
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_gm_common);
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_server_state);

				world_server::getInstance().gate_server_func_when_game_info_load_end();
			}
			else
			{
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_gm_common);
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_cross_server_state);
			}
		}
	}

	void dp2ws_rep_get_game_info_ws(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		_RLOG_(MINFO, "dp2ws_rep_get_game_info_ws begin");
		dp2s_proto_dp2ws_get_game_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			_RLOG_(MINFO, "dp2ws_rep_get_game_info_ws nullptr == packet");
			return;
		}
		int32 t_server_info_arr[e_server_info_type_max];
		for (int32 i = 0; i < request.server_info_arr_size(); i++)
		{
			if (i >= e_server_info_type_max)
			{
				break;
			}
			t_server_info_arr[i] = request.server_info_arr(i);
		}

		int32 t_gm_commond_arr[e_need_server_cross_max - 1];
		for (int32 i = 0; i < request.gm_commond_arr_size(); i++)
		{
			if (i >= e_need_server_cross_max - 1)
			{
				break;
			}
			t_gm_commond_arr[i] = request.gm_commond_arr(i);
		}
		dp2ws_rep_get_game_info_ws_lua(conn_index, request.server_id(), request.begin_time(), t_server_info_arr, t_gm_commond_arr);
		
	}
	void dp2ws_rep_get_role_info_ws_lua(uint32 conn_index, int64 role_guid, xstring account, xstring role_name, xstring role_appearance, xstring role_first_kill_npc_id_arr, int64 map_guid, int64 data_i64_ary[e_role_i64_info_max], int32 data_ary[e_role_info_max], int32 ws_data_ary[e_role_ws_info_max], int32 is_login, int32 is_send_chat, int32 worship_value, int32 eresult)
	{
		client_session* session = client_session_mgr::getInstance().get_session(role_guid);
		if (session == NULL)
		{
			CONSOLE_ERROR("dp2ws_rep_get_role_info_ws packet->role_info.role_guid:{}", role_guid);
			return;
		}
		if (session->m_status != client_session::e_ss_map_transfer)
		{
			CONSOLE_ERROR("dp2ws_rep_get_role_info_ws session->m_status:{}", (int32)session->m_status);
			return;
		}

		switch (eresult)
		{
		case e_error_code_success:
		{
			if (is_login > 0)
			{
				_RLOG_(MINFO, "dp2ws_rep_get_role_info_ws is_login > 0");
				ws2fep_enter_game rep;
				rep.client_uid = session->m_client_uid;
				rep.e_result = e_error_code_enter_no_allow;
				memcpy(rep.account, account.c_str(), account.size());
				world_server::getInstance().send_to_fep(session->m_client_uid.fepserver_uid, &rep, sizeof(rep));
				return;
			}
			session->m_role_info.role_guid = role_guid;
			session->m_role_info.set_role_account(account);
			session->m_role_info.set_role_name(role_name);
			session->m_role_info.set_appearance(role_appearance);
			session->m_role_info.set_first_kill_npc_id_arr(role_first_kill_npc_id_arr);
			session->m_role_info.map_guid = map_guid;
			for (int32 i = 0; i < e_role_i64_info_max; i++)
			{
				session->m_role_info.data_i64_ary[i] = data_i64_ary[i];
			}
			for (int32 i = 0; i < e_role_info_max; i++)
			{
				session->m_role_info.data_ary[i] = data_ary[i];
			}

			session->m_is_send_chat = false;
			for (int32 i = 0; i < max_character_num; ++i)
			{
				if (session->m_ban_chat_array[i] == session->get_role_guid().server_64)
				{
					session->m_is_send_chat = true;
					break;
				}
			}
			if (is_send_chat > (utility::get_tick_count() / 1000))
			{
				session->m_is_send_chat = true;
			}
			session->m_worship_value = worship_value;
			session->m_step_num = client_session::e_session_step_dp_load;
			session->m_login_time = utility::get_tick_count();
			if (session->get_role_info_data(e_role_info_server_id) == world_server::getInstance().get_server_id())
			{
				if (session->get_role_info_data(e_role_info_move_server_id) != 0 && session->get_role_info_data(e_role_info_move_server_id) != world_server::getInstance().get_server_id())
				{
					_RLOG_(MINFO, ::faith::log_detail::format_message("dp2ws_rep_get_role_info_ws move_server_id:{}, cur_server_id:{}",  session->get_role_info_data(e_role_info_move_server_id),  world_server::getInstance().get_server_id()));
					ws2ws_kick_out_player kick_msg;
					kick_msg.role_guid = session->get_role_guid();
					kick_msg.need_send_save_end = true;
					cross::send_msg_to_ws(guid_64(), session->get_role_info_data(e_role_info_move_server_id), e_msgindex_ws2ws_kick_player, &kick_msg, sizeof(kick_msg));
					session->set_role_info_data(e_role_info_move_server_id, world_server::getInstance().get_server_id());
					int64 save_end_time = time_helper::get_cur_time_new().millisecond + second_tick_time * 3;
					session->set_cross_server_save_time(save_end_time);
					return;
				}
			}
			else
			{
				if (session->get_role_info_data(e_role_info_move_server_id) != world_server::getInstance().get_server_id())
				{
					_RLOG_(MINFO, ::faith::log_detail::format_message("dp2ws_rep_get_role_info_ws move_server_id:{}, cur_server_id:{}",  session->get_role_info_data(e_role_info_move_server_id),  world_server::getInstance().get_server_id()));
					ws2fep_enter_game rep;
					rep.client_uid = session->m_client_uid;
					memcpy(rep.account, session->m_role_info.account, sizeof(rep.account));
					rep.e_result = e_error_code_enter_error_server_id;
					world_server::getInstance().send_to_fep(session->m_client_uid.fepserver_uid, &rep, sizeof(rep));
					return;
				}
			}
			session->func_session_load_dp_end();
		}
		break;
		default:
		{
			CONSOLE_ERROR("dp2ws_rep_get_role_info_ws eresult:{}", eresult);
			ws2fep_enter_game rep;
			rep.client_uid = session->m_client_uid;
			memcpy(rep.account, session->m_role_info.account, sizeof(rep.account));
			rep.e_result = (e_error_code)eresult;
			world_server::getInstance().send_to_fep(session->m_client_uid.fepserver_uid, &rep, sizeof(rep));
		}
		break;
		}
	}

	void dp2ws_rep_get_role_info_ws(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2s_proto_dp2ws_load_character request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			CONSOLE_ERROR("dp2ws_rep_get_role_info_ws parse_message_server failed");
			return;
		}
		const dp2s_proto_s_unit_info& unit_info = request.role_info();

		int64 t_data_i64_ary[e_role_i64_info_max];
		for (int32 i = 0; i < unit_info.data_i64_ary_size(); i++)
		{
			if (i >= e_role_i64_info_max)
			{
				break;
			}
			t_data_i64_ary[i] = unit_info.data_i64_ary(i);
		}
		int32 t_data_ary[e_role_info_max];
		for (int32 i = 0; i < unit_info.data_ary_size(); i++)
		{
			if (i >= e_role_info_max)
			{
				break;
			}
			t_data_ary[i] = unit_info.data_ary(i);
		}
		int32 t_ws_data_ary[e_role_ws_info_max];
		for (int32 i = 0; i < request.ws_data_ary_size(); i++)
		{
			if (i >= e_role_ws_info_max)
			{
				break;
			}
			t_ws_data_ary[i] = request.ws_data_ary(i);
		}
		dp2ws_rep_get_role_info_ws_lua(conn_index, unit_info.role_guid(), unit_info.account(), unit_info.role_name(), unit_info.role_appearance(), unit_info.role_first_kill_npc_id_arr(), unit_info.map_guid()
			, t_data_i64_ary, t_data_ary, t_ws_data_ary, request.is_login(), request.is_send_chat(),
			request.worship_value(), request.eresult());

	}
	void dp2ws_rep_load_arena_rank_lua(uint32 conn_index,const char* data_ptr, size_t data_len, int32 start_index)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_s_arena_rank_all msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_arena_rank));
		if (p_data == nullptr)
		{
			return;
		}

		s_arena_rank *p_row = (s_arena_rank *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_arena_rank one_row = msg.load_data(i);

			p_row->rank_pos = one_row.rank_pos();
			p_row->role_guid = one_row.role_guid();
			parse_msg::getInstance().my_memcopy(p_row->role_name, max_name_size, one_row.role_name().c_str(), one_row.role_name().size());
			p_row->gs_value = one_row.gs_value();
			p_row->role_template_id = one_row.role_template_id();
			p_row->robot_template_id = one_row.robot_template_id();
			p_row->military_rank_level = one_row.military_rank_level();
			p_row->vip_level = one_row.vip_level();
			parse_msg::getInstance().my_memcopy(p_row->game_channel, e_login_fixed_data_lenght, one_row.game_channel().c_str(), one_row.game_channel().size());
			p_row++;
		}

		arena_mgr_ws::get_instance().load_ranks_from_db((s_arena_rank *)p_data, start_index, row_count);
	}

	void dp2ws_rep_load_arena_rank(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_dp2ws_load_arena_rank all_msg;
		if (!parse_msg::getInstance().parse_message_server(&all_msg, data_ptr, data_len))
		{
			return;
		}
		const dp2s_proto_s_arena_rank_all& msg = all_msg.load_data_all();

		int32 row_count = msg.row_count();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_arena_rank));
		if (p_data == nullptr)
		{
			return;
		}

		s_arena_rank *p_row = (s_arena_rank *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_arena_rank one_row = msg.load_data(i);

			p_row->rank_pos = one_row.rank_pos();
			p_row->role_guid = one_row.role_guid();
			parse_msg::getInstance().my_memcopy(p_row->role_name, max_name_size, one_row.role_name().c_str(), one_row.role_name().size());
			p_row->gs_value = one_row.gs_value();
			p_row->role_template_id = one_row.role_template_id();
			p_row->robot_template_id = one_row.robot_template_id();
			p_row->military_rank_level = one_row.military_rank_level();
			p_row->vip_level = one_row.vip_level();
			parse_msg::getInstance().my_memcopy(p_row->game_channel, e_login_fixed_data_lenght, one_row.game_channel().c_str(), one_row.game_channel().size());
			p_row++;
		}

		arena_mgr_ws::get_instance().load_ranks_from_db((s_arena_rank *)p_data, all_msg.start_index(), row_count);
	}

	void dp2ws_create_arena_robot(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		arena_mgr_ws::get_instance().create_robot();
	}
	void dp2ws_load_arena_finish(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		arena_mgr_ws::get_instance().fill_empty_pos();
	}

	void dp2ws_rep_load_all_ranking_info_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int32 ranking_type)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_s_ranking_top_info msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}

		int32 row_count = msg.data_num();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_ranking_player_info));
		if (p_data == nullptr)
		{
			return;
		}

		s_ranking_player_info *p_row = (s_ranking_player_info *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_ranking_player_info one_row = msg.data_ary(i);

			p_row->role_guid = one_row.role_guid();
			parse_msg::getInstance().my_memcopy(p_row->role_name, max_name_size, one_row.role_name().c_str(), one_row.role_name().size());
			p_row->ranking_type = one_row.ranking_type();
			p_row->ranking_value = one_row.ranking_value();
			p_row->save_time = one_row.save_time();
			p_row->supporting_guid = one_row.supporting_guid();
			parse_msg::getInstance().my_memcopy(p_row->supporting_name, max_name_size, one_row.supporting_name().c_str(), one_row.supporting_name().size());
			p_row->vip_level = one_row.vip_level();
			p_row->Peak = one_row.peak();
			p_row->server_id = one_row.server_id();
			parse_msg::getInstance().my_memcopy(p_row->game_channel, e_login_fixed_data_lenght, one_row.game_channel().c_str(), one_row.game_channel().size());
			p_row->standby_parameter = one_row.standby_parameter();
			p_row++;
		}
		ranking_mgr_ws::load_ranking_from_db((s_ranking_player_info *)p_data, row_count, ranking_type);

	}

	void dp2ws_rep_load_all_ranking_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_dp2ws_load_all_ranking_info all_msg;
		if (!parse_msg::getInstance().parse_message_server(&all_msg, data_ptr, data_len))
		{
			return;
		}
		const dp2s_proto_s_ranking_top_info& msg = all_msg.data_info();

		int32 row_count = msg.data_num();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_ranking_player_info));
		if (p_data == nullptr)
		{
			return;
		}

		s_ranking_player_info *p_row = (s_ranking_player_info *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_ranking_player_info one_row = msg.data_ary(i);

			p_row->role_guid = one_row.role_guid();
			parse_msg::getInstance().my_memcopy(p_row->role_name, max_name_size, one_row.role_name().c_str(), one_row.role_name().size());
			p_row->ranking_type = one_row.ranking_type();
			p_row->ranking_value = one_row.ranking_value();
			p_row->save_time = one_row.save_time();
			p_row->supporting_guid = one_row.supporting_guid();
			parse_msg::getInstance().my_memcopy(p_row->supporting_name, max_name_size, one_row.supporting_name().c_str(), one_row.supporting_name().size());
			p_row->vip_level = one_row.vip_level();
			p_row->Peak = one_row.peak();
			p_row->server_id = one_row.server_id();
			parse_msg::getInstance().my_memcopy(p_row->game_channel, e_login_fixed_data_lenght, one_row.game_channel().c_str(), one_row.game_channel().size());
			p_row->standby_parameter = one_row.standby_parameter();
			p_row++;
		}
		ranking_mgr_ws::load_ranking_from_db((s_ranking_player_info *)p_data, row_count, all_msg.ranking_type());
	}

	void dp2ws_rep_load_one_ranking_info_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int64 sender_guid, int32 ranking_type, int32 worship_add_value, int32 worship_type, int32 end_result)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return;
		}
		dp2s_proto_s_ranking_player_info player_info;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&player_info, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}
		s_ranking_player_info t_player_info;
		t_player_info.role_guid = player_info.role_guid();
		t_player_info.set_role_name(player_info.role_name());
		t_player_info.ranking_type = player_info.ranking_type();
		t_player_info.ranking_value = player_info.ranking_value();
		t_player_info.save_time = player_info.save_time();
		t_player_info.supporting_guid = player_info.supporting_guid();
		t_player_info.set_supporting_name(player_info.supporting_name());
		t_player_info.vip_level = player_info.vip_level();
		t_player_info.Peak = player_info.peak();
		t_player_info.server_id = player_info.server_id();
		t_player_info.set_game_channel(player_info.game_channel());
		t_player_info.standby_parameter = player_info.standby_parameter();
		ranking_mgr_ws::load_one_worship_ranking_from_db(sender_guid, ranking_type, worship_add_value, t_player_info, worship_type, end_result);

	}

	void dp2ws_rep_load_one_ranking_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}
		dp2s_proto_dp2ws_load_one_ranking_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const dp2s_proto_s_ranking_player_info& player_info = request.data_info();
		s_ranking_player_info t_player_info;
		t_player_info.role_guid = player_info.role_guid();
		t_player_info.set_role_name(player_info.role_name());
		t_player_info.ranking_type = player_info.ranking_type();
		t_player_info.ranking_value = player_info.ranking_value();
		t_player_info.save_time = player_info.save_time();
		t_player_info.supporting_guid = player_info.supporting_guid();
		t_player_info.set_supporting_name(player_info.supporting_name());
		t_player_info.vip_level = player_info.vip_level();
		t_player_info.Peak = player_info.peak();
		t_player_info.server_id = player_info.server_id();
		t_player_info.set_game_channel(player_info.game_channel());
		t_player_info.standby_parameter = player_info.standby_parameter();
		ranking_mgr_ws::load_one_worship_ranking_from_db(request.sender_guid(), request.ranking_type(), request.worship_add_value(), t_player_info, request.worship_type(), request.end_result());
	}

	void dp2ws_rep_load_all_service_rank_info_lua(uint32 conn_index, const char* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_s_service_rank_info msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}
		int32 row_count = msg.data_num();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_service_rank_player_info));
		if (p_data == nullptr)
		{
			return;
		}

		s_service_rank_player_info *p_row = (s_service_rank_player_info *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_service_rank_player_info one_row = msg.data_ary(i);

			p_row->role_guid = one_row.role_guid();
			p_row->rank_type = one_row.rank_type();
			p_row->role_num = one_row.role_num();
			p_row++;
		}

		ranking_mgr_ws::load_all_server_rank_from_db((s_service_rank_player_info *)p_data, msg.data_num());

	}

	void dp2ws_rep_load_all_service_rank_info(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_dp2ws_load_all_service_rank_info all_msg;
		if (!parse_msg::getInstance().parse_message_server(&all_msg, data_ptr, data_len))
		{
			return;
		}
		const dp2s_proto_s_service_rank_info& msg = all_msg.all_data();
		int32 row_count = msg.data_num();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_service_rank_player_info));
		if (p_data == nullptr)
		{
			return;
		}

		s_service_rank_player_info *p_row = (s_service_rank_player_info *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_service_rank_player_info one_row = msg.data_ary(i);

			p_row->role_guid = one_row.role_guid();
			p_row->rank_type = one_row.rank_type();
			p_row->role_num = one_row.role_num();
			p_row++;
		}

		ranking_mgr_ws::load_all_server_rank_from_db((s_service_rank_player_info *)p_data, row_count);
	}

	void dp2ws_rep_load_one_worship_info_proc_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int64 sender, bool is_load)
	{
		if (data_ptr == nullptr || !data_len)
		{
			return;
		}
		dp2s_proto_s_ranking_player_info player_info;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&player_info, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}
		s_ranking_player_info t_player_info;
		t_player_info.role_guid = player_info.role_guid();
		t_player_info.set_role_name(player_info.role_name());
		t_player_info.ranking_type = player_info.ranking_type();
		t_player_info.ranking_value = player_info.ranking_value();
		t_player_info.save_time = player_info.save_time();
		t_player_info.supporting_guid = player_info.supporting_guid();
		t_player_info.set_supporting_name(player_info.supporting_name());
		t_player_info.vip_level = player_info.vip_level();
		t_player_info.Peak = player_info.peak();
		t_player_info.server_id = player_info.server_id();
		t_player_info.set_game_channel(player_info.game_channel());
		t_player_info.standby_parameter = player_info.standby_parameter();
		ranking_mgr_ws::get_player_worship_value_end(is_load, sender, t_player_info);

	}

	void dp2ws_rep_load_one_worship_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}
		dp2s_proto_dp2ws_get_worship_value_end request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		const dp2s_proto_s_ranking_player_info& player_info = request.target();
		s_ranking_player_info t_player_info;
		t_player_info.role_guid = player_info.role_guid();
		t_player_info.set_role_name(player_info.role_name());
		t_player_info.ranking_type = player_info.ranking_type();
		t_player_info.ranking_value = player_info.ranking_value();
		t_player_info.save_time = player_info.save_time();
		t_player_info.supporting_guid = player_info.supporting_guid();
		t_player_info.set_supporting_name(player_info.supporting_name());
		t_player_info.vip_level = player_info.vip_level();
		t_player_info.Peak = player_info.peak();
		t_player_info.server_id = player_info.server_id();
		t_player_info.set_game_channel(player_info.game_channel());
		t_player_info.standby_parameter = player_info.standby_parameter();

		ranking_mgr_ws::get_player_worship_value_end(request.is_load(), request.sender(), t_player_info);
	}

	void dp2ws_resend_mail_to_sender(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}
		const faith::dp2ws_add_mail_to_sql_failed* packet = static_cast<const faith::dp2ws_add_mail_to_sql_failed*>(data_ptr);
		dp2ws_add_mail_to_sql_failed resp;
		memcpy(&resp, packet, sizeof(dp2ws_add_mail_to_sql_failed));

		guid_64 sender_guid = guid_64(resp.mail_info.data_ary[faith::EMailInfo_SenderGuid1], resp.mail_info.data_ary[faith::EMailInfo_SenderGuid2]);
		client_session* session = client_session_mgr::getInstance().get_session(sender_guid);

		if (session == nullptr)
		{
			return;
		}

		ws2cs_send_mail request;

		request.addressee_guid = sender_guid;
		request.item_num = resp.item_num;
		request.mail_info = resp.mail_info;
		for (int32 i = 0; i < packet->item_num; ++i)
		{
			if (request.item_num >= max_item_per_mail)
			{
				break;
			}
			request.item_list[i] = resp.item_list[i];
		}

		bool is_use = proto_by_lua(e_msgindex_ws2cs_recive_mail);
		if (is_use == false)
		{
			session->send_to_cs(&request, sizeof(request));
		}
		else
		{
			faith::ws2cs_proto::send_mail pro_msg;
			request.to_proto(pro_msg);
			session->send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_recive_mail);
		}

		//session->send_to_cs(&request, sizeof(request));
	}


	void dp2ws_load_mail_event_end_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}

		const faith::dp2ws_load_mail_event_end* packet = static_cast<const faith::dp2ws_load_mail_event_end*>(data_ptr);
		dp2ws_load_mail_event_end resp;
		memcpy(&resp, packet, sizeof(dp2ws_load_mail_event_end));

		event_ws_mgr::get_instance().load_mail_event_end(resp.mail_info, resp.mail_num);


	}
	void dp2ws_load_mail_event_item_end_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (!data_ptr)
		{
			return;
		}

		const faith::dp2ws_load_mail_event_item_end* packet = static_cast<const faith::dp2ws_load_mail_event_item_end*>(data_ptr);
		dp2ws_load_mail_event_item_end resp;
		memcpy(&resp, packet, sizeof(dp2ws_load_mail_event_item_end));

		event_ws_mgr::get_instance().load_mail_event_item_end(resp.item_list, resp.item_num);
	}

	void dp2ws_check_id_mail_end_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_check_id_mail_end* packet = static_cast<const dp2ws_check_id_mail_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		mail_ws_mgr::get_instance().check_id_mail_end(packet->role_guid, packet->server_id, packet->data_array, packet->data_num);
	}

	void dp2ws_load_other_server_mail_record_end_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_other_server_mail_record_end* packet = static_cast<const dp2ws_load_other_server_mail_record_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		mail_ws_mgr::get_instance().load_other_server_mail_record_end(packet->mail_record, packet->data_num);
	}

	void dp2ws_load_other_server_mail_end_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_other_server_mail_end* packet = static_cast<const dp2ws_load_other_server_mail_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		mail_ws_mgr::get_instance().load_other_server_mail_end(packet->server_id, packet->mail_info, packet->data_num);
	}

	void dp2ws_load_all_legion_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_info* packet = static_cast<const dp2ws_load_all_legion_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}

		int32 legion_num = packet->legion_info_num;
		legion_ws_mgr& legion_ws_mgr_ref = legion_ws_mgr::get_instance();
		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();


		if (legion_num <= 0 || world_server::getInstance().is_sky_island_server())
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_load_legion_info);
			return;
		}
		for (int32 i = 0; i < legion_num; ++i)
		{
			const s_legion_info& legion_info = packet->legion_info[i];
			legion_ws loaded_legion(legion_info);
			if (legion_ws_mgr_ref.add_legion(loaded_legion) != 1)
			{
				continue;
			}
		}
		legion_ws_mgr_ref.set_is_load_over(true);
		if (legion_ws_mgr_ref.get_is_need_load())
		{
			city_war_territory_mgr::get_instance().pre_load_bid_info();
			legion_ws_mgr_ref.set_is_need_load(false);
		}
		world_server::getInstance().set_ws_loading_flag(e_ws_flag_load_legion_info);
	}

	void dp2ws_load_all_legion_city_war_member_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_city_war_member_info* packet = static_cast<const dp2ws_load_all_legion_city_war_member_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}

		guid_64 legion_guid = packet->legion_guid;
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		s_legion_city_war_member_info member_info;
		for (int32 i = 0; i < packet->member_info_num; ++i)
		{
			member_info = packet->member_info[i];
			if (!member_info.is_valid())
			{
				continue;
			}
			legion_ws_ptr->add_city_war_member_from_db(member_info);
		}
	}

	void dp2ws_load_all_legion_member_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_member_info* packet = static_cast<const dp2ws_load_all_legion_member_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			_RLOG_(MINFO, "dp2ws_load_all_legion_member_info_process data_len = 0�� ");
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			_RLOG_(MINFO, "dp2ws_load_all_legion_member_info_process data_len != get_pak_length�� ");
			return;
		}

		guid_64 legion_guid = packet->legion_guid;
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			_RLOG_(MINFO, "dp2ws_load_all_legion_member_info_process legion_ws_ptr == null ");
			return;
		}

		s_legion_member_info loaded_member_info;
		for (int32 i = 0; i < packet->member_info_num; ++i)
		{
			loaded_member_info = packet->member_info[i];
			if (loaded_member_info.is_valid() == false)
			{
				_RLOG_(MINFO, "dp2ws_load_all_legion_member_info_process loaded_member_info.guid == null ");
				continue;
			}

			loaded_member_info.data_ary[e_legion_member_info_is_online] = 0;
			if (loaded_member_info.data_ary[e_legion_member_info_last_logout_stamp] <= 0)
			{
				loaded_member_info.data_ary[e_legion_member_info_last_logout_stamp] = time_helper::get_cur_time_new().second;
			}

			if (loaded_member_info.gs_value <= 0 && loaded_member_info.data_ary[e_legion_member_info_fighting_power] > 0)
			{
				loaded_member_info.gs_value = loaded_member_info.data_ary[e_legion_member_info_fighting_power];
			}
			if (loaded_member_info.data_ary[e_legion_member_info_donate_contribution] < 0)
			{
				loaded_member_info.data_ary[e_legion_member_info_donate_contribution] = INT32_MAX;
			}
			if (legion_ws_ptr->add_member(loaded_member_info, true) != 1)
			{
				continue;
			}

			legion_ws_mgr::get_instance().get_boss_award_get_log().load_legion_boss_award_get_log_all(loaded_member_info.role_guid);
		}
		legion_ws_ptr->set_is_load_legion_member_end(true);
		legion_ws_mgr::get_instance().calcu_one_legion_ranks(*legion_ws_ptr);
	}

	void dp2ws_load_all_legion_applicant_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_applicant* packet = static_cast<const dp2ws_load_all_legion_applicant*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}

		if (packet->applicant_info_num <= 0)
		{
			return;
		}

		guid_64 legion_guid = packet->applicant_info[0].legion_guid;
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		legion_ws_applicant_list& applicant_list = legion_ws_ptr->get_applicant_list();
		for (int32 i = 0; i < packet->applicant_info_num; ++i)
		{
			legion_ws_applicant loaded_applicant(packet->applicant_info[i]);
			if (applicant_list.add_applicant(loaded_applicant) != 1)
			{
				continue;
			}
		}
		legion_ws_ptr->send_applicant_info_list_to_have_job_member();
	}

	void dp2ws_load_all_legion_boss_award_get_log_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_boss_award_get_log* packet = static_cast<const dp2ws_load_all_legion_boss_award_get_log*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}

		if (packet->legion_boss_award_get_log.get_logs_num <= 0)
		{
			return;
		}

		legion_ws_boss_award_get_log& boss_award_get_log = legion_ws_mgr::get_instance().get_boss_award_get_log();
		boss_award_get_log.set_role_legion_boss_award_get_log_from_db(packet->legion_boss_award_get_log);
	}

	void dp2ws_load_city_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_city_info* packet = static_cast<const dp2ws_load_city_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}

		guid_64 occupation_legion_guid = packet->overload_info.legion_guid;
		if (occupation_legion_guid.is_valid() == false)
		{
			return;
		}
		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			return;
		}

		std::string master_name(packet->overload_info.city_master_name);
		city_war_mgr.set_overlord_legion(occupation_legion_guid, true);
		if (master_name.empty())
		{
			city_war_mgr.set_city_master_name(city_war_mgr.get_city_master_name_old(), true);
		}
		else
		{
			city_war_mgr.set_city_master_name(packet->overload_info.city_master_name, false);
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(occupation_legion_guid);
		if (nullptr != legion_ws_ptr)
		{
			legion_ws_ptr->add_city_war_territory(overlord_war_territory_flag);
		}
	}

	void dp2ws_load_all_city_bid_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_war_bid_info* packet = static_cast<const dp2ws_load_all_war_bid_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}

		city_war_territory_mgr& city_war_territory = city_war_territory_mgr::get_instance();
		for (int32 i = 0; i < packet->data_num; i++) //kero
		{
			const s_city_war_bid_info& bid_info = packet->bid_infos[i];
			city_war_territory.load_bid_info(bid_info);
		}
		city_war_territory.on_load_bid_info_finish();
		if (world_server::getInstance().get_cross_id() <= 0)
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_city_war);
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_city_war);
				if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
				{
					city_war_territory_mgr::get_instance().send_city_info_all_to_other_ws(0);
				}
				else
				{
					return;
				}
			}
			else
			{
				if (world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war))
				{
					world_server::getInstance().send_to_gate_get_msg(e_ws_flag_city_war);
				}
				else
				{
					world_server::getInstance().set_ws_loading_flag(e_ws_flag_city_war);
				}
			}
		}
	}

	void dp2ws_load_all_legion_event_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_event* packet = static_cast<const dp2ws_load_all_legion_event*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		if (packet->data_num <= 0)
		{
			return;
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(packet->event_datas[0].legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}
		legion_ws_ptr->get_event_logger().recv_load_all_legion_event_from_db(packet->event_datas, packet->data_num);
	}

	void dp2ws_load_legion_bonus_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_legion_bonus_info* packet = static_cast<const dp2ws_load_legion_bonus_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		/*if (data_len != packet->get_pak_length())
		{
			return;
		}*/
		if (packet->data_num <= 0)
		{
			return;
		}
		for (int32 i = 0; i < packet->data_num; i++)
		{
			legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(packet->bonus_info[i].legion_guid);
			if (nullptr == legion_ws_ptr)
			{
				return;
			}
			legion_ws_ptr->get_bonus_info_ins().recv_load_all_legion_bonus_info_from_db(packet->bonus_info[i]);
		}
	}

	void dp2ws_load_legion_warehouse_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_warehouse_info* packet = static_cast<const dp2ws_load_all_legion_warehouse_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		uint32 len = packet->get_pak_length();
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		if (packet->legion_warehouse_num <= 0)
		{
			return;
		}

		guid_64 legion_guid = packet->legion_warehouse[0].legion_guid;

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		for (int32 i = 0; i < packet->legion_warehouse_num; i++)
		{
			legion_ws_ptr->get_warehouse().add_legion_warehouse(packet->legion_warehouse[i], true);
		}
	}

	void dp2ws_load_legion_warehouse_log_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_legion_warehouse_log* packet = static_cast<const dp2ws_load_all_legion_warehouse_log*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		uint32 len = packet->get_pak_length();
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		if (packet->log_num <= 0)
		{
			return;
		}

		guid_64 legion_guid = packet->log[0].legion_guid;

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_legion(legion_guid);
		if (nullptr == legion_ws_ptr)
		{
			return;
		}

		for (int32 i = 0; i < packet->log_num; i++)
		{
			legion_ws_ptr->get_warehouse().add_legion_log(packet->log[i], true);
		}
	}

	void dp2ws_load_all_best_record_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_best_record* packet = static_cast<const dp2ws_load_all_best_record*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}

		if (packet->db_record_num <= 0)
		{
			return;
		}
		if (best_record_mgr::get_instance().is_use_lua())
		{
			dp2ws_load_all_best_record* tmp = const_cast<dp2ws_load_all_best_record*>(packet);
			faith::dp2s_proto::load_all_best_record pro_msg;
			tmp->to_proto(pro_msg);
			packet_s2s* p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "best_record_mgr_load_all_best_record", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}

		best_record_mgr& best_record_mgr_ref = best_record_mgr::get_instance();
		s_single_map_record new_record;
		for (int32 i = 0; i < packet->db_record_num; ++i)
		{
			new_record.role_guid = packet->db_record_list[i].role_guid;
			new_record.set_role_name(packet->db_record_list[i].role_name);
			new_record.value = packet->db_record_list[i].value;
			best_record_mgr_ref.set_best_record(packet->db_record_list[i].map_template_id, new_record, true);
		}
	}

	void dp2ws_load_big_player_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_big_player_end* packet = static_cast<const dp2ws_load_big_player_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_big_player_end))
		{
			return;
		}
		if (big_player_ws_mgr::get_instance().is_use_lua())
		{
			dp2ws_load_big_player_end* tmp = const_cast<dp2ws_load_big_player_end*>(packet);
			faith::dp2s_proto::load_big_player_end pro_msg;
			tmp->to_proto(pro_msg);
			packet_s2s* p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "big_player_mgr_load_data_by_db_end", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;

		}
		big_player_ws_mgr::get_instance().load_data_by_db_end(packet->data_info);
	}

	void dp2ws_load_big_player_base_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_big_player_base_info_end* packet = static_cast<const dp2ws_load_big_player_base_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_big_player_base_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		big_player_ws_mgr::get_instance().handle_base_detail_info(big_type, packet->role_info);
	}

	void dp2ws_load_big_player_skill_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_big_player_skill_info_end* packet = static_cast<const dp2ws_load_big_player_skill_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_big_player_skill_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		big_player_ws_mgr::get_instance().handle_skill_info(big_type, packet->skill_data);
	}
	void dp2ws_load_big_player_att_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_big_player_att_info_end* packet = static_cast<const dp2ws_load_big_player_att_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_big_player_att_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		big_player_ws_mgr::get_instance().handle_att_info(big_type, packet->att_info);
	}
	void dp2ws_load_big_player_item_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_big_player_item_info_end* packet = static_cast<const dp2ws_load_big_player_item_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_big_player_item_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		big_player_ws_mgr::get_instance().handle_item_info(big_type, packet->item_data);
	}

	void dp2ws_load_big_player_buff_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_big_player_buff_end* packet = static_cast<const dp2ws_load_big_player_buff_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 big_type = packet->big_type;
		big_player_ws_mgr::get_instance().handle_buff_info(big_type, packet->buff_data);
	}

	void dp2ws_gm_set_result_proc_lua(uint32 conn_index, int32 listen_port, int64 handle_index, int32 result_code, xstring result_msg)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		web_client::get_instance().gm_order_end(result_code, result_msg, callback_info);
	}

	void dp2ws_gm_set_result_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2s_proto_dp2ws_gm_set_result request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		dp2ws_gm_set_result_proc_lua(conn_index, request.listen_port(), request.handle_index(), request.result_code(), request.result_msg());
	}
	void dp2ws_save_recharge_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		//const dp2ws_save_recharge* packet = static_cast<const dp2ws_save_recharge*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_dp_to_ws_packet_is_null, 0, invalid_payment_type, guid_64(), 0, nullptr);
		//	return;
		//}
		//recharge_mgr_ws::get_instance().db_save_recharge_end(packet->order_id, packet->role_guid, packet->game_goods_id, packet->pay_price, packet->success, packet->order_num, packet->payment_type, packet->client_uid, packet->order_token, packet->direct_diamond);

		if (data_ptr == nullptr)
		{
			return;
		}
		dp2s_proto_dp2ws_save_recharge request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		dp2ws_save_recharge_proc_lua(conn_index, request.order_id(), request.role_guid(), request.game_goods_id(), request.pay_price(), request.success(), request.order_num(), request.payment_type(), request.direct_diamond(), request.client_uid(), request.order_token());
	}

	void dp2ws_save_recharge_proc_lua(uint32 conn_index, xstring order_id, int64 role_guid, int32 game_goods_id, float pay_price, int32 success, int32 order_num, int32 payment_type, int32 direct_diamond, int64 client_uid, xstring order_token)
	{
		xchar	t_order_id[max_recharge_order_length + 1];
		memset(t_order_id, 0, sizeof(t_order_id));
		memcpy(t_order_id, order_id.c_str(), order_id.size() > sizeof(t_order_id) ? sizeof(t_order_id) : order_id.size());

		recharge_mgr_ws::get_instance().db_save_recharge_end(t_order_id, role_guid, game_goods_id, pay_price, success, order_num, payment_type, client_uid, order_token, direct_diamond);
	}

	void dp2ws_send_recharge_end_result_proc_lua(uint32 conn_index, int32 result_type, int64 order_num, int32 payment_type, int32 goods_id, int32 direct_diamond, int64 role_guid, xstring order_id)
	{
		xchar	t_order_id[max_recharge_order_length + 1];
		memset(t_order_id, 0, sizeof(t_order_id));
		memcpy(t_order_id, order_id.c_str(), order_id.size() > sizeof(t_order_id) ? sizeof(t_order_id) : order_id.size());

		recharge_mgr_ws::get_instance().send_recharge_end_result(result_type, order_num, payment_type, role_guid, goods_id, t_order_id);

	}

	void dp2ws_send_recharge_end_result_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2s_proto_dp2ws_send_recharge_end_result request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_dp2ws_send_recharge_end_result_proc_packet_is_null, 0, invalid_payment_type, guid_64(), 0, nullptr);
			return;
		}
		dp2ws_send_recharge_end_result_proc_lua(conn_index, request.result_type(), request.order_num(), request.payment_type(), request.goods_id(), request.direct_diamond()
			, request.role_guid(), request.order_id());

		//const dp2ws_send_recharge_end_result* packet = static_cast<const dp2ws_send_recharge_end_result*>(data_ptr);
		//if (nullptr == packet)
		//{
		//	recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_dp2ws_send_recharge_end_result_proc_packet_is_null, 0, invalid_payment_type, guid_64(), 0, nullptr);
		//	return;
		//}
		//recharge_mgr_ws::get_instance().send_recharge_end_result(packet->result_type, packet->order_num, packet->payment_type, packet->role_guid, packet->goods_id, packet->order_id);
	}
	void dp2ws_gm_change_account_proc_lua(uint32 conn_index, int32 change_res, int32 listen_port, int64 handle_index)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		web_client::get_instance().send_change_account_end(change_res, callback_info);
	}

	void dp2ws_gm_change_account_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2s_proto_dp2ws_gm_change_account request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}

		dp2ws_gm_change_account_proc_lua(conn_index, request.change_res(), request.listen_port(), request.handle_index());
	}

	void dp2ws_gm_get_server_info_proc_lua(uint32 conn_index, int32 acc_num, int32 role_num)
	{
		server_info_gm t_gm_info;
		t_gm_info.acc_num = acc_num;
		t_gm_info.role_num = role_num;
		http_send::get_instance().send_server_state(t_gm_info, client_session_mgr::getInstance().get_game_num());
	}

	void dp2ws_gm_get_server_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2s_proto_dp2ws_query_server_info_gm request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		dp2ws_gm_get_server_info_proc_lua(conn_index,request.acc_num(),request.role_num());
	}

	void dp2ws_load_red_package_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_red_package_end* packet = static_cast<const dp2ws_load_red_package_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		red_package_ws_mgr::get_instance().load_red_package_end(packet->red_package_list, packet->data_num);
	}
	void lua_dp2ws_load_red_package_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		faith::dp2s_proto::load_red_package_end msg;       
	    bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len); 
	    if (is_sucess == false)   
	    {                        
		 return;               
	    }                          
		dp2ws_load_red_package_end packet; 
	    packet.from_proto(msg);          
		dp2ws_load_red_package_end_proc(conn_index, &packet, sizeof(dp2ws_load_red_package_end)); 

	}

	void dp2ws_load_red_package_receiver_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_red_package_receiver_end* packet = static_cast<const dp2ws_load_red_package_receiver_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		red_package_ws_mgr::get_instance().load_red_package_receiver_end(packet->receiver_list, packet->data_num);
	}

	void lua_dp2ws_load_red_package_receiver_end_proc(uint32 conn_index, const char * data_ptr, int32 data_len)
	{
		faith::dp2s_proto::load_red_package_receiver_end msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (is_sucess == false)
		{
			return;
		}
		dp2ws_load_red_package_receiver_end packet;
		packet.from_proto(msg);
		dp2ws_load_red_package_receiver_end_proc(conn_index, &packet, sizeof(dp2ws_load_red_package_receiver_end));
	}

	void dp2ws_load_lucky_draw_record_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_draw_record_by_db_end* packet = static_cast<const dp2ws_load_draw_record_by_db_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		lucky_draw_record_ws_mgr::get_instance().load_data_by_db_end(packet->data_info, packet->data_num);
	}

	void dp2ws_rep_load_relationlist_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_load_player_relation* load_player_relation = static_cast<const dp2ws_load_player_relation*>(data_ptr);
		if (nullptr == load_player_relation)
		{
			return;
		}

		client_session* session_player = client_session_mgr::getInstance().get_session(load_player_relation->role_guid);
		if (nullptr == session_player)
		{
			return;
		}
		if (false == session_player->is_vaild())
		{
			return;
		}
		session_player->get_relation_list_mgr().clear_relation_list();
		session_player->get_relation_list_mgr().load_relation_from_db(load_player_relation->relation_array, load_player_relation->data_num);
		session_player->get_relation_list_mgr().update_relation_state(true, false);
		session_player->get_relation_list_mgr().sync_relation_list_to_client();
		session_player->get_relation_list_mgr().sync_full_relation_list_to_cs(false);
	}

	void dp2ws_rep_load_relation_info_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_load_relation_info* load_msg_data = static_cast<const dp2ws_load_relation_info*>(data_ptr);
		if (nullptr == load_msg_data)
		{
			return;
		}

		client_session* session_player = client_session_mgr::getInstance().get_session(load_msg_data->sender_guid);
		if (nullptr == session_player)
		{
			return;
		}

		if (load_msg_data->other_relation.relation_guid == load_msg_data->sender_guid)
		{
			return;
		}
		session_player->get_relation_list_mgr().recv_other_relation_info_from_db(load_msg_data->other_relation);
	}

	void dp2ws_add_relation_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_add_relation* packet = static_cast<const dp2ws_add_relation*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		if (1 == packet->result)
		{
			return;
		}

		if (packet->relation_type == e_relationlist_type_friend)
		{
			client_session* session_ptr = client_session_mgr::getInstance().get_session(packet->relation_guid);
			if (nullptr != session_ptr)
			{
				session_ptr->get_relation_list_mgr().del_relation(packet->role_guid);
			}
			else
			{
				//ws2dp_del_relation msg;
				//msg.role_guid = packet->relation_guid;
				//msg.target_role_guid = packet->role_guid;
				//msg.relation_type = e_relationlist_type_friend;

				//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));

				script_mgr::get_instance().call_func("ws2dp_send", "send_ws2dp_req_del_relation_lua", 0, false, "%l%l%d", packet->relation_guid.server_64, packet->role_guid.server_64, (int32)e_relationlist_type_friend);
			}
		}
	}

	void dp2ws_rep_load_relation_info_other_server_end(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2ws_load_relation_info_other_server* load_msg_data = static_cast<const dp2ws_load_relation_info_other_server*>(data_ptr);
		if (nullptr == load_msg_data)
		{
			return;
		}
		ws2ws_load_offline_other_relation_end msg;
		msg.sender_guid = load_msg_data->sender_guid;
		msg.other_relation = load_msg_data->other_relation;
		cross::send_msg_to_ws(guid_64(), load_msg_data->sender_server_id, e_msgindex_ws2ws_load_offline_other_relation_end, &msg, sizeof(msg));
	}

	void dp2ws_rep_auction_find_buy_target_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_auction_find_buy_target_end* find_msg_data = static_cast<const dp2ws_auction_find_buy_target_end*>(data_ptr);
		if (nullptr == find_msg_data)
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			faith::dp2s_proto::auction_find_buy_target_end pro_msg;
			dp2ws_auction_find_buy_target_end* tmp = const_cast<dp2ws_auction_find_buy_target_end *>(find_msg_data);
			tmp->to_proto(pro_msg);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_find_buy_target_end", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}
		auction_mgr_ws::get_instance().find_buy_target_end(find_msg_data->auction_info, find_msg_data->role_guid, find_msg_data->item_guid);
	}

	void dp2ws_rep_auction_cancel_sell_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_auction_cancel_sell* find_msg_data = static_cast<const dp2ws_auction_cancel_sell*>(data_ptr);
		if (nullptr == find_msg_data)
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			faith::dp2s_proto::auction_cancel_sell pro_msg;
			dp2ws_auction_cancel_sell* tmp = const_cast<dp2ws_auction_cancel_sell *>(find_msg_data);
			tmp->to_proto(pro_msg);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_cancel_sell_end", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}
		auction_mgr_ws::get_instance().cancel_sell_end(find_msg_data->auction_info, find_msg_data->role_guid);
	}

	void dp2ws_rep_auction_sell_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_auction_sell_end* send_end_msg = static_cast<const dp2ws_auction_sell_end*>(data_ptr);
		if (nullptr == send_end_msg)
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			faith::dp2s_proto::auction_sell_end pro_msg;
			dp2ws_auction_sell_end* tmp = const_cast<dp2ws_auction_sell_end *>(send_end_msg);
			tmp->to_proto(pro_msg);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_sell_end", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}
		auction_mgr_ws::get_instance().sell_end(send_end_msg->info, send_end_msg->operate_result, send_end_msg->role_info, send_end_msg->is_add_to_log, send_end_msg->third_info);
	}

	void dp2ws_rep_auction_find_bid_target_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_auction_find_bid_target_end* send_end_msg = static_cast<const dp2ws_auction_find_bid_target_end*>(data_ptr);
		if (nullptr == send_end_msg)
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			faith::dp2s_proto::auction_find_bid_target_end pro_msg;
			dp2ws_auction_find_bid_target_end* tmp = const_cast<dp2ws_auction_find_bid_target_end *>(send_end_msg);
			tmp->to_proto(pro_msg);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_find_bid_end_proc", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}
		auction_mgr_ws::get_instance().find_bid_end_proc(send_end_msg->role_guid, send_end_msg->item_guid, send_end_msg->auction_info, send_end_msg->price_money_type, send_end_msg->price_money_value);
	}

	void dp2ws_auction_clear_selling_lock_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_auction_clear_selling_lock* packet = static_cast<const dp2ws_auction_clear_selling_lock*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_auction_clear_selling_lock))
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

	void dp2ws_auction_clear_item_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_auction_clear_old_item_end* packet = static_cast<const dp2ws_auction_clear_old_item_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			faith::dp2s_proto::auction_clear_old_item_end pro_msg;
			dp2ws_auction_clear_old_item_end* tmp = const_cast<dp2ws_auction_clear_old_item_end *>(packet);
			tmp->to_proto(pro_msg);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_get_old_item_end_proc", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}
		auction_mgr_ws::get_instance().get_old_item_end_proc(packet->data_info, packet->data_num);
	}

	void dp2ws_req_share_list_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_auction_req_share_list_end* packet = static_cast<const dp2ws_auction_req_share_list_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != packet->get_pak_length())
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			faith::dp2s_proto::auction_req_share_list_end pro_msg;
			dp2ws_auction_req_share_list_end* tmp = const_cast<dp2ws_auction_req_share_list_end*>(packet);
			tmp->to_proto(pro_msg);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_req_share_list_end_proc", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;

		}
		auction_mgr_ws::get_instance().req_share_list_end_proc(packet->money_type, packet->money_value, packet->item_template_id, packet->data_info, packet->data_num, packet->buyer_guid);
	}

	void dp2ws_auction_check_legion_selling_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_auction_check_legion_selling_end* packet = static_cast<const dp2ws_auction_check_legion_selling_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_auction_check_legion_selling_end))
		{
			return;
		}
		legion_ws* temp_legion = legion_ws_mgr::get_instance().get_legion(packet->legion_guid);
		if (temp_legion == nullptr)
		{
			return;
		}
		temp_legion->check_legion_selling_end(packet->selling_num);
	}

	void dp2ws_check_world_auction_sell(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_auction_check_world_sell_end* packet = static_cast<const dp2ws_auction_check_world_sell_end*>(data_ptr);;
		if (NULL == packet)
		{
			return;
		}

		if (data_len != sizeof(dp2ws_auction_check_world_sell_end))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auciton_mgr_dp_check_world_auction_sell", 0, false, "%d", packet->sell_num);
			return;

		}

		auction_mgr_ws::get_instance().dp_check_world_auction_sell_end(packet->sell_num);



		/*	auction_mgr_ws::get_instance().set_world_auction_sell_num(packet->sell_num);

			bool is_world_sell = packet->sell_num > 0 ? true : false;
			auction_mgr_ws::get_instance().set_need_world_acution_check(is_world_sell);

			auction_proto_auction_sell_flag msg;
			msg.set_sell_flag(is_world_sell);
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_auction_sell_flag);*/
	}

	void dp2ws_auction_show_sell_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_auction_show_sell_end* packet = static_cast<const dp2ws_auction_show_sell_end*>(data_ptr);;
		if (NULL == packet)
		{
			return;
		}

		if (data_len != sizeof(dp2ws_auction_show_sell_end))
		{
			return;
		}

		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			dp2ws_auction_show_sell_end* tmp = const_cast<dp2ws_auction_show_sell_end*>(packet);
			packet_s2s* p_s2s = auction_mgr_ws::get_instance().get_auction_buffer(tmp->data_info);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_send_to_dp_show_end", 0, true, "%d%s", packet->result, p_s2s->google_data, p_s2s->google_data_len);
			return;
		}


		auction_mgr_ws::get_instance().send_to_dp_show_end(packet->result, packet->data_info);

	}

	void dp2ws_auction_time_out_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_aution_time_out_end* packet = static_cast<const dp2ws_aution_time_out_end*>(data_ptr);
		if (NULL == packet)
		{
			return;
		}

		if (data_len != packet->get_pak_length())
		{
			return;
		}


		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			faith::dp2s_proto::aution_time_out_end pro_msg;
			dp2ws_aution_time_out_end* tmp = const_cast<dp2ws_aution_time_out_end*>(packet);
			tmp->to_proto(pro_msg);
			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_send_to_dp_time_out_end", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;
		}

		auction_mgr_ws::get_instance().send_to_dp_time_out_end(packet->data_info, packet->data_num);

	}

	void db2ws_auction_bid_db_sucess_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_auction_bid_db_sucess *packet = static_cast<const dp2ws_auction_bid_db_sucess *>(data_ptr);
		if (NULL == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_auction_bid_db_sucess))
		{
			return;
		}
		if (auction_mgr_ws::get_instance().is_use_lua())
		{
			script_mgr::get_instance().call_func(nullptr, "auction_mgr_auction_bid_db_sucess", 0, false, "%l%l", packet->buy_guid.server_64, packet->item_guid.server_64);
			return;
		}
		auction_mgr_ws::get_instance().auction_bid_db_sucess(packet->buy_guid, packet->item_guid);
	}

	void dp2ws_rep_marry_couple_info_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_load_marry_couple_info_end* packet = static_cast<const dp2ws_load_marry_couple_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_marry_couple_info_end))
		{
			return;
		}

		marry_mgr_ws::get_instance().load_data_end(packet->load_data, packet->load_num);
	}


	void dp2ws_ret_marry_couple_last_login_time_end(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_marry_get_last_login_time_end* packet = static_cast<const dp2ws_marry_get_last_login_time_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_marry_get_last_login_time_end))
		{
			return;
		}
		marry_mgr_ws::get_instance().dp2ws_ret_last_login_time_end(packet->sender_guid, packet->last_login);
	}

	void dp2ws_load_marry_info_cross_server_end_proc(uint32 conn_index, const void *data_ptr, size_t data_len)
	{
		const dp2ws_load_marry_info_cross_server_end* packet = static_cast<const dp2ws_load_marry_info_cross_server_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_marry_info_cross_server_end))
		{
			return;
		}
		client_session* session_player = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr != session_player)
		{
			relation_proto_s2c_update_marry_info msg;
			if(packet->role_guid == packet->data_one.main_couple_guid)
				msg.set_couple_name(packet->data_one.sub_couple_name);
			else
				msg.set_couple_name(packet->data_one.main_couple_name);
			msg.set_marry_time(packet->data_one.save_event_time);
			session_player->send_to_client(&msg, e_msgindex_s2c_update_marry_info);
		}
	}

	void dp2ws_load_server_attr_val_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_server_attr_val_end* packet = static_cast<const dp2ws_load_server_attr_val_end*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		if (packet->attr_num < 0)
		{
			return;
		}

		int32 stamp_z = 0;
		int32 stamp_f = 0;
		int32 world_lv_cur = 0;
		int32 world_lv_last = 0;
		int32 service_rank_check_index = 0;

		int32 stamp_h = 0;
		int32 is_refresh_cross_city_war = 0;
		int32 total_recharge_num = 0;

		int32 week_contribution_refresh_time = 0;
		int32 week_legion_welfare_refresh_time = 0;
		int32 clear_city_war_bid_time = 0;
		int32 last_broken_sky_level = 0;
		int32 ladder_world_level = 0;
		for (int32 i = 0; i < packet->attr_num; i++)
		{
			switch (packet->attr_int_val[i].attr_type)
			{
			case e_srv_attr_val_refresh_zero:
				stamp_z = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_refresh_five:
				stamp_f = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_world_lv_cur:
				world_lv_cur = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_world_lv_last:
				world_lv_last = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_service_rank_check:
				service_rank_check_index = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_server_harry_refresh_time:
				stamp_h = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_is_refresh_cross_city_war:
				is_refresh_cross_city_war = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_total_time_limit_activity_recharge:
				total_recharge_num = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_broken_sky_boss_level_last:
				last_broken_sky_level = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_week_contribution_refresh_time:
				week_contribution_refresh_time = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_clear_city_war_bid_time:
				clear_city_war_bid_time = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_week_welfare_refresh_time:
				week_legion_welfare_refresh_time = packet->attr_int_val[i].attr_val;
				break;
			case e_srv_attr_val_ladder_world_level:
				ladder_world_level = packet->attr_int_val[i].attr_val;
				break;
			default:
				break;
			}
		}
		world_server::getInstance().set_server_time_zero(stamp_z);
		world_server::getInstance().set_server_time_five(stamp_f);
		world_server::getInstance().set_refresh_harry_time(stamp_h);
		world_server::getInstance().set_week_refresh_time(week_contribution_refresh_time);
		world_server::getInstance().set_week_twenty_three_refresh_time(week_legion_welfare_refresh_time);
		world_server::getInstance().set_clear_bid_time(clear_city_war_bid_time);
		ranking_mgr_ws::load_world_level_from_db(world_lv_cur, world_lv_last);
		ranking_mgr_ws::set_service_rank_check_index(service_rank_check_index);
		cross_server_city_war_ws_mgr::get_instance().set_refresh_cross_city_war(is_refresh_cross_city_war);
		broken_sky_mgr::get_instance().set_boss_level(last_broken_sky_level);
		world_server::getInstance().set_ws_loading_flag(e_ws_flag_server_refresh);

		if (ladder_world_level == 0)
		{
			world_server::getInstance().set_ladder_world_level(0, false, true);
		}
		else
		{
			world_server::getInstance().set_ladder_world_level(ladder_world_level, false, false);
		}
		
	}

	void dp2ws_load_all_gain_treasure_record_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_all_gain_treasure_record_info* packet = static_cast<const dp2ws_load_all_gain_treasure_record_info*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		if (packet->data_num < 0)
		{
			return;
		}
		if (gain_treasure_ws_mgr::get_instance().is_use_lua())
		{
			dp2ws_load_all_gain_treasure_record_info *tmp = const_cast<dp2ws_load_all_gain_treasure_record_info*>(packet);
			faith::dp2s_proto::load_all_gain_treasure_record_info pro_msg;
			tmp->to_proto(pro_msg);

			packet_s2s *p_s2s = parse_msg::getInstance().serialze_buffer(&pro_msg);
			if (p_s2s == nullptr)
			{
				return;
			}
			script_mgr::get_instance().call_func(nullptr, "gain_treasure_mgr_load_gain_treasure_record_info_from_db_proc", 0, true, "%s", p_s2s->google_data, p_s2s->google_data_len);
			return;


		}

		gain_treasure_ws_mgr::get_instance().load_gain_treasure_record_info_from_db_proc(packet->record_infos, packet->data_num);
	}

	void dp2ws_rep_cloud_shop_role_record_end(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_cloud_shop_role_record_end* packet = static_cast<const dp2ws_load_cloud_shop_role_record_end*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}

		cloud_shop_mgr::get_instance().load_role_record_data(packet->load_data, packet->load_num, packet->is_end);
	}

	void dp2ws_check_player_name_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_check_player_name_end* packet = static_cast<const dp2ws_check_player_name_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}

		client_session* session_player = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == session_player)
		{
			return;
		}

		rename_mgr::get_instance().check_role_name_end(packet->role_guid.server_64, packet->role_name, packet->result);

	}

	void dp2ws_change_player_name_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_change_player_name_end* packet = static_cast<const dp2ws_change_player_name_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		xstring role_name(packet->role_name);

		rename_mgr::get_instance().change_player_name_end(packet->role_guid.server_64, role_name);
	}

	void dp2ws_load_chat_with_offline_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_chat_with_offline* packet = static_cast<const dp2ws_load_chat_with_offline*>(data_ptr);
		if (nullptr == packet || 0 == data_len)
		{
			return;
		}

		client_session* client_session_ptr = client_session_mgr::getInstance().get_session(packet->role_guid);
		if (nullptr == client_session_ptr)
		{
			return;
		}

		for (int32 i = 0; i < packet->data_num; ++i)
		{
			e_chat_offline_info chat_info = packet->chat_info_list[i];
			chat_proto_chat_content msg;
			msg.set_chat_typ(chat_info.chat_type);
			msg.set_sender_guid(chat_info.sender_guid.server_64);
			msg.set_sender_template_id(chat_info.sender_templete_id);
			msg.set_sender_name(chat_info.sender_name);
			msg.set_relation_template_id(chat_info.relation_template_id);
			msg.set_relation_level(chat_info.relation_level);
			msg.set_chat_guid(chat_info.chat_guid);
			msg.set_vip_title_template_id(chat_info.vip_title_template_id);
			
			std::vector<xstring> chat_content_vec;
			init_unit::explode(chat_info.content_text, chat_content_vec);
			for (int32 j = 0; j < chat_content_vec.size(); ++j)
			{
				chat_proto_chat_content_info* temp_info_msg = msg.add_content_info_array();

				temp_info_msg->set_commontext(chat_content_vec[j]);
			}
			msg.set_is_finish_send(true);
			msg.add_addressee_guid(chat_info.address_guid.A);
			msg.add_addressee_guid(chat_info.address_guid.B);

			if (!client_session_ptr->get_relation_list_mgr().is_relation_exist(e_relationlist_type_chat, chat_info.sender_guid))
			{
				client_session* target_client_session_ptr = client_session_mgr::getInstance().get_session(chat_info.sender_guid);
				if (nullptr != target_client_session_ptr)
				{

					client_session_ptr->get_relation_list_mgr().add_chat(target_client_session_ptr);
				}
				else
				{
					client_session_ptr->get_relation_list_mgr().add_relation(e_relationlist_type_chat, chat_info.sender_guid);
				}
			}

			client_session_ptr->get_relation_list_mgr().recv_chat(chat_info.sender_guid);

			client_session_ptr->send_to_client(&msg, e_msgindex_s2c_receive_chat_new);
		}

		ws2dp_eliminate_chat_with_offline msg;
		msg.role_guid = client_session_ptr->get_role_guid();

		ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
	}

	void dp2ws_load_all_cross_server_pk_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (world_server::getInstance().get_cross_id() <= 0 || !world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_pk))
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_pk);
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				const dp2ws_load_cross_server_pk_msg_all* packet = static_cast<const dp2ws_load_cross_server_pk_msg_all*>(data_ptr);
				if (nullptr == packet || 0 == data_len)
				{
					world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_pk);
					cross_server_pk_ws_mgr::get_instance().send_winner_fake_player_to_other_server();
					return;
				}
				int32 data_num = packet->data_num;

				s_dp2ws_island_simple_msg simple_msg;
				for (int32 i = 0; i < data_num; i++)
				{
					simple_msg.clear_data();
					simple_msg = packet->simple_msg[i];
					cross_server_pk_ws_mgr::get_instance().recv_dp_msg_to_ws(simple_msg);
				}
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_pk);
				cross_server_pk_ws_mgr::get_instance().send_winner_fake_player_to_other_server();
			}
			else
			{
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_cross_pk);
			}
		}
	}

	void dp2ws_load_all_cross_server_harry_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_cross_server_harry_msg_all* packet = static_cast<const dp2ws_load_cross_server_harry_msg_all*>(data_ptr);
		if (nullptr == packet || 0 == data_len)
		{
			return;
		}
		int32 data_num = packet->data_num;
		bool is_empty = packet->is_empty_msg;
		if (world_server::getInstance().get_cross_id() <= 0 || !world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_harry))
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				cross_server_harry_ws_mgr::get_instance().recv_load_all_server_harry_msg(is_empty, data_num, packet->harry_msg);
			}
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_harry);
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				cross_server_harry_ws_mgr::get_instance().recv_load_all_server_harry_msg(is_empty, data_num, packet->harry_msg);
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_harry);
				cross_server_harry_ws_mgr::get_instance().send_all_harry_msg_to_other_server(0, true);
			}
			else
			{
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_harry);
			}
		}
	}

	void dp2ws_load_harry_player_base_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_harry_player_base_info_end* packet = static_cast<const dp2ws_load_harry_player_base_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_harry_player_base_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		cross_server_harry_ws_mgr::get_instance().handle_base_detail_info(big_type, packet->role_info);
	}

	void dp2ws_load_harry_player_skill_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_harry_player_skill_info_end* packet = static_cast<const dp2ws_load_harry_player_skill_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_harry_player_skill_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		cross_server_harry_ws_mgr::get_instance().handle_skill_info(big_type, packet->skill_data);
	}
	void dp2ws_load_harry_player_att_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_harry_player_att_info_end* packet = static_cast<const dp2ws_load_harry_player_att_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_harry_player_att_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		cross_server_harry_ws_mgr::get_instance().handle_att_info(big_type, packet->att_info);
	}
	void dp2ws_load_harry_player_fight_att_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_harry_player_fight_att_info_end* packet = static_cast<const dp2ws_load_harry_player_fight_att_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_harry_player_fight_att_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		cross_server_harry_ws_mgr::get_instance().handle_fight_att_info(big_type, packet->att_info);
	}
	void dp2ws_load_harry_player_item_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_harry_player_item_info_end* packet = static_cast<const dp2ws_load_harry_player_item_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		if (data_len != sizeof(dp2ws_load_harry_player_item_info_end))
		{
			return;
		}
		int32 big_type = packet->big_type;
		cross_server_harry_ws_mgr::get_instance().handle_item_info(big_type, packet->item_data);
	}

	void dp2ws_load_harry_player_buff_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_harry_player_buff_end* packet = static_cast<const dp2ws_load_harry_player_buff_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 big_type = packet->big_type;
		cross_server_harry_ws_mgr::get_instance().handle_buff_info(big_type, packet->buff_data);
	}

	void dp2ws_load_harry_player_belief_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_harry_player_belief_end* packet = static_cast<const dp2ws_load_harry_player_belief_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 big_type = packet->big_type;
		cross_server_harry_ws_mgr::get_instance().handle_belief_info(big_type, packet->data_list);
	}

	void dp2ws_load_cross_server_world_boss_info_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		if (world_server::getInstance().get_cross_id() <= 0)
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_boss);
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				const dp2ws_load_cross_server_world_boss_msg_all* packet = static_cast<const dp2ws_load_cross_server_world_boss_msg_all*>(data_ptr);
				if (nullptr == packet || data_len == 0)
				{
					return;
				}
				cross_server_world_boss_ws_mgr::get_instance().locd_boss_show_info(packet->dp_info, packet->data_num);
				cross_server_world_boss_ws_mgr::get_instance().send_cross_boss_info_all_to_other_ws(0);
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_cross_boss);
			}
			else
			{
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_cross_boss);
			}
		}
	}

	void dp2ws_load_all_ws_time_limit_act_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2ws_load_time_limit_ws_info_end* packet = static_cast<const dp2ws_load_time_limit_ws_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		int32 data_num = packet->data_num;
		time_limit_activity_ws_mgr::get_instance().load_end_time_limit_act(packet->act_info, packet->data_num, packet->is_end);
	}

	void dp2ws_load_time_limit_temp_end_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_time_limit_temp_end* packet = static_cast<const dp2ws_load_time_limit_temp_end*>(data_ptr);
		if (NULL == packet)
		{
			return;
		}
		time_limit_activity_temp_ws_mgr::get_instance().load_temp_end(packet->temp_info, packet->is_load_all, packet->data_num);
	}

	void dp2ws_load_time_limit_branch_temp_end_func(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_time_limit_branch_temp_end* packet = static_cast<const dp2ws_load_time_limit_branch_temp_end*>(data_ptr);
		if (NULL == packet)
		{
			return;
		}
		time_limit_activity_temp_ws_mgr::get_instance().load_branch_temp_end(packet->temp_info, packet->is_load_all, packet->data_num);
	}

	void cs2ws_sync_oracle_trial_rank_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const cs2ws_sync_oracle_trial_rank* packet = static_cast<const cs2ws_sync_oracle_trial_rank*>(data_ptr);
		if (nullptr == packet || data_len == 0)
		{
			return;
		}
		ranking_mgr_ws::sync_oracle_trial_play_info(packet->role_guid, packet->rank_value);
	}

	void dp2ws_load_legion_cross_city_info_process(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (world_server::getInstance().get_cross_id() <= 0 || !world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_city_war))
		{
			const dp2ws_load_cross_city_info_all* packet = static_cast<const dp2ws_load_cross_city_info_all*>(data_ptr);
			if (nullptr == packet || data_len == 0)
			{
				return;
			}
			bool is_empty_msg = packet->is_empty_msg;
			int32 data_num = packet->data_num;
			cross_server_city_war_ws_mgr::get_instance().req_load_db_msg(packet->city_info_msg, data_num, is_empty_msg);
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_overload_war);
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				const dp2ws_load_cross_city_info_all* packet = static_cast<const dp2ws_load_cross_city_info_all*>(data_ptr);
				if (nullptr == packet || data_len == 0)
				{
					return;
				}
				bool is_empty_msg = packet->is_empty_msg;
				int32 data_num = packet->data_num;
				cross_server_city_war_ws_mgr::get_instance().req_load_db_msg(packet->city_info_msg, data_num, is_empty_msg);
				cross_server_city_war_ws_mgr::get_instance().send_city_info_all_to_other_ws();
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_overload_war);
			}
			else
			{
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_overload_war);
			}
		}
	}

	void dp2ws_cross_ladder_load_role_info_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_cross_ladder_load_role_info* packet = static_cast<const dp2ws_cross_ladder_load_role_info*>(data_ptr);
		if (nullptr == packet)
			return;
		for (int32 i = 0; i < packet->cur_data_num; ++i)
			cross_ladder_ws_mgr_new::get_instance().add_cross_ladder_info(packet->data_array[i]);
		if (packet->is_load_all)
			cross_ladder_ws_mgr_new::get_instance().set_is_load_all(true);
	}
	void dp2ws_save_appearance_info_to_id_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_save_appearance_info_to_id_end* packet = static_cast<const dp2ws_save_appearance_info_to_id_end*>(data_ptr);
		if (nullptr == packet)
			return;
		appearance_info_mgr::get_instance().req_save_appearance_info_to_id_end(packet->client_uid, packet->appearance_info, packet->appearance_id);
	}
	void dp2ws_load_appearance_info_by_id_end_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_load_appearance_info_by_id_end* packet = static_cast<const dp2ws_load_appearance_info_by_id_end*>(data_ptr);
		if (nullptr == packet)
			return;
		appearance_info_mgr::get_instance().req_load_appearance_info_by_id_end(packet->client_uid, packet->appearance_id, packet->appearance_info);
	}
	void dp2ws_element_war_load_role_info_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2ws_element_war_load_role_info* packet = static_cast<const dp2ws_element_war_load_role_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		for (int32 i = 0; i < packet->cur_data_num; ++i)
		{
			element_war_ws_mgr::get_instance().add_element_war_info(packet->data_array[i]);
		}
		if (packet->is_load_all)
		{
			element_war_ws_mgr::get_instance().set_is_load_all(true);
		}
	}

	void dp2ws_attack_city_load_legion_info_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2ws_attack_city_load_legion_info_end* packet = static_cast<const dp2ws_attack_city_load_legion_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		
		if (world_server::getInstance().get_cross_id() <= 0)
		{
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_attack_city);
		}
		else
		{
			if (world_server::getInstance().is_sky_island_server())
			{
				const dp2ws_attack_city_load_legion_info_end* packet = static_cast<const dp2ws_attack_city_load_legion_info_end*>(data_ptr);
				if (nullptr == packet || data_len == 0)
				{
					return;
				}
				attack_city_ws_mgr::get_instance().load_group_data_end(packet->info_list, packet->data_num);
				attack_city_ws_mgr::get_instance().send_info_all_to_other_ws(0);
				world_server::getInstance().set_ws_loading_flag(e_ws_flag_attack_city);
			}
			else
			{
				world_server::getInstance().send_to_gate_get_msg(e_ws_flag_attack_city);
			}
		}
	}

	void dp2ws_attack_city_load_rank_info_end_proc(uint32 conn_index, const void * data_ptr, size_t data_len)
	{
		const dp2ws_attack_city_load_rank_info_end* packet = static_cast<const dp2ws_attack_city_load_rank_info_end*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		attack_city_ws_mgr::get_instance().clear_rank_data();
		for (int32 i = 0; i < packet->data_num; ++i)
		{
			attack_city_ws_mgr::get_instance().add_rank_data(packet->info_list[i], false);
		}
	}
	
	void dp2ws_gm_get_role_info_proc_lua(uint32 conn_index, const char* data_ptr, size_t data_len, int32 listen_port, int64 handle_index, int32 check_type)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_dp2ws_gm_get_role_info_arr msg;
		bool is_sucess = parse_msg::getInstance().parse_buffer_to_proto(&msg, data_ptr, data_len);
		if (!is_sucess)
		{
			return;
		}

		int32 row_count = msg.data_num();

		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_unit_info));
		if (p_data == nullptr)
		{
			return;
		}

		s_unit_info *p_row = (s_unit_info *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_unit_info one_row = msg.role_info(i);

			p_row->role_guid = one_row.role_guid();
			parse_msg::getInstance().my_memcopy(p_row->account, max_account_length, one_row.account().c_str(), one_row.account().size());
			parse_msg::getInstance().my_memcopy(p_row->role_name, max_name_size, one_row.role_name().c_str(), one_row.role_name().size());
			parse_msg::getInstance().my_memcopy(p_row->role_appearance, max_appearance_size, one_row.role_appearance().c_str(), one_row.role_appearance().size());
			parse_msg::getInstance().my_memcopy(p_row->role_first_kill_npc_id_arr, max_first_kill_npc_id_str, one_row.role_first_kill_npc_id_arr().c_str(), one_row.role_first_kill_npc_id_arr().size());
			p_row->map_guid = one_row.map_guid();

			for (int32 i = 0; i < one_row.data_i64_ary_size(); i++)
			{
				if (i >= e_role_i64_info_max)
				{
					break;
				}
				p_row->data_i64_ary[i] = one_row.data_i64_ary(i);
			}
			for (int32 i = 0; i < one_row.data_ary_size(); i++)
			{
				if (i >= e_role_info_max)
				{
					break;
				}
				p_row->data_ary[i] = one_row.data_ary(i);
			}
			p_row++;
		}

		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		web_client::get_instance().set_role_info((s_unit_info *)p_data, row_count, callback_info, check_type);

	}

	void dp2ws_gm_get_role_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (nullptr == data_ptr)
		{
			return;
		}
		dp2s_proto_dp2ws_gm_get_role_info all_msg;
		if (!parse_msg::getInstance().parse_message_server(&all_msg, data_ptr, data_len))
		{
			return;
		}

		const dp2s_proto_dp2ws_gm_get_role_info_arr& msg = all_msg.role_info_arr();
		int32 row_count = msg.data_num();
		xchar* p_data = parse_msg::getInstance().get_cache(row_count * sizeof(s_unit_info));
		if (p_data == nullptr)
		{
			return;
		}
		s_unit_info *p_row = (s_unit_info *)p_data;
		for (int32 i = 0; i < row_count; i++)
		{
			if (nullptr == p_row)
			{
				break;
			}
			dp2s_proto_s_unit_info one_row = msg.role_info(i);

			p_row->role_guid = one_row.role_guid();
			parse_msg::getInstance().my_memcopy(p_row->account, max_account_length, one_row.account().c_str(), one_row.account().size());
			parse_msg::getInstance().my_memcopy(p_row->role_name, max_name_size, one_row.role_name().c_str(), one_row.role_name().size());
			parse_msg::getInstance().my_memcopy(p_row->role_appearance, max_appearance_size, one_row.role_appearance().c_str(), one_row.role_appearance().size());
			parse_msg::getInstance().my_memcopy(p_row->role_first_kill_npc_id_arr, max_first_kill_npc_id_str, one_row.role_first_kill_npc_id_arr().c_str(), one_row.role_first_kill_npc_id_arr().size());
			p_row->map_guid = one_row.map_guid();

			for (int32 i = 0; i < one_row.data_i64_ary_size(); i++)
			{
				if (i >= e_role_i64_info_max)
				{
					break;
				}
				p_row->data_i64_ary[i] = one_row.data_i64_ary(i);
			}
			for (int32 i = 0; i < one_row.data_ary_size(); i++)
			{
				if (i >= e_role_info_max)
				{
					break;
				}
				p_row->data_ary[i] = one_row.data_ary(i);
			}
			p_row++;
		}

		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = all_msg.handle_index();
		callback_info.listen_port = all_msg.listen_port();

		web_client::get_instance().set_role_info((s_unit_info *)p_data, row_count, callback_info, all_msg.check_type());
	}

	void dp2ws_gm_modify_role_info_proc_lua(uint32 conn_index, int64 role_guid, int32 listen_port, int64 handle_index)
	{
		s_daemon_callback_info callback_info;
		callback_info.clear_data();
		callback_info.handle_index = handle_index;
		callback_info.listen_port = listen_port;

		web_client::get_instance().db_role_info_modified(role_guid, callback_info);
	}

	void dp2ws_gm_modify_role_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		dp2s_proto_dp2ws_gm_modify_role_info request;
		if (!parse_msg::getInstance().parse_message_server(&request, data_ptr, data_len))
		{
			return;
		}
		dp2ws_gm_modify_role_info_proc_lua(conn_index, request.role_guid(), request.listen_port(), request.handle_index());
	}

	void dp2ws_gm_get_role_time_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_time_info* packet = static_cast<const dp2cs_load_time_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		web_client::get_instance().set_role_time(packet->role_guid, packet->data_info, packet->callback_info.handle_index);
	}
	void dp2ws_gm_get_role_money_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_money_info* packet = static_cast<const dp2cs_load_money_info*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		web_client::get_instance().set_role_money(packet->role_guid, packet->data_info, packet->callback_info.handle_index);
	}
	void dp2ws_gm_get_role_lock_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2ws_gm_get_role_lock* packet = static_cast<const dp2ws_gm_get_role_lock*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		web_client::get_instance().set_role_lock(packet->role_guid, packet->is_login, packet->is_send_chat, packet->callback_info.handle_index);
	}
	void dp2ws_gm_get_role_items_info_proc(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const dp2cs_load_char_item* packet = static_cast<const dp2cs_load_char_item*>(data_ptr);
		if (nullptr == packet)
		{
			return;
		}
		web_client::get_instance().set_role_items_info(packet->role_guid, packet->data_list, packet->data_num, packet->callback_info.handle_index);
	}

}
