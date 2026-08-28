/********************************************************************
	created:	2015/03/24
	created:	24:3:2015   20:43
	file base:	npc
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once
/************************************************************************/
/*                           File Include                               */
/************************************************************************/
#include "logic/unit.hpp"
#include "ai/ai_unit.h"
#include "Logic/gain_treasure_def.hpp"
#include "Logic/npc_def.hpp"
#include "utility/score_indicator.h"
/************************************************************************/
/*                            Maroc Define                              */
/************************************************************************/
namespace hld
{
	enum e_dropbox_owner_type;
	struct s_fake_player_info;
	struct box_award_component;
	struct wheel_plate_component;
	struct treasure_pic_component;

	/************************************************************************/
	/*                            Class Declare                             */
	/************************************************************************/
	// 所有场景逻辑对象的基类
	class npc : public unit
	{
	public:
		npc();
		virtual ~npc();
	private://内存已经创建好了 禁止拷贝
		npc(const npc& unit_ref);
		npc& operator=(const npc&);
	public: 
		static	npc*	cast(unit* unit_ptr);
	public:
		virtual void	heart_tick(const int64& new_time, const int32& tick_time);
		virtual void	clear_data();
		bool			init_by_template_data(int32 template_id, s_map_pos born_pos, Entity* map_ent, int32 map_template_id, int32 line_id, player* player_ptr, const s_map_pos& patrol_pos = s_map_pos(),int32 show_wing_id = 0);
		bool			init_fake_player_data(int32 npc_template_id, const s_fake_player_info& fake_data, Entity* map_ent, s_map_pos born_pos, int32 map_template_id, int32 npc_type = e_npc_type_lord_city);
		bool			init_player_data_robot(int32 npc_template_id, const s_fake_player_info& fake_data, Entity* map_ent, s_map_pos born_pos, int32 map_template_id, int32 npc_type = e_npc_type_lord_city);
		bool			init_by_robot_data(int32 robot_template_id, int32 npc_template_id, s_map_pos born_pos, Entity* map_ent, int32 map_template_id);
		void			init_att_over();
		virtual void	send_message_to_self(void* data, uint32 date_len) {}
		virtual void	send_message_to_aoi(google::protobuf::Message* net_pro, uint32 header, bool to_self = true, bool use_length = false);
		virtual void	get_aoi_msg(hld::aoi_proto_unit_aoi_all& msg);
		virtual void	be_attacked(const int32& attack_index, const double hp);
		virtual	void	be_dead(bool is_auto_put_into_bag = false);
		virtual	bool	can_become_target();
		virtual bool	can_attack_other(int32 target_unit_idx = -1);
		int32			get_spawn_point_id(){ return m_spawn_point_id; }
		void			set_spawn_point_id(int32 point_id);
		void			creat_dead_chests();
		void			get_drop_chest_pos(std::vector<fvector>& pos);

		int32			get_item_id() { return m_item_id; }
		void			set_item_id(int32 item_id) { m_item_id = item_id; }
		int32			get_item_lock() { return m_item_lock; }
		void			set_item_lock(int32 item_lock_State) { m_item_lock = item_lock_State; }

		virtual bool	check_can_show_head(guid_64 unit_guid);

		virtual int32   is_control_boss();
		bool            is_area_npc();

		NpcTemplate*	get_npc_template() const { return m_npc_template_ptr; };
		int32			get_npc_template_id() const; 

		const s_unit_identifier&	get_owner() { return m_owner_unit_identifier; }
		void						set_owner(const s_unit_identifier& unit_identifier);
		e_dropbox_owner_type		get_owner_type();
		void						set_owner_type(e_dropbox_owner_type owner_type);

		int32			get_born_anim() { return m_born_type; }
		void			set_born_anim(int32 born_type) { m_born_type = born_type; }

		int64			get_residue_born_time();
		void			set_residue_born_time(int64 born_stamp) { m_residue_born_time = born_stamp; }

		void            set_live_time(int64 live_time) { m_life_time = live_time; m_is_life_change = true; }
		void            set_show_map(bool show_map) { init_att_over(); m_show_map = show_map; }
		std::vector<float> const& get_init_poroperty_ratio() const { return m_init_poroperty_ratio;}
		std::vector<float> const& get_init_poroperty_plus() const { return m_init_property_plus; }
		
		void			set_level_and_upgrade_id(int32 new_level, int32 npc_temp_id);

		std::vector<team_set>			get_aoi_team_hate_list();

		void							tick_clamor(const int64& tick_dt);
		void							npc_send_notice_with_location(int32 notice_id, std::string* string_param = nullptr, int32 string_param_num = 0, int32 sender_name_id = 0);

