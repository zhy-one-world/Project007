/********************************************************************
	created:	2018年1月4日20:44:55
	file base:	data_manager
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DP_DATAMANAGER_HPP_
#define _DP_DATAMANAGER_HPP_

#include "logic/type_def.hpp"
#include "data_set.hpp"
#include "sql_builder.hpp"

namespace faith
{
	typedef std::map<int64, data_set> data_set_map;
	typedef data_set_map::iterator data_set_map_it;
	typedef std::map<int64, xstring> data_server_map;
	typedef data_server_map::iterator data_server_map_it;
	typedef std::map<xstring, xstring> data_account_map;
	typedef data_account_map::iterator data_account_map_it;

	typedef std::map<xstring, int32> enum_character_map;
	typedef std::vector<s_unit_info_db_check> s_unit_info_arr;
	typedef std::map<xstring, s_unit_info_arr> enum_character_unit_info_map;

	typedef std::map<xstring, int32> gm_query_map;
	typedef std::map<xstring, std::vector<s_unit_info>> gm_query_unit_info_map;

	const int32 role_guid_len = 10000;
	const int32 account_len = 3000;
	void init_all_temp_end(db_result_type result);
	class data_manager
	{
	private:
		data_manager();
	public:
		~data_manager();
	public:
		static data_manager& get_instance()
		{
			static data_manager s_data_manager;
			return s_data_manager;
		}
	public:
		void clear_data();
		void init();
	public:
		static xstring get_table_name(int64 data_type);
		void del_data_set(const guid_64& role_guid);
		data_set* add_data_set(const guid_64& role_guid);
		data_set* get_data_set(const guid_64& role_guid);
		int32 get_data_set_num() { return m_data_set_map.size(); }
		int32 get_data_server_num() { return m_data_server_map.size(); }
		int32 get_data_account_num() { return m_data_account_map.size(); }
		int32 get_enum_char_num() { return m_enum_char_map.size(); }
		int32 get_unit_info_arr_map_num() { return m_unit_info_arr_map.size(); }
		void add_data_server(const int64& data_guid, int32 server_id);
		xstring get_db_name(const int64& data_guid);
		void add_data_server(const xstring& account, int32 server_id);
		xstring get_db_name(const xstring& account);
		int32 is_new_account(const xstring& account);
		
		bool mark_enum_character(const xstring& account);
		void add_unit_info_with_client_uid(const xstring& account,const s_unit_info_db_check& unit_info);
		const std::vector<s_unit_info_db_check>& get_unit_info_arr_with_client_uid(const xstring& account);
		void clear_all_with_client_uid(const xstring& account);

		//gm
		bool check_query_all_db(const xstring& role_mark);
		void add_unit_info_gm_qeury(const xstring& role_mark, const s_unit_info& unit_info);
		void reset_query_data(const xstring& role_mark);
		std::vector<s_unit_info> get_gm_query_data(const xstring& role_mark);
	private:
		void init_all_temp();
		void init_time_limit_activity_template();
		void init_marry_info();
		void clear_merge_template();
		void init_role_guid();
		void init_role_guid(xstring db_name, int32 server_id, int32 begin_index);
		void init_role_guid_end(db_result_type result, xstring db_name, int32 server_id, int32 end_index);
		void init_legion_guid();
		void init_legion_guid_end(db_result_type result, int32 server_id);
		void init_all_account();
		void init_all_account(xstring db_name, int32 server_id, int32 begin_index);
		void init_all_account_end(db_result_type result, xstring db_name, int32 server_id, int32 end_index);
		
		void del_character_with_max(const s_unit_info_db_check& unit_info_db);
		void del_character_with_max_end(db_result_type result);
	private:
		data_set_map m_data_set_map;
		data_server_map m_data_server_map;
		data_account_map m_data_account_map;
		enum_character_map m_enum_char_map;//用于遍历所有库查找角色时置标记位
		enum_character_unit_info_map m_unit_info_arr_map;//用于记录对应玩家的角色数据
		s_unit_info_arr m_empty_unit_info_arr;//空的用于返回

		gm_query_map  m_gm_query_map;	  //gm查询数据库计数
		gm_query_unit_info_map m_gm_query_unit_info_map; //gm查询角色数据内容
	};

}

#endif
