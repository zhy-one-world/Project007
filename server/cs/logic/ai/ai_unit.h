#ifndef AI_UNIT_HEAD
#define AI_UNIT_HEAD
#include "Logic/ai_def.hpp"
#include "ai_level_nomal.h"
namespace faith
{
	struct s_fake_player_info;

	class ai_unit
	{
	public:
		ai_unit();
		~ai_unit(){};
		void clear_data();
	public:
		//npc初始化
		void init_ai(npc* unit_ptr, const s_map_pos& born_pos, const s_map_pos& patrol_pos, NpcTemplate* npc_template_ptr, Entity* map_ent);
		void set_has_combo_skill(bool is_has) { m_ai_level_nomal.set_has_combo_skill(is_has); };
		void init_fake_player_info(const s_fake_player_info& fake_player_ptr);
		//npc时间tick
		void ai_tick(const int64& new_time);
		void relieve_stop_time(const int32& skill_template_id);

		hate_info_vector const& get_hate_list() const { return m_ai_level_nomal.get_hate_list(); }
		damage_info_vector const& get_damage_list() const { return m_ai_level_nomal.get_damage_list(); }
		const int64& get_all_hate_list_player_gs() const { return m_ai_level_nomal.get_all_hate_list_player_gs(); }
		int32 get_first_attack() { return m_ai_level_nomal.get_first_attack(); }
		bool check_play_in_damage_list(guid_64 player_guid);
		void set_state(e_ai_state m_state) { m_ai_level_nomal.set_state(m_state); }
		void clear_bron_data();
	public:
		//有人攻击我
		void attack_value(const int32& attack_index, const int64 hp);
		void add_damage_value_with_summon(const int32& attack_index, const int64 hp);
		void on_dead() { m_ai_level_nomal.on_dead(); }
		bool aoi_out(const s_unit_identifier& unit_identifier) { return m_ai_level_nomal.aoi_out(unit_identifier); }
		bool restore_cache_hate_val(const s_unit_identifier& unit_identifier, int64 dead_ms) { return m_ai_level_nomal.restore_cache_hate_val(unit_identifier, dead_ms); }
		const bool is_war_state() const{ return m_ai_level_nomal.is_boss_war_state(); }
		void add_level_nomal_next_time(int32 add_time);
		void set_level_nomal_born_pos(s_map_pos born_pos);
		void clear_hate_value();
		guid_64 get_follow_guid();
		void set_follow_guid(guid_64 role_guid);
		const ai_level_nomal&	get_ai_level();
		fvector get_bron_location();
	private:
		
	private:
		ai_level_nomal		m_ai_level_nomal;
	};
}
#endif

