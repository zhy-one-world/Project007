#pragma once
#include "logic/type_def.hpp"

namespace hld
{
	class skill_set;
	class skill_proto_skill_operation;
	class skill_proto_skill_save_load;
	struct use_skill_param;
	struct s_unit_identifier;

    class skill_manager
    {
    public:
		static void heart_tick(const int32 unit_array_index, const int64& new_time, const int32& tick_time);
		static void init_manager();
	public:
		static void skill_change_msg(int32 unint_index, int32 skill_template_id, int32 pos_index);
		static void skill_spell(use_skill_param& param);
		static void skill_cast(use_skill_param& param);
		static void skill_hurt(use_skill_param& param);
		static void distribute_msg(s_unit_identifier const& unit_idf, skill_proto_skill_operation& request, int32 damage_num);
		static void skill_set_owner(const int32& unit_array_index);
		static void load_skill(const int32& unit_array_index, const skill_proto_skill_save_load& load_msg);
		static bool load_skill_by_db_lua(int32 unit_array_index, const char* data_ptr, int32 data_len);
		static void save_skill(const int32& unit_array_index, int32 save_type_ex);
		static void send_skill_all(const int32& unit_array_index);
		static skill_set& get_skill_set(const int32 unit_array_index);
		static void remove_unit_skill(const int32& unit_array_index);
		static void exp_level_up(const int32& unit_array_index, int32 cur_level);
		static void replace_skill_id(const int32& unit_array_index, int32 skill_series, int32 skill_template_id, bool apply, bool use_level = false);
	public:	 
		static void resp_master_skill_by_id(const int32& unit_array_index, int32 skill_id);
		static void remove_unit_skill_by_id(const int32& unit_array_index, int32 skill_id);
    };
}

