/********************************************************************
created: 2018年3月15日
file base: interaction_mgr
file ext: h
author: wucun

purpose: about interaction
*********************************************************************/
#ifndef _INTERACTION_MGR_H_
#define _INTERACTION_MGR_H_

#include "Logic/char_def.hpp"

namespace faith
{
	class player;

	enum e_interaction_state
	{
		e_interaction_state_none,
		e_interaction_state_wait_reply,
		e_interaction_state_wait_start_real_interaction,
		e_interaction_state_interacting,
	};

	class cinteraction_mgr
	{
	public:
		//初始化
		cinteraction_mgr();
		~cinteraction_mgr();
		void clear_data();
		void heart_tick(const int64& new_time);
		void set_unit_index(int32 unit_index) { m_unit_array_index = unit_index; }

		guid_64 get_interaction_target_guid() { return m_interaction_target_guid; };

		//基础操作
		void	set_interaction_info(guid_64 target_guid, faith::e_interaction_type interaction_type);
		void	reset_interaction_info();
		guid_64	get_interaction_guid();
		faith::e_interaction_type get_interaction_type();
		faith::e_interaction_type get_real_interaction_type();

		e_interaction_type	get_target_interaction_type(faith::e_interaction_type interaction_type, bool target_is_man);
		e_interaction_type	get_marry_raid_interaction_type(bool is_back, bool is_man);

		bool	get_cur_player_if_sex_is_man();
		bool	get_interaction_player_if_sex_is_man();

		void	set_is_inviter(bool is_inviter);
		bool	get_is_inviter();
		void	set_interaction_state(e_interaction_state state);
		e_interaction_state	get_interaction_state() { return m_interaction_state; };

		//流程操作
		void	recv_interaction_invite(guid_64 invite_guid, faith::e_interaction_type interaction_type);				//收到交互请求
		void	send_interaction_invite(guid_64 invite_guid, faith::e_interaction_type interaction_type);				//发送交互请求
		void	recv_interaction_answer(guid_64 invite_guid, faith::e_interaction_type interaction_type, faith::e_interaction_reply result);					//收到交互应答
		void	send_interaction_answer_to_two_side(guid_64 invite_guid, faith::e_interaction_type interaction_type, faith::e_interaction_reply result);		//向双方发送交互应答
		void	recv_real_start_interaction(guid_64 be_invited_guid, faith::e_interaction_type interaction_type);		//收到真正开始交互消息 默认邀请方收到
		void	send_real_start_interaction_to_aoi(guid_64 invite_guid,guid_64 be_invited_guid, faith::e_interaction_type interaction_type);				//发送真正开始交互消息	默认邀请方发送
		void	set_interaction_type_attribute(guid_64 be_invited_guid, faith::e_interaction_type interaction_type);	//开始交互后设置双方交互属性
		void	stop_interaction();
		void	stop_interaction_by_guid(guid_64 guid);


		//保护代码
		void	set_wait_real_interaction_time(float duration_time);

	private:
		float						m_handle_invite_cd_time;

		guid_64						m_interaction_target_guid;
		faith::e_interaction_type	m_interaction_type;

		bool						m_is_inviter;

		e_interaction_state			m_interaction_state;

		float						m_wait_real_interaction_time;	//一方移动到对方等待时间 若超时则自动停止交互
	private:
		int32						m_unit_array_index;
		int64						m_old_tick_time;

	public:
		int64                       m_interaction_cdtime;
	};
}

#endif
