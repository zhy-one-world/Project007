/********************************************************************
  created: 2019-09-24
  file base: web_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _WEB_DEF_HPP_
#define _WEB_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "item_def.hpp"
#include "mail_def.hpp"

namespace hld
{
#pragma pack(push,1)

	const  int32 ROLE_INFO_MAP_MAX_SIZE = 150;
	struct s_gm_info
	{
		s_unit_info		role_info;
		s_time_info		time_info;
		s_money_info	money_info;
		int32			is_login;
		int32			is_send_chat;
		bool			time_ok;
		bool			money_ok;
		bool			lock_ok;
		int32			main_mission_id;
		s_gm_info()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_send()
		{
			return money_ok && time_ok && lock_ok;
		}
	};



	typedef std::unordered_map<ui64, s_gm_info> gm_info_map;
	typedef gm_info_map::iterator gm_info_map_it;
	struct s_gm_all_info
	{
		gm_info_map info_map;
		int32		check_type;
		int64		time;
	};
	typedef std::unordered_map<ui64, s_gm_all_info> gm_all_info_map;
	typedef gm_all_info_map::iterator gm_all_info_map_it;

	typedef std::unordered_map<ui64, xstring> gm_mod_role_info_map;
	typedef gm_mod_role_info_map::iterator gm_mod_role_info_map_it;

	enum e_web_proto_id
	{
		e_ws2w_pi_login = 1,
		e_ws2w_pi_json_str = 2,
		e_ws2w_pi_activate_code = 3,
		e_ws2w_pi_server_state = 4,
		e_ws2w_pi_server_dump_notice = 5,
		e_w2ws_pi_login = 6,
		e_w2ws_pi_json_str = 7
	};

	enum e_web_json_type
	{
		e_web_json_type_role_info = 1,
		e_web_json_type_send_chat,				//禁言
		e_web_json_type_kickout,				//踢登陆
		e_web_json_type_send_mail,				//发送邮件
		e_web_json_type_char_info_modify,
		//e_web_json_type_money_info_modify,
		e_web_json_type_role_item_info,
		e_web_json_type_role_item_modify,
		e_web_json_type_marquee,				//发送公告
		e_web_json_type_textcommand,			//自定义指令
		e_web_json_type_activate_code,			//发放礼包	
		e_web_json_type_robot_op,
		e_web_json_type_recharge,				//充值
		e_web_json_type_open_time,				//设置开服时间
		e_web_json_type_new_password,			//修改密码
		e_web_json_type_ban_login,				//封号
	};
#pragma pack(pop)
}

#endif
