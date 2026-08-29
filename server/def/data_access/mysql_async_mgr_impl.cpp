/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:35
	file base:	mysql_async_mgr_impl
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "db_config.hpp"
#include <net/scheduler.hpp>
#include <mlb.hpp>
#include <persistence_id_generator.hpp>
#include <mem_pool.hpp>
#include "mysql_async_mgr_impl.hpp"

//	global logger obj.
namespace faith
{
	bool mysql_async_mgr_impl::mlb_init(size_t workerthread_num, xstring host, xstring user, xstring password, xstring db, uint32 port, int32 max_rows, int32 max_row_size)
	{
		assert(workerthread_num > 0);

		//connection to server
		mysql_driver * db_object_1;
		mysql_conn_params conn_params;
		conn_params.db = db;
		conn_params.host = host;
		conn_params.password = password;
		conn_params.port = port;
		conn_params.user = user;

		for (size_t i = 0; i < workerthread_num; ++i)
		{
			db_object_1 = new mysql_driver(max_rows,max_row_size);
			if (db_object_1)
			{
				if(db_object_1->connect(conn_params))
				{
					mysql_conn_info mysql_conn;
					mysql_conn.conn = db_object_1;
					mysql_conn.conn_params = conn_params;
					m_conn_info.push_back(mysql_conn);
				}
				else
				{
					std::cout<< __XFUNCTION__ << _XTEXT(" mysql connect driver fail.")<<std::endl;
					delete db_object_1;
					return false;
				}
			}
			else
			{
				std::cout << __XFUNCTION__ << _XTEXT(" mysql create driver fail.") << std::endl;
				return false;
			}
		}
		std::cout << __XFUNCTION__<< _XTEXT(" connection to db [")<< conn_params.db <<_XTEXT("] success." ) << std::endl;
		//start thread
		for(size_t i = 0; i < workerthread_num; ++i)
		{
			worker_thread* workerthread = new worker_thread(this, i);
			m_thread_ptrs.push_back(workerthread);
		}
		return true;
	}

	namespace
	{
		extern const xchar sz_init[] = _XTEXT("mysql_async_mgr::init");
	}
	bool mysql_async_mgr_impl::init(size_t workerthread_num, mysql_conn_params conn_params, int32 max_rows, int32 max_row_size, uint32 server_group_flag)
	{
		m_server_group_flag = server_group_flag;
		mlb_helper<sz_init,bool (size_t, xstring, xstring, xstring, xstring, uint32, int32, int32)> helper(
			boost::bind(&mysql_async_mgr_impl::mlb_init,this,_1,_2,_3,_4,_5,_6,_7,_8)
			);
		return helper(workerthread_num,conn_params.host,conn_params.user,conn_params.password,conn_params.db,conn_params.port,max_rows,max_row_size);
	}


	MLB_CLASS_FUNC_0(void, mysql_async_mgr_impl, release)
	{
		query_queue empty_queue;
		swap(empty_queue, m_querys);
		m_querys_ex.clear();
		//stop thread
		std::list<worker_thread*>::iterator iter;
		for (iter = m_thread_ptrs.begin(); iter != m_thread_ptrs.end(); ++iter)
		{
			(*iter)->stop();
			delete (*iter);
		}
		m_thread_ptrs.clear();

		//disconnection server
		for(size_t i = 0; i < m_conn_info.size(); ++i)
		{
			delete m_conn_info[i].conn;
		}
		m_conn_info.clear();
	}

	bool mysql_async_mgr_impl::get_one_query(db_query_type* query, boost::uint32_t& mlb_id)
	{
		boost::recursive_mutex::scoped_lock lock(m_querys_mutex);
		
		if(m_querys.empty() == false)
		{
			db_query_type& front = m_querys.front();
			query->swap(front);
			m_querys.pop();

			mlb_id = 0;

			return true;
		}
		return false;
	}

	void mysql_async_mgr_impl::add_query(db_query_type& query)
	{
		boost::recursive_mutex::scoped_lock lock(m_querys_mutex);

		//db_query_type empty;
		m_querys.push(query);

		//db_query_type & back = m_querys.back();
		//back.swap(query);
	}

