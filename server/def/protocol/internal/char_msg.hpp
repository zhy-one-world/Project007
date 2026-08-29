/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 20:05
  file base: character
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_CHARACTER_HPP_
#define _FAITH_CHARACTER_HPP_

#include "base.hpp"
#include "internet/error.pb.h"
#include "internet/ws2cs.pb.h"
#include "Logic/char_def.hpp"
#include "Logic/dragontrip_def.h"
#include "Logic/guide_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/login_def.hpp"
#include "Logic/pokedex_def.hpp"
#include "Logic/relation_def.h"
#include "Logic/skill_def.hpp"
#include "Logic/skytreasure_def.h"
#include "Logic/special_name_def.hpp"
#include "Logic/starark_def.h"
#include "Logic/team_def.hpp"
#include "Logic/time_limit_activity_def.hpp"
#include "logic/type_def.hpp"
#include "Logic/world_def.hpp"

namespace faith 
{

#pragma pack(push,1)

enum
{
		// Enum character
		e_msgindex_ls2dp_enum_character = e_msg_base_character,
		e_msgindex_dp2ls_enum_character,
		e_msgindex_dp2ls_load_enum_item,
		e_msgindex_dp2ls_load_enum_buff,
		e_msgindex_dp2ls_load_enum_spirit,
		e_msgindex_dp2ls_load_enum_time,
		// Create character
		e_msgindex_fep2ls_create_character,
		e_msgindex_ls2dp_create_character,
		e_msgindex_dp2ls_create_character,
		e_msgindex_ls2fep_create_character,
		// game data
		e_msgindex_ws2dp_get_game_info,
		e_msgindex_dp2ws_get_game_info,
		// gm
		e_msgindex_ws2dp_set_role_right,
		e_msgindex_ws2dp_gm_get_role_info,
		e_msgindex_dp2ws_gm_get_role_info,
		e_msgindex_dp2ws_gm_get_role_lock,
		e_msgindex_ws2dp_gm_modify_role_info,
		e_msgindex_dp2ws_gm_modify_role_info,
		e_msgindex_ws2dp_save_recharge,
		e_msgindex_dp2ws_save_recharge,
		e_msgindex_ws2dp_save_recharge_end,
		e_msgindex_dp2ws_send_recharge_end,
		e_msgindex_ws2dp_gm_get_role_items_info,
		e_msgindex_ws2dp_gm_open_time,
		e_msgindex_ws2dp_gm_change_password,
		e_msgindex_ws2dp_gm_begin_cross,
		e_msgindex_ws2dp_gm_change_name,
		e_msgindex_dp2ws_gm_change_name,
		e_msgindex_ws2dp_gm_del_cache,
		e_msgindex_ws2dp_gm_del_role,
		e_msgindex_ws2dp_gm_stop_login,
		e_msgindex_ws2dp_set_cross_time,
		e_msgindex_dp2ws_gm_set_result,
		e_msgindex_ws2dp_save_server_list,
		e_msgindex_ws2dp_load_server_list,
		e_msgindex_dp2ws_load_server_list_end,
		// Load character
		e_msgindex_ws2dp_load_character,
		e_msgindex_dp2ws_load_character,
		e_msgindex_dp2cs_load_character,
		e_msgindex_dp2cs_load_character_error,
		// Save character
		e_msgindex_cs2dp_save_character,
		e_msgindex_dp2cs_save_character,
		e_msgindex_ws2db_save_character_ws_info,
		//data
		e_msgindex_dp2cs_load_char_data,
		e_msgindex_dp2cs_save_char_data,
		//money
		e_msgindex_dp2cs_load_char_money,
		e_msgindex_cs2dp_save_char_money,
		//time
		e_msgindex_dp2cs_load_char_time,
		e_msgindex_cs2dp_save_char_time,
		//logic
		e_msgindex_dp2cs_load_char_logic,
		e_msgindex_cs2dp_save_char_logic,

		// Delete character
		e_msgindex_ls2dp_delete_character,
		e_msgindex_dp2ls_delete_character,
		// Check character
		e_msgindex_ls2dp_check_character_name,
		e_msgindex_dp2ls_check_character_name,
		// Revive character
		e_msgindex_ls2dp_revive_character,
		e_msgindex_dp2ls_revive_character,

		//show_info mount/wing/quick_call_mount
		e_msgindex_dp2cs_load_char_show,
		e_msgindex_cs2dp_save_char_show,

		//yesterday_daily_must_do
		e_msgindex_dp2cs_load_char_yesterday_must_do,
		e_msgindex_cs2dp_save_char_yesterday_must_do,

		//get other player info
		e_msgindex_cs2dp_get_other_player_info,
		e_msgindex_dp2cs_get_other_player_info_base_end,
		e_msgindex_dp2cs_get_other_player_info_equiping_end,
		e_msgindex_dp2cs_get_other_player_info_special_name_end,
		e_msgindex_dp2cs_get_other_player_info_base_group_end,
		e_msgindex_dp2cs_get_other_player_info_spirit_end,
		e_msgindex_dp2cs_get_other_player_info_wing_or_mount_end,
		e_msgindex_dp2cs_get_other_player_info_buff_end,
		e_msgindex_dp2cs_get_other_player_info_belief_end,
		e_msgindex_dp2cs_get_other_player_info_feather_end,

