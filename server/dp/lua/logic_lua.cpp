/*
** Lua binding: logic
** Generated automatically by tolua++-1.0.93-lua53 on Tue Aug 27 09:42:06 2024.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_logic_open (lua_State* tolua_S);

#include "lua_call_handle.h"
#include "dbproxy_service.hpp"
#include "msgproc/ws/msgproc_ranking_ws.hpp"
#include "logic/game_enum_def_s.h"
#include "utility/init_unit.h"
#include "utility/globle_data.h"
#include "msgproc/ws/msgproc_ws.hpp"
#include "msgproc/ws/msgproc_relation.hpp"
#include "msgproc/ws/msgproc_mail_ws.hpp"
#include "msgproc/ws/msgproc_legion_ws.hpp"
#include "msgproc/cs/msgproc_cs.hpp"
#include "msgproc/cs/msgproc_recycle.h"
#include "msgproc/cs/msgproc_phantom.hpp"
#include "msgproc/cs/msgproc_chat.hpp"
#include "msgproc/cs/msgproc_oracle_trial.hpp"
#include "msgproc/cs/msgproc_skill.hpp"
#include "msgproc/cs/msgproc_role_harry_info.hpp"
#include "msgproc/cs/msgproc_boss_island.hpp"
#include "msgproc/cs/msgproc_seven_day_goal.hpp"
#include "msgproc/cs/msgproc_cs_time_limit_activity.hpp"
#include "msgproc/cs/msgproc_gain_treasure.hpp"
#include "msgproc/cs/msgproc_service_rank.hpp"
#include "msgproc/cs/msgproc_highest_record.hpp"
#include "msgproc/cs/msgproc_must_do.hpp"
#include "msgproc/cs/msgproc_talent.hpp"
#include "msgproc/cs/msgproc_title.hpp"
#include "msgproc/cs/msgproc_welfare.hpp"
#include "msgproc/cs/msgproc_convert.hpp"
#include "msgproc/cs/msgproc_worship_target.hpp"
#include "msgproc/cs/msgproc_pokedex.hpp"
#include "msgproc/cs/msgproc_achievement.hpp"
#include "msgproc/cs/msgproc_special_name.hpp"
#include "msgproc/cs/msgproc_service_goal.hpp"
#include "msgproc/cs/msgproc_map.hpp"
#include "msgproc/cs/msgproc_goods.hpp"
#include "msgproc/cs/msgproc_mission.hpp"
#include "msgproc/cs/msgproc_money.hpp"
#include "msgproc/cs/msgproc_att.hpp"
#include <type_def.hpp>
#include "data_manager/data_manager.hpp"
#include "data_manager/data_set.hpp"
#include "msgproc/ws/msgproc_best_record_ws.hpp"
#include "msgproc/ws/msgproc_world_boss_ws.hpp"
#include "msgproc/ws/msgproc_cross_server_pk_ws.hpp"
#include "msgproc/ws/msgproc_cross_server_harry_ws.hpp"
#include "msgproc/ws/msgproc_role_pk_ws.hpp"
#include "msgproc/ws/msgproc_red_package_ws.hpp"
#include "msgproc/ws/msgproc_lucky_draw_record_ws.hpp"
#include "msgproc/ws/msgproc_big_player_ws.hpp"
#include "msgproc/ws/msgproc_auction_ws.hpp"
using namespace faith;

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_hld__c_mail_info (lua_State* tolua_S)
{
 faith::c_mail_info* self = (faith::c_mail_info*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_hld__guid_64 (lua_State* tolua_S)
{
 faith::guid_64* self = (faith::guid_64*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"faith::dbproxy_service");
 Mtolua_typeid(tolua_S,typeid(faith::dbproxy_service), "faith::dbproxy_service");
 tolua_usertype(tolua_S,"faith::init_unit");
 Mtolua_typeid(tolua_S,typeid(faith::init_unit), "faith::init_unit");
 tolua_usertype(tolua_S,"faith::data_manager");
 Mtolua_typeid(tolua_S,typeid(faith::data_manager), "faith::data_manager");
 tolua_usertype(tolua_S,"s_item_info");
 Mtolua_typeid(tolua_S,typeid(s_item_info), "s_item_info");
 tolua_usertype(tolua_S,"faith::c_mail_info");
 Mtolua_typeid(tolua_S,typeid(faith::c_mail_info), "faith::c_mail_info");
 tolua_usertype(tolua_S,"faith::guid_64");
 Mtolua_typeid(tolua_S,typeid(faith::guid_64), "faith::guid_64");
 tolua_usertype(tolua_S,"faith::globle_data");
 Mtolua_typeid(tolua_S,typeid(faith::globle_data), "faith::globle_data");
 tolua_usertype(tolua_S,"faith::data_set");
 Mtolua_typeid(tolua_S,typeid(faith::data_set), "faith::data_set");
}

/* method: getInstance of class  faith::dbproxy_service */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dbproxy_service_getInstance00
static int tolua_logic_hld_dbproxy_service_getInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::dbproxy_service",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::dbproxy_service& tolua_ret = (faith::dbproxy_service&)  faith::dbproxy_service::getInstance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::dbproxy_service");
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

