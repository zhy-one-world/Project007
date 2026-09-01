/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   15:02
	file base:	queued_login_mgr
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WS_QUEUED_LOGIN_MGR_H_
#define _WS_QUEUED_LOGIN_MGR_H_

#include "client_session.hpp"
#include "world_server.hpp"
#include "Logic/count_def.hpp"
#include <list>
#include <singleton.hpp>

namespace faith
{

	class queued_login_mgr : public singleton<queued_login_mgr>
	{
		friend class singleton<queued_login_mgr>;	

		enum
		{	
			e_queuelogin_interval = 1000 / world_server::e_gameloop_interval,	
		};

		typedef std::list<int32>					queued_login_list;
		typedef queued_login_list::iterator			queued_login_list_it;

	public:
		queued_login_mgr(void);
		~queued_login_mgr(void);

	public:
		void				pop_gm();
		bool				pop_front();
		void				login_complete(client_session* client_session_ptr);									// 完成登陆.
		void				tick(const int64& new_time);
		void				on_session_login(client_session* client_session_ptr, e_login_type_new login_type);			// 调用当：帐号密码验证完毕，session 切换为 LOGIN_OK 状态时
		void				on_session_logout(client_session* client_session_ptr, bool is_need_reduce_count = true);			// 调用当：session 断开时,如果session在登陆队列中则删除,如果sessin已在游戏中.则调用pop_front().

		void				change_online_counter(int32 change_num) { m_online_counter += change_num; }
		int32				get_online_limit() const			{ return init_session_max; }
		int32				get_queue_length() const			{ return m_queued_client_list.size(); }
		int32				get_online_counter() const				{ return m_online_counter; }
	private:
		int32				m_online_counter;								//	已在游戏中的玩家计数
		int64				m_update_time_interval;							//	更新计时
		unit_index_list		m_queued_client_list;							//	玩家队列

		int32				m_max_login_normal_player;						//	普通玩家最大登录数
		int32				m_max_login_player;								//	所有玩家最大登录数
	};

}

#endif