		//relation
		e_msgindex_ws2dp_save_player_relation,
		e_msgindex_dp2ws_load_player_relation,
		e_msgindex_ws2dp_load_relation_info,
		e_msgindex_dp2ws_load_relation_info,
		e_msgindex_ws2dp_req_load_relation_list,
		e_msgindex_cs2ws_add_friendliness_value,
		e_msgindex_ws2cs_full_friend_guid,
		e_msgindex_cs2ws_fuben_add_friendliness_value,
		e_msgindex_ws2dp_add_relation,
		e_msgindex_dp2ws_add_relation,
		e_msgindex_ws2dp_del_relation,
		e_msgindex_ws2cs_req_relation_end,

		//hightest record
		e_msgindex_cs2dp_save_hightest_record,
		e_msgindex_dp2cs_load_hightest_record_end,


		//daily_must_do_num
		e_msgindex_dp2cs_load_char_daily_must_do_count,
		e_msgindex_cs2dp_save_char_daily_must_do_count,

		//player rename
		e_msgindex_ws2dp_check_player_name,
		e_msgindex_dp2ws_check_player_name,
		e_msgindex_ws2dp_change_player_name,
		e_msgindex_dp2ws_change_player_name_end,
		e_msgindex_ws2cs_change_player_name,
		e_msgindex_ws2cs_sub_player_rename_item,
		e_msgindex_cs2ws_sub_player_rename_item_end,

		//peron_information
		e_msgindex_cs2dp_save_person_information_to_db,
		e_msgindex_cs2dp_get_person_information,
		e_msgindex_dp2cs_get_person_information,
		e_msgindex_cs2dp_get_other_person_information,
		e_msgindex_dp2cs_get_other_person_information,

		e_msgindex_cs2ws_sync_person_information,
		e_msgindex_cs2ws_delete_person_information,
		e_msgindex_cs2ws_get_person_couple,

		// competition
		e_msgindex_cs2dp_save_role_competition_to_db,
		e_msgindex_dp2cs_get_role_competition,

		e_msgindex_dp2cs_send_connect_success,

		// appearance
		e_msgindex_dp2cs_get_other_player_info_appearance_end,

		// dargontrip
		e_msgindex_cs2dp_save_role_dragontrip_to_db,
		e_msgindex_dp2cs_get_role_dragontrip,

		//skytreasure
		e_msgindex_cs2dp_save_role_skytreasure_to_db,
		e_msgindex_dp2cs_get_role_skytreasure,
		
		// starark
		e_msgindex_cs2dp_save_role_starark_to_db,
		e_msgindex_dp2cs_get_role_starark,

		// time_feed_back
		e_msgindex_cs2dp_save_time_feed_back_to_db,
		e_msgindex_dp2cs_get_time_feed_back_to_db_end,

		// time_limit_gift
		e_msgindex_cs2dp_save_time_limit_gift_to_db,
		e_msgindex_dp2cs_get_time_limit_gift_to_db_end,

		// time_limit_gift
		e_msgindex_cs2dp_save_subscribe_daily_info_to_db,
		e_msgindex_dp2cs_get_subscribe_daily_info_to_db_end,

		// other_relation
		e_msgindex_ws2dp_load_relation_info_other_server,
		e_msgindex_dp2ws_load_relation_info_other_server,
	};

	/************************************************************************/
	/*           Enum                                                      */
	/************************************************************************/
	struct ls2dp_enum_character : public packet_base
	{
		s_client_uid	client_uid;		//	unique client session identifier
		ui16				page_index;
		xchar					account[max_account_length + 1];	//	account name string
		int32						server_id;

