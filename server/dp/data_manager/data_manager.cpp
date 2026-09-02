/********************************************************************
	created:	2018年1月4日20:45:59
	file base:	data_manager
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "data_manager.hpp"
#include "time.hpp"
#include "Logic/count_def.hpp"
#include "game_cfg/servers_config.h"
#include "db_manager.hpp"
#include "dbproxy_service.hpp"
#include "legion_def.hpp"
#include <rlog.hpp>
namespace faith
{
	std::unordered_map<int64, xstring> g_table_map;

	data_manager::data_manager()
	{
		clear_data();
	}

	data_manager::~data_manager()
	{
	}
	void data_manager::clear_data()
	{
		m_data_set_map.clear();
		m_data_server_map.clear();
		m_enum_char_map.clear();
		m_unit_info_arr_map.clear();
		m_empty_unit_info_arr.clear();
	}
	void data_manager::init()
	{
		g_table_map[e_data_flag_logic_info] = "role_logic";
		g_table_map[e_data_flag_time] = "role_time";
		g_table_map[e_data_flag_map] = "role_map_record";
		g_table_map[e_data_flag_time_activity] = "role_time_activity";
		g_table_map[e_data_flag_sprite_fetter] = "role_sprite_fetter";
		g_table_map[e_data_flag_passive_skill] = "role_passive_skill";
		g_table_map[e_data_flag_item_upgrade] = "role_item_upgrade";
		for (int32 i = e_bag_type_none + 1; i < e_bag_type_max; ++i)
		{
			g_table_map[e_data_flag_item + i] = "role_item" + boost::lexical_cast<std::string>(i);
		}

		if (db_manager::getInstance().compare_is_db_name_change())//发现是合服则需要执行的逻辑
		{
			init_time_limit_activity_template();
			init_marry_info();
			init_all_temp();
			if (db_manager::getInstance().get_old_data_name_map().size() > 0)//新服不执行
			{
				clear_merge_template();
			}
		}
		else
		{
			if (!dbproxy_service::getInstance().start())
			{
				_RLOG_(MINFO, "dbproxy_service start false ");
				dbproxy_service::getInstance().stop();
				return;
			}
		}
		init_role_guid();
		//init_legion_guid();
		init_all_account();
	}
	xstring data_manager::get_table_name(int64 data_type)
	{
		auto it = g_table_map.find(data_type);
		if (it != g_table_map.end())
		{
			return it->second;
		}
		return xstring();
	}

	void data_manager::clear_merge_template()
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << it->second << ".sp_merge_server_clear();";
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}

	void data_manager::init_time_limit_activity_template()
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			if (it->first != SERVERCONFIG->game_id)
			{
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("call ") << it->second << ".sp_server_merge_time_limit_temp('" << db_manager::getInstance().get_game_db_name() << "');";
				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				db_manager::getInstance().get_db_link().game_db.add_query(query);
			}
		}
	}

	void data_manager::init_marry_info()
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			if (it->first != SERVERCONFIG->game_id)
			{
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("call ") << it->second << ".sp_server_merge_marry_info('" << db_manager::getInstance().get_game_db_name() << SERVERCONFIG->game_id << "');";
				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				db_manager::getInstance().get_db_link().game_db.add_query(query);
			}
		}
	}

	void init_all_temp_end(db_result_type result)
	{
		if (result.error)
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("init_all_temp_end false ",  result.error));
			dbproxy_service::getInstance().stop();
			return;
		}
		else
		{
			_RLOG_(MINFO, "init_all_temp_end ");
			if (!dbproxy_service::getInstance().start())
			{
				_RLOG_(MINFO, "dbproxy_service start false ");
				dbproxy_service::getInstance().stop();
				return;
			}
		}
	}

	void data_manager::init_all_temp()
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			if (it->first != SERVERCONFIG->game_id)
			{
				sql_builder sql(db_manager::getInstance().get_db_link().game_db);
				sql << _XTEXT("call ") << it->second << ".sp_server_merge_auction('" << db_manager::getInstance().get_game_db_name() << "');";
				db_query_type query;
				query.sql_str.assign(sql.c_str(), sql.size());
				query.handler = boost::bind(&init_all_temp_end, _1);
				db_manager::getInstance().get_db_link().game_db.add_query(query);
			}
		}
	}

	void data_manager::init_all_account()
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			init_all_account(it->second, it->first, 0);
		}
	}

	void data_manager::init_all_account(xstring db_name, int32 server_id, int32 begin_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_name << ".sp_load_all_account(" << begin_index << "," << account_len << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&data_manager::init_all_account_end, this, _1, db_name, server_id, begin_index + account_len);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void data_manager::init_all_account_end(db_result_type result, xstring db_name, int32 server_id, int32 end_index)
	{
		if (result.error)
		{
			return;
		}
		int32 data_num = result.query.data_select.row_count;
		if (data_num <= 0)
		{
			return;
		}
		struct cur_account_data_info
		{
			xchar account[max_account_length + 1];
			cur_account_data_info()
			{
				clear_data();
			}
			void clear_data()
			{
				memset(this, 0, sizeof(*this));
			}
		};
		cur_account_data_info* data_info = new cur_account_data_info[account_len];
		if (data_num > account_len)
		{
			data_num = account_len;
		}
		int32 data_size = result.query.data_select.row_size * data_num;
		db_read_data(&result, data_info, data_size);
		for (int32 i = 0; i < data_num; ++i)
		{
			add_data_server(data_info[i].account, server_id);
		}
		if (data_num == account_len)
		{
			init_all_account(db_name, server_id, end_index);
		}
		delete[] data_info;
	}

	void data_manager::init_role_guid()
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			init_role_guid(it->second, it->first, 0);
		}
	}
	void data_manager::init_role_guid(xstring db_name, int32 server_id, int32 begin_index)
	{
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_name << ".sp_load_all_role_guid(" << begin_index <<","<< role_guid_len << ");";
		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&data_manager::init_role_guid_end, this, _1, db_name, server_id, begin_index + role_guid_len);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}
	void data_manager::init_role_guid_end(db_result_type result, xstring db_name, int32 server_id, int32 end_index)
	{
		if (result.error)
		{
			return;
		}
		int32 data_num = result.query.data_select.row_count;
		if (data_num <= 0)
		{
			return;
		}
		guid_64* data_info = new guid_64[role_guid_len];
		if (data_num > role_guid_len)
		{
			data_num = role_guid_len;
		}
		int32 data_size = result.query.data_select.row_size * data_num;
		db_read_data(&result, data_info, data_size);
		for (int32 i = 0 ; i < data_num; ++i)
		{
			add_data_server(data_info[i], server_id);
		}
		if (data_num == role_guid_len)
		{
			init_role_guid(db_name, server_id, end_index);
		}
		delete[] data_info;
	}
	void data_manager::init_legion_guid()
	{
		db_name_map& db_map = db_manager::getInstance().get_data_name_map();
		for (db_name_map_it it = db_map.begin(); it != db_map.end(); ++it)
		{
			sql_builder sql(db_manager::getInstance().get_db_link().game_db);
			sql << _XTEXT("call ") << it->second << ".sp_load_all_legion_guid();";
			db_query_type query;
			query.sql_str.assign(sql.c_str(), sql.size());
			query.handler = boost::bind(&data_manager::init_legion_guid_end, this, _1, it->first);
			db_manager::getInstance().get_db_link().game_db.add_query(query);
		}
	}
	void data_manager::init_legion_guid_end(db_result_type result, int32 server_id)
	{
		if (result.error)
		{
			return;
		}
		int32 data_num = result.query.data_select.row_count;
		if (data_num <= 0)
		{
			return;
		}
		guid_64 data_info[max_legion_count];
		if (data_num > max_legion_count)
		{
			data_num = max_legion_count;
		}
		int32 data_size = result.query.data_select.row_size * data_num;
		db_read_data(&result, data_info, data_size);
		for (int32 i = 0; i < data_num; ++i)
		{
			add_data_server(data_info[i], server_id);
		}
	}
	void data_manager::del_data_set(const guid_64& role_guid)
	{
		data_set_map_it it = m_data_set_map.find(role_guid.server_64);
		if (it != m_data_set_map.end())
		{
			m_data_set_map.erase(it);
		}
	}
	data_set* data_manager::add_data_set(const guid_64& role_guid)
	{
		//return nullptr;
		if (m_data_set_map.size() >= max_save_data_count)
		{
			data_set_map_it old_it;
			int64 last_time = utility::get_tick_count();
			data_set_map_it begin_it = m_data_set_map.begin();
			data_set_map_it end_it = m_data_set_map.end();
			for (data_set_map_it it = begin_it; it != end_it; ++it)
			{
				if (it ->second.get_save_time_last() < last_time)
				{
					old_it = it;
					last_time = it->second.get_save_time_last();
				}
			}
			if (old_it != end_it)
			{
				m_data_set_map.erase(old_it);
			}
		}
		
		return &(m_data_set_map[role_guid.server_64]);
	}
	data_set* data_manager::get_data_set(const guid_64& role_guid)
	{
		data_set_map_it it = m_data_set_map.find(role_guid.server_64);
		if (it == m_data_set_map.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	void data_manager::add_data_server(const int64& data_guid, int32 server_id)
	{
		xstring game_name = db_manager::getInstance().get_game_db_name();
		m_data_server_map[data_guid] = game_name;
	}

	void data_manager::add_data_server(const xstring& account, int32 server_id)
	{
		xstring game_name = db_manager::getInstance().get_game_db_name();
		m_data_account_map[account] = game_name;
	}

	xstring data_manager::get_db_name(const int64& data_guid)
	{
		data_server_map_it it = m_data_server_map.find(data_guid);
		if (it != m_data_server_map.end())
		{
			return it->second;
		}
		xstring game_name = db_manager::getInstance().get_game_db_name();
		return game_name;
	}

	xstring data_manager::get_db_name(const xstring& account)
	{
		data_account_map_it it = m_data_account_map.find(account);
		if (it != m_data_account_map.end())
		{
			return it->second;
		}
		xstring game_name = db_manager::getInstance().get_game_db_name();
		return game_name;
	}

	int32 data_manager::is_new_account(const xstring& account)
	{
		data_account_map_it it = m_data_account_map.find(account);
		if (it != m_data_account_map.end())
		{
			return 0;
		}
		return 1;
	}

	bool data_manager::mark_enum_character(const xstring& account)
	{
		enum_character_map::iterator ite = m_enum_char_map.find(account);
		if (ite == m_enum_char_map.end())
		{
			m_enum_char_map[account] = 0;
		}
		m_enum_char_map[account] ++;
		if (m_enum_char_map[account] >= db_manager::getInstance().get_data_name_map_size())
		{
			return true;
		}
		return false;
	}
	const std::vector<s_unit_info_db_check>& data_manager::get_unit_info_arr_with_client_uid(const xstring& account)
	{
		enum_character_unit_info_map::iterator ite = m_unit_info_arr_map.find(account);
		if (ite != m_unit_info_arr_map.end())
		{
			return ite->second;
		}
		return m_empty_unit_info_arr;
	}

	void data_manager::clear_all_with_client_uid(const xstring& account)
	{
		m_enum_char_map[account] = 0;
		m_unit_info_arr_map[account].clear();
	}

	void data_manager::add_unit_info_with_client_uid(const xstring& account, const s_unit_info_db_check& unit_info)
	{
		bool is_insert = false;
		enum_character_unit_info_map::iterator ite = m_unit_info_arr_map.find(account);
		if (ite != m_unit_info_arr_map.end())
		{
			if (m_unit_info_arr_map[account].size() >= max_character_num)
			{
				for (int32 i = 0; i < max_character_num; i++)
				{
					if (m_unit_info_arr_map[account][i].get_gs_value() <= unit_info.data_i64_ary[e_role_i64_info_gs_value])
					{
						m_unit_info_arr_map[account].insert(m_unit_info_arr_map[account].begin() + i, unit_info);
						break;
					}
				}
				if (m_unit_info_arr_map[account].size() > max_character_num)
				{
					for (int32 i = max_character_num; i < m_unit_info_arr_map[account].size(); i++)
					{
						del_character_with_max(m_unit_info_arr_map[account][i]);
					}
					m_unit_info_arr_map[account].erase(m_unit_info_arr_map[account].begin() + max_character_num, m_unit_info_arr_map[account].end());
				}
			}
			else if (m_unit_info_arr_map[account].size() > 0 && m_unit_info_arr_map[account].size() < max_character_num)
			{
				for (int32 i = 0; i < m_unit_info_arr_map[account].size(); i++)
				{
					if (m_unit_info_arr_map[account][i].get_gs_value() <= unit_info.data_i64_ary[e_role_i64_info_gs_value])
					{
						m_unit_info_arr_map[account].insert(m_unit_info_arr_map[account].begin() + i, unit_info);
						is_insert = true;
						break;
					}				
				}
				if (!is_insert)
				{
					m_unit_info_arr_map[account].push_back(unit_info);
				}
			}
			else
			{
				m_unit_info_arr_map[account].push_back(unit_info);
			}
		}
		else
		{
			m_unit_info_arr_map[account].clear();
			m_unit_info_arr_map[account].push_back(unit_info);
		}
	}

	void data_manager::del_character_with_max(const s_unit_info_db_check& unit_info_db)
	{
		if (!unit_info_db.role_guid.is_valid())
		{
			return;
		}
		sql_builder sql(db_manager::getInstance().get_db_link().game_db);
		sql << _XTEXT("call ") << db_manager::getInstance().get_game_db_name() << _XTEXT(".sp_login_del_role('") << unit_info_db.account << _XTEXT("',") <<unit_info_db.role_guid.server_64 << _XTEXT(",") << (int32)e_del_char_delete << _XTEXT(")");

		db_query_type query;
		query.sql_str.assign(sql.c_str(), sql.size());
		query.handler = boost::bind(&data_manager::del_character_with_max_end, this, _1);
		db_manager::getInstance().get_db_link().game_db.add_query(query);
	}

	void data_manager::del_character_with_max_end(db_result_type result)
	{

	}


	bool data_manager::check_query_all_db(const xstring& role_mark)
	{
		m_gm_query_map[role_mark]++;

		int32 query_num = 0;
		auto iter = m_gm_query_map.find(role_mark);
		if (iter != m_gm_query_map.end())
		{
			query_num = iter->second;
		}

		if (query_num >= db_manager::getInstance().get_data_name_map_size())
		{
			return true;
		}

		return false;
	}
	void data_manager::add_unit_info_gm_qeury(const xstring& role_mark, const s_unit_info& unit_info)
	{
		m_gm_query_unit_info_map[role_mark].push_back(unit_info);
	}

	void data_manager::reset_query_data(const xstring& role_mark)
	{
		m_gm_query_map[role_mark] = 0;
		m_gm_query_unit_info_map[role_mark].clear();
	}

	std::vector<s_unit_info> data_manager::get_gm_query_data(const xstring& role_mark)
	{
		auto iter =  m_gm_query_unit_info_map.find(role_mark);
		if (iter == m_gm_query_unit_info_map.end())
		{
			return std::vector<s_unit_info>();
		}

		return iter->second;		
	}
}