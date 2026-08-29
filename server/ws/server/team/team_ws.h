#ifndef _WS_TEAM_WS_H_
#define _WS_TEAM_WS_H_

#include "server/client_session_mgr.hpp"
#include "logic/type_def.hpp"
#include "team.pb.h"
#include "Logic/team_def.hpp"
#include "Logic/game_enum_def_s.h"

namespace faith
{
	struct team_ready_transfer_info
	{
		bool	is_active;
		uint64	ready_time;
		int32	member_num;
		int32	map_template_id;

		team_ready_transfer_info()
		{
			clear_data();
		}

		void clear_data()
		{
			is_active = false;
			ready_time = 0;
			member_num = 0;
			map_template_id = 0;
		}
	};

	enum notice_id_team_ws_operate
	{
		e_team_add_member_notice_id = 93000050,
		e_team_del_member_notice_id = 93000051,
		e_team_change_captain_notice_id = 93000052,
		e_team_max
	};

	class team_ws
	{
	public:
		explicit team_ws(e_team_type tm_type = e_team_type_none);
		~team_ws();
		void								clear_data();

		void								tick(const uint64& time_now);
		void								tick_1_sec(const uint64& time_now);
		void								tick_3_sec(const uint64& time_now);
		void								tick_1_min(const uint64& time_now);

		bool								add_member(client_session* mem_session, s_team_member_info member_info, bool check_condition = false);
		bool								del_member(guid_64 player_guid);
		void								del_member_common_op(guid_64 player_guid);
		void								clear_member();
		s_team_member_info*					get_member(guid_64 player_guid);
		int32								get_member_num() const;
		bool								update_member(client_session* mem_session, s_team_member_info member_info);

		guid_64								get_team_guid() const { return m_team_guid; }
		void								set_team_map_guid(guid_64 map_guid)	{ m_team_map_guid = map_guid; }
		guid_64								get_team_map_guid() { return m_team_map_guid; }
		void								set_team_type(e_team_type t_type);
		e_team_type							get_team_type() const { return m_team_type; }
		void								set_team_sub_type_id(int32 sub_type) { m_team_sub_type_id = sub_type; }
		int32								get_team_sub_type_id() const { return m_team_sub_type_id; }
		bool								set_one_stop_flags(int32 flags_val);
		int32								get_one_stop_flags() const { return m_one_stop_flags; }
		bool								is_one_stop_team() const { return m_one_stop_flags >= 0; }
		int32								get_team_map_tempalte_id();
		bool								change_team_aim(e_team_type new_team_type, int32 new_sub_type_id);
		void								sync_team_attribute_to_mems();
		bool								is_element_war();

		void								update_recruit_info_to_client(bool is_delete = false);
		void								check_team_captain_offline(const uint64& time_now);
		void								set_member_online(client_session* mem_session,  bool is_online);
		bool								get_member_online(guid_64 player_guid);
		bool								is_all_member_offline();
		bool								is_all_other_member_offline(guid_64 player_guid);
		void								dissolve_team(); // 解散队伍
		void								on_team_delete(); // 解散队伍
		void								on_team_mem_online(client_session* session);
		void								on_team_mem_offline(client_session* session);
		void								on_team_mem_login(client_session* session);
		void								on_team_mem_logout(client_session* session);
		void								set_member_check_ret(guid_64 player_guid, e_error_code check_ret);
		e_error_code						get_member_check_ret(guid_64 player_guid);
		void								reset_mems_check_ret();
		void								set_member_one_stop_flags(guid_64 player_guid, int32 _flags);
		int32								get_member_one_stop_flags(guid_64 player_guid);

		void								update_member_map(client_session* session, guid_64 map_guid);
		
		void								set_captain_guid(guid_64 player_guid) { m_captain_guid = player_guid; }
		bool								set_captain(guid_64 player_guid);
		bool								change_captain();
		guid_64								get_captain_guid() { return m_captain_guid; }

		const std::list<s_team_member_info>& get_member_list() { return m_members_list; }

		int32								get_required_fighting_power() const { return m_required_fighting_power; }
		void								set_required_fighting_power(int32 fighting_power) { m_required_fighting_power = fighting_power; }
		
		int32								get_required_level() const { return m_required_level; }
		void								set_required_level(int32 level) { m_required_level = level; }

		bool								is_auto_matching() const { return m_is_auto_matching; }
		void								set_auto_matching(bool auto_matching);

		bool								is_dissolved() const { return m_is_dissolved; }

		// 传送相关操作
		void								ready_transfer_all_member_to_scene();
		bool								is_ready_transfer_now() { return m_ready_transfer_info.is_active; }
		void								set_ready_transfer_info(int32 map_template_id);
		void								ready_transfer_tick(const uint64& time_now);
		bool								check_can_start_transfer();
		e_team_error						transfer_all_member_to_scene(int32 map_template_id, bool need_ready = true);
		void								on_team_map_recycle();
		void								on_team_game_over();

