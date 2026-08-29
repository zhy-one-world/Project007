/********************************************************************
created: 2017年7月7日
file base: team_cs_mgr
file ext: h
author: Kero
purpose: team mgr in cs
*********************************************************************/
#ifndef _TEAM_CS_MGR_HPP_
#define _TEAM_CS_MGR_HPP_

#include "Logic/team_def.hpp"
#include "Logic/ai_def.hpp"

namespace faith
{
	class player;		
	class team_proto_set_player_team_aoi_info;
	class team_proto_create_team;
	class team_proto_join_team;
	class team_proto_update_team_attribute;

	class team_set
	{
	public:
		team_set();
		~team_set();
		void clear_data();

		void init(guid_64 team_guid);

	public:
		guid_64 get_team_guid();


		player* get_player_ptr(const int32 index);

		int32 get_team_member_num();

		bool join_team(player* player_ptr);

		bool has_in_team(guid_64 player_guid);

	private:
		void add_to_team(player* player_ptr);

	private:
		guid_64				 m_team_guid;
		std::vector<player*> m_team_members;
		int32				 team_member_num;
	};

	class team_cs_mgr
	{
	public:
		team_cs_mgr();
		~team_cs_mgr(void){};
		
		void						set_unit_array_index(int32 unit_array_index);
		void						clear_data();
		s_player_team_info&			get_unit_team_info() { return m_unit_team_info; }
		void						set_player_team_info(const s_player_team_info& player_team_info, bool is_player_init = false);
		void						clear_player_team_info();
		void						fill_aoi_info_msg(team_proto_set_player_team_aoi_info& msg_data);

		void						gen_player_team_member_info(s_team_member_info& team_mem_info, e_team_type team_type = e_team_type_none, int32 team_sub_type_id = -1);
		void						send_leave_team_to_ws();
		void						on_player_reconnect();
		void						on_team_captain_changed(const s_player_team_info& old_team_info);
		void						on_team_type_changed(const s_player_team_info& old_team_info);
		void						on_player_transfer_finished();
		bool						is_in_same_legion(guid_64& my_guid, guid_64& other_mem_guid);

		void						on_enter_team(const s_player_team_info& old_team_info);
		void						on_leave_team(const s_player_team_info& old_team_info);
		
		void						share_team_mission_npc_kill(int32 npc_id, hate_info_vector const& hate_list);
		void						share_team_exp(void* npc_ptr);
		uint32						calc_team_extra_fix_num(int32 map_template_id);
		//void						share_team_drop_item(const guid_64& map_guid, int32 drop_template_id, s_map_pos& init_pos, s_map_pos& init_target_pos);

		player*						get_captain_from_aoi();
		e_team_type					get_team_type();
		bool						is_assist_in_raid();
		void						set_is_not_drop(bool not_drop) { m_is_not_drop = not_drop; }
		bool						get_is_not_drop() { return m_is_not_drop; }
		e_mission_slot				get_misssion_index_by_team_type(int32 team_type = -1);
		int32						get_team_sub_type_id();
		void						set_team_aim_info(e_team_type team_type, int32 sub_id);

		void						sync_mission_id();
		void						refresh_mission_id();
		void						refresh_mission_id_when_cant_finish();
		void						check_update_team_mission();
		void						give_captain_reward();

		void						check_raid_team_finish_state(); //一条龙扣完副本次数用
		void						check_one_stop_finish_flags(bool is_player_init = false); //一条龙完成日常 讨伐用
		void						check_new_aim_for_one_stop(bool from_raid_map = true);
		int32						get_self_one_stop_flags(int32 temp = -1);
		void						get_self_adapt_team_aim(e_team_type& adapt_type, int32& adapt_sub_id);

	public:
		static bool					is_team_type_raid(e_team_type team_type);
		static bool					is_map_type_can_assist(e_map_type map_type, int32 map_order);

	public:
		static void					c2cs_create_team(uint32 connindex, const void *data_ptr, size_t data_len);
		static void                     c2cs_create_team_logic(player& player_ref, team_proto_create_team& req);
		static void                     c2cs_create_team_lua(player& player_ref, const char* msg, int32 msg_len);
		static void					c2cs_join_team(uint32 connindex, const void *data_ptr, size_t data_len);
		static void                     c2cs_join_team_logic(player& player_ref, team_proto_join_team& req);
		static void                     c2cs_join_team_lua(player& player_ref, const char* msg, int32 msg_len);
		static void					c2cs_change_team_type(uint32 connindex, const void *data_ptr, size_t data_len);
		static void                     c2cs_change_team_type_logic(player& player_ref, team_proto_update_team_attribute& req);
		static void                     c2cs_change_team_type_lua(player& player_ref, const char* msg, int32 msg_len);


		
		static void					ws2cs_apply_to_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_set_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_clear_player_team_info_func(uint32 conn_index, const void* data_ptr, size_t data_len);
//		static void					ws2cs_join_team_func(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_transfer_team_member_to_scene_func(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_team_common_check_in_cs_func(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_sync_team_start_aim_func(uint32 conn_index, const void* data_ptr, size_t data_len);

		static void					lua_ws2cs_apply_to_join_team_func(uint32 conn_index, const char * data_ptr, int32 data_len);
		static void					lua_ws2cs_set_player_team_info_func(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_clear_player_team_info_func(uint32 conn_index, const char* data_ptr, int32 data_len);

		static void					lua_ws2cs_transfer_team_member_to_scene_func(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_team_common_check_in_cs_func(uint32 conn_index, const char * data_ptr, int32 data_len);
		static void					lua_ws2cs_sync_team_start_aim_func(uint32 conn_index, const char * data_ptr, int32 data_len);

		

	private:
		int32						m_unit_array_index;
		bool						m_is_not_drop;	//设置不掉落
		s_player_team_info			m_unit_team_info;
		int32						m_self_one_stop_flags;
	};
}
#endif