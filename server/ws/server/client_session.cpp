/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   14:56
	file base:	client_session
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include "ws_client.hpp"
#include "client_session.hpp"
#include "client_session_mgr.hpp"
#include "utility/serialize_msg.h"
#include "utility/cs_date.hpp"
#include "Logic/time_def.hpp"
#include "team/team_ws_mgr.h"
#include "role_pk/role_pk_manager.h"
#include "cross_group/gate_proxy.hpp"
#include "cross_group/cross_transfer_logic.hpp"
#include "world_server.hpp"
#include "legion/legion_ws_mgr.h"
#include "ranking/ranking_mgr_ws.h"
#include "big_player_ws_mgr.h"
#include <chat_msg.hpp>
#include "server_log.hpp"
#include "world_boss/world_boss_ws_mgr.h"
#include "activity/gain_treasure_ws_mgr.h"
#include "activity/boss_island_ws_mgr.h"
#include "recharge_mgr_ws.hpp"
#include "activity/cross_server_pk_ws_mgr.h"
#include "activity/cross_server_harry_ws_mgr.h"
#include "activity/cross_server_city_war_ws_mgr.h"
#include "activity/time_limit_activity_ws_mgr.h"
#include "activity/cloud_shop_mgr.h"
#include "activity/cross_ladder_ws_mgr.h"
#include "activity/element_war_ws_mgr.h"
#include "assist_fight/assist_fight_mgr.h"
#include "auction/auction_mgr_ws.h"
#include "activity/pk_king_mgr.h"
#include "connection/daemon_client.hpp"
#include "utility/parse_msg.h"
#include "globle_data.h"
#include "system/scene/cs_map_system.h"
#include "system/scene/cs_map_mgr_system.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "components/scene/cs_map_component.h"
#include "login_msg.hpp"
#include "character.pb.h"
#include "net.pb.h"
#include "char_msg.hpp"
#include "game.pb.h"

namespace hld
{
	client_session::client_session(void):m_relation_mgr(this)
	{
		m_array_index = 0;
		clear_data();
	}

	client_session::~client_session(void)
	{
	}

	void client_session::clear_data()
	{
		m_is_data_use = false;
		m_role_info.clear_data();
		m_role_ws_info.clear_data();
		m_is_send_chat = false;
		m_worship_value = 0;
		m_client_uid.clear_data();
		m_status = e_ss_created;
		m_step_num = e_session_step_null;
		m_online_state = e_os_offline;
		m_transfer_info.clear_data();
		m_dest_group_id = 0;
		m_cs_conn_index = -1;
		m_cs_array_index = 0;
		memset(m_account, 0, sizeof(m_account));
		m_login_type = e_login_type_new_max;
		m_login_type_plus = 0;
		m_login_third_data.clear_data();
		m_ip_address.clear();
		memset(m_cross_callback_val, 0, sizeof(m_cross_callback_val));
		m_map_guid.clear_data();
		m_line_id = 0;
		m_team_guid.clear_data();
		m_player_legion_info.clear_data();
		m_logintime = 0;
		m_relation_mgr.clear_data();
		m_is_already_login = false;
		m_logout_time = 0;
		m_logout_begin = false;
		m_update_time_count = 0;
		m_out_send_count = 0;
		memset(&m_role_data_ws_ex, 0, sizeof(m_role_data_ws_ex));
		memset(m_ban_role_array, 0, sizeof(m_ban_role_array));
		memset(m_ban_chat_array, 0, sizeof(m_ban_chat_array));
		m_login_time = 0;
		m_jewel_num = 0;
		m_activate_code_time = 0;
		m_client_session_tick_min_last = 0;
		m_is_need_send_cross_server_notice = false;
		m_last_send_world_boss_damage_list_sec = 0;
		m_recharge_time = 0;
		m_cross_server_save_time_sec = 0;
		m_is_cross_server_save_end = false;
		m_is_need_send_save_end = false;
		m_is_in_assist = false;
		m_is_robot_account = false;
	}
	bool& client_session::is_vaild()
	{
		return m_is_data_use;
	}
	void	client_session::send_to_fep(const void* data_ptr,size_t data_len)
	{
		world_server::getInstance().send_to_fep(m_client_uid.fepserver_uid, data_ptr, data_len);
	}
	void	client_session::send_to_cs(const void* data_ptr,size_t data_len)
	{
		world_server::getInstance().send_to_cs(m_cs_conn_index, data_ptr,data_len);
	}
	bool   client_session::send_to_cs_lua(google::protobuf::Message* net_pro, uint32 header)
	{
		packet_s2s *p_cs = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
		if (p_cs == nullptr)
		{
			return false;
		}
		world_server::getInstance().send_to_cs(m_cs_conn_index, p_cs, p_cs->get_packet_len());
		return true;
	}
	void client_session::send_to_cs_lua(const char* msg, int32 msg_len, uint32 header)
	{
		packet_s2s* p_cs = parse_msg::getInstance().get_server_msg();
		if (p_cs == nullptr)
		{
			return;
		}

		p_cs->wheader = header;
		p_cs->logic_guid = 0;
		p_cs->logic_index = 0;
		p_cs->logic_ex = 0;
		if (msg_len < 0 || msg_len >= INTERNAL_SERVER_MAX_PACKET_SIZE)
		{
			return;
		}
		memcpy(p_cs->google_data, msg, msg_len);

		p_cs->google_data_len = msg_len;

		world_server::getInstance().send_to_cs(m_cs_conn_index, p_cs, p_cs->get_packet_len());

	}
	