		ls2dp_enum_character()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msgindex_ls2dp_enum_character;  
		}
	};

	struct dp2ls_enum_character : public faith::packet_base
	{
		faith::s_client_uid			client_uid;				//	unique client session identifier
		int32						char_count;
		s_unit_info					char_data[faith::max_character_num];

		dp2ls_enum_character()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ls_enum_character;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&char_data - (ULONG_PTR)&wheader;
			return (basic_len + char_count * sizeof(s_unit_info));
		}
	};

	struct dp2ls_load_enum_item : public faith::packet_base
	{
		faith::s_client_uid					client_uid;
		guid_64								role_guid;
		int32								data_num; 
		s_item_info							item_data[role_show_item_num];

		dp2ls_load_enum_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ls_load_enum_item;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&item_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_item_info));
		}
	};
	struct dp2ls_load_enum_buff : public faith::packet_base
	{
		faith::s_client_uid					client_uid;
		guid_64								role_guid;
		int32								data_num;
		int32								buff_data[max_enum_buff_data_num];


		dp2ls_load_enum_buff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ls_load_enum_buff;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&buff_data - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(int32));
		}
	};
	struct dp2ls_load_enum_spirit : public faith::packet_base
	{
		faith::s_client_uid					client_uid;
		guid_64								role_guid;
		s_item_info					temp_sprite_info;

		dp2ls_load_enum_spirit()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ls_load_enum_spirit;
		}
	};
	struct dp2ls_load_enum_time : public faith::packet_base
	{
		faith::s_client_uid					client_uid;
		guid_64								role_guid;
		int64								login_out_time;

		dp2ls_load_enum_time()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ls_load_enum_time;
		}
	};

	struct dp2cs_load_char_show : public faith::packet_base
	{
		faith::s_client_uid			client_uid;				//	unique client session identifier
		s_char_show_info			show_info;		
		int32						unit_array_index;

		dp2cs_load_char_show()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_show;
		}
	};
	struct cs2dp_save_char_show : public faith::packet_base
	{
		s_char_show_info			show_info;

		cs2dp_save_char_show()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_show;
		}
	};

	/************************************************************************/
	/*           Create                                                     */
	/************************************************************************/
	struct fep2ls_create_character : public packet_base
	{
		s_client_uid						client_uid;
		int32								login_msg_len;
		ui8									login_proto_msg[MAX_C2S_S2C_PACKAGE_SIZE];
		int32								create_character_len;
		ui8									create_character_msg[MAX_C2S_S2C_PACKAGE_SIZE];
		fep2ls_create_character()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_fep2ls_create_character;
		}
	};
	struct ls2dp_create_character : public packet_base
	{
		s_client_uid						client_uid;
		int32								server_id;
		s_unit_info			                init_role;
		int32								item_num;
		faith::s_item_info					init_item[init_item_num];
		int32								skill_num;
		faith::s_skill_info					init_skill[init_skill_num];
		int32								create_level_requirement;
		s_record_info						record_info_list[faith::max_guide_record_num];
		int32								record_num;
		int32								login_msg_len;
		ui8									login_proto_msg[MAX_C2S_S2C_PACKAGE_SIZE];
		ls2dp_create_character()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ls2dp_create_character;
		}
	};
	struct dp2ls_create_character : public faith::packet_base
	{
		e_create_role_result eresult;
		faith::s_client_uid		client_uid;				//	unique client session identifier
		guid_64				role_guid;				//	only available when eResult==e_success
		s_unit_info role_info;
		int32 login_msg_len;
		ui8	 login_proto_msg[MAX_C2S_S2C_PACKAGE_SIZE];
		dp2ls_create_character()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ls_create_character;
		}
	};
	struct ls2fep_create_role : public packet_base
	{
		e_create_role_result eresult;
		faith::s_client_uid		client_uid;				//	unique client session identifier
		guid_64				role_guid;				//	only available when eResult==e_success
		s_unit_info role_info;
		ls2fep_create_role()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ls2fep_create_character;
		}
	};
	/************************************************************************/
	/*           Delete                                                     */
	/************************************************************************/
	struct ls2dp_delete_character : public packet_base
	{
		s_client_uid					client_uid;	//	unique client session identifier
		int32							server_id;
		xchar							account[max_account_length + 1];
		guid_64							role_guid;	//
		e_del_char_type					del_type;	//0暂时删除,可恢复,1恢复,2彻底删除

		ls2dp_delete_character()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ls2dp_delete_character;
		}
	};

	struct dp2ls_delete_character_end : public packet_base
	{
		int32						del_time;
		s_client_uid				client_uid;		//	unique client session identifier
		guid_64						role_guid;		//	only available when eResult==e_success
		e_del_char_type				del_type;

		dp2ls_delete_character_end()
		{
			memset(this, 0, sizeof(*this));
			del_type = e_del_char_none;
			wheader = e_msgindex_dp2ls_delete_character;
		}
	};
	/************************************************************************/
	/*           Load                                                       */
	/************************************************************************/
	struct dp2ws_get_game_info : public packet_base
	{
		int32			server_id;
		int32			begin_time;
		int32			server_info_arr[e_server_info_type_max];
		int32			gm_commond_arr[e_need_server_cross_max - 1];
		dp2ws_get_game_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_get_game_info;
		}
	};

	struct ws2dp_set_role_right : public packet_base
	{
		xchar	role_mark[max_account_length + 1];
		int32	right_type;
		int32	right_value;
		int32	right_time;
		s_daemon_callback_info	callback_info;
		ws2dp_set_role_right()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_set_role_right;
		}
	};

	struct dp2ws_gm_set_result : public packet_base
	{
		s_daemon_callback_info callback_info;
		int32	result_code;
		xchar	result_msg[gm_set_result_len + 1];
		dp2ws_gm_set_result()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_dp2ws_gm_set_result;
		}
	};

	struct ws2dp_gm_get_role_info : public packet_base
	{
		xchar	role_mark[max_account_length + 1];
		int32	get_type;
		s_daemon_callback_info callback_info;
		int32	check_type;
		ws2dp_gm_get_role_info()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_get_role_info;
		}
	};
	struct dp2ws_gm_get_role_info : public packet_base
	{
		s_daemon_callback_info callback_info;
		s_unit_info		role_info[faith::max_character_num * 10];
		int32			data_num;
		int32			check_type;
		dp2ws_gm_get_role_info()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_dp2ws_gm_get_role_info;
		}
	};
	struct ws2dp_gm_modify_role_info : public packet_base
	{
		guid_64 role_guid;
		char	sql_str[max_gm_sql_str_length + 1];
		s_daemon_callback_info callback_info;
		ws2dp_gm_modify_role_info()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_modify_role_info;
		}
	};
	struct dp2ws_gm_modify_role_info : public packet_base
	{
		guid_64 role_guid;
		s_daemon_callback_info callback_info;
		dp2ws_gm_modify_role_info()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_dp2ws_gm_modify_role_info;
		}
	};
	struct ws2dp_save_recharge : public packet_base
	{
		xchar				order_id[max_recharge_order_length + 1];
		guid_64				role_guid;
		int32				game_goods_id;
		float				pay_price;
		int64				order_num;
		int32				payment_type;
		int64				client_uid;
		int32				force_error_code;
		int32				direct_diamond;
		login_fixed_data	third_info;
		int32				login_type;
		xchar				order_token[max_recharge_order_token_length + 1];

		ws2dp_save_recharge()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_recharge;
			force_error_code = e_recharge_error_none;
		}
	};
	struct dp2ws_save_recharge : public packet_base
	{
		xchar	order_id[max_recharge_order_length + 1];
		guid_64 role_guid;
		int32	game_goods_id;
		float	pay_price;
		int32	success;
		int64	order_num;
		int32	payment_type;
		int32	direct_diamond;
		int64	client_uid;
		xchar	order_token[max_recharge_order_token_length + 1];
		dp2ws_save_recharge()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_save_recharge;
		}
	};

	struct dp2ws_send_recharge_end_result : public packet_base
	{
		int32	result_type;
		int64	order_num;
		int32	payment_type;
		int32	goods_id;
		int32	direct_diamond;
		guid_64	role_guid;
		xchar	order_id[max_recharge_order_length + 1];
		dp2ws_send_recharge_end_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_send_recharge_end;
			payment_type = invalid_payment_type;
		}
	};

	struct ws2dp_save_recharge_end : public packet_base
	{
		xchar	order_id[max_recharge_order_length + 1];
		guid_64 role_guid;
		int32	game_goods_id;
		int64	order_num;
		int32	payment_type;
		ws2dp_save_recharge_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_recharge_end;
		}
	};

	struct dp2ws_gm_get_role_lock : public packet_base
	{
		guid_64		role_guid;
		int32		is_login;
		int32		is_send_chat;
		s_daemon_callback_info callback_info;
		dp2ws_gm_get_role_lock()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_gm_get_role_lock;
		}
	};
	
	struct ws2dp_gm_get_role_items_info : public packet_base
	{
		guid_64		role_guid;
		s_daemon_callback_info callback_info;
		ws2dp_gm_get_role_items_info()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_get_role_items_info;
		}
	};
	struct ws2dp_gm_open_time : public packet_base
	{
		int32		open_time;
		s_daemon_callback_info callback_info;
		ws2dp_gm_open_time()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_open_time;
		}
	};
	struct ws2dp_gm_change_password : public packet_base
	{
		xchar			account[max_account_length + 1];
		xchar			new_password[max_password_length + 1];
		s_daemon_callback_info callback_info;
		ws2dp_gm_change_password()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_change_password;
		}
	};

	struct ws2dp_set_server_info : public packet_base
	{
		int32		server_info_value;
		int32		server_info_type;
		s_daemon_callback_info callback_info;
		ws2dp_set_server_info()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_set_cross_time;
		}
	};
	struct ws2dp_gm_need_begin_cross : public packet_base
	{
		int32		need_begin_cross;
		int32		gm_type;
		int64		req_http_id;
		ws2dp_gm_need_begin_cross()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_gm_begin_cross;
		}
	};
	struct ws2dp_save_last_server_list : public packet_base
	{
		int32		server_id;
		xchar		last_server_list[last_server_list_max_length];
		ws2dp_save_last_server_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_server_list;
		}
	};

	struct ws2dp_load_last_server_list : public packet_base
	{
		int32		server_id;
		ws2dp_load_last_server_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_server_list;
		}
	};

	struct dp2ws_load_last_server_list : public packet_base
	{
		s_last_server_list_db last_server_list;
		dp2ws_load_last_server_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_server_list_end;
		}
	};

	struct ws2dp_gm_change_account : public packet_base
	{
		xchar		change_name[max_account_length + 1];
		xchar		target_name[max_account_length + 1];
		s_daemon_callback_info callback_info;
		ws2dp_gm_change_account()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_change_name;
		}
	};
	struct dp2ws_gm_change_account : public packet_base
	{
		int32		change_res;
		s_daemon_callback_info callback_info;
		dp2ws_gm_change_account()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_gm_change_name;
		}
	};
	struct ws2dp_gm_del_cache : public packet_base
	{
		guid_64		role_guid;
		ws2dp_gm_del_cache()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_gm_del_cache;
		}
	};
	struct ws2dp_gm_stop_login : public packet_base
	{
		int32						is_stop;
		s_daemon_callback_info		callback_info;
		ws2dp_gm_stop_login()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_stop_login;
		}
	};
	struct ws2dp_gm_del_role : public packet_base
	{
		guid_64		role_guid;
		int32		is_del;
		s_daemon_callback_info callback_info;
		ws2dp_gm_del_role()
		{
			memset(this, 0, sizeof(*this));
			callback_info.clear_data();
			wheader = e_msgindex_ws2dp_gm_del_role;
		}
	};

	struct ws2dp_load_character : public packet_base
	{
		guid_64	role_guid;
		xchar	role_account[max_account_length + 1];

		ws2dp_load_character()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msgindex_ws2dp_load_character;
		}
	};


	struct dp2ws_load_character : public packet_base
	{
		e_error_code	eresult;
		s_unit_info		role_info;
		s_unit_ws_info	role_ws_info;
		int32			is_login;//是否可以登录
		int32			is_send_chat;//是否可以发言
		int64			worship_value;	//被崇拜值
		dp2ws_load_character()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msgindex_dp2ws_load_character;
		}
	};

	struct ws2dp_save_role_ws_info : public packet_base
	{
		guid_64			role_guid;
		s_unit_ws_info	role_ws_info;
		ws2dp_save_role_ws_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2db_save_character_ws_info;
		}
	};

	struct dp2cs_load_role_info : public  packet_base
	{
		s_unit_info				data_info;
		int32					unit_array_index;
		dp2cs_load_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_character;
		}
	};
	



	//角色load失败
	struct dp2cs_load_data_error : public faith::packet_base
	{
		guid_64		role_guid;
		int32		array_index;
		int64		error_code;
		int			error_result;
		dp2cs_load_data_error()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_character_error;
		}
	};
	struct dp2cs_load_money_info : public packet_base
	{
		guid_64					role_guid;
		int32					unit_array_index;
		s_money_info			data_info;
		s_daemon_callback_info callback_info;
		dp2cs_load_money_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_money;
		}
	};
	struct dp2cs_load_time_info : public packet_base
	{
		guid_64				role_guid;
		int32				unit_array_index;
		s_time_info			data_info;
		s_daemon_callback_info callback_info;
		dp2cs_load_time_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_time;
		}
	};
	struct dp2cs_load_logic_info : public packet_base
	{
		guid_64				role_guid;
		int32							unit_array_index;
		s_logic_info			data_info;
		dp2cs_load_logic_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_logic;
		}
	};

	/************************************************************************/
	/*           Save                                                       */
	/************************************************************************/
	struct cs2dp_save_character : public packet_base
	{
		int32					save_type_ex;
		s_unit_info				role_info;
		int32					unit_array_index;

		cs2dp_save_character()
		{
			memset(this, 0, sizeof(*this));
			save_type_ex = -1;
			wheader		 = e_msgindex_cs2dp_save_character;
		}
	};

	struct cs2dp_save_character_yesterday_remain_must_do : public packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		s_unit_yesterday_must_do_remain data_info[resource_find_back_day];

		cs2dp_save_character_yesterday_remain_must_do()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_yesterday_must_do;
		}
	};

	struct dp2cs_load_character_yesterday_remain_must_do : public packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		s_unit_yesterday_must_do_remain data_info[resource_find_back_day];

		dp2cs_load_character_yesterday_remain_must_do()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_yesterday_must_do;
		}
	};


	struct dp2cs_save_data_result : public packet_base
	{
		guid_64					role_guid;
		int32					unit_array_index;
		e_data_flag				save_flag;
		bool					is_ok;
		int32					save_type_ex;

		dp2cs_save_data_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_save_character;
		}
	};
	struct cs2dp_save_money : public faith::packet_base
	{
		guid_64							role_guid;								// guid	
		int32							unit_array_index;
		int32							save_type_ex;
		s_money_info					money_info;
		cs2dp_save_money()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_money;
		}
	};
	struct cs2dp_save_time : public faith::packet_base
	{
		guid_64								role_guid;								// guid	
		int32								unit_array_index;
		int32								save_type_ex;
		s_time_info							time_info;
		cs2dp_save_time()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_time;
		}
	};
	struct cs2dp_save_logic : public faith::packet_base
	{
		guid_64								role_guid;								// guid	
		int32								unit_array_index;
		int32								save_type_ex;
		s_logic_info						logic_info;
		cs2dp_save_logic()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_logic;
		}
	};

	//get other player info
	//base info
	struct cs2dp_get_other_player_info : public packet_base
	{
		guid_64				role_guid;
		guid_64				target_guid;
		cs2dp_get_other_player_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_get_other_player_info;
		}
	};
	struct dp2cs_get_other_player_info_base_end : public packet_base
	{
		guid_64					role_guid;
		s_unit_info				data_info;
		dp2cs_get_other_player_info_base_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_base_end;
		}
	};
	//base_group
	struct	s_base_group_info_db
	{
		int64					group_job;
		xchar					group_name[faith::max_name_size + 1];
		s_base_group_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct dp2cs_get_other_player_info_base_group_end : public packet_base
	{
		guid_64					role_guid;
		guid_64					target_guid;
		s_base_group_info_db	data_info;
		dp2cs_get_other_player_info_base_group_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_base_group_end;
		}
	};
	//equiping info
	struct dp2cs_get_other_player_info_equiping_end : public packet_base
	{
		guid_64					role_guid;
		s_item_info_db			data_info[role_show_item_num];
		dp2cs_get_other_player_info_equiping_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_equiping_end;
		}
	};

	//special name info
	struct dp2cs_get_other_player_info_special_name_end : public packet_base
	{
		guid_64					role_guid;
		s_special_name_info_db  data_info;
		dp2cs_get_other_player_info_special_name_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_special_name_end;
		}
	};
	//spirit info
	struct dp2cs_get_other_player_info_spirit_end : public packet_base
	{
		guid_64						role_guid;
		s_item_info_db			data_info;
		dp2cs_get_other_player_info_spirit_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_spirit_end;
		}
	};

	//wing or mount
	struct dp2cs_get_other_player_info_wing_or_mount_end : public packet_base
	{
		guid_64					role_guid;
		s_item_info_db			data_info[role_show_item_num];
		bool					is_wing;
		dp2cs_get_other_player_info_wing_or_mount_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_wing_or_mount_end;
		}
	};

	//buff
	struct dp2cs_get_other_player_info_buff_end : public packet_base
	{
		guid_64					role_guid;
		guid_64					target_guid;
		int32					data_num;
		int32					buff_info[max_enum_buff_data_num];
		dp2cs_get_other_player_info_buff_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_buff_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&buff_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(int32));
		}
	};

	//feather
	struct dp2cs_get_other_player_info_feather_end : public packet_base
	{
		guid_64					role_guid;
		s_item_info_db			feather_info[e_feather_sub_type_feather4];

		dp2cs_get_other_player_info_feather_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_feather_end;
		}
	};

	struct dp2ws_load_player_relation : public packet_base
	{
		guid_64					role_guid;
		int32					data_num;
		s_relation_info			relation_array[MAX_FRIEND_NUM*e_relationlist_type_max];
		dp2ws_load_player_relation()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_player_relation;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&relation_array - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_relation_info));
		}
	};

	struct ws2dp_save_player_relation : public packet_base
	{
		guid_64					role_guid;
		int32					data_num;
		s_relation_info			data_list[MAX_FRIEND_NUM*e_relationlist_type_max];
		ws2dp_save_player_relation()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_player_relation;
		}
		void clear_data()
		{
			role_guid.clear_data();
			data_num = 0;
			g_clear_data(data_list, MAX_FRIEND_NUM * e_relationlist_type_max);
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_list - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_relation_info));
		}
	};

	struct ws2dp_load_relation_info : public packet_base
	{
		guid_64					sender_guid;
		int32					list_type;
		guid_64					other_guid;
		ws2dp_load_relation_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_relation_info;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};

	struct dp2ws_load_relation_info : public packet_base
	{
		guid_64					sender_guid;
		s_relation_info			other_relation;
		dp2ws_load_relation_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_relation_info;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};

	struct ws2dp_load_relation_info_other_server : public packet_base
	{
		guid_64					sender_guid;
		int32					sender_server_id;
		int32					list_type;
		guid_64					other_guid;
		ws2dp_load_relation_info_other_server()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_relation_info_other_server;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};

	struct dp2ws_load_relation_info_other_server : public packet_base
	{
		guid_64					sender_guid;
		int32					sender_server_id;
		s_relation_info			other_relation;
		dp2ws_load_relation_info_other_server()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_relation_info_other_server;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};


	struct ws2dp_req_load_relation_list : public packet_base
	{
		guid_64					sender_guid;
		ws2dp_req_load_relation_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_req_load_relation_list;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
	};

	struct ws2dp_add_relation : public packet_base
	{
		guid_64					role_guid;
		guid_64					relation_guid;
		xchar					relation_name[max_name_size + 1];
		int64					relation_gs_value;
		int32					data_array[e_relation_data_max];
		
		ws2dp_add_relation()
		{
			role_guid.clear_data();
			relation_guid.clear_data();
			memset(relation_name, 0, sizeof(relation_name));
			relation_gs_value = 0;
			memset(data_array, 0, sizeof(data_array));

			wheader = e_msgindex_ws2dp_add_relation;
		}

		void set_relation_name(xstring role_name)
		{
			memset(relation_name, 0, sizeof(relation_name));
			memcpy(relation_name, role_name.c_str(), role_name.size() > sizeof(relation_name) ? sizeof(relation_name) : role_name.size());
		}
	};

	struct dp2ws_add_relation : public packet_base
	{
		guid_64					role_guid;
		guid_64					relation_guid;
		int32					result;
		int32					relation_type;
		dp2ws_add_relation()
		{
			role_guid.clear_data();
			relation_guid.clear_data();
			result = 0;
			relation_type = 0;
			wheader = e_msgindex_dp2ws_add_relation;
		}
	};

	struct ws2dp_del_relation : public faith::packet_base
	{
		guid_64					role_guid;
		guid_64					target_role_guid;
		int32					relation_type;

		ws2dp_del_relation()
		{
			role_guid.clear_data();
			target_role_guid.clear_data();
			relation_type = 0;

			wheader = e_msgindex_ws2dp_del_relation;
		}
	}; 

	struct cs2dp_save_hightest_record : public faith::packet_base
	{
		guid_64								role_guid;
		int32								unit_array_index;
		int32								save_type_ex;
		s_role_history_high_record_info		highest_info;
		cs2dp_save_hightest_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_hightest_record;
		}
	};

	struct dp2cs_load_hightest_record : public packet_base
	{
		guid_64									role_guid;
		int32									unit_array_index;
		s_role_history_high_record_info			highest_info;
		dp2cs_load_hightest_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_hightest_record_end;
			highest_info.clear_data();
		}
	};

	struct cs2ws_add_friendliness_value : public packet_base
	{
		guid_64			sender_guid;
		guid_64			addreessee_guid;
		int32			gift_id;
		int32			gift_count;
		cs2ws_add_friendliness_value()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_add_friendliness_value;
		}
	};


	struct ws2cs_full_friend_guid : public packet_base
	{
		int32			player_index;
		int32			highest_firendliness_value;
		int32			friend_num;
		bool			is_add;
		ws2cs_full_friend_guid()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_full_friend_guid;
		}
		void to_proto(faith::ws2cs_proto::full_friend_guid& msg)
		{
			msg.set_player_index(player_index);
			msg.set_highest_firendliness_value(highest_firendliness_value);
			msg.set_friend_num(friend_num);
			msg.set_is_add(is_add);
		}
		void from_proto(const faith::ws2cs_proto::full_friend_guid& msg)
		{
			player_index = msg.player_index();
			highest_firendliness_value = msg.has_highest_firendliness_value();
			friend_num = msg.friend_num();
			is_add = msg.is_add();

		}

	};

	struct cs2ws_fuben_add_friendliness_value : public packet_base
	{
		guid_64			team_member_guid[faith::max_team_member_num];
		int32           team_num;
		cs2ws_fuben_add_friendliness_value()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_fuben_add_friendliness_value;
		}
	};

	struct cs2dp_save_character_daily_must_do_count : public packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		int32							save_type_ex;
		s_role_daily_must_do_count data_info;

		cs2dp_save_character_daily_must_do_count()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_char_daily_must_do_count;
		}
	};

	struct dp2cs_load_character_daily_must_do_count : public packet_base
	{
		guid_64							role_guid;
		int32							unit_array_index;
		s_role_daily_must_do_count data_info;

		dp2cs_load_character_daily_must_do_count()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_load_char_daily_must_do_count;
		}
	};

	//rename
	enum e_change_role_name
	{
		e_change_failed_common_error = 0,
		e_change_role_success = 1,
		e_change_failed_role_name_invalid,
		e_change_failed_role_name_duplicate,
		e_change_failed_role_name_size_too_short,
		e_change_failed_role_name_size_too_long,
		e_change_failed_role_item_not_enough,
		e_change_role_name_available,
	};

	struct ws2dp_check_player_name : public packet_base
	{
		guid_64					role_guid;
		xchar					role_name[max_name_size + 1];

		ws2dp_check_player_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_check_player_name;
		}

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
		void to_proto(faith::server2dp_proto::ws2dp_check_player_name& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_role_name(role_name);
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::server2dp_proto::ws2dp_check_player_name& msg)
		{
			role_guid.server_64 = msg.role_guid();
			my_memcopy_string(role_name, max_name_size, msg.role_name());
		}
	};

	struct dp2ws_check_player_name_end : public packet_base
	{
		guid_64					role_guid;
		xchar					role_name[max_name_size + 1];
		int32					result;

		dp2ws_check_player_name_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_check_player_name;
		}

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
	};

	struct ws2dp_change_player_name : public packet_base
	{
		guid_64					role_guid;
		xchar					role_name[max_name_size + 1];
		
		ws2dp_change_player_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_change_player_name;
		}

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
		void to_proto(faith::server2dp_proto::ws2dp_change_player_name& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_role_name(role_name);
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::server2dp_proto::ws2dp_change_player_name& msg)
		{
			role_guid.server_64 = msg.role_guid();
			my_memcopy_string(role_name, max_name_size, msg.role_name());
		}
	};

	struct dp2ws_change_player_name_end : public packet_base
	{
		guid_64					role_guid;
		xchar					role_name[max_name_size + 1];
		int32					result;

		dp2ws_change_player_name_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_change_player_name_end;
		}

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
	};

	struct ws2cs_change_player_name : public packet_base
	{
		guid_64					role_guid;
		xchar					role_name[max_name_size + 1];
		ws2cs_change_player_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_change_player_name;
		}

		void set_role_name(xstring p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
		void to_proto(faith::ws2cs_proto::change_player_name& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_role_name(role_name);
		}
		void  my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}
		void from_proto(const faith::ws2cs_proto::change_player_name& msg)
		{
			role_guid.server_64 = msg.role_guid();
			my_memcopy_string(role_name, max_name_size, msg.role_name());
		}
	};

	struct ws2cs_sub_player_rename_item : packet_base
	{
		guid_64					role_guid;
		uint32					item_templete_id;
		
		ws2cs_sub_player_rename_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_sub_player_rename_item;
		}
		void to_proto(faith::ws2cs_proto::sub_player_rename_item& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_item_templete_id(item_templete_id);
		}
		void from_proto(const faith::ws2cs_proto::sub_player_rename_item& msg)
		{
			role_guid.server_64 = msg.role_guid();
			item_templete_id = msg.item_templete_id();
		}
	};

	struct cs2ws_sub_player_rename_item_end : packet_base
	{
		guid_64					role_guid;
		int32					result;

		cs2ws_sub_player_rename_item_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_sub_player_rename_item_end;
		}
	};

	struct ws2cs_req_relation_end : public packet_base
	{
		int32			relation_type;
		int32			self_index;
		guid_64			target_guid;
		ws2cs_req_relation_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_req_relation_end;
		}
		void to_proto(faith::ws2cs_proto::req_relation_end& msg)
		{
			msg.set_relation_type(relation_type);
			msg.set_self_index(self_index);
			msg.set_target_guid(target_guid.server_64);

		}
		void from_proto(const faith::ws2cs_proto::req_relation_end& msg)
		{
			relation_type = msg.relation_type();
			self_index = msg.self_index();
			target_guid.server_64 = msg.target_guid();
		}
	};

	struct dp2cs_connect_success : public faith::packet_base
	{
		int32		connect_success;
		dp2cs_connect_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_send_connect_success;
		}
	};
	struct cs2dp_save_person_information_to_db : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_role_person_information person_info;
		int32 save_type_ex;
		int32 data_num;
		cs2dp_save_person_information_to_db()
		{
			data_num = 1;
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_person_information_to_db;
		}
	};
	struct cs2dp_get_person_information : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		cs2dp_get_person_information()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_get_person_information;
		}
	};
	struct dp2cs_get_person_information : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_role_person_information person_info;
		dp2cs_get_person_information()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_person_information;
		}
	};
	struct cs2dp_get_other_person_information : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		guid_64 target_guid;
		bool is_ask_player_base_info;
		cs2dp_get_other_person_information()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_get_other_person_information;
		}
	};
	struct dp2cs_get_other_person_information : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		guid_64 target_guid;
		s_role_person_information person_info;
		bool is_ask_player_base_info;
		dp2cs_get_other_person_information()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_person_information;
		}
	};

	struct cs2ws_sync_person_information : public faith::packet_base
	{
		guid_64 role_guid;
		s_role_person_information person_info;
		cs2ws_sync_person_information()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_sync_person_information;
		}
	};
	struct cs2ws_delete_person_information : public faith::packet_base
	{
		guid_64 role_guid;
		cs2ws_delete_person_information()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_delete_person_information;
		}
	};
	struct cs2ws_get_person_couple : public faith::packet_base
	{
		guid_64 role_guid;
		guid_64 target_guid;
		cs2ws_get_person_couple()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_get_person_couple;
		}
	};
	struct dp2cs_get_role_competition : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_role_competition_info competition_info;
		dp2cs_get_role_competition()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_role_competition;
		}
	};

	struct cs2dp_save_role_competition_to_db : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_role_competition_info competition_info;
		int32 save_type_ex;
		cs2dp_save_role_competition_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_role_competition_to_db;
		}
	};

	//排行榜等加载其他人的捏脸信息
	struct dp2cs_get_other_player_info_appearance_end : public packet_base
	{
		guid_64								role_guid;
		guid_64								target_guid;
		int32								data_info;

		dp2cs_get_other_player_info_appearance_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_other_player_info_appearance_end;
		}
	};

	struct dp2cs_get_role_dragontrip : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_dragontrip_info dragontrip_info;
		dp2cs_get_role_dragontrip()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_role_dragontrip;
		}
	};

	struct cs2dp_save_role_dragontrip_to_db : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_dragontrip_info dragontrip_info;
		int32 save_type_ex;
		cs2dp_save_role_dragontrip_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_role_dragontrip_to_db;
		}
	};

	struct dp2cs_get_role_skytreasure : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		int32 data_num;
		s_skytreasure_info skytreasure_data_list[faith::max_skytreasure_num];
		dp2cs_get_role_skytreasure()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_role_skytreasure;
		}
	};

	struct cs2dp_save_role_skytreasure_to_db : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_skytreasure_info skytreasure_info;
		int32 save_type_ex;
		cs2dp_save_role_skytreasure_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_role_skytreasure_to_db;
		}
	};

	struct dp2cs_get_role_starark : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_starark_info starark_info;
		dp2cs_get_role_starark()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_role_starark;
		}
	};

	struct cs2dp_save_role_starark_to_db : public faith::packet_base
	{
		guid_64 role_guid;
		int32 unit_array_index;
		s_starark_info starark_info;
		int32 save_type_ex;
		cs2dp_save_role_starark_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_role_starark_to_db;
		}
	};

	struct cs2dp_save_time_feed_back_to_db : public faith::packet_base
	{
		guid_64						role_guid;
		int32						save_type_ex;
		int32						unit_array_index;
		int32						date_num;
		s_time_feed_back_db_info	info_list[time_feed_back_to_db_num];

		cs2dp_save_time_feed_back_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_time_feed_back_to_db;
		}
	};

	struct dp2cs_get_time_feed_back_to_db_end : public faith::packet_base
	{
		guid_64						role_guid;
		int32						unit_array_index;
		int32						date_num;
		s_time_feed_back_db_info	info_list[time_feed_back_to_db_num];

		dp2cs_get_time_feed_back_to_db_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_time_feed_back_to_db_end;
		}
	};


	struct cs2dp_save_time_limit_gift_to_db : public faith::packet_base
	{
		guid_64						role_guid;
		int32						save_type_ex;
		int32						unit_array_index;
		int32						date_num;
		s_time_limit_gift_db_info	info_list[time_limit_gift_db_num];

		cs2dp_save_time_limit_gift_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_time_limit_gift_to_db;
		}
	};

	struct dp2cs_get_time_limit_gift_to_db_end : public faith::packet_base
	{
		guid_64						role_guid;
		int32						unit_array_index;
		int32						date_num;
		s_time_limit_gift_db_info	info_list[time_limit_gift_db_num];

		dp2cs_get_time_limit_gift_to_db_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_time_limit_gift_to_db_end;
		}
	};

	struct cs2dp_save_subscribe_daily_info_to_db : public faith::packet_base
	{
		guid_64						role_guid;
		int32						save_type_ex;
		int32						unit_array_index;
		int32						date_num;
		s_subscribe_daily_db_info	info_list[time_limit_gift_db_num];

		cs2dp_save_subscribe_daily_info_to_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_save_subscribe_daily_info_to_db;
		}
	};

	struct dp2cs_get_subscribe_daily_info_to_db_end : public faith::packet_base
	{
		guid_64						role_guid;
		int32						unit_array_index;
		int32						date_num;
		s_subscribe_daily_db_info	info_list[time_limit_gift_db_num];

		dp2cs_get_subscribe_daily_info_to_db_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_get_subscribe_daily_info_to_db_end;
		}
	};
	
#pragma pack(pop)

}

#endif 