	public:
		const ai_unit&					get_ai_unit() { return m_ai_unit; }
		int32							get_first_attacker() { return m_ai_unit.get_first_attack(); }
		void							relieve_stop_time(const int32& skill_template_id);
		void							add_damage_value_with_summon(const int32& attack_index, const double hp);

		void							boss_remove_if_crazy();
	public:
		virtual void					aoi_watch_all_data_out(int32 unit_index);
		bool							restore_cache_hate_val(int32 unit_index, int64 dead_ms);

		virtual guid_64&				get_legion_guid();
		virtual guid_64&				get_team_guid();
		virtual e_unit_type				get_owner_unit_type();
		virtual int32					get_owner_unit_level();

		void							send_plot_with_npc_hp();
		void							refresh_npc_left_hp_per();	//先把血量更新到ws 再更新到所有客户端 每下降百分之10更新一次
		void							send_npc_left_hp_per();
		void							refresh_npc_damaget_list(const int32& unit_array_index, double damage_value, player& player_ref, e_boss_type boss_type);
		void							send_damage_score_to_ws();
		void							send_legion_score_to_ws();
		void							check_damage_rewards(int32 unit_index, int64 damage, e_activity_type activity_type);

		void							send_gain_treasure_record_to_ws(int32 killed_stamp, const std::vector<s_gain_treasure_record_player_info_one_things>& player_info_arr);
		void							send_gain_treasure_boss_owner_to_ws();
		bool							send_gain_treasure_boss_info_when_dead();
		void							send_gain_treasure_boss_info_when_spawn();
		bool							is_gain_treasure_boss();
		bool							is_island_boss();
		bool							is_belief_cloister_boss();
		bool							is_boss();
		virtual bool					is_mission_see(int32 unit_array_index);

		void							set_drop_npc_temp_id(int32 npc_temp_id) { m_drop_with_npc_temp_id = npc_temp_id; };
		int32							get_drop_npc_temp_id() { return m_drop_with_npc_temp_id; };

		void							mark_kill_legion_boss_player(const hate_info_vector& hate_list);

		void							npc_change_legion_name_func(const guid_64& legion_guid, const xchar* legion_name);
		void							init_legion_score_rank(const guid_64& legion_guid, const xchar* legion_name, int64 role_score = 0, int32 server_id = 0);
		score_indicator					get_legion_score_rank() {return m_legion_score_rank_indicator;}
		score_indicator					get_play_score_rank() { return m_score_rank_indicator; }

		void							clear_legion_score_rank() { return m_legion_score_rank_indicator.clear_data(); }

		bool							is_cross_server_boss();//判断是否是跨服世界boss
		int32							get_cross_boss_buff();//获得跨服世界boss触发的buffid

		void							set_boss_god_skill_begin_time(int64 begin_time) { m_boss_god_skill_begin_tiem = begin_time; };	//设置无敌开始时间
		int64							get_boss_god_skill_begion_time() {return m_boss_god_skill_begin_tiem;};							//获得无敌开始时间

		void							send_boss_god_skill_begion_time();
		bool							is_god_skill(int32 skill_id);	// 判断是不是跨服世界boss的无敌技能

		bool							is_summon_pet();
		void							cross_boss_send_boss_god_time(guid_64 role_guid);

		void							set_stop_ai(bool b_setting) { m_stop_ai = b_setting; }
		bool							init_cross_ladder_npc(int32 npc_template_id, s_map_pos born_pos, Entity* map_ent, int32 map_template_id, int32 line_id, const s_map_pos& patrol_pos, std::string& name_str, int32 faker_player_template_id, bool need_show_fasion);
		void							set_ai_state(e_ai_state m_state) { m_ai_unit.set_state(m_state); }

		void							record_treasure_log(const int32 map_type);
		void							record_world_boss_log();
		void							record_broken_boss_log();
		void							record_legion_boss_log();
		xstring							load_random_role_name();

		void							set_can_see_player_guid(guid_64 unit_guid);
		guid_64							get_can_see_player_guid();
		void							check_can_see_npc_state();//如果任务怪的玩家下线删除任务怪

		void							robot_be_dead();
		void							send_kill_prompt(int32 kill_player_array_index, bool is_end = false);

		void							fuhuo_npc();
		void							clear_bron_data();
		void							add_ai_level_next_time(int32 add_time);
		void							set_ai_unit_born_pos(s_map_pos born_pos);
		void							clear_hate_value();
		
		void							set_follow_guid(guid_64 role_guid);
		guid_64							get_follow_guid();
	public:
		std::shared_ptr<box_award_component> m_box_award_component;
		std::shared_ptr<treasure_pic_component> m_treasure_pic_component;
		std::shared_ptr<wheel_plate_component> m_wheel_plate_component;
	private:
		NpcTemplate*	m_npc_template_ptr;
		ai_unit			m_ai_unit;
		int32			m_spawn_point_id;
		int32			m_item_id;					// 掉落包包含了什么物品;
		int32			m_item_lock;				// 掉落包物品是否绑定;

