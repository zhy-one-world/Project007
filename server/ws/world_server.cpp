/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   15:02
	file base:	world_server
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include <net/scheduler.hpp>
#include "world_server.hpp"
#include "ws_client.hpp"
#include "server/client_session_mgr.hpp"
#include "server/queued_login_mgr.h"
#include "game_cfg/servers_config.h"
#include "time.hpp"
#include "utility/globle_data.h"
#include "utility/guid_gen.h"
#include "def/app/app_server.hpp"
#include "cross_group/gate_proxy.hpp"
#include "net/net_client_mgr.hpp"
#include "server_log.hpp"
#include "server_data/server_data.hpp"
#include "Logic/time_def.hpp"
#include "server/web_client.hpp"
#include "connection/daemon_client.hpp"
#include <internal.hpp>
#include "daemon_eye_msg.hpp"
#include "msgproc/msgproc_daemon.hpp"
#include "server/activity/gain_treasure_ws_mgr.h"
#include "server/activity/cloud_shop_mgr.h"
#include "server/activity/time_limit_activity_ws_mgr.h"
#include "server/legion/legion_ws_answer.h"
#include "server/legion/legion_ws_mgr.h"
#include "server/legion/city_war_territory_mgr.h"
#include "server/ranking/ranking_mgr_ws.h"
#include "server/recharge_mgr_ws.hpp"
#include <gate_msg.hpp>
#include "server/activity/cross_server_pk_ws_mgr.h"
#include "server/activity/cross_server_world_boss_ws_mgr.h"
#include "server/activity/cross_server_city_war_ws_mgr.h"
#include "server/activity/cross_ladder_ws_mgr.h"
#include "utility/parse_msg.h"
#include "http/http_send.hpp"
#include "world_server_msg.hpp"
#include "server/activity/time_limit_activity_temp_ws_mgr.h"
#include "server/activity/element_war_ws_mgr.h"
#include "server/activity/cross_server_harry_ws_mgr.h"
#include "lua/script_mgr.h"
#include "server/legion/attack_city_ws_mgr.h"
#include "system/scene/cs_map_mgr_system.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "server/mail/mail_event_ws.h"
#include "server/activity/pk_king_mgr.h"
#include "server/activity/crystal_fairyland_mgr.h"
#include "server/auction/auction_mgr_ws.h"
#include "server/activity/chest_arrival_mgr.h"
#include "server/activity/broken_sky_mgr.h"
#include "server/re_login_mgr.h"
#include "server/big_player_ws_mgr.h"
#include "server/mail/event_ws_mgr.h"
#include "server/role_pk/role_pk_manager.h"
#include "server/red_package/red_package_ws_mgr.h"
#include "server/team/team_ws_mgr.h"
#include "server/world_boss/world_boss_ws_mgr.h"
#include "server/arena/arena_mgr_ws.h"
#include "server/login_role/login_role_mgr.h"
#include "net.pb.h"
namespace hld
{
	world_server::world_server(void)
	{	
		m_world_server_start = false;
		m_timerindex_gameloop=scheduler::scheduler_invalid_timer_index;
		m_server_time_five = 0;
		m_server_time_zero = 0;
		m_gm_state = SERVERCONFIG->gm_state;
		m_server_id = SERVERCONFIG->game_id;
		m_out_ip = FEPCONFIG->external_host;
		m_out_port = FEPCONFIG->external_port;
		m_ws_ip = WSCONFIG->internal_host;
		m_ws_port = WSCONFIG->internal_port;;
		m_server_name = SERVERCONFIG->game_name;
		m_server_time_refresh_harry = 0;
		m_server_time_week_constribution_refresh = 0;
		m_server_time_week_legion_welfare_refresh = 0;
		m_server_time_clear_city_war_bid = 0;
		m_begin_time = 0;

		m_server_act_hidden = false;
		m_is_begin_reload_csv = false;
		m_is_begin_cs_reload_csv = false;
		for (int32 i = e_need_server_cross_begin_cross; i < e_need_server_cross_max; i++)
		{
			m_need_begin_cross_config[i] = false;
		}
		m_open_cross_server_days = 0;		
		for (int32 i = e_server_info_type_begin_cross_server_time; i < e_server_info_type_max; i++)
		{
			m_server_info_arr[i] = 0;
		}
		m_is_set_cross_time = false;
		guid_gen::set_server_id(m_server_id);
		memset(m_fep_conn_index, 0, sizeof(m_fep_conn_index));
		m_cross_info_map.clear();
		m_ws_loading_flag = e_ws_flag_big_player
			| e_ws_flag_cs_connect
			| e_ws_flag_gm_common
			| e_ws_flag_harry
			| e_ws_flag_cross_pk 
			| e_ws_flag_overload_war
			| e_ws_flag_city_war
			| e_ws_flag_cross_boss
			| e_ws_flag_server_cross_time
			| e_ws_flag_server_refresh
			| e_ws_flag_time_limit_info 
			| e_ws_flag_time_limit_template 
			| e_ws_flag_ranking_info 
			| e_ws_flag_load_legion_info
			| e_ws_flag_cross_server_state
			| e_ws_flag_init_time_limit_template
			| e_ws_flag_load_cloud_shop_info
			| e_ws_flag_fep_connect
			| e_ws_flag_attack_city
			;
		m_need_send_gate_flag_arr.clear();
		m_last_server_list_server_id.clear();
		m_channel_count.clear();
	}