	void client_session::send_to_client(google::protobuf::Message* net_pro, uint32 header)
	{
		packet_c2s_s2c msg;
		serialize_msg::get_instance().set_serialize_msg_new(msg, net_pro, m_client_uid, header);
		send_to_fep((void*)&msg, msg.get_packet_len());
	}
	void client_session::send_to_client(const void* data_ptr, int32 data_len, uint32 header)
	{
		packet_c2s_s2c msg;
		msg.client_uid = m_client_uid;
		msg.wheader = header;
		msg.google_data_len = data_len;
		memcpy(msg.google_data, data_ptr, data_len);
		send_to_fep((void*)&msg, msg.get_packet_len());
	}
	void client_session::tick(const int64& new_time)
	{
		bool is_gm_close = daemon_client::getInstance().get_server_close();
		if (is_gm_close)
		{
			client_session_mgr::getInstance().logout_complete(this);
			return;
		}

		if (m_status == client_session::e_ss_logout)
		{
			ws2cs_client_logout	req_logout;
			//	周期性广播，通知所有 CS LOGOUT
			req_logout.role_guid = get_role_guid();
			req_logout.client_uid = get_client_uid();
			req_logout.need_send_save_end = m_is_need_send_save_end;
			bool is_use = proto_by_lua(e_msg_index_ws2cs_client_logout);
			if (is_use == false)
			{
				world_server::getInstance().broadcast(&req_logout, sizeof(req_logout), e_server_type_cs);
			}
			else
			{
				hld::ws2cs_proto::client_logout pro_msg;
				pro_msg.set_role_guid(get_role_guid().server_64);
				pro_msg.set_client_uid(get_client_uid().fep_uid_64);
				pro_msg.set_need_send_save_end(m_is_need_send_save_end);
				world_server::getInstance().broadcast_lua(&pro_msg, e_msg_index_ws2cs_client_logout, e_server_type_cs);
			}
			m_out_send_count++;
			if (m_out_send_count > diconnect_repeat_num)
			{
				client_session_mgr::getInstance().logout_complete(this);
			}
			return;
		}
		if (m_logout_begin)
		{
			if (m_logout_time > 0 && new_time >= m_logout_time)
			{
				client_session_mgr::getInstance().logout_client(this);
				m_logout_time = 0;
			}
			return;
		}
		if (m_cross_server_save_time_sec > 0)
		{
			if (m_is_cross_server_save_end || m_cross_server_save_time_sec <= new_time)
			{
				m_is_cross_server_save_end = false;
				m_cross_server_save_time_sec = 0;
				func_session_load_dp_end();
			}
		}
		if (new_time < m_update_time_count)
		{
			return;
		}
		m_update_time_count = new_time + second_tick_time;
		if (m_client_session_tick_min_last <= new_time)
		{
			m_client_session_tick_min_last = new_time + minute_tick_time;
			get_relation_list_mgr().update_relation_state(true, true);//每隔一分钟同步一次好友的状态
		}
		//TODO:	添加周期性存回数据到 DP
		if (is_ws_info_daily_refresh())
		{
			refresh_ws_faily_info();
			save_role_ws_info();
			send_ws_info_all();
			//m_relation_mgr.update_relation_state();
			m_relation_mgr.save_relation_to_db(true);
		}
	}

