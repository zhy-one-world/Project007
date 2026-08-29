#pragma once

#include "logic/type_def.hpp"
#include "Logic/skill_def.hpp"
#include "unit_def.hpp"

namespace faith
{
	struct SkillTemplate;

	class skill_work
	{
	public:
		skill_work();
		~skill_work();
	public:
		void					heart_tick(const int64& new_time, const int32& tick_time);
		void					clear_data();
		float                   get_end_time();
		void					init_skill_data();
		bool					is_remove_tick();
	public:
		void					add_effect(const use_skill_param& param, const unit_index_map& skill_add_buff_target_map, const unit_index_map& skill_add_buff_self_map, bool send_effect, bool is_move_effect);
	public:
		void					skill_move(const int64& new_time);
	public:
		float                   get_skill_cast_start_time();
		void					skill_cast();
		void					skill_operate();
		void					send_skill_do(e_skill_operation_type operation_type, bool send_self);
		void					send_skill_spell(bool send_self);
		void					send_skill_cast(bool send_self);
		void					send_effect();
	public:
		void					relieve_stop_time();
		void					do_skill(const use_skill_param& param, const unit_index_map& skill_add_buff_target_map, const unit_index_map& skill_add_buff_self_map);
		s_unit_identifier const& get_owner() const { return m_unit_idf; }
		void					set_owner(s_unit_identifier const& v) { m_unit_idf = v; }
	private:
		s_unit_identifier		m_unit_idf;
		int32					m_skill_template_id;
		e_skill_state			m_skill_state;
		int64					m_refresh_time;
		use_skill_param			m_skill_param;
		unit_index_map			m_skill_add_buff_self_map;
		unit_index_map			m_skill_add_buff_target_map;
		int32					m_effect_index;
		int64					m_effect_begin_time;
		int32					m_cast_time_axle_len;
		SkillTemplate*			m_skill_template_ptr;
	private:
		bool					m_is_skill_move;
		int64					m_end_time;
		int64					m_move_tick_time;
		float					m_move_length;
		int32					m_move_index;
		fvector					m_move_location;
		float					m_move_yaw;
		bool					m_is_not_sync_rotation;
		bool					m_is_client_driver;
	};
}
