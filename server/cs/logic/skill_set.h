/********************************************************************
created:	2015/03/21
file base:  skill_effect	
file ext:	h
author:	    wenhongming	
purpose:
*********************************************************************/
#pragma once

#include "logic/type_def.hpp"
#include "skill.h"

namespace faith
{
    class unit;
    class skill_effect;
	class skill_proto_skill_operation;
	typedef std::map<int32, skill*> skill_map;
	typedef skill_map::iterator skill_map_it;
	typedef std::list<skill*> skill_ptr_map;
	typedef skill_ptr_map::iterator skill_ptr_map_it;
	typedef std::map<int64, skill_effect*> skill_effect_map;
	typedef skill_effect_map::iterator skill_effect_map_it;
    // 封装一个人身上的技能逻辑
    class skill_set
    {
    public:
		skill_set() { clear_data(); }
		~skill_set();

	private://内存已经创建好了 禁止拷贝
		skill_set(const skill_set& skill_set_ref);
		skill_set& operator=(const skill_set&);
	public:
		void clear_data();
		void set_owner_index(const int32& unit_index) { m_unit_idf.runtime_id = unit_index; }
		void set_owner_guid(const guid_64& unit_guid) { m_unit_idf.unit_guid = unit_guid; }
		void set_owner(s_unit_identifier const& v) {m_unit_idf = v;}
	public:
		skill_effect*				get_skill_effect(int32 skill_order, int32 effect_index);
		void						add_skill_effect(skill_effect* skill_effect_ptr);
		void						skill_hurt(const use_skill_param& param);
	public:
		void						heart_tick(const int64& new_time, const int32& tick_time);
		void						load_skill_by_db(const skill_proto_skill_save_load& load_msg);
		bool						save_skill_to_db(int32 save_type_ex);
		void						send_skill_all();
		void						send_skill_one(const skill* skill_inst);
		void						exp_level_up(int32 cur_level);
	public:
		void						skill_change_msg(int32 skill_template_id, int32 pos_index);
		void						change_skill_sub_cd_all(int32 sub_cd, bool apply);
		void						change_skill_percent_cd_all(float percent_cd, bool apply);
		float						get_fix_skill_value(int32 series_type, e_fix_skill fix_type);
		void						change_skill_fix_value(int32 series_type, e_fix_skill fix_type, float fix_val, bool apply);
		void						change_skill(int32 series_type, int32 skill_template_id, bool apply);
		void						skill_add_buff(int32 series_type, int32 buff_template_id, int32 is_self, bool apply);
		fvector						get_return_location() { return m_return_location; }
		void						set_return_location(fvector return_location) { m_return_location = return_location; }
	public:
		skill* 						add_skill_inst_by_id(int32 skill_template_id, bool activated);
		skill* 						add_skill_inst_by_info(const s_skill_info& skill_data, bool is_send);
	public:
		skill*						get_skill_inst_by_series(int32 series);
		void						remove_skill_inst_by_series(int32 series);
		skill*						get_skill_inst_by_id(const int32& skill_template_id);
		void						remove_skill_inst_by_id(int32 skill_template_id);
		skill*						get_skill_inst_by_type(e_skill_type skill_type);
		skill*						get_skill_inst_by_pos(int32 skill_pos);
	public:
		void						resp_spell_cast(use_skill_param& param);
		void						resp_spell_hurt(s_unit_identifier const& target_unit_idf, use_skill_param& param);

		void						resp_cancel_skill(const skill_proto_skill_operation& req);
		void						resp_learn_skill(const skill_proto_skill_operation& req);
		void						unlock_skill(const int32& skill_template_id, const int32& up_level);
		void						resp_upgrade_skill(const skill_proto_skill_operation& req);
		
		void						upgrade_skill_achievement();//更新成就统计
		void						all_skill_level_count_to_achievement();
		
		void						activate_skill(int32 skill_template_id, bool is_activate = true);
		void						relieve_stop_time(const int32& skill_template_id);
		
		bool						set_skill_cd_by_type(e_skill_type skill_type, int32 skill_cd, bool is_send);

		bool						is_can_break_skill(int32 skill_template_id);
		void						set_break_skill(std::vector<int32> skill_array);
		//gm特殊操作
		bool						gm_set_all_career_skill_level(int32 new_level);
		bool						gm_reset_all_career_skill_cd();
		bool						activate_all_skill();

		void						set_fake_skill_info(s_skill_info * play_skill_info, int32 max_num);
		int32						get_skill_num() { return m_skill_map.size(); };
		int32						get_skill_equip_num();
    private:
		s_unit_identifier			m_unit_idf;//谁拥有这个技能池
		skill_map					m_skill_map;//技能的实例列表
		skill_ptr_map				m_skill_tick_map;//哪些技能需要走tick，减少tick数量
		fvector						m_return_location;
		float						m_sub_cd_all;
		float						m_sub_percent_all;
		int64						m_public_time;
		skill_effect_map			m_effect_map;//子弹的列表
		std::vector<int32>			m_break_skill_array;
    };

}