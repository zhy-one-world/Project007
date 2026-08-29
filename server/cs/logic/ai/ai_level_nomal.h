#ifndef __AI_LEVEL_NOMAL_H__
#define __AI_LEVEL_NOMAL_H__

#include "logic/type_def.hpp"
#include "Logic/ai_def.hpp"
#include "base/ecs_world.h"

namespace faith
{
	class npc;
	class unit;
	struct s_fake_player_info;
	struct NpcTemplate;

	enum e_npc_skill_type
	{
		e_npc_skill_type_nomal,
		e_npc_skill_type_cd,
		e_npc_skill_type_hp,
		e_npc_skill_type_hate,
		e_npc_skill_type_war_time
	};
	struct s_npc_skill
	{
		int32 skill_template_id;
		int32 skill_type;
		int32 skill_self;
		int64 skill_cd_cur_time;
		float skill_cd_max_time;
		float skill_distance;
		float skill_distance_max;
		float skill_weight;
		float spell_time;
		float channel_time;
		int32 move_length;
		e_npc_skill_type npc_skill_type;
		float value_high;
		float value_low;
		fvector	hurt_offset;
		int32 AutoReleasePriority;
		std::vector<float> cast_time_axle;
		float cast_duration;
		int32 cast_trace_target_location;
		int32 cast_trace_target_rotation;
		void clear();

		s_npc_skill()
			: skill_template_id(0)
			, skill_self(0)
			, skill_type(0)
			, skill_cd_cur_time(0)
			, skill_cd_max_time(0)
			, skill_distance(0)
			, skill_distance_max(0)
			, skill_weight(0)
			, spell_time(0)
			, channel_time(0)
			, move_length(0)
			, npc_skill_type(e_npc_skill_type_nomal)
			, value_high(0)
			, value_low(0)
			, AutoReleasePriority(0)
			, cast_duration(0.f)
			, cast_trace_target_location(0)
			, cast_trace_target_rotation(0)

		{
			hurt_offset.clear_data();
			cast_time_axle.clear();
		}
	};

	class ai_level_nomal
	{
	public:
		ai_level_nomal();
		virtual ~ai_level_nomal() {}
		virtual	void clear_data();
		virtual void create_ai(npc* unit_ptr, const s_map_pos& born_pos, const s_map_pos& patrol_pos, NpcTemplate* npc_template_ptr, Entity* map_ent);
		virtual void ai_tick(const int64& new_time);
		void init_skill_from_template();
		void init_fake_player_info(const s_fake_player_info& fake_player_ptr);
		void add_fake_player_normal_skill(int32 start_skill_template_id);
		void add_fake_player_cd_skill(int32 skill_template_id);
		static void init_skill_inst(s_npc_skill& skill_inst, int32 skill_template_id);
		s_npc_skill* get_current_skill();
		s_npc_skill* find_current_skill(const int64& new_time);
		s_npc_skill* find_skill_by_template_id(const int32& skill_template_id);
		s_unit_identifier check_enemy();
		void on_dead();
		bool aoi_out(const s_unit_identifier& unit_identifier);
		bool restore_cache_hate_val(const s_unit_identifier& unit_identifier, int64 dead_duration);
		bool const is_boss_war_state() const;
		void set_state(e_ai_state m_state) { m_ai_state = m_state; }
		void clear_hate_value();
		void set_born_pos(s_map_pos born_pos);
	public:
		void set_has_combo_skill(bool is_has) { m_has_combo_skill = is_has; };
		hate_info_vector const& get_hate_list() const { return m_hate_vec; };
		damage_info_vector const& get_damage_list() const { return m_damage_vec; };
		int32 get_first_attack() { return m_first_attack_index; }
		const int64& get_all_hate_list_player_gs() const{ return m_all_hate_list_gs_value; };
		void set_follow_guid(guid_64 role_guid);
		guid_64 get_follow_guid();
		fvector get_bron_location();
	private:
		void add_hate_value(const s_unit_identifier& unit_identifier, int64 hate_value);
		bool del_hate_value(const s_unit_identifier& unit_identifier);
		bool reduce_hate_value(const s_unit_identifier& unit_identifier);
		s_unit_identifier get_hate_max();
		s_unit_identifier get_target_by_hate();//通过仇恨列表获得仇恨值第一的玩家或npc
		e_ai_target_check_ret check_can_be_ai_target(const s_unit_identifier& unit_identifier, int32 hate_val);
		void on_hate_list_may_change();
		void add_damage_value(const s_unit_identifier& unit_identifier, const guid_64& legion_guid, const int64& damage_value);
		void clear_damage_value();
		
