/********************************************************************
	created:	2014/07/31
	created:	31:7:2014   15:20
	file base:	msg_dispatch
	file ext:	cpp
	author:		zhy
	
	purpose:	WS_SLIM_BUILD: login / gateway / session / map / DP login only
*********************************************************************/
#include "msg_dispatch.hpp"
#include "msgproc/msgproc_cs.hpp"
#include "msgproc/msgproc_dp.hpp"
#include "msgproc/msgproc_gateway.hpp"
#include "cross_group/msgproc_cross.hpp"
#include "net/message_manager.hpp"
#include "world_server.hpp"
#include "ws_client.hpp"
#include "net.pb.h"
#include "game.pb.h"
#include <core.hpp>
#include <daemon.hpp>
#include <gate_msg.hpp>
#include <internal.hpp>
#include <login_msg.hpp>
#include <char_msg.hpp>
#include <world_server_msg.hpp>


namespace faith
{
	msg_dispatch::msg_dispatch() 
	{
	}

	msg_dispatch::~msg_dispatch() 
	{
	}

	bool msg_dispatch::res_load()
	{
		return true;
	}

	bool msg_dispatch::res_unload()
	{
		return true;
	}

	bool msg_dispatch::register_lua_handler()
	{
		return true;
	}

	bool msg_dispatch::unregister_lua_handler()
	{
		return true;
	}

bool msg_dispatch::init()
{
	res_load();
	register_lua_handler();

	// internal
	message_manager::getInstance().register_handler(e_msg_index_req_login, boost::bind(&world_server::internal_req_login, &world_server::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_index_appserver_dataupdate, boost::bind(&world_server::internal_appserver_dataupdate, &world_server::getInstance(), _1, _2, _3));
	message_manager::getInstance().register_handler(e_msg_server2gate_send_to_server, boost::bind(&ws_client::internal_gate_msg, &ws_client::getInstance(), _1, _2, _3));

	// gate2ws
	message_manager::getInstance().register_handler(e_msg_server2gate_register, gate2ws_rep_register);
	message_manager::getInstance().register_handler(e_msg_gate2server_off_line, gate2ws_rep_off_line);
	message_manager::getInstance().register_handler(e_msg_gate2gate_new, gate2ws_rep_gate_new);
	message_manager::getInstance().register_handler(e_msg_gate2ws_rep_check_from_ws, gate2ws_rep_from_ws);
	message_manager::getInstance().register_handler(e_msg_gate2ws_change_cross_id, gate2ws_change_server_id_func);
	message_manager::getInstance().register_handler(e_msg_gate2ws_all_server_arr, gate2ws_this_cross_all_server_info);
	message_manager::getInstance().register_handler(e_msg_gate2ws_end_refresh_list, gate2ws_end_refresh_server_list_func);
	message_manager::getInstance().register_handler(e_msg_gate2ws_server_hide_act, gate2ws_set_server_act_hide_func);
	message_manager::getInstance().register_handler(e_msg_gate2ws_reload_csv, gate2ws_reload_csv);

	// cs2ws session / map
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_reconnect_game, cs2ws_rep_reconnect_game);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_enter_game, cs2ws_rep_enter_game);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_client_logout, cs2ws_rep_logout);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_enter_scene, cs2ws_rep_enter_scene);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_transfer_player, cs2ws_req_transfer_player);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_role_info, cs2ws_rep_role_info);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_map_state, cs2ws_rep_map_state);
	message_manager::getInstance().register_handler(e_msg_index_cs2ws_get_map_info_from_cs_end, cs2ws_rep_get_map_info_from_cs_end);
	message_manager::getInstance().register_handler(e_msg_index_cs_reload_csv_end, cs2ws_reload_csv_end);

	// gateway2ws login / session
	message_manager::getInstance().register_handler(e_msgindex_c2s_client_login, gateway2ls_request_client_login);
	message_manager::getInstance().register_handler(e_msgindex_c2s_enum_char, gateway2ls_enum_character_func);
	message_manager::getInstance().register_handler(e_msgindex_gateway2ls_create_character, gateway2ls_create_character_func);
	message_manager::getInstance().register_handler(e_msgindex_c2s_del_char, gateway2ls_delete_character_func);
	message_manager::getInstance().register_handler(e_msg_index_gateway2ws_client_reconnect, gateway2ws_client_reconnect_func);
	message_manager::getInstance().register_handler(e_msg_index_gateway2ws_client_logined, gateway2ws_client_loginned);
	message_manager::getInstance().register_handler(e_msg_index_gateway2ws_leave_queued, gateway2ws_client_leave_queued);
	message_manager::getInstance().register_handler(e_msg_index_gateway2ws_client_logout, gateway2ws_req_logout_client);

	// client enter game / scene
	message_manager::getInstance().register_handler(e_msgindex_c2s_enter_game, c2ws_req_enter_game);
	message_manager::getInstance().register_handler(e_msgindex_c2s_enter_scene, c2ws_enter_scene);

	// dp2ws login
	message_manager::getInstance().register_handler(e_msg_index_dp2ls_client_login, dp2ls_reply_client_login);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_enum_character, dp2ls_reply_character_list);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_create_character, dp2ls_reply_create_character);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_delete_character, dp2ls_reply_delete_character);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_item, dp2ls_load_enum_item_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_buff, dp2ls_load_enum_buff_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_spirit, dp2ls_load_enum_spirit_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ls_load_enum_time, dp2ls_load_enum_time_end);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_get_game_info, dp2ws_rep_get_game_info_ws);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_character, dp2ws_rep_get_role_info_ws);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_server_list_end, dp2ws_rep_load_last_server_list);
	message_manager::getInstance().register_handler(e_msgindex_dp2ws_load_server_attr_val_end, dp2ws_load_server_attr_val_proc);

	return true;
}
}
