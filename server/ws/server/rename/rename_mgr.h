/********************************************************************
created: 2018-8-24 16:45:37
file base: 
file ext: h
author: dengronggen

purpose: about role's treasure_mgr
*********************************************************************/
#ifndef _RENAME_MGR_H_
#define _RENAME_MGR_H_
#include "logic/type_def.hpp"

namespace faith
{
	class client_session;

	class rename_mgr
	{
	public:
		rename_mgr();
		~rename_mgr();
	public:
		static rename_mgr& get_instance()
		{
			static rename_mgr s_reanme_mgr;
			return s_reanme_mgr;
		}
		void heart_tick(const int64& new_time);
		void clear_data(void);
	public:
		void add_want_use_name(const xstring& name, uint64 role_guid);
		void del_want_use_name(const xstring& name);
		void give_back_rename_card(uint64 role_guid);
		bool is_in_want_use_name(const xstring& name);
		void get_name_in_want_use_name(const uint64& role_guid, xstring& name);

	public:
		bool change_player_name(xstring name, guid_64 role_guid); //修改玩家名字
		void sub_rename_item_end(int32 ret, guid_64 role_guid, xstring& want_name, xstring& original_name);
		void check_role_name_end(uint64 role_guid, xstring role_name, int32 result); //检查名字存在否
		void change_player_name_end(uint64 role_guid, xstring role_name);
		void confirm_change_name(bool confirm_type, guid_64 role_guid, const xstring& role_name);
	public:
		void check_name_is_avaliable(const xstring& role_name, uint32& check_ret, const uint64& role_guid);
	public:
		bool is_use_lua();
	private:
		std::map<xstring,uint64>		m_want_use_names;							 //限制的名字-----role_guid
		
	};
}

#endif
