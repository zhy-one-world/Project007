#pragma once
#include "logic/type_def.hpp"
#include <Logic/att_def.hpp>
#include "arena_def.hpp"

namespace hld
{
	struct s_adversity_aid
	{
		int32 m_buff_template_id;
		e_unit_attack_state m_attack_state;
		s_adversity_aid()
		{
			m_buff_template_id = 0;
			m_attack_state = e_unit_attack_state_dead;
		}
		s_adversity_aid(int32 buff_id, e_unit_attack_state attack_state)
		{
			m_buff_template_id = buff_id;
			m_attack_state = attack_state;
		}
	};
	struct s_damage_absorb
	{
		int32 m_buff_template_id;
		int32 m_skill_template_id;
		int64 m_absorb_cur_num;
		int64 m_absorb_max_num;
		s_damage_absorb()
		{
			m_buff_template_id = 0;
			m_skill_template_id = 0;
			m_absorb_cur_num = 0;
			m_absorb_max_num = 0;
		}
	};
	struct s_exp_add
	{
		int32 m_buff_level;
		int32 m_add_num;
		s_exp_add()
		{
			m_buff_level = 0;
			m_add_num = 0;
		}
	};
	class pawn_att
	{
	public:
		pawn_att();
		~pawn_att();
		/************************************************************************/
		/*  外部调用属性更新接口                                                */
		/************************************************************************/
		void									set_unit_ptr(int32 unit_array_index){ m_unit_array_index = unit_array_index; }
		void									clear_data();
		void									heart_tick(const int64& new_time);
		void									heart_tick_3_sec(const int64& new_time);
		void									stop_tick() { m_unit_tick_begin = false; }
		/************************************************************************/
		/* 初始化、加载、存盘相关                                               */
		/************************************************************************/
		//初始化
		void									load_base_att_by_db(const s_base_att_info& base_att);
		bool                                    load_base_att_by_db_lua(const char *data_ptr, int32 data_len);
		void									load_unit_att_by_db(const s_arena_char_fight_att& unit_att);
		void									save_base_att_to_db(int32 save_type_ex);
		void									send_base_att_all();
		void									send_base_att_one(int32 att_index);
		void									set_unit_base_att(int32 att_index, int64 iValue);
		int64									get_unit_base_att(int32 att_index);
		s_base_att_info&						get_unit_base_att_inst() { return m_unit_base_att; }
		void									update_unit_att_info(int32 template_id, bool is_add = true);
		void									update_unit_pure_hurt_att_info(int32 template_id, bool is_add = true);
		void									init_unit_att_over(bool use_owner);
		void									calcu_unit_att_by_level(bool use_ower);
		s_arena_char_fight_att					get_unit_fight_att_inst();
		/************************************************************************/
		/* 升级相关                                                             */
		/************************************************************************/
		void									exp_levelup();
		/************************************************************************/
		/* 属性相关                                                             */
		/************************************************************************/
		//装备增加给人物一对属性,一个数值一个百分比 
		//att_level 属性等级 e_pawn_att_index
		//id 属性id e_unit_att_index
		//value 属性值
		//percent 属性百分比
		//apply	属性是增加还是减少
		//coef	属性层数
		//is_gs 属性是否计算战力值
		void									apply_att_change_by_array(const std::vector<float>& att_array, bool is_add, f32 add_count = 1.0);
		void									apply_att_change_by_array(const std::vector<std::string>& att_array, bool is_add, f32 add_count = 1.0);
		void									apply_att_change_effect(int32 att_level, int32 id, f32 value, f32 percent, int32 is_show, bool apply);
		double									get_attack_att_gs(uint32 att_index) const;//获取计算战斗力的属性的最终属性值

		double									get_attack_att_by_att(uint32 att_index) const;//获取需要同步属性的最终属性值
	private:
		double									get_attack_att_by_gs_and_hide(uint32 att_index) const;//获取所有的属性最终属性值
	public:
		double									get_attack_att_value(uint32 att_index);
		const double*							get_attack_att_all() { return m_unit_att_end; }
		f32										get_unit_att(uint32 att_level, int32 att_id) const;//获取某层次,某个具体值
		int32									get_use_owner_att() { return m_use_owner_att; }
		void									set_use_owner_att(int32 use_owner) { m_use_owner_att = use_owner; }
		void									change_base_body_att(const std::vector<int32>& base_body_att, bool is_refresh = false);
		void									change_base_att(int32 att_type, int32 value, bool is_add, bool is_refresh = false);
		void									change_gs_for_att(e_pawn_att_index pawn_att_index, const std::vector<int32>& change_arr, int32 value, int32 is_show);
		void									change_equip_gs(bool is_add, bool is_refresh);
		/************************************************************************/
		/* 单一属性相关                                                         */
		/************************************************************************/
		void									set_game_att(int32 att_index, float value, bool apply);
		float									get_game_att(uint32 att_index) const;
		/************************************************************************/
		/* 状态相关                                                             */
		/************************************************************************/
		void									set_state_att(uint32 att_index, bool flag);
		bool									get_state_att(uint32 att_index) const;
		void									set_state_percent(uint32 att_index, int32 flag_percent, bool apply);
		int32									get_state_percent(uint32 att_index) const;
		bool									is_dead();

		void									set_state_trigger(uint32 att_index, bool flag);
		bool									get_state_trigger(uint32 att_index) const;

		bool									is_can_move();

		int32									get_be_hurt_type() { return m_be_hurt_type; }
		void									set_be_hurt_type(int32 be_hurt_type) { m_be_hurt_type = be_hurt_type; }
		
