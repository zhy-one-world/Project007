/********************************************************************
created:	2018年9月6日11:19:20
file base:	db_manager
file ext:	hpp
author:		zhy

purpose:
*********************************************************************/

#include "db_manager.hpp"
#include "server_log.hpp"
#include "data_manager/data_manager.hpp"
#include "sql_builder.hpp"
#include <rlog.hpp>

namespace faith
{
	db_manager::db_manager()
	{
		m_db_error = 0;
		auto db_game_config = DPCONFIG->m_db_info_map[0];
		m_game_db_name = db_game_config.schema_name;
	}
	db_manager::~db_manager()
	{
		m_main_db_link.game_db.release();
	}
	void db_manager::start()
	{

	}
	bool db_manager::add_db_link(const s_db_conn_info& db_conn_info)
	{
		db_conn_params game_conn_params;
		game_conn_params.host = db_conn_info.game_ip;
		game_conn_params.port = db_conn_info.game_port;
		game_conn_params.user = db_conn_info.game_user_name;
		game_conn_params.password = db_conn_info.game_user_password;;
		game_conn_params.db = db_conn_info.game_name;
		if (false == m_main_db_link.game_db.init(MYSQL_CONNECT_SCHEDULER_THREAD_COUNT, game_conn_params, 0))
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("add_db_link game_db is fail host:{} port:{} user:{} password:{} db:{}",  db_conn_info.game_ip,  db_conn_info.game_port,  db_conn_info.game_user_name,  db_conn_info.game_user_password,  db_conn_info.game_name));
			return false;
		}
		db_conn_params log_conn_params;
		log_conn_params.host = db_conn_info.log_ip;
		log_conn_params.port = db_conn_info.log_port;
		log_conn_params.user = db_conn_info.log_user_name;
		log_conn_params.password = db_conn_info.log_user_password;;
		log_conn_params.db = db_conn_info.log_name;
		if (false == m_main_db_link.log_db.init(MYSQL_CONNECT_SCHEDULER_THREAD_COUNT, log_conn_params, 0))
		{
			_RLOG_(MINFO, ::faith::log_detail::format_message("add_db_link log_db is fail host:{} port:{} user:{} password:{} db:{} ",  db_conn_info.log_ip,  db_conn_info.log_port,  db_conn_info.log_user_name,  db_conn_info.log_user_password,  db_conn_info.log_name));
			return false;
		}

		m_db_conn_info = db_conn_info;
		m_db_name_map.clear();
		m_old_db_name_map.clear();
		db_manager::getInstance().ping_db();
		return true;
	}
	s_db_link& db_manager::get_db_link()
	{
		return m_main_db_link;
	}

	void db_manager::get_cur_db_name_map()
	{
		return;
		//game
		{
			sql_builder sql_user(get_db_link().game_db);
			sql_user << _XTEXT("call sp_get_databases_name('") << m_game_db_name << "%');";

			db_query_type query_account;
			query_account.sql_str.assign(sql_user.c_str(), sql_user.size());
			query_account.handler = boost::bind(&db_manager::get_cur_db_name_map_end, this, _1, DB_TYPE_GAME);
			get_db_link().game_db.add_query(query_account);
		}
	}

	void db_manager::get_cur_db_name_map_end(db_result_type result, ui32 db_type)
	{
		if (result.error)
		{
			return;
		}
		else
		{
			if (m_db_name_map.empty())
			{
				int32 data_num = result.query.data_select.row_count;
				if (data_num > 0)
				{
					struct s_db_name
					{
						xchar db_name[30 + 1];
						int32 server_id;
						s_db_name()
						{
							memset(this, 0, sizeof(s_db_name));
						}
					};
					s_db_name* db_info = new s_db_name[data_num];
					int32 data_size = result.query.data_select.row_size * data_num;
					db_read_data(&result, db_info, data_size);
					for (int i = 0; i < data_num; i++)
					{
						m_db_name_map[db_info[i].server_id] = db_info[i].db_name;
					}
					delete[] db_info;
					data_manager::get_instance().init();
				}
			}
		}
	}

	bool db_manager::is_have_this_db_name_in_map(int32 server_id)
	{
		return true;
		db_name_map::iterator ite = m_db_name_map.find(server_id);
		if (ite == m_db_name_map.end())
		{
			return false;
		}
		return true;
	}

	bool db_manager::compare_is_db_name_change()
	{
		return false;
		if (m_db_name_map.size() <= 0 || m_old_db_name_map.size() <= 0)
		{
			return false;
		}
		if (m_db_name_map.size() != m_old_db_name_map.size())
		{
			return true;
		}
		db_name_map::iterator ite;
		for (ite = m_db_name_map.begin(); ite != m_db_name_map.end();++ite)
		{
			if (m_old_db_name_map.find(ite->first) == m_old_db_name_map.end())
			{
				return true;
			}
			if (m_old_db_name_map.find(ite->first)->second != ite->second)
			{
				return true;
			}
		}
		return false;
	}

	void db_manager::ping_db()
	{
		{
			sql_builder sql_user(get_db_link().game_db);
			sql_user << _XTEXT("select 1");

			db_query_type query_account;
			query_account.sql_str.assign(sql_user.c_str(), sql_user.size());
			query_account.handler = boost::bind(&db_manager::ping_db_end, this, _1, DB_TYPE_GAME);
			get_db_link().game_db.add_query(query_account);
		}
	}

	void db_manager::ping_db_end(db_result_type result, ui32 db_type)
	{
		if (result.error)
		{
			m_db_error++;
			if (m_db_error >= 10)
			{
				_RLOG_(MINFO, ::faith::log_detail::format_message("dp stop self! error_count:{} error_id:{}",  m_db_error,  result.error));
				m_main_db_link.game_db.release();
				m_main_db_link.log_db.release();
				add_db_link(m_db_conn_info);
			}
		}
		else
		{
			m_db_error = 0;
			if (m_old_db_name_map.empty())
			{
				m_old_db_name_map[SERVERCONFIG->game_id] = m_game_db_name;
				m_db_name_map[SERVERCONFIG->game_id] = m_game_db_name;
				data_manager::get_instance().init();
			}
		}
	}

	xstring db_manager::get_db_names()
	{
		xstring db_names;
		int32 game_count = 0;
		for (auto ite = m_db_name_map.begin(); ite != m_db_name_map.end();++ite)
		{
			db_names = db_names + ite->second;
			if (game_count < m_db_name_map.size() - 1)
			{
				db_names = db_names + ",";
			}

			game_count++;
		
		}

		return db_names;
	}
}
