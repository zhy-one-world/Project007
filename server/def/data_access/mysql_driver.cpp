/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:27
	file base:	mysql_driver
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "db_config.hpp"
#include "mysql_driver.hpp"
#include "server_log.hpp"

/*
	֧�������ַ������ͻ���ֻ��ʹ��gb2312, utf8��֧�����ĵ��ַ�����mysql���������ַ������ü���mysql���������ú�dpʹ������.doc��
	���¼�����涨�˿ͻ�����ʹ�õ��ַ�����ʹ�������ַ��趨��SET_LOCALE_CHARSET��
	�ͻ���ʹ�õ������ַ���������DEFALUT_CHARSET_NAME�����һ�£�DEFALUT_CHARSET_MAX_LENΪ���ַ��������󳤶ȡ�
	���������ַ������Ƽ�ʹ��gb2312�� ���ʹ�õ���utf8, DEFALUT_CHARSET_MAX_LEN = 3��gbk,DEFALUT_CHARSET_MAX_LEN=2
*/
#define DEFALUT_CHARSET_NAME		"latin1"
#define DEFALUT_CHARSET_MAX_LEN		1

namespace faith
{
	inline int32 mysql_driver::get_type_size(MYSQL_FIELD* field, int32& real_len)
	{
		switch(field->type)
		{
		case MYSQL_TYPE_TINY:
			real_len = 1;
			return real_len;
		case MYSQL_TYPE_SHORT:
			real_len = 2;
			return real_len;
		case MYSQL_TYPE_LONG:
		case MYSQL_TYPE_FLOAT:
			real_len = 4;
			return real_len;
		case MYSQL_TYPE_LONGLONG:
		case MYSQL_TYPE_DOUBLE:
			real_len = 8;
			return real_len;
		case MYSQL_TYPE_STRING:
		case MYSQL_TYPE_VAR_STRING:
			if(field->flags & BINARY_FLAG)
			{
				real_len = field->length;
				return real_len;
			}
			real_len = field->length;
			return real_len + 1;
		case MYSQL_TYPE_DATETIME:
		case MYSQL_TYPE_DATE:
		case MYSQL_TYPE_TIME:
		case MYSQL_TYPE_TIMESTAMP:
			/*
				����ʱ������
					��ʱ���������ݵ�mysql���ݿ⣬������'YYYY-MM-DD HH:MM:SS'/ 'YYYYMMDDHHMMSS'/YYYYMMDDHHMMSS�ȵȸ�ʽ���ַ���������
					ȡʱ���������ݣ�ʹ��_time_type���͡�������ʹ��_time_type���ͱ�ʾһ��ʱ�䡣
			*/
			real_len = sizeof(MYSQL_TIME);
			return real_len;
		default:
			real_len = field->length;
			return real_len;
		}
	}

	mysql_driver::mysql_driver(int32 max_rows, int32 max_row_size)
	{
		is_connected_ = false;
		is_runing = false;
		memset(fields_, 0, sizeof(fields_));
		max_rows_ = max_rows;
		max_row_size_ = max_row_size;
		init_batch_ = false;
		field_count_ = 0;
		memset(offset_, 0, sizeof(offset_));
		row_size_ = 0;
	}
	
	mysql_driver::~mysql_driver()
	{
		disconnect();
	}


	bool mysql_driver::connected()
	{
		return is_connected_;
	}

	#define cstr(x) x

	bool mysql_driver::connect(mysql_conn_params& params)
	{
		is_runing = true;
		mysql_init(&mysql_);
		mysql_stmt_ = mysql_stmt_init(&mysql_);
		//set client charset code
		const xchar * charset_name = DEFALUT_CHARSET_NAME; 
		mysql_options(&mysql_,MYSQL_SET_CHARSET_NAME, charset_name);

		bool on_data_truncation = true;
		mysql_options(&mysql_, MYSQL_REPORT_DATA_TRUNCATION, &on_data_truncation);

		// Establish the connection. CLIENT_MULTI_STATEMENTS:support multi query.
		MYSQL* temp =
			mysql_real_connect(&mysql_, cstr(params.host.c_str()), cstr(params.user.c_str()),
			cstr(params.password.c_str()), cstr(params.db.c_str()), params.port, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_INTERACTIVE);
		if (temp)
		{
			m_conn_params = params;
			is_connected_ = true;
			///*
			//	*/
			//xchar sql[256];
			//sprintf(sql, "CALL SP_ON_DP_STARTED (%u)", 0);
			//bool is_ok = inner_execute(sql, strlen(sql));
			//if (is_ok)
			//{
			//	inner_next_result();
			//	is_connected_ = true;
			//}
		}
		else
		{
			is_runing = false;
		}
		return is_connected_;
	}

