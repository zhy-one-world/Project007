/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   14:25
	file base:	db_config
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _DB_CONFIG_H_
#define _DB_CONFIG_H_

#include <winsock2.h>
#include <mysql.h>
#include <windows.h>
#include "logic/type_def.hpp"
#include "data_access/mysql_async_mgr.hpp"
#include "data_access/recvbuffer.hpp"

namespace faith
{
	typedef	mysql_async_mgr		db_accessor;
	typedef db_query_type		db_query_type;
	typedef db_result_type		db_result_type;
	typedef mysql_conn_params	db_conn_params;
	typedef recvbuffer			db_recvbuffer;
	typedef _time_type			db_time_type;
	typedef field_type			db_field_type;

	using namespace faith;

	inline void db_read_data(db_result_type* result_ptr, void * out, int32 size)
	{	
		db_recvbuffer* buff = (db_recvbuffer*)(result_ptr->query.data_select.data);
		if (buff)
		{
			if (size > result_ptr->query.data_select.row_count * result_ptr->query.data_select.row_size)
			{
				size = result_ptr->query.data_select.row_count * result_ptr->query.data_select.row_size;
			}
			buff->read(out, size);
		}
	};

	inline void db_read_fields_info(db_result_type* result_ptr, void * out, int32 size)
	{
		db_recvbuffer* buff = (db_recvbuffer*)(result_ptr->query.data_select.fields);
		if (size > sizeof(db_field_type)*MAX_COLUMNS_LIMIT)
		{
			size = sizeof(db_field_type)*MAX_COLUMNS_LIMIT;
		}
		buff->read(out, size);
	}

}

#endif
