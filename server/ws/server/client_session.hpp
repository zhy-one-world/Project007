/********************************************************************
	created:	2014/08/07
	created:	7:8:2014   14:55
	file base:	client_session
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _WS_CLIENT_SESSION_H_
#define _WS_CLIENT_SESSION_H_

#include "Logic/char_def.hpp"
#include "Logic/transfer_def.hpp"
#include "Logic/login_def.hpp"
#include "relation/relation_mgr.h"
#include "Logic/legion_def.hpp"
#include "server_log.hpp"
#include "base.hpp"

namespace faith
{
	class client_session
	{
	public:
		enum e_session_status
		{
			e_ss_logout,
			e_ss_created,
			e_ss_login_ok,
			e_ss_ingame,
			e_ss_queue,
			e_ss_map_transfer,
			e_ss_cross_transfer,
			e_ss_check_token,
		};
		enum e_session_step
		{
			e_session_step_null,
			e_session_step_login,
			e_session_step_login_win,
			e_session_step_enter_game,			//	enter_game
			e_session_step_dp_load,
			e_session_step_send_enter_game_cs,
			e_session_step_cs_enter_game,
			e_session_step_send_enter_scene,
			e_session_step_cs_enter_scene,
		};
		enum e_player_scene_status_type
		{
			e_pss_left,
			e_pss_leaving,
			e_pss_entering,
			e_pss_entered,
			e_pss_sys_transfer,
		};

		/*
		�Ƿ�������״̬
		����մ���ʱ,m_online_state=0.
		������к�m_online_state=1
		��m_status��ΪSS_LOGIN_OK��һ�̿�ʼ��ֱ���ö���ɾ��,����m_online_state=2

		WS������m_online_state=1 client_session�������ֵ,����client_session_mgr�е�m_online_counterֵ��
		*/
		enum e_online_state
		{
			e_os_offline,
			e_os_online,
			e_os_inqueue,
		};
	public:
		client_session();
		~client_session();
		void									clear_data();
		bool&									is_vaild();
		void									set_data_use(bool is_use) { m_is_data_use = is_use; }
		int32									get_array_index() { return m_array_index; }
		void									set_array_index(int32 array_index) { m_array_index = array_index; }
	public:
		void									set_cs_array_index(int32 array_index) { m_cs_array_index = array_index; }
		int32									get_cs_array_index() { return m_cs_array_index; }
		int32									get_cs_conn_index() { return m_cs_conn_index; }
		void									set_cs_conn_index(int32 cs_conn_index) { m_cs_conn_index = cs_conn_index; }
	public:
		void									set_client_logout_begin();
	public:
		void									tick(const int64& new_time);
		void									send_to_fep( const void*,size_t );
		void									send_to_cs( const void*,size_t );
		bool                                    send_to_cs_lua(google::protobuf::Message* net_pro, uint32 header);
		void                                    send_to_cs_lua(const char* msg, int32 msg_len, uint32 header);
		void									send_to_client(google::protobuf::Message* net_pro, uint32 header);
		void									send_to_client(const void* data_ptr, int32 data_len,uint32 header);
	public:
		s_client_uid&							get_client_uid() { return m_client_uid; }
		guid_64&								get_map_guid(){ return m_map_guid; }
		MapTemplate*							get_map_temp_ptr();
		void									set_map_guid(guid_64 map_guid){ m_map_guid = map_guid; }
		int32									get_line_id(){ return m_line_id; }
		void									set_line_id(int32 line_id) { m_line_id = line_id; }
		guid_64&								get_team_guid() { return m_team_guid; }
		void									set_team_guid(guid_64 _guid) { m_team_guid = _guid; }
		void									clear_team_guid() { m_team_guid.clear_data(); }
		guid_64&								get_legion_guid() { return m_player_legion_info.legion_guid; }
		s_player_legion_info&					get_legion_info() { return m_player_legion_info; };
		void									set_legion_guid(guid_64 legion_guid) { m_player_legion_info.legion_guid = legion_guid; };
		void									set_legion_info(s_player_legion_info leigon_info) { m_player_legion_info = leigon_info; };
		void									set_legion_name(xchar* legion_name );
		void									clear_legion_info() { m_player_legion_info.clear_data(); }
		bool									is_in_game() { return m_status == e_ss_ingame; };
		void									send_role_right(e_role_right right_type, int32 type_value, int32 right_time);
		void									set_role_info_data(e_role_info eType, int32 num);
		void									send_to_cs_role_info_data(e_role_info eType);
		int32									get_role_info_data(e_role_info eType) { return m_role_info.data_ary[eType]; }
		guid_64									get_role_guid() { return m_role_info.role_guid; };
		void									set_role_guid(const guid_64& role_guid);
		guid_64									get_role_map_guid() { return m_role_info.map_guid; };
		int64									get_role_gs_value() { return m_role_info.data_i64_ary[e_role_i64_info_gs_value]; };
		void									set_role_gs_value(int64 gs_value);
		void									save_role_ws_info();
		void									send_ws_info_one(int32 index);
		void									send_ws_info_all();
		bool									is_ws_info_daily_refresh();
		void									refresh_ws_faily_info();
		bool									is_already_login() { return m_is_already_login; };
		void									set_is_already_login(bool is_already) { m_is_already_login = is_already; };
		relation_mgr&							get_relation_list_mgr() { return m_relation_mgr; }
		xchar*									get_role_name() { return m_role_info.role_name; }
		void									set_role_name(xchar* _role_name);
		bool									set_transfer_info(const s_transfer_info& transfer_info);
		void									player_enter_game_transfer(guid_64 role_guid, int32 server_id);
		void									login_try_enter_scene();
		void									enter_scene_logic_proc();
		void									enter_scene_logic_proc_login();
		void									send_notice(std::string notice_str);
		void									send_recharge(const xchar* order_id, int32 goods_id, float pay_price, int64 order_num, int32 payment_type, int32 direct_diamond = 0);
		int64									get_login_time() { return m_logout_time - m_logintime; };
		bool									is_self_server(guid_64 target_guid = guid_64(), bool is_need_same_server_for_ws = true);
		void									load_offline_chat();

		guid_64									get_last_map_guid() { return m_last_map_guid; };
		void									set_last_map_guid(guid_64 map_guid) { m_last_map_guid = map_guid; };

		void									set_cross_server_save_time(int64 time_sec) { m_cross_server_save_time_sec = time_sec; };
		void									set_need_send_save_end(bool need_send) { m_is_need_send_save_end = need_send; };
		void									set_is_cross_server_save_end(bool is_save_end) { m_is_cross_server_save_end = is_save_end; };
		void									func_session_load_dp_end();

		void									get_log_common_head_info(s_log_common_head &log_head);
		xstring									get_class_name();
		void									check_is_robot();
		bool									get_is_robot() { return m_is_robot_account; };
		login_fixed_data&	                    get_third_info() { return m_login_third_data; }
		int32                                   get_login_type() { return m_login_type; }
		s_unit_info&                            get_unit_info() { return m_role_info; }

		bool                                    get_m_is_in_assist() { return m_is_in_assist; }
		void                                    set_m_is_in_assist(bool value) { m_is_in_assist = value; }
		void                                    set_assit_fight_guid(int64 value) { assit_fight_guid.server_64 = value; }
		int64                                   get_assit_fight_guid() { return assit_fight_guid; }
		int64                                   get_m_activate_code_time() { return m_activate_code_time; }
		void                                    set_m_activate_code_time(int64 value) { m_activate_code_time = value; }

	public:
		bool									m_is_robot_account;
		bool									m_is_data_use;
		int32									m_array_index;
		s_unit_info								m_role_info;
		s_unit_ws_info							m_role_ws_info;
		int32									m_is_send_chat;
		int64									m_worship_value;
		s_client_uid							m_client_uid;
		e_session_status						m_status;
		int32									m_step_num;
		ui8										m_online_state;
		s_transfer_info							m_transfer_info;
		int32									m_dest_group_id;
		int32									m_cs_conn_index;
		int32									m_cs_array_index;
		xchar									m_account[max_account_length + 1];
		int32									m_login_type;
		int32									m_login_type_plus;
		login_fixed_data						m_login_third_data;
		xstring									m_ip_address;
		uint32									m_cross_callback_val[limit_call_back_val_count];
		guid_64									m_map_guid;
		int32									m_line_id;

		guid_64									m_team_guid;

		int64									m_logintime;
		relation_mgr							m_relation_mgr;

		bool									m_is_already_login;
		int64									m_logout_time;
		bool									m_logout_begin;
		int64									m_activate_code_time;

		int64									m_update_time_count;
		int32									m_out_send_count;
		double									m_role_data_ws_ex[e_role_data_ws_ex_max];
		int64									m_ban_role_array[faith::max_character_num];
		int64									m_ban_chat_array[faith::max_character_num];
		int64									m_login_time;
		int32									m_jewel_num;

		int64									m_last_send_world_boss_damage_list_sec;

		int64									m_client_session_tick_min_last;

		bool									m_is_need_send_cross_server_notice;

		s_player_legion_info					m_player_legion_info;

		guid_64									m_last_map_guid;

		int64									m_recharge_time;

		bool									m_is_cross_server_save_end;
		int64									m_cross_server_save_time_sec;
		bool									m_is_need_send_save_end;

		bool									m_is_in_assist;
		guid_64									assit_fight_guid;
	};

}

#endif
