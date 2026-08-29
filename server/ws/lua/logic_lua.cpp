/*
** Lua binding: logic
** Generated automatically by tolua++-1.0.93-lua53 on Wed Aug 21 16:21:07 2024.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_logic_open (lua_State* tolua_S);

#include "lua_call_handle.h"
#include "logic/game_enum_def_s.h"
#include "logic/server_log_def.hpp"
#include "server_log.hpp"
#include "utility/init_unit.h"
#include "utility/globle_data.h"
#include "ws_client.hpp"
#include "msgproc/msgproc_dp.hpp"
#include "server/arena/arena_mgr_ws.h"
#include "server/relation/marry_mgr_ws.hpp"
#include "server/legion/legion_ws_mgr.h"
#include "server/mail/mail_event_ws.h"
#include "server/activity/cross_server_city_war_ws_mgr.h"
#include "lua_globle_data.h"
#include "system/scene/cs_map_system.h"
#include "system/scene/cs_map_mgr_system.h"
using namespace faith;

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_hld__s_team_member_info (lua_State* tolua_S)
{
 faith::s_team_member_info* self = (faith::s_team_member_info*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__s_map_pos (lua_State* tolua_S)
{
 faith::s_map_pos* self = (faith::s_map_pos*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__s_client_uid (lua_State* tolua_S)
{
 faith::s_client_uid* self = (faith::s_client_uid*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__frotator (lua_State* tolua_S)
{
 faith::frotator* self = (faith::frotator*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__s_legion_member_info (lua_State* tolua_S)
{
 faith::s_legion_member_info* self = (faith::s_legion_member_info*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__fvector (lua_State* tolua_S)
{
 faith::fvector* self = (faith::fvector*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__guid_64 (lua_State* tolua_S)
{
 faith::guid_64* self = (faith::guid_64*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_e_error_code (lua_State* tolua_S)
{
 e_error_code* self = (e_error_code*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__c_mail_info (lua_State* tolua_S)
{
 faith::c_mail_info* self = (faith::c_mail_info*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
#ifndef Mtolua_typeid
#define Mtolua_typeid(L,TI,T)
#endif
 tolua_usertype(tolua_S,"faith::team_ws");
 Mtolua_typeid(tolua_S,typeid(faith::team_ws), "faith::team_ws");
 tolua_usertype(tolua_S,"login_fixed_data");
 Mtolua_typeid(tolua_S,typeid(login_fixed_data), "login_fixed_data");
 tolua_usertype(tolua_S,"faith::invalid_ansi_word");
 Mtolua_typeid(tolua_S,typeid(faith::invalid_ansi_word), "faith::invalid_ansi_word");
 tolua_usertype(tolua_S,"faith::cloud_shop_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::cloud_shop_mgr), "faith::cloud_shop_mgr");
 tolua_usertype(tolua_S,"faith::legion_ws_applicant_list");
 Mtolua_typeid(tolua_S,typeid(faith::legion_ws_applicant_list), "faith::legion_ws_applicant_list");
 tolua_usertype(tolua_S,"faith::fvector");
 Mtolua_typeid(tolua_S,typeid(faith::fvector), "faith::fvector");
 tolua_usertype(tolua_S,"e_client_req_type");
 Mtolua_typeid(tolua_S,typeid(e_client_req_type), "e_client_req_type");
 tolua_usertype(tolua_S,"faith::client_session");
 Mtolua_typeid(tolua_S,typeid(faith::client_session), "faith::client_session");
 tolua_usertype(tolua_S,"faith::mail_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::mail_ws_mgr), "faith::mail_ws_mgr");
 tolua_usertype(tolua_S,"faith::ranking_mgr_ws");
 Mtolua_typeid(tolua_S,typeid(faith::ranking_mgr_ws), "faith::ranking_mgr_ws");
 tolua_usertype(tolua_S,"faith::appearance_info_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::appearance_info_mgr), "faith::appearance_info_mgr");
 tolua_usertype(tolua_S,"faith::rename_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::rename_mgr), "faith::rename_mgr");
 tolua_usertype(tolua_S,"faith::s_client_uid");
 Mtolua_typeid(tolua_S,typeid(faith::s_client_uid), "faith::s_client_uid");
 tolua_usertype(tolua_S,"faith::globle_data");
 Mtolua_typeid(tolua_S,typeid(faith::globle_data), "faith::globle_data");
 tolua_usertype(tolua_S,"faith::crystal_fairyland_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::crystal_fairyland_mgr), "faith::crystal_fairyland_mgr");
 tolua_usertype(tolua_S,"e_error_code");
 Mtolua_typeid(tolua_S,typeid(e_error_code), "e_error_code");
 tolua_usertype(tolua_S,"legion_ws_applicant");
 Mtolua_typeid(tolua_S,typeid(legion_ws_applicant), "legion_ws_applicant");
 tolua_usertype(tolua_S,"faith::big_player_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::big_player_ws_mgr), "faith::big_player_ws_mgr");
 tolua_usertype(tolua_S,"faith::arena_mgr_ws");
 Mtolua_typeid(tolua_S,typeid(faith::arena_mgr_ws), "faith::arena_mgr_ws");
 tolua_usertype(tolua_S,"faith::element_war_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::element_war_ws_mgr), "faith::element_war_ws_mgr");
 tolua_usertype(tolua_S,"faith::gain_treasure_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::gain_treasure_ws_mgr), "faith::gain_treasure_ws_mgr");
 tolua_usertype(tolua_S,"faith::random_gen");
 Mtolua_typeid(tolua_S,typeid(faith::random_gen), "faith::random_gen");
 tolua_usertype(tolua_S,"faith::event_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::event_ws_mgr), "faith::event_ws_mgr");
 tolua_usertype(tolua_S,"s_fake_player_info");
 Mtolua_typeid(tolua_S,typeid(s_fake_player_info), "s_fake_player_info");
 tolua_usertype(tolua_S,"faith::cross_server_city_war_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::cross_server_city_war_ws_mgr), "faith::cross_server_city_war_ws_mgr");
 tolua_usertype(tolua_S,"s_log_common_head");
 Mtolua_typeid(tolua_S,typeid(s_log_common_head), "s_log_common_head");
 tolua_usertype(tolua_S,"faith::web_client");
 Mtolua_typeid(tolua_S,typeid(faith::web_client), "faith::web_client");
 tolua_usertype(tolua_S,"faith::city_war_territory_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::city_war_territory_mgr), "faith::city_war_territory_mgr");
 tolua_usertype(tolua_S,"faith::relation_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::relation_mgr), "faith::relation_mgr");
 tolua_usertype(tolua_S,"s_unit_info");
 Mtolua_typeid(tolua_S,typeid(s_unit_info), "s_unit_info");
 tolua_usertype(tolua_S,"faith::world_server");
 Mtolua_typeid(tolua_S,typeid(faith::world_server), "faith::world_server");
 tolua_usertype(tolua_S,"faith::marry_mgr_ws");
 Mtolua_typeid(tolua_S,typeid(faith::marry_mgr_ws), "faith::marry_mgr_ws");
 tolua_usertype(tolua_S,"faith::world_boss_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::world_boss_ws_mgr), "faith::world_boss_ws_mgr");
 tolua_usertype(tolua_S,"faith::pk_king_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::pk_king_mgr), "faith::pk_king_mgr");
 tolua_usertype(tolua_S,"faith::client_session_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::client_session_mgr), "faith::client_session_mgr");
 tolua_usertype(tolua_S,"faith::cross_server_harry_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::cross_server_harry_ws_mgr), "faith::cross_server_harry_ws_mgr");
 tolua_usertype(tolua_S,"faith::s_team_member_info");
 Mtolua_typeid(tolua_S,typeid(faith::s_team_member_info), "faith::s_team_member_info");
 tolua_usertype(tolua_S,"faith::team_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::team_ws_mgr), "faith::team_ws_mgr");
 tolua_usertype(tolua_S,"faith::ws_client");
 Mtolua_typeid(tolua_S,typeid(faith::ws_client), "faith::ws_client");
 tolua_usertype(tolua_S,"faith::init_unit");
 Mtolua_typeid(tolua_S,typeid(faith::init_unit), "faith::init_unit");
 tolua_usertype(tolua_S,"faith::guid_64");
 Mtolua_typeid(tolua_S,typeid(faith::guid_64), "faith::guid_64");
 tolua_usertype(tolua_S,"faith::legion_ws_city_war");
 Mtolua_typeid(tolua_S,typeid(faith::legion_ws_city_war), "faith::legion_ws_city_war");
 tolua_usertype(tolua_S,"faith::server_log");
 Mtolua_typeid(tolua_S,typeid(faith::server_log), "faith::server_log");
 tolua_usertype(tolua_S,"ws2ws_send_big_player_msg");
 Mtolua_typeid(tolua_S,typeid(ws2ws_send_big_player_msg), "ws2ws_send_big_player_msg");
 tolua_usertype(tolua_S,"uint64");
 Mtolua_typeid(tolua_S,typeid(uint64), "uint64");
 tolua_usertype(tolua_S,"faith::cross_server_pk_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::cross_server_pk_ws_mgr), "faith::cross_server_pk_ws_mgr");
 tolua_usertype(tolua_S,"faith::c_element_war_role_info");
 Mtolua_typeid(tolua_S,typeid(faith::c_element_war_role_info), "faith::c_element_war_role_info");
 tolua_usertype(tolua_S,"std::vector<faith::s_item_info>");
 Mtolua_typeid(tolua_S,typeid(std::vector<faith::s_item_info>), "std::vector<faith::s_item_info>");
 tolua_usertype(tolua_S,"faith::legion_ws_warehouse");
 Mtolua_typeid(tolua_S,typeid(faith::legion_ws_warehouse), "faith::legion_ws_warehouse");
 tolua_usertype(tolua_S,"faith::s_map_pos");
 Mtolua_typeid(tolua_S,typeid(faith::s_map_pos), "faith::s_map_pos");
 tolua_usertype(tolua_S,"faith::s_item_info");
 Mtolua_typeid(tolua_S,typeid(faith::s_item_info), "faith::s_item_info");
 tolua_usertype(tolua_S,"faith::legion_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::legion_ws_mgr), "faith::legion_ws_mgr");
 tolua_usertype(tolua_S,"s_auction_info");
 Mtolua_typeid(tolua_S,typeid(s_auction_info), "s_auction_info");
 tolua_usertype(tolua_S,"std::vector<int32>");
 Mtolua_typeid(tolua_S,typeid(std::vector<int32>), "std::vector<int32>");
 tolua_usertype(tolua_S,"faith::s_legion_member_info");
 Mtolua_typeid(tolua_S,typeid(faith::s_legion_member_info), "faith::s_legion_member_info");
 tolua_usertype(tolua_S,"faith::cs_map_mgr_system");
 Mtolua_typeid(tolua_S,typeid(faith::cs_map_mgr_system), "faith::cs_map_mgr_system");
 tolua_usertype(tolua_S,"faith::cs_map_system");
 Mtolua_typeid(tolua_S,typeid(faith::cs_map_system), "faith::cs_map_system");
 tolua_usertype(tolua_S,"faith::legion_ws_answer");
 Mtolua_typeid(tolua_S,typeid(faith::legion_ws_answer), "faith::legion_ws_answer");
 tolua_usertype(tolua_S,"std::vector<s_item_template_info>");
 Mtolua_typeid(tolua_S,typeid(std::vector<s_item_template_info>), "std::vector<s_item_template_info>");
 tolua_usertype(tolua_S,"faith::legion_ws");
 Mtolua_typeid(tolua_S,typeid(faith::legion_ws), "faith::legion_ws");
 tolua_usertype(tolua_S,"faith::frotator");
 Mtolua_typeid(tolua_S,typeid(faith::frotator), "faith::frotator");
 tolua_usertype(tolua_S,"faith::broken_sky_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::broken_sky_mgr), "faith::broken_sky_mgr");
 tolua_usertype(tolua_S,"faith::cross_server_world_boss_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::cross_server_world_boss_ws_mgr), "faith::cross_server_world_boss_ws_mgr");
 tolua_usertype(tolua_S,"s_mail_info");
 Mtolua_typeid(tolua_S,typeid(s_mail_info), "s_mail_info");
 tolua_usertype(tolua_S,"faith::assist_fight_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::assist_fight_mgr), "faith::assist_fight_mgr");
 tolua_usertype(tolua_S,"faith::time_limit_activity_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(faith::time_limit_activity_ws_mgr), "faith::time_limit_activity_ws_mgr");
 tolua_usertype(tolua_S,"faith::c_mail_info");
 Mtolua_typeid(tolua_S,typeid(faith::c_mail_info), "faith::c_mail_info");
}

/* method: new of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new00
static int tolua_logic_hld_s_client_uid_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_client_uid* tolua_ret = (faith::s_client_uid*)  Mtolua_new((faith::s_client_uid)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_client_uid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new00_local
static int tolua_logic_hld_s_client_uid_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_client_uid* tolua_ret = (faith::s_client_uid*)  Mtolua_new((faith::s_client_uid)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_client_uid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new01
static int tolua_logic_hld_s_client_uid_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_client_uid",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int64 InValue = ((int64)  tolua_tointeger(tolua_S,2,0));
  {
   faith::s_client_uid* tolua_ret = (faith::s_client_uid*)  Mtolua_new((faith::s_client_uid)(InValue));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_client_uid");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_s_client_uid_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new01_local
static int tolua_logic_hld_s_client_uid_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_client_uid",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int64 InValue = ((int64)  tolua_tointeger(tolua_S,2,0));
  {
   faith::s_client_uid* tolua_ret = (faith::s_client_uid*)  Mtolua_new((faith::s_client_uid)(InValue));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_client_uid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_s_client_uid_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_valid of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_is_valid00
static int tolua_logic_hld_s_client_uid_is_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::s_client_uid* self = (faith::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_valid'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_valid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_valid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear_data of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_clear_data00
static int tolua_logic_hld_s_client_uid_clear_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::s_client_uid* self = (faith::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear_data'", NULL);
#endif
  {
   self->clear_data();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: fep_uid_64 of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_client_uid_fep_uid_64
static int tolua_get_hld__s_client_uid_fep_uid_64(lua_State* tolua_S)
{
  faith::s_client_uid* self = (faith::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fep_uid_64'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->fep_uid_64);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: fep_uid_64 of class  faith::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_client_uid_fep_uid_64
static int tolua_set_hld__s_client_uid_fep_uid_64(lua_State* tolua_S)
{
  faith::s_client_uid* self = (faith::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fep_uid_64'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->fep_uid_64 = ((int64)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new00
static int tolua_logic_hld_guid_64_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  Mtolua_new((faith::guid_64)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new00_local
static int tolua_logic_hld_guid_64_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  Mtolua_new((faith::guid_64)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new01
static int tolua_logic_hld_guid_64_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  long long In64 = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  Mtolua_new((faith::guid_64)(In64));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new01_local
static int tolua_logic_hld_guid_64_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  long long In64 = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  Mtolua_new((faith::guid_64)(In64));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new02
static int tolua_logic_hld_guid_64_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int InA = ((int)  tolua_tointeger(tolua_S,2,0));
  int InB = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  Mtolua_new((faith::guid_64)(InA,InB));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new02_local
static int tolua_logic_hld_guid_64_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int InA = ((int)  tolua_tointeger(tolua_S,2,0));
  int InB = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  Mtolua_new((faith::guid_64)(InA,InB));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_A of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_get_A00
static int tolua_logic_hld_guid_64_get_A00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::guid_64* self = (faith::guid_64*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_A'", NULL);
#endif
  {
   int tolua_ret = (int)  self->get_A();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_A'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_B of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_get_B00
static int tolua_logic_hld_guid_64_get_B00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::guid_64* self = (faith::guid_64*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_B'", NULL);
#endif
  {
   int tolua_ret = (int)  self->get_B();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_B'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_AB of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_get_AB00
static int tolua_logic_hld_guid_64_get_AB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::guid_64* self = (faith::guid_64*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_AB'", NULL);
#endif
  {
   long long tolua_ret = (long long)  self->get_AB();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_AB'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_valid of class  faith::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_is_valid00
static int tolua_logic_hld_guid_64_is_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::guid_64* self = (faith::guid_64*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_valid'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_valid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_valid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_get_hld__fvector_x
static int tolua_get_hld__fvector_x(lua_State* tolua_S)
{
  faith::fvector* self = (faith::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_set_hld__fvector_x
static int tolua_set_hld__fvector_x(lua_State* tolua_S)
{
  faith::fvector* self = (faith::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_get_hld__fvector_y
static int tolua_get_hld__fvector_y(lua_State* tolua_S)
{
  faith::fvector* self = (faith::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_set_hld__fvector_y
static int tolua_set_hld__fvector_y(lua_State* tolua_S)
{
  faith::fvector* self = (faith::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_get_hld__fvector_z
static int tolua_get_hld__fvector_z(lua_State* tolua_S)
{
  faith::fvector* self = (faith::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_set_hld__fvector_z
static int tolua_set_hld__fvector_z(lua_State* tolua_S)
{
  faith::fvector* self = (faith::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_logic_hld_fvector_new00
static int tolua_logic_hld_fvector_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::fvector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::fvector* tolua_ret = (faith::fvector*)  Mtolua_new((faith::fvector)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::fvector");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_logic_hld_fvector_new00_local
static int tolua_logic_hld_fvector_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::fvector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::fvector* tolua_ret = (faith::fvector*)  Mtolua_new((faith::fvector)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::fvector");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_data of class  faith::fvector */
#ifndef TOLUA_DISABLE_tolua_logic_hld_fvector_set_data00
static int tolua_logic_hld_fvector_set_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::fvector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::fvector* self = (faith::fvector*)  tolua_tousertype(tolua_S,1,0);
  float temp_x = ((float)  tolua_tonumber(tolua_S,2,0));
  float temp_y = ((float)  tolua_tonumber(tolua_S,3,0));
  float temp_z = ((float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_data'", NULL);
#endif
  {
   self->set_data(temp_x,temp_y,temp_z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: roll of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_get_hld__frotator_roll
static int tolua_get_hld__frotator_roll(lua_State* tolua_S)
{
  faith::frotator* self = (faith::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'roll'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->roll);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: roll of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_set_hld__frotator_roll
static int tolua_set_hld__frotator_roll(lua_State* tolua_S)
{
  faith::frotator* self = (faith::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'roll'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->roll = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pitch of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_get_hld__frotator_pitch
static int tolua_get_hld__frotator_pitch(lua_State* tolua_S)
{
  faith::frotator* self = (faith::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pitch'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->pitch);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pitch of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_set_hld__frotator_pitch
static int tolua_set_hld__frotator_pitch(lua_State* tolua_S)
{
  faith::frotator* self = (faith::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pitch'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pitch = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: yaw of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_get_hld__frotator_yaw
static int tolua_get_hld__frotator_yaw(lua_State* tolua_S)
{
  faith::frotator* self = (faith::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yaw'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->yaw);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yaw of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_set_hld__frotator_yaw
static int tolua_set_hld__frotator_yaw(lua_State* tolua_S)
{
  faith::frotator* self = (faith::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yaw'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->yaw = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_logic_hld_frotator_new00
static int tolua_logic_hld_frotator_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::frotator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::frotator* tolua_ret = (faith::frotator*)  Mtolua_new((faith::frotator)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::frotator");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::frotator */
#ifndef TOLUA_DISABLE_tolua_logic_hld_frotator_new00_local
static int tolua_logic_hld_frotator_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::frotator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::frotator* tolua_ret = (faith::frotator*)  Mtolua_new((faith::frotator)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::frotator");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unit_location of class  faith::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_map_pos_unit_location
static int tolua_get_hld__s_map_pos_unit_location(lua_State* tolua_S)
{
  faith::s_map_pos* self = (faith::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_location'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->unit_location,"faith::fvector");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unit_location of class  faith::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_map_pos_unit_location
static int tolua_set_hld__s_map_pos_unit_location(lua_State* tolua_S)
{
  faith::s_map_pos* self = (faith::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_location'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::fvector",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->unit_location = *((faith::fvector*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unit_rotation of class  faith::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_map_pos_unit_rotation
static int tolua_get_hld__s_map_pos_unit_rotation(lua_State* tolua_S)
{
  faith::s_map_pos* self = (faith::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_rotation'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->unit_rotation,"faith::frotator");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unit_rotation of class  faith::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_map_pos_unit_rotation
static int tolua_set_hld__s_map_pos_unit_rotation(lua_State* tolua_S)
{
  faith::s_map_pos* self = (faith::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_rotation'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::frotator",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->unit_rotation = *((faith::frotator*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_map_pos_new00
static int tolua_logic_hld_s_map_pos_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_map_pos",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_map_pos* tolua_ret = (faith::s_map_pos*)  Mtolua_new((faith::s_map_pos)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_map_pos");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_map_pos_new00_local
static int tolua_logic_hld_s_map_pos_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_map_pos",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_map_pos* tolua_ret = (faith::s_map_pos*)  Mtolua_new((faith::s_map_pos)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_map_pos");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_vaild of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_is_vaild00
static int tolua_logic_hld_client_session_is_vaild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_vaild'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_vaild();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_vaild'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_array_index of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_array_index00
static int tolua_logic_hld_client_session_get_array_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_array_index'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_array_index();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_array_index'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_cs_conn_index of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_cs_conn_index00
static int tolua_logic_hld_client_session_get_cs_conn_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_cs_conn_index'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_cs_conn_index();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_cs_conn_index'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_to_client of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_send_to_client00
static int tolua_logic_hld_client_session_send_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  uint32 header = ((uint32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_to_client'", NULL);
#endif
  {
   self->send_to_client(data_ptr,data_len,header);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_to_client'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_to_cs_lua of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_send_to_cs_lua00
static int tolua_logic_hld_client_session_send_to_cs_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  uint32 header = ((uint32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_to_cs_lua'", NULL);
#endif
  {
   self->send_to_cs_lua(msg,msg_len,header);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_to_cs_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_client_uid of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_client_uid00
static int tolua_logic_hld_client_session_get_client_uid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_client_uid'", NULL);
#endif
  {
   faith::s_client_uid& tolua_ret = (faith::s_client_uid&)  self->get_client_uid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::s_client_uid");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_client_uid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_map_guid of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_map_guid00
static int tolua_logic_hld_client_session_get_map_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_map_guid'", NULL);
#endif
  {
   faith::guid_64& tolua_ret = (faith::guid_64&)  self->get_map_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::guid_64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_map_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_team_guid of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_team_guid00
static int tolua_logic_hld_client_session_get_team_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_team_guid'", NULL);
#endif
  {
   faith::guid_64& tolua_ret = (faith::guid_64&)  self->get_team_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::guid_64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_team_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion_guid of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_legion_guid00
static int tolua_logic_hld_client_session_get_legion_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_guid'", NULL);
#endif
  {
   faith::guid_64& tolua_ret = (faith::guid_64&)  self->get_legion_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::guid_64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_role_info_data of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_role_info_data00
static int tolua_logic_hld_client_session_get_role_info_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  faith::e_role_info eType = ((faith::e_role_info) (int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_role_info_data'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_role_info_data(eType);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_role_info_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_role_guid of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_role_guid00
static int tolua_logic_hld_client_session_get_role_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_role_guid'", NULL);
#endif
  {
   faith::guid_64 tolua_ret = (faith::guid_64)  self->get_role_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((faith::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(faith::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_role_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_relation_list_mgr of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_relation_list_mgr00
static int tolua_logic_hld_client_session_get_relation_list_mgr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_relation_list_mgr'", NULL);
#endif
  {
   faith::relation_mgr& tolua_ret = (faith::relation_mgr&)  self->get_relation_list_mgr();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::relation_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_relation_list_mgr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_role_name of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_role_name00
static int tolua_logic_hld_client_session_get_role_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_role_name'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->get_role_name();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_role_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_role_name of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_role_name00
static int tolua_logic_hld_client_session_set_role_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  char* _role_name = ((char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_role_name'", NULL);
#endif
  {
   self->set_role_name(_role_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_role_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_notice of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_send_notice00
static int tolua_logic_hld_client_session_send_notice00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  std::string notice_str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_notice'", NULL);
#endif
  {
   self->send_notice(notice_str);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_notice'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_self_server of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_is_self_server00
static int tolua_logic_hld_client_session_is_self_server00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_self_server'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_self_server();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_self_server'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_self_server of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_is_self_server01
static int tolua_logic_hld_client_session_is_self_server01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 target_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_need_same_server_for_ws = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_self_server'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_self_server(target_guid,is_need_same_server_for_ws);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_client_session_is_self_server00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_third_info of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_third_info00
static int tolua_logic_hld_client_session_get_third_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_third_info'", NULL);
#endif
  {
   login_fixed_data& tolua_ret = (login_fixed_data&)  self->get_third_info();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"login_fixed_data");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_third_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_login_type of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_login_type00
static int tolua_logic_hld_client_session_get_login_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_login_type'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_login_type();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_login_type'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_unit_info of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_unit_info00
static int tolua_logic_hld_client_session_get_unit_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_unit_info'", NULL);
#endif
  {
   s_unit_info& tolua_ret = (s_unit_info&)  self->get_unit_info();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"s_unit_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_unit_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_m_is_in_assist of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_m_is_in_assist00
static int tolua_logic_hld_client_session_get_m_is_in_assist00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_m_is_in_assist'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->get_m_is_in_assist();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_m_is_in_assist'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_m_is_in_assist of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_m_is_in_assist00
static int tolua_logic_hld_client_session_set_m_is_in_assist00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  bool value = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_m_is_in_assist'", NULL);
#endif
  {
   self->set_m_is_in_assist(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_m_is_in_assist'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_assit_fight_guid of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_assit_fight_guid00
static int tolua_logic_hld_client_session_set_assit_fight_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  int64 value = ((int64)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_assit_fight_guid'", NULL);
#endif
  {
   self->set_assit_fight_guid(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_assit_fight_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_assit_fight_guid of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_assit_fight_guid00
static int tolua_logic_hld_client_session_get_assit_fight_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_assit_fight_guid'", NULL);
#endif
  {
   int64 tolua_ret = (int64)  self->get_assit_fight_guid();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_assit_fight_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_m_activate_code_time of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_m_activate_code_time00
static int tolua_logic_hld_client_session_get_m_activate_code_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_m_activate_code_time'", NULL);
#endif
  {
   int64 tolua_ret = (int64)  self->get_m_activate_code_time();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_m_activate_code_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_m_activate_code_time of class  faith::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_m_activate_code_time00
static int tolua_logic_hld_client_session_set_m_activate_code_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* self = (faith::client_session*)  tolua_tousertype(tolua_S,1,0);
  int64 value = ((int64)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_m_activate_code_time'", NULL);
#endif
  {
   self->set_m_activate_code_time(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_m_activate_code_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_guid_6400
static int tolua_logic_hld_get_guid_6400(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int64 value = ((int64)  tolua_tointeger(tolua_S,1,0));
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  faith::get_guid_64(value);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_guid_64'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_guid_6401
static int tolua_logic_hld_get_guid_6401(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int32 a = ((int32)  tolua_tointeger(tolua_S,1,0));
  int32 b = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  faith::get_guid_64(a,b);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_get_guid_6400(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_guid_6402
static int tolua_logic_hld_get_guid_6402(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  {
   faith::guid_64* tolua_ret = (faith::guid_64*)  faith::get_guid_64();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_get_guid_6401(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_client_session_by_index */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_client_session_by_index00
static int tolua_logic_hld_get_client_session_by_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int64 unit_index = ((int64)  tolua_tointeger(tolua_S,1,0));
  {
   faith::client_session* tolua_ret = (faith::client_session*)  faith::get_client_session_by_index(unit_index);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::client_session");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_client_session_by_index'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_client_session_by_guid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_client_session_by_guid00
static int tolua_logic_hld_get_client_session_by_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::guid_64 guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,1,0));
  {
   faith::client_session* tolua_ret = (faith::client_session*)  faith::get_client_session_by_guid(guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::client_session");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_client_session_by_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_world_server00
static int tolua_logic_hld_get_world_server00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::world_server* tolua_ret = (faith::world_server*)  faith::get_world_server();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::world_server");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_world_server'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::make_guid_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_make_guid_lua00
static int tolua_logic_hld_make_guid_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int64 in_guid = ((int64)  tolua_tointeger(tolua_S,1,0));
  {
   int64 tolua_ret = (int64)  faith::make_guid_lua(in_guid);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_guid_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_client_session_mgr00
static int tolua_logic_hld_get_client_session_mgr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::client_session_mgr* tolua_ret = (faith::client_session_mgr*)  faith::get_client_session_mgr();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::client_session_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_client_session_mgr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_str_by_string_template_id */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_str_by_string_template_id00
static int tolua_logic_hld_get_str_by_string_template_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int32 template_id = ((int32)  tolua_tointeger(tolua_S,1,0));
  int32 index = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   std::string tolua_ret = (std::string)  faith::get_str_by_string_template_id(template_id,index);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_str_by_string_template_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_item_info_by_str */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_item_info_by_str00
static int tolua_logic_hld_get_item_info_by_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   faith::s_item_info* tolua_ret = (faith::s_item_info*)  faith::get_item_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_item_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_item_info_by_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_item_info_by_str */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_item_info_by_str01
static int tolua_logic_hld_get_item_info_by_str01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   faith::s_item_info* tolua_ret = (faith::s_item_info*)  faith::get_item_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_item_info");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_get_item_info_by_str00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_auction_info_by_str */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_auction_info_by_str00
static int tolua_logic_hld_get_auction_info_by_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   s_auction_info* tolua_ret = (s_auction_info*)  faith::get_auction_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"s_auction_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_auction_info_by_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_auction_buffer */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_auction_buffer00
static int tolua_logic_hld_get_auction_buffer00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"s_auction_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s_auction_info* auction_ptr = ((s_auction_info*)  tolua_tousertype(tolua_S,1,0));
  {
   char* tolua_ret = (char*)  faith::get_auction_buffer(auction_ptr);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_auction_buffer'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_unit_info_by_str */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_unit_info_by_str00
static int tolua_logic_hld_get_unit_info_by_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   s_unit_info* tolua_ret = (s_unit_info*)  faith::get_unit_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"s_unit_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_unit_info_by_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_login_fixed_data_by_str */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_login_fixed_data_by_str00
static int tolua_logic_hld_get_login_fixed_data_by_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   login_fixed_data* tolua_ret = (login_fixed_data*)  faith::get_login_fixed_data_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"login_fixed_data");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_login_fixed_data_by_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_log_common_head */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_log_common_head00
static int tolua_logic_hld_get_log_common_head00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   s_log_common_head* tolua_ret = (s_log_common_head*)  faith::get_log_common_head();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"s_log_common_head");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_log_common_head'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_set_log_common_head_part1 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_set_log_common_head_part100
static int tolua_logic_hld_lua_set_log_common_head_part100(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"s_log_common_head",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s_log_common_head* head = ((s_log_common_head*)  tolua_tousertype(tolua_S,1,0));
  string game_id = ((string)  tolua_tocppstring(tolua_S,2,0));
  string channel_id = ((string)  tolua_tocppstring(tolua_S,3,0));
  string media_id = ((string)  tolua_tocppstring(tolua_S,4,0));
  string device_id = ((string)  tolua_tocppstring(tolua_S,5,0));
  {
   faith::lua_set_log_common_head_part1(*head,game_id,channel_id,media_id,device_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_set_log_common_head_part1'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_set_log_common_head_part2 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_set_log_common_head_part200
static int tolua_logic_hld_lua_set_log_common_head_part200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"s_log_common_head",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s_log_common_head* head = ((s_log_common_head*)  tolua_tousertype(tolua_S,1,0));
  string version_name = ((string)  tolua_tocppstring(tolua_S,2,0));
  int32 version_code = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_set_log_common_head_part2(*head,version_name,version_code);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_set_log_common_head_part2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_set_log_common_head_part3 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_set_log_common_head_part300
static int tolua_logic_hld_lua_set_log_common_head_part300(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"s_log_common_head",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s_log_common_head* head = ((s_log_common_head*)  tolua_tousertype(tolua_S,1,0));
  string user_id = ((string)  tolua_tocppstring(tolua_S,2,0));
  int32 server_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int64 log_time = ((int64)  tolua_tointeger(tolua_S,4,0));
  {
   faith::lua_set_log_common_head_part3(*head,user_id,server_id,log_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_set_log_common_head_part3'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_set_log_common_head_part4 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_set_log_common_head_part400
static int tolua_logic_hld_lua_set_log_common_head_part400(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"s_log_common_head",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s_log_common_head* head = ((s_log_common_head*)  tolua_tousertype(tolua_S,1,0));
  int64 role_id = ((int64)  tolua_tointeger(tolua_S,2,0));
  string role_name = ((string)  tolua_tocppstring(tolua_S,3,0));
  int32 role_level = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 role_gender = ((int32)  tolua_tointeger(tolua_S,5,0));
  {
   faith::lua_set_log_common_head_part4(*head,role_id,role_name,role_level,role_gender);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_set_log_common_head_part4'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_set_log_common_head_part5 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_set_log_common_head_part500
static int tolua_logic_hld_lua_set_log_common_head_part500(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"s_log_common_head",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s_log_common_head* head = ((s_log_common_head*)  tolua_tousertype(tolua_S,1,0));
  int64 battle_points = ((int64)  tolua_tointeger(tolua_S,2,0));
  int32 role_type = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 vip_level = ((int32)  tolua_tointeger(tolua_S,4,0));
  {
   faith::lua_set_log_common_head_part5(*head,battle_points,role_type,vip_level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_set_log_common_head_part5'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_app_key */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_app_key00
static int tolua_logic_hld_get_app_key00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   string tolua_ret = (string)  faith::get_app_key();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_app_key'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_mail_info_by_str */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_mail_info_by_str00
static int tolua_logic_hld_get_mail_info_by_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   s_mail_info* tolua_ret = (s_mail_info*)  faith::get_mail_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"s_mail_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_mail_info_by_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_fake_player_info_by_str */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_fake_player_info_by_str00
static int tolua_logic_hld_get_fake_player_info_by_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   s_fake_player_info* tolua_ret = (s_fake_player_info*)  faith::get_fake_player_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"s_fake_player_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_fake_player_info_by_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_send_big_player_msg */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_send_big_player_msg00
static int tolua_logic_hld_get_send_big_player_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* msg = ((const char*)  tolua_tostring(tolua_S,1,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,2,0));
  int32 out_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   ws2ws_send_big_player_msg* tolua_ret = (ws2ws_send_big_player_msg*)  faith::get_send_big_player_msg(msg,msg_len,out_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ws2ws_send_big_player_msg");
   tolua_pushinteger(tolua_S,(lua_Integer)out_len);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_send_big_player_msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_instance00
static int tolua_logic_hld_globle_data_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::globle_data& tolua_ret = (faith::globle_data&)  faith::globle_data::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::globle_data");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_mail_to_another_player_by_system of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_send_mail_to_another_player_by_system00
static int tolua_logic_hld_globle_data_send_mail_to_another_player_by_system00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,11,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,12,"const faith::s_item_info",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,13,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,14,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 target_player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  faith::guid_64 sender_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int receiver_server_id = ((int)  tolua_tointeger(tolua_S,4,0));
  std::string SenderName = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string title = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  std::string contentText = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  int money_typ1 = ((int)  tolua_tointeger(tolua_S,8,0));
  int money_num1 = ((int)  tolua_tointeger(tolua_S,9,0));
  int money_typ2 = ((int)  tolua_tointeger(tolua_S,10,0));
  int money_num2 = ((int)  tolua_tointeger(tolua_S,11,0));
  const faith::s_item_info* item_list = ((const faith::s_item_info*)  tolua_tousertype(tolua_S,12,0));
  int item_num = ((int)  tolua_tointeger(tolua_S,13,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_mail_to_another_player_by_system'", NULL);
#endif
  {
   self->send_mail_to_another_player_by_system(target_player_guid,sender_guid,receiver_server_id,SenderName,title,contentText,money_typ1,money_num1,money_typ2,money_num2,item_list,item_num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_mail_to_another_player_by_system'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear_char_notice_info of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_clear_char_notice_info00
static int tolua_logic_hld_globle_data_clear_char_notice_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear_char_notice_info'", NULL);
#endif
  {
   self->clear_char_notice_info();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_char_notice_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_notice_id of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_set_notice_id00
static int tolua_logic_hld_globle_data_set_notice_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  int notice_id = ((int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_notice_id'", NULL);
#endif
  {
   self->set_notice_id(notice_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_notice_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_other_name of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_other_name00
static int tolua_logic_hld_globle_data_add_other_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  const std::string temp_other_name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_other_name'", NULL);
#endif
  {
   self->add_other_name(temp_other_name);
   tolua_pushcppstring(tolua_S,(const char*)temp_other_name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_other_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_describe_str of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_describe_str00
static int tolua_logic_hld_globle_data_add_describe_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  const std::string temp_describe_str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_describe_str'", NULL);
#endif
  {
   self->add_describe_str(temp_describe_str);
   tolua_pushcppstring(tolua_S,(const char*)temp_describe_str);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_describe_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_item_name of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_item_name00
static int tolua_logic_hld_globle_data_add_item_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  const std::string temp_item_name = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_item_name'", NULL);
#endif
  {
   self->add_item_name(temp_item_name);
   tolua_pushcppstring(tolua_S,(const char*)temp_item_name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_item_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_num_type_data of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_num_type_data00
static int tolua_logic_hld_globle_data_add_num_type_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  const int temp_num_type_data = ((const int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_num_type_data'", NULL);
#endif
  {
   self->add_num_type_data(temp_num_type_data);
   tolua_pushinteger(tolua_S,(lua_Integer)temp_num_type_data);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_num_type_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_item_data of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_item_data00
static int tolua_logic_hld_globle_data_add_item_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::s_item_info",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  const faith::s_item_info* temp_item_data = ((const faith::s_item_info*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_item_data'", NULL);
#endif
  {
   self->add_item_data(*temp_item_data);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_item_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_mail_common_text of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_mail_common_text00
static int tolua_logic_hld_globle_data_get_mail_common_text00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  int id_index = ((int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_mail_common_text'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->get_mail_common_text(id_index);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_mail_common_text'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_mail_common_text_id of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_mail_common_text_id00
static int tolua_logic_hld_globle_data_get_mail_common_text_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  faith::e_mail_common_text id_index = ((faith::e_mail_common_text) (int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_mail_common_text_id'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->get_mail_common_text_id(id_index);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_mail_common_text_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_region_code of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_region_code00
static int tolua_logic_hld_globle_data_get_region_code00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_region_code'", NULL);
#endif
  {
   int tolua_ret = (int)  self->get_region_code();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_region_code'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_money_str_by_type of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_money_str_by_type00
static int tolua_logic_hld_globle_data_get_money_str_by_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  int32 money_type = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_money_str_by_type'", NULL);
#endif
  {
   string tolua_ret = (string)  self->get_money_str_by_type(money_type);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_money_str_by_type'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_server_on_days of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_server_on_days00
static int tolua_logic_hld_globle_data_get_server_on_days00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  int32 time_stamp = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_server_on_days'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_server_on_days(time_stamp);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_server_on_days'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: make_auction_info of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_make_auction_info00
static int tolua_logic_hld_globle_data_make_auction_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"s_auction_info",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::s_item_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  s_auction_info* target_info = ((s_auction_info*)  tolua_tousertype(tolua_S,2,0));
  faith::s_item_info item_info = *((faith::s_item_info*)  tolua_tousertype(tolua_S,3,0));
  int32 sell_num = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 trade_type = ((int32)  tolua_tointeger(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'make_auction_info'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->make_auction_info(*target_info,item_info,sell_num,trade_type);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_auction_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: make_auction_info of class  faith::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_make_auction_info01
static int tolua_logic_hld_globle_data_make_auction_info01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"s_auction_info",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::s_item_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"faith::guid_64",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,7,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  s_auction_info* target_info = ((s_auction_info*)  tolua_tousertype(tolua_S,2,0));
  faith::s_item_info item_info = *((faith::s_item_info*)  tolua_tousertype(tolua_S,3,0));
  int32 sell_num = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 trade_type = ((int32)  tolua_tointeger(tolua_S,5,0));
  faith::guid_64 seller_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,6,0));
  std::string seller_name = ((std::string)  tolua_tocppstring(tolua_S,7,""));
  int32 seller_vip_level = ((int32)  tolua_tointeger(tolua_S,8,0));
  int32 sell_money_type = ((int32)  tolua_tointeger(tolua_S,9,0));
  int64 sell_total_money = ((int64)  tolua_tointeger(tolua_S,10,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'make_auction_info'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->make_auction_info(*target_info,item_info,sell_num,trade_type,seller_guid,seller_name,seller_vip_level,sell_money_type,sell_total_money);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_globle_data_make_auction_info00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* get function: TargetPlayerGuid of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_TargetPlayerGuid
static int tolua_get_hld__c_mail_info_TargetPlayerGuid(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TargetPlayerGuid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->TargetPlayerGuid,"faith::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: TargetPlayerGuid of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_TargetPlayerGuid
static int tolua_set_hld__c_mail_info_TargetPlayerGuid(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TargetPlayerGuid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->TargetPlayerGuid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: SenderPlayerGuid of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_SenderPlayerGuid
static int tolua_get_hld__c_mail_info_SenderPlayerGuid(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SenderPlayerGuid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->SenderPlayerGuid,"faith::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SenderPlayerGuid of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_SenderPlayerGuid
static int tolua_set_hld__c_mail_info_SenderPlayerGuid(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SenderPlayerGuid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SenderPlayerGuid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: SenderName of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_SenderName
static int tolua_get_hld__c_mail_info_SenderName(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SenderName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->SenderName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SenderName of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_SenderName
static int tolua_set_hld__c_mail_info_SenderName(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SenderName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SenderName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: title of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_title
static int tolua_get_hld__c_mail_info_title(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'title'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->title);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: title of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_title
static int tolua_set_hld__c_mail_info_title(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'title'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->title = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: contentText of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_contentText
static int tolua_get_hld__c_mail_info_contentText(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'contentText'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->contentText);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: contentText of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_contentText
static int tolua_set_hld__c_mail_info_contentText(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'contentText'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->contentText = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: money_typ1 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_typ1
static int tolua_get_hld__c_mail_info_money_typ1(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_typ1'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_typ1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_typ1 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_typ1
static int tolua_set_hld__c_mail_info_money_typ1(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_typ1'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->money_typ1 = ((int)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: money_num1 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_num1
static int tolua_get_hld__c_mail_info_money_num1(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_num1'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_num1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_num1 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_num1
static int tolua_set_hld__c_mail_info_money_num1(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_num1'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->money_num1 = ((int)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: money_typ2 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_typ2
static int tolua_get_hld__c_mail_info_money_typ2(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_typ2'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_typ2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_typ2 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_typ2
static int tolua_set_hld__c_mail_info_money_typ2(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_typ2'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->money_typ2 = ((int)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: money_num2 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_num2
static int tolua_get_hld__c_mail_info_money_num2(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_num2'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_num2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_num2 of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_num2
static int tolua_set_hld__c_mail_info_money_num2(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_num2'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->money_num2 = ((int)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: item_list of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_item_list_ptr
static int tolua_get_hld__c_mail_info_item_list_ptr(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_list'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->item_list,"const faith::s_item_info");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: item_num of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_item_num
static int tolua_get_hld__c_mail_info_item_num(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_num'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->item_num);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: item_num of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_item_num
static int tolua_set_hld__c_mail_info_item_num(lua_State* tolua_S)
{
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_num'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->item_num = ((int)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new00
static int tolua_logic_hld_c_mail_info_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::c_mail_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::c_mail_info* tolua_ret = (faith::c_mail_info*)  Mtolua_new((faith::c_mail_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::c_mail_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new00_local
static int tolua_logic_hld_c_mail_info_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::c_mail_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::c_mail_info* tolua_ret = (faith::c_mail_info*)  Mtolua_new((faith::c_mail_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::c_mail_info");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new01
static int tolua_logic_hld_c_mail_info_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::c_mail_info",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string _SenderName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string _title = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string _contentText = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   faith::c_mail_info* tolua_ret = (faith::c_mail_info*)  Mtolua_new((faith::c_mail_info)(_SenderName,_title,_contentText));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::c_mail_info");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_c_mail_info_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new01_local
static int tolua_logic_hld_c_mail_info_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::c_mail_info",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  std::string _SenderName = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string _title = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string _contentText = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  {
   faith::c_mail_info* tolua_ret = (faith::c_mail_info*)  Mtolua_new((faith::c_mail_info)(_SenderName,_title,_contentText));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::c_mail_info");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_c_mail_info_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear_data of class  faith::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_clear_data00
static int tolua_logic_hld_c_mail_info_clear_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::c_mail_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::c_mail_info* self = (faith::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'clear_data'", NULL);
#endif
  {
   self->clear_data();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_left_shift_operation_result of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_left_shift_operation_result00
static int tolua_logic_hld_init_unit_get_left_shift_operation_result00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int left_operator = ((int)  tolua_tointeger(tolua_S,2,0));
  int right_operator = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   int tolua_ret = (int)  faith::init_unit::get_left_shift_operation_result(left_operator,right_operator);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_left_shift_operation_result'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_bitwise_and_operation_result of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_bitwise_and_operation_result00
static int tolua_logic_hld_init_unit_get_bitwise_and_operation_result00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int left_operator = ((int)  tolua_tointeger(tolua_S,2,0));
  int right_operator = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   int tolua_ret = (int)  faith::init_unit::get_bitwise_and_operation_result(left_operator,right_operator);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_bitwise_and_operation_result'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_bitwise_or_operation_result of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_bitwise_or_operation_result00
static int tolua_logic_hld_init_unit_get_bitwise_or_operation_result00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int left_operator = ((int)  tolua_tointeger(tolua_S,2,0));
  int right_operator = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   int tolua_ret = (int)  faith::init_unit::get_bitwise_or_operation_result(left_operator,right_operator);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_bitwise_or_operation_result'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_item_bag_type of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_item_bag_type00
static int tolua_logic_hld_init_unit_get_item_bag_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int item_template_id = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::e_bag_type tolua_ret = (faith::e_bag_type)  faith::init_unit::get_item_bag_type(item_template_id);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_item_bag_type'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_use_this_jewel_slot_on_this_equip_level of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_equip_level00
static int tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_equip_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int equip_level = ((int)  tolua_tointeger(tolua_S,2,0));
  int slot_index = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::init_unit::is_can_use_this_jewel_slot_on_this_equip_level(equip_level,slot_index);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_use_this_jewel_slot_on_this_equip_level'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_use_this_jewel_slot_on_this_vip_level of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_vip_level00
static int tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_vip_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int role_vip_level = ((int)  tolua_tointeger(tolua_S,2,0));
  int slot_index = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::init_unit::is_can_use_this_jewel_slot_on_this_vip_level(role_vip_level,slot_index);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_use_this_jewel_slot_on_this_vip_level'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_flag_data of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_flag_data00
static int tolua_logic_hld_init_unit_get_flag_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int _flag_datas = ((int)  tolua_tointeger(tolua_S,2,0));
  int _idx = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   int tolua_ret = (int)  faith::init_unit::get_flag_data(_flag_datas,_idx);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_flag_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_max_player_level of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_max_player_level00
static int tolua_logic_hld_init_unit_get_max_player_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  faith::init_unit::get_max_player_level();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_max_player_level'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_max_grade_level of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_max_grade_level00
static int tolua_logic_hld_init_unit_get_max_grade_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  faith::init_unit::get_max_grade_level();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_max_grade_level'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_big_endian of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_big_endian00
static int tolua_logic_hld_init_unit_is_big_endian00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   bool tolua_ret = (bool)  faith::init_unit::is_big_endian();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_big_endian'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_cross_server_big_player of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_cross_server_big_player00
static int tolua_logic_hld_init_unit_is_cross_server_big_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int32 player_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  bool is_begin_cross_activity = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool is_begin_cross_city_war = ((bool)  tolua_toboolean(tolua_S,4,false));
  {
   bool tolua_ret = (bool)  faith::init_unit::is_cross_server_big_player(player_type,is_begin_cross_activity,is_begin_cross_city_war);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_cross_server_big_player'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: init_item_data of class  faith::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_init_item_data00
static int tolua_logic_hld_init_unit_init_item_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::init_unit",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::s_item_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::s_item_info* msg_data = ((faith::s_item_info*)  tolua_tousertype(tolua_S,2,0));
  int32 template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 slot = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 stack_count = ((int32)  tolua_tointeger(tolua_S,5,0));
  faith::e_bag_type container_type = ((faith::e_bag_type) (int)  tolua_tointeger(tolua_S,6,0));
  int32 is_activate = ((int32)  tolua_tointeger(tolua_S,7,0));
  int32 locked = ((int32)  tolua_tointeger(tolua_S,8,1));
  int32 item_over_time = ((int32)  tolua_tointeger(tolua_S,9,0));
  int32 item_const_att = ((int32)  tolua_tointeger(tolua_S,10,0));
  {
   bool tolua_ret = (bool)  faith::init_unit::init_item_data(*msg_data,template_id,slot,stack_count,container_type,is_activate,locked,item_over_time,item_const_att);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'init_item_data'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInstance of class  faith::ws_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws_client_getInstance00
static int tolua_logic_hld_ws_client_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ws_client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::ws_client& tolua_ret = (faith::ws_client&)  faith::ws_client::getInstance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::ws_client");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_to_dp_lua of class  faith::ws_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws_client_send_to_dp_lua00
static int tolua_logic_hld_ws_client_send_to_dp_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::ws_client",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::ws_client* self = (faith::ws_client*)  tolua_tousertype(tolua_S,1,0);
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int msg_len = ((int)  tolua_tointeger(tolua_S,3,0));
  int header = ((int)  tolua_tointeger(tolua_S,4,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_to_dp_lua'", NULL);
#endif
  {
   self->send_to_dp_lua(msg,msg_len,header,server_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_to_dp_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_get_game_info_ws_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_get_game_info_ws_lua00
static int tolua_logic_hld_dp2ws_rep_get_game_info_ws_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_istable(tolua_S,4,0,&tolua_err) ||
     !tolua_istable(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  int begin_time = ((int)  tolua_tointeger(tolua_S,3,0));
  int server_info_arr[8];
  int gm_commond_arr[10];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,4,8,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<8;i++)
    server_info_arr[i] = ((int)  tolua_tofieldinteger(tolua_S,4,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,5,10,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<10;i++)
    gm_commond_arr[i] = ((int)  tolua_tofieldinteger(tolua_S,5,i+1,0));
   }
  }
  {
   faith::dp2ws_rep_get_game_info_ws_lua(conn_index,server_id,begin_time,server_info_arr,gm_commond_arr);
  }
  {
   int i;
   for(i=0; i<8;i++)
    tolua_pushfieldinteger(tolua_S,4,i+1,(lua_Integer) server_info_arr[i]);
  }
  {
   int i;
   for(i=0; i<10;i++)
    tolua_pushfieldinteger(tolua_S,5,i+1,(lua_Integer) gm_commond_arr[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_get_game_info_ws_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_get_role_info_ws_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_get_role_info_ws_lua00
static int tolua_logic_hld_dp2ws_rep_get_role_info_ws_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_istable(tolua_S,8,0,&tolua_err) ||
     !tolua_istable(tolua_S,9,0,&tolua_err) ||
     !tolua_istable(tolua_S,10,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,11,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,12,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,13,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,14,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,15,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  std::string account = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string role_appearance = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string role_first_kill_npc_id_arr = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  long long map_guid = ((long long)  tolua_tointeger(tolua_S,7,0));
#ifdef __cplusplus
  int64* data_i64_ary = Mtolua_new_dim(int64, faith::e_role_i64_info_max);
#else
  int64* data_i64_ary = (int64*) malloc((faith::e_role_i64_info_max)*sizeof(int64));
#endif
#ifdef __cplusplus
  int* data_ary = Mtolua_new_dim(int, faith::e_role_info_max);
#else
  int* data_ary = (int*) malloc((faith::e_role_info_max)*sizeof(int));
#endif
#ifdef __cplusplus
  int* ws_data_ary = Mtolua_new_dim(int, faith::e_role_ws_info_max);
#else
  int* ws_data_ary = (int*) malloc((faith::e_role_ws_info_max)*sizeof(int));
#endif
  int is_login = ((int)  tolua_tointeger(tolua_S,11,0));
  int is_send_chat = ((int)  tolua_tointeger(tolua_S,12,0));
  int worship_value = ((int)  tolua_tointeger(tolua_S,13,0));
  int eresult = ((int)  tolua_tointeger(tolua_S,14,0));
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,8,faith::e_role_i64_info_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<faith::e_role_i64_info_max;i++)
    data_i64_ary[i] = ((int64)  tolua_tofieldinteger(tolua_S,8,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,9,faith::e_role_info_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<faith::e_role_info_max;i++)
    data_ary[i] = ((int)  tolua_tofieldinteger(tolua_S,9,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,10,faith::e_role_ws_info_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<faith::e_role_ws_info_max;i++)
    ws_data_ary[i] = ((int)  tolua_tofieldinteger(tolua_S,10,i+1,0));
   }
  }
  {
   faith::dp2ws_rep_get_role_info_ws_lua(conn_index,role_guid,account,role_name,role_appearance,role_first_kill_npc_id_arr,map_guid,data_i64_ary,data_ary,ws_data_ary,is_login,is_send_chat,worship_value,eresult);
  }
  {
   int i;
   for(i=0; i<faith::e_role_i64_info_max;i++)
    tolua_pushfieldinteger(tolua_S,8,i+1,(lua_Integer) data_i64_ary[i]);
  }
  {
   int i;
   for(i=0; i<faith::e_role_info_max;i++)
    tolua_pushfieldinteger(tolua_S,9,i+1,(lua_Integer) data_ary[i]);
  }
  {
   int i;
   for(i=0; i<faith::e_role_ws_info_max;i++)
    tolua_pushfieldinteger(tolua_S,10,i+1,(lua_Integer) ws_data_ary[i]);
  }
#ifdef __cplusplus
  Mtolua_delete_dim(data_i64_ary);
#else
  free(data_i64_ary);
#endif
#ifdef __cplusplus
  Mtolua_delete_dim(data_ary);
#else
  free(data_ary);
#endif
#ifdef __cplusplus
  Mtolua_delete_dim(ws_data_ary);
#else
  free(ws_data_ary);
#endif
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_get_role_info_ws_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_load_arena_rank_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_load_arena_rank_lua00
static int tolua_logic_hld_dp2ws_rep_load_arena_rank_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  int start_index = ((int)  tolua_tointeger(tolua_S,4,0));
  {
   faith::dp2ws_rep_load_arena_rank_lua(conn_index,data_ptr,data_len,start_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_load_arena_rank_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_load_all_ranking_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_load_all_ranking_info_lua00
static int tolua_logic_hld_dp2ws_rep_load_all_ranking_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  int ranking_type = ((int)  tolua_tointeger(tolua_S,4,0));
  {
   faith::dp2ws_rep_load_all_ranking_info_lua(conn_index,data_ptr,data_len,ranking_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_load_all_ranking_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_load_one_ranking_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_load_one_ranking_info_lua00
static int tolua_logic_hld_dp2ws_rep_load_one_ranking_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  int64 sender_guid = ((int64)  tolua_tointeger(tolua_S,4,0));
  int ranking_type = ((int)  tolua_tointeger(tolua_S,5,0));
  int worship_add_value = ((int)  tolua_tointeger(tolua_S,6,0));
  int worship_type = ((int)  tolua_tointeger(tolua_S,7,0));
  int end_result = ((int)  tolua_tointeger(tolua_S,8,0));
  {
   faith::dp2ws_rep_load_one_ranking_info_lua(conn_index,data_ptr,data_len,sender_guid,ranking_type,worship_add_value,worship_type,end_result);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_load_one_ranking_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_load_one_worship_info_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_load_one_worship_info_proc_lua00
static int tolua_logic_hld_dp2ws_rep_load_one_worship_info_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  int64 sender = ((int64)  tolua_tointeger(tolua_S,4,0));
  bool is_load = ((bool)  tolua_toboolean(tolua_S,5,0));
  {
   faith::dp2ws_rep_load_one_worship_info_proc_lua(conn_index,data_ptr,data_len,sender,is_load);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_load_one_worship_info_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_load_all_service_rank_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_load_all_service_rank_info_lua00
static int tolua_logic_hld_dp2ws_rep_load_all_service_rank_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::dp2ws_rep_load_all_service_rank_info_lua(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_load_all_service_rank_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_rep_load_last_server_list_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_rep_load_last_server_list_lua00
static int tolua_logic_hld_dp2ws_rep_load_last_server_list_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  std::string last_server_list = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   faith::dp2ws_rep_load_last_server_list_lua(conn_index,server_id,last_server_list);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_rep_load_last_server_list_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_gm_set_result_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_gm_set_result_proc_lua00
static int tolua_logic_hld_dp2ws_gm_set_result_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,2,0));
  int64 handle_index = ((int64)  tolua_tointeger(tolua_S,3,0));
  int result_code = ((int)  tolua_tointeger(tolua_S,4,0));
  std::string result_msg = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  {
   faith::dp2ws_gm_set_result_proc_lua(conn_index,listen_port,handle_index,result_code,result_msg);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_gm_set_result_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_save_recharge_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_save_recharge_proc_lua00
static int tolua_logic_hld_dp2ws_save_recharge_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,11,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,12,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  std::string order_id = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  int game_goods_id = ((int)  tolua_tointeger(tolua_S,4,0));
  float pay_price = ((float)  tolua_tonumber(tolua_S,5,0));
  int success = ((int)  tolua_tointeger(tolua_S,6,0));
  int order_num = ((int)  tolua_tointeger(tolua_S,7,0));
  int payment_type = ((int)  tolua_tointeger(tolua_S,8,0));
  int direct_diamond = ((int)  tolua_tointeger(tolua_S,9,0));
  int64 client_uid = ((int64)  tolua_tointeger(tolua_S,10,0));
  std::string order_token = ((std::string)  tolua_tocppstring(tolua_S,11,0));
  {
   faith::dp2ws_save_recharge_proc_lua(conn_index,order_id,role_guid,game_goods_id,pay_price,success,order_num,payment_type,direct_diamond,client_uid,order_token);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_save_recharge_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_send_recharge_end_result_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_send_recharge_end_result_proc_lua00
static int tolua_logic_hld_dp2ws_send_recharge_end_result_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int result_type = ((int)  tolua_tointeger(tolua_S,2,0));
  int64 order_num = ((int64)  tolua_tointeger(tolua_S,3,0));
  int payment_type = ((int)  tolua_tointeger(tolua_S,4,0));
  int goods_id = ((int)  tolua_tointeger(tolua_S,5,0));
  int direct_diamond = ((int)  tolua_tointeger(tolua_S,6,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,7,0));
  std::string order_id = ((std::string)  tolua_tocppstring(tolua_S,8,0));
  {
   faith::dp2ws_send_recharge_end_result_proc_lua(conn_index,result_type,order_num,payment_type,goods_id,direct_diamond,role_guid,order_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_send_recharge_end_result_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_gm_get_server_info_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_gm_get_server_info_proc_lua00
static int tolua_logic_hld_dp2ws_gm_get_server_info_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int acc_num = ((int)  tolua_tointeger(tolua_S,2,0));
  int role_num = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::dp2ws_gm_get_server_info_proc_lua(conn_index,acc_num,role_num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_gm_get_server_info_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_gm_change_account_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_gm_change_account_proc_lua00
static int tolua_logic_hld_dp2ws_gm_change_account_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int change_res = ((int)  tolua_tointeger(tolua_S,2,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,3,0));
  int64 handle_index = ((int64)  tolua_tointeger(tolua_S,4,0));
  {
   faith::dp2ws_gm_change_account_proc_lua(conn_index,change_res,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_gm_change_account_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_gm_get_role_info_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_gm_get_role_info_proc_lua00
static int tolua_logic_hld_dp2ws_gm_get_role_info_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,4,0));
  int64 handle_index = ((int64)  tolua_tointeger(tolua_S,5,0));
  int check_type = ((int)  tolua_tointeger(tolua_S,6,0));
  {
   faith::dp2ws_gm_get_role_info_proc_lua(conn_index,data_ptr,data_len,listen_port,handle_index,check_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_gm_get_role_info_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::dp2ws_gm_modify_role_info_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dp2ws_gm_modify_role_info_proc_lua00
static int tolua_logic_hld_dp2ws_gm_modify_role_info_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int conn_index = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,3,0));
  int64 handle_index = ((int64)  tolua_tointeger(tolua_S,4,0));
  {
   faith::dp2ws_gm_modify_role_info_proc_lua(conn_index,role_guid,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dp2ws_gm_modify_role_info_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_dp2ws_load_red_package_end_proc */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_dp2ws_load_red_package_end_proc00
static int tolua_logic_hld_lua_dp2ws_load_red_package_end_proc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_dp2ws_load_red_package_end_proc(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_dp2ws_load_red_package_end_proc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_dp2ws_load_red_package_receiver_end_proc */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_dp2ws_load_red_package_receiver_end_proc00
static int tolua_logic_hld_lua_dp2ws_load_red_package_receiver_end_proc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_dp2ws_load_red_package_receiver_end_proc(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_dp2ws_load_red_package_receiver_end_proc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::arena_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_arena_mgr_ws_get_instance00
static int tolua_logic_hld_arena_mgr_ws_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::arena_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::arena_mgr_ws& tolua_ret = (faith::arena_mgr_ws&)  faith::arena_mgr_ws::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::arena_mgr_ws");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create_robot of class  faith::arena_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_arena_mgr_ws_create_robot00
static int tolua_logic_hld_arena_mgr_ws_create_robot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::arena_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::arena_mgr_ws* self = (faith::arena_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create_robot'", NULL);
#endif
  {
   self->create_robot();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_robot'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: fill_empty_pos of class  faith::arena_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_arena_mgr_ws_fill_empty_pos00
static int tolua_logic_hld_arena_mgr_ws_fill_empty_pos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::arena_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::arena_mgr_ws* self = (faith::arena_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'fill_empty_pos'", NULL);
#endif
  {
   self->fill_empty_pos();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'fill_empty_pos'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::cross_server_city_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_city_war_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_city_war_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cross_server_city_war_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::cross_server_city_war_ws_mgr& tolua_ret = (faith::cross_server_city_war_ws_mgr&)  faith::cross_server_city_war_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::cross_server_city_war_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_in_game_time of class  faith::cross_server_city_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_city_war_ws_mgr_is_in_game_time00
static int tolua_logic_hld_cross_server_city_war_ws_mgr_is_in_game_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_city_war_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_city_war_ws_mgr* self = (faith::cross_server_city_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_in_game_time'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_in_game_time();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_in_game_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_cur_legion_in_rank of class  faith::cross_server_city_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_city_war_ws_mgr_get_cur_legion_in_rank00
static int tolua_logic_hld_cross_server_city_war_ws_mgr_get_cur_legion_in_rank00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_city_war_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_city_war_ws_mgr* self = (faith::cross_server_city_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_cur_legion_in_rank'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_cur_legion_in_rank(legion_guid);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_cur_legion_in_rank'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: role_guid of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_legion_member_info_role_guid
static int tolua_get_hld__s_legion_member_info_role_guid(lua_State* tolua_S)
{
  faith::s_legion_member_info* self = (faith::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->role_guid,"faith::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_guid of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_legion_member_info_role_guid
static int tolua_set_hld__s_legion_member_info_role_guid(lua_State* tolua_S)
{
  faith::s_legion_member_info* self = (faith::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: role_name of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_legion_member_info_role_name
static int tolua_get_hld__s_legion_member_info_role_name(lua_State* tolua_S)
{
  faith::s_legion_member_info* self = (faith::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->role_name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_name of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_legion_member_info_role_name
static int tolua_set_hld__s_legion_member_info_role_name(lua_State* tolua_S)
{
  faith::s_legion_member_info* self = (faith::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_name'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->role_name,(const char*)tolua_tostring(tolua_S,2,0),101-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gs_value of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_legion_member_info_gs_value
static int tolua_get_hld__s_legion_member_info_gs_value(lua_State* tolua_S)
{
  faith::s_legion_member_info* self = (faith::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gs_value'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->gs_value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gs_value of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_legion_member_info_gs_value
static int tolua_set_hld__s_legion_member_info_gs_value(lua_State* tolua_S)
{
  faith::s_legion_member_info* self = (faith::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gs_value'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->gs_value = ((int64)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: data_ary of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_logic_hld_s_legion_member_info_data_ary
static int tolua_get_logic_hld_s_legion_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  faith::s_legion_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (faith::s_legion_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=faith::e_legion_member_info_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushinteger(tolua_S,(lua_Integer)self->data_ary[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: data_ary of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_logic_hld_s_legion_member_info_data_ary
static int tolua_set_logic_hld_s_legion_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  faith::s_legion_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (faith::s_legion_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=faith::e_legion_member_info_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->data_ary[tolua_index] = ((int64)  tolua_tointeger(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_legion_member_info_new00
static int tolua_logic_hld_s_legion_member_info_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_legion_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_legion_member_info* tolua_ret = (faith::s_legion_member_info*)  Mtolua_new((faith::s_legion_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_legion_member_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_legion_member_info_new00_local
static int tolua_logic_hld_s_legion_member_info_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_legion_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_legion_member_info* tolua_ret = (faith::s_legion_member_info*)  Mtolua_new((faith::s_legion_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_legion_member_info");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_applicant of class  faith::legion_ws_applicant_list */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_applicant_list_get_applicant00
static int tolua_logic_hld_legion_ws_applicant_list_get_applicant00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_applicant_list",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_applicant_list* self = (faith::legion_ws_applicant_list*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 applicant_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_applicant'", NULL);
#endif
  {
   legion_ws_applicant* tolua_ret = (legion_ws_applicant*)  self->get_applicant(applicant_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"legion_ws_applicant");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_applicant'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_applicant of class  faith::legion_ws_applicant_list */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_applicant_list_del_applicant00
static int tolua_logic_hld_legion_ws_applicant_list_del_applicant00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_applicant_list",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_applicant_list* self = (faith::legion_ws_applicant_list*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 applicant_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_applicant'", NULL);
#endif
  {
   self->del_applicant(applicant_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_applicant'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_applicant_with_join_legion of class  faith::legion_ws_applicant_list */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_applicant_list_del_applicant_with_join_legion00
static int tolua_logic_hld_legion_ws_applicant_list_del_applicant_with_join_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_applicant_list",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_applicant_list* self = (faith::legion_ws_applicant_list*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_applicant_with_join_legion'", NULL);
#endif
  {
   self->del_applicant_with_join_legion(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_applicant_with_join_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::legion_ws_answer */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_answer_get_instance00
static int tolua_logic_hld_legion_ws_answer_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::legion_ws_answer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::legion_ws_answer& tolua_ret = (faith::legion_ws_answer&)  faith::legion_ws_answer::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::legion_ws_answer");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion_question_info of class  faith::legion_ws_answer */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_answer_get_legion_question_info00
static int tolua_logic_hld_legion_ws_answer_get_legion_question_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_answer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_answer* self = (faith::legion_ws_answer*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_question_info'", NULL);
#endif
  {
   self->get_legion_question_info(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion_question_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_legion_answer_rank_list of class  faith::legion_ws_answer */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_answer_send_legion_answer_rank_list00
static int tolua_logic_hld_legion_ws_answer_send_legion_answer_rank_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_answer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_answer* self = (faith::legion_ws_answer*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_legion_answer_rank_list'", NULL);
#endif
  {
   self->send_legion_answer_rank_list(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_legion_answer_rank_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_item_list_to_client of class  faith::legion_ws_warehouse */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_warehouse_send_item_list_to_client00
static int tolua_logic_hld_legion_ws_warehouse_send_item_list_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_warehouse",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"uint64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_warehouse* self = (faith::legion_ws_warehouse*)  tolua_tousertype(tolua_S,1,0);
  uint64 role_guid = *((uint64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_item_list_to_client'", NULL);
#endif
  {
   self->send_item_list_to_client(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_item_list_to_client'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_log_list_to_client of class  faith::legion_ws_warehouse */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_warehouse_send_log_list_to_client00
static int tolua_logic_hld_legion_ws_warehouse_send_log_list_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_warehouse",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"uint64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_warehouse* self = (faith::legion_ws_warehouse*)  tolua_tousertype(tolua_S,1,0);
  uint64 role_guid = *((uint64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_log_list_to_client'", NULL);
#endif
  {
   self->send_log_list_to_client(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_log_list_to_client'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_legion_info of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_legion_info00
static int tolua_logic_hld_legion_ws_set_legion_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::ELegionInfo info_index = ((faith::ELegionInfo) (int)  tolua_tointeger(tolua_S,2,0));
  int64 info_data = ((int64)  tolua_tointeger(tolua_S,3,0));
  bool sv_to_db = ((bool)  tolua_toboolean(tolua_S,4,false));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_legion_info'", NULL);
#endif
  {
   self->set_legion_info(info_index,info_data,sv_to_db);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_legion_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion_gs_value of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_gs_value00
static int tolua_logic_hld_legion_ws_get_legion_gs_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_gs_value'", NULL);
#endif
  {
   int64 tolua_ret = (int64)  self->get_legion_gs_value();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion_gs_value'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_member of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_member00
static int tolua_logic_hld_legion_ws_get_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_member'", NULL);
#endif
  {
   faith::s_legion_member_info* tolua_ret = (faith::s_legion_member_info*)  self->get_member(player_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_legion_member_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_member'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_member of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_add_member00
static int tolua_logic_hld_legion_ws_add_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::s_legion_member_info* member_info = ((faith::s_legion_member_info*)  tolua_tousertype(tolua_S,2,0));
  bool is_from_db = ((bool)  tolua_toboolean(tolua_S,3,false));
  bool is_need_save_db = ((bool)  tolua_toboolean(tolua_S,4,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_member'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->add_member(*member_info,is_from_db,is_need_save_db);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_member'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_member of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_add_member01
static int tolua_logic_hld_legion_ws_add_member01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::s_legion_member_info* member_info = ((faith::s_legion_member_info*)  tolua_tousertype(tolua_S,2,0));
  bool is_from_db = ((bool)  tolua_toboolean(tolua_S,3,0));
  bool is_need_save_db = ((bool)  tolua_toboolean(tolua_S,4,0));
  const login_fixed_data* third_info = ((const login_fixed_data*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_member'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->add_member(*member_info,is_from_db,is_need_save_db,*third_info);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_legion_ws_add_member00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_member of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_add_member02
static int tolua_logic_hld_legion_ws_add_member02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::s_legion_member_info* member_info = ((faith::s_legion_member_info*)  tolua_tousertype(tolua_S,2,0));
  bool is_from_db = ((bool)  tolua_toboolean(tolua_S,3,0));
  bool is_need_save_db = ((bool)  tolua_toboolean(tolua_S,4,0));
  const login_fixed_data* third_info = ((const login_fixed_data*)  tolua_tousertype(tolua_S,5,0));
  const int32 login_type = ((const int32)  tolua_tointeger(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_member'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->add_member(*member_info,is_from_db,is_need_save_db,*third_info,login_type);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushinteger(tolua_S,(lua_Integer)login_type);
  }
 }
 return 2;
tolua_lerror:
 return tolua_logic_hld_legion_ws_add_member01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: construction_level_up of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_construction_level_up00
static int tolua_logic_hld_legion_ws_construction_level_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  int32 construction_index = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'construction_level_up'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->construction_level_up(construction_index);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'construction_level_up'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_in_this_legion of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_in_this_legion00
static int tolua_logic_hld_legion_ws_is_in_this_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 target_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_in_this_legion'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_in_this_legion(target_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_in_this_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_message_to_all_member_data_lua of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_message_to_all_member_data_lua00
static int tolua_logic_hld_legion_ws_send_message_to_all_member_data_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  uint32 header = ((uint32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_message_to_all_member_data_lua'", NULL);
#endif
  {
   self->send_message_to_all_member_data_lua(data_ptr,data_len,header);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_message_to_all_member_data_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: leave_voie_channel of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_leave_voie_channel00
static int tolua_logic_hld_legion_ws_leave_voie_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'leave_voie_channel'", NULL);
#endif
  {
   self->leave_voie_channel(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'leave_voie_channel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_applicant_info_list of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_applicant_info_list00
static int tolua_logic_hld_legion_ws_send_applicant_info_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_applicant_info_list'", NULL);
#endif
  {
   self->send_applicant_info_list(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_applicant_info_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_member of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_del_member00
static int tolua_logic_hld_legion_ws_del_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const login_fixed_data* third_info = ((const login_fixed_data*)  tolua_tousertype(tolua_S,3,0));
  const int32 login_type = ((const int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_member'", NULL);
#endif
  {
   self->del_member(player_guid,*third_info,login_type);
   tolua_pushinteger(tolua_S,(lua_Integer)login_type);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_member'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: appoint of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_appoint00
static int tolua_logic_hld_legion_ws_appoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  faith::e_legion_job_title job_title = ((faith::e_legion_job_title) (int)  tolua_tointeger(tolua_S,3,0));
  const login_fixed_data* third_info = ((const login_fixed_data*)  tolua_tousertype(tolua_S,4,0));
  const int32 login_type = ((const int32)  tolua_tointeger(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'appoint'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->appoint(player_guid,job_title,*third_info,login_type);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
   tolua_pushinteger(tolua_S,(lua_Integer)login_type);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'appoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_job_title of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_job_title00
static int tolua_logic_hld_legion_ws_get_job_title00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_job_title'", NULL);
#endif
  {
   faith::e_legion_job_title tolua_ret = (faith::e_legion_job_title)  self->get_job_title(player_guid);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_job_title'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion_rank of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_rank00
static int tolua_logic_hld_legion_ws_get_legion_rank00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_rank'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_legion_rank();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion_rank'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_chief of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_chief00
static int tolua_logic_hld_legion_ws_is_chief00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_chief'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_chief(player_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_chief'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_assistant_chief of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_assistant_chief00
static int tolua_logic_hld_legion_ws_is_assistant_chief00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_assistant_chief'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_assistant_chief(player_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_assistant_chief'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion_guid of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_guid00
static int tolua_logic_hld_legion_ws_get_legion_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_guid'", NULL);
#endif
  {
   faith::guid_64& tolua_ret = (faith::guid_64&)  self->get_legion_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::guid_64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_announcement of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_announcement00
static int tolua_logic_hld_legion_ws_set_announcement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  const char* content = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 content_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  bool need_send_mail = ((bool)  tolua_toboolean(tolua_S,4,true));
  bool need_save_db = ((bool)  tolua_toboolean(tolua_S,5,0));
  faith::guid_64 do_change_role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,6,0));
  const login_fixed_data* third_info = ((const login_fixed_data*)  tolua_tousertype(tolua_S,7,0));
  const int32 login_type = ((const int32)  tolua_tointeger(tolua_S,8,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_announcement'", NULL);
#endif
  {
   self->set_announcement(content,content_len,need_send_mail,need_save_db,do_change_role_guid,*third_info,login_type);
   tolua_pushinteger(tolua_S,(lua_Integer)login_type);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_announcement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_legion_announcement of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_legion_announcement00
static int tolua_logic_hld_legion_ws_send_legion_announcement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_legion_announcement'", NULL);
#endif
  {
   self->send_legion_announcement();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_legion_announcement'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion_name of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_name00
static int tolua_logic_hld_legion_ws_get_legion_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_name'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->get_legion_name();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_chief_guid of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_chief_guid00
static int tolua_logic_hld_legion_ws_get_chief_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_chief_guid'", NULL);
#endif
  {
   faith::guid_64 tolua_ret = (faith::guid_64)  self->get_chief_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((faith::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(faith::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_chief_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_legion_attr_one of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_legion_attr_one00
static int tolua_logic_hld_legion_ws_send_legion_attr_one00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::ELegionInfo legion_info_index = ((faith::ELegionInfo) (int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_legion_attr_one'", NULL);
#endif
  {
   self->send_legion_attr_one(legion_info_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_legion_attr_one'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: level_up_legion_skill of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_level_up_legion_skill00
static int tolua_logic_hld_legion_ws_level_up_legion_skill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 skill_template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'level_up_legion_skill'", NULL);
#endif
  {
   self->level_up_legion_skill(player_guid,skill_template_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'level_up_legion_skill'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: join_legion_station_map of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_join_legion_station_map00
static int tolua_logic_hld_legion_ws_join_legion_station_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'join_legion_station_map'", NULL);
#endif
  {
   self->join_legion_station_map(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'join_legion_station_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_legion_boss_map_info of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_legion_boss_map_info00
static int tolua_logic_hld_legion_ws_send_legion_boss_map_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_legion_boss_map_info'", NULL);
#endif
  {
   self->send_legion_boss_map_info(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_legion_boss_map_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_applicant_list of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_applicant_list00
static int tolua_logic_hld_legion_ws_get_applicant_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_applicant_list'", NULL);
#endif
  {
   faith::legion_ws_applicant_list& tolua_ret = (faith::legion_ws_applicant_list&)  self->get_applicant_list();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::legion_ws_applicant_list");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_applicant_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_warehouse of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_warehouse00
static int tolua_logic_hld_legion_ws_get_warehouse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_warehouse'", NULL);
#endif
  {
   faith::legion_ws_warehouse& tolua_ret = (faith::legion_ws_warehouse&)  self->get_warehouse();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::legion_ws_warehouse");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_warehouse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_auction_selling_check_start of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_auction_selling_check_start00
static int tolua_logic_hld_legion_ws_set_auction_selling_check_start00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  bool is_start = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_auction_selling_check_start'", NULL);
#endif
  {
   self->set_auction_selling_check_start(is_start);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_auction_selling_check_start'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: join_legion_station_map of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_join_legion_station_map01
static int tolua_logic_hld_legion_ws_join_legion_station_map01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'join_legion_station_map'", NULL);
#endif
  {
   self->join_legion_station_map(role_guid);
  }
 }
 return 0;
tolua_lerror:
 return tolua_logic_hld_legion_ws_join_legion_station_map00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: join_voice_channel of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_join_voice_channel00
static int tolua_logic_hld_legion_ws_join_voice_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'join_voice_channel'", NULL);
#endif
  {
   self->join_voice_channel(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'join_voice_channel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: leave_voie_channel of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_leave_voie_channel01
static int tolua_logic_hld_legion_ws_leave_voie_channel01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'leave_voie_channel'", NULL);
#endif
  {
   self->leave_voie_channel(role_guid);
  }
 }
 return 0;
tolua_lerror:
 return tolua_logic_hld_legion_ws_leave_voie_channel00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: change_legion_name of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_change_legion_name00
static int tolua_logic_hld_legion_ws_change_legion_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const char* legion_name = ((const char*)  tolua_tostring(tolua_S,3,0));
  int32 legion_name_len = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'change_legion_name'", NULL);
#endif
  {
   self->change_legion_name(*role_guid,legion_name,legion_name_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'change_legion_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_next_bid_time of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_next_bid_time00
static int tolua_logic_hld_legion_ws_set_next_bid_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_next_bid_time'", NULL);
#endif
  {
   self->set_next_bid_time();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_next_bid_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_bid of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_can_bid00
static int tolua_logic_hld_legion_ws_is_can_bid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_can_bid'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_can_bid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_bid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_all_member_guid of class  faith::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_all_member_guid00
static int tolua_logic_hld_legion_ws_get_all_member_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws* self = (faith::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_all_member_guid'", NULL);
#endif
  {
   string tolua_ret = (string)  self->get_all_member_guid();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_all_member_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_get_instance00
static int tolua_logic_hld_city_war_territory_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::city_war_territory_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::city_war_territory_mgr& tolua_ret = (faith::city_war_territory_mgr&)  faith::city_war_territory_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::city_war_territory_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: bid_one_territory of class  faith::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_bid_one_territory00
static int tolua_logic_hld_city_war_territory_mgr_bid_one_territory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::city_war_territory_mgr* self = (faith::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 territory_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 bid_index = ((int32)  tolua_tointeger(tolua_S,4,0));
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'bid_one_territory'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->bid_one_territory(legion_guid,territory_id,bid_index,role_guid);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'bid_one_territory'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_is_prior_maintain of class  faith::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_set_is_prior_maintain00
static int tolua_logic_hld_city_war_territory_mgr_set_is_prior_maintain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::city_war_territory_mgr* self = (faith::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 territory_id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_is_prior_maintain'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->set_is_prior_maintain(legion_guid,territory_id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_is_prior_maintain'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_cross_server_apply_city_war_bid of class  faith::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_req_cross_server_apply_city_war_bid00
static int tolua_logic_hld_city_war_territory_mgr_req_cross_server_apply_city_war_bid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::city_war_territory_mgr* self = (faith::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 territory_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 bid_index = ((int32)  tolua_tointeger(tolua_S,4,0));
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_cross_server_apply_city_war_bid'", NULL);
#endif
  {
   self->req_cross_server_apply_city_war_bid(legion_guid,territory_id,bid_index,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_cross_server_apply_city_war_bid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_cross_server_territory_prior_maintain of class  faith::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_req_cross_server_territory_prior_maintain00
static int tolua_logic_hld_city_war_territory_mgr_req_cross_server_territory_prior_maintain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::city_war_territory_mgr* self = (faith::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legin_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifdef __cplusplus
  int32* TerritoryIds = Mtolua_new_dim(int32, cross_server_territory_city_max);
#else
  int32* TerritoryIds = (int32*) malloc((cross_server_territory_city_max)*sizeof(int32));
#endif
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,4,0));
  int32 territoryids_num = ((int32)  tolua_tointeger(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_cross_server_territory_prior_maintain'", NULL);
#endif
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,3,cross_server_territory_city_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<cross_server_territory_city_max;i++)
    TerritoryIds[i] = ((int32)  tolua_tofieldinteger(tolua_S,3,i+1,0));
   }
  }
  {
   self->req_cross_server_territory_prior_maintain(legin_guid,TerritoryIds,role_guid,territoryids_num);
  }
  {
   int i;
   for(i=0; i<cross_server_territory_city_max;i++)
    tolua_pushfieldinteger(tolua_S,3,i+1,(lua_Integer) TerritoryIds[i]);
  }
#ifdef __cplusplus
  Mtolua_delete_dim(TerritoryIds);
#else
  free(TerritoryIds);
#endif
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_cross_server_territory_prior_maintain'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_cross_server_legion_city_war_info of class  faith::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_req_cross_server_legion_city_war_info00
static int tolua_logic_hld_city_war_territory_mgr_req_cross_server_legion_city_war_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::city_war_territory_mgr* self = (faith::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_cross_server_legion_city_war_info'", NULL);
#endif
  {
   self->req_cross_server_legion_city_war_info(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_cross_server_legion_city_war_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_overlord_legion of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_overlord_legion00
static int tolua_logic_hld_legion_ws_city_war_get_overlord_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_overlord_legion'", NULL);
#endif
  {
   faith::guid_64& tolua_ret = (faith::guid_64&)  self->get_overlord_legion();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::guid_64");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_overlord_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_overlord_legion_name of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_overlord_legion_name00
static int tolua_logic_hld_legion_ws_city_war_get_overlord_legion_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_overlord_legion_name'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->get_overlord_legion_name();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_overlord_legion_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_city_master_guid of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_city_master_guid00
static int tolua_logic_hld_legion_ws_city_war_get_city_master_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_city_master_guid'", NULL);
#endif
  {
   faith::guid_64 tolua_ret = (faith::guid_64)  self->get_city_master_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((faith::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(faith::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_city_master_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_city_master_name of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_city_master_name00
static int tolua_logic_hld_legion_ws_city_war_get_city_master_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_city_master_name'", NULL);
#endif
  {
   char* tolua_ret = (char*)  self->get_city_master_name();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_city_master_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_during_city_war of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_is_during_city_war00
static int tolua_logic_hld_legion_ws_city_war_is_during_city_war00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64 legion_guid = *((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_during_city_war'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_during_city_war(legion_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_during_city_war'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_during_bid_time of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_is_during_bid_time00
static int tolua_logic_hld_legion_ws_city_war_is_during_bid_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  int32 terr_id = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_during_bid_time'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_during_bid_time(terr_id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_during_bid_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_territory_info_one of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_send_territory_info_one00
static int tolua_logic_hld_legion_ws_city_war_send_territory_info_one00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 terr_id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_territory_info_one'", NULL);
#endif
  {
   self->send_territory_info_one(session,terr_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_territory_info_one'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_city_war_info_all_msg of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_send_city_war_info_all_msg00
static int tolua_logic_hld_legion_ws_city_war_send_city_war_info_all_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_city_war_info_all_msg'", NULL);
#endif
  {
   self->send_city_war_info_all_msg(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_city_war_info_all_msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_banquet_holding of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_is_banquet_holding00
static int tolua_logic_hld_legion_ws_city_war_is_banquet_holding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_banquet_holding'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_banquet_holding();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_banquet_holding'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: take_part_in_banquet of class  faith::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_take_part_in_banquet00
static int tolua_logic_hld_legion_ws_city_war_take_part_in_banquet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_city_war* self = (faith::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'take_part_in_banquet'", NULL);
#endif
  {
   self->take_part_in_banquet(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'take_part_in_banquet'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_instance00
static int tolua_logic_hld_legion_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::legion_ws_mgr& tolua_ret = (faith::legion_ws_mgr&)  faith::legion_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::legion_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_legion of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_del_legion00
static int tolua_logic_hld_legion_ws_mgr_del_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool need_del_db = ((bool)  tolua_toboolean(tolua_S,3,0));
  faith::guid_64 del_role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,4,0));
  const login_fixed_data* third_info = ((const login_fixed_data*)  tolua_tousertype(tolua_S,5,0));
  const int32 login_type = ((const int32)  tolua_tointeger(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_legion'", NULL);
#endif
  {
   self->del_legion(legion_guid,need_del_db,del_role_guid,*third_info,login_type);
   tolua_pushinteger(tolua_S,(lua_Integer)login_type);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_legion00
static int tolua_logic_hld_legion_ws_mgr_get_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion'", NULL);
#endif
  {
   faith::legion_ws* tolua_ret = (faith::legion_ws*)  self->get_legion(legion_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::legion_ws");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: create_legion_begin of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_create_legion_begin00
static int tolua_logic_hld_legion_ws_mgr_create_legion_begin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const char* l_name = ((const char*)  tolua_tostring(tolua_S,3,0));
  int32 l_name_len = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 l_level = ((int32)  tolua_tointeger(tolua_S,5,0));
  bool auto_accept = ((bool)  tolua_toboolean(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'create_legion_begin'", NULL);
#endif
  {
   self->create_legion_begin(player_guid,l_name,l_name_len,l_level,auto_accept);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'create_legion_begin'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: check_can_join_legion of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_check_can_join_legion00
static int tolua_logic_hld_legion_ws_mgr_check_can_join_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::legion_ws",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::legion_ws* legion_ref = ((const faith::legion_ws*)  tolua_tousertype(tolua_S,2,0));
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  faith::client_session* op_member_session = ((faith::client_session*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'check_can_join_legion'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->check_can_join_legion(*legion_ref,player_guid,op_member_session);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'check_can_join_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: check_can_leave_legion of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_check_can_leave_legion00
static int tolua_logic_hld_legion_ws_mgr_check_can_leave_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::legion_ws",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::legion_ws* legion_ref = ((const faith::legion_ws*)  tolua_tousertype(tolua_S,2,0));
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  faith::client_session* op_member_session = ((faith::client_session*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'check_can_leave_legion'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->check_can_leave_legion(*legion_ref,player_guid,op_member_session);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'check_can_leave_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: generate_player_legion_info of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_generate_player_legion_info00
static int tolua_logic_hld_legion_ws_mgr_generate_player_legion_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"legion_ws_applicant",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  faith::s_legion_member_info* member_info = ((faith::s_legion_member_info*)  tolua_tousertype(tolua_S,3,0));
  legion_ws_applicant* applicant_info = ((legion_ws_applicant*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'generate_player_legion_info'", NULL);
#endif
  {
   self->generate_player_legion_info(session,*member_info,applicant_info);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'generate_player_legion_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_legion_error_to_session of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_send_legion_error_to_session00
static int tolua_logic_hld_legion_ws_mgr_send_legion_error_to_session00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  faith::e_legion_error err_type = ((faith::e_legion_error) (int)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_legion_error_to_session'", NULL);
#endif
  {
   self->send_legion_error_to_session(session,err_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_legion_error_to_session'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_unit_legion of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_unit_legion00
static int tolua_logic_hld_legion_ws_mgr_get_unit_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* unit_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_unit_legion'", NULL);
#endif
  {
   faith::legion_ws* tolua_ret = (faith::legion_ws*)  self->get_unit_legion(*unit_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::legion_ws");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_unit_legion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_into_auto_accept_new_member_ordered_legion_list of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_add_into_auto_accept_new_member_ordered_legion_list00
static int tolua_logic_hld_legion_ws_mgr_add_into_auto_accept_new_member_ordered_legion_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int64 legion_fighting_power = ((int64)  tolua_tointeger(tolua_S,2,0));
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_into_auto_accept_new_member_ordered_legion_list'", NULL);
#endif
  {
   self->add_into_auto_accept_new_member_ordered_legion_list(legion_fighting_power,legion_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_into_auto_accept_new_member_ordered_legion_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_from_auto_accept_new_member_ordered_legion_list of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_del_from_auto_accept_new_member_ordered_legion_list00
static int tolua_logic_hld_legion_ws_mgr_del_from_auto_accept_new_member_ordered_legion_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_from_auto_accept_new_member_ordered_legion_list'", NULL);
#endif
  {
   self->del_from_auto_accept_new_member_ordered_legion_list(legion_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_from_auto_accept_new_member_ordered_legion_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_legion_city_war of class  faith::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_legion_city_war00
static int tolua_logic_hld_legion_ws_mgr_get_legion_city_war00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::legion_ws_mgr* self = (faith::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_city_war'", NULL);
#endif
  {
   faith::legion_ws_city_war& tolua_ret = (faith::legion_ws_city_war&)  self->get_legion_city_war();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::legion_ws_city_war");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_legion_city_war'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_legion_list_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_legion_list_logic00
static int tolua_logic_hld_c2ws_req_get_legion_list_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 start_index = ((int32)  tolua_tointeger(tolua_S,2,0));
  int64 base_fighting_power = ((int64)  tolua_tointeger(tolua_S,3,0));
  int32 only_get_auto_accpet_new_member_legion = ((int32)  tolua_tointeger(tolua_S,4,0));
  {
   faith::c2ws_req_get_legion_list_logic(session,start_index,base_fighting_power,only_get_auto_accpet_new_member_legion);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_legion_list_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_legion_donate_money_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_legion_donate_money_logic00
static int tolua_logic_hld_c2ws_req_legion_donate_money_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 donate_times = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   faith::c2ws_req_legion_donate_money_logic(session,donate_times);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_legion_donate_money_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_legion_donate_item_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_legion_donate_item_logic00
static int tolua_logic_hld_c2ws_req_legion_donate_item_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 donate_item_index = ((int32)  tolua_tointeger(tolua_S,2,0));
  int32 donate_times = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_legion_donate_item_logic(session,donate_item_index,donate_times);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_legion_donate_item_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_event_list_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_event_list_logic00
static int tolua_logic_hld_c2ws_req_get_event_list_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 from_happen_time = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   faith::c2ws_req_get_event_list_logic(session,from_happen_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_event_list_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_legion_boss_award_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_legion_boss_award_logic00
static int tolua_logic_hld_c2ws_req_get_legion_boss_award_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   faith::c2ws_req_get_legion_boss_award_logic(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_legion_boss_award_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_banquet_info_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_banquet_info_logic00
static int tolua_logic_hld_c2ws_req_get_banquet_info_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   faith::c2ws_req_get_banquet_info_logic(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_banquet_info_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_request_hold_banquet_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_request_hold_banquet_logic00
static int tolua_logic_hld_c2ws_req_request_hold_banquet_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 in_banquet_level = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   faith::c2ws_req_request_hold_banquet_logic(session,in_banquet_level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_request_hold_banquet_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_enter_city_war_map_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_enter_city_war_map_logic00
static int tolua_logic_hld_c2ws_req_enter_city_war_map_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 in_terr_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   faith::c2ws_req_enter_city_war_map_logic(session,in_terr_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_enter_city_war_map_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_city_war_operate_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_city_war_operate_logic00
static int tolua_logic_hld_c2ws_req_city_war_operate_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 in_operate_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  int64 in_prop_npc_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  int32 in_terr_id = ((int32)  tolua_tointeger(tolua_S,4,0));
  {
   faith::c2ws_req_city_war_operate_logic(session,in_operate_type,in_prop_npc_guid,in_terr_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_city_war_operate_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_add_fuel_to_fire_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_add_fuel_to_fire_logic00
static int tolua_logic_hld_c2ws_req_add_fuel_to_fire_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   faith::c2ws_req_add_fuel_to_fire_logic(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_add_fuel_to_fire_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_send_legion_recruit_in_world_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_send_legion_recruit_in_world_logic00
static int tolua_logic_hld_c2ws_req_send_legion_recruit_in_world_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  bool in_is_rm_cd = ((bool)  tolua_toboolean(tolua_S,2,0));
  {
   faith::c2ws_req_send_legion_recruit_in_world_logic(session,in_is_rm_cd);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_send_legion_recruit_in_world_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_city_war_legion_rank_info_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_city_war_legion_rank_info_logic00
static int tolua_logic_hld_c2ws_req_get_city_war_legion_rank_info_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   faith::c2ws_req_get_city_war_legion_rank_info_logic(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_city_war_legion_rank_info_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_draw_bonfire_daily_reward_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_draw_bonfire_daily_reward_logic00
static int tolua_logic_hld_c2ws_req_draw_bonfire_daily_reward_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   faith::c2ws_req_draw_bonfire_daily_reward_logic(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_draw_bonfire_daily_reward_logic'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_legion_call_operate_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_legion_call_operate_lua00
static int tolua_logic_hld_c2ws_req_legion_call_operate_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_legion_call_operate_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_legion_call_operate_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_bonus_rewards_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_bonus_rewards_lua00
static int tolua_logic_hld_c2ws_req_get_bonus_rewards_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_get_bonus_rewards_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_bonus_rewards_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_stimulate_bonus_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_stimulate_bonus_lua00
static int tolua_logic_hld_c2ws_req_stimulate_bonus_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_stimulate_bonus_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_stimulate_bonus_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_join_cross_city_war_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_join_cross_city_war_lua00
static int tolua_logic_hld_c2ws_req_join_cross_city_war_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_join_cross_city_war_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_join_cross_city_war_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_goods_operate_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_goods_operate_lua00
static int tolua_logic_hld_c2ws_req_goods_operate_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_goods_operate_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_goods_operate_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_invite_player_join_team_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_invite_player_join_team_lua00
static int tolua_logic_hld_c2ws_req_invite_player_join_team_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_invite_player_join_team_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_invite_player_join_team_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_ready_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_ready_lua00
static int tolua_logic_hld_c2ws_req_ready_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_ready_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_ready_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_search_team_list_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_search_team_list_lua00
static int tolua_logic_hld_c2ws_req_search_team_list_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_search_team_list_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_search_team_list_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_team_list_by_aim_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_team_list_by_aim_lua00
static int tolua_logic_hld_c2ws_req_get_team_list_by_aim_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_get_team_list_by_aim_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_team_list_by_aim_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_team_mem_operate_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_team_mem_operate_lua00
static int tolua_logic_hld_c2ws_req_team_mem_operate_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_team_mem_operate_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_team_mem_operate_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_get_best_record_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_best_record_lua00
static int tolua_logic_hld_c2ws_req_get_best_record_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_get_best_record_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_get_best_record_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_syc_nearly_state_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_syc_nearly_state_lua00
static int tolua_logic_hld_c2ws_req_syc_nearly_state_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_syc_nearly_state_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_syc_nearly_state_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2ws_req_operate_other_req_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_operate_other_req_lua00
static int tolua_logic_hld_c2ws_req_operate_other_req_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2ws_req_operate_other_req_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_operate_other_req_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::c2s_confirm_receive_marry_task_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2s_confirm_receive_marry_task_lua00
static int tolua_logic_hld_c2s_confirm_receive_marry_task_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::c2s_confirm_receive_marry_task_lua(session,msg,msg_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2s_confirm_receive_marry_task_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: include_invalid_ansi_str of class  faith::invalid_ansi_word */
#ifndef TOLUA_DISABLE_tolua_logic_hld_invalid_ansi_word_include_invalid_ansi_str00
static int tolua_logic_hld_invalid_ansi_word_include_invalid_ansi_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::invalid_ansi_word",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  faith::invalid_ansi_word::include_invalid_ansi_str(str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'include_invalid_ansi_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_valid_ansi_str of class  faith::invalid_ansi_word */
#ifndef TOLUA_DISABLE_tolua_logic_hld_invalid_ansi_word_is_valid_ansi_str00
static int tolua_logic_hld_invalid_ansi_word_is_valid_ansi_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::invalid_ansi_word",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  faith::invalid_ansi_word::is_valid_ansi_str(str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_valid_ansi_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replace_invalid_ansi_str of class  faith::invalid_ansi_word */
#ifndef TOLUA_DISABLE_tolua_logic_hld_invalid_ansi_word_replace_invalid_ansi_str00
static int tolua_logic_hld_invalid_ansi_word_replace_invalid_ansi_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::invalid_ansi_word",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  bool only_check_symbols = ((bool)  tolua_toboolean(tolua_S,3,false));
  {
   faith::invalid_ansi_word::replace_invalid_ansi_str(str,only_check_symbols);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replace_invalid_ansi_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: role_guid of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_role_guid
static int tolua_get_hld__s_team_member_info_role_guid(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->role_guid,"faith::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_guid of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_role_guid
static int tolua_set_hld__s_team_member_info_role_guid(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: role_index of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_role_index
static int tolua_get_hld__s_team_member_info_role_index(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_index'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->role_index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_index of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_role_index
static int tolua_set_hld__s_team_member_info_role_index(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_index'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->role_index = ((int32)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: role_name of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_role_name
static int tolua_get_hld__s_team_member_info_role_name(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->role_name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_name of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_role_name
static int tolua_set_hld__s_team_member_info_role_name(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_name'",NULL);
  if (!tolua_istable(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
 strncpy((char*)
self->role_name,(const char*)tolua_tostring(tolua_S,2,0),101-1);
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: gs_value of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_gs_value
static int tolua_get_hld__s_team_member_info_gs_value(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gs_value'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->gs_value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gs_value of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_gs_value
static int tolua_set_hld__s_team_member_info_gs_value(lua_State* tolua_S)
{
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gs_value'",NULL);
  if (!tolua_isinteger(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->gs_value = ((int64)  tolua_tointeger(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: data_ary of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_logic_hld_s_team_member_info_data_ary
static int tolua_get_logic_hld_s_team_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  faith::s_team_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (faith::s_team_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=faith::ETeamMemberInfo_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushinteger(tolua_S,(lua_Integer)self->data_ary[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: data_ary of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_logic_hld_s_team_member_info_data_ary
static int tolua_set_logic_hld_s_team_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  faith::s_team_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (faith::s_team_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=faith::ETeamMemberInfo_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->data_ary[tolua_index] = ((int64)  tolua_tointeger(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_valid of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_team_member_info_is_valid00
static int tolua_logic_hld_s_team_member_info_is_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::s_team_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::s_team_member_info* self = (faith::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_valid'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_valid();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_valid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_team_member_info_new00
static int tolua_logic_hld_s_team_member_info_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_team_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_team_member_info* tolua_ret = (faith::s_team_member_info*)  Mtolua_new((faith::s_team_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_team_member_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  faith::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_team_member_info_new00_local
static int tolua_logic_hld_s_team_member_info_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::s_team_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::s_team_member_info* tolua_ret = (faith::s_team_member_info*)  Mtolua_new((faith::s_team_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_team_member_info");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_member of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_del_member00
static int tolua_logic_hld_team_ws_del_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_member'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->del_member(player_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_member'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_member of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_member00
static int tolua_logic_hld_team_ws_get_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_member'", NULL);
#endif
  {
   faith::s_team_member_info* tolua_ret = (faith::s_team_member_info*)  self->get_member(player_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_team_member_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_member'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_member_num of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_member_num00
static int tolua_logic_hld_team_ws_get_member_num00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_member_num'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_member_num();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_member_num'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_captain of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_set_captain00
static int tolua_logic_hld_team_ws_set_captain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_captain'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->set_captain(player_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_captain'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_captain_guid of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_captain_guid00
static int tolua_logic_hld_team_ws_get_captain_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_captain_guid'", NULL);
#endif
  {
   faith::guid_64 tolua_ret = (faith::guid_64)  self->get_captain_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((faith::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(faith::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_captain_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transfer_all_member_to_scene of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_transfer_all_member_to_scene00
static int tolua_logic_hld_team_ws_transfer_all_member_to_scene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
  int32 map_template_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  bool need_ready = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transfer_all_member_to_scene'", NULL);
#endif
  {
   faith::e_team_error tolua_ret = (faith::e_team_error)  self->transfer_all_member_to_scene(map_template_id,need_ready);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transfer_all_member_to_scene'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_team_state of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_team_state00
static int tolua_logic_hld_team_ws_get_team_state00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_team_state'", NULL);
#endif
  {
   faith::e_team_state tolua_ret = (faith::e_team_state)  self->get_team_state();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_team_state'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_add_member_to_cs of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_req_add_member_to_cs00
static int tolua_logic_hld_team_ws_req_add_member_to_cs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_add_member_to_cs'", NULL);
#endif
  {
   self->req_add_member_to_cs(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_add_member_to_cs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: join_voice_channel of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_join_voice_channel00
static int tolua_logic_hld_team_ws_join_voice_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'join_voice_channel'", NULL);
#endif
  {
   self->join_voice_channel(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'join_voice_channel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: leave_voie_channel of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_leave_voie_channel00
static int tolua_logic_hld_team_ws_leave_voie_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'leave_voie_channel'", NULL);
#endif
  {
   self->leave_voie_channel(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'leave_voie_channel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_member_list_name of class  faith::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_member_list_name00
static int tolua_logic_hld_team_ws_get_member_list_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws* self = (faith::team_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_member_list_name'", NULL);
#endif
  {
   string tolua_ret = (string)  self->get_member_list_name();
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_member_list_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_get_instance00
static int tolua_logic_hld_team_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::team_ws_mgr& tolua_ret = (faith::team_ws_mgr&)  faith::team_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::team_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_team of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_get_team00
static int tolua_logic_hld_team_ws_mgr_get_team00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws_mgr* self = (faith::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 team_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_team'", NULL);
#endif
  {
   faith::team_ws* tolua_ret = (faith::team_ws*)  self->get_team(team_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::team_ws");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_team'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_unit_team of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_get_unit_team00
static int tolua_logic_hld_team_ws_mgr_get_unit_team00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws_mgr* self = (faith::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* unit_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_unit_team'", NULL);
#endif
  {
   faith::team_ws* tolua_ret = (faith::team_ws*)  self->get_unit_team(*unit_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::team_ws");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_unit_team'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_team_error_to_session of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_send_team_error_to_session00
static int tolua_logic_hld_team_ws_mgr_send_team_error_to_session00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws_mgr* self = (faith::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  faith::e_team_error err_type = ((faith::e_team_error) (int)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_team_error_to_session'", NULL);
#endif
  {
   self->send_team_error_to_session(session,err_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_team_error_to_session'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: player_stop_auto_match_team of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_player_stop_auto_match_team00
static int tolua_logic_hld_team_ws_mgr_player_stop_auto_match_team00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws_mgr* self = (faith::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64* player_guid = ((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'player_stop_auto_match_team'", NULL);
#endif
  {
   self->player_stop_auto_match_team(*player_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'player_stop_auto_match_team'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_team_type_valid of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_is_team_type_valid00
static int tolua_logic_hld_team_ws_mgr_is_team_type_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws_mgr* self = (faith::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::e_team_type team_type = ((faith::e_team_type) (int)  tolua_tointeger(tolua_S,2,0));
  int32 sub_type_id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_team_type_valid'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_team_type_valid(team_type,sub_type_id);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_team_type_valid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_condition_check_to_cs of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_req_condition_check_to_cs00
static int tolua_logic_hld_team_ws_mgr_req_condition_check_to_cs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws_mgr* self = (faith::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  faith::e_team_common_check_invoker invoker = ((faith::e_team_common_check_invoker) (int)  tolua_tointeger(tolua_S,3,0));
  faith::e_team_type team_type = ((faith::e_team_type) (int)  tolua_tointeger(tolua_S,4,0));
  int32 team_sub_type_id = ((int32)  tolua_tointeger(tolua_S,5,0));
  faith::guid_64 team_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_condition_check_to_cs'", NULL);
#endif
  {
   self->req_condition_check_to_cs(session,invoker,team_type,team_sub_type_id,team_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_condition_check_to_cs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_no_team_mate_list of class  faith::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_req_no_team_mate_list00
static int tolua_logic_hld_team_ws_mgr_req_no_team_mate_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::team_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::team_ws_mgr* self = (faith::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  faith::e_team_req_mate_type mate_type = ((faith::e_team_req_mate_type) (int)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_no_team_mate_list'", NULL);
#endif
  {
   self->req_no_team_mate_list(session,mate_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_no_team_mate_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_map_template_id of class  faith::cs_map_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_system_get_map_template_id00
static int tolua_logic_hld_cs_map_system_get_map_template_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cs_map_system",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const faith::guid_64* map_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  {
   int32 tolua_ret = (int32)  faith::cs_map_system::get_map_template_id(*map_guid);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_map_template_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_map_error_to_client of class  faith::cs_map_mgr_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_mgr_system_send_map_error_to_client00
static int tolua_logic_hld_cs_map_mgr_system_send_map_error_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cs_map_mgr_system",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"e_error_code",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  e_error_code error_code = *((e_error_code*)  tolua_tousertype(tolua_S,3,0));
  {
   faith::cs_map_mgr_system::send_map_error_to_client(session,error_code);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_map_error_to_client'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transfer_to_map of class  faith::cs_map_mgr_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_mgr_system_transfer_to_map00
static int tolua_logic_hld_cs_map_mgr_system_transfer_to_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cs_map_mgr_system",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 map_template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  faith::guid_64 map_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,4,0));
  int32 war_index = ((int32)  tolua_tointeger(tolua_S,5,0));
  int32 server_id = ((int32)  tolua_tointeger(tolua_S,6,0));
  {
   faith::cs_map_mgr_system::transfer_to_map(session,map_template_id,map_guid,war_index,server_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transfer_to_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_big_player of class  faith::cs_map_mgr_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_mgr_system_send_big_player00
static int tolua_logic_hld_cs_map_mgr_system_send_big_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cs_map_mgr_system",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"s_fake_player_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int32 map_template_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  s_fake_player_info* fake_player_info = ((s_fake_player_info*)  tolua_tousertype(tolua_S,3,0));
  int32 big_type = ((int32)  tolua_tointeger(tolua_S,4,0));
  {
   faith::cs_map_mgr_system::send_big_player(map_template_id,*fake_player_info,big_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_big_player'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_get_instance00
static int tolua_logic_hld_pk_king_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::pk_king_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::pk_king_mgr& tolua_ret = (faith::pk_king_mgr&)  faith::pk_king_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::pk_king_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_join of class  faith::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_is_can_join00
static int tolua_logic_hld_pk_king_mgr_is_can_join00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::pk_king_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::pk_king_mgr* self = (faith::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_can_join'", NULL);
#endif
  {
   e_error_code tolua_ret = (e_error_code)  self->is_can_join(role_guid);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((e_error_code)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(e_error_code));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_join'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_in_ready_time of class  faith::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_is_in_ready_time00
static int tolua_logic_hld_pk_king_mgr_is_in_ready_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::pk_king_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::pk_king_mgr* self = (faith::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_in_ready_time'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_in_ready_time();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_in_ready_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_in_notice_time of class  faith::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_is_in_notice_time00
static int tolua_logic_hld_pk_king_mgr_is_in_notice_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::pk_king_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::pk_king_mgr* self = (faith::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_in_notice_time'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_in_notice_time();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_in_notice_time'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_valid_pkking_map of class  faith::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_get_valid_pkking_map00
static int tolua_logic_hld_pk_king_mgr_get_valid_pkking_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::pk_king_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::pk_king_mgr* self = (faith::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 map_war_idx = ((int32)  tolua_tointeger(tolua_S,2,0));
  faith::guid_64* map_guid = ((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_valid_pkking_map'", NULL);
#endif
  {
   self->get_valid_pkking_map(map_war_idx,*map_guid);
   tolua_pushinteger(tolua_S,(lua_Integer)map_war_idx);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_valid_pkking_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_player_num_in_map_to_client of class  faith::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_send_player_num_in_map_to_client00
static int tolua_logic_hld_pk_king_mgr_send_player_num_in_map_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::pk_king_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::pk_king_mgr* self = (faith::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_player_num_in_map_to_client'", NULL);
#endif
  {
   self->send_player_num_in_map_to_client(role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_player_num_in_map_to_client'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: break_line_login_msg of class  faith::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_break_line_login_msg00
static int tolua_logic_hld_pk_king_mgr_break_line_login_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::pk_king_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::pk_king_mgr* self = (faith::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_login = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'break_line_login_msg'", NULL);
#endif
  {
   self->break_line_login_msg(role_guid,is_login);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'break_line_login_msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_pk_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::cross_server_pk_ws_mgr& tolua_ret = (faith::cross_server_pk_ws_mgr&)  faith::cross_server_pk_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::cross_server_pk_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_join_with_cross of class  faith::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_is_can_join_with_cross00
static int tolua_logic_hld_cross_server_pk_ws_mgr_is_can_join_with_cross00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_pk_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_pk_ws_mgr* self = (faith::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_can_join_with_cross'", NULL);
#endif
  {
   e_error_code tolua_ret = (e_error_code)  self->is_can_join_with_cross(role_guid);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((e_error_code)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(e_error_code));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_join_with_cross'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_to_gate_transfer_map of class  faith::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_send_to_gate_transfer_map00
static int tolua_logic_hld_cross_server_pk_ws_mgr_send_to_gate_transfer_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_pk_ws_mgr* self = (faith::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 class_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int32 activity_type = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 server_id = ((int32)  tolua_tointeger(tolua_S,5,0));
  faith::guid_64 legion_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_to_gate_transfer_map'", NULL);
#endif
  {
   self->send_to_gate_transfer_map(class_type,role_guid,activity_type,server_id,legion_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_to_gate_transfer_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_pk_king_map_guid of class  faith::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_get_pk_king_map_guid00
static int tolua_logic_hld_cross_server_pk_ws_mgr_get_pk_king_map_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_pk_ws_mgr* self = (faith::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 class_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  faith::guid_64* map_guid = ((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int32 war_idex = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_pk_king_map_guid'", NULL);
#endif
  {
   self->get_pk_king_map_guid(class_type,*map_guid,war_idex);
   tolua_pushinteger(tolua_S,(lua_Integer)war_idex);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_pk_king_map_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_map_temp_id_with_class_type of class  faith::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_get_map_temp_id_with_class_type00
static int tolua_logic_hld_cross_server_pk_ws_mgr_get_map_temp_id_with_class_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_pk_ws_mgr* self = (faith::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 class_type = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_map_temp_id_with_class_type'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_map_temp_id_with_class_type(class_type);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_map_temp_id_with_class_type'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_get_instance00
static int tolua_logic_hld_crystal_fairyland_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::crystal_fairyland_mgr& tolua_ret = (faith::crystal_fairyland_mgr&)  faith::crystal_fairyland_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::crystal_fairyland_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_activity_open of class  faith::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_is_activity_open00
static int tolua_logic_hld_crystal_fairyland_mgr_is_activity_open00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::crystal_fairyland_mgr* self = (faith::crystal_fairyland_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_activity_open'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_activity_open();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_activity_open'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_join of class  faith::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_is_can_join00
static int tolua_logic_hld_crystal_fairyland_mgr_is_can_join00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::crystal_fairyland_mgr* self = (faith::crystal_fairyland_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_can_join'", NULL);
#endif
  {
   e_error_code tolua_ret = (e_error_code)  self->is_can_join();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((e_error_code)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(e_error_code));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_join'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_valid_shui_jing_map of class  faith::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_get_valid_shui_jing_map00
static int tolua_logic_hld_crystal_fairyland_mgr_get_valid_shui_jing_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::crystal_fairyland_mgr* self = (faith::crystal_fairyland_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 map_war_idx = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_valid_shui_jing_map'", NULL);
#endif
  {
   faith::guid_64 tolua_ret = (faith::guid_64)  self->get_valid_shui_jing_map(map_war_idx);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((faith::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(faith::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
   tolua_pushinteger(tolua_S,(lua_Integer)map_war_idx);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_valid_shui_jing_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::broken_sky_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_broken_sky_mgr_get_instance00
static int tolua_logic_hld_broken_sky_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::broken_sky_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::broken_sky_mgr& tolua_ret = (faith::broken_sky_mgr&)  faith::broken_sky_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::broken_sky_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_join of class  faith::broken_sky_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_broken_sky_mgr_is_can_join00
static int tolua_logic_hld_broken_sky_mgr_is_can_join00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::broken_sky_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::broken_sky_mgr* self = (faith::broken_sky_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_can_join'", NULL);
#endif
  {
   e_error_code tolua_ret = (e_error_code)  self->is_can_join();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((e_error_code)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(e_error_code));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_join'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_get_instance00
static int tolua_logic_hld_world_boss_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::world_boss_ws_mgr& tolua_ret = (faith::world_boss_ws_mgr&)  faith::world_boss_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::world_boss_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_frist_kill_world_boss_prize of class  faith::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_req_frist_kill_world_boss_prize00
static int tolua_logic_hld_world_boss_ws_mgr_req_frist_kill_world_boss_prize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_boss_ws_mgr* self = (faith::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* req_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 spawn_npc_template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_frist_kill_world_boss_prize'", NULL);
#endif
  {
   self->req_frist_kill_world_boss_prize(*req_guid,spawn_npc_template_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_frist_kill_world_boss_prize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_damage_list of class  faith::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_send_damage_list00
static int tolua_logic_hld_world_boss_ws_mgr_send_damage_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_boss_ws_mgr* self = (faith::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 boss_id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_damage_list'", NULL);
#endif
  {
   self->send_damage_list(session,boss_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_damage_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_strategy_world_boss_info of class  faith::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_send_strategy_world_boss_info00
static int tolua_logic_hld_world_boss_ws_mgr_send_strategy_world_boss_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_boss_ws_mgr* self = (faith::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* legion_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_strategy_world_boss_info'", NULL);
#endif
  {
   self->send_strategy_world_boss_info(*legion_guid,*role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_strategy_world_boss_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_world_boss_info of class  faith::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_send_world_boss_info00
static int tolua_logic_hld_world_boss_ws_mgr_send_world_boss_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_boss_ws_mgr* self = (faith::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* legion_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int32 cur_boss_level = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_world_boss_info'", NULL);
#endif
  {
   self->send_world_boss_info(*legion_guid,*role_guid,cur_boss_level);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_world_boss_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::cross_server_harry_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_harry_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_harry_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cross_server_harry_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::cross_server_harry_ws_mgr& tolua_ret = (faith::cross_server_harry_ws_mgr&)  faith::cross_server_harry_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::cross_server_harry_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_join_harry_map of class  faith::cross_server_harry_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_harry_ws_mgr_is_can_join_harry_map00
static int tolua_logic_hld_cross_server_harry_ws_mgr_is_can_join_harry_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_harry_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_harry_ws_mgr* self = (faith::cross_server_harry_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session_ptr = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_can_join_harry_map'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_can_join_harry_map(session_ptr);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_join_harry_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_valid_harry_map of class  faith::cross_server_harry_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_harry_ws_mgr_get_valid_harry_map00
static int tolua_logic_hld_cross_server_harry_ws_mgr_get_valid_harry_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_harry_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_harry_ws_mgr* self = (faith::cross_server_harry_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64* map_guid = ((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 war_idex = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_valid_harry_map'", NULL);
#endif
  {
   self->get_valid_harry_map(*map_guid,war_idex);
   tolua_pushinteger(tolua_S,(lua_Integer)war_idex);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_valid_harry_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::web_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_web_client_get_instance00
static int tolua_logic_hld_web_client_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::web_client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::web_client& tolua_ret = (faith::web_client&)  faith::web_client::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::web_client");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_activate_code of class  faith::web_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_web_client_send_activate_code00
static int tolua_logic_hld_web_client_send_activate_code00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::web_client",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::web_client* self = (faith::web_client*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  std::string activate_code = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  int32 login_type = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_activate_code'", NULL);
#endif
  {
   self->send_activate_code(session,activate_code,login_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_activate_code'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_relation of class  faith::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_del_relation00
static int tolua_logic_hld_relation_mgr_del_relation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::relation_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::relation_mgr* self = (faith::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* unit_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  faith::e_relationlist_type type = ((faith::e_relationlist_type) (int)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_relation'", NULL);
#endif
  {
   self->del_relation(*unit_guid,type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_relation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: find_player_to_add_friend of class  faith::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_find_player_to_add_friend00
static int tolua_logic_hld_relation_mgr_find_player_to_add_friend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::relation_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::relation_mgr* self = (faith::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'find_player_to_add_friend'", NULL);
#endif
  {
   self->find_player_to_add_friend();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_player_to_add_friend'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: find_player_to_add_friend_by_pattern of class  faith::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_find_player_to_add_friend_by_pattern00
static int tolua_logic_hld_relation_mgr_find_player_to_add_friend_by_pattern00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::relation_mgr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::relation_mgr* self = (faith::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
  const string pattern = ((const string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'find_player_to_add_friend_by_pattern'", NULL);
#endif
  {
   self->find_player_to_add_friend_by_pattern(pattern);
   tolua_pushcppstring(tolua_S,(const char*)pattern);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'find_player_to_add_friend_by_pattern'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_relation of class  faith::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_add_relation00
static int tolua_logic_hld_relation_mgr_add_relation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::relation_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::relation_mgr* self = (faith::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::e_relationlist_type relation_type = ((faith::e_relationlist_type) (int)  tolua_tointeger(tolua_S,2,0));
  const faith::guid_64* guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_relation'", NULL);
#endif
  {
   self->add_relation(relation_type,*guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_relation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: del_relation_to_dp of class  faith::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_del_relation_to_dp00
static int tolua_logic_hld_relation_mgr_del_relation_to_dp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::relation_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::relation_mgr* self = (faith::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  faith::guid_64 target_role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int32 relation_type = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'del_relation_to_dp'", NULL);
#endif
  {
   self->del_relation_to_dp(role_guid,target_role_guid,relation_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_relation_to_dp'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_get_instance00
static int tolua_logic_hld_marry_mgr_ws_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::marry_mgr_ws& tolua_ret = (faith::marry_mgr_ws&)  faith::marry_mgr_ws::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::marry_mgr_ws");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_get_married_reward of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_get_married_reward00
static int tolua_logic_hld_marry_mgr_ws_c2ws_get_married_reward00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::marry_mgr_ws* self = (faith::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c2ws_get_married_reward'", NULL);
#endif
  {
   self->c2ws_get_married_reward(*role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_get_married_reward'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_get_marry_mission of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_get_marry_mission00
static int tolua_logic_hld_marry_mgr_ws_c2ws_get_marry_mission00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::marry_mgr_ws* self = (faith::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 is_near = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c2ws_get_marry_mission'", NULL);
#endif
  {
   self->c2ws_get_marry_mission(*role_guid,is_near);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_get_marry_mission'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_check_marry_with_sb of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_req_check_marry_with_sb00
static int tolua_logic_hld_marry_mgr_ws_req_check_marry_with_sb00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::marry_mgr_ws* self = (faith::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* main_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const faith::guid_64* sub_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int32 marry_item = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_check_marry_with_sb'", NULL);
#endif
  {
   self->req_check_marry_with_sb(*main_guid,*sub_guid,marry_item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_check_marry_with_sb'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_reply_marry_choice of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_reply_marry_choice00
static int tolua_logic_hld_marry_mgr_ws_c2ws_reply_marry_choice00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::marry_mgr_ws* self = (faith::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_agree = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c2ws_reply_marry_choice'", NULL);
#endif
  {
   self->c2ws_reply_marry_choice(*role_guid,is_agree);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_reply_marry_choice'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_req_check_divorce of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_req_check_divorce00
static int tolua_logic_hld_marry_mgr_ws_c2ws_req_check_divorce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::marry_mgr_ws* self = (faith::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* main_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const faith::guid_64* sub_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  bool is_force = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c2ws_req_check_divorce'", NULL);
#endif
  {
   self->c2ws_req_check_divorce(*main_guid,*sub_guid,is_force);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_req_check_divorce'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_reply_divorce_choice of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_reply_divorce_choice00
static int tolua_logic_hld_marry_mgr_ws_c2ws_reply_divorce_choice00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::marry_mgr_ws* self = (faith::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_agree = ((bool)  tolua_toboolean(tolua_S,3,0));
  bool is_force = ((bool)  tolua_toboolean(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c2ws_reply_divorce_choice'", NULL);
#endif
  {
   self->c2ws_reply_divorce_choice(*role_guid,is_agree,is_force);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_reply_divorce_choice'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_bless_one_couple of class  faith::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_req_bless_one_couple00
static int tolua_logic_hld_marry_mgr_ws_req_bless_one_couple00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::marry_mgr_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::marry_mgr_ws* self = (faith::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* client_ptr = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  const faith::guid_64* target_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_bless_one_couple'", NULL);
#endif
  {
   self->req_bless_one_couple(client_ptr,*target_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_bless_one_couple'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_get_ranking_list_func of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_c2ws_get_ranking_list_func00
static int tolua_logic_hld_ranking_mgr_ws_c2ws_get_ranking_list_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::e_RankingIndex ranking_index = ((faith::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  {
   faith::ranking_mgr_ws::c2ws_get_ranking_list_func(ranking_index,*role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_get_ranking_list_func'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_get_my_rank_func of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_c2ws_get_my_rank_func00
static int tolua_logic_hld_ranking_mgr_ws_c2ws_get_my_rank_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::e_RankingIndex ranking_type = ((faith::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  const faith::guid_64* sender_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  const faith::guid_64* legion_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,4,0));
  {
   faith::ranking_mgr_ws::c2ws_get_my_rank_func(ranking_type,*sender_guid,*legion_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_get_my_rank_func'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c2ws_get_player_ranking_info_func of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_c2ws_get_player_ranking_info_func00
static int tolua_logic_hld_ranking_mgr_ws_c2ws_get_player_ranking_info_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::e_RankingIndex ranking_type = ((faith::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  const faith::guid_64* sender_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  const faith::guid_64* target_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,4,0));
  {
   faith::ranking_mgr_ws::c2ws_get_player_ranking_info_func(ranking_type,*sender_guid,*target_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c2ws_get_player_ranking_info_func'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_service_rank_first_player_to_client_by_type of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_send_service_rank_first_player_to_client_by_type00
static int tolua_logic_hld_ranking_mgr_ws_send_service_rank_first_player_to_client_by_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 send_type = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::ranking_mgr_ws::send_service_rank_first_player_to_client_by_type(session,send_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_service_rank_first_player_to_client_by_type'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_oracle_trial_rank_info of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_get_oracle_trial_rank_info00
static int tolua_logic_hld_ranking_mgr_ws_get_oracle_trial_rank_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::guid_64 play_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  {
   faith::ranking_mgr_ws::get_oracle_trial_rank_info(play_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_oracle_trial_rank_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sync_player_arena_lua of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_sync_player_arena_lua00
static int tolua_logic_hld_ranking_mgr_ws_sync_player_arena_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* my_str = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 my_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  const char* dst_str = ((const char*)  tolua_tostring(tolua_S,4,0));
  int32 dst_len = ((int32)  tolua_tointeger(tolua_S,5,0));
  bool is_robot = ((bool)  tolua_toboolean(tolua_S,6,0));
  {
   faith::ranking_mgr_ws::sync_player_arena_lua(my_str,my_len,dst_str,dst_len,is_robot);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sync_player_arena_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_srv_avg_level of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_get_srv_avg_level00
static int tolua_logic_hld_ranking_mgr_ws_get_srv_avg_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int32 tolua_ret = (int32)  faith::ranking_mgr_ws::get_srv_avg_level();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_srv_avg_level'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_ranking_player_guid of class  faith::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_get_ranking_player_guid00
static int tolua_logic_hld_ranking_mgr_ws_get_ranking_player_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::e_RankingIndex ranking_index = ((faith::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  int32 ranking_pos = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::guid_64 tolua_ret = (faith::guid_64)  faith::ranking_mgr_ws::get_ranking_player_guid(ranking_index,ranking_pos);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((faith::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(faith::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"faith::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_ranking_player_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::cloud_shop_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cloud_shop_mgr_get_instance00
static int tolua_logic_hld_cloud_shop_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cloud_shop_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::cloud_shop_mgr& tolua_ret = (faith::cloud_shop_mgr&)  faith::cloud_shop_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::cloud_shop_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: handler_client_request of class  faith::cloud_shop_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cloud_shop_mgr_handler_client_request00
static int tolua_logic_hld_cloud_shop_mgr_handler_client_request00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cloud_shop_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"e_client_req_type",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cloud_shop_mgr* self = (faith::cloud_shop_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* pSession = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  e_client_req_type req_type = *((e_client_req_type*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'handler_client_request'", NULL);
#endif
  {
   self->handler_client_request(pSession,req_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'handler_client_request'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::gain_treasure_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gain_treasure_ws_mgr_get_instance00
static int tolua_logic_hld_gain_treasure_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::gain_treasure_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::gain_treasure_ws_mgr& tolua_ret = (faith::gain_treasure_ws_mgr&)  faith::gain_treasure_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::gain_treasure_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_kill_record_end of class  faith::gain_treasure_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gain_treasure_ws_mgr_req_kill_record_end00
static int tolua_logic_hld_gain_treasure_ws_mgr_req_kill_record_end00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::gain_treasure_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::gain_treasure_ws_mgr* self = (faith::gain_treasure_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 boss_spawn_point_template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_kill_record_end'", NULL);
#endif
  {
   self->req_kill_record_end(session,boss_spawn_point_template_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_kill_record_end'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_gain_item_record of class  faith::gain_treasure_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gain_treasure_ws_mgr_req_gain_item_record00
static int tolua_logic_hld_gain_treasure_ws_mgr_req_gain_item_record00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::gain_treasure_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::gain_treasure_ws_mgr* self = (faith::gain_treasure_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 record_index = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_gain_item_record'", NULL);
#endif
  {
   self->req_gain_item_record(session,record_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_gain_item_record'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::time_limit_activity_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_time_limit_activity_ws_mgr_get_instance00
static int tolua_logic_hld_time_limit_activity_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::time_limit_activity_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::time_limit_activity_ws_mgr& tolua_ret = (faith::time_limit_activity_ws_mgr&)  faith::time_limit_activity_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::time_limit_activity_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_rank_info of class  faith::time_limit_activity_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_time_limit_activity_ws_mgr_get_rank_info00
static int tolua_logic_hld_time_limit_activity_ws_mgr_get_rank_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::time_limit_activity_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::time_limit_activity_ws_mgr* self = (faith::time_limit_activity_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::e_time_limit_activity_type activity_type = ((faith::e_time_limit_activity_type) (int)  tolua_tointeger(tolua_S,2,0));
  faith::guid_64 role_uid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int32 source_server_id = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_rank_info'", NULL);
#endif
  {
   self->get_rank_info(activity_type,role_uid,source_server_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_rank_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_activity_common_data_to_gate of class  faith::time_limit_activity_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_time_limit_activity_ws_mgr_get_activity_common_data_to_gate00
static int tolua_logic_hld_time_limit_activity_ws_mgr_get_activity_common_data_to_gate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::time_limit_activity_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::time_limit_activity_ws_mgr* self = (faith::time_limit_activity_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session_ptr = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_activity_common_data_to_gate'", NULL);
#endif
  {
   self->get_activity_common_data_to_gate(session_ptr);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_activity_common_data_to_gate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::rename_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_rename_mgr_get_instance00
static int tolua_logic_hld_rename_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::rename_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::rename_mgr& tolua_ret = (faith::rename_mgr&)  faith::rename_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::rename_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: change_player_name of class  faith::rename_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_rename_mgr_change_player_name00
static int tolua_logic_hld_rename_mgr_change_player_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::rename_mgr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::rename_mgr* self = (faith::rename_mgr*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'change_player_name'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->change_player_name(name,role_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'change_player_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: confirm_change_name of class  faith::rename_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_rename_mgr_confirm_change_name00
static int tolua_logic_hld_rename_mgr_confirm_change_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::rename_mgr",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::rename_mgr* self = (faith::rename_mgr*)  tolua_tousertype(tolua_S,1,0);
  bool confirm_type = ((bool)  tolua_toboolean(tolua_S,2,0));
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
  const string role_name = ((const string)  tolua_tocppstring(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'confirm_change_name'", NULL);
#endif
  {
   self->confirm_change_name(confirm_type,role_guid,role_name);
   tolua_pushcppstring(tolua_S,(const char*)role_name);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'confirm_change_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::cross_server_world_boss_ws_mgr& tolua_ret = (faith::cross_server_world_boss_ws_mgr&)  faith::cross_server_world_boss_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::cross_server_world_boss_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_in_cross_boss_map of class  faith::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_is_in_cross_boss_map00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_is_in_cross_boss_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_world_boss_ws_mgr* self = (faith::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 map_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_in_cross_boss_map'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_in_cross_boss_map(map_guid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_in_cross_boss_map'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_can_join_with_cross of class  faith::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_is_can_join_with_cross00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_is_can_join_with_cross00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_world_boss_ws_mgr* self = (faith::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 map_type = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_can_join_with_cross'", NULL);
#endif
  {
   e_error_code tolua_ret = (e_error_code)  self->is_can_join_with_cross(role_guid,map_type);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((e_error_code)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(e_error_code));
     tolua_pushusertype(tolua_S,tolua_obj,"e_error_code");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_can_join_with_cross'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_world_boss_map_guid of class  faith::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_get_world_boss_map_guid00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_get_world_boss_map_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_world_boss_ws_mgr* self = (faith::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64* map_guid = ((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 war_idex = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 map_type = ((int32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_world_boss_map_guid'", NULL);
#endif
  {
   self->get_world_boss_map_guid(*map_guid,war_idex,map_type);
   tolua_pushinteger(tolua_S,(lua_Integer)war_idex);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_world_boss_map_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_cross_server_world_boss_map_temp_id of class  faith::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_get_cross_server_world_boss_map_temp_id00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_get_cross_server_world_boss_map_temp_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_world_boss_ws_mgr* self = (faith::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 map_type = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_cross_server_world_boss_map_temp_id'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_cross_server_world_boss_map_temp_id(map_type);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_cross_server_world_boss_map_temp_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_kill_boss_show of class  faith::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_send_kill_boss_show00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_send_kill_boss_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::cross_server_world_boss_ws_mgr* self = (faith::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_kill_boss_show'", NULL);
#endif
  {
   self->send_kill_boss_show(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_kill_boss_show'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_get_instance00
static int tolua_logic_hld_assist_fight_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::assist_fight_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::assist_fight_mgr& tolua_ret = (faith::assist_fight_mgr&)  faith::assist_fight_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::assist_fight_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sync_assist_fight_info of class  faith::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info00
static int tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::assist_fight_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::assist_fight_mgr* self = (faith::assist_fight_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_login = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sync_assist_fight_info'", NULL);
#endif
  {
   self->sync_assist_fight_info(role_guid,is_login);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sync_assist_fight_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sync_assist_fight_info of class  faith::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info01
static int tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::assist_fight_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  faith::assist_fight_mgr* self = (faith::assist_fight_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_login = ((bool)  tolua_toboolean(tolua_S,3,0));
  faith::guid_64 create_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'sync_assist_fight_info'", NULL);
#endif
  {
   self->sync_assist_fight_info(role_guid,is_login,create_guid);
  }
 }
 return 0;
tolua_lerror:
 return tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_assist_fight of class  faith::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_req_assist_fight00
static int tolua_logic_hld_assist_fight_mgr_req_assist_fight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::assist_fight_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::assist_fight_mgr* self = (faith::assist_fight_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  faith::guid_64 assist_fight_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_assist_fight'", NULL);
#endif
  {
   self->req_assist_fight(role_guid,assist_fight_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_assist_fight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::appearance_info_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_appearance_info_mgr_get_instance00
static int tolua_logic_hld_appearance_info_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::appearance_info_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::appearance_info_mgr& tolua_ret = (faith::appearance_info_mgr&)  faith::appearance_info_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::appearance_info_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_save_appearance_info_to_id of class  faith::appearance_info_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_appearance_info_mgr_req_save_appearance_info_to_id00
static int tolua_logic_hld_appearance_info_mgr_req_save_appearance_info_to_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::appearance_info_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::s_client_uid",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::appearance_info_mgr* self = (faith::appearance_info_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::s_client_uid client_uid = *((faith::s_client_uid*)  tolua_tousertype(tolua_S,2,0));
  const std::string appearance_info = ((const std::string)  tolua_tocppstring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_save_appearance_info_to_id'", NULL);
#endif
  {
   self->req_save_appearance_info_to_id(client_uid,appearance_info);
   tolua_pushcppstring(tolua_S,(const char*)appearance_info);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_save_appearance_info_to_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: req_load_appearance_info_by_id of class  faith::appearance_info_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_appearance_info_mgr_req_load_appearance_info_by_id00
static int tolua_logic_hld_appearance_info_mgr_req_load_appearance_info_by_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::appearance_info_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::s_client_uid",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::appearance_info_mgr* self = (faith::appearance_info_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::s_client_uid client_uid = *((faith::s_client_uid*)  tolua_tousertype(tolua_S,2,0));
  int32 id = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'req_load_appearance_info_by_id'", NULL);
#endif
  {
   self->req_load_appearance_info_by_id(client_uid,id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'req_load_appearance_info_by_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_get_instance00
static int tolua_logic_hld_element_war_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::element_war_ws_mgr& tolua_ret = (faith::element_war_ws_mgr&)  faith::element_war_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::element_war_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_req_element_war_role_info of class  faith::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_req_element_war_role_info00
static int tolua_logic_hld_element_war_ws_mgr_send_req_element_war_role_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::element_war_ws_mgr* self = (faith::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
  bool is_online = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_req_element_war_role_info'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->send_req_element_war_role_info(session,is_online);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_req_element_war_role_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_team_sign_up of class  faith::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_team_sign_up00
static int tolua_logic_hld_element_war_ws_mgr_send_team_sign_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::element_war_ws_mgr* self = (faith::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_team_sign_up'", NULL);
#endif
  {
   self->send_team_sign_up(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_team_sign_up'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_single_sign_up of class  faith::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_single_sign_up00
static int tolua_logic_hld_element_war_ws_mgr_send_single_sign_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::element_war_ws_mgr* self = (faith::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_single_sign_up'", NULL);
#endif
  {
   self->send_single_sign_up(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_single_sign_up'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_close_sign_up of class  faith::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_close_sign_up00
static int tolua_logic_hld_element_war_ws_mgr_send_close_sign_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::element_war_ws_mgr* self = (faith::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::client_session* session = ((faith::client_session*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_close_sign_up'", NULL);
#endif
  {
   self->send_close_sign_up(session);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_close_sign_up'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: break_line_login_msg of class  faith::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_break_line_login_msg00
static int tolua_logic_hld_element_war_ws_mgr_break_line_login_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::element_war_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::element_war_ws_mgr* self = (faith::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_login = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'break_line_login_msg'", NULL);
#endif
  {
   self->break_line_login_msg(role_guid,is_login);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'break_line_login_msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_mission_reward of class  faith::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_get_mission_reward00
static int tolua_logic_hld_element_war_ws_mgr_get_mission_reward00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::element_war_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::element_war_ws_mgr* self = (faith::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 mission_idex = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_mission_reward'", NULL);
#endif
  {
   self->get_mission_reward(role_guid,mission_idex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_mission_reward'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_need_begin_cross_gm_common of class  faith::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_get_need_begin_cross_gm_common00
static int tolua_logic_hld_world_server_get_need_begin_cross_gm_common00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_server",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_server* self = (faith::world_server*)  tolua_tousertype(tolua_S,1,0);
  faith::e_need_server_cross gm_type = ((faith::e_need_server_cross) (int)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_need_begin_cross_gm_common'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_need_begin_cross_gm_common(gm_type);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_need_begin_cross_gm_common'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_sky_island_server of class  faith::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_is_sky_island_server00
static int tolua_logic_hld_world_server_is_sky_island_server00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_server* self = (faith::world_server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'is_sky_island_server'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->is_sky_island_server();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'is_sky_island_server'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_server_id of class  faith::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_get_server_id00
static int tolua_logic_hld_world_server_get_server_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_server* self = (faith::world_server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_server_id'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_server_id();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_server_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_cross_id of class  faith::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_get_cross_id00
static int tolua_logic_hld_world_server_get_cross_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_server* self = (faith::world_server*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_cross_id'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_cross_id();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_cross_id'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_server_act_hidden of class  faith::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_set_server_act_hidden00
static int tolua_logic_hld_world_server_set_server_act_hidden00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_server",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_server* self = (faith::world_server*)  tolua_tousertype(tolua_S,1,0);
  bool is_need_hidden = ((bool)  tolua_toboolean(tolua_S,2,0));
  bool is_other_server = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_server_act_hidden'", NULL);
#endif
  {
   self->set_server_act_hidden(is_need_hidden,is_other_server);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_server_act_hidden'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_ws_loading_flag of class  faith::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_set_ws_loading_flag00
static int tolua_logic_hld_world_server_set_ws_loading_flag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_server",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_server* self = (faith::world_server*)  tolua_tousertype(tolua_S,1,0);
  int64 flag = ((int64)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_ws_loading_flag'", NULL);
#endif
  {
   self->set_ws_loading_flag(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_ws_loading_flag'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_to_gate_get_msg of class  faith::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_send_to_gate_get_msg00
static int tolua_logic_hld_world_server_send_to_gate_get_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::world_server",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::world_server* self = (faith::world_server*)  tolua_tousertype(tolua_S,1,0);
  int64 flag = ((int64)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_to_gate_get_msg'", NULL);
#endif
  {
   self->send_to_gate_get_msg(flag);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_to_gate_get_msg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_message_to_all_client_data_lua of class  faith::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_send_message_to_all_client_data_lua00
static int tolua_logic_hld_client_session_mgr_send_message_to_all_client_data_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session_mgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session_mgr* self = (faith::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  uint32 header = ((uint32)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_message_to_all_client_data_lua'", NULL);
#endif
  {
   self->send_message_to_all_client_data_lua(msg,msg_len,header);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_message_to_all_client_data_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_message_to_map_lua of class  faith::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_send_message_to_map_lua00
static int tolua_logic_hld_client_session_mgr_send_message_to_map_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session_mgr* self = (faith::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 map_template_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,3,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,4,0));
  uint32 header = ((uint32)  tolua_tointeger(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_message_to_map_lua'", NULL);
#endif
  {
   self->send_message_to_map_lua(map_template_id,msg,msg_len,header);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_message_to_map_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_session_array_used of class  faith::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_get_session_array_used00
static int tolua_logic_hld_client_session_mgr_get_session_array_used00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session_mgr* self = (faith::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_session_array_used'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_session_array_used();
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_session_array_used'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_session_by_use_index of class  faith::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_get_session_by_use_index00
static int tolua_logic_hld_client_session_mgr_get_session_by_use_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::client_session_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::client_session_mgr* self = (faith::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 index = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_session_by_use_index'", NULL);
#endif
  {
   faith::client_session* tolua_ret = (faith::client_session*)  self->get_session_by_use_index(index);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::client_session");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_session_by_use_index'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_instance00
static int tolua_logic_hld_event_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::event_ws_mgr& tolua_ret = (faith::event_ws_mgr&)  faith::event_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::event_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_notice_to_all of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_send_notice_to_all00
static int tolua_logic_hld_event_ws_mgr_send_notice_to_all00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"faith::guid_64",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 notice_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  int32 notice_create_time = ((int32)  tolua_tointeger(tolua_S,3,0));
  faith::guid_64 sender_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,4,0));
  std::string final_string = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  int32 sender_template_id = ((int32)  tolua_tointeger(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_notice_to_all'", NULL);
#endif
  {
   self->send_notice_to_all(notice_id,notice_create_time,sender_guid,final_string,sender_template_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_notice_to_all'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_mail_to_player_by_system of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_send_mail_to_player_by_system00
static int tolua_logic_hld_event_ws_mgr_send_mail_to_player_by_system00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,11,&tolua_err) || !tolua_isusertype(tolua_S,11,"const std::vector<faith::s_item_info>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,12,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 terget_player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 target_server_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  std::string sender_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string title = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string content_text = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  int32 money_typ1 = ((int32)  tolua_tointeger(tolua_S,7,0));
  int32 money_num1 = ((int32)  tolua_tointeger(tolua_S,8,0));
  int32 money_typ2 = ((int32)  tolua_tointeger(tolua_S,9,0));
  int32 money_num2 = ((int32)  tolua_tointeger(tolua_S,10,0));
  const std::vector<faith::s_item_info>* item_list = ((const std::vector<faith::s_item_info>*)  tolua_tousertype(tolua_S,11,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_mail_to_player_by_system'", NULL);
#endif
  {
   self->send_mail_to_player_by_system(terget_player_guid,target_server_id,sender_name,title,content_text,money_typ1,money_num1,money_typ2,money_num2,*item_list);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_mail_to_player_by_system'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_notice_with_param of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_send_notice_with_param00
static int tolua_logic_hld_event_ws_mgr_send_notice_with_param00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const std::vector<int32>",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 sender_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const char* sender_name = ((const char*)  tolua_tostring(tolua_S,3,0));
  const char* content_text = ((const char*)  tolua_tostring(tolua_S,4,0));
  const std::vector<int32>* param_array = ((const std::vector<int32>*)  tolua_tousertype(tolua_S,5,0));
  int32 param_type = ((int32)  tolua_tointeger(tolua_S,6,0));
  int32 chat_type = ((int32)  tolua_tointeger(tolua_S,7,0));
  int32 sender_template_id = ((int32)  tolua_tointeger(tolua_S,8,0));
  int32 notice_id = ((int32)  tolua_tointeger(tolua_S,9,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_notice_with_param'", NULL);
#endif
  {
   self->send_notice_with_param(sender_guid,sender_name,content_text,*param_array,param_type,chat_type,sender_template_id,notice_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_notice_with_param'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_free_item of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_free_item00
static int tolua_logic_hld_event_ws_mgr_get_free_item00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_free_item'", NULL);
#endif
  {
   faith::s_item_info* tolua_ret = (faith::s_item_info*)  self->get_free_item();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::s_item_info");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_free_item'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_free_list of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_free_list00
static int tolua_logic_hld_event_ws_mgr_get_free_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_free_list'", NULL);
#endif
  {
   std::vector<faith::s_item_info>* tolua_ret = (std::vector<faith::s_item_info>*)  self->get_free_list();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<faith::s_item_info>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_free_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_free_item of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_add_free_item00
static int tolua_logic_hld_event_ws_mgr_add_free_item00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"faith::s_item_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::s_item_info* free_item = ((faith::s_item_info*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_free_item'", NULL);
#endif
  {
   self->add_free_item(free_item);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_free_item'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_free_param of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_free_param00
static int tolua_logic_hld_event_ws_mgr_get_free_param00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_free_param'", NULL);
#endif
  {
   std::vector<int32>* tolua_ret = (std::vector<int32>*)  self->get_free_param();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<int32>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_free_param'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_free_param of class  faith::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_add_free_param00
static int tolua_logic_hld_event_ws_mgr_add_free_param00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::event_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::event_ws_mgr* self = (faith::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 value = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_free_param'", NULL);
#endif
  {
   self->add_free_param(value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_free_param'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_random of class  faith::random_gen */
#ifndef TOLUA_DISABLE_tolua_logic_hld_random_gen_get_random00
static int tolua_logic_hld_random_gen_get_random00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::random_gen",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int nmin = ((int)  tolua_tointeger(tolua_S,2,0));
  int nmax = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   int tolua_ret = (int)  faith::random_gen::get_random(nmin,nmax);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_random'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::big_player_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_big_player_ws_mgr_get_instance00
static int tolua_logic_hld_big_player_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::big_player_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::big_player_ws_mgr& tolua_ret = (faith::big_player_ws_mgr&)  faith::big_player_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::big_player_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set_big_player_guid of class  faith::big_player_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_big_player_ws_mgr_set_big_player_guid00
static int tolua_logic_hld_big_player_ws_mgr_set_big_player_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::big_player_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::big_player_ws_mgr* self = (faith::big_player_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::e_big_player_type big_type = ((faith::e_big_player_type) (int)  tolua_tointeger(tolua_S,2,0));
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set_big_player_guid'", NULL);
#endif
  {
   self->set_big_player_guid(big_type,player_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set_big_player_guid'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_big_player_type_with_ranking_type of class  faith::big_player_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_big_player_ws_mgr_get_big_player_type_with_ranking_type00
static int tolua_logic_hld_big_player_ws_mgr_get_big_player_type_with_ranking_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::big_player_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::big_player_ws_mgr* self = (faith::big_player_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 ranking_type = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_big_player_type_with_ranking_type'", NULL);
#endif
  {
   int32 tolua_ret = (int32)  self->get_big_player_type_with_ranking_type(ranking_type);
   tolua_pushinteger(tolua_S,(lua_Integer)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_big_player_type_with_ranking_type'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_send_red_package_process */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_send_red_package_process00
static int tolua_logic_hld_lua_cs2ws_send_red_package_process00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_send_red_package_process(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_send_red_package_process'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_get_red_package_process */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_get_red_package_process00
static int tolua_logic_hld_lua_cs2ws_get_red_package_process00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_get_red_package_process(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_get_red_package_process'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_get_arena_rank */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_get_arena_rank00
static int tolua_logic_hld_lua_cs2ws_get_arena_rank00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_get_arena_rank(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_get_arena_rank'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_req_challenge */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_req_challenge00
static int tolua_logic_hld_lua_cs2ws_req_challenge00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_req_challenge(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_req_challenge'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_req_get_choose_list */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_req_get_choose_list00
static int tolua_logic_hld_lua_cs2ws_req_get_choose_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_req_get_choose_list(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_req_get_choose_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_req_get_first_three */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_req_get_first_three00
static int tolua_logic_hld_lua_cs2ws_req_get_first_three00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_req_get_first_three(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_req_get_first_three'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_chellenge_over_req */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_chellenge_over_req00
static int tolua_logic_hld_lua_cs2ws_chellenge_over_req00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_chellenge_over_req(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_chellenge_over_req'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::lua_cs2ws_arena_sync_gs_value_proc */
#ifndef TOLUA_DISABLE_tolua_logic_hld_lua_cs2ws_arena_sync_gs_value_proc00
static int tolua_logic_hld_lua_cs2ws_arena_sync_gs_value_proc00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  uint32 conn_index = ((uint32)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 data_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   faith::lua_cs2ws_arena_sync_gs_value_proc(conn_index,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'lua_cs2ws_arena_sync_gs_value_proc'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_instance of class  faith::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_get_instance00
static int tolua_logic_hld_mail_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::mail_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::mail_ws_mgr& tolua_ret = (faith::mail_ws_mgr&)  faith::mail_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::mail_ws_mgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_instance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_mail of class  faith::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_send_mail00
static int tolua_logic_hld_mail_ws_mgr_send_mail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::mail_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const s_mail_info",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,5,"const faith::s_item_info",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::mail_ws_mgr* self = (faith::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 address_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 address_server_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  const s_mail_info* mail_info = ((const s_mail_info*)  tolua_tousertype(tolua_S,4,0));
  const faith::s_item_info* item_list = ((const faith::s_item_info*)  tolua_tousertype(tolua_S,5,0));
  int32 item_num = ((int32)  tolua_tointeger(tolua_S,6,0));
  bool is_only_to_sql = ((bool)  tolua_toboolean(tolua_S,7,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_mail'", NULL);
#endif
  {
   self->send_mail(address_guid,address_server_id,*mail_info,item_list,item_num,is_only_to_sql);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_mail'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: send_mail_system of class  faith::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_send_mail_system00
static int tolua_logic_hld_mail_ws_mgr_send_mail_system00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::mail_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const std::vector<s_item_template_info>",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::mail_ws_mgr* self = (faith::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  faith::guid_64 player_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 player_server_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  const std::vector<s_item_template_info>* item_list = ((const std::vector<s_item_template_info>*)  tolua_tousertype(tolua_S,4,0));
  std::string title = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string contentText = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  int32 money_typ1 = ((int32)  tolua_tointeger(tolua_S,7,0));
  int32 money_num1 = ((int32)  tolua_tointeger(tolua_S,8,0));
  int32 money_typ2 = ((int32)  tolua_tointeger(tolua_S,9,0));
  int32 money_num2 = ((int32)  tolua_tointeger(tolua_S,10,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_mail_system'", NULL);
#endif
  {
   self->send_mail_system(player_guid,player_server_id,*item_list,title,contentText,money_typ1,money_num1,money_typ2,money_num2);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_mail_system'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_free_item_list of class  faith::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_get_free_item_list00
static int tolua_logic_hld_mail_ws_mgr_get_free_item_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::mail_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::mail_ws_mgr* self = (faith::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_free_item_list'", NULL);
#endif
  {
   std::vector<s_item_template_info>* tolua_ret = (std::vector<s_item_template_info>*)  self->get_free_item_list();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<s_item_template_info>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_free_item_list'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_free_item of class  faith::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_add_free_item00
static int tolua_logic_hld_mail_ws_mgr_add_free_item00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::mail_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::mail_ws_mgr* self = (faith::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 item_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  int32 item_num = ((int32)  tolua_tointeger(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_free_item'", NULL);
#endif
  {
   self->add_free_item(item_id,item_num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_free_item'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: auction_sell_log of class  faith::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_auction_sell_log00
static int tolua_logic_hld_server_log_auction_sell_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::server_log",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int32 item_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  int32 item_num = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 money_type = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 item_price = ((int32)  tolua_tointeger(tolua_S,5,0));
  {
   faith::server_log::auction_sell_log(item_id,item_num,money_type,item_price);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_sell_log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: auction_db_log of class  faith::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_auction_db_log00
static int tolua_logic_hld_server_log_auction_db_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::server_log",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const s_auction_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const s_unit_info",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const s_auction_info* auction_info = ((const s_auction_info*)  tolua_tousertype(tolua_S,2,0));
  int32 auction_state = ((int32)  tolua_tointeger(tolua_S,3,0));
  const s_unit_info* role_info = ((const s_unit_info*)  tolua_tousertype(tolua_S,4,0));
  const login_fixed_data* third_info = ((const login_fixed_data*)  tolua_tousertype(tolua_S,5,0));
  const int32 login_type = ((const int32)  tolua_tointeger(tolua_S,6,0));
  {
   faith::server_log::auction_db_log(*auction_info,auction_state,*role_info,*third_info,login_type);
   tolua_pushinteger(tolua_S,(lua_Integer)login_type);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_db_log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: auctionbid_log of class  faith::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_auctionbid_log00
static int tolua_logic_hld_server_log_auctionbid_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::server_log",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"login_fixed_data",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"s_unit_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,8,&tolua_err) || !tolua_isusertype(tolua_S,8,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  login_fixed_data* third_info = ((login_fixed_data*)  tolua_tousertype(tolua_S,2,0));
  s_unit_info* role_info = ((s_unit_info*)  tolua_tousertype(tolua_S,3,0));
  const int32 money_type = ((const int32)  tolua_tointeger(tolua_S,4,0));
  const int32 money_value = ((const int32)  tolua_tointeger(tolua_S,5,0));
  const int32 item_template_id = ((const int32)  tolua_tointeger(tolua_S,6,0));
  const int32 data_num = ((const int32)  tolua_tointeger(tolua_S,7,0));
  const faith::guid_64* buyer_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,8,0));
  {
   faith::server_log::auctionbid_log(*third_info,*role_info,money_type,money_value,item_template_id,data_num,*buyer_guid);
   tolua_pushinteger(tolua_S,(lua_Integer)money_type);
   tolua_pushinteger(tolua_S,(lua_Integer)money_value);
   tolua_pushinteger(tolua_S,(lua_Integer)item_template_id);
   tolua_pushinteger(tolua_S,(lua_Integer)data_num);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auctionbid_log'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: serverAuction of class  faith::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_serverAuction00
static int tolua_logic_hld_server_log_serverAuction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::server_log",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"s_log_common_head",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,11,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,12,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,13,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,14,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,15,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,16,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,17,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,18,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,19,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s_log_common_head* lg_common_head = ((s_log_common_head*)  tolua_tousertype(tolua_S,2,0));
  string sellerAccountId = ((string)  tolua_tocppstring(tolua_S,3,0));
  string sellerRoleGuid = ((string)  tolua_tocppstring(tolua_S,4,0));
  string sellerRoleName = ((string)  tolua_tocppstring(tolua_S,5,0));
  int32 sellerRoleLevel = ((int32)  tolua_tointeger(tolua_S,6,0));
  string itemGuid = ((string)  tolua_tocppstring(tolua_S,7,0));
  int32 itemId = ((int32)  tolua_tointeger(tolua_S,8,0));
  int32 itemNum = ((int32)  tolua_tointeger(tolua_S,9,0));
  int32 auctionType = ((int32)  tolua_tointeger(tolua_S,10,0));
  string buyerAccountId = ((string)  tolua_tocppstring(tolua_S,11,0));
  string buyerRoleGuid = ((string)  tolua_tocppstring(tolua_S,12,0));
  string buyerRoleName = ((string)  tolua_tocppstring(tolua_S,13,0));
  int32 buyerRoleLevel = ((int32)  tolua_tointeger(tolua_S,14,0));
  int32 moneyType = ((int32)  tolua_tointeger(tolua_S,15,0));
  string moneyName = ((string)  tolua_tocppstring(tolua_S,16,0));
  int32 moneyValue = ((int32)  tolua_tointeger(tolua_S,17,0));
  int32 auctionState = ((int32)  tolua_tointeger(tolua_S,18,0));
  {
   faith::server_log::serverAuction(*lg_common_head,sellerAccountId,sellerRoleGuid,sellerRoleName,sellerRoleLevel,itemGuid,itemId,itemNum,auctionType,buyerAccountId,buyerRoleGuid,buyerRoleName,buyerRoleLevel,moneyType,moneyName,moneyValue,auctionState);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'serverAuction'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cross::send_msg_to_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_send_msg_to_ws00
static int tolua_logic_hld_cross_send_msg_to_ws00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"faith::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::guid_64 role_guid = *((faith::guid_64*)  tolua_tousertype(tolua_S,1,0));
  int32 server_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  uint32 dataType = ((uint32)  tolua_tointeger(tolua_S,3,0));
  const void* pdata = ((const void*)  tolua_touserdata(tolua_S,4,0));
  uint32 len = ((uint32)  tolua_tointeger(tolua_S,5,0));
  {
   faith::cross::send_msg_to_ws(role_guid,server_id,dataType,pdata,len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_msg_to_ws'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_logic_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"s_client_uid","faith::s_client_uid","",tolua_collect_hld__s_client_uid);
   #else
   tolua_cclass(tolua_S,"s_client_uid","faith::s_client_uid","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"s_client_uid");
    tolua_function(tolua_S,"new",tolua_logic_hld_s_client_uid_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_s_client_uid_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_s_client_uid_new00_local);
    tolua_function(tolua_S,"new",tolua_logic_hld_s_client_uid_new01);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_s_client_uid_new01_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_s_client_uid_new01_local);
    tolua_function(tolua_S,"is_valid",tolua_logic_hld_s_client_uid_is_valid00);
    tolua_function(tolua_S,"clear_data",tolua_logic_hld_s_client_uid_clear_data00);
    tolua_variable(tolua_S,"fep_uid_64",tolua_get_hld__s_client_uid_fep_uid_64,tolua_set_hld__s_client_uid_fep_uid_64);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"guid_64","faith::guid_64","",tolua_collect_hld__guid_64);
   #else
   tolua_cclass(tolua_S,"guid_64","faith::guid_64","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"guid_64");
    tolua_function(tolua_S,"new",tolua_logic_hld_guid_64_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_guid_64_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_guid_64_new00_local);
    tolua_function(tolua_S,"new",tolua_logic_hld_guid_64_new01);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_guid_64_new01_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_guid_64_new01_local);
    tolua_function(tolua_S,"new",tolua_logic_hld_guid_64_new02);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_guid_64_new02_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_guid_64_new02_local);
    tolua_function(tolua_S,"get_A",tolua_logic_hld_guid_64_get_A00);
    tolua_function(tolua_S,"get_B",tolua_logic_hld_guid_64_get_B00);
    tolua_function(tolua_S,"get_AB",tolua_logic_hld_guid_64_get_AB00);
    tolua_function(tolua_S,"is_valid",tolua_logic_hld_guid_64_is_valid00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"fvector","faith::fvector","",tolua_collect_hld__fvector);
   #else
   tolua_cclass(tolua_S,"fvector","faith::fvector","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"fvector");
    tolua_variable(tolua_S,"x",tolua_get_hld__fvector_x,tolua_set_hld__fvector_x);
    tolua_variable(tolua_S,"y",tolua_get_hld__fvector_y,tolua_set_hld__fvector_y);
    tolua_variable(tolua_S,"z",tolua_get_hld__fvector_z,tolua_set_hld__fvector_z);
    tolua_function(tolua_S,"new",tolua_logic_hld_fvector_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_fvector_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_fvector_new00_local);
    tolua_function(tolua_S,"set_data",tolua_logic_hld_fvector_set_data00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"frotator","faith::frotator","",tolua_collect_hld__frotator);
   #else
   tolua_cclass(tolua_S,"frotator","faith::frotator","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"frotator");
    tolua_variable(tolua_S,"roll",tolua_get_hld__frotator_roll,tolua_set_hld__frotator_roll);
    tolua_variable(tolua_S,"pitch",tolua_get_hld__frotator_pitch,tolua_set_hld__frotator_pitch);
    tolua_variable(tolua_S,"yaw",tolua_get_hld__frotator_yaw,tolua_set_hld__frotator_yaw);
    tolua_function(tolua_S,"new",tolua_logic_hld_frotator_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_frotator_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_frotator_new00_local);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"s_map_pos","faith::s_map_pos","",tolua_collect_hld__s_map_pos);
   #else
   tolua_cclass(tolua_S,"s_map_pos","faith::s_map_pos","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"s_map_pos");
    tolua_variable(tolua_S,"unit_location",tolua_get_hld__s_map_pos_unit_location,tolua_set_hld__s_map_pos_unit_location);
    tolua_variable(tolua_S,"unit_rotation",tolua_get_hld__s_map_pos_unit_rotation,tolua_set_hld__s_map_pos_unit_rotation);
    tolua_function(tolua_S,"new",tolua_logic_hld_s_map_pos_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_s_map_pos_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_s_map_pos_new00_local);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"client_session","faith::client_session","",NULL);
   tolua_beginmodule(tolua_S,"client_session");
    tolua_constant(tolua_S,"e_ss_logout",faith::client_session::e_ss_logout);
    tolua_constant(tolua_S,"e_ss_created",faith::client_session::e_ss_created);
    tolua_constant(tolua_S,"e_ss_login_ok",faith::client_session::e_ss_login_ok);
    tolua_constant(tolua_S,"e_ss_ingame",faith::client_session::e_ss_ingame);
    tolua_constant(tolua_S,"e_ss_queue",faith::client_session::e_ss_queue);
    tolua_constant(tolua_S,"e_ss_map_transfer",faith::client_session::e_ss_map_transfer);
    tolua_constant(tolua_S,"e_ss_cross_transfer",faith::client_session::e_ss_cross_transfer);
    tolua_constant(tolua_S,"e_ss_check_token",faith::client_session::e_ss_check_token);
    tolua_constant(tolua_S,"e_session_step_null",faith::client_session::e_session_step_null);
    tolua_constant(tolua_S,"e_session_step_login",faith::client_session::e_session_step_login);
    tolua_constant(tolua_S,"e_session_step_login_win",faith::client_session::e_session_step_login_win);
    tolua_constant(tolua_S,"e_session_step_enter_game",faith::client_session::e_session_step_enter_game);
    tolua_constant(tolua_S,"e_session_step_dp_load",faith::client_session::e_session_step_dp_load);
    tolua_constant(tolua_S,"e_session_step_send_enter_game_cs",faith::client_session::e_session_step_send_enter_game_cs);
    tolua_constant(tolua_S,"e_session_step_cs_enter_game",faith::client_session::e_session_step_cs_enter_game);
    tolua_constant(tolua_S,"e_session_step_send_enter_scene",faith::client_session::e_session_step_send_enter_scene);
    tolua_constant(tolua_S,"e_session_step_cs_enter_scene",faith::client_session::e_session_step_cs_enter_scene);
    tolua_constant(tolua_S,"e_pss_left",faith::client_session::e_pss_left);
    tolua_constant(tolua_S,"e_pss_leaving",faith::client_session::e_pss_leaving);
    tolua_constant(tolua_S,"e_pss_entering",faith::client_session::e_pss_entering);
    tolua_constant(tolua_S,"e_pss_entered",faith::client_session::e_pss_entered);
    tolua_constant(tolua_S,"e_pss_sys_transfer",faith::client_session::e_pss_sys_transfer);
    tolua_function(tolua_S,"is_vaild",tolua_logic_hld_client_session_is_vaild00);
    tolua_function(tolua_S,"get_array_index",tolua_logic_hld_client_session_get_array_index00);
    tolua_function(tolua_S,"get_cs_conn_index",tolua_logic_hld_client_session_get_cs_conn_index00);
    tolua_function(tolua_S,"send_to_client",tolua_logic_hld_client_session_send_to_client00);
    tolua_function(tolua_S,"send_to_cs_lua",tolua_logic_hld_client_session_send_to_cs_lua00);
    tolua_function(tolua_S,"get_client_uid",tolua_logic_hld_client_session_get_client_uid00);
    tolua_function(tolua_S,"get_map_guid",tolua_logic_hld_client_session_get_map_guid00);
    tolua_function(tolua_S,"get_team_guid",tolua_logic_hld_client_session_get_team_guid00);
    tolua_function(tolua_S,"get_legion_guid",tolua_logic_hld_client_session_get_legion_guid00);
    tolua_function(tolua_S,"get_role_info_data",tolua_logic_hld_client_session_get_role_info_data00);
    tolua_function(tolua_S,"get_role_guid",tolua_logic_hld_client_session_get_role_guid00);
    tolua_function(tolua_S,"get_relation_list_mgr",tolua_logic_hld_client_session_get_relation_list_mgr00);
    tolua_function(tolua_S,"get_role_name",tolua_logic_hld_client_session_get_role_name00);
    tolua_function(tolua_S,"set_role_name",tolua_logic_hld_client_session_set_role_name00);
    tolua_function(tolua_S,"send_notice",tolua_logic_hld_client_session_send_notice00);
    tolua_function(tolua_S,"is_self_server",tolua_logic_hld_client_session_is_self_server00);
    tolua_function(tolua_S,"is_self_server",tolua_logic_hld_client_session_is_self_server01);
    tolua_function(tolua_S,"get_third_info",tolua_logic_hld_client_session_get_third_info00);
    tolua_function(tolua_S,"get_login_type",tolua_logic_hld_client_session_get_login_type00);
    tolua_function(tolua_S,"get_unit_info",tolua_logic_hld_client_session_get_unit_info00);
    tolua_function(tolua_S,"get_m_is_in_assist",tolua_logic_hld_client_session_get_m_is_in_assist00);
    tolua_function(tolua_S,"set_m_is_in_assist",tolua_logic_hld_client_session_set_m_is_in_assist00);
    tolua_function(tolua_S,"set_assit_fight_guid",tolua_logic_hld_client_session_set_assit_fight_guid00);
    tolua_function(tolua_S,"get_assit_fight_guid",tolua_logic_hld_client_session_get_assit_fight_guid00);
    tolua_function(tolua_S,"get_m_activate_code_time",tolua_logic_hld_client_session_get_m_activate_code_time00);
    tolua_function(tolua_S,"set_m_activate_code_time",tolua_logic_hld_client_session_set_m_activate_code_time00);
   tolua_endmodule(tolua_S);
   tolua_function(tolua_S,"get_guid_64",tolua_logic_hld_get_guid_6400);
   tolua_function(tolua_S,"get_guid_64",tolua_logic_hld_get_guid_6401);
   tolua_function(tolua_S,"get_guid_64",tolua_logic_hld_get_guid_6402);
   tolua_function(tolua_S,"get_client_session_by_index",tolua_logic_hld_get_client_session_by_index00);
   tolua_function(tolua_S,"get_client_session_by_guid",tolua_logic_hld_get_client_session_by_guid00);
   tolua_function(tolua_S,"get_world_server",tolua_logic_hld_get_world_server00);
   tolua_function(tolua_S,"make_guid_lua",tolua_logic_hld_make_guid_lua00);
   tolua_function(tolua_S,"get_client_session_mgr",tolua_logic_hld_get_client_session_mgr00);
   tolua_function(tolua_S,"get_str_by_string_template_id",tolua_logic_hld_get_str_by_string_template_id00);
   tolua_function(tolua_S,"get_item_info_by_str",tolua_logic_hld_get_item_info_by_str00);
   tolua_function(tolua_S,"get_item_info_by_str",tolua_logic_hld_get_item_info_by_str01);
   tolua_function(tolua_S,"get_auction_info_by_str",tolua_logic_hld_get_auction_info_by_str00);
   tolua_function(tolua_S,"get_auction_buffer",tolua_logic_hld_get_auction_buffer00);
   tolua_function(tolua_S,"get_unit_info_by_str",tolua_logic_hld_get_unit_info_by_str00);
   tolua_function(tolua_S,"get_login_fixed_data_by_str",tolua_logic_hld_get_login_fixed_data_by_str00);
   tolua_function(tolua_S,"get_log_common_head",tolua_logic_hld_get_log_common_head00);
   tolua_function(tolua_S,"lua_set_log_common_head_part1",tolua_logic_hld_lua_set_log_common_head_part100);
   tolua_function(tolua_S,"lua_set_log_common_head_part2",tolua_logic_hld_lua_set_log_common_head_part200);
   tolua_function(tolua_S,"lua_set_log_common_head_part3",tolua_logic_hld_lua_set_log_common_head_part300);
   tolua_function(tolua_S,"lua_set_log_common_head_part4",tolua_logic_hld_lua_set_log_common_head_part400);
   tolua_function(tolua_S,"lua_set_log_common_head_part5",tolua_logic_hld_lua_set_log_common_head_part500);
   tolua_function(tolua_S,"get_app_key",tolua_logic_hld_get_app_key00);
   tolua_function(tolua_S,"get_mail_info_by_str",tolua_logic_hld_get_mail_info_by_str00);
   tolua_function(tolua_S,"get_fake_player_info_by_str",tolua_logic_hld_get_fake_player_info_by_str00);
   tolua_function(tolua_S,"get_send_big_player_msg",tolua_logic_hld_get_send_big_player_msg00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"globle_data","faith::globle_data","",NULL);
   tolua_beginmodule(tolua_S,"globle_data");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_globle_data_get_instance00);
    tolua_function(tolua_S,"send_mail_to_another_player_by_system",tolua_logic_hld_globle_data_send_mail_to_another_player_by_system00);
    tolua_function(tolua_S,"clear_char_notice_info",tolua_logic_hld_globle_data_clear_char_notice_info00);
    tolua_function(tolua_S,"set_notice_id",tolua_logic_hld_globle_data_set_notice_id00);
    tolua_function(tolua_S,"add_other_name",tolua_logic_hld_globle_data_add_other_name00);
    tolua_function(tolua_S,"add_describe_str",tolua_logic_hld_globle_data_add_describe_str00);
    tolua_function(tolua_S,"add_item_name",tolua_logic_hld_globle_data_add_item_name00);
    tolua_function(tolua_S,"add_num_type_data",tolua_logic_hld_globle_data_add_num_type_data00);
    tolua_function(tolua_S,"add_item_data",tolua_logic_hld_globle_data_add_item_data00);
    tolua_function(tolua_S,"get_mail_common_text",tolua_logic_hld_globle_data_get_mail_common_text00);
    tolua_function(tolua_S,"get_mail_common_text_id",tolua_logic_hld_globle_data_get_mail_common_text_id00);
    tolua_function(tolua_S,"get_region_code",tolua_logic_hld_globle_data_get_region_code00);
    tolua_function(tolua_S,"get_money_str_by_type",tolua_logic_hld_globle_data_get_money_str_by_type00);
    tolua_function(tolua_S,"get_server_on_days",tolua_logic_hld_globle_data_get_server_on_days00);
    tolua_function(tolua_S,"make_auction_info",tolua_logic_hld_globle_data_make_auction_info00);
    tolua_function(tolua_S,"make_auction_info",tolua_logic_hld_globle_data_make_auction_info01);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"c_mail_info","faith::c_mail_info","",tolua_collect_hld__c_mail_info);
   #else
   tolua_cclass(tolua_S,"c_mail_info","faith::c_mail_info","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"c_mail_info");
    tolua_variable(tolua_S,"TargetPlayerGuid",tolua_get_hld__c_mail_info_TargetPlayerGuid,tolua_set_hld__c_mail_info_TargetPlayerGuid);
    tolua_variable(tolua_S,"SenderPlayerGuid",tolua_get_hld__c_mail_info_SenderPlayerGuid,tolua_set_hld__c_mail_info_SenderPlayerGuid);
    tolua_variable(tolua_S,"SenderName",tolua_get_hld__c_mail_info_SenderName,tolua_set_hld__c_mail_info_SenderName);
    tolua_variable(tolua_S,"title",tolua_get_hld__c_mail_info_title,tolua_set_hld__c_mail_info_title);
    tolua_variable(tolua_S,"contentText",tolua_get_hld__c_mail_info_contentText,tolua_set_hld__c_mail_info_contentText);
    tolua_variable(tolua_S,"money_typ1",tolua_get_hld__c_mail_info_money_typ1,tolua_set_hld__c_mail_info_money_typ1);
    tolua_variable(tolua_S,"money_num1",tolua_get_hld__c_mail_info_money_num1,tolua_set_hld__c_mail_info_money_num1);
    tolua_variable(tolua_S,"money_typ2",tolua_get_hld__c_mail_info_money_typ2,tolua_set_hld__c_mail_info_money_typ2);
    tolua_variable(tolua_S,"money_num2",tolua_get_hld__c_mail_info_money_num2,tolua_set_hld__c_mail_info_money_num2);
    tolua_variable(tolua_S,"item_list",tolua_get_hld__c_mail_info_item_list_ptr,NULL);
    tolua_variable(tolua_S,"item_num",tolua_get_hld__c_mail_info_item_num,tolua_set_hld__c_mail_info_item_num);
    tolua_function(tolua_S,"new",tolua_logic_hld_c_mail_info_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_c_mail_info_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_c_mail_info_new00_local);
    tolua_function(tolua_S,"new",tolua_logic_hld_c_mail_info_new01);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_c_mail_info_new01_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_c_mail_info_new01_local);
    tolua_function(tolua_S,"clear_data",tolua_logic_hld_c_mail_info_clear_data00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"init_unit","faith::init_unit","",NULL);
   tolua_beginmodule(tolua_S,"init_unit");
    tolua_function(tolua_S,"get_left_shift_operation_result",tolua_logic_hld_init_unit_get_left_shift_operation_result00);
    tolua_function(tolua_S,"get_bitwise_and_operation_result",tolua_logic_hld_init_unit_get_bitwise_and_operation_result00);
    tolua_function(tolua_S,"get_bitwise_or_operation_result",tolua_logic_hld_init_unit_get_bitwise_or_operation_result00);
    tolua_function(tolua_S,"get_item_bag_type",tolua_logic_hld_init_unit_get_item_bag_type00);
    tolua_function(tolua_S,"is_can_use_this_jewel_slot_on_this_equip_level",tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_equip_level00);
    tolua_function(tolua_S,"is_can_use_this_jewel_slot_on_this_vip_level",tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_vip_level00);
    tolua_function(tolua_S,"get_flag_data",tolua_logic_hld_init_unit_get_flag_data00);
    tolua_function(tolua_S,"get_max_player_level",tolua_logic_hld_init_unit_get_max_player_level00);
    tolua_function(tolua_S,"get_max_grade_level",tolua_logic_hld_init_unit_get_max_grade_level00);
    tolua_function(tolua_S,"is_big_endian",tolua_logic_hld_init_unit_is_big_endian00);
    tolua_function(tolua_S,"is_cross_server_big_player",tolua_logic_hld_init_unit_is_cross_server_big_player00);
    tolua_function(tolua_S,"init_item_data",tolua_logic_hld_init_unit_init_item_data00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_role_pk_map_state_one_role",faith::e_role_pk_map_state_one_role);
   tolua_constant(tolua_S,"e_role_pk_map_state_wait_begin",faith::e_role_pk_map_state_wait_begin);
   tolua_constant(tolua_S,"e_role_pk_map_state_begin_war",faith::e_role_pk_map_state_begin_war);
   tolua_constant(tolua_S,"e_role_pk_map_state_end_war",faith::e_role_pk_map_state_end_war);
   tolua_constant(tolua_S,"e_role_pk_map_state_max",faith::e_role_pk_map_state_max);
   tolua_constant(tolua_S,"e_map_type_ui",faith::e_map_type_ui);
   tolua_constant(tolua_S,"e_map_type_new",faith::e_map_type_new);
   tolua_constant(tolua_S,"e_map_type_main",faith::e_map_type_main);
   tolua_constant(tolua_S,"e_map_type_battle",faith::e_map_type_battle);
   tolua_constant(tolua_S,"e_map_type_field",faith::e_map_type_field);
   tolua_constant(tolua_S,"e_map_type_big_map",faith::e_map_type_big_map);
   tolua_constant(tolua_S,"e_map_type_fly_battle",faith::e_map_type_fly_battle);
   tolua_constant(tolua_S,"e_map_type_exp_fuben",faith::e_map_type_exp_fuben);
   tolua_constant(tolua_S,"e_map_type_money_fuben",faith::e_map_type_money_fuben);
   tolua_constant(tolua_S,"e_map_type_demon_tower_fuben",faith::e_map_type_demon_tower_fuben);
   tolua_constant(tolua_S,"e_map_type_legion_boss",faith::e_map_type_legion_boss);
   tolua_constant(tolua_S,"e_map_type_city_war",faith::e_map_type_city_war);
   tolua_constant(tolua_S,"e_map_type_broken_sky",faith::e_map_type_broken_sky);
   tolua_constant(tolua_S,"e_map_type_king_of_pk",faith::e_map_type_king_of_pk);
   tolua_constant(tolua_S,"e_map_type_arena",faith::e_map_type_arena);
   tolua_constant(tolua_S,"e_map_type_temple",faith::e_map_type_temple);
   tolua_constant(tolua_S,"e_map_type_empire_treasure",faith::e_map_type_empire_treasure);
   tolua_constant(tolua_S,"e_map_type_crystak_dreamland",faith::e_map_type_crystak_dreamland);
   tolua_constant(tolua_S,"e_map_type_legion_bonfire",faith::e_map_type_legion_bonfire);
   tolua_constant(tolua_S,"e_map_type_boss_single",faith::e_map_type_boss_single);
   tolua_constant(tolua_S,"e_map_type_boss_home",faith::e_map_type_boss_home);
   tolua_constant(tolua_S,"e_map_type_boss_vip_home",faith::e_map_type_boss_vip_home);
   tolua_constant(tolua_S,"e_map_type_boss_island",faith::e_map_type_boss_island);
   tolua_constant(tolua_S,"e_map_type_belief_cloister",faith::e_map_type_belief_cloister);
   tolua_constant(tolua_S,"e_map_type_princess_guard_war",faith::e_map_type_princess_guard_war);
   tolua_constant(tolua_S,"e_map_type_lava_hellfire",faith::e_map_type_lava_hellfire);
   tolua_constant(tolua_S,"e_map_type_sea_of_hate",faith::e_map_type_sea_of_hate);
   tolua_constant(tolua_S,"e_map_type_abyss_of_fear",faith::e_map_type_abyss_of_fear);
   tolua_constant(tolua_S,"e_map_type_fallen_fantasy",faith::e_map_type_fallen_fantasy);
   tolua_constant(tolua_S,"e_map_type_star_test",faith::e_map_type_star_test);
   tolua_constant(tolua_S,"e_map_type_cross_server_pk",faith::e_map_type_cross_server_pk);
   tolua_constant(tolua_S,"e_map_type_cross_server_harry",faith::e_map_type_cross_server_harry);
   tolua_constant(tolua_S,"e_map_type_cross_ladder",faith::e_map_type_cross_ladder);
   tolua_constant(tolua_S,"e_map_type_oracle_trial_map",faith::e_map_type_oracle_trial_map);
   tolua_constant(tolua_S,"e_map_type_legion_station",faith::e_map_type_legion_station);
   tolua_constant(tolua_S,"e_map_type_appearance",faith::e_map_type_appearance);
   tolua_constant(tolua_S,"e_map_type_single_land_boss",faith::e_map_type_single_land_boss);
   tolua_constant(tolua_S,"e_map_type_single_assist_boss",faith::e_map_type_single_assist_boss);
   tolua_constant(tolua_S,"e_map_type_level_up_map",faith::e_map_type_level_up_map);
   tolua_constant(tolua_S,"e_map_type_element_war",faith::e_map_type_element_war);
   tolua_constant(tolua_S,"e_map_type_attack_city",faith::e_map_type_attack_city);
   tolua_constant(tolua_S,"e_map_type_single_monster",faith::e_map_type_single_monster);
   tolua_constant(tolua_S,"e_map_type_single_open_box",faith::e_map_type_single_open_box);
   tolua_constant(tolua_S,"e_map_type_max",faith::e_map_type_max);
   tolua_constant(tolua_S,"e_role_info_template_id",faith::e_role_info_template_id);
   tolua_constant(tolua_S,"e_role_info_server_id",faith::e_role_info_server_id);
   tolua_constant(tolua_S,"e_role_info_img_id",faith::e_role_info_img_id);
   tolua_constant(tolua_S,"e_role_info_class_type",faith::e_role_info_class_type);
   tolua_constant(tolua_S,"e_role_info_class_branch",faith::e_role_info_class_branch);
   tolua_constant(tolua_S,"e_role_info_gender",faith::e_role_info_gender);
   tolua_constant(tolua_S,"e_role_info_exp_level",faith::e_role_info_exp_level);
   tolua_constant(tolua_S,"e_role_info_vip_level",faith::e_role_info_vip_level);
   tolua_constant(tolua_S,"e_role_info_vip_experience_level",faith::e_role_info_vip_experience_level);
   tolua_constant(tolua_S,"e_role_info_recharge_num",faith::e_role_info_recharge_num);
   tolua_constant(tolua_S,"e_role_info_main_map_id",faith::e_role_info_main_map_id);
   tolua_constant(tolua_S,"e_role_info_main_pos_x",faith::e_role_info_main_pos_x);
   tolua_constant(tolua_S,"e_role_info_main_pos_y",faith::e_role_info_main_pos_y);
   tolua_constant(tolua_S,"e_role_info_main_pos_z",faith::e_role_info_main_pos_z);
   tolua_constant(tolua_S,"e_role_info_main_pos_yaw",faith::e_role_info_main_pos_yaw);
   tolua_constant(tolua_S,"e_role_info_main_line_id",faith::e_role_info_main_line_id);
   tolua_constant(tolua_S,"e_role_info_move_map_id",faith::e_role_info_move_map_id);
   tolua_constant(tolua_S,"e_role_info_move_pos_x",faith::e_role_info_move_pos_x);
   tolua_constant(tolua_S,"e_role_info_move_pos_y",faith::e_role_info_move_pos_y);
   tolua_constant(tolua_S,"e_role_info_move_pos_z",faith::e_role_info_move_pos_z);
   tolua_constant(tolua_S,"e_role_info_move_pos_yaw",faith::e_role_info_move_pos_yaw);
   tolua_constant(tolua_S,"e_role_info_move_line_id",faith::e_role_info_move_line_id);
   tolua_constant(tolua_S,"e_role_info_GS_value",faith::e_role_info_GS_value);
   tolua_constant(tolua_S,"e_role_info_upgrade_id",faith::e_role_info_upgrade_id);
   tolua_constant(tolua_S,"e_role_info_daily_done_num",faith::e_role_info_daily_done_num);
   tolua_constant(tolua_S,"e_role_info_against_done_num",faith::e_role_info_against_done_num);
   tolua_constant(tolua_S,"e_role_info_wing_showd_template_id",faith::e_role_info_wing_showd_template_id);
   tolua_constant(tolua_S,"e_role_info_mount_showd_template_id",faith::e_role_info_mount_showd_template_id);
   tolua_constant(tolua_S,"e_role_info_element_heart_drop_group",faith::e_role_info_element_heart_drop_group);
   tolua_constant(tolua_S,"e_role_info_arena_times",faith::e_role_info_arena_times);
   tolua_constant(tolua_S,"e_role_info_arena_vip_times",faith::e_role_info_arena_vip_times);
   tolua_constant(tolua_S,"e_role_info_arena_vip_buy_times",faith::e_role_info_arena_vip_buy_times);
   tolua_constant(tolua_S,"e_role_info_arena_winning_streak",faith::e_role_info_arena_winning_streak);
   tolua_constant(tolua_S,"e_role_info_legion_donate_earn_by_money",faith::e_role_info_legion_donate_earn_by_money);
   tolua_constant(tolua_S,"e_role_info_legion_donate_earn_by_item",faith::e_role_info_legion_donate_earn_by_item);
   tolua_constant(tolua_S,"e_role_info_worship_other_time",faith::e_role_info_worship_other_time);
   tolua_constant(tolua_S,"e_role_info_get_func_unlock_award_record",faith::e_role_info_get_func_unlock_award_record);
   tolua_constant(tolua_S,"e_role_info_vip_item_geted_level",faith::e_role_info_vip_item_geted_level);
   tolua_constant(tolua_S,"e_role_info_vip_free_revive_time",faith::e_role_info_vip_free_revive_time);
   tolua_constant(tolua_S,"e_role_info_element_heart_refine_time",faith::e_role_info_element_heart_refine_time);
   tolua_constant(tolua_S,"e_role_info_total_login_days",faith::e_role_info_total_login_days);
   tolua_constant(tolua_S,"e_role_info_continue_login_time",faith::e_role_info_continue_login_time);
   tolua_constant(tolua_S,"e_role_info_daily_activity_degree",faith::e_role_info_daily_activity_degree);
   tolua_constant(tolua_S,"e_role_info_daily_recharge_num",faith::e_role_info_daily_recharge_num);
   tolua_constant(tolua_S,"e_role_info_total_diamond_cost_num",faith::e_role_info_total_diamond_cost_num);
   tolua_constant(tolua_S,"e_role_info_daily_active_degree_get_flag",faith::e_role_info_daily_active_degree_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_recharge_num",faith::e_role_info_fund_recharge_num);
   tolua_constant(tolua_S,"e_role_info_fund_payment_flag",faith::e_role_info_fund_payment_flag);
   tolua_constant(tolua_S,"e_role_info_fund_time_get_flag",faith::e_role_info_fund_time_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_pride_get_flag",faith::e_role_info_fund_pride_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_reborn_get_flag",faith::e_role_info_fund_reborn_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_cost_num",faith::e_role_info_fund_cost_num);
   tolua_constant(tolua_S,"e_role_info_lord_of_city_worship_time",faith::e_role_info_lord_of_city_worship_time);
   tolua_constant(tolua_S,"e_role_info_lord_of_war_worship_time",faith::e_role_info_lord_of_war_worship_time);
   tolua_constant(tolua_S,"e_role_info_lord_of_sanctuary_worship_time",faith::e_role_info_lord_of_sanctuary_worship_time);
   tolua_constant(tolua_S,"e_role_info_lord_of_god_worship_time",faith::e_role_info_lord_of_god_worship_time);
   tolua_constant(tolua_S,"e_role_info_hope_value",faith::e_role_info_hope_value);
   tolua_constant(tolua_S,"e_role_info_del_time",faith::e_role_info_del_time);
   tolua_constant(tolua_S,"e_role_info_kill_player_num",faith::e_role_info_kill_player_num);
   tolua_constant(tolua_S,"e_role_info_spirit_value",faith::e_role_info_spirit_value);
   tolua_constant(tolua_S,"e_role_info_watch_count",faith::e_role_info_watch_count);
   tolua_constant(tolua_S,"e_role_info_crystal_dreamland_chests",faith::e_role_info_crystal_dreamland_chests);
   tolua_constant(tolua_S,"e_role_info_protect_mark_slot_open_num",faith::e_role_info_protect_mark_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_element_heart_slot_open_num",faith::e_role_info_element_heart_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_sprite_equip_slot_open_num",faith::e_role_info_sprite_equip_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_exclusive_flag",faith::e_role_info_exclusive_flag);
   tolua_constant(tolua_S,"e_role_info_red_package_geted_times",faith::e_role_info_red_package_geted_times);
   tolua_constant(tolua_S,"e_role_info_free_red_package_send_times",faith::e_role_info_free_red_package_send_times);
   tolua_constant(tolua_S,"e_role_info_world_boss_chests",faith::e_role_info_world_boss_chests);
   tolua_constant(tolua_S,"e_role_info_leave_map_id",faith::e_role_info_leave_map_id);
   tolua_constant(tolua_S,"e_role_info_assist_chests",faith::e_role_info_assist_chests);
   tolua_constant(tolua_S,"e_role_info_next_side_mission_main_trigger_id",faith::e_role_info_next_side_mission_main_trigger_id);
   tolua_constant(tolua_S,"e_role_info_show_fashion",faith::e_role_info_show_fashion);
   tolua_constant(tolua_S,"e_role_info_real_recharge_jewel_num",faith::e_role_info_real_recharge_jewel_num);
   tolua_constant(tolua_S,"e_role_info_relic_kill_num",faith::e_role_info_relic_kill_num);
   tolua_constant(tolua_S,"e_role_info_relic_exp_num",faith::e_role_info_relic_exp_num);
   tolua_constant(tolua_S,"e_role_info_head_frame",faith::e_role_info_head_frame);
   tolua_constant(tolua_S,"e_role_info_fund_pride_cost_value",faith::e_role_info_fund_pride_cost_value);
   tolua_constant(tolua_S,"e_role_info_tinder_prop_slot_open_num",faith::e_role_info_tinder_prop_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_show_sky_suit_shape",faith::e_role_info_show_sky_suit_shape);
   tolua_constant(tolua_S,"e_role_info_move_server_id",faith::e_role_info_move_server_id);
   tolua_constant(tolua_S,"e_role_info_move_war_idex",faith::e_role_info_move_war_idex);
   tolua_constant(tolua_S,"e_role_info_goddess_slot_flag",faith::e_role_info_goddess_slot_flag);
   tolua_constant(tolua_S,"e_role_info_daily_cost",faith::e_role_info_daily_cost);
   tolua_constant(tolua_S,"e_role_info_marry_done_num",faith::e_role_info_marry_done_num);
   tolua_constant(tolua_S,"e_role_info_marry_dati_id",faith::e_role_info_marry_dati_id);
   tolua_constant(tolua_S,"e_role_info_marry_dati_num",faith::e_role_info_marry_dati_num);
   tolua_constant(tolua_S,"e_role_info_marry_dati_right_num",faith::e_role_info_marry_dati_right_num);
   tolua_constant(tolua_S,"e_role_info_marry_dati_current_answer",faith::e_role_info_marry_dati_current_answer);
   tolua_constant(tolua_S,"e_role_info_legion_dance_exp_num",faith::e_role_info_legion_dance_exp_num);
   tolua_constant(tolua_S,"e_role_info_legion_donate_num",faith::e_role_info_legion_donate_num);
   tolua_constant(tolua_S,"e_role_info_appearance_id",faith::e_role_info_appearance_id);
   tolua_constant(tolua_S,"e_role_info_is_show_vip",faith::e_role_info_is_show_vip);
   tolua_constant(tolua_S,"e_role_info_need_check_guide",faith::e_role_info_need_check_guide);
   tolua_constant(tolua_S,"e_role_info_enchant_show_type",faith::e_role_info_enchant_show_type);
   tolua_constant(tolua_S,"e_role_info_push_map_id",faith::e_role_info_push_map_id);
   tolua_constant(tolua_S,"e_role_info_max",faith::e_role_info_max);
   tolua_constant(tolua_S,"e_role_logic_info_bag_open",faith::e_role_logic_info_bag_open);
   tolua_constant(tolua_S,"e_role_logic_info_main_pk_mode",faith::e_role_logic_info_main_pk_mode);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_first_one",faith::e_role_logic_info_jewel_first_one);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_first_ten",faith::e_role_logic_info_jewel_first_ten);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_luck",faith::e_role_logic_info_jewel_luck);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_free",faith::e_role_logic_info_jewel_free);
   tolua_constant(tolua_S,"e_role_logic_info_element_first_one",faith::e_role_logic_info_element_first_one);
   tolua_constant(tolua_S,"e_role_logic_info_element_first_ten",faith::e_role_logic_info_element_first_ten);
   tolua_constant(tolua_S,"e_role_logic_info_element_luck",faith::e_role_logic_info_element_luck);
   tolua_constant(tolua_S,"e_role_logic_info_element_free",faith::e_role_logic_info_element_free);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_first_one",faith::e_role_logic_info_spirit_first_one);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_first_ten",faith::e_role_logic_info_spirit_first_ten);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_luck",faith::e_role_logic_info_spirit_luck);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_free",faith::e_role_logic_info_spirit_free);
   tolua_constant(tolua_S,"e_role_logic_info_storage_open",faith::e_role_logic_info_storage_open);
   tolua_constant(tolua_S,"e_role_logic_info_first_look_legion",faith::e_role_logic_info_first_look_legion);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission",faith::e_role_logic_info_done_grade_mission);
   tolua_constant(tolua_S,"e_role_logic_info_god_hood_grade",faith::e_role_logic_info_god_hood_grade);
   tolua_constant(tolua_S,"e_role_logic_info_first_join_legion_award",faith::e_role_logic_info_first_join_legion_award);
   tolua_constant(tolua_S,"e_role_logic_info_is_get_kill_legion_boss_award",faith::e_role_logic_info_is_get_kill_legion_boss_award);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_1",faith::e_role_logic_info_done_grade_mission_1);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_2",faith::e_role_logic_info_done_grade_mission_2);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_3",faith::e_role_logic_info_done_grade_mission_3);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_4",faith::e_role_logic_info_done_grade_mission_4);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_total_recharge",faith::e_role_logic_info_jewel_total_recharge);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_total_consume",faith::e_role_logic_info_jewel_total_consume);
   tolua_constant(tolua_S,"e_role_logic_info_cross_ladder_join_num",faith::e_role_logic_info_cross_ladder_join_num);
   tolua_constant(tolua_S,"e_role_logic_info_cross_ladder_win_num",faith::e_role_logic_info_cross_ladder_win_num);
   tolua_constant(tolua_S,"e_role_logic_info_cross_ladder_reward_tag",faith::e_role_logic_info_cross_ladder_reward_tag);
   tolua_constant(tolua_S,"e_role_logic_info_marry_times",faith::e_role_logic_info_marry_times);
   tolua_constant(tolua_S,"e_role_logic_info_marry_divorce_times",faith::e_role_logic_info_marry_divorce_times);
   tolua_constant(tolua_S,"e_role_logic_info_daily_lucky_times",faith::e_role_logic_info_daily_lucky_times);
   tolua_constant(tolua_S,"e_role_logic_info_zero_gs_up_start_value",faith::e_role_logic_info_zero_gs_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_mount_up_start_value",faith::e_role_logic_info_zero_mount_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_wing_up_start_value",faith::e_role_logic_info_zero_wing_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_equip_up_start_value",faith::e_role_logic_info_zero_equip_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_spirit_up_start_value",faith::e_role_logic_info_zero_spirit_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_helper_reward_send_times",faith::e_role_logic_info_helper_reward_send_times);
   tolua_constant(tolua_S,"e_role_logic_info_boss_home_cur_used_times",faith::e_role_logic_info_boss_home_cur_used_times);
   tolua_constant(tolua_S,"e_role_logic_info_gain_treasure_restore_num",faith::e_role_logic_info_gain_treasure_restore_num);
   tolua_constant(tolua_S,"e_role_logic_info_gain_treasure_restore_time",faith::e_role_logic_info_gain_treasure_restore_time);
   tolua_constant(tolua_S,"e_role_logic_info_sky_land_boss_restore_num",faith::e_role_logic_info_sky_land_boss_restore_num);
   tolua_constant(tolua_S,"e_role_logic_info_sky_land_boss_restore_time",faith::e_role_logic_info_sky_land_boss_restore_time);
   tolua_constant(tolua_S,"e_role_logic_info_boss_island_cur_kill_times",faith::e_role_logic_info_boss_island_cur_kill_times);
   tolua_constant(tolua_S,"e_role_logic_info_help_reward_send_times",faith::e_role_logic_info_help_reward_send_times);
   tolua_constant(tolua_S,"e_role_logic_info_sprite_equip_slot_open_flag",faith::e_role_logic_info_sprite_equip_slot_open_flag);
   tolua_constant(tolua_S,"e_role_logic_info_check_in_days",faith::e_role_logic_info_check_in_days);
   tolua_constant(tolua_S,"e_role_logic_info_check_in_repetition_num",faith::e_role_logic_info_check_in_repetition_num);
   tolua_constant(tolua_S,"e_role_logic_info_exp_raid_add_damage_gold",faith::e_role_logic_info_exp_raid_add_damage_gold);
   tolua_constant(tolua_S,"e_role_logic_info_exp_raid_add_damage_diamond",faith::e_role_logic_info_exp_raid_add_damage_diamond);
   tolua_constant(tolua_S,"e_role_logic_info_element_war_mission_tag",faith::e_role_logic_info_element_war_mission_tag);
   tolua_constant(tolua_S,"e_role_logic_info_element_war_buy_ticket",faith::e_role_logic_info_element_war_buy_ticket);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_carve_lucky",faith::e_role_logic_info_jewel_carve_lucky);
   tolua_constant(tolua_S,"e_role_logic_info_choose_fashion_id",faith::e_role_logic_info_choose_fashion_id);
   tolua_constant(tolua_S,"e_role_logic_info_help_tip_send_times",faith::e_role_logic_info_help_tip_send_times);
   tolua_constant(tolua_S,"e_role_logic_info_is_have_new_server_mail",faith::e_role_logic_info_is_have_new_server_mail);
   tolua_constant(tolua_S,"e_role_logic_info_awaken_fetter",faith::e_role_logic_info_awaken_fetter);
   tolua_constant(tolua_S,"e_role_logic_info_offline_award_time",faith::e_role_logic_info_offline_award_time);
   tolua_constant(tolua_S,"e_role_logic_info_offline_award_random",faith::e_role_logic_info_offline_award_random);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_money_type",faith::e_role_logic_info_online_award_money_type);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_money_num",faith::e_role_logic_info_online_award_money_num);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_time",faith::e_role_logic_info_online_award_time);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_money_count",faith::e_role_logic_info_online_award_money_count);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_item_count",faith::e_role_logic_info_online_award_item_count);
   tolua_constant(tolua_S,"e_role_logic_info_skill_level",faith::e_role_logic_info_skill_level);
   tolua_constant(tolua_S,"e_role_logic_info_search_value",faith::e_role_logic_info_search_value);
   tolua_constant(tolua_S,"e_role_logic_info_search_level",faith::e_role_logic_info_search_level);
   tolua_constant(tolua_S,"e_role_logic_info_max",faith::e_role_logic_info_max);
   tolua_constant(tolua_S,"e_role_box_map_next_award",faith::e_role_box_map_next_award);
   tolua_constant(tolua_S,"e_role_box_map_search_time",faith::e_role_box_map_search_time);
   tolua_constant(tolua_S,"e_role_box_map_middle_open_compensate",faith::e_role_box_map_middle_open_compensate);
   tolua_constant(tolua_S,"e_role_box_map_big_open_compensate",faith::e_role_box_map_big_open_compensate);
   tolua_constant(tolua_S,"e_role_box_map_special_refrush_compensate",faith::e_role_box_map_special_refrush_compensate);
   tolua_constant(tolua_S,"e_role_box_map_special_open_compensate",faith::e_role_box_map_special_open_compensate);
   tolua_constant(tolua_S,"e_role_box_map_max",faith::e_role_box_map_max);
   tolua_constant(tolua_S,"e_role_element_map_mop_up_count",faith::e_role_element_map_mop_up_count);
   tolua_constant(tolua_S,"e_role_element_map_max",faith::e_role_element_map_max);
   tolua_constant(tolua_S,"e_role_battle_map_mop_up_count",faith::e_role_battle_map_mop_up_count);
   tolua_constant(tolua_S,"e_role_battle_map_max",faith::e_role_battle_map_max);
   tolua_constant(tolua_S,"e_unit_attack_att_none",faith::e_unit_attack_att_none);
   tolua_constant(tolua_S,"e_unit_attack_att_strength",faith::e_unit_attack_att_strength);
   tolua_constant(tolua_S,"e_unit_attack_att_agility",faith::e_unit_attack_att_agility);
   tolua_constant(tolua_S,"e_unit_attack_att_intellect",faith::e_unit_attack_att_intellect);
   tolua_constant(tolua_S,"e_unit_attack_att_stamina",faith::e_unit_attack_att_stamina);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_min",faith::e_unit_attack_att_attack_min);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_max",faith::e_unit_attack_att_attack_max);
   tolua_constant(tolua_S,"e_unit_attack_att_armor",faith::e_unit_attack_att_armor);
   tolua_constant(tolua_S,"e_unit_attack_att_hp_max",faith::e_unit_attack_att_hp_max);
   tolua_constant(tolua_S,"e_unit_attack_att_hit_force",faith::e_unit_attack_att_hit_force);
   tolua_constant(tolua_S,"e_unit_attack_att_dodge_force",faith::e_unit_attack_att_dodge_force);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_rate",faith::e_unit_attack_att_critical_rate);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_armor_rate",faith::e_unit_attack_att_critical_armor_rate);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_attack_rate",faith::e_unit_attack_att_critical_attack_rate);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_attack_relief",faith::e_unit_attack_att_critical_attack_relief);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_1",faith::e_unit_attack_att_att_attack_1);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_1",faith::e_unit_attack_att_att_armor_1);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_2",faith::e_unit_attack_att_att_attack_2);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_2",faith::e_unit_attack_att_att_armor_2);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_3",faith::e_unit_attack_att_att_attack_3);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_3",faith::e_unit_attack_att_att_armor_3);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_4",faith::e_unit_attack_att_att_attack_4);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_4",faith::e_unit_attack_att_att_armor_4);
   tolua_constant(tolua_S,"e_unit_attack_att_element_ability",faith::e_unit_attack_att_element_ability);
   tolua_constant(tolua_S,"e_unit_attack_att_element_resist",faith::e_unit_attack_att_element_resist);
   tolua_constant(tolua_S,"e_unit_attack_att_effect_damage",faith::e_unit_attack_att_effect_damage);
   tolua_constant(tolua_S,"e_unit_attack_att_effect_damage_relief",faith::e_unit_attack_att_effect_damage_relief);
   tolua_constant(tolua_S,"e_unit_attack_att_hit_prob",faith::e_unit_attack_att_hit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_un_hit_prob",faith::e_unit_attack_att_un_hit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_crit_prob",faith::e_unit_attack_att_crit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_un_crit_prob",faith::e_unit_attack_att_un_crit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_move_speed",faith::e_unit_attack_att_move_speed);
   tolua_constant(tolua_S,"e_unit_attack_att_un_dizzy",faith::e_unit_attack_att_un_dizzy);
   tolua_constant(tolua_S,"e_unit_attack_att_un_move_speed",faith::e_unit_attack_att_un_move_speed);
   tolua_constant(tolua_S,"e_unit_attack_att_un_frozen",faith::e_unit_attack_att_un_frozen);
   tolua_constant(tolua_S,"e_unit_attack_att_un_no_skill",faith::e_unit_attack_att_un_no_skill);
   tolua_constant(tolua_S,"e_unit_attack_att_magic",faith::e_unit_attack_att_magic);
   tolua_constant(tolua_S,"e_unit_attack_att_body_power",faith::e_unit_attack_att_body_power);
   tolua_constant(tolua_S,"e_unit_attack_att_exp_addition",faith::e_unit_attack_att_exp_addition);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_npc",faith::e_unit_attack_att_attack_npc);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_player",faith::e_unit_attack_att_attack_player);
   tolua_constant(tolua_S,"e_unit_attack_att_exp_add_all",faith::e_unit_attack_att_exp_add_all);
   tolua_constant(tolua_S,"e_unit_attack_att_be_attack_player",faith::e_unit_attack_att_be_attack_player);
   tolua_constant(tolua_S,"e_unit_attack_att_pure_attack",faith::e_unit_attack_att_pure_attack);
   tolua_constant(tolua_S,"e_unit_attack_att_pure_armor",faith::e_unit_attack_att_pure_armor);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_multiple",faith::e_unit_attack_att_attack_multiple);
   tolua_constant(tolua_S,"e_unit_attack_att_flight_energy",faith::e_unit_attack_att_flight_energy);
   tolua_constant(tolua_S,"e_unit_attack_att_max",faith::e_unit_attack_att_max);
   tolua_constant(tolua_S,"e_unit_game_att_movement",faith::e_unit_game_att_movement);
   tolua_constant(tolua_S,"e_unit_game_att_exp_only",faith::e_unit_game_att_exp_only);
   tolua_constant(tolua_S,"e_unit_game_att_suck_blood",faith::e_unit_game_att_suck_blood);
   tolua_constant(tolua_S,"e_unit_game_att_damage_reflect",faith::e_unit_game_att_damage_reflect);
   tolua_constant(tolua_S,"e_unit_game_att_war_state",faith::e_unit_game_att_war_state);
   tolua_constant(tolua_S,"e_unit_game_att_damage_sub",faith::e_unit_game_att_damage_sub);
   tolua_constant(tolua_S,"e_unit_game_att_fight_lock",faith::e_unit_game_att_fight_lock);
   tolua_constant(tolua_S,"e_unit_game_att_jump",faith::e_unit_game_att_jump);
   tolua_constant(tolua_S,"e_unit_game_att_interaction",faith::e_unit_game_att_interaction);
   tolua_constant(tolua_S,"e_unit_game_att_exp_add",faith::e_unit_game_att_exp_add);
   tolua_constant(tolua_S,"e_unit_game_att_attack_num",faith::e_unit_game_att_attack_num);
   tolua_constant(tolua_S,"e_unit_game_att_back_hp_percent",faith::e_unit_game_att_back_hp_percent);
   tolua_constant(tolua_S,"e_unit_game_att_back_hp_value",faith::e_unit_game_att_back_hp_value);
   tolua_constant(tolua_S,"e_unit_game_att_no_back_hp_percent",faith::e_unit_game_att_no_back_hp_percent);
   tolua_constant(tolua_S,"e_unit_game_att_pure_hurt_percent",faith::e_unit_game_att_pure_hurt_percent);
   tolua_constant(tolua_S,"e_unit_game_att_pure_hurt_value",faith::e_unit_game_att_pure_hurt_value);
   tolua_constant(tolua_S,"e_unit_game_att_max",faith::e_unit_game_att_max);
   tolua_constant(tolua_S,"e_war_state_peace",faith::e_war_state_peace);
   tolua_constant(tolua_S,"e_war_state_attack_ready",faith::e_war_state_attack_ready);
   tolua_constant(tolua_S,"e_war_state_attack_in",faith::e_war_state_attack_in);
   tolua_constant(tolua_S,"e_war_state_max",faith::e_war_state_max);
   tolua_constant(tolua_S,"e_unit_attack_state_dead",faith::e_unit_attack_state_dead);
   tolua_constant(tolua_S,"e_unit_attack_state_dizzy",faith::e_unit_attack_state_dizzy);
   tolua_constant(tolua_S,"e_unit_attack_state_frozen",faith::e_unit_attack_state_frozen);
   tolua_constant(tolua_S,"e_unit_attack_state_attack_back",faith::e_unit_attack_state_attack_back);
   tolua_constant(tolua_S,"e_unit_attack_state_no_control",faith::e_unit_attack_state_no_control);
   tolua_constant(tolua_S,"e_unit_attack_state_no_hurt",faith::e_unit_attack_state_no_hurt);
   tolua_constant(tolua_S,"e_unit_attack_state_stealth",faith::e_unit_attack_state_stealth);
   tolua_constant(tolua_S,"e_unit_attack_state_no_dead",faith::e_unit_attack_state_no_dead);
   tolua_constant(tolua_S,"e_unit_attack_state_no_skill",faith::e_unit_attack_state_no_skill);
   tolua_constant(tolua_S,"e_unit_attack_state_adversity_aid",faith::e_unit_attack_state_adversity_aid);
   tolua_constant(tolua_S,"e_unit_attack_state_speed_down",faith::e_unit_attack_state_speed_down);
   tolua_constant(tolua_S,"e_unit_attack_state_speed_up",faith::e_unit_attack_state_speed_up);
   tolua_constant(tolua_S,"e_unit_attack_state_stealth_show_me",faith::e_unit_attack_state_stealth_show_me);
   tolua_constant(tolua_S,"e_unit_attack_state_bleeding",faith::e_unit_attack_state_bleeding);
   tolua_constant(tolua_S,"e_unit_attack_state_burn",faith::e_unit_attack_state_burn);
   tolua_constant(tolua_S,"e_unit_attack_state_adsorbent",faith::e_unit_attack_state_adsorbent);
   tolua_constant(tolua_S,"e_unit_attack_state_crazy",faith::e_unit_attack_state_crazy);
   tolua_constant(tolua_S,"e_unit_attack_state_lock_cd",faith::e_unit_attack_state_lock_cd);
   tolua_constant(tolua_S,"e_unit_attack_state_will_dead",faith::e_unit_attack_state_will_dead);
   tolua_constant(tolua_S,"e_unit_attack_state_will_dead_percent",faith::e_unit_attack_state_will_dead_percent);
   tolua_constant(tolua_S,"e_unit_attack_state_max",faith::e_unit_attack_state_max);
   tolua_constant(tolua_S,"e_pawn_att_type_attack",faith::e_pawn_att_type_attack);
   tolua_constant(tolua_S,"e_pawn_att_type_game",faith::e_pawn_att_type_game);
   tolua_constant(tolua_S,"e_pawn_att_type_state",faith::e_pawn_att_type_state);
   tolua_constant(tolua_S,"e_pawn_att_type_sign",faith::e_pawn_att_type_sign);
   tolua_constant(tolua_S,"e_pawn_att_type_max",faith::e_pawn_att_type_max);
   tolua_constant(tolua_S,"e_class_type_none",faith::e_class_type_none);
   tolua_constant(tolua_S,"e_class_type_barserker",faith::e_class_type_barserker);
   tolua_constant(tolua_S,"e_class_type_wizard",faith::e_class_type_wizard);
   tolua_constant(tolua_S,"e_class_type_guardian",faith::e_class_type_guardian);
   tolua_constant(tolua_S,"e_class_type_assassinator",faith::e_class_type_assassinator);
   tolua_constant(tolua_S,"e_class_type_ranger",faith::e_class_type_ranger);
   tolua_constant(tolua_S,"e_class_type_monster",faith::e_class_type_monster);
   tolua_constant(tolua_S,"e_class_type_max",faith::e_class_type_max);
   tolua_constant(tolua_S,"e_branch_type_common",faith::e_branch_type_common);
   tolua_constant(tolua_S,"e_branch_type_ice",faith::e_branch_type_ice);
   tolua_constant(tolua_S,"e_branch_type_fire",faith::e_branch_type_fire);
   tolua_constant(tolua_S,"e_branch_type_light",faith::e_branch_type_light);
   tolua_constant(tolua_S,"e_branch_type_dark",faith::e_branch_type_dark);
   tolua_constant(tolua_S,"e_branch_type_max",faith::e_branch_type_max);
   tolua_constant(tolua_S,"e_map_record_info_map_template_id",faith::e_map_record_info_map_template_id);
   tolua_constant(tolua_S,"e_map_record_info_unlock",faith::e_map_record_info_unlock);
   tolua_constant(tolua_S,"e_map_record_info_enter_count",faith::e_map_record_info_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_fastest_time",faith::e_map_record_info_fastest_time);
   tolua_constant(tolua_S,"e_map_record_info_extra_enter_count",faith::e_map_record_info_extra_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_score_record",faith::e_map_record_info_score_record);
   tolua_constant(tolua_S,"e_map_record_info_real_enter_count",faith::e_map_record_info_real_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_buy_count",faith::e_map_record_info_buy_count);
   tolua_constant(tolua_S,"e_map_record_info_use_things_count",faith::e_map_record_info_use_things_count);
   tolua_constant(tolua_S,"e_map_record_info_first_join",faith::e_map_record_info_first_join);
   tolua_constant(tolua_S,"e_map_record_info_first_award",faith::e_map_record_info_first_award);
   tolua_constant(tolua_S,"e_map_record_info_next_refresh_time",faith::e_map_record_info_next_refresh_time);
   tolua_constant(tolua_S,"e_map_record_info_mopping_num",faith::e_map_record_info_mopping_num);
   tolua_constant(tolua_S,"e_map_record_info_all_enter_count",faith::e_map_record_info_all_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_max",faith::e_map_record_info_max);
   tolua_constant(tolua_S,"e_big_player_type_lord_of_city",faith::e_big_player_type_lord_of_city);
   tolua_constant(tolua_S,"e_big_player_type_lord_or_war",faith::e_big_player_type_lord_or_war);
   tolua_constant(tolua_S,"e_big_player_type_lord_or_sanctuary",faith::e_big_player_type_lord_or_sanctuary);
   tolua_constant(tolua_S,"e_big_player_type_lord_or_god",faith::e_big_player_type_lord_or_god);
   tolua_constant(tolua_S,"e_big_player_type_best_barserker",faith::e_big_player_type_best_barserker);
   tolua_constant(tolua_S,"e_big_player_type_best_wizard",faith::e_big_player_type_best_wizard);
   tolua_constant(tolua_S,"e_big_player_type_best_guardian",faith::e_big_player_type_best_guardian);
   tolua_constant(tolua_S,"e_big_player_type_best_assassinator",faith::e_big_player_type_best_assassinator);
   tolua_constant(tolua_S,"e_big_player_type_overload_legion",faith::e_big_player_type_overload_legion);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_barserker",faith::e_big_player_type_gs_first_barserker);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_wizard",faith::e_big_player_type_gs_first_wizard);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_guardian",faith::e_big_player_type_gs_first_guardian);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_assassinator",faith::e_big_player_type_gs_first_assassinator);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_player",faith::e_big_player_type_gs_first_player);
   tolua_constant(tolua_S,"e_big_player_type_money_first_player",faith::e_big_player_type_money_first_player);
   tolua_constant(tolua_S,"e_big_player_type_worship_first_player",faith::e_big_player_type_worship_first_player);
   tolua_constant(tolua_S,"e_big_player_type_arena_first_player",faith::e_big_player_type_arena_first_player);
   tolua_constant(tolua_S,"e_big_player_type_max",faith::e_big_player_type_max);
   tolua_constant(tolua_S,"e_map_state_join",faith::e_map_state_join);
   tolua_constant(tolua_S,"e_map_state_game",faith::e_map_state_game);
   tolua_constant(tolua_S,"e_map_state_in_game",faith::e_map_state_in_game);
   tolua_constant(tolua_S,"e_map_state_over",faith::e_map_state_over);
   tolua_constant(tolua_S,"e_map_state_return_data",faith::e_map_state_return_data);
   tolua_constant(tolua_S,"e_map_state_max",faith::e_map_state_max);
   tolua_constant(tolua_S,"e_bag_type_none",faith::e_bag_type_none);
   tolua_constant(tolua_S,"e_bag_type_equip",faith::e_bag_type_equip);
   tolua_constant(tolua_S,"e_bag_type_bag",faith::e_bag_type_bag);
   tolua_constant(tolua_S,"e_bag_type_wing",faith::e_bag_type_wing);
   tolua_constant(tolua_S,"e_bag_type_mount",faith::e_bag_type_mount);
   tolua_constant(tolua_S,"e_bag_type_protect",faith::e_bag_type_protect);
   tolua_constant(tolua_S,"e_bag_type_equip_protect",faith::e_bag_type_equip_protect);
   tolua_constant(tolua_S,"e_bag_type_mail",faith::e_bag_type_mail);
   tolua_constant(tolua_S,"e_bag_type_hope_item",faith::e_bag_type_hope_item);
   tolua_constant(tolua_S,"e_bag_type_elemenet",faith::e_bag_type_elemenet);
   tolua_constant(tolua_S,"e_bag_type_equip_elemenet",faith::e_bag_type_equip_elemenet);
   tolua_constant(tolua_S,"e_bag_type_fasion",faith::e_bag_type_fasion);
   tolua_constant(tolua_S,"e_bag_type_equip_fasion",faith::e_bag_type_equip_fasion);
   tolua_constant(tolua_S,"e_bag_type_mount_beast_spirit",faith::e_bag_type_mount_beast_spirit);
   tolua_constant(tolua_S,"e_bag_type_wing_beast_spirit",faith::e_bag_type_wing_beast_spirit);
   tolua_constant(tolua_S,"e_bag_type_treasure",faith::e_bag_type_treasure);
   tolua_constant(tolua_S,"e_bag_type_storage",faith::e_bag_type_storage);
   tolua_constant(tolua_S,"e_bag_type_belief_rune",faith::e_bag_type_belief_rune);
   tolua_constant(tolua_S,"e_bag_type_belief_rune_equip",faith::e_bag_type_belief_rune_equip);
   tolua_constant(tolua_S,"e_bag_type_equip_spirit",faith::e_bag_type_equip_spirit);
   tolua_constant(tolua_S,"e_bag_type_equip_tinder_prop",faith::e_bag_type_equip_tinder_prop);
   tolua_constant(tolua_S,"e_bag_type_sky_equip",faith::e_bag_type_sky_equip);
   tolua_constant(tolua_S,"e_bag_type_skill_book",faith::e_bag_type_skill_book);
   tolua_constant(tolua_S,"e_bag_type_goddess_equip",faith::e_bag_type_goddess_equip);
   tolua_constant(tolua_S,"e_bag_type_wedding_equip",faith::e_bag_type_wedding_equip);
   tolua_constant(tolua_S,"e_bag_type_core_element",faith::e_bag_type_core_element);
   tolua_constant(tolua_S,"e_bag_type_spirit_beast_spirit",faith::e_bag_type_spirit_beast_spirit);
   tolua_constant(tolua_S,"e_bag_type_skygod_equip",faith::e_bag_type_skygod_equip);
   tolua_constant(tolua_S,"e_bag_type_supreme_equip",faith::e_bag_type_supreme_equip);
   tolua_constant(tolua_S,"e_bag_type_spirit_upgrade_promote",faith::e_bag_type_spirit_upgrade_promote);
   tolua_constant(tolua_S,"e_bag_type_break_will",faith::e_bag_type_break_will);
   tolua_constant(tolua_S,"e_bag_type_awaken_item",faith::e_bag_type_awaken_item);
   tolua_constant(tolua_S,"e_bag_type_online_award",faith::e_bag_type_online_award);
   tolua_constant(tolua_S,"e_bag_type_max",faith::e_bag_type_max);
   tolua_constant(tolua_S,"e_item_info_info_id",faith::e_item_info_info_id);
   tolua_constant(tolua_S,"e_item_info_slot",faith::e_item_info_slot);
   tolua_constant(tolua_S,"e_item_info_equip_slot",faith::e_item_info_equip_slot);
   tolua_constant(tolua_S,"e_item_info_container_type",faith::e_item_info_container_type);
   tolua_constant(tolua_S,"e_item_info_stack_count",faith::e_item_info_stack_count);
   tolua_constant(tolua_S,"e_item_info_locked",faith::e_item_info_locked);
   tolua_constant(tolua_S,"e_item_info_activate",faith::e_item_info_activate);
   tolua_constant(tolua_S,"e_item_info_is_first",faith::e_item_info_is_first);
   tolua_constant(tolua_S,"e_item_info_upgrade_count",faith::e_item_info_upgrade_count);
   tolua_constant(tolua_S,"e_item_info_stargrade_count",faith::e_item_info_stargrade_count);
   tolua_constant(tolua_S,"e_item_info_starskill_flag",faith::e_item_info_starskill_flag);
   tolua_constant(tolua_S,"e_item_info_add_on",faith::e_item_info_add_on);
   tolua_constant(tolua_S,"e_item_info_wing_exp",faith::e_item_info_wing_exp);
   tolua_constant(tolua_S,"e_item_info_wing_bless",faith::e_item_info_wing_bless);
   tolua_constant(tolua_S,"e_item_info_random_had_flag",faith::e_item_info_random_had_flag);
   tolua_constant(tolua_S,"e_item_info_random_property1",faith::e_item_info_random_property1);
   tolua_constant(tolua_S,"e_item_info_random_property2",faith::e_item_info_random_property2);
   tolua_constant(tolua_S,"e_item_info_random_property3",faith::e_item_info_random_property3);
   tolua_constant(tolua_S,"e_item_info_random_property4",faith::e_item_info_random_property4);
   tolua_constant(tolua_S,"e_item_info_random_property5",faith::e_item_info_random_property5);
   tolua_constant(tolua_S,"e_item_info_random_property6",faith::e_item_info_random_property6);
   tolua_constant(tolua_S,"e_item_info_succinct_property1",faith::e_item_info_succinct_property1);
   tolua_constant(tolua_S,"e_item_info_succinct_property2",faith::e_item_info_succinct_property2);
   tolua_constant(tolua_S,"e_item_info_succinct_property3",faith::e_item_info_succinct_property3);
   tolua_constant(tolua_S,"e_item_info_succinct_property4",faith::e_item_info_succinct_property4);
   tolua_constant(tolua_S,"e_item_info_succinct_property5",faith::e_item_info_succinct_property5);
   tolua_constant(tolua_S,"e_item_info_succinct_property6",faith::e_item_info_succinct_property6);
   tolua_constant(tolua_S,"e_item_info_showing_illusion_idex",faith::e_item_info_showing_illusion_idex);
   tolua_constant(tolua_S,"e_item_info_put_in_bag_count",faith::e_item_info_put_in_bag_count);
   tolua_constant(tolua_S,"e_item_info_illusion_had_byte",faith::e_item_info_illusion_had_byte);
   tolua_constant(tolua_S,"e_item_info_over_time",faith::e_item_info_over_time);
   tolua_constant(tolua_S,"e_item_info_succinct_level",faith::e_item_info_succinct_level);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_0",faith::e_item_info_jewel_slot_0);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_1",faith::e_item_info_jewel_slot_1);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_2",faith::e_item_info_jewel_slot_2);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_3",faith::e_item_info_jewel_slot_3);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_4",faith::e_item_info_jewel_slot_4);
   tolua_constant(tolua_S,"e_item_info_jewel_vip_slot_0",faith::e_item_info_jewel_vip_slot_0);
   tolua_constant(tolua_S,"e_item_info_jewel_vip_slot_1",faith::e_item_info_jewel_vip_slot_1);
   tolua_constant(tolua_S,"e_item_info_prototype_id",faith::e_item_info_prototype_id);
   tolua_constant(tolua_S,"e_item_info_forge_property1",faith::e_item_info_forge_property1);
   tolua_constant(tolua_S,"e_item_info_forge_property2",faith::e_item_info_forge_property2);
   tolua_constant(tolua_S,"e_item_info_forge_property3",faith::e_item_info_forge_property3);
   tolua_constant(tolua_S,"e_item_info_forge_property4",faith::e_item_info_forge_property4);
   tolua_constant(tolua_S,"e_item_info_forge_property5",faith::e_item_info_forge_property5);
   tolua_constant(tolua_S,"e_item_info_forge_property6",faith::e_item_info_forge_property6);
   tolua_constant(tolua_S,"e_item_info_jewel_carve_0",faith::e_item_info_jewel_carve_0);
   tolua_constant(tolua_S,"e_item_info_jewel_carve_1",faith::e_item_info_jewel_carve_1);
   tolua_constant(tolua_S,"e_item_info_jewel_carve_2",faith::e_item_info_jewel_carve_2);
   tolua_constant(tolua_S,"e_item_info_jewel_lucky_carve",faith::e_item_info_jewel_lucky_carve);
   tolua_constant(tolua_S,"e_item_info_max",faith::e_item_info_max);
   tolua_constant(tolua_S,"e_item_lock_all",faith::e_item_lock_all);
   tolua_constant(tolua_S,"e_item_lock_lock",faith::e_item_lock_lock);
   tolua_constant(tolua_S,"e_item_lock_un_lock",faith::e_item_lock_un_lock);
   tolua_constant(tolua_S,"e_item_lock_max",faith::e_item_lock_max);
   tolua_constant(tolua_S,"e_item_operation_none",faith::e_item_operation_none);
   tolua_constant(tolua_S,"e_item_operation_equipon",faith::e_item_operation_equipon);
   tolua_constant(tolua_S,"e_item_operation_equipoff",faith::e_item_operation_equipoff);
   tolua_constant(tolua_S,"e_item_operation_weaponactivate",faith::e_item_operation_weaponactivate);
   tolua_constant(tolua_S,"e_item_operation_weaponfirst",faith::e_item_operation_weaponfirst);
   tolua_constant(tolua_S,"e_item_operation_levelup",faith::e_item_operation_levelup);
   tolua_constant(tolua_S,"e_item_operation_synthesis",faith::e_item_operation_synthesis);
   tolua_constant(tolua_S,"e_item_operation_sell",faith::e_item_operation_sell);
   tolua_constant(tolua_S,"e_item_operation_addon",faith::e_item_operation_addon);
   tolua_constant(tolua_S,"e_item_operation_recovery",faith::e_item_operation_recovery);
   tolua_constant(tolua_S,"e_item_operation_use",faith::e_item_operation_use);
   tolua_constant(tolua_S,"e_item_operation_wing_upgrade",faith::e_item_operation_wing_upgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_starupgrade",faith::e_item_operation_wing_starupgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_illusionupgrade",faith::e_item_operation_wing_illusionupgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_addsoul",faith::e_item_operation_wing_addsoul);
   tolua_constant(tolua_S,"e_item_operation_wing_addspirit",faith::e_item_operation_wing_addspirit);
   tolua_constant(tolua_S,"e_item_operation_featherupgrade",faith::e_item_operation_featherupgrade);
   tolua_constant(tolua_S,"e_item_operation_tidy",faith::e_item_operation_tidy);
   tolua_constant(tolua_S,"e_item_operation_inherit",faith::e_item_operation_inherit);
   tolua_constant(tolua_S,"e_item_operation_advanced",faith::e_item_operation_advanced);
   tolua_constant(tolua_S,"e_item_operation_spirit_upgrade",faith::e_item_operation_spirit_upgrade);
   tolua_constant(tolua_S,"e_item_operation_split",faith::e_item_operation_split);
   tolua_constant(tolua_S,"e_item_operation_spirit_send_out",faith::e_item_operation_spirit_send_out);
   tolua_constant(tolua_S,"e_item_operation_spirit_recovery",faith::e_item_operation_spirit_recovery);
   tolua_constant(tolua_S,"e_item_operation_godassembly",faith::e_item_operation_godassembly);
   tolua_constant(tolua_S,"e_item_operation_succinct_gold",faith::e_item_operation_succinct_gold);
   tolua_constant(tolua_S,"e_item_operation_succinct_diamond",faith::e_item_operation_succinct_diamond);
   tolua_constant(tolua_S,"e_item_operation_save_succinct_result",faith::e_item_operation_save_succinct_result);
   tolua_constant(tolua_S,"e_item_operation_show_this_wing",faith::e_item_operation_show_this_wing);
   tolua_constant(tolua_S,"e_item_operation_mount_unlock",faith::e_item_operation_mount_unlock);
   tolua_constant(tolua_S,"e_item_operation_mount_upgrade",faith::e_item_operation_mount_upgrade);
   tolua_constant(tolua_S,"e_item_operation_mount_starupgrade",faith::e_item_operation_mount_starupgrade);
   tolua_constant(tolua_S,"e_item_operation_mount_illusionupgrade",faith::e_item_operation_mount_illusionupgrade);
   tolua_constant(tolua_S,"e_item_operation_mount_show_this_mount",faith::e_item_operation_mount_show_this_mount);
   tolua_constant(tolua_S,"e_item_operation_mount_show_this_mount_illusion",faith::e_item_operation_mount_show_this_mount_illusion);
   tolua_constant(tolua_S,"e_item_operation_mount_get_illusion",faith::e_item_operation_mount_get_illusion);
   tolua_constant(tolua_S,"e_item_operation_retrieve_psyche",faith::e_item_operation_retrieve_psyche);
   tolua_constant(tolua_S,"e_item_operation_put_into_bag",faith::e_item_operation_put_into_bag);
   tolua_constant(tolua_S,"e_item_operation_tidy_hope_item",faith::e_item_operation_tidy_hope_item);
   tolua_constant(tolua_S,"e_item_operation_featherupgrade_star",faith::e_item_operation_featherupgrade_star);
   tolua_constant(tolua_S,"e_item_operation_featherupgrade_grade",faith::e_item_operation_featherupgrade_grade);
   tolua_constant(tolua_S,"e_item_operation_spirit_had",faith::e_item_operation_spirit_had);
   tolua_constant(tolua_S,"e_item_operation_wing_shape_upgrade",faith::e_item_operation_wing_shape_upgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_shape_unlock",faith::e_item_operation_wing_shape_unlock);
   tolua_constant(tolua_S,"e_item_operation_wing_shape_illusion_unlock",faith::e_item_operation_wing_shape_illusion_unlock);
   tolua_constant(tolua_S,"e_item_operation_one_key_use",faith::e_item_operation_one_key_use);
   tolua_constant(tolua_S,"e_item_operation_quickly_hp",faith::e_item_operation_quickly_hp);
   tolua_constant(tolua_S,"e_item_operation_against_buy",faith::e_item_operation_against_buy);
   tolua_constant(tolua_S,"e_item_operation_buy_and_use",faith::e_item_operation_buy_and_use);
   tolua_constant(tolua_S,"e_item_operation_use_beast_spirit",faith::e_item_operation_use_beast_spirit);
   tolua_constant(tolua_S,"e_item_operation_set_jewel_on",faith::e_item_operation_set_jewel_on);
   tolua_constant(tolua_S,"e_item_operation_set_jewel_off",faith::e_item_operation_set_jewel_off);
   tolua_constant(tolua_S,"e_item_operation_jewel_upgrade",faith::e_item_operation_jewel_upgrade);
   tolua_constant(tolua_S,"e_item_operation_tidy_treasure_item",faith::e_item_operation_tidy_treasure_item);
   tolua_constant(tolua_S,"e_item_operation_get_treasure_item",faith::e_item_operation_get_treasure_item);
   tolua_constant(tolua_S,"e_item_operation_godassembled",faith::e_item_operation_godassembled);
   tolua_constant(tolua_S,"e_item_operation_tidy_storage_item",faith::e_item_operation_tidy_storage_item);
   tolua_constant(tolua_S,"e_item_operation_fashion_upgrade",faith::e_item_operation_fashion_upgrade);
   tolua_constant(tolua_S,"e_item_operation_forge",faith::e_item_operation_forge);
   tolua_constant(tolua_S,"e_item_operation_n_succinct_gold",faith::e_item_operation_n_succinct_gold);
   tolua_constant(tolua_S,"e_item_operation_n_succinct_diamond",faith::e_item_operation_n_succinct_diamond);
   tolua_constant(tolua_S,"e_item_operation_enchant",faith::e_item_operation_enchant);
   tolua_constant(tolua_S,"e_item_operation_unenchant",faith::e_item_operation_unenchant);
   tolua_constant(tolua_S,"e_item_operation_wing_zhuling",faith::e_item_operation_wing_zhuling);
   tolua_constant(tolua_S,"e_item_operation_wing_lengque",faith::e_item_operation_wing_lengque);
   tolua_constant(tolua_S,"e_item_operation_wing_xiling",faith::e_item_operation_wing_xiling);
   tolua_constant(tolua_S,"e_item_operation_spirit_upgrade_promote",faith::e_item_operation_spirit_upgrade_promote);
   tolua_constant(tolua_S,"e_item_operation_break_will_activate",faith::e_item_operation_break_will_activate);
   tolua_constant(tolua_S,"e_item_operation_awaken_fetter",faith::e_item_operation_awaken_fetter);
   tolua_constant(tolua_S,"e_item_operation_awaken_fetter_skill",faith::e_item_operation_awaken_fetter_skill);
   tolua_constant(tolua_S,"e_item_operation_max",faith::e_item_operation_max);
   tolua_constant(tolua_S,"e_item_composit_type_none",faith::e_item_composit_type_none);
   tolua_constant(tolua_S,"e_item_composit_type_element",faith::e_item_composit_type_element);
   tolua_constant(tolua_S,"e_item_composit_type_equip",faith::e_item_composit_type_equip);
   tolua_constant(tolua_S,"e_item_composit_type_sprite",faith::e_item_composit_type_sprite);
   tolua_constant(tolua_S,"e_item_composit_type_max",faith::e_item_composit_type_max);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_0",faith::e_welfare_random_item_daily_online_0);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_1",faith::e_welfare_random_item_daily_online_1);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_2",faith::e_welfare_random_item_daily_online_2);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_3",faith::e_welfare_random_item_daily_online_3);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_4",faith::e_welfare_random_item_daily_online_4);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_5",faith::e_welfare_random_item_daily_online_5);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_6",faith::e_welfare_random_item_daily_online_6);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_7",faith::e_welfare_random_item_daily_online_7);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_0",faith::e_welfare_random_item_continue_login_0);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_1",faith::e_welfare_random_item_continue_login_1);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_2",faith::e_welfare_random_item_continue_login_2);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_3",faith::e_welfare_random_item_continue_login_3);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_4",faith::e_welfare_random_item_continue_login_4);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_5",faith::e_welfare_random_item_continue_login_5);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_6",faith::e_welfare_random_item_continue_login_6);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_7",faith::e_welfare_random_item_continue_login_7);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_8",faith::e_welfare_random_item_continue_login_8);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_9",faith::e_welfare_random_item_continue_login_9);
   tolua_constant(tolua_S,"e_welfare_random_item_max",faith::e_welfare_random_item_max);
   tolua_constant(tolua_S,"e_item_use_type_can_not_use",faith::e_item_use_type_can_not_use);
   tolua_constant(tolua_S,"e_item_use_type_can_use",faith::e_item_use_type_can_use);
   tolua_constant(tolua_S,"e_item_use_type_can_batch_use",faith::e_item_use_type_can_batch_use);
   tolua_constant(tolua_S,"e_item_use_type_only_jump",faith::e_item_use_type_only_jump);
   tolua_constant(tolua_S,"e_item_use_type_max",faith::e_item_use_type_max);
   tolua_constant(tolua_S,"e_item_color_white",faith::e_item_color_white);
   tolua_constant(tolua_S,"e_item_color_green",faith::e_item_color_green);
   tolua_constant(tolua_S,"e_item_color_blue",faith::e_item_color_blue);
   tolua_constant(tolua_S,"e_item_color_purple",faith::e_item_color_purple);
   tolua_constant(tolua_S,"e_item_color_orange",faith::e_item_color_orange);
   tolua_constant(tolua_S,"e_item_color_red",faith::e_item_color_red);
   tolua_constant(tolua_S,"e_item_color_pink",faith::e_item_color_pink);
   tolua_constant(tolua_S,"e_item_color_max",faith::e_item_color_max);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_gs",faith::e_tem_melting_sort_type_gs);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_upgrade",faith::e_tem_melting_sort_type_upgrade);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_addon",faith::e_tem_melting_sort_type_addon);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_max",faith::e_tem_melting_sort_type_max);
   tolua_constant(tolua_S,"e_unit_type_null",faith::e_unit_type_null);
   tolua_constant(tolua_S,"e_unit_type_player",faith::e_unit_type_player);
   tolua_constant(tolua_S,"e_unit_type_npc",faith::e_unit_type_npc);
   tolua_constant(tolua_S,"e_unit_type_monster",faith::e_unit_type_monster);
   tolua_constant(tolua_S,"e_unit_type_drop_bag",faith::e_unit_type_drop_bag);
   tolua_constant(tolua_S,"e_unit_type_trap",faith::e_unit_type_trap);
   tolua_constant(tolua_S,"e_unit_type_friend_npc",faith::e_unit_type_friend_npc);
   tolua_constant(tolua_S,"e_unit_type_summoned",faith::e_unit_type_summoned);
   tolua_constant(tolua_S,"e_unit_type_hide",faith::e_unit_type_hide);
   tolua_constant(tolua_S,"e_unit_type_award_mark",faith::e_unit_type_award_mark);
   tolua_constant(tolua_S,"e_unit_type_award_play",faith::e_unit_type_award_play);
   tolua_constant(tolua_S,"e_unit_type_max",faith::e_unit_type_max);
   tolua_constant(tolua_S,"e_npc_type_mission",faith::e_npc_type_mission);
   tolua_constant(tolua_S,"e_npc_type_grade_up",faith::e_npc_type_grade_up);
   tolua_constant(tolua_S,"e_npc_type_strong_hold",faith::e_npc_type_strong_hold);
   tolua_constant(tolua_S,"e_npc_type_gather_crystal",faith::e_npc_type_gather_crystal);
   tolua_constant(tolua_S,"e_npc_type_lord_city",faith::e_npc_type_lord_city);
   tolua_constant(tolua_S,"e_npc_type_lord_pk",faith::e_npc_type_lord_pk);
   tolua_constant(tolua_S,"e_npc_type_lord_sanctuary",faith::e_npc_type_lord_sanctuary);
   tolua_constant(tolua_S,"e_npc_type_lord_god",faith::e_npc_type_lord_god);
   tolua_constant(tolua_S,"e_npc_type_protect_npc",faith::e_npc_type_protect_npc);
   tolua_constant(tolua_S,"e_npc_type_interact_gather",faith::e_npc_type_interact_gather);
   tolua_constant(tolua_S,"e_npc_type_touch_gather",faith::e_npc_type_touch_gather);
   tolua_constant(tolua_S,"e_npc_type_hit_and_get_npc",faith::e_npc_type_hit_and_get_npc);
   tolua_constant(tolua_S,"e_npc_type_marry_npc",faith::e_npc_type_marry_npc);
   tolua_constant(tolua_S,"e_npc_type_divorce_npc",faith::e_npc_type_divorce_npc);
   tolua_constant(tolua_S,"e_npc_type_world_boss_chests",faith::e_npc_type_world_boss_chests);
   tolua_constant(tolua_S,"e_npc_type_island_big_chests",faith::e_npc_type_island_big_chests);
   tolua_constant(tolua_S,"e_npc_type_island_small_chests",faith::e_npc_type_island_small_chests);
   tolua_constant(tolua_S,"e_npc_type_cross_pk_chests",faith::e_npc_type_cross_pk_chests);
   tolua_constant(tolua_S,"e_npc_type_server_harry_chests",faith::e_npc_type_server_harry_chests);
   tolua_constant(tolua_S,"e_npc_type_server_harry_submit",faith::e_npc_type_server_harry_submit);
   tolua_constant(tolua_S,"e_npc_type_marry_task",faith::e_npc_type_marry_task);
   tolua_constant(tolua_S,"e_npc_type_marry_exam",faith::e_npc_type_marry_exam);
   tolua_constant(tolua_S,"e_npc_type_attack_city_npc",faith::e_npc_type_attack_city_npc);
   tolua_constant(tolua_S,"e_npc_type_transfiguration_npc",faith::e_npc_type_transfiguration_npc);
   tolua_constant(tolua_S,"e_npc_type_transfer",faith::e_npc_type_transfer);
   tolua_constant(tolua_S,"e_npc_type_max",faith::e_npc_type_max);
   tolua_constant(tolua_S,"e_monster_type_nomal",faith::e_monster_type_nomal);
   tolua_constant(tolua_S,"e_monster_type_elite",faith::e_monster_type_elite);
   tolua_constant(tolua_S,"e_monster_type_boss",faith::e_monster_type_boss);
   tolua_constant(tolua_S,"e_monster_type_world_boss",faith::e_monster_type_world_boss);
   tolua_constant(tolua_S,"e_monster_type_gold_army",faith::e_monster_type_gold_army);
   tolua_constant(tolua_S,"e_monster_type_player_data",faith::e_monster_type_player_data);
   tolua_constant(tolua_S,"e_monster_type_legion_bonfire",faith::e_monster_type_legion_bonfire);
   tolua_constant(tolua_S,"e_monster_type_robot_player",faith::e_monster_type_robot_player);
   tolua_constant(tolua_S,"e_monster_type_award_boss",faith::e_monster_type_award_boss);
   tolua_constant(tolua_S,"e_monster_type_max",faith::e_monster_type_max);
   tolua_constant(tolua_S,"e_hide_type_null",faith::e_hide_type_null);
   tolua_constant(tolua_S,"e_hide_type_fire",faith::e_hide_type_fire);
   tolua_constant(tolua_S,"e_hide_type_snowstorm",faith::e_hide_type_snowstorm);
   tolua_constant(tolua_S,"e_hide_type_max",faith::e_hide_type_max);
   tolua_constant(tolua_S,"e_award_mark_type_nomal",faith::e_award_mark_type_nomal);
   tolua_constant(tolua_S,"e_award_mark_type_treasure_pic",faith::e_award_mark_type_treasure_pic);
   tolua_constant(tolua_S,"e_award_mark_type_special",faith::e_award_mark_type_special);
   tolua_constant(tolua_S,"e_award_mark_type_bank_robbery",faith::e_award_mark_type_bank_robbery);
   tolua_constant(tolua_S,"e_award_mark_type_refrush_monster",faith::e_award_mark_type_refrush_monster);
   tolua_constant(tolua_S,"e_award_mark_type_finger_guess",faith::e_award_mark_type_finger_guess);
   tolua_constant(tolua_S,"e_award_mark_type_smashing_bottle",faith::e_award_mark_type_smashing_bottle);
   tolua_constant(tolua_S,"e_award_mark_type_max",faith::e_award_mark_type_max);
   tolua_constant(tolua_S,"e_award_play_type_null",faith::e_award_play_type_null);
   tolua_constant(tolua_S,"e_award_play_type_bank_robbery_box",faith::e_award_play_type_bank_robbery_box);
   tolua_constant(tolua_S,"e_award_play_type_smashing_bottle",faith::e_award_play_type_smashing_bottle);
   tolua_constant(tolua_S,"e_award_play_type_smashing_bottle_guard",faith::e_award_play_type_smashing_bottle_guard);
   tolua_constant(tolua_S,"e_award_play_type_smashing_bottle_award",faith::e_award_play_type_smashing_bottle_award);
   tolua_constant(tolua_S,"e_award_play_type_max",faith::e_award_play_type_max);
   tolua_constant(tolua_S,"e_drop_bag_type_nomal",faith::e_drop_bag_type_nomal);
   tolua_constant(tolua_S,"e_drop_bag_type_award",faith::e_drop_bag_type_award);
   tolua_constant(tolua_S,"e_drop_bag_type_bank_robbery_box",faith::e_drop_bag_type_bank_robbery_box);
   tolua_constant(tolua_S,"e_drop_bag_type_smashing_bottle",faith::e_drop_bag_type_smashing_bottle);
   tolua_constant(tolua_S,"e_drop_bag_type_max",faith::e_drop_bag_type_max);
   tolua_constant(tolua_S,"e_box_award_type_null",faith::e_box_award_type_null);
   tolua_constant(tolua_S,"e_box_award_type_player_search_level",faith::e_box_award_type_player_search_level);
   tolua_constant(tolua_S,"e_box_award_type_self_drop",faith::e_box_award_type_self_drop);
   tolua_constant(tolua_S,"e_box_award_type_max",faith::e_box_award_type_max);
   tolua_constant(tolua_S,"e_activity_type_temple",faith::e_activity_type_temple);
   tolua_constant(tolua_S,"e_activity_type_empire_treasure",faith::e_activity_type_empire_treasure);
   tolua_constant(tolua_S,"e_activity_type_pk_king",faith::e_activity_type_pk_king);
   tolua_constant(tolua_S,"e_activity_type_legion_bonfire",faith::e_activity_type_legion_bonfire);
   tolua_constant(tolua_S,"e_activity_type_broken_sky",faith::e_activity_type_broken_sky);
   tolua_constant(tolua_S,"e_activity_type_sky_treasure_box",faith::e_activity_type_sky_treasure_box);
   tolua_constant(tolua_S,"e_activity_type_legion_boss",faith::e_activity_type_legion_boss);
   tolua_constant(tolua_S,"e_activity_type_city_war",faith::e_activity_type_city_war);
   tolua_constant(tolua_S,"e_activity_type_crystal_fairyland",faith::e_activity_type_crystal_fairyland);
   tolua_constant(tolua_S,"e_activity_type_overlord_city_war",faith::e_activity_type_overlord_city_war);
   tolua_constant(tolua_S,"e_activity_type_world_elite",faith::e_activity_type_world_elite);
   tolua_constant(tolua_S,"e_activity_type_world_boss",faith::e_activity_type_world_boss);
   tolua_constant(tolua_S,"e_activity_type_ancient_field",faith::e_activity_type_ancient_field);
   tolua_constant(tolua_S,"e_activity_type_boss_field",faith::e_activity_type_boss_field);
   tolua_constant(tolua_S,"e_activity_type_boss_home",faith::e_activity_type_boss_home);
   tolua_constant(tolua_S,"e_activity_type_boss_single",faith::e_activity_type_boss_single);
   tolua_constant(tolua_S,"e_activity_type_cloud_shop",faith::e_activity_type_cloud_shop);
   tolua_constant(tolua_S,"e_activity_type_belief_cloister",faith::e_activity_type_belief_cloister);
   tolua_constant(tolua_S,"e_activity_type_cross_server_pk",faith::e_activity_type_cross_server_pk);
   tolua_constant(tolua_S,"e_activity_type_cross_server_harry",faith::e_activity_type_cross_server_harry);
   tolua_constant(tolua_S,"e_activity_type_cross_ladder",faith::e_activity_type_cross_ladder);
   tolua_constant(tolua_S,"e_activity_type_cross_server_world_boss",faith::e_activity_type_cross_server_world_boss);
   tolua_constant(tolua_S,"e_activity_type_city_war_cross_server",faith::e_activity_type_city_war_cross_server);
   tolua_constant(tolua_S,"e_activity_type_legion_answer",faith::e_activity_type_legion_answer);
   tolua_constant(tolua_S,"e_activity_type_legion_dance",faith::e_activity_type_legion_dance);
   tolua_constant(tolua_S,"e_activity_type_element_war",faith::e_activity_type_element_war);
   tolua_constant(tolua_S,"e_activity_type_attack_city",faith::e_activity_type_attack_city);
   tolua_constant(tolua_S,"e_activity_type_max",faith::e_activity_type_max);
   tolua_constant(tolua_S,"e_widget_jump_img_id",faith::e_widget_jump_img_id);
   tolua_constant(tolua_S,"e_widget_jump_describe_id",faith::e_widget_jump_describe_id);
   tolua_constant(tolua_S,"e_widget_jump_widget_id",faith::e_widget_jump_widget_id);
   tolua_constant(tolua_S,"e_widget_jump_second_index",faith::e_widget_jump_second_index);
   tolua_constant(tolua_S,"e_widget_jump_third_index",faith::e_widget_jump_third_index);
   tolua_constant(tolua_S,"e_widget_jump_max",faith::e_widget_jump_max);
   tolua_constant(tolua_S,"e_logout_result_self",faith::e_logout_result_self);
   tolua_constant(tolua_S,"e_logout_result_load_data_fail",faith::e_logout_result_load_data_fail);
   tolua_constant(tolua_S,"e_logout_result_save_data_fail",faith::e_logout_result_save_data_fail);
   tolua_constant(tolua_S,"e_logout_result_link_fail",faith::e_logout_result_link_fail);
   tolua_constant(tolua_S,"e_logout_result_time_out",faith::e_logout_result_time_out);
   tolua_constant(tolua_S,"e_logout_result_net_lost",faith::e_logout_result_net_lost);
   tolua_constant(tolua_S,"e_logout_result_ban_account",faith::e_logout_result_ban_account);
   tolua_constant(tolua_S,"e_logout_result_new_login",faith::e_logout_result_new_login);
   tolua_constant(tolua_S,"e_logout_result_replaced_by_new_login",faith::e_logout_result_replaced_by_new_login);
   tolua_constant(tolua_S,"e_logout_result_enter_scene_failed",faith::e_logout_result_enter_scene_failed);
   tolua_constant(tolua_S,"e_logout_result_leave_scene_failed",faith::e_logout_result_leave_scene_failed);
   tolua_constant(tolua_S,"e_logout_result_cs_crash",faith::e_logout_result_cs_crash);
   tolua_constant(tolua_S,"e_logout_result_ls_crash",faith::e_logout_result_ls_crash);
   tolua_constant(tolua_S,"e_logout_result_kick_by_rpc",faith::e_logout_result_kick_by_rpc);
   tolua_constant(tolua_S,"e_logout_result_no_find_player",faith::e_logout_result_no_find_player);
   tolua_constant(tolua_S,"e_logout_result_fep_id_error",faith::e_logout_result_fep_id_error);
   tolua_constant(tolua_S,"e_logout_result_connect_dis",faith::e_logout_result_connect_dis);
   tolua_constant(tolua_S,"e_logout_result_begin_time",faith::e_logout_result_begin_time);
   tolua_constant(tolua_S,"e_logout_result_password_error",faith::e_logout_result_password_error);
   tolua_constant(tolua_S,"e_logout_result_version_wrong",faith::e_logout_result_version_wrong);
   tolua_constant(tolua_S,"e_logout_result_account_error",faith::e_logout_result_account_error);
   tolua_constant(tolua_S,"e_logout_result_queue_full",faith::e_logout_result_queue_full);
   tolua_constant(tolua_S,"e_logout_result_login_full",faith::e_logout_result_login_full);
   tolua_constant(tolua_S,"e_logout_result_bi",faith::e_logout_result_bi);
   tolua_constant(tolua_S,"e_logout_result_fake_server_full",faith::e_logout_result_fake_server_full);
   tolua_constant(tolua_S,"e_logout_result_reconnect_account",faith::e_logout_result_reconnect_account);
   tolua_constant(tolua_S,"e_logout_result_login_game_need_update",faith::e_logout_result_login_game_need_update);
   tolua_constant(tolua_S,"e_logout_result_enter_game_server_maintenance",faith::e_logout_result_enter_game_server_maintenance);
   tolua_constant(tolua_S,"e_logout_result_enter_game_no_net",faith::e_logout_result_enter_game_no_net);
   tolua_constant(tolua_S,"e_logout_result_online_time_limit",faith::e_logout_result_online_time_limit);
   tolua_constant(tolua_S,"e_logout_result_online_duration_over",faith::e_logout_result_online_duration_over);
   tolua_constant(tolua_S,"e_logout_result_only_create_role",faith::e_logout_result_only_create_role);
   tolua_constant(tolua_S,"e_logout_result_max",faith::e_logout_result_max);
   tolua_constant(tolua_S,"e_reconnect_result_win",faith::e_reconnect_result_win);
   tolua_constant(tolua_S,"e_reconnect_result_fep_no_find",faith::e_reconnect_result_fep_no_find);
   tolua_constant(tolua_S,"e_reconnect_result_ws_no_find",faith::e_reconnect_result_ws_no_find);
   tolua_constant(tolua_S,"e_reconnect_result_cs_no_find",faith::e_reconnect_result_cs_no_find);
   tolua_constant(tolua_S,"e_reconnect_result_max",faith::e_reconnect_result_max);
   tolua_constant(tolua_S,"e_demons_tower_info_cur_challenge_tier",faith::e_demons_tower_info_cur_challenge_tier);
   tolua_constant(tolua_S,"e_demons_tower_info_left_mopping_up_count",faith::e_demons_tower_info_left_mopping_up_count);
   tolua_constant(tolua_S,"e_demons_tower_info_cur_mopping_up_tier",faith::e_demons_tower_info_cur_mopping_up_tier);
   tolua_constant(tolua_S,"e_demons_tower_info_can_get_award",faith::e_demons_tower_info_can_get_award);
   tolua_constant(tolua_S,"e_demons_tower_info_max",faith::e_demons_tower_info_max);
   tolua_constant(tolua_S,"e_map_category_common",faith::e_map_category_common);
   tolua_constant(tolua_S,"e_map_category_raid",faith::e_map_category_raid);
   tolua_constant(tolua_S,"e_map_category_activity",faith::e_map_category_activity);
   tolua_constant(tolua_S,"e_map_category_max",faith::e_map_category_max);
   tolua_constant(tolua_S,"e_money_type_exp",faith::e_money_type_exp);
   tolua_constant(tolua_S,"e_money_type_treasure_pic",faith::e_money_type_treasure_pic);
   tolua_constant(tolua_S,"e_money_type_silver_bind",faith::e_money_type_silver_bind);
   tolua_constant(tolua_S,"e_money_type_jewel",faith::e_money_type_jewel);
   tolua_constant(tolua_S,"e_money_type_jewel_bind",faith::e_money_type_jewel_bind);
   tolua_constant(tolua_S,"e_money_type_boss_progress",faith::e_money_type_boss_progress);
   tolua_constant(tolua_S,"e_money_type_magic_crystal",faith::e_money_type_magic_crystal);
   tolua_constant(tolua_S,"e_money_type_battle_key",faith::e_money_type_battle_key);
   tolua_constant(tolua_S,"e_money_type_skill_active",faith::e_money_type_skill_active);
   tolua_constant(tolua_S,"e_money_type_skill_upgrade",faith::e_money_type_skill_upgrade);
   tolua_constant(tolua_S,"e_money_type_pound",faith::e_money_type_pound);
   tolua_constant(tolua_S,"e_money_type_guard_point",faith::e_money_type_guard_point);
   tolua_constant(tolua_S,"e_money_type_achievement_point",faith::e_money_type_achievement_point);
   tolua_constant(tolua_S,"e_money_type_reputation",faith::e_money_type_reputation);
   tolua_constant(tolua_S,"e_money_type_hope_point",faith::e_money_type_hope_point);
   tolua_constant(tolua_S,"e_money_type_battle_achievement",faith::e_money_type_battle_achievement);
   tolua_constant(tolua_S,"e_money_type_cross_honor",faith::e_money_type_cross_honor);
   tolua_constant(tolua_S,"e_money_type_talent",faith::e_money_type_talent);
   tolua_constant(tolua_S,"e_money_type_assist_fighting",faith::e_money_type_assist_fighting);
   tolua_constant(tolua_S,"e_money_type_treasure_score",faith::e_money_type_treasure_score);
   tolua_constant(tolua_S,"e_money_type_belief_rune_piece",faith::e_money_type_belief_rune_piece);
   tolua_constant(tolua_S,"e_money_type_legion_warehouse_score",faith::e_money_type_legion_warehouse_score);
   tolua_constant(tolua_S,"e_money_type_attribute_talent",faith::e_money_type_attribute_talent);
   tolua_constant(tolua_S,"e_money_type_world_essence",faith::e_money_type_world_essence);
   tolua_constant(tolua_S,"e_money_type_cross_server_money",faith::e_money_type_cross_server_money);
   tolua_constant(tolua_S,"e_money_type_red_diamond",faith::e_money_type_red_diamond);
   tolua_constant(tolua_S,"e_money_type_feather_piece",faith::e_money_type_feather_piece);
   tolua_constant(tolua_S,"e_money_type_cross_ladder_honor",faith::e_money_type_cross_ladder_honor);
   tolua_constant(tolua_S,"e_money_type_season_point",faith::e_money_type_season_point);
   tolua_constant(tolua_S,"e_money_type_element_score",faith::e_money_type_element_score);
   tolua_constant(tolua_S,"e_money_type_element_point",faith::e_money_type_element_point);
   tolua_constant(tolua_S,"e_money_type_energy",faith::e_money_type_energy);
   tolua_constant(tolua_S,"e_money_type_max",faith::e_money_type_max);
   tolua_constant(tolua_S,"e_team_type_none",faith::e_team_type_none);
   tolua_constant(tolua_S,"e_team_type_one_dragon",faith::e_team_type_one_dragon);
   tolua_constant(tolua_S,"e_team_type_mission_daily",faith::e_team_type_mission_daily);
   tolua_constant(tolua_S,"e_team_type_mission_against",faith::e_team_type_mission_against);
   tolua_constant(tolua_S,"e_team_type_act_world_boss",faith::e_team_type_act_world_boss);
   tolua_constant(tolua_S,"e_team_type_element_war",faith::e_team_type_element_war);
   tolua_constant(tolua_S,"e_team_type_raid_daily_exp",faith::e_team_type_raid_daily_exp);
   tolua_constant(tolua_S,"e_team_type_raid_daily_gold",faith::e_team_type_raid_daily_gold);
   tolua_constant(tolua_S,"e_team_type_raid_multi_0",faith::e_team_type_raid_multi_0);
   tolua_constant(tolua_S,"e_team_type_raid_sky",faith::e_team_type_raid_sky);
   tolua_constant(tolua_S,"e_team_type_raid_temple",faith::e_team_type_raid_temple);
   tolua_constant(tolua_S,"e_team_type_raid_multi_1",faith::e_team_type_raid_multi_1);
   tolua_constant(tolua_S,"e_team_type_raid_treasure",faith::e_team_type_raid_treasure);
   tolua_constant(tolua_S,"e_team_type_raid_multi_2",faith::e_team_type_raid_multi_2);
   tolua_constant(tolua_S,"e_team_type_hang_up",faith::e_team_type_hang_up);
   tolua_constant(tolua_S,"e_team_type_ancient_hang_up",faith::e_team_type_ancient_hang_up);
   tolua_constant(tolua_S,"e_team_type_act_world_elite",faith::e_team_type_act_world_elite);
   tolua_constant(tolua_S,"e_team_type_max",faith::e_team_type_max);
   tolua_constant(tolua_S,"e_skill_info_template_id",faith::e_skill_info_template_id);
   tolua_constant(tolua_S,"e_skill_info_cd_cur_time",faith::e_skill_info_cd_cur_time);
   tolua_constant(tolua_S,"e_skill_info_cd_max_time",faith::e_skill_info_cd_max_time);
   tolua_constant(tolua_S,"e_skill_info_activated",faith::e_skill_info_activated);
   tolua_constant(tolua_S,"e_skill_info_skill_type",faith::e_skill_info_skill_type);
   tolua_constant(tolua_S,"e_skill_info_maturity",faith::e_skill_info_maturity);
   tolua_constant(tolua_S,"e_skill_info_use_pos",faith::e_skill_info_use_pos);
   tolua_constant(tolua_S,"e_skill_info_level",faith::e_skill_info_level);
   tolua_constant(tolua_S,"e_skill_info_max",faith::e_skill_info_max);
   tolua_constant(tolua_S,"e_mission_end_type_npc_kill",faith::e_mission_end_type_npc_kill);
   tolua_constant(tolua_S,"e_mission_end_type_dialog",faith::e_mission_end_type_dialog);
   tolua_constant(tolua_S,"e_mission_end_type_mission",faith::e_mission_end_type_mission);
   tolua_constant(tolua_S,"e_mission_end_type_map",faith::e_mission_end_type_map);
   tolua_constant(tolua_S,"e_mission_end_type_transfer",faith::e_mission_end_type_transfer);
   tolua_constant(tolua_S,"e_mission_end_type_beat_npc",faith::e_mission_end_type_beat_npc);
   tolua_constant(tolua_S,"e_mission_end_type_kill_monster_get_item",faith::e_mission_end_type_kill_monster_get_item);
   tolua_constant(tolua_S,"e_mission_end_type_get_item",faith::e_mission_end_type_get_item);
   tolua_constant(tolua_S,"e_mission_end_type_equip_upgrade",faith::e_mission_end_type_equip_upgrade);
   tolua_constant(tolua_S,"e_mission_end_type_equip_addon",faith::e_mission_end_type_equip_addon);
   tolua_constant(tolua_S,"e_mission_end_type_rune_equip",faith::e_mission_end_type_rune_equip);
   tolua_constant(tolua_S,"e_mission_end_type_map_clear_current",faith::e_mission_end_type_map_clear_current);
   tolua_constant(tolua_S,"e_mission_end_type_activity_clear_current",faith::e_mission_end_type_activity_clear_current);
   tolua_constant(tolua_S,"e_mission_end_type_mount_unlock",faith::e_mission_end_type_mount_unlock);
   tolua_constant(tolua_S,"e_mission_end_type_wing_upgrade",faith::e_mission_end_type_wing_upgrade);
   tolua_constant(tolua_S,"e_mission_end_type_pokedex_activation",faith::e_mission_end_type_pokedex_activation);
   tolua_constant(tolua_S,"e_mission_end_type_sprite_activation",faith::e_mission_end_type_sprite_activation);
   tolua_constant(tolua_S,"e_mission_end_type_character_level",faith::e_mission_end_type_character_level);
   tolua_constant(tolua_S,"e_mission_end_type_map_clear_history",faith::e_mission_end_type_map_clear_history);
   tolua_constant(tolua_S,"e_mission_end_type_clear_daily_mission",faith::e_mission_end_type_clear_daily_mission);
   tolua_constant(tolua_S,"e_mission_end_type_clear_against_mission",faith::e_mission_end_type_clear_against_mission);
   tolua_constant(tolua_S,"e_mission_end_type_clear_demon_tower",faith::e_mission_end_type_clear_demon_tower);
   tolua_constant(tolua_S,"e_mission_end_type_clear_arena",faith::e_mission_end_type_clear_arena);
   tolua_constant(tolua_S,"e_mission_end_type_cheer_worship",faith::e_mission_end_type_cheer_worship);
   tolua_constant(tolua_S,"e_mission_end_type_transform",faith::e_mission_end_type_transform);
   tolua_constant(tolua_S,"e_mission_end_type_equip_levelup",faith::e_mission_end_type_equip_levelup);
   tolua_constant(tolua_S,"e_mission_end_type_add_friend",faith::e_mission_end_type_add_friend);
   tolua_constant(tolua_S,"e_mission_end_type_purchase_goods",faith::e_mission_end_type_purchase_goods);
   tolua_constant(tolua_S,"e_mission_end_type_npc_attacked_num",faith::e_mission_end_type_npc_attacked_num);
   tolua_constant(tolua_S,"e_mission_end_type_join_legion",faith::e_mission_end_type_join_legion);
   tolua_constant(tolua_S,"e_mission_end_type_rune_total_level",faith::e_mission_end_type_rune_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_upgrade_total_level",faith::e_mission_end_type_upgrade_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_add_on_total_level",faith::e_mission_end_type_add_on_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_mount_total_grade",faith::e_mission_end_type_mount_total_grade);
   tolua_constant(tolua_S,"e_mission_end_type_join_legion_boss_map",faith::e_mission_end_type_join_legion_boss_map);
   tolua_constant(tolua_S,"e_mission_end_type_tinder_total_level",faith::e_mission_end_type_tinder_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_equip_target_color",faith::e_mission_end_type_equip_target_color);
   tolua_constant(tolua_S,"e_mission_end_type_legion_donate",faith::e_mission_end_type_legion_donate);
   tolua_constant(tolua_S,"e_mission_end_type_boss_field",faith::e_mission_end_type_boss_field);
   tolua_constant(tolua_S,"e_mission_end_type_grade_levelup",faith::e_mission_end_type_grade_levelup);
   tolua_constant(tolua_S,"e_mission_end_type_upgrade_level_num",faith::e_mission_end_type_upgrade_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_addon_level_num",faith::e_mission_end_type_addon_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_awaken_level_num",faith::e_mission_end_type_awaken_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_succinct_level_num",faith::e_mission_end_type_succinct_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_marry_dance",faith::e_mission_end_type_marry_dance);
   tolua_constant(tolua_S,"e_mission_end_type_reach_level",faith::e_mission_end_type_reach_level);
   tolua_constant(tolua_S,"e_mission_end_type_learning_skill",faith::e_mission_end_type_learning_skill);
   tolua_constant(tolua_S,"e_mission_end_type_arena_military_rank",faith::e_mission_end_type_arena_military_rank);
   tolua_constant(tolua_S,"e_mission_end_type_use_item",faith::e_mission_end_type_use_item);
   tolua_constant(tolua_S,"e_mission_end_type_compound_item",faith::e_mission_end_type_compound_item);
   tolua_constant(tolua_S,"e_mission_end_type_spirit_baptism",faith::e_mission_end_type_spirit_baptism);
   tolua_constant(tolua_S,"e_mission_end_type_spirit_baptism_color",faith::e_mission_end_type_spirit_baptism_color);
   tolua_constant(tolua_S,"e_mission_end_type_inset_genstone",faith::e_mission_end_type_inset_genstone);
   tolua_constant(tolua_S,"e_mission_end_type_belief_all_num",faith::e_mission_end_type_belief_all_num);
   tolua_constant(tolua_S,"e_mission_end_type_kill_home_boss",faith::e_mission_end_type_kill_home_boss);
   tolua_constant(tolua_S,"e_mission_end_type_new_map_finish",faith::e_mission_end_type_new_map_finish);
   tolua_constant(tolua_S,"e_mission_end_type_mount_upgrade_num",faith::e_mission_end_type_mount_upgrade_num);
   tolua_constant(tolua_S,"e_mission_end_type_spirit_upgrade_num",faith::e_mission_end_type_spirit_upgrade_num);
   tolua_constant(tolua_S,"e_mission_end_type_wing_all_star",faith::e_mission_end_type_wing_all_star);
   tolua_constant(tolua_S,"e_mission_end_type_install_equip",faith::e_mission_end_type_install_equip);
   tolua_constant(tolua_S,"e_mission_end_type_unlock_skill_by_mission",faith::e_mission_end_type_unlock_skill_by_mission);
   tolua_constant(tolua_S,"e_mission_end_type_auction_goods",faith::e_mission_end_type_auction_goods);
   tolua_constant(tolua_S,"e_mission_end_type_equip_spirit",faith::e_mission_end_type_equip_spirit);
   tolua_constant(tolua_S,"e_mission_end_type_assis_fight_num",faith::e_mission_end_type_assis_fight_num);
   tolua_constant(tolua_S,"e_mission_end_type_auction_shopping",faith::e_mission_end_type_auction_shopping);
   tolua_constant(tolua_S,"e_mission_end_type_kill_any_home_boss",faith::e_mission_end_type_kill_any_home_boss);
   tolua_constant(tolua_S,"e_mission_end_type_kill_any_single_boss",faith::e_mission_end_type_kill_any_single_boss);
   tolua_constant(tolua_S,"e_mission_end_type_service_goal",faith::e_mission_end_type_service_goal);
   tolua_constant(tolua_S,"e_mission_end_type_total_login",faith::e_mission_end_type_total_login);
   tolua_constant(tolua_S,"e_mission_end_type_operatrion_raid",faith::e_mission_end_type_operatrion_raid);
   tolua_constant(tolua_S,"e_mission_end_type_add_dot",faith::e_mission_end_type_add_dot);
   tolua_constant(tolua_S,"e_mission_end_type_gs_welfare_num",faith::e_mission_end_type_gs_welfare_num);
   tolua_constant(tolua_S,"e_mission_end_type_level_welfare_num",faith::e_mission_end_type_level_welfare_num);
   tolua_constant(tolua_S,"e_mission_end_type_by_assis_fight_num",faith::e_mission_end_type_by_assis_fight_num);
   tolua_constant(tolua_S,"e_mission_end_type_intensify_add_gs",faith::e_mission_end_type_intensify_add_gs);
   tolua_constant(tolua_S,"e_mission_end_type_choose_a_fashion",faith::e_mission_end_type_choose_a_fashion);
   tolua_constant(tolua_S,"e_mission_end_type_active_break_will",faith::e_mission_end_type_active_break_will);
   tolua_constant(tolua_S,"e_mission_end_type_search_count",faith::e_mission_end_type_search_count);
   tolua_constant(tolua_S,"e_mission_end_type_active_skill_count",faith::e_mission_end_type_active_skill_count);
   tolua_constant(tolua_S,"e_mission_end_type_passive_skill_count",faith::e_mission_end_type_passive_skill_count);
   tolua_constant(tolua_S,"e_mission_end_type_active_skill_level",faith::e_mission_end_type_active_skill_level);
   tolua_constant(tolua_S,"e_mission_end_type_max",faith::e_mission_end_type_max);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_0",faith::e_unlock_func_type_skill_0);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_1",faith::e_unlock_func_type_skill_1);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_2",faith::e_unlock_func_type_skill_2);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_3",faith::e_unlock_func_type_skill_3);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_4",faith::e_unlock_func_type_skill_4);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_5",faith::e_unlock_func_type_skill_5);
   tolua_constant(tolua_S,"e_unlock_func_type_auto_battle",faith::e_unlock_func_type_auto_battle);
   tolua_constant(tolua_S,"e_unlock_func_type_mount",faith::e_unlock_func_type_mount);
   tolua_constant(tolua_S,"e_unlock_func_type_equip_upgrade",faith::e_unlock_func_type_equip_upgrade);
   tolua_constant(tolua_S,"e_unlock_func_type_equip_addon",faith::e_unlock_func_type_equip_addon);
   tolua_constant(tolua_S,"e_unlock_func_type_wing",faith::e_unlock_func_type_wing);
   tolua_constant(tolua_S,"e_unlock_func_type_activity",faith::e_unlock_func_type_activity);
   tolua_constant(tolua_S,"e_unlock_func_type_convert",faith::e_unlock_func_type_convert);
   tolua_constant(tolua_S,"e_unlock_func_type_ranking",faith::e_unlock_func_type_ranking);
   tolua_constant(tolua_S,"e_unlock_func_type_achievement",faith::e_unlock_func_type_achievement);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_daily_exp",faith::e_unlock_func_type_raid_daily_exp);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_daily_money",faith::e_unlock_func_type_raid_daily_money);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_demons_tower",faith::e_unlock_func_type_raid_demons_tower);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_multiplayer",faith::e_unlock_func_type_raid_multiplayer);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_sky_battle",faith::e_unlock_func_type_raid_sky_battle);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_drama",faith::e_unlock_func_type_raid_drama);
   tolua_constant(tolua_S,"e_unlock_func_type_max",faith::e_unlock_func_type_max);
   tolua_constant(tolua_S,"e_guide_type_none",faith::e_guide_type_none);
   tolua_constant(tolua_S,"e_guide_type_operate",faith::e_guide_type_operate);
   tolua_constant(tolua_S,"e_guide_type_open_func",faith::e_guide_type_open_func);
   tolua_constant(tolua_S,"e_guide_type_mask",faith::e_guide_type_mask);
   tolua_constant(tolua_S,"e_guide_type_cinematics_anim",faith::e_guide_type_cinematics_anim);
   tolua_constant(tolua_S,"e_guide_type_trigger_event",faith::e_guide_type_trigger_event);
   tolua_constant(tolua_S,"e_guide_type_max",faith::e_guide_type_max);
   tolua_constant(tolua_S,"e_time_type_login_time",faith::e_time_type_login_time);
   tolua_constant(tolua_S,"e_time_type_logout_time",faith::e_time_type_logout_time);
   tolua_constant(tolua_S,"e_time_type_login_time_all",faith::e_time_type_login_time_all);
   tolua_constant(tolua_S,"e_time_type_last_save_data_time",faith::e_time_type_last_save_data_time);
   tolua_constant(tolua_S,"e_time_type_last_refresh_time",faith::e_time_type_last_refresh_time);
   tolua_constant(tolua_S,"e_time_type_spirit_last_free_hunt_time",faith::e_time_type_spirit_last_free_hunt_time);
   tolua_constant(tolua_S,"e_time_type_arena_last_update",faith::e_time_type_arena_last_update);
   tolua_constant(tolua_S,"e_time_type_arena_colling_time",faith::e_time_type_arena_colling_time);
   tolua_constant(tolua_S,"e_time_type_hope_item_free_get_time",faith::e_time_type_hope_item_free_get_time);
   tolua_constant(tolua_S,"e_time_type_next_free_change_gold_time",faith::e_time_type_next_free_change_gold_time);
   tolua_constant(tolua_S,"e_time_type_have_meditation_time",faith::e_time_type_have_meditation_time);
   tolua_constant(tolua_S,"e_time_type_month_card_activity_time",faith::e_time_type_month_card_activity_time);
   tolua_constant(tolua_S,"e_time_type_month_card_remain_time",faith::e_time_type_month_card_remain_time);
   tolua_constant(tolua_S,"e_time_type_bag_time",faith::e_time_type_bag_time);
   tolua_constant(tolua_S,"e_time_type_last_daily_info_refresh_time",faith::e_time_type_last_daily_info_refresh_time);
   tolua_constant(tolua_S,"e_time_type_ancient_battelfield",faith::e_time_type_ancient_battelfield);
   tolua_constant(tolua_S,"e_time_type_last_map_record_update_time",faith::e_time_type_last_map_record_update_time);
   tolua_constant(tolua_S,"e_time_type_cur_level_time",faith::e_time_type_cur_level_time);
   tolua_constant(tolua_S,"e_time_type_legion_daily_award_flags",faith::e_time_type_legion_daily_award_flags);
   tolua_constant(tolua_S,"e_time_type_convert_last_update_time",faith::e_time_type_convert_last_update_time);
   tolua_constant(tolua_S,"e_time_type_arena_last_get_reward_time",faith::e_time_type_arena_last_get_reward_time);
   tolua_constant(tolua_S,"e_time_type_map_auto_revive_time",faith::e_time_type_map_auto_revive_time);
   tolua_constant(tolua_S,"e_time_type_exclusive_card_activity_time",faith::e_time_type_exclusive_card_activity_time);
   tolua_constant(tolua_S,"e_time_type_last_month_info_refresh_time",faith::e_time_type_last_month_info_refresh_time);
   tolua_constant(tolua_S,"e_time_type_welfare_last_refresh_time",faith::e_time_type_welfare_last_refresh_time);
   tolua_constant(tolua_S,"e_time_type_active_degree_last_refresh_time",faith::e_time_type_active_degree_last_refresh_time);
   tolua_constant(tolua_S,"e_time_type_meditation_full_send_mail",faith::e_time_type_meditation_full_send_mail);
   tolua_constant(tolua_S,"e_time_type_legion_bonus_award_flags",faith::e_time_type_legion_bonus_award_flags);
   tolua_constant(tolua_S,"e_time_type_vip_experience_level_time",faith::e_time_type_vip_experience_level_time);
   tolua_constant(tolua_S,"e_time_type_treasure_jewel_free",faith::e_time_type_treasure_jewel_free);
   tolua_constant(tolua_S,"e_time_type_treasure_element_free",faith::e_time_type_treasure_element_free);
   tolua_constant(tolua_S,"e_time_type_treasure_spirit_free",faith::e_time_type_treasure_spirit_free);
   tolua_constant(tolua_S,"e_time_type_storage_time",faith::e_time_type_storage_time);
   tolua_constant(tolua_S,"e_time_type_meditation_remaining_time",faith::e_time_type_meditation_remaining_time);
   tolua_constant(tolua_S,"e_time_type_last_refresh_by_zero_no_cross",faith::e_time_type_last_refresh_by_zero_no_cross);
   tolua_constant(tolua_S,"e_time_type_next_world_boss_can_count",faith::e_time_type_next_world_boss_can_count);
   tolua_constant(tolua_S,"e_time_type_last_harry_clear_time",faith::e_time_type_last_harry_clear_time);
   tolua_constant(tolua_S,"e_time_type_last_refresh_by_zero_cross",faith::e_time_type_last_refresh_by_zero_cross);
   tolua_constant(tolua_S,"e_time_type_last_refresh_gs_up_start_value_time",faith::e_time_type_last_refresh_gs_up_start_value_time);
   tolua_constant(tolua_S,"e_time_type_frist_recharge_time",faith::e_time_type_frist_recharge_time);
   tolua_constant(tolua_S,"e_time_type_exp_raid_join_time",faith::e_time_type_exp_raid_join_time);
   tolua_constant(tolua_S,"e_time_type_exp_raid_skill_cd",faith::e_time_type_exp_raid_skill_cd);
   tolua_constant(tolua_S,"e_time_type_exp_raid_extra_time",faith::e_time_type_exp_raid_extra_time);
   tolua_constant(tolua_S,"e_time_type_service_goal_close_time",faith::e_time_type_service_goal_close_time);
   tolua_constant(tolua_S,"e_time_type_first_recharge_open_time",faith::e_time_type_first_recharge_open_time);
   tolua_constant(tolua_S,"e_time_type_fuben_level_buy_tip_time",faith::e_time_type_fuben_level_buy_tip_time);
   tolua_constant(tolua_S,"e_time_type_money_buy_tip_time",faith::e_time_type_money_buy_tip_time);
   tolua_constant(tolua_S,"e_time_type_red_convert_last_time",faith::e_time_type_red_convert_last_time);
   tolua_constant(tolua_S,"e_time_type_energy_back",faith::e_time_type_energy_back);
   tolua_constant(tolua_S,"e_time_type_offline_award_time",faith::e_time_type_offline_award_time);
   tolua_constant(tolua_S,"e_time_type_max",faith::e_time_type_max);
   tolua_constant(tolua_S,"e_title_info_template_id",faith::e_title_info_template_id);
   tolua_constant(tolua_S,"e_title_info_start_time",faith::e_title_info_start_time);
   tolua_constant(tolua_S,"e_title_info_end_time",faith::e_title_info_end_time);
   tolua_constant(tolua_S,"e_title_info_if_geted",faith::e_title_info_if_geted);
   tolua_constant(tolua_S,"e_title_info_if_need_see",faith::e_title_info_if_need_see);
   tolua_constant(tolua_S,"e_title_info_if_need_equip",faith::e_title_info_if_need_equip);
   tolua_constant(tolua_S,"e_title_info_max",faith::e_title_info_max);
   tolua_constant(tolua_S,"e_title_end_type_forever",faith::e_title_end_type_forever);
   tolua_constant(tolua_S,"e_title_end_type_time",faith::e_title_end_type_time);
   tolua_constant(tolua_S,"e_title_end_type_condition_1",faith::e_title_end_type_condition_1);
   tolua_constant(tolua_S,"e_title_end_type_condition_2",faith::e_title_end_type_condition_2);
   tolua_constant(tolua_S,"e_title_end_type_max",faith::e_title_end_type_max);
   tolua_constant(tolua_S,"e_buff_add_judge_non",faith::e_buff_add_judge_non);
   tolua_constant(tolua_S,"e_buff_add_judge_has_it",faith::e_buff_add_judge_has_it);
   tolua_constant(tolua_S,"e_buff_add_judge_has_stronger_one",faith::e_buff_add_judge_has_stronger_one);
   tolua_constant(tolua_S,"e_buff_add_judge_can_cover",faith::e_buff_add_judge_can_cover);
   tolua_constant(tolua_S,"e_buff_add_judge_can_wrap",faith::e_buff_add_judge_can_wrap);
   tolua_constant(tolua_S,"e_buff_add_judge_can_cumulative",faith::e_buff_add_judge_can_cumulative);
   tolua_constant(tolua_S,"e_buff_add_judge_direct_add",faith::e_buff_add_judge_direct_add);
   tolua_constant(tolua_S,"e_buff_add_judge_can_percent",faith::e_buff_add_judge_can_percent);
   tolua_constant(tolua_S,"e_buff_add_judge_max",faith::e_buff_add_judge_max);
   tolua_constant(tolua_S,"e_spirit_info_template_id",faith::e_spirit_info_template_id);
   tolua_constant(tolua_S,"e_spirit_info_spirit_slot",faith::e_spirit_info_spirit_slot);
   tolua_constant(tolua_S,"e_spirit_info_container_type",faith::e_spirit_info_container_type);
   tolua_constant(tolua_S,"e_spirit_info_spirit_is_equiped",faith::e_spirit_info_spirit_is_equiped);
   tolua_constant(tolua_S,"e_spirit_info_spirit_is_fighting",faith::e_spirit_info_spirit_is_fighting);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_1",faith::e_spirit_info_flair_property_1);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_2",faith::e_spirit_info_flair_property_2);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_3",faith::e_spirit_info_flair_property_3);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_4",faith::e_spirit_info_flair_property_4);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_5",faith::e_spirit_info_flair_property_5);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_6",faith::e_spirit_info_flair_property_6);
   tolua_constant(tolua_S,"e_spirit_info_spirit_typ",faith::e_spirit_info_spirit_typ);
   tolua_constant(tolua_S,"e_spirit_info_showing_illusion_idex",faith::e_spirit_info_showing_illusion_idex);
   tolua_constant(tolua_S,"e_spirit_info_illusion_had_byte",faith::e_spirit_info_illusion_had_byte);
   tolua_constant(tolua_S,"e_spirit_info_upgrade_count",faith::e_spirit_info_upgrade_count);
   tolua_constant(tolua_S,"e_spirit_info_star_upgrade_count",faith::e_spirit_info_star_upgrade_count);
   tolua_constant(tolua_S,"e_spirit_info_spirit_exp",faith::e_spirit_info_spirit_exp);
   tolua_constant(tolua_S,"e_spirit_info_random_has_flag",faith::e_spirit_info_random_has_flag);
   tolua_constant(tolua_S,"e_spirit_info_star_skill_flag",faith::e_spirit_info_star_skill_flag);
   tolua_constant(tolua_S,"e_spirit_info_efficient_skill_id",faith::e_spirit_info_efficient_skill_id);
   tolua_constant(tolua_S,"e_spirit_info_efficient_skill_id_2",faith::e_spirit_info_efficient_skill_id_2);
   tolua_constant(tolua_S,"e_spirit_info_max",faith::e_spirit_info_max);
   tolua_constant(tolua_S,"e_spirit_fetter_info_template_id",faith::e_spirit_fetter_info_template_id);
   tolua_constant(tolua_S,"e_spirit_fetter_info_upgrade_level",faith::e_spirit_fetter_info_upgrade_level);
   tolua_constant(tolua_S,"e_spirit_fetter_info_max",faith::e_spirit_fetter_info_max);
   tolua_constant(tolua_S,"e_spirit_bag_type_equiped",faith::e_spirit_bag_type_equiped);
   tolua_constant(tolua_S,"e_spirit_bag_type_spiritbag",faith::e_spirit_bag_type_spiritbag);
   tolua_constant(tolua_S,"e_spirit_bag_type_itembag",faith::e_spirit_bag_type_itembag);
   tolua_constant(tolua_S,"e_spirit_bag_type_max",faith::e_spirit_bag_type_max);
   tolua_constant(tolua_S,"e_spirit_hunt_type_free",faith::e_spirit_hunt_type_free);
   tolua_constant(tolua_S,"e_spirit_hunt_type_by_voucher",faith::e_spirit_hunt_type_by_voucher);
   tolua_constant(tolua_S,"e_spirit_hunt_type_by_ten_voucher",faith::e_spirit_hunt_type_by_ten_voucher);
   tolua_constant(tolua_S,"e_spirit_hunt_type_one_by_cost",faith::e_spirit_hunt_type_one_by_cost);
   tolua_constant(tolua_S,"e_spirit_hunt_type_ten",faith::e_spirit_hunt_type_ten);
   tolua_constant(tolua_S,"e_spirit_hunt_type_max",faith::e_spirit_hunt_type_max);
   tolua_constant(tolua_S,"e_map_difficulty_easy",faith::e_map_difficulty_easy);
   tolua_constant(tolua_S,"e_map_difficulty_normal",faith::e_map_difficulty_normal);
   tolua_constant(tolua_S,"e_map_difficulty_heard",faith::e_map_difficulty_heard);
   tolua_constant(tolua_S,"e_map_difficulty_max",faith::e_map_difficulty_max);
   tolua_constant(tolua_S,"e_ranking_worship_type_normal",faith::e_ranking_worship_type_normal);
   tolua_constant(tolua_S,"e_ranking_worship_type_money",faith::e_ranking_worship_type_money);
   tolua_constant(tolua_S,"e_ranking_worship_type_max",faith::e_ranking_worship_type_max);
   tolua_constant(tolua_S,"e_pk_mode_peace",faith::e_pk_mode_peace);
   tolua_constant(tolua_S,"e_pk_mode_good_and_evil",faith::e_pk_mode_good_and_evil);
   tolua_constant(tolua_S,"e_pk_mode_team",faith::e_pk_mode_team);
   tolua_constant(tolua_S,"e_pk_mode_family",faith::e_pk_mode_family);
   tolua_constant(tolua_S,"e_pk_mode_slaughter",faith::e_pk_mode_slaughter);
   tolua_constant(tolua_S,"e_pk_mode_cross_sever",faith::e_pk_mode_cross_sever);
   tolua_constant(tolua_S,"e_pk_mode_plunder",faith::e_pk_mode_plunder);
   tolua_constant(tolua_S,"e_pk_mode_max",faith::e_pk_mode_max);
   tolua_constant(tolua_S,"e_pk_mode_type_cant",faith::e_pk_mode_type_cant);
   tolua_constant(tolua_S,"e_pk_mode_type_allow",faith::e_pk_mode_type_allow);
   tolua_constant(tolua_S,"e_pk_mode_type_no_kill_point",faith::e_pk_mode_type_no_kill_point);
   tolua_constant(tolua_S,"e_pk_mode_type_max",faith::e_pk_mode_type_max);
   tolua_constant(tolua_S,"e_recurit_type_none",faith::e_recurit_type_none);
   tolua_constant(tolua_S,"e_recurit_type_legion_recurit",faith::e_recurit_type_legion_recurit);
   tolua_constant(tolua_S,"e_recurit_type_world_recurit",faith::e_recurit_type_world_recurit);
   tolua_constant(tolua_S,"e_recurit_type_max",faith::e_recurit_type_max);
   tolua_constant(tolua_S,"e_fruit_type_none",faith::e_fruit_type_none);
   tolua_constant(tolua_S,"e_fruit_type_strength",faith::e_fruit_type_strength);
   tolua_constant(tolua_S,"e_fruit_type_agility",faith::e_fruit_type_agility);
   tolua_constant(tolua_S,"e_fruit_type_intellect",faith::e_fruit_type_intellect);
   tolua_constant(tolua_S,"e_fruit_type_stamina",faith::e_fruit_type_stamina);
   tolua_constant(tolua_S,"e_fruit_type_max",faith::e_fruit_type_max);
   tolua_constant(tolua_S,"e_welfare_type_online_time",faith::e_welfare_type_online_time);
   tolua_constant(tolua_S,"e_welfare_type_total_login",faith::e_welfare_type_total_login);
   tolua_constant(tolua_S,"e_welfare_type_continue_login",faith::e_welfare_type_continue_login);
   tolua_constant(tolua_S,"e_welfare_type_daily_activity_reward",faith::e_welfare_type_daily_activity_reward);
   tolua_constant(tolua_S,"e_welfare_type_monthcard_reward",faith::e_welfare_type_monthcard_reward);
   tolua_constant(tolua_S,"e_welfare_type_daily_recharge_reward",faith::e_welfare_type_daily_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_total_recharge_reward",faith::e_welfare_type_total_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_total_cost_reward",faith::e_welfare_type_total_cost_reward);
   tolua_constant(tolua_S,"e_welfare_type_exp_level_reward",faith::e_welfare_type_exp_level_reward);
   tolua_constant(tolua_S,"e_welfare_type_gs_reward",faith::e_welfare_type_gs_reward);
   tolua_constant(tolua_S,"e_welfare_type_first_recharge_reward",faith::e_welfare_type_first_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_first_recharge_geted",faith::e_welfare_type_first_recharge_geted);
   tolua_constant(tolua_S,"e_welfare_type_regular_check_in",faith::e_welfare_type_regular_check_in);
   tolua_constant(tolua_S,"e_welfare_type_exclusive_check_in",faith::e_welfare_type_exclusive_check_in);
   tolua_constant(tolua_S,"e_welfare_type_vip_double_check_in",faith::e_welfare_type_vip_double_check_in);
   tolua_constant(tolua_S,"e_welfare_type_is_today_check_in",faith::e_welfare_type_is_today_check_in);
   tolua_constant(tolua_S,"e_welfare_type_replace_times",faith::e_welfare_type_replace_times);
   tolua_constant(tolua_S,"e_welfare_type_time_limit_reward",faith::e_welfare_type_time_limit_reward);
   tolua_constant(tolua_S,"e_welfare_type_cumulative_sign_in",faith::e_welfare_type_cumulative_sign_in);
   tolua_constant(tolua_S,"e_welfare_type_weekend_recharge_reward",faith::e_welfare_type_weekend_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_max",faith::e_welfare_type_max);
   tolua_constant(tolua_S,"e_red_package_type_globel",faith::e_red_package_type_globel);
   tolua_constant(tolua_S,"e_red_package_type_legion",faith::e_red_package_type_legion);
   tolua_constant(tolua_S,"e_red_package_type_vip",faith::e_red_package_type_vip);
   tolua_constant(tolua_S,"e_red_package_type_max",faith::e_red_package_type_max);
   tolua_constant(tolua_S,"e_role_ws_info_save_time",faith::e_role_ws_info_save_time);
   tolua_constant(tolua_S,"e_role_ws_info_free_red_package_count",faith::e_role_ws_info_free_red_package_count);
   tolua_constant(tolua_S,"e_role_ws_info_max",faith::e_role_ws_info_max);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_levelup",faith::e_power_up_prompt_mount_levelup);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_upgrade",faith::e_power_up_prompt_mount_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_unlock",faith::e_power_up_prompt_mount_unlock);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_levelup",faith::e_power_up_prompt_wing_levelup);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_upgrade",faith::e_power_up_prompt_wing_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_shengling",faith::e_power_up_prompt_wing_shengling);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_yuling",faith::e_power_up_prompt_wing_yuling);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_juhun",faith::e_power_up_prompt_wing_juhun);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_upgrade",faith::e_power_up_prompt_equip_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_add_on",faith::e_power_up_prompt_equip_add_on);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_jinjie",faith::e_power_up_prompt_equip_jinjie);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_xilian",faith::e_power_up_prompt_equip_xilian);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_chongzhu",faith::e_power_up_prompt_equip_chongzhu);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_shenzhuangzaizao",faith::e_power_up_prompt_equip_shenzhuangzaizao);
   tolua_constant(tolua_S,"e_power_up_prompt_exchange_amulet",faith::e_power_up_prompt_exchange_amulet);
   tolua_constant(tolua_S,"e_power_up_prompt_raise_achievement",faith::e_power_up_prompt_raise_achievement);
   tolua_constant(tolua_S,"e_power_up_prompt_raise_rank",faith::e_power_up_prompt_raise_rank);
   tolua_constant(tolua_S,"e_power_up_prompt_raise_sprite",faith::e_power_up_prompt_raise_sprite);
   tolua_constant(tolua_S,"e_power_up_prompt_role_add_point",faith::e_power_up_prompt_role_add_point);
   tolua_constant(tolua_S,"e_power_up_prompt_add_tinder",faith::e_power_up_prompt_add_tinder);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_beast_spirit",faith::e_power_up_prompt_mount_beast_spirit);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_beast_spirit",faith::e_power_up_prompt_wing_beast_spirit);
   tolua_constant(tolua_S,"e_power_up_prompt_jewel_set",faith::e_power_up_prompt_jewel_set);
   tolua_constant(tolua_S,"e_power_up_prompt_jewel_composit",faith::e_power_up_prompt_jewel_composit);
   tolua_constant(tolua_S,"e_power_up_prompt_level_package",faith::e_power_up_prompt_level_package);
   tolua_constant(tolua_S,"e_power_up_prompt_gs_package",faith::e_power_up_prompt_gs_package);
   tolua_constant(tolua_S,"e_power_up_prompt_god_assembled",faith::e_power_up_prompt_god_assembled);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_illusion",faith::e_power_up_prompt_mount_illusion);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_star_grade",faith::e_power_up_prompt_mount_star_grade);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_illusion",faith::e_power_up_prompt_wing_illusion);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_star_grade",faith::e_power_up_prompt_wing_star_grade);
   tolua_constant(tolua_S,"e_power_up_prompt_fighting_power_analysis",faith::e_power_up_prompt_fighting_power_analysis);
   tolua_constant(tolua_S,"e_power_up_prompt_element_power_up",faith::e_power_up_prompt_element_power_up);
   tolua_constant(tolua_S,"e_power_up_prompt_tinder_skill",faith::e_power_up_prompt_tinder_skill);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_activate",faith::e_power_up_prompt_feather_activate);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_upgrade",faith::e_power_up_prompt_feather_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_skillbook_upgrade",faith::e_power_up_prompt_feather_skillbook_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_skillbook_EquipOn",faith::e_power_up_prompt_feather_skillbook_EquipOn);
   tolua_constant(tolua_S,"e_power_up_prompt_Phantom_upgrade",faith::e_power_up_prompt_Phantom_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_goddess",faith::e_power_up_prompt_goddess);
   tolua_constant(tolua_S,"e_power_up_prompt_goddess_chain",faith::e_power_up_prompt_goddess_chain);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_star",faith::e_power_up_prompt_mount_star);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_psychic",faith::e_power_up_prompt_wing_psychic);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_awaken",faith::e_power_up_prompt_equip_awaken);
   tolua_constant(tolua_S,"e_power_up_prompt_max",faith::e_power_up_prompt_max);
   tolua_constant(tolua_S,"e_sound_type_sound",faith::e_sound_type_sound);
   tolua_constant(tolua_S,"e_sound_type_music",faith::e_sound_type_music);
   tolua_constant(tolua_S,"e_sound_type_max",faith::e_sound_type_max);
   tolua_constant(tolua_S,"e_pk_king_competition_stage_preliminary",faith::e_pk_king_competition_stage_preliminary);
   tolua_constant(tolua_S,"e_pk_king_competition_stage_final",faith::e_pk_king_competition_stage_final);
   tolua_constant(tolua_S,"e_pk_king_competition_stage_max",faith::e_pk_king_competition_stage_max);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_monster",faith::e_small_map_show_unit_type_monster);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_boss",faith::e_small_map_show_unit_type_boss);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_partner",faith::e_small_map_show_unit_type_partner);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_target",faith::e_small_map_show_unit_type_target);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_target_in_map_border",faith::e_small_map_show_unit_type_target_in_map_border);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_city_war_born_point",faith::e_small_map_show_unit_type_city_war_born_point);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_world_boss",faith::e_small_map_show_unit_type_world_boss);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_max",faith::e_small_map_show_unit_type_max);
   tolua_constant(tolua_S,"e_money_tuple_id",faith::e_money_tuple_id);
   tolua_constant(tolua_S,"e_money_tuple_num",faith::e_money_tuple_num);
   tolua_constant(tolua_S,"e_money_tuple_max",faith::e_money_tuple_max);
   tolua_constant(tolua_S,"e_item_tuple_id",faith::e_item_tuple_id);
   tolua_constant(tolua_S,"e_item_tuple_num",faith::e_item_tuple_num);
   tolua_constant(tolua_S,"e_item_tuple_max",faith::e_item_tuple_max);
   tolua_constant(tolua_S,"e_recruit_activites_type_fuben",faith::e_recruit_activites_type_fuben);
   tolua_constant(tolua_S,"e_recruit_activites_type_hunting",faith::e_recruit_activites_type_hunting);
   tolua_constant(tolua_S,"e_recruit_activites_type_max",faith::e_recruit_activites_type_max);
   tolua_constant(tolua_S,"e_widget_show_mode_normal",faith::e_widget_show_mode_normal);
   tolua_constant(tolua_S,"e_widget_show_mode_OnlyDeleteBySelf",faith::e_widget_show_mode_OnlyDeleteBySelf);
   tolua_constant(tolua_S,"e_widget_show_mode_HiddenInGuide",faith::e_widget_show_mode_HiddenInGuide);
   tolua_constant(tolua_S,"e_widget_show_mode_OnlyDeleteBySelfAndHiddenInGuide",faith::e_widget_show_mode_OnlyDeleteBySelfAndHiddenInGuide);
   tolua_constant(tolua_S,"e_widget_show_mode_max",faith::e_widget_show_mode_max);
   tolua_constant(tolua_S,"e_arena_end_type_failed",faith::e_arena_end_type_failed);
   tolua_constant(tolua_S,"e_arena_end_type_win",faith::e_arena_end_type_win);
   tolua_constant(tolua_S,"e_arena_end_type_draw",faith::e_arena_end_type_draw);
   tolua_constant(tolua_S,"e_arena_end_type_max",faith::e_arena_end_type_max);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_wing_add_star",faith::e_item_upgrade_effect_type_wing_add_star);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_wing_upgrade",faith::e_item_upgrade_effect_type_wing_upgrade);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_mount_upgrade",faith::e_item_upgrade_effect_type_mount_upgrade);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_mount_upgrade_class",faith::e_item_upgrade_effect_type_mount_upgrade_class);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_fashion_upgrade",faith::e_item_upgrade_effect_type_fashion_upgrade);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_max",faith::e_item_upgrade_effect_type_max);
   tolua_constant(tolua_S,"e_map_info_panel_type_none",faith::e_map_info_panel_type_none);
   tolua_constant(tolua_S,"e_map_info_panel_type_wave",faith::e_map_info_panel_type_wave);
   tolua_constant(tolua_S,"e_map_info_panel_type_damage_rank",faith::e_map_info_panel_type_damage_rank);
   tolua_constant(tolua_S,"e_map_info_panel_type_boss_buff",faith::e_map_info_panel_type_boss_buff);
   tolua_constant(tolua_S,"e_map_info_panel_type_final_boss",faith::e_map_info_panel_type_final_boss);
   tolua_constant(tolua_S,"e_map_info_panel_type_protect_npc",faith::e_map_info_panel_type_protect_npc);
   tolua_constant(tolua_S,"e_map_info_panel_type_wave_timer",faith::e_map_info_panel_type_wave_timer);
   tolua_constant(tolua_S,"e_map_info_panel_type_score_rank",faith::e_map_info_panel_type_score_rank);
   tolua_constant(tolua_S,"e_map_info_panel_type_score_get_reward",faith::e_map_info_panel_type_score_get_reward);
   tolua_constant(tolua_S,"e_map_info_panel_type_aim_monster_killed_num",faith::e_map_info_panel_type_aim_monster_killed_num);
   tolua_constant(tolua_S,"e_map_info_panel_type_exp_raid",faith::e_map_info_panel_type_exp_raid);
   tolua_constant(tolua_S,"e_map_info_panel_type_boss_home",faith::e_map_info_panel_type_boss_home);
   tolua_constant(tolua_S,"e_map_info_panel_type_tower_reward",faith::e_map_info_panel_type_tower_reward);
   tolua_constant(tolua_S,"e_map_info_panel_type_crystak_raid",faith::e_map_info_panel_type_crystak_raid);
   tolua_constant(tolua_S,"e_map_info_panel_type_relic_raid",faith::e_map_info_panel_type_relic_raid);
   tolua_constant(tolua_S,"e_map_info_panel_type_belief_home",faith::e_map_info_panel_type_belief_home);
   tolua_constant(tolua_S,"e_map_info_panel_type_answer_question",faith::e_map_info_panel_type_answer_question);
   tolua_constant(tolua_S,"e_map_info_panel_type_legion_station",faith::e_map_info_panel_type_legion_station);
   tolua_constant(tolua_S,"e_map_info_panel_type_pk_king",faith::e_map_info_panel_type_pk_king);
   tolua_constant(tolua_S,"e_map_info_panel_type_element_war",faith::e_map_info_panel_type_element_war);
   tolua_constant(tolua_S,"e_map_info_panel_type_max",faith::e_map_info_panel_type_max);
   tolua_constant(tolua_S,"e_legion_member_info_is_online",faith::e_legion_member_info_is_online);
   tolua_constant(tolua_S,"e_legion_member_info_level",faith::e_legion_member_info_level);
   tolua_constant(tolua_S,"e_legion_member_info_player_template_id",faith::e_legion_member_info_player_template_id);
   tolua_constant(tolua_S,"e_legion_member_info_fighting_power",faith::e_legion_member_info_fighting_power);
   tolua_constant(tolua_S,"e_legion_member_info_job_title",faith::e_legion_member_info_job_title);
   tolua_constant(tolua_S,"e_legion_member_info_city_war_score",faith::e_legion_member_info_city_war_score);
   tolua_constant(tolua_S,"e_legion_member_info_donate_contribution",faith::e_legion_member_info_donate_contribution);
   tolua_constant(tolua_S,"e_legion_member_info_add_fuel_times",faith::e_legion_member_info_add_fuel_times);
   tolua_constant(tolua_S,"e_legion_member_info_last_logout_stamp",faith::e_legion_member_info_last_logout_stamp);
   tolua_constant(tolua_S,"e_legion_member_info_last_get_city_award",faith::e_legion_member_info_last_get_city_award);
   tolua_constant(tolua_S,"e_legion_member_info_agora_id",faith::e_legion_member_info_agora_id);
   tolua_constant(tolua_S,"e_legion_member_info_join_time_stamp",faith::e_legion_member_info_join_time_stamp);
   tolua_constant(tolua_S,"e_legion_member_info_vip_level",faith::e_legion_member_info_vip_level);
   tolua_constant(tolua_S,"e_legion_member_info_get_master_city_award",faith::e_legion_member_info_get_master_city_award);
   tolua_constant(tolua_S,"e_legion_member_info_week_contribution",faith::e_legion_member_info_week_contribution);
   tolua_constant(tolua_S,"e_legion_member_info_is_show_vip",faith::e_legion_member_info_is_show_vip);
   tolua_constant(tolua_S,"e_legion_member_info_max",faith::e_legion_member_info_max);
   tolua_constant(tolua_S,"e_first_time_do_type_none",faith::e_first_time_do_type_none);
   tolua_constant(tolua_S,"e_first_time_do_type_composit",faith::e_first_time_do_type_composit);
   tolua_constant(tolua_S,"e_first_time_do_type_pop_power_up_prompt_widget",faith::e_first_time_do_type_pop_power_up_prompt_widget);
   tolua_constant(tolua_S,"e_first_time_do_type_sky_raid",faith::e_first_time_do_type_sky_raid);
   tolua_constant(tolua_S,"e_first_time_do_type_close_raid_win_widget",faith::e_first_time_do_type_close_raid_win_widget);
   tolua_constant(tolua_S,"e_first_time_do_type_equip_upgrade",faith::e_first_time_do_type_equip_upgrade);
   tolua_constant(tolua_S,"e_first_time_do_type_lucky_draw_sprite",faith::e_first_time_do_type_lucky_draw_sprite);
   tolua_constant(tolua_S,"e_first_time_do_type_lucky_draw_item",faith::e_first_time_do_type_lucky_draw_item);
   tolua_constant(tolua_S,"e_first_time_do_type_get_equip",faith::e_first_time_do_type_get_equip);
   tolua_constant(tolua_S,"e_first_time_do_type_lucky_draw_elementheart",faith::e_first_time_do_type_lucky_draw_elementheart);
   tolua_constant(tolua_S,"e_first_time_do_type_killed",faith::e_first_time_do_type_killed);
   tolua_constant(tolua_S,"e_first_time_do_type_open_legion_store",faith::e_first_time_do_type_open_legion_store);
   tolua_constant(tolua_S,"e_first_time_do_type_open_old_store",faith::e_first_time_do_type_open_old_store);
   tolua_constant(tolua_S,"e_first_time_do_type_open_hope_store",faith::e_first_time_do_type_open_hope_store);
   tolua_constant(tolua_S,"e_first_time_do_type_exp_raid",faith::e_first_time_do_type_exp_raid);
   tolua_constant(tolua_S,"e_first_time_do_type_online_reward",faith::e_first_time_do_type_online_reward);
   tolua_constant(tolua_S,"e_first_time_do_type_spirit_forge",faith::e_first_time_do_type_spirit_forge);
   tolua_constant(tolua_S,"e_first_time_do_type_auction_buy",faith::e_first_time_do_type_auction_buy);
   tolua_constant(tolua_S,"e_first_time_do_type_auction_sell",faith::e_first_time_do_type_auction_sell);
   tolua_constant(tolua_S,"e_first_time_do_type_max",faith::e_first_time_do_type_max);
   tolua_constant(tolua_S,"e_move_type_stop",faith::e_move_type_stop);
   tolua_constant(tolua_S,"e_move_type_front",faith::e_move_type_front);
   tolua_constant(tolua_S,"e_move_type_back",faith::e_move_type_back);
   tolua_constant(tolua_S,"e_move_type_left",faith::e_move_type_left);
   tolua_constant(tolua_S,"e_move_type_right",faith::e_move_type_right);
   tolua_constant(tolua_S,"e_move_type_max",faith::e_move_type_max);
   tolua_constant(tolua_S,"e_revive_type_transfer",faith::e_revive_type_transfer);
   tolua_constant(tolua_S,"e_revive_type_born_pos",faith::e_revive_type_born_pos);
   tolua_constant(tolua_S,"e_revive_type_free_revive_by_CD",faith::e_revive_type_free_revive_by_CD);
   tolua_constant(tolua_S,"e_revive_type_same_pos",faith::e_revive_type_same_pos);
   tolua_constant(tolua_S,"e_revive_type_cant_revive",faith::e_revive_type_cant_revive);
   tolua_constant(tolua_S,"e_revive_type_max",faith::e_revive_type_max);
   tolua_constant(tolua_S,"e_team_state_none",faith::e_team_state_none);
   tolua_constant(tolua_S,"e_team_state_waiting_join",faith::e_team_state_waiting_join);
   tolua_constant(tolua_S,"e_team_state_waiting_ready",faith::e_team_state_waiting_ready);
   tolua_constant(tolua_S,"e_team_state_game",faith::e_team_state_game);
   tolua_constant(tolua_S,"e_team_state_max",faith::e_team_state_max);
   tolua_constant(tolua_S,"e_pk_info_pk_mode",faith::e_pk_info_pk_mode);
   tolua_constant(tolua_S,"e_pk_info_community_type",faith::e_pk_info_community_type);
   tolua_constant(tolua_S,"e_pk_info_max",faith::e_pk_info_max);
   tolua_constant(tolua_S,"e_region_type_zhongli",faith::e_region_type_zhongli);
   tolua_constant(tolua_S,"e_region_type_danger_zhongli",faith::e_region_type_danger_zhongli);
   tolua_constant(tolua_S,"e_region_type_safe",faith::e_region_type_safe);
   tolua_constant(tolua_S,"e_region_type_protect",faith::e_region_type_protect);
   tolua_constant(tolua_S,"e_region_type_max",faith::e_region_type_max);
   tolua_constant(tolua_S,"e_npc_interact_state_none",faith::e_npc_interact_state_none);
   tolua_constant(tolua_S,"e_npc_interact_state_begin",faith::e_npc_interact_state_begin);
   tolua_constant(tolua_S,"e_npc_interact_state_stop",faith::e_npc_interact_state_stop);
   tolua_constant(tolua_S,"e_npc_interact_state_finish",faith::e_npc_interact_state_finish);
   tolua_constant(tolua_S,"e_npc_interact_state_max",faith::e_npc_interact_state_max);
   tolua_constant(tolua_S,"e_mission_operate_type_accept",faith::e_mission_operate_type_accept);
   tolua_constant(tolua_S,"e_mission_operate_type_finish",faith::e_mission_operate_type_finish);
   tolua_constant(tolua_S,"e_mission_operate_type_refresh_star",faith::e_mission_operate_type_refresh_star);
   tolua_constant(tolua_S,"e_mission_operate_type_dialog",faith::e_mission_operate_type_dialog);
   tolua_constant(tolua_S,"e_mission_operate_type_finish_all_daily",faith::e_mission_operate_type_finish_all_daily);
   tolua_constant(tolua_S,"e_mission_operate_type_special_finish",faith::e_mission_operate_type_special_finish);
   tolua_constant(tolua_S,"e_mission_operate_type_create_npc",faith::e_mission_operate_type_create_npc);
   tolua_constant(tolua_S,"e_mission_operate_type_max",faith::e_mission_operate_type_max);
   tolua_constant(tolua_S,"e_protect_mark_type_single_equip_upgrade_history_record",faith::e_protect_mark_type_single_equip_upgrade_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_total_equip_upgrade_num_history_record",faith::e_protect_mark_type_total_equip_upgrade_num_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_total_equip_addon_num_history_record",faith::e_protect_mark_type_total_equip_addon_num_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_equip_succinct_max_highest_color",faith::e_protect_mark_type_equip_succinct_max_highest_color);
   tolua_constant(tolua_S,"e_protect_mark_type_total_mount_grade",faith::e_protect_mark_type_total_mount_grade);
   tolua_constant(tolua_S,"e_protect_mark_type_mount_num",faith::e_protect_mark_type_mount_num);
   tolua_constant(tolua_S,"e_protect_mark_type_wing_and_feather_total_grade",faith::e_protect_mark_type_wing_and_feather_total_grade);
   tolua_constant(tolua_S,"e_protect_mark_type_pokedex_num",faith::e_protect_mark_type_pokedex_num);
   tolua_constant(tolua_S,"e_protect_mark_type_total_sprite_equip_level_history_record",faith::e_protect_mark_type_total_sprite_equip_level_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_total_belief_grade",faith::e_protect_mark_type_total_belief_grade);
   tolua_constant(tolua_S,"e_protect_mark_type_role_exp_level",faith::e_protect_mark_type_role_exp_level);
   tolua_constant(tolua_S,"e_protect_mark_type_role_military_rank",faith::e_protect_mark_type_role_military_rank);
   tolua_constant(tolua_S,"e_protect_mark_type_achievement",faith::e_protect_mark_type_achievement);
   tolua_constant(tolua_S,"e_protect_mark_type_total_element_heart_equip_level_history_record",faith::e_protect_mark_type_total_element_heart_equip_level_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_max",faith::e_protect_mark_type_max);
   tolua_constant(tolua_S,"e_patron_saint_info_template_id",faith::e_patron_saint_info_template_id);
   tolua_constant(tolua_S,"e_patron_saint_info_last_update_time",faith::e_patron_saint_info_last_update_time);
   tolua_constant(tolua_S,"e_patron_saint_info_psyche_recovery_num",faith::e_patron_saint_info_psyche_recovery_num);
   tolua_constant(tolua_S,"e_patron_saint_info_psyche_oneday_limit",faith::e_patron_saint_info_psyche_oneday_limit);
   tolua_constant(tolua_S,"e_patron_saint_info_psyche_today_exceed_num",faith::e_patron_saint_info_psyche_today_exceed_num);
   tolua_constant(tolua_S,"e_patron_saint_info_upgrade_num",faith::e_patron_saint_info_upgrade_num);
   tolua_constant(tolua_S,"e_patron_saint_info_max",faith::e_patron_saint_info_max);
   tolua_constant(tolua_S,"e_role_history_highest_record_single_equip_upgrade",faith::e_role_history_highest_record_single_equip_upgrade);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_equip_upgrade_num",faith::e_role_history_highest_record_total_equip_upgrade_num);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_equip_addon_num",faith::e_role_history_highest_record_total_equip_addon_num);
   tolua_constant(tolua_S,"e_role_history_highest_record_item_succinct_reach_mark_level",faith::e_role_history_highest_record_item_succinct_reach_mark_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_sprite_equip_level",faith::e_role_history_highest_record_total_sprite_equip_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_element_heart_equip_level",faith::e_role_history_highest_record_total_element_heart_equip_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_arena_higest",faith::e_role_history_highest_record_arena_higest);
   tolua_constant(tolua_S,"e_role_history_highest_record_demon_tower_highest_level",faith::e_role_history_highest_record_demon_tower_highest_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_friend_num",faith::e_role_history_highest_record_higest_friend_num);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_friendliness_value",faith::e_role_history_highest_record_higest_friendliness_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_weapon_upgrade_buff",faith::e_role_history_highest_record_higest_weapon_upgrade_buff);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_equip_upgrade_buff",faith::e_role_history_highest_record_higest_equip_upgrade_buff);
   tolua_constant(tolua_S,"e_role_history_highest_record_rune_total_level",faith::e_role_history_highest_record_rune_total_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_addon_weapon",faith::e_role_history_highest_record_addon_weapon);
   tolua_constant(tolua_S,"e_role_history_highest_record_addon_equip",faith::e_role_history_highest_record_addon_equip);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_spring",faith::e_role_history_highest_record_belief_spring);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_summer",faith::e_role_history_highest_record_belief_summer);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_autumn",faith::e_role_history_highest_record_belief_autumn);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_winter",faith::e_role_history_highest_record_belief_winter);
   tolua_constant(tolua_S,"e_role_history_highest_record_weapon_full_succinct_grade",faith::e_role_history_highest_record_weapon_full_succinct_grade);
   tolua_constant(tolua_S,"e_role_history_highest_record_equip_full_succinct_grade",faith::e_role_history_highest_record_equip_full_succinct_grade);
   tolua_constant(tolua_S,"e_role_history_highest_record_battle_achievement",faith::e_role_history_highest_record_battle_achievement);
   tolua_constant(tolua_S,"e_role_history_highest_record_gs_value",faith::e_role_history_highest_record_gs_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_mount_value",faith::e_role_history_highest_record_mount_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_wing_value",faith::e_role_history_highest_record_wing_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_equip_value",faith::e_role_history_highest_record_equip_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_spirit_value",faith::e_role_history_highest_record_spirit_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_awaken_level",faith::e_role_history_highest_record_max_awaken_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_forge_level",faith::e_role_history_highest_record_max_forge_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_equip_enchant_level",faith::e_role_history_highest_record_max_equip_enchant_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_jewelry_enchant_level",faith::e_role_history_highest_record_max_jewelry_enchant_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_spirit_composit_level",faith::e_role_history_highest_record_max_spirit_composit_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max",faith::e_role_history_highest_record_max);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_purchase_new_slot",faith::e_patron_saint_mark_operate_type_purchase_new_slot);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_equip_mark",faith::e_patron_saint_mark_operate_type_equip_mark);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_unequip_mark",faith::e_patron_saint_mark_operate_type_unequip_mark);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_upgrade",faith::e_patron_saint_mark_operate_type_upgrade);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_equip_prop",faith::e_patron_saint_mark_operate_type_equip_prop);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_unequip_prop",faith::e_patron_saint_mark_operate_type_unequip_prop);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_purchase_prop_new_slot",faith::e_patron_saint_mark_operate_type_purchase_prop_new_slot);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_max",faith::e_patron_saint_mark_operate_type_max);
   tolua_constant(tolua_S,"e_belief_info_template_id",faith::e_belief_info_template_id);
   tolua_constant(tolua_S,"e_belief_info_upgrade_count",faith::e_belief_info_upgrade_count);
   tolua_constant(tolua_S,"e_belief_info_try_upgrade_times",faith::e_belief_info_try_upgrade_times);
   tolua_constant(tolua_S,"e_belief_info_max",faith::e_belief_info_max);
   tolua_constant(tolua_S,"e_talent_info_template_id",faith::e_talent_info_template_id);
   tolua_constant(tolua_S,"e_talent_info_cast_money",faith::e_talent_info_cast_money);
   tolua_constant(tolua_S,"e_talent_info_max",faith::e_talent_info_max);
   tolua_constant(tolua_S,"e_talent_layer_need_id",faith::e_talent_layer_need_id);
   tolua_constant(tolua_S,"e_talent_layer_need_num",faith::e_talent_layer_need_num);
   tolua_constant(tolua_S,"e_talent_layer_template_id_0",faith::e_talent_layer_template_id_0);
   tolua_constant(tolua_S,"e_talent_layer_template_id_1",faith::e_talent_layer_template_id_1);
   tolua_constant(tolua_S,"e_talent_layer_template_id_2",faith::e_talent_layer_template_id_2);
   tolua_constant(tolua_S,"e_talent_layer_template_id_3",faith::e_talent_layer_template_id_3);
   tolua_constant(tolua_S,"e_talent_layer_max",faith::e_talent_layer_max);
   tolua_constant(tolua_S,"e_relation_data_type",faith::e_relation_data_type);
   tolua_constant(tolua_S,"e_relation_data_level",faith::e_relation_data_level);
   tolua_constant(tolua_S,"e_relation_data_template_id",faith::e_relation_data_template_id);
   tolua_constant(tolua_S,"e_relation_data_gs",faith::e_relation_data_gs);
   tolua_constant(tolua_S,"e_relation_data_relation_state",faith::e_relation_data_relation_state);
   tolua_constant(tolua_S,"e_relation_data_last_login_time",faith::e_relation_data_last_login_time);
   tolua_constant(tolua_S,"e_relation_data_friendliness",faith::e_relation_data_friendliness);
   tolua_constant(tolua_S,"e_relation_data_vip_title_template_id",faith::e_relation_data_vip_title_template_id);
   tolua_constant(tolua_S,"e_relation_data_max",faith::e_relation_data_max);
   tolua_constant(tolua_S,"e_role_equip_slot_body",faith::e_role_equip_slot_body);
   tolua_constant(tolua_S,"e_role_equip_slot_weapon_1",faith::e_role_equip_slot_weapon_1);
   tolua_constant(tolua_S,"e_role_equip_slot_hat",faith::e_role_equip_slot_hat);
   tolua_constant(tolua_S,"e_role_equip_slot_earring",faith::e_role_equip_slot_earring);
   tolua_constant(tolua_S,"e_role_equip_slot_necklace",faith::e_role_equip_slot_necklace);
   tolua_constant(tolua_S,"e_role_equip_slot_armor",faith::e_role_equip_slot_armor);
   tolua_constant(tolua_S,"e_role_equip_slot_pants",faith::e_role_equip_slot_pants);
   tolua_constant(tolua_S,"e_role_equip_slot_ring",faith::e_role_equip_slot_ring);
   tolua_constant(tolua_S,"e_role_equip_slot_glove",faith::e_role_equip_slot_glove);
   tolua_constant(tolua_S,"e_role_equip_slot_boot",faith::e_role_equip_slot_boot);
   tolua_constant(tolua_S,"e_role_equip_slot_amulet",faith::e_role_equip_slot_amulet);
   tolua_constant(tolua_S,"e_role_equip_slot_magic_1",faith::e_role_equip_slot_magic_1);
   tolua_constant(tolua_S,"e_role_equip_slot_magic_2",faith::e_role_equip_slot_magic_2);
   tolua_constant(tolua_S,"e_role_equip_slot_wing",faith::e_role_equip_slot_wing);
   tolua_constant(tolua_S,"e_role_equip_slot_mount",faith::e_role_equip_slot_mount);
   tolua_constant(tolua_S,"e_role_equip_slot_sprite",faith::e_role_equip_slot_sprite);
   tolua_constant(tolua_S,"e_role_equip_slot_mantle",faith::e_role_equip_slot_mantle);
   tolua_constant(tolua_S,"e_role_equip_slot_badge",faith::e_role_equip_slot_badge);
   tolua_constant(tolua_S,"e_role_equip_slot_head",faith::e_role_equip_slot_head);
   tolua_constant(tolua_S,"e_role_equip_slot_hair",faith::e_role_equip_slot_hair);
   tolua_constant(tolua_S,"e_role_equip_slot_headdress",faith::e_role_equip_slot_headdress);
   tolua_constant(tolua_S,"e_role_equip_slot_mouth",faith::e_role_equip_slot_mouth);
   tolua_constant(tolua_S,"e_role_equip_slot_eye",faith::e_role_equip_slot_eye);
   tolua_constant(tolua_S,"e_role_equip_slot_max",faith::e_role_equip_slot_max);
   tolua_constant(tolua_S,"e_item_type_none",faith::e_item_type_none);
   tolua_constant(tolua_S,"e_item_type_weapon",faith::e_item_type_weapon);
   tolua_constant(tolua_S,"e_item_type_armor",faith::e_item_type_armor);
   tolua_constant(tolua_S,"e_item_type_mount",faith::e_item_type_mount);
   tolua_constant(tolua_S,"e_item_type_expendable",faith::e_item_type_expendable);
   tolua_constant(tolua_S,"e_item_type_stone",faith::e_item_type_stone);
   tolua_constant(tolua_S,"e_item_type_fashion",faith::e_item_type_fashion);
   tolua_constant(tolua_S,"e_item_type_mission",faith::e_item_type_mission);
   tolua_constant(tolua_S,"e_item_type_spirit",faith::e_item_type_spirit);
   tolua_constant(tolua_S,"e_item_type_wing",faith::e_item_type_wing);
   tolua_constant(tolua_S,"e_item_type_element_heart",faith::e_item_type_element_heart);
   tolua_constant(tolua_S,"e_item_type_protect",faith::e_item_type_protect);
   tolua_constant(tolua_S,"e_item_type_beast_spirit",faith::e_item_type_beast_spirit);
   tolua_constant(tolua_S,"e_item_type_time_limit",faith::e_item_type_time_limit);
   tolua_constant(tolua_S,"e_item_type_belief_rune",faith::e_item_type_belief_rune);
   tolua_constant(tolua_S,"e_item_type_sky_equip",faith::e_item_type_sky_equip);
   tolua_constant(tolua_S,"e_item_type_skill_book",faith::e_item_type_skill_book);
   tolua_constant(tolua_S,"e_item_type_goddess_equip",faith::e_item_type_goddess_equip);
   tolua_constant(tolua_S,"e_item_type_wedding_equip",faith::e_item_type_wedding_equip);
   tolua_constant(tolua_S,"e_item_type_skygod_equip",faith::e_item_type_skygod_equip);
   tolua_constant(tolua_S,"e_item_type_mount_equip",faith::e_item_type_mount_equip);
   tolua_constant(tolua_S,"e_item_type_supreme_equip",faith::e_item_type_supreme_equip);
   tolua_constant(tolua_S,"e_item_type_star_map_spirit",faith::e_item_type_star_map_spirit);
   tolua_constant(tolua_S,"e_item_type_mount_star",faith::e_item_type_mount_star);
   tolua_constant(tolua_S,"e_item_type_wing_psychic",faith::e_item_type_wing_psychic);
   tolua_constant(tolua_S,"e_item_type_break_will",faith::e_item_type_break_will);
   tolua_constant(tolua_S,"e_item_type_awaken_item",faith::e_item_type_awaken_item);
   tolua_constant(tolua_S,"e_item_type_max",faith::e_item_type_max);
   tolua_constant(tolua_S,"e_armor_sub_type_none",faith::e_armor_sub_type_none);
   tolua_constant(tolua_S,"e_armor_sub_type_hat",faith::e_armor_sub_type_hat);
   tolua_constant(tolua_S,"e_armor_sub_type_earring",faith::e_armor_sub_type_earring);
   tolua_constant(tolua_S,"e_armor_sub_type_necklace",faith::e_armor_sub_type_necklace);
   tolua_constant(tolua_S,"e_armor_sub_type_armor",faith::e_armor_sub_type_armor);
   tolua_constant(tolua_S,"e_armor_sub_type_pants",faith::e_armor_sub_type_pants);
   tolua_constant(tolua_S,"e_armor_sub_type_ring",faith::e_armor_sub_type_ring);
   tolua_constant(tolua_S,"e_armor_sub_type_glove",faith::e_armor_sub_type_glove);
   tolua_constant(tolua_S,"e_armor_sub_type_boot",faith::e_armor_sub_type_boot);
   tolua_constant(tolua_S,"e_armor_sub_type_amulet",faith::e_armor_sub_type_amulet);
   tolua_constant(tolua_S,"e_armor_sub_type_fashion",faith::e_armor_sub_type_fashion);
   tolua_constant(tolua_S,"e_armor_sub_type_wedding",faith::e_armor_sub_type_wedding);
   tolua_constant(tolua_S,"e_armor_sub_type_spirit",faith::e_armor_sub_type_spirit);
   tolua_constant(tolua_S,"e_armor_sub_type_mantle",faith::e_armor_sub_type_mantle);
   tolua_constant(tolua_S,"e_armor_sub_type_max",faith::e_armor_sub_type_max);
   tolua_constant(tolua_S,"e_prop_sub_type_none",faith::e_prop_sub_type_none);
   tolua_constant(tolua_S,"e_prop_sub_type_fruit",faith::e_prop_sub_type_fruit);
   tolua_constant(tolua_S,"e_prop_sub_type_gife",faith::e_prop_sub_type_gife);
   tolua_constant(tolua_S,"e_prop_sub_type_money",faith::e_prop_sub_type_money);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_min",faith::e_prop_sub_type_hp_min);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_mld",faith::e_prop_sub_type_hp_mld);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_max",faith::e_prop_sub_type_hp_max);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_shap_card",faith::e_prop_sub_type_wing_shap_card);
   tolua_constant(tolua_S,"e_prop_sub_type_title_card",faith::e_prop_sub_type_title_card);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_shop",faith::e_prop_sub_type_hp_shop);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_essence",faith::e_prop_sub_type_hp_essence);
   tolua_constant(tolua_S,"e_prop_sub_type_store_medicine",faith::e_prop_sub_type_store_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_gain_medicine",faith::e_prop_sub_type_gain_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_min_level_up_medicine",faith::e_prop_sub_type_min_level_up_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_mid_level_up_medicine",faith::e_prop_sub_type_mid_level_up_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_max_level_up_medicine",faith::e_prop_sub_type_max_level_up_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_wash_talent_water",faith::e_prop_sub_type_wash_talent_water);
   tolua_constant(tolua_S,"e_prop_sub_type_legion_donate",faith::e_prop_sub_type_legion_donate);
   tolua_constant(tolua_S,"e_prop_sub_type_reduce_Pk_value",faith::e_prop_sub_type_reduce_Pk_value);
   tolua_constant(tolua_S,"e_prop_sub_type_mount_illusion_unlock",faith::e_prop_sub_type_mount_illusion_unlock);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_illusion_unlock",faith::e_prop_sub_type_wing_illusion_unlock);
   tolua_constant(tolua_S,"e_prop_sub_type_element_package",faith::e_prop_sub_type_element_package);
   tolua_constant(tolua_S,"e_prop_sub_type_vip_card",faith::e_prop_sub_type_vip_card);
   tolua_constant(tolua_S,"e_prop_sub_type_rand_equip",faith::e_prop_sub_type_rand_equip);
   tolua_constant(tolua_S,"e_prop_sub_type_globel_red_package",faith::e_prop_sub_type_globel_red_package);
   tolua_constant(tolua_S,"e_prop_sub_type_legion_red_package",faith::e_prop_sub_type_legion_red_package);
   tolua_constant(tolua_S,"e_prop_sub_type_mount_shap_card",faith::e_prop_sub_type_mount_shap_card);
   tolua_constant(tolua_S,"e_prop_sub_type_marry_prop",faith::e_prop_sub_type_marry_prop);
   tolua_constant(tolua_S,"e_prop_sub_type_diamonds_package",faith::e_prop_sub_type_diamonds_package);
   tolua_constant(tolua_S,"e_prop_sub_type_refresh_body_att",faith::e_prop_sub_type_refresh_body_att);
   tolua_constant(tolua_S,"e_prop_sub_type_time_limit_prop",faith::e_prop_sub_type_time_limit_prop);
   tolua_constant(tolua_S,"e_prop_sub_type_recharge_package",faith::e_prop_sub_type_recharge_package);
   tolua_constant(tolua_S,"e_prop_sub_type_vip_experience_card",faith::e_prop_sub_type_vip_experience_card);
   tolua_constant(tolua_S,"e_prop_sub_type_const_att_equip",faith::e_prop_sub_type_const_att_equip);
   tolua_constant(tolua_S,"e_prop_sub_type_exp_addition",faith::e_prop_sub_type_exp_addition);
   tolua_constant(tolua_S,"e_prop_sub_type_add_map_count",faith::e_prop_sub_type_add_map_count);
   tolua_constant(tolua_S,"e_prop_sub_type_add_exp_with_num",faith::e_prop_sub_type_add_exp_with_num);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_experience",faith::e_prop_sub_type_wing_experience);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_piece",faith::e_prop_sub_type_wing_piece);
   tolua_constant(tolua_S,"e_prop_sub_type_change_class_card",faith::e_prop_sub_type_change_class_card);
   tolua_constant(tolua_S,"e_prop_sub_type_change_name_card",faith::e_prop_sub_type_change_name_card);
   tolua_constant(tolua_S,"e_prop_sub_type_choose_gife",faith::e_prop_sub_type_choose_gife);
   tolua_constant(tolua_S,"e_prop_sub_type_meditation_medicine",faith::e_prop_sub_type_meditation_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_fashion_prop",faith::e_prop_sub_type_fashion_prop);
   tolua_constant(tolua_S,"e_prop_sub_type_tinder_skill_book",faith::e_prop_sub_type_tinder_skill_book);
   tolua_constant(tolua_S,"e_prop_sub_type_grade_up",faith::e_prop_sub_type_grade_up);
   tolua_constant(tolua_S,"e_prop_sub_type_legion_change_name_card",faith::e_prop_sub_type_legion_change_name_card);
   tolua_constant(tolua_S,"e_prop_sub_type_fireworks",faith::e_prop_sub_type_fireworks);
   tolua_constant(tolua_S,"e_prop_sub_type_horn",faith::e_prop_sub_type_horn);
   tolua_constant(tolua_S,"e_prop_sub_type_area_horn",faith::e_prop_sub_type_area_horn);
   tolua_constant(tolua_S,"e_prop_sub_type_add_map_time",faith::e_prop_sub_type_add_map_time);
   tolua_constant(tolua_S,"e_prop_sub_type_appearance_card",faith::e_prop_sub_type_appearance_card);
   tolua_constant(tolua_S,"e_prop_sub_type_add_already_time",faith::e_prop_sub_type_add_already_time);
   tolua_constant(tolua_S,"e_prop_sub_type_add_single_boss_times",faith::e_prop_sub_type_add_single_boss_times);
   tolua_constant(tolua_S,"e_prop_sub_type_awken_item",faith::e_prop_sub_type_awken_item);
   tolua_constant(tolua_S,"e_prop_sub_type_sprite_upgrade",faith::e_prop_sub_type_sprite_upgrade);
   tolua_constant(tolua_S,"e_prop_sub_type_max",faith::e_prop_sub_type_max);
   tolua_constant(tolua_S,"e_stone_sub_type_none",faith::e_stone_sub_type_none);
   tolua_constant(tolua_S,"e_stone_sub_type_equip_upgrade",faith::e_stone_sub_type_equip_upgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_equip_addon",faith::e_stone_sub_type_equip_addon);
   tolua_constant(tolua_S,"e_stone_sub_type_mount",faith::e_stone_sub_type_mount);
   tolua_constant(tolua_S,"e_stone_sub_type_addsoul",faith::e_stone_sub_type_addsoul);
   tolua_constant(tolua_S,"e_stone_sub_type_addspririt",faith::e_stone_sub_type_addspririt);
   tolua_constant(tolua_S,"e_stone_sub_type_refine",faith::e_stone_sub_type_refine);
   tolua_constant(tolua_S,"e_stone_sub_type_upgrade_protect",faith::e_stone_sub_type_upgrade_protect);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_upgrade",faith::e_stone_sub_type_mount_upgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_upstair",faith::e_stone_sub_type_mount_upstair);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_unlock",faith::e_stone_sub_type_mount_unlock);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_unlock_highgrade",faith::e_stone_sub_type_mount_unlock_highgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_wing_upgrade",faith::e_stone_sub_type_wing_upgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_wing_upstair",faith::e_stone_sub_type_wing_upstair);
   tolua_constant(tolua_S,"e_stone_sub_type_wing_highgrade",faith::e_stone_sub_type_wing_highgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_orangered_shard",faith::e_stone_sub_type_orangered_shard);
   tolua_constant(tolua_S,"e_stone_sub_type_basic_synthetic_material",faith::e_stone_sub_type_basic_synthetic_material);
   tolua_constant(tolua_S,"e_stone_sub_type_spirit_shard",faith::e_stone_sub_type_spirit_shard);
   tolua_constant(tolua_S,"e_stone_sub_type_resurection_stone",faith::e_stone_sub_type_resurection_stone);
   tolua_constant(tolua_S,"e_stone_sub_type_dragon_soul",faith::e_stone_sub_type_dragon_soul);
   tolua_constant(tolua_S,"e_stone_sub_type_debris",faith::e_stone_sub_type_debris);
   tolua_constant(tolua_S,"e_stone_sub_type_psyche",faith::e_stone_sub_type_psyche);
   tolua_constant(tolua_S,"e_stone_sub_type_jewel",faith::e_stone_sub_type_jewel);
   tolua_constant(tolua_S,"e_stone_sub_type_book_card",faith::e_stone_sub_type_book_card);
   tolua_constant(tolua_S,"e_stone_sub_type_tinder_prop",faith::e_stone_sub_type_tinder_prop);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_equip",faith::e_stone_sub_type_mount_equip);
   tolua_constant(tolua_S,"e_stone_sub_type_max",faith::e_stone_sub_type_max);
   tolua_constant(tolua_S,"e_feather_sub_type_none",faith::e_feather_sub_type_none);
   tolua_constant(tolua_S,"e_feather_sub_type_feather1",faith::e_feather_sub_type_feather1);
   tolua_constant(tolua_S,"e_feather_sub_type_feather2",faith::e_feather_sub_type_feather2);
   tolua_constant(tolua_S,"e_feather_sub_type_feather3",faith::e_feather_sub_type_feather3);
   tolua_constant(tolua_S,"e_feather_sub_type_feather4",faith::e_feather_sub_type_feather4);
   tolua_constant(tolua_S,"e_feather_sub_type_max",faith::e_feather_sub_type_max);
   tolua_constant(tolua_S,"e_wing_sub_type_none",faith::e_wing_sub_type_none);
   tolua_constant(tolua_S,"e_wing_sub_type_wing",faith::e_wing_sub_type_wing);
   tolua_constant(tolua_S,"e_wing_sub_type_feather",faith::e_wing_sub_type_feather);
   tolua_constant(tolua_S,"e_wing_sub_type_soul",faith::e_wing_sub_type_soul);
   tolua_constant(tolua_S,"e_wing_sub_type_spirit",faith::e_wing_sub_type_spirit);
   tolua_constant(tolua_S,"e_wing_sub_type_illusion",faith::e_wing_sub_type_illusion);
   tolua_constant(tolua_S,"e_wing_sub_type_max",faith::e_wing_sub_type_max);
   tolua_constant(tolua_S,"e_element_heart_sub_type_equip",faith::e_element_heart_sub_type_equip);
   tolua_constant(tolua_S,"e_element_heart_sub_type_exp",faith::e_element_heart_sub_type_exp);
   tolua_constant(tolua_S,"e_element_heart_sub_type_max",faith::e_element_heart_sub_type_max);
   tolua_constant(tolua_S,"e_store_type_non",faith::e_store_type_non);
   tolua_constant(tolua_S,"e_store_type_recommend",faith::e_store_type_recommend);
   tolua_constant(tolua_S,"e_store_type_daily",faith::e_store_type_daily);
   tolua_constant(tolua_S,"e_store_type_expendables",faith::e_store_type_expendables);
   tolua_constant(tolua_S,"e_store_type_bind_jewel_0",faith::e_store_type_bind_jewel_0);
   tolua_constant(tolua_S,"e_store_type_shape",faith::e_store_type_shape);
   tolua_constant(tolua_S,"e_store_type_limit_buy",faith::e_store_type_limit_buy);
   tolua_constant(tolua_S,"e_store_type_sprite_first",faith::e_store_type_sprite_first);
   tolua_constant(tolua_S,"e_store_type_sprite_second",faith::e_store_type_sprite_second);
   tolua_constant(tolua_S,"e_store_type_sprite_third",faith::e_store_type_sprite_third);
   tolua_constant(tolua_S,"e_store_type_magic_crystal_first",faith::e_store_type_magic_crystal_first);
   tolua_constant(tolua_S,"e_store_type_magic_crystal_second",faith::e_store_type_magic_crystal_second);
   tolua_constant(tolua_S,"e_store_type_magic_crystal_third",faith::e_store_type_magic_crystal_third);
   tolua_constant(tolua_S,"e_store_type_medicine",faith::e_store_type_medicine);
   tolua_constant(tolua_S,"e_store_type_prop",faith::e_store_type_prop);
   tolua_constant(tolua_S,"e_store_type_hope_point_first",faith::e_store_type_hope_point_first);
   tolua_constant(tolua_S,"e_store_type_hope_point_second",faith::e_store_type_hope_point_second);
   tolua_constant(tolua_S,"e_store_type_hope_point_third",faith::e_store_type_hope_point_third);
   tolua_constant(tolua_S,"e_store_type_legion",faith::e_store_type_legion);
   tolua_constant(tolua_S,"e_store_type_hornor",faith::e_store_type_hornor);
   tolua_constant(tolua_S,"e_store_type_assist_fighting",faith::e_store_type_assist_fighting);
   tolua_constant(tolua_S,"e_store_type_fashion",faith::e_store_type_fashion);
   tolua_constant(tolua_S,"e_store_type_bind_jewel_1",faith::e_store_type_bind_jewel_1);
   tolua_constant(tolua_S,"e_store_type_bind_jewel_2",faith::e_store_type_bind_jewel_2);
   tolua_constant(tolua_S,"e_store_type_treasure_royal",faith::e_store_type_treasure_royal);
   tolua_constant(tolua_S,"e_store_type_treasure_element",faith::e_store_type_treasure_element);
   tolua_constant(tolua_S,"e_store_type_cross_server_buy",faith::e_store_type_cross_server_buy);
   tolua_constant(tolua_S,"e_store_type_red_diamond",faith::e_store_type_red_diamond);
   tolua_constant(tolua_S,"e_store_type_cross_ladder_store",faith::e_store_type_cross_ladder_store);
   tolua_constant(tolua_S,"e_store_type_non_29",faith::e_store_type_non_29);
   tolua_constant(tolua_S,"e_store_type_non_30",faith::e_store_type_non_30);
   tolua_constant(tolua_S,"e_store_type_non_31",faith::e_store_type_non_31);
   tolua_constant(tolua_S,"e_store_type_non_32",faith::e_store_type_non_32);
   tolua_constant(tolua_S,"e_store_type_non_33",faith::e_store_type_non_33);
   tolua_constant(tolua_S,"e_store_type_non_34",faith::e_store_type_non_34);
   tolua_constant(tolua_S,"e_store_type_non_35",faith::e_store_type_non_35);
   tolua_constant(tolua_S,"e_store_type_non_36",faith::e_store_type_non_36);
   tolua_constant(tolua_S,"e_store_type_non_37",faith::e_store_type_non_37);
   tolua_constant(tolua_S,"e_store_type_non_38",faith::e_store_type_non_38);
   tolua_constant(tolua_S,"e_store_type_non_39",faith::e_store_type_non_39);
   tolua_constant(tolua_S,"e_store_type_non_40",faith::e_store_type_non_40);
   tolua_constant(tolua_S,"e_store_type_limit_buy_41",faith::e_store_type_limit_buy_41);
   tolua_constant(tolua_S,"e_store_type_limit_buy_42",faith::e_store_type_limit_buy_42);
   tolua_constant(tolua_S,"e_store_type_limit_buy_43",faith::e_store_type_limit_buy_43);
   tolua_constant(tolua_S,"e_store_type_limit_buy_44",faith::e_store_type_limit_buy_44);
   tolua_constant(tolua_S,"e_store_type_limit_buy_45",faith::e_store_type_limit_buy_45);
   tolua_constant(tolua_S,"e_store_type_limit_buy_46",faith::e_store_type_limit_buy_46);
   tolua_constant(tolua_S,"e_store_type_limit_buy_47",faith::e_store_type_limit_buy_47);
   tolua_constant(tolua_S,"e_store_type_limit_buy_48",faith::e_store_type_limit_buy_48);
   tolua_constant(tolua_S,"e_store_type_limit_buy_49",faith::e_store_type_limit_buy_49);
   tolua_constant(tolua_S,"e_store_type_limit_buy_50",faith::e_store_type_limit_buy_50);
   tolua_constant(tolua_S,"e_store_type_limit_buy_51",faith::e_store_type_limit_buy_51);
   tolua_constant(tolua_S,"e_store_type_limit_buy_52",faith::e_store_type_limit_buy_52);
   tolua_constant(tolua_S,"e_store_type_limit_buy_53",faith::e_store_type_limit_buy_53);
   tolua_constant(tolua_S,"e_store_type_limit_buy_54",faith::e_store_type_limit_buy_54);
   tolua_constant(tolua_S,"e_store_type_limit_buy_55",faith::e_store_type_limit_buy_55);
   tolua_constant(tolua_S,"e_store_type_limit_buy_56",faith::e_store_type_limit_buy_56);
   tolua_constant(tolua_S,"e_store_type_limit_buy_57",faith::e_store_type_limit_buy_57);
   tolua_constant(tolua_S,"e_store_type_limit_buy_58",faith::e_store_type_limit_buy_58);
   tolua_constant(tolua_S,"e_store_type_limit_buy_59",faith::e_store_type_limit_buy_59);
   tolua_constant(tolua_S,"e_store_type_limit_buy_60",faith::e_store_type_limit_buy_60);
   tolua_constant(tolua_S,"e_store_type_limit_buy_61",faith::e_store_type_limit_buy_61);
   tolua_constant(tolua_S,"e_store_type_limit_buy_62",faith::e_store_type_limit_buy_62);
   tolua_constant(tolua_S,"e_store_type_limit_buy_63",faith::e_store_type_limit_buy_63);
   tolua_constant(tolua_S,"e_store_type_limit_buy_64",faith::e_store_type_limit_buy_64);
   tolua_constant(tolua_S,"e_store_type_limit_buy_65",faith::e_store_type_limit_buy_65);
   tolua_constant(tolua_S,"e_store_type_limit_buy_66",faith::e_store_type_limit_buy_66);
   tolua_constant(tolua_S,"e_store_type_limit_buy_67",faith::e_store_type_limit_buy_67);
   tolua_constant(tolua_S,"e_store_type_limit_buy_68",faith::e_store_type_limit_buy_68);
   tolua_constant(tolua_S,"e_store_type_limit_buy_69",faith::e_store_type_limit_buy_69);
   tolua_constant(tolua_S,"e_store_type_limit_buy_70",faith::e_store_type_limit_buy_70);
   tolua_constant(tolua_S,"e_store_type_limit_buy_71",faith::e_store_type_limit_buy_71);
   tolua_constant(tolua_S,"e_store_type_limit_buy_72",faith::e_store_type_limit_buy_72);
   tolua_constant(tolua_S,"e_store_type_limit_buy_73",faith::e_store_type_limit_buy_73);
   tolua_constant(tolua_S,"e_store_type_limit_buy_74",faith::e_store_type_limit_buy_74);
   tolua_constant(tolua_S,"e_store_type_limit_buy_75",faith::e_store_type_limit_buy_75);
   tolua_constant(tolua_S,"e_store_type_limit_buy_76",faith::e_store_type_limit_buy_76);
   tolua_constant(tolua_S,"e_store_type_limit_buy_77",faith::e_store_type_limit_buy_77);
   tolua_constant(tolua_S,"e_store_type_limit_buy_78",faith::e_store_type_limit_buy_78);
   tolua_constant(tolua_S,"e_store_type_limit_buy_79",faith::e_store_type_limit_buy_79);
   tolua_constant(tolua_S,"e_store_type_limit_buy_80",faith::e_store_type_limit_buy_80);
   tolua_constant(tolua_S,"e_store_type_limit_buy_81",faith::e_store_type_limit_buy_81);
   tolua_constant(tolua_S,"e_store_type_limit_buy_82",faith::e_store_type_limit_buy_82);
   tolua_constant(tolua_S,"e_store_type_limit_buy_83",faith::e_store_type_limit_buy_83);
   tolua_constant(tolua_S,"e_store_type_limit_buy_84",faith::e_store_type_limit_buy_84);
   tolua_constant(tolua_S,"e_store_type_limit_buy_85",faith::e_store_type_limit_buy_85);
   tolua_constant(tolua_S,"e_store_type_limit_buy_86",faith::e_store_type_limit_buy_86);
   tolua_constant(tolua_S,"e_store_type_limit_buy_87",faith::e_store_type_limit_buy_87);
   tolua_constant(tolua_S,"e_store_type_limit_buy_88",faith::e_store_type_limit_buy_88);
   tolua_constant(tolua_S,"e_store_type_limit_buy_89",faith::e_store_type_limit_buy_89);
   tolua_constant(tolua_S,"e_store_type_limit_buy_90",faith::e_store_type_limit_buy_90);
   tolua_constant(tolua_S,"e_store_type_limit_buy_91",faith::e_store_type_limit_buy_91);
   tolua_constant(tolua_S,"e_store_type_limit_buy_92",faith::e_store_type_limit_buy_92);
   tolua_constant(tolua_S,"e_store_type_limit_buy_93",faith::e_store_type_limit_buy_93);
   tolua_constant(tolua_S,"e_store_type_limit_buy_94",faith::e_store_type_limit_buy_94);
   tolua_constant(tolua_S,"e_store_type_limit_buy_95",faith::e_store_type_limit_buy_95);
   tolua_constant(tolua_S,"e_store_type_limit_buy_96",faith::e_store_type_limit_buy_96);
   tolua_constant(tolua_S,"e_store_type_limit_buy_97",faith::e_store_type_limit_buy_97);
   tolua_constant(tolua_S,"e_store_type_limit_buy_98",faith::e_store_type_limit_buy_98);
   tolua_constant(tolua_S,"e_store_type_limit_buy_99",faith::e_store_type_limit_buy_99);
   tolua_constant(tolua_S,"e_store_type_limit_buy_100",faith::e_store_type_limit_buy_100);
   tolua_constant(tolua_S,"e_store_type_limit_buy_101",faith::e_store_type_limit_buy_101);
   tolua_constant(tolua_S,"e_store_type_limit_buy_102",faith::e_store_type_limit_buy_102);
   tolua_constant(tolua_S,"e_store_type_limit_buy_103",faith::e_store_type_limit_buy_103);
   tolua_constant(tolua_S,"e_store_type_limit_buy_104",faith::e_store_type_limit_buy_104);
   tolua_constant(tolua_S,"e_store_type_limit_buy_105",faith::e_store_type_limit_buy_105);
   tolua_constant(tolua_S,"e_store_type_limit_buy_106",faith::e_store_type_limit_buy_106);
   tolua_constant(tolua_S,"e_store_type_limit_buy_107",faith::e_store_type_limit_buy_107);
   tolua_constant(tolua_S,"e_store_type_limit_buy_108",faith::e_store_type_limit_buy_108);
   tolua_constant(tolua_S,"e_store_type_limit_buy_109",faith::e_store_type_limit_buy_109);
   tolua_constant(tolua_S,"e_store_type_limit_buy_110",faith::e_store_type_limit_buy_110);
   tolua_constant(tolua_S,"e_store_type_limit_buy_111",faith::e_store_type_limit_buy_111);
   tolua_constant(tolua_S,"e_store_type_limit_buy_112",faith::e_store_type_limit_buy_112);
   tolua_constant(tolua_S,"e_store_type_limit_buy_113",faith::e_store_type_limit_buy_113);
   tolua_constant(tolua_S,"e_store_type_limit_buy_114",faith::e_store_type_limit_buy_114);
   tolua_constant(tolua_S,"e_store_type_limit_buy_115",faith::e_store_type_limit_buy_115);
   tolua_constant(tolua_S,"e_store_type_limit_buy_116",faith::e_store_type_limit_buy_116);
   tolua_constant(tolua_S,"e_store_type_limit_buy_117",faith::e_store_type_limit_buy_117);
   tolua_constant(tolua_S,"e_store_type_limit_buy_118",faith::e_store_type_limit_buy_118);
   tolua_constant(tolua_S,"e_store_type_limit_buy_119",faith::e_store_type_limit_buy_119);
   tolua_constant(tolua_S,"e_store_type_limit_buy_120",faith::e_store_type_limit_buy_120);
   tolua_constant(tolua_S,"e_store_type_limit_buy_121",faith::e_store_type_limit_buy_121);
   tolua_constant(tolua_S,"e_store_type_limit_buy_122",faith::e_store_type_limit_buy_122);
   tolua_constant(tolua_S,"e_store_type_limit_buy_123",faith::e_store_type_limit_buy_123);
   tolua_constant(tolua_S,"e_store_type_limit_buy_124",faith::e_store_type_limit_buy_124);
   tolua_constant(tolua_S,"e_store_type_limit_buy_125",faith::e_store_type_limit_buy_125);
   tolua_constant(tolua_S,"e_store_type_limit_buy_126",faith::e_store_type_limit_buy_126);
   tolua_constant(tolua_S,"e_store_type_limit_buy_127",faith::e_store_type_limit_buy_127);
   tolua_constant(tolua_S,"e_store_type_limit_buy_128",faith::e_store_type_limit_buy_128);
   tolua_constant(tolua_S,"e_store_type_limit_buy_129",faith::e_store_type_limit_buy_129);
   tolua_constant(tolua_S,"e_store_type_limit_buy_130",faith::e_store_type_limit_buy_130);
   tolua_constant(tolua_S,"e_store_type_limit_buy_131",faith::e_store_type_limit_buy_131);
   tolua_constant(tolua_S,"e_store_type_limit_buy_132",faith::e_store_type_limit_buy_132);
   tolua_constant(tolua_S,"e_store_type_limit_buy_133",faith::e_store_type_limit_buy_133);
   tolua_constant(tolua_S,"e_store_type_limit_buy_134",faith::e_store_type_limit_buy_134);
   tolua_constant(tolua_S,"e_store_type_limit_buy_135",faith::e_store_type_limit_buy_135);
   tolua_constant(tolua_S,"e_store_type_limit_buy_136",faith::e_store_type_limit_buy_136);
   tolua_constant(tolua_S,"e_store_type_limit_buy_137",faith::e_store_type_limit_buy_137);
   tolua_constant(tolua_S,"e_store_type_limit_buy_138",faith::e_store_type_limit_buy_138);
   tolua_constant(tolua_S,"e_store_type_limit_buy_139",faith::e_store_type_limit_buy_139);
   tolua_constant(tolua_S,"e_store_type_limit_buy_140",faith::e_store_type_limit_buy_140);
   tolua_constant(tolua_S,"e_store_type_max",faith::e_store_type_max);
   tolua_constant(tolua_S,"e_element_heart_operate_get_one",faith::e_element_heart_operate_get_one);
   tolua_constant(tolua_S,"e_element_heart_operate_get_ten",faith::e_element_heart_operate_get_ten);
   tolua_constant(tolua_S,"e_element_heart_operate_upgrade",faith::e_element_heart_operate_upgrade);
   tolua_constant(tolua_S,"e_element_heart_operate_equip_on",faith::e_element_heart_operate_equip_on);
   tolua_constant(tolua_S,"e_element_heart_operate_equip_off",faith::e_element_heart_operate_equip_off);
   tolua_constant(tolua_S,"e_element_heart_operate_summon",faith::e_element_heart_operate_summon);
   tolua_constant(tolua_S,"e_element_heart_operate_tidy",faith::e_element_heart_operate_tidy);
   tolua_constant(tolua_S,"e_element_heart_operate_purchase_equip_slot",faith::e_element_heart_operate_purchase_equip_slot);
   tolua_constant(tolua_S,"e_element_heart_operate_recovery",faith::e_element_heart_operate_recovery);
   tolua_constant(tolua_S,"e_element_heart_operate_common",faith::e_element_heart_operate_common);
   tolua_constant(tolua_S,"e_element_heart_operate_superior",faith::e_element_heart_operate_superior);
   tolua_constant(tolua_S,"e_element_heart_operate_rare",faith::e_element_heart_operate_rare);
   tolua_constant(tolua_S,"e_element_heart_operate_epic",faith::e_element_heart_operate_epic);
   tolua_constant(tolua_S,"e_element_heart_operate_legend",faith::e_element_heart_operate_legend);
   tolua_constant(tolua_S,"e_element_heart_operate_merge",faith::e_element_heart_operate_merge);
   tolua_constant(tolua_S,"e_element_heart_operate_core_unlock",faith::e_element_heart_operate_core_unlock);
   tolua_constant(tolua_S,"e_element_heart_operate_core_upgrade",faith::e_element_heart_operate_core_upgrade);
   tolua_constant(tolua_S,"e_element_heart_operate_max",faith::e_element_heart_operate_max);
   tolua_constant(tolua_S,"e_top_tip_type_normal",faith::e_top_tip_type_normal);
   tolua_constant(tolua_S,"e_top_tip_type_important",faith::e_top_tip_type_important);
   tolua_constant(tolua_S,"e_top_tip_type_max",faith::e_top_tip_type_max);
   tolua_constant(tolua_S,"e_sprite_operate_fight",faith::e_sprite_operate_fight);
   tolua_constant(tolua_S,"e_sprite_operate_equip",faith::e_sprite_operate_equip);
   tolua_constant(tolua_S,"e_sprite_operate_unequip",faith::e_sprite_operate_unequip);
   tolua_constant(tolua_S,"e_sprite_operate_upgrade",faith::e_sprite_operate_upgrade);
   tolua_constant(tolua_S,"e_sprite_operate_sell",faith::e_sprite_operate_sell);
   tolua_constant(tolua_S,"e_sprite_operate_send_to_item_bag",faith::e_sprite_operate_send_to_item_bag);
   tolua_constant(tolua_S,"e_sprite_operate_tidy",faith::e_sprite_operate_tidy);
   tolua_constant(tolua_S,"e_sprite_operate_auto_sell",faith::e_sprite_operate_auto_sell);
   tolua_constant(tolua_S,"e_sprite_operate_purchase_equip_slot",faith::e_sprite_operate_purchase_equip_slot);
   tolua_constant(tolua_S,"e_sprite_operate_purchase_illusion",faith::e_sprite_operate_purchase_illusion);
   tolua_constant(tolua_S,"e_sprite_operate_use_illusion",faith::e_sprite_operate_use_illusion);
   tolua_constant(tolua_S,"e_sprite_operate_star_upgrade",faith::e_sprite_operate_star_upgrade);
   tolua_constant(tolua_S,"e_sprite_operate_max",faith::e_sprite_operate_max);
   tolua_constant(tolua_S,"e_chat_type_none",faith::e_chat_type_none);
   tolua_constant(tolua_S,"e_chat_type_system",faith::e_chat_type_system);
   tolua_constant(tolua_S,"e_chat_type_world",faith::e_chat_type_world);
   tolua_constant(tolua_S,"e_chat_type_legion",faith::e_chat_type_legion);
   tolua_constant(tolua_S,"e_chat_type_team",faith::e_chat_type_team);
   tolua_constant(tolua_S,"e_chat_type_private",faith::e_chat_type_private);
   tolua_constant(tolua_S,"e_chat_type_near",faith::e_chat_type_near);
   tolua_constant(tolua_S,"e_chat_type_total",faith::e_chat_type_total);
   tolua_constant(tolua_S,"e_chat_type_friend",faith::e_chat_type_friend);
   tolua_constant(tolua_S,"e_chat_type_world_recruit",faith::e_chat_type_world_recruit);
   tolua_constant(tolua_S,"e_chat_type_legion_recruit",faith::e_chat_type_legion_recruit);
   tolua_constant(tolua_S,"e_chat_type_radio_host",faith::e_chat_type_radio_host);
   tolua_constant(tolua_S,"e_chat_type_server",faith::e_chat_type_server);
   tolua_constant(tolua_S,"e_chat_type_all_server",faith::e_chat_type_all_server);
   tolua_constant(tolua_S,"e_chat_type_max",faith::e_chat_type_max);
   tolua_constant(tolua_S,"e_recharge_type_none",faith::e_recharge_type_none);
   tolua_constant(tolua_S,"e_recharge_type_vip",faith::e_recharge_type_vip);
   tolua_constant(tolua_S,"e_recharge_type_month_card",faith::e_recharge_type_month_card);
   tolua_constant(tolua_S,"e_recharge_type_exclusive",faith::e_recharge_type_exclusive);
   tolua_constant(tolua_S,"e_recharge_type_max",faith::e_recharge_type_max);
   tolua_constant(tolua_S,"e_relationlist_type_friend",faith::e_relationlist_type_friend);
   tolua_constant(tolua_S,"e_relationlist_type_black",faith::e_relationlist_type_black);
   tolua_constant(tolua_S,"e_relationlist_type_marry",faith::e_relationlist_type_marry);
   tolua_constant(tolua_S,"e_relationlist_type_hate",faith::e_relationlist_type_hate);
   tolua_constant(tolua_S,"e_relationlist_type_friendex",faith::e_relationlist_type_friendex);
   tolua_constant(tolua_S,"e_relationlist_type_other_req_add",faith::e_relationlist_type_other_req_add);
   tolua_constant(tolua_S,"e_relationlist_type_chat",faith::e_relationlist_type_chat);
   tolua_constant(tolua_S,"e_relationlist_type_lucky",faith::e_relationlist_type_lucky);
   tolua_constant(tolua_S,"e_relationlist_type_other_req_add_lucky",faith::e_relationlist_type_other_req_add_lucky);
   tolua_constant(tolua_S,"e_relationlist_type_max",faith::e_relationlist_type_max);
   tolua_constant(tolua_S,"e_first_kill_prize_type_cant_get",faith::e_first_kill_prize_type_cant_get);
   tolua_constant(tolua_S,"e_first_kill_prize_type_not_get",faith::e_first_kill_prize_type_not_get);
   tolua_constant(tolua_S,"e_first_kill_prize_type_was_get",faith::e_first_kill_prize_type_was_get);
   tolua_constant(tolua_S,"e_first_kill_prize_type_max",faith::e_first_kill_prize_type_max);
   tolua_constant(tolua_S,"e_service_goal_state_unfinished",faith::e_service_goal_state_unfinished);
   tolua_constant(tolua_S,"e_service_goal_state_finished",faith::e_service_goal_state_finished);
   tolua_constant(tolua_S,"e_service_goal_state_has_been_received",faith::e_service_goal_state_has_been_received);
   tolua_constant(tolua_S,"e_service_goal_state_max",faith::e_service_goal_state_max);
   tolua_constant(tolua_S,"e_service_goal_inst_data_id",faith::e_service_goal_inst_data_id);
   tolua_constant(tolua_S,"e_service_goal_inst_data_schedule",faith::e_service_goal_inst_data_schedule);
   tolua_constant(tolua_S,"e_service_goal_inst_data_state",faith::e_service_goal_inst_data_state);
   tolua_constant(tolua_S,"e_service_goal_inst_data_active_state",faith::e_service_goal_inst_data_active_state);
   tolua_constant(tolua_S,"e_service_goal_inst_data_over_time",faith::e_service_goal_inst_data_over_time);
   tolua_constant(tolua_S,"e_service_goal_inst_data_max",faith::e_service_goal_inst_data_max);
   tolua_constant(tolua_S,"e_service_goal_operate_type_none",faith::e_service_goal_operate_type_none);
   tolua_constant(tolua_S,"e_service_goal_operate_type_receive",faith::e_service_goal_operate_type_receive);
   tolua_constant(tolua_S,"e_service_goal_operate_type_max",faith::e_service_goal_operate_type_max);
   tolua_constant(tolua_S,"e_service_goal_type_role_level",faith::e_service_goal_type_role_level);
   tolua_constant(tolua_S,"e_service_goal_type_role_gs",faith::e_service_goal_type_role_gs);
   tolua_constant(tolua_S,"e_service_goal_type_big_reward",faith::e_service_goal_type_big_reward);
   tolua_constant(tolua_S,"e_service_goal_type_strength_any",faith::e_service_goal_type_strength_any);
   tolua_constant(tolua_S,"e_service_goal_type_addition_the_body",faith::e_service_goal_type_addition_the_body);
   tolua_constant(tolua_S,"e_service_goal_type_mount_level",faith::e_service_goal_type_mount_level);
   tolua_constant(tolua_S,"e_service_goal_type_mount_num",faith::e_service_goal_type_mount_num);
   tolua_constant(tolua_S,"e_service_goal_type_friend_num",faith::e_service_goal_type_friend_num);
   tolua_constant(tolua_S,"e_service_goal_type_friendliness",faith::e_service_goal_type_friendliness);
   tolua_constant(tolua_S,"e_service_goal_type_clearance_fuben",faith::e_service_goal_type_clearance_fuben);
   tolua_constant(tolua_S,"e_service_goal_type_body_has_color_equip",faith::e_service_goal_type_body_has_color_equip);
   tolua_constant(tolua_S,"e_service_goal_type_legion_donate_num",faith::e_service_goal_type_legion_donate_num);
   tolua_constant(tolua_S,"e_service_goal_type_soldier_rank_level",faith::e_service_goal_type_soldier_rank_level);
   tolua_constant(tolua_S,"e_service_goal_type_arena_rank_level",faith::e_service_goal_type_arena_rank_level);
   tolua_constant(tolua_S,"e_service_goal_type_actived_illustration_num",faith::e_service_goal_type_actived_illustration_num);
   tolua_constant(tolua_S,"e_service_goal_type_actived_any_chapter_illustration",faith::e_service_goal_type_actived_any_chapter_illustration);
   tolua_constant(tolua_S,"e_service_goal_type_demons_tower_layer_num",faith::e_service_goal_type_demons_tower_layer_num);
   tolua_constant(tolua_S,"e_service_goal_type_achievement_title",faith::e_service_goal_type_achievement_title);
   tolua_constant(tolua_S,"e_service_goal_type_wing_level",faith::e_service_goal_type_wing_level);
   tolua_constant(tolua_S,"e_service_goal_type_element_heart_total_level",faith::e_service_goal_type_element_heart_total_level);
   tolua_constant(tolua_S,"e_service_goal_type_equiped_element_heart_num",faith::e_service_goal_type_equiped_element_heart_num);
   tolua_constant(tolua_S,"e_service_goal_type_equiped_spirit_total_level",faith::e_service_goal_type_equiped_spirit_total_level);
   tolua_constant(tolua_S,"e_service_goal_type_equiped_spirit_num",faith::e_service_goal_type_equiped_spirit_num);
   tolua_constant(tolua_S,"e_service_goal_type_belief_rank",faith::e_service_goal_type_belief_rank);
   tolua_constant(tolua_S,"e_service_goal_type_get_all_reward",faith::e_service_goal_type_get_all_reward);
   tolua_constant(tolua_S,"e_service_goal_type_final",faith::e_service_goal_type_final);
   tolua_constant(tolua_S,"e_service_goal_type_god_hood_grade",faith::e_service_goal_type_god_hood_grade);
   tolua_constant(tolua_S,"e_service_goal_type_seven_day_final",faith::e_service_goal_type_seven_day_final);
   tolua_constant(tolua_S,"e_service_goal_type_no_limit_time_final",faith::e_service_goal_type_no_limit_time_final);
   tolua_constant(tolua_S,"e_service_goal_type_awaken_level",faith::e_service_goal_type_awaken_level);
   tolua_constant(tolua_S,"e_service_goal_type_max",faith::e_service_goal_type_max);
   tolua_constant(tolua_S,"e_service_goal_active_type_basis",faith::e_service_goal_active_type_basis);
   tolua_constant(tolua_S,"e_service_goal_active_type_advance",faith::e_service_goal_active_type_advance);
   tolua_constant(tolua_S,"e_service_goal_active_type_final",faith::e_service_goal_active_type_final);
   tolua_constant(tolua_S,"e_service_goal_active_type_seven_day",faith::e_service_goal_active_type_seven_day);
   tolua_constant(tolua_S,"e_service_goal_active_type_max",faith::e_service_goal_active_type_max);
   tolua_constant(tolua_S,"e_team_operate_follow_cap",faith::e_team_operate_follow_cap);
   tolua_constant(tolua_S,"e_team_operate_urge_cap",faith::e_team_operate_urge_cap);
   tolua_constant(tolua_S,"e_team_operate_call_together",faith::e_team_operate_call_together);
   tolua_constant(tolua_S,"e_team_operate_urge_together",faith::e_team_operate_urge_together);
   tolua_constant(tolua_S,"e_team_operate_max",faith::e_team_operate_max);
   tolua_constant(tolua_S,"e_relation_operate_result_success",faith::e_relation_operate_result_success);
   tolua_constant(tolua_S,"e_relation_operate_result_offline",faith::e_relation_operate_result_offline);
   tolua_constant(tolua_S,"e_relation_operate_result_add_friend_success",faith::e_relation_operate_result_add_friend_success);
   tolua_constant(tolua_S,"e_relation_operate_result_del_friend_success",faith::e_relation_operate_result_del_friend_success);
   tolua_constant(tolua_S,"e_relation_operate_result_add_black_success",faith::e_relation_operate_result_add_black_success);
   tolua_constant(tolua_S,"e_relation_operate_result_del_black_success",faith::e_relation_operate_result_del_black_success);
   tolua_constant(tolua_S,"e_relation_operate_result_add_hate_success",faith::e_relation_operate_result_add_hate_success);
   tolua_constant(tolua_S,"e_relation_operate_result_add_friend_all_once",faith::e_relation_operate_result_add_friend_all_once);
   tolua_constant(tolua_S,"e_relation_operate_result_cant_operate_self",faith::e_relation_operate_result_cant_operate_self);
   tolua_constant(tolua_S,"e_relation_operate_result_friend_list_full",faith::e_relation_operate_result_friend_list_full);
   tolua_constant(tolua_S,"e_relation_operate_result_hate_list_full",faith::e_relation_operate_result_hate_list_full);
   tolua_constant(tolua_S,"e_relation_operate_result_black_list_full",faith::e_relation_operate_result_black_list_full);
   tolua_constant(tolua_S,"e_relation_operate_result_friend_exist",faith::e_relation_operate_result_friend_exist);
   tolua_constant(tolua_S,"e_relation_operate_result_black_exist",faith::e_relation_operate_result_black_exist);
   tolua_constant(tolua_S,"e_relation_operate_result_hate_exist",faith::e_relation_operate_result_hate_exist);
   tolua_constant(tolua_S,"e_relation_operate_result_too_fast",faith::e_relation_operate_result_too_fast);
   tolua_constant(tolua_S,"e_relation_operate_result_req_friend_success",faith::e_relation_operate_result_req_friend_success);
   tolua_constant(tolua_S,"e_relation_operate_result_pass_req_friend",faith::e_relation_operate_result_pass_req_friend);
   tolua_constant(tolua_S,"e_relation_operate_result_other_add_friend",faith::e_relation_operate_result_other_add_friend);
   tolua_constant(tolua_S,"e_relation_operate_result_other_is_cross_server",faith::e_relation_operate_result_other_is_cross_server);
   tolua_constant(tolua_S,"e_relation_operate_result_max",faith::e_relation_operate_result_max);
   tolua_constant(tolua_S,"e_item_wing_type_non",faith::e_item_wing_type_non);
   tolua_constant(tolua_S,"e_item_wing_type_normal",faith::e_item_wing_type_normal);
   tolua_constant(tolua_S,"e_item_wing_type_shape_no_illusion",faith::e_item_wing_type_shape_no_illusion);
   tolua_constant(tolua_S,"e_item_wing_type_shape_illusion",faith::e_item_wing_type_shape_illusion);
   tolua_constant(tolua_S,"e_item_wing_type_illusion",faith::e_item_wing_type_illusion);
   tolua_constant(tolua_S,"e_item_wing_type_special_santo",faith::e_item_wing_type_special_santo);
   tolua_constant(tolua_S,"e_item_wing_type_max",faith::e_item_wing_type_max);
   tolua_constant(tolua_S,"e_convert_type_bind_siliver",faith::e_convert_type_bind_siliver);
   tolua_constant(tolua_S,"e_convert_type_exp",faith::e_convert_type_exp);
   tolua_constant(tolua_S,"e_convert_type_bind_diamond",faith::e_convert_type_bind_diamond);
   tolua_constant(tolua_S,"e_convert_type_bag_bind_siliver",faith::e_convert_type_bag_bind_siliver);
   tolua_constant(tolua_S,"e_convert_type_bag_siliver",faith::e_convert_type_bag_siliver);
   tolua_constant(tolua_S,"e_convert_type_max",faith::e_convert_type_max);
   tolua_constant(tolua_S,"e_belief_series_type_spring",faith::e_belief_series_type_spring);
   tolua_constant(tolua_S,"e_belief_series_type_summer",faith::e_belief_series_type_summer);
   tolua_constant(tolua_S,"e_belief_series_type_autumn",faith::e_belief_series_type_autumn);
   tolua_constant(tolua_S,"e_belief_series_type_winter",faith::e_belief_series_type_winter);
   tolua_constant(tolua_S,"e_belief_series_type_max",faith::e_belief_series_type_max);
   tolua_constant(tolua_S,"e_belief_buff_level_index_first",faith::e_belief_buff_level_index_first);
   tolua_constant(tolua_S,"e_belief_buff_level_index_second",faith::e_belief_buff_level_index_second);
   tolua_constant(tolua_S,"e_belief_buff_level_index_third",faith::e_belief_buff_level_index_third);
   tolua_constant(tolua_S,"e_belief_buff_level_max",faith::e_belief_buff_level_max);
   tolua_constant(tolua_S,"e_legion_call_type_call_legion_boss",faith::e_legion_call_type_call_legion_boss);
   tolua_constant(tolua_S,"e_legion_call_type_call_city_war",faith::e_legion_call_type_call_city_war);
   tolua_constant(tolua_S,"e_legion_call_type_call_bonfire",faith::e_legion_call_type_call_bonfire);
   tolua_constant(tolua_S,"e_legion_call_type_call_elite_boss",faith::e_legion_call_type_call_elite_boss);
   tolua_constant(tolua_S,"e_legion_call_type_call_world_boss",faith::e_legion_call_type_call_world_boss);
   tolua_constant(tolua_S,"e_legion_call_type_max",faith::e_legion_call_type_max);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_low",faith::e_reduce_pk_value_prop_quality_low);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_mid",faith::e_reduce_pk_value_prop_quality_mid);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_high",faith::e_reduce_pk_value_prop_quality_high);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_max",faith::e_reduce_pk_value_prop_quality_max);
   tolua_constant(tolua_S,"e_spirit_operate_result_unknown",faith::e_spirit_operate_result_unknown);
   tolua_constant(tolua_S,"e_spirit_operate_result_success",faith::e_spirit_operate_result_success);
   tolua_constant(tolua_S,"e_spirit_operate_result_no_space",faith::e_spirit_operate_result_no_space);
   tolua_constant(tolua_S,"e_spirit_operate_result_max",faith::e_spirit_operate_result_max);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_exp",faith::e_daily_must_do_typ_raid_exp);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_daily",faith::e_daily_must_do_typ_raid_daily);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_devil_square",faith::e_daily_must_do_typ_raid_devil_square);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_ancient_battlefield",faith::e_daily_must_do_typ_raid_ancient_battlefield);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_siliver",faith::e_daily_must_do_typ_raid_siliver);
   tolua_constant(tolua_S,"e_daily_must_do_typ_community_fight",faith::e_daily_must_do_typ_community_fight);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_drame",faith::e_daily_must_do_typ_raid_drame);
   tolua_constant(tolua_S,"e_daily_must_do_typ_bloody_castle",faith::e_daily_must_do_typ_bloody_castle);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_multiple_princess_guard_war",faith::e_daily_must_do_typ_raid_multiple_princess_guard_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_multiple_lava_hellfire",faith::e_daily_must_do_typ_raid_multiple_lava_hellfire);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_multiple_abyss_of_fear",faith::e_daily_must_do_typ_raid_multiple_abyss_of_fear);
   tolua_constant(tolua_S,"e_daily_must_do_typ_demon_tower",faith::e_daily_must_do_typ_demon_tower);
   tolua_constant(tolua_S,"e_daily_must_do_typ_angel_temple",faith::e_daily_must_do_typ_angel_temple);
   tolua_constant(tolua_S,"e_daily_must_do_typ_pk_king",faith::e_daily_must_do_typ_pk_king);
   tolua_constant(tolua_S,"e_daily_must_do_typ_arena",faith::e_daily_must_do_typ_arena);
   tolua_constant(tolua_S,"e_daily_must_do_typ_crusade_mission",faith::e_daily_must_do_typ_crusade_mission);
   tolua_constant(tolua_S,"e_daily_must_do_typ_crystal_illusion",faith::e_daily_must_do_typ_crystal_illusion);
   tolua_constant(tolua_S,"e_daily_must_do_typ_mingmie_xukong",faith::e_daily_must_do_typ_mingmie_xukong);
   tolua_constant(tolua_S,"e_daily_must_do_typ_out_board",faith::e_daily_must_do_typ_out_board);
   tolua_constant(tolua_S,"e_daily_must_do_typ_convert",faith::e_daily_must_do_typ_convert);
   tolua_constant(tolua_S,"e_daily_must_do_typ_rank",faith::e_daily_must_do_typ_rank);
   tolua_constant(tolua_S,"e_daily_must_do_typ_gold_army",faith::e_daily_must_do_typ_gold_army);
   tolua_constant(tolua_S,"e_daily_must_do_typ_world_boss",faith::e_daily_must_do_typ_world_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_sky_treasure_box",faith::e_daily_must_do_typ_sky_treasure_box);
   tolua_constant(tolua_S,"e_daily_must_do_typ_field_treasure_box",faith::e_daily_must_do_typ_field_treasure_box);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_boss",faith::e_daily_must_do_typ_legion_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_meditation",faith::e_daily_must_do_typ_meditation);
   tolua_constant(tolua_S,"e_daily_must_do_typ_activaty_degree",faith::e_daily_must_do_typ_activaty_degree);
   tolua_constant(tolua_S,"e_daily_must_do_typ_city_war",faith::e_daily_must_do_typ_city_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_fight",faith::e_daily_must_do_typ_cross_fight);
   tolua_constant(tolua_S,"e_daily_must_do_typ_gold_store",faith::e_daily_must_do_typ_gold_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_magic_crystal_store",faith::e_daily_must_do_typ_magic_crystal_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_ancient_coin_store",faith::e_daily_must_do_typ_ancient_coin_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_hope_store",faith::e_daily_must_do_typ_hope_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_store",faith::e_daily_must_do_typ_legion_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_hope_item",faith::e_daily_must_do_typ_hope_item);
   tolua_constant(tolua_S,"e_daily_must_do_typ_shop",faith::e_daily_must_do_typ_shop);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_bonfire",faith::e_daily_must_do_typ_legion_bonfire);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_war",faith::e_daily_must_do_typ_legion_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_boss_home",faith::e_daily_must_do_typ_boss_home);
   tolua_constant(tolua_S,"e_daily_must_do_typ_boss_island",faith::e_daily_must_do_typ_boss_island);
   tolua_constant(tolua_S,"e_daily_must_do_typ_belief_cloister",faith::e_daily_must_do_typ_belief_cloister);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_pk",faith::e_daily_must_do_typ_cross_pk);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_rob",faith::e_daily_must_do_typ_cross_rob);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_dance",faith::e_daily_must_do_typ_legion_dance);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_answer",faith::e_daily_must_do_typ_legion_answer);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_ladder",faith::e_daily_must_do_typ_cross_ladder);
   tolua_constant(tolua_S,"e_daily_must_do_typ_single_boss",faith::e_daily_must_do_typ_single_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_element_war",faith::e_daily_must_do_typ_element_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_vip_boss_home",faith::e_daily_must_do_typ_vip_boss_home);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_server_boss",faith::e_daily_must_do_typ_cross_server_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_oracle_trial",faith::e_daily_must_do_typ_oracle_trial);
   tolua_constant(tolua_S,"e_daily_must_do_typ_attack_city",faith::e_daily_must_do_typ_attack_city);
   tolua_constant(tolua_S,"e_daily_must_do_typ_max",faith::e_daily_must_do_typ_max);
   tolua_constant(tolua_S,"e_daily_active_degree_type_cost_one_diamond",faith::e_daily_active_degree_type_cost_one_diamond);
   tolua_constant(tolua_S,"e_daily_active_degree_type_ten_daily_mission",faith::e_daily_active_degree_type_ten_daily_mission);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_normal_raid",faith::e_daily_active_degree_type_three_normal_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_elite_raid",faith::e_daily_active_degree_type_three_elite_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_hell_raid",faith::e_daily_active_degree_type_three_hell_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_empire_raid",faith::e_daily_active_degree_type_empire_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_temple_raid",faith::e_daily_active_degree_type_temple_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_warp_raid",faith::e_daily_active_degree_type_warp_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_upgrade",faith::e_daily_active_degree_type_three_upgrade);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_addon",faith::e_daily_active_degree_type_three_addon);
   tolua_constant(tolua_S,"e_daily_active_degree_type_kill_200_monster",faith::e_daily_active_degree_type_kill_200_monster);
   tolua_constant(tolua_S,"e_daily_active_degree_type_kill_500_monster",faith::e_daily_active_degree_type_kill_500_monster);
   tolua_constant(tolua_S,"e_daily_active_degree_type_kill_1000_monster",faith::e_daily_active_degree_type_kill_1000_monster);
   tolua_constant(tolua_S,"e_daily_active_degree_type_login",faith::e_daily_active_degree_type_login);
   tolua_constant(tolua_S,"e_daily_active_degree_type_online_30_minutes",faith::e_daily_active_degree_type_online_30_minutes);
   tolua_constant(tolua_S,"e_daily_active_degree_type_strengthing_one_spirit",faith::e_daily_active_degree_type_strengthing_one_spirit);
   tolua_constant(tolua_S,"e_daily_active_degree_type_strengthing_one_mount",faith::e_daily_active_degree_type_strengthing_one_mount);
   tolua_constant(tolua_S,"e_daily_active_degree_type_strengthing_one_wing",faith::e_daily_active_degree_type_strengthing_one_wing);
   tolua_constant(tolua_S,"e_daily_active_degree_type_upgrade_one_element_heart",faith::e_daily_active_degree_type_upgrade_one_element_heart);
   tolua_constant(tolua_S,"e_daily_active_degree_type_upgrade_one_patron_saint",faith::e_daily_active_degree_type_upgrade_one_patron_saint);
   tolua_constant(tolua_S,"e_daily_active_degree_type_upgrade_one_belief",faith::e_daily_active_degree_type_upgrade_one_belief);
   tolua_constant(tolua_S,"e_daily_active_degree_type_submit_one_illustration",faith::e_daily_active_degree_type_submit_one_illustration);
   tolua_constant(tolua_S,"e_daily_active_degree_type_attend_three_arena",faith::e_daily_active_degree_type_attend_three_arena);
   tolua_constant(tolua_S,"e_daily_active_degree_type_attend_one_city_war",faith::e_daily_active_degree_type_attend_one_city_war);
   tolua_constant(tolua_S,"e_daily_active_degree_type_attend_one_legion_war",faith::e_daily_active_degree_type_attend_one_legion_war);
   tolua_constant(tolua_S,"e_daily_active_degree_type_max",faith::e_daily_active_degree_type_max);
   tolua_constant(tolua_S,"e_notice_param_go_to_location",faith::e_notice_param_go_to_location);
   tolua_constant(tolua_S,"e_notice_param_open_widget",faith::e_notice_param_open_widget);
   tolua_constant(tolua_S,"e_notice_param_item_info",faith::e_notice_param_item_info);
   tolua_constant(tolua_S,"e_notice_param_max",faith::e_notice_param_max);
   tolua_constant(tolua_S,"e_RankingIndex_gs",faith::e_RankingIndex_gs);
   tolua_constant(tolua_S,"e_RankingIndex_level",faith::e_RankingIndex_level);
   tolua_constant(tolua_S,"e_RankingIndex_box_map_level",faith::e_RankingIndex_box_map_level);
   tolua_constant(tolua_S,"e_RankingIndex_demonstower",faith::e_RankingIndex_demonstower);
   tolua_constant(tolua_S,"e_RankingIndex_wing",faith::e_RankingIndex_wing);
   tolua_constant(tolua_S,"e_RankingIndex_mount",faith::e_RankingIndex_mount);
   tolua_constant(tolua_S,"e_RankingIndex_worship",faith::e_RankingIndex_worship);
   tolua_constant(tolua_S,"e_RankingIndex_arena",faith::e_RankingIndex_arena);
   tolua_constant(tolua_S,"e_RankingIndex_legion_core",faith::e_RankingIndex_legion_core);
   tolua_constant(tolua_S,"e_RankingIndex_gs_barserker",faith::e_RankingIndex_gs_barserker);
   tolua_constant(tolua_S,"e_RankingIndex_gs_wizard",faith::e_RankingIndex_gs_wizard);
   tolua_constant(tolua_S,"e_RankingIndex_gs_guardian",faith::e_RankingIndex_gs_guardian);
   tolua_constant(tolua_S,"e_RankingIndex_gs_assassinator",faith::e_RankingIndex_gs_assassinator);
   tolua_constant(tolua_S,"e_RankingIndex_level_barserker",faith::e_RankingIndex_level_barserker);
   tolua_constant(tolua_S,"e_RankingIndex_level_wizard",faith::e_RankingIndex_level_wizard);
   tolua_constant(tolua_S,"e_RankingIndex_level_guardian",faith::e_RankingIndex_level_guardian);
   tolua_constant(tolua_S,"e_RankingIndex_level_assassinator",faith::e_RankingIndex_level_assassinator);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_main",faith::e_RankingIndex_equipment_main);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_weapon",faith::e_RankingIndex_equipment_weapon);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_necklace",faith::e_RankingIndex_equipment_necklace);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_armor",faith::e_RankingIndex_equipment_armor);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_pants",faith::e_RankingIndex_equipment_pants);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_hat",faith::e_RankingIndex_equipment_hat);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_shoes",faith::e_RankingIndex_equipment_shoes);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_glove",faith::e_RankingIndex_equipment_glove);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_ring",faith::e_RankingIndex_equipment_ring);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_earring",faith::e_RankingIndex_equipment_earring);
   tolua_constant(tolua_S,"e_RankingIndex_mount_train",faith::e_RankingIndex_mount_train);
   tolua_constant(tolua_S,"e_RankingIndex_wing_train",faith::e_RankingIndex_wing_train);
   tolua_constant(tolua_S,"e_RankingIndex_elves_train",faith::e_RankingIndex_elves_train);
   tolua_constant(tolua_S,"e_RankingIndex_belief",faith::e_RankingIndex_belief);
   tolua_constant(tolua_S,"e_RankingIndex_element",faith::e_RankingIndex_element);
   tolua_constant(tolua_S,"e_RankingIndex_inspiration",faith::e_RankingIndex_inspiration);
   tolua_constant(tolua_S,"e_RankingIndex_military_rank",faith::e_RankingIndex_military_rank);
   tolua_constant(tolua_S,"e_RankingIndex_achievement",faith::e_RankingIndex_achievement);
   tolua_constant(tolua_S,"e_RankingIndex_illustrated",faith::e_RankingIndex_illustrated);
   tolua_constant(tolua_S,"e_RankingIndex_legion_territory",faith::e_RankingIndex_legion_territory);
   tolua_constant(tolua_S,"e_RankingIndex_legion_boss",faith::e_RankingIndex_legion_boss);
   tolua_constant(tolua_S,"e_RankingIndex_legion_bonfire",faith::e_RankingIndex_legion_bonfire);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_one",faith::e_RankingIndex_world_boss_one);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_two",faith::e_RankingIndex_world_boss_two);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_three",faith::e_RankingIndex_world_boss_three);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_four",faith::e_RankingIndex_world_boss_four);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_five",faith::e_RankingIndex_world_boss_five);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_six",faith::e_RankingIndex_world_boss_six);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_seven",faith::e_RankingIndex_world_boss_seven);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_one",faith::e_RankingIndex_world_elite_one);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_two",faith::e_RankingIndex_world_elite_two);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_three",faith::e_RankingIndex_world_elite_three);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_four",faith::e_RankingIndex_world_elite_four);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_five",faith::e_RankingIndex_world_elite_five);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_six",faith::e_RankingIndex_world_elite_six);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_seven",faith::e_RankingIndex_world_elite_seven);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_eight",faith::e_RankingIndex_world_elite_eight);
   tolua_constant(tolua_S,"e_RankingIndex_elves_gs",faith::e_RankingIndex_elves_gs);
   tolua_constant(tolua_S,"e_RankingIndex_vip_level_one",faith::e_RankingIndex_vip_level_one);
   tolua_constant(tolua_S,"e_RankingIndex_vip_level_five",faith::e_RankingIndex_vip_level_five);
   tolua_constant(tolua_S,"e_RankingIndex_vip_level_seven",faith::e_RankingIndex_vip_level_seven);
   tolua_constant(tolua_S,"e_RankingIndex_recharge_rmb",faith::e_RankingIndex_recharge_rmb);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_total_recharge",faith::e_RankingIndex_time_limit_activity_total_recharge);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_total_cost",faith::e_RankingIndex_time_limit_activity_total_cost);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_treasure",faith::e_RankingIndex_time_limit_activity_treasure);
   tolua_constant(tolua_S,"e_RankingIndex_gs_all",faith::e_RankingIndex_gs_all);
   tolua_constant(tolua_S,"e_RankingIndex_cross_server_plinder_guard_value",faith::e_RankingIndex_cross_server_plinder_guard_value);
   tolua_constant(tolua_S,"e_RankingIndex_legion_gs_all",faith::e_RankingIndex_legion_gs_all);
   tolua_constant(tolua_S,"e_RankingIndex_cross_ladder_score",faith::e_RankingIndex_cross_ladder_score);
   tolua_constant(tolua_S,"e_RankingIndex_feather_heart",faith::e_RankingIndex_feather_heart);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_one",faith::e_RankingIndex_cross_boss_one);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_two",faith::e_RankingIndex_cross_boss_two);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_three",faith::e_RankingIndex_cross_boss_three);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_four",faith::e_RankingIndex_cross_boss_four);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_five",faith::e_RankingIndex_cross_boss_five);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_six",faith::e_RankingIndex_cross_boss_six);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_seven",faith::e_RankingIndex_cross_boss_seven);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_eight",faith::e_RankingIndex_cross_boss_eight);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_nine",faith::e_RankingIndex_cross_boss_nine);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_ten",faith::e_RankingIndex_cross_boss_ten);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_eleven",faith::e_RankingIndex_cross_boss_eleven);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_tweleve",faith::e_RankingIndex_cross_boss_tweleve);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_gs_up",faith::e_RankingIndex_time_limit_activity_gs_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_mount_up",faith::e_RankingIndex_time_limit_activity_mount_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_wing_up",faith::e_RankingIndex_time_limit_activity_wing_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_equip_up",faith::e_RankingIndex_time_limit_activity_equip_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_spirit_up",faith::e_RankingIndex_time_limit_activity_spirit_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_gs_up",faith::e_RankingIndex_time_limit_activity_cross_gs_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_mount_up",faith::e_RankingIndex_time_limit_activity_cross_mount_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_wing_up",faith::e_RankingIndex_time_limit_activity_cross_wing_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_equip_up",faith::e_RankingIndex_time_limit_activity_cross_equip_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_spirit_up",faith::e_RankingIndex_time_limit_activity_cross_spirit_up);
   tolua_constant(tolua_S,"e_RankingIndex_level_all",faith::e_RankingIndex_level_all);
   tolua_constant(tolua_S,"e_RankingIndex_couple",faith::e_RankingIndex_couple);
   tolua_constant(tolua_S,"e_RankingIndex_Oracle_Trial",faith::e_RankingIndex_Oracle_Trial);
   tolua_constant(tolua_S,"e_RankingIndex_cross_ladder_last_seaon",faith::e_RankingIndex_cross_ladder_last_seaon);
   tolua_constant(tolua_S,"e_RankingIndex_cross_ladder_cur_season",faith::e_RankingIndex_cross_ladder_cur_season);
   tolua_constant(tolua_S,"e_RankingIndex_pk_king",faith::e_RankingIndex_pk_king);
   tolua_constant(tolua_S,"e_RankingIndex_element_rank_list",faith::e_RankingIndex_element_rank_list);
   tolua_constant(tolua_S,"e_RankingIndex_element_war_cur_season",faith::e_RankingIndex_element_war_cur_season);
   tolua_constant(tolua_S,"e_RankingIndex_element_war_last_season",faith::e_RankingIndex_element_war_last_season);
   tolua_constant(tolua_S,"e_RankingIndex_cross_time_limit_activity_total_recharge",faith::e_RankingIndex_cross_time_limit_activity_total_recharge);
   tolua_constant(tolua_S,"e_RankingIndex_max",faith::e_RankingIndex_max);
   tolua_constant(tolua_S,"e_mount_sub_type_none",faith::e_mount_sub_type_none);
   tolua_constant(tolua_S,"e_mount_sub_type_origin",faith::e_mount_sub_type_origin);
   tolua_constant(tolua_S,"e_mount_sub_type_illusion",faith::e_mount_sub_type_illusion);
   tolua_constant(tolua_S,"e_mount_sub_type_max",faith::e_mount_sub_type_max);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_none",faith::e_goods_purchase_limit_type_none);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_per_day",faith::e_goods_purchase_limit_type_per_day);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_forever",faith::e_goods_purchase_limit_type_forever);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_per_week",faith::e_goods_purchase_limit_type_per_week);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_max",faith::e_goods_purchase_limit_type_max);
   tolua_constant(tolua_S,"e_update_item_info_mode_default",faith::e_update_item_info_mode_default);
   tolua_constant(tolua_S,"e_update_item_info_mode_pick_up_new",faith::e_update_item_info_mode_pick_up_new);
   tolua_constant(tolua_S,"e_update_item_info_mode_update_all",faith::e_update_item_info_mode_update_all);
   tolua_constant(tolua_S,"e_update_item_info_mode_max",faith::e_update_item_info_mode_max);
   tolua_constant(tolua_S,"e_exhibition_actor_array_model",faith::e_exhibition_actor_array_model);
   tolua_constant(tolua_S,"e_exhibition_actor_array_item",faith::e_exhibition_actor_array_item);
   tolua_constant(tolua_S,"e_exhibition_actor_array_max",faith::e_exhibition_actor_array_max);
   tolua_constant(tolua_S,"e_auction_operate_type_non",faith::e_auction_operate_type_non);
   tolua_constant(tolua_S,"e_auction_operate_type_sell",faith::e_auction_operate_type_sell);
   tolua_constant(tolua_S,"e_auction_operate_type_purchase",faith::e_auction_operate_type_purchase);
   tolua_constant(tolua_S,"e_auction_operate_type_cancel_sell",faith::e_auction_operate_type_cancel_sell);
   tolua_constant(tolua_S,"e_auction_operate_type_bid",faith::e_auction_operate_type_bid);
   tolua_constant(tolua_S,"e_auction_operate_type_read_record",faith::e_auction_operate_type_read_record);
   tolua_constant(tolua_S,"e_auction_operate_type_max",faith::e_auction_operate_type_max);
   tolua_constant(tolua_S,"e_trading_type_normal_sell",faith::e_trading_type_normal_sell);
   tolua_constant(tolua_S,"e_trading_type_legion",faith::e_trading_type_legion);
   tolua_constant(tolua_S,"e_trading_type_world",faith::e_trading_type_world);
   tolua_constant(tolua_S,"e_trading_type_max",faith::e_trading_type_max);
   tolua_constant(tolua_S,"e_trading_source_type_non",faith::e_trading_source_type_non);
   tolua_constant(tolua_S,"e_trading_source_type_world_boss",faith::e_trading_source_type_world_boss);
   tolua_constant(tolua_S,"e_trading_source_type_gold_army",faith::e_trading_source_type_gold_army);
   tolua_constant(tolua_S,"e_trading_source_type_bonfire",faith::e_trading_source_type_bonfire);
   tolua_constant(tolua_S,"e_trading_source_type_legion_boss",faith::e_trading_source_type_legion_boss);
   tolua_constant(tolua_S,"e_trading_source_type_max",faith::e_trading_source_type_max);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_green",faith::e_excellentatt_quality_type_green);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_blue",faith::e_excellentatt_quality_type_blue);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_purple",faith::e_excellentatt_quality_type_purple);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_orange",faith::e_excellentatt_quality_type_orange);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_red",faith::e_excellentatt_quality_type_red);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_upper",faith::e_excellentatt_quality_type_upper);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_max",faith::e_excellentatt_quality_type_max);
   tolua_constant(tolua_S,"e_ranking_operate_result_success",faith::e_ranking_operate_result_success);
   tolua_constant(tolua_S,"e_ranking_operate_result_error",faith::e_ranking_operate_result_error);
   tolua_constant(tolua_S,"e_ranking_operate_result_no_worship_value",faith::e_ranking_operate_result_no_worship_value);
   tolua_constant(tolua_S,"e_ranking_operate_result_ranking_no_player",faith::e_ranking_operate_result_ranking_no_player);
   tolua_constant(tolua_S,"e_ranking_operate_result_max",faith::e_ranking_operate_result_max);
   tolua_constant(tolua_S,"e_camera_move_info_boom_length",faith::e_camera_move_info_boom_length);
   tolua_constant(tolua_S,"e_camera_move_info_field_of_view",faith::e_camera_move_info_field_of_view);
   tolua_constant(tolua_S,"e_camera_move_info_camera_location_x",faith::e_camera_move_info_camera_location_x);
   tolua_constant(tolua_S,"e_camera_move_info_camera_location_y",faith::e_camera_move_info_camera_location_y);
   tolua_constant(tolua_S,"e_camera_move_info_camera_location_z",faith::e_camera_move_info_camera_location_z);
   tolua_constant(tolua_S,"e_camera_move_info_camera_rotation_roll",faith::e_camera_move_info_camera_rotation_roll);
   tolua_constant(tolua_S,"e_camera_move_info_camera_rotation_pitch",faith::e_camera_move_info_camera_rotation_pitch);
   tolua_constant(tolua_S,"e_camera_move_info_camera_rotation_yaw",faith::e_camera_move_info_camera_rotation_yaw);
   tolua_constant(tolua_S,"e_camera_move_info_boom_location_x",faith::e_camera_move_info_boom_location_x);
   tolua_constant(tolua_S,"e_camera_move_info_boom_location_y",faith::e_camera_move_info_boom_location_y);
   tolua_constant(tolua_S,"e_camera_move_info_boom_location_z",faith::e_camera_move_info_boom_location_z);
   tolua_constant(tolua_S,"e_camera_move_info_boom_rotation_roll",faith::e_camera_move_info_boom_rotation_roll);
   tolua_constant(tolua_S,"e_camera_move_info_boom_rotation_pitch",faith::e_camera_move_info_boom_rotation_pitch);
   tolua_constant(tolua_S,"e_camera_move_info_boom_rotation_yaw",faith::e_camera_move_info_boom_rotation_yaw);
   tolua_constant(tolua_S,"e_camera_move_info_ani_time",faith::e_camera_move_info_ani_time);
   tolua_constant(tolua_S,"e_camera_move_info_remain_time",faith::e_camera_move_info_remain_time);
   tolua_constant(tolua_S,"e_camera_move_info_max",faith::e_camera_move_info_max);
   tolua_constant(tolua_S,"e_notice_param_type_unit_name",faith::e_notice_param_type_unit_name);
   tolua_constant(tolua_S,"e_notice_param_type_item_name",faith::e_notice_param_type_item_name);
   tolua_constant(tolua_S,"e_notice_param_type_num_data",faith::e_notice_param_type_num_data);
   tolua_constant(tolua_S,"e_notice_param_type_item_data",faith::e_notice_param_type_item_data);
   tolua_constant(tolua_S,"e_notice_param_type_describe_str",faith::e_notice_param_type_describe_str);
   tolua_constant(tolua_S,"e_notice_param_type_other_name",faith::e_notice_param_type_other_name);
   tolua_constant(tolua_S,"e_notice_param_type_max",faith::e_notice_param_type_max);
   tolua_constant(tolua_S,"e_marry_operate_type_success",faith::e_marry_operate_type_success);
   tolua_constant(tolua_S,"e_marry_operate_type_unknow_error",faith::e_marry_operate_type_unknow_error);
   tolua_constant(tolua_S,"e_marry_operate_type_team_error",faith::e_marry_operate_type_team_error);
   tolua_constant(tolua_S,"e_marry_operate_type_level_error",faith::e_marry_operate_type_level_error);
   tolua_constant(tolua_S,"e_marry_operate_type_not_friend",faith::e_marry_operate_type_not_friend);
   tolua_constant(tolua_S,"e_marry_operate_type_low_friendliness",faith::e_marry_operate_type_low_friendliness);
   tolua_constant(tolua_S,"e_marry_operate_type_one_is_cant_marry_state",faith::e_marry_operate_type_one_is_cant_marry_state);
   tolua_constant(tolua_S,"e_marry_operate_type_far_away",faith::e_marry_operate_type_far_away);
   tolua_constant(tolua_S,"e_marry_operate_type_lack_marry_item",faith::e_marry_operate_type_lack_marry_item);
   tolua_constant(tolua_S,"e_marry_operate_type_one_refuse",faith::e_marry_operate_type_one_refuse);
   tolua_constant(tolua_S,"e_marry_operate_type_one_offline",faith::e_marry_operate_type_one_offline);
   tolua_constant(tolua_S,"e_marry_operate_type_can_marry",faith::e_marry_operate_type_can_marry);
   tolua_constant(tolua_S,"e_marry_operate_type_marry_success",faith::e_marry_operate_type_marry_success);
   tolua_constant(tolua_S,"e_marry_operate_type_check_marry_item",faith::e_marry_operate_type_check_marry_item);
   tolua_constant(tolua_S,"e_marry_operate_type_mary_func_unlock",faith::e_marry_operate_type_mary_func_unlock);
   tolua_constant(tolua_S,"e_marry_operate_type_check_marry_success",faith::e_marry_operate_type_check_marry_success);
   tolua_constant(tolua_S,"e_marry_operate_type_single",faith::e_marry_operate_type_single);
   tolua_constant(tolua_S,"e_marry_operate_type_one_agree",faith::e_marry_operate_type_one_agree);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_team_error",faith::e_marry_operate_type_divorce_team_error);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_isnt_marry",faith::e_marry_operate_type_divorce_isnt_marry);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_lack_money",faith::e_marry_operate_type_divorce_lack_money);
   tolua_constant(tolua_S,"e_marry_operate_type_one_refuse_divorce",faith::e_marry_operate_type_one_refuse_divorce);
   tolua_constant(tolua_S,"e_marry_operate_type_one_agree_divorce",faith::e_marry_operate_type_one_agree_divorce);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_success",faith::e_marry_operate_type_divorce_success);
   tolua_constant(tolua_S,"e_marry_operate_type_check_divorce_success",faith::e_marry_operate_type_check_divorce_success);
   tolua_constant(tolua_S,"e_marry_operate_type_offline_over_seven",faith::e_marry_operate_type_offline_over_seven);
   tolua_constant(tolua_S,"e_marry_operate_type_check_force_divorce_success",faith::e_marry_operate_type_check_force_divorce_success);
   tolua_constant(tolua_S,"e_marry_operate_type_had_getted_reward",faith::e_marry_operate_type_had_getted_reward);
   tolua_constant(tolua_S,"e_marry_operate_type_get_reward_success",faith::e_marry_operate_type_get_reward_success);
   tolua_constant(tolua_S,"e_marry_operate_type_max",faith::e_marry_operate_type_max);
   tolua_constant(tolua_S,"e_fashion_equip_slot_non",faith::e_fashion_equip_slot_non);
   tolua_constant(tolua_S,"e_fashion_equip_slot_weapon_mesh",faith::e_fashion_equip_slot_weapon_mesh);
   tolua_constant(tolua_S,"e_fashion_equip_slot_body_mesh",faith::e_fashion_equip_slot_body_mesh);
   tolua_constant(tolua_S,"e_fashion_equip_slot_wedding_ring",faith::e_fashion_equip_slot_wedding_ring);
   tolua_constant(tolua_S,"e_fashion_equip_slot_weapon_effect",faith::e_fashion_equip_slot_weapon_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_body_effect",faith::e_fashion_equip_slot_body_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_track_effect",faith::e_fashion_equip_slot_track_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_head_effect",faith::e_fashion_equip_slot_head_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_breastplate_effect",faith::e_fashion_equip_slot_breastplate_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_glove_effect",faith::e_fashion_equip_slot_glove_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_legs_effect",faith::e_fashion_equip_slot_legs_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_feet_effect",faith::e_fashion_equip_slot_feet_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_headdress",faith::e_fashion_equip_slot_headdress);
   tolua_constant(tolua_S,"e_fashion_equip_slot_mouth",faith::e_fashion_equip_slot_mouth);
   tolua_constant(tolua_S,"e_fashion_equip_slot_eye",faith::e_fashion_equip_slot_eye);
   tolua_constant(tolua_S,"e_fashion_equip_slot_max",faith::e_fashion_equip_slot_max);
   tolua_constant(tolua_S,"e_fashion_show_type_all",faith::e_fashion_show_type_all);
   tolua_constant(tolua_S,"e_fashion_show_type_weapon",faith::e_fashion_show_type_weapon);
   tolua_constant(tolua_S,"e_fashion_show_type_cloth",faith::e_fashion_show_type_cloth);
   tolua_constant(tolua_S,"e_fashion_show_type_weapon_effect",faith::e_fashion_show_type_weapon_effect);
   tolua_constant(tolua_S,"e_fashion_show_type_cloth_effect",faith::e_fashion_show_type_cloth_effect);
   tolua_constant(tolua_S,"e_fashion_show_type_foot_effect",faith::e_fashion_show_type_foot_effect);
   tolua_constant(tolua_S,"e_fashion_show_type_head_frame",faith::e_fashion_show_type_head_frame);
   tolua_constant(tolua_S,"e_fashion_show_type_headdress",faith::e_fashion_show_type_headdress);
   tolua_constant(tolua_S,"e_fashion_show_type_mouth",faith::e_fashion_show_type_mouth);
   tolua_constant(tolua_S,"e_fashion_show_type_eye",faith::e_fashion_show_type_eye);
   tolua_constant(tolua_S,"e_fashion_show_type_max",faith::e_fashion_show_type_max);
   tolua_constant(tolua_S,"e_interaction_reply_none",faith::e_interaction_reply_none);
   tolua_constant(tolua_S,"e_interaction_reply_reject",faith::e_interaction_reply_reject);
   tolua_constant(tolua_S,"e_interaction_reply_agree",faith::e_interaction_reply_agree);
   tolua_constant(tolua_S,"e_interaction_reply_timeout",faith::e_interaction_reply_timeout);
   tolua_constant(tolua_S,"e_interaction_reply_be_invited",faith::e_interaction_reply_be_invited);
   tolua_constant(tolua_S,"e_interaction_reply_is_interacting",faith::e_interaction_reply_is_interacting);
   tolua_constant(tolua_S,"e_interaction_reply_max",faith::e_interaction_reply_max);
   tolua_constant(tolua_S,"e_interaction_type_none",faith::e_interaction_type_none);
   tolua_constant(tolua_S,"e_interaction_type_interrupted",faith::e_interaction_type_interrupted);
   tolua_constant(tolua_S,"e_interaction_type_doubleride_front",faith::e_interaction_type_doubleride_front);
   tolua_constant(tolua_S,"e_interaction_type_doubleride_back",faith::e_interaction_type_doubleride_back);
   tolua_constant(tolua_S,"e_interaction_type_bao_da_tui",faith::e_interaction_type_bao_da_tui);
   tolua_constant(tolua_S,"e_interaction_type_bei_bao_da_tui",faith::e_interaction_type_bei_bao_da_tui);
   tolua_constant(tolua_S,"e_interaction_type_gong_zhu_bao",faith::e_interaction_type_gong_zhu_bao);
   tolua_constant(tolua_S,"e_interaction_type_bei_gong_zhu_bao",faith::e_interaction_type_bei_gong_zhu_bao);
   tolua_constant(tolua_S,"e_interaction_type_qiu_hun",faith::e_interaction_type_qiu_hun);
   tolua_constant(tolua_S,"e_interaction_type_bei_qiu_hun",faith::e_interaction_type_bei_qiu_hun);
   tolua_constant(tolua_S,"e_interaction_type_tiao_wu",faith::e_interaction_type_tiao_wu);
   tolua_constant(tolua_S,"e_interaction_type_bei_tiao_wu",faith::e_interaction_type_bei_tiao_wu);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_back_target_man",faith::e_interaction_type_marry_raid_back_target_man);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_back_target_woman",faith::e_interaction_type_marry_raid_back_target_woman);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_front_target_man",faith::e_interaction_type_marry_raid_front_target_man);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_front_target_woman",faith::e_interaction_type_marry_raid_front_target_woman);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid",faith::e_interaction_type_marry_raid);
   tolua_constant(tolua_S,"e_interaction_type_yao_legion_dance",faith::e_interaction_type_yao_legion_dance);
   tolua_constant(tolua_S,"e_interaction_type_bei_legion_dance",faith::e_interaction_type_bei_legion_dance);
   tolua_constant(tolua_S,"e_interaction_type_marry_dance_accord",faith::e_interaction_type_marry_dance_accord);
   tolua_constant(tolua_S,"e_interaction_type_marry_dance_passive",faith::e_interaction_type_marry_dance_passive);
   tolua_constant(tolua_S,"e_interaction_type_max",faith::e_interaction_type_max);
   tolua_constant(tolua_S,"e_belief_upgrade_type_none",faith::e_belief_upgrade_type_none);
   tolua_constant(tolua_S,"e_belief_upgrade_type_upgrade_star",faith::e_belief_upgrade_type_upgrade_star);
   tolua_constant(tolua_S,"e_belief_upgrade_type_upgrade_rank",faith::e_belief_upgrade_type_upgrade_rank);
   tolua_constant(tolua_S,"e_belief_upgrade_type_max",faith::e_belief_upgrade_type_max);
   tolua_constant(tolua_S,"e_belief_operate_type_upgrade_star",faith::e_belief_operate_type_upgrade_star);
   tolua_constant(tolua_S,"e_belief_operate_type_upgrade_rank",faith::e_belief_operate_type_upgrade_rank);
   tolua_constant(tolua_S,"e_belief_operate_type_max",faith::e_belief_operate_type_max);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_self",faith::e_exhibition_show_type_player_self);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_other",faith::e_exhibition_show_type_player_other);
   tolua_constant(tolua_S,"e_exhibition_show_type_npc",faith::e_exhibition_show_type_npc);
   tolua_constant(tolua_S,"e_exhibition_show_type_other_slot",faith::e_exhibition_show_type_other_slot);
   tolua_constant(tolua_S,"e_exhibition_show_type_spirit",faith::e_exhibition_show_type_spirit);
   tolua_constant(tolua_S,"e_exhibition_show_type_protect_spirit",faith::e_exhibition_show_type_protect_spirit);
   tolua_constant(tolua_S,"e_exhibition_show_type_self_by_info",faith::e_exhibition_show_type_self_by_info);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_self_simple",faith::e_exhibition_show_type_player_self_simple);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_other_on_mount",faith::e_exhibition_show_type_player_other_on_mount);
   tolua_constant(tolua_S,"e_exhibition_show_type_try_on_fashion",faith::e_exhibition_show_type_try_on_fashion);
   tolua_constant(tolua_S,"e_exhibition_show_type_spirit_composit",faith::e_exhibition_show_type_spirit_composit);
   tolua_constant(tolua_S,"e_exhibition_show_type_max",faith::e_exhibition_show_type_max);
   tolua_constant(tolua_S,"e_pos_type_pos_x",faith::e_pos_type_pos_x);
   tolua_constant(tolua_S,"e_pos_type_pos_y",faith::e_pos_type_pos_y);
   tolua_constant(tolua_S,"e_pos_type_pos_z",faith::e_pos_type_pos_z);
   tolua_constant(tolua_S,"e_pos_type_max",faith::e_pos_type_max);
   tolua_constant(tolua_S,"e_rotation_type_pitch",faith::e_rotation_type_pitch);
   tolua_constant(tolua_S,"e_rotation_type_yaw",faith::e_rotation_type_yaw);
   tolua_constant(tolua_S,"e_rotation_type_roll",faith::e_rotation_type_roll);
   tolua_constant(tolua_S,"e_rotation_type_max",faith::e_rotation_type_max);
   tolua_constant(tolua_S,"e_week_day_monday",faith::e_week_day_monday);
   tolua_constant(tolua_S,"e_week_day_tuesday",faith::e_week_day_tuesday);
   tolua_constant(tolua_S,"e_week_day_wednesday",faith::e_week_day_wednesday);
   tolua_constant(tolua_S,"e_week_day_thursday",faith::e_week_day_thursday);
   tolua_constant(tolua_S,"e_week_day_friday",faith::e_week_day_friday);
   tolua_constant(tolua_S,"e_week_day_saturday",faith::e_week_day_saturday);
   tolua_constant(tolua_S,"e_week_day_sunday",faith::e_week_day_sunday);
   tolua_constant(tolua_S,"e_week_day_max",faith::e_week_day_max);
   tolua_constant(tolua_S,"e_month_january",faith::e_month_january);
   tolua_constant(tolua_S,"e_month_february",faith::e_month_february);
   tolua_constant(tolua_S,"e_month_march",faith::e_month_march);
   tolua_constant(tolua_S,"e_month_april",faith::e_month_april);
   tolua_constant(tolua_S,"e_month_may",faith::e_month_may);
   tolua_constant(tolua_S,"e_month_june",faith::e_month_june);
   tolua_constant(tolua_S,"e_month_july",faith::e_month_july);
   tolua_constant(tolua_S,"e_month_august",faith::e_month_august);
   tolua_constant(tolua_S,"e_month_september",faith::e_month_september);
   tolua_constant(tolua_S,"e_month_october",faith::e_month_october);
   tolua_constant(tolua_S,"e_month_november",faith::e_month_november);
   tolua_constant(tolua_S,"e_month_december",faith::e_month_december);
   tolua_constant(tolua_S,"e_month_max",faith::e_month_max);
   tolua_constant(tolua_S,"e_item_get_info_item_template_id",faith::e_item_get_info_item_template_id);
   tolua_constant(tolua_S,"e_item_get_info_item_num",faith::e_item_get_info_item_num);
   tolua_constant(tolua_S,"e_item_get_info_locked_state",faith::e_item_get_info_locked_state);
   tolua_constant(tolua_S,"e_item_get_info_max",faith::e_item_get_info_max);
   tolua_constant(tolua_S,"e_addition_buff_upgrade",faith::e_addition_buff_upgrade);
   tolua_constant(tolua_S,"e_addition_buff_quality",faith::e_addition_buff_quality);
   tolua_constant(tolua_S,"e_addition_buff_achievement",faith::e_addition_buff_achievement);
   tolua_constant(tolua_S,"e_addition_buff_rank",faith::e_addition_buff_rank);
   tolua_constant(tolua_S,"e_addition_buff_feather",faith::e_addition_buff_feather);
   tolua_constant(tolua_S,"e_addition_buff_addon",faith::e_addition_buff_addon);
   tolua_constant(tolua_S,"e_addition_buff_succinct",faith::e_addition_buff_succinct);
   tolua_constant(tolua_S,"e_addition_buff_fahion_sum_level",faith::e_addition_buff_fahion_sum_level);
   tolua_constant(tolua_S,"e_addition_buff_sky_equip",faith::e_addition_buff_sky_equip);
   tolua_constant(tolua_S,"e_addition_buff_enchant",faith::e_addition_buff_enchant);
   tolua_constant(tolua_S,"e_addition_buff_goddess",faith::e_addition_buff_goddess);
   tolua_constant(tolua_S,"e_addition_buff_couple_heart_value",faith::e_addition_buff_couple_heart_value);
   tolua_constant(tolua_S,"e_addition_buff_skygod_equip",faith::e_addition_buff_skygod_equip);
   tolua_constant(tolua_S,"e_addition_buff_supreme_equip",faith::e_addition_buff_supreme_equip);
   tolua_constant(tolua_S,"e_addition_buff_max",faith::e_addition_buff_max);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_non",faith::e_item_upgrade_protect_type_non);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_low_level_undown",faith::e_item_upgrade_protect_type_low_level_undown);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_high_level_undown",faith::e_item_upgrade_protect_type_high_level_undown);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_add_success_percentage",faith::e_item_upgrade_protect_type_add_success_percentage);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_max",faith::e_item_upgrade_protect_type_max);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_none",faith::e_item_succinct_buff_level_none);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_F",faith::e_item_succinct_buff_level_F);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_E",faith::e_item_succinct_buff_level_E);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_D",faith::e_item_succinct_buff_level_D);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_C",faith::e_item_succinct_buff_level_C);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_B",faith::e_item_succinct_buff_level_B);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_A",faith::e_item_succinct_buff_level_A);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_S",faith::e_item_succinct_buff_level_S);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_SS",faith::e_item_succinct_buff_level_SS);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_SSS",faith::e_item_succinct_buff_level_SSS);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_max",faith::e_item_succinct_buff_level_max);
   tolua_constant(tolua_S,"e_this_launch_already_do_play_cg",faith::e_this_launch_already_do_play_cg);
   tolua_constant(tolua_S,"e_this_launch_already_do_show_notice",faith::e_this_launch_already_do_show_notice);
   tolua_constant(tolua_S,"e_this_launch_already_do_sdk_game_start",faith::e_this_launch_already_do_sdk_game_start);
   tolua_constant(tolua_S,"e_this_launch_already_do_max",faith::e_this_launch_already_do_max);
   tolua_constant(tolua_S,"e_tinder_info_current_level_template_id",faith::e_tinder_info_current_level_template_id);
   tolua_constant(tolua_S,"e_tinder_info_current_unlock_template_id",faith::e_tinder_info_current_unlock_template_id);
   tolua_constant(tolua_S,"e_tinder_info_already_play_unlock_animation_id",faith::e_tinder_info_already_play_unlock_animation_id);
   tolua_constant(tolua_S,"e_tinder_info_total_star_level",faith::e_tinder_info_total_star_level);
   tolua_constant(tolua_S,"e_tinder_info_add_star_level",faith::e_tinder_info_add_star_level);
   tolua_constant(tolua_S,"e_tinder_info_max",faith::e_tinder_info_max);
   tolua_constant(tolua_S,"e_tinder_operation_type_levelup_tinder",faith::e_tinder_operation_type_levelup_tinder);
   tolua_constant(tolua_S,"e_tinder_operation_type_unlock_tinder",faith::e_tinder_operation_type_unlock_tinder);
   tolua_constant(tolua_S,"e_tinder_operation_type_update_unlock_ani",faith::e_tinder_operation_type_update_unlock_ani);
   tolua_constant(tolua_S,"e_tinder_operation_type_unlock_active_skill",faith::e_tinder_operation_type_unlock_active_skill);
   tolua_constant(tolua_S,"e_tinder_operation_type_max",faith::e_tinder_operation_type_max);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_create",faith::e_get_aoi_info_all_type_create);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_simple_to_normal",faith::e_get_aoi_info_all_type_simple_to_normal);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_normal_to_simple",faith::e_get_aoi_info_all_type_normal_to_simple);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_max",faith::e_get_aoi_info_all_type_max);
   tolua_constant(tolua_S,"e_service_rank_type_level",faith::e_service_rank_type_level);
   tolua_constant(tolua_S,"e_service_rank_type_mount",faith::e_service_rank_type_mount);
   tolua_constant(tolua_S,"e_service_rank_type_spirit",faith::e_service_rank_type_spirit);
   tolua_constant(tolua_S,"e_service_rank_type_element",faith::e_service_rank_type_element);
   tolua_constant(tolua_S,"e_service_rank_type_wing",faith::e_service_rank_type_wing);
   tolua_constant(tolua_S,"e_service_rank_type_gs",faith::e_service_rank_type_gs);
   tolua_constant(tolua_S,"e_service_rank_type_max",faith::e_service_rank_type_max);
   tolua_constant(tolua_S,"e_mission_type_main",faith::e_mission_type_main);
   tolua_constant(tolua_S,"e_mission_type_daily",faith::e_mission_type_daily);
   tolua_constant(tolua_S,"e_mission_type_against",faith::e_mission_type_against);
   tolua_constant(tolua_S,"e_mission_type_side",faith::e_mission_type_side);
   tolua_constant(tolua_S,"e_mission_type_grade_up",faith::e_mission_type_grade_up);
   tolua_constant(tolua_S,"e_mission_type_marry",faith::e_mission_type_marry);
   tolua_constant(tolua_S,"e_mission_type_max",faith::e_mission_type_max);
   tolua_constant(tolua_S,"e_mission_slot_main",faith::e_mission_slot_main);
   tolua_constant(tolua_S,"e_mission_slot_daily",faith::e_mission_slot_daily);
   tolua_constant(tolua_S,"e_mission_slot_against",faith::e_mission_slot_against);
   tolua_constant(tolua_S,"e_mission_slot_grade_up",faith::e_mission_slot_grade_up);
   tolua_constant(tolua_S,"e_mission_slot_side_0",faith::e_mission_slot_side_0);
   tolua_constant(tolua_S,"e_mission_slot_side_1",faith::e_mission_slot_side_1);
   tolua_constant(tolua_S,"e_mission_slot_side_2",faith::e_mission_slot_side_2);
   tolua_constant(tolua_S,"e_mission_slot_side_3",faith::e_mission_slot_side_3);
   tolua_constant(tolua_S,"e_mission_slot_side_4",faith::e_mission_slot_side_4);
   tolua_constant(tolua_S,"e_mission_slot_side_5",faith::e_mission_slot_side_5);
   tolua_constant(tolua_S,"e_mission_slot_side_6",faith::e_mission_slot_side_6);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_0",faith::e_mission_slot_extra_side_0);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_1",faith::e_mission_slot_extra_side_1);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_2",faith::e_mission_slot_extra_side_2);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_3",faith::e_mission_slot_extra_side_3);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_4",faith::e_mission_slot_extra_side_4);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_5",faith::e_mission_slot_extra_side_5);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_6",faith::e_mission_slot_extra_side_6);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_7",faith::e_mission_slot_extra_side_7);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_8",faith::e_mission_slot_extra_side_8);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_9",faith::e_mission_slot_extra_side_9);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_10",faith::e_mission_slot_extra_side_10);
   tolua_constant(tolua_S,"e_mission_slot_max",faith::e_mission_slot_max);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_no_qualification",faith::e_service_rank_reward_state_type_no_qualification);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_have_qualification",faith::e_service_rank_reward_state_type_have_qualification);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_already_receive",faith::e_service_rank_reward_state_type_already_receive);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_max",faith::e_service_rank_reward_state_type_max);
   tolua_constant(tolua_S,"e_service_rank_reward_type_first",faith::e_service_rank_reward_type_first);
   tolua_constant(tolua_S,"e_service_rank_reward_type_second",faith::e_service_rank_reward_type_second);
   tolua_constant(tolua_S,"e_service_rank_reward_type_third",faith::e_service_rank_reward_type_third);
   tolua_constant(tolua_S,"e_service_rank_reward_type_fourth",faith::e_service_rank_reward_type_fourth);
   tolua_constant(tolua_S,"e_service_rank_reward_type_fifth",faith::e_service_rank_reward_type_fifth);
   tolua_constant(tolua_S,"e_service_rank_reward_type_max",faith::e_service_rank_reward_type_max);
   tolua_constant(tolua_S,"e_beast_spirit_type_mount",faith::e_beast_spirit_type_mount);
   tolua_constant(tolua_S,"e_beast_spirit_type_wing",faith::e_beast_spirit_type_wing);
   tolua_constant(tolua_S,"e_beast_spirit_type_spirit",faith::e_beast_spirit_type_spirit);
   tolua_constant(tolua_S,"e_beast_spirit_type_max",faith::e_beast_spirit_type_max);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_killed_field_boss",faith::e_gain_treasure_info_type_killed_field_boss);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_killed_simple_boss",faith::e_gain_treasure_info_type_killed_simple_boss);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_buy_simple_boss",faith::e_gain_treasure_info_type_buy_simple_boss);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_max",faith::e_gain_treasure_info_type_max);
   tolua_constant(tolua_S,"e_jewel_type_non",faith::e_jewel_type_non);
   tolua_constant(tolua_S,"e_jewel_type_attack",faith::e_jewel_type_attack);
   tolua_constant(tolua_S,"e_jewel_type_defense",faith::e_jewel_type_defense);
   tolua_constant(tolua_S,"e_jewel_type_max",faith::e_jewel_type_max);
   tolua_constant(tolua_S,"e_drop_record_model_type_no",faith::e_drop_record_model_type_no);
   tolua_constant(tolua_S,"e_drop_record_model_type_record",faith::e_drop_record_model_type_record);
   tolua_constant(tolua_S,"e_drop_record_model_type_set_top",faith::e_drop_record_model_type_set_top);
   tolua_constant(tolua_S,"e_drop_record_model_type_max",faith::e_drop_record_model_type_max);
   tolua_constant(tolua_S,"e_treasure_record_time",faith::e_treasure_record_time);
   tolua_constant(tolua_S,"e_treasure_record_treasure_type",faith::e_treasure_record_treasure_type);
   tolua_constant(tolua_S,"e_treasure_record_prize_type",faith::e_treasure_record_prize_type);
   tolua_constant(tolua_S,"e_treasure_record_prize_0",faith::e_treasure_record_prize_0);
   tolua_constant(tolua_S,"e_treasure_record_prize_1",faith::e_treasure_record_prize_1);
   tolua_constant(tolua_S,"e_treasure_record_prize_2",faith::e_treasure_record_prize_2);
   tolua_constant(tolua_S,"e_treasure_record_prize_3",faith::e_treasure_record_prize_3);
   tolua_constant(tolua_S,"e_treasure_record_prize_4",faith::e_treasure_record_prize_4);
   tolua_constant(tolua_S,"e_treasure_record_prize_5",faith::e_treasure_record_prize_5);
   tolua_constant(tolua_S,"e_treasure_record_prize_6",faith::e_treasure_record_prize_6);
   tolua_constant(tolua_S,"e_treasure_record_prize_7",faith::e_treasure_record_prize_7);
   tolua_constant(tolua_S,"e_treasure_record_prize_8",faith::e_treasure_record_prize_8);
   tolua_constant(tolua_S,"e_treasure_record_prize_9",faith::e_treasure_record_prize_9);
   tolua_constant(tolua_S,"e_treasure_record_max",faith::e_treasure_record_max);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_charge",faith::e_time_limit_activity_type_total_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_cost",faith::e_time_limit_activity_type_total_cost);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge",faith::e_time_limit_activity_type_daily_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure",faith::e_time_limit_activity_type_treasure);
   tolua_constant(tolua_S,"e_time_limit_activity_type_shop",faith::e_time_limit_activity_type_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare",faith::e_time_limit_activity_type_time_welfare);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare",faith::e_time_limit_activity_type_behavior_welfare);
   tolua_constant(tolua_S,"e_time_limit_activity_type_word_desc",faith::e_time_limit_activity_type_word_desc);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare2",faith::e_time_limit_activity_type_time_welfare2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare3",faith::e_time_limit_activity_type_time_welfare3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare4",faith::e_time_limit_activity_type_time_welfare4);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_sprite",faith::e_time_limit_activity_type_treasure_sprite);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_exchange",faith::e_time_limit_activity_type_item_exchange);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare5",faith::e_time_limit_activity_type_time_welfare5);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge2",faith::e_time_limit_activity_type_daily_charge2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_charge2",faith::e_time_limit_activity_type_total_charge2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward",faith::e_time_limit_activity_type_behavior_welfare_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop",faith::e_time_limit_activity_type_discount_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_small_charge",faith::e_time_limit_activity_type_small_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_landing",faith::e_time_limit_activity_type_total_landing);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_cost_jewel",faith::e_time_limit_activity_type_total_cost_jewel);
   tolua_constant(tolua_S,"e_time_limit_activity_type_landing_reward",faith::e_time_limit_activity_type_landing_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_added_drop",faith::e_time_limit_activity_type_added_drop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_add_upgrade_level",faith::e_time_limit_activity_type_add_upgrade_level);
   tolua_constant(tolua_S,"e_time_limit_activity_type_activity_notice_1",faith::e_time_limit_activity_type_activity_notice_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_activity_notice_2",faith::e_time_limit_activity_type_activity_notice_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_1",faith::e_time_limit_activity_type_national_treasure_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_2",faith::e_time_limit_activity_type_national_treasure_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_gs_up",faith::e_time_limit_activity_type_gs_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_mount_up",faith::e_time_limit_activity_type_mount_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_wing_up",faith::e_time_limit_activity_type_wing_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_equip_up",faith::e_time_limit_activity_type_equip_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_spirit_up",faith::e_time_limit_activity_type_spirit_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_gs_up",faith::e_time_limit_activity_type_cross_gs_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_mount_up",faith::e_time_limit_activity_type_cross_mount_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_wing_up",faith::e_time_limit_activity_type_cross_wing_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_equip_up",faith::e_time_limit_activity_type_cross_equip_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_spirit_up",faith::e_time_limit_activity_type_cross_spirit_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_element",faith::e_time_limit_activity_type_treasure_element);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cloud_shop",faith::e_time_limit_activity_type_cloud_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_cloud_shop",faith::e_time_limit_activity_type_cross_cloud_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_gift_charge",faith::e_time_limit_activity_type_daily_gift_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop",faith::e_time_limit_activity_type_item_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_dragontrip",faith::e_time_limit_activity_type_dragontrip);
   tolua_constant(tolua_S,"e_time_limit_activity_type_special_exchange",faith::e_time_limit_activity_type_special_exchange);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_landing",faith::e_time_limit_activity_type_new_total_landing);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_behavior_welfare",faith::e_time_limit_activity_type_new_behavior_welfare);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_behavior_welfare_reward",faith::e_time_limit_activity_type_new_behavior_welfare_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_sky_treasure",faith::e_time_limit_activity_type_sky_treasure);
   tolua_constant(tolua_S,"e_time_limit_activity_type_starark",faith::e_time_limit_activity_type_starark);
   tolua_constant(tolua_S,"e_time_limit_activity_type_support_for_promotion",faith::e_time_limit_activity_type_support_for_promotion);
   tolua_constant(tolua_S,"e_time_limit_activity_type_weekend_top_up",faith::e_time_limit_activity_type_weekend_top_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare1",faith::e_time_limit_activity_type_behavior_welfare1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare2",faith::e_time_limit_activity_type_behavior_welfare2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare3",faith::e_time_limit_activity_type_behavior_welfare3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_royal1",faith::e_time_limit_activity_type_treasure_royal1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_royal2",faith::e_time_limit_activity_type_treasure_royal2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_royal3",faith::e_time_limit_activity_type_treasure_royal3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge3",faith::e_time_limit_activity_type_daily_charge3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge4",faith::e_time_limit_activity_type_daily_charge4);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge5",faith::e_time_limit_activity_type_daily_charge5);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge6",faith::e_time_limit_activity_type_daily_charge6);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop1",faith::e_time_limit_activity_type_discount_shop1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop2",faith::e_time_limit_activity_type_discount_shop2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop3",faith::e_time_limit_activity_type_discount_shop3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward1",faith::e_time_limit_activity_type_behavior_welfare_reward1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward2",faith::e_time_limit_activity_type_behavior_welfare_reward2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward3",faith::e_time_limit_activity_type_behavior_welfare_reward3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop1",faith::e_time_limit_activity_type_item_shop1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop2",faith::e_time_limit_activity_type_item_shop2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop3",faith::e_time_limit_activity_type_item_shop3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_lucky_card",faith::e_time_limit_activity_type_lucky_card);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_total_charge",faith::e_time_limit_activity_type_cross_total_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_charge_1",faith::e_time_limit_activity_type_new_total_charge_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_charge_2",faith::e_time_limit_activity_type_new_total_charge_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_charge_3",faith::e_time_limit_activity_type_new_total_charge_3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_cost_jewel_1",faith::e_time_limit_activity_type_new_total_cost_jewel_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_cost_jewel_2",faith::e_time_limit_activity_type_new_total_cost_jewel_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_cost_jewel_3",faith::e_time_limit_activity_type_new_total_cost_jewel_3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_server_treasure",faith::e_time_limit_activity_type_new_server_treasure);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_server_mail",faith::e_time_limit_activity_type_new_server_mail);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_3",faith::e_time_limit_activity_type_national_treasure_3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_4",faith::e_time_limit_activity_type_national_treasure_4);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_free_reward",faith::e_time_limit_activity_type_daily_free_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_feed_back1",faith::e_time_limit_activity_type_time_feed_back1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_feed_back2",faith::e_time_limit_activity_type_time_feed_back2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_feed_back3",faith::e_time_limit_activity_type_time_feed_back3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_limit_time_treasure2",faith::e_time_limit_activity_type_limit_time_treasure2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_limit_time_notice",faith::e_time_limit_activity_type_limit_time_notice);
   tolua_constant(tolua_S,"e_time_limit_activity_type_diamond_shop",faith::e_time_limit_activity_type_diamond_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_star_trip",faith::e_time_limit_activity_type_star_trip);
   tolua_constant(tolua_S,"e_time_limit_activity_type_max",faith::e_time_limit_activity_type_max);
   tolua_constant(tolua_S,"e_mail_type_normal_mail",faith::e_mail_type_normal_mail);
   tolua_constant(tolua_S,"e_mail_type_server_merge_to_normal_player",faith::e_mail_type_server_merge_to_normal_player);
   tolua_constant(tolua_S,"e_mail_type_server_merge_to_legion_del_player",faith::e_mail_type_server_merge_to_legion_del_player);
   tolua_constant(tolua_S,"e_mail_type_server_merge_to_be_del_player",faith::e_mail_type_server_merge_to_be_del_player);
   tolua_constant(tolua_S,"e_mail_type_max",faith::e_mail_type_max);
   tolua_constant(tolua_S,"e_time_limit_activity_info_type",faith::e_time_limit_activity_info_type);
   tolua_constant(tolua_S,"e_time_limit_activity_info_start_time",faith::e_time_limit_activity_info_start_time);
   tolua_constant(tolua_S,"e_time_limit_activity_info_end_time",faith::e_time_limit_activity_info_end_time);
   tolua_constant(tolua_S,"e_time_limit_activity_info_guid",faith::e_time_limit_activity_info_guid);
   tolua_constant(tolua_S,"e_time_limit_activity_info_first_branch_guid",faith::e_time_limit_activity_info_first_branch_guid);
   tolua_constant(tolua_S,"e_time_limit_activity_info_max",faith::e_time_limit_activity_info_max);
   tolua_constant(tolua_S,"e_unit_safe_map_info_pos_x",faith::e_unit_safe_map_info_pos_x);
   tolua_constant(tolua_S,"e_unit_safe_map_info_pos_y",faith::e_unit_safe_map_info_pos_y);
   tolua_constant(tolua_S,"e_unit_safe_map_info_map_tempate_id",faith::e_unit_safe_map_info_map_tempate_id);
   tolua_constant(tolua_S,"e_unit_safe_map_info_map_have_not_safe_region",faith::e_unit_safe_map_info_map_have_not_safe_region);
   tolua_constant(tolua_S,"e_unit_safe_map_info_safe_map_template_id",faith::e_unit_safe_map_info_safe_map_template_id);
   tolua_constant(tolua_S,"e_unit_safe_map_info_safe_missiondone_id",faith::e_unit_safe_map_info_safe_missiondone_id);
   tolua_constant(tolua_S,"e_unit_safe_map_info_safe_map_type",faith::e_unit_safe_map_info_safe_map_type);
   tolua_constant(tolua_S,"e_unit_safe_map_info_max",faith::e_unit_safe_map_info_max);
   tolua_constant(tolua_S,"e_island_record_killed_island_boss",faith::e_island_record_killed_island_boss);
   tolua_constant(tolua_S,"e_island_record_island_collect_count",faith::e_island_record_island_collect_count);
   tolua_constant(tolua_S,"e_island_record_cross_pk_collect_count",faith::e_island_record_cross_pk_collect_count);
   tolua_constant(tolua_S,"e_island_record_oracle_trial_count",faith::e_island_record_oracle_trial_count);
   tolua_constant(tolua_S,"e_island_record_max",faith::e_island_record_max);
   tolua_constant(tolua_S,"e_login_type_new_account_and_password",faith::e_login_type_new_account_and_password);
   tolua_constant(tolua_S,"e_login_type_new_robot",faith::e_login_type_new_robot);
   tolua_constant(tolua_S,"e_login_type_new_token",faith::e_login_type_new_token);
   tolua_constant(tolua_S,"e_login_type_new_zw",faith::e_login_type_new_zw);
   tolua_constant(tolua_S,"e_login_type_new_facebook",faith::e_login_type_new_facebook);
   tolua_constant(tolua_S,"e_login_type_new_google",faith::e_login_type_new_google);
   tolua_constant(tolua_S,"e_login_type_new_gate_server",faith::e_login_type_new_gate_server);
   tolua_constant(tolua_S,"e_login_type_new_tw",faith::e_login_type_new_tw);
   tolua_constant(tolua_S,"e_login_type_new_korea",faith::e_login_type_new_korea);
   tolua_constant(tolua_S,"e_login_type_new_en",faith::e_login_type_new_en);
   tolua_constant(tolua_S,"e_login_type_new_umisdk",faith::e_login_type_new_umisdk);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_vn",faith::e_login_type_new_umisdk_vn);
   tolua_constant(tolua_S,"e_login_type_new_appstore",faith::e_login_type_new_appstore);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_tw",faith::e_login_type_new_umisdk_tw);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_jp",faith::e_login_type_new_umisdk_jp);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_rus",faith::e_login_type_new_umisdk_rus);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_sg",faith::e_login_type_new_umisdk_sg);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_new_tw",faith::e_login_type_new_umisdk_new_tw);
   tolua_constant(tolua_S,"e_login_type_new_max",faith::e_login_type_new_max);
   tolua_constant(tolua_S,"e_payment_method_test",faith::e_payment_method_test);
   tolua_constant(tolua_S,"e_payment_method_zw",faith::e_payment_method_zw);
   tolua_constant(tolua_S,"e_payment_method_google",faith::e_payment_method_google);
   tolua_constant(tolua_S,"e_payment_method_one_store",faith::e_payment_method_one_store);
   tolua_constant(tolua_S,"e_payment_method_app_store",faith::e_payment_method_app_store);
   tolua_constant(tolua_S,"e_payment_method_tw_android",faith::e_payment_method_tw_android);
   tolua_constant(tolua_S,"e_payment_method_tw_ios",faith::e_payment_method_tw_ios);
   tolua_constant(tolua_S,"e_payment_method_tw_third",faith::e_payment_method_tw_third);
   tolua_constant(tolua_S,"e_payment_method_samsung",faith::e_payment_method_samsung);
   tolua_constant(tolua_S,"e_payment_method_en_ios",faith::e_payment_method_en_ios);
   tolua_constant(tolua_S,"e_payment_method_en_android",faith::e_payment_method_en_android);
   tolua_constant(tolua_S,"e_payment_method_kr_gameloop",faith::e_payment_method_kr_gameloop);
   tolua_constant(tolua_S,"e_payment_method_tl_umisdk_google",faith::e_payment_method_tl_umisdk_google);
   tolua_constant(tolua_S,"e_payment_method_tl_umisdk_ios",faith::e_payment_method_tl_umisdk_ios);
   tolua_constant(tolua_S,"e_payment_method_vn_umisdk_google",faith::e_payment_method_vn_umisdk_google);
   tolua_constant(tolua_S,"e_payment_method_vn_umisdk_ios",faith::e_payment_method_vn_umisdk_ios);
   tolua_constant(tolua_S,"e_payment_method_tw_android_new",faith::e_payment_method_tw_android_new);
   tolua_constant(tolua_S,"e_payment_method_tw_ios_new",faith::e_payment_method_tw_ios_new);
   tolua_constant(tolua_S,"e_payment_method_tw_third_new",faith::e_payment_method_tw_third_new);
   tolua_constant(tolua_S,"e_payment_method_tw_umisdk_android",faith::e_payment_method_tw_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_tw_umisdk_ios",faith::e_payment_method_tw_umisdk_ios);
   tolua_constant(tolua_S,"e_payment_method_tw_umisdk_third",faith::e_payment_method_tw_umisdk_third);
   tolua_constant(tolua_S,"e_payment_method_jp_umisdk_android",faith::e_payment_method_jp_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_jp_uminsdk_ios",faith::e_payment_method_jp_uminsdk_ios);
   tolua_constant(tolua_S,"e_payment_method_rus_umisdk_android",faith::e_payment_method_rus_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_rus_uminsdk_ios",faith::e_payment_method_rus_uminsdk_ios);
   tolua_constant(tolua_S,"e_payment_method_sg_umisdk_android",faith::e_payment_method_sg_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_sg_uminsdk_ios",faith::e_payment_method_sg_uminsdk_ios);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_android",faith::e_payment_method_umisdk_tw_android);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_ios",faith::e_payment_method_umisdk_tw_ios);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_third",faith::e_payment_method_umisdk_tw_third);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_hw",faith::e_payment_method_umisdk_tw_hw);
   tolua_constant(tolua_S,"e_payment_method_max",faith::e_payment_method_max);
   tolua_constant(tolua_S,"e_treasure_prize_type_one",faith::e_treasure_prize_type_one);
   tolua_constant(tolua_S,"e_treasure_prize_type_ten",faith::e_treasure_prize_type_ten);
   tolua_constant(tolua_S,"e_treasure_prize_type_one_item",faith::e_treasure_prize_type_one_item);
   tolua_constant(tolua_S,"e_treasure_prize_type_ten_item",faith::e_treasure_prize_type_ten_item);
   tolua_constant(tolua_S,"e_treasure_prize_type_max",faith::e_treasure_prize_type_max);
   tolua_constant(tolua_S,"e_treasure_type_item",faith::e_treasure_type_item);
   tolua_constant(tolua_S,"e_treasure_type_element",faith::e_treasure_type_element);
   tolua_constant(tolua_S,"e_treasure_type_spirit",faith::e_treasure_type_spirit);
   tolua_constant(tolua_S,"e_treasure_type_new_server",faith::e_treasure_type_new_server);
   tolua_constant(tolua_S,"e_treasure_type_time_limit",faith::e_treasure_type_time_limit);
   tolua_constant(tolua_S,"e_treasure_type_max",faith::e_treasure_type_max);
   tolua_constant(tolua_S,"e_ranking_show_type_non",faith::e_ranking_show_type_non);
   tolua_constant(tolua_S,"e_ranking_show_type_gs",faith::e_ranking_show_type_gs);
   tolua_constant(tolua_S,"e_ranking_show_type_exp_level",faith::e_ranking_show_type_exp_level);
   tolua_constant(tolua_S,"e_ranking_show_type_equip",faith::e_ranking_show_type_equip);
   tolua_constant(tolua_S,"e_ranking_show_type_mount",faith::e_ranking_show_type_mount);
   tolua_constant(tolua_S,"e_ranking_show_type_wing",faith::e_ranking_show_type_wing);
   tolua_constant(tolua_S,"e_ranking_show_type_sprite",faith::e_ranking_show_type_sprite);
   tolua_constant(tolua_S,"e_ranking_show_type_role",faith::e_ranking_show_type_role);
   tolua_constant(tolua_S,"e_ranking_show_type_challenge",faith::e_ranking_show_type_challenge);
   tolua_constant(tolua_S,"e_ranking_show_type_vip",faith::e_ranking_show_type_vip);
   tolua_constant(tolua_S,"e_ranking_show_type_legion",faith::e_ranking_show_type_legion);
   tolua_constant(tolua_S,"e_ranking_show_type_world_boss",faith::e_ranking_show_type_world_boss);
   tolua_constant(tolua_S,"e_ranking_show_type_elite",faith::e_ranking_show_type_elite);
   tolua_constant(tolua_S,"e_ranking_show_type_cross_service",faith::e_ranking_show_type_cross_service);
   tolua_constant(tolua_S,"e_ranking_show_type_max",faith::e_ranking_show_type_max);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_non",faith::e_time_limit_behavior_type_non);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_boss",faith::e_time_limit_behavior_type_kill_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_monster",faith::e_time_limit_behavior_type_kill_monster);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_treasure_lottery",faith::e_time_limit_behavior_type_treasure_lottery);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_exp_stage",faith::e_time_limit_behavior_type_exp_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_coin_stage",faith::e_time_limit_behavior_type_coin_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_illusion_stage",faith::e_time_limit_behavior_type_illusion_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_belief_stage",faith::e_time_limit_behavior_type_belief_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_sprite_stage",faith::e_time_limit_behavior_type_sprite_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_recharge",faith::e_time_limit_behavior_type_recharge);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_auction_bid",faith::e_time_limit_behavior_type_auction_bid);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_buy_in_diamond_store",faith::e_time_limit_behavior_type_buy_in_diamond_store);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_telves_awaken",faith::e_time_limit_behavior_type_telves_awaken);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_personal_boss",faith::e_time_limit_behavior_type_kill_personal_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_legion_dance",faith::e_time_limit_behavior_type_legion_dance);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_world_boss",faith::e_time_limit_behavior_type_kill_world_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_legion_boss",faith::e_time_limit_behavior_type_kill_legion_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_arena_win",faith::e_time_limit_behavior_type_arena_win);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_ranking_worship",faith::e_time_limit_behavior_type_ranking_worship);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_max",faith::e_time_limit_behavior_type_max);
   tolua_constant(tolua_S,"e_passive_skill_info_passive_skill_id",faith::e_passive_skill_info_passive_skill_id);
   tolua_constant(tolua_S,"e_passive_skill_info_equip_slot",faith::e_passive_skill_info_equip_slot);
   tolua_constant(tolua_S,"e_passive_skill_info_cd",faith::e_passive_skill_info_cd);
   tolua_constant(tolua_S,"e_passive_skill_info_max",faith::e_passive_skill_info_max);
   tolua_constant(tolua_S,"e_legion_skill_info_legion_skill_id",faith::e_legion_skill_info_legion_skill_id);
   tolua_constant(tolua_S,"e_legion_skill_info_curr_level",faith::e_legion_skill_info_curr_level);
   tolua_constant(tolua_S,"e_legion_skill_info_max",faith::e_legion_skill_info_max);
   tolua_constant(tolua_S,"e_passive_skill_info_type_non",faith::e_passive_skill_info_type_non);
   tolua_constant(tolua_S,"e_passive_skill_info_type_tinder",faith::e_passive_skill_info_type_tinder);
   tolua_constant(tolua_S,"e_passive_skill_info_type_sprite",faith::e_passive_skill_info_type_sprite);
   tolua_constant(tolua_S,"e_passive_skill_info_type_mount",faith::e_passive_skill_info_type_mount);
   tolua_constant(tolua_S,"e_passive_skill_info_type_wing",faith::e_passive_skill_info_type_wing);
   tolua_constant(tolua_S,"e_passive_skill_info_type_feather",faith::e_passive_skill_info_type_feather);
   tolua_constant(tolua_S,"e_passive_skill_info_type_phantom",faith::e_passive_skill_info_type_phantom);
   tolua_constant(tolua_S,"e_passive_skill_info_type_sky_weapon",faith::e_passive_skill_info_type_sky_weapon);
   tolua_constant(tolua_S,"e_passive_skill_info_type_mount_star",faith::e_passive_skill_info_type_mount_star);
   tolua_constant(tolua_S,"e_passive_skill_info_type_wing_psychic",faith::e_passive_skill_info_type_wing_psychic);
   tolua_constant(tolua_S,"e_passive_skill_info_type_break_will",faith::e_passive_skill_info_type_break_will);
   tolua_constant(tolua_S,"e_passive_skill_info_type_direct_get",faith::e_passive_skill_info_type_direct_get);
   tolua_constant(tolua_S,"e_passive_skill_info_type_max",faith::e_passive_skill_info_type_max);
   tolua_constant(tolua_S,"e_map_order_type_other_raid",faith::e_map_order_type_other_raid);
   tolua_constant(tolua_S,"e_map_order_type_story_raid",faith::e_map_order_type_story_raid);
   tolua_constant(tolua_S,"e_map_order_type_daily_raid",faith::e_map_order_type_daily_raid);
   tolua_constant(tolua_S,"e_map_order_type_multiplayer_raid",faith::e_map_order_type_multiplayer_raid);
   tolua_constant(tolua_S,"e_map_order_type_max",faith::e_map_order_type_max);
   tolua_constant(tolua_S,"e_pokedex_Info_template_id",faith::e_pokedex_Info_template_id);
   tolua_constant(tolua_S,"e_pokedex_Info_upgrade_num",faith::e_pokedex_Info_upgrade_num);
   tolua_constant(tolua_S,"e_pokedex_Info_state",faith::e_pokedex_Info_state);
   tolua_constant(tolua_S,"e_pokedex_Info_fetter_state_1",faith::e_pokedex_Info_fetter_state_1);
   tolua_constant(tolua_S,"e_pokedex_Info_fetter_state_2",faith::e_pokedex_Info_fetter_state_2);
   tolua_constant(tolua_S,"e_pokedex_Info_fetter_state_3",faith::e_pokedex_Info_fetter_state_3);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_1",faith::e_pokedex_Info_psyche_num_1);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_2",faith::e_pokedex_Info_psyche_num_2);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_3",faith::e_pokedex_Info_psyche_num_3);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_4",faith::e_pokedex_Info_psyche_num_4);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_5",faith::e_pokedex_Info_psyche_num_5);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_6",faith::e_pokedex_Info_psyche_num_6);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_7",faith::e_pokedex_Info_psyche_num_7);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_8",faith::e_pokedex_Info_psyche_num_8);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_9",faith::e_pokedex_Info_psyche_num_9);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_10",faith::e_pokedex_Info_psyche_num_10);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_11",faith::e_pokedex_Info_psyche_num_11);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_12",faith::e_pokedex_Info_psyche_num_12);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_13",faith::e_pokedex_Info_psyche_num_13);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_14",faith::e_pokedex_Info_psyche_num_14);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_15",faith::e_pokedex_Info_psyche_num_15);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_16",faith::e_pokedex_Info_psyche_num_16);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_17",faith::e_pokedex_Info_psyche_num_17);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_18",faith::e_pokedex_Info_psyche_num_18);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_19",faith::e_pokedex_Info_psyche_num_19);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_20",faith::e_pokedex_Info_psyche_num_20);
   tolua_constant(tolua_S,"e_pokedex_Info_max",faith::e_pokedex_Info_max);
   tolua_constant(tolua_S,"e_pokedex_operation_type_submit_psyche",faith::e_pokedex_operation_type_submit_psyche);
   tolua_constant(tolua_S,"e_pokedex_operation_type_change",faith::e_pokedex_operation_type_change);
   tolua_constant(tolua_S,"e_pokedex_operation_type_main_palace_upgrade",faith::e_pokedex_operation_type_main_palace_upgrade);
   tolua_constant(tolua_S,"e_pokedex_operation_type_oracle_activate",faith::e_pokedex_operation_type_oracle_activate);
   tolua_constant(tolua_S,"e_pokedex_operation_type_oracle_upgrade",faith::e_pokedex_operation_type_oracle_upgrade);
   tolua_constant(tolua_S,"e_pokedex_operation_type_mainland_activate",faith::e_pokedex_operation_type_mainland_activate);
   tolua_constant(tolua_S,"e_pokedex_operation_type_mainland_upgrade",faith::e_pokedex_operation_type_mainland_upgrade);
   tolua_constant(tolua_S,"e_pokedex_operation_type_max",faith::e_pokedex_operation_type_max);
   tolua_constant(tolua_S,"e_goddess_operate_type_none",faith::e_goddess_operate_type_none);
   tolua_constant(tolua_S,"e_goddess_operate_type_unlock",faith::e_goddess_operate_type_unlock);
   tolua_constant(tolua_S,"e_goddess_operate_type_equip",faith::e_goddess_operate_type_equip);
   tolua_constant(tolua_S,"e_goddess_operate_type_unequip",faith::e_goddess_operate_type_unequip);
   tolua_constant(tolua_S,"e_goddess_operate_type_fight",faith::e_goddess_operate_type_fight);
   tolua_constant(tolua_S,"e_goddess_operate_type_equip_upgrade",faith::e_goddess_operate_type_equip_upgrade);
   tolua_constant(tolua_S,"e_goddess_operate_type_goddess_slot_unlock",faith::e_goddess_operate_type_goddess_slot_unlock);
   tolua_constant(tolua_S,"e_goddess_operate_type_max",faith::e_goddess_operate_type_max);
   tolua_constant(tolua_S,"e_cross_pk_state_none",faith::e_cross_pk_state_none);
   tolua_constant(tolua_S,"e_cross_pk_state_16",faith::e_cross_pk_state_16);
   tolua_constant(tolua_S,"e_cross_pk_state_8",faith::e_cross_pk_state_8);
   tolua_constant(tolua_S,"e_cross_pk_state_4",faith::e_cross_pk_state_4);
   tolua_constant(tolua_S,"e_cross_pk_state_2",faith::e_cross_pk_state_2);
   tolua_constant(tolua_S,"e_cross_pk_state_max",faith::e_cross_pk_state_max);
   tolua_constant(tolua_S,"e_1v1_pk_state_ready",faith::e_1v1_pk_state_ready);
   tolua_constant(tolua_S,"e_1v1_pk_state_begin",faith::e_1v1_pk_state_begin);
   tolua_constant(tolua_S,"e_1v1_pk_state_end",faith::e_1v1_pk_state_end);
   tolua_constant(tolua_S,"e_1v1_pk_state_max",faith::e_1v1_pk_state_max);
   tolua_constant(tolua_S,"e_auction_state_start_sell",faith::e_auction_state_start_sell);
   tolua_constant(tolua_S,"e_auction_state_cancel",faith::e_auction_state_cancel);
   tolua_constant(tolua_S,"e_auction_state_deal",faith::e_auction_state_deal);
   tolua_constant(tolua_S,"e_auction_state_bid",faith::e_auction_state_bid);
   tolua_constant(tolua_S,"e_auction_state_max",faith::e_auction_state_max);
   tolua_constant(tolua_S,"e_cloud_shop_record_date_value",faith::e_cloud_shop_record_date_value);
   tolua_constant(tolua_S,"e_cloud_shop_record_day_index",faith::e_cloud_shop_record_day_index);
   tolua_constant(tolua_S,"e_cloud_shop_record_template_id",faith::e_cloud_shop_record_template_id);
   tolua_constant(tolua_S,"e_cloud_shop_record_total_number",faith::e_cloud_shop_record_total_number);
   tolua_constant(tolua_S,"e_cloud_shop_record_buy_count",faith::e_cloud_shop_record_buy_count);
   tolua_constant(tolua_S,"e_cloud_shop_record_send_award_flag",faith::e_cloud_shop_record_send_award_flag);
   tolua_constant(tolua_S,"e_cloud_shop_record_max",faith::e_cloud_shop_record_max);
   tolua_constant(tolua_S,"e_seven_day_goal_info_id",faith::e_seven_day_goal_info_id);
   tolua_constant(tolua_S,"e_seven_day_goal_info_schedule",faith::e_seven_day_goal_info_schedule);
   tolua_constant(tolua_S,"e_seven_day_goal_info_state",faith::e_seven_day_goal_info_state);
   tolua_constant(tolua_S,"e_seven_day_goal_info_open_time",faith::e_seven_day_goal_info_open_time);
   tolua_constant(tolua_S,"e_seven_day_goal_info_max",faith::e_seven_day_goal_info_max);
   tolua_constant(tolua_S,"e_harry_record_harry_value",faith::e_harry_record_harry_value);
   tolua_constant(tolua_S,"e_harry_record_defend_value",faith::e_harry_record_defend_value);
   tolua_constant(tolua_S,"e_harry_record_normal_harry_count",faith::e_harry_record_normal_harry_count);
   tolua_constant(tolua_S,"e_harry_record_special_harry_count",faith::e_harry_record_special_harry_count);
   tolua_constant(tolua_S,"e_harry_record_kill_ct_num",faith::e_harry_record_kill_ct_num);
   tolua_constant(tolua_S,"e_harry_record_kill_terrorist_num",faith::e_harry_record_kill_terrorist_num);
   tolua_constant(tolua_S,"e_harry_record_harry_item_server_id",faith::e_harry_record_harry_item_server_id);
   tolua_constant(tolua_S,"e_harry_record_max",faith::e_harry_record_max);
   tolua_constant(tolua_S,"e_quality_level_non",faith::e_quality_level_non);
   tolua_constant(tolua_S,"e_quality_level_one",faith::e_quality_level_one);
   tolua_constant(tolua_S,"e_quality_level_two",faith::e_quality_level_two);
   tolua_constant(tolua_S,"e_quality_level_three",faith::e_quality_level_three);
   tolua_constant(tolua_S,"e_quality_level_four",faith::e_quality_level_four);
   tolua_constant(tolua_S,"e_quality_level_five",faith::e_quality_level_five);
   tolua_constant(tolua_S,"e_quality_level_six",faith::e_quality_level_six);
   tolua_constant(tolua_S,"e_quality_level_seven",faith::e_quality_level_seven);
   tolua_constant(tolua_S,"e_quality_level_eight",faith::e_quality_level_eight);
   tolua_constant(tolua_S,"e_quality_level_nine",faith::e_quality_level_nine);
   tolua_constant(tolua_S,"e_quality_level_ten",faith::e_quality_level_ten);
   tolua_constant(tolua_S,"e_quality_level_max",faith::e_quality_level_max);
   tolua_constant(tolua_S,"e_lua_item_info_non",faith::e_lua_item_info_non);
   tolua_constant(tolua_S,"e_lua_item_info_info_id",faith::e_lua_item_info_info_id);
   tolua_constant(tolua_S,"e_lua_item_info_slot",faith::e_lua_item_info_slot);
   tolua_constant(tolua_S,"e_lua_item_info_container_type",faith::e_lua_item_info_container_type);
   tolua_constant(tolua_S,"e_lua_item_info_stack_count",faith::e_lua_item_info_stack_count);
   tolua_constant(tolua_S,"e_lua_item_info_activate",faith::e_lua_item_info_activate);
   tolua_constant(tolua_S,"e_lua_item_info_is_first",faith::e_lua_item_info_is_first);
   tolua_constant(tolua_S,"e_lua_item_info_upgrade_count",faith::e_lua_item_info_upgrade_count);
   tolua_constant(tolua_S,"e_lua_item_info_stargrade_count",faith::e_lua_item_info_stargrade_count);
   tolua_constant(tolua_S,"e_lua_item_info_starskill_flag",faith::e_lua_item_info_starskill_flag);
   tolua_constant(tolua_S,"e_lua_item_info_add_on",faith::e_lua_item_info_add_on);
   tolua_constant(tolua_S,"e_lua_item_info_locked",faith::e_lua_item_info_locked);
   tolua_constant(tolua_S,"e_lua_item_info_wing_exp",faith::e_lua_item_info_wing_exp);
   tolua_constant(tolua_S,"e_lua_item_info_wing_bless",faith::e_lua_item_info_wing_bless);
   tolua_constant(tolua_S,"e_lua_item_info_random_had_flag",faith::e_lua_item_info_random_had_flag);
   tolua_constant(tolua_S,"e_lua_item_info_random_property1",faith::e_lua_item_info_random_property1);
   tolua_constant(tolua_S,"e_lua_item_info_random_property2",faith::e_lua_item_info_random_property2);
   tolua_constant(tolua_S,"e_lua_item_info_random_property3",faith::e_lua_item_info_random_property3);
   tolua_constant(tolua_S,"e_lua_item_info_random_property4",faith::e_lua_item_info_random_property4);
   tolua_constant(tolua_S,"e_lua_item_info_random_property5",faith::e_lua_item_info_random_property5);
   tolua_constant(tolua_S,"e_lua_item_info_random_property6",faith::e_lua_item_info_random_property6);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property1",faith::e_lua_item_info_succinct_property1);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property2",faith::e_lua_item_info_succinct_property2);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property3",faith::e_lua_item_info_succinct_property3);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property4",faith::e_lua_item_info_succinct_property4);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property5",faith::e_lua_item_info_succinct_property5);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property6",faith::e_lua_item_info_succinct_property6);
   tolua_constant(tolua_S,"e_lua_item_info_showing_illusion_idex",faith::e_lua_item_info_showing_illusion_idex);
   tolua_constant(tolua_S,"e_lua_item_info_put_in_bag_count",faith::e_lua_item_info_put_in_bag_count);
   tolua_constant(tolua_S,"e_lua_item_info_illusion_had_byte",faith::e_lua_item_info_illusion_had_byte);
   tolua_constant(tolua_S,"e_lua_item_info_over_time",faith::e_lua_item_info_over_time);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_level",faith::e_lua_item_info_succinct_level);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_0",faith::e_lua_item_info_jewel_slot_0);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_1",faith::e_lua_item_info_jewel_slot_1);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_2",faith::e_lua_item_info_jewel_slot_2);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_3",faith::e_lua_item_info_jewel_slot_3);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_4",faith::e_lua_item_info_jewel_slot_4);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_vip_slot_0",faith::e_lua_item_info_jewel_vip_slot_0);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_vip_slot_1",faith::e_lua_item_info_jewel_vip_slot_1);
   tolua_constant(tolua_S,"e_lua_item_info_prototype_id",faith::e_lua_item_info_prototype_id);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property1",faith::e_lua_item_info_forge_property1);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property2",faith::e_lua_item_info_forge_property2);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property3",faith::e_lua_item_info_forge_property3);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property4",faith::e_lua_item_info_forge_property4);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property5",faith::e_lua_item_info_forge_property5);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property6",faith::e_lua_item_info_forge_property6);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_carve_0",faith::e_lua_item_info_jewel_carve_0);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_carve_1",faith::e_lua_item_info_jewel_carve_1);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_carve_2",faith::e_lua_item_info_jewel_carve_2);
   tolua_constant(tolua_S,"e_lua_item_info_max",faith::e_lua_item_info_max);
   tolua_constant(tolua_S,"e_money_array_type_cur",faith::e_money_array_type_cur);
   tolua_constant(tolua_S,"e_money_array_type_total_get",faith::e_money_array_type_total_get);
   tolua_constant(tolua_S,"e_money_array_type_total_cost",faith::e_money_array_type_total_cost);
   tolua_constant(tolua_S,"e_money_array_type_max",faith::e_money_array_type_max);
   tolua_constant(tolua_S,"e_summon_pet_type_normal_summon",faith::e_summon_pet_type_normal_summon);
   tolua_constant(tolua_S,"e_summon_pet_type_goddess",faith::e_summon_pet_type_goddess);
   tolua_constant(tolua_S,"e_summon_pet_type_boss_pet",faith::e_summon_pet_type_boss_pet);
   tolua_constant(tolua_S,"e_summon_pet_type_max",faith::e_summon_pet_type_max);
   tolua_constant(tolua_S,"e_version_region_type_china_main",faith::e_version_region_type_china_main);
   tolua_constant(tolua_S,"e_version_region_type_kr",faith::e_version_region_type_kr);
   tolua_constant(tolua_S,"e_version_region_type_china_tw",faith::e_version_region_type_china_tw);
   tolua_constant(tolua_S,"e_version_region_type_en",faith::e_version_region_type_en);
   tolua_constant(tolua_S,"e_version_region_type_tha",faith::e_version_region_type_tha);
   tolua_constant(tolua_S,"e_version_region_type_vn",faith::e_version_region_type_vn);
   tolua_constant(tolua_S,"e_version_region_type_jp",faith::e_version_region_type_jp);
   tolua_constant(tolua_S,"e_version_region_type_rus",faith::e_version_region_type_rus);
   tolua_constant(tolua_S,"e_version_region_type_sg",faith::e_version_region_type_sg);
   tolua_constant(tolua_S,"e_version_region_type_max",faith::e_version_region_type_max);
   tolua_constant(tolua_S,"e_manual_reward_type_add_item",faith::e_manual_reward_type_add_item);
   tolua_constant(tolua_S,"e_manual_reward_type_manual_level",faith::e_manual_reward_type_manual_level);
   tolua_constant(tolua_S,"e_manual_reward_type_add_point",faith::e_manual_reward_type_add_point);
   tolua_constant(tolua_S,"e_manual_reward_type_max",faith::e_manual_reward_type_max);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_none",faith::e_time_limit_shop_purchase_limit_type_none);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_day",faith::e_time_limit_shop_purchase_limit_type_day);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_cycle",faith::e_time_limit_shop_purchase_limit_type_cycle);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_max",faith::e_time_limit_shop_purchase_limit_type_max);
   tolua_constant(tolua_S,"e_role_i64_info_gs_value",faith::e_role_i64_info_gs_value);
   tolua_constant(tolua_S,"e_role_i64_info_history_best_gs_value",faith::e_role_i64_info_history_best_gs_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_mount_up_start_value",faith::e_role_i64_info_zero_mount_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_wing_up_start_value",faith::e_role_i64_info_zero_wing_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_equip_up_start_value",faith::e_role_i64_info_zero_equip_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_spirit_up_start_value",faith::e_role_i64_info_zero_spirit_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_create_time",faith::e_role_i64_info_create_time);
   tolua_constant(tolua_S,"e_role_i64_info_max",faith::e_role_i64_info_max);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_berserker",faith::e_cross_server_pk_sub_type_berserker);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_caster",faith::e_cross_server_pk_sub_type_caster);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_add_rider",faith::e_cross_server_pk_sub_type_add_rider);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_add_assassion",faith::e_cross_server_pk_sub_type_add_assassion);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_word_boss",faith::e_cross_server_pk_sub_type_word_boss);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_max",faith::e_cross_server_pk_sub_type_max);
   tolua_constant(tolua_S,"e_star_trip_target_type_buy_goods",faith::e_star_trip_target_type_buy_goods);
   tolua_constant(tolua_S,"e_star_trip_target_type_buy_single_boss",faith::e_star_trip_target_type_buy_single_boss);
   tolua_constant(tolua_S,"e_star_trip_target_type_convert",faith::e_star_trip_target_type_convert);
   tolua_constant(tolua_S,"e_star_trip_target_type_legion_donate",faith::e_star_trip_target_type_legion_donate);
   tolua_constant(tolua_S,"e_star_trip_target_type_join_activity",faith::e_star_trip_target_type_join_activity);
   tolua_constant(tolua_S,"e_star_trip_target_type_buy_today_gift",faith::e_star_trip_target_type_buy_today_gift);
   tolua_constant(tolua_S,"e_star_trip_target_type_map_finish",faith::e_star_trip_target_type_map_finish);
   tolua_constant(tolua_S,"e_star_trip_target_type_kill_single_boss",faith::e_star_trip_target_type_kill_single_boss);
   tolua_constant(tolua_S,"e_star_trip_target_type_kill_home_boss",faith::e_star_trip_target_type_kill_home_boss);
   tolua_constant(tolua_S,"e_star_trip_target_type_treasure",faith::e_star_trip_target_type_treasure);
   tolua_constant(tolua_S,"e_star_trip_target_type_recharge",faith::e_star_trip_target_type_recharge);
   tolua_constant(tolua_S,"e_star_trip_target_type_bid",faith::e_star_trip_target_type_bid);
   tolua_constant(tolua_S,"e_star_trip_target_type_max",faith::e_star_trip_target_type_max);
   tolua_constant(tolua_S,"e_map_play_type_non",faith::e_map_play_type_non);
   tolua_constant(tolua_S,"e_map_play_type_kill_boss",faith::e_map_play_type_kill_boss);
   tolua_constant(tolua_S,"e_map_play_type_max",faith::e_map_play_type_max);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_no_kill",faith::e_map_search_boss_type_boss_no_kill);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_can_kill",faith::e_map_search_boss_type_boss_can_kill);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_wait",faith::e_map_search_boss_type_boss_wait);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_refresh",faith::e_map_search_boss_type_boss_refresh);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_killed",faith::e_map_search_boss_type_boss_killed);
   tolua_constant(tolua_S,"e_map_search_boss_type_be_killed",faith::e_map_search_boss_type_be_killed);
   tolua_constant(tolua_S,"e_map_search_boss_type_max",faith::e_map_search_boss_type_max);
   tolua_constant(tolua_S,"e_daily_play_count_element_map",faith::e_daily_play_count_element_map);
   tolua_constant(tolua_S,"e_daily_play_count_battle_map",faith::e_daily_play_count_battle_map);
   tolua_constant(tolua_S,"e_daily_play_count_max",faith::e_daily_play_count_max);
   tolua_constant(tolua_S,"e_time_activity_type_null",faith::e_time_activity_type_null);
   tolua_constant(tolua_S,"e_time_activity_type_wheel_plate",faith::e_time_activity_type_wheel_plate);
   tolua_constant(tolua_S,"e_time_activity_type_max",faith::e_time_activity_type_max);
   tolua_constant(tolua_S,"e_time_activity_score_type_nomal_search",faith::e_time_activity_score_type_nomal_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_monster_search",faith::e_time_activity_score_type_monster_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_flip_box_search",faith::e_time_activity_score_type_flip_box_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_finger_guess_search",faith::e_time_activity_score_type_finger_guess_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_special_search",faith::e_time_activity_score_type_special_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_max",faith::e_time_activity_score_type_max);
   tolua_constant(tolua_S,"e_wheel_plate_info_accumulate_num",faith::e_wheel_plate_info_accumulate_num);
   tolua_constant(tolua_S,"e_wheel_plate_info_open_index",faith::e_wheel_plate_info_open_index);
   tolua_constant(tolua_S,"e_wheel_plate_info_award_index",faith::e_wheel_plate_info_award_index);
   tolua_constant(tolua_S,"e_wheel_plate_info_max",faith::e_wheel_plate_info_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"ws_client","faith::ws_client","",NULL);
   tolua_beginmodule(tolua_S,"ws_client");
    tolua_function(tolua_S,"getInstance",tolua_logic_hld_ws_client_getInstance00);
    tolua_function(tolua_S,"send_to_dp_lua",tolua_logic_hld_ws_client_send_to_dp_lua00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"dp2ws_rep_get_game_info_ws_lua",tolua_logic_hld_dp2ws_rep_get_game_info_ws_lua00);
   tolua_function(tolua_S,"dp2ws_rep_get_role_info_ws_lua",tolua_logic_hld_dp2ws_rep_get_role_info_ws_lua00);
   tolua_function(tolua_S,"dp2ws_rep_load_arena_rank_lua",tolua_logic_hld_dp2ws_rep_load_arena_rank_lua00);
   tolua_function(tolua_S,"dp2ws_rep_load_all_ranking_info_lua",tolua_logic_hld_dp2ws_rep_load_all_ranking_info_lua00);
   tolua_function(tolua_S,"dp2ws_rep_load_one_ranking_info_lua",tolua_logic_hld_dp2ws_rep_load_one_ranking_info_lua00);
   tolua_function(tolua_S,"dp2ws_rep_load_one_worship_info_proc_lua",tolua_logic_hld_dp2ws_rep_load_one_worship_info_proc_lua00);
   tolua_function(tolua_S,"dp2ws_rep_load_all_service_rank_info_lua",tolua_logic_hld_dp2ws_rep_load_all_service_rank_info_lua00);
   tolua_function(tolua_S,"dp2ws_rep_load_last_server_list_lua",tolua_logic_hld_dp2ws_rep_load_last_server_list_lua00);
   tolua_function(tolua_S,"dp2ws_gm_set_result_proc_lua",tolua_logic_hld_dp2ws_gm_set_result_proc_lua00);
   tolua_function(tolua_S,"dp2ws_save_recharge_proc_lua",tolua_logic_hld_dp2ws_save_recharge_proc_lua00);
   tolua_function(tolua_S,"dp2ws_send_recharge_end_result_proc_lua",tolua_logic_hld_dp2ws_send_recharge_end_result_proc_lua00);
   tolua_function(tolua_S,"dp2ws_gm_get_server_info_proc_lua",tolua_logic_hld_dp2ws_gm_get_server_info_proc_lua00);
   tolua_function(tolua_S,"dp2ws_gm_change_account_proc_lua",tolua_logic_hld_dp2ws_gm_change_account_proc_lua00);
   tolua_function(tolua_S,"dp2ws_gm_get_role_info_proc_lua",tolua_logic_hld_dp2ws_gm_get_role_info_proc_lua00);
   tolua_function(tolua_S,"dp2ws_gm_modify_role_info_proc_lua",tolua_logic_hld_dp2ws_gm_modify_role_info_proc_lua00);
   tolua_function(tolua_S,"lua_dp2ws_load_red_package_end_proc",tolua_logic_hld_lua_dp2ws_load_red_package_end_proc00);
   tolua_function(tolua_S,"lua_dp2ws_load_red_package_receiver_end_proc",tolua_logic_hld_lua_dp2ws_load_red_package_receiver_end_proc00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"arena_mgr_ws","faith::arena_mgr_ws","",NULL);
   tolua_beginmodule(tolua_S,"arena_mgr_ws");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_arena_mgr_ws_get_instance00);
    tolua_function(tolua_S,"create_robot",tolua_logic_hld_arena_mgr_ws_create_robot00);
    tolua_function(tolua_S,"fill_empty_pos",tolua_logic_hld_arena_mgr_ws_fill_empty_pos00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"cross_server_city_war_ws_mgr","faith::cross_server_city_war_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_city_war_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_city_war_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_in_game_time",tolua_logic_hld_cross_server_city_war_ws_mgr_is_in_game_time00);
    tolua_function(tolua_S,"get_cur_legion_in_rank",tolua_logic_hld_cross_server_city_war_ws_mgr_get_cur_legion_in_rank00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_banquet_level_ordinary",faith::e_banquet_level_ordinary);
   tolua_constant(tolua_S,"e_banquet_level_hearty",faith::e_banquet_level_hearty);
   tolua_constant(tolua_S,"e_banquet_level_luxury",faith::e_banquet_level_luxury);
   tolua_constant(tolua_S,"e_banquet_level_max",faith::e_banquet_level_max);
   tolua_constant(tolua_S,"e_legion_job_title_none",faith::e_legion_job_title_none);
   tolua_constant(tolua_S,"e_legion_job_title_commander",faith::e_legion_job_title_commander);
   tolua_constant(tolua_S,"e_legion_job_title_assistant_chief",faith::e_legion_job_title_assistant_chief);
   tolua_constant(tolua_S,"e_legion_job_title_chief",faith::e_legion_job_title_chief);
   tolua_constant(tolua_S,"e_legion_job_title_max",faith::e_legion_job_title_max);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_main",faith::ELegionInfo_construction_level_main);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_1",faith::ELegionInfo_construction_level_1);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_2",faith::ELegionInfo_construction_level_2);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_3",faith::ELegionInfo_construction_level_3);
   tolua_constant(tolua_S,"ELegionInfo_asset_money",faith::ELegionInfo_asset_money);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_1",faith::ELegionInfo_asset_item_1);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_2",faith::ELegionInfo_asset_item_2);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_3",faith::ELegionInfo_asset_item_3);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_4",faith::ELegionInfo_asset_item_4);
   tolua_constant(tolua_S,"ELegionInfo_fighting_power",faith::ELegionInfo_fighting_power);
   tolua_constant(tolua_S,"ELegionInfo_auto_accept_new_member",faith::ELegionInfo_auto_accept_new_member);
   tolua_constant(tolua_S,"ELegionInfo_last_refresh_time",faith::ELegionInfo_last_refresh_time);
   tolua_constant(tolua_S,"ELegionInfo_glory_glow",faith::ELegionInfo_glory_glow);
   tolua_constant(tolua_S,"ELegionInfo_act_score_raid",faith::ELegionInfo_act_score_raid);
   tolua_constant(tolua_S,"ELegionInfo_act_score_bonfire",faith::ELegionInfo_act_score_bonfire);
   tolua_constant(tolua_S,"ELegionInfo_legion_welfare_num",faith::ELegionInfo_legion_welfare_num);
   tolua_constant(tolua_S,"ELegionInfo_max",faith::ELegionInfo_max);
   tolua_constant(tolua_S,"e_legion_apply_stage_send_failed",faith::e_legion_apply_stage_send_failed);
   tolua_constant(tolua_S,"e_legion_apply_stage_send_success",faith::e_legion_apply_stage_send_success);
   tolua_constant(tolua_S,"e_legion_apply_stage_is_accepted",faith::e_legion_apply_stage_is_accepted);
   tolua_constant(tolua_S,"e_legion_apply_stage_is_refused",faith::e_legion_apply_stage_is_refused);
   tolua_constant(tolua_S,"e_legion_apply_stage_auto_accepted",faith::e_legion_apply_stage_auto_accepted);
   tolua_constant(tolua_S,"e_legion_apply_stage_max",faith::e_legion_apply_stage_max);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"s_legion_member_info","faith::s_legion_member_info","",tolua_collect_hld__s_legion_member_info);
   #else
   tolua_cclass(tolua_S,"s_legion_member_info","faith::s_legion_member_info","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"s_legion_member_info");
    tolua_variable(tolua_S,"role_guid",tolua_get_hld__s_legion_member_info_role_guid,tolua_set_hld__s_legion_member_info_role_guid);
    tolua_variable(tolua_S,"role_name",tolua_get_hld__s_legion_member_info_role_name,tolua_set_hld__s_legion_member_info_role_name);
    tolua_variable(tolua_S,"gs_value",tolua_get_hld__s_legion_member_info_gs_value,tolua_set_hld__s_legion_member_info_gs_value);
    tolua_array(tolua_S,"data_ary",tolua_get_logic_hld_s_legion_member_info_data_ary,tolua_set_logic_hld_s_legion_member_info_data_ary);
    tolua_function(tolua_S,"new",tolua_logic_hld_s_legion_member_info_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_s_legion_member_info_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_s_legion_member_info_new00_local);
   tolua_endmodule(tolua_S);
   tolua_constant(tolua_S,"e_legion_name_error_change_name_succeed",faith::e_legion_name_error_change_name_succeed);
   tolua_constant(tolua_S,"e_legion_name_error_not_chief",faith::e_legion_name_error_not_chief);
   tolua_constant(tolua_S,"e_legion_name_error_formal_error",faith::e_legion_name_error_formal_error);
   tolua_constant(tolua_S,"e_legion_name_error_repetition",faith::e_legion_name_error_repetition);
   tolua_constant(tolua_S,"e_legion_name_error_contains_illegal_characters",faith::e_legion_name_error_contains_illegal_characters);
   tolua_constant(tolua_S,"e_legion_name_error_cross_server",faith::e_legion_name_error_cross_server);
   tolua_constant(tolua_S,"e_legion_name_error_available",faith::e_legion_name_error_available);
   tolua_constant(tolua_S,"e_legion_name_error_player_not_legion",faith::e_legion_name_error_player_not_legion);
   tolua_constant(tolua_S,"e_legion_name_error_item_not_enough",faith::e_legion_name_error_item_not_enough);
   tolua_constant(tolua_S,"e_legion_error_none",faith::e_legion_error_none);
   tolua_constant(tolua_S,"e_legion_error_template_error",faith::e_legion_error_template_error);
   tolua_constant(tolua_S,"e_legion_error_member_full",faith::e_legion_error_member_full);
   tolua_constant(tolua_S,"e_legion_error_applicant_list_full",faith::e_legion_error_applicant_list_full);
   tolua_constant(tolua_S,"e_legion_error_applicant_already_exist",faith::e_legion_error_applicant_already_exist);
   tolua_constant(tolua_S,"e_legion_error_player_added_not_online",faith::e_legion_error_player_added_not_online);
   tolua_constant(tolua_S,"e_legion_error_player_added_is_legion_member",faith::e_legion_error_player_added_is_legion_member);
   tolua_constant(tolua_S,"e_legion_error_player_added_is_already_join_other_legion",faith::e_legion_error_player_added_is_already_join_other_legion);
   tolua_constant(tolua_S,"e_legion_error_used_all_take_part_in_banquet_times",faith::e_legion_error_used_all_take_part_in_banquet_times);
   tolua_constant(tolua_S,"e_legion_error_banquet_already_over",faith::e_legion_error_banquet_already_over);
   tolua_constant(tolua_S,"e_legion_error_no_city_war_qualification",faith::e_legion_error_no_city_war_qualification);
   tolua_constant(tolua_S,"e_legion_error_city_war_not_begin",faith::e_legion_error_city_war_not_begin);
   tolua_constant(tolua_S,"e_legion_error_can_not_dissolve_during_city_war",faith::e_legion_error_can_not_dissolve_during_city_war);
   tolua_constant(tolua_S,"e_legion_error_can_not_leave_legion_during_city_war",faith::e_legion_error_can_not_leave_legion_during_city_war);
   tolua_constant(tolua_S,"e_legion_error_error_legion_name_illegal",faith::e_legion_error_error_legion_name_illegal);
   tolua_constant(tolua_S,"e_legion_error_can_not_dissolve_during_banquet",faith::e_legion_error_can_not_dissolve_during_banquet);
   tolua_constant(tolua_S,"e_legion_error_can_not_kickout_member_during_city_war",faith::e_legion_error_can_not_kickout_member_during_city_war);
   tolua_constant(tolua_S,"e_legion_error_repeat_legion_name",faith::e_legion_error_repeat_legion_name);
   tolua_constant(tolua_S,"e_legion_error_full_legion_num",faith::e_legion_error_full_legion_num);
   tolua_constant(tolua_S,"e_legion_error_create_legion_invalid_role_state",faith::e_legion_error_create_legion_invalid_role_state);
   tolua_constant(tolua_S,"e_legion_error_create_legion_no_enough_level",faith::e_legion_error_create_legion_no_enough_level);
   tolua_constant(tolua_S,"e_legion_error_create_legion_no_enough_money",faith::e_legion_error_create_legion_no_enough_money);
   tolua_constant(tolua_S,"e_legion_error_invalid_banquet_type",faith::e_legion_error_invalid_banquet_type);
   tolua_constant(tolua_S,"e_legion_error_can_cur_time_error",faith::e_legion_error_can_cur_time_error);
   tolua_constant(tolua_S,"e_legion_error_can_not_hold_banquet_in_legion_war_day",faith::e_legion_error_can_not_hold_banquet_in_legion_war_day);
   tolua_constant(tolua_S,"e_legion_error_invalid_hold_banquet_time",faith::e_legion_error_invalid_hold_banquet_time);
   tolua_constant(tolua_S,"e_legion_error_request_hold_banquet_again",faith::e_legion_error_request_hold_banquet_again);
   tolua_constant(tolua_S,"e_legion_error_already_occupy_stronghold",faith::e_legion_error_already_occupy_stronghold);
   tolua_constant(tolua_S,"e_legion_error_already_get_occupation_daily_award",faith::e_legion_error_already_get_occupation_daily_award);
   tolua_constant(tolua_S,"e_legion_error_next_day_get_occupation_daily_award",faith::e_legion_error_next_day_get_occupation_daily_award);
   tolua_constant(tolua_S,"e_legion_error_city_war_gather_crystal_field_not_open",faith::e_legion_error_city_war_gather_crystal_field_not_open);
   tolua_constant(tolua_S,"e_legion_error_stronghold_occupied_by_others",faith::e_legion_error_stronghold_occupied_by_others);
   tolua_constant(tolua_S,"e_legion_error_crystal_gathered_by_others",faith::e_legion_error_crystal_gathered_by_others);
   tolua_constant(tolua_S,"e_legion_error_job_title_num_max",faith::e_legion_error_job_title_num_max);
   tolua_constant(tolua_S,"e_legion_error_player_joined_other_legion",faith::e_legion_error_player_joined_other_legion);
   tolua_constant(tolua_S,"e_legion_error_is_in_legion_answer_time",faith::e_legion_error_is_in_legion_answer_time);
   tolua_constant(tolua_S,"e_legion_error_max",faith::e_legion_error_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"legion_ws_applicant_list","faith::legion_ws_applicant_list","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_applicant_list");
    tolua_function(tolua_S,"get_applicant",tolua_logic_hld_legion_ws_applicant_list_get_applicant00);
    tolua_function(tolua_S,"del_applicant",tolua_logic_hld_legion_ws_applicant_list_del_applicant00);
    tolua_function(tolua_S,"del_applicant_with_join_legion",tolua_logic_hld_legion_ws_applicant_list_del_applicant_with_join_legion00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"legion_ws_answer","faith::legion_ws_answer","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_answer");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_legion_ws_answer_get_instance00);
    tolua_function(tolua_S,"get_legion_question_info",tolua_logic_hld_legion_ws_answer_get_legion_question_info00);
    tolua_function(tolua_S,"send_legion_answer_rank_list",tolua_logic_hld_legion_ws_answer_send_legion_answer_rank_list00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"legion_ws_warehouse","faith::legion_ws_warehouse","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_warehouse");
    tolua_function(tolua_S,"send_item_list_to_client",tolua_logic_hld_legion_ws_warehouse_send_item_list_to_client00);
    tolua_function(tolua_S,"send_log_list_to_client",tolua_logic_hld_legion_ws_warehouse_send_log_list_to_client00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"legion_ws","faith::legion_ws","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws");
    tolua_function(tolua_S,"set_legion_info",tolua_logic_hld_legion_ws_set_legion_info00);
    tolua_function(tolua_S,"get_legion_gs_value",tolua_logic_hld_legion_ws_get_legion_gs_value00);
    tolua_function(tolua_S,"get_member",tolua_logic_hld_legion_ws_get_member00);
    tolua_function(tolua_S,"add_member",tolua_logic_hld_legion_ws_add_member00);
    tolua_function(tolua_S,"add_member",tolua_logic_hld_legion_ws_add_member01);
    tolua_function(tolua_S,"add_member",tolua_logic_hld_legion_ws_add_member02);
    tolua_function(tolua_S,"construction_level_up",tolua_logic_hld_legion_ws_construction_level_up00);
    tolua_function(tolua_S,"is_in_this_legion",tolua_logic_hld_legion_ws_is_in_this_legion00);
    tolua_function(tolua_S,"send_message_to_all_member_data_lua",tolua_logic_hld_legion_ws_send_message_to_all_member_data_lua00);
    tolua_function(tolua_S,"leave_voie_channel",tolua_logic_hld_legion_ws_leave_voie_channel00);
    tolua_function(tolua_S,"send_applicant_info_list",tolua_logic_hld_legion_ws_send_applicant_info_list00);
    tolua_function(tolua_S,"del_member",tolua_logic_hld_legion_ws_del_member00);
    tolua_function(tolua_S,"appoint",tolua_logic_hld_legion_ws_appoint00);
    tolua_function(tolua_S,"get_job_title",tolua_logic_hld_legion_ws_get_job_title00);
    tolua_function(tolua_S,"get_legion_rank",tolua_logic_hld_legion_ws_get_legion_rank00);
    tolua_function(tolua_S,"is_chief",tolua_logic_hld_legion_ws_is_chief00);
    tolua_function(tolua_S,"is_assistant_chief",tolua_logic_hld_legion_ws_is_assistant_chief00);
    tolua_function(tolua_S,"get_legion_guid",tolua_logic_hld_legion_ws_get_legion_guid00);
    tolua_function(tolua_S,"set_announcement",tolua_logic_hld_legion_ws_set_announcement00);
    tolua_function(tolua_S,"send_legion_announcement",tolua_logic_hld_legion_ws_send_legion_announcement00);
    tolua_function(tolua_S,"get_legion_name",tolua_logic_hld_legion_ws_get_legion_name00);
    tolua_function(tolua_S,"get_chief_guid",tolua_logic_hld_legion_ws_get_chief_guid00);
    tolua_function(tolua_S,"send_legion_attr_one",tolua_logic_hld_legion_ws_send_legion_attr_one00);
    tolua_function(tolua_S,"level_up_legion_skill",tolua_logic_hld_legion_ws_level_up_legion_skill00);
    tolua_function(tolua_S,"join_legion_station_map",tolua_logic_hld_legion_ws_join_legion_station_map00);
    tolua_function(tolua_S,"send_legion_boss_map_info",tolua_logic_hld_legion_ws_send_legion_boss_map_info00);
    tolua_function(tolua_S,"get_applicant_list",tolua_logic_hld_legion_ws_get_applicant_list00);
    tolua_function(tolua_S,"get_warehouse",tolua_logic_hld_legion_ws_get_warehouse00);
    tolua_function(tolua_S,"set_auction_selling_check_start",tolua_logic_hld_legion_ws_set_auction_selling_check_start00);
    tolua_function(tolua_S,"join_legion_station_map",tolua_logic_hld_legion_ws_join_legion_station_map01);
    tolua_function(tolua_S,"join_voice_channel",tolua_logic_hld_legion_ws_join_voice_channel00);
    tolua_function(tolua_S,"leave_voie_channel",tolua_logic_hld_legion_ws_leave_voie_channel01);
    tolua_function(tolua_S,"change_legion_name",tolua_logic_hld_legion_ws_change_legion_name00);
    tolua_function(tolua_S,"set_next_bid_time",tolua_logic_hld_legion_ws_set_next_bid_time00);
    tolua_function(tolua_S,"is_can_bid",tolua_logic_hld_legion_ws_is_can_bid00);
    tolua_function(tolua_S,"get_all_member_guid",tolua_logic_hld_legion_ws_get_all_member_guid00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"city_war_territory_mgr","faith::city_war_territory_mgr","",NULL);
   tolua_beginmodule(tolua_S,"city_war_territory_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_city_war_territory_mgr_get_instance00);
    tolua_function(tolua_S,"bid_one_territory",tolua_logic_hld_city_war_territory_mgr_bid_one_territory00);
    tolua_function(tolua_S,"set_is_prior_maintain",tolua_logic_hld_city_war_territory_mgr_set_is_prior_maintain00);
    tolua_function(tolua_S,"req_cross_server_apply_city_war_bid",tolua_logic_hld_city_war_territory_mgr_req_cross_server_apply_city_war_bid00);
    tolua_function(tolua_S,"req_cross_server_territory_prior_maintain",tolua_logic_hld_city_war_territory_mgr_req_cross_server_territory_prior_maintain00);
    tolua_function(tolua_S,"req_cross_server_legion_city_war_info",tolua_logic_hld_city_war_territory_mgr_req_cross_server_legion_city_war_info00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"legion_ws_city_war","faith::legion_ws_city_war","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_city_war");
    tolua_function(tolua_S,"get_overlord_legion",tolua_logic_hld_legion_ws_city_war_get_overlord_legion00);
    tolua_function(tolua_S,"get_overlord_legion_name",tolua_logic_hld_legion_ws_city_war_get_overlord_legion_name00);
    tolua_function(tolua_S,"get_city_master_guid",tolua_logic_hld_legion_ws_city_war_get_city_master_guid00);
    tolua_function(tolua_S,"get_city_master_name",tolua_logic_hld_legion_ws_city_war_get_city_master_name00);
    tolua_function(tolua_S,"is_during_city_war",tolua_logic_hld_legion_ws_city_war_is_during_city_war00);
    tolua_function(tolua_S,"is_during_bid_time",tolua_logic_hld_legion_ws_city_war_is_during_bid_time00);
    tolua_function(tolua_S,"send_territory_info_one",tolua_logic_hld_legion_ws_city_war_send_territory_info_one00);
    tolua_function(tolua_S,"send_city_war_info_all_msg",tolua_logic_hld_legion_ws_city_war_send_city_war_info_all_msg00);
    tolua_function(tolua_S,"is_banquet_holding",tolua_logic_hld_legion_ws_city_war_is_banquet_holding00);
    tolua_function(tolua_S,"take_part_in_banquet",tolua_logic_hld_legion_ws_city_war_take_part_in_banquet00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"legion_ws_mgr","faith::legion_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_legion_ws_mgr_get_instance00);
    tolua_function(tolua_S,"del_legion",tolua_logic_hld_legion_ws_mgr_del_legion00);
    tolua_function(tolua_S,"get_legion",tolua_logic_hld_legion_ws_mgr_get_legion00);
    tolua_function(tolua_S,"create_legion_begin",tolua_logic_hld_legion_ws_mgr_create_legion_begin00);
    tolua_function(tolua_S,"check_can_join_legion",tolua_logic_hld_legion_ws_mgr_check_can_join_legion00);
    tolua_function(tolua_S,"check_can_leave_legion",tolua_logic_hld_legion_ws_mgr_check_can_leave_legion00);
    tolua_function(tolua_S,"generate_player_legion_info",tolua_logic_hld_legion_ws_mgr_generate_player_legion_info00);
    tolua_function(tolua_S,"send_legion_error_to_session",tolua_logic_hld_legion_ws_mgr_send_legion_error_to_session00);
    tolua_function(tolua_S,"get_unit_legion",tolua_logic_hld_legion_ws_mgr_get_unit_legion00);
    tolua_function(tolua_S,"add_into_auto_accept_new_member_ordered_legion_list",tolua_logic_hld_legion_ws_mgr_add_into_auto_accept_new_member_ordered_legion_list00);
    tolua_function(tolua_S,"del_from_auto_accept_new_member_ordered_legion_list",tolua_logic_hld_legion_ws_mgr_del_from_auto_accept_new_member_ordered_legion_list00);
    tolua_function(tolua_S,"get_legion_city_war",tolua_logic_hld_legion_ws_mgr_get_legion_city_war00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"c2ws_req_get_legion_list_logic",tolua_logic_hld_c2ws_req_get_legion_list_logic00);
   tolua_function(tolua_S,"c2ws_req_legion_donate_money_logic",tolua_logic_hld_c2ws_req_legion_donate_money_logic00);
   tolua_function(tolua_S,"c2ws_req_legion_donate_item_logic",tolua_logic_hld_c2ws_req_legion_donate_item_logic00);
   tolua_function(tolua_S,"c2ws_req_get_event_list_logic",tolua_logic_hld_c2ws_req_get_event_list_logic00);
   tolua_function(tolua_S,"c2ws_req_get_legion_boss_award_logic",tolua_logic_hld_c2ws_req_get_legion_boss_award_logic00);
   tolua_function(tolua_S,"c2ws_req_get_banquet_info_logic",tolua_logic_hld_c2ws_req_get_banquet_info_logic00);
   tolua_function(tolua_S,"c2ws_req_request_hold_banquet_logic",tolua_logic_hld_c2ws_req_request_hold_banquet_logic00);
   tolua_function(tolua_S,"c2ws_req_enter_city_war_map_logic",tolua_logic_hld_c2ws_req_enter_city_war_map_logic00);
   tolua_function(tolua_S,"c2ws_req_city_war_operate_logic",tolua_logic_hld_c2ws_req_city_war_operate_logic00);
   tolua_function(tolua_S,"c2ws_req_add_fuel_to_fire_logic",tolua_logic_hld_c2ws_req_add_fuel_to_fire_logic00);
   tolua_function(tolua_S,"c2ws_req_send_legion_recruit_in_world_logic",tolua_logic_hld_c2ws_req_send_legion_recruit_in_world_logic00);
   tolua_function(tolua_S,"c2ws_req_get_city_war_legion_rank_info_logic",tolua_logic_hld_c2ws_req_get_city_war_legion_rank_info_logic00);
   tolua_function(tolua_S,"c2ws_req_draw_bonfire_daily_reward_logic",tolua_logic_hld_c2ws_req_draw_bonfire_daily_reward_logic00);
   tolua_function(tolua_S,"c2ws_req_legion_call_operate_lua",tolua_logic_hld_c2ws_req_legion_call_operate_lua00);
   tolua_function(tolua_S,"c2ws_req_get_bonus_rewards_lua",tolua_logic_hld_c2ws_req_get_bonus_rewards_lua00);
   tolua_function(tolua_S,"c2ws_req_stimulate_bonus_lua",tolua_logic_hld_c2ws_req_stimulate_bonus_lua00);
   tolua_function(tolua_S,"c2ws_req_join_cross_city_war_lua",tolua_logic_hld_c2ws_req_join_cross_city_war_lua00);
   tolua_function(tolua_S,"c2ws_req_goods_operate_lua",tolua_logic_hld_c2ws_req_goods_operate_lua00);
   tolua_function(tolua_S,"c2ws_req_invite_player_join_team_lua",tolua_logic_hld_c2ws_req_invite_player_join_team_lua00);
   tolua_function(tolua_S,"c2ws_req_ready_lua",tolua_logic_hld_c2ws_req_ready_lua00);
   tolua_function(tolua_S,"c2ws_req_search_team_list_lua",tolua_logic_hld_c2ws_req_search_team_list_lua00);
   tolua_function(tolua_S,"c2ws_req_get_team_list_by_aim_lua",tolua_logic_hld_c2ws_req_get_team_list_by_aim_lua00);
   tolua_function(tolua_S,"c2ws_req_team_mem_operate_lua",tolua_logic_hld_c2ws_req_team_mem_operate_lua00);
   tolua_function(tolua_S,"c2ws_req_get_best_record_lua",tolua_logic_hld_c2ws_req_get_best_record_lua00);
   tolua_function(tolua_S,"c2ws_req_syc_nearly_state_lua",tolua_logic_hld_c2ws_req_syc_nearly_state_lua00);
   tolua_function(tolua_S,"c2ws_req_operate_other_req_lua",tolua_logic_hld_c2ws_req_operate_other_req_lua00);
   tolua_function(tolua_S,"c2s_confirm_receive_marry_task_lua",tolua_logic_hld_c2s_confirm_receive_marry_task_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"invalid_ansi_word","faith::invalid_ansi_word","",NULL);
   tolua_beginmodule(tolua_S,"invalid_ansi_word");
    tolua_function(tolua_S,"include_invalid_ansi_str",tolua_logic_hld_invalid_ansi_word_include_invalid_ansi_str00);
    tolua_function(tolua_S,"is_valid_ansi_str",tolua_logic_hld_invalid_ansi_word_is_valid_ansi_str00);
    tolua_function(tolua_S,"replace_invalid_ansi_str",tolua_logic_hld_invalid_ansi_word_replace_invalid_ansi_str00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_join_team_way_by_team_guid",faith::e_join_team_way_by_team_guid);
   tolua_constant(tolua_S,"e_join_team_way_by_captain_guid",faith::e_join_team_way_by_captain_guid);
   tolua_constant(tolua_S,"e_join_team_way_max",faith::e_join_team_way_max);
   tolua_constant(tolua_S,"ETeamMemberInfo_is_online",faith::ETeamMemberInfo_is_online);
   tolua_constant(tolua_S,"ETeamMemberInfo_line_id",faith::ETeamMemberInfo_line_id);
   tolua_constant(tolua_S,"ETeamMemberInfo_player_template_id",faith::ETeamMemberInfo_player_template_id);
   tolua_constant(tolua_S,"ETeamMemberInfo_cur_hp",faith::ETeamMemberInfo_cur_hp);
   tolua_constant(tolua_S,"ETeamMemberInfo_max_hp",faith::ETeamMemberInfo_max_hp);
   tolua_constant(tolua_S,"ETeamMemberInfo_level",faith::ETeamMemberInfo_level);
   tolua_constant(tolua_S,"ETeamMemberInfo_fighting_power",faith::ETeamMemberInfo_fighting_power);
   tolua_constant(tolua_S,"ETeamMemberInfo_cur_map",faith::ETeamMemberInfo_cur_map);
   tolua_constant(tolua_S,"ETeamMemberInfo_damage",faith::ETeamMemberInfo_damage);
   tolua_constant(tolua_S,"ETeamMemberInfo_PosX",faith::ETeamMemberInfo_PosX);
   tolua_constant(tolua_S,"ETeamMemberInfo_PosY",faith::ETeamMemberInfo_PosY);
   tolua_constant(tolua_S,"ETeamMemberInfo_PosZ",faith::ETeamMemberInfo_PosZ);
   tolua_constant(tolua_S,"ETeamMemberInfo_ready_state",faith::ETeamMemberInfo_ready_state);
   tolua_constant(tolua_S,"ETeamMemberInfo_follow_state",faith::ETeamMemberInfo_follow_state);
   tolua_constant(tolua_S,"ETeamMemberInfo_check_ret",faith::ETeamMemberInfo_check_ret);
   tolua_constant(tolua_S,"ETeamMemberInfo_one_stop_flags",faith::ETeamMemberInfo_one_stop_flags);
   tolua_constant(tolua_S,"ETeamMemberInfo_agora_id",faith::ETeamMemberInfo_agora_id);
   tolua_constant(tolua_S,"ETeamMemberInfo_max",faith::ETeamMemberInfo_max);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"s_team_member_info","faith::s_team_member_info","",tolua_collect_hld__s_team_member_info);
   #else
   tolua_cclass(tolua_S,"s_team_member_info","faith::s_team_member_info","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"s_team_member_info");
    tolua_variable(tolua_S,"role_guid",tolua_get_hld__s_team_member_info_role_guid,tolua_set_hld__s_team_member_info_role_guid);
    tolua_variable(tolua_S,"role_index",tolua_get_hld__s_team_member_info_role_index,tolua_set_hld__s_team_member_info_role_index);
    tolua_variable(tolua_S,"role_name",tolua_get_hld__s_team_member_info_role_name,tolua_set_hld__s_team_member_info_role_name);
    tolua_variable(tolua_S,"gs_value",tolua_get_hld__s_team_member_info_gs_value,tolua_set_hld__s_team_member_info_gs_value);
    tolua_array(tolua_S,"data_ary",tolua_get_logic_hld_s_team_member_info_data_ary,tolua_set_logic_hld_s_team_member_info_data_ary);
    tolua_function(tolua_S,"is_valid",tolua_logic_hld_s_team_member_info_is_valid00);
    tolua_function(tolua_S,"new",tolua_logic_hld_s_team_member_info_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_s_team_member_info_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_s_team_member_info_new00_local);
   tolua_endmodule(tolua_S);
   tolua_constant(tolua_S,"e_team_error_none",faith::e_team_error_none);
   tolua_constant(tolua_S,"e_team_error_team_full",faith::e_team_error_team_full);
   tolua_constant(tolua_S,"e_team_error_fighting_power_not_enough",faith::e_team_error_fighting_power_not_enough);
   tolua_constant(tolua_S,"e_team_error_level_not_enough",faith::e_team_error_level_not_enough);
   tolua_constant(tolua_S,"e_team_error_player_already_join_team",faith::e_team_error_player_already_join_team);
   tolua_constant(tolua_S,"e_team_error_player_already_join_other_team",faith::e_team_error_player_already_join_other_team);
   tolua_constant(tolua_S,"e_team_error_player_already_offline",faith::e_team_error_player_already_offline);
   tolua_constant(tolua_S,"e_team_error_team_already_dissolve",faith::e_team_error_team_already_dissolve);
   tolua_constant(tolua_S,"e_team_error_target_player_not_in_team",faith::e_team_error_target_player_not_in_team);
   tolua_constant(tolua_S,"e_team_error_target_player_is_not_captain",faith::e_team_error_target_player_is_not_captain);
   tolua_constant(tolua_S,"e_team_error_captain_change",faith::e_team_error_captain_change);
   tolua_constant(tolua_S,"e_team_error_create_raid_failed",faith::e_team_error_create_raid_failed);
   tolua_constant(tolua_S,"e_team_error_invalid_raid_instance",faith::e_team_error_invalid_raid_instance);
   tolua_constant(tolua_S,"e_team_error_member_change_during_transfer",faith::e_team_error_member_change_during_transfer);
   tolua_constant(tolua_S,"e_team_error_raid_condition_less",faith::e_team_error_raid_condition_less);
   tolua_constant(tolua_S,"e_team_error_cur_map_cant_ready",faith::e_team_error_cur_map_cant_ready);
   tolua_constant(tolua_S,"e_team_error_team_member_is_full",faith::e_team_error_team_member_is_full);
   tolua_constant(tolua_S,"e_team_error_raidmulti_ingame",faith::e_team_error_raidmulti_ingame);
   tolua_constant(tolua_S,"e_team_error_max",faith::e_team_error_max);
   tolua_constant(tolua_S,"e_player_match_team_none",faith::e_player_match_team_none);
   tolua_constant(tolua_S,"e_player_match_team_waiting",faith::e_player_match_team_waiting);
   tolua_constant(tolua_S,"e_player_match_team_succeed",faith::e_player_match_team_succeed);
   tolua_constant(tolua_S,"e_team_common_check_invoker_auto_match_team",faith::e_team_common_check_invoker_auto_match_team);
   tolua_constant(tolua_S,"e_team_common_check_invoker_mem_ready",faith::e_team_common_check_invoker_mem_ready);
   tolua_constant(tolua_S,"e_team_common_check_invoker_up_mem_check_ret",faith::e_team_common_check_invoker_up_mem_check_ret);
   tolua_constant(tolua_S,"e_team_req_mate_type_friend",faith::e_team_req_mate_type_friend);
   tolua_constant(tolua_S,"e_team_req_mate_type_legion",faith::e_team_req_mate_type_legion);
   tolua_constant(tolua_S,"e_team_mem_ready_state_none",faith::e_team_mem_ready_state_none);
   tolua_constant(tolua_S,"e_team_mem_ready_state_ready",faith::e_team_mem_ready_state_ready);
   tolua_constant(tolua_S,"e_team_mem_ready_state_waiting_ready",faith::e_team_mem_ready_state_waiting_ready);
   tolua_constant(tolua_S,"e_team_one_stop_aim_ok",faith::e_team_one_stop_aim_ok);
   tolua_constant(tolua_S,"e_team_one_stop_aim_lack_of_times",faith::e_team_one_stop_aim_lack_of_times);
   tolua_constant(tolua_S,"e_team_one_stop_aim_locked",faith::e_team_one_stop_aim_locked);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"team_ws","faith::team_ws","",NULL);
   tolua_beginmodule(tolua_S,"team_ws");
    tolua_function(tolua_S,"del_member",tolua_logic_hld_team_ws_del_member00);
    tolua_function(tolua_S,"get_member",tolua_logic_hld_team_ws_get_member00);
    tolua_function(tolua_S,"get_member_num",tolua_logic_hld_team_ws_get_member_num00);
    tolua_function(tolua_S,"set_captain",tolua_logic_hld_team_ws_set_captain00);
    tolua_function(tolua_S,"get_captain_guid",tolua_logic_hld_team_ws_get_captain_guid00);
    tolua_function(tolua_S,"transfer_all_member_to_scene",tolua_logic_hld_team_ws_transfer_all_member_to_scene00);
    tolua_function(tolua_S,"get_team_state",tolua_logic_hld_team_ws_get_team_state00);
    tolua_function(tolua_S,"req_add_member_to_cs",tolua_logic_hld_team_ws_req_add_member_to_cs00);
    tolua_function(tolua_S,"join_voice_channel",tolua_logic_hld_team_ws_join_voice_channel00);
    tolua_function(tolua_S,"leave_voie_channel",tolua_logic_hld_team_ws_leave_voie_channel00);
    tolua_function(tolua_S,"get_member_list_name",tolua_logic_hld_team_ws_get_member_list_name00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"team_ws_mgr","faith::team_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"team_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_team_ws_mgr_get_instance00);
    tolua_function(tolua_S,"get_team",tolua_logic_hld_team_ws_mgr_get_team00);
    tolua_function(tolua_S,"get_unit_team",tolua_logic_hld_team_ws_mgr_get_unit_team00);
    tolua_function(tolua_S,"send_team_error_to_session",tolua_logic_hld_team_ws_mgr_send_team_error_to_session00);
    tolua_function(tolua_S,"player_stop_auto_match_team",tolua_logic_hld_team_ws_mgr_player_stop_auto_match_team00);
    tolua_function(tolua_S,"is_team_type_valid",tolua_logic_hld_team_ws_mgr_is_team_type_valid00);
    tolua_function(tolua_S,"req_condition_check_to_cs",tolua_logic_hld_team_ws_mgr_req_condition_check_to_cs00);
    tolua_function(tolua_S,"req_no_team_mate_list",tolua_logic_hld_team_ws_mgr_req_no_team_mate_list00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"cs_map_system","faith::cs_map_system","",NULL);
   tolua_beginmodule(tolua_S,"cs_map_system");
    tolua_function(tolua_S,"get_map_template_id",tolua_logic_hld_cs_map_system_get_map_template_id00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"cs_map_mgr_system","faith::cs_map_mgr_system","",NULL);
   tolua_beginmodule(tolua_S,"cs_map_mgr_system");
    tolua_function(tolua_S,"send_map_error_to_client",tolua_logic_hld_cs_map_mgr_system_send_map_error_to_client00);
    tolua_function(tolua_S,"transfer_to_map",tolua_logic_hld_cs_map_mgr_system_transfer_to_map00);
    tolua_function(tolua_S,"send_big_player",tolua_logic_hld_cs_map_mgr_system_send_big_player00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"pk_king_mgr","faith::pk_king_mgr","",NULL);
   tolua_beginmodule(tolua_S,"pk_king_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_pk_king_mgr_get_instance00);
    tolua_function(tolua_S,"is_can_join",tolua_logic_hld_pk_king_mgr_is_can_join00);
    tolua_function(tolua_S,"is_in_ready_time",tolua_logic_hld_pk_king_mgr_is_in_ready_time00);
    tolua_function(tolua_S,"is_in_notice_time",tolua_logic_hld_pk_king_mgr_is_in_notice_time00);
    tolua_function(tolua_S,"get_valid_pkking_map",tolua_logic_hld_pk_king_mgr_get_valid_pkking_map00);
    tolua_function(tolua_S,"send_player_num_in_map_to_client",tolua_logic_hld_pk_king_mgr_send_player_num_in_map_to_client00);
    tolua_function(tolua_S,"break_line_login_msg",tolua_logic_hld_pk_king_mgr_break_line_login_msg00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_map_info_player_num",faith::e_map_info_player_num);
   tolua_constant(tolua_S,"e_map_info_boss_cur_hp",faith::e_map_info_boss_cur_hp);
   tolua_constant(tolua_S,"e_map_info_boss_max_hp",faith::e_map_info_boss_max_hp);
   tolua_constant(tolua_S,"e_map_open_new",faith::e_map_open_new);
   tolua_constant(tolua_S,"e_map_open_old",faith::e_map_open_old);
   tolua_constant(tolua_S,"e_map_close_close",faith::e_map_close_close);
   tolua_constant(tolua_S,"e_map_close_hold",faith::e_map_close_hold);
   tolua_constant(tolua_S,"e_map_time_mode_none",faith::e_map_time_mode_none);
   tolua_constant(tolua_S,"e_map_time_mode_timeout_success",faith::e_map_time_mode_timeout_success);
   tolua_constant(tolua_S,"e_map_time_mode_timeout_fail",faith::e_map_time_mode_timeout_fail);
   tolua_constant(tolua_S,"e_map_time_mode_timeout_transfer",faith::e_map_time_mode_timeout_transfer);
   tolua_constant(tolua_S,"e_map_time_mode_max",faith::e_map_time_mode_max);
   tolua_constant(tolua_S,"e_map_war_result_lost",faith::e_map_war_result_lost);
   tolua_constant(tolua_S,"e_map_war_result_peace",faith::e_map_war_result_peace);
   tolua_constant(tolua_S,"e_map_war_result_win",faith::e_map_war_result_win);
   tolua_constant(tolua_S,"e_mopping_up_result_none",faith::e_mopping_up_result_none);
   tolua_constant(tolua_S,"e_mopping_up_result_finish",faith::e_mopping_up_result_finish);
   tolua_constant(tolua_S,"e_mopping_up_result_no_enter_count",faith::e_mopping_up_result_no_enter_count);
   tolua_constant(tolua_S,"e_mopping_up_result_max",faith::e_mopping_up_result_max);
   tolua_constant(tolua_S,"e_consume_enter_count_cond_none",faith::e_consume_enter_count_cond_none);
   tolua_constant(tolua_S,"e_consume_enter_count_cond_enter",faith::e_consume_enter_count_cond_enter);
   tolua_constant(tolua_S,"e_consume_enter_count_cond_win",faith::e_consume_enter_count_cond_win);
   tolua_constant(tolua_S,"e_map_win_cond_none",faith::e_map_win_cond_none);
   tolua_constant(tolua_S,"e_map_win_cond_kill_boss",faith::e_map_win_cond_kill_boss);
   tolua_constant(tolua_S,"e_map_win_cond_clear_all_wave",faith::e_map_win_cond_clear_all_wave);
   tolua_constant(tolua_S,"e_map_win_cond_time_out",faith::e_map_win_cond_time_out);
   tolua_constant(tolua_S,"e_map_win_cond_max",faith::e_map_win_cond_max);
   tolua_constant(tolua_S,"e_map_settle_type_finish_time",faith::e_map_settle_type_finish_time);
   tolua_constant(tolua_S,"e_map_settle_type_kill_boss_num",faith::e_map_settle_type_kill_boss_num);
   tolua_constant(tolua_S,"e_map_settle_type_protect_npc_hp",faith::e_map_settle_type_protect_npc_hp);
   tolua_constant(tolua_S,"e_map_settle_type_max",faith::e_map_settle_type_max);
   tolua_constant(tolua_S,"e_map_result_dialog_type_none",faith::e_map_result_dialog_type_none);
   tolua_constant(tolua_S,"e_map_result_dialog_type_lucky_draw",faith::e_map_result_dialog_type_lucky_draw);
   tolua_constant(tolua_S,"e_map_result_dialog_type_lucky_draw_and_fixed_award",faith::e_map_result_dialog_type_lucky_draw_and_fixed_award);
   tolua_constant(tolua_S,"e_map_result_dialog_type_demons_tower",faith::e_map_result_dialog_type_demons_tower);
   tolua_constant(tolua_S,"e_map_result_dialog_type_legion_activity",faith::e_map_result_dialog_type_legion_activity);
   tolua_constant(tolua_S,"e_map_result_dialog_type_score_and_rank_award",faith::e_map_result_dialog_type_score_and_rank_award);
   tolua_constant(tolua_S,"e_map_result_dialog_type_city_war_award",faith::e_map_result_dialog_type_city_war_award);
   tolua_constant(tolua_S,"e_map_result_dialog_type_arena",faith::e_map_result_dialog_type_arena);
   tolua_constant(tolua_S,"e_map_result_dialog_type_exp_raid",faith::e_map_result_dialog_type_exp_raid);
   tolua_constant(tolua_S,"e_map_result_dialog_type_pk_king",faith::e_map_result_dialog_type_pk_king);
   tolua_constant(tolua_S,"e_map_result_dialog_type_max",faith::e_map_result_dialog_type_max);
   tolua_constant(tolua_S,"e_map_result_dialog_pk_king_sub_type_death",faith::e_map_result_dialog_pk_king_sub_type_death);
   tolua_constant(tolua_S,"e_map_result_dialog_pk_king_sub_type_No_death",faith::e_map_result_dialog_pk_king_sub_type_No_death);
   tolua_constant(tolua_S,"e_map_lua_type_init",faith::e_map_lua_type_init);
   tolua_constant(tolua_S,"e_map_lua_type_tick",faith::e_map_lua_type_tick);
   tolua_constant(tolua_S,"e_map_lua_type_end",faith::e_map_lua_type_end);
   tolua_constant(tolua_S,"e_map_lua_type_unit_in",faith::e_map_lua_type_unit_in);
   tolua_constant(tolua_S,"e_map_lua_type_unit_out",faith::e_map_lua_type_unit_out);
   tolua_constant(tolua_S,"e_map_lua_type_unit_dead",faith::e_map_lua_type_unit_dead);
   tolua_constant(tolua_S,"e_double_line_map_template_id",faith::e_double_line_map_template_id);
   tolua_constant(tolua_S,"e_double_line_open_time_hour",faith::e_double_line_open_time_hour);
   tolua_constant(tolua_S,"e_double_line_open_time_min",faith::e_double_line_open_time_min);
   tolua_constant(tolua_S,"e_double_line_open_time_seconds",faith::e_double_line_open_time_seconds);
   tolua_constant(tolua_S,"e_double_line_end_time_hour",faith::e_double_line_end_time_hour);
   tolua_constant(tolua_S,"e_double_line_end_time_min",faith::e_double_line_end_time_min);
   tolua_constant(tolua_S,"e_double_line_end_time_seconds",faith::e_double_line_end_time_seconds);
   tolua_constant(tolua_S,"e_double_line_buff_id",faith::e_double_line_buff_id);
   tolua_constant(tolua_S,"e_double_line_notice_id",faith::e_double_line_notice_id);
   tolua_constant(tolua_S,"e_double_line_max",faith::e_double_line_max);
   tolua_constant(tolua_S,"e_map_target_image_element_red",faith::e_map_target_image_element_red);
   tolua_constant(tolua_S,"e_map_target_image_element_blue",faith::e_map_target_image_element_blue);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"cross_server_pk_ws_mgr","faith::cross_server_pk_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_pk_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_pk_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_can_join_with_cross",tolua_logic_hld_cross_server_pk_ws_mgr_is_can_join_with_cross00);
    tolua_function(tolua_S,"send_to_gate_transfer_map",tolua_logic_hld_cross_server_pk_ws_mgr_send_to_gate_transfer_map00);
    tolua_function(tolua_S,"get_pk_king_map_guid",tolua_logic_hld_cross_server_pk_ws_mgr_get_pk_king_map_guid00);
    tolua_function(tolua_S,"get_map_temp_id_with_class_type",tolua_logic_hld_cross_server_pk_ws_mgr_get_map_temp_id_with_class_type00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"crystal_fairyland_mgr","faith::crystal_fairyland_mgr","",NULL);
   tolua_beginmodule(tolua_S,"crystal_fairyland_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_crystal_fairyland_mgr_get_instance00);
    tolua_function(tolua_S,"is_activity_open",tolua_logic_hld_crystal_fairyland_mgr_is_activity_open00);
    tolua_function(tolua_S,"is_can_join",tolua_logic_hld_crystal_fairyland_mgr_is_can_join00);
    tolua_function(tolua_S,"get_valid_shui_jing_map",tolua_logic_hld_crystal_fairyland_mgr_get_valid_shui_jing_map00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_broken_sky_not_start",faith::e_broken_sky_not_start);
   tolua_constant(tolua_S,"e_broken_sky_in_join",faith::e_broken_sky_in_join);
   tolua_constant(tolua_S,"e_broken_sky_in_game",faith::e_broken_sky_in_game);
   tolua_constant(tolua_S,"e_broken_sky_game_end",faith::e_broken_sky_game_end);
   tolua_cclass(tolua_S,"broken_sky_mgr","faith::broken_sky_mgr","",NULL);
   tolua_beginmodule(tolua_S,"broken_sky_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_broken_sky_mgr_get_instance00);
    tolua_function(tolua_S,"is_can_join",tolua_logic_hld_broken_sky_mgr_is_can_join00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"world_boss_ws_mgr","faith::world_boss_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"world_boss_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_world_boss_ws_mgr_get_instance00);
    tolua_function(tolua_S,"req_frist_kill_world_boss_prize",tolua_logic_hld_world_boss_ws_mgr_req_frist_kill_world_boss_prize00);
    tolua_function(tolua_S,"send_damage_list",tolua_logic_hld_world_boss_ws_mgr_send_damage_list00);
    tolua_function(tolua_S,"send_strategy_world_boss_info",tolua_logic_hld_world_boss_ws_mgr_send_strategy_world_boss_info00);
    tolua_function(tolua_S,"send_world_boss_info",tolua_logic_hld_world_boss_ws_mgr_send_world_boss_info00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"cross_server_harry_ws_mgr","faith::cross_server_harry_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_harry_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_harry_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_can_join_harry_map",tolua_logic_hld_cross_server_harry_ws_mgr_is_can_join_harry_map00);
    tolua_function(tolua_S,"get_valid_harry_map",tolua_logic_hld_cross_server_harry_ws_mgr_get_valid_harry_map00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"web_client","faith::web_client","",NULL);
   tolua_beginmodule(tolua_S,"web_client");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_web_client_get_instance00);
    tolua_function(tolua_S,"send_activate_code",tolua_logic_hld_web_client_send_activate_code00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"relation_mgr","faith::relation_mgr","",NULL);
   tolua_beginmodule(tolua_S,"relation_mgr");
    tolua_function(tolua_S,"del_relation",tolua_logic_hld_relation_mgr_del_relation00);
    tolua_function(tolua_S,"find_player_to_add_friend",tolua_logic_hld_relation_mgr_find_player_to_add_friend00);
    tolua_function(tolua_S,"find_player_to_add_friend_by_pattern",tolua_logic_hld_relation_mgr_find_player_to_add_friend_by_pattern00);
    tolua_function(tolua_S,"add_relation",tolua_logic_hld_relation_mgr_add_relation00);
    tolua_function(tolua_S,"del_relation_to_dp",tolua_logic_hld_relation_mgr_del_relation_to_dp00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"marry_mgr_ws","faith::marry_mgr_ws","",NULL);
   tolua_beginmodule(tolua_S,"marry_mgr_ws");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_marry_mgr_ws_get_instance00);
    tolua_function(tolua_S,"c2ws_get_married_reward",tolua_logic_hld_marry_mgr_ws_c2ws_get_married_reward00);
    tolua_function(tolua_S,"c2ws_get_marry_mission",tolua_logic_hld_marry_mgr_ws_c2ws_get_marry_mission00);
    tolua_function(tolua_S,"req_check_marry_with_sb",tolua_logic_hld_marry_mgr_ws_req_check_marry_with_sb00);
    tolua_function(tolua_S,"c2ws_reply_marry_choice",tolua_logic_hld_marry_mgr_ws_c2ws_reply_marry_choice00);
    tolua_function(tolua_S,"c2ws_req_check_divorce",tolua_logic_hld_marry_mgr_ws_c2ws_req_check_divorce00);
    tolua_function(tolua_S,"c2ws_reply_divorce_choice",tolua_logic_hld_marry_mgr_ws_c2ws_reply_divorce_choice00);
    tolua_function(tolua_S,"req_bless_one_couple",tolua_logic_hld_marry_mgr_ws_req_bless_one_couple00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"ranking_mgr_ws","faith::ranking_mgr_ws","",NULL);
   tolua_beginmodule(tolua_S,"ranking_mgr_ws");
    tolua_function(tolua_S,"c2ws_get_ranking_list_func",tolua_logic_hld_ranking_mgr_ws_c2ws_get_ranking_list_func00);
    tolua_function(tolua_S,"c2ws_get_my_rank_func",tolua_logic_hld_ranking_mgr_ws_c2ws_get_my_rank_func00);
    tolua_function(tolua_S,"c2ws_get_player_ranking_info_func",tolua_logic_hld_ranking_mgr_ws_c2ws_get_player_ranking_info_func00);
    tolua_function(tolua_S,"send_service_rank_first_player_to_client_by_type",tolua_logic_hld_ranking_mgr_ws_send_service_rank_first_player_to_client_by_type00);
    tolua_function(tolua_S,"get_oracle_trial_rank_info",tolua_logic_hld_ranking_mgr_ws_get_oracle_trial_rank_info00);
    tolua_function(tolua_S,"sync_player_arena_lua",tolua_logic_hld_ranking_mgr_ws_sync_player_arena_lua00);
    tolua_function(tolua_S,"get_srv_avg_level",tolua_logic_hld_ranking_mgr_ws_get_srv_avg_level00);
    tolua_function(tolua_S,"get_ranking_player_guid",tolua_logic_hld_ranking_mgr_ws_get_ranking_player_guid00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"cloud_shop_mgr","faith::cloud_shop_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cloud_shop_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cloud_shop_mgr_get_instance00);
    tolua_function(tolua_S,"handler_client_request",tolua_logic_hld_cloud_shop_mgr_handler_client_request00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"gain_treasure_ws_mgr","faith::gain_treasure_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"gain_treasure_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_gain_treasure_ws_mgr_get_instance00);
    tolua_function(tolua_S,"req_kill_record_end",tolua_logic_hld_gain_treasure_ws_mgr_req_kill_record_end00);
    tolua_function(tolua_S,"req_gain_item_record",tolua_logic_hld_gain_treasure_ws_mgr_req_gain_item_record00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"time_limit_activity_ws_mgr","faith::time_limit_activity_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"time_limit_activity_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_time_limit_activity_ws_mgr_get_instance00);
    tolua_function(tolua_S,"get_rank_info",tolua_logic_hld_time_limit_activity_ws_mgr_get_rank_info00);
    tolua_function(tolua_S,"get_activity_common_data_to_gate",tolua_logic_hld_time_limit_activity_ws_mgr_get_activity_common_data_to_gate00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"rename_mgr","faith::rename_mgr","",NULL);
   tolua_beginmodule(tolua_S,"rename_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_rename_mgr_get_instance00);
    tolua_function(tolua_S,"change_player_name",tolua_logic_hld_rename_mgr_change_player_name00);
    tolua_function(tolua_S,"confirm_change_name",tolua_logic_hld_rename_mgr_confirm_change_name00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"cross_server_world_boss_ws_mgr","faith::cross_server_world_boss_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_world_boss_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_world_boss_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_in_cross_boss_map",tolua_logic_hld_cross_server_world_boss_ws_mgr_is_in_cross_boss_map00);
    tolua_function(tolua_S,"is_can_join_with_cross",tolua_logic_hld_cross_server_world_boss_ws_mgr_is_can_join_with_cross00);
    tolua_function(tolua_S,"get_world_boss_map_guid",tolua_logic_hld_cross_server_world_boss_ws_mgr_get_world_boss_map_guid00);
    tolua_function(tolua_S,"get_cross_server_world_boss_map_temp_id",tolua_logic_hld_cross_server_world_boss_ws_mgr_get_cross_server_world_boss_map_temp_id00);
    tolua_function(tolua_S,"send_kill_boss_show",tolua_logic_hld_cross_server_world_boss_ws_mgr_send_kill_boss_show00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"assist_fight_mgr","faith::assist_fight_mgr","",NULL);
   tolua_beginmodule(tolua_S,"assist_fight_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_assist_fight_mgr_get_instance00);
    tolua_function(tolua_S,"sync_assist_fight_info",tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info00);
    tolua_function(tolua_S,"sync_assist_fight_info",tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info01);
    tolua_function(tolua_S,"req_assist_fight",tolua_logic_hld_assist_fight_mgr_req_assist_fight00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"appearance_info_mgr","faith::appearance_info_mgr","",NULL);
   tolua_beginmodule(tolua_S,"appearance_info_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_appearance_info_mgr_get_instance00);
    tolua_function(tolua_S,"req_save_appearance_info_to_id",tolua_logic_hld_appearance_info_mgr_req_save_appearance_info_to_id00);
    tolua_function(tolua_S,"req_load_appearance_info_by_id",tolua_logic_hld_appearance_info_mgr_req_load_appearance_info_by_id00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"c_element_war_role_info","faith::c_element_war_role_info","",NULL);
   tolua_beginmodule(tolua_S,"c_element_war_role_info");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"element_war_ws_mgr","faith::element_war_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"element_war_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_element_war_ws_mgr_get_instance00);
    tolua_function(tolua_S,"send_req_element_war_role_info",tolua_logic_hld_element_war_ws_mgr_send_req_element_war_role_info00);
    tolua_function(tolua_S,"send_team_sign_up",tolua_logic_hld_element_war_ws_mgr_send_team_sign_up00);
    tolua_function(tolua_S,"send_single_sign_up",tolua_logic_hld_element_war_ws_mgr_send_single_sign_up00);
    tolua_function(tolua_S,"send_close_sign_up",tolua_logic_hld_element_war_ws_mgr_send_close_sign_up00);
    tolua_function(tolua_S,"break_line_login_msg",tolua_logic_hld_element_war_ws_mgr_break_line_login_msg00);
    tolua_function(tolua_S,"get_mission_reward",tolua_logic_hld_element_war_ws_mgr_get_mission_reward00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_need_server_cross_none",faith::e_need_server_cross_none);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross",faith::e_need_server_cross_begin_cross);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_pk",faith::e_need_server_cross_begin_cross_pk);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_activity",faith::e_need_server_cross_begin_cross_activity);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_harry",faith::e_need_server_cross_begin_cross_harry);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_city_war",faith::e_need_server_cross_begin_cross_city_war);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_legion_territory_war",faith::e_need_server_cross_begin_cross_legion_territory_war);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_boss",faith::e_need_server_cross_begin_cross_boss);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_ladder",faith::e_need_server_cross_begin_cross_ladder);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_cloud",faith::e_need_server_cross_begin_cross_cloud);
   tolua_constant(tolua_S,"e_need_server_element_war",faith::e_need_server_element_war);
   tolua_constant(tolua_S,"e_need_server_cross_max",faith::e_need_server_cross_max);
   tolua_cclass(tolua_S,"world_server","faith::world_server","",NULL);
   tolua_beginmodule(tolua_S,"world_server");
    tolua_function(tolua_S,"get_need_begin_cross_gm_common",tolua_logic_hld_world_server_get_need_begin_cross_gm_common00);
    tolua_function(tolua_S,"is_sky_island_server",tolua_logic_hld_world_server_is_sky_island_server00);
    tolua_function(tolua_S,"get_server_id",tolua_logic_hld_world_server_get_server_id00);
    tolua_function(tolua_S,"get_cross_id",tolua_logic_hld_world_server_get_cross_id00);
    tolua_function(tolua_S,"set_server_act_hidden",tolua_logic_hld_world_server_set_server_act_hidden00);
    tolua_function(tolua_S,"set_ws_loading_flag",tolua_logic_hld_world_server_set_ws_loading_flag00);
    tolua_function(tolua_S,"send_to_gate_get_msg",tolua_logic_hld_world_server_send_to_gate_get_msg00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"client_session_mgr","faith::client_session_mgr","",NULL);
   tolua_beginmodule(tolua_S,"client_session_mgr");
    tolua_function(tolua_S,"send_message_to_all_client_data_lua",tolua_logic_hld_client_session_mgr_send_message_to_all_client_data_lua00);
    tolua_function(tolua_S,"send_message_to_map_lua",tolua_logic_hld_client_session_mgr_send_message_to_map_lua00);
    tolua_function(tolua_S,"get_session_array_used",tolua_logic_hld_client_session_mgr_get_session_array_used00);
    tolua_function(tolua_S,"get_session_by_use_index",tolua_logic_hld_client_session_mgr_get_session_by_use_index00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"s_item_info","faith::s_item_info","",NULL);
   tolua_beginmodule(tolua_S,"s_item_info");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"event_ws_mgr","faith::event_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"event_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_event_ws_mgr_get_instance00);
    tolua_function(tolua_S,"send_notice_to_all",tolua_logic_hld_event_ws_mgr_send_notice_to_all00);
    tolua_function(tolua_S,"send_mail_to_player_by_system",tolua_logic_hld_event_ws_mgr_send_mail_to_player_by_system00);
    tolua_function(tolua_S,"send_notice_with_param",tolua_logic_hld_event_ws_mgr_send_notice_with_param00);
    tolua_function(tolua_S,"get_free_item",tolua_logic_hld_event_ws_mgr_get_free_item00);
    tolua_function(tolua_S,"get_free_list",tolua_logic_hld_event_ws_mgr_get_free_list00);
    tolua_function(tolua_S,"add_free_item",tolua_logic_hld_event_ws_mgr_add_free_item00);
    tolua_function(tolua_S,"get_free_param",tolua_logic_hld_event_ws_mgr_get_free_param00);
    tolua_function(tolua_S,"add_free_param",tolua_logic_hld_event_ws_mgr_add_free_param00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"random_gen","faith::random_gen","",NULL);
   tolua_beginmodule(tolua_S,"random_gen");
    tolua_function(tolua_S,"get_random",tolua_logic_hld_random_gen_get_random00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"big_player_ws_mgr","faith::big_player_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"big_player_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_big_player_ws_mgr_get_instance00);
    tolua_function(tolua_S,"set_big_player_guid",tolua_logic_hld_big_player_ws_mgr_set_big_player_guid00);
    tolua_function(tolua_S,"get_big_player_type_with_ranking_type",tolua_logic_hld_big_player_ws_mgr_get_big_player_type_with_ranking_type00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"lua_cs2ws_send_red_package_process",tolua_logic_hld_lua_cs2ws_send_red_package_process00);
   tolua_function(tolua_S,"lua_cs2ws_get_red_package_process",tolua_logic_hld_lua_cs2ws_get_red_package_process00);
   tolua_function(tolua_S,"lua_cs2ws_get_arena_rank",tolua_logic_hld_lua_cs2ws_get_arena_rank00);
   tolua_function(tolua_S,"lua_cs2ws_req_challenge",tolua_logic_hld_lua_cs2ws_req_challenge00);
   tolua_function(tolua_S,"lua_cs2ws_req_get_choose_list",tolua_logic_hld_lua_cs2ws_req_get_choose_list00);
   tolua_function(tolua_S,"lua_cs2ws_req_get_first_three",tolua_logic_hld_lua_cs2ws_req_get_first_three00);
   tolua_function(tolua_S,"lua_cs2ws_chellenge_over_req",tolua_logic_hld_lua_cs2ws_chellenge_over_req00);
   tolua_function(tolua_S,"lua_cs2ws_arena_sync_gs_value_proc",tolua_logic_hld_lua_cs2ws_arena_sync_gs_value_proc00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"mail_ws_mgr","faith::mail_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"mail_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_mail_ws_mgr_get_instance00);
    tolua_function(tolua_S,"send_mail",tolua_logic_hld_mail_ws_mgr_send_mail00);
    tolua_function(tolua_S,"send_mail_system",tolua_logic_hld_mail_ws_mgr_send_mail_system00);
    tolua_function(tolua_S,"get_free_item_list",tolua_logic_hld_mail_ws_mgr_get_free_item_list00);
    tolua_function(tolua_S,"add_free_item",tolua_logic_hld_mail_ws_mgr_add_free_item00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"EMailInfo_IsNeedDelete",faith::EMailInfo_IsNeedDelete);
   tolua_constant(tolua_S,"EMailInfo_SendTime",faith::EMailInfo_SendTime);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid11",faith::EMailInfo_ItemGuid11);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid12",faith::EMailInfo_ItemGuid12);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid21",faith::EMailInfo_ItemGuid21);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid22",faith::EMailInfo_ItemGuid22);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid31",faith::EMailInfo_ItemGuid31);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid32",faith::EMailInfo_ItemGuid32);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid41",faith::EMailInfo_ItemGuid41);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid42",faith::EMailInfo_ItemGuid42);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid51",faith::EMailInfo_ItemGuid51);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid52",faith::EMailInfo_ItemGuid52);
   tolua_constant(tolua_S,"EMailInfo_MoneyTyp1",faith::EMailInfo_MoneyTyp1);
   tolua_constant(tolua_S,"EMailInfo_MoneyNum1",faith::EMailInfo_MoneyNum1);
   tolua_constant(tolua_S,"EMailInfo_MoneyTyp2",faith::EMailInfo_MoneyTyp2);
   tolua_constant(tolua_S,"EMailInfo_MoneyNum2",faith::EMailInfo_MoneyNum2);
   tolua_constant(tolua_S,"EMailInfo_ReadState",faith::EMailInfo_ReadState);
   tolua_constant(tolua_S,"EMailInfo_SenderGuid1",faith::EMailInfo_SenderGuid1);
   tolua_constant(tolua_S,"EMailInfo_SenderGuid2",faith::EMailInfo_SenderGuid2);
   tolua_constant(tolua_S,"EMailInfo_ServerId",faith::EMailInfo_ServerId);
   tolua_constant(tolua_S,"EMailInfo_max",faith::EMailInfo_max);
   tolua_constant(tolua_S,"e_mail_common_text_pk_king_title",faith::e_mail_common_text_pk_king_title);
   tolua_constant(tolua_S,"e_mail_common_text_pk_king_context",faith::e_mail_common_text_pk_king_context);
   tolua_constant(tolua_S,"e_mail_common_text_broken_sky_title",faith::e_mail_common_text_broken_sky_title);
   tolua_constant(tolua_S,"e_mail_common_text_broken_sky_context",faith::e_mail_common_text_broken_sky_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_title",faith::e_mail_common_text_city_war_title);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_win_context",faith::e_mail_common_text_city_war_win_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_lose_context",faith::e_mail_common_text_city_war_lose_context);
   tolua_constant(tolua_S,"e_mail_common_text_resource_back_title",faith::e_mail_common_text_resource_back_title);
   tolua_constant(tolua_S,"e_mail_common_text_resource_back_context",faith::e_mail_common_text_resource_back_context);
   tolua_constant(tolua_S,"e_mail_common_text_get_remaining_title",faith::e_mail_common_text_get_remaining_title);
   tolua_constant(tolua_S,"e_mail_common_text_get_remaining_context",faith::e_mail_common_text_get_remaining_context);
   tolua_constant(tolua_S,"e_mail_common_text_sender_system",faith::e_mail_common_text_sender_system);
   tolua_constant(tolua_S,"e_mail_common_text_blessing_title",faith::e_mail_common_text_blessing_title);
   tolua_constant(tolua_S,"e_mail_common_text_blessing_context",faith::e_mail_common_text_blessing_context);
   tolua_constant(tolua_S,"e_mail_common_text_auction_purchase_title",faith::e_mail_common_text_auction_purchase_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_purchase_content",faith::e_mail_common_text_auction_purchase_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_seller_title",faith::e_mail_common_text_auction_seller_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_seller_content",faith::e_mail_common_text_auction_seller_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_cancel_sell_title",faith::e_mail_common_text_auction_cancel_sell_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_cancel_sell_content",faith::e_mail_common_text_auction_cancel_sell_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_cancel_sell_bag_full_content",faith::e_mail_common_text_auction_cancel_sell_bag_full_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_reward_share_title",faith::e_mail_common_text_auction_reward_share_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_reward_share_kill_boss_content",faith::e_mail_common_text_auction_reward_share_kill_boss_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_reward_share_activity_content",faith::e_mail_common_text_auction_reward_share_activity_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_bid_failed_title",faith::e_mail_common_text_auction_bid_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_bid_failed_content",faith::e_mail_common_text_auction_bid_failed_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_sell_failed_title",faith::e_mail_common_text_auction_sell_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_sell_failed_content",faith::e_mail_common_text_auction_sell_failed_content);
   tolua_constant(tolua_S,"e_mail_common_text_force_divorce_title",faith::e_mail_common_text_force_divorce_title);
   tolua_constant(tolua_S,"e_mail_common_text_force_divorce_content",faith::e_mail_common_text_force_divorce_content);
   tolua_constant(tolua_S,"e_mail_common_text_legion_rank_rwd_title",faith::e_mail_common_text_legion_rank_rwd_title);
   tolua_constant(tolua_S,"e_mail_common_text_legion_rank_rwd_content",faith::e_mail_common_text_legion_rank_rwd_content);
   tolua_constant(tolua_S,"e_mail_common_text_active_code_gift_title",faith::e_mail_common_text_active_code_gift_title);
   tolua_constant(tolua_S,"e_mail_common_text_active_code_gift_context",faith::e_mail_common_text_active_code_gift_context);
   tolua_constant(tolua_S,"e_mail_common_text_worship_failed_title",faith::e_mail_common_text_worship_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_worship_failed_context",faith::e_mail_common_text_worship_failed_context);
   tolua_constant(tolua_S,"e_mail_common_text_join_legion_title",faith::e_mail_common_text_join_legion_title);
   tolua_constant(tolua_S,"e_mail_common_text_join_legion_context",faith::e_mail_common_text_join_legion_context);
   tolua_constant(tolua_S,"e_mail_common_text_daily_mission_bag_full_title",faith::e_mail_common_text_daily_mission_bag_full_title);
   tolua_constant(tolua_S,"e_mail_common_text_daily_mission_bag_full_context",faith::e_mail_common_text_daily_mission_bag_full_context);
   tolua_constant(tolua_S,"e_mail_common_text_against_mission_bag_full_title",faith::e_mail_common_text_against_mission_bag_full_title);
   tolua_constant(tolua_S,"e_mail_common_text_against_mission_bag_full_context",faith::e_mail_common_text_against_mission_bag_full_context);
   tolua_constant(tolua_S,"e_mail_common_text_active_recharge_package_title",faith::e_mail_common_text_active_recharge_package_title);
   tolua_constant(tolua_S,"e_mail_common_text_active_recharge_package_context",faith::e_mail_common_text_active_recharge_package_context);
   tolua_constant(tolua_S,"e_mail_common_text_jewel_send_mail_title",faith::e_mail_common_text_jewel_send_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_jewel_send_mail_content",faith::e_mail_common_text_jewel_send_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_arena_reward_mail_title",faith::e_mail_common_text_arena_reward_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_arena_reward_mail_content",faith::e_mail_common_text_arena_reward_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_announcement_title",faith::e_mail_common_text_set_legion_announcement_title);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_announcement_context",faith::e_mail_common_text_set_legion_announcement_context);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_job_title",faith::e_mail_common_text_set_legion_job_title);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_job_context",faith::e_mail_common_text_set_legion_job_context);
   tolua_constant(tolua_S,"e_mail_common_text_master_city_war_title",faith::e_mail_common_text_master_city_war_title);
   tolua_constant(tolua_S,"e_mail_common_text_master_city_war_context",faith::e_mail_common_text_master_city_war_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_bid_title",faith::e_mail_common_text_city_bid_title);
   tolua_constant(tolua_S,"e_mail_common_text_city_bid_context",faith::e_mail_common_text_city_bid_context);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_basic_award_title",faith::e_mail_common_text_cloud_shop_basic_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_basic_award_context",faith::e_mail_common_text_cloud_shop_basic_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_award_title",faith::e_mail_common_text_cloud_shop_big_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_award_context",faith::e_mail_common_text_cloud_shop_big_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_buy_failed_title",faith::e_mail_common_text_cloud_shop_big_buy_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_buy_failed_context",faith::e_mail_common_text_cloud_shop_big_buy_failed_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_recharge_title",faith::e_mail_common_text_time_limit_activity_rank_reward_total_recharge_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_recharge_context",faith::e_mail_common_text_time_limit_activity_rank_reward_total_recharge_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_cost_title",faith::e_mail_common_text_time_limit_activity_rank_reward_total_cost_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_cost_context",faith::e_mail_common_text_time_limit_activity_rank_reward_total_cost_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_treasure_title",faith::e_mail_common_text_time_limit_activity_rank_reward_treasure_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_treasure_context",faith::e_mail_common_text_time_limit_activity_rank_reward_treasure_context);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_legion_member_title",faith::e_mail_common_text_change_name_inform_legion_member_title);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_legion_member_content",faith::e_mail_common_text_change_name_inform_legion_member_content);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_friend_title",faith::e_mail_common_text_change_name_inform_friend_title);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_friend_content",faith::e_mail_common_text_change_name_inform_friend_content);
   tolua_constant(tolua_S,"e_mail_common_text_service_rank_end_time_reward_title",faith::e_mail_common_text_service_rank_end_time_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_service_rank_end_time_reward_context",faith::e_mail_common_text_service_rank_end_time_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_out_16_award_title",faith::e_mail_common_text_cross_pk_out_16_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_out_16_award_context",faith::e_mail_common_text_cross_pk_out_16_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_16_award_title",faith::e_mail_common_text_cross_pk_16_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_16_award_context",faith::e_mail_common_text_cross_pk_16_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_8_award_title",faith::e_mail_common_text_cross_pk_8_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_8_award_context",faith::e_mail_common_text_cross_pk_8_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_4_award_title",faith::e_mail_common_text_cross_pk_4_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_4_award_context",faith::e_mail_common_text_cross_pk_4_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_2_award_title",faith::e_mail_common_text_cross_pk_2_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_2_award_context",faith::e_mail_common_text_cross_pk_2_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_1_award_title",faith::e_mail_common_text_cross_pk_1_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_1_award_context",faith::e_mail_common_text_cross_pk_1_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_player_award_title",faith::e_mail_common_text_cross_harry_player_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_player_award_context",faith::e_mail_common_text_cross_harry_player_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_server_award_title",faith::e_mail_common_text_cross_harry_server_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_server_award_context",faith::e_mail_common_text_cross_harry_server_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_change_legion_name_inform_legion_member_title",faith::e_mail_common_text_change_legion_name_inform_legion_member_title);
   tolua_constant(tolua_S,"e_mail_common_text_change_legion_name_inform_legion_member_content",faith::e_mail_common_text_change_legion_name_inform_legion_member_content);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_title",faith::e_mail_common_text_cross_city_rank_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_context",faith::e_mail_common_text_cross_city_rank_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_title_chief",faith::e_mail_common_text_cross_city_rank_award_title_chief);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_context_chief",faith::e_mail_common_text_cross_city_rank_award_context_chief);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_refund_title",faith::e_mail_common_text_cross_ladder_refund_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_refund_context",faith::e_mail_common_text_cross_ladder_refund_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_reward_title",faith::e_mail_common_text_cross_ladder_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_reward_context",faith::e_mail_common_text_cross_ladder_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_season_reward_title",faith::e_mail_common_text_cross_ladder_season_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_season_reward_context",faith::e_mail_common_text_cross_ladder_season_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_rank_reward_title",faith::e_mail_common_text_cross_ladder_rank_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_rank_reward_context",faith::e_mail_common_text_cross_ladder_rank_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_skill_bag_full_send_mail_title",faith::e_mail_common_text_skill_bag_full_send_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_skill_bag_full_send_mail_content",faith::e_mail_common_text_skill_bag_full_send_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_war_send_mail_title",faith::e_mail_common_text_cross_city_war_send_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_war_send_mail_content",faith::e_mail_common_text_cross_city_war_send_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_cross_server_terrritory_war_title",faith::e_mail_common_text_cross_server_terrritory_war_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_server_terrritory_war_content",faith::e_mail_common_text_cross_server_terrritory_war_content);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_gs_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_gs_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_gs_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_gs_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_mount_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_mount_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_mount_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_mount_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_wing_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_wing_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_wing_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_wing_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_equip_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_equip_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_equip_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_equip_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_spirit_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_spirit_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_spirit_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_spirit_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_title",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_context",faith::e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_couple_bless_title",faith::e_mail_common_text_couple_bless_title);
   tolua_constant(tolua_S,"e_mail_common_text_couple_bless_context",faith::e_mail_common_text_couple_bless_context);
   tolua_constant(tolua_S,"e_mail_common_text_couple_marry_title",faith::e_mail_common_text_couple_marry_title);
   tolua_constant(tolua_S,"e_mail_common_text_couple_marry_context",faith::e_mail_common_text_couple_marry_context);
   tolua_constant(tolua_S,"e_mail_common_text_couple_divorce_title",faith::e_mail_common_text_couple_divorce_title);
   tolua_constant(tolua_S,"e_mail_common_text_couple_divorce_context",faith::e_mail_common_text_couple_divorce_context);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_title",faith::e_mail_common_text_oracle_trial_funben_over_title);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_content_one",faith::e_mail_common_text_oracle_trial_funben_over_content_one);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_content_two",faith::e_mail_common_text_oracle_trial_funben_over_content_two);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_content_three",faith::e_mail_common_text_oracle_trial_funben_over_content_three);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_upgrade_title",faith::e_mail_common_text_cross_ladder_upgrade_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_upgrade_context",faith::e_mail_common_text_cross_ladder_upgrade_context);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_begin_title",faith::e_mail_common_text_overload_city_war_begin_title);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_begin_context",faith::e_mail_common_text_overload_city_war_begin_context);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_end_title",faith::e_mail_common_text_overload_city_war_end_title);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_end_context",faith::e_mail_common_text_overload_city_war_end_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_begin_title",faith::e_mail_common_text_cross_overload_city_war_begin_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_begin_context",faith::e_mail_common_text_cross_overload_city_war_begin_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_end_title",faith::e_mail_common_text_cross_overload_city_war_end_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_end_context",faith::e_mail_common_text_cross_overload_city_war_end_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_rank_award_title_chief",faith::e_mail_common_text_city_rank_award_title_chief);
   tolua_constant(tolua_S,"e_mail_common_text_city_rank_award_context_chief",faith::e_mail_common_text_city_rank_award_context_chief);
   tolua_constant(tolua_S,"e_mail_common_overload_city_war_end_title",faith::e_mail_common_overload_city_war_end_title);
   tolua_constant(tolua_S,"e_mail_common_overload_city_war_end_context",faith::e_mail_common_overload_city_war_end_context);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_begin_title",faith::e_mail_common_cross_ladder_season_begin_title);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_begin_context",faith::e_mail_common_cross_ladder_season_begin_context);
   tolua_constant(tolua_S,"e_mail_common_text_auction_time_out_title",faith::e_mail_common_text_auction_time_out_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_time_out_content",faith::e_mail_common_text_auction_time_out_content);
   tolua_constant(tolua_S,"e_mail_common_text_help_reward_title",faith::e_mail_common_text_help_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_help_reward_content",faith::e_mail_common_text_help_reward_content);
   tolua_constant(tolua_S,"e_mail_common_text_helper_reward_title",faith::e_mail_common_text_helper_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_helper_reward_content",faith::e_mail_common_text_helper_reward_content);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_win_reward_title",faith::e_mail_common_text_city_war_win_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_win_reward_content",faith::e_mail_common_text_city_war_win_reward_content);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_ranking_reward_title",faith::e_mail_common_cross_ladder_season_ranking_reward_title);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_ranking_reward_context",faith::e_mail_common_cross_ladder_season_ranking_reward_context);
   tolua_constant(tolua_S,"e_mail_common_legion_welfare_reward_title",faith::e_mail_common_legion_welfare_reward_title);
   tolua_constant(tolua_S,"e_mail_common_legion_welfare_reward_context",faith::e_mail_common_legion_welfare_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_season_reward_title",faith::e_mail_common_text_element_war_season_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_season_reward_context",faith::e_mail_common_text_element_war_season_reward_context);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_begin_title",faith::e_mail_common_element_war_season_begin_title);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_begin_context",faith::e_mail_common_element_war_season_begin_context);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_ranking_reward_title",faith::e_mail_common_element_war_season_ranking_reward_title);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_ranking_reward_context",faith::e_mail_common_element_war_season_ranking_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_upgrade_title",faith::e_mail_common_text_element_war_upgrade_title);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_upgrade_context",faith::e_mail_common_text_element_war_upgrade_context);
   tolua_constant(tolua_S,"e_mail_common_text_treasure_clear_title",faith::e_mail_common_text_treasure_clear_title);
   tolua_constant(tolua_S,"e_mail_common_text_treasure_clear_context",faith::e_mail_common_text_treasure_clear_context);
   tolua_constant(tolua_S,"e_mail_common_text_world_boss_strategy_reward_title",faith::e_mail_common_text_world_boss_strategy_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_world_boss_strategy_reward_context",faith::e_mail_common_text_world_boss_strategy_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_max",faith::e_mail_common_text_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"server_log","faith::server_log","",NULL);
   tolua_beginmodule(tolua_S,"server_log");
    tolua_function(tolua_S,"auction_sell_log",tolua_logic_hld_server_log_auction_sell_log00);
    tolua_function(tolua_S,"auction_db_log",tolua_logic_hld_server_log_auction_db_log00);
    tolua_function(tolua_S,"auctionbid_log",tolua_logic_hld_server_log_auctionbid_log00);
    tolua_function(tolua_S,"serverAuction",tolua_logic_hld_server_log_serverAuction00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_weap_sub_type_none",faith::e_weap_sub_type_none);
   tolua_constant(tolua_S,"e_weap_sub_type_jian",faith::e_weap_sub_type_jian);
   tolua_constant(tolua_S,"e_weap_sub_type_fu",faith::e_weap_sub_type_fu);
   tolua_constant(tolua_S,"e_weap_sub_type_faqi",faith::e_weap_sub_type_faqi);
   tolua_constant(tolua_S,"e_weap_sub_type_duanzhang",faith::e_weap_sub_type_duanzhang);
   tolua_constant(tolua_S,"e_weap_sub_type_shuangdao",faith::e_weap_sub_type_shuangdao);
   tolua_constant(tolua_S,"e_weap_sub_type_shuanggun",faith::e_weap_sub_type_shuanggun);
   tolua_constant(tolua_S,"e_weap_sub_type_jujian",faith::e_weap_sub_type_jujian);
   tolua_constant(tolua_S,"e_weap_sub_type_juchu",faith::e_weap_sub_type_juchu);
   tolua_constant(tolua_S,"e_weap_sub_type_qianggun",faith::e_weap_sub_type_qianggun);
   tolua_constant(tolua_S,"e_weap_sub_type_changzhang",faith::e_weap_sub_type_changzhang);
   tolua_constant(tolua_S,"e_weap_sub_type_jiyue",faith::e_weap_sub_type_jiyue);
   tolua_constant(tolua_S,"e_weap_sub_type_gong",faith::e_weap_sub_type_gong);
   tolua_constant(tolua_S,"e_weap_sub_type_gongjian",faith::e_weap_sub_type_gongjian);
   tolua_constant(tolua_S,"e_weap_sub_type_dao",faith::e_weap_sub_type_dao);
   tolua_constant(tolua_S,"e_weap_sub_type_zhonggong",faith::e_weap_sub_type_zhonggong);
   tolua_constant(tolua_S,"e_weap_sub_type_assistant",faith::e_weap_sub_type_assistant);
   tolua_constant(tolua_S,"e_weap_sub_type_max",faith::e_weap_sub_type_max);
   tolua_constant(tolua_S,"e_item_lock_status_unlocked",faith::e_item_lock_status_unlocked);
   tolua_constant(tolua_S,"e_item_lock_status_locked",faith::e_item_lock_status_locked);
   tolua_constant(tolua_S,"e_item_lock_status_ignore",faith::e_item_lock_status_ignore);
   tolua_constant(tolua_S,"e_item_lock_status_max",faith::e_item_lock_status_max);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_one_free",faith::e_item_get_hope_item_typ_one_free);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_one_voucher",faith::e_item_get_hope_item_typ_one_voucher);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_ten_voucher",faith::e_item_get_hope_item_typ_ten_voucher);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_one_cost",faith::e_item_get_hope_item_typ_one_cost);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_ten_cost",faith::e_item_get_hope_item_typ_ten_cost);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_max",faith::e_item_get_hope_item_typ_max);
   tolua_constant(tolua_S,"e_quest_sub_type_none",faith::e_quest_sub_type_none);
   tolua_constant(tolua_S,"e_quest_sub_type_unexpendable",faith::e_quest_sub_type_unexpendable);
   tolua_constant(tolua_S,"e_quest_sub_type_expendable",faith::e_quest_sub_type_expendable);
   tolua_constant(tolua_S,"e_quest_sub_type_max",faith::e_quest_sub_type_max);
   tolua_constant(tolua_S,"e_item_color_white",faith::e_item_color_white);
   tolua_constant(tolua_S,"e_item_color_green",faith::e_item_color_green);
   tolua_constant(tolua_S,"e_item_color_blue",faith::e_item_color_blue);
   tolua_constant(tolua_S,"e_item_color_purple",faith::e_item_color_purple);
   tolua_constant(tolua_S,"e_item_color_orange",faith::e_item_color_orange);
   tolua_constant(tolua_S,"e_item_color_red",faith::e_item_color_red);
   tolua_constant(tolua_S,"e_item_color_pink",faith::e_item_color_pink);
   tolua_constant(tolua_S,"e_item_color_max",faith::e_item_color_max);
   tolua_constant(tolua_S,"item_temp_succinct_property_1",faith::item_temp_succinct_property_1);
   tolua_constant(tolua_S,"item_temp_succinct_property_2",faith::item_temp_succinct_property_2);
   tolua_constant(tolua_S,"item_temp_succinct_property_3",faith::item_temp_succinct_property_3);
   tolua_constant(tolua_S,"item_temp_succinct_property_4",faith::item_temp_succinct_property_4);
   tolua_constant(tolua_S,"item_temp_succinct_property_max",faith::item_temp_succinct_property_max);
   tolua_constant(tolua_S,"item_temp_forge_property_1",faith::item_temp_forge_property_1);
   tolua_constant(tolua_S,"item_temp_forge_property_2",faith::item_temp_forge_property_2);
   tolua_constant(tolua_S,"item_temp_forge_property_3",faith::item_temp_forge_property_3);
   tolua_constant(tolua_S,"item_temp_forge_property_4",faith::item_temp_forge_property_4);
   tolua_constant(tolua_S,"item_temp_forge_property_5",faith::item_temp_forge_property_5);
   tolua_constant(tolua_S,"item_temp_forge_property_6",faith::item_temp_forge_property_6);
   tolua_constant(tolua_S,"item_temp_forge_property_max",faith::item_temp_forge_property_max);
   tolua_constant(tolua_S,"item_succinct_money_cost_non",faith::item_succinct_money_cost_non);
   tolua_constant(tolua_S,"item_succinct_money_cost_gold",faith::item_succinct_money_cost_gold);
   tolua_constant(tolua_S,"item_succinct_money_cost_diamond",faith::item_succinct_money_cost_diamond);
   tolua_constant(tolua_S,"item_succinct_change_des_max",faith::item_succinct_change_des_max);
   tolua_constant(tolua_S,"item_succinct_change_des_min",faith::item_succinct_change_des_min);
   tolua_constant(tolua_S,"item_succinct_change_ins_min",faith::item_succinct_change_ins_min);
   tolua_constant(tolua_S,"item_succinct_change_ins_max",faith::item_succinct_change_ins_max);
   tolua_constant(tolua_S,"item_succinct_change_max",faith::item_succinct_change_max);
   tolua_constant(tolua_S,"mount_type_non",faith::mount_type_non);
   tolua_constant(tolua_S,"mount_type_normal",faith::mount_type_normal);
   tolua_constant(tolua_S,"mount_type_special",faith::mount_type_special);
   tolua_constant(tolua_S,"mount_type_warlord",faith::mount_type_warlord);
   tolua_constant(tolua_S,"mount_type_illusion",faith::mount_type_illusion);
   tolua_constant(tolua_S,"mount_type_lordlegionmem",faith::mount_type_lordlegionmem);
   tolua_constant(tolua_S,"e_item_transform_type_none",faith::e_item_transform_type_none);
   tolua_constant(tolua_S,"e_item_transform_type_two",faith::e_item_transform_type_two);
   tolua_constant(tolua_S,"e_item_transform_type_three",faith::e_item_transform_type_three);
   tolua_constant(tolua_S,"e_item_transform_type_four",faith::e_item_transform_type_four);
   tolua_constant(tolua_S,"e_item_transform_type_max",faith::e_item_transform_type_max);
   tolua_constant(tolua_S,"e_item_operate_money_gold",faith::e_item_operate_money_gold);
   tolua_constant(tolua_S,"e_item_operate_money_diamond",faith::e_item_operate_money_diamond);
   tolua_constant(tolua_S,"e_item_unlocked",faith::e_item_unlocked);
   tolua_constant(tolua_S,"e_item_locked",faith::e_item_locked);
   tolua_constant(tolua_S,"e_wing_illusion_originalid_range_low",faith::e_wing_illusion_originalid_range_low);
   tolua_constant(tolua_S,"e_wing_illusion_originalid_range_high",faith::e_wing_illusion_originalid_range_high);
   tolua_constant(tolua_S,"e_wing_illusion_originalid_range_max",faith::e_wing_illusion_originalid_range_max);
   tolua_constant(tolua_S,"e_mount_illusion_originalid_range_low",faith::e_mount_illusion_originalid_range_low);
   tolua_constant(tolua_S,"e_mount_illusion_originalid_range_high",faith::e_mount_illusion_originalid_range_high);
   tolua_constant(tolua_S,"e_mount_illusion_originalid_range_max",faith::e_mount_illusion_originalid_range_max);
   tolua_constant(tolua_S,"e_wing_rush_first",faith::e_wing_rush_first);
   tolua_constant(tolua_S,"e_wing_rush_second",faith::e_wing_rush_second);
   tolua_constant(tolua_S,"e_wing_rush_third",faith::e_wing_rush_third);
   tolua_constant(tolua_S,"e_wing_rush_max",faith::e_wing_rush_max);
   tolua_constant(tolua_S,"e_item_advance_ban",faith::e_item_advance_ban);
   tolua_constant(tolua_S,"e_item_advance_normal",faith::e_item_advance_normal);
   tolua_constant(tolua_S,"e_item_advance_godassembly",faith::e_item_advance_godassembly);
   tolua_constant(tolua_S,"EItemProperty_Level",faith::EItemProperty_Level);
   tolua_constant(tolua_S,"EItemProperty_PropertyID",faith::EItemProperty_PropertyID);
   tolua_constant(tolua_S,"EItemProperty_Points",faith::EItemProperty_Points);
   tolua_constant(tolua_S,"EItemProperty_Percent",faith::EItemProperty_Percent);
   tolua_constant(tolua_S,"EItemProperty_IsShow",faith::EItemProperty_IsShow);
   tolua_constant(tolua_S,"EItemProperty_Max",faith::EItemProperty_Max);
   tolua_constant(tolua_S,"e_luck_draw_record_type_sprite",faith::e_luck_draw_record_type_sprite);
   tolua_constant(tolua_S,"e_luck_draw_record_type_hope_item",faith::e_luck_draw_record_type_hope_item);
   tolua_constant(tolua_S,"e_luck_draw_record_type_max",faith::e_luck_draw_record_type_max);
   tolua_constant(tolua_S,"e_composit_type_none",faith::e_composit_type_none);
   tolua_constant(tolua_S,"e_composit_type_yinji",faith::e_composit_type_yinji);
   tolua_constant(tolua_S,"e_composit_type_chibang",faith::e_composit_type_chibang);
   tolua_constant(tolua_S,"e_composit_type_zhuangbeisuipian",faith::e_composit_type_zhuangbeisuipian);
   tolua_constant(tolua_S,"e_composit_type_zhuangbeicailiao",faith::e_composit_type_zhuangbeicailiao);
   tolua_constant(tolua_S,"e_composit_type_zuoqi",faith::e_composit_type_zuoqi);
   tolua_constant(tolua_S,"e_composit_type_jingling",faith::e_composit_type_jingling);
   tolua_constant(tolua_S,"e_composit_type_chengzhuang",faith::e_composit_type_chengzhuang);
   tolua_constant(tolua_S,"e_composit_type_hongzhuang",faith::e_composit_type_hongzhuang);
   tolua_constant(tolua_S,"e_composit_type_jewel",faith::e_composit_type_jewel);
   tolua_constant(tolua_S,"e_composit_type_item_advance",faith::e_composit_type_item_advance);
   tolua_constant(tolua_S,"e_composit_type_god_assembly",faith::e_composit_type_god_assembly);
   tolua_constant(tolua_S,"e_composit_type_god_assembled",faith::e_composit_type_god_assembled);
   tolua_constant(tolua_S,"e_composit_type_item_cast",faith::e_composit_type_item_cast);
   tolua_constant(tolua_S,"e_composit_type_max",faith::e_composit_type_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_module(tolua_S,"cross",0);
   tolua_beginmodule(tolua_S,"cross");
    tolua_function(tolua_S,"send_msg_to_ws",tolua_logic_hld_cross_send_msg_to_ws00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_constant(tolua_S,"e_title_type_title_card",faith::e_title_type_title_card);
   tolua_constant(tolua_S,"e_title_type_kingofpk",faith::e_title_type_kingofpk);
   tolua_constant(tolua_S,"e_title_type_kingofgods",faith::e_title_type_kingofgods);
   tolua_constant(tolua_S,"e_title_type_legion_city_win_chief",faith::e_title_type_legion_city_win_chief);
   tolua_constant(tolua_S,"e_title_type_legion_city_win_member",faith::e_title_type_legion_city_win_member);
   tolua_constant(tolua_S,"e_title_type_capture_point_chief",faith::e_title_type_capture_point_chief);
   tolua_constant(tolua_S,"e_title_type_capture_point_member",faith::e_title_type_capture_point_member);
   tolua_constant(tolua_S,"e_title_type_login_day_num",faith::e_title_type_login_day_num);
   tolua_constant(tolua_S,"e_title_type_level",faith::e_title_type_level);
   tolua_constant(tolua_S,"e_title_type_kill_player_num",faith::e_title_type_kill_player_num);
   tolua_constant(tolua_S,"e_title_type_arena_rank_first",faith::e_title_type_arena_rank_first);
   tolua_constant(tolua_S,"e_title_type_worship_rank_first",faith::e_title_type_worship_rank_first);
   tolua_constant(tolua_S,"e_title_type_gs_rank_first",faith::e_title_type_gs_rank_first);
   tolua_constant(tolua_S,"e_title_type_money_rank_first",faith::e_title_type_money_rank_first);
   tolua_constant(tolua_S,"e_title_type_first_clearance_heard_map",faith::e_title_type_first_clearance_heard_map);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_barserker",faith::e_title_type_gs_ranking_barserker);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_wizard",faith::e_title_type_gs_ranking_wizard);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_guardian",faith::e_title_type_gs_ranking_guardian);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_assassinator",faith::e_title_type_gs_ranking_assassinator);
   tolua_constant(tolua_S,"e_title_type_vip",faith::e_title_type_vip);
   tolua_constant(tolua_S,"e_title_type_internal_test",faith::e_title_type_internal_test);
   tolua_constant(tolua_S,"e_title_type_best_barserker",faith::e_title_type_best_barserker);
   tolua_constant(tolua_S,"e_title_type_best_wizard",faith::e_title_type_best_wizard);
   tolua_constant(tolua_S,"e_title_type_best_guardian",faith::e_title_type_best_guardian);
   tolua_constant(tolua_S,"e_title_type_best_assassinator",faith::e_title_type_best_assassinator);
   tolua_constant(tolua_S,"e_title_type_marry_times",faith::e_title_type_marry_times);
   tolua_constant(tolua_S,"e_title_type_marry_divorce_times",faith::e_title_type_marry_divorce_times);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_one",faith::e_title_type_cross_ladder_one);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_two",faith::e_title_type_cross_ladder_two);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_three",faith::e_title_type_cross_ladder_three);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_four",faith::e_title_type_cross_ladder_four);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_five",faith::e_title_type_cross_ladder_five);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_six",faith::e_title_type_cross_ladder_six);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_ranking",faith::e_title_type_cross_ladder_ranking);
   tolua_constant(tolua_S,"e_title_type_pk_king_kill_num",faith::e_title_type_pk_king_kill_num);
   tolua_constant(tolua_S,"e_title_type_element_war_one",faith::e_title_type_element_war_one);
   tolua_constant(tolua_S,"e_title_type_element_war_two",faith::e_title_type_element_war_two);
   tolua_constant(tolua_S,"e_title_type_element_war_three",faith::e_title_type_element_war_three);
   tolua_constant(tolua_S,"e_title_type_element_war_ranking",faith::e_title_type_element_war_ranking);
   tolua_constant(tolua_S,"e_title_type_max",faith::e_title_type_max);
   tolua_constant(tolua_S,"e_title_add_result_type_none",faith::e_title_add_result_type_none);
   tolua_constant(tolua_S,"e_title_add_result_type_normal",faith::e_title_add_result_type_normal);
   tolua_constant(tolua_S,"e_title_add_result_type_add_time",faith::e_title_add_result_type_add_time);
   tolua_constant(tolua_S,"e_title_add_result_type_cannot_use_again",faith::e_title_add_result_type_cannot_use_again);
   tolua_constant(tolua_S,"e_title_add_result_type_max",faith::e_title_add_result_type_max);
   tolua_constant(tolua_S,"e_title_operate_type_equip_on",faith::e_title_operate_type_equip_on);
   tolua_constant(tolua_S,"e_title_operate_type_equip_off",faith::e_title_operate_type_equip_off);
   tolua_constant(tolua_S,"e_title_operate_type_max",faith::e_title_operate_type_max);
   tolua_constant(tolua_S,"e_title_end_time_type_forever",faith::e_title_end_time_type_forever);
   tolua_constant(tolua_S,"e_title_end_time_type_time",faith::e_title_end_time_type_time);
   tolua_constant(tolua_S,"e_title_end_time_type_condition1",faith::e_title_end_time_type_condition1);
   tolua_constant(tolua_S,"e_title_end_time_type_condition2",faith::e_title_end_time_type_condition2);
   tolua_constant(tolua_S,"e_title_special_handle_none",faith::e_title_special_handle_none);
   tolua_constant(tolua_S,"e_title_special_handle_add",faith::e_title_special_handle_add);
   tolua_constant(tolua_S,"e_title_special_handle_remove",faith::e_title_special_handle_remove);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_logic (lua_State* tolua_S) {
 return tolua_logic_open(tolua_S);
};
#endif