		bool									can_cut_magic(double magic_value);
		void									change_magic(double magic_value);
		bool									can_cut_body_power(double body_power);
		void									change_body_power(double body_power);

		bool									can_cur_energy(double energy_value);
		void									change_energy(double energy_value);
		/************************************************************************/
		/*	工具函数		                                                    */
		/************************************************************************/
		inline void								set_att_update_mask(int32 att_index, e_pawn_att_type att_type);
		inline void								clear_att_update_mask();
		void									syn_att_change();
		void									syn_gs_change();
		void									syn_all_att();
		void									send_unit_dead(bool is_dead);
	private:
		void									strength_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show);//力量属性变化增加二级属性
		void									agility_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show);//敏捷属性变化增加二级属性
		void									intellect_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show);//力量属性增加二级属性
		void									stamina_to_second_att(int32 class_type, e_pawn_att_index pawn_att_index, int32 value, int32 is_show);//耐力属性变化增加二级属性
		bool									att_modifier(e_pawn_att_index modifier_type, int32 att_index, f32 value, int32 is_show, bool apply);//具体调整某个属性
	public:
		inline bool&							is_att_mask();
	public:
		void									adversity_aid_buff_id(int32 buff_id, int32 attack_state,bool apply);
		void									use_adversity_aid_buff_id(int32 attack_state);
	public:
		void									damage_absorb_change(int32 buff_template_id, int32 skill_template_id, int64 absorb_num, bool apply);
		int64									damage_absorb_cut(int64 damage_num);
	public:
		bool									exp_add_change(int32 buff_race, int32 buff_level, int32 add_num, bool apply, int32& cut_num);
	public:
		bool									move_check(fvector& move_location, int64& time_now);
		void									set_close_speed(int32 close_type);
		int32									get_move_speed() { return m_max_move_length; }
		void									set_move_speed(int32 move_length) { m_max_move_length = move_length; }
		void									set_last_move_time(int64 last_time) { m_last_move_time = last_time; }

		int32									get_change_gs_value(const std::vector<float>& add_att, const std::vector<float>& del_att);

		int32									get_change_bush_id() { return m_change_bush_id; }
		void									set_change_bush_id(int32 bush_id) { m_change_bush_id = bush_id; }
	public:
		void									show_att(int32 att_type);
	private:
		s_base_att_info							m_unit_base_att;//人物需要存档的属性
		int32									m_unit_array_index;//对应unit
		bool									m_unit_tick_begin;//在线状态
		bool									m_att_change;//是否是属性发生了改变

		tag_unit_att							m_unit_att_all[send_max][epai_max];

		double									m_unit_att_end[e_unit_attack_att_max];//玩家属性的最终结果
		f32										m_game_att[e_unit_game_att_max];//单一常用属性
		int32									m_state_att[e_unit_attack_state_max];//玩家状态
		int32									m_state_att_percent[e_unit_attack_state_max];//玩家状态生效百分比
		bool									m_state_trigger[e_unit_attack_state_max];//玩家触发状态
		bool									m_update_att_mask[e_unit_attack_att_max + e_unit_game_att_max + e_unit_attack_state_max + e_unit_attack_state_max];//全属性是否赋值判断
		bool									m_is_update_att;
		int32									m_be_hurt_type;
		int32									m_use_owner_att;
		std::list<s_adversity_aid>				m_adversity_aid_buff_list;//逆境援助buffid
		std::list<s_damage_absorb>				m_damage_absorb_list;//伤害吸收
		std::map<int32, s_exp_add>				m_exp_add_map;//经验加成计算
	private:
		int64									m_cur_hp_last_sync;
		int64									m_cur_hp_last_sync_ws;
		int64									m_max_hp_last_sync_ws;
		int64									m_back_hp_magic_time;
		int64									m_back_hp_buff_time;
		int64									m_next_3_sec_tick_time;
		int64									m_back_hp_energy_time;
	private:
		int64									m_last_move_time;
		int32									m_max_move_length;
		bool									m_close_speed;

		int32									m_is_dead;//单独写出的原因是优化性能
		int32									m_change_bush_id;
	};
	/************************************************************************/
	/*				      inline function implement                         */
	/************************************************************************/

	inline void pawn_att::set_att_update_mask(int32 att_index, e_pawn_att_type att_type)
	{
		switch (att_type)
		{
		case hld::e_pawn_att_type_attack:
		{
			if (att_index < e_unit_attack_att_none || att_index >= e_unit_attack_att_max)
			{
				return;
			}
			m_update_att_mask[att_index] = true;
		}
			break;
		case hld::e_pawn_att_type_game:
		{
			if (att_index < e_unit_game_att_movement || att_index >= e_unit_game_att_max)
			{
				return;
			}
			m_update_att_mask[att_index + e_unit_attack_att_max] = true;
		}
			break;
		case hld::e_pawn_att_type_state:
		{
			if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
			{
				return;
			}
			m_update_att_mask[att_index + e_unit_attack_att_max + e_unit_game_att_max] = true;
		}
		break;
		case hld::e_pawn_att_type_sign:
		{
			if (att_index < e_unit_attack_state_dead || att_index >= e_unit_attack_state_max)
			{
				return;
			}
			m_update_att_mask[att_index + e_unit_attack_att_max + e_unit_game_att_max + e_unit_attack_state_max] = true;
		}
		break;
		default:
			break;
		}
		m_is_update_att = true;
	}
	inline void pawn_att::clear_att_update_mask()
	{
		memset(m_update_att_mask, 0, sizeof(m_update_att_mask));
		m_is_update_att = false;
	}
	inline bool& pawn_att::is_att_mask()
	{
		return m_is_update_att;
	}

}
