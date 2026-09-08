/* slim auto-extracted login handlers */
#include "msgproc/msgproc_dp.hpp"
#include "server/client_session.hpp"
#include "server/client_session_mgr.hpp"
#include "server/login/login_service_mgr.hpp"
#include "server/login_role/login_role_mgr.h"
#include "world_server.hpp"
#include "ws_client.hpp"
#include "utility/parse_msg.h"
#include "utility/serialize_msg.h"
#include "utility/init_unit.h"
#include "server_log.hpp"
#include "character.pb.h"
#include "net.pb.h"
#include "dp2s.pb.h"
#include <login_msg.hpp>
#include <char_msg.hpp>
#include <core.hpp>
#include <rlog.hpp>
#include "business_stubs.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "game_cfg/servers_config.h"
#include "Utility/cs_date.hpp"

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
			world_server::getInstance().send_to_gateway(db_reponse->client_uid.gatewayserver_uid, &msg, msg.get_packet_len());
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
		ls2gateway_create_role msg;
		msg.client_uid = pdata->client_uid;
		msg.eresult = pdata->eresult;
		msg.role_guid = pdata->role_guid;
		msg.role_info = pdata->role_info;
		world_server::getInstance().send_to_gateway(pdata->client_uid.gatewayserver_uid, &msg, sizeof(msg));
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
		world_server::getInstance().send_to_gateway(pdata->client_uid.gatewayserver_uid, &msg, msg.get_packet_len());
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
				ws2gateway_enter_game rep;
				rep.client_uid = session->m_client_uid;
				rep.e_result = e_error_code_enter_no_allow;
				memcpy(rep.account, account.c_str(), account.size());
				world_server::getInstance().send_to_gateway(session->m_client_uid.gatewayserver_uid, &rep, sizeof(rep));
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
					ws2gateway_enter_game rep;
					rep.client_uid = session->m_client_uid;
					memcpy(rep.account, session->m_role_info.account, sizeof(rep.account));
					rep.e_result = e_error_code_enter_error_server_id;
					world_server::getInstance().send_to_gateway(session->m_client_uid.gatewayserver_uid, &rep, sizeof(rep));
					return;
				}
			}
			session->func_session_load_dp_end();
		}
		break;
		default:
		{
			CONSOLE_ERROR("dp2ws_rep_get_role_info_ws eresult:{}", eresult);
			ws2gateway_enter_game rep;
			rep.client_uid = session->m_client_uid;
			memcpy(rep.account, session->m_role_info.account, sizeof(rep.account));
			rep.e_result = (e_error_code)eresult;
			world_server::getInstance().send_to_gateway(session->m_client_uid.gatewayserver_uid, &rep, sizeof(rep));
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

}
