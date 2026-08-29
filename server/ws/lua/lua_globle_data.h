/********************************************************************
  created: 2017-7-17 17:36:24
  file base: lua_globle_data
  file ext: h
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_LUA_GLOBLE_DATA_H_
#define _FAITH_LUA_GLOBLE_DATA_H_
#include "logic/type_def.hpp"
#include "server/client_session_mgr.hpp"
#include "world_server.hpp"
#include "msgproc/msgproc_client.hpp"
#include "server/legion/city_war_territory_mgr.h"
#include "server/legion/legion_ws_answer.h"
#include "invalid_word/invalid_ansi_word.h"
#include "server/team/team_ws_mgr.h"
#include "cross_group/gate_proxy.hpp"
#include "server/activity/pk_king_mgr.h"
#include "server/activity/cross_server_pk_ws_mgr.h"
#include "server/activity/cross_server_harry_ws_mgr.h"
#include "server/activity/cross_server_world_boss_ws_mgr.h"
#include "server/activity/crystal_fairyland_mgr.h"
#include "server/activity/broken_sky_mgr.h"
#include "server/world_boss/world_boss_ws_mgr.h"
#include "server/web_client.hpp"
#include "server/ranking/ranking_mgr_ws.h"
#include "server/activity/cloud_shop_mgr.h"
#include "server/activity/gain_treasure_ws_mgr.h"
#include "server/rename/rename_mgr.h"
#include "server/activity/time_limit_activity_ws_mgr.h"
#include "server/activity/cross_server_world_boss_ws_mgr.h"
#include "server/assist_fight/assist_fight_mgr.h"
#include "server/appearance_info_mgr.h"
#include "server/activity/element_war_ws_mgr.h"
#include "server/mail/event_ws_mgr.h"
#include "utility/random.h"
#include "server/big_player_ws_mgr.h"
#include "msgproc/msgproc_cs.hpp"
#include "server/auction/auction_mgr_ws.h"


namespace faith
{
	class client_session;
	class world_server;
	class gate_proxy;

	guid_64* get_guid_64(int64 value);
	guid_64* get_guid_64(int32 a, int32 b);
	guid_64* get_guid_64();

	////////////////////////////////////////////////////////////////////////

	client_session*	get_client_session_by_index(int64 unit_index);
	client_session* get_client_session_by_guid(guid_64 guid);
	
	world_server* get_world_server();

	gate_proxy* get_gate_proxy();

	int64  make_guid_lua(int64 in_guid);

	client_session_mgr* get_client_session_mgr();

	////////////////////////////////////////////////////////////////////////////

	std::string get_str_by_string_template_id(int32 template_id, int32 index);

	s_item_info* get_item_info_by_str(const char *msg, int32 msg_len);
	s_auction_info* get_auction_info_by_str(const char* msg, int32 msg_len);
	char*        get_auction_buffer(s_auction_info *auction_ptr);
	s_unit_info*    get_unit_info_by_str(const char* msg, int32 msg_len);
	login_fixed_data *get_login_fixed_data_by_str(const char *msg, int32 msg_len);


	s_log_common_head* get_log_common_head();

	void lua_set_log_common_head_part1(s_log_common_head& head, xstring game_id, xstring channel_id, xstring media_id, xstring device_id);

	void lua_set_log_common_head_part2(s_log_common_head& head, xstring version_name, int32  version_code);
	
	void lua_set_log_common_head_part3(s_log_common_head& head, xstring user_id, int32  server_id, int64 log_time);
	
	void lua_set_log_common_head_part4(s_log_common_head& head, int64 role_id, xstring role_name, int32 role_level, int32 role_gender);
	
	void lua_set_log_common_head_part5(s_log_common_head& head, int64 battle_points, int32 role_type, int32 vip_level);

	xstring get_app_key();

	s_mail_info* get_mail_info_by_str(const char *msg, int32 msg_len);
	s_fake_player_info* get_fake_player_info_by_str(const char *msg, int32 msg_len);
	ws2ws_send_big_player_msg* get_send_big_player_msg(const char *msg, int32 msg_len, int32& out_len);






}

#endif