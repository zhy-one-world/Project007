/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:36
	file base:	mysql_async_mgr
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _MYSQL_ASYNC_MGR_H_
#define _MYSQL_ASYNC_MGR_H_

#include "logic/type_def.hpp"
#include <xchar.hpp>
#include "Logic/char_def.hpp"

namespace faith
{
	#define MAX_ROWS_LIMIT				100000
	#define MAX_ROW_SIZE_LIMIT			13072	//13072 Byte
	#define MAX_COLUMNS_LIMIT			256
	#define DEFAULT_PREFETCH_ROWS		10

	#define QT_QUERY					1
	#define QT_EXECUTE					2
	#define QT_BATCH_QUERY				3
	#define QT_NORMAL_QUERY				4				//include type QUERY and EXECUTE

	struct db_result_type;
	typedef boost::function<void(db_result_type)>	db_callback_handler_type;

	struct db_query_type
	{
		db_query_type()
		{
			query_type				= QT_NORMAL_QUERY;
			prefetch_rows			= DEFAULT_PREFETCH_ROWS;
			logic_id				= e_data_flag_none;
			//role_guid					= 0;
		}

		void swap(db_query_type& other)
		{
			handler.swap(other.handler);
			sql_str.swap(other.sql_str);
			int32 tmp_query_type		= query_type;
			int32 tmp_prefetch_rows	= prefetch_rows;
			int64 tmp_logic_id		= logic_id;
			guid_64 tmp_role_guid = role_guid;
			query_type				= other.query_type;
			prefetch_rows			= other.prefetch_rows;
			logic_id				= other.logic_id;
			role_guid = other.role_guid;
			other.query_type		= tmp_query_type;
			other.prefetch_rows		= tmp_prefetch_rows;
			other.logic_id			= tmp_logic_id;
			other.role_guid = tmp_role_guid;
		}

		xstring					sql_str;
		db_callback_handler_type	handler;
		int32							query_type;				//sql type: normal, batch_query
		int32							prefetch_rows;			//number of rows per one COM_FETCH
		int64							logic_id;
		guid_64					role_guid;
	};

	struct field_type
	{
		int32	lenght;
		int32 fieldtype;
	};

	struct db_result_type
	{
		union query_union_type
		{
			struct data_select_type
			{
				int32			row_count;
				int32			row_size;
				void*		data;
				void*		fields;
				int32			prefetch_rows;			//number of rows per one COM_FETCH
			} data_select;
			struct data_execute_type
			{
				ui64	effected;
			} data_execute;			
		} query;
		int32					error;			//0:no error
		int32					query_type;
	};

	struct mysql_conn_params
	{
		xstring			host;
		xstring			user;
		xstring			password;
		xstring			db;
		uint32	port;
		mysql_conn_params()
		{
			port = 3306;
		}
	};


#pragma pack(push,1)
	//	datetime type
	struct _time_type
	{
		uint32	year, month, day, hour, minute, second;
		uint32	reserve1,reserve2,reserve3;
	};
#pragma pack(pop)

	class mysql_driver;

	struct mysql_conn_info
	{
		mysql_driver*		conn;
		mysql_conn_params	conn_params;
	};

	class mysql_async_mgr_impl;
	typedef boost::shared_ptr<mysql_async_mgr_impl> db_async_mgr_impl_ptr;
	typedef std::map<uint32, db_async_mgr_impl_ptr> db_async_mgr_impl_ptr_map;

	//
	//	none-singleton, every mysql_async_mgr for one database. maybe multi for cross.
	//
	class mysql_async_mgr : public boost::noncopyable
	{
	public:
		mysql_async_mgr(	);
		~mysql_async_mgr(	);
	public:
		bool									init(
			size_t workerthread_num,
			mysql_conn_params conn_params,
			int32 server_group_id,
			int32 max_rows = MAX_ROWS_LIMIT,
			int32 max_row_size = MAX_ROW_SIZE_LIMIT );
		void									release(	);
		//	
		//	desc:	order a SQL query for mysql_async_mgr
		//			if want to batch query, please set param query.query_type = QT_BATCH_QUERY,
		//			and param query.prefetch_rows used to indicate number of rows per one COM_FETCH
		void									add_query( db_query_type& query );

		//
		//	desc:	process the (binary) string to escape the special characters. 
		//			usually be used to convert binary/varbinary date, so we can correctly send SQL statement to mysql server.
		//
		uint32									escape_string( xchar *to, const xchar *from, uint32 length );
		bool									empty(	);

		db_async_mgr_impl_ptr&					get_db_impl(guid_64 role_src_guid = 0);

	private:
		db_async_mgr_impl_ptr					impl_;
		bool									is_vaild;
	};

}

#endif
