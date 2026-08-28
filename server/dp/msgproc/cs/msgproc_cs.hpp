#ifndef _MSGPROC_CS_H_
#define _MSGPROC_CS_H_

#include "logic/type_def.hpp"
#include "db_manager.hpp"
#include "utility/parse_msg.h"
#include  "../../def/template/template_manager.h"

namespace hld
{
	/*
	 * 角色上线统一LOADINDG链处理
	 */
	void cs2dp_req_char_online_loading_data( uint32 connindex, const void* data_ptr, size_t data_len );

	//角色基本属性
	void cs2dp_req_load_other_info(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_send_info_end(bool result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag save_flag, int32 save_type_ex);
	void cs2dp_req_load_char_info_cs(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_info_cs_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_save_char_info_cs(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_save_char_info_cs_end(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag save_flag, int32 save_type_ex, xstring sql);

	//角色事件/邮件
	void cs2dp_req_load_char_globel_mail_had_cs(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_req_load_char_globel_mail_had_cs_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);

	void cs2dp_req_del_char_mail(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_del_char_mail_end(db_result_type result, uint32 connindex, guid_64 unit_guid, int32 unit_array_index);
	void cs2dp_req_del_char_mail_item(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_add_char_mail_item_end(db_result_type result);

	void cs2dp_req_add_char_mail(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_auto_load_character_mail(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_add_char_mail_end(db_result_type result);


	void cs2dp_req_loading_arena_player_data(uint32 connindex, const void* data_ptr, size_t data_len);

	//获取其他玩家信息
	void cs2dp_req_get_other_player_info(uint32 connindex, const void* data_ptr, size_t data_len);
	//void cs2dp_req_get_other_player_info_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);

	void cs2dp_req_get_other_player_info_base(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_req_get_other_player_info_base_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid);
	void cs2dp_req_get_other_player_info_equiping(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_equiping_end(db_result_type result, uint32 connindex, const guid_64& role_guid,const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_special_name(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_special_name_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_base_group(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_base_group_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_spirit(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_spirit_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_wing(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_mount(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_wing_or_mount_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info, bool is_wing);
	void cs2dp_req_get_other_player_info_buff(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid);
	void cs2dp_req_get_other_player_info_buff_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid);
	void cs2dp_req_get_other_player_info_feather(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_feather_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const s_unit_info& unit_info);
	void cs2dp_req_get_other_player_info_appearance(uint32 connindex, const guid_64& role_guid, const guid_64& target_guid);
	void cs2dp_req_get_other_player_info_appearance_end(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid);

	void cs2dp_save_person_information_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_save_person_information_to_db_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag data_flag, int32 save_type_ex);

	void cs2dp_load_person_information_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_get_person_information_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);

	void cs2dp_get_other_person_information_proc(uint32 connindex, const void* data_ptr, size_t data_len);
	void cs2dp_get_other_person_information_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const guid_64& target_guid, const int32& unit_array_index, bool is_ask_player_base_info);

	void cs2dp_load_role_competition_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index, const int32& competition_type);
	void cs2dp_load_role_competition_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index, const int32& competition_type);
	void cs2dp_save_role_competition_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len);

	void cs2dp_save_role_competition_to_db_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, int32 unit_array_index, e_data_flag data_flag, int32 save_type_ex);

	void cs2dp_load_role_dragontrip_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_load_role_dragontrip_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_save_role_dragontrip_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len);


	void cs2dp_load_role_skytreasure_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_load_role_skytreasure_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_save_role_skytreasure_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len);

	void cs2dp_load_role_starark_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_load_role_starark_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_save_role_starark_to_db_proc(uint32 connindex, const void* data_ptr, size_t data_len);

	bool cs2dp_save_role_starark_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
	void cs2dp_save_push_sql_lua(uint32 connindex, int64 role_guid, int32 unit_array_index, int64 data_flag, const char *sql, int32 save_type_ex);
	void cs2dp_save_push_sql_no_cache_lua(uint32 connindex, int64 role_guid, int32 unit_array_index, int64 data_flag, const char *sql, int32 save_type_ex);
	void cs2dp_save_push_sql_no_callback_lua(const char *sql, int64 data_flag, int64 role_guid);
	bool cs2dp_save_role_dragontrip_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
	bool cs2dp_save_role_competition_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);
	bool cs2dp_save_role_person_information_cache_lua(int64 role_id, const char* data_ptr, size_t data_len);


	void cs2dp_load_role_time_feed_back_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_load_role_time_feed_back_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_save_role_time_feed_back_proc(uint32 connindex, const void* data_ptr, size_t data_len);

	void cs2dp_load_role_time_limit_gift_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_load_role_time_limit_gift_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_save_role_time_limit_gift_proc(uint32 connindex, const void* data_ptr, size_t data_len);

	void cs2dp_load_subscribe_daily_info_proc(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index);
	void cs2dp_load_subscribe_daily_info_end_proc(db_result_type result, uint32 connindex, const guid_64& role_guid, const int32& unit_array_index);
	void cs2dp_save_subscribe_daily_info_proc(uint32 connindex, const void* data_ptr, size_t data_len);


	//清理全部缓存
	void cs2dp_req_load_char_data(uint32 connindex, const guid_64& role_guid, const guid_64& up_role_guid, const int32& unit_array_index, e_data_flag e_data_flag);
	void cs2dp_req_save_char_data(uint32 connindex, const void* data_ptr, size_t data_len);

}

#endif
