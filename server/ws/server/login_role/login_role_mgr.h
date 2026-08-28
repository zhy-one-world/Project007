/*@@

	Copyright (c) Beijing Second Laboratory Game Studio. All rights reserved. 
	
	Created_datetime : 	2012-4-17 11:14
	
	File Name :	login_role_mgr.h
	
	Author : zhy 
	
	Description : 
	
	Change List :


@@*/

#ifndef _LOGIN_ROLE_MGR_H_
#define _LOGIN_ROLE_MGR_H_

//////////////////////////////////////////////////////////////////////////
//
//	File Include
//
//////////////////////////////////////////////////////////////////////////
#include "Logic/char_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/buff_def.hpp"
#include "Logic/login_def.hpp"

//////////////////////////////////////////////////////////////////////////
//
//	Class Declare
//
//////////////////////////////////////////////////////////////////////////
namespace hld
{
	struct s_login_info
	{
		s_client_uid		client_uid;
		s_unit_info			role_info;
		s_item_info			item_data[role_show_item_num];
		int32				buff_data[max_enum_buff_data_num];
		s_item_info			temp_sprite_info;
		int64				log_out_time;
		int32				appearance_info;
		bool				item_ok;
		bool				buff_ok;
		bool				spirit_ok;
		bool				time_ok;
		s_login_info()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_send()
		{
			return item_ok && buff_ok && spirit_ok && time_ok;
		}
	};
	typedef std::map<ui64, s_login_info> login_info_map;
	typedef login_info_map::iterator login_info_map_it;

	typedef std::map<std::string, int32> login_account_map;
	typedef login_account_map::iterator login_account_map_it;

	class login_role_mgr
	{
	public:
		static login_role_mgr& getInstance()
		{
			static login_role_mgr s_mgr;
			return s_mgr;
		}
	public:
		login_role_mgr();
		~login_role_mgr();
	public:
		void tick(float tick_time);
	public:
		void set_account_info(const std::string& account, int32 role_num);
		void set_role_info(const s_unit_info& role_info, const s_client_uid& client_uid);
		void set_item_data(const guid_64& role_guid, const s_item_info* item_array);
		void set_buff_data(const guid_64& role_guid, const int32* buff_array);
		void set_spirit_data(const guid_64& role_guid, const s_item_info& temp_spirit_info);
		void set_time_data(const guid_64& role_guid, const int64 login_out_time);
	public:
		void send_role_login_data(guid_64& role_guid);
	private:
		login_info_map m_login_info_map;
		login_account_map m_login_account_map;
	};
}


#endif