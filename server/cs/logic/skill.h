/*@@

	file name :	skill.h
	
	author : wenhongming
	
	description : 
	
	change list :

@@*/

#ifndef _SKILL_H_
#define _SKILL_H_

//////////////////////////////////////////////////////////////////////////
//
//	file include
//
//////////////////////////////////////////////////////////////////////////
#include "logic/type_def.hpp"
#include "Logic/skill_def.hpp"
#include "unit_def.hpp"
#include "skill_work.h"

//////////////////////////////////////////////////////////////////////////
//
//	class declare
//
//////////////////////////////////////////////////////////////////////////

namespace faith
{
	class skill_proto_skill_save_load;
	struct SkillTemplate;

	class skill
	{
	public:
		skill();
		~skill();
	public:
		void					heart_tick(const int64& new_time, const int32& tick_time);
		void					clear_data();
	public:
		int64					get_skill_cd() const;
		void					set_skill_cd(const int32& skill_cd);
		int32					get_next_cd() { return m_next_skill_cd; };
		float					get_fix_skill_value(e_fix_skill fix_type);
		void					change_skill_fix_value(e_fix_skill fix_type, float fix_val, bool apply);
		void					send_skill_fixed_param();
		void					skill_add_buff(int32 buff_template_id, int32 is_self, bool apply);
	public:
		int32					get_cur_skill_id() { return m_cur_skill_id; }
		void					set_skill_info(const s_skill_info& skill_info);
		const s_skill_info&		get_data_all(){ return m_skill_info; }
		int32					get_data_info(e_skill_info eIndex) const { return m_skill_info.data_ary[eIndex]; }
		void					set_data_info(e_skill_info eIndex, int32 iValue){ m_skill_info.data_ary[eIndex] = iValue; }
		int32&					get_skill_template_id() { return m_skill_info.data_ary[e_skill_info_template_id]; }
		SkillTemplate*			get_skill_template_ptr() { return m_skill_template_ptr; }
		int32					get_skill_series();
		void					add_maturity_num(int32 maturity_num);
	public:		
		void					set_passive_buff(bool be_add);
		void					deactivate();
		void					relieve_stop_time();
	public:
		bool					is_remove_tick();
		void					do_skill(const use_skill_param& param);
		void					skill_hurt(const use_skill_param& param);
	public:
		s_unit_identifier const& get_owner() const { return m_unit_idf; }
		void					set_owner(s_unit_identifier const& v) { m_unit_idf = v; m_skill_work.set_owner(v); }
		bool					save_skill_info(skill_proto_skill_save_load& request_save, int64& now_time);
	private:
		skill_work				m_skill_work;
		s_unit_identifier		m_unit_idf;
		s_skill_info			m_skill_info;
		int64					m_skill_cd;
		int64					m_next_skill_cd;
		s_fix_skill				m_fix_info;
		fvector					m_back_location;
		unit_index_map			m_skill_add_buff_target_map;
		unit_index_map			m_skill_add_buff_self_map;
	private:
		unit_index_list			m_change_skill_list;
		int32					m_cur_skill_id;
		int32					m_begin_skill_id;
		SkillTemplate*			m_skill_template_ptr;
	public:
		void					change_skill(int32 skill_template_id, bool is_add);
		void					send_change_skill(int32 skill_template_id);
	public:
		fvector                 get_enemy_pos(int32 in_radius);

	};
}

#endif
