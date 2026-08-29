#pragma once
#include "logic/type_def.hpp"

namespace faith
{
	class buff_set;
	class buff_item;
	class buff_proto_buff_save_load;
	class buff_proto_buff_operation;
	struct s_trigger_param;
	struct buff_ex_env_param;

	class buff_man
	{
	public:
		static bool init_manager( );
		static void heart_tick(const int32& unit_array_index, const int64& new_time);
		static bool on_event(s_trigger_param& trigger_param);
		static void load(const int32& unit_array_index, const buff_proto_buff_save_load& load_data);
		static bool load_buff_by_db_lua(int32 unit_array_index, const char *data_ptr, int32 data_len);
		static void save(const int32& unit_array_index, const int32& save_type_ex);
		static buff_set& get_buff_set_by_index(const int32& unit_array_index);
	public:
		static void handle_msg(const int32& unit_array_index, buff_proto_buff_operation const& msg);
	public:
		static guid_64 add_buff_inst(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, buff_ex_env_param* penv = NULL, bool is_ignore_att= false, int32 effect_index = -1);
		static bool add_buff_pile(const int32& target_array_index, const guid_64& buff_guid);
		static bool dec_buff_pile(const int32& sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool same_sender_del = false);
		static bool del_buff_pile_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del = false);
		static bool del_buff_by_inst_id(const int32& sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool server = true, bool same_sender_del = false);
		static bool del_buff_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del = false);
		static bool del_buff_by_template_id_one(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del = false);
		static bool del_buff_by_series_id(const int32& sender_array_index, const int32& target_array_index, const int32& series_id, bool same_sender_del = false);
		static bool del_buff_by_type(const int32& sender_array_index, const int32& target_array_index, const int32& buff_type);
		static bool del_buff_when_map_end(const int32& target_array_index);
		static bool del_all_buff(const int32& sender_array_index, const int32& target_array_index, bool same_sender_del = false);
		static bool is_have_buff_by_inst_id(const int32& sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool same_sender_del = false);
		static bool is_have_buff_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del = false);
		static bool is_have_buff_by_serise_id(const int32& sender_array_index, const int32& target_array_index, const int32& serise_id, bool same_sender_del = false);
		static int32 get_buff_pile_by_inst_id(const int32& sender_array_index, const int32& target_array_index, const guid_64& buff_guid, bool same_sender_del = false);
		static int32 get_buff_pile_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del = false);
		static buff_item* get_buff_item_by_series_id(const int32& sender_array_index, const int32& target_array_index, const int32& series_id, bool same_sender_del = false);
		static buff_item* get_buff_item_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del = false);
		static void unit_destroy(const int32& unit_array_index);
		static void offline_buffs(const int32& unit_array_index);
		static guid_64 find_buff_by_template_id(const int32& sender_array_index, const int32& target_array_index, const int32& buff_template_id, bool same_sender_del = false);
		static void sync_buff_set_info(int32 sender_array_index, int32 target_array_index = -1);
		static void mod_buff_during_time_by_series_id(const int32& target_array_index, const int32& series_id, const int32& during_time_fix);
	};
}