	void client_session::func_session_load_dp_end()
	{
		if (m_login_type == e_login_type_new_token)//跨服需要取记录的传送数据
		{
			s_transfer_info transfer_info;
			transfer_info.map_guid = m_role_info.map_guid;
			transfer_info.map_id = get_role_info_data(e_role_info_move_map_id);
			transfer_info.war_index = get_role_info_data(e_role_info_move_war_idex);
			set_transfer_info(transfer_info);
		}
		marry_mgr_ws::get_instance().session_online_marry_handle(get_role_guid());
		world_server::getInstance().send_server_config_all_to_client(this);
		world_server::getInstance().send_need_begin_cross_pk_to_client(this);
		login_try_enter_scene();
	}

	void client_session::get_log_common_head_info(s_log_common_head &log_head)
	{
		set_log_common_head_part1(log_head, SERVERCONFIG->app_key, m_login_third_data.param7, m_login_third_data.param4, m_login_third_data.param5);
		set_log_common_head_part2(log_head, m_login_third_data.param2, init_unit::change_string_to_i32(m_login_third_data.param15));
		set_log_common_head_part3(log_head, m_role_info.account, m_role_info.data_ary[e_role_info_server_id], utility::get_tick_count())
		set_log_common_head_part4(log_head, m_role_info.role_guid, m_role_info.role_name, m_role_info.data_ary[e_role_info_exp_level], m_role_info.data_ary[e_role_info_gender])
		set_log_common_head_part5(log_head, m_role_info.data_i64_ary[e_role_i64_info_gs_value], m_role_info.data_ary[e_role_info_class_type], m_role_info.data_ary[e_role_info_vip_level])
	}

	xstring client_session::get_class_name()
	{
		return globle_data::get_instance().get_class_name_by_type(m_role_info.data_ary[e_role_info_class_type]);
	}

	void client_session::set_client_logout_begin()
	{
		if (m_logout_time == 0)
		{
			m_logout_time = utility::get_tick_count() + client_session_logout_time;
			auto map_template = cs_map_system::get_map_template(get_map_guid());
			if (map_template != nullptr)
			{
				if(map_template->Type == hld::e_map_type_cross_ladder)
					m_logout_time = utility::get_tick_count() + 10 * second_tick_time;
			}
		}
		m_logout_begin = true;
	}
	void client_session::send_ws_info_all()
	{
		if (!get_role_guid().is_valid())
		{
			return;
		}
		character_proto_load_role_ws_info_end ws_info_msg;
		ws_info_msg.add_role_guid(m_role_info.role_guid.A);
		ws_info_msg.add_role_guid(m_role_info.role_guid.B);
		for (int32 i = 0; i < e_role_ws_info_max; ++i)
		{
			ws_info_msg.add_data_array(m_role_ws_info.data_ary[i]);
		}
		send_to_client(&ws_info_msg, e_msgindex_s2c_load_role_ws_info_end);
	}

	bool client_session::is_ws_info_daily_refresh()
	{
		int32 last_refresh = m_role_ws_info.data_ary[e_role_ws_info_save_time];

		if (last_refresh <= 0)
		{
			return true;
		}
		time_info last_time_info = time_helper::get_time_by_stamp_new(last_refresh);
		int32 last_refresh_year = last_time_info.year;
		int32 last_refresh_month = last_time_info.month_in_year;
		int32 last_refresh_day = last_time_info.day_in_month;

		time_info cur_time_info = time_helper::get_cur_time_new();
		int32 cur_year = cur_time_info.year;
		int32 cur_month = cur_time_info.month_in_year;
		int32 cur_day = cur_time_info.day_in_month;

		//测试，提升刷新频率
		int32 last_refresh_hour = last_time_info.hour_in_day;
		int32 last_refresh_minut = last_time_info.minute_in_hour;
		int32 last_refresh_second = last_time_info.second_in_hour;
		int32 cur_hour = cur_time_info.hour_in_day;
		int32 cur_minut = cur_time_info.minute_in_hour;
		int32 cur_second = cur_time_info.second_in_minute;

		if (cur_year != last_refresh_year
			|| cur_month != last_refresh_month
			|| cur_day != last_refresh_day

			/* || cur_minut != last_refresh_minut
			|| cur_second - 10 > last_refresh_second*/)
		{
			return true;
		}
		return false;
	}
	void client_session::refresh_ws_faily_info()
	{
		for (int32 i = 0; i < e_role_ws_info_max; ++i)
		{
			m_role_ws_info.data_ary[i] = 0;;
		}
	}