	void mysql_async_mgr_impl::add_queryex(db_query_type& query)
	{
		boost::recursive_mutex::scoped_lock lock(m_querys_mutex_ex);
		query_key queryKey(query.logic_id, query.role_guid.server_64);
		query_iter it = m_querys_ex.find(queryKey);
		if ( it == m_querys_ex.end())
		{
			m_querys_ex.insert(std::make_pair(queryKey, query_queue()));
		}

		it = m_querys_ex.find(queryKey);
		bool send_now = false;
		if (it->second.empty())
			send_now = true;

		it->second.push(query);

		if (send_now)
		{
			add_query(query);
		}
	}

	bool mysql_async_mgr_impl::empty()
	{
		return m_querys.empty() && m_querys_ex.empty();
	}

	static recvbuffer g_data,g_fields;

	void mysql_async_mgr_impl::recur_add_query(
		boost::uint32_t instance_id,
		db_callback_handler_type handler,
		const xstring& dbresult,
		const xstring& data,
		const xstring& fields )
	{
		db_result_type* result = (db_result_type*)(dbresult.c_str());
		if(!result->error && (result->query_type == QT_QUERY || result->query_type == QT_BATCH_QUERY))
		{
			g_fields.write(fields.c_str(),fields.size());
			g_data.write(data.c_str(), data.size());
			result->query.data_select.data = (void*)(&g_data);
			result->query.data_select.fields = (void*)(&g_fields);
		}
		if (!handler.empty())	handler(*result);
		if (!result->error && (result->query_type == QT_QUERY || result->query_type == QT_BATCH_QUERY))
		{
			g_fields.rnext(fields.size());
			g_data.rnext(data.size());
		}

		if ( !(result->query_type == QT_BATCH_QUERY && result->query.data_select.row_count && result->error==0) )
		{
			persistence_id_generator::getInstance().return_id(_XTEXT("mysql_async_mgr.add_query"),instance_id);
		}
	}

	void mysql_async_mgr_impl::call_add_query(int64 logic_id, boost::uint64_t role_guid, boost::uint32_t instance_id,db_callback_handler_type handler,db_result_type result)
	{
		recvbuffer * data;
		recvbuffer* fields;
		int32 len, fields_len;
		if(!result.error && (result.query_type == QT_QUERY || result.query_type == QT_BATCH_QUERY))
		{
			int32 row_count = result.query.data_select.prefetch_rows;
			data = (recvbuffer *)(result.query.data_select.data);
			fields = (recvbuffer*)(result.query.data_select.fields);
			len = result.query.data_select.row_size * row_count;
			if(len)
			{
				fields_len = sizeof(field_type)*(MAX_COLUMNS_LIMIT+1);
			}
			else
			{
				fields_len = 0;
			}
		}

		if (handler.empty() == false)
			handler(result);
		
		if (!result.error && (result.query_type == QT_QUERY || result.query_type == QT_BATCH_QUERY))
		{			
			data->rnext(len);
			fields->rnext(fields_len);
		}

		if (logic_id != e_data_flag_none)
		{
			boost::recursive_mutex::scoped_lock lock(m_querys_mutex_ex);
			query_key queryKey(logic_id, role_guid);
			query_iter it = m_querys_ex.find(queryKey);
			if ( it != m_querys_ex.end())
			{
				if (it->second.empty() /*|| it->second.front().call_id != call_id*/)
				{
					return;
				}

				it->second.pop();

				if (it->second.empty())
				{
					m_querys_ex.erase(it);
					return;
				}

				add_query(it->second.front());
			}
		}
	}

	namespace
	{
		extern const xchar sz_escape_string[] = _XTEXT("mysql_async_mgr::escape_string");
	}

	uint32 mysql_async_mgr_impl::escape_string(xchar *to, const xchar *from, uint32 length)
	{	
		mlb_helper<sz_escape_string,xstring (xstring&)> helper(
			boost::bind(&mysql_async_mgr_impl::mlb_escape_string,this,_1)
			);
		xstring str = xstring(from, length);
		str = helper(str);
		memcpy(to,str.c_str(),str.size());
		return str.size();
	}

	const xstring& mysql_async_mgr_impl::mlb_escape_string(const xstring& from)
	{
		static xstring escape_string;
		assert(m_conn_info.size() > 0 && m_conn_info[0].conn);
		int32 from_len = from.size();
		void * ptr = mem_pool::getInstance().alloc(from_len * 2 + 1);
		uint32 len = m_conn_info[0].conn->escape_string((xchar*)ptr, from.c_str(), from_len);
		escape_string.assign((xchar*)ptr,len);
		mem_pool::getInstance().free(ptr, from_len * 2 + 1);
		return escape_string;
	}
}
