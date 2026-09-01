#ifndef _WS_TEAM_WS_MGR_H_
#define _WS_TEAM_WS_MGR_H_

#include <core.hpp>
#include "logic/type_def.hpp"
#include "team.pb.h"
#include "team_ws.h"

#include "ws_client.hpp"


namespace faith
{
	class team_ws_mgr
	{
		typedef std::map<ui64, team_ws>		team_ws_map; // map的键值对为[team的GUID/team_ws实例]
		typedef std::map<int64, guid_64>		team_create_stamp_map; // map的键值对为[team的创建时间戳/team_ws的guid]
		typedef std::map<ui64, guid_64>		role_team_pair_map; // map的键值对为[role的GUID/team的GUID]
	private:
		team_ws_mgr();
		~team_ws_mgr();
	public:
		static team_ws_mgr&		get_instance()
		{
			static team_ws_mgr instance;
			return instance;
		}

		void					tick(const uint64& time_now);

		team_ws*				get_team(guid_64 team_guid);
		team_ws*				add_team(team_ws& new_team);
		void					del_team(guid_64 team_guid);
		void					set_unit_team(const guid_64& unit_guid, const guid_64& team_guid);
		team_ws*				get_unit_team(const guid_64& unit_guid);
		void					clear_unit_team(const guid_64& unit_guid);
		team_ws*				create_new_team(client_session* session, const s_team_member_info& captain_info, e_team_type team_type, int32 sub_type_id, bool is_auto_match, int32 one_stop_val = -1, int32 required_fp = 0, int32 required_lv = 0);
		void					gen_player_team_mem_info_ws(client_session* session, s_team_member_info& team_mem_info);

		void					get_team_ws_list_by_map_id(int32 map_template_id, std::vector<team_ws*>& ret_team_list);
		bool					is_two_player_in_same_team(guid_64 player1, guid_64 player2);

		void					send_msg_to_all_member(guid_64 sender_guid, google::protobuf::Message* net_pro, uint32 header);
		void					send_team_error_to_session(client_session* session, e_team_error err_type);
		int32					send_recruit_proc(guid_64 team_guid, int32 template_id, int32 activites_type, int32 recruit_type, guid_64 sender_guid, xstring sender_name, int32 role_template_id, int32 one_dragon_flag);
		
		bool					team_condition_common_check(team_ws& team_ws_ref, client_session* session = nullptr);
		void					fill_team_list_msg_by_aim(team_proto_get_team_list_by_aim_end& get_team_list_msg, e_team_type team_type, int32 sub_type_id, client_session* session = nullptr);
		void					fill_team_list_msg_all(team_proto_get_team_list_by_aim_end& get_team_list_msg, const unit_index_map& can_transfer_sub_ids, client_session* session = nullptr);
		e_player_match_team_ret	player_auto_match_team(e_team_type team_type, int32 sub_type, client_session* session, e_error_code map_check_ret = e_error_code_success, int32 self_one_stop_flags = -1, bool is_one_stop = false);
		void					player_stop_auto_match_team(guid_64 player_guid);
		player_auto_match_map&	get_auto_matching_players() { return m_auto_matching_players; }
		e_team_type				get_team_type_by_sub_id(int32 sub_type_id);
		bool					is_team_type_raid(e_team_type team_type);
		bool					is_team_type_have_sub_id(e_team_type team_type);
		bool					is_team_type_valid(e_team_type team_type, int32 sub_type_id);

		void					get_team_aims_by_os_flags(int32 os_flags, e_team_type& team_type, int32& sub_type_id);

		void					on_unit_leave_map(client_session* session, int32 unit_num_after_rm);
		void					on_team_map_recycle(guid_64 team_guid, guid_64 recycling_map_guid);
		void					on_player_online(client_session* session);
		void					on_player_offline(client_session* session);
		void					on_player_login(client_session* session);
		void					on_player_logout(client_session* session);
		void					req_condition_check_to_cs(client_session* session, e_team_common_check_invoker invoker, e_team_type team_type, int32 team_sub_type_id, guid_64 team_guid);
		void					recv_condition_check_from_cs(int32 conn_index, const void* data_ptr, size_t data_len);

		void					req_no_team_mate_list(client_session* session, e_team_req_mate_type mate_type);

		void					notice_all_player_team_created(team_ws& team_ref);
		void					change_player_name_func(const guid_64& role_guid, const xstring& role_name);	//角色改名同步

	private:
		team_ws_map				m_team_ws_map;
		team_create_stamp_map	m_team_create_stamp_map;
		role_team_pair_map		m_role_team_pair_map;
		int32					m_timer;

		player_auto_match_map	m_auto_matching_players;
	};
}

#endif