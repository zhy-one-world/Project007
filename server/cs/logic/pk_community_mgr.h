/********************************************************************
created: 2016年8月9日13:56:28
file base: pk_mgr
file ext: h
author: zhangminghai

purpose: about pk and community system's store_mgr
*********************************************************************/
#ifndef _PK_MGR_HPP_
#define _PK_MGR_HPP_

#include "Logic/pk_community_def.hpp"
#include "Logic/att_def.hpp"
#include "Logic/skill_def.hpp"
#include "Logic/game_enum_def_s.h"

namespace faith
{
	typedef std::unordered_map<ui64, int32> c_fight_back_map;
	typedef c_fight_back_map::iterator c_fight_back_map_ite;

	class unit;
	class pk_community_mgr
	{
	public:
		pk_community_mgr() { m_unit_array_index = 0; clear_data(); }
		~pk_community_mgr(void){};
		void	heart_tick(const int64& new_time);
		void	heart_tick_1sec(const int64& new_time);
		void	set_unit_array_index(int32 unit_array_index);
		void	clear_data();

		static bool	is_map_can_change_pk_mode(int32 map_template_id);

		public:
		void	change_pk_info(int32 pk_type, int32 pk_value, bool need_check_map = false);
		void	sync_pk_info_aoi(e_pk_info pk_type);
		int32	get_entire_pk_mode();
		void	set_entire_pk_mode(int32 pk_cfg_entire, bool sync_aoi = true);
		int32	get_pk_mode(int32 pk_cfg_entire = -1);
		void	set_pk_mode(int32 cfg_value);
		bool	get_is_fight_back();
		void	set_is_fight_back(bool cfg_value);
		bool	get_team_protect();
		void	set_team_protect(bool cfg_value);
		bool	get_legion_protect();
		void	set_legion_protect(bool cfg_value);

		void	add_pk_value(int32 pk_value);
		int32	cut_pk_value(int32 pk_value); //返回成功减少多少点pk值
		int32	get_pk_value();
		void	pk_value_skip_level_add_buff(int32 old_value, int32 new_value);
		void	pk_value_add_buff(int32 pk_value);
		void	mod_buff_dur_time(int32 pk_value);

		void	check_first_hurt_add_pk_vaule();

		bool	check_if_can_attack_target_by_region(unit* target_unit);
		bool	is_skill_target(int32 target_unit_index, int32 skill_target_type = faith::eskilltarget_enemy);

		bool	is_in_not_peace_and_kindevil();
		bool	is_in_peace_pk_mode();
		bool	is_in_legal_pk_area();

		bool	check_if_move();
		void	set_big_map_region_type();
		void	init_map_pk_type(e_pk_mode_type mode_type = e_pk_mode_type_max);
		e_pk_mode_type	get_cur_map_pk_type();
		
		void	on_player_be_attacked(int32 unit_array_index);
		void	on_player_be_attacked_by_robot_player(int32 unit_array_index);
		void	add_fight_back(const guid_64& attacker_guid);
		void	check_fight_back();
		void	on_player_transfer_init();
		int32	get_region_type();
		int32	get_safe_map_template_id() { return m_pre_safe_map_template_id; };
		void	set_map_have_safe_region(int32 is_have) { m_map_have_not_safe_region = is_have; };
	private:
		int32				m_unit_array_index;
		int64				m_pk_value_time;
		bool				m_is_pk_value_time;
		//int64				m_fight_back_end_stamp;
		c_fight_back_map	m_fight_back_list;
		int64				m_tick_interval_1sec;
		//安全区判定
		int64				m_cut_interval_get_region;	//区域类型tick
		int32				m_region_type = e_region_type_zhongli;
		fvector				m_pre_location;
		int32				m_map_have_not_safe_region;
		int32				m_pre_safe_map_template_id;
		int32				m_max_safe_map_priority;
	};
}
#endif