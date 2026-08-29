/********************************************************************
	created:	2015年8月17日10:32:06
	file base:	init_unit
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#pragma once

#include "logic/type_def.hpp"
#include "Logic/skill_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/spirit_def.hpp"
#include "Logic/mail_def.hpp"
#include "Logic/pk_community_def.hpp"
#include "Logic/game_enum_def_s.h"
#include "cs_date.hpp"
#include "Logic/activity_def.hpp"
#include "Logic/pk_king_def.hpp"
#include "Logic/time_limit_activity_def.hpp"

using namespace std;

namespace faith
{
	struct ItemUpgradeTemplate;
	struct EquipTemplate;
	struct SpiritTemplate;

	class init_unit
	{
		init_unit();
		~init_unit();
	public:
		//位运算处理
		static int get_left_shift_operation_result(int left_operator, int right_operator);
		static int get_bitwise_and_operation_result(int left_operator, int right_operator);
		static int get_bitwise_or_operation_result(int left_operator, int right_operator);

		static int32 get_flag_data(int32 _flag_datas, int32 _idx);
		static void  mark_flag_data(int32& _flag_datas, int32 _idx);
		static void  erase_flag_data(int32& _flag_datas, int32 _idx);
		static int32 get_flag_data(int64 _flag_datas, int32 _idx);
		static void  mark_flag_data(int64& _flag_datas, int32 _idx);
		static void  erase_flag_data(int64& _flag_datas, int32 _idx);
		static int32 get_flag_data_2bit(int32 _flag_datas, int32 _idx);
		static void  set_flag_data_2bit(int32& _flag_datas, int32 _idx, uint32 flag_val);
		static void  erase_flag_data_2bit(int32& _flag_datas, int32 _idx);
		static bool  is_had_this_flag(int32 _flag_datas, int32 _idx);

		//一些字符串的处理
		static string	implode(const vector<string>& v_src, string sep = "&#");
		static void		explode(const string& str_src, vector<string>& v_ret, string sep = "&#");
		static char*	get_host_ip(void);
		static void		format_str(char* szBuf, int32 nBufSize, const char* szText, const vector<string>& vtVar);
		static bool get_map_public(int32 map_type);
		static bool is_cross_server_npc(int32 npc_type);//是否可以跨服与这些NPC交互
		static bool is_all_server_npc(int32 npc_type);//跨服与否都可与这些NPC交互
		static bool is_send_clear_title_with_clear_big_player(int32 player_type);
		static bool is_cross_server_big_player(int32 player_type, bool is_begin_cross_activity = false, bool is_begin_cross_city_war = false);
		static bool is_cross_server_title(int32 title_type, bool is_begin_cross_activity = false);
		static bool is_levelup_add_hp(int32 map_type);
		static bool is_rank_type_cross_server(int32 ranking_type);//是否为跨服排行榜
		static bool get_map_need_add_hate(int32 map_type);
		static bool get_map_level_suppress(int32 map_type, int32 map_order);
		static bool get_map_gain_treasure_multiplayer(int32 map_type);
		static bool is_team_exp_map(int32 map_type);
		static bool get_map_open_new(int32 map_type);
		static bool get_map_create_full(int32 map_type);
		static bool get_map_hold(int32 map_type);
		static bool is_map_gain_treasure(int32 map_type);
		static bool is_map_boss_island(int32 map_type);
		static bool is_map_belief_cloister(int32 map_type);
		static bool is_can_change_pk_mode(int32 map_type);
		static bool is_map_cross_server(int32 map_temp_id, bool is_cross_activity = false);//是否为跨服地图
		static bool is_map_other_server(int32 map_temp_id);//是否为普通的跨服地图
		static bool is_tansfer_server(int32 map_temp_id, bool is_gate_server, bool is_cross_activity = false);//是否能传送
		static bool is_map_all_server(int32 map_temp_id);//是否为gate和普通服都存在的地图
		static bool is_boss(int32 boss_type);
		static bool is_world_boss(int32 boss_type);
		static bool is_goto_same_map(int32 map_id_to, int32 line_id_to, int32 map_id_cur, int32 line_id_cur);
		static bool is_can_same_map_transfer(int32 map_type);//是否能同地图传送
		static e_map_category get_map_category(int32 map_type);
		static bool is_team_type_raid_map(int32 team_type);
		static bool is_team_type_raid_for_team(int32 team_type); //组队才能进入的副本
		static bool is_team_type_one_stop(int32 team_type);
		static bool is_fake_player(int32 unit_type, int32 sub_type);
		static e_role_equip_slot get_slot_by_item_type(int32 item_type, int32 sub_type);
		static bool is_equip_type(int32 item_type);
		static bool is_spirit_type(int32 item_type);
		static int32 get_max_player_level();
		static int32 get_max_grade_level();
		static int32 get_ladder_season();
		static int32 get_element_season();
		static bool is_all_time_map(int32 map_temp_id);

		static bool init_item_data(faith::s_item_info& msg_data, int32 template_id, int32 slot, int32 stack_count, e_bag_type container_type = e_bag_type_bag, int32 is_activate = 0, int32 locked = 1, int32 item_over_time = 0, int32 item_const_att = 0);
		static bool init_spirit_data(faith::s_spirit_info& msg_data, int32 template_id, int32 slot, e_spirit_bag_type container_type, int32 is_activate);
		static bool init_skill_data(faith::s_skill_info& msg_data, int32 template_id, int32 activated = 0);//判断是否能释放该技能
		static int64 get_distance(const int64& x, const int64& y, const int64& z, const int64& x1, const int64& y1, const int64& z1);
		static int64 get_distance(const int64& x, const int64& y, const int64& x1, const int64& y1);
		static int64 get_distance(const fvector& pos1, const fvector& pos2);
		static bool is_in_distance(const fvector& pos1, const fvector& pos2, const int64& need_length);
		static bool is_in_distance_with_high(const fvector& pos1, const fvector& pos2, const int64& need_length, const float& ratio, const bool& only_compare_high);
		static bool	is_in_safe(const std::vector<fvector>& PointAry, fvector unit_pos);

		static int32 calcu_fighting_power_by_att_array(const std::vector<float>& att_array, const double* unit_att, const f32& att_ratio = 1.0 ,bool AddPercentAtt = true);

		static fvector in_skill_move(fvector boss_location, fvector target_location, int32 move_length);
		static bool in_skill_target(fvector boss_location, fvector target_location, int32 skill_wight, int32 skill_length, fvector unit_location);
		static bool in_skill_target_x(fvector boss_location, fvector target_location, int32 skill_wight, int32 skill_length, fvector unit_location);
		static bool in_skill_target_y(fvector boss_location, fvector target_location, int32 skill_wight, int32 skill_length, fvector unit_location);
		static bool init_mail_data_array(faith::s_mail_info& msg_data, const std::vector<guid_64>& item_guid, int32 money_typ1, int32 money_num1, int32 money_typ2, int32 money_num2, guid_64 sender_guid, int64 cur_time_sec = time_helper::get_cur_time_new().second);
		static void range_pos(s_map_pos& pos);
			
		static int32 get_player_grade_num(int32 level);				// 获得角色当前的转生数
		static int32 get_player_level_of_cur_grade(int32 level);	// 获得角色在当前转数下的等级

		static e_bag_type get_item_bag_type(int32 item_template_id);

		static void change_account_to_low(xchar* account);
		static bool account_is_right(const std::string& account);

		static int32 change_string_to_i32(std::string str_i32);
		static std::string change_i32_to_string(int32 value_i32);
		static int64 change_string_to_i64(std::string str_i64);
		static std::string change_i64_to_string(int64 value_i64);
		static float change_string_to_float(std::string str_i32);
		static std::string change_float_to_string(f32 value_f32);
		// 功能解锁判断(返回true表示解锁,false表示为解锁)
		static bool judge_func_unlock(int32 player_cond_id, int32 func_type);

		static bool is_map_can_change_pk_mode(int32 map_type);

		static int64 get_end_time(const float& add_time, const int64& now_time = -1);
		static int64 get_end_time(const int32& add_time);
		static bool can_career_use_item(int32 role_class, const std::vector<int32>& class_arr);


		static int32 get_map_id_by_diff_level(int32 map_id, int32 map_type, int32 map_group_type);

		static void get_money_array_by_mission_id(int32 mission_id, int32 player_upgrade_id, int32 star_num, int32 get_times, std::vector<s_item_template_info>& money_array, int32 world_base_exp = 300);
		static void get_money_array_by_mission_library_id(int32 mission_library_id, int32 player_upgrade_id, std::vector<s_item_template_info>& money_array, int32 world_base_exp = 300);
		static void merge_money_to_two_tuples(std::vector<s_item_template_info>& source_array, int32 new_id, int32 new_num);
		static void merge_item(std::vector<s_item_template_info>& source_array, int32 new_id, int32 new_num, bool new_lock_stated = true);

		static void get_item_recovery_money_info(const faith::s_item_info& item_inst_data, std::vector<s_item_template_info>& money_reward_array, std::vector<s_item_template_info>& money_cost_array, bool is_clear_array, bool is_sprite_use_diamond = false);
		static int32 get_mission_money_empty_slot(int64* money_value_array);

		static int32 get_mission_money_same_slot(int32* money_type_array, int32 target_type);

		static int32 get_upgrade_buff_id(ItemUpgradeTemplate* upgrade_templalte_ptr, int32 unit_class, int32 branch_type, int32 grade_num, bool is_weapon);
		static int32 get_upgrade_phase_level(int32 upgrade_num);
		static int32 get_addon_phase_level(int32 addon_num);

		static void random_sprite_flair_property(s_item_info& temp_info);
		static bool is_has_this_item_illusion(int32 has_illusion_byte, int32 target_index);

		static void get_team_share_exp_ratio(int32 team_mem_num, int32 team_same_legion_mem_num, float& ratio_team, float& ratio_legion, float& ratio_team_raid);

		static s_damage_count_all skill_damage_end//技能buff带来的伤害
			(
				const s_hit_critical_calcu_param& attack_att_param,
				const s_hit_critical_calcu_param& target_att_param,
				const double* attack_att_array,
				const double* target_att_array,
				use_buff_param& param,
				const std::vector<f32>& coefficient_a,
				const std::vector<f32>& coefficient_b,
				f32 skill_ratio,
				int32 skill_plus,
				f32 damage_sub,
				f32 person_attack_change,
				f32 sky_attack_ground
				);
		static void hit_and_critical//计算命中和暴击
			(
				const s_hit_critical_calcu_param& attack_att_param,
				const s_hit_critical_calcu_param& target_att_param,
				const std::vector<float>& HitRateParams,
				const std::vector<float>& CriticalRateParams,
				bool& be_hit,
				bool& be_critical
				);
		static int32 get_npc_level_by_upgrade(int32 up_id, int32 cur_npc_temp_id);

		static bool is_skill_target_with_param(const s_is_skill_target_param& param);
		static bool is_skill_friendly(e_skill_target_type skill_target_type);
		static bool is_enemy_with_param(const s_is_skill_target_param& param);
		static int32 get_community_group_relation_by_data(int32 type1, int32 type2);

		static TArray<int32> get_item_by_class_type(const e_class_type class_type, const TArray<int32> item_array);

		static bool is_robot_account(xstring robot_standard, xstring robot_account);

		//生成卓越属性
	private:
		static void set_excellent_had_flag(const int32& total_num, const int32& final_num, s_item_info& data_info, int32 item_const_att = 0);
		static void set_excellent_ratio_value(EquipTemplate* equip_template_ptr, s_item_info& data_info, int32 item_const_att = 0);
		static void set_spirit_excellent_ratio_value(SpiritTemplate* spirit_template_ptr, s_item_info& data_info, int32 item_const_att = 0);
		static int32 rand_excellent_att_color(EquipTemplate* equip_template_ptr, int32 item_const_att = 0);
		static int32 rand_spirit_excellent_att_color(SpiritTemplate* spirit_template_ptr, int32 item_const_att = 0);
	public:
		static void make_new_excellent_att(EquipTemplate* equip_template_ptr, faith::s_item_info& item_data, int32 item_const_att);
		static void make_new_spirit_eccellent_att(SpiritTemplate* spirit_template_ptr, faith::s_item_info& item_data, int32 item_const_att);
		static int32 get_activity_sec_left(int32 activity_type, e_activity_time_get time_get, bool is_other_time = false, bool is_cross_city_war = false, bool is_cross_active = false);

		static int32 get_activity_sec_left(int32 activity_type, const std::vector<int32>& ActivityOpenTime, const std::vector<int32>& OpenDate, e_activity_time_get time_get, bool is_cross_city_war = false, bool is_cross_active = false);

		static TArray<float> get_excellent_att_array(const TArray<float>& base_att_array, const faith::s_item_info& item_data);
		static TArray<int32> get_excellent_color_array(
			const TArray<float>&  base_att_info_array,
			const TArray<int32>&  green_color_standard_array,
			const TArray<int32>&  blue_color_standard_array,
			const TArray<int32>&  purple_color_standard_array,
			const TArray<int32>&  orange_color_standard_array,
			const TArray<int32>&  red_color_standard_array,
			const TArray<int32>&  pink_color_standard_array,
			const faith::s_item_info& item_data);
		
		static bool is_can_use_this_jewel_slot_on_this_equip_level(int32 equip_level, int32 slot_index);
		static bool is_can_use_this_jewel_slot_on_this_vip_level(int32 role_vip_level, int32 slot_index);
		static int32 get_jewel_slot_open_num_by_equip(int32 equip_level);
		static int32 get_jewel_slot_open_num_by_vip(int32 role_vip_level);
		static float get_arena_first_high_reward_factor(int32 role_rank);
		static int32 get_cur_time_limit_activity_id_by_temp(const s_time_limit_activity_temp& time_limit_act_temp, const std::map<int32, s_time_limit_activity_branch_temp>& branch_temp_map, int32& open_time, int32& end_time, int32& cycle_num, int32& cycle_index, int32& basics_open_time, int32& first_branch_temp_id, int32 cross_server_time, int32 merge_server_time, bool is_cross_server);
		static int32 get_cur_time_limit_activity_base_time_by_temp(const s_time_limit_activity_temp& time_limit_act_temp, int32 cross_server_time, int32 merge_server_time);
		static void	parse_char_to_vector(std::vector<int32>& int_arr_value, const xstring& temp_str, bool is_need_add_mid_str = false, xstring symbol_str = ",");//string转int vector,格式为(1,2,3,) 末尾如果不是逗号则bool值填true
		static void	parse_vector_to_char(xstring& temp_str, const std::vector<int32>& int_arr_value);//int vector转string,转换后格式为(1,2,3,)
		static bool is_no_superposition_time_limit_activity(int32 act_type);//是否为只存在一个的活动
		static int32 get_cur_time_limit_branch_id_with_first_id(const s_time_limit_activity_temp& time_limit_act_temp, const std::map<int32, s_time_limit_activity_branch_temp>& branch_temp_map, int32 open_time, int32 end_time, int32 first_branch_temp_id);//根据首ID，时间等数据算出当前子表ID，主要用于旧活动最后周期时做处理
		static bool is_cross_time_limit_cross_activity(int32 act_type);//是否为跨服类型的活动
		static bool is_rank_time_limit_cross_activity(int32 act_type);//是否为排行榜类型的活动
		static bool is_time_limit_behavior_welfare(int32 act_type);//是否为启示之路类活动
		static int32 get_behavior_welfare_reward_type(int32 act_type);//获得启示之路领奖的活动类型
		static int32 get_pk_king_max_num_by_time(e_pk_king_time_type time_type);		//根据阶段获取当前阶段最大人数
		static std::string load_random_role_name();
		static bool is_big_endian();
		static std::vector<float> get_equip_upgrade_att_array(int32 equip_id, int32 upgrade_num);
		static std::vector<int32> reward_regional_diff(int32 template_id);
		static float upgrade_number_change(float numerical);
	};
}
