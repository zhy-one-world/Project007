/*@@

	file name :	skill_effect.h
	
	author : wenhongming
	
	description : 
	
	change list :

@@*/

#ifndef _SKILL_EFFECT_H_
#define _SKILL_EFFECT_H_

//////////////////////////////////////////////////////////////////////////
//	file include
//////////////////////////////////////////////////////////////////////////
#include "logic/type_def.hpp"
#include "Logic/att_def.hpp"
#include "Logic/skill_def.hpp"
#include "Logic/aoi_def.h"
#include "template/ServerTemplateHead.h"
#include "Logic/buff_def.hpp"
#include "unit_def.hpp"

//////////////////////////////////////////////////////////////////////////
//	class declare
//////////////////////////////////////////////////////////////////////////

namespace faith
{
    class skill_effect
    {
    public:
		explicit skill_effect() { m_array_index = 0; clear_data(); }
	public:
		void					set_effect_index(int32 array_index) { m_array_index = array_index; }
		int32					get_effect_index() { return m_array_index; }
		void					heart_tick(const int64& new_time);
		void					clear_data();
		bool&					is_valid();
	public:
		void					effect_move(const int64& new_time);
		void					effect_hurt();
		void					skill_hurt(const use_skill_param& param);
	public:
		void					init_effect_data(const use_skill_param& skill_param, const unit_index_map& skill_add_buff_target_map, const unit_index_map& skill_add_buff_self_map, const int32& effect_index, bool is_move_effect);
		void					send_effect_fly();
	public:
		int32					get_info_id() { return m_skill_param.skill_template_id; }
		int32					get_skill_order() { return m_skill_param.skill_order; }
		void					set_life_time(int64& life_time) { m_life_time = life_time; }
		int64&					get_life_time() { return m_life_time; }
		void					end_effect();
    private:		
		void					trig_buf(const s_unit_identifier& target_identifier, fvector& pos, int32 skill_template_id, bool is_begin, int32 cast_idx, const use_skill_param& skill_param, bool by_client = false);
	private:
		bool					m_is_data_use;
		bool					m_is_move_effect;
		int32					m_array_index;
		use_skill_param			m_skill_param;
		unit_index_map			m_skill_add_buff_self_map;
		unit_index_map			m_skill_add_buff_target_map;
		SkillTemplate*			m_skill_template_ptr;
		SkillBulletTemplate*	m_skill_bullet_template_ptr;
		int32					m_bullet_template_id;
		int64					m_life_time;
		int64					m_move_tick_time;
		unit_guid_map			m_target_index_map;

		float					m_begin_radius;
		float					m_add_radius;
		fvector					m_begin_pos;
		fvector					m_speed_vector;
		fvector					m_speed_time;
    };
}

#endif
