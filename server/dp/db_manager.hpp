/********************************************************************
created:	2018年9月6日11:19:20
file base:	db_manager
file ext:	hpp
author:		zhy

purpose:
*********************************************************************/
#ifndef _DB_MANAGER_H_
#define _DB_MANAGER_H_

#include "logic/type_def.hpp"
#include "server_def.hpp"
#include "sql_builder.hpp"
#include <singleton.hpp>

namespace faith
{
#define DB_TYPE_LOG		0
#define DB_TYPE_GAME	1

	enum e_data_flag_type
	{
		e_data_flag_type_role_ranking = 1,
		e_data_flag_type_legion_applicant,
		e_data_flag_type_time_limit_activity_ws_info,
		e_data_flag_type_time_limit_activity_template,
	};
	struct s_db_link
	{
		db_accessor		game_db;
		db_accessor		log_db;
		s_db_link()
		{

		}
		~s_db_link()
		{
			game_db.release();
		}
	};
	typedef std::map<int32, std::string> db_name_map;
	typedef db_name_map::iterator db_name_map_it;
	class db_manager : public singleton<db_manager>
	{
		friend class singleton<db_manager>;
	protected:
		db_manager();
		virtual ~db_manager();
		void ping_db_end(db_result_type result, ui32 db_type);

	public:
		bool add_db_link(const s_db_conn_info& server_config);
		s_db_link& get_db_link();
		void start();
		void ping_db();
		void get_cur_db_name_map();
		void get_cur_db_name_map_end(db_result_type result, ui32 db_type);
		const std::string& get_game_db_name() { return m_game_db_name; }
		db_name_map& get_data_name_map() { return m_db_name_map; }
		db_name_map& get_old_data_name_map() { return m_old_db_name_map; }
		bool compare_is_db_name_change();
		int32 get_data_name_map_size() { return m_db_name_map.size(); }
		bool is_have_this_db_name_in_map(int32 server_id);
		xstring get_db_names();
	private:
		s_db_conn_info m_db_conn_info;
		s_db_link m_main_db_link;
		int32	m_db_error;
		db_name_map m_db_name_map;
		db_name_map m_old_db_name_map;
		std::string m_game_db_name;

	};
}

#endif
