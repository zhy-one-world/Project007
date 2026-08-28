/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:24
	file base:	mysql_driver
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MYSQL_DRIVER_H_
#define _MYSQL_DRIVER_H_

#include "db_config.hpp"
#include "mysql_async_mgr_impl.hpp"
#include "recvbuffer.hpp"

namespace hld
{
	struct mysql_conn_params;
	struct db_result_type;
	struct db_query_type;

	class mysql_driver
	{
	public:
		mysql_driver( int32 max_rows = MAX_ROWS_LIMIT, int32 max_row_size = MAX_ROW_SIZE_LIMIT );
		~mysql_driver(	);
	public:
		bool			connected(	);
		bool			connect( mysql_conn_params& params );
		void			disconnect(	);
		void			execute( db_query_type& query, db_result_type& result );
		int32				batch_query( db_query_type& query, db_result_type& result );
		uint32	escape_string( xchar *to, const xchar *from, uint32 length );
		void			thread_start(	);
		void			thread_end(	);
		const xchar *	last_error(	);
		uint32	last_errno(	)	{ return mysql_stmt_errno(mysql_stmt_); };
		int32				ping(	);
		bool			inner_execute( const xchar * sql, uint32 len );
		void			inner_next_result(	)	{ mysql_next_result(&mysql_); };
	private:
		inline int32		get_type_size( MYSQL_FIELD* field, int32& real_len );
		int32				_stmt_prepare( db_query_type& query, int32 & error );
		void			_fetch_rows( db_query_type& query,db_result_type& result );
	private:
		recvbuffer		recv_buffer_;
		recvbuffer		fields_buffer_;  
		MYSQL			mysql_;
		MYSQL_STMT*		mysql_stmt_;
		MYSQL_BIND		binds_[MAX_COLUMNS_LIMIT+1];
		bool			is_connected_;
		bool			is_runing;
		bool			init_batch_;
		field_type		fields_[MAX_COLUMNS_LIMIT+1];
		int32				field_count_;
		int32				offset_[MAX_COLUMNS_LIMIT+1];
		int32				row_size_;		//limit
		int32				max_rows_;
		int32				max_row_size_;
		mysql_conn_params	m_conn_params;
	};
}

#endif