	void mysql_driver::disconnect()
	{
		is_connected_ = false;
		is_runing = false;
		mysql_close(&mysql_);
		mysql_stmt_close(mysql_stmt_);
	}

	void mysql_driver::execute(db_query_type& query, db_result_type& result)
	{
		if (false == is_connected_)
		{
			CONSOLE_ERROR("mysql thread is close {}", query.sql_str.c_str());
			result.error = 5;
			if (false == is_runing)
			{
				connect(m_conn_params);
			}
			return;
		}
		int32 rc = _stmt_prepare(query, result.error);
		if (rc == 1)
		{
			_fetch_rows(query, result);
			result.query_type = QT_QUERY;
		}
		else if(rc == 2)
		{
			result.query_type = QT_EXECUTE;
			result.query.data_execute.effected = mysql_stmt_affected_rows(mysql_stmt_);
		}
	}

	uint32 mysql_driver::escape_string(xchar *to, const xchar *from, uint32 length)
	{
		return mysql_real_escape_string(&mysql_,to,from,length);
	}

	void mysql_driver::thread_start()
	{
		mysql_thread_init();
	}

	void mysql_driver::thread_end()
	{
		mysql_thread_end();
	}

	const xchar * mysql_driver::last_error()
	{
		return mysql_error(&mysql_);	
	}

	int32 mysql_driver::ping()
	{
		return mysql_ping(&mysql_);
	}

	int32 mysql_driver::batch_query(db_query_type& query, db_result_type& result)
	{
		if (init_batch_ == false)
		{
			int32 ret = _stmt_prepare(query, result.error);
			assert(ret != 2);
			if (!result.error)
			{
				init_batch_ = true;
			}
			else
			{
				return 0;
			}
		}
		_fetch_rows(query, result);
		if (result.error || result.query.data_select.row_count == 0)
		{
			init_batch_ = false;
			mysql_stmt_free_result(mysql_stmt_);
		}
		return result.query.data_select.row_count;
	}

	int32 mysql_driver::_stmt_prepare(db_query_type& query, int32 & error)
	{
		MYSQL_RES *	prepare_meta_result = NULL;
		MYSQL_FIELD * field_info = NULL;

		error = 0;
		if (query.query_type != QT_BATCH_QUERY)
		{
			uint32 type = (uint32) CURSOR_TYPE_NO_CURSOR;
			mysql_stmt_attr_set(mysql_stmt_, STMT_ATTR_CURSOR_TYPE, (void*) &type);
		}
		else
		{
			uint32 type = (uint32) CURSOR_TYPE_READ_ONLY;
			mysql_stmt_attr_set(mysql_stmt_, STMT_ATTR_CURSOR_TYPE, (void*) &type);
			if(mysql_stmt_attr_set(mysql_stmt_, STMT_ATTR_PREFETCH_ROWS,(void*) &query.prefetch_rows))
			{
				error = 23;
				return 0;
			}
		}

		error = mysql_stmt_prepare(mysql_stmt_, query.sql_str.c_str(), query.sql_str.size());
		if (error)
		{
			CONSOLE_ERROR(" error_id:{} error_msg:{} sql:{} ", error, mysql_stmt_error(mysql_stmt_), query.sql_str.c_str());
			error = 5;
			disconnect();
			connect(m_conn_params);
			return 0;
		}

		error = mysql_stmt_execute(mysql_stmt_);
		if (error)
		{
			CONSOLE_ERROR(" error_id:{} error_msg:{} sql:{} ", error, mysql_stmt_error(mysql_stmt_), query.sql_str.c_str());
			error = 6;
			return 0;
		}

		prepare_meta_result = mysql_stmt_result_metadata(mysql_stmt_);
		if(prepare_meta_result)
		{
			row_size_ = field_count_ = offset_[0] = 0;
			memset(binds_, 0, sizeof(MYSQL_BIND) * (MAX_COLUMNS_LIMIT+1));
			while((field_info = mysql_fetch_field(prepare_meta_result)))
			{
				int32 real_len;
				int32 len = get_type_size(field_info, real_len);
				binds_[field_count_].buffer_type = field_info->type;
				binds_[field_count_].buffer_length = real_len;
				if(field_info->flags & UNSIGNED_FLAG)
				{
					binds_[field_count_].is_unsigned = bool(1);
				}
				fields_[field_count_].lenght = len;
				fields_[field_count_].fieldtype = (real_len == len)?0:1;
				++field_count_;
				row_size_ += len;
				offset_[field_count_] = row_size_;
			}
			fields_[field_count_].lenght = 0;
			assert(row_size_ <= max_row_size_);
			mysql_free_result(prepare_meta_result);
			return 1;
		}
		return 2;
	}