		// 成员准备相关操作
		void								set_member_ready_status(guid_64 member_guid, bool mem_ready, e_error_code check_ret = e_error_code_success);
		bool								is_all_member_ready();
		void								clear_all_member_ready_state();
		int32								check_and_set_members_ready_state();
		bool								is_mem_cur_map_can_ready_trans(const s_team_member_info& mem_info);
		void								try_start_team_aim();
		void								sync_to_cs_start_team_aim();

		// 队伍状态相关操作
		e_team_state						get_team_state() const { return m_team_state; }
		void								set_team_state(e_team_state team_state) { m_team_state = team_state; }
		bool								can_team_state_add_member() const; // 判断队伍当前的状态是否可以加入新成员
		//bool								can_team_state_start_ready();
		bool								can_team_state_ready(); // 判断队伍当前的状态是否可以让成员准备
		bool								can_team_state_start_transfer();
		bool								can_team_state_be_searched() const;
		void								change_team_type(cs2ws_change_team_type msg);

		void								get_team_brief_info_pkg(team_proto_team_brief_info* brief_info_msg_ptr);

		void								update_team_member_position(guid_64 role_guid, int32 pos_x, int32 pos_y, int32 pos_z);					 //刷新队伍玩家位置信息
		void								send_all_team_member_position();					 //发送队伍玩家位置信息

		bool								fill_team_info_msg(team_proto_team_info& team_info_msg);
		void								fill_team_member_info_msg(team_proto_member_info& team_member_info_msg, const s_team_member_info& member_info);
		
		void								send_cs_msg_to_all_member(const void* data_ptr, size_t data_len);
		void								send_message_to_all_member(google::protobuf::Message* net_pro, uint32 header);
		void								send_message_to_all_member_except(google::protobuf::Message* net_pro, uint32 header, guid_64 except_role_guid);

		void								send_team_info_to_session(client_session* session, bool is_when_join = false);
		void								send_team_info_to_all_member();
		void								send_add_team_member_message(const s_team_member_info& new_member_info);
		void								send_del_team_member_message(const guid_64& leave_member_guid);
		void								send_claer_team_member_message(const guid_64& leave_member_guid);//用于跨服时清理客户端组队信息

		void								send_aoi_team_info(client_session* session);
		void								send_all_member_aoi_team_info();
		void								update_team_member_info_one(guid_64 role_guid, int32 info_index, int32 info_value, bool need_sync2c);
		void								send_team_member_info_one(guid_64 role_guid, int32 info_index);
		void								send_team_gs_value(guid_64 role_guid);
		void								send_set_captain_message(guid_64 new_captain_guid);
		void								send_team_error_to_all_member(e_team_error error_type);
		void								set_recruit_type(e_recurit_type new_type);
		void								gen_player_team_info(s_player_team_info& team_info);

		void								req_add_member_to_cs(client_session* session);
		void								match_waiting_players();

		bool								is_check_sub_id_for_platform();
		void								on_team_attribute_changed();
		void								stop_auto_match();

		void								sync_all_member_check_ret();

		void								join_voice_channel(guid_64 role_guid);
		void								leave_voie_channel(guid_64 role_guid);


		void								add_invite_guid(guid_64 invite_guid);
		void								del_invite_guid(guid_64 invite_guid);
		bool								is_have_invite_guid(guid_64 invite_guid);

		bool								check_all_commmon_legion();
		void								add_assist_fight_notice();

		void								set_is_need_save_team(bool need_save) { m_is_need_save_team = need_save; };

	private:
		void								init_team_state(e_team_type team_type); // 设置队伍的初始状态
		/************************************************************************/
		/*                          公告                                        */
		/************************************************************************/
		void								notice_add_member(FString member_name);
		void								notice_del_member(FString member_name);
		void								notice_change_captain(FString member_name);

		void								debug_log_team_operate(xstring log_str);




	private:
		guid_64								m_team_guid;
		e_team_type							m_team_type;
		int32								m_team_sub_type_id;
		//int32								m_continuous_line_begin;
		int32								m_one_stop_flags; //一条龙标记 -1无效 >=0有效
		guid_64								m_team_map_guid;
		guid_64								m_captain_guid;
		e_team_state						m_team_state;
		int32								m_required_fighting_power;
		int32								m_required_level;
		std::list<s_team_member_info>		m_members_list;
		player_session_map					m_member_sessions; //维护这个可能会有bug 要谨慎
		e_recurit_type						m_recruit_type;
		uint64								m_check_captain_offline_stamp;

		team_ready_transfer_info			m_ready_transfer_info;
		int32								m_checked_ready_mem_num;

		std::set<guid_64>					m_captain_invite_guid;//队长邀请的玩家guid
		bool								m_is_need_save_team;//当所有人都离线的话是否需要保留队伍，用于跨服

		bool								m_is_auto_matching;
		bool								m_is_dissolved;
	public:
		xstring                              get_member_list_name();

	};
}

#endif