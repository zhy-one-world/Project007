/********************************************************************
created: 2017年6月20日
file base: legion_cs_mgr
file ext: h
author: Kero
purpose: legion mgr in cs
*********************************************************************/
#ifndef _LEGION_CS_MGR_HPP_
#define _LEGION_CS_MGR_HPP_

#include "Logic/legion_def.hpp"
#include "Logic/skill_def.hpp"

namespace faith
{
	class legion_cs_mgr
	{
	public:
		legion_cs_mgr();
		~legion_cs_mgr(void){};
		void						set_unit_array_index(int32 unit_array_index);
		void						clear_data();
		s_player_legion_info&		get_legion_info() { return m_unit_legion_info; }
		void						set_legion_info(const s_player_legion_info& legion_info);

	public:
		void						change_legion_terr_buff();
		void						ws2cs_set_legion_info(const void* data_ptr);
		void						ws2cs_clear_legion_info();
		std::vector<int32>&			get_get_legion_boss_award_log() { return m_get_legion_boss_award_log; }
		void						add_get_legion_boss_award_log(int32 template_id);
		bool						exist_get_legion_boss_award_log(int32 template_id);
		int64&						get_last_convenient_join_legion_time() { return m_last_convenient_join_legion_time; }
		void						set_last_convenient_join_legion_time();
		bool						can_convenient_join_legion();
  		static void					on_npc_dead(int32 npc_arr_idx);
		void						send_to_ws_add_legion_event(std::string show_string);
		void						add_kill_boss_event(std::string member_name, int32 boss_id, const std::vector<int32>& item_id_array);

	public:
		static bool					is_legion_boss_map(guid_64 map_guid);
		static void					send_create_legion_result_to_ws(s_legion_member_info& chief_info,guid_64 legion_guid, e_legion_error error_code);

		static void					ws2cs_create_legion_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_legion_donate_money_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_legion_donate_item_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_enter_legion_boss_map_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_get_legion_boss_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_set_get_legion_boss_award_log_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_fire_bonfire_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_add_fuel_to_bonfire_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_legion_rm_recruit_cd_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_draw_bonfire_daily_rwd_fun(uint32 conn_index, const void* data_ptr, size_t data_len);

		static void					lua_ws2cs_create_legion_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_legion_donate_money_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_legion_donate_item_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_enter_legion_boss_map_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_get_legion_boss_award_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_set_get_legion_boss_award_log_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_fire_bonfire_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_add_fuel_to_bonfire_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_legion_rm_recruit_cd_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_draw_bonfire_daily_rwd_fun(uint32 conn_index, const char* data_ptr, int32 data_len);

		static void					c2cs_convenient_join_legion(uint32 connindex, const void *data_ptr, size_t data_len);
		static void					c2cs_draw_mem_daily_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					c2cs_legion_territory_buff_operate(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					c2cs_goto_next_stage_of_bonfire_map(uint32 conn_index, const void* data_ptr, size_t data_len);
		static int32                c2cs_store_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len);
		static int32                c2cs_role_get_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len);
		static int32                c2cs_destroy_legion_warehouse_item(uint32 connindex, const void *data_ptr, size_t data_len);
		
		static void					ws2cs_begin_city_war_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_enter_city_war_map_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_city_war_operate_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_get_construction_buff_fun(uint32 conn_index, const void* data_ptr, size_t data_len);  // 废弃
		static void					ws2cs_get_occupation_daily_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_get_city_war_master_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_del_city_war_master_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_add_city_war_winner_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_del_city_war_winner_stuff_fun(uint32 conn_index, const void* data_ptr, size_t data_len);

		static void					ws2cs_level_up_legion_skill_func(uint32 conn_index, const void* data_ptr, size_t data_len); // 未调用 legion_skill
		static void					ws2cs_get_bonus_award_fun(uint32 conn_index, const void* data_ptr, size_t data_len);
		static int32				get_bonus_gold_num(guid_64 role_guid, int32 bonus_enum,int32 sub_type, float award_rate);
		static void					ws2cs_answer_question_right_func(uint32 conn_index, const void* data_ptr, size_t data_len);


