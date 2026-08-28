/********************************************************************
	created:	2016年10月18日21:38:05
	file base:	map_object
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once

#include "utility/score_indicator.h"
#include "../player.hpp"
#include "npc_def.hpp"

namespace hld
{
	struct ws2cs_map_dynamic_params;

	class map_object
	{
	public:
		map_object();
		virtual ~map_object();
	public:
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
	public:
		// 这个方法中会自动设置m_map_template_id和m_map_template_ptr
		bool					init_map_data(const guid_64 map_guid, int32 template_id, int32 line_id, int32 map_all_time);
	public:
		virtual bool			create_by_template_data(const guid_64& map_guid, int32 template_id, int32 line_id, int32 map_all_time) { return false; }
		virtual void			heart_tick(const int64& new_time) {}
		virtual void			clear_data();
	public:
		bool					is_can_revive(e_revive_type revive_type, int32 player_idx = -1);
		bool					is_can_change_pk_mode();
		inline int32			get_max_player_count() { return (m_map_template_ptr != nullptr ? m_map_template_ptr->MaxPlayerCount : 1); }
		inline e_map_type		get_map_type() { return (m_map_template_ptr != nullptr ? (e_map_type)m_map_template_ptr->Type : e_map_type_ui); }
	public:
		bool					is_valid() { return m_map_guid.is_valid(); }
		inline void				set_map_guid(const guid_64& map_guid) { m_map_guid = map_guid; }
		inline const guid_64&	get_map_guid() const { return m_map_guid; }
		inline void				set_map_template_id(int32 map_template_id) { m_map_template_id = map_template_id; }
		inline int32			get_map_template_id() const { return m_map_template_id; }
		inline void				set_is_double_line(int32 is_double_line) { m_is_double_line = is_double_line; }
		inline int32			get_is_double_line() const { return m_is_double_line; }
		inline void				set_map_template_ptr(MapTemplate* map_template_ptr) { m_map_template_ptr = map_template_ptr; }
		inline MapTemplate*		get_map_template_ptr() const { return m_map_template_ptr; }
		int32                   get_map_template_type() { return m_map_template_ptr != nullptr ? m_map_template_ptr->Type : -1; }
		void					set_line_id(int32 line_id) { m_line_id = line_id; }
		int32					get_line_id() const { return m_line_id; }
		inline int32			get_map_state() const { return m_map_state; }
		void					set_game_state_to_player(e_map_state game_state, int32 state_time, int32 unit_index = -1);
		void					send_cur_game_state_to_player(int32 unit_index = -1);
		void					send_cur_wave_trig_state_to_player(int32 unit_index = -1);
		void					clear_wave_trig_state() { m_wave_trig_message_set.clear(); };
		bool					is_in_map_win(e_map_win_cond win_cond);
	public:
		void					add_aoi(const fvector& unit_location, const int32& unit_index);
		void					remove_aoi(const fvector& unit_location, const int32& unit_index, bool is_dead);
		void					update_aoi(const fvector& old_location, const fvector& new_location, const int32& unit_index);
	public:
		bool					add_player(const int32& unit_index);
		int32					get_player_one();
		const unit_index_map&	get_player_all(int32& watch_array_num)const { return m_player_index_list; }
		virtual bool			on_player_add(const int32& unit_index) { return true; }
		void					del_player(const int32& unit_index);
		virtual void			on_player_delete(const int32& unit_index) {}
 		void					player_enter_scene(const int32& unit_index);
		virtual void			on_player_enter_scene(const int32& unit_index) {}
 		void					player_leave_scene(const int32& unit_index);
		virtual void			on_player_leave_scene(const int32& unit_index) {}
		void					player_logout(const int32& unit_index);
		void					player_disconnect(const int32& unit_index);
		virtual void			on_player_disconnect(const int32& unit_index) {}
		void					player_reconnect(const int32& unit_index);
		virtual void			on_player_reconnect(const int32& unit_index);
		void					add_npc(const int32& unit_index);
		virtual void			on_npc_add(const int32& unit_index) {}
 		void					del_npc(const int32& unit_index, bool is_dead);
		virtual void			on_npc_delete(const int32& unit_index, bool is_dead) {}
		void					unit_dead(const int32& unit_index);
		void					npc_dead(const int32& unit_index);
		void					player_dead(const int32& unit_index);
		virtual void			on_player_dead(const int32& unit_index) {}
		virtual void			on_npc_dead(const int32& unit_index) {}
		unit_guid_map_it		get_player_dead_stamp(int32 unit_index);
		player*					get_first_valid_player();
		npc*					get_first_valid_npc(int32 npc_template_id);
		npc*					get_npc_one();
		void					calcu_player_avg_level();
		void					caclu_now_player_avg_level() ;
		int32					get_now_player_avg_level()const { return m_now_player_avg_lv; };
		int32					get_player_avg_lv() const { return m_player_avg_lv; }
		void					kill_all_monster(const int32& unit_array_index, int32 buff_damage_num = int32_MAX_NUM);
		void					remove_all_monster();
		virtual npc*			spawn_npc(int32 spawn_point_id, int32 dynamic_lv = -1);
		virtual void			on_unit_be_damage(const int32& attack_index, const int32& be_attack_index, double damage_value) {}
		virtual int32			get_player_num();
		virtual int32			get_npc_num();
		virtual void			handle_dynamic_params_msg(const ws2cs_map_dynamic_params & msg) {};
		virtual void			on_npc_interact_success(guid_64 npc_guid, guid_64 role_guid) {}
		virtual void			on_npc_interact_failed(guid_64 npc_guid, guid_64 role_guid) {}
		virtual void			get_game_over_gift(int32 unit_index, int32 gift_num = 1){}
		virtual void			send_message_to_all_player(google::protobuf::Message* net_pro, uint32 header);
		virtual void			send_message_to_one_player(google::protobuf::Message* net_pro, uint32 header, int32 unit_index);
		virtual void			transfer_all_player(int32 map_template_id = 0);
		virtual void			send_notice_to_all_player(std::string notice_str);
		virtual void			send_notice_to_all_player(int32 notice_id, std::string notice_str);
		virtual void			on_pop_plot_tip(int32 str_id) {}
		virtual float			get_time_left(int32 player_idx = -1) { return m_map_state_time; } // 获得场景的当前剩余时间
		virtual float			get_time_passed();
		virtual s_map_pos		get_born_pos(int32 unit_index);
		virtual bool			is_game_over() { return false; }
		virtual void			check_default_pk_mode(const int32& unit_index);
		virtual e_pk_mode_type	get_map_pk_mode();
		void					set_map_pk_mode(e_pk_mode_type mode_type);
		void					handle_remaining_drop_item_by_email(const int32 unit_index);
		bool					have_be_assisted_mem(guid_64 team_guid);
		void					set_legion_average_lv(int32 legion_average_lv) { m_legion_average_lv = legion_average_lv; }
		int32					get_legion_average_lv() const { return m_legion_average_lv; }
		void					set_map_own_legion(guid_64 legion_guid) { m_map_own_legion = legion_guid; }
		guid_64					get_map_own_legion() const { return m_map_own_legion; }
		void					add_buff_to_all_player(int32 buff_temp_id);
		virtual void			get_transfer_map_pos(int32 unit_index, s_map_pos& tem_pos, int32 war_index = 0);

		virtual void			add_buff(int32 unit_index, int32 buff_templater_id) {}
	protected:
		bool					have_info_panel(e_map_info_panel_type info_panel_type);
		bool					is_fuben_boss(const int32& unit_index);
		virtual void			game_over();
		void					finish_log(player& temp_player);
		void					finish_log(player& temp_player, int32 temp_is_win);

		void					send_team_game_over();

	public:
		void					send_game_state(e_map_state map_state);
		void					send_wave_trigger_message_to_all_player(int32 message);
		void					send_boss_state_message_to_all_player(int32 boss_id, e_boss_state boss_state, bool is_final_boss);
		static int32			get_activity_map_sec_left(int32 act_map_type, int32 player_idx = -1);
		void					relive_all_player();

		virtual int64			get_map_first_join_time() { return m_first_player_join_time; };
		virtual int64			get_map_life_time();//获取当前地图从第一个玩家进入以后已经存在的时间

		void					send_map_target_list_to_all();
		void					send_map_target_list(guid_64 role_guid);
		void					add_map_target_info(s_map_target_show_info target_info, bool is_send = false);
	private:
		guid_64						m_map_guid;
		int32						m_line_id;
		int32						m_map_template_id;
	protected:
		unit_index_map				m_player_index_list;
		unit_index_map				m_npc_index_list;

		MapTemplate*				m_map_template_ptr;
		int32						m_map_type;
		float						m_map_all_time;
		e_map_state					m_map_state;
		float						m_map_state_time;
		bool						m_is_win;
		int32						m_is_double_line;
		std::set<int32>				m_wave_trig_message_set;

		int64						m_end_time;
		int64						m_begin_map_time;
		int64						m_first_player_join_time;
		
		score_indicator				m_score_rank_indicator;//积分排行 不同副本中不同 可能是伤害排行或其他

		int32						m_map_pk_mode;	//地图是否可pk
		guid_64						m_map_own_legion;
		int32						m_player_avg_lv;
		unit_guid_map				m_dead_player_map;
		int32						m_legion_average_lv;

		int32						m_now_player_avg_lv;//实时等级，每次取之前先调用计算

		std::vector<guid_64>		m_enter_map_array;	//用来记录是否是第一次进入地图
		std::vector<s_map_target_show_info>	m_map_target_show_info;
	};
}