	void client_session::send_role_right(e_role_right right_type, int32 type_value, int32 right_time)
	{
		ws2cs_role_right msg;
		msg.role_guid = get_role_guid();
		msg.right_type = right_type;
		msg.right_value = type_value;
		msg.right_time = right_time;
		bool is_use = proto_by_lua(e_msgindex_cs2ws_role_right);
		if (is_use == false)
		{
			send_to_cs(&msg, sizeof(ws2cs_role_right));
		}
		else
		{
			hld::ws2cs_proto::role_right pro_msg;
			msg.to_proto(pro_msg);
			send_to_cs_lua(&pro_msg, e_msgindex_cs2ws_role_right);
		}
	}

	void client_session::send_to_cs_role_info_data(e_role_info eType)
	{
		ws2cs_role_info_change msg;
		msg.role_type = (int32)eType;
		msg.role_value = m_role_info.data_ary[eType];
		msg.role_guid = get_role_guid();
		bool is_use = proto_by_lua(e_msgindex_ws2cs_role_info_change);
		if (is_use == false)
		{
			send_to_cs(&msg, sizeof(ws2cs_role_info_change));
		}
		else
		{
			hld::ws2cs_proto::role_info_change pro_msg;
			msg.to_proto(pro_msg);
			send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_role_info_change);
		}
	}

	void client_session::set_role_info_data(e_role_info eType, int32 num)
	{
		m_role_info.data_ary[eType] = num;
	}

	void client_session::set_role_guid(const guid_64& role_guid)
	{
		m_role_info.role_guid = role_guid;
	}

	void client_session::set_role_gs_value(int64 gs_value)
	{
		//m_role_info.data_ary[e_role_info_GS_value] = gs_value;
		m_role_info.data_i64_ary[e_role_i64_info_gs_value] = gs_value;
	}

	void client_session::save_role_ws_info()
	{
		ws2dp_save_role_ws_info ws_info_save_msg;
		m_role_ws_info.data_ary[e_role_ws_info_save_time] = time_helper::get_cur_time_new().second;
		ws_info_save_msg.role_guid = get_role_guid();
		ws_info_save_msg.role_ws_info = m_role_ws_info;
		ws_client::getInstance().send_to_dp(&ws_info_save_msg, sizeof(ws2dp_save_role_ws_info), m_role_info.data_ary[e_role_info_server_id]);
	}

	MapTemplate* client_session::get_map_temp_ptr()
	{
		return cs_map_system::get_map_template(get_map_guid());
	}

	void client_session::send_ws_info_one(int32 index)
	{
		if (index < 0 || index >= e_role_ws_info_max)
		{
			return;
		}
		character_proto_update_role_ws_info_one msg;
		msg.set_info_index(index);
		msg.set_info_value(m_role_ws_info.data_ary[index]);
		msg.add_role_guid(get_role_guid().A);
		msg.add_role_guid(get_role_guid().B);
		send_to_client(&msg, e_msgindex_s2c_update_role_ws_info_one);
	}

	bool client_session::set_transfer_info(const s_transfer_info& transfer_info)
	{
		auto map_temp_ptr = GET_TEMPLATE(MapTemplate, transfer_info.map_id);
		if (nullptr == map_temp_ptr)
		{
			CONSOLE_ERROR("map_temp_ptr is null, map_id:{}", transfer_info.map_id);
			return false;
		}
		m_transfer_info = transfer_info;
		return true;
	}
	void client_session::player_enter_game_transfer(guid_64 role_guid, int32 server_id)
	{
		if (role_guid.is_valid())
		{
			set_role_guid(role_guid);
		}
		if (m_status == e_ss_ingame)
		{
			CONSOLE_ERROR("player_enter_game_transfer m_status == e_ss_ingame");
			return;
		}
		if (false == get_role_guid().is_valid())
		{
			CONSOLE_ERROR("player_enter_game_transfer get_role_guid().is_valid() == false");
			return;
		}
		for (int32 i = 0; i < max_character_num; ++i)
		{
			if (m_ban_role_array[i] == role_guid.server_64)
			{//限制登录
				CONSOLE_INFO("player_enter_game_transfer m_ban_role_array[i] == role_guid.server_64 role_guid.server_64:{}", role_guid.server_64);
				ws2fep_enter_game rep;
				rep.client_uid = m_client_uid;
				rep.e_result =e_error_code_enter_no_allow;
				memcpy(rep.account, m_account, sizeof(rep.account));
				send_to_fep(&rep, sizeof(rep));
				return;
			}
		}

		if (world_server::getInstance().get_server_info_arr(e_server_info_type_only_create_role))
		{
			CONSOLE_ERROR("e_server_info_type_only_create_role");
			ws2fep_enter_game rep;
			rep.client_uid = m_client_uid;
			rep.e_result = e_error_code_enter_only_create;
			memcpy(rep.account, m_account, sizeof(rep.account));
			send_to_fep(&rep, sizeof(rep));
			return;
		}

		m_status = client_session::e_ss_map_transfer;
		m_step_num = client_session::e_session_step_enter_game;
		m_cs_conn_index = -1;

		server2dp_proto_ws2dp_load_character req;
		req.set_role_guid(get_role_guid().server_64);
		req.set_role_account(m_account);
		ws_client::getInstance().send_to_dp_new(&req, e_msg_ws2dp_load_character, server_id);

		//script_mgr::get_instance().call_func("ws2dp_send", "send_get_char_info_ws_lua", 0, false, "%l%s%d", get_role_guid().server_64, m_account, server_id);
	}

	bool client_session::is_self_server(guid_64 target_guid,bool is_need_same_server_id_for_ws)
	{
		if (false == gate_proxy::getInstance().is_gate_run())
		{
			return true;
		}
		if (gate_proxy::getInstance().is_gate_run() == false)
		{
			return true;
		}
		if (world_server::getInstance().get_server_id() == m_role_info.data_ary[e_role_info_server_id] && false == target_guid.is_valid())
		{
			return true;
		}
		client_session* target_session = client_session_mgr::getInstance().get_session(target_guid);//下面的逻辑都用于互动相关的，非同服玩家禁止交互
		if (nullptr == target_session)
		{
			return false;
		}
		if (target_session->get_role_info_data(e_role_info_server_id) == m_role_info.data_ary[e_role_info_server_id])		
		{
			if (true == is_need_same_server_id_for_ws)
			{
				if (world_server::getInstance().get_server_id() == m_role_info.data_ary[e_role_info_server_id])
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	void client_session::login_try_enter_scene()
	{
		guid_64 role_guid = get_role_guid();
		guid_64 transfer_map_guid = m_transfer_info.map_guid;
		int32 transfer_map_id = m_transfer_info.map_id;

		if (transfer_map_guid.is_valid() == false)
		{
			// 之前用map_template_id判断断线重连 不够精确
			guid_64 cur_map_guid = get_role_map_guid();
			auto map_ent = get_entity(cur_map_guid);
			if (nullptr != map_ent)
			{
				auto map_cp = map_ent->get_component<cs_map_component>();
				team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(role_guid);
				if (nullptr != team_ws_ptr && cur_map_guid == team_ws_ptr->get_team_map_guid())
				{
					transfer_map_guid = cur_map_guid;
				}
				else if (map_cp->m_map_template_ptr->BreakLineLogin > 0 && map_cp->m_role_list.find(role_guid) != map_cp->m_role_list.end())
				{
					transfer_map_guid = cur_map_guid;
				}
			}
		}

		guid_64 role_pk_map = role_pk_manager::getInstance().get_player_role_pk_map(role_guid);
		if (role_pk_map.is_valid())
		{
			auto map_ent = get_entity(role_pk_map);
			if (nullptr == map_ent)
			{
				role_pk_manager::getInstance().clear_player_role_pk_map(role_guid);
				if (world_server::getInstance().is_sky_island_server())
				{
					//回原服
					m_transfer_info.clear_data();
					cross::transfer_player(this, -1, m_transfer_info);
					return;
				}
			}
			else
			{
				transfer_map_guid = role_pk_map;
			}
		}

		bool need_cross_big_map = false;

		Entity* map_ent = nullptr;
		if (transfer_map_guid.is_valid())
		{
			map_ent = get_entity(transfer_map_guid);
			if (map_ent)
			{
				auto map_cp = map_ent->get_component<cs_map_component>();
				if (map_cp->m_map_state == e_map_state_over)
				{
					map_ent = nullptr;
					need_cross_big_map = true;
				}
				else if (map_cp->m_map_state == e_map_state_game)
				{
					if (map_cp->m_map_template_ptr->BeginCanJoin == false)
					{
						map_ent = nullptr;
					}
				}
			}
			else
			{
				map_ent = cs_map_mgr_system::init_map_by_battle(get_role_info_data(e_role_info_main_map_id));
			}
		}
		else
		{
			if (transfer_map_id > 0)
			{
				map_ent = cs_map_mgr_system::init_map_by_battle(transfer_map_id);
			}
			else
			{
				map_ent = cs_map_mgr_system::init_map_by_battle(get_role_info_data(e_role_info_main_map_id));
			}
		}

		if (need_cross_big_map && world_server::getInstance().get_server_id() == get_role_info_data(e_role_info_server_id))//玩家想传送到一张已结束的地图时，会被强制拉回大世界而不是直接踢出
		{
			if (world_server::getInstance().is_sky_island_server())//如果是gate服则传送到天空岛
			{
				map_ent = cs_map_mgr_system::get_map_ws_by_min(boss_island_temp_id);
			}
			else
			{
				map_ent = cs_map_mgr_system::init_map_by_battle(get_role_info_data(e_role_info_main_map_id));
			}
		}
		
		if (nullptr == map_ent)
		{
			CONSOLE_ERROR("login_try_enter_scene map_ent == nullptr move_map_id:{}", get_role_info_data(e_role_info_main_map_id));
			ws2fep_enter_game ws2fep_rep;
			ws2fep_rep.client_uid = m_client_uid;
			memcpy(ws2fep_rep.account, m_role_info.account, max_account_length);
			ws2fep_rep.e_result = e_error_code_enter_area_not_exist;
			send_to_fep(&ws2fep_rep, sizeof(ws2fep_rep));
			return;
		}

		auto map_cp = map_ent->get_component<cs_map_component>();

		net_server* cs = net_server_mgr::getInstance().get_peer_by_conn_index(map_cp->m_cs_uid);
		if (nullptr == cs || m_status == client_session::e_ss_logout)
		{
			CONSOLE_ERROR("login_try_enter_scene cs == nullptr cs_uid:{}", map_cp->m_cs_uid);
			ws2fep_enter_game ws2fep_rep;
			ws2fep_rep.client_uid = m_client_uid;
			memcpy(ws2fep_rep.account, m_role_info.account, max_account_length);
			ws2fep_rep.e_result = e_error_code_enter_area_not_exist;
			send_to_fep(&ws2fep_rep, sizeof(ws2fep_rep));
			return;
		}
		
		m_cs_conn_index = map_cp->m_cs_uid;
		set_map_guid(map_cp->m_map_guid);
		set_line_id(map_cp->m_line_id);
		m_step_num = client_session::e_session_step_send_enter_game_cs;
		cs_map_system::add_unit(map_ent, get_role_guid());

		ws2cs_map_dynamic_params map_dynamic_params_msg;
		map_dynamic_params_msg.map_guid = map_cp->m_map_guid;
		memcpy(map_dynamic_params_msg.dynamic_params_int, m_transfer_info.dynamic_ints, sizeof(map_dynamic_params_msg.dynamic_params_int));
		bool is_use = proto_by_lua(e_msg_index_ws2cs_map_dynamic_params);
		if (is_use == false)
		{
			send_to_cs(&map_dynamic_params_msg, sizeof(map_dynamic_params_msg));
		}
		else
		{
			hld::ws2cs_proto::map_dynamic_params pro_msg;
			map_dynamic_params_msg.to_proto(pro_msg);
			send_to_cs_lua(&pro_msg, e_msg_index_ws2cs_map_dynamic_params);
		}

		ws2cs_enter_game ws2cs_req;
		memcpy(ws2cs_req.account, m_role_info.account, max_account_length);
		ws2cs_req.client_uid = m_client_uid;
		ws2cs_req.login_type = m_login_type;
		ws2cs_req.role_guid = role_guid;
		ws2cs_req.server_id = m_role_info.data_ary[e_role_info_server_id];
		ws2cs_req.exp_level = m_role_info.data_ary[e_role_info_exp_level];
		ws2cs_req.is_already_login = is_already_login();
		ws2cs_req.map_guid = map_cp->m_map_guid;
		ws2cs_req.line_id = map_cp->m_line_id;
		ws2cs_req.war_index = m_transfer_info.war_index;
		ws2cs_req.is_send_chat = m_is_send_chat;
		ws2cs_req.worship_value = m_worship_value;
		marry_couple_data couple_data = marry_mgr_ws::get_instance().get_couple_data(get_role_guid());
		ws2cs_req.couple_guid = couple_data.role_guid;
		memcpy(ws2cs_req.couple_name, couple_data.role_name, max_name_size);
		if (false == is_self_server())
		{
			ws2cs_req.legion_info = m_player_legion_info;
		}
		team_ws* team_ws_ptr = team_ws_mgr::get_instance().get_unit_team(role_guid);
		if (team_ws_ptr)
		{
			// 如果有玩家在组队中就要更新当前所在地址的ID并推送给队友
			team_ws_ptr->update_member_map(this, map_cp->m_map_guid);
			team_ws_ptr->gen_player_team_info(ws2cs_req.team_info);
		}
		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (nullptr != legion_ws_ptr)
		{
			// 传送时附带军团GUID是为了在传送进军团城战地图的时候可以标识玩家属于哪个军团
			legion_ws_ptr->get_player_legion_info(ws2cs_req.legion_info, role_guid);
			//设置是否是军团战胜利方成员, 是否是城主  
			if (big_player_ws_mgr::get_instance().get_big_player_guid(e_big_player_type_overload_legion) == legion_ws_ptr->get_legion_guid())
			{
				ws2cs_req.bflag_loading_title[e_title_type_legion_city_win_member] = true;
			}
		}
		if (pk_king_mgr::get_instance().player_is_pk_king(role_guid))
		{
			ws2cs_req.bflag_loading_title[e_title_type_kingofpk] = true;
		}
		else if (world_server::getInstance().get_server_id() == get_role_info_data(e_role_info_server_id))
		{
			ws2cs_req.bflag_loading_title[e_title_type_kingofpk] = false;
		}
		ws2cs_req.bflag_loading_title[e_title_type_pk_king_kill_num] = false;
		memcpy(&ws2cs_req.fixed_data, &m_login_third_data, sizeof(login_fixed_data));
		memcpy(ws2cs_req.cross_callback_val, m_cross_callback_val, sizeof(ws2cs_req.cross_callback_val));

		big_player_ws_mgr::get_instance().set_title_session_login(role_guid, ws2cs_req.bflag_loading_title);
		ws2cs_req.login_time = m_login_time;
		send_to_cs(&ws2cs_req, sizeof(ws2cs_req));
	}

	void client_session::enter_scene_logic_proc()
	{
		m_step_num = client_session::e_session_step_cs_enter_scene;
		
		if (false == is_already_login())
		{
			enter_scene_logic_proc_login();
		}

		world_boss_ws_mgr::get_instance().send_all_boss_hp_per(this);// 同步所有boss的血量信息
		cross_ladder_ws_mgr_new::get_instance().send_upgrade_notify(this);
		element_war_ws_mgr::get_instance().send_upgrade_notify(this);
		game_proto_line_list line_list_msg;
		line_id_vec temp_vec;
		cs_map_mgr_system::get_line_vec(cs_map_system::get_map_template_id(get_map_guid()), temp_vec);
		for (uint32 i = 0; i < temp_vec.size(); ++i)
		{
			line_list_msg.add_line_id(temp_vec[i]);
		}
		send_to_client(&line_list_msg, e_msgindex_s2c_map_line_ary);
		if (m_is_need_send_cross_server_notice == true)
		{
			send_notice("90096605");//跨服成功给的提示
			m_is_need_send_cross_server_notice = false;
		}
		//同步助战信息
		if (is_self_server())
		{
			assist_fight_mgr::get_instance().check_assist_fight_legion_team(get_role_guid());
			//向cs同步助战信息
			assist_fight_mgr::get_instance().sync_assist_fight_info_to_cs_login(get_role_guid());
			//向客户端同步助战信息
			assist_fight_mgr::get_instance().sync_assist_fight_info(get_role_guid(), true);
		}
		world_server::getInstance().send_server_act_hidden_to_client(this);
	}

	void client_session::enter_scene_logic_proc_login()
	{
		guid_64 role_guid = get_role_guid();
		get_relation_list_mgr().req_load_relation_from_db();
		//marry_mgr_ws::get_instance().session_online_marry_handle(role_guid);
		marry_mgr_ws::get_instance().sync_wedding_ring_info(role_guid);
		marry_mgr_ws::get_instance().sync_marry_mission_info(role_guid);

		team_ws_mgr::get_instance().on_player_login(this);

		legion_ws* legion_ws_ptr = legion_ws_mgr::get_instance().get_unit_legion(role_guid);
		if (legion_ws_ptr != nullptr)
		{
			legion_ws_ptr->on_legion_member_login(this);
		}
		legion_ws_mgr::get_instance().get_legion_city_war().check_and_send_city_master_online_acconcement(role_guid);
		ranking_mgr_ws::send_service_rank_info_all_to_client(this);
		ranking_mgr_ws::send_avg_level_to_client(this);
		gain_treasure_ws_mgr::get_instance().send_gain_treasure_boss_info_all(this);
		boss_island_ws_mgr::get_instance().send_boss_island_info_all(this);
		cross_server_pk_ws_mgr::get_instance().send_island_msg_all_to_client(this);
		cross_server_harry_ws_mgr::get_instance().send_harry_msg_to_one_client(this);
		cross_server_city_war_ws_mgr::get_instance().send_city_info_all_to_client(this);
		time_limit_activity_ws_mgr::get_instance().get_activity_common_data_to_gate(this);
		cloud_shop_mgr::get_instance().send_open_msg_to_client(this);
		cross_ladder_ws_mgr_new::get_instance().session_online(this);
		element_war_ws_mgr::get_instance().session_online(this);
		auction_mgr_ws::get_instance().on_login(this);
		client_session_mgr::getInstance().send_transfer_state_to_client(this);

		load_offline_chat();

		set_is_already_login(true);
	}

	void client_session::send_notice(std::string notice_str)
	{
		character_proto_map_fuben_send_notice msg;
		msg.set_notice_content(notice_str.c_str());
		send_to_client(&msg, e_msgindex_s2c_fuben_send_notice);
	}

	void client_session::send_recharge(const xchar* order_id, int32 goods_id, float pay_price,int64 order_num, int32 payment_type, int32 direct_diamond)
	{
		if (order_id == nullptr)
		{
			recharge_mgr_ws::get_instance().send_recharge_end_result(e_recharge_error_client_seesion_save_recharge_order_id_is_null, order_num, payment_type, get_role_guid(), goods_id, order_id);
			server_log::recharge_step_log("", get_role_guid(), goods_id, e_recharge_error_client_seesion_save_recharge_order_id_is_null, m_login_third_data, m_login_type, &m_role_info);

			return;
		}
		xstring order_id_str = xstring(order_id);
		server_log::recharge_step_log(order_id_str, get_role_guid(), goods_id, e_recharge_step_ws_to_cs_send_recharge, m_login_third_data, m_login_type, &m_role_info);

		ws2cs_recharge msg;
		int32 order_id_len = strlen(order_id);
		memcpy(msg.order_id, order_id, order_id_len < max_recharge_order_length ? order_id_len : max_recharge_order_length);
		msg.role_guid = get_role_guid();
		msg.goods_id = goods_id;
		msg.pay_price = pay_price;
		msg.order_num = order_num;
		msg.payment_type = payment_type;
		msg.direct_diamond = direct_diamond;
		bool is_use = proto_by_lua(e_msgindex_ws2cs_recharge);
		if (is_use == false)
		{
			send_to_cs(&msg, sizeof(ws2cs_recharge));
		}
		else
		{
			hld::ws2cs_proto::recharge pro_msg;
			msg.to_proto(pro_msg);
			send_to_cs_lua(&pro_msg, e_msgindex_ws2cs_recharge);
		}
	}

	void client_session::set_role_name(xchar* _role_name)
	{ 
		memset(m_role_info.role_name, 0, sizeof(m_role_info.role_name));
		memcpy(m_role_info.role_name, _role_name, max_name_size);
	}

	void client_session::set_legion_name(xchar* legion_name)
	{
		memset(m_player_legion_info.legion_name, 0, sizeof(m_player_legion_info.legion_name));
		memcpy(m_player_legion_info.legion_name, legion_name, max_name_size);
	}

	void client_session::load_offline_chat()
	{
		ws2dp_load_chat_with_offline msg;
		msg.role_guid = get_role_guid();

		ws_client::getInstance().send_to_dp(&msg, sizeof(msg), m_role_info.data_ary[e_role_info_server_id]);
	}

	void client_session::check_is_robot()
	{
		if (init_unit::is_robot_account(SERVERCONFIG->robot_account, m_role_info.account))
		{
			m_is_robot_account = true;
		}
	}
}