		static void					lua_ws2cs_begin_city_war_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_enter_city_war_map_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_city_war_operate_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_get_occupation_daily_award_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_get_city_war_master_stuff_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_del_city_war_master_stuff_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_add_city_war_winner_stuff_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_del_city_war_winner_stuff_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_get_bonus_award_fun(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_answer_question_right_func(uint32 conn_index, const char* data_ptr, int32 data_len);




		static void					ws2cs_operation_legion_warehouse_item(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_confirm_change_legion_name_func(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_rem_item_change_legion_name_care_func(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					ws2cs_change_gate_legion_name_func(uint32 conn_index, const void* data_ptr, size_t data_len);

		//static void				ws2cs_operation_legion_warehouse_item(uint32 conn_index, const void* data_ptr, size_t data_len);
		static void					lua_ws2cs_confirm_change_legion_name_func(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_rem_item_change_legion_name_care_func(uint32 conn_index, const char* data_ptr, int32 data_len);
		static void					lua_ws2cs_change_gate_legion_name_func(uint32 conn_index, const char* data_ptr, int32 data_len);


		static void					add_cross_server_boss_kill_legion_log(int32 npc_arr_idx);
	public:
		bool						is_city_master() { return m_is_city_master; };
		void						set_is_city_master(bool is_city_master) { m_is_city_master = is_city_master; };
		void						set_is_win_city_war_member(bool is_member) { m_is_win_city_war_legion_member = is_member; };
		bool						is_win_city_war_member() { return m_is_win_city_war_legion_member; };

		bool						add_mount_legion_reward(const int32& mount_item_template_id, bool is_force_equip = false);
		bool						del_mount_legion_reward(const int32& mount_item_template_id);
		bool						add_wing_legion_reward(const int32& wing_item_template_id, bool is_force_equip = false);
		bool						del_wing_legion_reward(const int32& wing_item_template_id);
		bool						add_title_legion_reward(const int32& title_template_id, bool is_force_equip = false);
		bool						del_title_legion_reward(const int32& title_template_id);

		void						add_city_master_wing();
		void						del_city_master_wing();
		void						add_city_master_buff();
		void						del_city_master_buff();
		void						add_city_master_title();
		void						del_city_master_title();
		void						add_city_master_mount();
		void						del_city_master_mount();
		void						change_city_master_stuff();
		void						add_city_war_winner_title();
		void						del_city_war_winner_title();
		void						add_city_war_winner_mount();
		void						del_city_war_winner_mount();
		void						add_city_war_winner_buff();
		void						del_city_war_winner_buff();
		void						change_city_war_winner_legion_title_and_mount();
		
		void						delete_legion_skill();
		std::vector<int32>			get_all_legion_skill_level();
		void						send_all_legion_skill_level_to_client();

		int32						get_cur_legion_territory_safe_id();
		float						get_cur_territory_exp_fix();
		void						set_occupied_territorys();
		e_legion_job_title			get_legion_job_title() { return m_unit_legion_info.job_title; }
		void						del_territory_buff(int32 unit_index);
		void						add_territory_buff(int32 unit_index, int32 territory_id);
		void                     set_legion_skill_data(int64 role_guid,int32 legion_skill_id,int32 curr_level);
	private:
		int32						m_unit_array_index;
		
		s_player_legion_info		m_unit_legion_info;

		std::vector<int32>			m_get_legion_boss_award_log;
		int64						m_last_convenient_join_legion_time;

		std::vector<int32>			m_occupied_safe_ids;

		bool						m_is_city_master;
		bool						m_is_win_city_war_legion_member;
		int32						m_last_terr_safe_id;
	};

}
#endif