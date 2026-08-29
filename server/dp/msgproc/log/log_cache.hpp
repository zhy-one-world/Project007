#ifndef _LOG_CACHE_H_
#define _LOG_CACHE_H_

#include "logic/type_def.hpp"
#include "singleton.hpp"


namespace faith
{
	struct Table_Filed
	{
		const xstring* fileds;
		int32 filed_len;
	};

	struct db_result_type;

	class log_cache : public singleton<log_cache>
	{
		friend class singleton<log_cache>;

		typedef std::map<xstring, const Table_Filed*>			table_fileds_map;
		typedef table_fileds_map::iterator						table_fileds_map_it;
		typedef std::queue<xstring>								log_queue;
		typedef std::map<xstring, log_queue>					table_logs_map;
		typedef table_logs_map::iterator						table_logs_map_it;

	protected:
		log_cache();
	public:
		virtual ~log_cache();
	public:
		void	init();
		bool	push_log_to_cache(const xstring& table_name, const xstring& log_string);
		void	test_write_log();

	private:
		void	write_log_cache_to_db();
		void	write_log_cache_to_db_end(db_result_type result, xstring sql_str);

		void	init_ranking_list_log_table();
		void	init_ranking_list_log_table_end(db_result_type result);

		void	test_write_loginout_log(int day, int account_idx);
		void	test_write_create_role_log(int day, int account_idx);
		void	test_write_level_change_log();
		void	test_write_task_log();
		void	test_write_money_change_log();
		void	test_write_new_guide_log();
		void	test_write_recharge_step_log();
		void	test_write_player_count_log();
		void	test_write_recharge_success_log(int day, int account_idx, int is_first);

	private:
		table_fileds_map	m_table_fileds_map;
		table_logs_map		m_table_logs_map;
		uint32				m_write_db_timerindex;
		uint32				m_test_write_count;
		uint32				m_write_db_count;
		int64				m_last_write_time;
		int32				m_ranking_table_index;
	};
}

#endif