	world_server::~world_server(void)
	{
	}
	void world_server::set_server_config(const s_game_info& game_info)
	{
		m_cross_info_map[game_info.server_id] = game_info;
		std::string server_id_arr = "";
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			server_id_arr = server_id_arr + to_string(it->second.server_id) + ",";
		}
		CONSOLE_INFO("add_server_now_server_id :{}", server_id_arr);
	}

	void world_server::set_gate_server_config(const s_game_info & game_info)
	{
		if (get_gate_server_num() < max_gate_server_num)
		{
			m_gate_cross_info_map[game_info.server_id] = game_info;
		}
	}

	void world_server::func_when_add_other_server(int32 server_id)
	{
		if (is_sky_island_server())
		{
			cross_server_harry_ws_mgr::get_instance().func_when_add_other_server(server_id);
		}
	}

	void world_server::func_when_remove_other_server(int32 server_id)
	{
		if (is_sky_island_server())
		{
			cross_server_harry_ws_mgr::get_instance().func_when_remove_other_server(server_id);
			city_war_territory_mgr::get_instance().remove_bid_info_when_server_remove(server_id);
		}

	}

	void world_server::func_when_change_gate_server(int32 server_id)
	{
		if (server_id == get_cross_id())
		{
			return;
		}
		ws2gate_change_cross_id msg;
		msg.cur_server_id = get_server_id();
		msg.old_cross_id = get_cross_id();
		msg.cross_id = server_id;
		ws_client::getInstance().only_send_to_gate(&msg, sizeof(msg));
		clear_all_server_config();
	}

	void world_server::remove_server_config(const int32& server_id)
	{
		game_info_map_it it = m_cross_info_map.find(server_id);
		if (it != m_cross_info_map.end())
		{
			m_cross_info_map.erase(it);
		}
		std::string server_id_arr = "";
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			server_id_arr = server_id_arr + to_string(it->second.server_id) + ",";
		}
		CONSOLE_INFO("remove_end_now_server_id :{}", server_id_arr);
	}

	void world_server::clear_all_server_config()
	{
		if (is_sky_island_server())
		{
			cross_server_pk_ws_mgr::get_instance().clear_all_island_msg();
			for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
			{
				cross_server_harry_ws_mgr::get_instance().func_when_remove_other_server(it->second.server_id);
			}
			cross_server_city_war_ws_mgr::get_instance().func_clear_all_city_war_msg();
			cross_server_city_war_ws_mgr::get_instance().clear_city_master();
		}
		m_cross_info_map.clear();
		ws2cs_server_clear_all_config msg;
		bool is_use = proto_by_lua(e_msg_index_ws2cs_clear_server_config);
		if (is_use == false)
		{
			net_server_mgr::getInstance().send_message(&msg, sizeof(msg), -1, e_server_type_cs);
		}
		else
		{
			hld::ws2cs_proto::server_clear_all_config pro_msg;
			broadcast_lua(&pro_msg, e_msg_index_ws2cs_clear_server_config, e_server_type_cs);
		}
		world_server::getInstance().send_server_config_all_to_client(nullptr);
	}

	void world_server::get_server_id_arr(std::set<int32>& server_id_arr)
	{
		server_id_arr.clear();
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			server_id_arr.insert(it->second.server_id);
		}
	}

	void world_server::send_server_config_to_all_cs(const s_game_info& game_info, bool is_add, int32 conn_index)
	{
		ws2cs_server_config msg;
		msg.game_info = game_info;
		msg.is_add = is_add;

		bool is_use = proto_by_lua(e_msg_index_ws2cs_server_config);
		if (is_use == false)
		{
			net_server_mgr::getInstance().send_message(&msg, sizeof(msg), conn_index, e_server_type_cs);
		}
		else
		{
			hld::ws2cs_proto::server_config pro_msg;

			msg.to_proto(pro_msg);

			packet_s2s *p_s2s = parse_msg::getInstance().serialze_server(&pro_msg, e_msg_index_ws2cs_server_config, guid_64(), 0);
			if (p_s2s == nullptr)
			{
				return;
			}

			net_server_mgr::getInstance().send_message(p_s2s, p_s2s->get_packet_len(), conn_index, e_server_type_cs);
		}

	}
	void world_server::send_server_config_to_all_cs(int32 conn_index)
	{
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			send_server_config_to_all_cs(it->second, true, conn_index);
		}
	}

	void world_server::set_server_act_hidden(bool is_need_hidden, bool is_other_server)
	{
		m_server_act_hidden = is_need_hidden;
		send_server_act_hidden_to_client(nullptr);

		if (!is_other_server)
		{
			ws2gate_server_act_hide msg;
			msg.is_need_hide = m_server_act_hidden;
			ws_client::getInstance().only_send_to_gate(&msg, sizeof(msg));
		}
	}

	int32 world_server::get_activity_sec_left(int32 activity_type, e_activity_time_get time_get, bool is_other_time, bool is_cross_city_war)
	{
		bool is_cross = false;
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (_activity_cfg_ptr != nullptr && _activity_cfg_ptr->IsOpen > 0)
		{
			if ((_activity_cfg_ptr->CrossActivityOpenTime.size() > 0 || _activity_cfg_ptr->CrossOpenDate.size() > 0) && get_need_begin_cross_gm_common((e_need_server_cross)_activity_cfg_ptr->ActivityCrossType))
			{
				is_cross = true;
			}
		}
		return init_unit::get_activity_sec_left(activity_type, time_get, is_other_time, is_cross_city_war, is_cross);
	}

	std::vector<int32> world_server::get_activity_open_time(int32 activity_type)
	{
		std::vector<int32> open_time;
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return open_time;
		}

		bool is_cross = false;
		open_time = _activity_cfg_ptr->ActivityOpenTime;

		if ((_activity_cfg_ptr->CrossActivityOpenTime.size() > 0 || _activity_cfg_ptr->CrossOpenDate.size() > 0) && get_need_begin_cross_gm_common((e_need_server_cross)_activity_cfg_ptr->ActivityCrossType))
		{
			is_cross = true;
		}

		if (is_cross)
		{
			open_time = _activity_cfg_ptr->CrossActivityOpenTime;
		}
		return open_time;
	}

	std::vector<int32> world_server::get_activity_open_data(int32 activity_type)
	{
		std::vector<int32> open_data;
		ActivityCommonConfigTemplate* _activity_cfg_ptr = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + activity_type);
		if (_activity_cfg_ptr == nullptr || 1 != _activity_cfg_ptr->IsOpen)
		{
			return open_data;
		}

		bool is_cross = false;

		if (_activity_cfg_ptr->CrossOpenDate.size() > 0 && get_need_begin_cross_gm_common((e_need_server_cross)_activity_cfg_ptr->ActivityCrossType))
		{
			is_cross = true;
		}

		if (is_cross)
		{
			open_data = _activity_cfg_ptr->CrossOpenDate;
		}
		else
		{
			open_data = _activity_cfg_ptr->OpenDate;
		}

		return open_data;
	}

	void world_server::send_server_act_hidden_to_client(client_session* session)
	{
		game_proto_server_act_is_open_end msg;
		msg.set_is_hiden(m_server_act_hidden);
		if (nullptr == session)//如果不填session表示发给所有玩家
		{
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_mgsindex_s2c_send_server_act_is_open);
		}
		else
		{
			session->send_to_client(&msg, e_mgsindex_s2c_send_server_act_is_open);
		}
	}

	void world_server::send_server_config_all_to_client(client_session* session)
	{
		game_proto_server_config_all msg;
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			s_game_info& game_info_ref = it->second;
			msg.add_server_id(game_info_ref.server_id);
			if (game_info_ref.server_id == game_info_ref.cross_id)
			{
				msg.add_is_cross_server(1);
			}
			else
			{
				msg.add_is_cross_server(0);
			}
			msg.add_server_ip(game_info_ref.out_ip);
			msg.add_server_name(game_info_ref.server_name);
		}
		int32 len = m_cross_info_map.size();
		for (int32 i = len; i < max_server_num; ++i)
		{
			msg.add_server_id(0);
			msg.add_is_cross_server(0);
			msg.add_server_ip("");
			msg.add_server_name("");
		}
		if (nullptr == session)//如果不填session表示发给所有玩家
		{
			client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_player_server_config);
		}
		else
		{
			session->send_to_client(&msg, e_msgindex_s2c_player_server_config);
		}
	}

	void world_server::send_server_config_to_db()
	{
		world_boss_ws_mgr::get_instance().init_manager();
		arena_mgr_ws::get_instance().init_manager();
		ranking_mgr_ws::init_manager();
		marry_mgr_ws::get_instance().init_manager();
		gain_treasure_ws_mgr::get_instance().init_manager();
		cross_server_pk_ws_mgr::get_instance().init_manager();
		cross_server_harry_ws_mgr::get_instance().init_manager();
		cross_server_city_war_ws_mgr::get_instance().init_manager();
		cross_ladder_ws_mgr_new::get_instance().init_manager();
		cross_server_world_boss_ws_mgr::get_instance().init_manager();
		time_limit_activity_temp_ws_mgr::get_instance().init_manager();
		time_limit_activity_ws_mgr::get_instance().init_manager();
		cloud_shop_mgr::get_instance().init_manager();
		recharge_mgr_ws::get_instance().init_manager();
		element_war_ws_mgr::get_instance().init_manager();
		attack_city_ws_mgr::get_instance().init_manager();
		CONSOLE_INFO("ws start ok");
	}
	bool world_server::set_begin_time(const int32 begin_time)
	{
		m_begin_time = begin_time;
		tm* tm_ptr = time_helper::get_tm_by_time(m_begin_time);
		if (nullptr == tm_ptr)
		{
			return false;
		}
		globle_data::get_instance().init_server_open_time(m_begin_time);
		on_open_time_changed();
		ws2dp_gm_open_time req;
		req.open_time = begin_time;
		bool is_use = proto_by_lua(e_msgindex_ws2dp_gm_open_time);
		if (is_use == false)
		{
			world_server::getInstance().broadcast(&req, sizeof(req), e_server_type_cs);
		}
		else
		{
			hld::ws2cs_proto::cs_gm_open_time pro_msg;
			pro_msg.set_open_time(begin_time);
			world_server::getInstance().broadcast_lua(&pro_msg, e_msgindex_ws2dp_gm_open_time, e_server_type_cs);
		}
		return true;
	}

	int32 world_server::get_need_begin_cross_gm_common(e_need_server_cross gm_type)
	{
		if (gm_type < e_need_server_cross_begin_cross || gm_type >= e_need_server_cross_max)
		{
			return false;
		}
		return m_need_begin_cross_config[gm_type];
	}

	void world_server::set_need_begin_cross_gm_common(const int32 need_cross, e_need_server_cross gm_type)
	{
		if (gm_type < e_need_server_cross_begin_cross || gm_type >= e_need_server_cross_max)
		{
			return;
		}
		m_need_begin_cross_config[gm_type] = need_cross;
	}
	void world_server::send_begin_cross_gm_to_cs(e_need_server_cross gm_type)
	{
		if (gm_type < e_need_server_cross_begin_cross || gm_type >= e_need_server_cross_max)
		{
			return;
		}
		ws2cs_server_need_cross gm_msg;
		gm_msg.gm_type = gm_type;
		gm_msg.need_begin_cross = m_need_begin_cross_config[gm_type];

		bool is_use = proto_by_lua(e_msg_index_ws2cs_need_cross);
		if (is_use == false)
		{
			broadcast(&gm_msg, sizeof(gm_msg), e_server_type_cs);
		}
		else
		{
			hld::ws2cs_proto::server_need_cross pro_msg;
			pro_msg.set_gm_type(gm_type);
			pro_msg.set_need_begin_cross(m_need_begin_cross_config[gm_type]);
			broadcast_lua(&pro_msg, e_msg_index_ws2cs_need_cross, e_server_type_cs);

		}
	}

	void world_server::send_begin_cross_gm_to_client(e_need_server_cross gm_type)
	{
		if (gm_type < e_need_server_cross_begin_cross || gm_type >= e_need_server_cross_max)
		{
			return;
		}
		game_proto_gm_begin_cross_one msg;
		msg.set_gm_type(gm_type);
		msg.set_is_begin(m_need_begin_cross_config[gm_type]);
		client_session_mgr::getInstance().send_message_to_all_client(&msg, e_msgindex_s2c_need_cross_gm_one);
	}
	void world_server::save_server_info_to_db(int32 server_info_type, s_daemon_callback_info callback_info)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			return;
		}
		//ws2dp_set_server_info req;
		//req.server_info_value = m_server_info_arr[server_info_type];
		//req.server_info_type = server_info_type;
		//req.callback_info = callback_info;
		//ws_client::getInstance().send_to_dp(&req, sizeof(req));

		script_mgr::get_instance().call_func("ws2dp_send", "send_gm_set_server_info_lua", 0, false, "%d%d%d%l", m_server_info_arr[server_info_type],server_info_type, callback_info.listen_port, callback_info.handle_index);

	}

	void world_server::send_all_server_info_to_cs()
	{
		ws2cs_server_info_all msg;
		memcpy(msg.server_info_arr, m_server_info_arr, e_server_info_type_max * sizeof(int32));

		bool is_use = proto_by_lua(e_msg_index_ws2cs_server_info_all);
		if (is_use == false)
		{
			broadcast(&msg, msg.get_pak_length(), e_server_type_cs);
		}
		else
		{
			hld::ws2cs_proto::server_info_all pro_msg;
			for (int32 i = 0 ; i < e_server_info_type_max; i++)
			{
				pro_msg.add_server_info_arr(m_server_info_arr[i]);
			}

			broadcast_lua(&pro_msg, e_msg_index_ws2cs_server_info_all, e_server_type_cs);

		}
	}
	void world_server::send_server_info_arr_to_cs(int32 server_info_type)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			return;
		}
		ws2cs_server_info_arr gm_msg;
		gm_msg.server_info_type = server_info_type;
		gm_msg.server_info_value = m_server_info_arr[server_info_type];
		bool is_use = proto_by_lua(e_msg_index_ws2cs_server_Info_arr);
		if (is_use == false)
		{
			broadcast(&gm_msg, sizeof(gm_msg), e_server_type_cs);
		}
		else
		{
			hld::ws2cs_proto::server_info_arr pro_msg;
			pro_msg.set_server_info_type(server_info_type);
			pro_msg.set_server_info_value(m_server_info_arr[server_info_type]);
			broadcast_lua(&pro_msg, e_msg_index_ws2cs_server_Info_arr, e_server_type_cs);
		}
	}
	void world_server::send_need_begin_cross_pk_to_client(client_session* session)
	{
		if (session == nullptr)
		{
			return;
		}
		game_proto_gm_begin_cross_all msg;
		for (int32 i = e_need_server_cross_begin_cross; i < e_need_server_cross_max; i++)
		{
			msg.add_is_begin_arr(m_need_begin_cross_config[i]);
		}
		session->send_to_client(&msg, e_msgindex_s2c_need_cross_gm_all);

		game_proto_gm_begin_cross cross_msg;
		cross_msg.set_is_begin_cross(m_need_begin_cross_config[e_need_server_cross_begin_cross]);
		cross_msg.set_gate_server_id(world_server::getInstance().get_cross_id());
		session->send_to_client(&cross_msg, e_msgindex_s2c_need_cross_server);

		game_proto_is_close_exchange close_msg;
		close_msg.set_is_close(m_server_info_arr[e_server_info_type_close_exchange]);
		session->send_to_client(&close_msg, e_msgindex_s2c_is_close_exchange);
	}

	bool world_server::is_have_this_server(int32 server_id)
	{
		game_info_map_it it = m_cross_info_map.find(server_id);
		if (it == m_cross_info_map.end())
		{
			return false;
		}
		return true;
	}

	s_game_info* world_server::get_server_config_by_server_id(int32 server_id)
	{
		game_info_map_it it = m_cross_info_map.find(server_id);
		if (it == m_cross_info_map.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	s_game_info * world_server::get_gate_server_config_by_server_id(int32 server_id)
	{
		game_info_map_it it = m_gate_cross_info_map.find(server_id);
		if (it == m_gate_cross_info_map.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	void world_server::send_need_begin_cross(int32 conn_index)
	{
		ws2cs_server_need_cross msg;
		for (int32 i = e_need_server_cross_begin_cross; i < e_need_server_cross_max; i++)
		{
			msg.need_begin_cross = m_need_begin_cross_config[i];
			msg.gm_type = (e_need_server_cross)i;

			bool is_use = proto_by_lua(e_msg_index_ws2cs_need_cross);
			if (is_use == false)
			{
				broadcast(&msg, sizeof(msg), e_server_type_cs);
			}
			else
			{
				hld::ws2cs_proto::server_need_cross pro_msg;
				pro_msg.set_gm_type(i);
				pro_msg.set_need_begin_cross(m_need_begin_cross_config[i]);
				broadcast_lua(&pro_msg, e_msg_index_ws2cs_need_cross, e_server_type_cs);
			}
		}
		//ws2cs_server_info_all info_msg;
		//memcpy(info_msg.server_info_arr, m_server_info_arr, e_server_info_type_max * sizeof(int32));
		//net_server_mgr::getInstance().send_message(&info_msg, info_msg.get_pak_length(), conn_index, e_server_type_cs);
	}

	void world_server::set_server_info_arr(const int32 server_info_value, int32 server_info_type)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			return;
		}
		m_server_info_arr[server_info_type] = server_info_value;
	}

	int32 world_server::get_server_info_arr(int32 server_info_type)
	{
		if (server_info_type < e_server_info_type_begin_cross_server_time || server_info_type >= e_server_info_type_max)
		{
			return 0;
		}
		return m_server_info_arr[server_info_type];
	}

	bool world_server::init()
	{
		if (m_world_server_start)
		{
			return true;
		}

		// retrieve options
		xstring ip = WSCONFIG->internal_host;
		int32 port = WSCONFIG->internal_port;
		
		if (false == net_server_mgr::getInstance().init(e_server_type_ws, ip, port,
			WS_SERVER_SEND_BUFF_SIZE, WS_SERVER_RECV_BUFF_SIZE, INTERNAL_SERVER_MAX_PACKET_SIZE, WS_NEED_SERVER_COUNT, WS_NEED_SERVER_COUNT,
			boost::bind(&world_server::handler_onclose, this, _1)))
		{
			return false;
		}
		return true;
	}
	bool world_server::start()
	{
		m_timerindex_gameloop = scheduler::getInstance().add_timer(e_gameloop_interval, boost::bind(&world_server::server_loop, this, _1));
		return net_server_mgr::getInstance().start();
	}
	void world_server::stop()
	{
		ws_client::getInstance().stop();
		scheduler::getInstance().remove_timer(m_timerindex_gameloop);
	}
	void world_server::handler_onclose(const net_server* faith_server_ptr)
	{
		if (NULL == faith_server_ptr)
		{
			return;
		}
		switch (faith_server_ptr->get_server_type())
		{
		case e_server_type_fep:
		{
			client_session_mgr::getInstance().logout_by_fep(faith_server_ptr->get_conn_index());
		}
		break;
		case e_server_type_cs:
		{
			client_session_mgr::getInstance().logout_by_cs(faith_server_ptr->get_conn_index());
		}
		break;
		default:
			break;
		}
	}
	void world_server::internal_req_login(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		rep_login			pak_rep;

		const req_login* msg = static_cast<const req_login*>(data_ptr);
		if (NULL == msg)
		{
			return;
		}
		if (msg->server_info.server_type >= e_server_type_max)
		{
			return;
		}
		if (msg->server_info.server_index < 0)
		{
			return;
		}
		net_server* faith_server_ptr = net_server_mgr::getInstance().get_peer_by_conn_index(conn_index);
		if (nullptr == faith_server_ptr)
		{
			return;
		}
		faith_server_ptr->set_server_info(msg->server_info);
		std::cout << msg->server_info.ip_addr << " " << msg->server_info.port << " " << msg->server_info.server_type << std::endl;
		pak_rep.cross_server_id = get_cross_id();
		pak_rep.open_time = m_begin_time;
		send_by_uid(conn_index, &pak_rep, data_len);
		switch (msg->server_info.server_type)
		{
		case e_server_type_fep:
		{
			if (msg->server_info.server_index > SERVER_FEP_COUNT)
			{
				return;
			}
			m_fep_conn_index[msg->server_info.server_index] = conn_index;
			//用于fep重启后向fep发表
			if (world_server::getInstance().is_loading_flag_finish(e_ws_flag_fep_connect))
			{
				time_limit_activity_temp_ws_mgr::get_instance().send_to_fep_template();
			}
			world_server::getInstance().set_ws_loading_flag(e_ws_flag_fep_connect);
		}
		break;
		case e_server_type_cs:
		{
			if (msg->server_info.server_index > SERVER_CS_COUNT)
			{
				return;
			}
			if (m_server_id == get_cross_id())
			{
				cs_map_mgr_system::init_map_by_cross_world(conn_index);
			}
			else
			{
				cs_map_mgr_system::init_map_by_world(conn_index);
			}
			if (world_server::getInstance().is_ws_all_load_end())
			{
				world_server::getInstance().send_msg_to_cs_with_load_end();
			}
			//用于cs重启后向cs发表
			if (world_server::getInstance().is_loading_flag_finish(e_ws_flag_init_time_limit_template))
			{
				time_limit_activity_temp_ws_mgr::get_instance().send_to_cs_template();
			}

			world_server::getInstance().set_ws_loading_flag(e_ws_flag_cs_connect);
		}
		break;
		default:
			break;
		}
		if (msg->server_info.server_type == e_server_type_cs)
		{
		}
	}

	void world_server::internal_appserver_dataupdate(uint32 conn_index, const void* data_ptr, size_t data_len)
	{
		const app_server_update* msg = static_cast<const app_server_update*>(data_ptr);

		//	验证合法性
		net_server* faith_server_ptr = net_server_mgr::getInstance().get_peer_by_conn_index(conn_index);
		if (faith_server_ptr == NULL)
		{
			return;
		}
		faith_server_ptr->set_cur_count(msg->player_count);
		faith_server_ptr->set_max_count(msg->max_player_count);
	}
	void world_server::send_by_uid(int32 conn_index, const void* data_ptr, size_t data_len)
	{
		net_server_mgr::getInstance().send_message(data_ptr, data_len, conn_index);
	}
	void world_server::send_by_uid_lua(int32 conn_index, google::protobuf::Message* net_pro, uint32 header)
	{
		packet_s2s *p_s2s = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
		if (p_s2s == nullptr)
		{
			return;
		}
		send_by_uid(conn_index, p_s2s, p_s2s->get_packet_len());
	}
	void world_server::send_to_fep(int32 server_index, const void* data_ptr, size_t data_len)
	{
		if (server_index < 0 || server_index >= SERVER_FEP_COUNT)
		{
			return;
		}
		net_server_mgr::getInstance().send_message(data_ptr, data_len, m_fep_conn_index[server_index], e_server_type_fep);
	}
	void world_server::send_to_fep_all(const void* data_ptr, size_t data_len)
	{
		for (int32 i = 0; i < SERVER_FEP_COUNT; ++i)
		{
			net_server_mgr::getInstance().send_message(data_ptr, data_len, m_fep_conn_index[i], e_server_type_fep);
		}
	}
	void world_server::send_to_cs(int32 conn_index, const void* data_ptr, size_t data_len)
	{
		if (conn_index < 0 || conn_index >= WS_NEED_SERVER_COUNT)
		{
			return;
		}
		net_server_mgr::getInstance().send_message(data_ptr, data_len, conn_index, e_server_type_cs);
	}
	void world_server::send_to_cs(const void* data_ptr, size_t data_len)
	{
		net_server_mgr::getInstance().send_message(data_ptr, data_len, -1, e_server_type_cs);
	}
	void world_server::broadcast(const void* data_ptr, size_t data_len, e_server_type server_type)
	{
		net_server_mgr::getInstance().send_message(data_ptr, data_len, -1, server_type);
	}
	void world_server::broadcast_lua(google::protobuf::Message* net_pro, uint32 header, e_server_type server_type)
	{
		packet_s2s *p_s2s = parse_msg::getInstance().serialze_server(net_pro, header, guid_64(), 0);
		if (p_s2s == nullptr)
		{
			return;
		}
		broadcast(p_s2s, p_s2s->get_packet_len(), server_type);
	}
	
	void world_server::send_message_out(const void* data_ptr, size_t data_len, int32 conn_index, e_server_type server_type)
	{
		net_server_mgr::getInstance().send_message_out(data_ptr, data_len, conn_index, server_type);
	}
	void world_server::server_loop(uint32 timer_index)
	{
		ZoneScoped;
		static int64 game_time 	= utility::get_tick_count();
		int64 time_now			= utility::get_tick_count();
		uint32 elapse_time	= time_now - game_time;
		float elapse_time_sec = (float)elapse_time / 1000;

		static int32 heart_beat_log_timer = 0;
		static int32 loop_counter = 0;
		//float temp_time = elapse_time;
		game_time = time_now;

		check_send_get_gate_msg(time_now);
		if (world_server::getInstance().is_ws_all_load_end())
		{
			login_role_mgr::getInstance().tick(0);
			arena_mgr_ws::get_instance().heart_tick(elapse_time);
			ranking_mgr_ws::heart_tick(time_now);
			legion_ws_mgr::get_instance().tick(time_now);
			world_boss_ws_mgr::get_instance().tick(elapse_time);
			team_ws_mgr::get_instance().tick(time_now);
			red_package_ws_mgr::get_instance().tick(elapse_time);
			cs_map_mgr_system::tick(elapse_time_sec);
			role_pk_manager::getInstance().tick(elapse_time);
			event_ws_mgr::get_instance().tick(time_now);
			client_session_mgr::getInstance().tick(time_now);
			queued_login_mgr::getInstance().tick(time_now);
			big_player_ws_mgr::get_instance().tick(elapse_time);
			re_login_mgr::getInstance().tick(elapse_time);
			broken_sky_mgr::get_instance().tick(time_now);
			chest_arrival_mgr::get_instance().tick(elapse_time_sec);
			auction_mgr_ws::get_instance().tick(time_now);
			crystal_fairyland_mgr::get_instance().tick(time_now);
			web_client::get_instance().tick(time_now);
			cloud_shop_mgr::get_instance().tick(time_now);
			time_limit_activity_temp_ws_mgr::get_instance().heart_tick(time_now);
			time_limit_activity_ws_mgr::get_instance().tick(time_now);
			legion_ws_answer::get_instance().tick(time_now);
			recharge_mgr_ws::get_instance().tick(time_now);
			pk_king_mgr::get_instance().tick(elapse_time);
			cross_server_pk_ws_mgr::get_instance().tick(elapse_time);
			cross_server_harry_ws_mgr::get_instance().tick(elapse_time);
			cross_server_world_boss_ws_mgr::get_instance().tick(elapse_time);
			cross_server_city_war_ws_mgr::get_instance().tick(elapse_time);
			cross_ladder_ws_mgr_new::get_instance().tick(time_now);
			mail_ws_mgr::get_instance().tick(time_now);
			element_war_ws_mgr::get_instance().tick(time_now);
			attack_city_ws_mgr::get_instance().tick(time_now);
			tick(time_now);
#ifdef OPEN_LUA_TICK
			script_mgr::get_instance().call_func(nullptr, "UpdateSingleton", 0, false, "%l", time_now);
#endif
		}
		else
		{
			daemon_client::getInstance().set_server_error_string("do loading flag");
		}


		loop_counter++;
		heart_beat_log_timer += elapse_time;

		static int64 ws_load_flag_time = time_now + minute_tick_time;
		if (time_now > ws_load_flag_time && !is_ws_all_load_end() && ws_load_flag_time > 0)
		{
			ws_load_flag_time = -1;
			set_flag_when_one_min_start();
		}

		static int64 last_log_time = 0;
		if (time_now > last_log_time || last_log_time == 0)
		{
			last_log_time = time_now + server_console_time;

			if (m_gm_state)
			{
				CONSOLE_INFO("==========ws gm server status==========");
			}
			else
			{
				CONSOLE_INFO("==========ws server status==========");
			}
			if (globle_data::get_instance().get_version_template_ptr())
			{
				CONSOLE_INFO("res svn code : {}", globle_data::get_instance().get_version_template_ptr()->Version);
			}
			hld::int32 dp_num = net_client_mgr::getInstance().get_server_count(e_server_type_dp);
			hld::int32 fep_num = net_server_mgr::getInstance().get_server_count(e_server_type_fep);
			hld::int32 gate_num = net_client_mgr::getInstance().get_server_count(e_server_type_gate);
			hld::int32 cs_num = net_server_mgr::getInstance().get_server_count(e_server_type_cs);
			CONSOLE_INFO(
				"dp {}/{} cs {}/{} fep {}/{} gate {}/{}",dp_num, SERVER_DP_COUNT
				,cs_num, SERVER_CS_COUNT
				,fep_num, SERVER_FEP_COUNT
				,gate_num, SERVER_GATE_COUNT);
			// 输出信息
			int32 session_count = client_session_mgr::getInstance().get_session_num();
			CONSOLE_INFO("session num:{} session max:{}", session_count, init_session_max);

			//排队信息以这个为准
			int32 seesion_online_counter = queued_login_mgr::getInstance().get_online_counter();
			int32 seesion_queue_length = queued_login_mgr::getInstance().get_queue_length();
			CONSOLE_INFO("seesion_online_counter:{} seesion_queue_length:{}", seesion_online_counter, seesion_queue_length);


			int32 queue_count = queued_login_mgr::getInstance().get_queue_length();
			int32 in_game_count = client_session_mgr::getInstance().get_game_num();
			CONSOLE_INFO("queue_count num:{} in_game_count:{}", queue_count, in_game_count);
			//cs_map_mgr_system::show_big_map_unit_num();
			CONSOLE_INFO("tick:{}", heart_beat_log_timer / loop_counter);
			server_log::heart_beat_role_log(SERVERCONFIG->app_key, server_header_verstion, get_server_id(), session_count, queued_login_mgr::getInstance().get_queue_length());
			heart_beat_log_timer = 0;
			loop_counter = 0;
			server_data::get_instance().heart_tick(time_now, "ws", session_count);
			server_log::player_count_log(get_server_id(), session_count);

			set_log_var(log_head);
			set_log_common_head_part3(log_head, "null", get_server_id(), utility::get_tick_count());
			server_log::serverPlayerCount(log_head, session_count);

			time_info temp_time_info = time_helper::get_cur_time_new();
			int32 minute_1 = temp_time_info.minute_in_hour % 10;
			int32 minute_2 = temp_time_info.minute_in_hour / 10;
			if (minute_1 == 0 && temp_time_info.second_in_minute < 20)
			{
				static bool is_send_count = false;
				if (minute_2 == 0)
				{
					if (is_send_count)
					{
						return;
					}
					is_send_count = true;
					http_send::get_instance().send_role_count(SERVERCONFIG->app_key, SERVERCONFIG->game_id, m_channel_count);
					m_channel_count.clear();
				}
				else
				{
					is_send_count = false;
				}
				channel_cout_map channel_map = client_session_mgr::getInstance().get_channel_num();
				for (channel_cout_map_it it = channel_map.begin(); it != channel_map.end(); ++it)
				{
					m_channel_count[it->first].m_time_role_count[minute_2] = it->second;
				}
			}
			CONSOLE_INFO("ws loading flag:{}", m_ws_loading_flag);
		}

		// 输出信息
		int32 session_count = client_session_mgr::getInstance().get_session_num();
		if (daemon_client::getInstance().get_server_close() && session_count == 0)
		{
			CONSOLE_INFO("all fep close, all player offline, please shutdown ws");
			app_server::getInstance().stop();
		}
		daemon_client::getInstance().heart_tick(time_now);
		http_access_mgr::get_instance().tick(time_now);
		//script_mgr::get_instance().call_func(nullptr, "UpdateSingleton", 0, false, "%l", time_now);

	}

	void world_server::set_flag_when_one_min_start()
	{
		CONSOLE_INFO("ws flag error ={}", m_ws_loading_flag );
		//m_ws_loading_flag = 0;
		//send_msg_to_cs_with_load_end();
	}

	bool world_server::is_same_server_list()
	{
		std::vector<int32> cur_server_list;
		cur_server_list.clear();
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			cur_server_list.push_back(it->second.server_id);
		}
		if (cur_server_list.size() != m_last_server_list_server_id.size())
		{
			return false;
		}
		for (int32 i = 0; i < cur_server_list.size(); i++)
		{
			bool is_have = false;
			int32 server_id = cur_server_list[i];
			for (int32 j = 0; j < m_last_server_list_server_id.size(); j++)
			{
				if (server_id == m_last_server_list_server_id[j])
				{
					is_have = true;
					break;
				}
			}
			if (!is_have)
			{
				return false;
			}
		}
		return true;
	}

	void world_server::end_refresh_server_list()
	{
		if (is_sky_island_server())
		{
			std::string server_id_arr = "";
			for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
			{
				server_id_arr = server_id_arr + to_string(it->second.server_id) + ",";
			}
			CONSOLE_INFO("end_refresh_server_list_server_id :{}", server_id_arr);
			if (is_same_server_list())
			{
				return;
			}
			if (m_last_server_list_server_id.size() <= 0)
			{
				refresh_last_server_list_with_now();
				save_last_sever_list_to_db();
			}
			ranking_mgr_ws::delete_ranking_with_server_list_refresh_end();
			if (is_add_new_server_id())
			{
				cross_server_city_war_ws_mgr::get_instance().func_clear_all_city_war_msg();
				city_war_territory_mgr::get_instance().clear_all_bid_info();
				attack_city_ws_mgr::get_instance().clear_legion_list();
			}
			else
			{
				cross_server_city_war_ws_mgr::get_instance().refresh_rank_with_server_list();
				city_war_territory_mgr::get_instance().refresh_bid_info_when_server_list_end();
				attack_city_ws_mgr::get_instance().refresh_legion_list();
			}
			cross_server_harry_ws_mgr::get_instance().refresh_server_harry_with_server_list();
			refresh_last_server_list_with_now();
			save_last_sever_list_to_db();
		}
	}

	void world_server::check_send_get_gate_msg(const uint64& tick_time)
	{
		if (!gate_proxy::getInstance().is_gate_run())
		{
			return;
		}
		int64 tick_sec = tick_time / second_tick_time;
		static int64 tick_day_last_min = 0;
		if (tick_sec > tick_day_last_min)
		{
			tick_day_last_min = tick_sec;
			if (m_need_send_gate_flag_arr.size() > 0)
			{
				auto ite = m_need_send_gate_flag_arr.begin();
				for (ite; ite != m_need_send_gate_flag_arr.end(); ite++)
				{
					send_to_gate_get_msg(*ite);
				}
				m_need_send_gate_flag_arr.clear();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
			}
		}
	}

	void world_server::send_refresh_to_cs()
	{
		ws2cs_server_is_need_refresh_cs_daily_info_at_zero_hour msg;
		net_server_mgr::getInstance().send_message(&msg, sizeof(msg), -1, e_server_type_cs);
	}

	void world_server::tick(const uint64& tick_time)
	{
		int64 cur_time = time_helper::get_cur_time_new().second;
		if (is_need_refresh_ws_daily_info_at_zero_hour())
		{
			globle_data::get_instance().set_server_days();
			refresh_day_for_zero(tick_time);
			send_refresh_to_cs();
			m_server_time_zero = get_next_refresh_time(e_refresh_type_zero);
			if (m_server_time_zero >= 0)
			{
				save_server_attr_val_db(e_srv_attr_val_refresh_zero, m_server_time_zero);
			}
		}
		if (cur_time >= m_server_time_five)
		{
			refresh_day_for_five();
			m_server_time_five = get_next_refresh_time(e_refresh_type_five);
			if (m_server_time_five >= 0)
			{
				save_server_attr_val_db(e_srv_attr_val_refresh_five, m_server_time_five);
			}
		}
		if (cur_time >= m_server_time_refresh_harry && m_server_time_refresh_harry != 0)
		{
			refresh_for_harry_time();
			m_server_time_refresh_harry = get_next_harry_refresh_time();
			save_server_attr_val_db(e_srv_attr_val_server_harry_refresh_time, m_server_time_refresh_harry);
		}
		else if (m_server_time_refresh_harry == 0)//为0不刷新
		{
			m_server_time_refresh_harry = get_next_harry_refresh_time();
			if (m_server_time_refresh_harry > 0)
			{
				save_server_attr_val_db(e_srv_attr_val_server_harry_refresh_time, m_server_time_refresh_harry);
			}
		}
		if (is_need_refresh_ws_daily_info_at_week_time())
		{
			m_server_time_week_constribution_refresh = get_next_week_refresh_time();
			if (m_server_time_week_constribution_refresh > 0)
			{
				save_server_attr_val_db(e_srv_attr_val_week_contribution_refresh_time, m_server_time_week_constribution_refresh);
			}
		}
		if (is_need_refresh_ws_daily_info_at_week_twenty_three_time())
		{
			m_server_time_week_legion_welfare_refresh = get_next_week_twenty_three_refresh_time();
			legion_ws_mgr::get_instance().refresh_week_for_twenty_three();
			if (m_server_time_week_legion_welfare_refresh > 0)
			{
				save_server_attr_val_db(e_srv_attr_val_week_welfare_refresh_time, m_server_time_week_legion_welfare_refresh);
			}
		}
		if (is_need_refresh_ws_daily_info_at_clear_city_bid_time())
		{
			if (m_server_time_clear_city_war_bid > 0)
			{
				city_war_territory_mgr::get_instance().clear_bid_info_for_time();
				legion_ws_mgr::get_instance().clear_all_legion_city_war_member();
			}
			m_server_time_clear_city_war_bid = get_next_clear_city_bid_time();
			if (m_server_time_clear_city_war_bid > 0)
			{
				save_server_attr_val_db(e_srv_attr_val_clear_city_war_bid_time, m_server_time_clear_city_war_bid);
			}
		}
		if (m_is_begin_reload_csv)
		{
			m_is_begin_reload_csv = false;
			CONSOLE_INFO("m_is_begin_reload_csv_end");
			if (m_server_id != get_gate_server_id())
			{
				server2gate_reload_csv_end msg;
				msg.server_id = m_server_id;
				msg.set_out_ip(m_out_ip);
				ws_client::getInstance().only_send_to_gate(&msg, sizeof(msg));
			}
		}
		if (false)
		{
			reload_csv_func();
		}
	}

	void world_server::reload_csv_func()
	{
		CONSOLE_INFO("ws reload_csv_func begin");
		m_is_begin_reload_csv = true;
		if (m_is_begin_cs_reload_csv)
		{
			CONSOLE_INFO("cs_is_in_reload_csv");
			return;
		}
		if (get_gate_server_id() == m_server_id)
		{
			server2gate_reload_csv msg;
			msg.need_reload = 1;
			ws_client::getInstance().only_send_to_gate(&msg, sizeof(msg));
		}
		template_manager::get_instance().remove_template(e_RoleRewardTemplate);
		template_manager::get_instance().remove_template(e_OperatorUseActivityTemplate);
		template_manager::get_instance().remove_template(e_OperatorUseActivityBranchTemplate);
		template_manager::get_instance().remove_template(e_TimeLimitActivityTemplate);
		template_manager::get_instance().remove_template(e_TimeLimitActivityBranchTemplate);
		template_manager::get_instance().init();

		m_is_begin_cs_reload_csv = true;
			reload_csv req;
			bool is_use = proto_by_lua(e_msg_index_reload_csv);
			if (is_use == false)
			{
				broadcast(&req, sizeof(req));
			}
			else
			{
				hld::ws2cs_proto::reload_csv msg;
				broadcast_lua(&msg, e_msg_index_reload_csv);
			}

		ws_client::getInstance().send_to_dp(&req, sizeof(req));
		time_limit_activity_temp_ws_mgr::get_instance().load_new_temp(false, false);
		time_limit_activity_ws_mgr::get_instance().init_all_time_limit_ws();
		cloud_shop_mgr::get_instance().send_open_msg_to_all_client();
		cross_ladder_ws_mgr_new::get_instance().init_manager(false);
		element_war_ws_mgr::get_instance().init_manager(false);
		attack_city_ws_mgr::get_instance().init_manager(false);
		client_session_mgr::getInstance().send_temp_to_all_session();
	}
	
	void world_server::send_msg_when_gate_register(int64 flag)
	{
		switch (flag)
		{
		case e_ws_flag_server_cross_time:
		{
			ws2ws_cross_server_time msg;
			msg.cross_server_time = m_server_info_arr[e_server_info_type_begin_cross_server_time];
			cross::send_msg_to_ws(guid_64(), get_cross_id(), e_msgindex_ws2ws_cross_server_time, &msg, sizeof(msg));
		}
		break;
		default:
			break;
		}
	}

	void world_server::refresh_for_harry_time()
	{
		if (!is_sky_island_server())
		{
			ranking_list* cur_ranking_list_ptr = ranking_mgr_ws::get_ranking_list_by_type(e_RankingIndex_cross_server_plinder_guard_value);
			if (nullptr == cur_ranking_list_ptr)
			{
				return;
			}
			guid_64 best_guid[3];
			xchar player_name[max_name_size + 1];
			memset(player_name, 0, sizeof(player_name));
			for (int32 i = 0; i < 3; i++)
			{
				best_guid[i].clear_data();
			}
			ranking_list_ite cur_list_ite = cur_ranking_list_ptr->begin();
			for (int32 i = 0; i < 3; i++)
			{
				if (cur_list_ite == cur_ranking_list_ptr->end())
				{
					continue;
				}
				best_guid[i] = cur_list_ite->role_guid;
				if (i == 0)
				{
					memcpy(player_name, cur_list_ite->role_name, max_name_size);
				}
				cur_list_ite++;
			}
			cross_server_harry_ws_mgr::get_instance().set_server_harry_best_player(get_server_id(), best_guid[0], best_guid[1], best_guid[2], player_name);
			cross_server_harry_ws_mgr::get_instance().send_server_harry_best_player_msg_to_gate();
			cross_server_harry_ws_mgr::get_instance().send_award_with_player_defend_value();
			ranking_mgr_ws::clear_data_by_ranking_type(e_RankingIndex_cross_server_plinder_guard_value);
		}
		if (is_sky_island_server())
		{
			cross_server_harry_ws_mgr::get_instance().send_award_with_server_money_rank();
			cross_server_harry_ws_mgr::get_instance().init_server_money_with_refresh();
			cross_server_harry_ws_mgr::get_instance().send_all_harry_msg_to_other_server();
			return;
		}
	}

	void world_server::set_ladder_world_level(int64 world_level, bool need_send_to_cs, bool need_save)
	{

		if (world_level > 0)
		{
			m_server_ladder_wrold_level = world_level;
		}
		else
		{
			m_server_ladder_wrold_level = ranking_mgr_ws::get_srv_avg_level();
		}
		if (need_send_to_cs)
		{
			ws2cs_set_ladder_world_level msg;
			msg.world_level = m_server_ladder_wrold_level;
			net_server_mgr::getInstance().send_message(&msg, sizeof(msg), -1, e_server_type_cs);
		}
		if (need_save)
		{
			save_server_attr_val_db(e_srv_attr_val_ladder_world_level, m_server_ladder_wrold_level);
		}
	}

	int64 world_server::get_next_harry_refresh_time()
	{
		int64 next_refresh_time = 0;
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_cross_server_harry);
		if (nullptr == act_com_cfg)
		{
			return next_refresh_time;
		}
		int32 cur_day = time_helper::get_cur_time_new().day_in_week;
		int32 refresh_day = act_com_cfg->ParamInt1;
		int64 stamp_by_hour_min = time_helper::get_stamp_by_hour_min(0, 0);
		int32 need_day = 0;
		if (refresh_day > cur_day)
		{
			need_day = refresh_day - cur_day;
		}
		else if (refresh_day == cur_day)
		{
			need_day = all_day_in_week;
		}
		else
		{
			need_day = all_day_in_week - cur_day + refresh_day;
		}
		next_refresh_time = stamp_by_hour_min + need_day * day_time_second;
		return next_refresh_time;
	}

	void world_server::on_open_time_changed()
	{
		
	}

	int64 world_server::get_next_refresh_time(srv_refresh_type time_type)
	{
		int64 next_refresh_time = -1;

		RegionTemplate* region_template_ptr = globle_data::get_instance().get_region_template_ptr();
		if (region_template_ptr == nullptr
			|| region_template_ptr->RegionCode >= GAMECONFIG->SpecialRefreshTimeHour.size())
		{
			return next_refresh_time;
		}
		int64 cur_time = time_helper::get_cur_time_new().second;
		int32 time_hour = 0;
		int64 server_time = m_server_time_zero;
		if (time_type == e_refresh_type_five)
		{
			time_hour = GAMECONFIG->SpecialRefreshTimeHour[region_template_ptr->RegionCode];
			server_time = m_server_time_five;
		}
		int64 stamp_by_hour_min = time_helper::get_stamp_by_hour_min(time_hour, 0);
		if (false == time_helper::is_diff_day(server_time, cur_time))
		{
			next_refresh_time = stamp_by_hour_min + day_time_second;
		}
		else if (cur_time > server_time)
		{
			if (cur_time < stamp_by_hour_min)
			{
				next_refresh_time = stamp_by_hour_min;
			}
			else if (cur_time > stamp_by_hour_min)
			{
				next_refresh_time = stamp_by_hour_min + day_time_second;
			}
		}
		
		return next_refresh_time;
	}

	void world_server::refresh_day_for_zero(const uint64& tick_time)
	{
		legion_ws_mgr::get_instance().refresh_day_for_zero();
		cross_ladder_ws_mgr_new::get_instance().refresh_daily_ticket_info();
		set_cross_server_open_day();
		open_act_with_cross_server_open_day();
		time_limit_activity_ws_mgr::get_instance().refresh_day_for_zero(tick_time);
		cloud_shop_mgr::get_instance().send_open_msg_to_all_client();
		cross_server_harry_ws_mgr::get_instance().set_is_need_create_harry_player(true);
		element_war_ws_mgr::get_instance().refresh_daily_ticket_info();
	}

	void world_server::refresh_day_for_five()
	{
		legion_ws_mgr::get_instance().refresh_day_for_five();
		ranking_mgr_ws::sync_calcu_world_level_tick_day();

		cross_server_city_war_ws_mgr::get_instance().set_refresh_cross_city_war(0);
		save_server_attr_val_db(e_srv_attr_val_is_refresh_cross_city_war, 0);
	}

	void world_server::load_server_attr_val()
	{
		//ws2dp_load_server_attr_val load_server_attr_val_msg;
		//ws_client::getInstance().send_to_dp(&load_server_attr_val_msg, sizeof(load_server_attr_val_msg));

		server2dp_proto_ws2dp_load_server_attr_val msg;
		ws_client::getInstance().send_to_dp_new(&msg, e_msg_ws2dp_load_server_attr_val);
	}

	void world_server::save_server_attr_val_db(server_attr_val_type _type, int64 _val)
	{
		int32 cur_server_open_tm = world_server::getInstance().get_begin_time();
		int32 cur_time = time_helper::get_cur_time_new().second;
		if (cur_server_open_tm > cur_time || cur_server_open_tm <= 0)
		{
			return;
		}
		//s_server_attr_int svr_att;
		//svr_att.attr_type = _type;
		//svr_att.attr_val = _val;
		//ws2dp_save_server_attr_val save_svr_att_msg;
		//save_svr_att_msg.attr_int = svr_att;
		//ws_client::getInstance().send_to_dp(&save_svr_att_msg, sizeof(save_svr_att_msg));

		server2dp_proto_ws2dp_save_server_attr_val svr_att;
		svr_att.set_attr_type((int32)_type);
		svr_att.set_attr_val(_val);
		ws_client::getInstance().send_to_dp_new(&svr_att, e_msg_ws2dp_save_server_attr_val);
	}
	void world_server::handler_daemon_onrecv(const void* data_ptr, size_t data_len)
	{
		int32 msg_herder = parse_msg::getInstance().get_packet_header(data_ptr, data_len);

		switch (msg_herder)
		{
		case e_msgindex_d2s_close_process:
		{
			close_server(data_ptr, data_len);
		}
		break;
		case e_msgindex_d2s_gm_order:
		{
			gm_order_from_daemon(data_ptr, data_len);
		}
		break;
		default:
			break;
		}


		protocolUnpacker unpacker(data_ptr, data_len);
		int32 server_id = 0;
		unpacker.pop(server_id);

		switch (unpacker.getProtocol())
		{
		case e2s_proto_def:
		{
			if (unpacker.getSubProtocol() == e2s_exit_def)
			{
				daemon2ws_exit_func(unpacker);
			}
			else if (unpacker.getSubProtocol() == e2s_gm_cmd_def)
			{
				daemon2ws_gmcmd_func(unpacker);
			}
		}
		break;
		}
	}

	void world_server::close_server(const void* data_ptr, size_t data_len)
	{
		eye_proto_d2s_close_process request;
		if (!parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len))
		{
			return;
		}
		if (request.server_id() != SERVERCONFIG->game_id
			|| request.server_type() != e_server_type_ws)
		{
			return;
		}

		daemon_client::getInstance().set_server_close(true);
		CONSOLE_INFO("FaithEye Stop Game!");
	}

	void world_server::gm_order_from_daemon(const void* data_ptr, size_t data_len)
	{
		s_daemon_callback_info temp_callback_info;
		eye_proto_d2s_gm_order request;
		if (!parse_msg::getInstance().parse_message_new(&request, data_ptr, data_len))
		{
			web_client::get_instance().gm_order_end(e_http_error_code_proto_parse_failed, "msg is error", temp_callback_info);
			return;
		}
		CONSOLE_INFO("gm_json_string:{} server_id:{}", request.gm_string(), request.server_id());


		temp_callback_info.listen_port = request.listen_port();
		temp_callback_info.handle_index = request.handle_index();

		if (request.server_id() != SERVERCONFIG->game_id)
		{
			web_client::get_instance().gm_order_end(e_http_error_code_wrong_server_id, "server_id is error", temp_callback_info);
			return;
		}
		Json::Value json_value;
		Json::Reader json_reader;
		Json::FastWriter json_writer;
		if (json_reader.parse(request.gm_string(), json_value) == false || json_value.isObject() == false)
		{
			web_client::get_instance().gm_order_end(e_http_error_code_json_parse_failed, "json is error", temp_callback_info);
			return;
		}
		if (json_value["gmCode"].isNull()
			|| !json_value["gmCode"].isInt()
			|| json_value["data"].isNull()
			|| !json_value["data"].isObject())
		{
			web_client::get_instance().gm_order_end(e_http_error_code_json_parse_failed, "data is error", temp_callback_info);
			return;
		}

		web_client::get_instance().do_gm_order(json_value["gmCode"].asInt(), json_value["data"], temp_callback_info, false);
	}

	void world_server::set_cross_server_open_time(int32 time_stamp, bool is_from_other_server)
	{
		if (is_sky_island_server() && is_from_other_server)
		{
			if (m_server_info_arr[e_server_info_type_begin_cross_server_time] < time_stamp && m_server_info_arr[e_server_info_type_begin_cross_server_time] != 0)
			{
				ws2ws_cross_server_time msg;
				msg.cross_server_time = m_server_info_arr[e_server_info_type_begin_cross_server_time];
				cross::send_msg_to_ws(guid_64(), 0, e_msgindex_ws2ws_cross_server_time, &msg, sizeof(msg));
			}
			else
			{
				m_server_info_arr[e_server_info_type_begin_cross_server_time] = time_stamp;
			}
			set_cross_server_open_day();
			open_act_with_cross_server_open_day();
			return;
		}
		if (time_stamp <= 0)
		{
			return;
		}
		if (m_server_info_arr[e_server_info_type_begin_cross_server_time] == 0 || time_stamp < m_server_info_arr[e_server_info_type_begin_cross_server_time])
		{
			m_server_info_arr[e_server_info_type_begin_cross_server_time] = time_stamp;

			//ws2dp_set_server_info req;
			//req.server_info_value = time_stamp;
			//req.server_info_type = e_server_info_type_begin_cross_server_time;
			//ws_client::getInstance().send_to_dp(&req, sizeof(req));
			script_mgr::get_instance().call_func("ws2dp_send", "send_gm_set_server_info_lua", 0, false, "%d%d%d%l", time_stamp, (int32)e_server_info_type_begin_cross_server_time, 0, 0);


			set_cross_server_open_day();
			open_act_with_cross_server_open_day();
			send_server_info_arr_to_cs(e_server_info_type_begin_cross_server_time);
		}
	}

	void world_server::set_cross_server_open_day()
	{
		int32 time_stamp = m_server_info_arr[e_server_info_type_begin_cross_server_time];
		if (time_stamp <= 0)
		{
			return;
		}
		int32 cur_time_stamp = time_helper::get_cur_time_new().second;
		tm* local_time = nullptr;
		if (cur_time_stamp < 0)
		{
			local_time = time_helper::get_localtime();
		}
		else
		{
			local_time = time_helper::get_tm_by_time(cur_time_stamp);
			if (nullptr != local_time)
			{
				local_time->tm_year -= 1900;
			}
		}
		if (nullptr == local_time)
		{
			return;
		}
		int32 local_year = local_time->tm_year;
		int32 local_yday = local_time->tm_yday;

		tm* server_on_time = nullptr;
		server_on_time = time_helper::get_tm_by_time(time_stamp);
		if (nullptr == server_on_time)
		{
			return;
		}
		server_on_time->tm_year -= 1900;
		int32 server_on_year = server_on_time->tm_year;
		int32 server_on_yday = server_on_time->tm_yday;



		int32 days = 0;
		if (server_on_year == local_year)
		{
			days = local_yday - server_on_yday;
		}
		else if (server_on_year > local_year)
		{
			return;
		}
		else
		{
			days += time_helper::get_days_of_year(server_on_year) - server_on_yday; //开服年剩余天数
			days += local_yday;	//当前年已过天数
			for (int32 year = server_on_year + 1; year < local_year; year++)
			{
				days += time_helper::get_days_of_year(year);	// 中间年的天数
			}
		}
		if (days < 0)
		{
			days = 0;
		}
		m_open_cross_server_days = days + 1;
	}

	void world_server::open_act_with_cross_server_open_day()
	{
		if (m_open_cross_server_days <= 0)
		{
			return;
		}
		if (get_need_begin_cross_gm_common(e_need_server_cross_begin_cross) <= 0)
		{
			//没开跨服则不做自动化处理
			return;
		}

		std::vector<int32> act_day_arr = GAMECONFIG->CrossActTimeDay;
		if (act_day_arr.size() <= 0 || act_day_arr.size() % 2 != 0)
		{
			return;
		}
		for (int32 i = 0; i < act_day_arr.size() / 2; i++)
		{
			if (m_open_cross_server_days >= act_day_arr[i * 2 + 1] && act_day_arr[i * 2 + 1] != 0)
			{
				set_need_begin_cross_gm_common(true, (e_need_server_cross)act_day_arr[i * 2]);
				send_begin_cross_gm_to_client((e_need_server_cross)act_day_arr[i * 2]);
				send_begin_cross_gm_to_cs((e_need_server_cross)act_day_arr[i * 2]);
				save_gm_common_to_db();
			}
		}
	}

	void world_server::save_gm_common_to_db(int64 req_http_id)
	{
		for (int32 i = e_need_server_cross_none; i < e_need_server_cross_max; i++)
		{
			//ws2dp_gm_need_begin_cross req;
			//req.need_begin_cross = m_need_begin_cross_config[i];
			//req.gm_type = i;
			//req.req_http_id = req_http_id;
			//ws_client::getInstance().send_to_dp(&req, sizeof(req));

			script_mgr::get_instance().call_func("ws2dp_send", "send_gm_set_begin_cross_lua", 0, false, "%d%d%l", m_need_begin_cross_config[i], i, req_http_id);

		}
	}

	void world_server::set_ws_loading_flag(int64 flag)
	{
		int64 old_loading_flag = m_ws_loading_flag;
		m_ws_loading_flag &= ~flag;

		time_limit_activity_temp_ws_mgr::get_instance().load_new_temp_on_server_open();
		if (0 != m_ws_loading_flag)
		{
			return;
		}
		if (old_loading_flag != 0)//只执行一次
		{
			send_to_gate_with_flag_load_end();
			send_msg_to_cs_with_load_end();
		}
	}

	void world_server::send_msg_to_cs_with_load_end()
	{
		ranking_mgr_ws::send_world_srv_lv_info_all();
		send_server_config_to_all_cs();
		send_need_begin_cross();
		cross_server_harry_ws_mgr::get_instance().send_server_harry_msg_to_cs();
		send_all_server_info_to_cs();
		legion_ws_mgr::get_instance().create_all_legion_station_map();
		set_ladder_world_level(m_server_ladder_wrold_level, true, false);
	}

	bool world_server::is_loading_flag_finish(int64 target_loading_flag)
	{
		return !(m_ws_loading_flag & target_loading_flag);
	}

	void world_server::gate_server_func_when_game_info_load_end()
	{
		if (!is_sky_island_server() || !is_loading_flag_finish(e_ws_flag_gm_common) || !gate_proxy::getInstance().is_gate_run())
		{
			return;
		}
		legion_ws_city_war& city_war_mgr = legion_ws_mgr::get_instance().get_legion_city_war();
		city_war_mgr.set_cross_server_territory_war_state(world_server::getInstance().get_need_begin_cross_gm_common(e_need_server_cross_begin_cross_legion_territory_war));
		send_server_config_to_db();
		load_server_attr_val();
		send_need_server_cross_to_other_server();
		refresh_server_cross_time();
		world_server::getInstance().send_server_info_by_type_to_other_server(e_server_info_type_server_cross_state);
	}

	void world_server::set_last_server_list_from_db(const s_last_server_list_db & last_server_list)
	{
		m_last_server_list_server_id.clear();
		init_unit::parse_char_to_vector(m_last_server_list_server_id, last_server_list.last_server_list);
	}

	void world_server::refresh_last_server_list_with_now()
	{
		m_last_server_list_server_id.clear();
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			m_last_server_list_server_id.push_back(it->second.server_id);
		}
	}

	bool world_server::is_add_new_server_id()
	{
		for (game_info_map_it it = m_cross_info_map.begin(); it != m_cross_info_map.end(); ++it)
		{
			int32 server_id = it->second.server_id;
			bool is_have_server_id = false;
			for (int32 i = 0; i < m_last_server_list_server_id.size(); i++)
			{
				if (server_id == m_last_server_list_server_id[i])
				{
					is_have_server_id = true;
					break;
				}
			}
			if (!is_have_server_id)
			{
				return true;
			}
		}
		return false;
	}

	void world_server::save_last_sever_list_to_db()
	{
		xstring mid_str = "";
		//ws2dp_save_last_server_list msg;
		init_unit::parse_vector_to_char(mid_str, m_last_server_list_server_id);
		//memset(msg.last_server_list, 0, sizeof(msg.last_server_list));
		//memcpy(msg.last_server_list, mid_str.c_str(), mid_str.size() > sizeof(msg.last_server_list) ? sizeof(msg.last_server_list) : mid_str.size());
		//msg.server_id = get_server_id();
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));

		script_mgr::get_instance().call_func("ws2dp_send", "send_save_server_last_server_list_lua", 0, false, "%d%s", get_server_id(), mid_str.c_str());

	}

	void world_server::send_to_gate_with_flag_load_end()
	{
		if (m_ws_loading_flag != 0)
		{
			return;
		}
		ws2gate_flag_load_end msg;
		msg.cur_server_id = get_server_id();
		ws_client::getInstance().only_send_to_gate(&msg, sizeof(msg));
	}

	void world_server::load_last_server_list_from_db()
	{
		//ws2dp_load_last_server_list msg;
		//msg.server_id = get_server_id();
		//ws_client::getInstance().send_to_dp(&msg, sizeof(msg));
		script_mgr::get_instance().call_func("ws2dp_send", "send_load_server_last_server_list_lua", 0, false, "%d", get_server_id());
	}

	void world_server::send_need_server_cross_to_other_server(int32 server_id)
	{
		if (!is_sky_island_server() || !is_loading_flag_finish(e_ws_flag_gm_common))
		{
			return;
		}
		int32 data_num = 0;
		ws2ws_send_server_info msg;
		for (int32 i = e_need_server_cross_none; i < e_need_server_cross_max; i++)
		{
			msg.gm_commond_arr[i] = m_need_begin_cross_config[i];
			data_num++;
		}
		msg.data_num = data_num;
		cross::send_msg_to_ws(guid_64(), server_id, e_msgindex_ws2ws_server_info_msg, &msg, sizeof(msg));
		
	}

	void world_server::send_to_gate_get_msg(int64 flag)
	{
		if (!gate_proxy::getInstance().is_gate_run())
		{
			m_need_send_gate_flag_arr.insert(flag);
			return;
		}
		ws2ws_get_big_player_from_gate msg;
		msg.server_id = world_server::getInstance().get_server_id();
		msg.data_flag = flag;
		cross::send_msg_to_ws(guid_64(), world_server::getInstance().get_cross_id(), e_msgindex_ws2ws_get_gate_msg, &msg, sizeof(msg));
	}

	void world_server::send_to_gate_get_all_msg_with_change_gate_server()
	{
		send_to_gate_get_msg(e_ws_flag_big_player);
		send_to_gate_get_msg(e_ws_flag_harry);
		send_to_gate_get_msg(e_ws_flag_cross_pk);
		send_to_gate_get_msg(e_ws_flag_overload_war);
		send_to_gate_get_msg(e_ws_flag_city_war);
		send_to_gate_get_msg(e_ws_flag_cross_boss);
		send_to_gate_get_msg(e_ws_flag_cross_server_state);
	}

	void world_server::refresh_server_cross_time()
	{
		//没开跨服或者是gate服直接设置状态
		if (is_sky_island_server())
		{
			if (m_server_info_arr[e_server_info_type_begin_first_cross_server_time] <= 0)
			{
				m_server_info_arr[e_server_info_type_begin_first_cross_server_time] = time_helper::get_cur_time_new().second;
				save_server_info_to_db(e_server_info_type_begin_first_cross_server_time);
				send_server_info_arr_to_cs(e_server_info_type_begin_first_cross_server_time);
			}
			set_ws_loading_flag(e_ws_flag_server_cross_time);
		}
		else if (get_cross_id() <= 0)
		{
			set_ws_loading_flag(e_ws_flag_server_cross_time);
		}
		else
		{
			if (gate_proxy::getInstance().is_gate_run() && !m_is_set_cross_time)
			{//gate所在的服务器不需要自己设置
				m_is_set_cross_time = true;
				if (m_server_info_arr[e_server_info_type_begin_cross_server_time] <= 0)
				{
					set_cross_server_open_time(time_helper::get_cur_time_new().second, false);
				}
				ws2ws_cross_server_time msg;
				msg.cross_server_time = m_server_info_arr[e_server_info_type_begin_cross_server_time];
				cross::send_msg_to_ws(guid_64(), get_cross_id(), e_msgindex_ws2ws_cross_server_time, &msg, sizeof(msg));

				if (m_server_info_arr[e_server_info_type_begin_first_cross_server_time] <= 0)
				{
					m_server_info_arr[e_server_info_type_begin_first_cross_server_time] = time_helper::get_cur_time_new().second;
					save_server_info_to_db(e_server_info_type_begin_first_cross_server_time);
					send_server_info_arr_to_cs(e_server_info_type_begin_first_cross_server_time);
				}
				set_ws_loading_flag(e_ws_flag_server_cross_time);
			}
		}
	}

	void world_server::send_server_info_by_type_to_other_server(e_server_info_type server_info_type, int32 server_id /*= 0*/)
	{
		if (server_info_type < 0 || server_info_type >= e_server_info_type_max)
		{
			return;
		}
		gate2ws_server_info msg;
		msg.server_info_type = server_info_type;
		msg.server_info_value = get_server_info_arr(server_info_type);
		cross::send_msg_to_ws(guid_64(), server_id, e_msg_gate2ws_server_info, &msg, sizeof(msg));
	}

	bool world_server::is_need_refresh_ws_daily_info_at_zero_hour()
	{
		int64 last_refresh = m_server_time_zero;
		if (time_helper::is_diff_day(last_refresh, time_helper::get_cur_time_new().second + day_time_second))
		{
			return true;
		}
		return false;
	}

	bool world_server::is_need_refresh_ws_daily_info_at_week_time()
	{
		int64 cur_time = time_helper::get_cur_time_new().second;
		if (cur_time >= m_server_time_week_constribution_refresh || m_server_time_week_constribution_refresh == 0)
		{
			return true;
		}
		return false;
	}

	bool world_server::is_need_refresh_ws_daily_info_at_week_twenty_three_time()
	{
		if (m_server_time_week_legion_welfare_refresh == 0)
		{
			m_server_time_week_legion_welfare_refresh = get_next_week_twenty_three_refresh_time();
		}
		int64 cur_time = time_helper::get_cur_time_new().second;
		if (cur_time >= m_server_time_week_legion_welfare_refresh)
		{
			return true;
		}
		return false;
	}

	bool world_server::is_need_refresh_ws_daily_info_at_clear_city_bid_time()
	{
		int64 cur_time = time_helper::get_cur_time_new().second;
		if (cur_time >= m_server_time_clear_city_war_bid || m_server_time_clear_city_war_bid == 0)
		{
			return true;
		}
		return false;
	}

	int64 world_server::get_next_clear_city_bid_time()
	{
		int32 weekday = time_helper::get_cur_time_new().day_in_week;
		int64 cur_day_zero_time = time_helper::get_today_zero_time_info().second;
		int64 cur_time = time_helper::get_cur_time_new().second;
		int64 time_week_refresh = 0;
		ActivityCommonConfigTemplate* act_com_cfg = GET_TEMPLATE(ActivityCommonConfigTemplate, first_activity_common_template_id + e_activity_type_city_war);
		if (nullptr == act_com_cfg)
		{
			return 0;
		}
		int32 next_mid_day = 0;
		std::vector<int32> open_day_arr = get_activity_open_data(e_activity_type_city_war);
		if (open_day_arr.size() > 0)
		{
			int32 next_day = open_day_arr[0];
			for (int32 i = 0; i < open_day_arr.size(); i++)
			{
				if (weekday < open_day_arr[i] && m_server_time_clear_city_war_bid > 0)
				{
					next_day = open_day_arr[i];
					break;
				}
				else if (weekday <= open_day_arr[i] && m_server_time_clear_city_war_bid == 0)
				{
					next_day = open_day_arr[i];
					break;
				}
			}
			if (next_day > weekday)
			{
				next_mid_day = next_day - weekday;
			}
			else if (next_day == weekday)
			{
				if (m_server_time_clear_city_war_bid > 0)
				{
					next_mid_day = 7 - weekday + next_day;
				}
				else
				{
					next_mid_day = 0;
				}
			}
			else
			{
				next_mid_day = 7 - weekday + next_day;
			}
		}
		else
		{
			if (m_server_time_clear_city_war_bid > 0)
			{
				next_mid_day = 1;
			}
			else
			{
				next_mid_day = 0;
			}
		}
		time_week_refresh = cur_day_zero_time + legion_city_war_bid_time_begin * hour_tick_time / second_tick_time + next_mid_day * day_time_second;
		return time_week_refresh;
	}

	int64 world_server::get_next_week_refresh_time()
	{
		int32 weekday = time_helper::get_cur_time_new().day_in_week;
		int64 cur_day_zero_time = time_helper::get_today_zero_time_info().second;

		int32 mid_day = 7 - weekday;
		int64 time_week_refresh = cur_day_zero_time + mid_day * day_time_second;
		return time_week_refresh;
	}
	int64 world_server::get_next_week_twenty_three_refresh_time()
	{
		int32 weekday = time_helper::get_cur_time_new().day_in_week;
		int64 cur_day_zero_time = time_helper::get_today_zero_time_info().second;

		int32 mid_day = 7 - weekday;
		//获取当前周日的23:00
		int64 time_week_refresh = cur_day_zero_time + mid_day * day_time_second - hour_tick_time / second_tick_time;
		//可能获取时间是周日23:00之后改为获取下周的时间
		if (time_helper::get_cur_time_new().second > time_week_refresh)
		{
			time_week_refresh = time_week_refresh + all_day_in_week * day_time_second;
		}
		return time_week_refresh;
	}
}