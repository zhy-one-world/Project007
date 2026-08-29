/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:43
	file base:	mysql_async_mgr
	file ext:	cpp
	author:		zhy

	purpose:	
*********************************************************************/
#include "db_config.hpp"
#include "mysql_async_mgr.hpp"
#include "mysql_async_mgr_impl.hpp"
#include "utility/guid_gen.h"

namespace faith
{
	mysql_async_mgr::mysql_async_mgr():
		impl_(new mysql_async_mgr_impl())
	{
		is_vaild = false;
	}

	mysql_async_mgr::~mysql_async_mgr()
	{

	}

	bool mysql_async_mgr::init(size_t workerthread_num, mysql_conn_params conn_params, int32 server_group_id, int32 max_rows /* = MAX_ROWS_LIMIT */, int32 max_row_size /* = MAX_ROW_SIZE_LIMIT */)
	{
		is_vaild = true;
		return impl_->init(workerthread_num, conn_params, max_rows, max_row_size, server_group_id);
	}

	void mysql_async_mgr::release()
	{
		is_vaild = false;
		impl_->release();
	}

	void mysql_async_mgr::add_query(db_query_type& query)
	{
		if (false == is_vaild)
		{
			return;
		}
		db_async_mgr_impl_ptr& dp_impl = get_db_impl(query.role_guid);
		if (dp_impl)
		{
			if (query.logic_id == e_data_flag_none)
			{
				dp_impl->add_query(query);
			}
			else
			{
				dp_impl->add_queryex(query);
			}
		}
		else
		{
			db_result_type exec_result;
			exec_result.error = 7;
			query.handler(exec_result);
		}
	}

	uint32 mysql_async_mgr::escape_string(xchar *to, const xchar *from, uint32 length)
	{
		return impl_->escape_string(to,from,length);
	}

	bool  mysql_async_mgr::empty()
	{
		return impl_->empty();
	}

	db_async_mgr_impl_ptr& mysql_async_mgr::get_db_impl(guid_64 role_src_guid)
	{
		return impl_;
	}
}

