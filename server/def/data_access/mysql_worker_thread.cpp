/********************************************************************
	created:	2014/05/28
	created:	28:5:2014   21:22
	file base:	mysql_worker_thread
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#include "db_config.hpp"
#include <net/scheduler.hpp>
#include "mysql_worker_thread.hpp"
#include <time.hpp>

//	global logger obj.
namespace hld
{
	worker_thread::worker_thread(mysql_async_mgr_impl* ptr, int32 thread_no)
	{
		m_mgr_ptr = ptr;
		m_run = true;
		boost::thread work(boost::bind(&worker_thread::operator (),this, &(ptr->m_conn_info[thread_no])));
		m_thread.swap(work);
		m_pre_time = utility::get_tick_count();
	}

	worker_thread::~worker_thread()
	{
		m_thread.join();
	}

	void worker_thread::operator()(mysql_conn_info* conn_info)
	{
		conn_info->conn->thread_start();
		do
		{
			boost::uint32_t mlb_id = 0;
			while(m_mgr_ptr->get_one_query(&m_current_query, mlb_id))
			{		
				m_pre_time = utility::get_tick_count();
				db_result_type exec_result;
				if (m_current_query.query_type != QT_BATCH_QUERY)
				{
					conn_info->conn->execute(m_current_query, exec_result);
					net::scheduler::getInstance().post_raw
						(boost::bind(&mysql_async_mgr_impl::call_add_query,m_mgr_ptr, m_current_query.logic_id,m_current_query.role_guid.server_64,mlb_id,m_current_query.handler,exec_result));
				}
				else
				{
					while(conn_info->conn->batch_query(m_current_query, exec_result) && !exec_result.error)
					{
						net::scheduler::getInstance().post_raw
							(boost::bind(&mysql_async_mgr_impl::call_add_query,m_mgr_ptr, m_current_query.logic_id,m_current_query.role_guid.server_64,mlb_id,m_current_query.handler,exec_result));
					}
					net::scheduler::getInstance().post_raw
						(boost::bind(&mysql_async_mgr_impl::call_add_query,m_mgr_ptr, m_current_query.logic_id,m_current_query.role_guid.server_64,mlb_id,m_current_query.handler,exec_result));
				}

				if(exec_result.error)
				{
					switch(exec_result.error)
					{
					case 9999:
						break;
					case 9998:
						break;
					case 4:
						{
							xstring err = conn_info->conn->last_error();
							if (err == _XTEXT(""))
							{
							}
							else
							{
							}
						}
						break;
					default:
						{
 							xstring err = conn_info->conn->last_error();
						}
						break;
					}
				}
				conn_info->conn->inner_next_result();
			}

			ui64 now_time = utility::get_tick_count();
			if (now_time - m_pre_time >= INTERACTIVE_TIMEOUT)
			{
				conn_info->conn->ping();
				m_pre_time = now_time;
			}
			Sleep(1);
			//boost::this_thread::sleep(boost::posix_time::milliseconds(1));

		}while(m_run);
		conn_info->conn->thread_end();
	}

	void worker_thread::stop()
	{
		m_run = false;
	}
}