		int64			m_life_time;				// npc存活时间
		bool			m_is_life_change;			// npc的存活时间是否需要刷新
		int32			m_born_type;				// 是否在出生阶段
		e_dropbox_owner_type  m_owner_type;
		s_unit_identifier m_owner_unit_identifier;	//归属者的guid
		bool			m_show_map;					//是否显示到地图中了
		int32			m_show_weapon_id;			//冗余数据 用于同步
		std::vector<float> m_init_poroperty_ratio;	//用于初始化属性计算
		std::vector<float> m_init_property_plus;	//用于初始化属性计算
		int64			m_next_clamor_time;				// npc下一次喊话时间
		int64			m_next_clamor_tick;				// npc喊话tick时间

		int32			m_robot_template_id;
		int64			battle_begin_time;
		float			last_hp_scale_flag;				// 剩余血量记录标记
		int32			m_residue_born_time;//距离出场开始的剩余时间
		int64			m_last_sec_stamp;
		int64			m_last_2sec_stamp;
		int64			m_last_3sec_stamp;
		score_indicator				m_score_rank_indicator;//积分排行 针对世界boss
		score_indicator				m_legion_score_rank_indicator;//军团积分排行 针对世界boss
		std::unordered_map<ui64, int32>				m_player_got_damage_reward_map;
		std::vector<s_gain_treasure_player_info>  m_player_info_arr;	//打宝系统拥有拾取权的人

		int32			m_drop_with_npc_temp_id;//哪个npcID掉落的掉落包
		
		int64			m_boss_god_skill_begin_tiem;		//跨服世界boss使用 用于保存无敌技能的开始时间

		int32			m_last_send_plot_hp;

		bool			m_stop_ai;

		guid_64			m_can_see_player_guid;

		int64			m_next_come_back_time;

		int64			m_continuity_kill_end_time_stamp;
		int32			m_kill_num_now;

		bool			m_is_player_data_npc;

		guid_64			m_assist_fight_guid;

		bool			m_is_be_assist_fight_npc;

		int32			m_attack_city_npc_type;

	private:
		/******************************* BOSS伤害列表 (助战+组队) *******************************************************/
		std::map<guid_64, s_boss_damage_list_person_item> m_boss_damage_person_map;
		std::vector<s_boss_damage_list_total_item> m_boss_damage_total_vec;
	public:
		void add_boss_damage_vaule(player* attacker, int64 attack_value);
		void add_boss_damage_vaule(npc* attacker, int64 attack_value);
		void add_boss_damage_total_vaule(guid_64 master_guid, player* attacker, int64 attack_value);
		void add_boss_damage_total_vaule(guid_64 master_guid, npc* attacker, int64 attack_value);
		void send_boss_damage_list_to_player(player* attacker);
		void send_boss_damage_list_to_all_player();
		void send_leave_boss_damage_to_player(player* player_ptr);
		void transfer_damage_to_master(guid_64 role_guid, int32 array_index);
		void transfer_damage_to_killer(guid_64 role_guid, guid_64 killer_guid, int32 kill_index);
		void clear_all_damage(guid_64 role_guid, bool is_master);
		void send_boss_damage_ranking_reward();
		void send_boss_damage_helper_reward(guid_64 role_guid, int32 ranking_num, const std::vector< s_item_template_info >& drop_id_list);
		void create_gain_treasure_drop_record(player& player_ref, const std::vector< s_item_template_info >& item_list, std::vector<s_gain_treasure_record_player_info_one_things>& player_info);

		void change_team_captain(guid_64 old_captain, guid_64 new_captain);
		void role_leave_team(guid_64 role_guid, guid_64 captain_guid);
		void role_add_team(guid_64 role_guid, guid_64 captain_guid);

		static void transfer_damage_to_int32(int64 real_damage_value, int32& damage_value, int32& damage_rate);

		void set_assist_fight_guid(guid_64 role_guid) { m_assist_fight_guid = role_guid; };
		guid_64 get_assist_fight_guid() { return m_assist_fight_guid; };

		void set_is_be_assist_fight_npc(bool is_assist) { m_is_be_assist_fight_npc = is_assist; };
		bool get_is_be_assist_fight_npc() { return m_is_be_assist_fight_npc; };
		
		void send_assist_fight_tip(guid_64 assist_guid, guid_64 team_guid);

		void set_attack_city_npc_type(int32 npc_type) { m_attack_city_npc_type = npc_type; };
		int32 get_attack_city_npc_type() { return m_attack_city_npc_type; };
	public:
		void on_mission_event();
		void sync_boss_rank_result();
		void sync_single_boss_rank_result(player & player_ref);
	};
}