		bool check_need_follow_master(guid_64 role_guid);
		bool check_need_follow_attack_master(guid_64 role_guid);
		void get_map_pos_need_master(s_map_pos & map_pos, guid_64 role_guid);
	protected:
		int64						m_tick_next_time;//下一个tick消耗的时间
		int64						m_move_tick_time;//开始移动的时间点
		e_ai_state					m_ai_state;//当前的状态
		int64						m_ai_state_time;//执行的时间
		int64						m_face_next_time;//面向玩家的时间间隔
		npc*						m_npc_ptr;// npc的指针
		const NpcTemplate*			m_npc_template_ptr;
		fvector						m_born_location;//出生点
		fvector						m_patrol_point_location;//出生点
		frotator					m_born_rotation;//开始追击的面向
		fvector						m_patrol_location;	//巡逻点
		fvector						m_chase_location;//开始追击的点
		frotator					m_chase_rotation;//开始追击的面向
		int32						m_view_length;//最远可见距离
		int32						m_pursuit_length;//最大追击距离
		int32						m_patrol_length;//最大巡逻距离
		int32						m_follow_length;//最大跟随距离
		int32						m_follow_attack_length;//最大跟随攻击距离
		fvector						m_speed_vector;//移动速度
		Entity*						m_born_map_ent;//出生的地图id
		int32						m_skill_index;//技能列表的下标
		int64						m_skill_order;//技能顺序
		std::vector<s_npc_skill>	m_skill_vec;//技能列表
		int32						m_skill_vec_len;//技能列表长度
		hate_info_vector			m_hate_vec;//仇恨列表
		int32						m_hate_vec_len;//仇恨列表长度
		damage_info_vector			m_damage_vec;//伤害列表
		int32						m_damage_vec_len;//伤害列表长度
		int32						m_first_attack_index;//首个攻击者
		int32						m_total_hate;//总的仇恨值
		int64						m_war_time;//战斗经过的时间
		bool						m_back_hp_begin;//开始启动返回出生点回血
		int64						m_back_hp_time;//返回出生点回血间隔
		bool						m_has_combo_skill;
		int32						m_next_combo_skill;
		std::map<uint64, int32>		m_dead_hate_cache;

		int32						m_patrol_type;		//巡逻类型
		int32						m_patrol_direction;	//巡逻方向	1--正向	0--反向

		unit_index_map				m_attack_map;
		int32						m_attack_num;

		int64						m_all_hate_list_gs_value;//当前仇恨列表玩家战力总值

		guid_64						m_follow_role_guid;
	public:
		void attack_value(const int32& attack_index, const int64 hp);
		void add_damage_value_with_summon(const int32& attack_index, const int64 hp);
		void relieve_stop_time(const int32& skill_template_id);
		void npc_bron_clear();
		void set_next_time(const int64& new_time, int32 add_time);
	private:
		void born_anim_time(const int64& new_time);
		void born_anim_end();
		void check_and_operate_combo_skill(s_npc_skill* cur_use_skill);

		void state_stand(const int64& new_time);
		void state_move(const int64& new_time);
		void state_chase(const int64& new_time);
		void state_skill(const int64& new_time);
		void state_move_to_born(const int64& new_time);
		void back_chase(const int64& new_time);
		void state_follow(const int64& new_time);
		bool can_sand_skill_by_face(int32 skill_template_id);
		bool check_near_patrol_point_by_direction(int32 direction);
		void is_need_gather(const int64& new_time);
		fvector get_patrol_target_location_by_direction(int32 direction);
	private:
		void face_unit(const int64& new_time);
		void send_npc_face_unit();

		fvector m_destination_location;
		fvector m_target_location;
		void change_state_to_chase();
	};
}

#endif