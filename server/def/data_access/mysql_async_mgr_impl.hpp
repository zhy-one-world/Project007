/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:29
	file base:	mysql_async_mgr_impl
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MYSQL_ASYNC_MGR_IMPL_H_
#define _MYSQL_ASYNC_MGR_IMPL_H_

#include <mysqld_error.h>
#include "mysql_async_mgr.hpp"
#include "mysql_driver.hpp"
#include "mysql_worker_thread.hpp"
#include "recvbuffer.hpp"
#include "logic/type_def.hpp"

namespace hld
{
	struct db_result_type;
	struct mysql_conn_params;
	struct mysql_conn_info;

	class mysql_async_mgr_impl : public boost::noncopyable
	{
		typedef std::pair<int64,uint64_t>			query_key;
		typedef std::queue<db_query_type>			query_queue;
		typedef std::map<query_key, query_queue>	query_map;
		typedef query_map::iterator					query_iter;
		friend class								worker_thread;
	public:
		mysql_async_mgr_impl() { m_server_group_flag = 0; };
	public:
		bool										init( size_t workerthread_num, mysql_conn_params conn_params, int32 max_rows, int32 max_row_size, uint32 server_group_flag );
		void										release(	);
		void										add_query( db_query_type& query );
		void										add_queryex( db_query_type& query );
		bool										empty(	);
		uint32										escape_string( xchar *to, const xchar* from, uint32 length );
	private:
		// for worker-thread to call
		bool										get_one_query( db_query_type* query, boost::uint32_t& mlb_id );
		bool										mlb_init( size_t workerthread_num, xstring host, xstring user, xstring password, xstring db, uint32 port, int32 max_rows, int32 max_row_size );
		void										mlb_release(	);
		void										recur_add_query( boost::uint32_t instance_id, db_callback_handler_type handler, const xstring& dbresult, const xstring& data, const xstring& fields );
		void										call_add_query( int64 logic_id, boost::uint64_t role_guid, boost::uint32_t instance_id, db_callback_handler_type handler, db_result_type result );
		const xstring&								mlb_escape_string( const xstring& from );
	private:
		query_queue									m_querys;
		query_map									m_querys_ex;
		boost::recursive_mutex						m_querys_mutex;
		boost::recursive_mutex						m_querys_mutex_ex;
		std::list<class worker_thread*>				m_thread_ptrs;
		std::vector<mysql_conn_info>				m_conn_info;
		std::queue<boost::uint32_t>					m_mlb_ids;
		uint32										m_server_group_flag;
	};
}

#endif