	void mysql_driver::_fetch_rows(db_query_type& query,db_result_type& result)
	{
		int32 row_count;
		int32 fetch_result = 0;
		int32 real_num = 0;
		bool is_bind_ok = false;
		int32 total_size;
		if (query.query_type != QT_BATCH_QUERY)
		{
			if (mysql_stmt_store_result(mysql_stmt_))									
			{
				result.error = 3;
				return;
			}
			row_count = (int32)mysql_stmt_num_rows(mysql_stmt_);
		}
		else
		{
			row_count = query.prefetch_rows;
		}

		if(row_count > max_rows_)
		{
			result.error = 9998;
			goto L_EXIT;
		}

		total_size = row_count * row_size_;
		if(recv_buffer_.reserve(total_size))
		{
			xchar * wptr = recv_buffer_.wptr();
			for (int32 i = 0; i < field_count_; ++i)
			{
				binds_[i].buffer = wptr + offset_[i];
			}
			memset(wptr, 0, total_size);
		}
		else
		{
			result.error = 9999;
			goto L_EXIT;
		}

		is_bind_ok = mysql_stmt_bind_result(mysql_stmt_, binds_);
		assert(!is_bind_ok);

		while (!(fetch_result = mysql_stmt_fetch(mysql_stmt_)))								
		{
			if( ++real_num >= row_count)
			{
				break;
			}

			for (int32 i = 0; i < field_count_; ++i)											
			{
				binds_[i].buffer = (xchar *)binds_[i].buffer + row_size_;						
			}
			mysql_stmt_bind_result(mysql_stmt_, binds_);
		}
		if (fetch_result == 1 || fetch_result == MYSQL_DATA_TRUNCATED)
		{
			result.error = 4;
			recv_buffer_.rollback();
			goto L_EXIT;
		}

		if (real_num == 0)
		{
			result.error = 0;
			result.query_type = query.query_type;
			result.query.data_select.row_count = 0;
			result.query.data_select.row_size = row_size_;
			recv_buffer_.rollback();
			result.query.data_select.data = (void *)&recv_buffer_;
			result.query.data_select.fields = (void *)&fields_buffer_;
			result.query.data_select.prefetch_rows = 0;
		}
		else
		{
			if (!fields_buffer_.write(fields_, sizeof(fields_)))
			{
				result.error = 9999;
				recv_buffer_.rollback();
				goto L_EXIT;
			}

			result.error = 0;
			result.query_type = query.query_type;
			result.query.data_select.row_count = real_num;
			result.query.data_select.row_size = row_size_;
			recv_buffer_.commit();
			result.query.data_select.data = (void *)&recv_buffer_;
			result.query.data_select.fields = (void *)&fields_buffer_;

			if (query.query_type != QT_BATCH_QUERY)
			{
				result.query.data_select.prefetch_rows = real_num;
			}
			else
			{
				result.query.data_select.prefetch_rows = query.prefetch_rows;
			}
		}

L_EXIT:
		if (query.query_type != QT_BATCH_QUERY)
		{
			mysql_stmt_free_result(mysql_stmt_);
		}
		return;
	}

	bool mysql_driver::inner_execute(const xchar * sql, uint32 len)
	{
		bool ret = false;
		if(!mysql_stmt_prepare(mysql_stmt_,sql,len))
		{
			if(!mysql_stmt_execute(mysql_stmt_))
			{
				ret = true;
			}
		}
		return ret;
	}
}
