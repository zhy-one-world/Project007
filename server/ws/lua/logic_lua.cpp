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
using namespace hld;

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_hld__s_team_member_info (lua_State* tolua_S)
{
 hld::s_team_member_info* self = (hld::s_team_member_info*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__s_map_pos (lua_State* tolua_S)
{
 hld::s_map_pos* self = (hld::s_map_pos*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__s_client_uid (lua_State* tolua_S)
{
 hld::s_client_uid* self = (hld::s_client_uid*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__frotator (lua_State* tolua_S)
{
 hld::frotator* self = (hld::frotator*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__s_legion_member_info (lua_State* tolua_S)
{
 hld::s_legion_member_info* self = (hld::s_legion_member_info*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__fvector (lua_State* tolua_S)
{
 hld::fvector* self = (hld::fvector*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__guid_64 (lua_State* tolua_S)
{
 hld::guid_64* self = (hld::guid_64*) tolua_tousertype(tolua_S,1,0);
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
 hld::c_mail_info* self = (hld::c_mail_info*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"hld::team_ws");
 Mtolua_typeid(tolua_S,typeid(hld::team_ws), "hld::team_ws");
 tolua_usertype(tolua_S,"login_fixed_data");
 Mtolua_typeid(tolua_S,typeid(login_fixed_data), "login_fixed_data");
 tolua_usertype(tolua_S,"hld::invalid_ansi_word");
 Mtolua_typeid(tolua_S,typeid(hld::invalid_ansi_word), "hld::invalid_ansi_word");
 tolua_usertype(tolua_S,"hld::cloud_shop_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::cloud_shop_mgr), "hld::cloud_shop_mgr");
 tolua_usertype(tolua_S,"hld::legion_ws_applicant_list");
 Mtolua_typeid(tolua_S,typeid(hld::legion_ws_applicant_list), "hld::legion_ws_applicant_list");
 tolua_usertype(tolua_S,"hld::fvector");
 Mtolua_typeid(tolua_S,typeid(hld::fvector), "hld::fvector");
 tolua_usertype(tolua_S,"e_client_req_type");
 Mtolua_typeid(tolua_S,typeid(e_client_req_type), "e_client_req_type");
 tolua_usertype(tolua_S,"hld::client_session");
 Mtolua_typeid(tolua_S,typeid(hld::client_session), "hld::client_session");
 tolua_usertype(tolua_S,"hld::mail_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::mail_ws_mgr), "hld::mail_ws_mgr");
 tolua_usertype(tolua_S,"hld::ranking_mgr_ws");
 Mtolua_typeid(tolua_S,typeid(hld::ranking_mgr_ws), "hld::ranking_mgr_ws");
 tolua_usertype(tolua_S,"hld::appearance_info_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::appearance_info_mgr), "hld::appearance_info_mgr");
 tolua_usertype(tolua_S,"hld::rename_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::rename_mgr), "hld::rename_mgr");
 tolua_usertype(tolua_S,"hld::s_client_uid");
 Mtolua_typeid(tolua_S,typeid(hld::s_client_uid), "hld::s_client_uid");
 tolua_usertype(tolua_S,"hld::globle_data");
 Mtolua_typeid(tolua_S,typeid(hld::globle_data), "hld::globle_data");
 tolua_usertype(tolua_S,"hld::crystal_fairyland_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::crystal_fairyland_mgr), "hld::crystal_fairyland_mgr");
 tolua_usertype(tolua_S,"e_error_code");
 Mtolua_typeid(tolua_S,typeid(e_error_code), "e_error_code");
 tolua_usertype(tolua_S,"legion_ws_applicant");
 Mtolua_typeid(tolua_S,typeid(legion_ws_applicant), "legion_ws_applicant");
 tolua_usertype(tolua_S,"hld::big_player_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::big_player_ws_mgr), "hld::big_player_ws_mgr");
 tolua_usertype(tolua_S,"hld::arena_mgr_ws");
 Mtolua_typeid(tolua_S,typeid(hld::arena_mgr_ws), "hld::arena_mgr_ws");
 tolua_usertype(tolua_S,"hld::element_war_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::element_war_ws_mgr), "hld::element_war_ws_mgr");
 tolua_usertype(tolua_S,"hld::gain_treasure_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::gain_treasure_ws_mgr), "hld::gain_treasure_ws_mgr");
 tolua_usertype(tolua_S,"hld::random_gen");
 Mtolua_typeid(tolua_S,typeid(hld::random_gen), "hld::random_gen");
 tolua_usertype(tolua_S,"hld::event_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::event_ws_mgr), "hld::event_ws_mgr");
 tolua_usertype(tolua_S,"s_fake_player_info");
 Mtolua_typeid(tolua_S,typeid(s_fake_player_info), "s_fake_player_info");
 tolua_usertype(tolua_S,"hld::cross_server_city_war_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::cross_server_city_war_ws_mgr), "hld::cross_server_city_war_ws_mgr");
 tolua_usertype(tolua_S,"s_log_common_head");
 Mtolua_typeid(tolua_S,typeid(s_log_common_head), "s_log_common_head");
 tolua_usertype(tolua_S,"hld::web_client");
 Mtolua_typeid(tolua_S,typeid(hld::web_client), "hld::web_client");
 tolua_usertype(tolua_S,"hld::city_war_territory_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::city_war_territory_mgr), "hld::city_war_territory_mgr");
 tolua_usertype(tolua_S,"hld::relation_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::relation_mgr), "hld::relation_mgr");
 tolua_usertype(tolua_S,"s_unit_info");
 Mtolua_typeid(tolua_S,typeid(s_unit_info), "s_unit_info");
 tolua_usertype(tolua_S,"hld::world_server");
 Mtolua_typeid(tolua_S,typeid(hld::world_server), "hld::world_server");
 tolua_usertype(tolua_S,"hld::marry_mgr_ws");
 Mtolua_typeid(tolua_S,typeid(hld::marry_mgr_ws), "hld::marry_mgr_ws");
 tolua_usertype(tolua_S,"hld::world_boss_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::world_boss_ws_mgr), "hld::world_boss_ws_mgr");
 tolua_usertype(tolua_S,"hld::pk_king_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::pk_king_mgr), "hld::pk_king_mgr");
 tolua_usertype(tolua_S,"hld::client_session_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::client_session_mgr), "hld::client_session_mgr");
 tolua_usertype(tolua_S,"hld::cross_server_harry_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::cross_server_harry_ws_mgr), "hld::cross_server_harry_ws_mgr");
 tolua_usertype(tolua_S,"hld::s_team_member_info");
 Mtolua_typeid(tolua_S,typeid(hld::s_team_member_info), "hld::s_team_member_info");
 tolua_usertype(tolua_S,"hld::team_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::team_ws_mgr), "hld::team_ws_mgr");
 tolua_usertype(tolua_S,"hld::ws_client");
 Mtolua_typeid(tolua_S,typeid(hld::ws_client), "hld::ws_client");
 tolua_usertype(tolua_S,"hld::init_unit");
 Mtolua_typeid(tolua_S,typeid(hld::init_unit), "hld::init_unit");
 tolua_usertype(tolua_S,"hld::guid_64");
 Mtolua_typeid(tolua_S,typeid(hld::guid_64), "hld::guid_64");
 tolua_usertype(tolua_S,"hld::legion_ws_city_war");
 Mtolua_typeid(tolua_S,typeid(hld::legion_ws_city_war), "hld::legion_ws_city_war");
 tolua_usertype(tolua_S,"hld::server_log");
 Mtolua_typeid(tolua_S,typeid(hld::server_log), "hld::server_log");
 tolua_usertype(tolua_S,"ws2ws_send_big_player_msg");
 Mtolua_typeid(tolua_S,typeid(ws2ws_send_big_player_msg), "ws2ws_send_big_player_msg");
 tolua_usertype(tolua_S,"uint64");
 Mtolua_typeid(tolua_S,typeid(uint64), "uint64");
 tolua_usertype(tolua_S,"hld::cross_server_pk_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::cross_server_pk_ws_mgr), "hld::cross_server_pk_ws_mgr");
 tolua_usertype(tolua_S,"hld::c_element_war_role_info");
 Mtolua_typeid(tolua_S,typeid(hld::c_element_war_role_info), "hld::c_element_war_role_info");
 tolua_usertype(tolua_S,"std::vector<hld::s_item_info>");
 Mtolua_typeid(tolua_S,typeid(std::vector<hld::s_item_info>), "std::vector<hld::s_item_info>");
 tolua_usertype(tolua_S,"hld::legion_ws_warehouse");
 Mtolua_typeid(tolua_S,typeid(hld::legion_ws_warehouse), "hld::legion_ws_warehouse");
 tolua_usertype(tolua_S,"hld::s_map_pos");
 Mtolua_typeid(tolua_S,typeid(hld::s_map_pos), "hld::s_map_pos");
 tolua_usertype(tolua_S,"hld::s_item_info");
 Mtolua_typeid(tolua_S,typeid(hld::s_item_info), "hld::s_item_info");
 tolua_usertype(tolua_S,"hld::legion_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::legion_ws_mgr), "hld::legion_ws_mgr");
 tolua_usertype(tolua_S,"s_auction_info");
 Mtolua_typeid(tolua_S,typeid(s_auction_info), "s_auction_info");
 tolua_usertype(tolua_S,"std::vector<int32>");
 Mtolua_typeid(tolua_S,typeid(std::vector<int32>), "std::vector<int32>");
 tolua_usertype(tolua_S,"hld::s_legion_member_info");
 Mtolua_typeid(tolua_S,typeid(hld::s_legion_member_info), "hld::s_legion_member_info");
 tolua_usertype(tolua_S,"hld::cs_map_mgr_system");
 Mtolua_typeid(tolua_S,typeid(hld::cs_map_mgr_system), "hld::cs_map_mgr_system");
 tolua_usertype(tolua_S,"hld::cs_map_system");
 Mtolua_typeid(tolua_S,typeid(hld::cs_map_system), "hld::cs_map_system");
 tolua_usertype(tolua_S,"hld::legion_ws_answer");
 Mtolua_typeid(tolua_S,typeid(hld::legion_ws_answer), "hld::legion_ws_answer");
 tolua_usertype(tolua_S,"std::vector<s_item_template_info>");
 Mtolua_typeid(tolua_S,typeid(std::vector<s_item_template_info>), "std::vector<s_item_template_info>");
 tolua_usertype(tolua_S,"hld::legion_ws");
 Mtolua_typeid(tolua_S,typeid(hld::legion_ws), "hld::legion_ws");
 tolua_usertype(tolua_S,"hld::frotator");
 Mtolua_typeid(tolua_S,typeid(hld::frotator), "hld::frotator");
 tolua_usertype(tolua_S,"hld::broken_sky_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::broken_sky_mgr), "hld::broken_sky_mgr");
 tolua_usertype(tolua_S,"hld::cross_server_world_boss_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::cross_server_world_boss_ws_mgr), "hld::cross_server_world_boss_ws_mgr");
 tolua_usertype(tolua_S,"s_mail_info");
 Mtolua_typeid(tolua_S,typeid(s_mail_info), "s_mail_info");
 tolua_usertype(tolua_S,"hld::assist_fight_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::assist_fight_mgr), "hld::assist_fight_mgr");
 tolua_usertype(tolua_S,"hld::time_limit_activity_ws_mgr");
 Mtolua_typeid(tolua_S,typeid(hld::time_limit_activity_ws_mgr), "hld::time_limit_activity_ws_mgr");
 tolua_usertype(tolua_S,"hld::c_mail_info");
 Mtolua_typeid(tolua_S,typeid(hld::c_mail_info), "hld::c_mail_info");
}

/* method: new of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new00
static int tolua_logic_hld_s_client_uid_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_client_uid* tolua_ret = (hld::s_client_uid*)  Mtolua_new((hld::s_client_uid)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_client_uid");
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

/* method: new_local of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new00_local
static int tolua_logic_hld_s_client_uid_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_client_uid* tolua_ret = (hld::s_client_uid*)  Mtolua_new((hld::s_client_uid)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_client_uid");
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

/* method: new of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new01
static int tolua_logic_hld_s_client_uid_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_client_uid",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int64 InValue = ((int64)  tolua_tointeger(tolua_S,2,0));
  {
   hld::s_client_uid* tolua_ret = (hld::s_client_uid*)  Mtolua_new((hld::s_client_uid)(InValue));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_client_uid");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_s_client_uid_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_new01_local
static int tolua_logic_hld_s_client_uid_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_client_uid",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int64 InValue = ((int64)  tolua_tointeger(tolua_S,2,0));
  {
   hld::s_client_uid* tolua_ret = (hld::s_client_uid*)  Mtolua_new((hld::s_client_uid)(InValue));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_client_uid");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_s_client_uid_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_valid of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_is_valid00
static int tolua_logic_hld_s_client_uid_is_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::s_client_uid* self = (hld::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
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

/* method: clear_data of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_client_uid_clear_data00
static int tolua_logic_hld_s_client_uid_clear_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::s_client_uid",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::s_client_uid* self = (hld::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: fep_uid_64 of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_client_uid_fep_uid_64
static int tolua_get_hld__s_client_uid_fep_uid_64(lua_State* tolua_S)
{
  hld::s_client_uid* self = (hld::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'fep_uid_64'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->fep_uid_64);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: fep_uid_64 of class  hld::s_client_uid */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_client_uid_fep_uid_64
static int tolua_set_hld__s_client_uid_fep_uid_64(lua_State* tolua_S)
{
  hld::s_client_uid* self = (hld::s_client_uid*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new00
static int tolua_logic_hld_guid_64_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::guid_64* tolua_ret = (hld::guid_64*)  Mtolua_new((hld::guid_64)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
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

/* method: new_local of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new00_local
static int tolua_logic_hld_guid_64_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::guid_64* tolua_ret = (hld::guid_64*)  Mtolua_new((hld::guid_64)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
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

/* method: new of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new01
static int tolua_logic_hld_guid_64_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  long long In64 = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   hld::guid_64* tolua_ret = (hld::guid_64*)  Mtolua_new((hld::guid_64)(In64));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new01_local
static int tolua_logic_hld_guid_64_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  long long In64 = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   hld::guid_64* tolua_ret = (hld::guid_64*)  Mtolua_new((hld::guid_64)(In64));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new02
static int tolua_logic_hld_guid_64_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
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
   hld::guid_64* tolua_ret = (hld::guid_64*)  Mtolua_new((hld::guid_64)(InA,InB));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_new02_local
static int tolua_logic_hld_guid_64_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
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
   hld::guid_64* tolua_ret = (hld::guid_64*)  Mtolua_new((hld::guid_64)(InA,InB));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_guid_64_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_A of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_get_A00
static int tolua_logic_hld_guid_64_get_A00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::guid_64* self = (hld::guid_64*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_B of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_get_B00
static int tolua_logic_hld_guid_64_get_B00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::guid_64* self = (hld::guid_64*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_AB of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_get_AB00
static int tolua_logic_hld_guid_64_get_AB00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::guid_64* self = (hld::guid_64*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_valid of class  hld::guid_64 */
#ifndef TOLUA_DISABLE_tolua_logic_hld_guid_64_is_valid00
static int tolua_logic_hld_guid_64_is_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::guid_64",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::guid_64* self = (hld::guid_64*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: x of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_get_hld__fvector_x
static int tolua_get_hld__fvector_x(lua_State* tolua_S)
{
  hld::fvector* self = (hld::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_set_hld__fvector_x
static int tolua_set_hld__fvector_x(lua_State* tolua_S)
{
  hld::fvector* self = (hld::fvector*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: y of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_get_hld__fvector_y
static int tolua_get_hld__fvector_y(lua_State* tolua_S)
{
  hld::fvector* self = (hld::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_set_hld__fvector_y
static int tolua_set_hld__fvector_y(lua_State* tolua_S)
{
  hld::fvector* self = (hld::fvector*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: z of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_get_hld__fvector_z
static int tolua_get_hld__fvector_z(lua_State* tolua_S)
{
  hld::fvector* self = (hld::fvector*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_set_hld__fvector_z
static int tolua_set_hld__fvector_z(lua_State* tolua_S)
{
  hld::fvector* self = (hld::fvector*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_logic_hld_fvector_new00
static int tolua_logic_hld_fvector_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::fvector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::fvector* tolua_ret = (hld::fvector*)  Mtolua_new((hld::fvector)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::fvector");
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

/* method: new_local of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_logic_hld_fvector_new00_local
static int tolua_logic_hld_fvector_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::fvector",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::fvector* tolua_ret = (hld::fvector*)  Mtolua_new((hld::fvector)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::fvector");
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

/* method: set_data of class  hld::fvector */
#ifndef TOLUA_DISABLE_tolua_logic_hld_fvector_set_data00
static int tolua_logic_hld_fvector_set_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::fvector",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::fvector* self = (hld::fvector*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: roll of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_get_hld__frotator_roll
static int tolua_get_hld__frotator_roll(lua_State* tolua_S)
{
  hld::frotator* self = (hld::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'roll'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->roll);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: roll of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_set_hld__frotator_roll
static int tolua_set_hld__frotator_roll(lua_State* tolua_S)
{
  hld::frotator* self = (hld::frotator*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: pitch of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_get_hld__frotator_pitch
static int tolua_get_hld__frotator_pitch(lua_State* tolua_S)
{
  hld::frotator* self = (hld::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pitch'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->pitch);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pitch of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_set_hld__frotator_pitch
static int tolua_set_hld__frotator_pitch(lua_State* tolua_S)
{
  hld::frotator* self = (hld::frotator*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: yaw of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_get_hld__frotator_yaw
static int tolua_get_hld__frotator_yaw(lua_State* tolua_S)
{
  hld::frotator* self = (hld::frotator*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'yaw'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->yaw);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: yaw of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_set_hld__frotator_yaw
static int tolua_set_hld__frotator_yaw(lua_State* tolua_S)
{
  hld::frotator* self = (hld::frotator*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_logic_hld_frotator_new00
static int tolua_logic_hld_frotator_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::frotator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::frotator* tolua_ret = (hld::frotator*)  Mtolua_new((hld::frotator)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::frotator");
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

/* method: new_local of class  hld::frotator */
#ifndef TOLUA_DISABLE_tolua_logic_hld_frotator_new00_local
static int tolua_logic_hld_frotator_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::frotator",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::frotator* tolua_ret = (hld::frotator*)  Mtolua_new((hld::frotator)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::frotator");
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

/* get function: unit_location of class  hld::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_map_pos_unit_location
static int tolua_get_hld__s_map_pos_unit_location(lua_State* tolua_S)
{
  hld::s_map_pos* self = (hld::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_location'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->unit_location,"hld::fvector");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unit_location of class  hld::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_map_pos_unit_location
static int tolua_set_hld__s_map_pos_unit_location(lua_State* tolua_S)
{
  hld::s_map_pos* self = (hld::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_location'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::fvector",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->unit_location = *((hld::fvector*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: unit_rotation of class  hld::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_map_pos_unit_rotation
static int tolua_get_hld__s_map_pos_unit_rotation(lua_State* tolua_S)
{
  hld::s_map_pos* self = (hld::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_rotation'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->unit_rotation,"hld::frotator");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: unit_rotation of class  hld::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_map_pos_unit_rotation
static int tolua_set_hld__s_map_pos_unit_rotation(lua_State* tolua_S)
{
  hld::s_map_pos* self = (hld::s_map_pos*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'unit_rotation'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::frotator",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->unit_rotation = *((hld::frotator*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  hld::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_map_pos_new00
static int tolua_logic_hld_s_map_pos_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_map_pos",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_map_pos* tolua_ret = (hld::s_map_pos*)  Mtolua_new((hld::s_map_pos)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_map_pos");
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

/* method: new_local of class  hld::s_map_pos */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_map_pos_new00_local
static int tolua_logic_hld_s_map_pos_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_map_pos",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_map_pos* tolua_ret = (hld::s_map_pos*)  Mtolua_new((hld::s_map_pos)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_map_pos");
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

/* method: is_vaild of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_is_vaild00
static int tolua_logic_hld_client_session_is_vaild00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_array_index of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_array_index00
static int tolua_logic_hld_client_session_get_array_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_cs_conn_index of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_cs_conn_index00
static int tolua_logic_hld_client_session_get_cs_conn_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_to_client of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_send_to_client00
static int tolua_logic_hld_client_session_send_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_to_cs_lua of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_send_to_cs_lua00
static int tolua_logic_hld_client_session_send_to_cs_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_client_uid of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_client_uid00
static int tolua_logic_hld_client_session_get_client_uid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_client_uid'", NULL);
#endif
  {
   hld::s_client_uid& tolua_ret = (hld::s_client_uid&)  self->get_client_uid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::s_client_uid");
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

/* method: get_map_guid of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_map_guid00
static int tolua_logic_hld_client_session_get_map_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_map_guid'", NULL);
#endif
  {
   hld::guid_64& tolua_ret = (hld::guid_64&)  self->get_map_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::guid_64");
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

/* method: get_team_guid of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_team_guid00
static int tolua_logic_hld_client_session_get_team_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_team_guid'", NULL);
#endif
  {
   hld::guid_64& tolua_ret = (hld::guid_64&)  self->get_team_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::guid_64");
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

/* method: get_legion_guid of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_legion_guid00
static int tolua_logic_hld_client_session_get_legion_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_guid'", NULL);
#endif
  {
   hld::guid_64& tolua_ret = (hld::guid_64&)  self->get_legion_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::guid_64");
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

/* method: get_role_info_data of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_role_info_data00
static int tolua_logic_hld_client_session_get_role_info_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
  hld::e_role_info eType = ((hld::e_role_info) (int)  tolua_tointeger(tolua_S,2,0));
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

/* method: get_role_guid of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_role_guid00
static int tolua_logic_hld_client_session_get_role_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_role_guid'", NULL);
#endif
  {
   hld::guid_64 tolua_ret = (hld::guid_64)  self->get_role_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((hld::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(hld::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
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

/* method: get_relation_list_mgr of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_relation_list_mgr00
static int tolua_logic_hld_client_session_get_relation_list_mgr00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_relation_list_mgr'", NULL);
#endif
  {
   hld::relation_mgr& tolua_ret = (hld::relation_mgr&)  self->get_relation_list_mgr();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::relation_mgr");
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

/* method: get_role_name of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_role_name00
static int tolua_logic_hld_client_session_get_role_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_role_name of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_role_name00
static int tolua_logic_hld_client_session_set_role_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_notice of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_send_notice00
static int tolua_logic_hld_client_session_send_notice00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_self_server of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_is_self_server00
static int tolua_logic_hld_client_session_is_self_server00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_self_server of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_is_self_server01
static int tolua_logic_hld_client_session_is_self_server01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 target_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_third_info of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_third_info00
static int tolua_logic_hld_client_session_get_third_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_login_type of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_login_type00
static int tolua_logic_hld_client_session_get_login_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_unit_info of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_unit_info00
static int tolua_logic_hld_client_session_get_unit_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_m_is_in_assist of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_m_is_in_assist00
static int tolua_logic_hld_client_session_get_m_is_in_assist00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_m_is_in_assist of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_m_is_in_assist00
static int tolua_logic_hld_client_session_set_m_is_in_assist00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_assit_fight_guid of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_assit_fight_guid00
static int tolua_logic_hld_client_session_set_assit_fight_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_assit_fight_guid of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_assit_fight_guid00
static int tolua_logic_hld_client_session_get_assit_fight_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_m_activate_code_time of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_get_m_activate_code_time00
static int tolua_logic_hld_client_session_get_m_activate_code_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_m_activate_code_time of class  hld::client_session */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_set_m_activate_code_time00
static int tolua_logic_hld_client_session_set_m_activate_code_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* self = (hld::client_session*)  tolua_tousertype(tolua_S,1,0);
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

/* function: hld::get_guid_64 */
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
   hld::guid_64* tolua_ret = (hld::guid_64*)  hld::get_guid_64(value);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
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

/* function: hld::get_guid_64 */
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
   hld::guid_64* tolua_ret = (hld::guid_64*)  hld::get_guid_64(a,b);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_get_guid_6400(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: hld::get_guid_64 */
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
   hld::guid_64* tolua_ret = (hld::guid_64*)  hld::get_guid_64();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::guid_64");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_get_guid_6401(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: hld::get_client_session_by_index */
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
   hld::client_session* tolua_ret = (hld::client_session*)  hld::get_client_session_by_index(unit_index);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::client_session");
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

/* function: hld::get_client_session_by_guid */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_client_session_by_guid00
static int tolua_logic_hld_get_client_session_by_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::guid_64 guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,1,0));
  {
   hld::client_session* tolua_ret = (hld::client_session*)  hld::get_client_session_by_guid(guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::client_session");
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

/* function: hld::get_world_server */
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
   hld::world_server* tolua_ret = (hld::world_server*)  hld::get_world_server();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::world_server");
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

/* function: hld::make_guid_lua */
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
   int64 tolua_ret = (int64)  hld::make_guid_lua(in_guid);
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

/* function: hld::get_client_session_mgr */
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
   hld::client_session_mgr* tolua_ret = (hld::client_session_mgr*)  hld::get_client_session_mgr();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::client_session_mgr");
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

/* function: hld::get_str_by_string_template_id */
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
   std::string tolua_ret = (std::string)  hld::get_str_by_string_template_id(template_id,index);
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

/* function: hld::get_item_info_by_str */
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
   hld::s_item_info* tolua_ret = (hld::s_item_info*)  hld::get_item_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_item_info");
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

/* function: hld::get_item_info_by_str */
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
   hld::s_item_info* tolua_ret = (hld::s_item_info*)  hld::get_item_info_by_str(msg,msg_len);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_item_info");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_get_item_info_by_str00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* function: hld::get_auction_info_by_str */
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
   s_auction_info* tolua_ret = (s_auction_info*)  hld::get_auction_info_by_str(msg,msg_len);
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

/* function: hld::get_auction_buffer */
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
   char* tolua_ret = (char*)  hld::get_auction_buffer(auction_ptr);
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

/* function: hld::get_unit_info_by_str */
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
   s_unit_info* tolua_ret = (s_unit_info*)  hld::get_unit_info_by_str(msg,msg_len);
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

/* function: hld::get_login_fixed_data_by_str */
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
   login_fixed_data* tolua_ret = (login_fixed_data*)  hld::get_login_fixed_data_by_str(msg,msg_len);
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

/* function: hld::get_log_common_head */
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
   s_log_common_head* tolua_ret = (s_log_common_head*)  hld::get_log_common_head();
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

/* function: hld::lua_set_log_common_head_part1 */
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
   hld::lua_set_log_common_head_part1(*head,game_id,channel_id,media_id,device_id);
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

/* function: hld::lua_set_log_common_head_part2 */
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
   hld::lua_set_log_common_head_part2(*head,version_name,version_code);
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

/* function: hld::lua_set_log_common_head_part3 */
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
   hld::lua_set_log_common_head_part3(*head,user_id,server_id,log_time);
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

/* function: hld::lua_set_log_common_head_part4 */
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
   hld::lua_set_log_common_head_part4(*head,role_id,role_name,role_level,role_gender);
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

/* function: hld::lua_set_log_common_head_part5 */
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
   hld::lua_set_log_common_head_part5(*head,battle_points,role_type,vip_level);
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

/* function: hld::get_app_key */
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
   string tolua_ret = (string)  hld::get_app_key();
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

/* function: hld::get_mail_info_by_str */
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
   s_mail_info* tolua_ret = (s_mail_info*)  hld::get_mail_info_by_str(msg,msg_len);
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

/* function: hld::get_fake_player_info_by_str */
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
   s_fake_player_info* tolua_ret = (s_fake_player_info*)  hld::get_fake_player_info_by_str(msg,msg_len);
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

/* function: hld::get_send_big_player_msg */
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
   ws2ws_send_big_player_msg* tolua_ret = (ws2ws_send_big_player_msg*)  hld::get_send_big_player_msg(msg,msg_len,out_len);
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

/* method: get_instance of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_instance00
static int tolua_logic_hld_globle_data_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::globle_data& tolua_ret = (hld::globle_data&)  hld::globle_data::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::globle_data");
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

/* method: send_mail_to_another_player_by_system of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_send_mail_to_another_player_by_system00
static int tolua_logic_hld_globle_data_send_mail_to_another_player_by_system00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,11,0,&tolua_err) ||
     !tolua_isusertype(tolua_S,12,"const hld::s_item_info",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,13,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,14,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 target_player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  hld::guid_64 sender_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int receiver_server_id = ((int)  tolua_tointeger(tolua_S,4,0));
  std::string SenderName = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string title = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  std::string contentText = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  int money_typ1 = ((int)  tolua_tointeger(tolua_S,8,0));
  int money_num1 = ((int)  tolua_tointeger(tolua_S,9,0));
  int money_typ2 = ((int)  tolua_tointeger(tolua_S,10,0));
  int money_num2 = ((int)  tolua_tointeger(tolua_S,11,0));
  const hld::s_item_info* item_list = ((const hld::s_item_info*)  tolua_tousertype(tolua_S,12,0));
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

/* method: clear_char_notice_info of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_clear_char_notice_info00
static int tolua_logic_hld_globle_data_clear_char_notice_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_notice_id of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_set_notice_id00
static int tolua_logic_hld_globle_data_set_notice_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_other_name of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_other_name00
static int tolua_logic_hld_globle_data_add_other_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_describe_str of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_describe_str00
static int tolua_logic_hld_globle_data_add_describe_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_item_name of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_item_name00
static int tolua_logic_hld_globle_data_add_item_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_num_type_data of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_num_type_data00
static int tolua_logic_hld_globle_data_add_num_type_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_item_data of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_add_item_data00
static int tolua_logic_hld_globle_data_add_item_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::s_item_info",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
  const hld::s_item_info* temp_item_data = ((const hld::s_item_info*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_mail_common_text of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_mail_common_text00
static int tolua_logic_hld_globle_data_get_mail_common_text00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_mail_common_text_id of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_mail_common_text_id00
static int tolua_logic_hld_globle_data_get_mail_common_text_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
  hld::e_mail_common_text id_index = ((hld::e_mail_common_text) (int)  tolua_tointeger(tolua_S,2,0));
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

/* method: get_region_code of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_region_code00
static int tolua_logic_hld_globle_data_get_region_code00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_money_str_by_type of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_money_str_by_type00
static int tolua_logic_hld_globle_data_get_money_str_by_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_server_on_days of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_get_server_on_days00
static int tolua_logic_hld_globle_data_get_server_on_days00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
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

/* method: make_auction_info of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_make_auction_info00
static int tolua_logic_hld_globle_data_make_auction_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"s_auction_info",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::s_item_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
  s_auction_info* target_info = ((s_auction_info*)  tolua_tousertype(tolua_S,2,0));
  hld::s_item_info item_info = *((hld::s_item_info*)  tolua_tousertype(tolua_S,3,0));
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

/* method: make_auction_info of class  hld::globle_data */
#ifndef TOLUA_DISABLE_tolua_logic_hld_globle_data_make_auction_info01
static int tolua_logic_hld_globle_data_make_auction_info01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::globle_data",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"s_auction_info",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::s_item_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"hld::guid_64",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,7,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,1,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  hld::globle_data* self = (hld::globle_data*)  tolua_tousertype(tolua_S,1,0);
  s_auction_info* target_info = ((s_auction_info*)  tolua_tousertype(tolua_S,2,0));
  hld::s_item_info item_info = *((hld::s_item_info*)  tolua_tousertype(tolua_S,3,0));
  int32 sell_num = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 trade_type = ((int32)  tolua_tointeger(tolua_S,5,0));
  hld::guid_64 seller_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,6,0));
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

/* get function: TargetPlayerGuid of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_TargetPlayerGuid
static int tolua_get_hld__c_mail_info_TargetPlayerGuid(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TargetPlayerGuid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->TargetPlayerGuid,"hld::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: TargetPlayerGuid of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_TargetPlayerGuid
static int tolua_set_hld__c_mail_info_TargetPlayerGuid(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'TargetPlayerGuid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->TargetPlayerGuid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: SenderPlayerGuid of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_SenderPlayerGuid
static int tolua_get_hld__c_mail_info_SenderPlayerGuid(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SenderPlayerGuid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->SenderPlayerGuid,"hld::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SenderPlayerGuid of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_SenderPlayerGuid
static int tolua_set_hld__c_mail_info_SenderPlayerGuid(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SenderPlayerGuid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->SenderPlayerGuid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: SenderName of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_SenderName
static int tolua_get_hld__c_mail_info_SenderName(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'SenderName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->SenderName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: SenderName of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_SenderName
static int tolua_set_hld__c_mail_info_SenderName(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: title of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_title
static int tolua_get_hld__c_mail_info_title(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'title'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->title);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: title of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_title
static int tolua_set_hld__c_mail_info_title(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: contentText of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_contentText
static int tolua_get_hld__c_mail_info_contentText(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'contentText'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->contentText);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: contentText of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_contentText
static int tolua_set_hld__c_mail_info_contentText(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: money_typ1 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_typ1
static int tolua_get_hld__c_mail_info_money_typ1(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_typ1'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_typ1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_typ1 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_typ1
static int tolua_set_hld__c_mail_info_money_typ1(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: money_num1 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_num1
static int tolua_get_hld__c_mail_info_money_num1(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_num1'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_num1);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_num1 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_num1
static int tolua_set_hld__c_mail_info_money_num1(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: money_typ2 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_typ2
static int tolua_get_hld__c_mail_info_money_typ2(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_typ2'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_typ2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_typ2 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_typ2
static int tolua_set_hld__c_mail_info_money_typ2(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: money_num2 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_money_num2
static int tolua_get_hld__c_mail_info_money_num2(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'money_num2'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->money_num2);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: money_num2 of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_money_num2
static int tolua_set_hld__c_mail_info_money_num2(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: item_list of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_item_list_ptr
static int tolua_get_hld__c_mail_info_item_list_ptr(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_list'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)self->item_list,"const hld::s_item_info");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: item_num of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__c_mail_info_item_num
static int tolua_get_hld__c_mail_info_item_num(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'item_num'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->item_num);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: item_num of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__c_mail_info_item_num
static int tolua_set_hld__c_mail_info_item_num(lua_State* tolua_S)
{
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new00
static int tolua_logic_hld_c_mail_info_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::c_mail_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::c_mail_info* tolua_ret = (hld::c_mail_info*)  Mtolua_new((hld::c_mail_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::c_mail_info");
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

/* method: new_local of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new00_local
static int tolua_logic_hld_c_mail_info_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::c_mail_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::c_mail_info* tolua_ret = (hld::c_mail_info*)  Mtolua_new((hld::c_mail_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::c_mail_info");
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

/* method: new of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new01
static int tolua_logic_hld_c_mail_info_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::c_mail_info",0,&tolua_err) ||
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
   hld::c_mail_info* tolua_ret = (hld::c_mail_info*)  Mtolua_new((hld::c_mail_info)(_SenderName,_title,_contentText));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::c_mail_info");
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_c_mail_info_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_new01_local
static int tolua_logic_hld_c_mail_info_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::c_mail_info",0,&tolua_err) ||
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
   hld::c_mail_info* tolua_ret = (hld::c_mail_info*)  Mtolua_new((hld::c_mail_info)(_SenderName,_title,_contentText));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::c_mail_info");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_logic_hld_c_mail_info_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: clear_data of class  hld::c_mail_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c_mail_info_clear_data00
static int tolua_logic_hld_c_mail_info_clear_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::c_mail_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::c_mail_info* self = (hld::c_mail_info*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_left_shift_operation_result of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_left_shift_operation_result00
static int tolua_logic_hld_init_unit_get_left_shift_operation_result00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
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
   int tolua_ret = (int)  hld::init_unit::get_left_shift_operation_result(left_operator,right_operator);
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

/* method: get_bitwise_and_operation_result of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_bitwise_and_operation_result00
static int tolua_logic_hld_init_unit_get_bitwise_and_operation_result00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
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
   int tolua_ret = (int)  hld::init_unit::get_bitwise_and_operation_result(left_operator,right_operator);
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

/* method: get_bitwise_or_operation_result of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_bitwise_or_operation_result00
static int tolua_logic_hld_init_unit_get_bitwise_or_operation_result00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
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
   int tolua_ret = (int)  hld::init_unit::get_bitwise_or_operation_result(left_operator,right_operator);
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

/* method: get_item_bag_type of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_item_bag_type00
static int tolua_logic_hld_init_unit_get_item_bag_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int item_template_id = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   hld::e_bag_type tolua_ret = (hld::e_bag_type)  hld::init_unit::get_item_bag_type(item_template_id);
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

/* method: is_can_use_this_jewel_slot_on_this_equip_level of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_equip_level00
static int tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_equip_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
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
   bool tolua_ret = (bool)  hld::init_unit::is_can_use_this_jewel_slot_on_this_equip_level(equip_level,slot_index);
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

/* method: is_can_use_this_jewel_slot_on_this_vip_level of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_vip_level00
static int tolua_logic_hld_init_unit_is_can_use_this_jewel_slot_on_this_vip_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
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
   bool tolua_ret = (bool)  hld::init_unit::is_can_use_this_jewel_slot_on_this_vip_level(role_vip_level,slot_index);
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

/* method: get_flag_data of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_flag_data00
static int tolua_logic_hld_init_unit_get_flag_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
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
   int tolua_ret = (int)  hld::init_unit::get_flag_data(_flag_datas,_idx);
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

/* method: get_max_player_level of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_max_player_level00
static int tolua_logic_hld_init_unit_get_max_player_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  hld::init_unit::get_max_player_level();
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

/* method: get_max_grade_level of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_get_max_grade_level00
static int tolua_logic_hld_init_unit_get_max_grade_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int tolua_ret = (int)  hld::init_unit::get_max_grade_level();
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

/* method: is_big_endian of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_big_endian00
static int tolua_logic_hld_init_unit_is_big_endian00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   bool tolua_ret = (bool)  hld::init_unit::is_big_endian();
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

/* method: is_cross_server_big_player of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_is_cross_server_big_player00
static int tolua_logic_hld_init_unit_is_cross_server_big_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
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
   bool tolua_ret = (bool)  hld::init_unit::is_cross_server_big_player(player_type,is_begin_cross_activity,is_begin_cross_city_war);
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

/* method: init_item_data of class  hld::init_unit */
#ifndef TOLUA_DISABLE_tolua_logic_hld_init_unit_init_item_data00
static int tolua_logic_hld_init_unit_init_item_data00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::init_unit",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::s_item_info",0,&tolua_err)) ||
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
  hld::s_item_info* msg_data = ((hld::s_item_info*)  tolua_tousertype(tolua_S,2,0));
  int32 template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 slot = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 stack_count = ((int32)  tolua_tointeger(tolua_S,5,0));
  hld::e_bag_type container_type = ((hld::e_bag_type) (int)  tolua_tointeger(tolua_S,6,0));
  int32 is_activate = ((int32)  tolua_tointeger(tolua_S,7,0));
  int32 locked = ((int32)  tolua_tointeger(tolua_S,8,1));
  int32 item_over_time = ((int32)  tolua_tointeger(tolua_S,9,0));
  int32 item_const_att = ((int32)  tolua_tointeger(tolua_S,10,0));
  {
   bool tolua_ret = (bool)  hld::init_unit::init_item_data(*msg_data,template_id,slot,stack_count,container_type,is_activate,locked,item_over_time,item_const_att);
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

/* method: getInstance of class  hld::ws_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws_client_getInstance00
static int tolua_logic_hld_ws_client_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ws_client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::ws_client& tolua_ret = (hld::ws_client&)  hld::ws_client::getInstance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::ws_client");
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

/* method: send_to_dp_lua of class  hld::ws_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws_client_send_to_dp_lua00
static int tolua_logic_hld_ws_client_send_to_dp_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::ws_client",0,&tolua_err) ||
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
  hld::ws_client* self = (hld::ws_client*)  tolua_tousertype(tolua_S,1,0);
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

/* function: hld::dp2ws_rep_get_game_info_ws_lua */
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
   hld::dp2ws_rep_get_game_info_ws_lua(conn_index,server_id,begin_time,server_info_arr,gm_commond_arr);
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

/* function: hld::dp2ws_rep_get_role_info_ws_lua */
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
  int64* data_i64_ary = Mtolua_new_dim(int64, hld::e_role_i64_info_max);
#else
  int64* data_i64_ary = (int64*) malloc((hld::e_role_i64_info_max)*sizeof(int64));
#endif
#ifdef __cplusplus
  int* data_ary = Mtolua_new_dim(int, hld::e_role_info_max);
#else
  int* data_ary = (int*) malloc((hld::e_role_info_max)*sizeof(int));
#endif
#ifdef __cplusplus
  int* ws_data_ary = Mtolua_new_dim(int, hld::e_role_ws_info_max);
#else
  int* ws_data_ary = (int*) malloc((hld::e_role_ws_info_max)*sizeof(int));
#endif
  int is_login = ((int)  tolua_tointeger(tolua_S,11,0));
  int is_send_chat = ((int)  tolua_tointeger(tolua_S,12,0));
  int worship_value = ((int)  tolua_tointeger(tolua_S,13,0));
  int eresult = ((int)  tolua_tointeger(tolua_S,14,0));
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,8,hld::e_role_i64_info_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<hld::e_role_i64_info_max;i++)
    data_i64_ary[i] = ((int64)  tolua_tofieldinteger(tolua_S,8,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,9,hld::e_role_info_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<hld::e_role_info_max;i++)
    data_ary[i] = ((int)  tolua_tofieldinteger(tolua_S,9,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,10,hld::e_role_ws_info_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<hld::e_role_ws_info_max;i++)
    ws_data_ary[i] = ((int)  tolua_tofieldinteger(tolua_S,10,i+1,0));
   }
  }
  {
   hld::dp2ws_rep_get_role_info_ws_lua(conn_index,role_guid,account,role_name,role_appearance,role_first_kill_npc_id_arr,map_guid,data_i64_ary,data_ary,ws_data_ary,is_login,is_send_chat,worship_value,eresult);
  }
  {
   int i;
   for(i=0; i<hld::e_role_i64_info_max;i++)
    tolua_pushfieldinteger(tolua_S,8,i+1,(lua_Integer) data_i64_ary[i]);
  }
  {
   int i;
   for(i=0; i<hld::e_role_info_max;i++)
    tolua_pushfieldinteger(tolua_S,9,i+1,(lua_Integer) data_ary[i]);
  }
  {
   int i;
   for(i=0; i<hld::e_role_ws_info_max;i++)
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

/* function: hld::dp2ws_rep_load_arena_rank_lua */
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
   hld::dp2ws_rep_load_arena_rank_lua(conn_index,data_ptr,data_len,start_index);
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

/* function: hld::dp2ws_rep_load_all_ranking_info_lua */
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
   hld::dp2ws_rep_load_all_ranking_info_lua(conn_index,data_ptr,data_len,ranking_type);
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

/* function: hld::dp2ws_rep_load_one_ranking_info_lua */
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
   hld::dp2ws_rep_load_one_ranking_info_lua(conn_index,data_ptr,data_len,sender_guid,ranking_type,worship_add_value,worship_type,end_result);
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

/* function: hld::dp2ws_rep_load_one_worship_info_proc_lua */
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
   hld::dp2ws_rep_load_one_worship_info_proc_lua(conn_index,data_ptr,data_len,sender,is_load);
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

/* function: hld::dp2ws_rep_load_all_service_rank_info_lua */
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
   hld::dp2ws_rep_load_all_service_rank_info_lua(conn_index,data_ptr,data_len);
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

/* function: hld::dp2ws_rep_load_last_server_list_lua */
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
   hld::dp2ws_rep_load_last_server_list_lua(conn_index,server_id,last_server_list);
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

/* function: hld::dp2ws_gm_set_result_proc_lua */
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
   hld::dp2ws_gm_set_result_proc_lua(conn_index,listen_port,handle_index,result_code,result_msg);
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

/* function: hld::dp2ws_save_recharge_proc_lua */
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
   hld::dp2ws_save_recharge_proc_lua(conn_index,order_id,role_guid,game_goods_id,pay_price,success,order_num,payment_type,direct_diamond,client_uid,order_token);
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

/* function: hld::dp2ws_send_recharge_end_result_proc_lua */
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
   hld::dp2ws_send_recharge_end_result_proc_lua(conn_index,result_type,order_num,payment_type,goods_id,direct_diamond,role_guid,order_id);
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

/* function: hld::dp2ws_gm_get_server_info_proc_lua */
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
   hld::dp2ws_gm_get_server_info_proc_lua(conn_index,acc_num,role_num);
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

/* function: hld::dp2ws_gm_change_account_proc_lua */
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
   hld::dp2ws_gm_change_account_proc_lua(conn_index,change_res,listen_port,handle_index);
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

/* function: hld::dp2ws_gm_get_role_info_proc_lua */
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
   hld::dp2ws_gm_get_role_info_proc_lua(conn_index,data_ptr,data_len,listen_port,handle_index,check_type);
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

/* function: hld::dp2ws_gm_modify_role_info_proc_lua */
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
   hld::dp2ws_gm_modify_role_info_proc_lua(conn_index,role_guid,listen_port,handle_index);
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

/* function: hld::lua_dp2ws_load_red_package_end_proc */
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
   hld::lua_dp2ws_load_red_package_end_proc(conn_index,data_ptr,data_len);
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

/* function: hld::lua_dp2ws_load_red_package_receiver_end_proc */
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
   hld::lua_dp2ws_load_red_package_receiver_end_proc(conn_index,data_ptr,data_len);
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

/* method: get_instance of class  hld::arena_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_arena_mgr_ws_get_instance00
static int tolua_logic_hld_arena_mgr_ws_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::arena_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::arena_mgr_ws& tolua_ret = (hld::arena_mgr_ws&)  hld::arena_mgr_ws::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::arena_mgr_ws");
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

/* method: create_robot of class  hld::arena_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_arena_mgr_ws_create_robot00
static int tolua_logic_hld_arena_mgr_ws_create_robot00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::arena_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::arena_mgr_ws* self = (hld::arena_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: fill_empty_pos of class  hld::arena_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_arena_mgr_ws_fill_empty_pos00
static int tolua_logic_hld_arena_mgr_ws_fill_empty_pos00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::arena_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::arena_mgr_ws* self = (hld::arena_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_instance of class  hld::cross_server_city_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_city_war_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_city_war_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cross_server_city_war_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::cross_server_city_war_ws_mgr& tolua_ret = (hld::cross_server_city_war_ws_mgr&)  hld::cross_server_city_war_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::cross_server_city_war_ws_mgr");
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

/* method: is_in_game_time of class  hld::cross_server_city_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_city_war_ws_mgr_is_in_game_time00
static int tolua_logic_hld_cross_server_city_war_ws_mgr_is_in_game_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_city_war_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_city_war_ws_mgr* self = (hld::cross_server_city_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_cur_legion_in_rank of class  hld::cross_server_city_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_city_war_ws_mgr_get_cur_legion_in_rank00
static int tolua_logic_hld_cross_server_city_war_ws_mgr_get_cur_legion_in_rank00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_city_war_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_city_war_ws_mgr* self = (hld::cross_server_city_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* get function: role_guid of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_legion_member_info_role_guid
static int tolua_get_hld__s_legion_member_info_role_guid(lua_State* tolua_S)
{
  hld::s_legion_member_info* self = (hld::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->role_guid,"hld::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_guid of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_legion_member_info_role_guid
static int tolua_set_hld__s_legion_member_info_role_guid(lua_State* tolua_S)
{
  hld::s_legion_member_info* self = (hld::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: role_name of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_legion_member_info_role_name
static int tolua_get_hld__s_legion_member_info_role_name(lua_State* tolua_S)
{
  hld::s_legion_member_info* self = (hld::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->role_name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_name of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_legion_member_info_role_name
static int tolua_set_hld__s_legion_member_info_role_name(lua_State* tolua_S)
{
  hld::s_legion_member_info* self = (hld::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: gs_value of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_legion_member_info_gs_value
static int tolua_get_hld__s_legion_member_info_gs_value(lua_State* tolua_S)
{
  hld::s_legion_member_info* self = (hld::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gs_value'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->gs_value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gs_value of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_legion_member_info_gs_value
static int tolua_set_hld__s_legion_member_info_gs_value(lua_State* tolua_S)
{
  hld::s_legion_member_info* self = (hld::s_legion_member_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: data_ary of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_get_logic_hld_s_legion_member_info_data_ary
static int tolua_get_logic_hld_s_legion_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  hld::s_legion_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (hld::s_legion_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=hld::e_legion_member_info_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushinteger(tolua_S,(lua_Integer)self->data_ary[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: data_ary of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_set_logic_hld_s_legion_member_info_data_ary
static int tolua_set_logic_hld_s_legion_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  hld::s_legion_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (hld::s_legion_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=hld::e_legion_member_info_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->data_ary[tolua_index] = ((int64)  tolua_tointeger(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_legion_member_info_new00
static int tolua_logic_hld_s_legion_member_info_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_legion_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_legion_member_info* tolua_ret = (hld::s_legion_member_info*)  Mtolua_new((hld::s_legion_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_legion_member_info");
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

/* method: new_local of class  hld::s_legion_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_legion_member_info_new00_local
static int tolua_logic_hld_s_legion_member_info_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_legion_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_legion_member_info* tolua_ret = (hld::s_legion_member_info*)  Mtolua_new((hld::s_legion_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_legion_member_info");
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

/* method: get_applicant of class  hld::legion_ws_applicant_list */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_applicant_list_get_applicant00
static int tolua_logic_hld_legion_ws_applicant_list_get_applicant00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_applicant_list",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_applicant_list* self = (hld::legion_ws_applicant_list*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 applicant_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: del_applicant of class  hld::legion_ws_applicant_list */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_applicant_list_del_applicant00
static int tolua_logic_hld_legion_ws_applicant_list_del_applicant00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_applicant_list",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_applicant_list* self = (hld::legion_ws_applicant_list*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 applicant_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: del_applicant_with_join_legion of class  hld::legion_ws_applicant_list */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_applicant_list_del_applicant_with_join_legion00
static int tolua_logic_hld_legion_ws_applicant_list_del_applicant_with_join_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_applicant_list",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_applicant_list* self = (hld::legion_ws_applicant_list*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::legion_ws_answer */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_answer_get_instance00
static int tolua_logic_hld_legion_ws_answer_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::legion_ws_answer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::legion_ws_answer& tolua_ret = (hld::legion_ws_answer&)  hld::legion_ws_answer::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::legion_ws_answer");
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

/* method: get_legion_question_info of class  hld::legion_ws_answer */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_answer_get_legion_question_info00
static int tolua_logic_hld_legion_ws_answer_get_legion_question_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_answer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_answer* self = (hld::legion_ws_answer*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_legion_answer_rank_list of class  hld::legion_ws_answer */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_answer_send_legion_answer_rank_list00
static int tolua_logic_hld_legion_ws_answer_send_legion_answer_rank_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_answer",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_answer* self = (hld::legion_ws_answer*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_item_list_to_client of class  hld::legion_ws_warehouse */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_warehouse_send_item_list_to_client00
static int tolua_logic_hld_legion_ws_warehouse_send_item_list_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_warehouse",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"uint64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_warehouse* self = (hld::legion_ws_warehouse*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_log_list_to_client of class  hld::legion_ws_warehouse */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_warehouse_send_log_list_to_client00
static int tolua_logic_hld_legion_ws_warehouse_send_log_list_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_warehouse",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"uint64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_warehouse* self = (hld::legion_ws_warehouse*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_legion_info of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_legion_info00
static int tolua_logic_hld_legion_ws_set_legion_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::ELegionInfo info_index = ((hld::ELegionInfo) (int)  tolua_tointeger(tolua_S,2,0));
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

/* method: get_legion_gs_value of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_gs_value00
static int tolua_logic_hld_legion_ws_get_legion_gs_value00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_member of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_member00
static int tolua_logic_hld_legion_ws_get_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_member'", NULL);
#endif
  {
   hld::s_legion_member_info* tolua_ret = (hld::s_legion_member_info*)  self->get_member(player_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_legion_member_info");
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

/* method: add_member of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_add_member00
static int tolua_logic_hld_legion_ws_add_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::s_legion_member_info* member_info = ((hld::s_legion_member_info*)  tolua_tousertype(tolua_S,2,0));
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

/* method: add_member of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_add_member01
static int tolua_logic_hld_legion_ws_add_member01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::s_legion_member_info* member_info = ((hld::s_legion_member_info*)  tolua_tousertype(tolua_S,2,0));
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

/* method: add_member of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_add_member02
static int tolua_logic_hld_legion_ws_add_member02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::s_legion_member_info* member_info = ((hld::s_legion_member_info*)  tolua_tousertype(tolua_S,2,0));
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

/* method: construction_level_up of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_construction_level_up00
static int tolua_logic_hld_legion_ws_construction_level_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_in_this_legion of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_in_this_legion00
static int tolua_logic_hld_legion_ws_is_in_this_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 target_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_message_to_all_member_data_lua of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_message_to_all_member_data_lua00
static int tolua_logic_hld_legion_ws_send_message_to_all_member_data_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: leave_voie_channel of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_leave_voie_channel00
static int tolua_logic_hld_legion_ws_leave_voie_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_applicant_info_list of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_applicant_info_list00
static int tolua_logic_hld_legion_ws_send_applicant_info_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: del_member of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_del_member00
static int tolua_logic_hld_legion_ws_del_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: appoint of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_appoint00
static int tolua_logic_hld_legion_ws_appoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  hld::e_legion_job_title job_title = ((hld::e_legion_job_title) (int)  tolua_tointeger(tolua_S,3,0));
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

/* method: get_job_title of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_job_title00
static int tolua_logic_hld_legion_ws_get_job_title00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_job_title'", NULL);
#endif
  {
   hld::e_legion_job_title tolua_ret = (hld::e_legion_job_title)  self->get_job_title(player_guid);
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

/* method: get_legion_rank of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_rank00
static int tolua_logic_hld_legion_ws_get_legion_rank00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_chief of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_chief00
static int tolua_logic_hld_legion_ws_is_chief00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: is_assistant_chief of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_assistant_chief00
static int tolua_logic_hld_legion_ws_is_assistant_chief00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_legion_guid of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_guid00
static int tolua_logic_hld_legion_ws_get_legion_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_guid'", NULL);
#endif
  {
   hld::guid_64& tolua_ret = (hld::guid_64&)  self->get_legion_guid();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::guid_64");
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

/* method: set_announcement of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_announcement00
static int tolua_logic_hld_legion_ws_set_announcement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,1,&tolua_err) ||
     !tolua_isboolean(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,7,&tolua_err) || !tolua_isusertype(tolua_S,7,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  const char* content = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 content_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  bool need_send_mail = ((bool)  tolua_toboolean(tolua_S,4,true));
  bool need_save_db = ((bool)  tolua_toboolean(tolua_S,5,0));
  hld::guid_64 do_change_role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,6,0));
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

/* method: send_legion_announcement of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_legion_announcement00
static int tolua_logic_hld_legion_ws_send_legion_announcement00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_legion_name of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_legion_name00
static int tolua_logic_hld_legion_ws_get_legion_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_chief_guid of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_chief_guid00
static int tolua_logic_hld_legion_ws_get_chief_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_chief_guid'", NULL);
#endif
  {
   hld::guid_64 tolua_ret = (hld::guid_64)  self->get_chief_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((hld::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(hld::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
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

/* method: send_legion_attr_one of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_legion_attr_one00
static int tolua_logic_hld_legion_ws_send_legion_attr_one00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::ELegionInfo legion_info_index = ((hld::ELegionInfo) (int)  tolua_tointeger(tolua_S,2,0));
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

/* method: level_up_legion_skill of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_level_up_legion_skill00
static int tolua_logic_hld_legion_ws_level_up_legion_skill00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: join_legion_station_map of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_join_legion_station_map00
static int tolua_logic_hld_legion_ws_join_legion_station_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_legion_boss_map_info of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_send_legion_boss_map_info00
static int tolua_logic_hld_legion_ws_send_legion_boss_map_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_applicant_list of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_applicant_list00
static int tolua_logic_hld_legion_ws_get_applicant_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_applicant_list'", NULL);
#endif
  {
   hld::legion_ws_applicant_list& tolua_ret = (hld::legion_ws_applicant_list&)  self->get_applicant_list();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::legion_ws_applicant_list");
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

/* method: get_warehouse of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_warehouse00
static int tolua_logic_hld_legion_ws_get_warehouse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_warehouse'", NULL);
#endif
  {
   hld::legion_ws_warehouse& tolua_ret = (hld::legion_ws_warehouse&)  self->get_warehouse();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::legion_ws_warehouse");
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

/* method: set_auction_selling_check_start of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_auction_selling_check_start00
static int tolua_logic_hld_legion_ws_set_auction_selling_check_start00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: join_legion_station_map of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_join_legion_station_map01
static int tolua_logic_hld_legion_ws_join_legion_station_map01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: join_voice_channel of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_join_voice_channel00
static int tolua_logic_hld_legion_ws_join_voice_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: leave_voie_channel of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_leave_voie_channel01
static int tolua_logic_hld_legion_ws_leave_voie_channel01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: change_legion_name of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_change_legion_name00
static int tolua_logic_hld_legion_ws_change_legion_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: set_next_bid_time of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_set_next_bid_time00
static int tolua_logic_hld_legion_ws_set_next_bid_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_can_bid of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_is_can_bid00
static int tolua_logic_hld_legion_ws_is_can_bid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_all_member_guid of class  hld::legion_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_get_all_member_guid00
static int tolua_logic_hld_legion_ws_get_all_member_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws* self = (hld::legion_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_instance of class  hld::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_get_instance00
static int tolua_logic_hld_city_war_territory_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::city_war_territory_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::city_war_territory_mgr& tolua_ret = (hld::city_war_territory_mgr&)  hld::city_war_territory_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::city_war_territory_mgr");
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

/* method: bid_one_territory of class  hld::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_bid_one_territory00
static int tolua_logic_hld_city_war_territory_mgr_bid_one_territory00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::city_war_territory_mgr* self = (hld::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 territory_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 bid_index = ((int32)  tolua_tointeger(tolua_S,4,0));
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,5,0));
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

/* method: set_is_prior_maintain of class  hld::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_set_is_prior_maintain00
static int tolua_logic_hld_city_war_territory_mgr_set_is_prior_maintain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::city_war_territory_mgr* self = (hld::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: req_cross_server_apply_city_war_bid of class  hld::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_req_cross_server_apply_city_war_bid00
static int tolua_logic_hld_city_war_territory_mgr_req_cross_server_apply_city_war_bid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::city_war_territory_mgr* self = (hld::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 territory_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int32 bid_index = ((int32)  tolua_tointeger(tolua_S,4,0));
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,5,0));
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

/* method: req_cross_server_territory_prior_maintain of class  hld::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_req_cross_server_territory_prior_maintain00
static int tolua_logic_hld_city_war_territory_mgr_req_cross_server_territory_prior_maintain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::city_war_territory_mgr* self = (hld::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legin_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifdef __cplusplus
  int32* TerritoryIds = Mtolua_new_dim(int32, cross_server_territory_city_max);
#else
  int32* TerritoryIds = (int32*) malloc((cross_server_territory_city_max)*sizeof(int32));
#endif
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,4,0));
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

/* method: req_cross_server_legion_city_war_info of class  hld::city_war_territory_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_city_war_territory_mgr_req_cross_server_legion_city_war_info00
static int tolua_logic_hld_city_war_territory_mgr_req_cross_server_legion_city_war_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::city_war_territory_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::city_war_territory_mgr* self = (hld::city_war_territory_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_overlord_legion of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_overlord_legion00
static int tolua_logic_hld_legion_ws_city_war_get_overlord_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_overlord_legion'", NULL);
#endif
  {
   hld::guid_64& tolua_ret = (hld::guid_64&)  self->get_overlord_legion();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::guid_64");
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

/* method: get_overlord_legion_name of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_overlord_legion_name00
static int tolua_logic_hld_legion_ws_city_war_get_overlord_legion_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_city_master_guid of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_city_master_guid00
static int tolua_logic_hld_legion_ws_city_war_get_city_master_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_city_master_guid'", NULL);
#endif
  {
   hld::guid_64 tolua_ret = (hld::guid_64)  self->get_city_master_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((hld::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(hld::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
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

/* method: get_city_master_name of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_get_city_master_name00
static int tolua_logic_hld_legion_ws_city_war_get_city_master_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_during_city_war of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_is_during_city_war00
static int tolua_logic_hld_legion_ws_city_war_is_during_city_war00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64 legion_guid = *((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: is_during_bid_time of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_is_during_bid_time00
static int tolua_logic_hld_legion_ws_city_war_is_during_bid_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_territory_info_one of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_send_territory_info_one00
static int tolua_logic_hld_legion_ws_city_war_send_territory_info_one00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_city_war_info_all_msg of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_send_city_war_info_all_msg00
static int tolua_logic_hld_legion_ws_city_war_send_city_war_info_all_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: is_banquet_holding of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_is_banquet_holding00
static int tolua_logic_hld_legion_ws_city_war_is_banquet_holding00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
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

/* method: take_part_in_banquet of class  hld::legion_ws_city_war */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_city_war_take_part_in_banquet00
static int tolua_logic_hld_legion_ws_city_war_take_part_in_banquet00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_city_war",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_city_war* self = (hld::legion_ws_city_war*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_instance00
static int tolua_logic_hld_legion_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::legion_ws_mgr& tolua_ret = (hld::legion_ws_mgr&)  hld::legion_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::legion_ws_mgr");
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

/* method: del_legion of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_del_legion00
static int tolua_logic_hld_legion_ws_mgr_del_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,5,&tolua_err) || !tolua_isusertype(tolua_S,5,"const login_fixed_data",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool need_del_db = ((bool)  tolua_toboolean(tolua_S,3,0));
  hld::guid_64 del_role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,4,0));
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

/* method: get_legion of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_legion00
static int tolua_logic_hld_legion_ws_mgr_get_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion'", NULL);
#endif
  {
   hld::legion_ws* tolua_ret = (hld::legion_ws*)  self->get_legion(legion_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::legion_ws");
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

/* method: create_legion_begin of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_create_legion_begin00
static int tolua_logic_hld_legion_ws_mgr_create_legion_begin00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
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
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: check_can_join_legion of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_check_can_join_legion00
static int tolua_logic_hld_legion_ws_mgr_check_can_join_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::legion_ws",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::legion_ws* legion_ref = ((const hld::legion_ws*)  tolua_tousertype(tolua_S,2,0));
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
  hld::client_session* op_member_session = ((hld::client_session*)  tolua_tousertype(tolua_S,4,0));
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

/* method: check_can_leave_legion of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_check_can_leave_legion00
static int tolua_logic_hld_legion_ws_mgr_check_can_leave_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::legion_ws",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::legion_ws* legion_ref = ((const hld::legion_ws*)  tolua_tousertype(tolua_S,2,0));
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
  hld::client_session* op_member_session = ((hld::client_session*)  tolua_tousertype(tolua_S,4,0));
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

/* method: generate_player_legion_info of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_generate_player_legion_info00
static int tolua_logic_hld_legion_ws_mgr_generate_player_legion_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::s_legion_member_info",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,4,"legion_ws_applicant",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  hld::s_legion_member_info* member_info = ((hld::s_legion_member_info*)  tolua_tousertype(tolua_S,3,0));
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

/* method: send_legion_error_to_session of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_send_legion_error_to_session00
static int tolua_logic_hld_legion_ws_mgr_send_legion_error_to_session00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  hld::e_legion_error err_type = ((hld::e_legion_error) (int)  tolua_tointeger(tolua_S,3,0));
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

/* method: get_unit_legion of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_unit_legion00
static int tolua_logic_hld_legion_ws_mgr_get_unit_legion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* unit_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_unit_legion'", NULL);
#endif
  {
   hld::legion_ws* tolua_ret = (hld::legion_ws*)  self->get_unit_legion(*unit_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::legion_ws");
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

/* method: add_into_auto_accept_new_member_ordered_legion_list of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_add_into_auto_accept_new_member_ordered_legion_list00
static int tolua_logic_hld_legion_ws_mgr_add_into_auto_accept_new_member_ordered_legion_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int64 legion_fighting_power = ((int64)  tolua_tointeger(tolua_S,2,0));
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: del_from_auto_accept_new_member_ordered_legion_list of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_del_from_auto_accept_new_member_ordered_legion_list00
static int tolua_logic_hld_legion_ws_mgr_del_from_auto_accept_new_member_ordered_legion_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_legion_city_war of class  hld::legion_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_legion_ws_mgr_get_legion_city_war00
static int tolua_logic_hld_legion_ws_mgr_get_legion_city_war00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::legion_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::legion_ws_mgr* self = (hld::legion_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_legion_city_war'", NULL);
#endif
  {
   hld::legion_ws_city_war& tolua_ret = (hld::legion_ws_city_war&)  self->get_legion_city_war();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::legion_ws_city_war");
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

/* function: hld::c2ws_req_get_legion_list_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_legion_list_logic00
static int tolua_logic_hld_c2ws_req_get_legion_list_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 start_index = ((int32)  tolua_tointeger(tolua_S,2,0));
  int64 base_fighting_power = ((int64)  tolua_tointeger(tolua_S,3,0));
  int32 only_get_auto_accpet_new_member_legion = ((int32)  tolua_tointeger(tolua_S,4,0));
  {
   hld::c2ws_req_get_legion_list_logic(session,start_index,base_fighting_power,only_get_auto_accpet_new_member_legion);
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

/* function: hld::c2ws_req_legion_donate_money_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_legion_donate_money_logic00
static int tolua_logic_hld_c2ws_req_legion_donate_money_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 donate_times = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   hld::c2ws_req_legion_donate_money_logic(session,donate_times);
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

/* function: hld::c2ws_req_legion_donate_item_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_legion_donate_item_logic00
static int tolua_logic_hld_c2ws_req_legion_donate_item_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 donate_item_index = ((int32)  tolua_tointeger(tolua_S,2,0));
  int32 donate_times = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_legion_donate_item_logic(session,donate_item_index,donate_times);
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

/* function: hld::c2ws_req_get_event_list_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_event_list_logic00
static int tolua_logic_hld_c2ws_req_get_event_list_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 from_happen_time = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   hld::c2ws_req_get_event_list_logic(session,from_happen_time);
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

/* function: hld::c2ws_req_get_legion_boss_award_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_legion_boss_award_logic00
static int tolua_logic_hld_c2ws_req_get_legion_boss_award_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   hld::c2ws_req_get_legion_boss_award_logic(session);
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

/* function: hld::c2ws_req_get_banquet_info_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_banquet_info_logic00
static int tolua_logic_hld_c2ws_req_get_banquet_info_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   hld::c2ws_req_get_banquet_info_logic(session);
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

/* function: hld::c2ws_req_request_hold_banquet_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_request_hold_banquet_logic00
static int tolua_logic_hld_c2ws_req_request_hold_banquet_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 in_banquet_level = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   hld::c2ws_req_request_hold_banquet_logic(session,in_banquet_level);
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

/* function: hld::c2ws_req_enter_city_war_map_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_enter_city_war_map_logic00
static int tolua_logic_hld_c2ws_req_enter_city_war_map_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 in_terr_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  {
   hld::c2ws_req_enter_city_war_map_logic(session,in_terr_id);
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

/* function: hld::c2ws_req_city_war_operate_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_city_war_operate_logic00
static int tolua_logic_hld_c2ws_req_city_war_operate_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  int32 in_operate_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  int64 in_prop_npc_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  int32 in_terr_id = ((int32)  tolua_tointeger(tolua_S,4,0));
  {
   hld::c2ws_req_city_war_operate_logic(session,in_operate_type,in_prop_npc_guid,in_terr_id);
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

/* function: hld::c2ws_req_add_fuel_to_fire_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_add_fuel_to_fire_logic00
static int tolua_logic_hld_c2ws_req_add_fuel_to_fire_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   hld::c2ws_req_add_fuel_to_fire_logic(session);
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

/* function: hld::c2ws_req_send_legion_recruit_in_world_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_send_legion_recruit_in_world_logic00
static int tolua_logic_hld_c2ws_req_send_legion_recruit_in_world_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  bool in_is_rm_cd = ((bool)  tolua_toboolean(tolua_S,2,0));
  {
   hld::c2ws_req_send_legion_recruit_in_world_logic(session,in_is_rm_cd);
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

/* function: hld::c2ws_req_get_city_war_legion_rank_info_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_city_war_legion_rank_info_logic00
static int tolua_logic_hld_c2ws_req_get_city_war_legion_rank_info_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   hld::c2ws_req_get_city_war_legion_rank_info_logic(session);
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

/* function: hld::c2ws_req_draw_bonfire_daily_reward_logic */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_draw_bonfire_daily_reward_logic00
static int tolua_logic_hld_c2ws_req_draw_bonfire_daily_reward_logic00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  {
   hld::c2ws_req_draw_bonfire_daily_reward_logic(session);
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

/* function: hld::c2ws_req_legion_call_operate_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_legion_call_operate_lua00
static int tolua_logic_hld_c2ws_req_legion_call_operate_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_legion_call_operate_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_get_bonus_rewards_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_bonus_rewards_lua00
static int tolua_logic_hld_c2ws_req_get_bonus_rewards_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_get_bonus_rewards_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_stimulate_bonus_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_stimulate_bonus_lua00
static int tolua_logic_hld_c2ws_req_stimulate_bonus_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_stimulate_bonus_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_join_cross_city_war_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_join_cross_city_war_lua00
static int tolua_logic_hld_c2ws_req_join_cross_city_war_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_join_cross_city_war_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_goods_operate_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_goods_operate_lua00
static int tolua_logic_hld_c2ws_req_goods_operate_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_goods_operate_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_invite_player_join_team_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_invite_player_join_team_lua00
static int tolua_logic_hld_c2ws_req_invite_player_join_team_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_invite_player_join_team_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_ready_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_ready_lua00
static int tolua_logic_hld_c2ws_req_ready_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_ready_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_search_team_list_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_search_team_list_lua00
static int tolua_logic_hld_c2ws_req_search_team_list_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_search_team_list_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_get_team_list_by_aim_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_team_list_by_aim_lua00
static int tolua_logic_hld_c2ws_req_get_team_list_by_aim_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_get_team_list_by_aim_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_team_mem_operate_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_team_mem_operate_lua00
static int tolua_logic_hld_c2ws_req_team_mem_operate_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_team_mem_operate_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_get_best_record_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_get_best_record_lua00
static int tolua_logic_hld_c2ws_req_get_best_record_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_get_best_record_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_syc_nearly_state_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_syc_nearly_state_lua00
static int tolua_logic_hld_c2ws_req_syc_nearly_state_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_syc_nearly_state_lua(session,msg,msg_len);
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

/* function: hld::c2ws_req_operate_other_req_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2ws_req_operate_other_req_lua00
static int tolua_logic_hld_c2ws_req_operate_other_req_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2ws_req_operate_other_req_lua(session,msg,msg_len);
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

/* function: hld::c2s_confirm_receive_marry_task_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_c2s_confirm_receive_marry_task_lua00
static int tolua_logic_hld_c2s_confirm_receive_marry_task_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,1,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,2,0));
  int32 msg_len = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::c2s_confirm_receive_marry_task_lua(session,msg,msg_len);
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

/* method: include_invalid_ansi_str of class  hld::invalid_ansi_word */
#ifndef TOLUA_DISABLE_tolua_logic_hld_invalid_ansi_word_include_invalid_ansi_str00
static int tolua_logic_hld_invalid_ansi_word_include_invalid_ansi_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::invalid_ansi_word",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  hld::invalid_ansi_word::include_invalid_ansi_str(str);
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

/* method: is_valid_ansi_str of class  hld::invalid_ansi_word */
#ifndef TOLUA_DISABLE_tolua_logic_hld_invalid_ansi_word_is_valid_ansi_str00
static int tolua_logic_hld_invalid_ansi_word_is_valid_ansi_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::invalid_ansi_word",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string str = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  {
   bool tolua_ret = (bool)  hld::invalid_ansi_word::is_valid_ansi_str(str);
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

/* method: replace_invalid_ansi_str of class  hld::invalid_ansi_word */
#ifndef TOLUA_DISABLE_tolua_logic_hld_invalid_ansi_word_replace_invalid_ansi_str00
static int tolua_logic_hld_invalid_ansi_word_replace_invalid_ansi_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::invalid_ansi_word",0,&tolua_err) ||
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
   hld::invalid_ansi_word::replace_invalid_ansi_str(str,only_check_symbols);
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

/* get function: role_guid of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_role_guid
static int tolua_get_hld__s_team_member_info_role_guid(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->role_guid,"hld::guid_64");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_guid of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_role_guid
static int tolua_set_hld__s_team_member_info_role_guid(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_guid'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: role_index of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_role_index
static int tolua_get_hld__s_team_member_info_role_index(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_index'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->role_index);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_index of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_role_index
static int tolua_set_hld__s_team_member_info_role_index(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: role_name of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_role_name
static int tolua_get_hld__s_team_member_info_role_name(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'role_name'",NULL);
#endif
  tolua_pushstring(tolua_S,(const char*)self->role_name);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: role_name of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_role_name
static int tolua_set_hld__s_team_member_info_role_name(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: gs_value of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_hld__s_team_member_info_gs_value
static int tolua_get_hld__s_team_member_info_gs_value(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'gs_value'",NULL);
#endif
  tolua_pushinteger(tolua_S,(lua_Integer)self->gs_value);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: gs_value of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_hld__s_team_member_info_gs_value
static int tolua_set_hld__s_team_member_info_gs_value(lua_State* tolua_S)
{
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: data_ary of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_get_logic_hld_s_team_member_info_data_ary
static int tolua_get_logic_hld_s_team_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  hld::s_team_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (hld::s_team_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=hld::ETeamMemberInfo_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
 tolua_pushinteger(tolua_S,(lua_Integer)self->data_ary[tolua_index]);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: data_ary of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_set_logic_hld_s_team_member_info_data_ary
static int tolua_set_logic_hld_s_team_member_info_data_ary(lua_State* tolua_S)
{
 int tolua_index;
  hld::s_team_member_info* self;
 lua_pushstring(tolua_S,".self");
 lua_rawget(tolua_S,1);
 self = (hld::s_team_member_info*)  lua_touserdata(tolua_S,-1);
#ifndef TOLUA_RELEASE
 {
  tolua_Error tolua_err;
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in array indexing.",&tolua_err);
 }
#endif
 tolua_index = (int)tolua_tonumber(tolua_S,2,0);
#ifndef TOLUA_RELEASE
 if (tolua_index<0 || tolua_index>=hld::ETeamMemberInfo_max)
  tolua_error(tolua_S,"array indexing out of range.",NULL);
#endif
  self->data_ary[tolua_index] = ((int64)  tolua_tointeger(tolua_S,3,0));
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: is_valid of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_team_member_info_is_valid00
static int tolua_logic_hld_s_team_member_info_is_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::s_team_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::s_team_member_info* self = (hld::s_team_member_info*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_team_member_info_new00
static int tolua_logic_hld_s_team_member_info_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_team_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_team_member_info* tolua_ret = (hld::s_team_member_info*)  Mtolua_new((hld::s_team_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_team_member_info");
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

/* method: new_local of class  hld::s_team_member_info */
#ifndef TOLUA_DISABLE_tolua_logic_hld_s_team_member_info_new00_local
static int tolua_logic_hld_s_team_member_info_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::s_team_member_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::s_team_member_info* tolua_ret = (hld::s_team_member_info*)  Mtolua_new((hld::s_team_member_info)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_team_member_info");
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

/* method: del_member of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_del_member00
static int tolua_logic_hld_team_ws_del_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_member of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_member00
static int tolua_logic_hld_team_ws_get_member00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_member'", NULL);
#endif
  {
   hld::s_team_member_info* tolua_ret = (hld::s_team_member_info*)  self->get_member(player_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_team_member_info");
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

/* method: get_member_num of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_member_num00
static int tolua_logic_hld_team_ws_get_member_num00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_captain of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_set_captain00
static int tolua_logic_hld_team_ws_set_captain00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_captain_guid of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_captain_guid00
static int tolua_logic_hld_team_ws_get_captain_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_captain_guid'", NULL);
#endif
  {
   hld::guid_64 tolua_ret = (hld::guid_64)  self->get_captain_guid();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((hld::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(hld::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
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

/* method: transfer_all_member_to_scene of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_transfer_all_member_to_scene00
static int tolua_logic_hld_team_ws_transfer_all_member_to_scene00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
  int32 map_template_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  bool need_ready = ((bool)  tolua_toboolean(tolua_S,3,true));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transfer_all_member_to_scene'", NULL);
#endif
  {
   hld::e_team_error tolua_ret = (hld::e_team_error)  self->transfer_all_member_to_scene(map_template_id,need_ready);
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

/* method: get_team_state of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_team_state00
static int tolua_logic_hld_team_ws_get_team_state00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_team_state'", NULL);
#endif
  {
   hld::e_team_state tolua_ret = (hld::e_team_state)  self->get_team_state();
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

/* method: req_add_member_to_cs of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_req_add_member_to_cs00
static int tolua_logic_hld_team_ws_req_add_member_to_cs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: join_voice_channel of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_join_voice_channel00
static int tolua_logic_hld_team_ws_join_voice_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: leave_voie_channel of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_leave_voie_channel00
static int tolua_logic_hld_team_ws_leave_voie_channel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_member_list_name of class  hld::team_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_get_member_list_name00
static int tolua_logic_hld_team_ws_get_member_list_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws* self = (hld::team_ws*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_instance of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_get_instance00
static int tolua_logic_hld_team_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::team_ws_mgr& tolua_ret = (hld::team_ws_mgr&)  hld::team_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::team_ws_mgr");
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

/* method: get_team of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_get_team00
static int tolua_logic_hld_team_ws_mgr_get_team00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws_mgr* self = (hld::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 team_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_team'", NULL);
#endif
  {
   hld::team_ws* tolua_ret = (hld::team_ws*)  self->get_team(team_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::team_ws");
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

/* method: get_unit_team of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_get_unit_team00
static int tolua_logic_hld_team_ws_mgr_get_unit_team00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws_mgr* self = (hld::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* unit_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_unit_team'", NULL);
#endif
  {
   hld::team_ws* tolua_ret = (hld::team_ws*)  self->get_unit_team(*unit_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::team_ws");
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

/* method: send_team_error_to_session of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_send_team_error_to_session00
static int tolua_logic_hld_team_ws_mgr_send_team_error_to_session00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws_mgr* self = (hld::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  hld::e_team_error err_type = ((hld::e_team_error) (int)  tolua_tointeger(tolua_S,3,0));
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

/* method: player_stop_auto_match_team of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_player_stop_auto_match_team00
static int tolua_logic_hld_team_ws_mgr_player_stop_auto_match_team00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws_mgr* self = (hld::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64* player_guid = ((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: is_team_type_valid of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_is_team_type_valid00
static int tolua_logic_hld_team_ws_mgr_is_team_type_valid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws_mgr* self = (hld::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::e_team_type team_type = ((hld::e_team_type) (int)  tolua_tointeger(tolua_S,2,0));
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

/* method: req_condition_check_to_cs of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_req_condition_check_to_cs00
static int tolua_logic_hld_team_ws_mgr_req_condition_check_to_cs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws_mgr* self = (hld::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  hld::e_team_common_check_invoker invoker = ((hld::e_team_common_check_invoker) (int)  tolua_tointeger(tolua_S,3,0));
  hld::e_team_type team_type = ((hld::e_team_type) (int)  tolua_tointeger(tolua_S,4,0));
  int32 team_sub_type_id = ((int32)  tolua_tointeger(tolua_S,5,0));
  hld::guid_64 team_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,6,0));
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

/* method: req_no_team_mate_list of class  hld::team_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_team_ws_mgr_req_no_team_mate_list00
static int tolua_logic_hld_team_ws_mgr_req_no_team_mate_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::team_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::team_ws_mgr* self = (hld::team_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  hld::e_team_req_mate_type mate_type = ((hld::e_team_req_mate_type) (int)  tolua_tointeger(tolua_S,3,0));
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

/* method: get_map_template_id of class  hld::cs_map_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_system_get_map_template_id00
static int tolua_logic_hld_cs_map_system_get_map_template_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cs_map_system",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const hld::guid_64* map_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  {
   int32 tolua_ret = (int32)  hld::cs_map_system::get_map_template_id(*map_guid);
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

/* method: send_map_error_to_client of class  hld::cs_map_mgr_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_mgr_system_send_map_error_to_client00
static int tolua_logic_hld_cs_map_mgr_system_send_map_error_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cs_map_mgr_system",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"e_error_code",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  e_error_code error_code = *((e_error_code*)  tolua_tousertype(tolua_S,3,0));
  {
   hld::cs_map_mgr_system::send_map_error_to_client(session,error_code);
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

/* method: transfer_to_map of class  hld::cs_map_mgr_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_mgr_system_transfer_to_map00
static int tolua_logic_hld_cs_map_mgr_system_transfer_to_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cs_map_mgr_system",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 map_template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  hld::guid_64 map_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,4,0));
  int32 war_index = ((int32)  tolua_tointeger(tolua_S,5,0));
  int32 server_id = ((int32)  tolua_tointeger(tolua_S,6,0));
  {
   hld::cs_map_mgr_system::transfer_to_map(session,map_template_id,map_guid,war_index,server_id);
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

/* method: send_big_player of class  hld::cs_map_mgr_system */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs_map_mgr_system_send_big_player00
static int tolua_logic_hld_cs_map_mgr_system_send_big_player00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cs_map_mgr_system",0,&tolua_err) ||
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
   hld::cs_map_mgr_system::send_big_player(map_template_id,*fake_player_info,big_type);
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

/* method: get_instance of class  hld::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_get_instance00
static int tolua_logic_hld_pk_king_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::pk_king_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::pk_king_mgr& tolua_ret = (hld::pk_king_mgr&)  hld::pk_king_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::pk_king_mgr");
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

/* method: is_can_join of class  hld::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_is_can_join00
static int tolua_logic_hld_pk_king_mgr_is_can_join00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::pk_king_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::pk_king_mgr* self = (hld::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: is_in_ready_time of class  hld::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_is_in_ready_time00
static int tolua_logic_hld_pk_king_mgr_is_in_ready_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::pk_king_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::pk_king_mgr* self = (hld::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_in_notice_time of class  hld::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_is_in_notice_time00
static int tolua_logic_hld_pk_king_mgr_is_in_notice_time00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::pk_king_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::pk_king_mgr* self = (hld::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_valid_pkking_map of class  hld::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_get_valid_pkking_map00
static int tolua_logic_hld_pk_king_mgr_get_valid_pkking_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::pk_king_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::pk_king_mgr* self = (hld::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 map_war_idx = ((int32)  tolua_tointeger(tolua_S,2,0));
  hld::guid_64* map_guid = ((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: send_player_num_in_map_to_client of class  hld::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_send_player_num_in_map_to_client00
static int tolua_logic_hld_pk_king_mgr_send_player_num_in_map_to_client00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::pk_king_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::pk_king_mgr* self = (hld::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: break_line_login_msg of class  hld::pk_king_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_pk_king_mgr_break_line_login_msg00
static int tolua_logic_hld_pk_king_mgr_break_line_login_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::pk_king_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::pk_king_mgr* self = (hld::pk_king_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_pk_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::cross_server_pk_ws_mgr& tolua_ret = (hld::cross_server_pk_ws_mgr&)  hld::cross_server_pk_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::cross_server_pk_ws_mgr");
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

/* method: is_can_join_with_cross of class  hld::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_is_can_join_with_cross00
static int tolua_logic_hld_cross_server_pk_ws_mgr_is_can_join_with_cross00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_pk_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_pk_ws_mgr* self = (hld::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_to_gate_transfer_map of class  hld::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_send_to_gate_transfer_map00
static int tolua_logic_hld_cross_server_pk_ws_mgr_send_to_gate_transfer_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,6,&tolua_err) || !tolua_isusertype(tolua_S,6,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_pk_ws_mgr* self = (hld::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 class_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
  int32 activity_type = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 server_id = ((int32)  tolua_tointeger(tolua_S,5,0));
  hld::guid_64 legion_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,6,0));
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

/* method: get_pk_king_map_guid of class  hld::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_get_pk_king_map_guid00
static int tolua_logic_hld_cross_server_pk_ws_mgr_get_pk_king_map_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_pk_ws_mgr* self = (hld::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 class_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  hld::guid_64* map_guid = ((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: get_map_temp_id_with_class_type of class  hld::cross_server_pk_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_pk_ws_mgr_get_map_temp_id_with_class_type00
static int tolua_logic_hld_cross_server_pk_ws_mgr_get_map_temp_id_with_class_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_pk_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_pk_ws_mgr* self = (hld::cross_server_pk_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_instance of class  hld::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_get_instance00
static int tolua_logic_hld_crystal_fairyland_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::crystal_fairyland_mgr& tolua_ret = (hld::crystal_fairyland_mgr&)  hld::crystal_fairyland_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::crystal_fairyland_mgr");
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

/* method: is_activity_open of class  hld::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_is_activity_open00
static int tolua_logic_hld_crystal_fairyland_mgr_is_activity_open00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::crystal_fairyland_mgr* self = (hld::crystal_fairyland_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: is_can_join of class  hld::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_is_can_join00
static int tolua_logic_hld_crystal_fairyland_mgr_is_can_join00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::crystal_fairyland_mgr* self = (hld::crystal_fairyland_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_valid_shui_jing_map of class  hld::crystal_fairyland_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_crystal_fairyland_mgr_get_valid_shui_jing_map00
static int tolua_logic_hld_crystal_fairyland_mgr_get_valid_shui_jing_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::crystal_fairyland_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::crystal_fairyland_mgr* self = (hld::crystal_fairyland_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 map_war_idx = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_valid_shui_jing_map'", NULL);
#endif
  {
   hld::guid_64 tolua_ret = (hld::guid_64)  self->get_valid_shui_jing_map(map_war_idx);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((hld::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(hld::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
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

/* method: get_instance of class  hld::broken_sky_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_broken_sky_mgr_get_instance00
static int tolua_logic_hld_broken_sky_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::broken_sky_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::broken_sky_mgr& tolua_ret = (hld::broken_sky_mgr&)  hld::broken_sky_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::broken_sky_mgr");
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

/* method: is_can_join of class  hld::broken_sky_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_broken_sky_mgr_is_can_join00
static int tolua_logic_hld_broken_sky_mgr_is_can_join00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::broken_sky_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::broken_sky_mgr* self = (hld::broken_sky_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_instance of class  hld::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_get_instance00
static int tolua_logic_hld_world_boss_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::world_boss_ws_mgr& tolua_ret = (hld::world_boss_ws_mgr&)  hld::world_boss_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::world_boss_ws_mgr");
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

/* method: req_frist_kill_world_boss_prize of class  hld::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_req_frist_kill_world_boss_prize00
static int tolua_logic_hld_world_boss_ws_mgr_req_frist_kill_world_boss_prize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_boss_ws_mgr* self = (hld::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* req_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_damage_list of class  hld::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_send_damage_list00
static int tolua_logic_hld_world_boss_ws_mgr_send_damage_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_boss_ws_mgr* self = (hld::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_strategy_world_boss_info of class  hld::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_send_strategy_world_boss_info00
static int tolua_logic_hld_world_boss_ws_mgr_send_strategy_world_boss_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_boss_ws_mgr* self = (hld::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* legion_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: send_world_boss_info of class  hld::world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_boss_ws_mgr_send_world_boss_info00
static int tolua_logic_hld_world_boss_ws_mgr_send_world_boss_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_boss_ws_mgr* self = (hld::world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* legion_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: get_instance of class  hld::cross_server_harry_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_harry_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_harry_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cross_server_harry_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::cross_server_harry_ws_mgr& tolua_ret = (hld::cross_server_harry_ws_mgr&)  hld::cross_server_harry_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::cross_server_harry_ws_mgr");
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

/* method: is_can_join_harry_map of class  hld::cross_server_harry_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_harry_ws_mgr_is_can_join_harry_map00
static int tolua_logic_hld_cross_server_harry_ws_mgr_is_can_join_harry_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_harry_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_harry_ws_mgr* self = (hld::cross_server_harry_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session_ptr = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_valid_harry_map of class  hld::cross_server_harry_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_harry_ws_mgr_get_valid_harry_map00
static int tolua_logic_hld_cross_server_harry_ws_mgr_get_valid_harry_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_harry_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_harry_ws_mgr* self = (hld::cross_server_harry_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64* map_guid = ((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::web_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_web_client_get_instance00
static int tolua_logic_hld_web_client_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::web_client",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::web_client& tolua_ret = (hld::web_client&)  hld::web_client::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::web_client");
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

/* method: send_activate_code of class  hld::web_client */
#ifndef TOLUA_DISABLE_tolua_logic_hld_web_client_send_activate_code00
static int tolua_logic_hld_web_client_send_activate_code00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::web_client",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::web_client* self = (hld::web_client*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: del_relation of class  hld::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_del_relation00
static int tolua_logic_hld_relation_mgr_del_relation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::relation_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::relation_mgr* self = (hld::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* unit_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  hld::e_relationlist_type type = ((hld::e_relationlist_type) (int)  tolua_tointeger(tolua_S,3,0));
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

/* method: find_player_to_add_friend of class  hld::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_find_player_to_add_friend00
static int tolua_logic_hld_relation_mgr_find_player_to_add_friend00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::relation_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::relation_mgr* self = (hld::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: find_player_to_add_friend_by_pattern of class  hld::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_find_player_to_add_friend_by_pattern00
static int tolua_logic_hld_relation_mgr_find_player_to_add_friend_by_pattern00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::relation_mgr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::relation_mgr* self = (hld::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_relation of class  hld::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_add_relation00
static int tolua_logic_hld_relation_mgr_add_relation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::relation_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::relation_mgr* self = (hld::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::e_relationlist_type relation_type = ((hld::e_relationlist_type) (int)  tolua_tointeger(tolua_S,2,0));
  const hld::guid_64* guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: del_relation_to_dp of class  hld::relation_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_relation_mgr_del_relation_to_dp00
static int tolua_logic_hld_relation_mgr_del_relation_to_dp00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::relation_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::relation_mgr* self = (hld::relation_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  hld::guid_64 target_role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: get_instance of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_get_instance00
static int tolua_logic_hld_marry_mgr_ws_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::marry_mgr_ws& tolua_ret = (hld::marry_mgr_ws&)  hld::marry_mgr_ws::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::marry_mgr_ws");
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

/* method: c2ws_get_married_reward of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_get_married_reward00
static int tolua_logic_hld_marry_mgr_ws_c2ws_get_married_reward00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::marry_mgr_ws* self = (hld::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: c2ws_get_marry_mission of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_get_marry_mission00
static int tolua_logic_hld_marry_mgr_ws_c2ws_get_marry_mission00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::marry_mgr_ws* self = (hld::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: req_check_marry_with_sb of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_req_check_marry_with_sb00
static int tolua_logic_hld_marry_mgr_ws_req_check_marry_with_sb00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::marry_mgr_ws* self = (hld::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* main_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const hld::guid_64* sub_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: c2ws_reply_marry_choice of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_reply_marry_choice00
static int tolua_logic_hld_marry_mgr_ws_c2ws_reply_marry_choice00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::marry_mgr_ws* self = (hld::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: c2ws_req_check_divorce of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_req_check_divorce00
static int tolua_logic_hld_marry_mgr_ws_c2ws_req_check_divorce00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::marry_mgr_ws* self = (hld::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* main_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  const hld::guid_64* sub_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: c2ws_reply_divorce_choice of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_c2ws_reply_divorce_choice00
static int tolua_logic_hld_marry_mgr_ws_c2ws_reply_divorce_choice00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::marry_mgr_ws* self = (hld::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: req_bless_one_couple of class  hld::marry_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_marry_mgr_ws_req_bless_one_couple00
static int tolua_logic_hld_marry_mgr_ws_req_bless_one_couple00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::marry_mgr_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::marry_mgr_ws* self = (hld::marry_mgr_ws*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* client_ptr = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  const hld::guid_64* target_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: c2ws_get_ranking_list_func of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_c2ws_get_ranking_list_func00
static int tolua_logic_hld_ranking_mgr_ws_c2ws_get_ranking_list_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::e_RankingIndex ranking_index = ((hld::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  const hld::guid_64* role_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
  {
   hld::ranking_mgr_ws::c2ws_get_ranking_list_func(ranking_index,*role_guid);
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

/* method: c2ws_get_my_rank_func of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_c2ws_get_my_rank_func00
static int tolua_logic_hld_ranking_mgr_ws_c2ws_get_my_rank_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::e_RankingIndex ranking_type = ((hld::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  const hld::guid_64* sender_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
  const hld::guid_64* legion_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,4,0));
  {
   hld::ranking_mgr_ws::c2ws_get_my_rank_func(ranking_type,*sender_guid,*legion_guid);
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

/* method: c2ws_get_player_ranking_info_func of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_c2ws_get_player_ranking_info_func00
static int tolua_logic_hld_ranking_mgr_ws_c2ws_get_player_ranking_info_func00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::e_RankingIndex ranking_type = ((hld::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  const hld::guid_64* sender_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
  const hld::guid_64* target_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,4,0));
  {
   hld::ranking_mgr_ws::c2ws_get_player_ranking_info_func(ranking_type,*sender_guid,*target_guid);
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

/* method: send_service_rank_first_player_to_client_by_type of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_send_service_rank_first_player_to_client_by_type00
static int tolua_logic_hld_ranking_mgr_ws_send_service_rank_first_player_to_client_by_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
  int32 send_type = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::ranking_mgr_ws::send_service_rank_first_player_to_client_by_type(session,send_type);
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

/* method: get_oracle_trial_rank_info of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_get_oracle_trial_rank_info00
static int tolua_logic_hld_ranking_mgr_ws_get_oracle_trial_rank_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::guid_64 play_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  {
   hld::ranking_mgr_ws::get_oracle_trial_rank_info(play_guid);
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

/* method: sync_player_arena_lua of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_sync_player_arena_lua00
static int tolua_logic_hld_ranking_mgr_ws_sync_player_arena_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
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
   hld::ranking_mgr_ws::sync_player_arena_lua(my_str,my_len,dst_str,dst_len,is_robot);
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

/* method: get_srv_avg_level of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_get_srv_avg_level00
static int tolua_logic_hld_ranking_mgr_ws_get_srv_avg_level00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   int32 tolua_ret = (int32)  hld::ranking_mgr_ws::get_srv_avg_level();
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

/* method: get_ranking_player_guid of class  hld::ranking_mgr_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ranking_mgr_ws_get_ranking_player_guid00
static int tolua_logic_hld_ranking_mgr_ws_get_ranking_player_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::ranking_mgr_ws",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::e_RankingIndex ranking_index = ((hld::e_RankingIndex) (int)  tolua_tointeger(tolua_S,2,0));
  int32 ranking_pos = ((int32)  tolua_tointeger(tolua_S,3,0));
  {
   hld::guid_64 tolua_ret = (hld::guid_64)  hld::ranking_mgr_ws::get_ranking_player_guid(ranking_index,ranking_pos);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((hld::guid_64)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(hld::guid_64));
     tolua_pushusertype(tolua_S,tolua_obj,"hld::guid_64");
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

/* method: get_instance of class  hld::cloud_shop_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cloud_shop_mgr_get_instance00
static int tolua_logic_hld_cloud_shop_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cloud_shop_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::cloud_shop_mgr& tolua_ret = (hld::cloud_shop_mgr&)  hld::cloud_shop_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::cloud_shop_mgr");
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

/* method: handler_client_request of class  hld::cloud_shop_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cloud_shop_mgr_handler_client_request00
static int tolua_logic_hld_cloud_shop_mgr_handler_client_request00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cloud_shop_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"e_client_req_type",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cloud_shop_mgr* self = (hld::cloud_shop_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* pSession = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::gain_treasure_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gain_treasure_ws_mgr_get_instance00
static int tolua_logic_hld_gain_treasure_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::gain_treasure_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::gain_treasure_ws_mgr& tolua_ret = (hld::gain_treasure_ws_mgr&)  hld::gain_treasure_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::gain_treasure_ws_mgr");
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

/* method: req_kill_record_end of class  hld::gain_treasure_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gain_treasure_ws_mgr_req_kill_record_end00
static int tolua_logic_hld_gain_treasure_ws_mgr_req_kill_record_end00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::gain_treasure_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::gain_treasure_ws_mgr* self = (hld::gain_treasure_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: req_gain_item_record of class  hld::gain_treasure_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gain_treasure_ws_mgr_req_gain_item_record00
static int tolua_logic_hld_gain_treasure_ws_mgr_req_gain_item_record00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::gain_treasure_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::gain_treasure_ws_mgr* self = (hld::gain_treasure_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::time_limit_activity_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_time_limit_activity_ws_mgr_get_instance00
static int tolua_logic_hld_time_limit_activity_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::time_limit_activity_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::time_limit_activity_ws_mgr& tolua_ret = (hld::time_limit_activity_ws_mgr&)  hld::time_limit_activity_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::time_limit_activity_ws_mgr");
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

/* method: get_rank_info of class  hld::time_limit_activity_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_time_limit_activity_ws_mgr_get_rank_info00
static int tolua_logic_hld_time_limit_activity_ws_mgr_get_rank_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::time_limit_activity_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::time_limit_activity_ws_mgr* self = (hld::time_limit_activity_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::e_time_limit_activity_type activity_type = ((hld::e_time_limit_activity_type) (int)  tolua_tointeger(tolua_S,2,0));
  hld::guid_64 role_uid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: get_activity_common_data_to_gate of class  hld::time_limit_activity_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_time_limit_activity_ws_mgr_get_activity_common_data_to_gate00
static int tolua_logic_hld_time_limit_activity_ws_mgr_get_activity_common_data_to_gate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::time_limit_activity_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::time_limit_activity_ws_mgr* self = (hld::time_limit_activity_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session_ptr = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::rename_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_rename_mgr_get_instance00
static int tolua_logic_hld_rename_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::rename_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::rename_mgr& tolua_ret = (hld::rename_mgr&)  hld::rename_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::rename_mgr");
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

/* method: change_player_name of class  hld::rename_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_rename_mgr_change_player_name00
static int tolua_logic_hld_rename_mgr_change_player_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::rename_mgr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::rename_mgr* self = (hld::rename_mgr*)  tolua_tousertype(tolua_S,1,0);
  string name = ((string)  tolua_tocppstring(tolua_S,2,0));
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: confirm_change_name of class  hld::rename_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_rename_mgr_confirm_change_name00
static int tolua_logic_hld_rename_mgr_confirm_change_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::rename_mgr",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::rename_mgr* self = (hld::rename_mgr*)  tolua_tousertype(tolua_S,1,0);
  bool confirm_type = ((bool)  tolua_toboolean(tolua_S,2,0));
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: get_instance of class  hld::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_get_instance00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::cross_server_world_boss_ws_mgr& tolua_ret = (hld::cross_server_world_boss_ws_mgr&)  hld::cross_server_world_boss_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::cross_server_world_boss_ws_mgr");
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

/* method: is_in_cross_boss_map of class  hld::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_is_in_cross_boss_map00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_is_in_cross_boss_map00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_world_boss_ws_mgr* self = (hld::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 map_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: is_can_join_with_cross of class  hld::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_is_can_join_with_cross00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_is_can_join_with_cross00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_world_boss_ws_mgr* self = (hld::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_world_boss_map_guid of class  hld::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_get_world_boss_map_guid00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_get_world_boss_map_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_world_boss_ws_mgr* self = (hld::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64* map_guid = ((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_cross_server_world_boss_map_temp_id of class  hld::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_get_cross_server_world_boss_map_temp_id00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_get_cross_server_world_boss_map_temp_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_world_boss_ws_mgr* self = (hld::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_kill_boss_show of class  hld::cross_server_world_boss_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_server_world_boss_ws_mgr_send_kill_boss_show00
static int tolua_logic_hld_cross_server_world_boss_ws_mgr_send_kill_boss_show00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::cross_server_world_boss_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::cross_server_world_boss_ws_mgr* self = (hld::cross_server_world_boss_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_get_instance00
static int tolua_logic_hld_assist_fight_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::assist_fight_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::assist_fight_mgr& tolua_ret = (hld::assist_fight_mgr&)  hld::assist_fight_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::assist_fight_mgr");
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

/* method: sync_assist_fight_info of class  hld::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info00
static int tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::assist_fight_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::assist_fight_mgr* self = (hld::assist_fight_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: sync_assist_fight_info of class  hld::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info01
static int tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::assist_fight_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  hld::assist_fight_mgr* self = (hld::assist_fight_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  bool is_login = ((bool)  tolua_toboolean(tolua_S,3,0));
  hld::guid_64 create_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,4,0));
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

/* method: req_assist_fight of class  hld::assist_fight_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_assist_fight_mgr_req_assist_fight00
static int tolua_logic_hld_assist_fight_mgr_req_assist_fight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::assist_fight_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::assist_fight_mgr* self = (hld::assist_fight_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  hld::guid_64 assist_fight_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: get_instance of class  hld::appearance_info_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_appearance_info_mgr_get_instance00
static int tolua_logic_hld_appearance_info_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::appearance_info_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::appearance_info_mgr& tolua_ret = (hld::appearance_info_mgr&)  hld::appearance_info_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::appearance_info_mgr");
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

/* method: req_save_appearance_info_to_id of class  hld::appearance_info_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_appearance_info_mgr_req_save_appearance_info_to_id00
static int tolua_logic_hld_appearance_info_mgr_req_save_appearance_info_to_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::appearance_info_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::s_client_uid",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::appearance_info_mgr* self = (hld::appearance_info_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::s_client_uid client_uid = *((hld::s_client_uid*)  tolua_tousertype(tolua_S,2,0));
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

/* method: req_load_appearance_info_by_id of class  hld::appearance_info_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_appearance_info_mgr_req_load_appearance_info_by_id00
static int tolua_logic_hld_appearance_info_mgr_req_load_appearance_info_by_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::appearance_info_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::s_client_uid",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::appearance_info_mgr* self = (hld::appearance_info_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::s_client_uid client_uid = *((hld::s_client_uid*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_instance of class  hld::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_get_instance00
static int tolua_logic_hld_element_war_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::element_war_ws_mgr& tolua_ret = (hld::element_war_ws_mgr&)  hld::element_war_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::element_war_ws_mgr");
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

/* method: send_req_element_war_role_info of class  hld::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_req_element_war_role_info00
static int tolua_logic_hld_element_war_ws_mgr_send_req_element_war_role_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::element_war_ws_mgr* self = (hld::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_team_sign_up of class  hld::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_team_sign_up00
static int tolua_logic_hld_element_war_ws_mgr_send_team_sign_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::element_war_ws_mgr* self = (hld::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_single_sign_up of class  hld::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_single_sign_up00
static int tolua_logic_hld_element_war_ws_mgr_send_single_sign_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::element_war_ws_mgr* self = (hld::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: send_close_sign_up of class  hld::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_send_close_sign_up00
static int tolua_logic_hld_element_war_ws_mgr_send_close_sign_up00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::element_war_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::client_session",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::element_war_ws_mgr* self = (hld::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::client_session* session = ((hld::client_session*)  tolua_tousertype(tolua_S,2,0));
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

/* method: break_line_login_msg of class  hld::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_break_line_login_msg00
static int tolua_logic_hld_element_war_ws_mgr_break_line_login_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::element_war_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::element_war_ws_mgr* self = (hld::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_mission_reward of class  hld::element_war_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_element_war_ws_mgr_get_mission_reward00
static int tolua_logic_hld_element_war_ws_mgr_get_mission_reward00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::element_war_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::element_war_ws_mgr* self = (hld::element_war_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_need_begin_cross_gm_common of class  hld::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_get_need_begin_cross_gm_common00
static int tolua_logic_hld_world_server_get_need_begin_cross_gm_common00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_server",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_server* self = (hld::world_server*)  tolua_tousertype(tolua_S,1,0);
  hld::e_need_server_cross gm_type = ((hld::e_need_server_cross) (int)  tolua_tointeger(tolua_S,2,0));
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

/* method: is_sky_island_server of class  hld::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_is_sky_island_server00
static int tolua_logic_hld_world_server_is_sky_island_server00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_server* self = (hld::world_server*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_server_id of class  hld::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_get_server_id00
static int tolua_logic_hld_world_server_get_server_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_server* self = (hld::world_server*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_cross_id of class  hld::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_get_cross_id00
static int tolua_logic_hld_world_server_get_cross_id00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_server",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_server* self = (hld::world_server*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_server_act_hidden of class  hld::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_set_server_act_hidden00
static int tolua_logic_hld_world_server_set_server_act_hidden00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_server",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_server* self = (hld::world_server*)  tolua_tousertype(tolua_S,1,0);
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

/* method: set_ws_loading_flag of class  hld::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_set_ws_loading_flag00
static int tolua_logic_hld_world_server_set_ws_loading_flag00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_server",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_server* self = (hld::world_server*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_to_gate_get_msg of class  hld::world_server */
#ifndef TOLUA_DISABLE_tolua_logic_hld_world_server_send_to_gate_get_msg00
static int tolua_logic_hld_world_server_send_to_gate_get_msg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::world_server",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::world_server* self = (hld::world_server*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_message_to_all_client_data_lua of class  hld::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_send_message_to_all_client_data_lua00
static int tolua_logic_hld_client_session_mgr_send_message_to_all_client_data_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session_mgr",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session_mgr* self = (hld::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: send_message_to_map_lua of class  hld::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_send_message_to_map_lua00
static int tolua_logic_hld_client_session_mgr_send_message_to_map_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session_mgr",0,&tolua_err) ||
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
  hld::client_session_mgr* self = (hld::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_session_array_used of class  hld::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_get_session_array_used00
static int tolua_logic_hld_client_session_mgr_get_session_array_used00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session_mgr* self = (hld::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_session_by_use_index of class  hld::client_session_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_client_session_mgr_get_session_by_use_index00
static int tolua_logic_hld_client_session_mgr_get_session_by_use_index00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::client_session_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::client_session_mgr* self = (hld::client_session_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 index = ((int32)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_session_by_use_index'", NULL);
#endif
  {
   hld::client_session* tolua_ret = (hld::client_session*)  self->get_session_by_use_index(index);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::client_session");
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

/* method: get_instance of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_instance00
static int tolua_logic_hld_event_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::event_ws_mgr& tolua_ret = (hld::event_ws_mgr&)  hld::event_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::event_ws_mgr");
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

/* method: send_notice_to_all of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_send_notice_to_all00
static int tolua_logic_hld_event_ws_mgr_send_notice_to_all00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"hld::guid_64",0,&tolua_err)) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  int32 notice_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  int32 notice_create_time = ((int32)  tolua_tointeger(tolua_S,3,0));
  hld::guid_64 sender_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,4,0));
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

/* method: send_mail_to_player_by_system of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_send_mail_to_player_by_system00
static int tolua_logic_hld_event_ws_mgr_send_mail_to_player_by_system00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,11,&tolua_err) || !tolua_isusertype(tolua_S,11,"const std::vector<hld::s_item_info>",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,12,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 terget_player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 target_server_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  std::string sender_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string title = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  std::string content_text = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  int32 money_typ1 = ((int32)  tolua_tointeger(tolua_S,7,0));
  int32 money_num1 = ((int32)  tolua_tointeger(tolua_S,8,0));
  int32 money_typ2 = ((int32)  tolua_tointeger(tolua_S,9,0));
  int32 money_num2 = ((int32)  tolua_tointeger(tolua_S,10,0));
  const std::vector<hld::s_item_info>* item_list = ((const std::vector<hld::s_item_info>*)  tolua_tousertype(tolua_S,11,0));
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

/* method: send_notice_with_param of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_send_notice_with_param00
static int tolua_logic_hld_event_ws_mgr_send_notice_with_param00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
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
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 sender_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_free_item of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_free_item00
static int tolua_logic_hld_event_ws_mgr_get_free_item00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_free_item'", NULL);
#endif
  {
   hld::s_item_info* tolua_ret = (hld::s_item_info*)  self->get_free_item();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"hld::s_item_info");
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

/* method: get_free_list of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_free_list00
static int tolua_logic_hld_event_ws_mgr_get_free_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_free_list'", NULL);
#endif
  {
   std::vector<hld::s_item_info>* tolua_ret = (std::vector<hld::s_item_info>*)  self->get_free_list();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"std::vector<hld::s_item_info>");
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

/* method: add_free_item of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_add_free_item00
static int tolua_logic_hld_event_ws_mgr_add_free_item00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"hld::s_item_info",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::s_item_info* free_item = ((hld::s_item_info*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_free_param of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_get_free_param00
static int tolua_logic_hld_event_ws_mgr_get_free_param00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_free_param of class  hld::event_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_event_ws_mgr_add_free_param00
static int tolua_logic_hld_event_ws_mgr_add_free_param00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::event_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::event_ws_mgr* self = (hld::event_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: get_random of class  hld::random_gen */
#ifndef TOLUA_DISABLE_tolua_logic_hld_random_gen_get_random00
static int tolua_logic_hld_random_gen_get_random00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::random_gen",0,&tolua_err) ||
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
   int tolua_ret = (int)  hld::random_gen::get_random(nmin,nmax);
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

/* method: get_instance of class  hld::big_player_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_big_player_ws_mgr_get_instance00
static int tolua_logic_hld_big_player_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::big_player_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::big_player_ws_mgr& tolua_ret = (hld::big_player_ws_mgr&)  hld::big_player_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::big_player_ws_mgr");
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

/* method: set_big_player_guid of class  hld::big_player_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_big_player_ws_mgr_set_big_player_guid00
static int tolua_logic_hld_big_player_ws_mgr_set_big_player_guid00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::big_player_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::big_player_ws_mgr* self = (hld::big_player_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::e_big_player_type big_type = ((hld::e_big_player_type) (int)  tolua_tointeger(tolua_S,2,0));
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,3,0));
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

/* method: get_big_player_type_with_ranking_type of class  hld::big_player_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_big_player_ws_mgr_get_big_player_type_with_ranking_type00
static int tolua_logic_hld_big_player_ws_mgr_get_big_player_type_with_ranking_type00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::big_player_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::big_player_ws_mgr* self = (hld::big_player_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* function: hld::lua_cs2ws_send_red_package_process */
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
   hld::lua_cs2ws_send_red_package_process(conn_index,data_ptr,data_len);
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

/* function: hld::lua_cs2ws_get_red_package_process */
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
   hld::lua_cs2ws_get_red_package_process(conn_index,data_ptr,data_len);
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

/* function: hld::lua_cs2ws_get_arena_rank */
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
   hld::lua_cs2ws_get_arena_rank(conn_index,data_ptr,data_len);
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

/* function: hld::lua_cs2ws_req_challenge */
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
   hld::lua_cs2ws_req_challenge(conn_index,data_ptr,data_len);
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

/* function: hld::lua_cs2ws_req_get_choose_list */
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
   hld::lua_cs2ws_req_get_choose_list(conn_index,data_ptr,data_len);
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

/* function: hld::lua_cs2ws_req_get_first_three */
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
   hld::lua_cs2ws_req_get_first_three(conn_index,data_ptr,data_len);
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

/* function: hld::lua_cs2ws_chellenge_over_req */
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
   hld::lua_cs2ws_chellenge_over_req(conn_index,data_ptr,data_len);
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

/* function: hld::lua_cs2ws_arena_sync_gs_value_proc */
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
   hld::lua_cs2ws_arena_sync_gs_value_proc(conn_index,data_ptr,data_len);
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

/* method: get_instance of class  hld::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_get_instance00
static int tolua_logic_hld_mail_ws_mgr_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::mail_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   hld::mail_ws_mgr& tolua_ret = (hld::mail_ws_mgr&)  hld::mail_ws_mgr::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"hld::mail_ws_mgr");
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

/* method: send_mail of class  hld::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_send_mail00
static int tolua_logic_hld_mail_ws_mgr_send_mail00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::mail_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const s_mail_info",0,&tolua_err)) ||
     !tolua_isusertype(tolua_S,5,"const hld::s_item_info",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::mail_ws_mgr* self = (hld::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 address_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
  int32 address_server_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  const s_mail_info* mail_info = ((const s_mail_info*)  tolua_tousertype(tolua_S,4,0));
  const hld::s_item_info* item_list = ((const hld::s_item_info*)  tolua_tousertype(tolua_S,5,0));
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

/* method: send_mail_system of class  hld::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_send_mail_system00
static int tolua_logic_hld_mail_ws_mgr_send_mail_system00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::mail_ws_mgr",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"hld::guid_64",0,&tolua_err)) ||
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
  hld::mail_ws_mgr* self = (hld::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
  hld::guid_64 player_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,2,0));
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

/* method: get_free_item_list of class  hld::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_get_free_item_list00
static int tolua_logic_hld_mail_ws_mgr_get_free_item_list00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::mail_ws_mgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::mail_ws_mgr* self = (hld::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: add_free_item of class  hld::mail_ws_mgr */
#ifndef TOLUA_DISABLE_tolua_logic_hld_mail_ws_mgr_add_free_item00
static int tolua_logic_hld_mail_ws_mgr_add_free_item00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"hld::mail_ws_mgr",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  hld::mail_ws_mgr* self = (hld::mail_ws_mgr*)  tolua_tousertype(tolua_S,1,0);
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

/* method: auction_sell_log of class  hld::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_auction_sell_log00
static int tolua_logic_hld_server_log_auction_sell_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::server_log",0,&tolua_err) ||
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
   hld::server_log::auction_sell_log(item_id,item_num,money_type,item_price);
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

/* method: auction_db_log of class  hld::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_auction_db_log00
static int tolua_logic_hld_server_log_auction_db_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::server_log",0,&tolua_err) ||
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
   hld::server_log::auction_db_log(*auction_info,auction_state,*role_info,*third_info,login_type);
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

/* method: auctionbid_log of class  hld::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_auctionbid_log00
static int tolua_logic_hld_server_log_auctionbid_log00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::server_log",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"login_fixed_data",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"s_unit_info",0,&tolua_err)) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,8,&tolua_err) || !tolua_isusertype(tolua_S,8,"const hld::guid_64",0,&tolua_err)) ||
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
  const hld::guid_64* buyer_guid = ((const hld::guid_64*)  tolua_tousertype(tolua_S,8,0));
  {
   hld::server_log::auctionbid_log(*third_info,*role_info,money_type,money_value,item_template_id,data_num,*buyer_guid);
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

/* method: serverAuction of class  hld::server_log */
#ifndef TOLUA_DISABLE_tolua_logic_hld_server_log_serverAuction00
static int tolua_logic_hld_server_log_serverAuction00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"hld::server_log",0,&tolua_err) ||
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
   hld::server_log::serverAuction(*lg_common_head,sellerAccountId,sellerRoleGuid,sellerRoleName,sellerRoleLevel,itemGuid,itemId,itemNum,auctionType,buyerAccountId,buyerRoleGuid,buyerRoleName,buyerRoleLevel,moneyType,moneyName,moneyValue,auctionState);
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

/* function: hld::cross::send_msg_to_ws */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cross_send_msg_to_ws00
static int tolua_logic_hld_cross_send_msg_to_ws00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     (tolua_isvaluenil(tolua_S,1,&tolua_err) || !tolua_isusertype(tolua_S,1,"hld::guid_64",0,&tolua_err)) ||
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
  hld::guid_64 role_guid = *((hld::guid_64*)  tolua_tousertype(tolua_S,1,0));
  int32 server_id = ((int32)  tolua_tointeger(tolua_S,2,0));
  uint32 dataType = ((uint32)  tolua_tointeger(tolua_S,3,0));
  const void* pdata = ((const void*)  tolua_touserdata(tolua_S,4,0));
  uint32 len = ((uint32)  tolua_tointeger(tolua_S,5,0));
  {
   hld::cross::send_msg_to_ws(role_guid,server_id,dataType,pdata,len);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"s_client_uid","hld::s_client_uid","",tolua_collect_hld__s_client_uid);
   #else
   tolua_cclass(tolua_S,"s_client_uid","hld::s_client_uid","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"guid_64","hld::guid_64","",tolua_collect_hld__guid_64);
   #else
   tolua_cclass(tolua_S,"guid_64","hld::guid_64","",NULL);
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
   tolua_cclass(tolua_S,"fvector","hld::fvector","",tolua_collect_hld__fvector);
   #else
   tolua_cclass(tolua_S,"fvector","hld::fvector","",NULL);
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
   tolua_cclass(tolua_S,"frotator","hld::frotator","",tolua_collect_hld__frotator);
   #else
   tolua_cclass(tolua_S,"frotator","hld::frotator","",NULL);
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
   tolua_cclass(tolua_S,"s_map_pos","hld::s_map_pos","",tolua_collect_hld__s_map_pos);
   #else
   tolua_cclass(tolua_S,"s_map_pos","hld::s_map_pos","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"s_map_pos");
    tolua_variable(tolua_S,"unit_location",tolua_get_hld__s_map_pos_unit_location,tolua_set_hld__s_map_pos_unit_location);
    tolua_variable(tolua_S,"unit_rotation",tolua_get_hld__s_map_pos_unit_rotation,tolua_set_hld__s_map_pos_unit_rotation);
    tolua_function(tolua_S,"new",tolua_logic_hld_s_map_pos_new00);
    tolua_function(tolua_S,"new_local",tolua_logic_hld_s_map_pos_new00_local);
    tolua_function(tolua_S,".call",tolua_logic_hld_s_map_pos_new00_local);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"client_session","hld::client_session","",NULL);
   tolua_beginmodule(tolua_S,"client_session");
    tolua_constant(tolua_S,"e_ss_logout",hld::client_session::e_ss_logout);
    tolua_constant(tolua_S,"e_ss_created",hld::client_session::e_ss_created);
    tolua_constant(tolua_S,"e_ss_login_ok",hld::client_session::e_ss_login_ok);
    tolua_constant(tolua_S,"e_ss_ingame",hld::client_session::e_ss_ingame);
    tolua_constant(tolua_S,"e_ss_queue",hld::client_session::e_ss_queue);
    tolua_constant(tolua_S,"e_ss_map_transfer",hld::client_session::e_ss_map_transfer);
    tolua_constant(tolua_S,"e_ss_cross_transfer",hld::client_session::e_ss_cross_transfer);
    tolua_constant(tolua_S,"e_ss_check_token",hld::client_session::e_ss_check_token);
    tolua_constant(tolua_S,"e_session_step_null",hld::client_session::e_session_step_null);
    tolua_constant(tolua_S,"e_session_step_login",hld::client_session::e_session_step_login);
    tolua_constant(tolua_S,"e_session_step_login_win",hld::client_session::e_session_step_login_win);
    tolua_constant(tolua_S,"e_session_step_enter_game",hld::client_session::e_session_step_enter_game);
    tolua_constant(tolua_S,"e_session_step_dp_load",hld::client_session::e_session_step_dp_load);
    tolua_constant(tolua_S,"e_session_step_send_enter_game_cs",hld::client_session::e_session_step_send_enter_game_cs);
    tolua_constant(tolua_S,"e_session_step_cs_enter_game",hld::client_session::e_session_step_cs_enter_game);
    tolua_constant(tolua_S,"e_session_step_send_enter_scene",hld::client_session::e_session_step_send_enter_scene);
    tolua_constant(tolua_S,"e_session_step_cs_enter_scene",hld::client_session::e_session_step_cs_enter_scene);
    tolua_constant(tolua_S,"e_pss_left",hld::client_session::e_pss_left);
    tolua_constant(tolua_S,"e_pss_leaving",hld::client_session::e_pss_leaving);
    tolua_constant(tolua_S,"e_pss_entering",hld::client_session::e_pss_entering);
    tolua_constant(tolua_S,"e_pss_entered",hld::client_session::e_pss_entered);
    tolua_constant(tolua_S,"e_pss_sys_transfer",hld::client_session::e_pss_sys_transfer);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"globle_data","hld::globle_data","",NULL);
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
   tolua_cclass(tolua_S,"c_mail_info","hld::c_mail_info","",tolua_collect_hld__c_mail_info);
   #else
   tolua_cclass(tolua_S,"c_mail_info","hld::c_mail_info","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"init_unit","hld::init_unit","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_role_pk_map_state_one_role",hld::e_role_pk_map_state_one_role);
   tolua_constant(tolua_S,"e_role_pk_map_state_wait_begin",hld::e_role_pk_map_state_wait_begin);
   tolua_constant(tolua_S,"e_role_pk_map_state_begin_war",hld::e_role_pk_map_state_begin_war);
   tolua_constant(tolua_S,"e_role_pk_map_state_end_war",hld::e_role_pk_map_state_end_war);
   tolua_constant(tolua_S,"e_role_pk_map_state_max",hld::e_role_pk_map_state_max);
   tolua_constant(tolua_S,"e_map_type_ui",hld::e_map_type_ui);
   tolua_constant(tolua_S,"e_map_type_new",hld::e_map_type_new);
   tolua_constant(tolua_S,"e_map_type_main",hld::e_map_type_main);
   tolua_constant(tolua_S,"e_map_type_battle",hld::e_map_type_battle);
   tolua_constant(tolua_S,"e_map_type_field",hld::e_map_type_field);
   tolua_constant(tolua_S,"e_map_type_big_map",hld::e_map_type_big_map);
   tolua_constant(tolua_S,"e_map_type_fly_battle",hld::e_map_type_fly_battle);
   tolua_constant(tolua_S,"e_map_type_exp_fuben",hld::e_map_type_exp_fuben);
   tolua_constant(tolua_S,"e_map_type_money_fuben",hld::e_map_type_money_fuben);
   tolua_constant(tolua_S,"e_map_type_demon_tower_fuben",hld::e_map_type_demon_tower_fuben);
   tolua_constant(tolua_S,"e_map_type_legion_boss",hld::e_map_type_legion_boss);
   tolua_constant(tolua_S,"e_map_type_city_war",hld::e_map_type_city_war);
   tolua_constant(tolua_S,"e_map_type_broken_sky",hld::e_map_type_broken_sky);
   tolua_constant(tolua_S,"e_map_type_king_of_pk",hld::e_map_type_king_of_pk);
   tolua_constant(tolua_S,"e_map_type_arena",hld::e_map_type_arena);
   tolua_constant(tolua_S,"e_map_type_temple",hld::e_map_type_temple);
   tolua_constant(tolua_S,"e_map_type_empire_treasure",hld::e_map_type_empire_treasure);
   tolua_constant(tolua_S,"e_map_type_crystak_dreamland",hld::e_map_type_crystak_dreamland);
   tolua_constant(tolua_S,"e_map_type_legion_bonfire",hld::e_map_type_legion_bonfire);
   tolua_constant(tolua_S,"e_map_type_boss_single",hld::e_map_type_boss_single);
   tolua_constant(tolua_S,"e_map_type_boss_home",hld::e_map_type_boss_home);
   tolua_constant(tolua_S,"e_map_type_boss_vip_home",hld::e_map_type_boss_vip_home);
   tolua_constant(tolua_S,"e_map_type_boss_island",hld::e_map_type_boss_island);
   tolua_constant(tolua_S,"e_map_type_belief_cloister",hld::e_map_type_belief_cloister);
   tolua_constant(tolua_S,"e_map_type_princess_guard_war",hld::e_map_type_princess_guard_war);
   tolua_constant(tolua_S,"e_map_type_lava_hellfire",hld::e_map_type_lava_hellfire);
   tolua_constant(tolua_S,"e_map_type_sea_of_hate",hld::e_map_type_sea_of_hate);
   tolua_constant(tolua_S,"e_map_type_abyss_of_fear",hld::e_map_type_abyss_of_fear);
   tolua_constant(tolua_S,"e_map_type_fallen_fantasy",hld::e_map_type_fallen_fantasy);
   tolua_constant(tolua_S,"e_map_type_star_test",hld::e_map_type_star_test);
   tolua_constant(tolua_S,"e_map_type_cross_server_pk",hld::e_map_type_cross_server_pk);
   tolua_constant(tolua_S,"e_map_type_cross_server_harry",hld::e_map_type_cross_server_harry);
   tolua_constant(tolua_S,"e_map_type_cross_ladder",hld::e_map_type_cross_ladder);
   tolua_constant(tolua_S,"e_map_type_oracle_trial_map",hld::e_map_type_oracle_trial_map);
   tolua_constant(tolua_S,"e_map_type_legion_station",hld::e_map_type_legion_station);
   tolua_constant(tolua_S,"e_map_type_appearance",hld::e_map_type_appearance);
   tolua_constant(tolua_S,"e_map_type_single_land_boss",hld::e_map_type_single_land_boss);
   tolua_constant(tolua_S,"e_map_type_single_assist_boss",hld::e_map_type_single_assist_boss);
   tolua_constant(tolua_S,"e_map_type_level_up_map",hld::e_map_type_level_up_map);
   tolua_constant(tolua_S,"e_map_type_element_war",hld::e_map_type_element_war);
   tolua_constant(tolua_S,"e_map_type_attack_city",hld::e_map_type_attack_city);
   tolua_constant(tolua_S,"e_map_type_single_monster",hld::e_map_type_single_monster);
   tolua_constant(tolua_S,"e_map_type_single_open_box",hld::e_map_type_single_open_box);
   tolua_constant(tolua_S,"e_map_type_max",hld::e_map_type_max);
   tolua_constant(tolua_S,"e_role_info_template_id",hld::e_role_info_template_id);
   tolua_constant(tolua_S,"e_role_info_server_id",hld::e_role_info_server_id);
   tolua_constant(tolua_S,"e_role_info_img_id",hld::e_role_info_img_id);
   tolua_constant(tolua_S,"e_role_info_class_type",hld::e_role_info_class_type);
   tolua_constant(tolua_S,"e_role_info_class_branch",hld::e_role_info_class_branch);
   tolua_constant(tolua_S,"e_role_info_gender",hld::e_role_info_gender);
   tolua_constant(tolua_S,"e_role_info_exp_level",hld::e_role_info_exp_level);
   tolua_constant(tolua_S,"e_role_info_vip_level",hld::e_role_info_vip_level);
   tolua_constant(tolua_S,"e_role_info_vip_experience_level",hld::e_role_info_vip_experience_level);
   tolua_constant(tolua_S,"e_role_info_recharge_num",hld::e_role_info_recharge_num);
   tolua_constant(tolua_S,"e_role_info_main_map_id",hld::e_role_info_main_map_id);
   tolua_constant(tolua_S,"e_role_info_main_pos_x",hld::e_role_info_main_pos_x);
   tolua_constant(tolua_S,"e_role_info_main_pos_y",hld::e_role_info_main_pos_y);
   tolua_constant(tolua_S,"e_role_info_main_pos_z",hld::e_role_info_main_pos_z);
   tolua_constant(tolua_S,"e_role_info_main_pos_yaw",hld::e_role_info_main_pos_yaw);
   tolua_constant(tolua_S,"e_role_info_main_line_id",hld::e_role_info_main_line_id);
   tolua_constant(tolua_S,"e_role_info_move_map_id",hld::e_role_info_move_map_id);
   tolua_constant(tolua_S,"e_role_info_move_pos_x",hld::e_role_info_move_pos_x);
   tolua_constant(tolua_S,"e_role_info_move_pos_y",hld::e_role_info_move_pos_y);
   tolua_constant(tolua_S,"e_role_info_move_pos_z",hld::e_role_info_move_pos_z);
   tolua_constant(tolua_S,"e_role_info_move_pos_yaw",hld::e_role_info_move_pos_yaw);
   tolua_constant(tolua_S,"e_role_info_move_line_id",hld::e_role_info_move_line_id);
   tolua_constant(tolua_S,"e_role_info_GS_value",hld::e_role_info_GS_value);
   tolua_constant(tolua_S,"e_role_info_upgrade_id",hld::e_role_info_upgrade_id);
   tolua_constant(tolua_S,"e_role_info_daily_done_num",hld::e_role_info_daily_done_num);
   tolua_constant(tolua_S,"e_role_info_against_done_num",hld::e_role_info_against_done_num);
   tolua_constant(tolua_S,"e_role_info_wing_showd_template_id",hld::e_role_info_wing_showd_template_id);
   tolua_constant(tolua_S,"e_role_info_mount_showd_template_id",hld::e_role_info_mount_showd_template_id);
   tolua_constant(tolua_S,"e_role_info_element_heart_drop_group",hld::e_role_info_element_heart_drop_group);
   tolua_constant(tolua_S,"e_role_info_arena_times",hld::e_role_info_arena_times);
   tolua_constant(tolua_S,"e_role_info_arena_vip_times",hld::e_role_info_arena_vip_times);
   tolua_constant(tolua_S,"e_role_info_arena_vip_buy_times",hld::e_role_info_arena_vip_buy_times);
   tolua_constant(tolua_S,"e_role_info_arena_winning_streak",hld::e_role_info_arena_winning_streak);
   tolua_constant(tolua_S,"e_role_info_legion_donate_earn_by_money",hld::e_role_info_legion_donate_earn_by_money);
   tolua_constant(tolua_S,"e_role_info_legion_donate_earn_by_item",hld::e_role_info_legion_donate_earn_by_item);
   tolua_constant(tolua_S,"e_role_info_worship_other_time",hld::e_role_info_worship_other_time);
   tolua_constant(tolua_S,"e_role_info_get_func_unlock_award_record",hld::e_role_info_get_func_unlock_award_record);
   tolua_constant(tolua_S,"e_role_info_vip_item_geted_level",hld::e_role_info_vip_item_geted_level);
   tolua_constant(tolua_S,"e_role_info_vip_free_revive_time",hld::e_role_info_vip_free_revive_time);
   tolua_constant(tolua_S,"e_role_info_element_heart_refine_time",hld::e_role_info_element_heart_refine_time);
   tolua_constant(tolua_S,"e_role_info_total_login_days",hld::e_role_info_total_login_days);
   tolua_constant(tolua_S,"e_role_info_continue_login_time",hld::e_role_info_continue_login_time);
   tolua_constant(tolua_S,"e_role_info_daily_activity_degree",hld::e_role_info_daily_activity_degree);
   tolua_constant(tolua_S,"e_role_info_daily_recharge_num",hld::e_role_info_daily_recharge_num);
   tolua_constant(tolua_S,"e_role_info_total_diamond_cost_num",hld::e_role_info_total_diamond_cost_num);
   tolua_constant(tolua_S,"e_role_info_daily_active_degree_get_flag",hld::e_role_info_daily_active_degree_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_recharge_num",hld::e_role_info_fund_recharge_num);
   tolua_constant(tolua_S,"e_role_info_fund_payment_flag",hld::e_role_info_fund_payment_flag);
   tolua_constant(tolua_S,"e_role_info_fund_time_get_flag",hld::e_role_info_fund_time_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_pride_get_flag",hld::e_role_info_fund_pride_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_reborn_get_flag",hld::e_role_info_fund_reborn_get_flag);
   tolua_constant(tolua_S,"e_role_info_fund_cost_num",hld::e_role_info_fund_cost_num);
   tolua_constant(tolua_S,"e_role_info_lord_of_city_worship_time",hld::e_role_info_lord_of_city_worship_time);
   tolua_constant(tolua_S,"e_role_info_lord_of_war_worship_time",hld::e_role_info_lord_of_war_worship_time);
   tolua_constant(tolua_S,"e_role_info_lord_of_sanctuary_worship_time",hld::e_role_info_lord_of_sanctuary_worship_time);
   tolua_constant(tolua_S,"e_role_info_lord_of_god_worship_time",hld::e_role_info_lord_of_god_worship_time);
   tolua_constant(tolua_S,"e_role_info_hope_value",hld::e_role_info_hope_value);
   tolua_constant(tolua_S,"e_role_info_del_time",hld::e_role_info_del_time);
   tolua_constant(tolua_S,"e_role_info_kill_player_num",hld::e_role_info_kill_player_num);
   tolua_constant(tolua_S,"e_role_info_spirit_value",hld::e_role_info_spirit_value);
   tolua_constant(tolua_S,"e_role_info_watch_count",hld::e_role_info_watch_count);
   tolua_constant(tolua_S,"e_role_info_crystal_dreamland_chests",hld::e_role_info_crystal_dreamland_chests);
   tolua_constant(tolua_S,"e_role_info_protect_mark_slot_open_num",hld::e_role_info_protect_mark_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_element_heart_slot_open_num",hld::e_role_info_element_heart_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_sprite_equip_slot_open_num",hld::e_role_info_sprite_equip_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_exclusive_flag",hld::e_role_info_exclusive_flag);
   tolua_constant(tolua_S,"e_role_info_red_package_geted_times",hld::e_role_info_red_package_geted_times);
   tolua_constant(tolua_S,"e_role_info_free_red_package_send_times",hld::e_role_info_free_red_package_send_times);
   tolua_constant(tolua_S,"e_role_info_world_boss_chests",hld::e_role_info_world_boss_chests);
   tolua_constant(tolua_S,"e_role_info_leave_map_id",hld::e_role_info_leave_map_id);
   tolua_constant(tolua_S,"e_role_info_assist_chests",hld::e_role_info_assist_chests);
   tolua_constant(tolua_S,"e_role_info_next_side_mission_main_trigger_id",hld::e_role_info_next_side_mission_main_trigger_id);
   tolua_constant(tolua_S,"e_role_info_show_fashion",hld::e_role_info_show_fashion);
   tolua_constant(tolua_S,"e_role_info_real_recharge_jewel_num",hld::e_role_info_real_recharge_jewel_num);
   tolua_constant(tolua_S,"e_role_info_relic_kill_num",hld::e_role_info_relic_kill_num);
   tolua_constant(tolua_S,"e_role_info_relic_exp_num",hld::e_role_info_relic_exp_num);
   tolua_constant(tolua_S,"e_role_info_head_frame",hld::e_role_info_head_frame);
   tolua_constant(tolua_S,"e_role_info_fund_pride_cost_value",hld::e_role_info_fund_pride_cost_value);
   tolua_constant(tolua_S,"e_role_info_tinder_prop_slot_open_num",hld::e_role_info_tinder_prop_slot_open_num);
   tolua_constant(tolua_S,"e_role_info_show_sky_suit_shape",hld::e_role_info_show_sky_suit_shape);
   tolua_constant(tolua_S,"e_role_info_move_server_id",hld::e_role_info_move_server_id);
   tolua_constant(tolua_S,"e_role_info_move_war_idex",hld::e_role_info_move_war_idex);
   tolua_constant(tolua_S,"e_role_info_goddess_slot_flag",hld::e_role_info_goddess_slot_flag);
   tolua_constant(tolua_S,"e_role_info_daily_cost",hld::e_role_info_daily_cost);
   tolua_constant(tolua_S,"e_role_info_marry_done_num",hld::e_role_info_marry_done_num);
   tolua_constant(tolua_S,"e_role_info_marry_dati_id",hld::e_role_info_marry_dati_id);
   tolua_constant(tolua_S,"e_role_info_marry_dati_num",hld::e_role_info_marry_dati_num);
   tolua_constant(tolua_S,"e_role_info_marry_dati_right_num",hld::e_role_info_marry_dati_right_num);
   tolua_constant(tolua_S,"e_role_info_marry_dati_current_answer",hld::e_role_info_marry_dati_current_answer);
   tolua_constant(tolua_S,"e_role_info_legion_dance_exp_num",hld::e_role_info_legion_dance_exp_num);
   tolua_constant(tolua_S,"e_role_info_legion_donate_num",hld::e_role_info_legion_donate_num);
   tolua_constant(tolua_S,"e_role_info_appearance_id",hld::e_role_info_appearance_id);
   tolua_constant(tolua_S,"e_role_info_is_show_vip",hld::e_role_info_is_show_vip);
   tolua_constant(tolua_S,"e_role_info_need_check_guide",hld::e_role_info_need_check_guide);
   tolua_constant(tolua_S,"e_role_info_enchant_show_type",hld::e_role_info_enchant_show_type);
   tolua_constant(tolua_S,"e_role_info_push_map_id",hld::e_role_info_push_map_id);
   tolua_constant(tolua_S,"e_role_info_max",hld::e_role_info_max);
   tolua_constant(tolua_S,"e_role_logic_info_bag_open",hld::e_role_logic_info_bag_open);
   tolua_constant(tolua_S,"e_role_logic_info_main_pk_mode",hld::e_role_logic_info_main_pk_mode);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_first_one",hld::e_role_logic_info_jewel_first_one);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_first_ten",hld::e_role_logic_info_jewel_first_ten);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_luck",hld::e_role_logic_info_jewel_luck);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_free",hld::e_role_logic_info_jewel_free);
   tolua_constant(tolua_S,"e_role_logic_info_element_first_one",hld::e_role_logic_info_element_first_one);
   tolua_constant(tolua_S,"e_role_logic_info_element_first_ten",hld::e_role_logic_info_element_first_ten);
   tolua_constant(tolua_S,"e_role_logic_info_element_luck",hld::e_role_logic_info_element_luck);
   tolua_constant(tolua_S,"e_role_logic_info_element_free",hld::e_role_logic_info_element_free);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_first_one",hld::e_role_logic_info_spirit_first_one);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_first_ten",hld::e_role_logic_info_spirit_first_ten);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_luck",hld::e_role_logic_info_spirit_luck);
   tolua_constant(tolua_S,"e_role_logic_info_spirit_free",hld::e_role_logic_info_spirit_free);
   tolua_constant(tolua_S,"e_role_logic_info_storage_open",hld::e_role_logic_info_storage_open);
   tolua_constant(tolua_S,"e_role_logic_info_first_look_legion",hld::e_role_logic_info_first_look_legion);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission",hld::e_role_logic_info_done_grade_mission);
   tolua_constant(tolua_S,"e_role_logic_info_god_hood_grade",hld::e_role_logic_info_god_hood_grade);
   tolua_constant(tolua_S,"e_role_logic_info_first_join_legion_award",hld::e_role_logic_info_first_join_legion_award);
   tolua_constant(tolua_S,"e_role_logic_info_is_get_kill_legion_boss_award",hld::e_role_logic_info_is_get_kill_legion_boss_award);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_1",hld::e_role_logic_info_done_grade_mission_1);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_2",hld::e_role_logic_info_done_grade_mission_2);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_3",hld::e_role_logic_info_done_grade_mission_3);
   tolua_constant(tolua_S,"e_role_logic_info_done_grade_mission_4",hld::e_role_logic_info_done_grade_mission_4);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_total_recharge",hld::e_role_logic_info_jewel_total_recharge);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_total_consume",hld::e_role_logic_info_jewel_total_consume);
   tolua_constant(tolua_S,"e_role_logic_info_cross_ladder_join_num",hld::e_role_logic_info_cross_ladder_join_num);
   tolua_constant(tolua_S,"e_role_logic_info_cross_ladder_win_num",hld::e_role_logic_info_cross_ladder_win_num);
   tolua_constant(tolua_S,"e_role_logic_info_cross_ladder_reward_tag",hld::e_role_logic_info_cross_ladder_reward_tag);
   tolua_constant(tolua_S,"e_role_logic_info_marry_times",hld::e_role_logic_info_marry_times);
   tolua_constant(tolua_S,"e_role_logic_info_marry_divorce_times",hld::e_role_logic_info_marry_divorce_times);
   tolua_constant(tolua_S,"e_role_logic_info_daily_lucky_times",hld::e_role_logic_info_daily_lucky_times);
   tolua_constant(tolua_S,"e_role_logic_info_zero_gs_up_start_value",hld::e_role_logic_info_zero_gs_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_mount_up_start_value",hld::e_role_logic_info_zero_mount_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_wing_up_start_value",hld::e_role_logic_info_zero_wing_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_equip_up_start_value",hld::e_role_logic_info_zero_equip_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_zero_spirit_up_start_value",hld::e_role_logic_info_zero_spirit_up_start_value);
   tolua_constant(tolua_S,"e_role_logic_info_helper_reward_send_times",hld::e_role_logic_info_helper_reward_send_times);
   tolua_constant(tolua_S,"e_role_logic_info_boss_home_cur_used_times",hld::e_role_logic_info_boss_home_cur_used_times);
   tolua_constant(tolua_S,"e_role_logic_info_gain_treasure_restore_num",hld::e_role_logic_info_gain_treasure_restore_num);
   tolua_constant(tolua_S,"e_role_logic_info_gain_treasure_restore_time",hld::e_role_logic_info_gain_treasure_restore_time);
   tolua_constant(tolua_S,"e_role_logic_info_sky_land_boss_restore_num",hld::e_role_logic_info_sky_land_boss_restore_num);
   tolua_constant(tolua_S,"e_role_logic_info_sky_land_boss_restore_time",hld::e_role_logic_info_sky_land_boss_restore_time);
   tolua_constant(tolua_S,"e_role_logic_info_boss_island_cur_kill_times",hld::e_role_logic_info_boss_island_cur_kill_times);
   tolua_constant(tolua_S,"e_role_logic_info_help_reward_send_times",hld::e_role_logic_info_help_reward_send_times);
   tolua_constant(tolua_S,"e_role_logic_info_sprite_equip_slot_open_flag",hld::e_role_logic_info_sprite_equip_slot_open_flag);
   tolua_constant(tolua_S,"e_role_logic_info_check_in_days",hld::e_role_logic_info_check_in_days);
   tolua_constant(tolua_S,"e_role_logic_info_check_in_repetition_num",hld::e_role_logic_info_check_in_repetition_num);
   tolua_constant(tolua_S,"e_role_logic_info_exp_raid_add_damage_gold",hld::e_role_logic_info_exp_raid_add_damage_gold);
   tolua_constant(tolua_S,"e_role_logic_info_exp_raid_add_damage_diamond",hld::e_role_logic_info_exp_raid_add_damage_diamond);
   tolua_constant(tolua_S,"e_role_logic_info_element_war_mission_tag",hld::e_role_logic_info_element_war_mission_tag);
   tolua_constant(tolua_S,"e_role_logic_info_element_war_buy_ticket",hld::e_role_logic_info_element_war_buy_ticket);
   tolua_constant(tolua_S,"e_role_logic_info_jewel_carve_lucky",hld::e_role_logic_info_jewel_carve_lucky);
   tolua_constant(tolua_S,"e_role_logic_info_choose_fashion_id",hld::e_role_logic_info_choose_fashion_id);
   tolua_constant(tolua_S,"e_role_logic_info_help_tip_send_times",hld::e_role_logic_info_help_tip_send_times);
   tolua_constant(tolua_S,"e_role_logic_info_is_have_new_server_mail",hld::e_role_logic_info_is_have_new_server_mail);
   tolua_constant(tolua_S,"e_role_logic_info_awaken_fetter",hld::e_role_logic_info_awaken_fetter);
   tolua_constant(tolua_S,"e_role_logic_info_offline_award_time",hld::e_role_logic_info_offline_award_time);
   tolua_constant(tolua_S,"e_role_logic_info_offline_award_random",hld::e_role_logic_info_offline_award_random);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_money_type",hld::e_role_logic_info_online_award_money_type);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_money_num",hld::e_role_logic_info_online_award_money_num);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_time",hld::e_role_logic_info_online_award_time);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_money_count",hld::e_role_logic_info_online_award_money_count);
   tolua_constant(tolua_S,"e_role_logic_info_online_award_item_count",hld::e_role_logic_info_online_award_item_count);
   tolua_constant(tolua_S,"e_role_logic_info_skill_level",hld::e_role_logic_info_skill_level);
   tolua_constant(tolua_S,"e_role_logic_info_search_value",hld::e_role_logic_info_search_value);
   tolua_constant(tolua_S,"e_role_logic_info_search_level",hld::e_role_logic_info_search_level);
   tolua_constant(tolua_S,"e_role_logic_info_max",hld::e_role_logic_info_max);
   tolua_constant(tolua_S,"e_role_box_map_next_award",hld::e_role_box_map_next_award);
   tolua_constant(tolua_S,"e_role_box_map_search_time",hld::e_role_box_map_search_time);
   tolua_constant(tolua_S,"e_role_box_map_middle_open_compensate",hld::e_role_box_map_middle_open_compensate);
   tolua_constant(tolua_S,"e_role_box_map_big_open_compensate",hld::e_role_box_map_big_open_compensate);
   tolua_constant(tolua_S,"e_role_box_map_special_refrush_compensate",hld::e_role_box_map_special_refrush_compensate);
   tolua_constant(tolua_S,"e_role_box_map_special_open_compensate",hld::e_role_box_map_special_open_compensate);
   tolua_constant(tolua_S,"e_role_box_map_max",hld::e_role_box_map_max);
   tolua_constant(tolua_S,"e_role_element_map_mop_up_count",hld::e_role_element_map_mop_up_count);
   tolua_constant(tolua_S,"e_role_element_map_max",hld::e_role_element_map_max);
   tolua_constant(tolua_S,"e_role_battle_map_mop_up_count",hld::e_role_battle_map_mop_up_count);
   tolua_constant(tolua_S,"e_role_battle_map_max",hld::e_role_battle_map_max);
   tolua_constant(tolua_S,"e_unit_attack_att_none",hld::e_unit_attack_att_none);
   tolua_constant(tolua_S,"e_unit_attack_att_strength",hld::e_unit_attack_att_strength);
   tolua_constant(tolua_S,"e_unit_attack_att_agility",hld::e_unit_attack_att_agility);
   tolua_constant(tolua_S,"e_unit_attack_att_intellect",hld::e_unit_attack_att_intellect);
   tolua_constant(tolua_S,"e_unit_attack_att_stamina",hld::e_unit_attack_att_stamina);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_min",hld::e_unit_attack_att_attack_min);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_max",hld::e_unit_attack_att_attack_max);
   tolua_constant(tolua_S,"e_unit_attack_att_armor",hld::e_unit_attack_att_armor);
   tolua_constant(tolua_S,"e_unit_attack_att_hp_max",hld::e_unit_attack_att_hp_max);
   tolua_constant(tolua_S,"e_unit_attack_att_hit_force",hld::e_unit_attack_att_hit_force);
   tolua_constant(tolua_S,"e_unit_attack_att_dodge_force",hld::e_unit_attack_att_dodge_force);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_rate",hld::e_unit_attack_att_critical_rate);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_armor_rate",hld::e_unit_attack_att_critical_armor_rate);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_attack_rate",hld::e_unit_attack_att_critical_attack_rate);
   tolua_constant(tolua_S,"e_unit_attack_att_critical_attack_relief",hld::e_unit_attack_att_critical_attack_relief);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_1",hld::e_unit_attack_att_att_attack_1);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_1",hld::e_unit_attack_att_att_armor_1);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_2",hld::e_unit_attack_att_att_attack_2);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_2",hld::e_unit_attack_att_att_armor_2);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_3",hld::e_unit_attack_att_att_attack_3);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_3",hld::e_unit_attack_att_att_armor_3);
   tolua_constant(tolua_S,"e_unit_attack_att_att_attack_4",hld::e_unit_attack_att_att_attack_4);
   tolua_constant(tolua_S,"e_unit_attack_att_att_armor_4",hld::e_unit_attack_att_att_armor_4);
   tolua_constant(tolua_S,"e_unit_attack_att_element_ability",hld::e_unit_attack_att_element_ability);
   tolua_constant(tolua_S,"e_unit_attack_att_element_resist",hld::e_unit_attack_att_element_resist);
   tolua_constant(tolua_S,"e_unit_attack_att_effect_damage",hld::e_unit_attack_att_effect_damage);
   tolua_constant(tolua_S,"e_unit_attack_att_effect_damage_relief",hld::e_unit_attack_att_effect_damage_relief);
   tolua_constant(tolua_S,"e_unit_attack_att_hit_prob",hld::e_unit_attack_att_hit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_un_hit_prob",hld::e_unit_attack_att_un_hit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_crit_prob",hld::e_unit_attack_att_crit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_un_crit_prob",hld::e_unit_attack_att_un_crit_prob);
   tolua_constant(tolua_S,"e_unit_attack_att_move_speed",hld::e_unit_attack_att_move_speed);
   tolua_constant(tolua_S,"e_unit_attack_att_un_dizzy",hld::e_unit_attack_att_un_dizzy);
   tolua_constant(tolua_S,"e_unit_attack_att_un_move_speed",hld::e_unit_attack_att_un_move_speed);
   tolua_constant(tolua_S,"e_unit_attack_att_un_frozen",hld::e_unit_attack_att_un_frozen);
   tolua_constant(tolua_S,"e_unit_attack_att_un_no_skill",hld::e_unit_attack_att_un_no_skill);
   tolua_constant(tolua_S,"e_unit_attack_att_magic",hld::e_unit_attack_att_magic);
   tolua_constant(tolua_S,"e_unit_attack_att_body_power",hld::e_unit_attack_att_body_power);
   tolua_constant(tolua_S,"e_unit_attack_att_exp_addition",hld::e_unit_attack_att_exp_addition);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_npc",hld::e_unit_attack_att_attack_npc);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_player",hld::e_unit_attack_att_attack_player);
   tolua_constant(tolua_S,"e_unit_attack_att_exp_add_all",hld::e_unit_attack_att_exp_add_all);
   tolua_constant(tolua_S,"e_unit_attack_att_be_attack_player",hld::e_unit_attack_att_be_attack_player);
   tolua_constant(tolua_S,"e_unit_attack_att_pure_attack",hld::e_unit_attack_att_pure_attack);
   tolua_constant(tolua_S,"e_unit_attack_att_pure_armor",hld::e_unit_attack_att_pure_armor);
   tolua_constant(tolua_S,"e_unit_attack_att_attack_multiple",hld::e_unit_attack_att_attack_multiple);
   tolua_constant(tolua_S,"e_unit_attack_att_flight_energy",hld::e_unit_attack_att_flight_energy);
   tolua_constant(tolua_S,"e_unit_attack_att_max",hld::e_unit_attack_att_max);
   tolua_constant(tolua_S,"e_unit_game_att_movement",hld::e_unit_game_att_movement);
   tolua_constant(tolua_S,"e_unit_game_att_exp_only",hld::e_unit_game_att_exp_only);
   tolua_constant(tolua_S,"e_unit_game_att_suck_blood",hld::e_unit_game_att_suck_blood);
   tolua_constant(tolua_S,"e_unit_game_att_damage_reflect",hld::e_unit_game_att_damage_reflect);
   tolua_constant(tolua_S,"e_unit_game_att_war_state",hld::e_unit_game_att_war_state);
   tolua_constant(tolua_S,"e_unit_game_att_damage_sub",hld::e_unit_game_att_damage_sub);
   tolua_constant(tolua_S,"e_unit_game_att_fight_lock",hld::e_unit_game_att_fight_lock);
   tolua_constant(tolua_S,"e_unit_game_att_jump",hld::e_unit_game_att_jump);
   tolua_constant(tolua_S,"e_unit_game_att_interaction",hld::e_unit_game_att_interaction);
   tolua_constant(tolua_S,"e_unit_game_att_exp_add",hld::e_unit_game_att_exp_add);
   tolua_constant(tolua_S,"e_unit_game_att_attack_num",hld::e_unit_game_att_attack_num);
   tolua_constant(tolua_S,"e_unit_game_att_back_hp_percent",hld::e_unit_game_att_back_hp_percent);
   tolua_constant(tolua_S,"e_unit_game_att_back_hp_value",hld::e_unit_game_att_back_hp_value);
   tolua_constant(tolua_S,"e_unit_game_att_no_back_hp_percent",hld::e_unit_game_att_no_back_hp_percent);
   tolua_constant(tolua_S,"e_unit_game_att_pure_hurt_percent",hld::e_unit_game_att_pure_hurt_percent);
   tolua_constant(tolua_S,"e_unit_game_att_pure_hurt_value",hld::e_unit_game_att_pure_hurt_value);
   tolua_constant(tolua_S,"e_unit_game_att_max",hld::e_unit_game_att_max);
   tolua_constant(tolua_S,"e_war_state_peace",hld::e_war_state_peace);
   tolua_constant(tolua_S,"e_war_state_attack_ready",hld::e_war_state_attack_ready);
   tolua_constant(tolua_S,"e_war_state_attack_in",hld::e_war_state_attack_in);
   tolua_constant(tolua_S,"e_war_state_max",hld::e_war_state_max);
   tolua_constant(tolua_S,"e_unit_attack_state_dead",hld::e_unit_attack_state_dead);
   tolua_constant(tolua_S,"e_unit_attack_state_dizzy",hld::e_unit_attack_state_dizzy);
   tolua_constant(tolua_S,"e_unit_attack_state_frozen",hld::e_unit_attack_state_frozen);
   tolua_constant(tolua_S,"e_unit_attack_state_attack_back",hld::e_unit_attack_state_attack_back);
   tolua_constant(tolua_S,"e_unit_attack_state_no_control",hld::e_unit_attack_state_no_control);
   tolua_constant(tolua_S,"e_unit_attack_state_no_hurt",hld::e_unit_attack_state_no_hurt);
   tolua_constant(tolua_S,"e_unit_attack_state_stealth",hld::e_unit_attack_state_stealth);
   tolua_constant(tolua_S,"e_unit_attack_state_no_dead",hld::e_unit_attack_state_no_dead);
   tolua_constant(tolua_S,"e_unit_attack_state_no_skill",hld::e_unit_attack_state_no_skill);
   tolua_constant(tolua_S,"e_unit_attack_state_adversity_aid",hld::e_unit_attack_state_adversity_aid);
   tolua_constant(tolua_S,"e_unit_attack_state_speed_down",hld::e_unit_attack_state_speed_down);
   tolua_constant(tolua_S,"e_unit_attack_state_speed_up",hld::e_unit_attack_state_speed_up);
   tolua_constant(tolua_S,"e_unit_attack_state_stealth_show_me",hld::e_unit_attack_state_stealth_show_me);
   tolua_constant(tolua_S,"e_unit_attack_state_bleeding",hld::e_unit_attack_state_bleeding);
   tolua_constant(tolua_S,"e_unit_attack_state_burn",hld::e_unit_attack_state_burn);
   tolua_constant(tolua_S,"e_unit_attack_state_adsorbent",hld::e_unit_attack_state_adsorbent);
   tolua_constant(tolua_S,"e_unit_attack_state_crazy",hld::e_unit_attack_state_crazy);
   tolua_constant(tolua_S,"e_unit_attack_state_lock_cd",hld::e_unit_attack_state_lock_cd);
   tolua_constant(tolua_S,"e_unit_attack_state_will_dead",hld::e_unit_attack_state_will_dead);
   tolua_constant(tolua_S,"e_unit_attack_state_will_dead_percent",hld::e_unit_attack_state_will_dead_percent);
   tolua_constant(tolua_S,"e_unit_attack_state_max",hld::e_unit_attack_state_max);
   tolua_constant(tolua_S,"e_pawn_att_type_attack",hld::e_pawn_att_type_attack);
   tolua_constant(tolua_S,"e_pawn_att_type_game",hld::e_pawn_att_type_game);
   tolua_constant(tolua_S,"e_pawn_att_type_state",hld::e_pawn_att_type_state);
   tolua_constant(tolua_S,"e_pawn_att_type_sign",hld::e_pawn_att_type_sign);
   tolua_constant(tolua_S,"e_pawn_att_type_max",hld::e_pawn_att_type_max);
   tolua_constant(tolua_S,"e_class_type_none",hld::e_class_type_none);
   tolua_constant(tolua_S,"e_class_type_barserker",hld::e_class_type_barserker);
   tolua_constant(tolua_S,"e_class_type_wizard",hld::e_class_type_wizard);
   tolua_constant(tolua_S,"e_class_type_guardian",hld::e_class_type_guardian);
   tolua_constant(tolua_S,"e_class_type_assassinator",hld::e_class_type_assassinator);
   tolua_constant(tolua_S,"e_class_type_ranger",hld::e_class_type_ranger);
   tolua_constant(tolua_S,"e_class_type_monster",hld::e_class_type_monster);
   tolua_constant(tolua_S,"e_class_type_max",hld::e_class_type_max);
   tolua_constant(tolua_S,"e_branch_type_common",hld::e_branch_type_common);
   tolua_constant(tolua_S,"e_branch_type_ice",hld::e_branch_type_ice);
   tolua_constant(tolua_S,"e_branch_type_fire",hld::e_branch_type_fire);
   tolua_constant(tolua_S,"e_branch_type_light",hld::e_branch_type_light);
   tolua_constant(tolua_S,"e_branch_type_dark",hld::e_branch_type_dark);
   tolua_constant(tolua_S,"e_branch_type_max",hld::e_branch_type_max);
   tolua_constant(tolua_S,"e_map_record_info_map_template_id",hld::e_map_record_info_map_template_id);
   tolua_constant(tolua_S,"e_map_record_info_unlock",hld::e_map_record_info_unlock);
   tolua_constant(tolua_S,"e_map_record_info_enter_count",hld::e_map_record_info_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_fastest_time",hld::e_map_record_info_fastest_time);
   tolua_constant(tolua_S,"e_map_record_info_extra_enter_count",hld::e_map_record_info_extra_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_score_record",hld::e_map_record_info_score_record);
   tolua_constant(tolua_S,"e_map_record_info_real_enter_count",hld::e_map_record_info_real_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_buy_count",hld::e_map_record_info_buy_count);
   tolua_constant(tolua_S,"e_map_record_info_use_things_count",hld::e_map_record_info_use_things_count);
   tolua_constant(tolua_S,"e_map_record_info_first_join",hld::e_map_record_info_first_join);
   tolua_constant(tolua_S,"e_map_record_info_first_award",hld::e_map_record_info_first_award);
   tolua_constant(tolua_S,"e_map_record_info_next_refresh_time",hld::e_map_record_info_next_refresh_time);
   tolua_constant(tolua_S,"e_map_record_info_mopping_num",hld::e_map_record_info_mopping_num);
   tolua_constant(tolua_S,"e_map_record_info_all_enter_count",hld::e_map_record_info_all_enter_count);
   tolua_constant(tolua_S,"e_map_record_info_max",hld::e_map_record_info_max);
   tolua_constant(tolua_S,"e_big_player_type_lord_of_city",hld::e_big_player_type_lord_of_city);
   tolua_constant(tolua_S,"e_big_player_type_lord_or_war",hld::e_big_player_type_lord_or_war);
   tolua_constant(tolua_S,"e_big_player_type_lord_or_sanctuary",hld::e_big_player_type_lord_or_sanctuary);
   tolua_constant(tolua_S,"e_big_player_type_lord_or_god",hld::e_big_player_type_lord_or_god);
   tolua_constant(tolua_S,"e_big_player_type_best_barserker",hld::e_big_player_type_best_barserker);
   tolua_constant(tolua_S,"e_big_player_type_best_wizard",hld::e_big_player_type_best_wizard);
   tolua_constant(tolua_S,"e_big_player_type_best_guardian",hld::e_big_player_type_best_guardian);
   tolua_constant(tolua_S,"e_big_player_type_best_assassinator",hld::e_big_player_type_best_assassinator);
   tolua_constant(tolua_S,"e_big_player_type_overload_legion",hld::e_big_player_type_overload_legion);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_barserker",hld::e_big_player_type_gs_first_barserker);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_wizard",hld::e_big_player_type_gs_first_wizard);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_guardian",hld::e_big_player_type_gs_first_guardian);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_assassinator",hld::e_big_player_type_gs_first_assassinator);
   tolua_constant(tolua_S,"e_big_player_type_gs_first_player",hld::e_big_player_type_gs_first_player);
   tolua_constant(tolua_S,"e_big_player_type_money_first_player",hld::e_big_player_type_money_first_player);
   tolua_constant(tolua_S,"e_big_player_type_worship_first_player",hld::e_big_player_type_worship_first_player);
   tolua_constant(tolua_S,"e_big_player_type_arena_first_player",hld::e_big_player_type_arena_first_player);
   tolua_constant(tolua_S,"e_big_player_type_max",hld::e_big_player_type_max);
   tolua_constant(tolua_S,"e_map_state_join",hld::e_map_state_join);
   tolua_constant(tolua_S,"e_map_state_game",hld::e_map_state_game);
   tolua_constant(tolua_S,"e_map_state_in_game",hld::e_map_state_in_game);
   tolua_constant(tolua_S,"e_map_state_over",hld::e_map_state_over);
   tolua_constant(tolua_S,"e_map_state_return_data",hld::e_map_state_return_data);
   tolua_constant(tolua_S,"e_map_state_max",hld::e_map_state_max);
   tolua_constant(tolua_S,"e_bag_type_none",hld::e_bag_type_none);
   tolua_constant(tolua_S,"e_bag_type_equip",hld::e_bag_type_equip);
   tolua_constant(tolua_S,"e_bag_type_bag",hld::e_bag_type_bag);
   tolua_constant(tolua_S,"e_bag_type_wing",hld::e_bag_type_wing);
   tolua_constant(tolua_S,"e_bag_type_mount",hld::e_bag_type_mount);
   tolua_constant(tolua_S,"e_bag_type_protect",hld::e_bag_type_protect);
   tolua_constant(tolua_S,"e_bag_type_equip_protect",hld::e_bag_type_equip_protect);
   tolua_constant(tolua_S,"e_bag_type_mail",hld::e_bag_type_mail);
   tolua_constant(tolua_S,"e_bag_type_hope_item",hld::e_bag_type_hope_item);
   tolua_constant(tolua_S,"e_bag_type_elemenet",hld::e_bag_type_elemenet);
   tolua_constant(tolua_S,"e_bag_type_equip_elemenet",hld::e_bag_type_equip_elemenet);
   tolua_constant(tolua_S,"e_bag_type_fasion",hld::e_bag_type_fasion);
   tolua_constant(tolua_S,"e_bag_type_equip_fasion",hld::e_bag_type_equip_fasion);
   tolua_constant(tolua_S,"e_bag_type_mount_beast_spirit",hld::e_bag_type_mount_beast_spirit);
   tolua_constant(tolua_S,"e_bag_type_wing_beast_spirit",hld::e_bag_type_wing_beast_spirit);
   tolua_constant(tolua_S,"e_bag_type_treasure",hld::e_bag_type_treasure);
   tolua_constant(tolua_S,"e_bag_type_storage",hld::e_bag_type_storage);
   tolua_constant(tolua_S,"e_bag_type_belief_rune",hld::e_bag_type_belief_rune);
   tolua_constant(tolua_S,"e_bag_type_belief_rune_equip",hld::e_bag_type_belief_rune_equip);
   tolua_constant(tolua_S,"e_bag_type_equip_spirit",hld::e_bag_type_equip_spirit);
   tolua_constant(tolua_S,"e_bag_type_equip_tinder_prop",hld::e_bag_type_equip_tinder_prop);
   tolua_constant(tolua_S,"e_bag_type_sky_equip",hld::e_bag_type_sky_equip);
   tolua_constant(tolua_S,"e_bag_type_skill_book",hld::e_bag_type_skill_book);
   tolua_constant(tolua_S,"e_bag_type_goddess_equip",hld::e_bag_type_goddess_equip);
   tolua_constant(tolua_S,"e_bag_type_wedding_equip",hld::e_bag_type_wedding_equip);
   tolua_constant(tolua_S,"e_bag_type_core_element",hld::e_bag_type_core_element);
   tolua_constant(tolua_S,"e_bag_type_spirit_beast_spirit",hld::e_bag_type_spirit_beast_spirit);
   tolua_constant(tolua_S,"e_bag_type_skygod_equip",hld::e_bag_type_skygod_equip);
   tolua_constant(tolua_S,"e_bag_type_supreme_equip",hld::e_bag_type_supreme_equip);
   tolua_constant(tolua_S,"e_bag_type_spirit_upgrade_promote",hld::e_bag_type_spirit_upgrade_promote);
   tolua_constant(tolua_S,"e_bag_type_break_will",hld::e_bag_type_break_will);
   tolua_constant(tolua_S,"e_bag_type_awaken_item",hld::e_bag_type_awaken_item);
   tolua_constant(tolua_S,"e_bag_type_online_award",hld::e_bag_type_online_award);
   tolua_constant(tolua_S,"e_bag_type_max",hld::e_bag_type_max);
   tolua_constant(tolua_S,"e_item_info_info_id",hld::e_item_info_info_id);
   tolua_constant(tolua_S,"e_item_info_slot",hld::e_item_info_slot);
   tolua_constant(tolua_S,"e_item_info_equip_slot",hld::e_item_info_equip_slot);
   tolua_constant(tolua_S,"e_item_info_container_type",hld::e_item_info_container_type);
   tolua_constant(tolua_S,"e_item_info_stack_count",hld::e_item_info_stack_count);
   tolua_constant(tolua_S,"e_item_info_locked",hld::e_item_info_locked);
   tolua_constant(tolua_S,"e_item_info_activate",hld::e_item_info_activate);
   tolua_constant(tolua_S,"e_item_info_is_first",hld::e_item_info_is_first);
   tolua_constant(tolua_S,"e_item_info_upgrade_count",hld::e_item_info_upgrade_count);
   tolua_constant(tolua_S,"e_item_info_stargrade_count",hld::e_item_info_stargrade_count);
   tolua_constant(tolua_S,"e_item_info_starskill_flag",hld::e_item_info_starskill_flag);
   tolua_constant(tolua_S,"e_item_info_add_on",hld::e_item_info_add_on);
   tolua_constant(tolua_S,"e_item_info_wing_exp",hld::e_item_info_wing_exp);
   tolua_constant(tolua_S,"e_item_info_wing_bless",hld::e_item_info_wing_bless);
   tolua_constant(tolua_S,"e_item_info_random_had_flag",hld::e_item_info_random_had_flag);
   tolua_constant(tolua_S,"e_item_info_random_property1",hld::e_item_info_random_property1);
   tolua_constant(tolua_S,"e_item_info_random_property2",hld::e_item_info_random_property2);
   tolua_constant(tolua_S,"e_item_info_random_property3",hld::e_item_info_random_property3);
   tolua_constant(tolua_S,"e_item_info_random_property4",hld::e_item_info_random_property4);
   tolua_constant(tolua_S,"e_item_info_random_property5",hld::e_item_info_random_property5);
   tolua_constant(tolua_S,"e_item_info_random_property6",hld::e_item_info_random_property6);
   tolua_constant(tolua_S,"e_item_info_succinct_property1",hld::e_item_info_succinct_property1);
   tolua_constant(tolua_S,"e_item_info_succinct_property2",hld::e_item_info_succinct_property2);
   tolua_constant(tolua_S,"e_item_info_succinct_property3",hld::e_item_info_succinct_property3);
   tolua_constant(tolua_S,"e_item_info_succinct_property4",hld::e_item_info_succinct_property4);
   tolua_constant(tolua_S,"e_item_info_succinct_property5",hld::e_item_info_succinct_property5);
   tolua_constant(tolua_S,"e_item_info_succinct_property6",hld::e_item_info_succinct_property6);
   tolua_constant(tolua_S,"e_item_info_showing_illusion_idex",hld::e_item_info_showing_illusion_idex);
   tolua_constant(tolua_S,"e_item_info_put_in_bag_count",hld::e_item_info_put_in_bag_count);
   tolua_constant(tolua_S,"e_item_info_illusion_had_byte",hld::e_item_info_illusion_had_byte);
   tolua_constant(tolua_S,"e_item_info_over_time",hld::e_item_info_over_time);
   tolua_constant(tolua_S,"e_item_info_succinct_level",hld::e_item_info_succinct_level);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_0",hld::e_item_info_jewel_slot_0);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_1",hld::e_item_info_jewel_slot_1);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_2",hld::e_item_info_jewel_slot_2);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_3",hld::e_item_info_jewel_slot_3);
   tolua_constant(tolua_S,"e_item_info_jewel_slot_4",hld::e_item_info_jewel_slot_4);
   tolua_constant(tolua_S,"e_item_info_jewel_vip_slot_0",hld::e_item_info_jewel_vip_slot_0);
   tolua_constant(tolua_S,"e_item_info_jewel_vip_slot_1",hld::e_item_info_jewel_vip_slot_1);
   tolua_constant(tolua_S,"e_item_info_prototype_id",hld::e_item_info_prototype_id);
   tolua_constant(tolua_S,"e_item_info_forge_property1",hld::e_item_info_forge_property1);
   tolua_constant(tolua_S,"e_item_info_forge_property2",hld::e_item_info_forge_property2);
   tolua_constant(tolua_S,"e_item_info_forge_property3",hld::e_item_info_forge_property3);
   tolua_constant(tolua_S,"e_item_info_forge_property4",hld::e_item_info_forge_property4);
   tolua_constant(tolua_S,"e_item_info_forge_property5",hld::e_item_info_forge_property5);
   tolua_constant(tolua_S,"e_item_info_forge_property6",hld::e_item_info_forge_property6);
   tolua_constant(tolua_S,"e_item_info_jewel_carve_0",hld::e_item_info_jewel_carve_0);
   tolua_constant(tolua_S,"e_item_info_jewel_carve_1",hld::e_item_info_jewel_carve_1);
   tolua_constant(tolua_S,"e_item_info_jewel_carve_2",hld::e_item_info_jewel_carve_2);
   tolua_constant(tolua_S,"e_item_info_jewel_lucky_carve",hld::e_item_info_jewel_lucky_carve);
   tolua_constant(tolua_S,"e_item_info_max",hld::e_item_info_max);
   tolua_constant(tolua_S,"e_item_lock_all",hld::e_item_lock_all);
   tolua_constant(tolua_S,"e_item_lock_lock",hld::e_item_lock_lock);
   tolua_constant(tolua_S,"e_item_lock_un_lock",hld::e_item_lock_un_lock);
   tolua_constant(tolua_S,"e_item_lock_max",hld::e_item_lock_max);
   tolua_constant(tolua_S,"e_item_operation_none",hld::e_item_operation_none);
   tolua_constant(tolua_S,"e_item_operation_equipon",hld::e_item_operation_equipon);
   tolua_constant(tolua_S,"e_item_operation_equipoff",hld::e_item_operation_equipoff);
   tolua_constant(tolua_S,"e_item_operation_weaponactivate",hld::e_item_operation_weaponactivate);
   tolua_constant(tolua_S,"e_item_operation_weaponfirst",hld::e_item_operation_weaponfirst);
   tolua_constant(tolua_S,"e_item_operation_levelup",hld::e_item_operation_levelup);
   tolua_constant(tolua_S,"e_item_operation_synthesis",hld::e_item_operation_synthesis);
   tolua_constant(tolua_S,"e_item_operation_sell",hld::e_item_operation_sell);
   tolua_constant(tolua_S,"e_item_operation_addon",hld::e_item_operation_addon);
   tolua_constant(tolua_S,"e_item_operation_recovery",hld::e_item_operation_recovery);
   tolua_constant(tolua_S,"e_item_operation_use",hld::e_item_operation_use);
   tolua_constant(tolua_S,"e_item_operation_wing_upgrade",hld::e_item_operation_wing_upgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_starupgrade",hld::e_item_operation_wing_starupgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_illusionupgrade",hld::e_item_operation_wing_illusionupgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_addsoul",hld::e_item_operation_wing_addsoul);
   tolua_constant(tolua_S,"e_item_operation_wing_addspirit",hld::e_item_operation_wing_addspirit);
   tolua_constant(tolua_S,"e_item_operation_featherupgrade",hld::e_item_operation_featherupgrade);
   tolua_constant(tolua_S,"e_item_operation_tidy",hld::e_item_operation_tidy);
   tolua_constant(tolua_S,"e_item_operation_inherit",hld::e_item_operation_inherit);
   tolua_constant(tolua_S,"e_item_operation_advanced",hld::e_item_operation_advanced);
   tolua_constant(tolua_S,"e_item_operation_spirit_upgrade",hld::e_item_operation_spirit_upgrade);
   tolua_constant(tolua_S,"e_item_operation_split",hld::e_item_operation_split);
   tolua_constant(tolua_S,"e_item_operation_spirit_send_out",hld::e_item_operation_spirit_send_out);
   tolua_constant(tolua_S,"e_item_operation_spirit_recovery",hld::e_item_operation_spirit_recovery);
   tolua_constant(tolua_S,"e_item_operation_godassembly",hld::e_item_operation_godassembly);
   tolua_constant(tolua_S,"e_item_operation_succinct_gold",hld::e_item_operation_succinct_gold);
   tolua_constant(tolua_S,"e_item_operation_succinct_diamond",hld::e_item_operation_succinct_diamond);
   tolua_constant(tolua_S,"e_item_operation_save_succinct_result",hld::e_item_operation_save_succinct_result);
   tolua_constant(tolua_S,"e_item_operation_show_this_wing",hld::e_item_operation_show_this_wing);
   tolua_constant(tolua_S,"e_item_operation_mount_unlock",hld::e_item_operation_mount_unlock);
   tolua_constant(tolua_S,"e_item_operation_mount_upgrade",hld::e_item_operation_mount_upgrade);
   tolua_constant(tolua_S,"e_item_operation_mount_starupgrade",hld::e_item_operation_mount_starupgrade);
   tolua_constant(tolua_S,"e_item_operation_mount_illusionupgrade",hld::e_item_operation_mount_illusionupgrade);
   tolua_constant(tolua_S,"e_item_operation_mount_show_this_mount",hld::e_item_operation_mount_show_this_mount);
   tolua_constant(tolua_S,"e_item_operation_mount_show_this_mount_illusion",hld::e_item_operation_mount_show_this_mount_illusion);
   tolua_constant(tolua_S,"e_item_operation_mount_get_illusion",hld::e_item_operation_mount_get_illusion);
   tolua_constant(tolua_S,"e_item_operation_retrieve_psyche",hld::e_item_operation_retrieve_psyche);
   tolua_constant(tolua_S,"e_item_operation_put_into_bag",hld::e_item_operation_put_into_bag);
   tolua_constant(tolua_S,"e_item_operation_tidy_hope_item",hld::e_item_operation_tidy_hope_item);
   tolua_constant(tolua_S,"e_item_operation_featherupgrade_star",hld::e_item_operation_featherupgrade_star);
   tolua_constant(tolua_S,"e_item_operation_featherupgrade_grade",hld::e_item_operation_featherupgrade_grade);
   tolua_constant(tolua_S,"e_item_operation_spirit_had",hld::e_item_operation_spirit_had);
   tolua_constant(tolua_S,"e_item_operation_wing_shape_upgrade",hld::e_item_operation_wing_shape_upgrade);
   tolua_constant(tolua_S,"e_item_operation_wing_shape_unlock",hld::e_item_operation_wing_shape_unlock);
   tolua_constant(tolua_S,"e_item_operation_wing_shape_illusion_unlock",hld::e_item_operation_wing_shape_illusion_unlock);
   tolua_constant(tolua_S,"e_item_operation_one_key_use",hld::e_item_operation_one_key_use);
   tolua_constant(tolua_S,"e_item_operation_quickly_hp",hld::e_item_operation_quickly_hp);
   tolua_constant(tolua_S,"e_item_operation_against_buy",hld::e_item_operation_against_buy);
   tolua_constant(tolua_S,"e_item_operation_buy_and_use",hld::e_item_operation_buy_and_use);
   tolua_constant(tolua_S,"e_item_operation_use_beast_spirit",hld::e_item_operation_use_beast_spirit);
   tolua_constant(tolua_S,"e_item_operation_set_jewel_on",hld::e_item_operation_set_jewel_on);
   tolua_constant(tolua_S,"e_item_operation_set_jewel_off",hld::e_item_operation_set_jewel_off);
   tolua_constant(tolua_S,"e_item_operation_jewel_upgrade",hld::e_item_operation_jewel_upgrade);
   tolua_constant(tolua_S,"e_item_operation_tidy_treasure_item",hld::e_item_operation_tidy_treasure_item);
   tolua_constant(tolua_S,"e_item_operation_get_treasure_item",hld::e_item_operation_get_treasure_item);
   tolua_constant(tolua_S,"e_item_operation_godassembled",hld::e_item_operation_godassembled);
   tolua_constant(tolua_S,"e_item_operation_tidy_storage_item",hld::e_item_operation_tidy_storage_item);
   tolua_constant(tolua_S,"e_item_operation_fashion_upgrade",hld::e_item_operation_fashion_upgrade);
   tolua_constant(tolua_S,"e_item_operation_forge",hld::e_item_operation_forge);
   tolua_constant(tolua_S,"e_item_operation_n_succinct_gold",hld::e_item_operation_n_succinct_gold);
   tolua_constant(tolua_S,"e_item_operation_n_succinct_diamond",hld::e_item_operation_n_succinct_diamond);
   tolua_constant(tolua_S,"e_item_operation_enchant",hld::e_item_operation_enchant);
   tolua_constant(tolua_S,"e_item_operation_unenchant",hld::e_item_operation_unenchant);
   tolua_constant(tolua_S,"e_item_operation_wing_zhuling",hld::e_item_operation_wing_zhuling);
   tolua_constant(tolua_S,"e_item_operation_wing_lengque",hld::e_item_operation_wing_lengque);
   tolua_constant(tolua_S,"e_item_operation_wing_xiling",hld::e_item_operation_wing_xiling);
   tolua_constant(tolua_S,"e_item_operation_spirit_upgrade_promote",hld::e_item_operation_spirit_upgrade_promote);
   tolua_constant(tolua_S,"e_item_operation_break_will_activate",hld::e_item_operation_break_will_activate);
   tolua_constant(tolua_S,"e_item_operation_awaken_fetter",hld::e_item_operation_awaken_fetter);
   tolua_constant(tolua_S,"e_item_operation_awaken_fetter_skill",hld::e_item_operation_awaken_fetter_skill);
   tolua_constant(tolua_S,"e_item_operation_max",hld::e_item_operation_max);
   tolua_constant(tolua_S,"e_item_composit_type_none",hld::e_item_composit_type_none);
   tolua_constant(tolua_S,"e_item_composit_type_element",hld::e_item_composit_type_element);
   tolua_constant(tolua_S,"e_item_composit_type_equip",hld::e_item_composit_type_equip);
   tolua_constant(tolua_S,"e_item_composit_type_sprite",hld::e_item_composit_type_sprite);
   tolua_constant(tolua_S,"e_item_composit_type_max",hld::e_item_composit_type_max);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_0",hld::e_welfare_random_item_daily_online_0);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_1",hld::e_welfare_random_item_daily_online_1);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_2",hld::e_welfare_random_item_daily_online_2);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_3",hld::e_welfare_random_item_daily_online_3);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_4",hld::e_welfare_random_item_daily_online_4);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_5",hld::e_welfare_random_item_daily_online_5);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_6",hld::e_welfare_random_item_daily_online_6);
   tolua_constant(tolua_S,"e_welfare_random_item_daily_online_7",hld::e_welfare_random_item_daily_online_7);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_0",hld::e_welfare_random_item_continue_login_0);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_1",hld::e_welfare_random_item_continue_login_1);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_2",hld::e_welfare_random_item_continue_login_2);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_3",hld::e_welfare_random_item_continue_login_3);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_4",hld::e_welfare_random_item_continue_login_4);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_5",hld::e_welfare_random_item_continue_login_5);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_6",hld::e_welfare_random_item_continue_login_6);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_7",hld::e_welfare_random_item_continue_login_7);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_8",hld::e_welfare_random_item_continue_login_8);
   tolua_constant(tolua_S,"e_welfare_random_item_continue_login_9",hld::e_welfare_random_item_continue_login_9);
   tolua_constant(tolua_S,"e_welfare_random_item_max",hld::e_welfare_random_item_max);
   tolua_constant(tolua_S,"e_item_use_type_can_not_use",hld::e_item_use_type_can_not_use);
   tolua_constant(tolua_S,"e_item_use_type_can_use",hld::e_item_use_type_can_use);
   tolua_constant(tolua_S,"e_item_use_type_can_batch_use",hld::e_item_use_type_can_batch_use);
   tolua_constant(tolua_S,"e_item_use_type_only_jump",hld::e_item_use_type_only_jump);
   tolua_constant(tolua_S,"e_item_use_type_max",hld::e_item_use_type_max);
   tolua_constant(tolua_S,"e_item_color_white",hld::e_item_color_white);
   tolua_constant(tolua_S,"e_item_color_green",hld::e_item_color_green);
   tolua_constant(tolua_S,"e_item_color_blue",hld::e_item_color_blue);
   tolua_constant(tolua_S,"e_item_color_purple",hld::e_item_color_purple);
   tolua_constant(tolua_S,"e_item_color_orange",hld::e_item_color_orange);
   tolua_constant(tolua_S,"e_item_color_red",hld::e_item_color_red);
   tolua_constant(tolua_S,"e_item_color_pink",hld::e_item_color_pink);
   tolua_constant(tolua_S,"e_item_color_max",hld::e_item_color_max);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_gs",hld::e_tem_melting_sort_type_gs);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_upgrade",hld::e_tem_melting_sort_type_upgrade);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_addon",hld::e_tem_melting_sort_type_addon);
   tolua_constant(tolua_S,"e_tem_melting_sort_type_max",hld::e_tem_melting_sort_type_max);
   tolua_constant(tolua_S,"e_unit_type_null",hld::e_unit_type_null);
   tolua_constant(tolua_S,"e_unit_type_player",hld::e_unit_type_player);
   tolua_constant(tolua_S,"e_unit_type_npc",hld::e_unit_type_npc);
   tolua_constant(tolua_S,"e_unit_type_monster",hld::e_unit_type_monster);
   tolua_constant(tolua_S,"e_unit_type_drop_bag",hld::e_unit_type_drop_bag);
   tolua_constant(tolua_S,"e_unit_type_trap",hld::e_unit_type_trap);
   tolua_constant(tolua_S,"e_unit_type_friend_npc",hld::e_unit_type_friend_npc);
   tolua_constant(tolua_S,"e_unit_type_summoned",hld::e_unit_type_summoned);
   tolua_constant(tolua_S,"e_unit_type_hide",hld::e_unit_type_hide);
   tolua_constant(tolua_S,"e_unit_type_award_mark",hld::e_unit_type_award_mark);
   tolua_constant(tolua_S,"e_unit_type_award_play",hld::e_unit_type_award_play);
   tolua_constant(tolua_S,"e_unit_type_max",hld::e_unit_type_max);
   tolua_constant(tolua_S,"e_npc_type_mission",hld::e_npc_type_mission);
   tolua_constant(tolua_S,"e_npc_type_grade_up",hld::e_npc_type_grade_up);
   tolua_constant(tolua_S,"e_npc_type_strong_hold",hld::e_npc_type_strong_hold);
   tolua_constant(tolua_S,"e_npc_type_gather_crystal",hld::e_npc_type_gather_crystal);
   tolua_constant(tolua_S,"e_npc_type_lord_city",hld::e_npc_type_lord_city);
   tolua_constant(tolua_S,"e_npc_type_lord_pk",hld::e_npc_type_lord_pk);
   tolua_constant(tolua_S,"e_npc_type_lord_sanctuary",hld::e_npc_type_lord_sanctuary);
   tolua_constant(tolua_S,"e_npc_type_lord_god",hld::e_npc_type_lord_god);
   tolua_constant(tolua_S,"e_npc_type_protect_npc",hld::e_npc_type_protect_npc);
   tolua_constant(tolua_S,"e_npc_type_interact_gather",hld::e_npc_type_interact_gather);
   tolua_constant(tolua_S,"e_npc_type_touch_gather",hld::e_npc_type_touch_gather);
   tolua_constant(tolua_S,"e_npc_type_hit_and_get_npc",hld::e_npc_type_hit_and_get_npc);
   tolua_constant(tolua_S,"e_npc_type_marry_npc",hld::e_npc_type_marry_npc);
   tolua_constant(tolua_S,"e_npc_type_divorce_npc",hld::e_npc_type_divorce_npc);
   tolua_constant(tolua_S,"e_npc_type_world_boss_chests",hld::e_npc_type_world_boss_chests);
   tolua_constant(tolua_S,"e_npc_type_island_big_chests",hld::e_npc_type_island_big_chests);
   tolua_constant(tolua_S,"e_npc_type_island_small_chests",hld::e_npc_type_island_small_chests);
   tolua_constant(tolua_S,"e_npc_type_cross_pk_chests",hld::e_npc_type_cross_pk_chests);
   tolua_constant(tolua_S,"e_npc_type_server_harry_chests",hld::e_npc_type_server_harry_chests);
   tolua_constant(tolua_S,"e_npc_type_server_harry_submit",hld::e_npc_type_server_harry_submit);
   tolua_constant(tolua_S,"e_npc_type_marry_task",hld::e_npc_type_marry_task);
   tolua_constant(tolua_S,"e_npc_type_marry_exam",hld::e_npc_type_marry_exam);
   tolua_constant(tolua_S,"e_npc_type_attack_city_npc",hld::e_npc_type_attack_city_npc);
   tolua_constant(tolua_S,"e_npc_type_transfiguration_npc",hld::e_npc_type_transfiguration_npc);
   tolua_constant(tolua_S,"e_npc_type_transfer",hld::e_npc_type_transfer);
   tolua_constant(tolua_S,"e_npc_type_max",hld::e_npc_type_max);
   tolua_constant(tolua_S,"e_monster_type_nomal",hld::e_monster_type_nomal);
   tolua_constant(tolua_S,"e_monster_type_elite",hld::e_monster_type_elite);
   tolua_constant(tolua_S,"e_monster_type_boss",hld::e_monster_type_boss);
   tolua_constant(tolua_S,"e_monster_type_world_boss",hld::e_monster_type_world_boss);
   tolua_constant(tolua_S,"e_monster_type_gold_army",hld::e_monster_type_gold_army);
   tolua_constant(tolua_S,"e_monster_type_player_data",hld::e_monster_type_player_data);
   tolua_constant(tolua_S,"e_monster_type_legion_bonfire",hld::e_monster_type_legion_bonfire);
   tolua_constant(tolua_S,"e_monster_type_robot_player",hld::e_monster_type_robot_player);
   tolua_constant(tolua_S,"e_monster_type_award_boss",hld::e_monster_type_award_boss);
   tolua_constant(tolua_S,"e_monster_type_max",hld::e_monster_type_max);
   tolua_constant(tolua_S,"e_hide_type_null",hld::e_hide_type_null);
   tolua_constant(tolua_S,"e_hide_type_fire",hld::e_hide_type_fire);
   tolua_constant(tolua_S,"e_hide_type_snowstorm",hld::e_hide_type_snowstorm);
   tolua_constant(tolua_S,"e_hide_type_max",hld::e_hide_type_max);
   tolua_constant(tolua_S,"e_award_mark_type_nomal",hld::e_award_mark_type_nomal);
   tolua_constant(tolua_S,"e_award_mark_type_treasure_pic",hld::e_award_mark_type_treasure_pic);
   tolua_constant(tolua_S,"e_award_mark_type_special",hld::e_award_mark_type_special);
   tolua_constant(tolua_S,"e_award_mark_type_bank_robbery",hld::e_award_mark_type_bank_robbery);
   tolua_constant(tolua_S,"e_award_mark_type_refrush_monster",hld::e_award_mark_type_refrush_monster);
   tolua_constant(tolua_S,"e_award_mark_type_finger_guess",hld::e_award_mark_type_finger_guess);
   tolua_constant(tolua_S,"e_award_mark_type_smashing_bottle",hld::e_award_mark_type_smashing_bottle);
   tolua_constant(tolua_S,"e_award_mark_type_max",hld::e_award_mark_type_max);
   tolua_constant(tolua_S,"e_award_play_type_null",hld::e_award_play_type_null);
   tolua_constant(tolua_S,"e_award_play_type_bank_robbery_box",hld::e_award_play_type_bank_robbery_box);
   tolua_constant(tolua_S,"e_award_play_type_smashing_bottle",hld::e_award_play_type_smashing_bottle);
   tolua_constant(tolua_S,"e_award_play_type_smashing_bottle_guard",hld::e_award_play_type_smashing_bottle_guard);
   tolua_constant(tolua_S,"e_award_play_type_smashing_bottle_award",hld::e_award_play_type_smashing_bottle_award);
   tolua_constant(tolua_S,"e_award_play_type_max",hld::e_award_play_type_max);
   tolua_constant(tolua_S,"e_drop_bag_type_nomal",hld::e_drop_bag_type_nomal);
   tolua_constant(tolua_S,"e_drop_bag_type_award",hld::e_drop_bag_type_award);
   tolua_constant(tolua_S,"e_drop_bag_type_bank_robbery_box",hld::e_drop_bag_type_bank_robbery_box);
   tolua_constant(tolua_S,"e_drop_bag_type_smashing_bottle",hld::e_drop_bag_type_smashing_bottle);
   tolua_constant(tolua_S,"e_drop_bag_type_max",hld::e_drop_bag_type_max);
   tolua_constant(tolua_S,"e_box_award_type_null",hld::e_box_award_type_null);
   tolua_constant(tolua_S,"e_box_award_type_player_search_level",hld::e_box_award_type_player_search_level);
   tolua_constant(tolua_S,"e_box_award_type_self_drop",hld::e_box_award_type_self_drop);
   tolua_constant(tolua_S,"e_box_award_type_max",hld::e_box_award_type_max);
   tolua_constant(tolua_S,"e_activity_type_temple",hld::e_activity_type_temple);
   tolua_constant(tolua_S,"e_activity_type_empire_treasure",hld::e_activity_type_empire_treasure);
   tolua_constant(tolua_S,"e_activity_type_pk_king",hld::e_activity_type_pk_king);
   tolua_constant(tolua_S,"e_activity_type_legion_bonfire",hld::e_activity_type_legion_bonfire);
   tolua_constant(tolua_S,"e_activity_type_broken_sky",hld::e_activity_type_broken_sky);
   tolua_constant(tolua_S,"e_activity_type_sky_treasure_box",hld::e_activity_type_sky_treasure_box);
   tolua_constant(tolua_S,"e_activity_type_legion_boss",hld::e_activity_type_legion_boss);
   tolua_constant(tolua_S,"e_activity_type_city_war",hld::e_activity_type_city_war);
   tolua_constant(tolua_S,"e_activity_type_crystal_fairyland",hld::e_activity_type_crystal_fairyland);
   tolua_constant(tolua_S,"e_activity_type_overlord_city_war",hld::e_activity_type_overlord_city_war);
   tolua_constant(tolua_S,"e_activity_type_world_elite",hld::e_activity_type_world_elite);
   tolua_constant(tolua_S,"e_activity_type_world_boss",hld::e_activity_type_world_boss);
   tolua_constant(tolua_S,"e_activity_type_ancient_field",hld::e_activity_type_ancient_field);
   tolua_constant(tolua_S,"e_activity_type_boss_field",hld::e_activity_type_boss_field);
   tolua_constant(tolua_S,"e_activity_type_boss_home",hld::e_activity_type_boss_home);
   tolua_constant(tolua_S,"e_activity_type_boss_single",hld::e_activity_type_boss_single);
   tolua_constant(tolua_S,"e_activity_type_cloud_shop",hld::e_activity_type_cloud_shop);
   tolua_constant(tolua_S,"e_activity_type_belief_cloister",hld::e_activity_type_belief_cloister);
   tolua_constant(tolua_S,"e_activity_type_cross_server_pk",hld::e_activity_type_cross_server_pk);
   tolua_constant(tolua_S,"e_activity_type_cross_server_harry",hld::e_activity_type_cross_server_harry);
   tolua_constant(tolua_S,"e_activity_type_cross_ladder",hld::e_activity_type_cross_ladder);
   tolua_constant(tolua_S,"e_activity_type_cross_server_world_boss",hld::e_activity_type_cross_server_world_boss);
   tolua_constant(tolua_S,"e_activity_type_city_war_cross_server",hld::e_activity_type_city_war_cross_server);
   tolua_constant(tolua_S,"e_activity_type_legion_answer",hld::e_activity_type_legion_answer);
   tolua_constant(tolua_S,"e_activity_type_legion_dance",hld::e_activity_type_legion_dance);
   tolua_constant(tolua_S,"e_activity_type_element_war",hld::e_activity_type_element_war);
   tolua_constant(tolua_S,"e_activity_type_attack_city",hld::e_activity_type_attack_city);
   tolua_constant(tolua_S,"e_activity_type_max",hld::e_activity_type_max);
   tolua_constant(tolua_S,"e_widget_jump_img_id",hld::e_widget_jump_img_id);
   tolua_constant(tolua_S,"e_widget_jump_describe_id",hld::e_widget_jump_describe_id);
   tolua_constant(tolua_S,"e_widget_jump_widget_id",hld::e_widget_jump_widget_id);
   tolua_constant(tolua_S,"e_widget_jump_second_index",hld::e_widget_jump_second_index);
   tolua_constant(tolua_S,"e_widget_jump_third_index",hld::e_widget_jump_third_index);
   tolua_constant(tolua_S,"e_widget_jump_max",hld::e_widget_jump_max);
   tolua_constant(tolua_S,"e_logout_result_self",hld::e_logout_result_self);
   tolua_constant(tolua_S,"e_logout_result_load_data_fail",hld::e_logout_result_load_data_fail);
   tolua_constant(tolua_S,"e_logout_result_save_data_fail",hld::e_logout_result_save_data_fail);
   tolua_constant(tolua_S,"e_logout_result_link_fail",hld::e_logout_result_link_fail);
   tolua_constant(tolua_S,"e_logout_result_time_out",hld::e_logout_result_time_out);
   tolua_constant(tolua_S,"e_logout_result_net_lost",hld::e_logout_result_net_lost);
   tolua_constant(tolua_S,"e_logout_result_ban_account",hld::e_logout_result_ban_account);
   tolua_constant(tolua_S,"e_logout_result_new_login",hld::e_logout_result_new_login);
   tolua_constant(tolua_S,"e_logout_result_replaced_by_new_login",hld::e_logout_result_replaced_by_new_login);
   tolua_constant(tolua_S,"e_logout_result_enter_scene_failed",hld::e_logout_result_enter_scene_failed);
   tolua_constant(tolua_S,"e_logout_result_leave_scene_failed",hld::e_logout_result_leave_scene_failed);
   tolua_constant(tolua_S,"e_logout_result_cs_crash",hld::e_logout_result_cs_crash);
   tolua_constant(tolua_S,"e_logout_result_ls_crash",hld::e_logout_result_ls_crash);
   tolua_constant(tolua_S,"e_logout_result_kick_by_rpc",hld::e_logout_result_kick_by_rpc);
   tolua_constant(tolua_S,"e_logout_result_no_find_player",hld::e_logout_result_no_find_player);
   tolua_constant(tolua_S,"e_logout_result_fep_id_error",hld::e_logout_result_fep_id_error);
   tolua_constant(tolua_S,"e_logout_result_connect_dis",hld::e_logout_result_connect_dis);
   tolua_constant(tolua_S,"e_logout_result_begin_time",hld::e_logout_result_begin_time);
   tolua_constant(tolua_S,"e_logout_result_password_error",hld::e_logout_result_password_error);
   tolua_constant(tolua_S,"e_logout_result_version_wrong",hld::e_logout_result_version_wrong);
   tolua_constant(tolua_S,"e_logout_result_account_error",hld::e_logout_result_account_error);
   tolua_constant(tolua_S,"e_logout_result_queue_full",hld::e_logout_result_queue_full);
   tolua_constant(tolua_S,"e_logout_result_login_full",hld::e_logout_result_login_full);
   tolua_constant(tolua_S,"e_logout_result_bi",hld::e_logout_result_bi);
   tolua_constant(tolua_S,"e_logout_result_fake_server_full",hld::e_logout_result_fake_server_full);
   tolua_constant(tolua_S,"e_logout_result_reconnect_account",hld::e_logout_result_reconnect_account);
   tolua_constant(tolua_S,"e_logout_result_login_game_need_update",hld::e_logout_result_login_game_need_update);
   tolua_constant(tolua_S,"e_logout_result_enter_game_server_maintenance",hld::e_logout_result_enter_game_server_maintenance);
   tolua_constant(tolua_S,"e_logout_result_enter_game_no_net",hld::e_logout_result_enter_game_no_net);
   tolua_constant(tolua_S,"e_logout_result_online_time_limit",hld::e_logout_result_online_time_limit);
   tolua_constant(tolua_S,"e_logout_result_online_duration_over",hld::e_logout_result_online_duration_over);
   tolua_constant(tolua_S,"e_logout_result_only_create_role",hld::e_logout_result_only_create_role);
   tolua_constant(tolua_S,"e_logout_result_max",hld::e_logout_result_max);
   tolua_constant(tolua_S,"e_reconnect_result_win",hld::e_reconnect_result_win);
   tolua_constant(tolua_S,"e_reconnect_result_fep_no_find",hld::e_reconnect_result_fep_no_find);
   tolua_constant(tolua_S,"e_reconnect_result_ws_no_find",hld::e_reconnect_result_ws_no_find);
   tolua_constant(tolua_S,"e_reconnect_result_cs_no_find",hld::e_reconnect_result_cs_no_find);
   tolua_constant(tolua_S,"e_reconnect_result_max",hld::e_reconnect_result_max);
   tolua_constant(tolua_S,"e_demons_tower_info_cur_challenge_tier",hld::e_demons_tower_info_cur_challenge_tier);
   tolua_constant(tolua_S,"e_demons_tower_info_left_mopping_up_count",hld::e_demons_tower_info_left_mopping_up_count);
   tolua_constant(tolua_S,"e_demons_tower_info_cur_mopping_up_tier",hld::e_demons_tower_info_cur_mopping_up_tier);
   tolua_constant(tolua_S,"e_demons_tower_info_can_get_award",hld::e_demons_tower_info_can_get_award);
   tolua_constant(tolua_S,"e_demons_tower_info_max",hld::e_demons_tower_info_max);
   tolua_constant(tolua_S,"e_map_category_common",hld::e_map_category_common);
   tolua_constant(tolua_S,"e_map_category_raid",hld::e_map_category_raid);
   tolua_constant(tolua_S,"e_map_category_activity",hld::e_map_category_activity);
   tolua_constant(tolua_S,"e_map_category_max",hld::e_map_category_max);
   tolua_constant(tolua_S,"e_money_type_exp",hld::e_money_type_exp);
   tolua_constant(tolua_S,"e_money_type_treasure_pic",hld::e_money_type_treasure_pic);
   tolua_constant(tolua_S,"e_money_type_silver_bind",hld::e_money_type_silver_bind);
   tolua_constant(tolua_S,"e_money_type_jewel",hld::e_money_type_jewel);
   tolua_constant(tolua_S,"e_money_type_jewel_bind",hld::e_money_type_jewel_bind);
   tolua_constant(tolua_S,"e_money_type_boss_progress",hld::e_money_type_boss_progress);
   tolua_constant(tolua_S,"e_money_type_magic_crystal",hld::e_money_type_magic_crystal);
   tolua_constant(tolua_S,"e_money_type_battle_key",hld::e_money_type_battle_key);
   tolua_constant(tolua_S,"e_money_type_skill_active",hld::e_money_type_skill_active);
   tolua_constant(tolua_S,"e_money_type_skill_upgrade",hld::e_money_type_skill_upgrade);
   tolua_constant(tolua_S,"e_money_type_pound",hld::e_money_type_pound);
   tolua_constant(tolua_S,"e_money_type_guard_point",hld::e_money_type_guard_point);
   tolua_constant(tolua_S,"e_money_type_achievement_point",hld::e_money_type_achievement_point);
   tolua_constant(tolua_S,"e_money_type_reputation",hld::e_money_type_reputation);
   tolua_constant(tolua_S,"e_money_type_hope_point",hld::e_money_type_hope_point);
   tolua_constant(tolua_S,"e_money_type_battle_achievement",hld::e_money_type_battle_achievement);
   tolua_constant(tolua_S,"e_money_type_cross_honor",hld::e_money_type_cross_honor);
   tolua_constant(tolua_S,"e_money_type_talent",hld::e_money_type_talent);
   tolua_constant(tolua_S,"e_money_type_assist_fighting",hld::e_money_type_assist_fighting);
   tolua_constant(tolua_S,"e_money_type_treasure_score",hld::e_money_type_treasure_score);
   tolua_constant(tolua_S,"e_money_type_belief_rune_piece",hld::e_money_type_belief_rune_piece);
   tolua_constant(tolua_S,"e_money_type_legion_warehouse_score",hld::e_money_type_legion_warehouse_score);
   tolua_constant(tolua_S,"e_money_type_attribute_talent",hld::e_money_type_attribute_talent);
   tolua_constant(tolua_S,"e_money_type_world_essence",hld::e_money_type_world_essence);
   tolua_constant(tolua_S,"e_money_type_cross_server_money",hld::e_money_type_cross_server_money);
   tolua_constant(tolua_S,"e_money_type_red_diamond",hld::e_money_type_red_diamond);
   tolua_constant(tolua_S,"e_money_type_feather_piece",hld::e_money_type_feather_piece);
   tolua_constant(tolua_S,"e_money_type_cross_ladder_honor",hld::e_money_type_cross_ladder_honor);
   tolua_constant(tolua_S,"e_money_type_season_point",hld::e_money_type_season_point);
   tolua_constant(tolua_S,"e_money_type_element_score",hld::e_money_type_element_score);
   tolua_constant(tolua_S,"e_money_type_element_point",hld::e_money_type_element_point);
   tolua_constant(tolua_S,"e_money_type_energy",hld::e_money_type_energy);
   tolua_constant(tolua_S,"e_money_type_max",hld::e_money_type_max);
   tolua_constant(tolua_S,"e_team_type_none",hld::e_team_type_none);
   tolua_constant(tolua_S,"e_team_type_one_dragon",hld::e_team_type_one_dragon);
   tolua_constant(tolua_S,"e_team_type_mission_daily",hld::e_team_type_mission_daily);
   tolua_constant(tolua_S,"e_team_type_mission_against",hld::e_team_type_mission_against);
   tolua_constant(tolua_S,"e_team_type_act_world_boss",hld::e_team_type_act_world_boss);
   tolua_constant(tolua_S,"e_team_type_element_war",hld::e_team_type_element_war);
   tolua_constant(tolua_S,"e_team_type_raid_daily_exp",hld::e_team_type_raid_daily_exp);
   tolua_constant(tolua_S,"e_team_type_raid_daily_gold",hld::e_team_type_raid_daily_gold);
   tolua_constant(tolua_S,"e_team_type_raid_multi_0",hld::e_team_type_raid_multi_0);
   tolua_constant(tolua_S,"e_team_type_raid_sky",hld::e_team_type_raid_sky);
   tolua_constant(tolua_S,"e_team_type_raid_temple",hld::e_team_type_raid_temple);
   tolua_constant(tolua_S,"e_team_type_raid_multi_1",hld::e_team_type_raid_multi_1);
   tolua_constant(tolua_S,"e_team_type_raid_treasure",hld::e_team_type_raid_treasure);
   tolua_constant(tolua_S,"e_team_type_raid_multi_2",hld::e_team_type_raid_multi_2);
   tolua_constant(tolua_S,"e_team_type_hang_up",hld::e_team_type_hang_up);
   tolua_constant(tolua_S,"e_team_type_ancient_hang_up",hld::e_team_type_ancient_hang_up);
   tolua_constant(tolua_S,"e_team_type_act_world_elite",hld::e_team_type_act_world_elite);
   tolua_constant(tolua_S,"e_team_type_max",hld::e_team_type_max);
   tolua_constant(tolua_S,"e_skill_info_template_id",hld::e_skill_info_template_id);
   tolua_constant(tolua_S,"e_skill_info_cd_cur_time",hld::e_skill_info_cd_cur_time);
   tolua_constant(tolua_S,"e_skill_info_cd_max_time",hld::e_skill_info_cd_max_time);
   tolua_constant(tolua_S,"e_skill_info_activated",hld::e_skill_info_activated);
   tolua_constant(tolua_S,"e_skill_info_skill_type",hld::e_skill_info_skill_type);
   tolua_constant(tolua_S,"e_skill_info_maturity",hld::e_skill_info_maturity);
   tolua_constant(tolua_S,"e_skill_info_use_pos",hld::e_skill_info_use_pos);
   tolua_constant(tolua_S,"e_skill_info_level",hld::e_skill_info_level);
   tolua_constant(tolua_S,"e_skill_info_max",hld::e_skill_info_max);
   tolua_constant(tolua_S,"e_mission_end_type_npc_kill",hld::e_mission_end_type_npc_kill);
   tolua_constant(tolua_S,"e_mission_end_type_dialog",hld::e_mission_end_type_dialog);
   tolua_constant(tolua_S,"e_mission_end_type_mission",hld::e_mission_end_type_mission);
   tolua_constant(tolua_S,"e_mission_end_type_map",hld::e_mission_end_type_map);
   tolua_constant(tolua_S,"e_mission_end_type_transfer",hld::e_mission_end_type_transfer);
   tolua_constant(tolua_S,"e_mission_end_type_beat_npc",hld::e_mission_end_type_beat_npc);
   tolua_constant(tolua_S,"e_mission_end_type_kill_monster_get_item",hld::e_mission_end_type_kill_monster_get_item);
   tolua_constant(tolua_S,"e_mission_end_type_get_item",hld::e_mission_end_type_get_item);
   tolua_constant(tolua_S,"e_mission_end_type_equip_upgrade",hld::e_mission_end_type_equip_upgrade);
   tolua_constant(tolua_S,"e_mission_end_type_equip_addon",hld::e_mission_end_type_equip_addon);
   tolua_constant(tolua_S,"e_mission_end_type_rune_equip",hld::e_mission_end_type_rune_equip);
   tolua_constant(tolua_S,"e_mission_end_type_map_clear_current",hld::e_mission_end_type_map_clear_current);
   tolua_constant(tolua_S,"e_mission_end_type_activity_clear_current",hld::e_mission_end_type_activity_clear_current);
   tolua_constant(tolua_S,"e_mission_end_type_mount_unlock",hld::e_mission_end_type_mount_unlock);
   tolua_constant(tolua_S,"e_mission_end_type_wing_upgrade",hld::e_mission_end_type_wing_upgrade);
   tolua_constant(tolua_S,"e_mission_end_type_pokedex_activation",hld::e_mission_end_type_pokedex_activation);
   tolua_constant(tolua_S,"e_mission_end_type_sprite_activation",hld::e_mission_end_type_sprite_activation);
   tolua_constant(tolua_S,"e_mission_end_type_character_level",hld::e_mission_end_type_character_level);
   tolua_constant(tolua_S,"e_mission_end_type_map_clear_history",hld::e_mission_end_type_map_clear_history);
   tolua_constant(tolua_S,"e_mission_end_type_clear_daily_mission",hld::e_mission_end_type_clear_daily_mission);
   tolua_constant(tolua_S,"e_mission_end_type_clear_against_mission",hld::e_mission_end_type_clear_against_mission);
   tolua_constant(tolua_S,"e_mission_end_type_clear_demon_tower",hld::e_mission_end_type_clear_demon_tower);
   tolua_constant(tolua_S,"e_mission_end_type_clear_arena",hld::e_mission_end_type_clear_arena);
   tolua_constant(tolua_S,"e_mission_end_type_cheer_worship",hld::e_mission_end_type_cheer_worship);
   tolua_constant(tolua_S,"e_mission_end_type_transform",hld::e_mission_end_type_transform);
   tolua_constant(tolua_S,"e_mission_end_type_equip_levelup",hld::e_mission_end_type_equip_levelup);
   tolua_constant(tolua_S,"e_mission_end_type_add_friend",hld::e_mission_end_type_add_friend);
   tolua_constant(tolua_S,"e_mission_end_type_purchase_goods",hld::e_mission_end_type_purchase_goods);
   tolua_constant(tolua_S,"e_mission_end_type_npc_attacked_num",hld::e_mission_end_type_npc_attacked_num);
   tolua_constant(tolua_S,"e_mission_end_type_join_legion",hld::e_mission_end_type_join_legion);
   tolua_constant(tolua_S,"e_mission_end_type_rune_total_level",hld::e_mission_end_type_rune_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_upgrade_total_level",hld::e_mission_end_type_upgrade_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_add_on_total_level",hld::e_mission_end_type_add_on_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_mount_total_grade",hld::e_mission_end_type_mount_total_grade);
   tolua_constant(tolua_S,"e_mission_end_type_join_legion_boss_map",hld::e_mission_end_type_join_legion_boss_map);
   tolua_constant(tolua_S,"e_mission_end_type_tinder_total_level",hld::e_mission_end_type_tinder_total_level);
   tolua_constant(tolua_S,"e_mission_end_type_equip_target_color",hld::e_mission_end_type_equip_target_color);
   tolua_constant(tolua_S,"e_mission_end_type_legion_donate",hld::e_mission_end_type_legion_donate);
   tolua_constant(tolua_S,"e_mission_end_type_boss_field",hld::e_mission_end_type_boss_field);
   tolua_constant(tolua_S,"e_mission_end_type_grade_levelup",hld::e_mission_end_type_grade_levelup);
   tolua_constant(tolua_S,"e_mission_end_type_upgrade_level_num",hld::e_mission_end_type_upgrade_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_addon_level_num",hld::e_mission_end_type_addon_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_awaken_level_num",hld::e_mission_end_type_awaken_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_succinct_level_num",hld::e_mission_end_type_succinct_level_num);
   tolua_constant(tolua_S,"e_mission_end_type_marry_dance",hld::e_mission_end_type_marry_dance);
   tolua_constant(tolua_S,"e_mission_end_type_reach_level",hld::e_mission_end_type_reach_level);
   tolua_constant(tolua_S,"e_mission_end_type_learning_skill",hld::e_mission_end_type_learning_skill);
   tolua_constant(tolua_S,"e_mission_end_type_arena_military_rank",hld::e_mission_end_type_arena_military_rank);
   tolua_constant(tolua_S,"e_mission_end_type_use_item",hld::e_mission_end_type_use_item);
   tolua_constant(tolua_S,"e_mission_end_type_compound_item",hld::e_mission_end_type_compound_item);
   tolua_constant(tolua_S,"e_mission_end_type_spirit_baptism",hld::e_mission_end_type_spirit_baptism);
   tolua_constant(tolua_S,"e_mission_end_type_spirit_baptism_color",hld::e_mission_end_type_spirit_baptism_color);
   tolua_constant(tolua_S,"e_mission_end_type_inset_genstone",hld::e_mission_end_type_inset_genstone);
   tolua_constant(tolua_S,"e_mission_end_type_belief_all_num",hld::e_mission_end_type_belief_all_num);
   tolua_constant(tolua_S,"e_mission_end_type_kill_home_boss",hld::e_mission_end_type_kill_home_boss);
   tolua_constant(tolua_S,"e_mission_end_type_new_map_finish",hld::e_mission_end_type_new_map_finish);
   tolua_constant(tolua_S,"e_mission_end_type_mount_upgrade_num",hld::e_mission_end_type_mount_upgrade_num);
   tolua_constant(tolua_S,"e_mission_end_type_spirit_upgrade_num",hld::e_mission_end_type_spirit_upgrade_num);
   tolua_constant(tolua_S,"e_mission_end_type_wing_all_star",hld::e_mission_end_type_wing_all_star);
   tolua_constant(tolua_S,"e_mission_end_type_install_equip",hld::e_mission_end_type_install_equip);
   tolua_constant(tolua_S,"e_mission_end_type_unlock_skill_by_mission",hld::e_mission_end_type_unlock_skill_by_mission);
   tolua_constant(tolua_S,"e_mission_end_type_auction_goods",hld::e_mission_end_type_auction_goods);
   tolua_constant(tolua_S,"e_mission_end_type_equip_spirit",hld::e_mission_end_type_equip_spirit);
   tolua_constant(tolua_S,"e_mission_end_type_assis_fight_num",hld::e_mission_end_type_assis_fight_num);
   tolua_constant(tolua_S,"e_mission_end_type_auction_shopping",hld::e_mission_end_type_auction_shopping);
   tolua_constant(tolua_S,"e_mission_end_type_kill_any_home_boss",hld::e_mission_end_type_kill_any_home_boss);
   tolua_constant(tolua_S,"e_mission_end_type_kill_any_single_boss",hld::e_mission_end_type_kill_any_single_boss);
   tolua_constant(tolua_S,"e_mission_end_type_service_goal",hld::e_mission_end_type_service_goal);
   tolua_constant(tolua_S,"e_mission_end_type_total_login",hld::e_mission_end_type_total_login);
   tolua_constant(tolua_S,"e_mission_end_type_operatrion_raid",hld::e_mission_end_type_operatrion_raid);
   tolua_constant(tolua_S,"e_mission_end_type_add_dot",hld::e_mission_end_type_add_dot);
   tolua_constant(tolua_S,"e_mission_end_type_gs_welfare_num",hld::e_mission_end_type_gs_welfare_num);
   tolua_constant(tolua_S,"e_mission_end_type_level_welfare_num",hld::e_mission_end_type_level_welfare_num);
   tolua_constant(tolua_S,"e_mission_end_type_by_assis_fight_num",hld::e_mission_end_type_by_assis_fight_num);
   tolua_constant(tolua_S,"e_mission_end_type_intensify_add_gs",hld::e_mission_end_type_intensify_add_gs);
   tolua_constant(tolua_S,"e_mission_end_type_choose_a_fashion",hld::e_mission_end_type_choose_a_fashion);
   tolua_constant(tolua_S,"e_mission_end_type_active_break_will",hld::e_mission_end_type_active_break_will);
   tolua_constant(tolua_S,"e_mission_end_type_search_count",hld::e_mission_end_type_search_count);
   tolua_constant(tolua_S,"e_mission_end_type_active_skill_count",hld::e_mission_end_type_active_skill_count);
   tolua_constant(tolua_S,"e_mission_end_type_passive_skill_count",hld::e_mission_end_type_passive_skill_count);
   tolua_constant(tolua_S,"e_mission_end_type_active_skill_level",hld::e_mission_end_type_active_skill_level);
   tolua_constant(tolua_S,"e_mission_end_type_max",hld::e_mission_end_type_max);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_0",hld::e_unlock_func_type_skill_0);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_1",hld::e_unlock_func_type_skill_1);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_2",hld::e_unlock_func_type_skill_2);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_3",hld::e_unlock_func_type_skill_3);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_4",hld::e_unlock_func_type_skill_4);
   tolua_constant(tolua_S,"e_unlock_func_type_skill_5",hld::e_unlock_func_type_skill_5);
   tolua_constant(tolua_S,"e_unlock_func_type_auto_battle",hld::e_unlock_func_type_auto_battle);
   tolua_constant(tolua_S,"e_unlock_func_type_mount",hld::e_unlock_func_type_mount);
   tolua_constant(tolua_S,"e_unlock_func_type_equip_upgrade",hld::e_unlock_func_type_equip_upgrade);
   tolua_constant(tolua_S,"e_unlock_func_type_equip_addon",hld::e_unlock_func_type_equip_addon);
   tolua_constant(tolua_S,"e_unlock_func_type_wing",hld::e_unlock_func_type_wing);
   tolua_constant(tolua_S,"e_unlock_func_type_activity",hld::e_unlock_func_type_activity);
   tolua_constant(tolua_S,"e_unlock_func_type_convert",hld::e_unlock_func_type_convert);
   tolua_constant(tolua_S,"e_unlock_func_type_ranking",hld::e_unlock_func_type_ranking);
   tolua_constant(tolua_S,"e_unlock_func_type_achievement",hld::e_unlock_func_type_achievement);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_daily_exp",hld::e_unlock_func_type_raid_daily_exp);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_daily_money",hld::e_unlock_func_type_raid_daily_money);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_demons_tower",hld::e_unlock_func_type_raid_demons_tower);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_multiplayer",hld::e_unlock_func_type_raid_multiplayer);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_sky_battle",hld::e_unlock_func_type_raid_sky_battle);
   tolua_constant(tolua_S,"e_unlock_func_type_raid_drama",hld::e_unlock_func_type_raid_drama);
   tolua_constant(tolua_S,"e_unlock_func_type_max",hld::e_unlock_func_type_max);
   tolua_constant(tolua_S,"e_guide_type_none",hld::e_guide_type_none);
   tolua_constant(tolua_S,"e_guide_type_operate",hld::e_guide_type_operate);
   tolua_constant(tolua_S,"e_guide_type_open_func",hld::e_guide_type_open_func);
   tolua_constant(tolua_S,"e_guide_type_mask",hld::e_guide_type_mask);
   tolua_constant(tolua_S,"e_guide_type_cinematics_anim",hld::e_guide_type_cinematics_anim);
   tolua_constant(tolua_S,"e_guide_type_trigger_event",hld::e_guide_type_trigger_event);
   tolua_constant(tolua_S,"e_guide_type_max",hld::e_guide_type_max);
   tolua_constant(tolua_S,"e_time_type_login_time",hld::e_time_type_login_time);
   tolua_constant(tolua_S,"e_time_type_logout_time",hld::e_time_type_logout_time);
   tolua_constant(tolua_S,"e_time_type_login_time_all",hld::e_time_type_login_time_all);
   tolua_constant(tolua_S,"e_time_type_last_save_data_time",hld::e_time_type_last_save_data_time);
   tolua_constant(tolua_S,"e_time_type_last_refresh_time",hld::e_time_type_last_refresh_time);
   tolua_constant(tolua_S,"e_time_type_spirit_last_free_hunt_time",hld::e_time_type_spirit_last_free_hunt_time);
   tolua_constant(tolua_S,"e_time_type_arena_last_update",hld::e_time_type_arena_last_update);
   tolua_constant(tolua_S,"e_time_type_arena_colling_time",hld::e_time_type_arena_colling_time);
   tolua_constant(tolua_S,"e_time_type_hope_item_free_get_time",hld::e_time_type_hope_item_free_get_time);
   tolua_constant(tolua_S,"e_time_type_next_free_change_gold_time",hld::e_time_type_next_free_change_gold_time);
   tolua_constant(tolua_S,"e_time_type_have_meditation_time",hld::e_time_type_have_meditation_time);
   tolua_constant(tolua_S,"e_time_type_month_card_activity_time",hld::e_time_type_month_card_activity_time);
   tolua_constant(tolua_S,"e_time_type_month_card_remain_time",hld::e_time_type_month_card_remain_time);
   tolua_constant(tolua_S,"e_time_type_bag_time",hld::e_time_type_bag_time);
   tolua_constant(tolua_S,"e_time_type_last_daily_info_refresh_time",hld::e_time_type_last_daily_info_refresh_time);
   tolua_constant(tolua_S,"e_time_type_ancient_battelfield",hld::e_time_type_ancient_battelfield);
   tolua_constant(tolua_S,"e_time_type_last_map_record_update_time",hld::e_time_type_last_map_record_update_time);
   tolua_constant(tolua_S,"e_time_type_cur_level_time",hld::e_time_type_cur_level_time);
   tolua_constant(tolua_S,"e_time_type_legion_daily_award_flags",hld::e_time_type_legion_daily_award_flags);
   tolua_constant(tolua_S,"e_time_type_convert_last_update_time",hld::e_time_type_convert_last_update_time);
   tolua_constant(tolua_S,"e_time_type_arena_last_get_reward_time",hld::e_time_type_arena_last_get_reward_time);
   tolua_constant(tolua_S,"e_time_type_map_auto_revive_time",hld::e_time_type_map_auto_revive_time);
   tolua_constant(tolua_S,"e_time_type_exclusive_card_activity_time",hld::e_time_type_exclusive_card_activity_time);
   tolua_constant(tolua_S,"e_time_type_last_month_info_refresh_time",hld::e_time_type_last_month_info_refresh_time);
   tolua_constant(tolua_S,"e_time_type_welfare_last_refresh_time",hld::e_time_type_welfare_last_refresh_time);
   tolua_constant(tolua_S,"e_time_type_active_degree_last_refresh_time",hld::e_time_type_active_degree_last_refresh_time);
   tolua_constant(tolua_S,"e_time_type_meditation_full_send_mail",hld::e_time_type_meditation_full_send_mail);
   tolua_constant(tolua_S,"e_time_type_legion_bonus_award_flags",hld::e_time_type_legion_bonus_award_flags);
   tolua_constant(tolua_S,"e_time_type_vip_experience_level_time",hld::e_time_type_vip_experience_level_time);
   tolua_constant(tolua_S,"e_time_type_treasure_jewel_free",hld::e_time_type_treasure_jewel_free);
   tolua_constant(tolua_S,"e_time_type_treasure_element_free",hld::e_time_type_treasure_element_free);
   tolua_constant(tolua_S,"e_time_type_treasure_spirit_free",hld::e_time_type_treasure_spirit_free);
   tolua_constant(tolua_S,"e_time_type_storage_time",hld::e_time_type_storage_time);
   tolua_constant(tolua_S,"e_time_type_meditation_remaining_time",hld::e_time_type_meditation_remaining_time);
   tolua_constant(tolua_S,"e_time_type_last_refresh_by_zero_no_cross",hld::e_time_type_last_refresh_by_zero_no_cross);
   tolua_constant(tolua_S,"e_time_type_next_world_boss_can_count",hld::e_time_type_next_world_boss_can_count);
   tolua_constant(tolua_S,"e_time_type_last_harry_clear_time",hld::e_time_type_last_harry_clear_time);
   tolua_constant(tolua_S,"e_time_type_last_refresh_by_zero_cross",hld::e_time_type_last_refresh_by_zero_cross);
   tolua_constant(tolua_S,"e_time_type_last_refresh_gs_up_start_value_time",hld::e_time_type_last_refresh_gs_up_start_value_time);
   tolua_constant(tolua_S,"e_time_type_frist_recharge_time",hld::e_time_type_frist_recharge_time);
   tolua_constant(tolua_S,"e_time_type_exp_raid_join_time",hld::e_time_type_exp_raid_join_time);
   tolua_constant(tolua_S,"e_time_type_exp_raid_skill_cd",hld::e_time_type_exp_raid_skill_cd);
   tolua_constant(tolua_S,"e_time_type_exp_raid_extra_time",hld::e_time_type_exp_raid_extra_time);
   tolua_constant(tolua_S,"e_time_type_service_goal_close_time",hld::e_time_type_service_goal_close_time);
   tolua_constant(tolua_S,"e_time_type_first_recharge_open_time",hld::e_time_type_first_recharge_open_time);
   tolua_constant(tolua_S,"e_time_type_fuben_level_buy_tip_time",hld::e_time_type_fuben_level_buy_tip_time);
   tolua_constant(tolua_S,"e_time_type_money_buy_tip_time",hld::e_time_type_money_buy_tip_time);
   tolua_constant(tolua_S,"e_time_type_red_convert_last_time",hld::e_time_type_red_convert_last_time);
   tolua_constant(tolua_S,"e_time_type_energy_back",hld::e_time_type_energy_back);
   tolua_constant(tolua_S,"e_time_type_offline_award_time",hld::e_time_type_offline_award_time);
   tolua_constant(tolua_S,"e_time_type_max",hld::e_time_type_max);
   tolua_constant(tolua_S,"e_title_info_template_id",hld::e_title_info_template_id);
   tolua_constant(tolua_S,"e_title_info_start_time",hld::e_title_info_start_time);
   tolua_constant(tolua_S,"e_title_info_end_time",hld::e_title_info_end_time);
   tolua_constant(tolua_S,"e_title_info_if_geted",hld::e_title_info_if_geted);
   tolua_constant(tolua_S,"e_title_info_if_need_see",hld::e_title_info_if_need_see);
   tolua_constant(tolua_S,"e_title_info_if_need_equip",hld::e_title_info_if_need_equip);
   tolua_constant(tolua_S,"e_title_info_max",hld::e_title_info_max);
   tolua_constant(tolua_S,"e_title_end_type_forever",hld::e_title_end_type_forever);
   tolua_constant(tolua_S,"e_title_end_type_time",hld::e_title_end_type_time);
   tolua_constant(tolua_S,"e_title_end_type_condition_1",hld::e_title_end_type_condition_1);
   tolua_constant(tolua_S,"e_title_end_type_condition_2",hld::e_title_end_type_condition_2);
   tolua_constant(tolua_S,"e_title_end_type_max",hld::e_title_end_type_max);
   tolua_constant(tolua_S,"e_buff_add_judge_non",hld::e_buff_add_judge_non);
   tolua_constant(tolua_S,"e_buff_add_judge_has_it",hld::e_buff_add_judge_has_it);
   tolua_constant(tolua_S,"e_buff_add_judge_has_stronger_one",hld::e_buff_add_judge_has_stronger_one);
   tolua_constant(tolua_S,"e_buff_add_judge_can_cover",hld::e_buff_add_judge_can_cover);
   tolua_constant(tolua_S,"e_buff_add_judge_can_wrap",hld::e_buff_add_judge_can_wrap);
   tolua_constant(tolua_S,"e_buff_add_judge_can_cumulative",hld::e_buff_add_judge_can_cumulative);
   tolua_constant(tolua_S,"e_buff_add_judge_direct_add",hld::e_buff_add_judge_direct_add);
   tolua_constant(tolua_S,"e_buff_add_judge_can_percent",hld::e_buff_add_judge_can_percent);
   tolua_constant(tolua_S,"e_buff_add_judge_max",hld::e_buff_add_judge_max);
   tolua_constant(tolua_S,"e_spirit_info_template_id",hld::e_spirit_info_template_id);
   tolua_constant(tolua_S,"e_spirit_info_spirit_slot",hld::e_spirit_info_spirit_slot);
   tolua_constant(tolua_S,"e_spirit_info_container_type",hld::e_spirit_info_container_type);
   tolua_constant(tolua_S,"e_spirit_info_spirit_is_equiped",hld::e_spirit_info_spirit_is_equiped);
   tolua_constant(tolua_S,"e_spirit_info_spirit_is_fighting",hld::e_spirit_info_spirit_is_fighting);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_1",hld::e_spirit_info_flair_property_1);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_2",hld::e_spirit_info_flair_property_2);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_3",hld::e_spirit_info_flair_property_3);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_4",hld::e_spirit_info_flair_property_4);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_5",hld::e_spirit_info_flair_property_5);
   tolua_constant(tolua_S,"e_spirit_info_flair_property_6",hld::e_spirit_info_flair_property_6);
   tolua_constant(tolua_S,"e_spirit_info_spirit_typ",hld::e_spirit_info_spirit_typ);
   tolua_constant(tolua_S,"e_spirit_info_showing_illusion_idex",hld::e_spirit_info_showing_illusion_idex);
   tolua_constant(tolua_S,"e_spirit_info_illusion_had_byte",hld::e_spirit_info_illusion_had_byte);
   tolua_constant(tolua_S,"e_spirit_info_upgrade_count",hld::e_spirit_info_upgrade_count);
   tolua_constant(tolua_S,"e_spirit_info_star_upgrade_count",hld::e_spirit_info_star_upgrade_count);
   tolua_constant(tolua_S,"e_spirit_info_spirit_exp",hld::e_spirit_info_spirit_exp);
   tolua_constant(tolua_S,"e_spirit_info_random_has_flag",hld::e_spirit_info_random_has_flag);
   tolua_constant(tolua_S,"e_spirit_info_star_skill_flag",hld::e_spirit_info_star_skill_flag);
   tolua_constant(tolua_S,"e_spirit_info_efficient_skill_id",hld::e_spirit_info_efficient_skill_id);
   tolua_constant(tolua_S,"e_spirit_info_efficient_skill_id_2",hld::e_spirit_info_efficient_skill_id_2);
   tolua_constant(tolua_S,"e_spirit_info_max",hld::e_spirit_info_max);
   tolua_constant(tolua_S,"e_spirit_fetter_info_template_id",hld::e_spirit_fetter_info_template_id);
   tolua_constant(tolua_S,"e_spirit_fetter_info_upgrade_level",hld::e_spirit_fetter_info_upgrade_level);
   tolua_constant(tolua_S,"e_spirit_fetter_info_max",hld::e_spirit_fetter_info_max);
   tolua_constant(tolua_S,"e_spirit_bag_type_equiped",hld::e_spirit_bag_type_equiped);
   tolua_constant(tolua_S,"e_spirit_bag_type_spiritbag",hld::e_spirit_bag_type_spiritbag);
   tolua_constant(tolua_S,"e_spirit_bag_type_itembag",hld::e_spirit_bag_type_itembag);
   tolua_constant(tolua_S,"e_spirit_bag_type_max",hld::e_spirit_bag_type_max);
   tolua_constant(tolua_S,"e_spirit_hunt_type_free",hld::e_spirit_hunt_type_free);
   tolua_constant(tolua_S,"e_spirit_hunt_type_by_voucher",hld::e_spirit_hunt_type_by_voucher);
   tolua_constant(tolua_S,"e_spirit_hunt_type_by_ten_voucher",hld::e_spirit_hunt_type_by_ten_voucher);
   tolua_constant(tolua_S,"e_spirit_hunt_type_one_by_cost",hld::e_spirit_hunt_type_one_by_cost);
   tolua_constant(tolua_S,"e_spirit_hunt_type_ten",hld::e_spirit_hunt_type_ten);
   tolua_constant(tolua_S,"e_spirit_hunt_type_max",hld::e_spirit_hunt_type_max);
   tolua_constant(tolua_S,"e_map_difficulty_easy",hld::e_map_difficulty_easy);
   tolua_constant(tolua_S,"e_map_difficulty_normal",hld::e_map_difficulty_normal);
   tolua_constant(tolua_S,"e_map_difficulty_heard",hld::e_map_difficulty_heard);
   tolua_constant(tolua_S,"e_map_difficulty_max",hld::e_map_difficulty_max);
   tolua_constant(tolua_S,"e_ranking_worship_type_normal",hld::e_ranking_worship_type_normal);
   tolua_constant(tolua_S,"e_ranking_worship_type_money",hld::e_ranking_worship_type_money);
   tolua_constant(tolua_S,"e_ranking_worship_type_max",hld::e_ranking_worship_type_max);
   tolua_constant(tolua_S,"e_pk_mode_peace",hld::e_pk_mode_peace);
   tolua_constant(tolua_S,"e_pk_mode_good_and_evil",hld::e_pk_mode_good_and_evil);
   tolua_constant(tolua_S,"e_pk_mode_team",hld::e_pk_mode_team);
   tolua_constant(tolua_S,"e_pk_mode_family",hld::e_pk_mode_family);
   tolua_constant(tolua_S,"e_pk_mode_slaughter",hld::e_pk_mode_slaughter);
   tolua_constant(tolua_S,"e_pk_mode_cross_sever",hld::e_pk_mode_cross_sever);
   tolua_constant(tolua_S,"e_pk_mode_plunder",hld::e_pk_mode_plunder);
   tolua_constant(tolua_S,"e_pk_mode_max",hld::e_pk_mode_max);
   tolua_constant(tolua_S,"e_pk_mode_type_cant",hld::e_pk_mode_type_cant);
   tolua_constant(tolua_S,"e_pk_mode_type_allow",hld::e_pk_mode_type_allow);
   tolua_constant(tolua_S,"e_pk_mode_type_no_kill_point",hld::e_pk_mode_type_no_kill_point);
   tolua_constant(tolua_S,"e_pk_mode_type_max",hld::e_pk_mode_type_max);
   tolua_constant(tolua_S,"e_recurit_type_none",hld::e_recurit_type_none);
   tolua_constant(tolua_S,"e_recurit_type_legion_recurit",hld::e_recurit_type_legion_recurit);
   tolua_constant(tolua_S,"e_recurit_type_world_recurit",hld::e_recurit_type_world_recurit);
   tolua_constant(tolua_S,"e_recurit_type_max",hld::e_recurit_type_max);
   tolua_constant(tolua_S,"e_fruit_type_none",hld::e_fruit_type_none);
   tolua_constant(tolua_S,"e_fruit_type_strength",hld::e_fruit_type_strength);
   tolua_constant(tolua_S,"e_fruit_type_agility",hld::e_fruit_type_agility);
   tolua_constant(tolua_S,"e_fruit_type_intellect",hld::e_fruit_type_intellect);
   tolua_constant(tolua_S,"e_fruit_type_stamina",hld::e_fruit_type_stamina);
   tolua_constant(tolua_S,"e_fruit_type_max",hld::e_fruit_type_max);
   tolua_constant(tolua_S,"e_welfare_type_online_time",hld::e_welfare_type_online_time);
   tolua_constant(tolua_S,"e_welfare_type_total_login",hld::e_welfare_type_total_login);
   tolua_constant(tolua_S,"e_welfare_type_continue_login",hld::e_welfare_type_continue_login);
   tolua_constant(tolua_S,"e_welfare_type_daily_activity_reward",hld::e_welfare_type_daily_activity_reward);
   tolua_constant(tolua_S,"e_welfare_type_monthcard_reward",hld::e_welfare_type_monthcard_reward);
   tolua_constant(tolua_S,"e_welfare_type_daily_recharge_reward",hld::e_welfare_type_daily_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_total_recharge_reward",hld::e_welfare_type_total_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_total_cost_reward",hld::e_welfare_type_total_cost_reward);
   tolua_constant(tolua_S,"e_welfare_type_exp_level_reward",hld::e_welfare_type_exp_level_reward);
   tolua_constant(tolua_S,"e_welfare_type_gs_reward",hld::e_welfare_type_gs_reward);
   tolua_constant(tolua_S,"e_welfare_type_first_recharge_reward",hld::e_welfare_type_first_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_first_recharge_geted",hld::e_welfare_type_first_recharge_geted);
   tolua_constant(tolua_S,"e_welfare_type_regular_check_in",hld::e_welfare_type_regular_check_in);
   tolua_constant(tolua_S,"e_welfare_type_exclusive_check_in",hld::e_welfare_type_exclusive_check_in);
   tolua_constant(tolua_S,"e_welfare_type_vip_double_check_in",hld::e_welfare_type_vip_double_check_in);
   tolua_constant(tolua_S,"e_welfare_type_is_today_check_in",hld::e_welfare_type_is_today_check_in);
   tolua_constant(tolua_S,"e_welfare_type_replace_times",hld::e_welfare_type_replace_times);
   tolua_constant(tolua_S,"e_welfare_type_time_limit_reward",hld::e_welfare_type_time_limit_reward);
   tolua_constant(tolua_S,"e_welfare_type_cumulative_sign_in",hld::e_welfare_type_cumulative_sign_in);
   tolua_constant(tolua_S,"e_welfare_type_weekend_recharge_reward",hld::e_welfare_type_weekend_recharge_reward);
   tolua_constant(tolua_S,"e_welfare_type_max",hld::e_welfare_type_max);
   tolua_constant(tolua_S,"e_red_package_type_globel",hld::e_red_package_type_globel);
   tolua_constant(tolua_S,"e_red_package_type_legion",hld::e_red_package_type_legion);
   tolua_constant(tolua_S,"e_red_package_type_vip",hld::e_red_package_type_vip);
   tolua_constant(tolua_S,"e_red_package_type_max",hld::e_red_package_type_max);
   tolua_constant(tolua_S,"e_role_ws_info_save_time",hld::e_role_ws_info_save_time);
   tolua_constant(tolua_S,"e_role_ws_info_free_red_package_count",hld::e_role_ws_info_free_red_package_count);
   tolua_constant(tolua_S,"e_role_ws_info_max",hld::e_role_ws_info_max);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_levelup",hld::e_power_up_prompt_mount_levelup);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_upgrade",hld::e_power_up_prompt_mount_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_unlock",hld::e_power_up_prompt_mount_unlock);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_levelup",hld::e_power_up_prompt_wing_levelup);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_upgrade",hld::e_power_up_prompt_wing_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_shengling",hld::e_power_up_prompt_wing_shengling);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_yuling",hld::e_power_up_prompt_wing_yuling);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_juhun",hld::e_power_up_prompt_wing_juhun);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_upgrade",hld::e_power_up_prompt_equip_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_add_on",hld::e_power_up_prompt_equip_add_on);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_jinjie",hld::e_power_up_prompt_equip_jinjie);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_xilian",hld::e_power_up_prompt_equip_xilian);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_chongzhu",hld::e_power_up_prompt_equip_chongzhu);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_shenzhuangzaizao",hld::e_power_up_prompt_equip_shenzhuangzaizao);
   tolua_constant(tolua_S,"e_power_up_prompt_exchange_amulet",hld::e_power_up_prompt_exchange_amulet);
   tolua_constant(tolua_S,"e_power_up_prompt_raise_achievement",hld::e_power_up_prompt_raise_achievement);
   tolua_constant(tolua_S,"e_power_up_prompt_raise_rank",hld::e_power_up_prompt_raise_rank);
   tolua_constant(tolua_S,"e_power_up_prompt_raise_sprite",hld::e_power_up_prompt_raise_sprite);
   tolua_constant(tolua_S,"e_power_up_prompt_role_add_point",hld::e_power_up_prompt_role_add_point);
   tolua_constant(tolua_S,"e_power_up_prompt_add_tinder",hld::e_power_up_prompt_add_tinder);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_beast_spirit",hld::e_power_up_prompt_mount_beast_spirit);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_beast_spirit",hld::e_power_up_prompt_wing_beast_spirit);
   tolua_constant(tolua_S,"e_power_up_prompt_jewel_set",hld::e_power_up_prompt_jewel_set);
   tolua_constant(tolua_S,"e_power_up_prompt_jewel_composit",hld::e_power_up_prompt_jewel_composit);
   tolua_constant(tolua_S,"e_power_up_prompt_level_package",hld::e_power_up_prompt_level_package);
   tolua_constant(tolua_S,"e_power_up_prompt_gs_package",hld::e_power_up_prompt_gs_package);
   tolua_constant(tolua_S,"e_power_up_prompt_god_assembled",hld::e_power_up_prompt_god_assembled);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_illusion",hld::e_power_up_prompt_mount_illusion);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_star_grade",hld::e_power_up_prompt_mount_star_grade);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_illusion",hld::e_power_up_prompt_wing_illusion);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_star_grade",hld::e_power_up_prompt_wing_star_grade);
   tolua_constant(tolua_S,"e_power_up_prompt_fighting_power_analysis",hld::e_power_up_prompt_fighting_power_analysis);
   tolua_constant(tolua_S,"e_power_up_prompt_element_power_up",hld::e_power_up_prompt_element_power_up);
   tolua_constant(tolua_S,"e_power_up_prompt_tinder_skill",hld::e_power_up_prompt_tinder_skill);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_activate",hld::e_power_up_prompt_feather_activate);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_upgrade",hld::e_power_up_prompt_feather_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_skillbook_upgrade",hld::e_power_up_prompt_feather_skillbook_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_feather_skillbook_EquipOn",hld::e_power_up_prompt_feather_skillbook_EquipOn);
   tolua_constant(tolua_S,"e_power_up_prompt_Phantom_upgrade",hld::e_power_up_prompt_Phantom_upgrade);
   tolua_constant(tolua_S,"e_power_up_prompt_goddess",hld::e_power_up_prompt_goddess);
   tolua_constant(tolua_S,"e_power_up_prompt_goddess_chain",hld::e_power_up_prompt_goddess_chain);
   tolua_constant(tolua_S,"e_power_up_prompt_mount_star",hld::e_power_up_prompt_mount_star);
   tolua_constant(tolua_S,"e_power_up_prompt_wing_psychic",hld::e_power_up_prompt_wing_psychic);
   tolua_constant(tolua_S,"e_power_up_prompt_equip_awaken",hld::e_power_up_prompt_equip_awaken);
   tolua_constant(tolua_S,"e_power_up_prompt_max",hld::e_power_up_prompt_max);
   tolua_constant(tolua_S,"e_sound_type_sound",hld::e_sound_type_sound);
   tolua_constant(tolua_S,"e_sound_type_music",hld::e_sound_type_music);
   tolua_constant(tolua_S,"e_sound_type_max",hld::e_sound_type_max);
   tolua_constant(tolua_S,"e_pk_king_competition_stage_preliminary",hld::e_pk_king_competition_stage_preliminary);
   tolua_constant(tolua_S,"e_pk_king_competition_stage_final",hld::e_pk_king_competition_stage_final);
   tolua_constant(tolua_S,"e_pk_king_competition_stage_max",hld::e_pk_king_competition_stage_max);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_monster",hld::e_small_map_show_unit_type_monster);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_boss",hld::e_small_map_show_unit_type_boss);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_partner",hld::e_small_map_show_unit_type_partner);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_target",hld::e_small_map_show_unit_type_target);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_target_in_map_border",hld::e_small_map_show_unit_type_target_in_map_border);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_city_war_born_point",hld::e_small_map_show_unit_type_city_war_born_point);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_world_boss",hld::e_small_map_show_unit_type_world_boss);
   tolua_constant(tolua_S,"e_small_map_show_unit_type_max",hld::e_small_map_show_unit_type_max);
   tolua_constant(tolua_S,"e_money_tuple_id",hld::e_money_tuple_id);
   tolua_constant(tolua_S,"e_money_tuple_num",hld::e_money_tuple_num);
   tolua_constant(tolua_S,"e_money_tuple_max",hld::e_money_tuple_max);
   tolua_constant(tolua_S,"e_item_tuple_id",hld::e_item_tuple_id);
   tolua_constant(tolua_S,"e_item_tuple_num",hld::e_item_tuple_num);
   tolua_constant(tolua_S,"e_item_tuple_max",hld::e_item_tuple_max);
   tolua_constant(tolua_S,"e_recruit_activites_type_fuben",hld::e_recruit_activites_type_fuben);
   tolua_constant(tolua_S,"e_recruit_activites_type_hunting",hld::e_recruit_activites_type_hunting);
   tolua_constant(tolua_S,"e_recruit_activites_type_max",hld::e_recruit_activites_type_max);
   tolua_constant(tolua_S,"e_widget_show_mode_normal",hld::e_widget_show_mode_normal);
   tolua_constant(tolua_S,"e_widget_show_mode_OnlyDeleteBySelf",hld::e_widget_show_mode_OnlyDeleteBySelf);
   tolua_constant(tolua_S,"e_widget_show_mode_HiddenInGuide",hld::e_widget_show_mode_HiddenInGuide);
   tolua_constant(tolua_S,"e_widget_show_mode_OnlyDeleteBySelfAndHiddenInGuide",hld::e_widget_show_mode_OnlyDeleteBySelfAndHiddenInGuide);
   tolua_constant(tolua_S,"e_widget_show_mode_max",hld::e_widget_show_mode_max);
   tolua_constant(tolua_S,"e_arena_end_type_failed",hld::e_arena_end_type_failed);
   tolua_constant(tolua_S,"e_arena_end_type_win",hld::e_arena_end_type_win);
   tolua_constant(tolua_S,"e_arena_end_type_draw",hld::e_arena_end_type_draw);
   tolua_constant(tolua_S,"e_arena_end_type_max",hld::e_arena_end_type_max);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_wing_add_star",hld::e_item_upgrade_effect_type_wing_add_star);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_wing_upgrade",hld::e_item_upgrade_effect_type_wing_upgrade);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_mount_upgrade",hld::e_item_upgrade_effect_type_mount_upgrade);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_mount_upgrade_class",hld::e_item_upgrade_effect_type_mount_upgrade_class);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_fashion_upgrade",hld::e_item_upgrade_effect_type_fashion_upgrade);
   tolua_constant(tolua_S,"e_item_upgrade_effect_type_max",hld::e_item_upgrade_effect_type_max);
   tolua_constant(tolua_S,"e_map_info_panel_type_none",hld::e_map_info_panel_type_none);
   tolua_constant(tolua_S,"e_map_info_panel_type_wave",hld::e_map_info_panel_type_wave);
   tolua_constant(tolua_S,"e_map_info_panel_type_damage_rank",hld::e_map_info_panel_type_damage_rank);
   tolua_constant(tolua_S,"e_map_info_panel_type_boss_buff",hld::e_map_info_panel_type_boss_buff);
   tolua_constant(tolua_S,"e_map_info_panel_type_final_boss",hld::e_map_info_panel_type_final_boss);
   tolua_constant(tolua_S,"e_map_info_panel_type_protect_npc",hld::e_map_info_panel_type_protect_npc);
   tolua_constant(tolua_S,"e_map_info_panel_type_wave_timer",hld::e_map_info_panel_type_wave_timer);
   tolua_constant(tolua_S,"e_map_info_panel_type_score_rank",hld::e_map_info_panel_type_score_rank);
   tolua_constant(tolua_S,"e_map_info_panel_type_score_get_reward",hld::e_map_info_panel_type_score_get_reward);
   tolua_constant(tolua_S,"e_map_info_panel_type_aim_monster_killed_num",hld::e_map_info_panel_type_aim_monster_killed_num);
   tolua_constant(tolua_S,"e_map_info_panel_type_exp_raid",hld::e_map_info_panel_type_exp_raid);
   tolua_constant(tolua_S,"e_map_info_panel_type_boss_home",hld::e_map_info_panel_type_boss_home);
   tolua_constant(tolua_S,"e_map_info_panel_type_tower_reward",hld::e_map_info_panel_type_tower_reward);
   tolua_constant(tolua_S,"e_map_info_panel_type_crystak_raid",hld::e_map_info_panel_type_crystak_raid);
   tolua_constant(tolua_S,"e_map_info_panel_type_relic_raid",hld::e_map_info_panel_type_relic_raid);
   tolua_constant(tolua_S,"e_map_info_panel_type_belief_home",hld::e_map_info_panel_type_belief_home);
   tolua_constant(tolua_S,"e_map_info_panel_type_answer_question",hld::e_map_info_panel_type_answer_question);
   tolua_constant(tolua_S,"e_map_info_panel_type_legion_station",hld::e_map_info_panel_type_legion_station);
   tolua_constant(tolua_S,"e_map_info_panel_type_pk_king",hld::e_map_info_panel_type_pk_king);
   tolua_constant(tolua_S,"e_map_info_panel_type_element_war",hld::e_map_info_panel_type_element_war);
   tolua_constant(tolua_S,"e_map_info_panel_type_max",hld::e_map_info_panel_type_max);
   tolua_constant(tolua_S,"e_legion_member_info_is_online",hld::e_legion_member_info_is_online);
   tolua_constant(tolua_S,"e_legion_member_info_level",hld::e_legion_member_info_level);
   tolua_constant(tolua_S,"e_legion_member_info_player_template_id",hld::e_legion_member_info_player_template_id);
   tolua_constant(tolua_S,"e_legion_member_info_fighting_power",hld::e_legion_member_info_fighting_power);
   tolua_constant(tolua_S,"e_legion_member_info_job_title",hld::e_legion_member_info_job_title);
   tolua_constant(tolua_S,"e_legion_member_info_city_war_score",hld::e_legion_member_info_city_war_score);
   tolua_constant(tolua_S,"e_legion_member_info_donate_contribution",hld::e_legion_member_info_donate_contribution);
   tolua_constant(tolua_S,"e_legion_member_info_add_fuel_times",hld::e_legion_member_info_add_fuel_times);
   tolua_constant(tolua_S,"e_legion_member_info_last_logout_stamp",hld::e_legion_member_info_last_logout_stamp);
   tolua_constant(tolua_S,"e_legion_member_info_last_get_city_award",hld::e_legion_member_info_last_get_city_award);
   tolua_constant(tolua_S,"e_legion_member_info_agora_id",hld::e_legion_member_info_agora_id);
   tolua_constant(tolua_S,"e_legion_member_info_join_time_stamp",hld::e_legion_member_info_join_time_stamp);
   tolua_constant(tolua_S,"e_legion_member_info_vip_level",hld::e_legion_member_info_vip_level);
   tolua_constant(tolua_S,"e_legion_member_info_get_master_city_award",hld::e_legion_member_info_get_master_city_award);
   tolua_constant(tolua_S,"e_legion_member_info_week_contribution",hld::e_legion_member_info_week_contribution);
   tolua_constant(tolua_S,"e_legion_member_info_is_show_vip",hld::e_legion_member_info_is_show_vip);
   tolua_constant(tolua_S,"e_legion_member_info_max",hld::e_legion_member_info_max);
   tolua_constant(tolua_S,"e_first_time_do_type_none",hld::e_first_time_do_type_none);
   tolua_constant(tolua_S,"e_first_time_do_type_composit",hld::e_first_time_do_type_composit);
   tolua_constant(tolua_S,"e_first_time_do_type_pop_power_up_prompt_widget",hld::e_first_time_do_type_pop_power_up_prompt_widget);
   tolua_constant(tolua_S,"e_first_time_do_type_sky_raid",hld::e_first_time_do_type_sky_raid);
   tolua_constant(tolua_S,"e_first_time_do_type_close_raid_win_widget",hld::e_first_time_do_type_close_raid_win_widget);
   tolua_constant(tolua_S,"e_first_time_do_type_equip_upgrade",hld::e_first_time_do_type_equip_upgrade);
   tolua_constant(tolua_S,"e_first_time_do_type_lucky_draw_sprite",hld::e_first_time_do_type_lucky_draw_sprite);
   tolua_constant(tolua_S,"e_first_time_do_type_lucky_draw_item",hld::e_first_time_do_type_lucky_draw_item);
   tolua_constant(tolua_S,"e_first_time_do_type_get_equip",hld::e_first_time_do_type_get_equip);
   tolua_constant(tolua_S,"e_first_time_do_type_lucky_draw_elementheart",hld::e_first_time_do_type_lucky_draw_elementheart);
   tolua_constant(tolua_S,"e_first_time_do_type_killed",hld::e_first_time_do_type_killed);
   tolua_constant(tolua_S,"e_first_time_do_type_open_legion_store",hld::e_first_time_do_type_open_legion_store);
   tolua_constant(tolua_S,"e_first_time_do_type_open_old_store",hld::e_first_time_do_type_open_old_store);
   tolua_constant(tolua_S,"e_first_time_do_type_open_hope_store",hld::e_first_time_do_type_open_hope_store);
   tolua_constant(tolua_S,"e_first_time_do_type_exp_raid",hld::e_first_time_do_type_exp_raid);
   tolua_constant(tolua_S,"e_first_time_do_type_online_reward",hld::e_first_time_do_type_online_reward);
   tolua_constant(tolua_S,"e_first_time_do_type_spirit_forge",hld::e_first_time_do_type_spirit_forge);
   tolua_constant(tolua_S,"e_first_time_do_type_auction_buy",hld::e_first_time_do_type_auction_buy);
   tolua_constant(tolua_S,"e_first_time_do_type_auction_sell",hld::e_first_time_do_type_auction_sell);
   tolua_constant(tolua_S,"e_first_time_do_type_max",hld::e_first_time_do_type_max);
   tolua_constant(tolua_S,"e_move_type_stop",hld::e_move_type_stop);
   tolua_constant(tolua_S,"e_move_type_front",hld::e_move_type_front);
   tolua_constant(tolua_S,"e_move_type_back",hld::e_move_type_back);
   tolua_constant(tolua_S,"e_move_type_left",hld::e_move_type_left);
   tolua_constant(tolua_S,"e_move_type_right",hld::e_move_type_right);
   tolua_constant(tolua_S,"e_move_type_max",hld::e_move_type_max);
   tolua_constant(tolua_S,"e_revive_type_transfer",hld::e_revive_type_transfer);
   tolua_constant(tolua_S,"e_revive_type_born_pos",hld::e_revive_type_born_pos);
   tolua_constant(tolua_S,"e_revive_type_free_revive_by_CD",hld::e_revive_type_free_revive_by_CD);
   tolua_constant(tolua_S,"e_revive_type_same_pos",hld::e_revive_type_same_pos);
   tolua_constant(tolua_S,"e_revive_type_cant_revive",hld::e_revive_type_cant_revive);
   tolua_constant(tolua_S,"e_revive_type_max",hld::e_revive_type_max);
   tolua_constant(tolua_S,"e_team_state_none",hld::e_team_state_none);
   tolua_constant(tolua_S,"e_team_state_waiting_join",hld::e_team_state_waiting_join);
   tolua_constant(tolua_S,"e_team_state_waiting_ready",hld::e_team_state_waiting_ready);
   tolua_constant(tolua_S,"e_team_state_game",hld::e_team_state_game);
   tolua_constant(tolua_S,"e_team_state_max",hld::e_team_state_max);
   tolua_constant(tolua_S,"e_pk_info_pk_mode",hld::e_pk_info_pk_mode);
   tolua_constant(tolua_S,"e_pk_info_community_type",hld::e_pk_info_community_type);
   tolua_constant(tolua_S,"e_pk_info_max",hld::e_pk_info_max);
   tolua_constant(tolua_S,"e_region_type_zhongli",hld::e_region_type_zhongli);
   tolua_constant(tolua_S,"e_region_type_danger_zhongli",hld::e_region_type_danger_zhongli);
   tolua_constant(tolua_S,"e_region_type_safe",hld::e_region_type_safe);
   tolua_constant(tolua_S,"e_region_type_protect",hld::e_region_type_protect);
   tolua_constant(tolua_S,"e_region_type_max",hld::e_region_type_max);
   tolua_constant(tolua_S,"e_npc_interact_state_none",hld::e_npc_interact_state_none);
   tolua_constant(tolua_S,"e_npc_interact_state_begin",hld::e_npc_interact_state_begin);
   tolua_constant(tolua_S,"e_npc_interact_state_stop",hld::e_npc_interact_state_stop);
   tolua_constant(tolua_S,"e_npc_interact_state_finish",hld::e_npc_interact_state_finish);
   tolua_constant(tolua_S,"e_npc_interact_state_max",hld::e_npc_interact_state_max);
   tolua_constant(tolua_S,"e_mission_operate_type_accept",hld::e_mission_operate_type_accept);
   tolua_constant(tolua_S,"e_mission_operate_type_finish",hld::e_mission_operate_type_finish);
   tolua_constant(tolua_S,"e_mission_operate_type_refresh_star",hld::e_mission_operate_type_refresh_star);
   tolua_constant(tolua_S,"e_mission_operate_type_dialog",hld::e_mission_operate_type_dialog);
   tolua_constant(tolua_S,"e_mission_operate_type_finish_all_daily",hld::e_mission_operate_type_finish_all_daily);
   tolua_constant(tolua_S,"e_mission_operate_type_special_finish",hld::e_mission_operate_type_special_finish);
   tolua_constant(tolua_S,"e_mission_operate_type_create_npc",hld::e_mission_operate_type_create_npc);
   tolua_constant(tolua_S,"e_mission_operate_type_max",hld::e_mission_operate_type_max);
   tolua_constant(tolua_S,"e_protect_mark_type_single_equip_upgrade_history_record",hld::e_protect_mark_type_single_equip_upgrade_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_total_equip_upgrade_num_history_record",hld::e_protect_mark_type_total_equip_upgrade_num_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_total_equip_addon_num_history_record",hld::e_protect_mark_type_total_equip_addon_num_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_equip_succinct_max_highest_color",hld::e_protect_mark_type_equip_succinct_max_highest_color);
   tolua_constant(tolua_S,"e_protect_mark_type_total_mount_grade",hld::e_protect_mark_type_total_mount_grade);
   tolua_constant(tolua_S,"e_protect_mark_type_mount_num",hld::e_protect_mark_type_mount_num);
   tolua_constant(tolua_S,"e_protect_mark_type_wing_and_feather_total_grade",hld::e_protect_mark_type_wing_and_feather_total_grade);
   tolua_constant(tolua_S,"e_protect_mark_type_pokedex_num",hld::e_protect_mark_type_pokedex_num);
   tolua_constant(tolua_S,"e_protect_mark_type_total_sprite_equip_level_history_record",hld::e_protect_mark_type_total_sprite_equip_level_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_total_belief_grade",hld::e_protect_mark_type_total_belief_grade);
   tolua_constant(tolua_S,"e_protect_mark_type_role_exp_level",hld::e_protect_mark_type_role_exp_level);
   tolua_constant(tolua_S,"e_protect_mark_type_role_military_rank",hld::e_protect_mark_type_role_military_rank);
   tolua_constant(tolua_S,"e_protect_mark_type_achievement",hld::e_protect_mark_type_achievement);
   tolua_constant(tolua_S,"e_protect_mark_type_total_element_heart_equip_level_history_record",hld::e_protect_mark_type_total_element_heart_equip_level_history_record);
   tolua_constant(tolua_S,"e_protect_mark_type_max",hld::e_protect_mark_type_max);
   tolua_constant(tolua_S,"e_patron_saint_info_template_id",hld::e_patron_saint_info_template_id);
   tolua_constant(tolua_S,"e_patron_saint_info_last_update_time",hld::e_patron_saint_info_last_update_time);
   tolua_constant(tolua_S,"e_patron_saint_info_psyche_recovery_num",hld::e_patron_saint_info_psyche_recovery_num);
   tolua_constant(tolua_S,"e_patron_saint_info_psyche_oneday_limit",hld::e_patron_saint_info_psyche_oneday_limit);
   tolua_constant(tolua_S,"e_patron_saint_info_psyche_today_exceed_num",hld::e_patron_saint_info_psyche_today_exceed_num);
   tolua_constant(tolua_S,"e_patron_saint_info_upgrade_num",hld::e_patron_saint_info_upgrade_num);
   tolua_constant(tolua_S,"e_patron_saint_info_max",hld::e_patron_saint_info_max);
   tolua_constant(tolua_S,"e_role_history_highest_record_single_equip_upgrade",hld::e_role_history_highest_record_single_equip_upgrade);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_equip_upgrade_num",hld::e_role_history_highest_record_total_equip_upgrade_num);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_equip_addon_num",hld::e_role_history_highest_record_total_equip_addon_num);
   tolua_constant(tolua_S,"e_role_history_highest_record_item_succinct_reach_mark_level",hld::e_role_history_highest_record_item_succinct_reach_mark_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_sprite_equip_level",hld::e_role_history_highest_record_total_sprite_equip_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_total_element_heart_equip_level",hld::e_role_history_highest_record_total_element_heart_equip_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_arena_higest",hld::e_role_history_highest_record_arena_higest);
   tolua_constant(tolua_S,"e_role_history_highest_record_demon_tower_highest_level",hld::e_role_history_highest_record_demon_tower_highest_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_friend_num",hld::e_role_history_highest_record_higest_friend_num);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_friendliness_value",hld::e_role_history_highest_record_higest_friendliness_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_weapon_upgrade_buff",hld::e_role_history_highest_record_higest_weapon_upgrade_buff);
   tolua_constant(tolua_S,"e_role_history_highest_record_higest_equip_upgrade_buff",hld::e_role_history_highest_record_higest_equip_upgrade_buff);
   tolua_constant(tolua_S,"e_role_history_highest_record_rune_total_level",hld::e_role_history_highest_record_rune_total_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_addon_weapon",hld::e_role_history_highest_record_addon_weapon);
   tolua_constant(tolua_S,"e_role_history_highest_record_addon_equip",hld::e_role_history_highest_record_addon_equip);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_spring",hld::e_role_history_highest_record_belief_spring);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_summer",hld::e_role_history_highest_record_belief_summer);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_autumn",hld::e_role_history_highest_record_belief_autumn);
   tolua_constant(tolua_S,"e_role_history_highest_record_belief_winter",hld::e_role_history_highest_record_belief_winter);
   tolua_constant(tolua_S,"e_role_history_highest_record_weapon_full_succinct_grade",hld::e_role_history_highest_record_weapon_full_succinct_grade);
   tolua_constant(tolua_S,"e_role_history_highest_record_equip_full_succinct_grade",hld::e_role_history_highest_record_equip_full_succinct_grade);
   tolua_constant(tolua_S,"e_role_history_highest_record_battle_achievement",hld::e_role_history_highest_record_battle_achievement);
   tolua_constant(tolua_S,"e_role_history_highest_record_gs_value",hld::e_role_history_highest_record_gs_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_mount_value",hld::e_role_history_highest_record_mount_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_wing_value",hld::e_role_history_highest_record_wing_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_equip_value",hld::e_role_history_highest_record_equip_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_spirit_value",hld::e_role_history_highest_record_spirit_value);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_awaken_level",hld::e_role_history_highest_record_max_awaken_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_forge_level",hld::e_role_history_highest_record_max_forge_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_equip_enchant_level",hld::e_role_history_highest_record_max_equip_enchant_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_jewelry_enchant_level",hld::e_role_history_highest_record_max_jewelry_enchant_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max_spirit_composit_level",hld::e_role_history_highest_record_max_spirit_composit_level);
   tolua_constant(tolua_S,"e_role_history_highest_record_max",hld::e_role_history_highest_record_max);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_purchase_new_slot",hld::e_patron_saint_mark_operate_type_purchase_new_slot);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_equip_mark",hld::e_patron_saint_mark_operate_type_equip_mark);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_unequip_mark",hld::e_patron_saint_mark_operate_type_unequip_mark);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_upgrade",hld::e_patron_saint_mark_operate_type_upgrade);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_equip_prop",hld::e_patron_saint_mark_operate_type_equip_prop);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_unequip_prop",hld::e_patron_saint_mark_operate_type_unequip_prop);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_purchase_prop_new_slot",hld::e_patron_saint_mark_operate_type_purchase_prop_new_slot);
   tolua_constant(tolua_S,"e_patron_saint_mark_operate_type_max",hld::e_patron_saint_mark_operate_type_max);
   tolua_constant(tolua_S,"e_belief_info_template_id",hld::e_belief_info_template_id);
   tolua_constant(tolua_S,"e_belief_info_upgrade_count",hld::e_belief_info_upgrade_count);
   tolua_constant(tolua_S,"e_belief_info_try_upgrade_times",hld::e_belief_info_try_upgrade_times);
   tolua_constant(tolua_S,"e_belief_info_max",hld::e_belief_info_max);
   tolua_constant(tolua_S,"e_talent_info_template_id",hld::e_talent_info_template_id);
   tolua_constant(tolua_S,"e_talent_info_cast_money",hld::e_talent_info_cast_money);
   tolua_constant(tolua_S,"e_talent_info_max",hld::e_talent_info_max);
   tolua_constant(tolua_S,"e_talent_layer_need_id",hld::e_talent_layer_need_id);
   tolua_constant(tolua_S,"e_talent_layer_need_num",hld::e_talent_layer_need_num);
   tolua_constant(tolua_S,"e_talent_layer_template_id_0",hld::e_talent_layer_template_id_0);
   tolua_constant(tolua_S,"e_talent_layer_template_id_1",hld::e_talent_layer_template_id_1);
   tolua_constant(tolua_S,"e_talent_layer_template_id_2",hld::e_talent_layer_template_id_2);
   tolua_constant(tolua_S,"e_talent_layer_template_id_3",hld::e_talent_layer_template_id_3);
   tolua_constant(tolua_S,"e_talent_layer_max",hld::e_talent_layer_max);
   tolua_constant(tolua_S,"e_relation_data_type",hld::e_relation_data_type);
   tolua_constant(tolua_S,"e_relation_data_level",hld::e_relation_data_level);
   tolua_constant(tolua_S,"e_relation_data_template_id",hld::e_relation_data_template_id);
   tolua_constant(tolua_S,"e_relation_data_gs",hld::e_relation_data_gs);
   tolua_constant(tolua_S,"e_relation_data_relation_state",hld::e_relation_data_relation_state);
   tolua_constant(tolua_S,"e_relation_data_last_login_time",hld::e_relation_data_last_login_time);
   tolua_constant(tolua_S,"e_relation_data_friendliness",hld::e_relation_data_friendliness);
   tolua_constant(tolua_S,"e_relation_data_vip_title_template_id",hld::e_relation_data_vip_title_template_id);
   tolua_constant(tolua_S,"e_relation_data_max",hld::e_relation_data_max);
   tolua_constant(tolua_S,"e_role_equip_slot_body",hld::e_role_equip_slot_body);
   tolua_constant(tolua_S,"e_role_equip_slot_weapon_1",hld::e_role_equip_slot_weapon_1);
   tolua_constant(tolua_S,"e_role_equip_slot_hat",hld::e_role_equip_slot_hat);
   tolua_constant(tolua_S,"e_role_equip_slot_earring",hld::e_role_equip_slot_earring);
   tolua_constant(tolua_S,"e_role_equip_slot_necklace",hld::e_role_equip_slot_necklace);
   tolua_constant(tolua_S,"e_role_equip_slot_armor",hld::e_role_equip_slot_armor);
   tolua_constant(tolua_S,"e_role_equip_slot_pants",hld::e_role_equip_slot_pants);
   tolua_constant(tolua_S,"e_role_equip_slot_ring",hld::e_role_equip_slot_ring);
   tolua_constant(tolua_S,"e_role_equip_slot_glove",hld::e_role_equip_slot_glove);
   tolua_constant(tolua_S,"e_role_equip_slot_boot",hld::e_role_equip_slot_boot);
   tolua_constant(tolua_S,"e_role_equip_slot_amulet",hld::e_role_equip_slot_amulet);
   tolua_constant(tolua_S,"e_role_equip_slot_magic_1",hld::e_role_equip_slot_magic_1);
   tolua_constant(tolua_S,"e_role_equip_slot_magic_2",hld::e_role_equip_slot_magic_2);
   tolua_constant(tolua_S,"e_role_equip_slot_wing",hld::e_role_equip_slot_wing);
   tolua_constant(tolua_S,"e_role_equip_slot_mount",hld::e_role_equip_slot_mount);
   tolua_constant(tolua_S,"e_role_equip_slot_sprite",hld::e_role_equip_slot_sprite);
   tolua_constant(tolua_S,"e_role_equip_slot_mantle",hld::e_role_equip_slot_mantle);
   tolua_constant(tolua_S,"e_role_equip_slot_badge",hld::e_role_equip_slot_badge);
   tolua_constant(tolua_S,"e_role_equip_slot_head",hld::e_role_equip_slot_head);
   tolua_constant(tolua_S,"e_role_equip_slot_hair",hld::e_role_equip_slot_hair);
   tolua_constant(tolua_S,"e_role_equip_slot_headdress",hld::e_role_equip_slot_headdress);
   tolua_constant(tolua_S,"e_role_equip_slot_mouth",hld::e_role_equip_slot_mouth);
   tolua_constant(tolua_S,"e_role_equip_slot_eye",hld::e_role_equip_slot_eye);
   tolua_constant(tolua_S,"e_role_equip_slot_max",hld::e_role_equip_slot_max);
   tolua_constant(tolua_S,"e_item_type_none",hld::e_item_type_none);
   tolua_constant(tolua_S,"e_item_type_weapon",hld::e_item_type_weapon);
   tolua_constant(tolua_S,"e_item_type_armor",hld::e_item_type_armor);
   tolua_constant(tolua_S,"e_item_type_mount",hld::e_item_type_mount);
   tolua_constant(tolua_S,"e_item_type_expendable",hld::e_item_type_expendable);
   tolua_constant(tolua_S,"e_item_type_stone",hld::e_item_type_stone);
   tolua_constant(tolua_S,"e_item_type_fashion",hld::e_item_type_fashion);
   tolua_constant(tolua_S,"e_item_type_mission",hld::e_item_type_mission);
   tolua_constant(tolua_S,"e_item_type_spirit",hld::e_item_type_spirit);
   tolua_constant(tolua_S,"e_item_type_wing",hld::e_item_type_wing);
   tolua_constant(tolua_S,"e_item_type_element_heart",hld::e_item_type_element_heart);
   tolua_constant(tolua_S,"e_item_type_protect",hld::e_item_type_protect);
   tolua_constant(tolua_S,"e_item_type_beast_spirit",hld::e_item_type_beast_spirit);
   tolua_constant(tolua_S,"e_item_type_time_limit",hld::e_item_type_time_limit);
   tolua_constant(tolua_S,"e_item_type_belief_rune",hld::e_item_type_belief_rune);
   tolua_constant(tolua_S,"e_item_type_sky_equip",hld::e_item_type_sky_equip);
   tolua_constant(tolua_S,"e_item_type_skill_book",hld::e_item_type_skill_book);
   tolua_constant(tolua_S,"e_item_type_goddess_equip",hld::e_item_type_goddess_equip);
   tolua_constant(tolua_S,"e_item_type_wedding_equip",hld::e_item_type_wedding_equip);
   tolua_constant(tolua_S,"e_item_type_skygod_equip",hld::e_item_type_skygod_equip);
   tolua_constant(tolua_S,"e_item_type_mount_equip",hld::e_item_type_mount_equip);
   tolua_constant(tolua_S,"e_item_type_supreme_equip",hld::e_item_type_supreme_equip);
   tolua_constant(tolua_S,"e_item_type_star_map_spirit",hld::e_item_type_star_map_spirit);
   tolua_constant(tolua_S,"e_item_type_mount_star",hld::e_item_type_mount_star);
   tolua_constant(tolua_S,"e_item_type_wing_psychic",hld::e_item_type_wing_psychic);
   tolua_constant(tolua_S,"e_item_type_break_will",hld::e_item_type_break_will);
   tolua_constant(tolua_S,"e_item_type_awaken_item",hld::e_item_type_awaken_item);
   tolua_constant(tolua_S,"e_item_type_max",hld::e_item_type_max);
   tolua_constant(tolua_S,"e_armor_sub_type_none",hld::e_armor_sub_type_none);
   tolua_constant(tolua_S,"e_armor_sub_type_hat",hld::e_armor_sub_type_hat);
   tolua_constant(tolua_S,"e_armor_sub_type_earring",hld::e_armor_sub_type_earring);
   tolua_constant(tolua_S,"e_armor_sub_type_necklace",hld::e_armor_sub_type_necklace);
   tolua_constant(tolua_S,"e_armor_sub_type_armor",hld::e_armor_sub_type_armor);
   tolua_constant(tolua_S,"e_armor_sub_type_pants",hld::e_armor_sub_type_pants);
   tolua_constant(tolua_S,"e_armor_sub_type_ring",hld::e_armor_sub_type_ring);
   tolua_constant(tolua_S,"e_armor_sub_type_glove",hld::e_armor_sub_type_glove);
   tolua_constant(tolua_S,"e_armor_sub_type_boot",hld::e_armor_sub_type_boot);
   tolua_constant(tolua_S,"e_armor_sub_type_amulet",hld::e_armor_sub_type_amulet);
   tolua_constant(tolua_S,"e_armor_sub_type_fashion",hld::e_armor_sub_type_fashion);
   tolua_constant(tolua_S,"e_armor_sub_type_wedding",hld::e_armor_sub_type_wedding);
   tolua_constant(tolua_S,"e_armor_sub_type_spirit",hld::e_armor_sub_type_spirit);
   tolua_constant(tolua_S,"e_armor_sub_type_mantle",hld::e_armor_sub_type_mantle);
   tolua_constant(tolua_S,"e_armor_sub_type_max",hld::e_armor_sub_type_max);
   tolua_constant(tolua_S,"e_prop_sub_type_none",hld::e_prop_sub_type_none);
   tolua_constant(tolua_S,"e_prop_sub_type_fruit",hld::e_prop_sub_type_fruit);
   tolua_constant(tolua_S,"e_prop_sub_type_gife",hld::e_prop_sub_type_gife);
   tolua_constant(tolua_S,"e_prop_sub_type_money",hld::e_prop_sub_type_money);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_min",hld::e_prop_sub_type_hp_min);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_mld",hld::e_prop_sub_type_hp_mld);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_max",hld::e_prop_sub_type_hp_max);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_shap_card",hld::e_prop_sub_type_wing_shap_card);
   tolua_constant(tolua_S,"e_prop_sub_type_title_card",hld::e_prop_sub_type_title_card);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_shop",hld::e_prop_sub_type_hp_shop);
   tolua_constant(tolua_S,"e_prop_sub_type_hp_essence",hld::e_prop_sub_type_hp_essence);
   tolua_constant(tolua_S,"e_prop_sub_type_store_medicine",hld::e_prop_sub_type_store_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_gain_medicine",hld::e_prop_sub_type_gain_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_min_level_up_medicine",hld::e_prop_sub_type_min_level_up_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_mid_level_up_medicine",hld::e_prop_sub_type_mid_level_up_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_max_level_up_medicine",hld::e_prop_sub_type_max_level_up_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_wash_talent_water",hld::e_prop_sub_type_wash_talent_water);
   tolua_constant(tolua_S,"e_prop_sub_type_legion_donate",hld::e_prop_sub_type_legion_donate);
   tolua_constant(tolua_S,"e_prop_sub_type_reduce_Pk_value",hld::e_prop_sub_type_reduce_Pk_value);
   tolua_constant(tolua_S,"e_prop_sub_type_mount_illusion_unlock",hld::e_prop_sub_type_mount_illusion_unlock);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_illusion_unlock",hld::e_prop_sub_type_wing_illusion_unlock);
   tolua_constant(tolua_S,"e_prop_sub_type_element_package",hld::e_prop_sub_type_element_package);
   tolua_constant(tolua_S,"e_prop_sub_type_vip_card",hld::e_prop_sub_type_vip_card);
   tolua_constant(tolua_S,"e_prop_sub_type_rand_equip",hld::e_prop_sub_type_rand_equip);
   tolua_constant(tolua_S,"e_prop_sub_type_globel_red_package",hld::e_prop_sub_type_globel_red_package);
   tolua_constant(tolua_S,"e_prop_sub_type_legion_red_package",hld::e_prop_sub_type_legion_red_package);
   tolua_constant(tolua_S,"e_prop_sub_type_mount_shap_card",hld::e_prop_sub_type_mount_shap_card);
   tolua_constant(tolua_S,"e_prop_sub_type_marry_prop",hld::e_prop_sub_type_marry_prop);
   tolua_constant(tolua_S,"e_prop_sub_type_diamonds_package",hld::e_prop_sub_type_diamonds_package);
   tolua_constant(tolua_S,"e_prop_sub_type_refresh_body_att",hld::e_prop_sub_type_refresh_body_att);
   tolua_constant(tolua_S,"e_prop_sub_type_time_limit_prop",hld::e_prop_sub_type_time_limit_prop);
   tolua_constant(tolua_S,"e_prop_sub_type_recharge_package",hld::e_prop_sub_type_recharge_package);
   tolua_constant(tolua_S,"e_prop_sub_type_vip_experience_card",hld::e_prop_sub_type_vip_experience_card);
   tolua_constant(tolua_S,"e_prop_sub_type_const_att_equip",hld::e_prop_sub_type_const_att_equip);
   tolua_constant(tolua_S,"e_prop_sub_type_exp_addition",hld::e_prop_sub_type_exp_addition);
   tolua_constant(tolua_S,"e_prop_sub_type_add_map_count",hld::e_prop_sub_type_add_map_count);
   tolua_constant(tolua_S,"e_prop_sub_type_add_exp_with_num",hld::e_prop_sub_type_add_exp_with_num);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_experience",hld::e_prop_sub_type_wing_experience);
   tolua_constant(tolua_S,"e_prop_sub_type_wing_piece",hld::e_prop_sub_type_wing_piece);
   tolua_constant(tolua_S,"e_prop_sub_type_change_class_card",hld::e_prop_sub_type_change_class_card);
   tolua_constant(tolua_S,"e_prop_sub_type_change_name_card",hld::e_prop_sub_type_change_name_card);
   tolua_constant(tolua_S,"e_prop_sub_type_choose_gife",hld::e_prop_sub_type_choose_gife);
   tolua_constant(tolua_S,"e_prop_sub_type_meditation_medicine",hld::e_prop_sub_type_meditation_medicine);
   tolua_constant(tolua_S,"e_prop_sub_type_fashion_prop",hld::e_prop_sub_type_fashion_prop);
   tolua_constant(tolua_S,"e_prop_sub_type_tinder_skill_book",hld::e_prop_sub_type_tinder_skill_book);
   tolua_constant(tolua_S,"e_prop_sub_type_grade_up",hld::e_prop_sub_type_grade_up);
   tolua_constant(tolua_S,"e_prop_sub_type_legion_change_name_card",hld::e_prop_sub_type_legion_change_name_card);
   tolua_constant(tolua_S,"e_prop_sub_type_fireworks",hld::e_prop_sub_type_fireworks);
   tolua_constant(tolua_S,"e_prop_sub_type_horn",hld::e_prop_sub_type_horn);
   tolua_constant(tolua_S,"e_prop_sub_type_area_horn",hld::e_prop_sub_type_area_horn);
   tolua_constant(tolua_S,"e_prop_sub_type_add_map_time",hld::e_prop_sub_type_add_map_time);
   tolua_constant(tolua_S,"e_prop_sub_type_appearance_card",hld::e_prop_sub_type_appearance_card);
   tolua_constant(tolua_S,"e_prop_sub_type_add_already_time",hld::e_prop_sub_type_add_already_time);
   tolua_constant(tolua_S,"e_prop_sub_type_add_single_boss_times",hld::e_prop_sub_type_add_single_boss_times);
   tolua_constant(tolua_S,"e_prop_sub_type_awken_item",hld::e_prop_sub_type_awken_item);
   tolua_constant(tolua_S,"e_prop_sub_type_sprite_upgrade",hld::e_prop_sub_type_sprite_upgrade);
   tolua_constant(tolua_S,"e_prop_sub_type_max",hld::e_prop_sub_type_max);
   tolua_constant(tolua_S,"e_stone_sub_type_none",hld::e_stone_sub_type_none);
   tolua_constant(tolua_S,"e_stone_sub_type_equip_upgrade",hld::e_stone_sub_type_equip_upgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_equip_addon",hld::e_stone_sub_type_equip_addon);
   tolua_constant(tolua_S,"e_stone_sub_type_mount",hld::e_stone_sub_type_mount);
   tolua_constant(tolua_S,"e_stone_sub_type_addsoul",hld::e_stone_sub_type_addsoul);
   tolua_constant(tolua_S,"e_stone_sub_type_addspririt",hld::e_stone_sub_type_addspririt);
   tolua_constant(tolua_S,"e_stone_sub_type_refine",hld::e_stone_sub_type_refine);
   tolua_constant(tolua_S,"e_stone_sub_type_upgrade_protect",hld::e_stone_sub_type_upgrade_protect);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_upgrade",hld::e_stone_sub_type_mount_upgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_upstair",hld::e_stone_sub_type_mount_upstair);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_unlock",hld::e_stone_sub_type_mount_unlock);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_unlock_highgrade",hld::e_stone_sub_type_mount_unlock_highgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_wing_upgrade",hld::e_stone_sub_type_wing_upgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_wing_upstair",hld::e_stone_sub_type_wing_upstair);
   tolua_constant(tolua_S,"e_stone_sub_type_wing_highgrade",hld::e_stone_sub_type_wing_highgrade);
   tolua_constant(tolua_S,"e_stone_sub_type_orangered_shard",hld::e_stone_sub_type_orangered_shard);
   tolua_constant(tolua_S,"e_stone_sub_type_basic_synthetic_material",hld::e_stone_sub_type_basic_synthetic_material);
   tolua_constant(tolua_S,"e_stone_sub_type_spirit_shard",hld::e_stone_sub_type_spirit_shard);
   tolua_constant(tolua_S,"e_stone_sub_type_resurection_stone",hld::e_stone_sub_type_resurection_stone);
   tolua_constant(tolua_S,"e_stone_sub_type_dragon_soul",hld::e_stone_sub_type_dragon_soul);
   tolua_constant(tolua_S,"e_stone_sub_type_debris",hld::e_stone_sub_type_debris);
   tolua_constant(tolua_S,"e_stone_sub_type_psyche",hld::e_stone_sub_type_psyche);
   tolua_constant(tolua_S,"e_stone_sub_type_jewel",hld::e_stone_sub_type_jewel);
   tolua_constant(tolua_S,"e_stone_sub_type_book_card",hld::e_stone_sub_type_book_card);
   tolua_constant(tolua_S,"e_stone_sub_type_tinder_prop",hld::e_stone_sub_type_tinder_prop);
   tolua_constant(tolua_S,"e_stone_sub_type_mount_equip",hld::e_stone_sub_type_mount_equip);
   tolua_constant(tolua_S,"e_stone_sub_type_max",hld::e_stone_sub_type_max);
   tolua_constant(tolua_S,"e_feather_sub_type_none",hld::e_feather_sub_type_none);
   tolua_constant(tolua_S,"e_feather_sub_type_feather1",hld::e_feather_sub_type_feather1);
   tolua_constant(tolua_S,"e_feather_sub_type_feather2",hld::e_feather_sub_type_feather2);
   tolua_constant(tolua_S,"e_feather_sub_type_feather3",hld::e_feather_sub_type_feather3);
   tolua_constant(tolua_S,"e_feather_sub_type_feather4",hld::e_feather_sub_type_feather4);
   tolua_constant(tolua_S,"e_feather_sub_type_max",hld::e_feather_sub_type_max);
   tolua_constant(tolua_S,"e_wing_sub_type_none",hld::e_wing_sub_type_none);
   tolua_constant(tolua_S,"e_wing_sub_type_wing",hld::e_wing_sub_type_wing);
   tolua_constant(tolua_S,"e_wing_sub_type_feather",hld::e_wing_sub_type_feather);
   tolua_constant(tolua_S,"e_wing_sub_type_soul",hld::e_wing_sub_type_soul);
   tolua_constant(tolua_S,"e_wing_sub_type_spirit",hld::e_wing_sub_type_spirit);
   tolua_constant(tolua_S,"e_wing_sub_type_illusion",hld::e_wing_sub_type_illusion);
   tolua_constant(tolua_S,"e_wing_sub_type_max",hld::e_wing_sub_type_max);
   tolua_constant(tolua_S,"e_element_heart_sub_type_equip",hld::e_element_heart_sub_type_equip);
   tolua_constant(tolua_S,"e_element_heart_sub_type_exp",hld::e_element_heart_sub_type_exp);
   tolua_constant(tolua_S,"e_element_heart_sub_type_max",hld::e_element_heart_sub_type_max);
   tolua_constant(tolua_S,"e_store_type_non",hld::e_store_type_non);
   tolua_constant(tolua_S,"e_store_type_recommend",hld::e_store_type_recommend);
   tolua_constant(tolua_S,"e_store_type_daily",hld::e_store_type_daily);
   tolua_constant(tolua_S,"e_store_type_expendables",hld::e_store_type_expendables);
   tolua_constant(tolua_S,"e_store_type_bind_jewel_0",hld::e_store_type_bind_jewel_0);
   tolua_constant(tolua_S,"e_store_type_shape",hld::e_store_type_shape);
   tolua_constant(tolua_S,"e_store_type_limit_buy",hld::e_store_type_limit_buy);
   tolua_constant(tolua_S,"e_store_type_sprite_first",hld::e_store_type_sprite_first);
   tolua_constant(tolua_S,"e_store_type_sprite_second",hld::e_store_type_sprite_second);
   tolua_constant(tolua_S,"e_store_type_sprite_third",hld::e_store_type_sprite_third);
   tolua_constant(tolua_S,"e_store_type_magic_crystal_first",hld::e_store_type_magic_crystal_first);
   tolua_constant(tolua_S,"e_store_type_magic_crystal_second",hld::e_store_type_magic_crystal_second);
   tolua_constant(tolua_S,"e_store_type_magic_crystal_third",hld::e_store_type_magic_crystal_third);
   tolua_constant(tolua_S,"e_store_type_medicine",hld::e_store_type_medicine);
   tolua_constant(tolua_S,"e_store_type_prop",hld::e_store_type_prop);
   tolua_constant(tolua_S,"e_store_type_hope_point_first",hld::e_store_type_hope_point_first);
   tolua_constant(tolua_S,"e_store_type_hope_point_second",hld::e_store_type_hope_point_second);
   tolua_constant(tolua_S,"e_store_type_hope_point_third",hld::e_store_type_hope_point_third);
   tolua_constant(tolua_S,"e_store_type_legion",hld::e_store_type_legion);
   tolua_constant(tolua_S,"e_store_type_hornor",hld::e_store_type_hornor);
   tolua_constant(tolua_S,"e_store_type_assist_fighting",hld::e_store_type_assist_fighting);
   tolua_constant(tolua_S,"e_store_type_fashion",hld::e_store_type_fashion);
   tolua_constant(tolua_S,"e_store_type_bind_jewel_1",hld::e_store_type_bind_jewel_1);
   tolua_constant(tolua_S,"e_store_type_bind_jewel_2",hld::e_store_type_bind_jewel_2);
   tolua_constant(tolua_S,"e_store_type_treasure_royal",hld::e_store_type_treasure_royal);
   tolua_constant(tolua_S,"e_store_type_treasure_element",hld::e_store_type_treasure_element);
   tolua_constant(tolua_S,"e_store_type_cross_server_buy",hld::e_store_type_cross_server_buy);
   tolua_constant(tolua_S,"e_store_type_red_diamond",hld::e_store_type_red_diamond);
   tolua_constant(tolua_S,"e_store_type_cross_ladder_store",hld::e_store_type_cross_ladder_store);
   tolua_constant(tolua_S,"e_store_type_non_29",hld::e_store_type_non_29);
   tolua_constant(tolua_S,"e_store_type_non_30",hld::e_store_type_non_30);
   tolua_constant(tolua_S,"e_store_type_non_31",hld::e_store_type_non_31);
   tolua_constant(tolua_S,"e_store_type_non_32",hld::e_store_type_non_32);
   tolua_constant(tolua_S,"e_store_type_non_33",hld::e_store_type_non_33);
   tolua_constant(tolua_S,"e_store_type_non_34",hld::e_store_type_non_34);
   tolua_constant(tolua_S,"e_store_type_non_35",hld::e_store_type_non_35);
   tolua_constant(tolua_S,"e_store_type_non_36",hld::e_store_type_non_36);
   tolua_constant(tolua_S,"e_store_type_non_37",hld::e_store_type_non_37);
   tolua_constant(tolua_S,"e_store_type_non_38",hld::e_store_type_non_38);
   tolua_constant(tolua_S,"e_store_type_non_39",hld::e_store_type_non_39);
   tolua_constant(tolua_S,"e_store_type_non_40",hld::e_store_type_non_40);
   tolua_constant(tolua_S,"e_store_type_limit_buy_41",hld::e_store_type_limit_buy_41);
   tolua_constant(tolua_S,"e_store_type_limit_buy_42",hld::e_store_type_limit_buy_42);
   tolua_constant(tolua_S,"e_store_type_limit_buy_43",hld::e_store_type_limit_buy_43);
   tolua_constant(tolua_S,"e_store_type_limit_buy_44",hld::e_store_type_limit_buy_44);
   tolua_constant(tolua_S,"e_store_type_limit_buy_45",hld::e_store_type_limit_buy_45);
   tolua_constant(tolua_S,"e_store_type_limit_buy_46",hld::e_store_type_limit_buy_46);
   tolua_constant(tolua_S,"e_store_type_limit_buy_47",hld::e_store_type_limit_buy_47);
   tolua_constant(tolua_S,"e_store_type_limit_buy_48",hld::e_store_type_limit_buy_48);
   tolua_constant(tolua_S,"e_store_type_limit_buy_49",hld::e_store_type_limit_buy_49);
   tolua_constant(tolua_S,"e_store_type_limit_buy_50",hld::e_store_type_limit_buy_50);
   tolua_constant(tolua_S,"e_store_type_limit_buy_51",hld::e_store_type_limit_buy_51);
   tolua_constant(tolua_S,"e_store_type_limit_buy_52",hld::e_store_type_limit_buy_52);
   tolua_constant(tolua_S,"e_store_type_limit_buy_53",hld::e_store_type_limit_buy_53);
   tolua_constant(tolua_S,"e_store_type_limit_buy_54",hld::e_store_type_limit_buy_54);
   tolua_constant(tolua_S,"e_store_type_limit_buy_55",hld::e_store_type_limit_buy_55);
   tolua_constant(tolua_S,"e_store_type_limit_buy_56",hld::e_store_type_limit_buy_56);
   tolua_constant(tolua_S,"e_store_type_limit_buy_57",hld::e_store_type_limit_buy_57);
   tolua_constant(tolua_S,"e_store_type_limit_buy_58",hld::e_store_type_limit_buy_58);
   tolua_constant(tolua_S,"e_store_type_limit_buy_59",hld::e_store_type_limit_buy_59);
   tolua_constant(tolua_S,"e_store_type_limit_buy_60",hld::e_store_type_limit_buy_60);
   tolua_constant(tolua_S,"e_store_type_limit_buy_61",hld::e_store_type_limit_buy_61);
   tolua_constant(tolua_S,"e_store_type_limit_buy_62",hld::e_store_type_limit_buy_62);
   tolua_constant(tolua_S,"e_store_type_limit_buy_63",hld::e_store_type_limit_buy_63);
   tolua_constant(tolua_S,"e_store_type_limit_buy_64",hld::e_store_type_limit_buy_64);
   tolua_constant(tolua_S,"e_store_type_limit_buy_65",hld::e_store_type_limit_buy_65);
   tolua_constant(tolua_S,"e_store_type_limit_buy_66",hld::e_store_type_limit_buy_66);
   tolua_constant(tolua_S,"e_store_type_limit_buy_67",hld::e_store_type_limit_buy_67);
   tolua_constant(tolua_S,"e_store_type_limit_buy_68",hld::e_store_type_limit_buy_68);
   tolua_constant(tolua_S,"e_store_type_limit_buy_69",hld::e_store_type_limit_buy_69);
   tolua_constant(tolua_S,"e_store_type_limit_buy_70",hld::e_store_type_limit_buy_70);
   tolua_constant(tolua_S,"e_store_type_limit_buy_71",hld::e_store_type_limit_buy_71);
   tolua_constant(tolua_S,"e_store_type_limit_buy_72",hld::e_store_type_limit_buy_72);
   tolua_constant(tolua_S,"e_store_type_limit_buy_73",hld::e_store_type_limit_buy_73);
   tolua_constant(tolua_S,"e_store_type_limit_buy_74",hld::e_store_type_limit_buy_74);
   tolua_constant(tolua_S,"e_store_type_limit_buy_75",hld::e_store_type_limit_buy_75);
   tolua_constant(tolua_S,"e_store_type_limit_buy_76",hld::e_store_type_limit_buy_76);
   tolua_constant(tolua_S,"e_store_type_limit_buy_77",hld::e_store_type_limit_buy_77);
   tolua_constant(tolua_S,"e_store_type_limit_buy_78",hld::e_store_type_limit_buy_78);
   tolua_constant(tolua_S,"e_store_type_limit_buy_79",hld::e_store_type_limit_buy_79);
   tolua_constant(tolua_S,"e_store_type_limit_buy_80",hld::e_store_type_limit_buy_80);
   tolua_constant(tolua_S,"e_store_type_limit_buy_81",hld::e_store_type_limit_buy_81);
   tolua_constant(tolua_S,"e_store_type_limit_buy_82",hld::e_store_type_limit_buy_82);
   tolua_constant(tolua_S,"e_store_type_limit_buy_83",hld::e_store_type_limit_buy_83);
   tolua_constant(tolua_S,"e_store_type_limit_buy_84",hld::e_store_type_limit_buy_84);
   tolua_constant(tolua_S,"e_store_type_limit_buy_85",hld::e_store_type_limit_buy_85);
   tolua_constant(tolua_S,"e_store_type_limit_buy_86",hld::e_store_type_limit_buy_86);
   tolua_constant(tolua_S,"e_store_type_limit_buy_87",hld::e_store_type_limit_buy_87);
   tolua_constant(tolua_S,"e_store_type_limit_buy_88",hld::e_store_type_limit_buy_88);
   tolua_constant(tolua_S,"e_store_type_limit_buy_89",hld::e_store_type_limit_buy_89);
   tolua_constant(tolua_S,"e_store_type_limit_buy_90",hld::e_store_type_limit_buy_90);
   tolua_constant(tolua_S,"e_store_type_limit_buy_91",hld::e_store_type_limit_buy_91);
   tolua_constant(tolua_S,"e_store_type_limit_buy_92",hld::e_store_type_limit_buy_92);
   tolua_constant(tolua_S,"e_store_type_limit_buy_93",hld::e_store_type_limit_buy_93);
   tolua_constant(tolua_S,"e_store_type_limit_buy_94",hld::e_store_type_limit_buy_94);
   tolua_constant(tolua_S,"e_store_type_limit_buy_95",hld::e_store_type_limit_buy_95);
   tolua_constant(tolua_S,"e_store_type_limit_buy_96",hld::e_store_type_limit_buy_96);
   tolua_constant(tolua_S,"e_store_type_limit_buy_97",hld::e_store_type_limit_buy_97);
   tolua_constant(tolua_S,"e_store_type_limit_buy_98",hld::e_store_type_limit_buy_98);
   tolua_constant(tolua_S,"e_store_type_limit_buy_99",hld::e_store_type_limit_buy_99);
   tolua_constant(tolua_S,"e_store_type_limit_buy_100",hld::e_store_type_limit_buy_100);
   tolua_constant(tolua_S,"e_store_type_limit_buy_101",hld::e_store_type_limit_buy_101);
   tolua_constant(tolua_S,"e_store_type_limit_buy_102",hld::e_store_type_limit_buy_102);
   tolua_constant(tolua_S,"e_store_type_limit_buy_103",hld::e_store_type_limit_buy_103);
   tolua_constant(tolua_S,"e_store_type_limit_buy_104",hld::e_store_type_limit_buy_104);
   tolua_constant(tolua_S,"e_store_type_limit_buy_105",hld::e_store_type_limit_buy_105);
   tolua_constant(tolua_S,"e_store_type_limit_buy_106",hld::e_store_type_limit_buy_106);
   tolua_constant(tolua_S,"e_store_type_limit_buy_107",hld::e_store_type_limit_buy_107);
   tolua_constant(tolua_S,"e_store_type_limit_buy_108",hld::e_store_type_limit_buy_108);
   tolua_constant(tolua_S,"e_store_type_limit_buy_109",hld::e_store_type_limit_buy_109);
   tolua_constant(tolua_S,"e_store_type_limit_buy_110",hld::e_store_type_limit_buy_110);
   tolua_constant(tolua_S,"e_store_type_limit_buy_111",hld::e_store_type_limit_buy_111);
   tolua_constant(tolua_S,"e_store_type_limit_buy_112",hld::e_store_type_limit_buy_112);
   tolua_constant(tolua_S,"e_store_type_limit_buy_113",hld::e_store_type_limit_buy_113);
   tolua_constant(tolua_S,"e_store_type_limit_buy_114",hld::e_store_type_limit_buy_114);
   tolua_constant(tolua_S,"e_store_type_limit_buy_115",hld::e_store_type_limit_buy_115);
   tolua_constant(tolua_S,"e_store_type_limit_buy_116",hld::e_store_type_limit_buy_116);
   tolua_constant(tolua_S,"e_store_type_limit_buy_117",hld::e_store_type_limit_buy_117);
   tolua_constant(tolua_S,"e_store_type_limit_buy_118",hld::e_store_type_limit_buy_118);
   tolua_constant(tolua_S,"e_store_type_limit_buy_119",hld::e_store_type_limit_buy_119);
   tolua_constant(tolua_S,"e_store_type_limit_buy_120",hld::e_store_type_limit_buy_120);
   tolua_constant(tolua_S,"e_store_type_limit_buy_121",hld::e_store_type_limit_buy_121);
   tolua_constant(tolua_S,"e_store_type_limit_buy_122",hld::e_store_type_limit_buy_122);
   tolua_constant(tolua_S,"e_store_type_limit_buy_123",hld::e_store_type_limit_buy_123);
   tolua_constant(tolua_S,"e_store_type_limit_buy_124",hld::e_store_type_limit_buy_124);
   tolua_constant(tolua_S,"e_store_type_limit_buy_125",hld::e_store_type_limit_buy_125);
   tolua_constant(tolua_S,"e_store_type_limit_buy_126",hld::e_store_type_limit_buy_126);
   tolua_constant(tolua_S,"e_store_type_limit_buy_127",hld::e_store_type_limit_buy_127);
   tolua_constant(tolua_S,"e_store_type_limit_buy_128",hld::e_store_type_limit_buy_128);
   tolua_constant(tolua_S,"e_store_type_limit_buy_129",hld::e_store_type_limit_buy_129);
   tolua_constant(tolua_S,"e_store_type_limit_buy_130",hld::e_store_type_limit_buy_130);
   tolua_constant(tolua_S,"e_store_type_limit_buy_131",hld::e_store_type_limit_buy_131);
   tolua_constant(tolua_S,"e_store_type_limit_buy_132",hld::e_store_type_limit_buy_132);
   tolua_constant(tolua_S,"e_store_type_limit_buy_133",hld::e_store_type_limit_buy_133);
   tolua_constant(tolua_S,"e_store_type_limit_buy_134",hld::e_store_type_limit_buy_134);
   tolua_constant(tolua_S,"e_store_type_limit_buy_135",hld::e_store_type_limit_buy_135);
   tolua_constant(tolua_S,"e_store_type_limit_buy_136",hld::e_store_type_limit_buy_136);
   tolua_constant(tolua_S,"e_store_type_limit_buy_137",hld::e_store_type_limit_buy_137);
   tolua_constant(tolua_S,"e_store_type_limit_buy_138",hld::e_store_type_limit_buy_138);
   tolua_constant(tolua_S,"e_store_type_limit_buy_139",hld::e_store_type_limit_buy_139);
   tolua_constant(tolua_S,"e_store_type_limit_buy_140",hld::e_store_type_limit_buy_140);
   tolua_constant(tolua_S,"e_store_type_max",hld::e_store_type_max);
   tolua_constant(tolua_S,"e_element_heart_operate_get_one",hld::e_element_heart_operate_get_one);
   tolua_constant(tolua_S,"e_element_heart_operate_get_ten",hld::e_element_heart_operate_get_ten);
   tolua_constant(tolua_S,"e_element_heart_operate_upgrade",hld::e_element_heart_operate_upgrade);
   tolua_constant(tolua_S,"e_element_heart_operate_equip_on",hld::e_element_heart_operate_equip_on);
   tolua_constant(tolua_S,"e_element_heart_operate_equip_off",hld::e_element_heart_operate_equip_off);
   tolua_constant(tolua_S,"e_element_heart_operate_summon",hld::e_element_heart_operate_summon);
   tolua_constant(tolua_S,"e_element_heart_operate_tidy",hld::e_element_heart_operate_tidy);
   tolua_constant(tolua_S,"e_element_heart_operate_purchase_equip_slot",hld::e_element_heart_operate_purchase_equip_slot);
   tolua_constant(tolua_S,"e_element_heart_operate_recovery",hld::e_element_heart_operate_recovery);
   tolua_constant(tolua_S,"e_element_heart_operate_common",hld::e_element_heart_operate_common);
   tolua_constant(tolua_S,"e_element_heart_operate_superior",hld::e_element_heart_operate_superior);
   tolua_constant(tolua_S,"e_element_heart_operate_rare",hld::e_element_heart_operate_rare);
   tolua_constant(tolua_S,"e_element_heart_operate_epic",hld::e_element_heart_operate_epic);
   tolua_constant(tolua_S,"e_element_heart_operate_legend",hld::e_element_heart_operate_legend);
   tolua_constant(tolua_S,"e_element_heart_operate_merge",hld::e_element_heart_operate_merge);
   tolua_constant(tolua_S,"e_element_heart_operate_core_unlock",hld::e_element_heart_operate_core_unlock);
   tolua_constant(tolua_S,"e_element_heart_operate_core_upgrade",hld::e_element_heart_operate_core_upgrade);
   tolua_constant(tolua_S,"e_element_heart_operate_max",hld::e_element_heart_operate_max);
   tolua_constant(tolua_S,"e_top_tip_type_normal",hld::e_top_tip_type_normal);
   tolua_constant(tolua_S,"e_top_tip_type_important",hld::e_top_tip_type_important);
   tolua_constant(tolua_S,"e_top_tip_type_max",hld::e_top_tip_type_max);
   tolua_constant(tolua_S,"e_sprite_operate_fight",hld::e_sprite_operate_fight);
   tolua_constant(tolua_S,"e_sprite_operate_equip",hld::e_sprite_operate_equip);
   tolua_constant(tolua_S,"e_sprite_operate_unequip",hld::e_sprite_operate_unequip);
   tolua_constant(tolua_S,"e_sprite_operate_upgrade",hld::e_sprite_operate_upgrade);
   tolua_constant(tolua_S,"e_sprite_operate_sell",hld::e_sprite_operate_sell);
   tolua_constant(tolua_S,"e_sprite_operate_send_to_item_bag",hld::e_sprite_operate_send_to_item_bag);
   tolua_constant(tolua_S,"e_sprite_operate_tidy",hld::e_sprite_operate_tidy);
   tolua_constant(tolua_S,"e_sprite_operate_auto_sell",hld::e_sprite_operate_auto_sell);
   tolua_constant(tolua_S,"e_sprite_operate_purchase_equip_slot",hld::e_sprite_operate_purchase_equip_slot);
   tolua_constant(tolua_S,"e_sprite_operate_purchase_illusion",hld::e_sprite_operate_purchase_illusion);
   tolua_constant(tolua_S,"e_sprite_operate_use_illusion",hld::e_sprite_operate_use_illusion);
   tolua_constant(tolua_S,"e_sprite_operate_star_upgrade",hld::e_sprite_operate_star_upgrade);
   tolua_constant(tolua_S,"e_sprite_operate_max",hld::e_sprite_operate_max);
   tolua_constant(tolua_S,"e_chat_type_none",hld::e_chat_type_none);
   tolua_constant(tolua_S,"e_chat_type_system",hld::e_chat_type_system);
   tolua_constant(tolua_S,"e_chat_type_world",hld::e_chat_type_world);
   tolua_constant(tolua_S,"e_chat_type_legion",hld::e_chat_type_legion);
   tolua_constant(tolua_S,"e_chat_type_team",hld::e_chat_type_team);
   tolua_constant(tolua_S,"e_chat_type_private",hld::e_chat_type_private);
   tolua_constant(tolua_S,"e_chat_type_near",hld::e_chat_type_near);
   tolua_constant(tolua_S,"e_chat_type_total",hld::e_chat_type_total);
   tolua_constant(tolua_S,"e_chat_type_friend",hld::e_chat_type_friend);
   tolua_constant(tolua_S,"e_chat_type_world_recruit",hld::e_chat_type_world_recruit);
   tolua_constant(tolua_S,"e_chat_type_legion_recruit",hld::e_chat_type_legion_recruit);
   tolua_constant(tolua_S,"e_chat_type_radio_host",hld::e_chat_type_radio_host);
   tolua_constant(tolua_S,"e_chat_type_server",hld::e_chat_type_server);
   tolua_constant(tolua_S,"e_chat_type_all_server",hld::e_chat_type_all_server);
   tolua_constant(tolua_S,"e_chat_type_max",hld::e_chat_type_max);
   tolua_constant(tolua_S,"e_recharge_type_none",hld::e_recharge_type_none);
   tolua_constant(tolua_S,"e_recharge_type_vip",hld::e_recharge_type_vip);
   tolua_constant(tolua_S,"e_recharge_type_month_card",hld::e_recharge_type_month_card);
   tolua_constant(tolua_S,"e_recharge_type_exclusive",hld::e_recharge_type_exclusive);
   tolua_constant(tolua_S,"e_recharge_type_max",hld::e_recharge_type_max);
   tolua_constant(tolua_S,"e_relationlist_type_friend",hld::e_relationlist_type_friend);
   tolua_constant(tolua_S,"e_relationlist_type_black",hld::e_relationlist_type_black);
   tolua_constant(tolua_S,"e_relationlist_type_marry",hld::e_relationlist_type_marry);
   tolua_constant(tolua_S,"e_relationlist_type_hate",hld::e_relationlist_type_hate);
   tolua_constant(tolua_S,"e_relationlist_type_friendex",hld::e_relationlist_type_friendex);
   tolua_constant(tolua_S,"e_relationlist_type_other_req_add",hld::e_relationlist_type_other_req_add);
   tolua_constant(tolua_S,"e_relationlist_type_chat",hld::e_relationlist_type_chat);
   tolua_constant(tolua_S,"e_relationlist_type_lucky",hld::e_relationlist_type_lucky);
   tolua_constant(tolua_S,"e_relationlist_type_other_req_add_lucky",hld::e_relationlist_type_other_req_add_lucky);
   tolua_constant(tolua_S,"e_relationlist_type_max",hld::e_relationlist_type_max);
   tolua_constant(tolua_S,"e_first_kill_prize_type_cant_get",hld::e_first_kill_prize_type_cant_get);
   tolua_constant(tolua_S,"e_first_kill_prize_type_not_get",hld::e_first_kill_prize_type_not_get);
   tolua_constant(tolua_S,"e_first_kill_prize_type_was_get",hld::e_first_kill_prize_type_was_get);
   tolua_constant(tolua_S,"e_first_kill_prize_type_max",hld::e_first_kill_prize_type_max);
   tolua_constant(tolua_S,"e_service_goal_state_unfinished",hld::e_service_goal_state_unfinished);
   tolua_constant(tolua_S,"e_service_goal_state_finished",hld::e_service_goal_state_finished);
   tolua_constant(tolua_S,"e_service_goal_state_has_been_received",hld::e_service_goal_state_has_been_received);
   tolua_constant(tolua_S,"e_service_goal_state_max",hld::e_service_goal_state_max);
   tolua_constant(tolua_S,"e_service_goal_inst_data_id",hld::e_service_goal_inst_data_id);
   tolua_constant(tolua_S,"e_service_goal_inst_data_schedule",hld::e_service_goal_inst_data_schedule);
   tolua_constant(tolua_S,"e_service_goal_inst_data_state",hld::e_service_goal_inst_data_state);
   tolua_constant(tolua_S,"e_service_goal_inst_data_active_state",hld::e_service_goal_inst_data_active_state);
   tolua_constant(tolua_S,"e_service_goal_inst_data_over_time",hld::e_service_goal_inst_data_over_time);
   tolua_constant(tolua_S,"e_service_goal_inst_data_max",hld::e_service_goal_inst_data_max);
   tolua_constant(tolua_S,"e_service_goal_operate_type_none",hld::e_service_goal_operate_type_none);
   tolua_constant(tolua_S,"e_service_goal_operate_type_receive",hld::e_service_goal_operate_type_receive);
   tolua_constant(tolua_S,"e_service_goal_operate_type_max",hld::e_service_goal_operate_type_max);
   tolua_constant(tolua_S,"e_service_goal_type_role_level",hld::e_service_goal_type_role_level);
   tolua_constant(tolua_S,"e_service_goal_type_role_gs",hld::e_service_goal_type_role_gs);
   tolua_constant(tolua_S,"e_service_goal_type_big_reward",hld::e_service_goal_type_big_reward);
   tolua_constant(tolua_S,"e_service_goal_type_strength_any",hld::e_service_goal_type_strength_any);
   tolua_constant(tolua_S,"e_service_goal_type_addition_the_body",hld::e_service_goal_type_addition_the_body);
   tolua_constant(tolua_S,"e_service_goal_type_mount_level",hld::e_service_goal_type_mount_level);
   tolua_constant(tolua_S,"e_service_goal_type_mount_num",hld::e_service_goal_type_mount_num);
   tolua_constant(tolua_S,"e_service_goal_type_friend_num",hld::e_service_goal_type_friend_num);
   tolua_constant(tolua_S,"e_service_goal_type_friendliness",hld::e_service_goal_type_friendliness);
   tolua_constant(tolua_S,"e_service_goal_type_clearance_fuben",hld::e_service_goal_type_clearance_fuben);
   tolua_constant(tolua_S,"e_service_goal_type_body_has_color_equip",hld::e_service_goal_type_body_has_color_equip);
   tolua_constant(tolua_S,"e_service_goal_type_legion_donate_num",hld::e_service_goal_type_legion_donate_num);
   tolua_constant(tolua_S,"e_service_goal_type_soldier_rank_level",hld::e_service_goal_type_soldier_rank_level);
   tolua_constant(tolua_S,"e_service_goal_type_arena_rank_level",hld::e_service_goal_type_arena_rank_level);
   tolua_constant(tolua_S,"e_service_goal_type_actived_illustration_num",hld::e_service_goal_type_actived_illustration_num);
   tolua_constant(tolua_S,"e_service_goal_type_actived_any_chapter_illustration",hld::e_service_goal_type_actived_any_chapter_illustration);
   tolua_constant(tolua_S,"e_service_goal_type_demons_tower_layer_num",hld::e_service_goal_type_demons_tower_layer_num);
   tolua_constant(tolua_S,"e_service_goal_type_achievement_title",hld::e_service_goal_type_achievement_title);
   tolua_constant(tolua_S,"e_service_goal_type_wing_level",hld::e_service_goal_type_wing_level);
   tolua_constant(tolua_S,"e_service_goal_type_element_heart_total_level",hld::e_service_goal_type_element_heart_total_level);
   tolua_constant(tolua_S,"e_service_goal_type_equiped_element_heart_num",hld::e_service_goal_type_equiped_element_heart_num);
   tolua_constant(tolua_S,"e_service_goal_type_equiped_spirit_total_level",hld::e_service_goal_type_equiped_spirit_total_level);
   tolua_constant(tolua_S,"e_service_goal_type_equiped_spirit_num",hld::e_service_goal_type_equiped_spirit_num);
   tolua_constant(tolua_S,"e_service_goal_type_belief_rank",hld::e_service_goal_type_belief_rank);
   tolua_constant(tolua_S,"e_service_goal_type_get_all_reward",hld::e_service_goal_type_get_all_reward);
   tolua_constant(tolua_S,"e_service_goal_type_final",hld::e_service_goal_type_final);
   tolua_constant(tolua_S,"e_service_goal_type_god_hood_grade",hld::e_service_goal_type_god_hood_grade);
   tolua_constant(tolua_S,"e_service_goal_type_seven_day_final",hld::e_service_goal_type_seven_day_final);
   tolua_constant(tolua_S,"e_service_goal_type_no_limit_time_final",hld::e_service_goal_type_no_limit_time_final);
   tolua_constant(tolua_S,"e_service_goal_type_awaken_level",hld::e_service_goal_type_awaken_level);
   tolua_constant(tolua_S,"e_service_goal_type_max",hld::e_service_goal_type_max);
   tolua_constant(tolua_S,"e_service_goal_active_type_basis",hld::e_service_goal_active_type_basis);
   tolua_constant(tolua_S,"e_service_goal_active_type_advance",hld::e_service_goal_active_type_advance);
   tolua_constant(tolua_S,"e_service_goal_active_type_final",hld::e_service_goal_active_type_final);
   tolua_constant(tolua_S,"e_service_goal_active_type_seven_day",hld::e_service_goal_active_type_seven_day);
   tolua_constant(tolua_S,"e_service_goal_active_type_max",hld::e_service_goal_active_type_max);
   tolua_constant(tolua_S,"e_team_operate_follow_cap",hld::e_team_operate_follow_cap);
   tolua_constant(tolua_S,"e_team_operate_urge_cap",hld::e_team_operate_urge_cap);
   tolua_constant(tolua_S,"e_team_operate_call_together",hld::e_team_operate_call_together);
   tolua_constant(tolua_S,"e_team_operate_urge_together",hld::e_team_operate_urge_together);
   tolua_constant(tolua_S,"e_team_operate_max",hld::e_team_operate_max);
   tolua_constant(tolua_S,"e_relation_operate_result_success",hld::e_relation_operate_result_success);
   tolua_constant(tolua_S,"e_relation_operate_result_offline",hld::e_relation_operate_result_offline);
   tolua_constant(tolua_S,"e_relation_operate_result_add_friend_success",hld::e_relation_operate_result_add_friend_success);
   tolua_constant(tolua_S,"e_relation_operate_result_del_friend_success",hld::e_relation_operate_result_del_friend_success);
   tolua_constant(tolua_S,"e_relation_operate_result_add_black_success",hld::e_relation_operate_result_add_black_success);
   tolua_constant(tolua_S,"e_relation_operate_result_del_black_success",hld::e_relation_operate_result_del_black_success);
   tolua_constant(tolua_S,"e_relation_operate_result_add_hate_success",hld::e_relation_operate_result_add_hate_success);
   tolua_constant(tolua_S,"e_relation_operate_result_add_friend_all_once",hld::e_relation_operate_result_add_friend_all_once);
   tolua_constant(tolua_S,"e_relation_operate_result_cant_operate_self",hld::e_relation_operate_result_cant_operate_self);
   tolua_constant(tolua_S,"e_relation_operate_result_friend_list_full",hld::e_relation_operate_result_friend_list_full);
   tolua_constant(tolua_S,"e_relation_operate_result_hate_list_full",hld::e_relation_operate_result_hate_list_full);
   tolua_constant(tolua_S,"e_relation_operate_result_black_list_full",hld::e_relation_operate_result_black_list_full);
   tolua_constant(tolua_S,"e_relation_operate_result_friend_exist",hld::e_relation_operate_result_friend_exist);
   tolua_constant(tolua_S,"e_relation_operate_result_black_exist",hld::e_relation_operate_result_black_exist);
   tolua_constant(tolua_S,"e_relation_operate_result_hate_exist",hld::e_relation_operate_result_hate_exist);
   tolua_constant(tolua_S,"e_relation_operate_result_too_fast",hld::e_relation_operate_result_too_fast);
   tolua_constant(tolua_S,"e_relation_operate_result_req_friend_success",hld::e_relation_operate_result_req_friend_success);
   tolua_constant(tolua_S,"e_relation_operate_result_pass_req_friend",hld::e_relation_operate_result_pass_req_friend);
   tolua_constant(tolua_S,"e_relation_operate_result_other_add_friend",hld::e_relation_operate_result_other_add_friend);
   tolua_constant(tolua_S,"e_relation_operate_result_other_is_cross_server",hld::e_relation_operate_result_other_is_cross_server);
   tolua_constant(tolua_S,"e_relation_operate_result_max",hld::e_relation_operate_result_max);
   tolua_constant(tolua_S,"e_item_wing_type_non",hld::e_item_wing_type_non);
   tolua_constant(tolua_S,"e_item_wing_type_normal",hld::e_item_wing_type_normal);
   tolua_constant(tolua_S,"e_item_wing_type_shape_no_illusion",hld::e_item_wing_type_shape_no_illusion);
   tolua_constant(tolua_S,"e_item_wing_type_shape_illusion",hld::e_item_wing_type_shape_illusion);
   tolua_constant(tolua_S,"e_item_wing_type_illusion",hld::e_item_wing_type_illusion);
   tolua_constant(tolua_S,"e_item_wing_type_special_santo",hld::e_item_wing_type_special_santo);
   tolua_constant(tolua_S,"e_item_wing_type_max",hld::e_item_wing_type_max);
   tolua_constant(tolua_S,"e_convert_type_bind_siliver",hld::e_convert_type_bind_siliver);
   tolua_constant(tolua_S,"e_convert_type_exp",hld::e_convert_type_exp);
   tolua_constant(tolua_S,"e_convert_type_bind_diamond",hld::e_convert_type_bind_diamond);
   tolua_constant(tolua_S,"e_convert_type_bag_bind_siliver",hld::e_convert_type_bag_bind_siliver);
   tolua_constant(tolua_S,"e_convert_type_bag_siliver",hld::e_convert_type_bag_siliver);
   tolua_constant(tolua_S,"e_convert_type_max",hld::e_convert_type_max);
   tolua_constant(tolua_S,"e_belief_series_type_spring",hld::e_belief_series_type_spring);
   tolua_constant(tolua_S,"e_belief_series_type_summer",hld::e_belief_series_type_summer);
   tolua_constant(tolua_S,"e_belief_series_type_autumn",hld::e_belief_series_type_autumn);
   tolua_constant(tolua_S,"e_belief_series_type_winter",hld::e_belief_series_type_winter);
   tolua_constant(tolua_S,"e_belief_series_type_max",hld::e_belief_series_type_max);
   tolua_constant(tolua_S,"e_belief_buff_level_index_first",hld::e_belief_buff_level_index_first);
   tolua_constant(tolua_S,"e_belief_buff_level_index_second",hld::e_belief_buff_level_index_second);
   tolua_constant(tolua_S,"e_belief_buff_level_index_third",hld::e_belief_buff_level_index_third);
   tolua_constant(tolua_S,"e_belief_buff_level_max",hld::e_belief_buff_level_max);
   tolua_constant(tolua_S,"e_legion_call_type_call_legion_boss",hld::e_legion_call_type_call_legion_boss);
   tolua_constant(tolua_S,"e_legion_call_type_call_city_war",hld::e_legion_call_type_call_city_war);
   tolua_constant(tolua_S,"e_legion_call_type_call_bonfire",hld::e_legion_call_type_call_bonfire);
   tolua_constant(tolua_S,"e_legion_call_type_call_elite_boss",hld::e_legion_call_type_call_elite_boss);
   tolua_constant(tolua_S,"e_legion_call_type_call_world_boss",hld::e_legion_call_type_call_world_boss);
   tolua_constant(tolua_S,"e_legion_call_type_max",hld::e_legion_call_type_max);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_low",hld::e_reduce_pk_value_prop_quality_low);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_mid",hld::e_reduce_pk_value_prop_quality_mid);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_high",hld::e_reduce_pk_value_prop_quality_high);
   tolua_constant(tolua_S,"e_reduce_pk_value_prop_quality_max",hld::e_reduce_pk_value_prop_quality_max);
   tolua_constant(tolua_S,"e_spirit_operate_result_unknown",hld::e_spirit_operate_result_unknown);
   tolua_constant(tolua_S,"e_spirit_operate_result_success",hld::e_spirit_operate_result_success);
   tolua_constant(tolua_S,"e_spirit_operate_result_no_space",hld::e_spirit_operate_result_no_space);
   tolua_constant(tolua_S,"e_spirit_operate_result_max",hld::e_spirit_operate_result_max);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_exp",hld::e_daily_must_do_typ_raid_exp);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_daily",hld::e_daily_must_do_typ_raid_daily);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_devil_square",hld::e_daily_must_do_typ_raid_devil_square);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_ancient_battlefield",hld::e_daily_must_do_typ_raid_ancient_battlefield);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_siliver",hld::e_daily_must_do_typ_raid_siliver);
   tolua_constant(tolua_S,"e_daily_must_do_typ_community_fight",hld::e_daily_must_do_typ_community_fight);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_drame",hld::e_daily_must_do_typ_raid_drame);
   tolua_constant(tolua_S,"e_daily_must_do_typ_bloody_castle",hld::e_daily_must_do_typ_bloody_castle);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_multiple_princess_guard_war",hld::e_daily_must_do_typ_raid_multiple_princess_guard_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_multiple_lava_hellfire",hld::e_daily_must_do_typ_raid_multiple_lava_hellfire);
   tolua_constant(tolua_S,"e_daily_must_do_typ_raid_multiple_abyss_of_fear",hld::e_daily_must_do_typ_raid_multiple_abyss_of_fear);
   tolua_constant(tolua_S,"e_daily_must_do_typ_demon_tower",hld::e_daily_must_do_typ_demon_tower);
   tolua_constant(tolua_S,"e_daily_must_do_typ_angel_temple",hld::e_daily_must_do_typ_angel_temple);
   tolua_constant(tolua_S,"e_daily_must_do_typ_pk_king",hld::e_daily_must_do_typ_pk_king);
   tolua_constant(tolua_S,"e_daily_must_do_typ_arena",hld::e_daily_must_do_typ_arena);
   tolua_constant(tolua_S,"e_daily_must_do_typ_crusade_mission",hld::e_daily_must_do_typ_crusade_mission);
   tolua_constant(tolua_S,"e_daily_must_do_typ_crystal_illusion",hld::e_daily_must_do_typ_crystal_illusion);
   tolua_constant(tolua_S,"e_daily_must_do_typ_mingmie_xukong",hld::e_daily_must_do_typ_mingmie_xukong);
   tolua_constant(tolua_S,"e_daily_must_do_typ_out_board",hld::e_daily_must_do_typ_out_board);
   tolua_constant(tolua_S,"e_daily_must_do_typ_convert",hld::e_daily_must_do_typ_convert);
   tolua_constant(tolua_S,"e_daily_must_do_typ_rank",hld::e_daily_must_do_typ_rank);
   tolua_constant(tolua_S,"e_daily_must_do_typ_gold_army",hld::e_daily_must_do_typ_gold_army);
   tolua_constant(tolua_S,"e_daily_must_do_typ_world_boss",hld::e_daily_must_do_typ_world_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_sky_treasure_box",hld::e_daily_must_do_typ_sky_treasure_box);
   tolua_constant(tolua_S,"e_daily_must_do_typ_field_treasure_box",hld::e_daily_must_do_typ_field_treasure_box);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_boss",hld::e_daily_must_do_typ_legion_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_meditation",hld::e_daily_must_do_typ_meditation);
   tolua_constant(tolua_S,"e_daily_must_do_typ_activaty_degree",hld::e_daily_must_do_typ_activaty_degree);
   tolua_constant(tolua_S,"e_daily_must_do_typ_city_war",hld::e_daily_must_do_typ_city_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_fight",hld::e_daily_must_do_typ_cross_fight);
   tolua_constant(tolua_S,"e_daily_must_do_typ_gold_store",hld::e_daily_must_do_typ_gold_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_magic_crystal_store",hld::e_daily_must_do_typ_magic_crystal_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_ancient_coin_store",hld::e_daily_must_do_typ_ancient_coin_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_hope_store",hld::e_daily_must_do_typ_hope_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_store",hld::e_daily_must_do_typ_legion_store);
   tolua_constant(tolua_S,"e_daily_must_do_typ_hope_item",hld::e_daily_must_do_typ_hope_item);
   tolua_constant(tolua_S,"e_daily_must_do_typ_shop",hld::e_daily_must_do_typ_shop);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_bonfire",hld::e_daily_must_do_typ_legion_bonfire);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_war",hld::e_daily_must_do_typ_legion_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_boss_home",hld::e_daily_must_do_typ_boss_home);
   tolua_constant(tolua_S,"e_daily_must_do_typ_boss_island",hld::e_daily_must_do_typ_boss_island);
   tolua_constant(tolua_S,"e_daily_must_do_typ_belief_cloister",hld::e_daily_must_do_typ_belief_cloister);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_pk",hld::e_daily_must_do_typ_cross_pk);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_rob",hld::e_daily_must_do_typ_cross_rob);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_dance",hld::e_daily_must_do_typ_legion_dance);
   tolua_constant(tolua_S,"e_daily_must_do_typ_legion_answer",hld::e_daily_must_do_typ_legion_answer);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_ladder",hld::e_daily_must_do_typ_cross_ladder);
   tolua_constant(tolua_S,"e_daily_must_do_typ_single_boss",hld::e_daily_must_do_typ_single_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_element_war",hld::e_daily_must_do_typ_element_war);
   tolua_constant(tolua_S,"e_daily_must_do_typ_vip_boss_home",hld::e_daily_must_do_typ_vip_boss_home);
   tolua_constant(tolua_S,"e_daily_must_do_typ_cross_server_boss",hld::e_daily_must_do_typ_cross_server_boss);
   tolua_constant(tolua_S,"e_daily_must_do_typ_oracle_trial",hld::e_daily_must_do_typ_oracle_trial);
   tolua_constant(tolua_S,"e_daily_must_do_typ_attack_city",hld::e_daily_must_do_typ_attack_city);
   tolua_constant(tolua_S,"e_daily_must_do_typ_max",hld::e_daily_must_do_typ_max);
   tolua_constant(tolua_S,"e_daily_active_degree_type_cost_one_diamond",hld::e_daily_active_degree_type_cost_one_diamond);
   tolua_constant(tolua_S,"e_daily_active_degree_type_ten_daily_mission",hld::e_daily_active_degree_type_ten_daily_mission);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_normal_raid",hld::e_daily_active_degree_type_three_normal_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_elite_raid",hld::e_daily_active_degree_type_three_elite_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_hell_raid",hld::e_daily_active_degree_type_three_hell_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_empire_raid",hld::e_daily_active_degree_type_empire_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_temple_raid",hld::e_daily_active_degree_type_temple_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_warp_raid",hld::e_daily_active_degree_type_warp_raid);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_upgrade",hld::e_daily_active_degree_type_three_upgrade);
   tolua_constant(tolua_S,"e_daily_active_degree_type_three_addon",hld::e_daily_active_degree_type_three_addon);
   tolua_constant(tolua_S,"e_daily_active_degree_type_kill_200_monster",hld::e_daily_active_degree_type_kill_200_monster);
   tolua_constant(tolua_S,"e_daily_active_degree_type_kill_500_monster",hld::e_daily_active_degree_type_kill_500_monster);
   tolua_constant(tolua_S,"e_daily_active_degree_type_kill_1000_monster",hld::e_daily_active_degree_type_kill_1000_monster);
   tolua_constant(tolua_S,"e_daily_active_degree_type_login",hld::e_daily_active_degree_type_login);
   tolua_constant(tolua_S,"e_daily_active_degree_type_online_30_minutes",hld::e_daily_active_degree_type_online_30_minutes);
   tolua_constant(tolua_S,"e_daily_active_degree_type_strengthing_one_spirit",hld::e_daily_active_degree_type_strengthing_one_spirit);
   tolua_constant(tolua_S,"e_daily_active_degree_type_strengthing_one_mount",hld::e_daily_active_degree_type_strengthing_one_mount);
   tolua_constant(tolua_S,"e_daily_active_degree_type_strengthing_one_wing",hld::e_daily_active_degree_type_strengthing_one_wing);
   tolua_constant(tolua_S,"e_daily_active_degree_type_upgrade_one_element_heart",hld::e_daily_active_degree_type_upgrade_one_element_heart);
   tolua_constant(tolua_S,"e_daily_active_degree_type_upgrade_one_patron_saint",hld::e_daily_active_degree_type_upgrade_one_patron_saint);
   tolua_constant(tolua_S,"e_daily_active_degree_type_upgrade_one_belief",hld::e_daily_active_degree_type_upgrade_one_belief);
   tolua_constant(tolua_S,"e_daily_active_degree_type_submit_one_illustration",hld::e_daily_active_degree_type_submit_one_illustration);
   tolua_constant(tolua_S,"e_daily_active_degree_type_attend_three_arena",hld::e_daily_active_degree_type_attend_three_arena);
   tolua_constant(tolua_S,"e_daily_active_degree_type_attend_one_city_war",hld::e_daily_active_degree_type_attend_one_city_war);
   tolua_constant(tolua_S,"e_daily_active_degree_type_attend_one_legion_war",hld::e_daily_active_degree_type_attend_one_legion_war);
   tolua_constant(tolua_S,"e_daily_active_degree_type_max",hld::e_daily_active_degree_type_max);
   tolua_constant(tolua_S,"e_notice_param_go_to_location",hld::e_notice_param_go_to_location);
   tolua_constant(tolua_S,"e_notice_param_open_widget",hld::e_notice_param_open_widget);
   tolua_constant(tolua_S,"e_notice_param_item_info",hld::e_notice_param_item_info);
   tolua_constant(tolua_S,"e_notice_param_max",hld::e_notice_param_max);
   tolua_constant(tolua_S,"e_RankingIndex_gs",hld::e_RankingIndex_gs);
   tolua_constant(tolua_S,"e_RankingIndex_level",hld::e_RankingIndex_level);
   tolua_constant(tolua_S,"e_RankingIndex_box_map_level",hld::e_RankingIndex_box_map_level);
   tolua_constant(tolua_S,"e_RankingIndex_demonstower",hld::e_RankingIndex_demonstower);
   tolua_constant(tolua_S,"e_RankingIndex_wing",hld::e_RankingIndex_wing);
   tolua_constant(tolua_S,"e_RankingIndex_mount",hld::e_RankingIndex_mount);
   tolua_constant(tolua_S,"e_RankingIndex_worship",hld::e_RankingIndex_worship);
   tolua_constant(tolua_S,"e_RankingIndex_arena",hld::e_RankingIndex_arena);
   tolua_constant(tolua_S,"e_RankingIndex_legion_core",hld::e_RankingIndex_legion_core);
   tolua_constant(tolua_S,"e_RankingIndex_gs_barserker",hld::e_RankingIndex_gs_barserker);
   tolua_constant(tolua_S,"e_RankingIndex_gs_wizard",hld::e_RankingIndex_gs_wizard);
   tolua_constant(tolua_S,"e_RankingIndex_gs_guardian",hld::e_RankingIndex_gs_guardian);
   tolua_constant(tolua_S,"e_RankingIndex_gs_assassinator",hld::e_RankingIndex_gs_assassinator);
   tolua_constant(tolua_S,"e_RankingIndex_level_barserker",hld::e_RankingIndex_level_barserker);
   tolua_constant(tolua_S,"e_RankingIndex_level_wizard",hld::e_RankingIndex_level_wizard);
   tolua_constant(tolua_S,"e_RankingIndex_level_guardian",hld::e_RankingIndex_level_guardian);
   tolua_constant(tolua_S,"e_RankingIndex_level_assassinator",hld::e_RankingIndex_level_assassinator);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_main",hld::e_RankingIndex_equipment_main);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_weapon",hld::e_RankingIndex_equipment_weapon);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_necklace",hld::e_RankingIndex_equipment_necklace);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_armor",hld::e_RankingIndex_equipment_armor);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_pants",hld::e_RankingIndex_equipment_pants);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_hat",hld::e_RankingIndex_equipment_hat);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_shoes",hld::e_RankingIndex_equipment_shoes);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_glove",hld::e_RankingIndex_equipment_glove);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_ring",hld::e_RankingIndex_equipment_ring);
   tolua_constant(tolua_S,"e_RankingIndex_equipment_earring",hld::e_RankingIndex_equipment_earring);
   tolua_constant(tolua_S,"e_RankingIndex_mount_train",hld::e_RankingIndex_mount_train);
   tolua_constant(tolua_S,"e_RankingIndex_wing_train",hld::e_RankingIndex_wing_train);
   tolua_constant(tolua_S,"e_RankingIndex_elves_train",hld::e_RankingIndex_elves_train);
   tolua_constant(tolua_S,"e_RankingIndex_belief",hld::e_RankingIndex_belief);
   tolua_constant(tolua_S,"e_RankingIndex_element",hld::e_RankingIndex_element);
   tolua_constant(tolua_S,"e_RankingIndex_inspiration",hld::e_RankingIndex_inspiration);
   tolua_constant(tolua_S,"e_RankingIndex_military_rank",hld::e_RankingIndex_military_rank);
   tolua_constant(tolua_S,"e_RankingIndex_achievement",hld::e_RankingIndex_achievement);
   tolua_constant(tolua_S,"e_RankingIndex_illustrated",hld::e_RankingIndex_illustrated);
   tolua_constant(tolua_S,"e_RankingIndex_legion_territory",hld::e_RankingIndex_legion_territory);
   tolua_constant(tolua_S,"e_RankingIndex_legion_boss",hld::e_RankingIndex_legion_boss);
   tolua_constant(tolua_S,"e_RankingIndex_legion_bonfire",hld::e_RankingIndex_legion_bonfire);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_one",hld::e_RankingIndex_world_boss_one);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_two",hld::e_RankingIndex_world_boss_two);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_three",hld::e_RankingIndex_world_boss_three);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_four",hld::e_RankingIndex_world_boss_four);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_five",hld::e_RankingIndex_world_boss_five);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_six",hld::e_RankingIndex_world_boss_six);
   tolua_constant(tolua_S,"e_RankingIndex_world_boss_seven",hld::e_RankingIndex_world_boss_seven);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_one",hld::e_RankingIndex_world_elite_one);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_two",hld::e_RankingIndex_world_elite_two);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_three",hld::e_RankingIndex_world_elite_three);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_four",hld::e_RankingIndex_world_elite_four);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_five",hld::e_RankingIndex_world_elite_five);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_six",hld::e_RankingIndex_world_elite_six);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_seven",hld::e_RankingIndex_world_elite_seven);
   tolua_constant(tolua_S,"e_RankingIndex_world_elite_eight",hld::e_RankingIndex_world_elite_eight);
   tolua_constant(tolua_S,"e_RankingIndex_elves_gs",hld::e_RankingIndex_elves_gs);
   tolua_constant(tolua_S,"e_RankingIndex_vip_level_one",hld::e_RankingIndex_vip_level_one);
   tolua_constant(tolua_S,"e_RankingIndex_vip_level_five",hld::e_RankingIndex_vip_level_five);
   tolua_constant(tolua_S,"e_RankingIndex_vip_level_seven",hld::e_RankingIndex_vip_level_seven);
   tolua_constant(tolua_S,"e_RankingIndex_recharge_rmb",hld::e_RankingIndex_recharge_rmb);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_total_recharge",hld::e_RankingIndex_time_limit_activity_total_recharge);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_total_cost",hld::e_RankingIndex_time_limit_activity_total_cost);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_treasure",hld::e_RankingIndex_time_limit_activity_treasure);
   tolua_constant(tolua_S,"e_RankingIndex_gs_all",hld::e_RankingIndex_gs_all);
   tolua_constant(tolua_S,"e_RankingIndex_cross_server_plinder_guard_value",hld::e_RankingIndex_cross_server_plinder_guard_value);
   tolua_constant(tolua_S,"e_RankingIndex_legion_gs_all",hld::e_RankingIndex_legion_gs_all);
   tolua_constant(tolua_S,"e_RankingIndex_cross_ladder_score",hld::e_RankingIndex_cross_ladder_score);
   tolua_constant(tolua_S,"e_RankingIndex_feather_heart",hld::e_RankingIndex_feather_heart);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_one",hld::e_RankingIndex_cross_boss_one);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_two",hld::e_RankingIndex_cross_boss_two);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_three",hld::e_RankingIndex_cross_boss_three);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_four",hld::e_RankingIndex_cross_boss_four);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_five",hld::e_RankingIndex_cross_boss_five);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_six",hld::e_RankingIndex_cross_boss_six);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_seven",hld::e_RankingIndex_cross_boss_seven);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_eight",hld::e_RankingIndex_cross_boss_eight);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_nine",hld::e_RankingIndex_cross_boss_nine);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_ten",hld::e_RankingIndex_cross_boss_ten);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_eleven",hld::e_RankingIndex_cross_boss_eleven);
   tolua_constant(tolua_S,"e_RankingIndex_cross_boss_tweleve",hld::e_RankingIndex_cross_boss_tweleve);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_gs_up",hld::e_RankingIndex_time_limit_activity_gs_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_mount_up",hld::e_RankingIndex_time_limit_activity_mount_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_wing_up",hld::e_RankingIndex_time_limit_activity_wing_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_equip_up",hld::e_RankingIndex_time_limit_activity_equip_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_spirit_up",hld::e_RankingIndex_time_limit_activity_spirit_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_gs_up",hld::e_RankingIndex_time_limit_activity_cross_gs_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_mount_up",hld::e_RankingIndex_time_limit_activity_cross_mount_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_wing_up",hld::e_RankingIndex_time_limit_activity_cross_wing_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_equip_up",hld::e_RankingIndex_time_limit_activity_cross_equip_up);
   tolua_constant(tolua_S,"e_RankingIndex_time_limit_activity_cross_spirit_up",hld::e_RankingIndex_time_limit_activity_cross_spirit_up);
   tolua_constant(tolua_S,"e_RankingIndex_level_all",hld::e_RankingIndex_level_all);
   tolua_constant(tolua_S,"e_RankingIndex_couple",hld::e_RankingIndex_couple);
   tolua_constant(tolua_S,"e_RankingIndex_Oracle_Trial",hld::e_RankingIndex_Oracle_Trial);
   tolua_constant(tolua_S,"e_RankingIndex_cross_ladder_last_seaon",hld::e_RankingIndex_cross_ladder_last_seaon);
   tolua_constant(tolua_S,"e_RankingIndex_cross_ladder_cur_season",hld::e_RankingIndex_cross_ladder_cur_season);
   tolua_constant(tolua_S,"e_RankingIndex_pk_king",hld::e_RankingIndex_pk_king);
   tolua_constant(tolua_S,"e_RankingIndex_element_rank_list",hld::e_RankingIndex_element_rank_list);
   tolua_constant(tolua_S,"e_RankingIndex_element_war_cur_season",hld::e_RankingIndex_element_war_cur_season);
   tolua_constant(tolua_S,"e_RankingIndex_element_war_last_season",hld::e_RankingIndex_element_war_last_season);
   tolua_constant(tolua_S,"e_RankingIndex_cross_time_limit_activity_total_recharge",hld::e_RankingIndex_cross_time_limit_activity_total_recharge);
   tolua_constant(tolua_S,"e_RankingIndex_max",hld::e_RankingIndex_max);
   tolua_constant(tolua_S,"e_mount_sub_type_none",hld::e_mount_sub_type_none);
   tolua_constant(tolua_S,"e_mount_sub_type_origin",hld::e_mount_sub_type_origin);
   tolua_constant(tolua_S,"e_mount_sub_type_illusion",hld::e_mount_sub_type_illusion);
   tolua_constant(tolua_S,"e_mount_sub_type_max",hld::e_mount_sub_type_max);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_none",hld::e_goods_purchase_limit_type_none);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_per_day",hld::e_goods_purchase_limit_type_per_day);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_forever",hld::e_goods_purchase_limit_type_forever);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_per_week",hld::e_goods_purchase_limit_type_per_week);
   tolua_constant(tolua_S,"e_goods_purchase_limit_type_max",hld::e_goods_purchase_limit_type_max);
   tolua_constant(tolua_S,"e_update_item_info_mode_default",hld::e_update_item_info_mode_default);
   tolua_constant(tolua_S,"e_update_item_info_mode_pick_up_new",hld::e_update_item_info_mode_pick_up_new);
   tolua_constant(tolua_S,"e_update_item_info_mode_update_all",hld::e_update_item_info_mode_update_all);
   tolua_constant(tolua_S,"e_update_item_info_mode_max",hld::e_update_item_info_mode_max);
   tolua_constant(tolua_S,"e_exhibition_actor_array_model",hld::e_exhibition_actor_array_model);
   tolua_constant(tolua_S,"e_exhibition_actor_array_item",hld::e_exhibition_actor_array_item);
   tolua_constant(tolua_S,"e_exhibition_actor_array_max",hld::e_exhibition_actor_array_max);
   tolua_constant(tolua_S,"e_auction_operate_type_non",hld::e_auction_operate_type_non);
   tolua_constant(tolua_S,"e_auction_operate_type_sell",hld::e_auction_operate_type_sell);
   tolua_constant(tolua_S,"e_auction_operate_type_purchase",hld::e_auction_operate_type_purchase);
   tolua_constant(tolua_S,"e_auction_operate_type_cancel_sell",hld::e_auction_operate_type_cancel_sell);
   tolua_constant(tolua_S,"e_auction_operate_type_bid",hld::e_auction_operate_type_bid);
   tolua_constant(tolua_S,"e_auction_operate_type_read_record",hld::e_auction_operate_type_read_record);
   tolua_constant(tolua_S,"e_auction_operate_type_max",hld::e_auction_operate_type_max);
   tolua_constant(tolua_S,"e_trading_type_normal_sell",hld::e_trading_type_normal_sell);
   tolua_constant(tolua_S,"e_trading_type_legion",hld::e_trading_type_legion);
   tolua_constant(tolua_S,"e_trading_type_world",hld::e_trading_type_world);
   tolua_constant(tolua_S,"e_trading_type_max",hld::e_trading_type_max);
   tolua_constant(tolua_S,"e_trading_source_type_non",hld::e_trading_source_type_non);
   tolua_constant(tolua_S,"e_trading_source_type_world_boss",hld::e_trading_source_type_world_boss);
   tolua_constant(tolua_S,"e_trading_source_type_gold_army",hld::e_trading_source_type_gold_army);
   tolua_constant(tolua_S,"e_trading_source_type_bonfire",hld::e_trading_source_type_bonfire);
   tolua_constant(tolua_S,"e_trading_source_type_legion_boss",hld::e_trading_source_type_legion_boss);
   tolua_constant(tolua_S,"e_trading_source_type_max",hld::e_trading_source_type_max);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_green",hld::e_excellentatt_quality_type_green);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_blue",hld::e_excellentatt_quality_type_blue);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_purple",hld::e_excellentatt_quality_type_purple);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_orange",hld::e_excellentatt_quality_type_orange);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_red",hld::e_excellentatt_quality_type_red);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_upper",hld::e_excellentatt_quality_type_upper);
   tolua_constant(tolua_S,"e_excellentatt_quality_type_max",hld::e_excellentatt_quality_type_max);
   tolua_constant(tolua_S,"e_ranking_operate_result_success",hld::e_ranking_operate_result_success);
   tolua_constant(tolua_S,"e_ranking_operate_result_error",hld::e_ranking_operate_result_error);
   tolua_constant(tolua_S,"e_ranking_operate_result_no_worship_value",hld::e_ranking_operate_result_no_worship_value);
   tolua_constant(tolua_S,"e_ranking_operate_result_ranking_no_player",hld::e_ranking_operate_result_ranking_no_player);
   tolua_constant(tolua_S,"e_ranking_operate_result_max",hld::e_ranking_operate_result_max);
   tolua_constant(tolua_S,"e_camera_move_info_boom_length",hld::e_camera_move_info_boom_length);
   tolua_constant(tolua_S,"e_camera_move_info_field_of_view",hld::e_camera_move_info_field_of_view);
   tolua_constant(tolua_S,"e_camera_move_info_camera_location_x",hld::e_camera_move_info_camera_location_x);
   tolua_constant(tolua_S,"e_camera_move_info_camera_location_y",hld::e_camera_move_info_camera_location_y);
   tolua_constant(tolua_S,"e_camera_move_info_camera_location_z",hld::e_camera_move_info_camera_location_z);
   tolua_constant(tolua_S,"e_camera_move_info_camera_rotation_roll",hld::e_camera_move_info_camera_rotation_roll);
   tolua_constant(tolua_S,"e_camera_move_info_camera_rotation_pitch",hld::e_camera_move_info_camera_rotation_pitch);
   tolua_constant(tolua_S,"e_camera_move_info_camera_rotation_yaw",hld::e_camera_move_info_camera_rotation_yaw);
   tolua_constant(tolua_S,"e_camera_move_info_boom_location_x",hld::e_camera_move_info_boom_location_x);
   tolua_constant(tolua_S,"e_camera_move_info_boom_location_y",hld::e_camera_move_info_boom_location_y);
   tolua_constant(tolua_S,"e_camera_move_info_boom_location_z",hld::e_camera_move_info_boom_location_z);
   tolua_constant(tolua_S,"e_camera_move_info_boom_rotation_roll",hld::e_camera_move_info_boom_rotation_roll);
   tolua_constant(tolua_S,"e_camera_move_info_boom_rotation_pitch",hld::e_camera_move_info_boom_rotation_pitch);
   tolua_constant(tolua_S,"e_camera_move_info_boom_rotation_yaw",hld::e_camera_move_info_boom_rotation_yaw);
   tolua_constant(tolua_S,"e_camera_move_info_ani_time",hld::e_camera_move_info_ani_time);
   tolua_constant(tolua_S,"e_camera_move_info_remain_time",hld::e_camera_move_info_remain_time);
   tolua_constant(tolua_S,"e_camera_move_info_max",hld::e_camera_move_info_max);
   tolua_constant(tolua_S,"e_notice_param_type_unit_name",hld::e_notice_param_type_unit_name);
   tolua_constant(tolua_S,"e_notice_param_type_item_name",hld::e_notice_param_type_item_name);
   tolua_constant(tolua_S,"e_notice_param_type_num_data",hld::e_notice_param_type_num_data);
   tolua_constant(tolua_S,"e_notice_param_type_item_data",hld::e_notice_param_type_item_data);
   tolua_constant(tolua_S,"e_notice_param_type_describe_str",hld::e_notice_param_type_describe_str);
   tolua_constant(tolua_S,"e_notice_param_type_other_name",hld::e_notice_param_type_other_name);
   tolua_constant(tolua_S,"e_notice_param_type_max",hld::e_notice_param_type_max);
   tolua_constant(tolua_S,"e_marry_operate_type_success",hld::e_marry_operate_type_success);
   tolua_constant(tolua_S,"e_marry_operate_type_unknow_error",hld::e_marry_operate_type_unknow_error);
   tolua_constant(tolua_S,"e_marry_operate_type_team_error",hld::e_marry_operate_type_team_error);
   tolua_constant(tolua_S,"e_marry_operate_type_level_error",hld::e_marry_operate_type_level_error);
   tolua_constant(tolua_S,"e_marry_operate_type_not_friend",hld::e_marry_operate_type_not_friend);
   tolua_constant(tolua_S,"e_marry_operate_type_low_friendliness",hld::e_marry_operate_type_low_friendliness);
   tolua_constant(tolua_S,"e_marry_operate_type_one_is_cant_marry_state",hld::e_marry_operate_type_one_is_cant_marry_state);
   tolua_constant(tolua_S,"e_marry_operate_type_far_away",hld::e_marry_operate_type_far_away);
   tolua_constant(tolua_S,"e_marry_operate_type_lack_marry_item",hld::e_marry_operate_type_lack_marry_item);
   tolua_constant(tolua_S,"e_marry_operate_type_one_refuse",hld::e_marry_operate_type_one_refuse);
   tolua_constant(tolua_S,"e_marry_operate_type_one_offline",hld::e_marry_operate_type_one_offline);
   tolua_constant(tolua_S,"e_marry_operate_type_can_marry",hld::e_marry_operate_type_can_marry);
   tolua_constant(tolua_S,"e_marry_operate_type_marry_success",hld::e_marry_operate_type_marry_success);
   tolua_constant(tolua_S,"e_marry_operate_type_check_marry_item",hld::e_marry_operate_type_check_marry_item);
   tolua_constant(tolua_S,"e_marry_operate_type_mary_func_unlock",hld::e_marry_operate_type_mary_func_unlock);
   tolua_constant(tolua_S,"e_marry_operate_type_check_marry_success",hld::e_marry_operate_type_check_marry_success);
   tolua_constant(tolua_S,"e_marry_operate_type_single",hld::e_marry_operate_type_single);
   tolua_constant(tolua_S,"e_marry_operate_type_one_agree",hld::e_marry_operate_type_one_agree);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_team_error",hld::e_marry_operate_type_divorce_team_error);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_isnt_marry",hld::e_marry_operate_type_divorce_isnt_marry);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_lack_money",hld::e_marry_operate_type_divorce_lack_money);
   tolua_constant(tolua_S,"e_marry_operate_type_one_refuse_divorce",hld::e_marry_operate_type_one_refuse_divorce);
   tolua_constant(tolua_S,"e_marry_operate_type_one_agree_divorce",hld::e_marry_operate_type_one_agree_divorce);
   tolua_constant(tolua_S,"e_marry_operate_type_divorce_success",hld::e_marry_operate_type_divorce_success);
   tolua_constant(tolua_S,"e_marry_operate_type_check_divorce_success",hld::e_marry_operate_type_check_divorce_success);
   tolua_constant(tolua_S,"e_marry_operate_type_offline_over_seven",hld::e_marry_operate_type_offline_over_seven);
   tolua_constant(tolua_S,"e_marry_operate_type_check_force_divorce_success",hld::e_marry_operate_type_check_force_divorce_success);
   tolua_constant(tolua_S,"e_marry_operate_type_had_getted_reward",hld::e_marry_operate_type_had_getted_reward);
   tolua_constant(tolua_S,"e_marry_operate_type_get_reward_success",hld::e_marry_operate_type_get_reward_success);
   tolua_constant(tolua_S,"e_marry_operate_type_max",hld::e_marry_operate_type_max);
   tolua_constant(tolua_S,"e_fashion_equip_slot_non",hld::e_fashion_equip_slot_non);
   tolua_constant(tolua_S,"e_fashion_equip_slot_weapon_mesh",hld::e_fashion_equip_slot_weapon_mesh);
   tolua_constant(tolua_S,"e_fashion_equip_slot_body_mesh",hld::e_fashion_equip_slot_body_mesh);
   tolua_constant(tolua_S,"e_fashion_equip_slot_wedding_ring",hld::e_fashion_equip_slot_wedding_ring);
   tolua_constant(tolua_S,"e_fashion_equip_slot_weapon_effect",hld::e_fashion_equip_slot_weapon_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_body_effect",hld::e_fashion_equip_slot_body_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_track_effect",hld::e_fashion_equip_slot_track_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_head_effect",hld::e_fashion_equip_slot_head_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_breastplate_effect",hld::e_fashion_equip_slot_breastplate_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_glove_effect",hld::e_fashion_equip_slot_glove_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_legs_effect",hld::e_fashion_equip_slot_legs_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_feet_effect",hld::e_fashion_equip_slot_feet_effect);
   tolua_constant(tolua_S,"e_fashion_equip_slot_headdress",hld::e_fashion_equip_slot_headdress);
   tolua_constant(tolua_S,"e_fashion_equip_slot_mouth",hld::e_fashion_equip_slot_mouth);
   tolua_constant(tolua_S,"e_fashion_equip_slot_eye",hld::e_fashion_equip_slot_eye);
   tolua_constant(tolua_S,"e_fashion_equip_slot_max",hld::e_fashion_equip_slot_max);
   tolua_constant(tolua_S,"e_fashion_show_type_all",hld::e_fashion_show_type_all);
   tolua_constant(tolua_S,"e_fashion_show_type_weapon",hld::e_fashion_show_type_weapon);
   tolua_constant(tolua_S,"e_fashion_show_type_cloth",hld::e_fashion_show_type_cloth);
   tolua_constant(tolua_S,"e_fashion_show_type_weapon_effect",hld::e_fashion_show_type_weapon_effect);
   tolua_constant(tolua_S,"e_fashion_show_type_cloth_effect",hld::e_fashion_show_type_cloth_effect);
   tolua_constant(tolua_S,"e_fashion_show_type_foot_effect",hld::e_fashion_show_type_foot_effect);
   tolua_constant(tolua_S,"e_fashion_show_type_head_frame",hld::e_fashion_show_type_head_frame);
   tolua_constant(tolua_S,"e_fashion_show_type_headdress",hld::e_fashion_show_type_headdress);
   tolua_constant(tolua_S,"e_fashion_show_type_mouth",hld::e_fashion_show_type_mouth);
   tolua_constant(tolua_S,"e_fashion_show_type_eye",hld::e_fashion_show_type_eye);
   tolua_constant(tolua_S,"e_fashion_show_type_max",hld::e_fashion_show_type_max);
   tolua_constant(tolua_S,"e_interaction_reply_none",hld::e_interaction_reply_none);
   tolua_constant(tolua_S,"e_interaction_reply_reject",hld::e_interaction_reply_reject);
   tolua_constant(tolua_S,"e_interaction_reply_agree",hld::e_interaction_reply_agree);
   tolua_constant(tolua_S,"e_interaction_reply_timeout",hld::e_interaction_reply_timeout);
   tolua_constant(tolua_S,"e_interaction_reply_be_invited",hld::e_interaction_reply_be_invited);
   tolua_constant(tolua_S,"e_interaction_reply_is_interacting",hld::e_interaction_reply_is_interacting);
   tolua_constant(tolua_S,"e_interaction_reply_max",hld::e_interaction_reply_max);
   tolua_constant(tolua_S,"e_interaction_type_none",hld::e_interaction_type_none);
   tolua_constant(tolua_S,"e_interaction_type_interrupted",hld::e_interaction_type_interrupted);
   tolua_constant(tolua_S,"e_interaction_type_doubleride_front",hld::e_interaction_type_doubleride_front);
   tolua_constant(tolua_S,"e_interaction_type_doubleride_back",hld::e_interaction_type_doubleride_back);
   tolua_constant(tolua_S,"e_interaction_type_bao_da_tui",hld::e_interaction_type_bao_da_tui);
   tolua_constant(tolua_S,"e_interaction_type_bei_bao_da_tui",hld::e_interaction_type_bei_bao_da_tui);
   tolua_constant(tolua_S,"e_interaction_type_gong_zhu_bao",hld::e_interaction_type_gong_zhu_bao);
   tolua_constant(tolua_S,"e_interaction_type_bei_gong_zhu_bao",hld::e_interaction_type_bei_gong_zhu_bao);
   tolua_constant(tolua_S,"e_interaction_type_qiu_hun",hld::e_interaction_type_qiu_hun);
   tolua_constant(tolua_S,"e_interaction_type_bei_qiu_hun",hld::e_interaction_type_bei_qiu_hun);
   tolua_constant(tolua_S,"e_interaction_type_tiao_wu",hld::e_interaction_type_tiao_wu);
   tolua_constant(tolua_S,"e_interaction_type_bei_tiao_wu",hld::e_interaction_type_bei_tiao_wu);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_back_target_man",hld::e_interaction_type_marry_raid_back_target_man);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_back_target_woman",hld::e_interaction_type_marry_raid_back_target_woman);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_front_target_man",hld::e_interaction_type_marry_raid_front_target_man);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid_front_target_woman",hld::e_interaction_type_marry_raid_front_target_woman);
   tolua_constant(tolua_S,"e_interaction_type_marry_raid",hld::e_interaction_type_marry_raid);
   tolua_constant(tolua_S,"e_interaction_type_yao_legion_dance",hld::e_interaction_type_yao_legion_dance);
   tolua_constant(tolua_S,"e_interaction_type_bei_legion_dance",hld::e_interaction_type_bei_legion_dance);
   tolua_constant(tolua_S,"e_interaction_type_marry_dance_accord",hld::e_interaction_type_marry_dance_accord);
   tolua_constant(tolua_S,"e_interaction_type_marry_dance_passive",hld::e_interaction_type_marry_dance_passive);
   tolua_constant(tolua_S,"e_interaction_type_max",hld::e_interaction_type_max);
   tolua_constant(tolua_S,"e_belief_upgrade_type_none",hld::e_belief_upgrade_type_none);
   tolua_constant(tolua_S,"e_belief_upgrade_type_upgrade_star",hld::e_belief_upgrade_type_upgrade_star);
   tolua_constant(tolua_S,"e_belief_upgrade_type_upgrade_rank",hld::e_belief_upgrade_type_upgrade_rank);
   tolua_constant(tolua_S,"e_belief_upgrade_type_max",hld::e_belief_upgrade_type_max);
   tolua_constant(tolua_S,"e_belief_operate_type_upgrade_star",hld::e_belief_operate_type_upgrade_star);
   tolua_constant(tolua_S,"e_belief_operate_type_upgrade_rank",hld::e_belief_operate_type_upgrade_rank);
   tolua_constant(tolua_S,"e_belief_operate_type_max",hld::e_belief_operate_type_max);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_self",hld::e_exhibition_show_type_player_self);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_other",hld::e_exhibition_show_type_player_other);
   tolua_constant(tolua_S,"e_exhibition_show_type_npc",hld::e_exhibition_show_type_npc);
   tolua_constant(tolua_S,"e_exhibition_show_type_other_slot",hld::e_exhibition_show_type_other_slot);
   tolua_constant(tolua_S,"e_exhibition_show_type_spirit",hld::e_exhibition_show_type_spirit);
   tolua_constant(tolua_S,"e_exhibition_show_type_protect_spirit",hld::e_exhibition_show_type_protect_spirit);
   tolua_constant(tolua_S,"e_exhibition_show_type_self_by_info",hld::e_exhibition_show_type_self_by_info);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_self_simple",hld::e_exhibition_show_type_player_self_simple);
   tolua_constant(tolua_S,"e_exhibition_show_type_player_other_on_mount",hld::e_exhibition_show_type_player_other_on_mount);
   tolua_constant(tolua_S,"e_exhibition_show_type_try_on_fashion",hld::e_exhibition_show_type_try_on_fashion);
   tolua_constant(tolua_S,"e_exhibition_show_type_spirit_composit",hld::e_exhibition_show_type_spirit_composit);
   tolua_constant(tolua_S,"e_exhibition_show_type_max",hld::e_exhibition_show_type_max);
   tolua_constant(tolua_S,"e_pos_type_pos_x",hld::e_pos_type_pos_x);
   tolua_constant(tolua_S,"e_pos_type_pos_y",hld::e_pos_type_pos_y);
   tolua_constant(tolua_S,"e_pos_type_pos_z",hld::e_pos_type_pos_z);
   tolua_constant(tolua_S,"e_pos_type_max",hld::e_pos_type_max);
   tolua_constant(tolua_S,"e_rotation_type_pitch",hld::e_rotation_type_pitch);
   tolua_constant(tolua_S,"e_rotation_type_yaw",hld::e_rotation_type_yaw);
   tolua_constant(tolua_S,"e_rotation_type_roll",hld::e_rotation_type_roll);
   tolua_constant(tolua_S,"e_rotation_type_max",hld::e_rotation_type_max);
   tolua_constant(tolua_S,"e_week_day_monday",hld::e_week_day_monday);
   tolua_constant(tolua_S,"e_week_day_tuesday",hld::e_week_day_tuesday);
   tolua_constant(tolua_S,"e_week_day_wednesday",hld::e_week_day_wednesday);
   tolua_constant(tolua_S,"e_week_day_thursday",hld::e_week_day_thursday);
   tolua_constant(tolua_S,"e_week_day_friday",hld::e_week_day_friday);
   tolua_constant(tolua_S,"e_week_day_saturday",hld::e_week_day_saturday);
   tolua_constant(tolua_S,"e_week_day_sunday",hld::e_week_day_sunday);
   tolua_constant(tolua_S,"e_week_day_max",hld::e_week_day_max);
   tolua_constant(tolua_S,"e_month_january",hld::e_month_january);
   tolua_constant(tolua_S,"e_month_february",hld::e_month_february);
   tolua_constant(tolua_S,"e_month_march",hld::e_month_march);
   tolua_constant(tolua_S,"e_month_april",hld::e_month_april);
   tolua_constant(tolua_S,"e_month_may",hld::e_month_may);
   tolua_constant(tolua_S,"e_month_june",hld::e_month_june);
   tolua_constant(tolua_S,"e_month_july",hld::e_month_july);
   tolua_constant(tolua_S,"e_month_august",hld::e_month_august);
   tolua_constant(tolua_S,"e_month_september",hld::e_month_september);
   tolua_constant(tolua_S,"e_month_october",hld::e_month_october);
   tolua_constant(tolua_S,"e_month_november",hld::e_month_november);
   tolua_constant(tolua_S,"e_month_december",hld::e_month_december);
   tolua_constant(tolua_S,"e_month_max",hld::e_month_max);
   tolua_constant(tolua_S,"e_item_get_info_item_template_id",hld::e_item_get_info_item_template_id);
   tolua_constant(tolua_S,"e_item_get_info_item_num",hld::e_item_get_info_item_num);
   tolua_constant(tolua_S,"e_item_get_info_locked_state",hld::e_item_get_info_locked_state);
   tolua_constant(tolua_S,"e_item_get_info_max",hld::e_item_get_info_max);
   tolua_constant(tolua_S,"e_addition_buff_upgrade",hld::e_addition_buff_upgrade);
   tolua_constant(tolua_S,"e_addition_buff_quality",hld::e_addition_buff_quality);
   tolua_constant(tolua_S,"e_addition_buff_achievement",hld::e_addition_buff_achievement);
   tolua_constant(tolua_S,"e_addition_buff_rank",hld::e_addition_buff_rank);
   tolua_constant(tolua_S,"e_addition_buff_feather",hld::e_addition_buff_feather);
   tolua_constant(tolua_S,"e_addition_buff_addon",hld::e_addition_buff_addon);
   tolua_constant(tolua_S,"e_addition_buff_succinct",hld::e_addition_buff_succinct);
   tolua_constant(tolua_S,"e_addition_buff_fahion_sum_level",hld::e_addition_buff_fahion_sum_level);
   tolua_constant(tolua_S,"e_addition_buff_sky_equip",hld::e_addition_buff_sky_equip);
   tolua_constant(tolua_S,"e_addition_buff_enchant",hld::e_addition_buff_enchant);
   tolua_constant(tolua_S,"e_addition_buff_goddess",hld::e_addition_buff_goddess);
   tolua_constant(tolua_S,"e_addition_buff_couple_heart_value",hld::e_addition_buff_couple_heart_value);
   tolua_constant(tolua_S,"e_addition_buff_skygod_equip",hld::e_addition_buff_skygod_equip);
   tolua_constant(tolua_S,"e_addition_buff_supreme_equip",hld::e_addition_buff_supreme_equip);
   tolua_constant(tolua_S,"e_addition_buff_max",hld::e_addition_buff_max);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_non",hld::e_item_upgrade_protect_type_non);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_low_level_undown",hld::e_item_upgrade_protect_type_low_level_undown);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_high_level_undown",hld::e_item_upgrade_protect_type_high_level_undown);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_add_success_percentage",hld::e_item_upgrade_protect_type_add_success_percentage);
   tolua_constant(tolua_S,"e_item_upgrade_protect_type_max",hld::e_item_upgrade_protect_type_max);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_none",hld::e_item_succinct_buff_level_none);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_F",hld::e_item_succinct_buff_level_F);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_E",hld::e_item_succinct_buff_level_E);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_D",hld::e_item_succinct_buff_level_D);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_C",hld::e_item_succinct_buff_level_C);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_B",hld::e_item_succinct_buff_level_B);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_A",hld::e_item_succinct_buff_level_A);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_S",hld::e_item_succinct_buff_level_S);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_SS",hld::e_item_succinct_buff_level_SS);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_SSS",hld::e_item_succinct_buff_level_SSS);
   tolua_constant(tolua_S,"e_item_succinct_buff_level_max",hld::e_item_succinct_buff_level_max);
   tolua_constant(tolua_S,"e_this_launch_already_do_play_cg",hld::e_this_launch_already_do_play_cg);
   tolua_constant(tolua_S,"e_this_launch_already_do_show_notice",hld::e_this_launch_already_do_show_notice);
   tolua_constant(tolua_S,"e_this_launch_already_do_sdk_game_start",hld::e_this_launch_already_do_sdk_game_start);
   tolua_constant(tolua_S,"e_this_launch_already_do_max",hld::e_this_launch_already_do_max);
   tolua_constant(tolua_S,"e_tinder_info_current_level_template_id",hld::e_tinder_info_current_level_template_id);
   tolua_constant(tolua_S,"e_tinder_info_current_unlock_template_id",hld::e_tinder_info_current_unlock_template_id);
   tolua_constant(tolua_S,"e_tinder_info_already_play_unlock_animation_id",hld::e_tinder_info_already_play_unlock_animation_id);
   tolua_constant(tolua_S,"e_tinder_info_total_star_level",hld::e_tinder_info_total_star_level);
   tolua_constant(tolua_S,"e_tinder_info_add_star_level",hld::e_tinder_info_add_star_level);
   tolua_constant(tolua_S,"e_tinder_info_max",hld::e_tinder_info_max);
   tolua_constant(tolua_S,"e_tinder_operation_type_levelup_tinder",hld::e_tinder_operation_type_levelup_tinder);
   tolua_constant(tolua_S,"e_tinder_operation_type_unlock_tinder",hld::e_tinder_operation_type_unlock_tinder);
   tolua_constant(tolua_S,"e_tinder_operation_type_update_unlock_ani",hld::e_tinder_operation_type_update_unlock_ani);
   tolua_constant(tolua_S,"e_tinder_operation_type_unlock_active_skill",hld::e_tinder_operation_type_unlock_active_skill);
   tolua_constant(tolua_S,"e_tinder_operation_type_max",hld::e_tinder_operation_type_max);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_create",hld::e_get_aoi_info_all_type_create);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_simple_to_normal",hld::e_get_aoi_info_all_type_simple_to_normal);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_normal_to_simple",hld::e_get_aoi_info_all_type_normal_to_simple);
   tolua_constant(tolua_S,"e_get_aoi_info_all_type_max",hld::e_get_aoi_info_all_type_max);
   tolua_constant(tolua_S,"e_service_rank_type_level",hld::e_service_rank_type_level);
   tolua_constant(tolua_S,"e_service_rank_type_mount",hld::e_service_rank_type_mount);
   tolua_constant(tolua_S,"e_service_rank_type_spirit",hld::e_service_rank_type_spirit);
   tolua_constant(tolua_S,"e_service_rank_type_element",hld::e_service_rank_type_element);
   tolua_constant(tolua_S,"e_service_rank_type_wing",hld::e_service_rank_type_wing);
   tolua_constant(tolua_S,"e_service_rank_type_gs",hld::e_service_rank_type_gs);
   tolua_constant(tolua_S,"e_service_rank_type_max",hld::e_service_rank_type_max);
   tolua_constant(tolua_S,"e_mission_type_main",hld::e_mission_type_main);
   tolua_constant(tolua_S,"e_mission_type_daily",hld::e_mission_type_daily);
   tolua_constant(tolua_S,"e_mission_type_against",hld::e_mission_type_against);
   tolua_constant(tolua_S,"e_mission_type_side",hld::e_mission_type_side);
   tolua_constant(tolua_S,"e_mission_type_grade_up",hld::e_mission_type_grade_up);
   tolua_constant(tolua_S,"e_mission_type_marry",hld::e_mission_type_marry);
   tolua_constant(tolua_S,"e_mission_type_max",hld::e_mission_type_max);
   tolua_constant(tolua_S,"e_mission_slot_main",hld::e_mission_slot_main);
   tolua_constant(tolua_S,"e_mission_slot_daily",hld::e_mission_slot_daily);
   tolua_constant(tolua_S,"e_mission_slot_against",hld::e_mission_slot_against);
   tolua_constant(tolua_S,"e_mission_slot_grade_up",hld::e_mission_slot_grade_up);
   tolua_constant(tolua_S,"e_mission_slot_side_0",hld::e_mission_slot_side_0);
   tolua_constant(tolua_S,"e_mission_slot_side_1",hld::e_mission_slot_side_1);
   tolua_constant(tolua_S,"e_mission_slot_side_2",hld::e_mission_slot_side_2);
   tolua_constant(tolua_S,"e_mission_slot_side_3",hld::e_mission_slot_side_3);
   tolua_constant(tolua_S,"e_mission_slot_side_4",hld::e_mission_slot_side_4);
   tolua_constant(tolua_S,"e_mission_slot_side_5",hld::e_mission_slot_side_5);
   tolua_constant(tolua_S,"e_mission_slot_side_6",hld::e_mission_slot_side_6);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_0",hld::e_mission_slot_extra_side_0);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_1",hld::e_mission_slot_extra_side_1);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_2",hld::e_mission_slot_extra_side_2);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_3",hld::e_mission_slot_extra_side_3);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_4",hld::e_mission_slot_extra_side_4);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_5",hld::e_mission_slot_extra_side_5);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_6",hld::e_mission_slot_extra_side_6);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_7",hld::e_mission_slot_extra_side_7);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_8",hld::e_mission_slot_extra_side_8);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_9",hld::e_mission_slot_extra_side_9);
   tolua_constant(tolua_S,"e_mission_slot_extra_side_10",hld::e_mission_slot_extra_side_10);
   tolua_constant(tolua_S,"e_mission_slot_max",hld::e_mission_slot_max);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_no_qualification",hld::e_service_rank_reward_state_type_no_qualification);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_have_qualification",hld::e_service_rank_reward_state_type_have_qualification);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_already_receive",hld::e_service_rank_reward_state_type_already_receive);
   tolua_constant(tolua_S,"e_service_rank_reward_state_type_max",hld::e_service_rank_reward_state_type_max);
   tolua_constant(tolua_S,"e_service_rank_reward_type_first",hld::e_service_rank_reward_type_first);
   tolua_constant(tolua_S,"e_service_rank_reward_type_second",hld::e_service_rank_reward_type_second);
   tolua_constant(tolua_S,"e_service_rank_reward_type_third",hld::e_service_rank_reward_type_third);
   tolua_constant(tolua_S,"e_service_rank_reward_type_fourth",hld::e_service_rank_reward_type_fourth);
   tolua_constant(tolua_S,"e_service_rank_reward_type_fifth",hld::e_service_rank_reward_type_fifth);
   tolua_constant(tolua_S,"e_service_rank_reward_type_max",hld::e_service_rank_reward_type_max);
   tolua_constant(tolua_S,"e_beast_spirit_type_mount",hld::e_beast_spirit_type_mount);
   tolua_constant(tolua_S,"e_beast_spirit_type_wing",hld::e_beast_spirit_type_wing);
   tolua_constant(tolua_S,"e_beast_spirit_type_spirit",hld::e_beast_spirit_type_spirit);
   tolua_constant(tolua_S,"e_beast_spirit_type_max",hld::e_beast_spirit_type_max);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_killed_field_boss",hld::e_gain_treasure_info_type_killed_field_boss);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_killed_simple_boss",hld::e_gain_treasure_info_type_killed_simple_boss);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_buy_simple_boss",hld::e_gain_treasure_info_type_buy_simple_boss);
   tolua_constant(tolua_S,"e_gain_treasure_info_type_max",hld::e_gain_treasure_info_type_max);
   tolua_constant(tolua_S,"e_jewel_type_non",hld::e_jewel_type_non);
   tolua_constant(tolua_S,"e_jewel_type_attack",hld::e_jewel_type_attack);
   tolua_constant(tolua_S,"e_jewel_type_defense",hld::e_jewel_type_defense);
   tolua_constant(tolua_S,"e_jewel_type_max",hld::e_jewel_type_max);
   tolua_constant(tolua_S,"e_drop_record_model_type_no",hld::e_drop_record_model_type_no);
   tolua_constant(tolua_S,"e_drop_record_model_type_record",hld::e_drop_record_model_type_record);
   tolua_constant(tolua_S,"e_drop_record_model_type_set_top",hld::e_drop_record_model_type_set_top);
   tolua_constant(tolua_S,"e_drop_record_model_type_max",hld::e_drop_record_model_type_max);
   tolua_constant(tolua_S,"e_treasure_record_time",hld::e_treasure_record_time);
   tolua_constant(tolua_S,"e_treasure_record_treasure_type",hld::e_treasure_record_treasure_type);
   tolua_constant(tolua_S,"e_treasure_record_prize_type",hld::e_treasure_record_prize_type);
   tolua_constant(tolua_S,"e_treasure_record_prize_0",hld::e_treasure_record_prize_0);
   tolua_constant(tolua_S,"e_treasure_record_prize_1",hld::e_treasure_record_prize_1);
   tolua_constant(tolua_S,"e_treasure_record_prize_2",hld::e_treasure_record_prize_2);
   tolua_constant(tolua_S,"e_treasure_record_prize_3",hld::e_treasure_record_prize_3);
   tolua_constant(tolua_S,"e_treasure_record_prize_4",hld::e_treasure_record_prize_4);
   tolua_constant(tolua_S,"e_treasure_record_prize_5",hld::e_treasure_record_prize_5);
   tolua_constant(tolua_S,"e_treasure_record_prize_6",hld::e_treasure_record_prize_6);
   tolua_constant(tolua_S,"e_treasure_record_prize_7",hld::e_treasure_record_prize_7);
   tolua_constant(tolua_S,"e_treasure_record_prize_8",hld::e_treasure_record_prize_8);
   tolua_constant(tolua_S,"e_treasure_record_prize_9",hld::e_treasure_record_prize_9);
   tolua_constant(tolua_S,"e_treasure_record_max",hld::e_treasure_record_max);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_charge",hld::e_time_limit_activity_type_total_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_cost",hld::e_time_limit_activity_type_total_cost);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge",hld::e_time_limit_activity_type_daily_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure",hld::e_time_limit_activity_type_treasure);
   tolua_constant(tolua_S,"e_time_limit_activity_type_shop",hld::e_time_limit_activity_type_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare",hld::e_time_limit_activity_type_time_welfare);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare",hld::e_time_limit_activity_type_behavior_welfare);
   tolua_constant(tolua_S,"e_time_limit_activity_type_word_desc",hld::e_time_limit_activity_type_word_desc);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare2",hld::e_time_limit_activity_type_time_welfare2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare3",hld::e_time_limit_activity_type_time_welfare3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare4",hld::e_time_limit_activity_type_time_welfare4);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_sprite",hld::e_time_limit_activity_type_treasure_sprite);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_exchange",hld::e_time_limit_activity_type_item_exchange);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_welfare5",hld::e_time_limit_activity_type_time_welfare5);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge2",hld::e_time_limit_activity_type_daily_charge2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_charge2",hld::e_time_limit_activity_type_total_charge2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward",hld::e_time_limit_activity_type_behavior_welfare_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop",hld::e_time_limit_activity_type_discount_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_small_charge",hld::e_time_limit_activity_type_small_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_landing",hld::e_time_limit_activity_type_total_landing);
   tolua_constant(tolua_S,"e_time_limit_activity_type_total_cost_jewel",hld::e_time_limit_activity_type_total_cost_jewel);
   tolua_constant(tolua_S,"e_time_limit_activity_type_landing_reward",hld::e_time_limit_activity_type_landing_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_added_drop",hld::e_time_limit_activity_type_added_drop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_add_upgrade_level",hld::e_time_limit_activity_type_add_upgrade_level);
   tolua_constant(tolua_S,"e_time_limit_activity_type_activity_notice_1",hld::e_time_limit_activity_type_activity_notice_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_activity_notice_2",hld::e_time_limit_activity_type_activity_notice_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_1",hld::e_time_limit_activity_type_national_treasure_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_2",hld::e_time_limit_activity_type_national_treasure_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_gs_up",hld::e_time_limit_activity_type_gs_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_mount_up",hld::e_time_limit_activity_type_mount_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_wing_up",hld::e_time_limit_activity_type_wing_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_equip_up",hld::e_time_limit_activity_type_equip_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_spirit_up",hld::e_time_limit_activity_type_spirit_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_gs_up",hld::e_time_limit_activity_type_cross_gs_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_mount_up",hld::e_time_limit_activity_type_cross_mount_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_wing_up",hld::e_time_limit_activity_type_cross_wing_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_equip_up",hld::e_time_limit_activity_type_cross_equip_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_spirit_up",hld::e_time_limit_activity_type_cross_spirit_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_element",hld::e_time_limit_activity_type_treasure_element);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cloud_shop",hld::e_time_limit_activity_type_cloud_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_cloud_shop",hld::e_time_limit_activity_type_cross_cloud_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_gift_charge",hld::e_time_limit_activity_type_daily_gift_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop",hld::e_time_limit_activity_type_item_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_dragontrip",hld::e_time_limit_activity_type_dragontrip);
   tolua_constant(tolua_S,"e_time_limit_activity_type_special_exchange",hld::e_time_limit_activity_type_special_exchange);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_landing",hld::e_time_limit_activity_type_new_total_landing);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_behavior_welfare",hld::e_time_limit_activity_type_new_behavior_welfare);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_behavior_welfare_reward",hld::e_time_limit_activity_type_new_behavior_welfare_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_sky_treasure",hld::e_time_limit_activity_type_sky_treasure);
   tolua_constant(tolua_S,"e_time_limit_activity_type_starark",hld::e_time_limit_activity_type_starark);
   tolua_constant(tolua_S,"e_time_limit_activity_type_support_for_promotion",hld::e_time_limit_activity_type_support_for_promotion);
   tolua_constant(tolua_S,"e_time_limit_activity_type_weekend_top_up",hld::e_time_limit_activity_type_weekend_top_up);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare1",hld::e_time_limit_activity_type_behavior_welfare1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare2",hld::e_time_limit_activity_type_behavior_welfare2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare3",hld::e_time_limit_activity_type_behavior_welfare3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_royal1",hld::e_time_limit_activity_type_treasure_royal1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_royal2",hld::e_time_limit_activity_type_treasure_royal2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_treasure_royal3",hld::e_time_limit_activity_type_treasure_royal3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge3",hld::e_time_limit_activity_type_daily_charge3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge4",hld::e_time_limit_activity_type_daily_charge4);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge5",hld::e_time_limit_activity_type_daily_charge5);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_charge6",hld::e_time_limit_activity_type_daily_charge6);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop1",hld::e_time_limit_activity_type_discount_shop1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop2",hld::e_time_limit_activity_type_discount_shop2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_discount_shop3",hld::e_time_limit_activity_type_discount_shop3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward1",hld::e_time_limit_activity_type_behavior_welfare_reward1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward2",hld::e_time_limit_activity_type_behavior_welfare_reward2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_behavior_welfare_reward3",hld::e_time_limit_activity_type_behavior_welfare_reward3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop1",hld::e_time_limit_activity_type_item_shop1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop2",hld::e_time_limit_activity_type_item_shop2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_item_shop3",hld::e_time_limit_activity_type_item_shop3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_lucky_card",hld::e_time_limit_activity_type_lucky_card);
   tolua_constant(tolua_S,"e_time_limit_activity_type_cross_total_charge",hld::e_time_limit_activity_type_cross_total_charge);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_charge_1",hld::e_time_limit_activity_type_new_total_charge_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_charge_2",hld::e_time_limit_activity_type_new_total_charge_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_charge_3",hld::e_time_limit_activity_type_new_total_charge_3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_cost_jewel_1",hld::e_time_limit_activity_type_new_total_cost_jewel_1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_cost_jewel_2",hld::e_time_limit_activity_type_new_total_cost_jewel_2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_total_cost_jewel_3",hld::e_time_limit_activity_type_new_total_cost_jewel_3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_server_treasure",hld::e_time_limit_activity_type_new_server_treasure);
   tolua_constant(tolua_S,"e_time_limit_activity_type_new_server_mail",hld::e_time_limit_activity_type_new_server_mail);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_3",hld::e_time_limit_activity_type_national_treasure_3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_national_treasure_4",hld::e_time_limit_activity_type_national_treasure_4);
   tolua_constant(tolua_S,"e_time_limit_activity_type_daily_free_reward",hld::e_time_limit_activity_type_daily_free_reward);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_feed_back1",hld::e_time_limit_activity_type_time_feed_back1);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_feed_back2",hld::e_time_limit_activity_type_time_feed_back2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_time_feed_back3",hld::e_time_limit_activity_type_time_feed_back3);
   tolua_constant(tolua_S,"e_time_limit_activity_type_limit_time_treasure2",hld::e_time_limit_activity_type_limit_time_treasure2);
   tolua_constant(tolua_S,"e_time_limit_activity_type_limit_time_notice",hld::e_time_limit_activity_type_limit_time_notice);
   tolua_constant(tolua_S,"e_time_limit_activity_type_diamond_shop",hld::e_time_limit_activity_type_diamond_shop);
   tolua_constant(tolua_S,"e_time_limit_activity_type_star_trip",hld::e_time_limit_activity_type_star_trip);
   tolua_constant(tolua_S,"e_time_limit_activity_type_max",hld::e_time_limit_activity_type_max);
   tolua_constant(tolua_S,"e_mail_type_normal_mail",hld::e_mail_type_normal_mail);
   tolua_constant(tolua_S,"e_mail_type_server_merge_to_normal_player",hld::e_mail_type_server_merge_to_normal_player);
   tolua_constant(tolua_S,"e_mail_type_server_merge_to_legion_del_player",hld::e_mail_type_server_merge_to_legion_del_player);
   tolua_constant(tolua_S,"e_mail_type_server_merge_to_be_del_player",hld::e_mail_type_server_merge_to_be_del_player);
   tolua_constant(tolua_S,"e_mail_type_max",hld::e_mail_type_max);
   tolua_constant(tolua_S,"e_time_limit_activity_info_type",hld::e_time_limit_activity_info_type);
   tolua_constant(tolua_S,"e_time_limit_activity_info_start_time",hld::e_time_limit_activity_info_start_time);
   tolua_constant(tolua_S,"e_time_limit_activity_info_end_time",hld::e_time_limit_activity_info_end_time);
   tolua_constant(tolua_S,"e_time_limit_activity_info_guid",hld::e_time_limit_activity_info_guid);
   tolua_constant(tolua_S,"e_time_limit_activity_info_first_branch_guid",hld::e_time_limit_activity_info_first_branch_guid);
   tolua_constant(tolua_S,"e_time_limit_activity_info_max",hld::e_time_limit_activity_info_max);
   tolua_constant(tolua_S,"e_unit_safe_map_info_pos_x",hld::e_unit_safe_map_info_pos_x);
   tolua_constant(tolua_S,"e_unit_safe_map_info_pos_y",hld::e_unit_safe_map_info_pos_y);
   tolua_constant(tolua_S,"e_unit_safe_map_info_map_tempate_id",hld::e_unit_safe_map_info_map_tempate_id);
   tolua_constant(tolua_S,"e_unit_safe_map_info_map_have_not_safe_region",hld::e_unit_safe_map_info_map_have_not_safe_region);
   tolua_constant(tolua_S,"e_unit_safe_map_info_safe_map_template_id",hld::e_unit_safe_map_info_safe_map_template_id);
   tolua_constant(tolua_S,"e_unit_safe_map_info_safe_missiondone_id",hld::e_unit_safe_map_info_safe_missiondone_id);
   tolua_constant(tolua_S,"e_unit_safe_map_info_safe_map_type",hld::e_unit_safe_map_info_safe_map_type);
   tolua_constant(tolua_S,"e_unit_safe_map_info_max",hld::e_unit_safe_map_info_max);
   tolua_constant(tolua_S,"e_island_record_killed_island_boss",hld::e_island_record_killed_island_boss);
   tolua_constant(tolua_S,"e_island_record_island_collect_count",hld::e_island_record_island_collect_count);
   tolua_constant(tolua_S,"e_island_record_cross_pk_collect_count",hld::e_island_record_cross_pk_collect_count);
   tolua_constant(tolua_S,"e_island_record_oracle_trial_count",hld::e_island_record_oracle_trial_count);
   tolua_constant(tolua_S,"e_island_record_max",hld::e_island_record_max);
   tolua_constant(tolua_S,"e_login_type_new_account_and_password",hld::e_login_type_new_account_and_password);
   tolua_constant(tolua_S,"e_login_type_new_robot",hld::e_login_type_new_robot);
   tolua_constant(tolua_S,"e_login_type_new_token",hld::e_login_type_new_token);
   tolua_constant(tolua_S,"e_login_type_new_zw",hld::e_login_type_new_zw);
   tolua_constant(tolua_S,"e_login_type_new_facebook",hld::e_login_type_new_facebook);
   tolua_constant(tolua_S,"e_login_type_new_google",hld::e_login_type_new_google);
   tolua_constant(tolua_S,"e_login_type_new_gate_server",hld::e_login_type_new_gate_server);
   tolua_constant(tolua_S,"e_login_type_new_tw",hld::e_login_type_new_tw);
   tolua_constant(tolua_S,"e_login_type_new_korea",hld::e_login_type_new_korea);
   tolua_constant(tolua_S,"e_login_type_new_en",hld::e_login_type_new_en);
   tolua_constant(tolua_S,"e_login_type_new_umisdk",hld::e_login_type_new_umisdk);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_vn",hld::e_login_type_new_umisdk_vn);
   tolua_constant(tolua_S,"e_login_type_new_appstore",hld::e_login_type_new_appstore);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_tw",hld::e_login_type_new_umisdk_tw);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_jp",hld::e_login_type_new_umisdk_jp);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_rus",hld::e_login_type_new_umisdk_rus);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_sg",hld::e_login_type_new_umisdk_sg);
   tolua_constant(tolua_S,"e_login_type_new_umisdk_new_tw",hld::e_login_type_new_umisdk_new_tw);
   tolua_constant(tolua_S,"e_login_type_new_max",hld::e_login_type_new_max);
   tolua_constant(tolua_S,"e_payment_method_test",hld::e_payment_method_test);
   tolua_constant(tolua_S,"e_payment_method_zw",hld::e_payment_method_zw);
   tolua_constant(tolua_S,"e_payment_method_google",hld::e_payment_method_google);
   tolua_constant(tolua_S,"e_payment_method_one_store",hld::e_payment_method_one_store);
   tolua_constant(tolua_S,"e_payment_method_app_store",hld::e_payment_method_app_store);
   tolua_constant(tolua_S,"e_payment_method_tw_android",hld::e_payment_method_tw_android);
   tolua_constant(tolua_S,"e_payment_method_tw_ios",hld::e_payment_method_tw_ios);
   tolua_constant(tolua_S,"e_payment_method_tw_third",hld::e_payment_method_tw_third);
   tolua_constant(tolua_S,"e_payment_method_samsung",hld::e_payment_method_samsung);
   tolua_constant(tolua_S,"e_payment_method_en_ios",hld::e_payment_method_en_ios);
   tolua_constant(tolua_S,"e_payment_method_en_android",hld::e_payment_method_en_android);
   tolua_constant(tolua_S,"e_payment_method_kr_gameloop",hld::e_payment_method_kr_gameloop);
   tolua_constant(tolua_S,"e_payment_method_tl_umisdk_google",hld::e_payment_method_tl_umisdk_google);
   tolua_constant(tolua_S,"e_payment_method_tl_umisdk_ios",hld::e_payment_method_tl_umisdk_ios);
   tolua_constant(tolua_S,"e_payment_method_vn_umisdk_google",hld::e_payment_method_vn_umisdk_google);
   tolua_constant(tolua_S,"e_payment_method_vn_umisdk_ios",hld::e_payment_method_vn_umisdk_ios);
   tolua_constant(tolua_S,"e_payment_method_tw_android_new",hld::e_payment_method_tw_android_new);
   tolua_constant(tolua_S,"e_payment_method_tw_ios_new",hld::e_payment_method_tw_ios_new);
   tolua_constant(tolua_S,"e_payment_method_tw_third_new",hld::e_payment_method_tw_third_new);
   tolua_constant(tolua_S,"e_payment_method_tw_umisdk_android",hld::e_payment_method_tw_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_tw_umisdk_ios",hld::e_payment_method_tw_umisdk_ios);
   tolua_constant(tolua_S,"e_payment_method_tw_umisdk_third",hld::e_payment_method_tw_umisdk_third);
   tolua_constant(tolua_S,"e_payment_method_jp_umisdk_android",hld::e_payment_method_jp_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_jp_uminsdk_ios",hld::e_payment_method_jp_uminsdk_ios);
   tolua_constant(tolua_S,"e_payment_method_rus_umisdk_android",hld::e_payment_method_rus_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_rus_uminsdk_ios",hld::e_payment_method_rus_uminsdk_ios);
   tolua_constant(tolua_S,"e_payment_method_sg_umisdk_android",hld::e_payment_method_sg_umisdk_android);
   tolua_constant(tolua_S,"e_payment_method_sg_uminsdk_ios",hld::e_payment_method_sg_uminsdk_ios);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_android",hld::e_payment_method_umisdk_tw_android);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_ios",hld::e_payment_method_umisdk_tw_ios);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_third",hld::e_payment_method_umisdk_tw_third);
   tolua_constant(tolua_S,"e_payment_method_umisdk_tw_hw",hld::e_payment_method_umisdk_tw_hw);
   tolua_constant(tolua_S,"e_payment_method_max",hld::e_payment_method_max);
   tolua_constant(tolua_S,"e_treasure_prize_type_one",hld::e_treasure_prize_type_one);
   tolua_constant(tolua_S,"e_treasure_prize_type_ten",hld::e_treasure_prize_type_ten);
   tolua_constant(tolua_S,"e_treasure_prize_type_one_item",hld::e_treasure_prize_type_one_item);
   tolua_constant(tolua_S,"e_treasure_prize_type_ten_item",hld::e_treasure_prize_type_ten_item);
   tolua_constant(tolua_S,"e_treasure_prize_type_max",hld::e_treasure_prize_type_max);
   tolua_constant(tolua_S,"e_treasure_type_item",hld::e_treasure_type_item);
   tolua_constant(tolua_S,"e_treasure_type_element",hld::e_treasure_type_element);
   tolua_constant(tolua_S,"e_treasure_type_spirit",hld::e_treasure_type_spirit);
   tolua_constant(tolua_S,"e_treasure_type_new_server",hld::e_treasure_type_new_server);
   tolua_constant(tolua_S,"e_treasure_type_time_limit",hld::e_treasure_type_time_limit);
   tolua_constant(tolua_S,"e_treasure_type_max",hld::e_treasure_type_max);
   tolua_constant(tolua_S,"e_ranking_show_type_non",hld::e_ranking_show_type_non);
   tolua_constant(tolua_S,"e_ranking_show_type_gs",hld::e_ranking_show_type_gs);
   tolua_constant(tolua_S,"e_ranking_show_type_exp_level",hld::e_ranking_show_type_exp_level);
   tolua_constant(tolua_S,"e_ranking_show_type_equip",hld::e_ranking_show_type_equip);
   tolua_constant(tolua_S,"e_ranking_show_type_mount",hld::e_ranking_show_type_mount);
   tolua_constant(tolua_S,"e_ranking_show_type_wing",hld::e_ranking_show_type_wing);
   tolua_constant(tolua_S,"e_ranking_show_type_sprite",hld::e_ranking_show_type_sprite);
   tolua_constant(tolua_S,"e_ranking_show_type_role",hld::e_ranking_show_type_role);
   tolua_constant(tolua_S,"e_ranking_show_type_challenge",hld::e_ranking_show_type_challenge);
   tolua_constant(tolua_S,"e_ranking_show_type_vip",hld::e_ranking_show_type_vip);
   tolua_constant(tolua_S,"e_ranking_show_type_legion",hld::e_ranking_show_type_legion);
   tolua_constant(tolua_S,"e_ranking_show_type_world_boss",hld::e_ranking_show_type_world_boss);
   tolua_constant(tolua_S,"e_ranking_show_type_elite",hld::e_ranking_show_type_elite);
   tolua_constant(tolua_S,"e_ranking_show_type_cross_service",hld::e_ranking_show_type_cross_service);
   tolua_constant(tolua_S,"e_ranking_show_type_max",hld::e_ranking_show_type_max);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_non",hld::e_time_limit_behavior_type_non);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_boss",hld::e_time_limit_behavior_type_kill_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_monster",hld::e_time_limit_behavior_type_kill_monster);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_treasure_lottery",hld::e_time_limit_behavior_type_treasure_lottery);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_exp_stage",hld::e_time_limit_behavior_type_exp_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_coin_stage",hld::e_time_limit_behavior_type_coin_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_illusion_stage",hld::e_time_limit_behavior_type_illusion_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_belief_stage",hld::e_time_limit_behavior_type_belief_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_sprite_stage",hld::e_time_limit_behavior_type_sprite_stage);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_recharge",hld::e_time_limit_behavior_type_recharge);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_auction_bid",hld::e_time_limit_behavior_type_auction_bid);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_buy_in_diamond_store",hld::e_time_limit_behavior_type_buy_in_diamond_store);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_telves_awaken",hld::e_time_limit_behavior_type_telves_awaken);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_personal_boss",hld::e_time_limit_behavior_type_kill_personal_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_legion_dance",hld::e_time_limit_behavior_type_legion_dance);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_world_boss",hld::e_time_limit_behavior_type_kill_world_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_kill_legion_boss",hld::e_time_limit_behavior_type_kill_legion_boss);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_arena_win",hld::e_time_limit_behavior_type_arena_win);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_ranking_worship",hld::e_time_limit_behavior_type_ranking_worship);
   tolua_constant(tolua_S,"e_time_limit_behavior_type_max",hld::e_time_limit_behavior_type_max);
   tolua_constant(tolua_S,"e_passive_skill_info_passive_skill_id",hld::e_passive_skill_info_passive_skill_id);
   tolua_constant(tolua_S,"e_passive_skill_info_equip_slot",hld::e_passive_skill_info_equip_slot);
   tolua_constant(tolua_S,"e_passive_skill_info_cd",hld::e_passive_skill_info_cd);
   tolua_constant(tolua_S,"e_passive_skill_info_max",hld::e_passive_skill_info_max);
   tolua_constant(tolua_S,"e_legion_skill_info_legion_skill_id",hld::e_legion_skill_info_legion_skill_id);
   tolua_constant(tolua_S,"e_legion_skill_info_curr_level",hld::e_legion_skill_info_curr_level);
   tolua_constant(tolua_S,"e_legion_skill_info_max",hld::e_legion_skill_info_max);
   tolua_constant(tolua_S,"e_passive_skill_info_type_non",hld::e_passive_skill_info_type_non);
   tolua_constant(tolua_S,"e_passive_skill_info_type_tinder",hld::e_passive_skill_info_type_tinder);
   tolua_constant(tolua_S,"e_passive_skill_info_type_sprite",hld::e_passive_skill_info_type_sprite);
   tolua_constant(tolua_S,"e_passive_skill_info_type_mount",hld::e_passive_skill_info_type_mount);
   tolua_constant(tolua_S,"e_passive_skill_info_type_wing",hld::e_passive_skill_info_type_wing);
   tolua_constant(tolua_S,"e_passive_skill_info_type_feather",hld::e_passive_skill_info_type_feather);
   tolua_constant(tolua_S,"e_passive_skill_info_type_phantom",hld::e_passive_skill_info_type_phantom);
   tolua_constant(tolua_S,"e_passive_skill_info_type_sky_weapon",hld::e_passive_skill_info_type_sky_weapon);
   tolua_constant(tolua_S,"e_passive_skill_info_type_mount_star",hld::e_passive_skill_info_type_mount_star);
   tolua_constant(tolua_S,"e_passive_skill_info_type_wing_psychic",hld::e_passive_skill_info_type_wing_psychic);
   tolua_constant(tolua_S,"e_passive_skill_info_type_break_will",hld::e_passive_skill_info_type_break_will);
   tolua_constant(tolua_S,"e_passive_skill_info_type_direct_get",hld::e_passive_skill_info_type_direct_get);
   tolua_constant(tolua_S,"e_passive_skill_info_type_max",hld::e_passive_skill_info_type_max);
   tolua_constant(tolua_S,"e_map_order_type_other_raid",hld::e_map_order_type_other_raid);
   tolua_constant(tolua_S,"e_map_order_type_story_raid",hld::e_map_order_type_story_raid);
   tolua_constant(tolua_S,"e_map_order_type_daily_raid",hld::e_map_order_type_daily_raid);
   tolua_constant(tolua_S,"e_map_order_type_multiplayer_raid",hld::e_map_order_type_multiplayer_raid);
   tolua_constant(tolua_S,"e_map_order_type_max",hld::e_map_order_type_max);
   tolua_constant(tolua_S,"e_pokedex_Info_template_id",hld::e_pokedex_Info_template_id);
   tolua_constant(tolua_S,"e_pokedex_Info_upgrade_num",hld::e_pokedex_Info_upgrade_num);
   tolua_constant(tolua_S,"e_pokedex_Info_state",hld::e_pokedex_Info_state);
   tolua_constant(tolua_S,"e_pokedex_Info_fetter_state_1",hld::e_pokedex_Info_fetter_state_1);
   tolua_constant(tolua_S,"e_pokedex_Info_fetter_state_2",hld::e_pokedex_Info_fetter_state_2);
   tolua_constant(tolua_S,"e_pokedex_Info_fetter_state_3",hld::e_pokedex_Info_fetter_state_3);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_1",hld::e_pokedex_Info_psyche_num_1);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_2",hld::e_pokedex_Info_psyche_num_2);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_3",hld::e_pokedex_Info_psyche_num_3);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_4",hld::e_pokedex_Info_psyche_num_4);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_5",hld::e_pokedex_Info_psyche_num_5);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_6",hld::e_pokedex_Info_psyche_num_6);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_7",hld::e_pokedex_Info_psyche_num_7);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_8",hld::e_pokedex_Info_psyche_num_8);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_9",hld::e_pokedex_Info_psyche_num_9);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_10",hld::e_pokedex_Info_psyche_num_10);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_11",hld::e_pokedex_Info_psyche_num_11);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_12",hld::e_pokedex_Info_psyche_num_12);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_13",hld::e_pokedex_Info_psyche_num_13);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_14",hld::e_pokedex_Info_psyche_num_14);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_15",hld::e_pokedex_Info_psyche_num_15);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_16",hld::e_pokedex_Info_psyche_num_16);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_17",hld::e_pokedex_Info_psyche_num_17);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_18",hld::e_pokedex_Info_psyche_num_18);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_19",hld::e_pokedex_Info_psyche_num_19);
   tolua_constant(tolua_S,"e_pokedex_Info_psyche_num_20",hld::e_pokedex_Info_psyche_num_20);
   tolua_constant(tolua_S,"e_pokedex_Info_max",hld::e_pokedex_Info_max);
   tolua_constant(tolua_S,"e_pokedex_operation_type_submit_psyche",hld::e_pokedex_operation_type_submit_psyche);
   tolua_constant(tolua_S,"e_pokedex_operation_type_change",hld::e_pokedex_operation_type_change);
   tolua_constant(tolua_S,"e_pokedex_operation_type_main_palace_upgrade",hld::e_pokedex_operation_type_main_palace_upgrade);
   tolua_constant(tolua_S,"e_pokedex_operation_type_oracle_activate",hld::e_pokedex_operation_type_oracle_activate);
   tolua_constant(tolua_S,"e_pokedex_operation_type_oracle_upgrade",hld::e_pokedex_operation_type_oracle_upgrade);
   tolua_constant(tolua_S,"e_pokedex_operation_type_mainland_activate",hld::e_pokedex_operation_type_mainland_activate);
   tolua_constant(tolua_S,"e_pokedex_operation_type_mainland_upgrade",hld::e_pokedex_operation_type_mainland_upgrade);
   tolua_constant(tolua_S,"e_pokedex_operation_type_max",hld::e_pokedex_operation_type_max);
   tolua_constant(tolua_S,"e_goddess_operate_type_none",hld::e_goddess_operate_type_none);
   tolua_constant(tolua_S,"e_goddess_operate_type_unlock",hld::e_goddess_operate_type_unlock);
   tolua_constant(tolua_S,"e_goddess_operate_type_equip",hld::e_goddess_operate_type_equip);
   tolua_constant(tolua_S,"e_goddess_operate_type_unequip",hld::e_goddess_operate_type_unequip);
   tolua_constant(tolua_S,"e_goddess_operate_type_fight",hld::e_goddess_operate_type_fight);
   tolua_constant(tolua_S,"e_goddess_operate_type_equip_upgrade",hld::e_goddess_operate_type_equip_upgrade);
   tolua_constant(tolua_S,"e_goddess_operate_type_goddess_slot_unlock",hld::e_goddess_operate_type_goddess_slot_unlock);
   tolua_constant(tolua_S,"e_goddess_operate_type_max",hld::e_goddess_operate_type_max);
   tolua_constant(tolua_S,"e_cross_pk_state_none",hld::e_cross_pk_state_none);
   tolua_constant(tolua_S,"e_cross_pk_state_16",hld::e_cross_pk_state_16);
   tolua_constant(tolua_S,"e_cross_pk_state_8",hld::e_cross_pk_state_8);
   tolua_constant(tolua_S,"e_cross_pk_state_4",hld::e_cross_pk_state_4);
   tolua_constant(tolua_S,"e_cross_pk_state_2",hld::e_cross_pk_state_2);
   tolua_constant(tolua_S,"e_cross_pk_state_max",hld::e_cross_pk_state_max);
   tolua_constant(tolua_S,"e_1v1_pk_state_ready",hld::e_1v1_pk_state_ready);
   tolua_constant(tolua_S,"e_1v1_pk_state_begin",hld::e_1v1_pk_state_begin);
   tolua_constant(tolua_S,"e_1v1_pk_state_end",hld::e_1v1_pk_state_end);
   tolua_constant(tolua_S,"e_1v1_pk_state_max",hld::e_1v1_pk_state_max);
   tolua_constant(tolua_S,"e_auction_state_start_sell",hld::e_auction_state_start_sell);
   tolua_constant(tolua_S,"e_auction_state_cancel",hld::e_auction_state_cancel);
   tolua_constant(tolua_S,"e_auction_state_deal",hld::e_auction_state_deal);
   tolua_constant(tolua_S,"e_auction_state_bid",hld::e_auction_state_bid);
   tolua_constant(tolua_S,"e_auction_state_max",hld::e_auction_state_max);
   tolua_constant(tolua_S,"e_cloud_shop_record_date_value",hld::e_cloud_shop_record_date_value);
   tolua_constant(tolua_S,"e_cloud_shop_record_day_index",hld::e_cloud_shop_record_day_index);
   tolua_constant(tolua_S,"e_cloud_shop_record_template_id",hld::e_cloud_shop_record_template_id);
   tolua_constant(tolua_S,"e_cloud_shop_record_total_number",hld::e_cloud_shop_record_total_number);
   tolua_constant(tolua_S,"e_cloud_shop_record_buy_count",hld::e_cloud_shop_record_buy_count);
   tolua_constant(tolua_S,"e_cloud_shop_record_send_award_flag",hld::e_cloud_shop_record_send_award_flag);
   tolua_constant(tolua_S,"e_cloud_shop_record_max",hld::e_cloud_shop_record_max);
   tolua_constant(tolua_S,"e_seven_day_goal_info_id",hld::e_seven_day_goal_info_id);
   tolua_constant(tolua_S,"e_seven_day_goal_info_schedule",hld::e_seven_day_goal_info_schedule);
   tolua_constant(tolua_S,"e_seven_day_goal_info_state",hld::e_seven_day_goal_info_state);
   tolua_constant(tolua_S,"e_seven_day_goal_info_open_time",hld::e_seven_day_goal_info_open_time);
   tolua_constant(tolua_S,"e_seven_day_goal_info_max",hld::e_seven_day_goal_info_max);
   tolua_constant(tolua_S,"e_harry_record_harry_value",hld::e_harry_record_harry_value);
   tolua_constant(tolua_S,"e_harry_record_defend_value",hld::e_harry_record_defend_value);
   tolua_constant(tolua_S,"e_harry_record_normal_harry_count",hld::e_harry_record_normal_harry_count);
   tolua_constant(tolua_S,"e_harry_record_special_harry_count",hld::e_harry_record_special_harry_count);
   tolua_constant(tolua_S,"e_harry_record_kill_ct_num",hld::e_harry_record_kill_ct_num);
   tolua_constant(tolua_S,"e_harry_record_kill_terrorist_num",hld::e_harry_record_kill_terrorist_num);
   tolua_constant(tolua_S,"e_harry_record_harry_item_server_id",hld::e_harry_record_harry_item_server_id);
   tolua_constant(tolua_S,"e_harry_record_max",hld::e_harry_record_max);
   tolua_constant(tolua_S,"e_quality_level_non",hld::e_quality_level_non);
   tolua_constant(tolua_S,"e_quality_level_one",hld::e_quality_level_one);
   tolua_constant(tolua_S,"e_quality_level_two",hld::e_quality_level_two);
   tolua_constant(tolua_S,"e_quality_level_three",hld::e_quality_level_three);
   tolua_constant(tolua_S,"e_quality_level_four",hld::e_quality_level_four);
   tolua_constant(tolua_S,"e_quality_level_five",hld::e_quality_level_five);
   tolua_constant(tolua_S,"e_quality_level_six",hld::e_quality_level_six);
   tolua_constant(tolua_S,"e_quality_level_seven",hld::e_quality_level_seven);
   tolua_constant(tolua_S,"e_quality_level_eight",hld::e_quality_level_eight);
   tolua_constant(tolua_S,"e_quality_level_nine",hld::e_quality_level_nine);
   tolua_constant(tolua_S,"e_quality_level_ten",hld::e_quality_level_ten);
   tolua_constant(tolua_S,"e_quality_level_max",hld::e_quality_level_max);
   tolua_constant(tolua_S,"e_lua_item_info_non",hld::e_lua_item_info_non);
   tolua_constant(tolua_S,"e_lua_item_info_info_id",hld::e_lua_item_info_info_id);
   tolua_constant(tolua_S,"e_lua_item_info_slot",hld::e_lua_item_info_slot);
   tolua_constant(tolua_S,"e_lua_item_info_container_type",hld::e_lua_item_info_container_type);
   tolua_constant(tolua_S,"e_lua_item_info_stack_count",hld::e_lua_item_info_stack_count);
   tolua_constant(tolua_S,"e_lua_item_info_activate",hld::e_lua_item_info_activate);
   tolua_constant(tolua_S,"e_lua_item_info_is_first",hld::e_lua_item_info_is_first);
   tolua_constant(tolua_S,"e_lua_item_info_upgrade_count",hld::e_lua_item_info_upgrade_count);
   tolua_constant(tolua_S,"e_lua_item_info_stargrade_count",hld::e_lua_item_info_stargrade_count);
   tolua_constant(tolua_S,"e_lua_item_info_starskill_flag",hld::e_lua_item_info_starskill_flag);
   tolua_constant(tolua_S,"e_lua_item_info_add_on",hld::e_lua_item_info_add_on);
   tolua_constant(tolua_S,"e_lua_item_info_locked",hld::e_lua_item_info_locked);
   tolua_constant(tolua_S,"e_lua_item_info_wing_exp",hld::e_lua_item_info_wing_exp);
   tolua_constant(tolua_S,"e_lua_item_info_wing_bless",hld::e_lua_item_info_wing_bless);
   tolua_constant(tolua_S,"e_lua_item_info_random_had_flag",hld::e_lua_item_info_random_had_flag);
   tolua_constant(tolua_S,"e_lua_item_info_random_property1",hld::e_lua_item_info_random_property1);
   tolua_constant(tolua_S,"e_lua_item_info_random_property2",hld::e_lua_item_info_random_property2);
   tolua_constant(tolua_S,"e_lua_item_info_random_property3",hld::e_lua_item_info_random_property3);
   tolua_constant(tolua_S,"e_lua_item_info_random_property4",hld::e_lua_item_info_random_property4);
   tolua_constant(tolua_S,"e_lua_item_info_random_property5",hld::e_lua_item_info_random_property5);
   tolua_constant(tolua_S,"e_lua_item_info_random_property6",hld::e_lua_item_info_random_property6);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property1",hld::e_lua_item_info_succinct_property1);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property2",hld::e_lua_item_info_succinct_property2);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property3",hld::e_lua_item_info_succinct_property3);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property4",hld::e_lua_item_info_succinct_property4);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property5",hld::e_lua_item_info_succinct_property5);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_property6",hld::e_lua_item_info_succinct_property6);
   tolua_constant(tolua_S,"e_lua_item_info_showing_illusion_idex",hld::e_lua_item_info_showing_illusion_idex);
   tolua_constant(tolua_S,"e_lua_item_info_put_in_bag_count",hld::e_lua_item_info_put_in_bag_count);
   tolua_constant(tolua_S,"e_lua_item_info_illusion_had_byte",hld::e_lua_item_info_illusion_had_byte);
   tolua_constant(tolua_S,"e_lua_item_info_over_time",hld::e_lua_item_info_over_time);
   tolua_constant(tolua_S,"e_lua_item_info_succinct_level",hld::e_lua_item_info_succinct_level);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_0",hld::e_lua_item_info_jewel_slot_0);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_1",hld::e_lua_item_info_jewel_slot_1);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_2",hld::e_lua_item_info_jewel_slot_2);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_3",hld::e_lua_item_info_jewel_slot_3);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_slot_4",hld::e_lua_item_info_jewel_slot_4);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_vip_slot_0",hld::e_lua_item_info_jewel_vip_slot_0);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_vip_slot_1",hld::e_lua_item_info_jewel_vip_slot_1);
   tolua_constant(tolua_S,"e_lua_item_info_prototype_id",hld::e_lua_item_info_prototype_id);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property1",hld::e_lua_item_info_forge_property1);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property2",hld::e_lua_item_info_forge_property2);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property3",hld::e_lua_item_info_forge_property3);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property4",hld::e_lua_item_info_forge_property4);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property5",hld::e_lua_item_info_forge_property5);
   tolua_constant(tolua_S,"e_lua_item_info_forge_property6",hld::e_lua_item_info_forge_property6);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_carve_0",hld::e_lua_item_info_jewel_carve_0);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_carve_1",hld::e_lua_item_info_jewel_carve_1);
   tolua_constant(tolua_S,"e_lua_item_info_jewel_carve_2",hld::e_lua_item_info_jewel_carve_2);
   tolua_constant(tolua_S,"e_lua_item_info_max",hld::e_lua_item_info_max);
   tolua_constant(tolua_S,"e_money_array_type_cur",hld::e_money_array_type_cur);
   tolua_constant(tolua_S,"e_money_array_type_total_get",hld::e_money_array_type_total_get);
   tolua_constant(tolua_S,"e_money_array_type_total_cost",hld::e_money_array_type_total_cost);
   tolua_constant(tolua_S,"e_money_array_type_max",hld::e_money_array_type_max);
   tolua_constant(tolua_S,"e_summon_pet_type_normal_summon",hld::e_summon_pet_type_normal_summon);
   tolua_constant(tolua_S,"e_summon_pet_type_goddess",hld::e_summon_pet_type_goddess);
   tolua_constant(tolua_S,"e_summon_pet_type_boss_pet",hld::e_summon_pet_type_boss_pet);
   tolua_constant(tolua_S,"e_summon_pet_type_max",hld::e_summon_pet_type_max);
   tolua_constant(tolua_S,"e_version_region_type_china_main",hld::e_version_region_type_china_main);
   tolua_constant(tolua_S,"e_version_region_type_kr",hld::e_version_region_type_kr);
   tolua_constant(tolua_S,"e_version_region_type_china_tw",hld::e_version_region_type_china_tw);
   tolua_constant(tolua_S,"e_version_region_type_en",hld::e_version_region_type_en);
   tolua_constant(tolua_S,"e_version_region_type_tha",hld::e_version_region_type_tha);
   tolua_constant(tolua_S,"e_version_region_type_vn",hld::e_version_region_type_vn);
   tolua_constant(tolua_S,"e_version_region_type_jp",hld::e_version_region_type_jp);
   tolua_constant(tolua_S,"e_version_region_type_rus",hld::e_version_region_type_rus);
   tolua_constant(tolua_S,"e_version_region_type_sg",hld::e_version_region_type_sg);
   tolua_constant(tolua_S,"e_version_region_type_max",hld::e_version_region_type_max);
   tolua_constant(tolua_S,"e_manual_reward_type_add_item",hld::e_manual_reward_type_add_item);
   tolua_constant(tolua_S,"e_manual_reward_type_manual_level",hld::e_manual_reward_type_manual_level);
   tolua_constant(tolua_S,"e_manual_reward_type_add_point",hld::e_manual_reward_type_add_point);
   tolua_constant(tolua_S,"e_manual_reward_type_max",hld::e_manual_reward_type_max);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_none",hld::e_time_limit_shop_purchase_limit_type_none);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_day",hld::e_time_limit_shop_purchase_limit_type_day);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_cycle",hld::e_time_limit_shop_purchase_limit_type_cycle);
   tolua_constant(tolua_S,"e_time_limit_shop_purchase_limit_type_max",hld::e_time_limit_shop_purchase_limit_type_max);
   tolua_constant(tolua_S,"e_role_i64_info_gs_value",hld::e_role_i64_info_gs_value);
   tolua_constant(tolua_S,"e_role_i64_info_history_best_gs_value",hld::e_role_i64_info_history_best_gs_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_mount_up_start_value",hld::e_role_i64_info_zero_mount_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_wing_up_start_value",hld::e_role_i64_info_zero_wing_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_equip_up_start_value",hld::e_role_i64_info_zero_equip_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_zero_spirit_up_start_value",hld::e_role_i64_info_zero_spirit_up_start_value);
   tolua_constant(tolua_S,"e_role_i64_info_create_time",hld::e_role_i64_info_create_time);
   tolua_constant(tolua_S,"e_role_i64_info_max",hld::e_role_i64_info_max);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_berserker",hld::e_cross_server_pk_sub_type_berserker);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_caster",hld::e_cross_server_pk_sub_type_caster);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_add_rider",hld::e_cross_server_pk_sub_type_add_rider);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_add_assassion",hld::e_cross_server_pk_sub_type_add_assassion);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_word_boss",hld::e_cross_server_pk_sub_type_word_boss);
   tolua_constant(tolua_S,"e_cross_server_pk_sub_type_max",hld::e_cross_server_pk_sub_type_max);
   tolua_constant(tolua_S,"e_star_trip_target_type_buy_goods",hld::e_star_trip_target_type_buy_goods);
   tolua_constant(tolua_S,"e_star_trip_target_type_buy_single_boss",hld::e_star_trip_target_type_buy_single_boss);
   tolua_constant(tolua_S,"e_star_trip_target_type_convert",hld::e_star_trip_target_type_convert);
   tolua_constant(tolua_S,"e_star_trip_target_type_legion_donate",hld::e_star_trip_target_type_legion_donate);
   tolua_constant(tolua_S,"e_star_trip_target_type_join_activity",hld::e_star_trip_target_type_join_activity);
   tolua_constant(tolua_S,"e_star_trip_target_type_buy_today_gift",hld::e_star_trip_target_type_buy_today_gift);
   tolua_constant(tolua_S,"e_star_trip_target_type_map_finish",hld::e_star_trip_target_type_map_finish);
   tolua_constant(tolua_S,"e_star_trip_target_type_kill_single_boss",hld::e_star_trip_target_type_kill_single_boss);
   tolua_constant(tolua_S,"e_star_trip_target_type_kill_home_boss",hld::e_star_trip_target_type_kill_home_boss);
   tolua_constant(tolua_S,"e_star_trip_target_type_treasure",hld::e_star_trip_target_type_treasure);
   tolua_constant(tolua_S,"e_star_trip_target_type_recharge",hld::e_star_trip_target_type_recharge);
   tolua_constant(tolua_S,"e_star_trip_target_type_bid",hld::e_star_trip_target_type_bid);
   tolua_constant(tolua_S,"e_star_trip_target_type_max",hld::e_star_trip_target_type_max);
   tolua_constant(tolua_S,"e_map_play_type_non",hld::e_map_play_type_non);
   tolua_constant(tolua_S,"e_map_play_type_kill_boss",hld::e_map_play_type_kill_boss);
   tolua_constant(tolua_S,"e_map_play_type_max",hld::e_map_play_type_max);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_no_kill",hld::e_map_search_boss_type_boss_no_kill);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_can_kill",hld::e_map_search_boss_type_boss_can_kill);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_wait",hld::e_map_search_boss_type_boss_wait);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_refresh",hld::e_map_search_boss_type_boss_refresh);
   tolua_constant(tolua_S,"e_map_search_boss_type_boss_killed",hld::e_map_search_boss_type_boss_killed);
   tolua_constant(tolua_S,"e_map_search_boss_type_be_killed",hld::e_map_search_boss_type_be_killed);
   tolua_constant(tolua_S,"e_map_search_boss_type_max",hld::e_map_search_boss_type_max);
   tolua_constant(tolua_S,"e_daily_play_count_element_map",hld::e_daily_play_count_element_map);
   tolua_constant(tolua_S,"e_daily_play_count_battle_map",hld::e_daily_play_count_battle_map);
   tolua_constant(tolua_S,"e_daily_play_count_max",hld::e_daily_play_count_max);
   tolua_constant(tolua_S,"e_time_activity_type_null",hld::e_time_activity_type_null);
   tolua_constant(tolua_S,"e_time_activity_type_wheel_plate",hld::e_time_activity_type_wheel_plate);
   tolua_constant(tolua_S,"e_time_activity_type_max",hld::e_time_activity_type_max);
   tolua_constant(tolua_S,"e_time_activity_score_type_nomal_search",hld::e_time_activity_score_type_nomal_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_monster_search",hld::e_time_activity_score_type_monster_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_flip_box_search",hld::e_time_activity_score_type_flip_box_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_finger_guess_search",hld::e_time_activity_score_type_finger_guess_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_special_search",hld::e_time_activity_score_type_special_search);
   tolua_constant(tolua_S,"e_time_activity_score_type_max",hld::e_time_activity_score_type_max);
   tolua_constant(tolua_S,"e_wheel_plate_info_accumulate_num",hld::e_wheel_plate_info_accumulate_num);
   tolua_constant(tolua_S,"e_wheel_plate_info_open_index",hld::e_wheel_plate_info_open_index);
   tolua_constant(tolua_S,"e_wheel_plate_info_award_index",hld::e_wheel_plate_info_award_index);
   tolua_constant(tolua_S,"e_wheel_plate_info_max",hld::e_wheel_plate_info_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"ws_client","hld::ws_client","",NULL);
   tolua_beginmodule(tolua_S,"ws_client");
    tolua_function(tolua_S,"getInstance",tolua_logic_hld_ws_client_getInstance00);
    tolua_function(tolua_S,"send_to_dp_lua",tolua_logic_hld_ws_client_send_to_dp_lua00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"arena_mgr_ws","hld::arena_mgr_ws","",NULL);
   tolua_beginmodule(tolua_S,"arena_mgr_ws");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_arena_mgr_ws_get_instance00);
    tolua_function(tolua_S,"create_robot",tolua_logic_hld_arena_mgr_ws_create_robot00);
    tolua_function(tolua_S,"fill_empty_pos",tolua_logic_hld_arena_mgr_ws_fill_empty_pos00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"cross_server_city_war_ws_mgr","hld::cross_server_city_war_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_city_war_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_city_war_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_in_game_time",tolua_logic_hld_cross_server_city_war_ws_mgr_is_in_game_time00);
    tolua_function(tolua_S,"get_cur_legion_in_rank",tolua_logic_hld_cross_server_city_war_ws_mgr_get_cur_legion_in_rank00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_banquet_level_ordinary",hld::e_banquet_level_ordinary);
   tolua_constant(tolua_S,"e_banquet_level_hearty",hld::e_banquet_level_hearty);
   tolua_constant(tolua_S,"e_banquet_level_luxury",hld::e_banquet_level_luxury);
   tolua_constant(tolua_S,"e_banquet_level_max",hld::e_banquet_level_max);
   tolua_constant(tolua_S,"e_legion_job_title_none",hld::e_legion_job_title_none);
   tolua_constant(tolua_S,"e_legion_job_title_commander",hld::e_legion_job_title_commander);
   tolua_constant(tolua_S,"e_legion_job_title_assistant_chief",hld::e_legion_job_title_assistant_chief);
   tolua_constant(tolua_S,"e_legion_job_title_chief",hld::e_legion_job_title_chief);
   tolua_constant(tolua_S,"e_legion_job_title_max",hld::e_legion_job_title_max);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_main",hld::ELegionInfo_construction_level_main);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_1",hld::ELegionInfo_construction_level_1);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_2",hld::ELegionInfo_construction_level_2);
   tolua_constant(tolua_S,"ELegionInfo_construction_level_3",hld::ELegionInfo_construction_level_3);
   tolua_constant(tolua_S,"ELegionInfo_asset_money",hld::ELegionInfo_asset_money);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_1",hld::ELegionInfo_asset_item_1);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_2",hld::ELegionInfo_asset_item_2);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_3",hld::ELegionInfo_asset_item_3);
   tolua_constant(tolua_S,"ELegionInfo_asset_item_4",hld::ELegionInfo_asset_item_4);
   tolua_constant(tolua_S,"ELegionInfo_fighting_power",hld::ELegionInfo_fighting_power);
   tolua_constant(tolua_S,"ELegionInfo_auto_accept_new_member",hld::ELegionInfo_auto_accept_new_member);
   tolua_constant(tolua_S,"ELegionInfo_last_refresh_time",hld::ELegionInfo_last_refresh_time);
   tolua_constant(tolua_S,"ELegionInfo_glory_glow",hld::ELegionInfo_glory_glow);
   tolua_constant(tolua_S,"ELegionInfo_act_score_raid",hld::ELegionInfo_act_score_raid);
   tolua_constant(tolua_S,"ELegionInfo_act_score_bonfire",hld::ELegionInfo_act_score_bonfire);
   tolua_constant(tolua_S,"ELegionInfo_legion_welfare_num",hld::ELegionInfo_legion_welfare_num);
   tolua_constant(tolua_S,"ELegionInfo_max",hld::ELegionInfo_max);
   tolua_constant(tolua_S,"e_legion_apply_stage_send_failed",hld::e_legion_apply_stage_send_failed);
   tolua_constant(tolua_S,"e_legion_apply_stage_send_success",hld::e_legion_apply_stage_send_success);
   tolua_constant(tolua_S,"e_legion_apply_stage_is_accepted",hld::e_legion_apply_stage_is_accepted);
   tolua_constant(tolua_S,"e_legion_apply_stage_is_refused",hld::e_legion_apply_stage_is_refused);
   tolua_constant(tolua_S,"e_legion_apply_stage_auto_accepted",hld::e_legion_apply_stage_auto_accepted);
   tolua_constant(tolua_S,"e_legion_apply_stage_max",hld::e_legion_apply_stage_max);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"s_legion_member_info","hld::s_legion_member_info","",tolua_collect_hld__s_legion_member_info);
   #else
   tolua_cclass(tolua_S,"s_legion_member_info","hld::s_legion_member_info","",NULL);
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
   tolua_constant(tolua_S,"e_legion_name_error_change_name_succeed",hld::e_legion_name_error_change_name_succeed);
   tolua_constant(tolua_S,"e_legion_name_error_not_chief",hld::e_legion_name_error_not_chief);
   tolua_constant(tolua_S,"e_legion_name_error_formal_error",hld::e_legion_name_error_formal_error);
   tolua_constant(tolua_S,"e_legion_name_error_repetition",hld::e_legion_name_error_repetition);
   tolua_constant(tolua_S,"e_legion_name_error_contains_illegal_characters",hld::e_legion_name_error_contains_illegal_characters);
   tolua_constant(tolua_S,"e_legion_name_error_cross_server",hld::e_legion_name_error_cross_server);
   tolua_constant(tolua_S,"e_legion_name_error_available",hld::e_legion_name_error_available);
   tolua_constant(tolua_S,"e_legion_name_error_player_not_legion",hld::e_legion_name_error_player_not_legion);
   tolua_constant(tolua_S,"e_legion_name_error_item_not_enough",hld::e_legion_name_error_item_not_enough);
   tolua_constant(tolua_S,"e_legion_error_none",hld::e_legion_error_none);
   tolua_constant(tolua_S,"e_legion_error_template_error",hld::e_legion_error_template_error);
   tolua_constant(tolua_S,"e_legion_error_member_full",hld::e_legion_error_member_full);
   tolua_constant(tolua_S,"e_legion_error_applicant_list_full",hld::e_legion_error_applicant_list_full);
   tolua_constant(tolua_S,"e_legion_error_applicant_already_exist",hld::e_legion_error_applicant_already_exist);
   tolua_constant(tolua_S,"e_legion_error_player_added_not_online",hld::e_legion_error_player_added_not_online);
   tolua_constant(tolua_S,"e_legion_error_player_added_is_legion_member",hld::e_legion_error_player_added_is_legion_member);
   tolua_constant(tolua_S,"e_legion_error_player_added_is_already_join_other_legion",hld::e_legion_error_player_added_is_already_join_other_legion);
   tolua_constant(tolua_S,"e_legion_error_used_all_take_part_in_banquet_times",hld::e_legion_error_used_all_take_part_in_banquet_times);
   tolua_constant(tolua_S,"e_legion_error_banquet_already_over",hld::e_legion_error_banquet_already_over);
   tolua_constant(tolua_S,"e_legion_error_no_city_war_qualification",hld::e_legion_error_no_city_war_qualification);
   tolua_constant(tolua_S,"e_legion_error_city_war_not_begin",hld::e_legion_error_city_war_not_begin);
   tolua_constant(tolua_S,"e_legion_error_can_not_dissolve_during_city_war",hld::e_legion_error_can_not_dissolve_during_city_war);
   tolua_constant(tolua_S,"e_legion_error_can_not_leave_legion_during_city_war",hld::e_legion_error_can_not_leave_legion_during_city_war);
   tolua_constant(tolua_S,"e_legion_error_error_legion_name_illegal",hld::e_legion_error_error_legion_name_illegal);
   tolua_constant(tolua_S,"e_legion_error_can_not_dissolve_during_banquet",hld::e_legion_error_can_not_dissolve_during_banquet);
   tolua_constant(tolua_S,"e_legion_error_can_not_kickout_member_during_city_war",hld::e_legion_error_can_not_kickout_member_during_city_war);
   tolua_constant(tolua_S,"e_legion_error_repeat_legion_name",hld::e_legion_error_repeat_legion_name);
   tolua_constant(tolua_S,"e_legion_error_full_legion_num",hld::e_legion_error_full_legion_num);
   tolua_constant(tolua_S,"e_legion_error_create_legion_invalid_role_state",hld::e_legion_error_create_legion_invalid_role_state);
   tolua_constant(tolua_S,"e_legion_error_create_legion_no_enough_level",hld::e_legion_error_create_legion_no_enough_level);
   tolua_constant(tolua_S,"e_legion_error_create_legion_no_enough_money",hld::e_legion_error_create_legion_no_enough_money);
   tolua_constant(tolua_S,"e_legion_error_invalid_banquet_type",hld::e_legion_error_invalid_banquet_type);
   tolua_constant(tolua_S,"e_legion_error_can_cur_time_error",hld::e_legion_error_can_cur_time_error);
   tolua_constant(tolua_S,"e_legion_error_can_not_hold_banquet_in_legion_war_day",hld::e_legion_error_can_not_hold_banquet_in_legion_war_day);
   tolua_constant(tolua_S,"e_legion_error_invalid_hold_banquet_time",hld::e_legion_error_invalid_hold_banquet_time);
   tolua_constant(tolua_S,"e_legion_error_request_hold_banquet_again",hld::e_legion_error_request_hold_banquet_again);
   tolua_constant(tolua_S,"e_legion_error_already_occupy_stronghold",hld::e_legion_error_already_occupy_stronghold);
   tolua_constant(tolua_S,"e_legion_error_already_get_occupation_daily_award",hld::e_legion_error_already_get_occupation_daily_award);
   tolua_constant(tolua_S,"e_legion_error_next_day_get_occupation_daily_award",hld::e_legion_error_next_day_get_occupation_daily_award);
   tolua_constant(tolua_S,"e_legion_error_city_war_gather_crystal_field_not_open",hld::e_legion_error_city_war_gather_crystal_field_not_open);
   tolua_constant(tolua_S,"e_legion_error_stronghold_occupied_by_others",hld::e_legion_error_stronghold_occupied_by_others);
   tolua_constant(tolua_S,"e_legion_error_crystal_gathered_by_others",hld::e_legion_error_crystal_gathered_by_others);
   tolua_constant(tolua_S,"e_legion_error_job_title_num_max",hld::e_legion_error_job_title_num_max);
   tolua_constant(tolua_S,"e_legion_error_player_joined_other_legion",hld::e_legion_error_player_joined_other_legion);
   tolua_constant(tolua_S,"e_legion_error_is_in_legion_answer_time",hld::e_legion_error_is_in_legion_answer_time);
   tolua_constant(tolua_S,"e_legion_error_max",hld::e_legion_error_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"legion_ws_applicant_list","hld::legion_ws_applicant_list","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_applicant_list");
    tolua_function(tolua_S,"get_applicant",tolua_logic_hld_legion_ws_applicant_list_get_applicant00);
    tolua_function(tolua_S,"del_applicant",tolua_logic_hld_legion_ws_applicant_list_del_applicant00);
    tolua_function(tolua_S,"del_applicant_with_join_legion",tolua_logic_hld_legion_ws_applicant_list_del_applicant_with_join_legion00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"legion_ws_answer","hld::legion_ws_answer","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_answer");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_legion_ws_answer_get_instance00);
    tolua_function(tolua_S,"get_legion_question_info",tolua_logic_hld_legion_ws_answer_get_legion_question_info00);
    tolua_function(tolua_S,"send_legion_answer_rank_list",tolua_logic_hld_legion_ws_answer_send_legion_answer_rank_list00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"legion_ws_warehouse","hld::legion_ws_warehouse","",NULL);
   tolua_beginmodule(tolua_S,"legion_ws_warehouse");
    tolua_function(tolua_S,"send_item_list_to_client",tolua_logic_hld_legion_ws_warehouse_send_item_list_to_client00);
    tolua_function(tolua_S,"send_log_list_to_client",tolua_logic_hld_legion_ws_warehouse_send_log_list_to_client00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"legion_ws","hld::legion_ws","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"city_war_territory_mgr","hld::city_war_territory_mgr","",NULL);
   tolua_beginmodule(tolua_S,"city_war_territory_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_city_war_territory_mgr_get_instance00);
    tolua_function(tolua_S,"bid_one_territory",tolua_logic_hld_city_war_territory_mgr_bid_one_territory00);
    tolua_function(tolua_S,"set_is_prior_maintain",tolua_logic_hld_city_war_territory_mgr_set_is_prior_maintain00);
    tolua_function(tolua_S,"req_cross_server_apply_city_war_bid",tolua_logic_hld_city_war_territory_mgr_req_cross_server_apply_city_war_bid00);
    tolua_function(tolua_S,"req_cross_server_territory_prior_maintain",tolua_logic_hld_city_war_territory_mgr_req_cross_server_territory_prior_maintain00);
    tolua_function(tolua_S,"req_cross_server_legion_city_war_info",tolua_logic_hld_city_war_territory_mgr_req_cross_server_legion_city_war_info00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"legion_ws_city_war","hld::legion_ws_city_war","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"legion_ws_mgr","hld::legion_ws_mgr","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"invalid_ansi_word","hld::invalid_ansi_word","",NULL);
   tolua_beginmodule(tolua_S,"invalid_ansi_word");
    tolua_function(tolua_S,"include_invalid_ansi_str",tolua_logic_hld_invalid_ansi_word_include_invalid_ansi_str00);
    tolua_function(tolua_S,"is_valid_ansi_str",tolua_logic_hld_invalid_ansi_word_is_valid_ansi_str00);
    tolua_function(tolua_S,"replace_invalid_ansi_str",tolua_logic_hld_invalid_ansi_word_replace_invalid_ansi_str00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_join_team_way_by_team_guid",hld::e_join_team_way_by_team_guid);
   tolua_constant(tolua_S,"e_join_team_way_by_captain_guid",hld::e_join_team_way_by_captain_guid);
   tolua_constant(tolua_S,"e_join_team_way_max",hld::e_join_team_way_max);
   tolua_constant(tolua_S,"ETeamMemberInfo_is_online",hld::ETeamMemberInfo_is_online);
   tolua_constant(tolua_S,"ETeamMemberInfo_line_id",hld::ETeamMemberInfo_line_id);
   tolua_constant(tolua_S,"ETeamMemberInfo_player_template_id",hld::ETeamMemberInfo_player_template_id);
   tolua_constant(tolua_S,"ETeamMemberInfo_cur_hp",hld::ETeamMemberInfo_cur_hp);
   tolua_constant(tolua_S,"ETeamMemberInfo_max_hp",hld::ETeamMemberInfo_max_hp);
   tolua_constant(tolua_S,"ETeamMemberInfo_level",hld::ETeamMemberInfo_level);
   tolua_constant(tolua_S,"ETeamMemberInfo_fighting_power",hld::ETeamMemberInfo_fighting_power);
   tolua_constant(tolua_S,"ETeamMemberInfo_cur_map",hld::ETeamMemberInfo_cur_map);
   tolua_constant(tolua_S,"ETeamMemberInfo_damage",hld::ETeamMemberInfo_damage);
   tolua_constant(tolua_S,"ETeamMemberInfo_PosX",hld::ETeamMemberInfo_PosX);
   tolua_constant(tolua_S,"ETeamMemberInfo_PosY",hld::ETeamMemberInfo_PosY);
   tolua_constant(tolua_S,"ETeamMemberInfo_PosZ",hld::ETeamMemberInfo_PosZ);
   tolua_constant(tolua_S,"ETeamMemberInfo_ready_state",hld::ETeamMemberInfo_ready_state);
   tolua_constant(tolua_S,"ETeamMemberInfo_follow_state",hld::ETeamMemberInfo_follow_state);
   tolua_constant(tolua_S,"ETeamMemberInfo_check_ret",hld::ETeamMemberInfo_check_ret);
   tolua_constant(tolua_S,"ETeamMemberInfo_one_stop_flags",hld::ETeamMemberInfo_one_stop_flags);
   tolua_constant(tolua_S,"ETeamMemberInfo_agora_id",hld::ETeamMemberInfo_agora_id);
   tolua_constant(tolua_S,"ETeamMemberInfo_max",hld::ETeamMemberInfo_max);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"s_team_member_info","hld::s_team_member_info","",tolua_collect_hld__s_team_member_info);
   #else
   tolua_cclass(tolua_S,"s_team_member_info","hld::s_team_member_info","",NULL);
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
   tolua_constant(tolua_S,"e_team_error_none",hld::e_team_error_none);
   tolua_constant(tolua_S,"e_team_error_team_full",hld::e_team_error_team_full);
   tolua_constant(tolua_S,"e_team_error_fighting_power_not_enough",hld::e_team_error_fighting_power_not_enough);
   tolua_constant(tolua_S,"e_team_error_level_not_enough",hld::e_team_error_level_not_enough);
   tolua_constant(tolua_S,"e_team_error_player_already_join_team",hld::e_team_error_player_already_join_team);
   tolua_constant(tolua_S,"e_team_error_player_already_join_other_team",hld::e_team_error_player_already_join_other_team);
   tolua_constant(tolua_S,"e_team_error_player_already_offline",hld::e_team_error_player_already_offline);
   tolua_constant(tolua_S,"e_team_error_team_already_dissolve",hld::e_team_error_team_already_dissolve);
   tolua_constant(tolua_S,"e_team_error_target_player_not_in_team",hld::e_team_error_target_player_not_in_team);
   tolua_constant(tolua_S,"e_team_error_target_player_is_not_captain",hld::e_team_error_target_player_is_not_captain);
   tolua_constant(tolua_S,"e_team_error_captain_change",hld::e_team_error_captain_change);
   tolua_constant(tolua_S,"e_team_error_create_raid_failed",hld::e_team_error_create_raid_failed);
   tolua_constant(tolua_S,"e_team_error_invalid_raid_instance",hld::e_team_error_invalid_raid_instance);
   tolua_constant(tolua_S,"e_team_error_member_change_during_transfer",hld::e_team_error_member_change_during_transfer);
   tolua_constant(tolua_S,"e_team_error_raid_condition_less",hld::e_team_error_raid_condition_less);
   tolua_constant(tolua_S,"e_team_error_cur_map_cant_ready",hld::e_team_error_cur_map_cant_ready);
   tolua_constant(tolua_S,"e_team_error_team_member_is_full",hld::e_team_error_team_member_is_full);
   tolua_constant(tolua_S,"e_team_error_raidmulti_ingame",hld::e_team_error_raidmulti_ingame);
   tolua_constant(tolua_S,"e_team_error_max",hld::e_team_error_max);
   tolua_constant(tolua_S,"e_player_match_team_none",hld::e_player_match_team_none);
   tolua_constant(tolua_S,"e_player_match_team_waiting",hld::e_player_match_team_waiting);
   tolua_constant(tolua_S,"e_player_match_team_succeed",hld::e_player_match_team_succeed);
   tolua_constant(tolua_S,"e_team_common_check_invoker_auto_match_team",hld::e_team_common_check_invoker_auto_match_team);
   tolua_constant(tolua_S,"e_team_common_check_invoker_mem_ready",hld::e_team_common_check_invoker_mem_ready);
   tolua_constant(tolua_S,"e_team_common_check_invoker_up_mem_check_ret",hld::e_team_common_check_invoker_up_mem_check_ret);
   tolua_constant(tolua_S,"e_team_req_mate_type_friend",hld::e_team_req_mate_type_friend);
   tolua_constant(tolua_S,"e_team_req_mate_type_legion",hld::e_team_req_mate_type_legion);
   tolua_constant(tolua_S,"e_team_mem_ready_state_none",hld::e_team_mem_ready_state_none);
   tolua_constant(tolua_S,"e_team_mem_ready_state_ready",hld::e_team_mem_ready_state_ready);
   tolua_constant(tolua_S,"e_team_mem_ready_state_waiting_ready",hld::e_team_mem_ready_state_waiting_ready);
   tolua_constant(tolua_S,"e_team_one_stop_aim_ok",hld::e_team_one_stop_aim_ok);
   tolua_constant(tolua_S,"e_team_one_stop_aim_lack_of_times",hld::e_team_one_stop_aim_lack_of_times);
   tolua_constant(tolua_S,"e_team_one_stop_aim_locked",hld::e_team_one_stop_aim_locked);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"team_ws","hld::team_ws","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"team_ws_mgr","hld::team_ws_mgr","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"cs_map_system","hld::cs_map_system","",NULL);
   tolua_beginmodule(tolua_S,"cs_map_system");
    tolua_function(tolua_S,"get_map_template_id",tolua_logic_hld_cs_map_system_get_map_template_id00);
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"cs_map_mgr_system","hld::cs_map_mgr_system","",NULL);
   tolua_beginmodule(tolua_S,"cs_map_mgr_system");
    tolua_function(tolua_S,"send_map_error_to_client",tolua_logic_hld_cs_map_mgr_system_send_map_error_to_client00);
    tolua_function(tolua_S,"transfer_to_map",tolua_logic_hld_cs_map_mgr_system_transfer_to_map00);
    tolua_function(tolua_S,"send_big_player",tolua_logic_hld_cs_map_mgr_system_send_big_player00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"pk_king_mgr","hld::pk_king_mgr","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_map_info_player_num",hld::e_map_info_player_num);
   tolua_constant(tolua_S,"e_map_info_boss_cur_hp",hld::e_map_info_boss_cur_hp);
   tolua_constant(tolua_S,"e_map_info_boss_max_hp",hld::e_map_info_boss_max_hp);
   tolua_constant(tolua_S,"e_map_open_new",hld::e_map_open_new);
   tolua_constant(tolua_S,"e_map_open_old",hld::e_map_open_old);
   tolua_constant(tolua_S,"e_map_close_close",hld::e_map_close_close);
   tolua_constant(tolua_S,"e_map_close_hold",hld::e_map_close_hold);
   tolua_constant(tolua_S,"e_map_time_mode_none",hld::e_map_time_mode_none);
   tolua_constant(tolua_S,"e_map_time_mode_timeout_success",hld::e_map_time_mode_timeout_success);
   tolua_constant(tolua_S,"e_map_time_mode_timeout_fail",hld::e_map_time_mode_timeout_fail);
   tolua_constant(tolua_S,"e_map_time_mode_timeout_transfer",hld::e_map_time_mode_timeout_transfer);
   tolua_constant(tolua_S,"e_map_time_mode_max",hld::e_map_time_mode_max);
   tolua_constant(tolua_S,"e_map_war_result_lost",hld::e_map_war_result_lost);
   tolua_constant(tolua_S,"e_map_war_result_peace",hld::e_map_war_result_peace);
   tolua_constant(tolua_S,"e_map_war_result_win",hld::e_map_war_result_win);
   tolua_constant(tolua_S,"e_mopping_up_result_none",hld::e_mopping_up_result_none);
   tolua_constant(tolua_S,"e_mopping_up_result_finish",hld::e_mopping_up_result_finish);
   tolua_constant(tolua_S,"e_mopping_up_result_no_enter_count",hld::e_mopping_up_result_no_enter_count);
   tolua_constant(tolua_S,"e_mopping_up_result_max",hld::e_mopping_up_result_max);
   tolua_constant(tolua_S,"e_consume_enter_count_cond_none",hld::e_consume_enter_count_cond_none);
   tolua_constant(tolua_S,"e_consume_enter_count_cond_enter",hld::e_consume_enter_count_cond_enter);
   tolua_constant(tolua_S,"e_consume_enter_count_cond_win",hld::e_consume_enter_count_cond_win);
   tolua_constant(tolua_S,"e_map_win_cond_none",hld::e_map_win_cond_none);
   tolua_constant(tolua_S,"e_map_win_cond_kill_boss",hld::e_map_win_cond_kill_boss);
   tolua_constant(tolua_S,"e_map_win_cond_clear_all_wave",hld::e_map_win_cond_clear_all_wave);
   tolua_constant(tolua_S,"e_map_win_cond_time_out",hld::e_map_win_cond_time_out);
   tolua_constant(tolua_S,"e_map_win_cond_max",hld::e_map_win_cond_max);
   tolua_constant(tolua_S,"e_map_settle_type_finish_time",hld::e_map_settle_type_finish_time);
   tolua_constant(tolua_S,"e_map_settle_type_kill_boss_num",hld::e_map_settle_type_kill_boss_num);
   tolua_constant(tolua_S,"e_map_settle_type_protect_npc_hp",hld::e_map_settle_type_protect_npc_hp);
   tolua_constant(tolua_S,"e_map_settle_type_max",hld::e_map_settle_type_max);
   tolua_constant(tolua_S,"e_map_result_dialog_type_none",hld::e_map_result_dialog_type_none);
   tolua_constant(tolua_S,"e_map_result_dialog_type_lucky_draw",hld::e_map_result_dialog_type_lucky_draw);
   tolua_constant(tolua_S,"e_map_result_dialog_type_lucky_draw_and_fixed_award",hld::e_map_result_dialog_type_lucky_draw_and_fixed_award);
   tolua_constant(tolua_S,"e_map_result_dialog_type_demons_tower",hld::e_map_result_dialog_type_demons_tower);
   tolua_constant(tolua_S,"e_map_result_dialog_type_legion_activity",hld::e_map_result_dialog_type_legion_activity);
   tolua_constant(tolua_S,"e_map_result_dialog_type_score_and_rank_award",hld::e_map_result_dialog_type_score_and_rank_award);
   tolua_constant(tolua_S,"e_map_result_dialog_type_city_war_award",hld::e_map_result_dialog_type_city_war_award);
   tolua_constant(tolua_S,"e_map_result_dialog_type_arena",hld::e_map_result_dialog_type_arena);
   tolua_constant(tolua_S,"e_map_result_dialog_type_exp_raid",hld::e_map_result_dialog_type_exp_raid);
   tolua_constant(tolua_S,"e_map_result_dialog_type_pk_king",hld::e_map_result_dialog_type_pk_king);
   tolua_constant(tolua_S,"e_map_result_dialog_type_max",hld::e_map_result_dialog_type_max);
   tolua_constant(tolua_S,"e_map_result_dialog_pk_king_sub_type_death",hld::e_map_result_dialog_pk_king_sub_type_death);
   tolua_constant(tolua_S,"e_map_result_dialog_pk_king_sub_type_No_death",hld::e_map_result_dialog_pk_king_sub_type_No_death);
   tolua_constant(tolua_S,"e_map_lua_type_init",hld::e_map_lua_type_init);
   tolua_constant(tolua_S,"e_map_lua_type_tick",hld::e_map_lua_type_tick);
   tolua_constant(tolua_S,"e_map_lua_type_end",hld::e_map_lua_type_end);
   tolua_constant(tolua_S,"e_map_lua_type_unit_in",hld::e_map_lua_type_unit_in);
   tolua_constant(tolua_S,"e_map_lua_type_unit_out",hld::e_map_lua_type_unit_out);
   tolua_constant(tolua_S,"e_map_lua_type_unit_dead",hld::e_map_lua_type_unit_dead);
   tolua_constant(tolua_S,"e_double_line_map_template_id",hld::e_double_line_map_template_id);
   tolua_constant(tolua_S,"e_double_line_open_time_hour",hld::e_double_line_open_time_hour);
   tolua_constant(tolua_S,"e_double_line_open_time_min",hld::e_double_line_open_time_min);
   tolua_constant(tolua_S,"e_double_line_open_time_seconds",hld::e_double_line_open_time_seconds);
   tolua_constant(tolua_S,"e_double_line_end_time_hour",hld::e_double_line_end_time_hour);
   tolua_constant(tolua_S,"e_double_line_end_time_min",hld::e_double_line_end_time_min);
   tolua_constant(tolua_S,"e_double_line_end_time_seconds",hld::e_double_line_end_time_seconds);
   tolua_constant(tolua_S,"e_double_line_buff_id",hld::e_double_line_buff_id);
   tolua_constant(tolua_S,"e_double_line_notice_id",hld::e_double_line_notice_id);
   tolua_constant(tolua_S,"e_double_line_max",hld::e_double_line_max);
   tolua_constant(tolua_S,"e_map_target_image_element_red",hld::e_map_target_image_element_red);
   tolua_constant(tolua_S,"e_map_target_image_element_blue",hld::e_map_target_image_element_blue);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"cross_server_pk_ws_mgr","hld::cross_server_pk_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_pk_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_pk_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_can_join_with_cross",tolua_logic_hld_cross_server_pk_ws_mgr_is_can_join_with_cross00);
    tolua_function(tolua_S,"send_to_gate_transfer_map",tolua_logic_hld_cross_server_pk_ws_mgr_send_to_gate_transfer_map00);
    tolua_function(tolua_S,"get_pk_king_map_guid",tolua_logic_hld_cross_server_pk_ws_mgr_get_pk_king_map_guid00);
    tolua_function(tolua_S,"get_map_temp_id_with_class_type",tolua_logic_hld_cross_server_pk_ws_mgr_get_map_temp_id_with_class_type00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"crystal_fairyland_mgr","hld::crystal_fairyland_mgr","",NULL);
   tolua_beginmodule(tolua_S,"crystal_fairyland_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_crystal_fairyland_mgr_get_instance00);
    tolua_function(tolua_S,"is_activity_open",tolua_logic_hld_crystal_fairyland_mgr_is_activity_open00);
    tolua_function(tolua_S,"is_can_join",tolua_logic_hld_crystal_fairyland_mgr_is_can_join00);
    tolua_function(tolua_S,"get_valid_shui_jing_map",tolua_logic_hld_crystal_fairyland_mgr_get_valid_shui_jing_map00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_broken_sky_not_start",hld::e_broken_sky_not_start);
   tolua_constant(tolua_S,"e_broken_sky_in_join",hld::e_broken_sky_in_join);
   tolua_constant(tolua_S,"e_broken_sky_in_game",hld::e_broken_sky_in_game);
   tolua_constant(tolua_S,"e_broken_sky_game_end",hld::e_broken_sky_game_end);
   tolua_cclass(tolua_S,"broken_sky_mgr","hld::broken_sky_mgr","",NULL);
   tolua_beginmodule(tolua_S,"broken_sky_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_broken_sky_mgr_get_instance00);
    tolua_function(tolua_S,"is_can_join",tolua_logic_hld_broken_sky_mgr_is_can_join00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"world_boss_ws_mgr","hld::world_boss_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"world_boss_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_world_boss_ws_mgr_get_instance00);
    tolua_function(tolua_S,"req_frist_kill_world_boss_prize",tolua_logic_hld_world_boss_ws_mgr_req_frist_kill_world_boss_prize00);
    tolua_function(tolua_S,"send_damage_list",tolua_logic_hld_world_boss_ws_mgr_send_damage_list00);
    tolua_function(tolua_S,"send_strategy_world_boss_info",tolua_logic_hld_world_boss_ws_mgr_send_strategy_world_boss_info00);
    tolua_function(tolua_S,"send_world_boss_info",tolua_logic_hld_world_boss_ws_mgr_send_world_boss_info00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"cross_server_harry_ws_mgr","hld::cross_server_harry_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_harry_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_harry_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_can_join_harry_map",tolua_logic_hld_cross_server_harry_ws_mgr_is_can_join_harry_map00);
    tolua_function(tolua_S,"get_valid_harry_map",tolua_logic_hld_cross_server_harry_ws_mgr_get_valid_harry_map00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"web_client","hld::web_client","",NULL);
   tolua_beginmodule(tolua_S,"web_client");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_web_client_get_instance00);
    tolua_function(tolua_S,"send_activate_code",tolua_logic_hld_web_client_send_activate_code00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"relation_mgr","hld::relation_mgr","",NULL);
   tolua_beginmodule(tolua_S,"relation_mgr");
    tolua_function(tolua_S,"del_relation",tolua_logic_hld_relation_mgr_del_relation00);
    tolua_function(tolua_S,"find_player_to_add_friend",tolua_logic_hld_relation_mgr_find_player_to_add_friend00);
    tolua_function(tolua_S,"find_player_to_add_friend_by_pattern",tolua_logic_hld_relation_mgr_find_player_to_add_friend_by_pattern00);
    tolua_function(tolua_S,"add_relation",tolua_logic_hld_relation_mgr_add_relation00);
    tolua_function(tolua_S,"del_relation_to_dp",tolua_logic_hld_relation_mgr_del_relation_to_dp00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"marry_mgr_ws","hld::marry_mgr_ws","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"ranking_mgr_ws","hld::ranking_mgr_ws","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"cloud_shop_mgr","hld::cloud_shop_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cloud_shop_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cloud_shop_mgr_get_instance00);
    tolua_function(tolua_S,"handler_client_request",tolua_logic_hld_cloud_shop_mgr_handler_client_request00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"gain_treasure_ws_mgr","hld::gain_treasure_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"gain_treasure_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_gain_treasure_ws_mgr_get_instance00);
    tolua_function(tolua_S,"req_kill_record_end",tolua_logic_hld_gain_treasure_ws_mgr_req_kill_record_end00);
    tolua_function(tolua_S,"req_gain_item_record",tolua_logic_hld_gain_treasure_ws_mgr_req_gain_item_record00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"time_limit_activity_ws_mgr","hld::time_limit_activity_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"time_limit_activity_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_time_limit_activity_ws_mgr_get_instance00);
    tolua_function(tolua_S,"get_rank_info",tolua_logic_hld_time_limit_activity_ws_mgr_get_rank_info00);
    tolua_function(tolua_S,"get_activity_common_data_to_gate",tolua_logic_hld_time_limit_activity_ws_mgr_get_activity_common_data_to_gate00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"rename_mgr","hld::rename_mgr","",NULL);
   tolua_beginmodule(tolua_S,"rename_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_rename_mgr_get_instance00);
    tolua_function(tolua_S,"change_player_name",tolua_logic_hld_rename_mgr_change_player_name00);
    tolua_function(tolua_S,"confirm_change_name",tolua_logic_hld_rename_mgr_confirm_change_name00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"cross_server_world_boss_ws_mgr","hld::cross_server_world_boss_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"cross_server_world_boss_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_cross_server_world_boss_ws_mgr_get_instance00);
    tolua_function(tolua_S,"is_in_cross_boss_map",tolua_logic_hld_cross_server_world_boss_ws_mgr_is_in_cross_boss_map00);
    tolua_function(tolua_S,"is_can_join_with_cross",tolua_logic_hld_cross_server_world_boss_ws_mgr_is_can_join_with_cross00);
    tolua_function(tolua_S,"get_world_boss_map_guid",tolua_logic_hld_cross_server_world_boss_ws_mgr_get_world_boss_map_guid00);
    tolua_function(tolua_S,"get_cross_server_world_boss_map_temp_id",tolua_logic_hld_cross_server_world_boss_ws_mgr_get_cross_server_world_boss_map_temp_id00);
    tolua_function(tolua_S,"send_kill_boss_show",tolua_logic_hld_cross_server_world_boss_ws_mgr_send_kill_boss_show00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"assist_fight_mgr","hld::assist_fight_mgr","",NULL);
   tolua_beginmodule(tolua_S,"assist_fight_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_assist_fight_mgr_get_instance00);
    tolua_function(tolua_S,"sync_assist_fight_info",tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info00);
    tolua_function(tolua_S,"sync_assist_fight_info",tolua_logic_hld_assist_fight_mgr_sync_assist_fight_info01);
    tolua_function(tolua_S,"req_assist_fight",tolua_logic_hld_assist_fight_mgr_req_assist_fight00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"appearance_info_mgr","hld::appearance_info_mgr","",NULL);
   tolua_beginmodule(tolua_S,"appearance_info_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_appearance_info_mgr_get_instance00);
    tolua_function(tolua_S,"req_save_appearance_info_to_id",tolua_logic_hld_appearance_info_mgr_req_save_appearance_info_to_id00);
    tolua_function(tolua_S,"req_load_appearance_info_by_id",tolua_logic_hld_appearance_info_mgr_req_load_appearance_info_by_id00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"c_element_war_role_info","hld::c_element_war_role_info","",NULL);
   tolua_beginmodule(tolua_S,"c_element_war_role_info");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"element_war_ws_mgr","hld::element_war_ws_mgr","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_need_server_cross_none",hld::e_need_server_cross_none);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross",hld::e_need_server_cross_begin_cross);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_pk",hld::e_need_server_cross_begin_cross_pk);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_activity",hld::e_need_server_cross_begin_cross_activity);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_harry",hld::e_need_server_cross_begin_cross_harry);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_city_war",hld::e_need_server_cross_begin_cross_city_war);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_legion_territory_war",hld::e_need_server_cross_begin_cross_legion_territory_war);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_boss",hld::e_need_server_cross_begin_cross_boss);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_ladder",hld::e_need_server_cross_begin_cross_ladder);
   tolua_constant(tolua_S,"e_need_server_cross_begin_cross_cloud",hld::e_need_server_cross_begin_cross_cloud);
   tolua_constant(tolua_S,"e_need_server_element_war",hld::e_need_server_element_war);
   tolua_constant(tolua_S,"e_need_server_cross_max",hld::e_need_server_cross_max);
   tolua_cclass(tolua_S,"world_server","hld::world_server","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"client_session_mgr","hld::client_session_mgr","",NULL);
   tolua_beginmodule(tolua_S,"client_session_mgr");
    tolua_function(tolua_S,"send_message_to_all_client_data_lua",tolua_logic_hld_client_session_mgr_send_message_to_all_client_data_lua00);
    tolua_function(tolua_S,"send_message_to_map_lua",tolua_logic_hld_client_session_mgr_send_message_to_map_lua00);
    tolua_function(tolua_S,"get_session_array_used",tolua_logic_hld_client_session_mgr_get_session_array_used00);
    tolua_function(tolua_S,"get_session_by_use_index",tolua_logic_hld_client_session_mgr_get_session_by_use_index00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"s_item_info","hld::s_item_info","",NULL);
   tolua_beginmodule(tolua_S,"s_item_info");
   tolua_endmodule(tolua_S);
   tolua_cclass(tolua_S,"event_ws_mgr","hld::event_ws_mgr","",NULL);
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
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"random_gen","hld::random_gen","",NULL);
   tolua_beginmodule(tolua_S,"random_gen");
    tolua_function(tolua_S,"get_random",tolua_logic_hld_random_gen_get_random00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"big_player_ws_mgr","hld::big_player_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"big_player_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_big_player_ws_mgr_get_instance00);
    tolua_function(tolua_S,"set_big_player_guid",tolua_logic_hld_big_player_ws_mgr_set_big_player_guid00);
    tolua_function(tolua_S,"get_big_player_type_with_ranking_type",tolua_logic_hld_big_player_ws_mgr_get_big_player_type_with_ranking_type00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_function(tolua_S,"lua_cs2ws_send_red_package_process",tolua_logic_hld_lua_cs2ws_send_red_package_process00);
   tolua_function(tolua_S,"lua_cs2ws_get_red_package_process",tolua_logic_hld_lua_cs2ws_get_red_package_process00);
   tolua_function(tolua_S,"lua_cs2ws_get_arena_rank",tolua_logic_hld_lua_cs2ws_get_arena_rank00);
   tolua_function(tolua_S,"lua_cs2ws_req_challenge",tolua_logic_hld_lua_cs2ws_req_challenge00);
   tolua_function(tolua_S,"lua_cs2ws_req_get_choose_list",tolua_logic_hld_lua_cs2ws_req_get_choose_list00);
   tolua_function(tolua_S,"lua_cs2ws_req_get_first_three",tolua_logic_hld_lua_cs2ws_req_get_first_three00);
   tolua_function(tolua_S,"lua_cs2ws_chellenge_over_req",tolua_logic_hld_lua_cs2ws_chellenge_over_req00);
   tolua_function(tolua_S,"lua_cs2ws_arena_sync_gs_value_proc",tolua_logic_hld_lua_cs2ws_arena_sync_gs_value_proc00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"mail_ws_mgr","hld::mail_ws_mgr","",NULL);
   tolua_beginmodule(tolua_S,"mail_ws_mgr");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_mail_ws_mgr_get_instance00);
    tolua_function(tolua_S,"send_mail",tolua_logic_hld_mail_ws_mgr_send_mail00);
    tolua_function(tolua_S,"send_mail_system",tolua_logic_hld_mail_ws_mgr_send_mail_system00);
    tolua_function(tolua_S,"get_free_item_list",tolua_logic_hld_mail_ws_mgr_get_free_item_list00);
    tolua_function(tolua_S,"add_free_item",tolua_logic_hld_mail_ws_mgr_add_free_item00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"EMailInfo_IsNeedDelete",hld::EMailInfo_IsNeedDelete);
   tolua_constant(tolua_S,"EMailInfo_SendTime",hld::EMailInfo_SendTime);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid11",hld::EMailInfo_ItemGuid11);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid12",hld::EMailInfo_ItemGuid12);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid21",hld::EMailInfo_ItemGuid21);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid22",hld::EMailInfo_ItemGuid22);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid31",hld::EMailInfo_ItemGuid31);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid32",hld::EMailInfo_ItemGuid32);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid41",hld::EMailInfo_ItemGuid41);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid42",hld::EMailInfo_ItemGuid42);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid51",hld::EMailInfo_ItemGuid51);
   tolua_constant(tolua_S,"EMailInfo_ItemGuid52",hld::EMailInfo_ItemGuid52);
   tolua_constant(tolua_S,"EMailInfo_MoneyTyp1",hld::EMailInfo_MoneyTyp1);
   tolua_constant(tolua_S,"EMailInfo_MoneyNum1",hld::EMailInfo_MoneyNum1);
   tolua_constant(tolua_S,"EMailInfo_MoneyTyp2",hld::EMailInfo_MoneyTyp2);
   tolua_constant(tolua_S,"EMailInfo_MoneyNum2",hld::EMailInfo_MoneyNum2);
   tolua_constant(tolua_S,"EMailInfo_ReadState",hld::EMailInfo_ReadState);
   tolua_constant(tolua_S,"EMailInfo_SenderGuid1",hld::EMailInfo_SenderGuid1);
   tolua_constant(tolua_S,"EMailInfo_SenderGuid2",hld::EMailInfo_SenderGuid2);
   tolua_constant(tolua_S,"EMailInfo_ServerId",hld::EMailInfo_ServerId);
   tolua_constant(tolua_S,"EMailInfo_max",hld::EMailInfo_max);
   tolua_constant(tolua_S,"e_mail_common_text_pk_king_title",hld::e_mail_common_text_pk_king_title);
   tolua_constant(tolua_S,"e_mail_common_text_pk_king_context",hld::e_mail_common_text_pk_king_context);
   tolua_constant(tolua_S,"e_mail_common_text_broken_sky_title",hld::e_mail_common_text_broken_sky_title);
   tolua_constant(tolua_S,"e_mail_common_text_broken_sky_context",hld::e_mail_common_text_broken_sky_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_title",hld::e_mail_common_text_city_war_title);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_win_context",hld::e_mail_common_text_city_war_win_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_lose_context",hld::e_mail_common_text_city_war_lose_context);
   tolua_constant(tolua_S,"e_mail_common_text_resource_back_title",hld::e_mail_common_text_resource_back_title);
   tolua_constant(tolua_S,"e_mail_common_text_resource_back_context",hld::e_mail_common_text_resource_back_context);
   tolua_constant(tolua_S,"e_mail_common_text_get_remaining_title",hld::e_mail_common_text_get_remaining_title);
   tolua_constant(tolua_S,"e_mail_common_text_get_remaining_context",hld::e_mail_common_text_get_remaining_context);
   tolua_constant(tolua_S,"e_mail_common_text_sender_system",hld::e_mail_common_text_sender_system);
   tolua_constant(tolua_S,"e_mail_common_text_blessing_title",hld::e_mail_common_text_blessing_title);
   tolua_constant(tolua_S,"e_mail_common_text_blessing_context",hld::e_mail_common_text_blessing_context);
   tolua_constant(tolua_S,"e_mail_common_text_auction_purchase_title",hld::e_mail_common_text_auction_purchase_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_purchase_content",hld::e_mail_common_text_auction_purchase_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_seller_title",hld::e_mail_common_text_auction_seller_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_seller_content",hld::e_mail_common_text_auction_seller_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_cancel_sell_title",hld::e_mail_common_text_auction_cancel_sell_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_cancel_sell_content",hld::e_mail_common_text_auction_cancel_sell_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_cancel_sell_bag_full_content",hld::e_mail_common_text_auction_cancel_sell_bag_full_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_reward_share_title",hld::e_mail_common_text_auction_reward_share_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_reward_share_kill_boss_content",hld::e_mail_common_text_auction_reward_share_kill_boss_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_reward_share_activity_content",hld::e_mail_common_text_auction_reward_share_activity_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_bid_failed_title",hld::e_mail_common_text_auction_bid_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_bid_failed_content",hld::e_mail_common_text_auction_bid_failed_content);
   tolua_constant(tolua_S,"e_mail_common_text_auction_sell_failed_title",hld::e_mail_common_text_auction_sell_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_sell_failed_content",hld::e_mail_common_text_auction_sell_failed_content);
   tolua_constant(tolua_S,"e_mail_common_text_force_divorce_title",hld::e_mail_common_text_force_divorce_title);
   tolua_constant(tolua_S,"e_mail_common_text_force_divorce_content",hld::e_mail_common_text_force_divorce_content);
   tolua_constant(tolua_S,"e_mail_common_text_legion_rank_rwd_title",hld::e_mail_common_text_legion_rank_rwd_title);
   tolua_constant(tolua_S,"e_mail_common_text_legion_rank_rwd_content",hld::e_mail_common_text_legion_rank_rwd_content);
   tolua_constant(tolua_S,"e_mail_common_text_active_code_gift_title",hld::e_mail_common_text_active_code_gift_title);
   tolua_constant(tolua_S,"e_mail_common_text_active_code_gift_context",hld::e_mail_common_text_active_code_gift_context);
   tolua_constant(tolua_S,"e_mail_common_text_worship_failed_title",hld::e_mail_common_text_worship_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_worship_failed_context",hld::e_mail_common_text_worship_failed_context);
   tolua_constant(tolua_S,"e_mail_common_text_join_legion_title",hld::e_mail_common_text_join_legion_title);
   tolua_constant(tolua_S,"e_mail_common_text_join_legion_context",hld::e_mail_common_text_join_legion_context);
   tolua_constant(tolua_S,"e_mail_common_text_daily_mission_bag_full_title",hld::e_mail_common_text_daily_mission_bag_full_title);
   tolua_constant(tolua_S,"e_mail_common_text_daily_mission_bag_full_context",hld::e_mail_common_text_daily_mission_bag_full_context);
   tolua_constant(tolua_S,"e_mail_common_text_against_mission_bag_full_title",hld::e_mail_common_text_against_mission_bag_full_title);
   tolua_constant(tolua_S,"e_mail_common_text_against_mission_bag_full_context",hld::e_mail_common_text_against_mission_bag_full_context);
   tolua_constant(tolua_S,"e_mail_common_text_active_recharge_package_title",hld::e_mail_common_text_active_recharge_package_title);
   tolua_constant(tolua_S,"e_mail_common_text_active_recharge_package_context",hld::e_mail_common_text_active_recharge_package_context);
   tolua_constant(tolua_S,"e_mail_common_text_jewel_send_mail_title",hld::e_mail_common_text_jewel_send_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_jewel_send_mail_content",hld::e_mail_common_text_jewel_send_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_arena_reward_mail_title",hld::e_mail_common_text_arena_reward_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_arena_reward_mail_content",hld::e_mail_common_text_arena_reward_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_announcement_title",hld::e_mail_common_text_set_legion_announcement_title);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_announcement_context",hld::e_mail_common_text_set_legion_announcement_context);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_job_title",hld::e_mail_common_text_set_legion_job_title);
   tolua_constant(tolua_S,"e_mail_common_text_set_legion_job_context",hld::e_mail_common_text_set_legion_job_context);
   tolua_constant(tolua_S,"e_mail_common_text_master_city_war_title",hld::e_mail_common_text_master_city_war_title);
   tolua_constant(tolua_S,"e_mail_common_text_master_city_war_context",hld::e_mail_common_text_master_city_war_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_bid_title",hld::e_mail_common_text_city_bid_title);
   tolua_constant(tolua_S,"e_mail_common_text_city_bid_context",hld::e_mail_common_text_city_bid_context);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_basic_award_title",hld::e_mail_common_text_cloud_shop_basic_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_basic_award_context",hld::e_mail_common_text_cloud_shop_basic_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_award_title",hld::e_mail_common_text_cloud_shop_big_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_award_context",hld::e_mail_common_text_cloud_shop_big_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_buy_failed_title",hld::e_mail_common_text_cloud_shop_big_buy_failed_title);
   tolua_constant(tolua_S,"e_mail_common_text_cloud_shop_big_buy_failed_context",hld::e_mail_common_text_cloud_shop_big_buy_failed_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_recharge_title",hld::e_mail_common_text_time_limit_activity_rank_reward_total_recharge_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_recharge_context",hld::e_mail_common_text_time_limit_activity_rank_reward_total_recharge_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_cost_title",hld::e_mail_common_text_time_limit_activity_rank_reward_total_cost_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_total_cost_context",hld::e_mail_common_text_time_limit_activity_rank_reward_total_cost_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_treasure_title",hld::e_mail_common_text_time_limit_activity_rank_reward_treasure_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_treasure_context",hld::e_mail_common_text_time_limit_activity_rank_reward_treasure_context);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_legion_member_title",hld::e_mail_common_text_change_name_inform_legion_member_title);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_legion_member_content",hld::e_mail_common_text_change_name_inform_legion_member_content);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_friend_title",hld::e_mail_common_text_change_name_inform_friend_title);
   tolua_constant(tolua_S,"e_mail_common_text_change_name_inform_friend_content",hld::e_mail_common_text_change_name_inform_friend_content);
   tolua_constant(tolua_S,"e_mail_common_text_service_rank_end_time_reward_title",hld::e_mail_common_text_service_rank_end_time_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_service_rank_end_time_reward_context",hld::e_mail_common_text_service_rank_end_time_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_out_16_award_title",hld::e_mail_common_text_cross_pk_out_16_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_out_16_award_context",hld::e_mail_common_text_cross_pk_out_16_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_16_award_title",hld::e_mail_common_text_cross_pk_16_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_16_award_context",hld::e_mail_common_text_cross_pk_16_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_8_award_title",hld::e_mail_common_text_cross_pk_8_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_8_award_context",hld::e_mail_common_text_cross_pk_8_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_4_award_title",hld::e_mail_common_text_cross_pk_4_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_4_award_context",hld::e_mail_common_text_cross_pk_4_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_2_award_title",hld::e_mail_common_text_cross_pk_2_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_2_award_context",hld::e_mail_common_text_cross_pk_2_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_1_award_title",hld::e_mail_common_text_cross_pk_1_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_pk_1_award_context",hld::e_mail_common_text_cross_pk_1_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_player_award_title",hld::e_mail_common_text_cross_harry_player_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_player_award_context",hld::e_mail_common_text_cross_harry_player_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_server_award_title",hld::e_mail_common_text_cross_harry_server_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_harry_server_award_context",hld::e_mail_common_text_cross_harry_server_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_change_legion_name_inform_legion_member_title",hld::e_mail_common_text_change_legion_name_inform_legion_member_title);
   tolua_constant(tolua_S,"e_mail_common_text_change_legion_name_inform_legion_member_content",hld::e_mail_common_text_change_legion_name_inform_legion_member_content);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_title",hld::e_mail_common_text_cross_city_rank_award_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_context",hld::e_mail_common_text_cross_city_rank_award_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_title_chief",hld::e_mail_common_text_cross_city_rank_award_title_chief);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_rank_award_context_chief",hld::e_mail_common_text_cross_city_rank_award_context_chief);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_refund_title",hld::e_mail_common_text_cross_ladder_refund_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_refund_context",hld::e_mail_common_text_cross_ladder_refund_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_reward_title",hld::e_mail_common_text_cross_ladder_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_reward_context",hld::e_mail_common_text_cross_ladder_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_season_reward_title",hld::e_mail_common_text_cross_ladder_season_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_season_reward_context",hld::e_mail_common_text_cross_ladder_season_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_rank_reward_title",hld::e_mail_common_text_cross_ladder_rank_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_rank_reward_context",hld::e_mail_common_text_cross_ladder_rank_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_skill_bag_full_send_mail_title",hld::e_mail_common_text_skill_bag_full_send_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_skill_bag_full_send_mail_content",hld::e_mail_common_text_skill_bag_full_send_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_war_send_mail_title",hld::e_mail_common_text_cross_city_war_send_mail_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_city_war_send_mail_content",hld::e_mail_common_text_cross_city_war_send_mail_content);
   tolua_constant(tolua_S,"e_mail_common_text_cross_server_terrritory_war_title",hld::e_mail_common_text_cross_server_terrritory_war_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_server_terrritory_war_content",hld::e_mail_common_text_cross_server_terrritory_war_content);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_gs_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_gs_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_gs_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_gs_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_mount_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_mount_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_mount_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_mount_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_wing_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_wing_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_wing_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_wing_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_equip_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_equip_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_equip_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_equip_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_spirit_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_spirit_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_spirit_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_spirit_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_title",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_title);
   tolua_constant(tolua_S,"e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_context",hld::e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_context);
   tolua_constant(tolua_S,"e_mail_common_text_couple_bless_title",hld::e_mail_common_text_couple_bless_title);
   tolua_constant(tolua_S,"e_mail_common_text_couple_bless_context",hld::e_mail_common_text_couple_bless_context);
   tolua_constant(tolua_S,"e_mail_common_text_couple_marry_title",hld::e_mail_common_text_couple_marry_title);
   tolua_constant(tolua_S,"e_mail_common_text_couple_marry_context",hld::e_mail_common_text_couple_marry_context);
   tolua_constant(tolua_S,"e_mail_common_text_couple_divorce_title",hld::e_mail_common_text_couple_divorce_title);
   tolua_constant(tolua_S,"e_mail_common_text_couple_divorce_context",hld::e_mail_common_text_couple_divorce_context);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_title",hld::e_mail_common_text_oracle_trial_funben_over_title);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_content_one",hld::e_mail_common_text_oracle_trial_funben_over_content_one);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_content_two",hld::e_mail_common_text_oracle_trial_funben_over_content_two);
   tolua_constant(tolua_S,"e_mail_common_text_oracle_trial_funben_over_content_three",hld::e_mail_common_text_oracle_trial_funben_over_content_three);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_upgrade_title",hld::e_mail_common_text_cross_ladder_upgrade_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_ladder_upgrade_context",hld::e_mail_common_text_cross_ladder_upgrade_context);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_begin_title",hld::e_mail_common_text_overload_city_war_begin_title);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_begin_context",hld::e_mail_common_text_overload_city_war_begin_context);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_end_title",hld::e_mail_common_text_overload_city_war_end_title);
   tolua_constant(tolua_S,"e_mail_common_text_overload_city_war_end_context",hld::e_mail_common_text_overload_city_war_end_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_begin_title",hld::e_mail_common_text_cross_overload_city_war_begin_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_begin_context",hld::e_mail_common_text_cross_overload_city_war_begin_context);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_end_title",hld::e_mail_common_text_cross_overload_city_war_end_title);
   tolua_constant(tolua_S,"e_mail_common_text_cross_overload_city_war_end_context",hld::e_mail_common_text_cross_overload_city_war_end_context);
   tolua_constant(tolua_S,"e_mail_common_text_city_rank_award_title_chief",hld::e_mail_common_text_city_rank_award_title_chief);
   tolua_constant(tolua_S,"e_mail_common_text_city_rank_award_context_chief",hld::e_mail_common_text_city_rank_award_context_chief);
   tolua_constant(tolua_S,"e_mail_common_overload_city_war_end_title",hld::e_mail_common_overload_city_war_end_title);
   tolua_constant(tolua_S,"e_mail_common_overload_city_war_end_context",hld::e_mail_common_overload_city_war_end_context);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_begin_title",hld::e_mail_common_cross_ladder_season_begin_title);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_begin_context",hld::e_mail_common_cross_ladder_season_begin_context);
   tolua_constant(tolua_S,"e_mail_common_text_auction_time_out_title",hld::e_mail_common_text_auction_time_out_title);
   tolua_constant(tolua_S,"e_mail_common_text_auction_time_out_content",hld::e_mail_common_text_auction_time_out_content);
   tolua_constant(tolua_S,"e_mail_common_text_help_reward_title",hld::e_mail_common_text_help_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_help_reward_content",hld::e_mail_common_text_help_reward_content);
   tolua_constant(tolua_S,"e_mail_common_text_helper_reward_title",hld::e_mail_common_text_helper_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_helper_reward_content",hld::e_mail_common_text_helper_reward_content);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_win_reward_title",hld::e_mail_common_text_city_war_win_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_city_war_win_reward_content",hld::e_mail_common_text_city_war_win_reward_content);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_ranking_reward_title",hld::e_mail_common_cross_ladder_season_ranking_reward_title);
   tolua_constant(tolua_S,"e_mail_common_cross_ladder_season_ranking_reward_context",hld::e_mail_common_cross_ladder_season_ranking_reward_context);
   tolua_constant(tolua_S,"e_mail_common_legion_welfare_reward_title",hld::e_mail_common_legion_welfare_reward_title);
   tolua_constant(tolua_S,"e_mail_common_legion_welfare_reward_context",hld::e_mail_common_legion_welfare_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_season_reward_title",hld::e_mail_common_text_element_war_season_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_season_reward_context",hld::e_mail_common_text_element_war_season_reward_context);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_begin_title",hld::e_mail_common_element_war_season_begin_title);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_begin_context",hld::e_mail_common_element_war_season_begin_context);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_ranking_reward_title",hld::e_mail_common_element_war_season_ranking_reward_title);
   tolua_constant(tolua_S,"e_mail_common_element_war_season_ranking_reward_context",hld::e_mail_common_element_war_season_ranking_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_upgrade_title",hld::e_mail_common_text_element_war_upgrade_title);
   tolua_constant(tolua_S,"e_mail_common_text_element_war_upgrade_context",hld::e_mail_common_text_element_war_upgrade_context);
   tolua_constant(tolua_S,"e_mail_common_text_treasure_clear_title",hld::e_mail_common_text_treasure_clear_title);
   tolua_constant(tolua_S,"e_mail_common_text_treasure_clear_context",hld::e_mail_common_text_treasure_clear_context);
   tolua_constant(tolua_S,"e_mail_common_text_world_boss_strategy_reward_title",hld::e_mail_common_text_world_boss_strategy_reward_title);
   tolua_constant(tolua_S,"e_mail_common_text_world_boss_strategy_reward_context",hld::e_mail_common_text_world_boss_strategy_reward_context);
   tolua_constant(tolua_S,"e_mail_common_text_max",hld::e_mail_common_text_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_cclass(tolua_S,"server_log","hld::server_log","",NULL);
   tolua_beginmodule(tolua_S,"server_log");
    tolua_function(tolua_S,"auction_sell_log",tolua_logic_hld_server_log_auction_sell_log00);
    tolua_function(tolua_S,"auction_db_log",tolua_logic_hld_server_log_auction_db_log00);
    tolua_function(tolua_S,"auctionbid_log",tolua_logic_hld_server_log_auctionbid_log00);
    tolua_function(tolua_S,"serverAuction",tolua_logic_hld_server_log_serverAuction00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_weap_sub_type_none",hld::e_weap_sub_type_none);
   tolua_constant(tolua_S,"e_weap_sub_type_jian",hld::e_weap_sub_type_jian);
   tolua_constant(tolua_S,"e_weap_sub_type_fu",hld::e_weap_sub_type_fu);
   tolua_constant(tolua_S,"e_weap_sub_type_faqi",hld::e_weap_sub_type_faqi);
   tolua_constant(tolua_S,"e_weap_sub_type_duanzhang",hld::e_weap_sub_type_duanzhang);
   tolua_constant(tolua_S,"e_weap_sub_type_shuangdao",hld::e_weap_sub_type_shuangdao);
   tolua_constant(tolua_S,"e_weap_sub_type_shuanggun",hld::e_weap_sub_type_shuanggun);
   tolua_constant(tolua_S,"e_weap_sub_type_jujian",hld::e_weap_sub_type_jujian);
   tolua_constant(tolua_S,"e_weap_sub_type_juchu",hld::e_weap_sub_type_juchu);
   tolua_constant(tolua_S,"e_weap_sub_type_qianggun",hld::e_weap_sub_type_qianggun);
   tolua_constant(tolua_S,"e_weap_sub_type_changzhang",hld::e_weap_sub_type_changzhang);
   tolua_constant(tolua_S,"e_weap_sub_type_jiyue",hld::e_weap_sub_type_jiyue);
   tolua_constant(tolua_S,"e_weap_sub_type_gong",hld::e_weap_sub_type_gong);
   tolua_constant(tolua_S,"e_weap_sub_type_gongjian",hld::e_weap_sub_type_gongjian);
   tolua_constant(tolua_S,"e_weap_sub_type_dao",hld::e_weap_sub_type_dao);
   tolua_constant(tolua_S,"e_weap_sub_type_zhonggong",hld::e_weap_sub_type_zhonggong);
   tolua_constant(tolua_S,"e_weap_sub_type_assistant",hld::e_weap_sub_type_assistant);
   tolua_constant(tolua_S,"e_weap_sub_type_max",hld::e_weap_sub_type_max);
   tolua_constant(tolua_S,"e_item_lock_status_unlocked",hld::e_item_lock_status_unlocked);
   tolua_constant(tolua_S,"e_item_lock_status_locked",hld::e_item_lock_status_locked);
   tolua_constant(tolua_S,"e_item_lock_status_ignore",hld::e_item_lock_status_ignore);
   tolua_constant(tolua_S,"e_item_lock_status_max",hld::e_item_lock_status_max);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_one_free",hld::e_item_get_hope_item_typ_one_free);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_one_voucher",hld::e_item_get_hope_item_typ_one_voucher);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_ten_voucher",hld::e_item_get_hope_item_typ_ten_voucher);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_one_cost",hld::e_item_get_hope_item_typ_one_cost);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_ten_cost",hld::e_item_get_hope_item_typ_ten_cost);
   tolua_constant(tolua_S,"e_item_get_hope_item_typ_max",hld::e_item_get_hope_item_typ_max);
   tolua_constant(tolua_S,"e_quest_sub_type_none",hld::e_quest_sub_type_none);
   tolua_constant(tolua_S,"e_quest_sub_type_unexpendable",hld::e_quest_sub_type_unexpendable);
   tolua_constant(tolua_S,"e_quest_sub_type_expendable",hld::e_quest_sub_type_expendable);
   tolua_constant(tolua_S,"e_quest_sub_type_max",hld::e_quest_sub_type_max);
   tolua_constant(tolua_S,"e_item_color_white",hld::e_item_color_white);
   tolua_constant(tolua_S,"e_item_color_green",hld::e_item_color_green);
   tolua_constant(tolua_S,"e_item_color_blue",hld::e_item_color_blue);
   tolua_constant(tolua_S,"e_item_color_purple",hld::e_item_color_purple);
   tolua_constant(tolua_S,"e_item_color_orange",hld::e_item_color_orange);
   tolua_constant(tolua_S,"e_item_color_red",hld::e_item_color_red);
   tolua_constant(tolua_S,"e_item_color_pink",hld::e_item_color_pink);
   tolua_constant(tolua_S,"e_item_color_max",hld::e_item_color_max);
   tolua_constant(tolua_S,"item_temp_succinct_property_1",hld::item_temp_succinct_property_1);
   tolua_constant(tolua_S,"item_temp_succinct_property_2",hld::item_temp_succinct_property_2);
   tolua_constant(tolua_S,"item_temp_succinct_property_3",hld::item_temp_succinct_property_3);
   tolua_constant(tolua_S,"item_temp_succinct_property_4",hld::item_temp_succinct_property_4);
   tolua_constant(tolua_S,"item_temp_succinct_property_max",hld::item_temp_succinct_property_max);
   tolua_constant(tolua_S,"item_temp_forge_property_1",hld::item_temp_forge_property_1);
   tolua_constant(tolua_S,"item_temp_forge_property_2",hld::item_temp_forge_property_2);
   tolua_constant(tolua_S,"item_temp_forge_property_3",hld::item_temp_forge_property_3);
   tolua_constant(tolua_S,"item_temp_forge_property_4",hld::item_temp_forge_property_4);
   tolua_constant(tolua_S,"item_temp_forge_property_5",hld::item_temp_forge_property_5);
   tolua_constant(tolua_S,"item_temp_forge_property_6",hld::item_temp_forge_property_6);
   tolua_constant(tolua_S,"item_temp_forge_property_max",hld::item_temp_forge_property_max);
   tolua_constant(tolua_S,"item_succinct_money_cost_non",hld::item_succinct_money_cost_non);
   tolua_constant(tolua_S,"item_succinct_money_cost_gold",hld::item_succinct_money_cost_gold);
   tolua_constant(tolua_S,"item_succinct_money_cost_diamond",hld::item_succinct_money_cost_diamond);
   tolua_constant(tolua_S,"item_succinct_change_des_max",hld::item_succinct_change_des_max);
   tolua_constant(tolua_S,"item_succinct_change_des_min",hld::item_succinct_change_des_min);
   tolua_constant(tolua_S,"item_succinct_change_ins_min",hld::item_succinct_change_ins_min);
   tolua_constant(tolua_S,"item_succinct_change_ins_max",hld::item_succinct_change_ins_max);
   tolua_constant(tolua_S,"item_succinct_change_max",hld::item_succinct_change_max);
   tolua_constant(tolua_S,"mount_type_non",hld::mount_type_non);
   tolua_constant(tolua_S,"mount_type_normal",hld::mount_type_normal);
   tolua_constant(tolua_S,"mount_type_special",hld::mount_type_special);
   tolua_constant(tolua_S,"mount_type_warlord",hld::mount_type_warlord);
   tolua_constant(tolua_S,"mount_type_illusion",hld::mount_type_illusion);
   tolua_constant(tolua_S,"mount_type_lordlegionmem",hld::mount_type_lordlegionmem);
   tolua_constant(tolua_S,"e_item_transform_type_none",hld::e_item_transform_type_none);
   tolua_constant(tolua_S,"e_item_transform_type_two",hld::e_item_transform_type_two);
   tolua_constant(tolua_S,"e_item_transform_type_three",hld::e_item_transform_type_three);
   tolua_constant(tolua_S,"e_item_transform_type_four",hld::e_item_transform_type_four);
   tolua_constant(tolua_S,"e_item_transform_type_max",hld::e_item_transform_type_max);
   tolua_constant(tolua_S,"e_item_operate_money_gold",hld::e_item_operate_money_gold);
   tolua_constant(tolua_S,"e_item_operate_money_diamond",hld::e_item_operate_money_diamond);
   tolua_constant(tolua_S,"e_item_unlocked",hld::e_item_unlocked);
   tolua_constant(tolua_S,"e_item_locked",hld::e_item_locked);
   tolua_constant(tolua_S,"e_wing_illusion_originalid_range_low",hld::e_wing_illusion_originalid_range_low);
   tolua_constant(tolua_S,"e_wing_illusion_originalid_range_high",hld::e_wing_illusion_originalid_range_high);
   tolua_constant(tolua_S,"e_wing_illusion_originalid_range_max",hld::e_wing_illusion_originalid_range_max);
   tolua_constant(tolua_S,"e_mount_illusion_originalid_range_low",hld::e_mount_illusion_originalid_range_low);
   tolua_constant(tolua_S,"e_mount_illusion_originalid_range_high",hld::e_mount_illusion_originalid_range_high);
   tolua_constant(tolua_S,"e_mount_illusion_originalid_range_max",hld::e_mount_illusion_originalid_range_max);
   tolua_constant(tolua_S,"e_wing_rush_first",hld::e_wing_rush_first);
   tolua_constant(tolua_S,"e_wing_rush_second",hld::e_wing_rush_second);
   tolua_constant(tolua_S,"e_wing_rush_third",hld::e_wing_rush_third);
   tolua_constant(tolua_S,"e_wing_rush_max",hld::e_wing_rush_max);
   tolua_constant(tolua_S,"e_item_advance_ban",hld::e_item_advance_ban);
   tolua_constant(tolua_S,"e_item_advance_normal",hld::e_item_advance_normal);
   tolua_constant(tolua_S,"e_item_advance_godassembly",hld::e_item_advance_godassembly);
   tolua_constant(tolua_S,"EItemProperty_Level",hld::EItemProperty_Level);
   tolua_constant(tolua_S,"EItemProperty_PropertyID",hld::EItemProperty_PropertyID);
   tolua_constant(tolua_S,"EItemProperty_Points",hld::EItemProperty_Points);
   tolua_constant(tolua_S,"EItemProperty_Percent",hld::EItemProperty_Percent);
   tolua_constant(tolua_S,"EItemProperty_IsShow",hld::EItemProperty_IsShow);
   tolua_constant(tolua_S,"EItemProperty_Max",hld::EItemProperty_Max);
   tolua_constant(tolua_S,"e_luck_draw_record_type_sprite",hld::e_luck_draw_record_type_sprite);
   tolua_constant(tolua_S,"e_luck_draw_record_type_hope_item",hld::e_luck_draw_record_type_hope_item);
   tolua_constant(tolua_S,"e_luck_draw_record_type_max",hld::e_luck_draw_record_type_max);
   tolua_constant(tolua_S,"e_composit_type_none",hld::e_composit_type_none);
   tolua_constant(tolua_S,"e_composit_type_yinji",hld::e_composit_type_yinji);
   tolua_constant(tolua_S,"e_composit_type_chibang",hld::e_composit_type_chibang);
   tolua_constant(tolua_S,"e_composit_type_zhuangbeisuipian",hld::e_composit_type_zhuangbeisuipian);
   tolua_constant(tolua_S,"e_composit_type_zhuangbeicailiao",hld::e_composit_type_zhuangbeicailiao);
   tolua_constant(tolua_S,"e_composit_type_zuoqi",hld::e_composit_type_zuoqi);
   tolua_constant(tolua_S,"e_composit_type_jingling",hld::e_composit_type_jingling);
   tolua_constant(tolua_S,"e_composit_type_chengzhuang",hld::e_composit_type_chengzhuang);
   tolua_constant(tolua_S,"e_composit_type_hongzhuang",hld::e_composit_type_hongzhuang);
   tolua_constant(tolua_S,"e_composit_type_jewel",hld::e_composit_type_jewel);
   tolua_constant(tolua_S,"e_composit_type_item_advance",hld::e_composit_type_item_advance);
   tolua_constant(tolua_S,"e_composit_type_god_assembly",hld::e_composit_type_god_assembly);
   tolua_constant(tolua_S,"e_composit_type_god_assembled",hld::e_composit_type_god_assembled);
   tolua_constant(tolua_S,"e_composit_type_item_cast",hld::e_composit_type_item_cast);
   tolua_constant(tolua_S,"e_composit_type_max",hld::e_composit_type_max);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_module(tolua_S,"cross",0);
   tolua_beginmodule(tolua_S,"cross");
    tolua_function(tolua_S,"send_msg_to_ws",tolua_logic_hld_cross_send_msg_to_ws00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"hld",0);
  tolua_beginmodule(tolua_S,"hld");
   tolua_constant(tolua_S,"e_title_type_title_card",hld::e_title_type_title_card);
   tolua_constant(tolua_S,"e_title_type_kingofpk",hld::e_title_type_kingofpk);
   tolua_constant(tolua_S,"e_title_type_kingofgods",hld::e_title_type_kingofgods);
   tolua_constant(tolua_S,"e_title_type_legion_city_win_chief",hld::e_title_type_legion_city_win_chief);
   tolua_constant(tolua_S,"e_title_type_legion_city_win_member",hld::e_title_type_legion_city_win_member);
   tolua_constant(tolua_S,"e_title_type_capture_point_chief",hld::e_title_type_capture_point_chief);
   tolua_constant(tolua_S,"e_title_type_capture_point_member",hld::e_title_type_capture_point_member);
   tolua_constant(tolua_S,"e_title_type_login_day_num",hld::e_title_type_login_day_num);
   tolua_constant(tolua_S,"e_title_type_level",hld::e_title_type_level);
   tolua_constant(tolua_S,"e_title_type_kill_player_num",hld::e_title_type_kill_player_num);
   tolua_constant(tolua_S,"e_title_type_arena_rank_first",hld::e_title_type_arena_rank_first);
   tolua_constant(tolua_S,"e_title_type_worship_rank_first",hld::e_title_type_worship_rank_first);
   tolua_constant(tolua_S,"e_title_type_gs_rank_first",hld::e_title_type_gs_rank_first);
   tolua_constant(tolua_S,"e_title_type_money_rank_first",hld::e_title_type_money_rank_first);
   tolua_constant(tolua_S,"e_title_type_first_clearance_heard_map",hld::e_title_type_first_clearance_heard_map);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_barserker",hld::e_title_type_gs_ranking_barserker);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_wizard",hld::e_title_type_gs_ranking_wizard);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_guardian",hld::e_title_type_gs_ranking_guardian);
   tolua_constant(tolua_S,"e_title_type_gs_ranking_assassinator",hld::e_title_type_gs_ranking_assassinator);
   tolua_constant(tolua_S,"e_title_type_vip",hld::e_title_type_vip);
   tolua_constant(tolua_S,"e_title_type_internal_test",hld::e_title_type_internal_test);
   tolua_constant(tolua_S,"e_title_type_best_barserker",hld::e_title_type_best_barserker);
   tolua_constant(tolua_S,"e_title_type_best_wizard",hld::e_title_type_best_wizard);
   tolua_constant(tolua_S,"e_title_type_best_guardian",hld::e_title_type_best_guardian);
   tolua_constant(tolua_S,"e_title_type_best_assassinator",hld::e_title_type_best_assassinator);
   tolua_constant(tolua_S,"e_title_type_marry_times",hld::e_title_type_marry_times);
   tolua_constant(tolua_S,"e_title_type_marry_divorce_times",hld::e_title_type_marry_divorce_times);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_one",hld::e_title_type_cross_ladder_one);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_two",hld::e_title_type_cross_ladder_two);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_three",hld::e_title_type_cross_ladder_three);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_four",hld::e_title_type_cross_ladder_four);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_five",hld::e_title_type_cross_ladder_five);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_six",hld::e_title_type_cross_ladder_six);
   tolua_constant(tolua_S,"e_title_type_cross_ladder_ranking",hld::e_title_type_cross_ladder_ranking);
   tolua_constant(tolua_S,"e_title_type_pk_king_kill_num",hld::e_title_type_pk_king_kill_num);
   tolua_constant(tolua_S,"e_title_type_element_war_one",hld::e_title_type_element_war_one);
   tolua_constant(tolua_S,"e_title_type_element_war_two",hld::e_title_type_element_war_two);
   tolua_constant(tolua_S,"e_title_type_element_war_three",hld::e_title_type_element_war_three);
   tolua_constant(tolua_S,"e_title_type_element_war_ranking",hld::e_title_type_element_war_ranking);
   tolua_constant(tolua_S,"e_title_type_max",hld::e_title_type_max);
   tolua_constant(tolua_S,"e_title_add_result_type_none",hld::e_title_add_result_type_none);
   tolua_constant(tolua_S,"e_title_add_result_type_normal",hld::e_title_add_result_type_normal);
   tolua_constant(tolua_S,"e_title_add_result_type_add_time",hld::e_title_add_result_type_add_time);
   tolua_constant(tolua_S,"e_title_add_result_type_cannot_use_again",hld::e_title_add_result_type_cannot_use_again);
   tolua_constant(tolua_S,"e_title_add_result_type_max",hld::e_title_add_result_type_max);
   tolua_constant(tolua_S,"e_title_operate_type_equip_on",hld::e_title_operate_type_equip_on);
   tolua_constant(tolua_S,"e_title_operate_type_equip_off",hld::e_title_operate_type_equip_off);
   tolua_constant(tolua_S,"e_title_operate_type_max",hld::e_title_operate_type_max);
   tolua_constant(tolua_S,"e_title_end_time_type_forever",hld::e_title_end_time_type_forever);
   tolua_constant(tolua_S,"e_title_end_time_type_time",hld::e_title_end_time_type_time);
   tolua_constant(tolua_S,"e_title_end_time_type_condition1",hld::e_title_end_time_type_condition1);
   tolua_constant(tolua_S,"e_title_end_time_type_condition2",hld::e_title_end_time_type_condition2);
   tolua_constant(tolua_S,"e_title_special_handle_none",hld::e_title_special_handle_none);
   tolua_constant(tolua_S,"e_title_special_handle_add",hld::e_title_special_handle_add);
   tolua_constant(tolua_S,"e_title_special_handle_remove",hld::e_title_special_handle_remove);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_logic (lua_State* tolua_S) {
 return tolua_logic_open(tolua_S);
};
#endif

