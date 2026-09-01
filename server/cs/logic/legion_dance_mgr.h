/********************************************************************
created: 2019年10月15日
file base: legion_dance
file ext: hpp
author: zhaoyuming
purpose: 军团传功
*********************************************************************/
#ifndef _legion_dance_H
#define _legion_dance_H

#include <Logic/legion_dance_def.hpp>


namespace faith
{
	class player;
	class legion_dance_mgr
	{
	public:
		legion_dance_mgr();
		~legion_dance_mgr();
	public:
		void	clear_data();
		void	set_unit_index(int32 array_index) { m_array_index = array_index; }

		//int32	get_legion_dance_target_index();

		//获取世界静态等级
		int32 get_world_server_last_level();

		//发送邀请错误提示
		void	send_legion_dance_invite_error_tip(e_legion_dance_invite_error error_index, xstring role_name);
		//发送邀请错误提示
		void	send_legion_dance_accept_error_tip(e_legion_dance_accept_error error_index, xstring role_name);

		//发送邀请
		void	send_legion_dance_invite(int32 target_index);

		//军团共舞正式开始
		void	legion_dance_official_start(guid_64 target_guid);

	//lua调用
	public:

	private:
		int32									m_array_index;
	};
}

#endif