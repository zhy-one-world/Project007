/********************************************************************
	file name:	marry_mgr_ws
	created:	2017/12/06 13:49
	author:		Allen_lei
	purpose:	
*********************************************************************/
#ifndef _MARRY_MGR_WS_HPP_
#define _MARRY_MGR_WS_HPP_ 

#include "Logic/marry_def.hpp" 
#include "Logic/game_enum_def_s.h" 
#include "../client_session.hpp"

namespace hld
{ 

	class marry_mgr_ws 
	{
	public:
		marry_mgr_ws();
		~marry_mgr_ws();
	public:
		static marry_mgr_ws& get_instance()
		{
			static marry_mgr_ws s_marry_mgr;
			return s_marry_mgr;
		}
	public: 
		void	heart_tick(const int64& tick);
		void	init_manager();
		void	load_data_end(const marry_record_data (&load_data)[max_load_num], int32 load_num);
		void	save_marry_data(const marry_record_data& marry_data, bool is_divorce = false);
		void	clear_data();   
		bool	insert_into_data_map(marry_record_data& couple_data); 
		bool	remove_from_data_map(const guid_64& main_guid, const guid_64& couple_guid); 
		void	add_into_waiting_array(marry_record_data& new_waiting);
		void	del_from_waiting_array(const guid_64& role_guid);
		void	add_into_waiting_divorce_array(marry_record_data& divorce_waiting);
		void	del_from_waiting_divorce_array(const guid_64& role_guid);
		int32	make_marry_data_piece(marry_record_data& new_data, const guid_64& main_guid, const guid_64& sub_guid, marry_state data_state = marry_state_single_none);
		marry_record_data*	find_marry_record_info(const guid_64& role_guid);
		marry_record_data*	get_waiting_marry_data(const guid_64& role_guid );
		marry_record_data*	get_waiting_divorce_data(const guid_64& role_guid); 
		marry_couple_data	get_couple_data(const guid_64& role_guid);
		bool	is_couple(const guid_64& role_guid , const guid_64& couple_guid);
		//rename																																		
		void	change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步

	public:
		void	session_offline_marry_handle(const guid_64& role_guid);
		void	session_online_marry_handle(const guid_64& role_guid); 
		void	remove_marry_reward(const marry_record_data& couple_data);
		void	update_marry_data(const guid_64& role_guid);
		void	sync_couple_data_info(const guid_64& role_guid);
		void	sync_couple_data_info(const guid_64& role_guid, const marry_record_data& marry_record_ref);
		void    c2ws_get_married_reward(const guid_64& role_guid);
		void    set_marry_mission_id(const guid_64& role_guid, int32 mission_id);
		void    c2ws_get_marry_mission(const guid_64& role_guid, int32 is_near);
		void    ws2cs_get_reward(const guid_64& role_guid, int32 get_end_state);
		void    cs2ws_get_reward_end(const guid_64& role_guid,const int32& old_marry_state,const int32& get_result);

		//-----		marry	 -------------------------- 
		bool	check_is_in_marry_ready(const guid_64& main_guid, const guid_64& sub_guid);
		int32	check_is_can_marry(const guid_64& main_guid, const guid_64& sub_guid); 
		void	req_check_marry_with_sb(const guid_64& main_guid, const guid_64& sub_guid,int32 marry_item);
		void	cs2ws_check_marry_end(const guid_64& main_guid, const guid_64& sub_guid, int32 result_type, int32 item_id);
		void	agree_marry_with_sb(const guid_64& main_guid, const guid_64& sub_guid, int32 marry_item);
		void	cs2ws_cost_marry_item_end(const guid_64& main_guid, const guid_64& sub_guid, int32 result_type);
		void	c2ws_reply_marry_choice(const guid_64& role_guid, bool is_agree); 
		void	agree_marry_proc(const guid_64& role_guid, marry_record_data* waiting_data_ptr);
		void	c2ws_get_couple_info(const marry_couple_data& couple_info_ref, const guid_64& role_guid);

		//-----		divorce	 -------------------------- 
		int32	check_is_can_divorce(const guid_64& main_guid, const guid_64& sub_guid, bool is_force);
		void	c2ws_req_check_divorce(const guid_64& main_guid, const guid_64& sub_guid, bool is_force); 
		void	dp2ws_ret_last_login_time_end(const guid_64& role_guid, const int64& last_login_time);
		void	c2ws_reply_divorce_choice(const guid_64& role_guid, bool is_agree, bool is_force);		
		void	cs2ws_check_divorce_end(const guid_64& main_guid, int32 result_type);
		void	cs2ws_force_divorce_end(const guid_64& main_guid, int32 result_type); 
		void	agree_divorce_proc(const guid_64& role_guid, marry_record_data* data_ptr);

	public:
		//-----		wedding_ring	 --------------------------
		void	sync_wedding_ring_info(const guid_64& role_guid);
		void    sync_marry_mission_info(const guid_64& role_guid);
		void	cs_update_wedding_ring_level(const guid_64& role_guid, int32 ring_level);
		bool	add_heart_value(const guid_64& role_guid, int32 add_value);
		void	req_bless_one_couple(client_session* client_ptr, const guid_64& target_guid);
		void	req_bless_one_couple_end(const char* role_name, const guid_64& target_guid);

		void	send_mail_to_couple(const guid_64& main_guid, const guid_64& sub_guid, bool is_divorce);

	public:
		void	send_marry_notice_to_cs(const 	marry_record_data& record_info ,bool is_marry = true, int32 marry_item_id = 0);
		void	send_marry_notice_to_all(int32 marry_item_id, const xstring& main_name, const xstring& sub_name);
		void	send_couple_online_notice(const guid_64& main_guid, const xstring& couple_name);
		void	send_operate_result_notice(int32 operate_result_type, const guid_64& main_guid, const guid_64& sub_guid, int32 support_id = 0);
		void	send_forced_divorce_mail(const guid_64& addresss, const  xstring&  couple_role_name);
		guid_64	get_main_role_guid(const guid_64& role_guid);
	private: 
		marry_data_map				m_marry_info_map;
		waiting_marry_array			m_marry_waiting_array; 
		waiting_marry_array			m_divorce_waiting_array;
		int64						m_new_tick_minute;  
	};



}

#endif