/* method: send_message_lua of class  faith::dbproxy_service */
#ifndef TOLUA_DISABLE_tolua_logic_hld_dbproxy_service_send_message_lua00
static int tolua_logic_hld_dbproxy_service_send_message_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::dbproxy_service",0,&tolua_err) ||
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
  faith::dbproxy_service* self = (faith::dbproxy_service*)  tolua_tousertype(tolua_S,1,0);
  int connindex = ((int)  tolua_tointeger(tolua_S,2,0));
  const char* msg = ((const char*)  tolua_tostring(tolua_S,3,0));
  int msg_len = ((int)  tolua_tointeger(tolua_S,4,0));
  int header = ((int)  tolua_tointeger(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'send_message_lua'", NULL);
#endif
  {
   self->send_message_lua(connindex,msg,msg_len,header);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'send_message_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_arena_rank_first_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_arena_rank_first_lua00
static int tolua_logic_hld_save_arena_rank_first_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,10,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,11,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,12,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,13,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,14,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,15,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,16,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,17,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,18,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,19,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,20,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int t_rank_pos = ((int)  tolua_tointeger(tolua_S,2,0));
  long long t_role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  std::string t_role_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  long long t_gs_value = ((long long)  tolua_tointeger(tolua_S,5,0));
  int t_role_temp_id = ((int)  tolua_tointeger(tolua_S,6,0));
  int t_robot_temp_id = ((int)  tolua_tointeger(tolua_S,7,0));
  int t_military_rank_level = ((int)  tolua_tointeger(tolua_S,8,0));
  int t_vip_level = ((int)  tolua_tointeger(tolua_S,9,0));
  std::string t_game_channel = ((std::string)  tolua_tocppstring(tolua_S,10,0));
  int n_rank_pos = ((int)  tolua_tointeger(tolua_S,11,0));
  long long n_role_guid = ((long long)  tolua_tointeger(tolua_S,12,0));
  std::string n_role_name = ((std::string)  tolua_tocppstring(tolua_S,13,0));
  long long n_gs_value = ((long long)  tolua_tointeger(tolua_S,14,0));
  int n_role_temp_id = ((int)  tolua_tointeger(tolua_S,15,0));
  int n_robot_temp_id = ((int)  tolua_tointeger(tolua_S,16,0));
  int n_military_rank_level = ((int)  tolua_tointeger(tolua_S,17,0));
  int n_vip_level = ((int)  tolua_tointeger(tolua_S,18,0));
  std::string n_game_channel = ((std::string)  tolua_tocppstring(tolua_S,19,0));
  {
   faith::save_arena_rank_first_lua(connindex,t_rank_pos,t_role_guid,t_role_name,t_gs_value,t_role_temp_id,t_robot_temp_id,t_military_rank_level,t_vip_level,t_game_channel,n_rank_pos,n_role_guid,n_role_name,n_gs_value,n_role_temp_id,n_robot_temp_id,n_military_rank_level,n_vip_level,n_game_channel);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_arena_rank_first_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_all_ranking_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_all_ranking_info_lua00
static int tolua_logic_hld_load_all_ranking_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int ranking_type = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::load_all_ranking_info_lua(connindex,ranking_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_all_ranking_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_ranking_info_one_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_ranking_info_one_lua00
static int tolua_logic_hld_save_ranking_info_one_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,11,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,12,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,13,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,14,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  long long ranking_type = ((long long)  tolua_tointeger(tolua_S,4,0));
  long long ranking_value = ((long long)  tolua_tointeger(tolua_S,5,0));
  long long save_time = ((long long)  tolua_tointeger(tolua_S,6,0));
  long long supporting_guid = ((long long)  tolua_tointeger(tolua_S,7,0));
  std::string supporting_name = ((std::string)  tolua_tocppstring(tolua_S,8,0));
  int vip_level = ((int)  tolua_tointeger(tolua_S,9,0));
  int peak = ((int)  tolua_tointeger(tolua_S,10,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,11,0));
  std::string game_channel = ((std::string)  tolua_tocppstring(tolua_S,12,0));
  long long standby_parameter = ((long long)  tolua_tointeger(tolua_S,13,0));
  {
   faith::save_ranking_info_one_lua(connindex,role_guid,role_name,ranking_type,ranking_value,save_time,supporting_guid,supporting_name,vip_level,peak,server_id,game_channel,standby_parameter);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_ranking_info_one_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_all_service_rank_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_all_service_rank_info_lua00
static int tolua_logic_hld_load_all_service_rank_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int rank_type = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::load_all_service_rank_info_lua(connindex,rank_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_all_service_rank_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_ranking_worship_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_ranking_worship_lua00
static int tolua_logic_hld_save_ranking_worship_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,11,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,12,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,13,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,14,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,15,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,16,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,17,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long sender_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  long long ranking_type = ((long long)  tolua_tointeger(tolua_S,5,0));
  long long ranking_value = ((long long)  tolua_tointeger(tolua_S,6,0));
  long long save_time = ((long long)  tolua_tointeger(tolua_S,7,0));
  long long supporting_guid = ((long long)  tolua_tointeger(tolua_S,8,0));
  std::string supporting_name = ((std::string)  tolua_tocppstring(tolua_S,9,0));
  int vip_level = ((int)  tolua_tointeger(tolua_S,10,0));
  int peak = ((int)  tolua_tointeger(tolua_S,11,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,12,0));
  std::string game_channel = ((std::string)  tolua_tocppstring(tolua_S,13,0));
  long long standby_parameter = ((long long)  tolua_tointeger(tolua_S,14,0));
  long long worship_add_value = ((long long)  tolua_tointeger(tolua_S,15,0));
  int worship_type = ((int)  tolua_tointeger(tolua_S,16,0));
  {
   faith::save_ranking_worship_lua(connindex,sender_guid,role_guid,role_name,ranking_type,ranking_value,save_time,supporting_guid,supporting_name,vip_level,peak,server_id,game_channel,standby_parameter,worship_add_value,worship_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_ranking_worship_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_player_worship_value_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_player_worship_value_lua00
static int tolua_logic_hld_load_player_worship_value_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long sender = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long target = ((long long)  tolua_tointeger(tolua_S,3,0));
  {
   faith::load_player_worship_value_lua(connindex,sender,target);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_player_worship_value_lua'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,12,"const s_item_info",0,&tolua_err) ||
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
  const s_item_info* item_list = ((const s_item_info*)  tolua_tousertype(tolua_S,12,0));
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
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const s_item_info",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::globle_data* self = (faith::globle_data*)  tolua_tousertype(tolua_S,1,0);
  const s_item_info* temp_item_data = ((const s_item_info*)  tolua_tousertype(tolua_S,2,0));
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
   tolua_pushusertype(tolua_S,(void*)self->item_list,"const s_item_info");
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

/* function: faith::gm_set_open_time_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_set_open_time_lua00
static int tolua_logic_hld_gm_set_open_time_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int open_time = ((int)  tolua_tointeger(tolua_S,2,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,3,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,4,0));
  {
   faith::gm_set_open_time_lua(connindex,open_time,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_set_open_time_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_set_cross_open_time_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_set_cross_open_time_lua00
static int tolua_logic_hld_gm_set_cross_open_time_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_info_value = ((int)  tolua_tointeger(tolua_S,2,0));
  int server_info_type = ((int)  tolua_tointeger(tolua_S,3,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,4,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,5,0));
  {
   faith::gm_set_cross_open_time_lua(connindex,server_info_value,server_info_type,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_set_cross_open_time_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_set_new_password_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_set_new_password_lua00
static int tolua_logic_hld_gm_set_new_password_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  std::string account = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string new_password = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,4,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,5,0));
  {
   faith::gm_set_new_password_lua(connindex,account,new_password,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_set_new_password_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_set_begin_cross_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_set_begin_cross_lua00
static int tolua_logic_hld_gm_set_begin_cross_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int need_begin_cross = ((int)  tolua_tointeger(tolua_S,2,0));
  int gm_type = ((int)  tolua_tointeger(tolua_S,3,0));
  long long req_http_id = ((long long)  tolua_tointeger(tolua_S,4,0));
  {
   faith::gm_set_begin_cross_lua(connindex,need_begin_cross,gm_type,req_http_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_set_begin_cross_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_server_last_server_list_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_server_last_server_list_lua00
static int tolua_logic_hld_save_server_last_server_list_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  std::string last_server_list = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   faith::save_server_last_server_list_lua(connindex,server_id,last_server_list);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_server_last_server_list_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_server_last_server_list_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_server_last_server_list_lua00
static int tolua_logic_hld_load_server_last_server_list_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::load_server_last_server_list_lua(connindex,server_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_server_last_server_list_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_set_role_right_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_set_role_right_lua00
static int tolua_logic_hld_gm_set_role_right_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  std::string role_mark = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int right_type = ((int)  tolua_tointeger(tolua_S,3,0));
  int right_value = ((int)  tolua_tointeger(tolua_S,4,0));
  int right_time = ((int)  tolua_tointeger(tolua_S,5,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,6,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,7,0));
  {
   faith::gm_set_role_right_lua(connindex,role_mark,right_type,right_value,right_time,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_set_role_right_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_change_account_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_change_account_lua00
static int tolua_logic_hld_gm_change_account_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  std::string change_name = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  std::string target_name = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,4,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,5,0));
  {
   faith::gm_change_account_lua(connindex,change_name,target_name,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_change_account_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_del_role_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_del_role_cache_lua00
static int tolua_logic_hld_gm_del_role_cache_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::gm_del_role_cache_lua(connindex,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_del_role_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_del_role_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_del_role_lua00
static int tolua_logic_hld_gm_del_role_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int is_del = ((int)  tolua_tointeger(tolua_S,3,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,4,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,5,0));
  {
   faith::gm_del_role_lua(connindex,role_guid,is_del,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_del_role_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_stop_login_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_stop_login_lua00
static int tolua_logic_hld_gm_stop_login_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int is_stop = ((int)  tolua_tointeger(tolua_S,2,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,3,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,4,0));
  {
   faith::gm_stop_login_lua(connindex,is_stop,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_stop_login_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::get_char_info_ws_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_get_char_info_ws_lua00
static int tolua_logic_hld_get_char_info_ws_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  std::string role_account = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   faith::get_char_info_ws_lua(connindex,role_guid,role_account);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_char_info_ws_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_get_role_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_get_role_info_lua00
static int tolua_logic_hld_gm_get_role_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  std::string role_mark = ((std::string)  tolua_tocppstring(tolua_S,2,0));
  int get_type = ((int)  tolua_tointeger(tolua_S,3,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,4,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,5,0));
  int check_type = ((int)  tolua_tointeger(tolua_S,6,0));
  {
   faith::gm_get_role_info_lua(connindex,role_mark,get_type,listen_port,handle_index,check_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_get_role_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_modify_role_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_modify_role_info_lua00
static int tolua_logic_hld_gm_modify_role_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  std::string sql_str = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,4,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,5,0));
  {
   faith::gm_modify_role_info_lua(connindex,role_guid,sql_str,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_modify_role_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::gm_get_role_items_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_gm_get_role_items_info_lua00
static int tolua_logic_hld_gm_get_role_items_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int listen_port = ((int)  tolua_tointeger(tolua_S,3,0));
  long long handle_index = ((long long)  tolua_tointeger(tolua_S,4,0));
  {
   faith::gm_get_role_items_info_lua(connindex,role_guid,listen_port,handle_index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'gm_get_role_items_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_server_attr_val_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_server_attr_val_lua00
static int tolua_logic_hld_save_server_attr_val_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int attr_type = ((int)  tolua_tointeger(tolua_S,2,0));
  long long attr_value = ((long long)  tolua_tointeger(tolua_S,3,0));
  {
   faith::save_server_attr_val_lua(connindex,attr_type,attr_value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_server_attr_val_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_server_attr_val_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_server_attr_val_lua00
static int tolua_logic_hld_load_server_attr_val_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_server_attr_val_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_server_attr_val_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::ws2dp_req_load_offline_other_relation_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws2dp_req_load_offline_other_relation_lua00
static int tolua_logic_hld_ws2dp_req_load_offline_other_relation_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long sender_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int list_type = ((int)  tolua_tointeger(tolua_S,3,0));
  long long other_guid = ((long long)  tolua_tointeger(tolua_S,4,0));
  {
   faith::ws2dp_req_load_offline_other_relation_lua(connindex,sender_guid,list_type,other_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ws2dp_req_load_offline_other_relation_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::ws2dp_req_load_player_relation_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws2dp_req_load_player_relation_lua00
static int tolua_logic_hld_ws2dp_req_load_player_relation_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long sender_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::ws2dp_req_load_player_relation_lua(connindex,sender_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ws2dp_req_load_player_relation_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::ws2dp_req_add_relation_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws2dp_req_add_relation_lua00
static int tolua_logic_hld_ws2dp_req_add_relation_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_istable(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long relation_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  std::string relation_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  long long relation_gs_value = ((long long)  tolua_tointeger(tolua_S,5,0));
  int data_array[8];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,6,8,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<8;i++)
    data_array[i] = ((int)  tolua_tofieldinteger(tolua_S,6,i+1,0));
   }
  }
  {
   faith::ws2dp_req_add_relation_lua(connindex,role_guid,relation_guid,relation_name,relation_gs_value,data_array);
  }
  {
   int i;
   for(i=0; i<8;i++)
    tolua_pushfieldinteger(tolua_S,6,i+1,(lua_Integer) data_array[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ws2dp_req_add_relation_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::ws2dp_req_del_relation_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_ws2dp_req_del_relation_lua00
static int tolua_logic_hld_ws2dp_req_del_relation_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long target_role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  int relation_type = ((int)  tolua_tointeger(tolua_S,4,0));
  {
   faith::ws2dp_req_del_relation_lua(connindex,role_guid,target_role_guid,relation_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ws2dp_req_del_relation_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::check_id_mail_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_check_id_mail_proc_lua00
static int tolua_logic_hld_check_id_mail_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,3,0));
  int cur_time = ((int)  tolua_tointeger(tolua_S,4,0));
  {
   faith::check_id_mail_proc_lua(connindex,role_guid,server_id,cur_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'check_id_mail_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_other_server_mail_record_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_other_server_mail_record_lua00
static int tolua_logic_hld_save_other_server_mail_record_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int source_server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  int target_server_id = ((int)  tolua_tointeger(tolua_S,3,0));
  long long mail_guid = ((long long)  tolua_tointeger(tolua_S,4,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,5,0));
  {
   faith::save_other_server_mail_record_lua(connindex,source_server_id,target_server_id,mail_guid,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_other_server_mail_record_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_other_server_mail_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_other_server_mail_lua00
static int tolua_logic_hld_load_other_server_mail_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  long long mail_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  {
   faith::load_other_server_mail_lua(connindex,server_id,mail_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_other_server_mail_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::del_other_server_mail_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_del_other_server_mail_lua00
static int tolua_logic_hld_del_other_server_mail_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int source_server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  int target_server_id = ((int)  tolua_tointeger(tolua_S,3,0));
  long long mail_guid = ((long long)  tolua_tointeger(tolua_S,4,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,5,0));
  {
   faith::del_other_server_mail_lua(connindex,source_server_id,target_server_id,mail_guid,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_other_server_mail_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_globel_mail_to_sql_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_globel_mail_to_sql_lua00
static int tolua_logic_hld_save_globel_mail_to_sql_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::save_globel_mail_to_sql_lua(connindex,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_globel_mail_to_sql_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_globel_mail_item_to_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_globel_mail_item_to_lua00
static int tolua_logic_hld_save_globel_mail_item_to_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::save_globel_mail_item_to_lua(connindex,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_globel_mail_item_to_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_mail_event_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_mail_event_lua00
static int tolua_logic_hld_load_mail_event_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  bool is_over = ((bool)  tolua_toboolean(tolua_S,2,0));
  {
   faith::load_mail_event_lua(connindex,is_over);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_mail_event_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_mail_event_item_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_mail_event_item_lua00
static int tolua_logic_hld_load_mail_event_item_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  bool is_over = ((bool)  tolua_toboolean(tolua_S,2,0));
  {
   faith::load_mail_event_item_lua(connindex,is_over);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_mail_event_item_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_globel_mail_new_get_info_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_globel_mail_new_get_info_proc_lua00
static int tolua_logic_hld_save_globel_mail_new_get_info_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int64 data_info[30];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,3,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    data_info[i] = ((int64)  tolua_tofieldinteger(tolua_S,3,i+1,0));
   }
  }
  {
   faith::save_globel_mail_new_get_info_proc_lua(connindex,role_guid,data_info);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,3,i+1,(lua_Integer) data_info[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_globel_mail_new_get_info_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_legion_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_legion_info_lua00
static int tolua_logic_hld_save_legion_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_istable(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  std::string legion_name = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string announcement = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  long long gs_value = ((long long)  tolua_tointeger(tolua_S,5,0));
  long long data_ary[16];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,6,16,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<16;i++)
    data_ary[i] = ((long)  tolua_tofieldinteger(tolua_S,6,i+1,0));
   }
  }
  {
   faith::save_legion_info_lua(connindex,legion_guid,legion_name,announcement,gs_value,data_ary);
  }
  {
   int i;
   for(i=0; i<16;i++)
    tolua_pushfieldinteger(tolua_S,6,i+1,(lua_Integer) data_ary[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_legion_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_legion_member_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_legion_member_info_lua00
static int tolua_logic_hld_save_legion_member_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_istable(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  long long gs_value = ((long long)  tolua_tointeger(tolua_S,5,0));
  long long data_ary[16];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,6,16,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<16;i++)
    data_ary[i] = ((long)  tolua_tofieldinteger(tolua_S,6,i+1,0));
   }
  }
  {
   faith::save_legion_member_info_lua(connindex,legion_guid,role_guid,role_name,gs_value,data_ary);
  }
  {
   int i;
   for(i=0; i<16;i++)
    tolua_pushfieldinteger(tolua_S,6,i+1,(lua_Integer) data_ary[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_legion_member_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_legion_city_war_member_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_legion_city_war_member_info_lua00
static int tolua_logic_hld_save_legion_city_war_member_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  long long use_type = ((long long)  tolua_tointeger(tolua_S,4,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  long long player_temp_id = ((long long)  tolua_tointeger(tolua_S,6,0));
  long long week_contribution = ((long long)  tolua_tointeger(tolua_S,7,0));
  long long job_title = ((long long)  tolua_tointeger(tolua_S,8,0));
  {
   faith::save_legion_city_war_member_info_lua(connindex,legion_guid,role_guid,use_type,role_name,player_temp_id,week_contribution,job_title);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_legion_city_war_member_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::delete_legion_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_delete_legion_info_lua00
static int tolua_logic_hld_delete_legion_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::delete_legion_info_lua(connindex,legion_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_legion_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::delete_legion_member_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_delete_legion_member_info_lua00
static int tolua_logic_hld_delete_legion_member_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::delete_legion_member_info_lua(connindex,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_legion_member_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::delete_legion_city_war_member_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_delete_legion_city_war_member_info_lua00
static int tolua_logic_hld_delete_legion_city_war_member_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int del_type = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::delete_legion_city_war_member_info_lua(connindex,del_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_legion_city_war_member_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::delete_legion_applicant_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_delete_legion_applicant_lua00
static int tolua_logic_hld_delete_legion_applicant_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::delete_legion_applicant_lua(connindex,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete_legion_applicant_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_legion_applicant_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_legion_applicant_lua00
static int tolua_logic_hld_save_legion_applicant_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_istable(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  float exist_time = ((float)  tolua_tonumber(tolua_S,5,0));
  long long gs_value = ((long long)  tolua_tointeger(tolua_S,6,0));
  int data_ary[4];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,7,4,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<4;i++)
    data_ary[i] = ((int)  tolua_tofieldinteger(tolua_S,7,i+1,0));
   }
  }
  {
   faith::save_legion_applicant_lua(connindex,legion_guid,role_guid,role_name,exist_time,gs_value,data_ary);
  }
  {
   int i;
   for(i=0; i<4;i++)
    tolua_pushfieldinteger(tolua_S,7,i+1,(lua_Integer) data_ary[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_legion_applicant_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_legion_boss_award_get_log_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_legion_boss_award_get_log_lua00
static int tolua_logic_hld_save_legion_boss_award_get_log_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int boss_map_id = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::save_legion_boss_award_get_log_lua(connindex,role_guid,boss_map_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_legion_boss_award_get_log_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_all_legion_boss_award_get_log_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_all_legion_boss_award_get_log_lua00
static int tolua_logic_hld_load_all_legion_boss_award_get_log_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::load_all_legion_boss_award_get_log_lua(connindex,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_all_legion_boss_award_get_log_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_city_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_city_info_lua00
static int tolua_logic_hld_save_city_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long occupation_legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  std::string city_master_name = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  {
   faith::save_city_info_lua(connindex,occupation_legion_guid,city_master_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_city_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_war_bid_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_war_bid_info_lua00
static int tolua_logic_hld_save_war_bid_info_lua00(lua_State* tolua_S)
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
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,9,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,10,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int territory_id = ((int)  tolua_tointeger(tolua_S,2,0));
  int index_in_terr = ((int)  tolua_tointeger(tolua_S,3,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,4,0));
  int cur_bid_price = ((int)  tolua_tointeger(tolua_S,5,0));
  int is_prior_maintain = ((int)  tolua_tointeger(tolua_S,6,0));
  int occupy_time_stamp = ((int)  tolua_tointeger(tolua_S,7,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,8,0));
  std::string legion_name = ((std::string)  tolua_tocppstring(tolua_S,9,0));
  {
   faith::save_war_bid_info_lua(connindex,territory_id,index_in_terr,legion_guid,cur_bid_price,is_prior_maintain,occupy_time_stamp,server_id,legion_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_war_bid_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::clear_war_bid_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_clear_war_bid_info_lua00
static int tolua_logic_hld_clear_war_bid_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int clear_type = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::clear_war_bid_info_lua(connindex,clear_type);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_war_bid_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::clear_city_war_reward_stuff_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_clear_city_war_reward_stuff_lua00
static int tolua_logic_hld_clear_city_war_reward_stuff_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int del_stuff_type = ((int)  tolua_tointeger(tolua_S,2,0));
  long long target_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  {
   faith::clear_city_war_reward_stuff_lua(connindex,del_stuff_type,target_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_city_war_reward_stuff_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_legion_event_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_legion_event_lua00
static int tolua_logic_hld_save_legion_event_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int event_idx = ((int)  tolua_tointeger(tolua_S,3,0));
  std::string extra_data = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  int happen_time = ((int)  tolua_tointeger(tolua_S,5,0));
  {
   faith::save_legion_event_lua(connindex,legion_guid,event_idx,extra_data,happen_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_legion_event_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::clear_legion_event_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_clear_legion_event_lua00
static int tolua_logic_hld_clear_legion_event_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::clear_legion_event_lua(connindex,legion_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_legion_event_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_legion_bonus_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_legion_bonus_info_lua00
static int tolua_logic_hld_save_legion_bonus_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int mission_id = ((int)  tolua_tointeger(tolua_S,3,0));
  int finish_count = ((int)  tolua_tointeger(tolua_S,4,0));
  {
   faith::save_legion_bonus_info_lua(connindex,legion_guid,mission_id,finish_count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_legion_bonus_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::clear_legion_bonus_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_clear_legion_bonus_info_lua00
static int tolua_logic_hld_clear_legion_bonus_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::clear_legion_bonus_info_lua(connindex,legion_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_legion_bonus_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_cross_city_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_cross_city_info_lua00
static int tolua_logic_hld_load_cross_city_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_cross_city_info_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_cross_city_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_cross_city_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_cross_city_info_lua00
static int tolua_logic_hld_save_cross_city_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int rank_idex = ((int)  tolua_tointeger(tolua_S,2,0));
  long long legion_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,4,0));
  long long chief_guid = ((long long)  tolua_tointeger(tolua_S,5,0));
  std::string legion_name = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  std::string chief_name = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  {
   faith::save_cross_city_info_lua(connindex,rank_idex,legion_guid,server_id,chief_guid,legion_name,chief_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_cross_city_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::clear_cross_city_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_clear_cross_city_info_lua00
static int tolua_logic_hld_clear_cross_city_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::clear_cross_city_info_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_cross_city_info_lua'.",&tolua_err);
 return 0;
#endif
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

/* method: get_instance of class  faith::data_manager */
#ifndef TOLUA_DISABLE_tolua_logic_hld_data_manager_get_instance00
static int tolua_logic_hld_data_manager_get_instance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"faith::data_manager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   faith::data_manager& tolua_ret = (faith::data_manager&)  faith::data_manager::get_instance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"faith::data_manager");
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

/* method: get_data_set of class  faith::data_manager */
#ifndef TOLUA_DISABLE_tolua_logic_hld_data_manager_get_data_set00
static int tolua_logic_hld_data_manager_get_data_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::data_manager",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const faith::guid_64",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::data_manager* self = (faith::data_manager*)  tolua_tousertype(tolua_S,1,0);
  const faith::guid_64* role_guid = ((const faith::guid_64*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_data_set'", NULL);
#endif
  {
   faith::data_set* tolua_ret = (faith::data_set*)  self->get_data_set(*role_guid);
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"faith::data_set");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_data_set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_db_name of class  faith::data_manager */
#ifndef TOLUA_DISABLE_tolua_logic_hld_data_manager_get_db_name00
static int tolua_logic_hld_data_manager_get_db_name00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::data_manager",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::data_manager* self = (faith::data_manager*)  tolua_tousertype(tolua_S,1,0);
  long const long data_guid = ((long const long)  tolua_tointeger(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_db_name'", NULL);
#endif
  {
   string tolua_ret = (string)  self->get_db_name(data_guid);
   tolua_pushcppstring(tolua_S,(const char*)tolua_ret);
   tolua_pushinteger(tolua_S,(lua_Integer)data_guid);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_db_name'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: add_data_info of class  faith::data_set */
#ifndef TOLUA_DISABLE_tolua_logic_hld_data_set_add_data_info00
static int tolua_logic_hld_data_set_add_data_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::data_set",0,&tolua_err) ||
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
  faith::data_set* self = (faith::data_set*)  tolua_tousertype(tolua_S,1,0);
  long long data_type = ((long long)  tolua_tointeger(tolua_S,2,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,3,0));
  int data_size = ((int)  tolua_tointeger(tolua_S,4,0));
  int data_num = ((int)  tolua_tointeger(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'add_data_info'", NULL);
#endif
  {
   self->add_data_info(data_type,data_ptr,data_size,data_num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_data_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_data_info of class  faith::data_set */
#ifndef TOLUA_DISABLE_tolua_logic_hld_data_set_get_data_info00
static int tolua_logic_hld_data_set_get_data_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::data_set",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::data_set* self = (faith::data_set*)  tolua_tousertype(tolua_S,1,0);
  long long data_type = ((long long)  tolua_tointeger(tolua_S,2,0));
  void* data_ptr = ((void*)  tolua_touserdata(tolua_S,3,0));
  int data_num = ((int)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_data_info'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->get_data_info(data_type,data_ptr,data_num);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushinteger(tolua_S,(lua_Integer)data_num);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_data_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: get_proto_info of class  faith::data_set */
#ifndef TOLUA_DISABLE_tolua_logic_hld_data_set_get_proto_info00
static int tolua_logic_hld_data_set_get_proto_info00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"faith::data_set",0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isuserdata(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  faith::data_set* self = (faith::data_set*)  tolua_tousertype(tolua_S,1,0);
  long long data_type = ((long long)  tolua_tointeger(tolua_S,2,0));
  void* data_ptr = ((void*)  tolua_touserdata(tolua_S,3,0));
  int data_size = ((int)  tolua_tointeger(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'get_proto_info'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->get_proto_info(data_type,data_ptr,data_size);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushinteger(tolua_S,(lua_Integer)data_size);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'get_proto_info'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_push_sql_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_push_sql_lua00
static int tolua_logic_hld_cs2dp_save_push_sql_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int unit_array_index = ((int)  tolua_tointeger(tolua_S,3,0));
  long long e_data_flag = ((long long)  tolua_tointeger(tolua_S,4,0));
  const char* sql = ((const char*)  tolua_tostring(tolua_S,5,0));
  int save_type_ex = ((int)  tolua_tointeger(tolua_S,6,0));
  {
   faith::cs2dp_save_push_sql_lua(connindex,role_guid,unit_array_index,e_data_flag,sql,save_type_ex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_push_sql_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_push_sql_no_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_push_sql_no_cache_lua00
static int tolua_logic_hld_cs2dp_save_push_sql_no_cache_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int unit_array_index = ((int)  tolua_tointeger(tolua_S,3,0));
  long long e_data_flag = ((long long)  tolua_tointeger(tolua_S,4,0));
  const char* sql = ((const char*)  tolua_tostring(tolua_S,5,0));
  int save_type_ex = ((int)  tolua_tointeger(tolua_S,6,0));
  {
   faith::cs2dp_save_push_sql_no_cache_lua(connindex,role_guid,unit_array_index,e_data_flag,sql,save_type_ex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_push_sql_no_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_push_sql_no_callback_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_push_sql_no_callback_lua00
static int tolua_logic_hld_cs2dp_save_push_sql_no_callback_lua00(lua_State* tolua_S)
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
  const char* sql = ((const char*)  tolua_tostring(tolua_S,1,0));
  long long e_data_flag = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  {
   faith::cs2dp_save_push_sql_no_callback_lua(sql,e_data_flag,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_push_sql_no_callback_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_starark_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_starark_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_starark_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_starark_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_starark_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_dragontrip_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_dragontrip_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_dragontrip_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_dragontrip_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_dragontrip_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_person_information_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_person_information_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_person_information_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_person_information_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_person_information_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_competition_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_competition_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_competition_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_competition_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_competition_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_recycle_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_recycle_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_recycle_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_recycle_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_recycle_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_recycle_task_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_recycle_task_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_recycle_task_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_recycle_task_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_recycle_task_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_phantom_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_phantom_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_phantom_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_phantom_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_phantom_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_oracle_trial_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_oracle_trial_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_oracle_trial_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_oracle_trial_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_oracle_trial_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_chat_record_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_chat_record_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_chat_record_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_chat_record_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_chat_record_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_legion_skill_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_legion_skill_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_legion_skill_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_legion_skill_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_legion_skill_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_boss_island_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_boss_island_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_boss_island_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_boss_island_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_boss_island_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_seven_day_goal_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_seven_day_goal_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_seven_day_goal_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_seven_day_goal_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_seven_day_goal_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_harry_record_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_harry_record_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_harry_record_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_harry_record_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_harry_record_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_time_limit_activity_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_time_limit_activity_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_time_limit_activity_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_time_limit_activity_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_time_limit_activity_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_gain_treasure_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_gain_treasure_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_gain_treasure_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_gain_treasure_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_gain_treasure_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_service_rank_reward_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_service_rank_reward_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_service_rank_reward_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_service_rank_reward_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_service_rank_reward_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_highest_record_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_highest_record_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_highest_record_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_highest_record_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_highest_record_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_yesterday_must_do_remain_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_yesterday_must_do_remain_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_yesterday_must_do_remain_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_yesterday_must_do_remain_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_yesterday_must_do_remain_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_daily_must_do_count_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_daily_must_do_count_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_daily_must_do_count_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_daily_must_do_count_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_daily_must_do_count_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_talent_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_talent_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_talent_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_talent_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_talent_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_title_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_title_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_title_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_title_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_title_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_welfare_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_welfare_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_welfare_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_welfare_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_welfare_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_active_degree_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_active_degree_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_active_degree_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_active_degree_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_active_degree_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_convert_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_convert_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_convert_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_convert_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_convert_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_worship_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_worship_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_worship_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_worship_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_worship_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_pokedex_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_pokedex_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_pokedex_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_pokedex_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_pokedex_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_tinder_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_tinder_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_tinder_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_tinder_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_tinder_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_achievement_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_achievement_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_achievement_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_achievement_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_achievement_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_special_name_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_special_name_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_special_name_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_special_name_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_special_name_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_service_goal_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_service_goal_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_service_goal_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_service_goal_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_service_goal_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_map_record_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_map_record_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_map_record_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_map_record_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_map_record_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_goods_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_goods_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_goods_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_goods_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_goods_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_mission_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_mission_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_mission_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_mission_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_mission_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_money_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_money_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_money_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_money_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_money_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_save_role_att_cache_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_save_role_att_cache_lua00
static int tolua_logic_hld_cs2dp_save_role_att_cache_lua00(lua_State* tolua_S)
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
  long long role_id = ((long long)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   bool tolua_ret = (bool)  faith::cs2dp_save_role_att_cache_lua(role_id,data_ptr,data_len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_save_role_att_cache_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::cs2dp_push_role_att_sql_add_callback */
#ifndef TOLUA_DISABLE_tolua_logic_hld_cs2dp_push_role_att_sql_add_callback00
static int tolua_logic_hld_cs2dp_push_role_att_sql_add_callback00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* sql = ((const char*)  tolua_tostring(tolua_S,1,0));
  long long role_id = ((long long)  tolua_tointeger(tolua_S,2,0));
#ifdef __cplusplus
  float* attack_att = Mtolua_new_dim(float, faith::e_unit_attack_att_max);
#else
  float* attack_att = (float*) malloc((faith::e_unit_attack_att_max)*sizeof(float));
#endif
  int connindex = ((int)  tolua_tointeger(tolua_S,4,0));
  int unit_array_index = ((int)  tolua_tointeger(tolua_S,5,0));
  int save_type_ex = ((int)  tolua_tointeger(tolua_S,6,0));
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isnumberarray(tolua_S,3,faith::e_unit_attack_att_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<faith::e_unit_attack_att_max;i++)
    attack_att[i] = ((float)  tolua_tofieldnumber(tolua_S,3,i+1,0));
   }
  }
  {
   faith::cs2dp_push_role_att_sql_add_callback(sql,role_id,attack_att,connindex,unit_array_index,save_type_ex);
  }
  {
   int i;
   for(i=0; i<faith::e_unit_attack_att_max;i++)
    tolua_pushfieldnumber(tolua_S,3,i+1,(lua_Number) attack_att[i]);
  }
#ifdef __cplusplus
  Mtolua_delete_dim(attack_att);
#else
  free(attack_att);
#endif
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'cs2dp_push_role_att_sql_add_callback'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_best_record_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_best_record_lua00
static int tolua_logic_hld_save_best_record_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     !tolua_istable(tolua_S,4,0,&tolua_err) ||
     !tolua_istable(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int map_template_id = ((int)  tolua_tointeger(tolua_S,2,0));
  long long role_guid[5];
  std::string role_name[5];
  int role_value[5];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,3,5,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<5;i++)
    role_guid[i] = ((long)  tolua_tofieldinteger(tolua_S,3,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,4,5,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<5;i++)
    role_name[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,4,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,5,5,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<5;i++)
    role_value[i] = ((int)  tolua_tofieldinteger(tolua_S,5,i+1,0));
   }
  }
  {
   faith::save_best_record_lua(connindex,map_template_id,role_guid,role_name,role_value);
  }
  {
   int i;
   for(i=0; i<5;i++)
    tolua_pushfieldinteger(tolua_S,3,i+1,(lua_Integer) role_guid[i]);
  }
  {
   int i;
   for(i=0; i<5;i++)
    tolua_pushfieldcppstring(tolua_S,4,i+1,(const char*) role_name[i]);
  }
  {
   int i;
   for(i=0; i<5;i++)
    tolua_pushfieldinteger(tolua_S,5,i+1,(lua_Integer) role_value[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_best_record_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_all_best_record_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_all_best_record_lua00
static int tolua_logic_hld_load_all_best_record_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_all_best_record_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_all_best_record_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_all_gain_treasure_record_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_all_gain_treasure_record_info_lua00
static int tolua_logic_hld_load_all_gain_treasure_record_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int boss_spawn_point_id = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::load_all_gain_treasure_record_info_lua(connindex,boss_spawn_point_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_all_gain_treasure_record_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_gain_treasure_record_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_gain_treasure_record_info_lua00
static int tolua_logic_hld_save_gain_treasure_record_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_istable(tolua_S,5,0,&tolua_err) ||
     !tolua_istable(tolua_S,6,0,&tolua_err) ||
     !tolua_istable(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int del_stamp = ((int)  tolua_tointeger(tolua_S,2,0));
  int boss_spawn_point_template_id = ((int)  tolua_tointeger(tolua_S,3,0));
  int killed_stamp = ((int)  tolua_tointeger(tolua_S,4,0));
  std::string role_name[3];
  std::string gain_item_template_id[3];
  long long role_guid[3];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,5,3,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<3;i++)
    role_name[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,5,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,6,3,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<3;i++)
    gain_item_template_id[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,6,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,7,3,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<3;i++)
    role_guid[i] = ((long)  tolua_tofieldinteger(tolua_S,7,i+1,0));
   }
  }
  {
   faith::save_gain_treasure_record_info_lua(connindex,del_stamp,boss_spawn_point_template_id,killed_stamp,role_name,gain_item_template_id,role_guid);
  }
  {
   int i;
   for(i=0; i<3;i++)
    tolua_pushfieldcppstring(tolua_S,5,i+1,(const char*) role_name[i]);
  }
  {
   int i;
   for(i=0; i<3;i++)
    tolua_pushfieldcppstring(tolua_S,6,i+1,(const char*) gain_item_template_id[i]);
  }
  {
   int i;
   for(i=0; i<3;i++)
    tolua_pushfieldinteger(tolua_S,7,i+1,(lua_Integer) role_guid[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_gain_treasure_record_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_world_boss_to_sql_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_world_boss_to_sql_lua00
static int tolua_logic_hld_save_world_boss_to_sql_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_istable(tolua_S,2,0,&tolua_err) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     !tolua_istable(tolua_S,4,0,&tolua_err) ||
     !tolua_istable(tolua_S,5,0,&tolua_err) ||
     !tolua_istable(tolua_S,6,0,&tolua_err) ||
     !tolua_istable(tolua_S,7,0,&tolua_err) ||
     !tolua_istable(tolua_S,8,0,&tolua_err) ||
     !tolua_istable(tolua_S,9,0,&tolua_err) ||
     !tolua_istable(tolua_S,10,0,&tolua_err) ||
     !tolua_istable(tolua_S,11,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,12,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int npc_index[30];
  int template_id[30];
  int is_alive[30];
  int line_id[30];
  int64 last_respawn_time[30];
  int respawn_phase[30];
  std::string killer_name[30];
  std::string first_killer_name[30];
  int first_killed_time[30];
  int is_get_prize[30];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,2,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    npc_index[i] = ((int)  tolua_tofieldinteger(tolua_S,2,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,3,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    template_id[i] = ((int)  tolua_tofieldinteger(tolua_S,3,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,4,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    is_alive[i] = ((int)  tolua_tofieldinteger(tolua_S,4,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,5,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    line_id[i] = ((int)  tolua_tofieldinteger(tolua_S,5,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,6,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    last_respawn_time[i] = ((int64)  tolua_tofieldinteger(tolua_S,6,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,7,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    respawn_phase[i] = ((int)  tolua_tofieldinteger(tolua_S,7,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,8,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    killer_name[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,8,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,9,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    first_killer_name[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,9,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,10,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    first_killed_time[i] = ((int)  tolua_tofieldinteger(tolua_S,10,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,11,30,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<30;i++)
    is_get_prize[i] = ((int)  tolua_tofieldinteger(tolua_S,11,i+1,0));
   }
  }
  {
   faith::save_world_boss_to_sql_lua(connindex,npc_index,template_id,is_alive,line_id,last_respawn_time,respawn_phase,killer_name,first_killer_name,first_killed_time,is_get_prize);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,2,i+1,(lua_Integer) npc_index[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,3,i+1,(lua_Integer) template_id[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,4,i+1,(lua_Integer) is_alive[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,5,i+1,(lua_Integer) line_id[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,6,i+1,(lua_Integer) last_respawn_time[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,7,i+1,(lua_Integer) respawn_phase[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldcppstring(tolua_S,8,i+1,(const char*) killer_name[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldcppstring(tolua_S,9,i+1,(const char*) first_killer_name[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,10,i+1,(lua_Integer) first_killed_time[i]);
  }
  {
   int i;
   for(i=0; i<30;i++)
    tolua_pushfieldinteger(tolua_S,11,i+1,(lua_Integer) is_get_prize[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_world_boss_to_sql_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_world_boss_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_world_boss_lua00
static int tolua_logic_hld_load_world_boss_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_world_boss_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_world_boss_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_cross_server_pk_ws_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_cross_server_pk_ws_info_lua00
static int tolua_logic_hld_save_cross_server_pk_ws_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_istable(tolua_S,4,0,&tolua_err) ||
     !tolua_istable(tolua_S,5,0,&tolua_err) ||
     !tolua_istable(tolua_S,6,0,&tolua_err) ||
     !tolua_istable(tolua_S,7,0,&tolua_err) ||
     !tolua_istable(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int pk_state = ((int)  tolua_tointeger(tolua_S,2,0));
  int war_index = ((int)  tolua_tointeger(tolua_S,3,0));
  long long player_guid[2];
  std::string role_name[2];
  int is_winner[2];
  int server_id[2];
  int win_num[2];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,4,2,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<2;i++)
    player_guid[i] = ((long)  tolua_tofieldinteger(tolua_S,4,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,5,2,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<2;i++)
    role_name[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,5,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,6,2,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<2;i++)
    is_winner[i] = ((int)  tolua_tofieldinteger(tolua_S,6,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,7,2,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<2;i++)
    server_id[i] = ((int)  tolua_tofieldinteger(tolua_S,7,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,8,2,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<2;i++)
    win_num[i] = ((int)  tolua_tofieldinteger(tolua_S,8,i+1,0));
   }
  }
  {
   faith::save_cross_server_pk_ws_info_lua(connindex,pk_state,war_index,player_guid,role_name,is_winner,server_id,win_num);
  }
  {
   int i;
   for(i=0; i<2;i++)
    tolua_pushfieldinteger(tolua_S,4,i+1,(lua_Integer) player_guid[i]);
  }
  {
   int i;
   for(i=0; i<2;i++)
    tolua_pushfieldcppstring(tolua_S,5,i+1,(const char*) role_name[i]);
  }
  {
   int i;
   for(i=0; i<2;i++)
    tolua_pushfieldinteger(tolua_S,6,i+1,(lua_Integer) is_winner[i]);
  }
  {
   int i;
   for(i=0; i<2;i++)
    tolua_pushfieldinteger(tolua_S,7,i+1,(lua_Integer) server_id[i]);
  }
  {
   int i;
   for(i=0; i<2;i++)
    tolua_pushfieldinteger(tolua_S,8,i+1,(lua_Integer) win_num[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_cross_server_pk_ws_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::clear_cross_server_pk_ws_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_clear_cross_server_pk_ws_info_lua00
static int tolua_logic_hld_clear_cross_server_pk_ws_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::clear_cross_server_pk_ws_info_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'clear_cross_server_pk_ws_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_cross_server_pk_ws_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_cross_server_pk_ws_info_lua00
static int tolua_logic_hld_load_cross_server_pk_ws_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_cross_server_pk_ws_info_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_cross_server_pk_ws_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_cross_server_harry_ws_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_cross_server_harry_ws_info_lua00
static int tolua_logic_hld_save_cross_server_harry_ws_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  int server_money = ((int)  tolua_tointeger(tolua_S,3,0));
  int special_money = ((int)  tolua_tointeger(tolua_S,4,0));
  long long first_player_guid = ((long long)  tolua_tointeger(tolua_S,5,0));
  long long second_player_guid = ((long long)  tolua_tointeger(tolua_S,6,0));
  long long third_player_guid = ((long long)  tolua_tointeger(tolua_S,7,0));
  std::string first_player_name = ((std::string)  tolua_tocppstring(tolua_S,8,0));
  {
   faith::save_cross_server_harry_ws_info_lua(connindex,server_id,server_money,special_money,first_player_guid,second_player_guid,third_player_guid,first_player_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_cross_server_harry_ws_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_cross_server_harry_ws_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_cross_server_harry_ws_info_lua00
static int tolua_logic_hld_load_cross_server_harry_ws_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_cross_server_harry_ws_info_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_cross_server_harry_ws_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_harry_player_detail_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_harry_player_detail_lua00
static int tolua_logic_hld_load_harry_player_detail_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int player_idex = ((int)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,4,0));
  {
   faith::load_harry_player_detail_lua(connindex,player_idex,role_guid,server_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_harry_player_detail_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::del_one_server_msg_func_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_del_one_server_msg_func_lua00
static int tolua_logic_hld_del_one_server_msg_func_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,2,0));
  {
   faith::del_one_server_msg_func_lua(connindex,server_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_one_server_msg_func_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_role_pk_top_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_role_pk_top_lua00
static int tolua_logic_hld_load_role_pk_top_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_role_pk_top_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_role_pk_top_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_role_pk_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_role_pk_info_lua00
static int tolua_logic_hld_load_role_pk_info_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int tempalte_id = ((int)  tolua_tointeger(tolua_S,3,0));
  int role_gs = ((int)  tolua_tointeger(tolua_S,4,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  {
   faith::load_role_pk_info_lua(connindex,role_guid,tempalte_id,role_gs,role_name);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_role_pk_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_role_pk_info_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_role_pk_info_lua00
static int tolua_logic_hld_save_role_pk_info_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,9,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  std::string target_name1 = ((std::string)  tolua_tocppstring(tolua_S,3,0));
  std::string target_name2 = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  std::string target_name3 = ((std::string)  tolua_tocppstring(tolua_S,5,0));
  int laddet_score = ((int)  tolua_tointeger(tolua_S,6,0));
  int win_result = ((int)  tolua_tointeger(tolua_S,7,0));
  int cross_honor = ((int)  tolua_tointeger(tolua_S,8,0));
  {
   faith::save_role_pk_info_lua(connindex,role_guid,target_name1,target_name2,target_name3,laddet_score,win_result,cross_honor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_role_pk_info_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_role_pk_honor_refresh_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_role_pk_honor_refresh_lua00
static int tolua_logic_hld_save_role_pk_honor_refresh_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int refresh_time = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::save_role_pk_honor_refresh_lua(connindex,role_guid,refresh_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_role_pk_honor_refresh_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_role_pk_log_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_role_pk_log_lua00
static int tolua_logic_hld_load_role_pk_log_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int server_id = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::load_role_pk_log_lua(connindex,role_guid,server_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_role_pk_log_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_red_package_to_sql_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_red_package_to_sql_lua00
static int tolua_logic_hld_save_red_package_to_sql_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::save_red_package_to_sql_lua(connindex,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_red_package_to_sql_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_red_package_by_sql_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_red_package_by_sql_lua00
static int tolua_logic_hld_load_red_package_by_sql_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_red_package_by_sql_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_red_package_by_sql_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_red_package_receiver_by_sql_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_red_package_receiver_by_sql_lua00
static int tolua_logic_hld_load_red_package_receiver_by_sql_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::load_red_package_receiver_by_sql_lua(connindex,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_red_package_receiver_by_sql_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::add_red_package_to_sql_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_add_red_package_to_sql_proc_lua00
static int tolua_logic_hld_add_red_package_to_sql_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,6,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,7,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,8,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,9,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,10,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  int template_id = ((int)  tolua_tointeger(tolua_S,3,0));
  long long sender_guid = ((long long)  tolua_tointeger(tolua_S,4,0));
  int create_time = ((int)  tolua_tointeger(tolua_S,5,0));
  std::string sender_name = ((std::string)  tolua_tocppstring(tolua_S,6,0));
  std::string red_text = ((std::string)  tolua_tocppstring(tolua_S,7,0));
  int sender_vip_level = ((int)  tolua_tointeger(tolua_S,8,0));
  int remain_times = ((int)  tolua_tointeger(tolua_S,9,0));
  int remain_money_num = ((int)  tolua_tointeger(tolua_S,10,0));
  {
   faith::add_red_package_to_sql_proc_lua(connindex,role_guid,template_id,sender_guid,create_time,sender_name,red_text,sender_vip_level,remain_times,remain_money_num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_red_package_to_sql_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::add_red_package_receiver_to_sql_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_add_red_package_receiver_to_sql_proc_lua00
static int tolua_logic_hld_add_red_package_receiver_to_sql_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long receiver_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  int money_num = ((int)  tolua_tointeger(tolua_S,5,0));
  {
   faith::add_red_package_receiver_to_sql_proc_lua(connindex,role_guid,receiver_guid,role_name,money_num);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_red_package_receiver_to_sql_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::del_red_package_by_sql_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_del_red_package_by_sql_proc_lua00
static int tolua_logic_hld_del_red_package_by_sql_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long red_package_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::del_red_package_by_sql_proc_lua(connindex,red_package_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_red_package_by_sql_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::del_draw_info_from_db_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_del_draw_info_from_db_lua00
static int tolua_logic_hld_del_draw_info_from_db_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long info_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  {
   faith::del_draw_info_from_db_lua(connindex,info_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'del_draw_info_from_db_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::add_draw_info_to_db_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_add_draw_info_to_db_lua00
static int tolua_logic_hld_add_draw_info_to_db_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,4,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  long long info_guid = ((long long)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  std::string role_name = ((std::string)  tolua_tocppstring(tolua_S,4,0));
  int item_template_id = ((int)  tolua_tointeger(tolua_S,5,0));
  int get_type = ((int)  tolua_tointeger(tolua_S,6,0));
  int record_type = ((int)  tolua_tointeger(tolua_S,7,0));
  long long record_time = ((long long)  tolua_tointeger(tolua_S,8,0));
  {
   faith::add_draw_info_to_db_lua(connindex,info_guid,role_guid,role_name,item_template_id,get_type,record_type,record_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'add_draw_info_to_db_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_draw_info_by_db_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_draw_info_by_db_lua00
static int tolua_logic_hld_load_draw_info_by_db_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_draw_info_by_db_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_draw_info_by_db_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::save_big_player_to_sql_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_save_big_player_to_sql_lua00
static int tolua_logic_hld_save_big_player_to_sql_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_istable(tolua_S,2,0,&tolua_err) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
#ifdef __cplusplus
  int* player_type = Mtolua_new_dim(int, faith::e_big_player_type_max);
#else
  int* player_type = (int*) malloc((faith::e_big_player_type_max)*sizeof(int));
#endif
#ifdef __cplusplus
  int64* role_guid = Mtolua_new_dim(int64, faith::e_big_player_type_max);
#else
  int64* role_guid = (int64*) malloc((faith::e_big_player_type_max)*sizeof(int64));
#endif
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,2,faith::e_big_player_type_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<faith::e_big_player_type_max;i++)
    player_type[i] = ((int)  tolua_tofieldinteger(tolua_S,2,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,3,faith::e_big_player_type_max,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<faith::e_big_player_type_max;i++)
    role_guid[i] = ((int64)  tolua_tofieldinteger(tolua_S,3,i+1,0));
   }
  }
  {
   faith::save_big_player_to_sql_lua(connindex,player_type,role_guid);
  }
  {
   int i;
   for(i=0; i<faith::e_big_player_type_max;i++)
    tolua_pushfieldinteger(tolua_S,2,i+1,(lua_Integer) player_type[i]);
  }
  {
   int i;
   for(i=0; i<faith::e_big_player_type_max;i++)
    tolua_pushfieldinteger(tolua_S,3,i+1,(lua_Integer) role_guid[i]);
  }
#ifdef __cplusplus
  Mtolua_delete_dim(player_type);
#else
  free(player_type);
#endif
#ifdef __cplusplus
  Mtolua_delete_dim(role_guid);
#else
  free(role_guid);
#endif
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'save_big_player_to_sql_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_big_player_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_big_player_lua00
static int tolua_logic_hld_load_big_player_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::load_big_player_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_big_player_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::load_big_player_detail_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_load_big_player_detail_lua00
static int tolua_logic_hld_load_big_player_detail_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int player_type = ((int)  tolua_tointeger(tolua_S,2,0));
  long long role_guid = ((long long)  tolua_tointeger(tolua_S,3,0));
  {
   faith::load_big_player_detail_lua(connindex,player_type,role_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'load_big_player_detail_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_add_item_into_db_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_add_item_into_db_proc_lua00
static int tolua_logic_hld_auction_add_item_into_db_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,2,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,3,0));
  {
   faith::auction_add_item_into_db_proc_lua(connindex,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_add_item_into_db_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_find_buy_target_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_find_buy_target_proc_lua00
static int tolua_logic_hld_auction_find_buy_target_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  int64 item_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  {
   faith::auction_find_buy_target_proc_lua(connindex,role_guid,item_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_find_buy_target_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_find_cancel_target_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_find_cancel_target_proc_lua00
static int tolua_logic_hld_auction_find_cancel_target_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  int64 item_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  {
   faith::auction_find_cancel_target_proc_lua(connindex,role_guid,item_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_find_cancel_target_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_find_bid_target_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_find_bid_target_proc_lua00
static int tolua_logic_hld_auction_find_bid_target_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  int64 item_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  int32 money_type = ((int32)  tolua_tointeger(tolua_S,4,0));
  int32 money_value = ((int32)  tolua_tointeger(tolua_S,5,0));
  {
   faith::auction_find_bid_target_proc_lua(connindex,role_guid,item_guid,money_type,money_value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_find_bid_target_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_bid_success_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_bid_success_proc_lua00
static int tolua_logic_hld_auction_bid_success_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 new_record_id = ((int64)  tolua_tointeger(tolua_S,2,0));
  int64 old_buyer_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,4,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,5,0));
  {
   faith::auction_bid_success_proc_lua(connindex,new_record_id,old_buyer_guid,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_bid_success_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_get_and_clear_old_item_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_get_and_clear_old_item_proc_lua00
static int tolua_logic_hld_auction_get_and_clear_old_item_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int32 trade_type = ((int32)  tolua_tointeger(tolua_S,2,0));
  int64 cur_time = ((int64)  tolua_tointeger(tolua_S,3,0));
  {
   faith::auction_get_and_clear_old_item_proc_lua(connindex,trade_type,cur_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_get_and_clear_old_item_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_add_share_list_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_add_share_list_proc_lua00
static int tolua_logic_hld_auction_add_share_list_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_istable(tolua_S,2,0,&tolua_err) ||
     !tolua_istable(tolua_S,3,0,&tolua_err) ||
     !tolua_istable(tolua_S,4,0,&tolua_err) ||
     !tolua_istable(tolua_S,5,0,&tolua_err) ||
     !tolua_istable(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 item_guid[130];
  int64 role_guid[130];
  float share_rate[130];
  std::string share_mail_title[130];
  std::string share_mail_content[130];
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,2,130,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<130;i++)
    item_guid[i] = ((int64)  tolua_tofieldinteger(tolua_S,2,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isintegerarray(tolua_S,3,130,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<130;i++)
    role_guid[i] = ((int64)  tolua_tofieldinteger(tolua_S,3,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_isnumberarray(tolua_S,4,130,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<130;i++)
    share_rate[i] = ((float)  tolua_tofieldnumber(tolua_S,4,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,5,130,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<130;i++)
    share_mail_title[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,5,i+1,0));
   }
  }
  {
#ifndef TOLUA_RELEASE
   if (!tolua_iscppstringarray(tolua_S,6,130,0,&tolua_err))
    goto tolua_lerror;
   else
#endif
   {
    int i;
    for(i=0; i<130;i++)
    share_mail_content[i] = ((std::string)  tolua_tofieldcppstring(tolua_S,6,i+1,0));
   }
  }
  {
   faith::auction_add_share_list_proc_lua(connindex,item_guid,role_guid,share_rate,share_mail_title,share_mail_content);
  }
  {
   int i;
   for(i=0; i<130;i++)
    tolua_pushfieldinteger(tolua_S,2,i+1,(lua_Integer) item_guid[i]);
  }
  {
   int i;
   for(i=0; i<130;i++)
    tolua_pushfieldinteger(tolua_S,3,i+1,(lua_Integer) role_guid[i]);
  }
  {
   int i;
   for(i=0; i<130;i++)
    tolua_pushfieldnumber(tolua_S,4,i+1,(lua_Number) share_rate[i]);
  }
  {
   int i;
   for(i=0; i<130;i++)
    tolua_pushfieldcppstring(tolua_S,5,i+1,(const char*) share_mail_title[i]);
  }
  {
   int i;
   for(i=0; i<130;i++)
    tolua_pushfieldcppstring(tolua_S,6,i+1,(const char*) share_mail_content[i]);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_add_share_list_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_add_trade_record_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_add_trade_record_proc_lua00
static int tolua_logic_hld_auction_add_trade_record_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,3,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,4,0,&tolua_err) ||
     !tolua_isstring(tolua_S,5,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 record_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  int64 role_guid = ((int64)  tolua_tointeger(tolua_S,3,0));
  bool is_sell = ((bool)  tolua_toboolean(tolua_S,4,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,5,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,6,0));
  {
   faith::auction_add_trade_record_proc_lua(connindex,record_guid,role_guid,is_sell,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_add_trade_record_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_req_share_list_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_req_share_list_proc_lua00
static int tolua_logic_hld_auction_req_share_list_proc_lua00(lua_State* tolua_S)
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
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 item_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  int32 item_template_id = ((int32)  tolua_tointeger(tolua_S,3,0));
  int64 buyer_guid = ((int64)  tolua_tointeger(tolua_S,4,0));
  int32 money_type = ((int32)  tolua_tointeger(tolua_S,5,0));
  int32 money_value = ((int32)  tolua_tointeger(tolua_S,6,0));
  {
   faith::auction_req_share_list_proc_lua(connindex,item_guid,item_template_id,buyer_guid,money_type,money_value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_req_share_list_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_check_legion_selling_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_check_legion_selling_proc_lua00
static int tolua_logic_hld_auction_check_legion_selling_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 legion_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  int64 cur_time = ((int64)  tolua_tointeger(tolua_S,3,0));
  {
   faith::auction_check_legion_selling_proc_lua(connindex,legion_guid,cur_time);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_check_legion_selling_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_del_bid_record_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_del_bid_record_proc_lua00
static int tolua_logic_hld_auction_del_bid_record_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 item_guid = ((int64)  tolua_tointeger(tolua_S,2,0));
  {
   faith::auction_del_bid_record_proc_lua(connindex,item_guid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_del_bid_record_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_clear_old_bid_record_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_clear_old_bid_record_proc_lua00
static int tolua_logic_hld_auction_clear_old_bid_record_proc_lua00(lua_State* tolua_S)
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  {
   faith::auction_clear_old_bid_record_proc_lua(connindex);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_clear_old_bid_record_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_update_sell_show_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_update_sell_show_proc_lua00
static int tolua_logic_hld_auction_update_sell_show_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isinteger(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int32 end_time = ((int32)  tolua_tointeger(tolua_S,2,0));
  const char* data_ptr = ((const char*)  tolua_tostring(tolua_S,3,0));
  int data_len = ((int)  tolua_tointeger(tolua_S,4,0));
  {
   faith::auction_update_sell_show_proc_lua(connindex,end_time,data_ptr,data_len);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_update_sell_show_proc_lua'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: faith::auction_time_out_proc_lua */
#ifndef TOLUA_DISABLE_tolua_logic_hld_auction_time_out_proc_lua00
static int tolua_logic_hld_auction_time_out_proc_lua00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isinteger(tolua_S,1,0,&tolua_err) ||
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
  int connindex = ((int)  tolua_tointeger(tolua_S,1,0));
  int64 cur_mil_sec = ((int64)  tolua_tointeger(tolua_S,2,0));
  int64 sell_time = ((int64)  tolua_tointeger(tolua_S,3,0));
  int64 deal_time = ((int64)  tolua_tointeger(tolua_S,4,0));
  int32 is_first = ((int32)  tolua_tointeger(tolua_S,5,0));
  {
   faith::auction_time_out_proc_lua(connindex,cur_mil_sec,sell_time,deal_time,is_first);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'auction_time_out_proc_lua'.",&tolua_err);
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
   tolua_cclass(tolua_S,"dbproxy_service","faith::dbproxy_service","",NULL);
   tolua_beginmodule(tolua_S,"dbproxy_service");
    tolua_function(tolua_S,"getInstance",tolua_logic_hld_dbproxy_service_getInstance00);
    tolua_function(tolua_S,"send_message_lua",tolua_logic_hld_dbproxy_service_send_message_lua00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"save_arena_rank_first_lua",tolua_logic_hld_save_arena_rank_first_lua00);
   tolua_function(tolua_S,"load_all_ranking_info_lua",tolua_logic_hld_load_all_ranking_info_lua00);
   tolua_function(tolua_S,"save_ranking_info_one_lua",tolua_logic_hld_save_ranking_info_one_lua00);
   tolua_function(tolua_S,"load_all_service_rank_info_lua",tolua_logic_hld_load_all_service_rank_info_lua00);
   tolua_function(tolua_S,"save_ranking_worship_lua",tolua_logic_hld_save_ranking_worship_lua00);
   tolua_function(tolua_S,"load_player_worship_value_lua",tolua_logic_hld_load_player_worship_value_lua00);
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
    tolua_function(tolua_S,"get_region_code",tolua_logic_hld_globle_data_get_region_code00);
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
   tolua_constant(tolua_S,"e_item_base_bag_type",faith::e_item_base_bag_type);
   tolua_constant(tolua_S,"e_item_base_count",faith::e_item_base_count);
   tolua_constant(tolua_S,"e_item_base_locked",faith::e_item_base_locked);
   tolua_constant(tolua_S,"e_item_base_max",faith::e_item_base_max);
   tolua_constant(tolua_S,"e_item_equip_level",faith::e_item_equip_level);
   tolua_constant(tolua_S,"e_item_equip_color",faith::e_item_equip_color);
   tolua_constant(tolua_S,"e_item_equip_attack",faith::e_item_equip_attack);
   tolua_constant(tolua_S,"e_item_equip_defense",faith::e_item_equip_defense);
   tolua_constant(tolua_S,"e_item_equip_hp",faith::e_item_equip_hp);
   tolua_constant(tolua_S,"e_item_equip_random_att_1",faith::e_item_equip_random_att_1);
   tolua_constant(tolua_S,"e_item_equip_random_att_2",faith::e_item_equip_random_att_2);
   tolua_constant(tolua_S,"e_item_equip_random_att_3",faith::e_item_equip_random_att_3);
   tolua_constant(tolua_S,"e_item_equip_random_att_4",faith::e_item_equip_random_att_4);
   tolua_constant(tolua_S,"e_item_equip_random_att_5",faith::e_item_equip_random_att_5);
   tolua_constant(tolua_S,"e_item_equip_max",faith::e_item_equip_max);
   tolua_constant(tolua_S,"e_item_sprite_equip_slot",faith::e_item_sprite_equip_slot);
   tolua_constant(tolua_S,"e_item_sprite_level",faith::e_item_sprite_level);
   tolua_constant(tolua_S,"e_item_sprite_max",faith::e_item_sprite_max);
   tolua_constant(tolua_S,"e_item_prop_param1",faith::e_item_prop_param1);
   tolua_constant(tolua_S,"e_item_prop_max",faith::e_item_prop_max);
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
   tolua_constant(tolua_S,"e_award_mark_level_small",faith::e_award_mark_level_small);
   tolua_constant(tolua_S,"e_award_mark_level_middle",faith::e_award_mark_level_middle);
   tolua_constant(tolua_S,"e_award_mark_level_big",faith::e_award_mark_level_big);
   tolua_constant(tolua_S,"e_award_mark_level_special",faith::e_award_mark_level_special);
   tolua_constant(tolua_S,"e_award_mark_level_max",faith::e_award_mark_level_max);
   tolua_constant(tolua_S,"e_award_mark_type_nomal",faith::e_award_mark_type_nomal);
   tolua_constant(tolua_S,"e_award_mark_type_treasure_pic",faith::e_award_mark_type_treasure_pic);
   tolua_constant(tolua_S,"e_award_mark_type_special",faith::e_award_mark_type_special);
   tolua_constant(tolua_S,"e_award_mark_type_bank_robbery",faith::e_award_mark_type_bank_robbery);
   tolua_constant(tolua_S,"e_award_mark_type_refrush_monster",faith::e_award_mark_type_refrush_monster);
   tolua_constant(tolua_S,"e_award_mark_type_finger_guess",faith::e_award_mark_type_finger_guess);
   tolua_constant(tolua_S,"e_award_mark_type_smashing_bottle",faith::e_award_mark_type_smashing_bottle);
   tolua_constant(tolua_S,"e_award_mark_type_transfer",faith::e_award_mark_type_transfer);
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
   tolua_function(tolua_S,"gm_set_open_time_lua",tolua_logic_hld_gm_set_open_time_lua00);
   tolua_function(tolua_S,"gm_set_cross_open_time_lua",tolua_logic_hld_gm_set_cross_open_time_lua00);
   tolua_function(tolua_S,"gm_set_new_password_lua",tolua_logic_hld_gm_set_new_password_lua00);
   tolua_function(tolua_S,"gm_set_begin_cross_lua",tolua_logic_hld_gm_set_begin_cross_lua00);
   tolua_function(tolua_S,"save_server_last_server_list_lua",tolua_logic_hld_save_server_last_server_list_lua00);
   tolua_function(tolua_S,"load_server_last_server_list_lua",tolua_logic_hld_load_server_last_server_list_lua00);
   tolua_function(tolua_S,"gm_set_role_right_lua",tolua_logic_hld_gm_set_role_right_lua00);
   tolua_function(tolua_S,"gm_change_account_lua",tolua_logic_hld_gm_change_account_lua00);
   tolua_function(tolua_S,"gm_del_role_cache_lua",tolua_logic_hld_gm_del_role_cache_lua00);
   tolua_function(tolua_S,"gm_del_role_lua",tolua_logic_hld_gm_del_role_lua00);
   tolua_function(tolua_S,"gm_stop_login_lua",tolua_logic_hld_gm_stop_login_lua00);
   tolua_function(tolua_S,"get_char_info_ws_lua",tolua_logic_hld_get_char_info_ws_lua00);
   tolua_function(tolua_S,"gm_get_role_info_lua",tolua_logic_hld_gm_get_role_info_lua00);
   tolua_function(tolua_S,"gm_modify_role_info_lua",tolua_logic_hld_gm_modify_role_info_lua00);
   tolua_function(tolua_S,"gm_get_role_items_info_lua",tolua_logic_hld_gm_get_role_items_info_lua00);
   tolua_function(tolua_S,"save_server_attr_val_lua",tolua_logic_hld_save_server_attr_val_lua00);
   tolua_function(tolua_S,"load_server_attr_val_lua",tolua_logic_hld_load_server_attr_val_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"ws2dp_req_load_offline_other_relation_lua",tolua_logic_hld_ws2dp_req_load_offline_other_relation_lua00);
   tolua_function(tolua_S,"ws2dp_req_load_player_relation_lua",tolua_logic_hld_ws2dp_req_load_player_relation_lua00);
   tolua_function(tolua_S,"ws2dp_req_add_relation_lua",tolua_logic_hld_ws2dp_req_add_relation_lua00);
   tolua_function(tolua_S,"ws2dp_req_del_relation_lua",tolua_logic_hld_ws2dp_req_del_relation_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"check_id_mail_proc_lua",tolua_logic_hld_check_id_mail_proc_lua00);
   tolua_function(tolua_S,"save_other_server_mail_record_lua",tolua_logic_hld_save_other_server_mail_record_lua00);
   tolua_function(tolua_S,"load_other_server_mail_lua",tolua_logic_hld_load_other_server_mail_lua00);
   tolua_function(tolua_S,"del_other_server_mail_lua",tolua_logic_hld_del_other_server_mail_lua00);
   tolua_function(tolua_S,"save_globel_mail_to_sql_lua",tolua_logic_hld_save_globel_mail_to_sql_lua00);
   tolua_function(tolua_S,"save_globel_mail_item_to_lua",tolua_logic_hld_save_globel_mail_item_to_lua00);
   tolua_function(tolua_S,"load_mail_event_lua",tolua_logic_hld_load_mail_event_lua00);
   tolua_function(tolua_S,"load_mail_event_item_lua",tolua_logic_hld_load_mail_event_item_lua00);
   tolua_function(tolua_S,"save_globel_mail_new_get_info_proc_lua",tolua_logic_hld_save_globel_mail_new_get_info_proc_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"save_legion_info_lua",tolua_logic_hld_save_legion_info_lua00);
   tolua_function(tolua_S,"save_legion_member_info_lua",tolua_logic_hld_save_legion_member_info_lua00);
   tolua_function(tolua_S,"save_legion_city_war_member_info_lua",tolua_logic_hld_save_legion_city_war_member_info_lua00);
   tolua_function(tolua_S,"delete_legion_info_lua",tolua_logic_hld_delete_legion_info_lua00);
   tolua_function(tolua_S,"delete_legion_member_info_lua",tolua_logic_hld_delete_legion_member_info_lua00);
   tolua_function(tolua_S,"delete_legion_city_war_member_info_lua",tolua_logic_hld_delete_legion_city_war_member_info_lua00);
   tolua_function(tolua_S,"delete_legion_applicant_lua",tolua_logic_hld_delete_legion_applicant_lua00);
   tolua_function(tolua_S,"save_legion_applicant_lua",tolua_logic_hld_save_legion_applicant_lua00);
   tolua_function(tolua_S,"save_legion_boss_award_get_log_lua",tolua_logic_hld_save_legion_boss_award_get_log_lua00);
   tolua_function(tolua_S,"load_all_legion_boss_award_get_log_lua",tolua_logic_hld_load_all_legion_boss_award_get_log_lua00);
   tolua_function(tolua_S,"save_city_info_lua",tolua_logic_hld_save_city_info_lua00);
   tolua_function(tolua_S,"save_war_bid_info_lua",tolua_logic_hld_save_war_bid_info_lua00);
   tolua_function(tolua_S,"clear_war_bid_info_lua",tolua_logic_hld_clear_war_bid_info_lua00);
   tolua_function(tolua_S,"clear_city_war_reward_stuff_lua",tolua_logic_hld_clear_city_war_reward_stuff_lua00);
   tolua_function(tolua_S,"save_legion_event_lua",tolua_logic_hld_save_legion_event_lua00);
   tolua_function(tolua_S,"clear_legion_event_lua",tolua_logic_hld_clear_legion_event_lua00);
   tolua_function(tolua_S,"save_legion_bonus_info_lua",tolua_logic_hld_save_legion_bonus_info_lua00);
   tolua_function(tolua_S,"clear_legion_bonus_info_lua",tolua_logic_hld_clear_legion_bonus_info_lua00);
   tolua_function(tolua_S,"load_cross_city_info_lua",tolua_logic_hld_load_cross_city_info_lua00);
   tolua_function(tolua_S,"save_cross_city_info_lua",tolua_logic_hld_save_cross_city_info_lua00);
   tolua_function(tolua_S,"clear_cross_city_info_lua",tolua_logic_hld_clear_cross_city_info_lua00);
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
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"data_manager","faith::data_manager","",NULL);
   tolua_beginmodule(tolua_S,"data_manager");
    tolua_function(tolua_S,"get_instance",tolua_logic_hld_data_manager_get_instance00);
    tolua_function(tolua_S,"get_data_set",tolua_logic_hld_data_manager_get_data_set00);
    tolua_function(tolua_S,"get_db_name",tolua_logic_hld_data_manager_get_db_name00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_cclass(tolua_S,"data_set","faith::data_set","",NULL);
   tolua_beginmodule(tolua_S,"data_set");
    tolua_function(tolua_S,"add_data_info",tolua_logic_hld_data_set_add_data_info00);
    tolua_function(tolua_S,"get_data_info",tolua_logic_hld_data_set_get_data_info00);
    tolua_function(tolua_S,"get_proto_info",tolua_logic_hld_data_set_get_proto_info00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_push_sql_lua",tolua_logic_hld_cs2dp_save_push_sql_lua00);
   tolua_function(tolua_S,"cs2dp_save_push_sql_no_cache_lua",tolua_logic_hld_cs2dp_save_push_sql_no_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_push_sql_no_callback_lua",tolua_logic_hld_cs2dp_save_push_sql_no_callback_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_starark_cache_lua",tolua_logic_hld_cs2dp_save_role_starark_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_dragontrip_cache_lua",tolua_logic_hld_cs2dp_save_role_dragontrip_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_person_information_cache_lua",tolua_logic_hld_cs2dp_save_role_person_information_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_competition_cache_lua",tolua_logic_hld_cs2dp_save_role_competition_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_recycle_cache_lua",tolua_logic_hld_cs2dp_save_role_recycle_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_recycle_task_cache_lua",tolua_logic_hld_cs2dp_save_role_recycle_task_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_phantom_cache_lua",tolua_logic_hld_cs2dp_save_role_phantom_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_oracle_trial_cache_lua",tolua_logic_hld_cs2dp_save_role_oracle_trial_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_chat_record_cache_lua",tolua_logic_hld_cs2dp_save_role_chat_record_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_legion_skill_cache_lua",tolua_logic_hld_cs2dp_save_role_legion_skill_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_boss_island_cache_lua",tolua_logic_hld_cs2dp_save_role_boss_island_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_seven_day_goal_cache_lua",tolua_logic_hld_cs2dp_save_role_seven_day_goal_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_harry_record_cache_lua",tolua_logic_hld_cs2dp_save_role_harry_record_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_time_limit_activity_cache_lua",tolua_logic_hld_cs2dp_save_role_time_limit_activity_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_gain_treasure_cache_lua",tolua_logic_hld_cs2dp_save_role_gain_treasure_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_service_rank_reward_cache_lua",tolua_logic_hld_cs2dp_save_role_service_rank_reward_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_highest_record_cache_lua",tolua_logic_hld_cs2dp_save_role_highest_record_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_yesterday_must_do_remain_cache_lua",tolua_logic_hld_cs2dp_save_role_yesterday_must_do_remain_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_daily_must_do_count_cache_lua",tolua_logic_hld_cs2dp_save_role_daily_must_do_count_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_talent_cache_lua",tolua_logic_hld_cs2dp_save_role_talent_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_title_cache_lua",tolua_logic_hld_cs2dp_save_role_title_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_welfare_cache_lua",tolua_logic_hld_cs2dp_save_role_welfare_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_active_degree_cache_lua",tolua_logic_hld_cs2dp_save_role_active_degree_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_convert_cache_lua",tolua_logic_hld_cs2dp_save_role_convert_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_worship_cache_lua",tolua_logic_hld_cs2dp_save_role_worship_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_pokedex_cache_lua",tolua_logic_hld_cs2dp_save_role_pokedex_cache_lua00);
   tolua_function(tolua_S,"cs2dp_save_role_tinder_cache_lua",tolua_logic_hld_cs2dp_save_role_tinder_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_achievement_cache_lua",tolua_logic_hld_cs2dp_save_role_achievement_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_special_name_cache_lua",tolua_logic_hld_cs2dp_save_role_special_name_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_service_goal_cache_lua",tolua_logic_hld_cs2dp_save_role_service_goal_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_map_record_cache_lua",tolua_logic_hld_cs2dp_save_role_map_record_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_goods_cache_lua",tolua_logic_hld_cs2dp_save_role_goods_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_mission_cache_lua",tolua_logic_hld_cs2dp_save_role_mission_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_money_cache_lua",tolua_logic_hld_cs2dp_save_role_money_cache_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"cs2dp_save_role_att_cache_lua",tolua_logic_hld_cs2dp_save_role_att_cache_lua00);
   tolua_function(tolua_S,"cs2dp_push_role_att_sql_add_callback",tolua_logic_hld_cs2dp_push_role_att_sql_add_callback00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"save_best_record_lua",tolua_logic_hld_save_best_record_lua00);
   tolua_function(tolua_S,"load_all_best_record_lua",tolua_logic_hld_load_all_best_record_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"load_all_gain_treasure_record_info_lua",tolua_logic_hld_load_all_gain_treasure_record_info_lua00);
   tolua_function(tolua_S,"save_gain_treasure_record_info_lua",tolua_logic_hld_save_gain_treasure_record_info_lua00);
   tolua_function(tolua_S,"save_world_boss_to_sql_lua",tolua_logic_hld_save_world_boss_to_sql_lua00);
   tolua_function(tolua_S,"load_world_boss_lua",tolua_logic_hld_load_world_boss_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"save_cross_server_pk_ws_info_lua",tolua_logic_hld_save_cross_server_pk_ws_info_lua00);
   tolua_function(tolua_S,"clear_cross_server_pk_ws_info_lua",tolua_logic_hld_clear_cross_server_pk_ws_info_lua00);
   tolua_function(tolua_S,"load_cross_server_pk_ws_info_lua",tolua_logic_hld_load_cross_server_pk_ws_info_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"save_cross_server_harry_ws_info_lua",tolua_logic_hld_save_cross_server_harry_ws_info_lua00);
   tolua_function(tolua_S,"load_cross_server_harry_ws_info_lua",tolua_logic_hld_load_cross_server_harry_ws_info_lua00);
   tolua_function(tolua_S,"load_harry_player_detail_lua",tolua_logic_hld_load_harry_player_detail_lua00);
   tolua_function(tolua_S,"del_one_server_msg_func_lua",tolua_logic_hld_del_one_server_msg_func_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"load_role_pk_top_lua",tolua_logic_hld_load_role_pk_top_lua00);
   tolua_function(tolua_S,"load_role_pk_info_lua",tolua_logic_hld_load_role_pk_info_lua00);
   tolua_function(tolua_S,"save_role_pk_info_lua",tolua_logic_hld_save_role_pk_info_lua00);
   tolua_function(tolua_S,"save_role_pk_honor_refresh_lua",tolua_logic_hld_save_role_pk_honor_refresh_lua00);
   tolua_function(tolua_S,"load_role_pk_log_lua",tolua_logic_hld_load_role_pk_log_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"save_red_package_to_sql_lua",tolua_logic_hld_save_red_package_to_sql_lua00);
   tolua_function(tolua_S,"load_red_package_by_sql_lua",tolua_logic_hld_load_red_package_by_sql_lua00);
   tolua_function(tolua_S,"load_red_package_receiver_by_sql_lua",tolua_logic_hld_load_red_package_receiver_by_sql_lua00);
   tolua_function(tolua_S,"add_red_package_to_sql_proc_lua",tolua_logic_hld_add_red_package_to_sql_proc_lua00);
   tolua_function(tolua_S,"add_red_package_receiver_to_sql_proc_lua",tolua_logic_hld_add_red_package_receiver_to_sql_proc_lua00);
   tolua_function(tolua_S,"del_red_package_by_sql_proc_lua",tolua_logic_hld_del_red_package_by_sql_proc_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"del_draw_info_from_db_lua",tolua_logic_hld_del_draw_info_from_db_lua00);
   tolua_function(tolua_S,"add_draw_info_to_db_lua",tolua_logic_hld_add_draw_info_to_db_lua00);
   tolua_function(tolua_S,"load_draw_info_by_db_lua",tolua_logic_hld_load_draw_info_by_db_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"save_big_player_to_sql_lua",tolua_logic_hld_save_big_player_to_sql_lua00);
   tolua_function(tolua_S,"load_big_player_lua",tolua_logic_hld_load_big_player_lua00);
   tolua_function(tolua_S,"load_big_player_detail_lua",tolua_logic_hld_load_big_player_detail_lua00);
  tolua_endmodule(tolua_S);
  tolua_module(tolua_S,"faith",0);
  tolua_beginmodule(tolua_S,"faith");
   tolua_function(tolua_S,"auction_add_item_into_db_proc_lua",tolua_logic_hld_auction_add_item_into_db_proc_lua00);
   tolua_function(tolua_S,"auction_find_buy_target_proc_lua",tolua_logic_hld_auction_find_buy_target_proc_lua00);
   tolua_function(tolua_S,"auction_find_cancel_target_proc_lua",tolua_logic_hld_auction_find_cancel_target_proc_lua00);
   tolua_function(tolua_S,"auction_find_bid_target_proc_lua",tolua_logic_hld_auction_find_bid_target_proc_lua00);
   tolua_function(tolua_S,"auction_bid_success_proc_lua",tolua_logic_hld_auction_bid_success_proc_lua00);
   tolua_function(tolua_S,"auction_get_and_clear_old_item_proc_lua",tolua_logic_hld_auction_get_and_clear_old_item_proc_lua00);
   tolua_function(tolua_S,"auction_add_share_list_proc_lua",tolua_logic_hld_auction_add_share_list_proc_lua00);
   tolua_function(tolua_S,"auction_add_trade_record_proc_lua",tolua_logic_hld_auction_add_trade_record_proc_lua00);
   tolua_function(tolua_S,"auction_req_share_list_proc_lua",tolua_logic_hld_auction_req_share_list_proc_lua00);
   tolua_function(tolua_S,"auction_check_legion_selling_proc_lua",tolua_logic_hld_auction_check_legion_selling_proc_lua00);
   tolua_function(tolua_S,"auction_del_bid_record_proc_lua",tolua_logic_hld_auction_del_bid_record_proc_lua00);
   tolua_function(tolua_S,"auction_clear_old_bid_record_proc_lua",tolua_logic_hld_auction_clear_old_bid_record_proc_lua00);
   tolua_function(tolua_S,"auction_update_sell_show_proc_lua",tolua_logic_hld_auction_update_sell_show_proc_lua00);
   tolua_function(tolua_S,"auction_time_out_proc_lua",tolua_logic_hld_auction_time_out_proc_lua00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_logic (lua_State* tolua_S) {
 return tolua_logic_open(tolua_S);
};
#endif

