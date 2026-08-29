/********************************************************************
  created: 2014/07/16
  created: 16:7:2014 20:31
  file base: game
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _FAITH_GAME_HPP_
#define _FAITH_GAME_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/login_def.hpp"
#include "Logic/team_def.hpp"
#include "Logic/transfer_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/mail_def.hpp"
#include "Logic/chat_def.hpp"
#include "Logic/arena_def.hpp"
#include "Logic/ranking_def.hpp"
#include "Logic/legion_def.hpp"
#include "Logic/map_def.hpp"
#include "Logic/best_record_def.hpp"
#include "Logic/gm_order_def.hpp"
#include "Logic/title_def.hpp"
#include "Logic/time_limit_activity_def.hpp"
#include "internet/ws2cs.pb.h"
#include "internet/cs2ws.pb.h"
#include "internet/error.pb.h"

namespace faith 
{
#pragma pack(push,1)

	enum
	{
		// EnterGame
		e_msg_index_fep2ws_enter_game = e_msg_base_entergame,
		e_msg_index_fep2c_enter_game,
		e_msg_index_ws2fep_enter_game,
		e_msg_index_ws2cs_time_out,
		e_msg_index_ws2cs_reconnect_game,
		e_msg_index_cs2ws_reconnect_game,
		e_msg_index_ws2fep_reconnect_game,
		e_msg_index_ws2cs_enter_game,
		e_msg_index_cs2ws_enter_game,
		e_msg_index_cs2ws_role_info,
		e_msg_index_ws2cs_need_cross,
		e_msg_index_ws2cs_server_Info_arr,
		e_msg_index_ws2cs_server_info_all,
		e_msg_index_ws2cs_need_cross_activity,
		e_msg_index_ws2cs_need_cross_city_war,
		e_msg_index_ws2cs_need_cross_territory_war,
		e_msg_index_ws2cs_need_cross_world_boss,

		// EnterScene	
		e_msg_index_ws2cs_enter_scene,
		e_msg_index_cs2ws_enter_scene,
		e_msg_index_fep2ws_enter_scene,
		e_msg_index_ws2cs_server_config,
		e_msg_index_ws2cs_clear_server_config,
		// SyncTimes
		e_msg_index_fep2cs_sync_time_stamp,
		e_msg_index_cs2fep_sync_time_stamp,

		// LeaveGame
		e_msg_index_fep2ws_logout_game,
		e_msg_index_ws2fep_logout_game,

		// 跨cs
		e_msg_index_cs2ws_transfer_player,
		e_msg_index_fep2ws_transfer,
		e_msg_index_ws2cs_transfer_old,
		e_msg_index_cs2ws_transfer_old,
		e_msg_index_ws2cs_transfer_new,
		e_msg_index_cs2ws_transfer_new,
		e_msg_index_ws2fep_transfer,


		// data
		e_msg_index_cs2dp_load_data_online,
		e_msg_index_cs2dp_load_arena_player_data,

		e_msg_index_cs2ws_dispatch_msg,
		e_msg_index_cs2ws_batch_send,
		e_msg_index_cs2fep_aoi_msg,
		e_msg_index_ws2fep_broadcast_msg,

		e_msg_index_cs2ws_sync_char_data,
		e_msg_index_cs2ws_send_role_info_to_gm,

		// map
		e_msg_index_ws2cs_map_create,
		e_msg_index_ws2cs_map_delete,
		e_msg_index_ws2cs_map_double_line,
		e_msg_index_ws2cs_map_dynamic_params,
		e_msg_index_cs2ws_map_state,
		e_msg_index_ws2cs_transfer_to_map,

		// activity
		e_msg_index_cs2ws_pk_king_winner,
		e_msg_index_cs2ws_broken_sky_info_sync,
		e_msg_index_ws2cs_broken_sky_info_sync,
		e_msg_index_cs2ws_broken_sky_creat_boss,
		e_msg_index_ws2cs_broken_sky_creat_boss,
		e_msg_index_cs2ws_broken_sky_kill_boss,
		e_msg_index_ws2cs_broken_sky_kill_boss,
		e_msg_index_ws2cs_chest_arrivai_spawn_chest_wave,
		e_msg_index_cs2ws_pk_king_begin,
		e_msg_index_ws2cs_clear_pk_king_title,

		// team
		e_msg_index_ws2cs_set_player_team_info,
		e_msg_index_ws2cs_clear_player_team_info,
		e_msg_index_cs2ws_create_team,
		e_msg_index_ws2cs_join_team,
		e_msg_index_cs2ws_join_team,
		e_msg_index_cs2ws_change_team_type,
		e_msg_index_ws2cs_apply_to_join_team,
		e_msg_index_cs2ws_apply_to_join_team,
		e_msg_index_cs2ws_del_team_member,
		e_msg_index_cs2ws_team_game_over,
		e_msg_index_ws2cs_transfer_team_member_to_scene,
		e_msg_index_cs2ws_update_team_member_info_one,
		e_msg_index_cs2ws_update_team_member_pos_info_one,
		e_msg_index_ws2cs_team_common_check_in_cs,
		e_msg_index_cs2ws_team_common_check_in_cs,
		e_msg_index_ws2cs_sync_team_start_aim,
		e_msg_index_cs2ws_team_change_aim_info,
		e_msg_index_cs2ws_after_team_aim_finish_op,

		// legion
		e_msg_index_ws2cs_set_player_legion_info,
		e_msg_index_ws2cs_clear_player_legion_info,
		e_msg_index_ws2cs_create_legion,
		e_msg_index_cs2ws_create_legion_result,
		e_msg_index_ws2cs_donate_legion_money,
		e_msg_index_cs2ws_donate_legion_money,
		e_msg_index_ws2cs_donate_legion_item,
		e_msg_index_cs2ws_donate_legion_item,
		e_msg_index_ws2cs_enter_legion_boss_map,
		e_msg_index_cs2ws_kill_legion_boss,
		e_msg_index_ws2cs_get_legion_boss_award,
		e_msg_index_ws2cs_get_map_info_from_cs,
		e_msg_index_cs2ws_get_map_info_from_cs_end,
		e_msg_index_ws2cs_request_hold_banquet_end,
		e_msg_index_ws2cs_get_take_part_in_banquet_award,
		e_msg_index_ws2cs_begin_city_war,
		e_msg_index_ws2cs_enter_city_war_map,
		e_msg_index_ws2cs_city_war_operate,
		e_msg_index_cs2ws_set_city_occupation,
		e_msg_index_cs2ws_update_legion_member_info_one,
		e_msg_index_ws2cs_set_get_legion_boss_award_log,
		e_msg_index_ws2cs_get_construction_buff,
		e_msg_index_ws2cs_get_occupation_daily_award,
		e_msg_index_cs2ws_convenient_join_legion,
		e_msg_index_ws2cs_get_city_master_stuff,
		e_msg_index_ws2cs_del_city_master_stuff,
		e_msg_index_ws2cs_fire_bonfire,
		e_msg_index_cs2ws_fire_bonfire_ret,
		e_msg_index_ws2cs_add_fuel_to_bonfire,
		e_msg_index_cs2ws_add_fuel_to_bonfire_ret,
		e_msg_index_cs2ws_bonfire_dead,
		e_msg_index_cs2ws_legion_add_glory_glow,
		e_msg_index_ws2cs_legion_rm_recruit_cd,
		e_msg_index_cs2ws_legion_rm_recruit_cd_ret,
		e_msg_index_ws2cs_draw_bonfire_daily_rwd,
		e_msg_index_ws2cs_add_city_war_winner_stuff,
		e_msg_index_ws2cs_del_city_war_winner_stuff,
		e_msg_index_cs2ws_add_legion_event,
		e_msg_index_cs2ws_get_legion_contribution,
		e_msg_index_ws2cs_level_up_legion_skill,
		e_msg_index_cs2ws_add_legion_act_score,
		e_msg_index_cs2ws_settle_legion_act_rank,
		e_msg_index_cs2ws_legion_boss_damage_player,
		e_msg_index_ws2cs_get_bonus_award,
		e_msg_index_cs2ws_add_bonus_count,
		e_msg_index_cs2ws_legion_get_bonus_award_end,	
		e_msgindex_ws2cs_legion_answer_question_right,
		e_msgindex_cs2ws_legion_warehouse_operation,
		e_msgindex_ws2cs_legion_warehouse_operation,
		e_msg_index_cs2ws_invite_to_join_legion,
		e_msg_index_cs2ws_apply_to_join_legion,
		e_msg_index_ws2cs_set_legion_average_lv_to_map,
		e_msg_index_ws2cs_confirm_change_legion_name,
		e_msg_index_cs2ws_confirm_change_legion_name,
		e_msg_index_ws2cs_rem_item_change_legion_name_care,
		e_msg_index_ws2cs_change_gate_legion_name,
		e_msg_index_ws2cs_set_legion_guid_to_map,
		e_msg_index_ws2cs_transfer_all_player_this_map,

		// best_record
		e_msg_index_cs2ws_set_best_record,

		//arena
		e_msg_index_cs2ws_req_get_arena_rank,
		e_msg_index_ws2cs_rep_get_arena_rank,
		e_msg_index_cs2ws_req_challenge,
		e_msg_index_ws2cs_req_challenge,
		e_msg_index_cs2ws_req_challenge_list,
		e_msg_index_ws2cs_req_challenge_list,
		e_msg_index_cs2ws_first_three,
		e_msg_index_ws2cs_first_three,
		e_msg_index_cs2ws_chellenge_over,
		e_msg_index_ws2cs_resp_fresh_challenge_time,
		e_msg_index_cs2ws_arena_sync_gs_value,

		//ranking
		e_msgindex_cs2ws_req_worship_player,
		e_msgindex_ws2cs_req_worship_player_end,
		e_msgindex_ws2cs_sync_first_rank_end,
		e_msgindex_ws2cs_sync_server_avg_lv,
		e_msgindex_cs2ws_sync_ranking_info,
		e_msgindex_cs2ws_sync_vip_level_data,
		e_msgindex_cs2ws_req_service_rank_info,
		e_msgindex_ws2cs_req_service_rank_info_end,

		//e_msgindex_ws2ws_req_gs_ranking,
		e_msgindex_ws2ws_res_gs_ranking,
		e_msgindex_ws2ws_req_all_gs_ranking,
		e_msgindex_ws2ws_res_all_gs_ranking,
		e_msgindex_ws2ws_req_get_my_rank_func,
		e_msgindex_ws2ws_res_get_my_rank_func,
		e_msgindex_ws2ws_req_time_limit_rank_info,
		e_msgindex_ws2ws_res_time_limit_rank_info,
		e_msgindex_ws2ws_get_oracle_trial_info,
		e_msgindex_ws2ws_send_oracle_ranking_notice,
		e_msgindex_ws2ws_get_ranking_list_error,
		// mail
		e_msgindex_cs2ws_send_mail,
		e_msgindex_ws2cs_recive_mail,
		e_msgindex_cs2ws_mail_send_to_all_player,

		//globel message & chat
		e_msgindex_cs2ws_send_globel_message,
		e_msgindex_ws2cs_receive_globel_message,

		e_msgindex_cs2ws_send_notice,
		e_msgindex_ws2cs_receive_notice,

		e_msgindex_cs2ws_role_right,
		e_msgindex_ws2cs_recharge,
		e_msgindex_cs2ws_recharge_end,
		e_msgindex_cs2ws_recharge_end_send_mail,
		e_msgindex_ws2cs_role_info_change,

		//store
		e_msgindex_cs2ws_buy_good,

		//time_limit_activity
		e_msg_index_cs2ws_time_limit_activity_common_data_sync,
		e_msg_index_ws2ws_time_limit_activity_common_data_sync,
		e_msg_index_ws2ws_time_limit_activity_common_data_sync_end,

		e_msg_index_cs2ws_check_time_limit_activity_common_data,
		e_msg_index_ws2cs_check_time_limit_activity_common_data,
		e_msg_index_ws2cs_set_time_limit_activity_pause_flag,
		e_msg_index_ws2ws_check_time_limit_activity_common_data,
		e_msg_index_ws2ws_check_time_limit_activity_common_data_end,

		e_msg_index_ws2ws_get_time_limit_activity_common_data_to_gate,
		e_msg_index_ws2ws_get_time_limit_activity_common_data_to_ws,

		//gm指令专用
		e_msgindex_cs2ws_gm_order,
		e_msg_index_ws2cs_del_unit_item,
		e_msg_index_ws2cs_change_unit_vip,
		e_msg_index_ws2cs_del_unit_specified_item,
		e_msgindex_ws2cs_spawn_npc,
		e_msgindex_ws2cs_add_guide_record,
		e_msgindex_ws2cs_modify_money,
		

		//server_attr_val
		e_msgindex_ws2dp_load_server_attr_val,
		e_msgindex_dp2ws_load_server_attr_val_end,
		e_msgindex_ws2dp_save_server_attr_val,

		//server_chat
		e_msgindex_ws2ws_chat_server_legion,
		e_msgindex_ws2ws_chat_server_all_member,

		e_msgindex_cs2fep_in_game,
		e_msgindex_fep2cs_in_game,

		//cross_server
		e_msgindex_ws2ws_kick_player,
		e_msgindex_ws2ws_player_legion_info,
		e_msgindex_ws2ws_send_cross_proto_msg,
		e_msgindex_ws2ws_cross_server_time,

		//save online time
		e_msgindex_ws2dp_save_account_online_time,
		e_msgindex_cs2fep_month_recharge_num,

		e_msg_index_ws2cs_set_ladder_world_level,
		e_msg_index_ws2cs_is_need_refresh_cs_daily_info_at_zero_hour,
	};

	/************************************************************************/
	/*           EnterGame                                                  */
	/************************************************************************/
	struct fep2ws_enter_game : public packet_base
	{ 
		ui64					charindex_for_play;		//	character index
		xchar					account[max_account_length + 1];
		s_client_uid			client_uid;

		fep2ws_enter_game()
		{
			memset(this, 0, sizeof(*this));
			wheader				= e_msg_index_fep2ws_enter_game;
		}
	};

	struct ws2fep_enter_game : public packet_base
	{
		e_error_code				e_result;
		s_client_uid				client_uid;
		uint32						cellserver_id;
		int32						cs_array_index;
		xchar						account[max_account_length + 1];
		int32						server_type;
		ws2fep_enter_game()
		{
			wheader		= e_msg_index_ws2fep_enter_game;
			e_result	= e_error_code_success;
			cellserver_id = 0;
			cs_array_index = 0;
			memset(account, 0, sizeof(account));
			server_type = 0;
		}
	};
	struct ws2fep_reconnect_game : public packet_base
	{
		s_client_uid			client_uid;
		xchar					account[max_account_length + 1];
		int32					cellserver_id;
		int32					array_index;
		int32					reconnect_res;
		ws2fep_reconnect_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2fep_reconnect_game;
		}
	};
	struct ws2cs_time_out : public packet_base
	{
		guid_64					role_guid;
		int32					cs_array_index;
		ws2cs_time_out()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_time_out;
		}
	};
	struct ws2cs_reconnect_game : public packet_base
	{
		s_client_uid			client_uid;
		guid_64					role_guid;
		int32					cur_map_id;
		int32					cur_map_line_id;
		ws2cs_reconnect_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_reconnect_game;
		}
	};
	struct cs2ws_reconnect_game : public packet_base
	{
		s_client_uid			client_uid;
		guid_64					role_guid;
		int32					reconnect_res;
		cs2ws_reconnect_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_reconnect_game;
		}
	};
	struct ws2cs_enter_game : public packet_base
	{
		s_client_uid			client_uid;
		xchar					account[max_account_length + 1];
		guid_64					role_guid;
		guid_64					up_role_guid;
		int32					exp_level;
		int32					server_id;
		uint32					cross_callback_val[8];
		int32					login_type;
		guid_64					map_guid;
		int32					line_id;
		int32					war_index;
		login_fixed_data		fixed_data;
		int32					is_send_chat;
		int64					worship_value;
		bool					is_city_master;
		bool					m_is_city_war_winner_legion_member;
		bool					is_already_login;
		bool					bflag_loading_title[faith::e_title_type_max];
		guid_64					couple_guid;
		xchar					couple_name[faith::max_name_size + 1];
		s_player_team_info		team_info;
		s_player_legion_info	legion_info;
		int64					login_time;
		ws2cs_enter_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_enter_game;
		}
		bool to_proto(faith::ws2cs_proto::enter_game & msg)
		{
			msg.set_client_uid(client_uid.fep_uid_64);
			msg.set_account(account);
			msg.set_role_guid(role_guid.server_64);
			msg.set_up_role_guid(up_role_guid.server_64);
			msg.set_exp_level(exp_level);
			msg.set_server_id(server_id);
			for (int32 i = 0 ; i< 8; i++)
			{
				msg.add_cross_callback_val(cross_callback_val[i]);
			}
			msg.set_login_type(login_type);
			msg.set_map_guid(map_guid.server_64);
			msg.set_line_id(line_id);
			msg.set_war_index(war_index);
			faith::st_proto::st_login_fixed_data* st_login_ptr = msg.mutable_fixed_data();
			if (st_login_ptr == nullptr)
			{
				return false;
			}
			fixed_data.to_proto(st_login_ptr);
			msg.set_is_send_chat(is_send_chat);
			msg.set_worship_value(worship_value);
			msg.set_is_city_master(is_city_master);
			msg.set_m_is_city_war_winner_legion_member(m_is_city_war_winner_legion_member);
			msg.set_is_already_login(is_already_login);
			for (int32 i = 0; i <faith::e_title_type_max; i++)
			{
				msg.add_bflag_loading_title(bflag_loading_title[i]);
			}
			msg.set_couple_guid(couple_guid.server_64);
			msg.set_couple_name(couple_name);
			
			faith::st_proto::st_player_team_info *st_team_ptr = msg.mutable_team_info();
			if (st_team_ptr == nullptr)
			{
				return false;
			}
			team_info.to_proto(st_team_ptr);

			faith::st_proto::st_player_legion_info *st_legion_ptr = msg.mutable_legion_info();
			if (st_legion_ptr == nullptr)
			{
				return false;
			}
			legion_info.to_proto(st_legion_ptr);
			msg.set_login_time(login_time);
			return true;
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

		void from_proto(faith::ws2cs_proto::enter_game & msg)
		{
			client_uid.fep_uid_64 =	msg.client_uid();
			my_memcopy_string(account, max_account_length, msg.account());

			role_guid.server_64 = msg.role_guid();
			up_role_guid.server_64 = msg.up_role_guid();
			exp_level = msg.exp_level();
			server_id = msg.server_id();
			for (int32 i = 0; i < 8; i++)
			{
				cross_callback_val[i] = msg.cross_callback_val(i);
			}
			login_type =  msg.login_type();
			map_guid.server_64 = msg.map_guid();
			line_id = msg.line_id();
			war_index = msg.war_index();
			const faith::st_proto::st_login_fixed_data& st_login_ref = msg.fixed_data();
	
			fixed_data.from_proto(st_login_ref);
			is_send_chat = msg.is_send_chat();
			worship_value = msg.worship_value();
			is_city_master = msg.is_city_master();
			m_is_city_war_winner_legion_member = msg.m_is_city_war_winner_legion_member();
			is_already_login =  msg.is_already_login();
			for (int32 i = 0; i < faith::e_title_type_max; i++)
			{
				bflag_loading_title[i] = msg.bflag_loading_title(i);
			}
			couple_guid.server_64 = msg.couple_guid();
			my_memcopy_string(couple_name, max_name_size, msg.couple_name());


			const faith::st_proto::st_player_team_info& st_team_ref = msg.team_info();
	
			team_info.from_proto(st_team_ref);

		    const faith::st_proto::st_player_legion_info& st_legion_ref = msg.legion_info();
	
			legion_info.from_proto(st_legion_ref);
			login_time = msg.login_time();


		}
		
	};

	struct cs2ws_enter_game : public packet_base
	{
		enum
		{
			e_failed_unknow_error = 0,
			e_success,
			e_failed_repeat_player_id,
			e_failed_join_map,

		} eResult;

		s_client_uid						client_uid;
		player_info_enter_game				char_info_to_ws;
		int32								map_template_id;
		guid_64								map_guid;

		cs2ws_enter_game()
		{
			memset(this, 0, sizeof(*this));
			wheader								= e_msg_index_cs2ws_enter_game;
		}
	};

	struct cs2ws_role_info : public packet_base
	{
		guid_64							role_guid;
		e_role_info						role_index;
		int32							role_value;
		cs2ws_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_role_info;
		}
	};
	/************************************************************************/
	/*           EnterScene                                                 */
	/************************************************************************/
	struct ws2cs_enter_scene : public packet_base
	{
		guid_64					role_guid;
		int32					cs_array_index;
		ws2cs_enter_scene()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_enter_scene;
		}
	};

	struct ws2cs_server_config : public packet_base
	{
		s_game_info				game_info;
		bool					is_add;
		ws2cs_server_config()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_server_config;
		}
		bool to_proto(faith::ws2cs_proto::server_config& msg)
		{
			faith::st_proto::st_game_info *st_game_ptr = msg.mutable_game_info();
			if (st_game_ptr == nullptr)
			{
				return false;
			}
			game_info.to_proto(st_game_ptr);
			msg.set_is_add(is_add);
			return true;
		}
		void from_proto(faith::ws2cs_proto::server_config& msg)
		{
			const faith::st_proto::st_game_info & st_game_ref = msg.game_info();
	
			game_info.from_proto(st_game_ref);
			is_add = msg.is_add();

		}
	};

	struct ws2cs_server_clear_all_config : public packet_base
	{
		ws2cs_server_clear_all_config()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_clear_server_config;
		}
	};

	struct ws2cs_server_is_need_refresh_cs_daily_info_at_zero_hour : public packet_base 
	{
		ws2cs_server_is_need_refresh_cs_daily_info_at_zero_hour()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_is_need_refresh_cs_daily_info_at_zero_hour;
		}
	};

	struct ws2cs_server_need_cross : public packet_base
	{
		int32 need_begin_cross;
		e_need_server_cross gm_type;
		ws2cs_server_need_cross()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_need_cross;
		}
	};

	struct ws2cs_server_info_arr : public packet_base
	{
		int32			server_info_value;
		int32			server_info_type;
		ws2cs_server_info_arr()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_server_Info_arr;
		}
	};

	struct ws2cs_server_info_all : public packet_base
	{
		int32			server_info_arr[e_server_info_type_max];
		ws2cs_server_info_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_server_info_all;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&server_info_arr - (ULONG_PTR)&wheader;
			return (basic_len + e_server_info_type_max * sizeof(int32));
		}
	};

	struct ws2cs_server_need_cross_city_war : public packet_base
	{
		bool			need_begin_cross;
		ws2cs_server_need_cross_city_war()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_need_cross_city_war;
		}
	};
	struct ws2cs_need_cross_territory_war : public packet_base
	{
		bool			need_begin_cross;
		ws2cs_need_cross_territory_war()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_need_cross_territory_war;
		}
	};
	
	struct ws2cs_need_cross_world_boss : public packet_base
	{
		bool			need_begin_cross;
		ws2cs_need_cross_world_boss()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_need_cross_world_boss;
		}
	};
	
	struct cs2ws_enter_scene : public packet_base
	{
		enum e_enter_ret
		{
			e_failed_unknow_error = 0,
			e_enter_ret_success,
			e_enter_ret_player_err,	//玩家数据错误
			e_enter_ret_already_in,	//重复进入场景
		};

		e_enter_ret	eResult;
		guid_64		role_guid;

		cs2ws_enter_scene()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_enter_scene;
		}
	};
	const int32 FEP_SESSION_UID_MAX = 2000;
	const int32 FEP_DATA_MAX = 1000;
	struct cs2fep_aoi_msg :public packet_base
	{
		int16 header;
		int16 data_size;
		ui8 data[MAX_C2S_S2C_PACKAGE_SIZE];
		int16 data_num;
		s_client_uid client_uid[FEP_SESSION_UID_MAX];
		cs2fep_aoi_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2fep_aoi_msg;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&client_uid - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_client_uid));
		}
	};

	struct ws2fep_broadcast_msg : public packet_base
	{
		uint32	data_size;
		uint32	header;
		ui8		data[FEP_DATA_MAX];

		ws2fep_broadcast_msg()
		{
			memset(this, 0, sizeof(ws2fep_broadcast_msg));
			wheader = e_msg_index_ws2fep_broadcast_msg;
		}
	};

	/************************************************************************/
	/*           SyncTimes                                                  */
	/************************************************************************/
	struct fep2cs_synctimestamp : public packet_base
	{
		uint32					time_stamp;			// 客户端此时时间戳
		s_client_uid	client_uid;

		fep2cs_synctimestamp()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_fep2cs_sync_time_stamp;
		}
	};

	struct cs2fep_synctmestamp : public packet_base
	{
		uint32					time_stamp;			// 服务器此时时间戳
		s_client_uid	client_uid;

		cs2fep_synctmestamp()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_cs2fep_sync_time_stamp;
		}
	};

	/************************************************************************/
	/*             LeaveGame                                                */
	/************************************************************************/

	struct fep2ws_logout_game : public packet_base
	{
		s_client_uid	client_uid;
		fep2ws_logout_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_fep2ws_logout_game;
		}
	};

	struct ws2fep_logout_game : public packet_base
	{
		s_client_uid	client_uid;
		enum
		{
			e_failed_unknow_error = 0,
			e_success,
			e_failed,	
		}eResult;

		ws2fep_logout_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2fep_logout_game;
		}
	};


	/************************************************************************/
	/*                         Transfer                                     */
	/************************************************************************/

	struct cs2ws_tansfer_player :
		public packet_base
	{
		//s_client_uid client_uid;
		guid_64 role_guid;
		s_transfer_info transfer_info;
		s_unit_info role_unit_info;
		int32 dest_group_id;
		uint32 callbackVal[limit_call_back_val_count];

		cs2ws_tansfer_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_transfer_player;
		}
	};

	struct fep2ws_transfer : public packet_base
	{
		s_client_uid  client_uid;
		uint32					switch_cs_id;
		fep2ws_transfer()
		{
			memset(this,0,sizeof(*this));
			wheader = e_msg_index_fep2ws_transfer;
		}
	};


	struct ws2cs_transfer_old : public packet_base
	{
		s_client_uid  client_uid;
		ws2cs_transfer_old()
		{
			memset(this,0,sizeof(*this));
			wheader = e_msg_index_ws2cs_transfer_old;
		}
	};

	struct ws2cs_transfer_new : public packet_base
	{
		guid_64	role_guid;
		xchar		account[max_account_length + 1];

		ws2cs_transfer_new()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_transfer_new;
		}
	};

	//	玩家对象所有权转移后，WS 通知 FEP
	struct ws2fep_transfer : public packet_base
	{
		s_client_uid	client_uid;
		uint32					dest_cs_uid;
		bool					req_ack;
		i8						result;
		uint32					map_id;				// 新的地图id，由map_id判断为主世界的场景或副本场景
		s_map_pos				map_pos;			// 新的位置信息
		int32						time_stamp;			// 时间
		ui8						move_flags;			// 保留
		int32						position_id;

		ws2fep_transfer()
		{
			memset(this, 0, sizeof(*this));
			wheader= e_msg_index_ws2fep_transfer;
		}
	};

	/************************************************************************/
	/*                         data                                         */
	/************************************************************************/
	struct cs2dp_load_data_online : public packet_base
	{
		guid_64	role_guid;
		guid_64 up_role_guid;
		int32 unit_array_index;
		cs2dp_load_data_online()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2dp_load_data_online;
		}
	};
	struct cs2dp_load_arena_player : public packet_base
	{
		guid_64	role_guid;
		guid_64 map_guid;
		cs2dp_load_arena_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2dp_load_arena_player_data;
		}
	};

	/************************************************************************/
	/*                         map                                          */
	/************************************************************************/
	struct ws2cs_map_create : public packet_base
	{
		guid_64		map_guid;
		int32		line_id;
		int32		map_template_id;
		int32		map_all_time;
		ws2cs_map_create()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_map_create;
		}
	};
	struct ws2cs_map_delete : public packet_base
	{
		guid_64		map_guid;//  ͼguid
		ws2cs_map_delete()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_map_delete;
		}
	};	
	struct ws2cs_map_double_line : public packet_base
	{
		guid_64		map_guid;
		int32		is_double_line;
		ws2cs_map_double_line()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_map_double_line;
		}
	};
	struct cs2ws_map_state : public packet_base
	{
		guid_64		map_guid;//地图guid
		e_map_state	map_state;
		cs2ws_map_state()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_map_state;
		}
	};

	struct ws2cs_transfer_to_map : public packet_base
	{
		guid_64	role_guid;
		int32	map_template_id;
		guid_64	map_guid;
		int32	war_idx;
		int32	server_id;

		ws2cs_transfer_to_map()
		{
			memset(this, 0, sizeof(ws2cs_transfer_to_map));
			wheader = e_msg_index_ws2cs_transfer_to_map;
		}
	};

	struct cs2ws_set_big_player : public packet_base
	{
		guid_64	winner_guid;
		int32 big_player_type;

		cs2ws_set_big_player()
		{
			wheader = e_msg_index_cs2ws_pk_king_winner;
			winner_guid.clear_data();
			big_player_type = 0;
		}
	};

	struct cs2ws_pk_king_begin : public packet_base
	{
		bool begin;

		cs2ws_pk_king_begin()
		{
			wheader = e_msg_index_cs2ws_pk_king_begin;
			begin = false;
		}
	};

	struct ws2cs_clear_big_player_title : public packet_base
	{
		guid_64		role_guid;
		int32		title_type;
		ws2cs_clear_big_player_title()
		{
			wheader = e_msg_index_ws2cs_clear_pk_king_title;
			title_type = 0;
		}
	};

	struct ws2cs_map_dynamic_params : public packet_base
	{
		guid_64		map_guid;
		int32		dynamic_params_map_type;	//仅用于区分通用发送和功能发送
		int32		dynamic_params_int[kmax_transfer_dynamic_int_num];
		xchar		dynamic_params_name[max_name_size + 1];

		ws2cs_map_dynamic_params()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_map_dynamic_params;
		}
		void set_dynamic_name(xstring _name)
		{
			memset(dynamic_params_name, 0, sizeof(dynamic_params_name));
			memcpy(dynamic_params_name, _name.c_str(), _name.size() > sizeof(dynamic_params_name) ? sizeof(dynamic_params_name) : _name.size());
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
		void to_proto(faith::ws2cs_proto::map_dynamic_params& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			msg.set_dynamic_params_map_type(dynamic_params_map_type);
			for (int32 i = 0; i < kmax_transfer_dynamic_int_num; i++)
			{
				msg.add_dynamic_params_int(dynamic_params_int[i]);
			}
			msg.set_dynamic_params_name(dynamic_params_name);
		}
		void from_proto(faith::ws2cs_proto::map_dynamic_params& msg)
		{
			map_guid.server_64 = msg.map_guid();
			dynamic_params_map_type = msg.dynamic_params_map_type();
			for (int32 i = 0; i < kmax_transfer_dynamic_int_num; i++)
			{
				dynamic_params_int[i] = msg.dynamic_params_int(i);
			}
			my_memcopy_string(dynamic_params_name, max_name_size, msg.dynamic_params_name());
		}
	};


	struct ws2cs_get_map_info_from_cs : public packet_base
	{
		guid_64	map_guid;
		guid_64	role_guid;
		int32	map_template_id;
		int32	map_info[map_info_arr_max_num];

		ws2cs_get_map_info_from_cs()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_get_map_info_from_cs;
		}
		void to_proto(faith::ws2cs_proto::get_map_info_from_cs& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			msg.set_role_guid(role_guid.server_64);
			msg.set_map_template_id(map_template_id);
			for (int32 i = 0; i < map_info_arr_max_num; i++)
			{
				msg.add_map_info(map_info[i]);
			}
		}
		void from_proto(const faith::ws2cs_proto::get_map_info_from_cs& msg)
		{
			map_guid.server_64 = msg.map_guid();
			role_guid.server_64 = msg.role_guid();
			map_template_id = msg.map_template_id();
			for (int32 i = 0; i < map_info_arr_max_num; i++)
			{
				map_info[i] = msg.map_info(i);
			}
		}
	};

	struct cs2ws_get_map_info_from_cs_end : public packet_base
	{
		guid_64	role_guid;
		int32	map_template_id;
		int64	map_info[map_info_arr_max_num];

		cs2ws_get_map_info_from_cs_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_get_map_info_from_cs_end;
		}
	};

	/************************************************************************/
	/*                         activity                                     */
	/************************************************************************/

	struct cs2ws_broken_sky_info_sync : public packet_base
	{
		int32	map_template_id;
		guid_64 broken_sky_inst_guid;
		int64	damage;

		int32	record_num;
		score_record top_records[sync_damage_rank_count];

		cs2ws_broken_sky_info_sync()
		{
			memset(this, 0, sizeof(cs2ws_broken_sky_info_sync));
			wheader = e_msg_index_cs2ws_broken_sky_info_sync;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&top_records - (ULONG_PTR)&wheader;
			return (basic_len + record_num * sizeof(score_record));
		}
	};

	struct ws2cs_broken_sky_info_sync : public packet_base
	{
		int32	map_template_id;
		guid_64 broken_sky_inst_guid;
		int64	damage;

		ws2cs_broken_sky_info_sync()
		{
			memset(this, 0, sizeof(ws2cs_broken_sky_info_sync));
			wheader = e_msg_index_ws2cs_broken_sky_info_sync;
		}
	};

	struct cs2ws_broken_sky_creat_boss : public packet_base
	{
		guid_64 from_map_guid;

		cs2ws_broken_sky_creat_boss()
		{
			memset(this, 0, sizeof(cs2ws_broken_sky_creat_boss));
			wheader = e_msg_index_cs2ws_broken_sky_creat_boss;
		}
	};

	struct ws2cs_broken_sky_creat_boss : public packet_base
	{
		int32	boss_level;
		int64	all_damage;
		guid_64 from_map_guid;

		ws2cs_broken_sky_creat_boss()
		{
			memset(this, 0, sizeof(ws2cs_broken_sky_creat_boss));
			wheader = e_msg_index_ws2cs_broken_sky_creat_boss;
		}
	};

	struct cs2ws_broken_sky_kill_boss : public packet_base
	{
		int32	map_template_id;
		int32	killer_unit_index;
		guid_64	killer_guid;

		cs2ws_broken_sky_kill_boss()
		{
			memset(this, 0, sizeof(cs2ws_broken_sky_kill_boss));
			wheader = e_msg_index_cs2ws_broken_sky_kill_boss;
		}
	};

	struct ws2cs_broken_sky_kill_boss : public packet_base
	{
		int32	map_template_id;
		int32	killer_unit_index;
		guid_64	killer_guid;

		//guid_64 top_record_roles[broken_sky_map::sync_damage_rank_count];

		ws2cs_broken_sky_kill_boss()
		{
			memset(this, 0, sizeof(ws2cs_broken_sky_kill_boss));
			wheader = e_msg_index_ws2cs_broken_sky_kill_boss;
		}
	};

	struct ws2cs_chest_arrivai_spawn_chest_wave : public packet_base
	{
		int32 chest_disappear_time; // 宝箱消失的时间

		ws2cs_chest_arrivai_spawn_chest_wave()
		{
			memset(this, 0, sizeof(ws2cs_chest_arrivai_spawn_chest_wave));
			wheader = e_msg_index_ws2cs_chest_arrivai_spawn_chest_wave;
		}
	};

	/************************************************************************/
	/*                         team                                         */
	/************************************************************************/

	struct ws2cs_set_player_team_info : public packet_base
	{
		guid_64				role_guid;
		s_player_team_info	player_team_info;

		ws2cs_set_player_team_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_set_player_team_info;
		}

		bool to_proto(faith::ws2cs_proto::set_player_team_info &msg)
		{
			msg.set_role_guid(role_guid.server_64);
			faith::st_proto::st_player_team_info* st_team_ptr = msg.mutable_player_team_info();
			if (st_team_ptr == nullptr)
			{
				return false;
			}
			player_team_info.to_proto(st_team_ptr);
			return true;
		}
		void  from_proto(const faith::ws2cs_proto::set_player_team_info& msg)
		{
			role_guid.server_64 = msg.role_guid();
			const faith::st_proto::st_player_team_info& st_team_ref = msg.player_team_info();
			player_team_info.from_proto(st_team_ref);
		}
	};

	struct ws2cs_clear_player_team_info : public packet_base
	{
		guid_64		role_guid;

		ws2cs_clear_player_team_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_clear_player_team_info;
		}
		void to_proto(faith::ws2cs_proto::clear_player_team_info& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::clear_player_team_info& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct cs2ws_create_team : public packet_base
	{
		e_team_type			team_type;
		int32				team_sub_type_id;
		int32				required_fighting_power;
		int32				required_level;
		bool				auto_accept_new_member;
		int32				team_one_stop_flags;
		s_team_member_info	captain_info;

		cs2ws_create_team()
		{
			memset(this, 0, sizeof(*this));
			team_one_stop_flags = -1;
			wheader = e_msg_index_cs2ws_create_team;
		}
	};

	struct ws2cs_join_team : public packet_base
	{
		guid_64 role_guid;
		guid_64 team_guid;

		ws2cs_join_team()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_join_team;
		}
	};

	struct cs2ws_join_team : public packet_base
	{
		guid_64				team_guid;
		s_team_member_info	member_info;

		cs2ws_join_team()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_join_team;
		}
	};

	struct cs2ws_change_team_type : public packet_base
	{
		int32 one_stop_;
		int32 sub_temp_id;
		guid_64 team_guid;
		guid_64 player_guid;
		bool is_auto_match;
		bool has_auto_match;
		e_team_type  team_type;
		bool has_team_type;
		bool has_team_sub_type_id;
		int32 limit_fight_power;
		int32 limit_level;
		s_team_member_info team_member_info[max_team_member_num];
		cs2ws_change_team_type()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_change_team_type;
		}
	};


	
	struct ws2cs_apply_to_join_team : public packet_base
	{
		guid_64		role_guid;
		guid_64		team_guid;
		e_team_type	team_type_id;
		int32		team_sub_type_id;

		ws2cs_apply_to_join_team()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_apply_to_join_team;
		}
		void to_proto(faith::ws2cs_proto::apply_to_join_team& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_team_guid(team_guid.server_64);
			msg.set_team_type_id(team_type_id);
			msg.set_team_sub_type_id(team_sub_type_id);
		}
		void from_proto(const faith::ws2cs_proto::apply_to_join_team& msg)
		{
			role_guid.server_64 = msg.role_guid();
			team_guid.server_64 = msg.team_guid();
			team_type_id = (e_team_type)msg.team_type_id();
			team_sub_type_id = msg.team_sub_type_id();

		}
	};

	struct cs2ws_apply_to_join_team : public packet_base
	{
		guid_64				team_guid;
		s_team_member_info	applicant_info;

		cs2ws_apply_to_join_team()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_apply_to_join_team;
		}
	};

	struct cs2ws_del_team_member : public packet_base
	{
		guid_64	team_guid;
		guid_64	member_guid;

		cs2ws_del_team_member()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_del_team_member;
		}
	};

	struct cs2ws_team_game_over : public packet_base
	{
		guid_64 raid_map_guid;

		cs2ws_team_game_over()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_team_game_over;
		}
	};

	struct ws2cs_transfer_team_member_to_scene : public packet_base
	{
		guid_64		team_guid;
		guid_64		role_guid;
		int32		map_template_id;
		guid_64		map_guid;

		ws2cs_transfer_team_member_to_scene()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_transfer_team_member_to_scene;
		}
		void to_proto( faith::ws2cs_proto::transfer_team_member_to_scene& msg)
		{
			msg.set_team_guid(team_guid.server_64);
			msg.set_role_guid(role_guid.server_64);
			msg.set_map_template_id(map_template_id);
			msg.set_map_guid(map_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::transfer_team_member_to_scene& msg)
		{
			team_guid.server_64 = msg.team_guid();
			role_guid.server_64 = msg.role_guid();
			map_template_id = msg.map_template_id();
			map_guid.server_64 = msg.map_guid();
		}
	};

	struct cs2ws_update_team_member_info_one : public packet_base
	{
		guid_64	member_guid;
		int32	info_index;
		int32	info_value;

		cs2ws_update_team_member_info_one()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_update_team_member_info_one;
		}
	};

	struct cs2ws_update_team_member_pos_info_one : public packet_base
	{
		guid_64            team_guid;
		guid_64			   role_guid;
		int32			   pos_x;
		int32			   pos_y;
		int32			   pos_z;

		cs2ws_update_team_member_pos_info_one()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_update_team_member_pos_info_one;
		}
	};
	
	struct ws2cs_team_common_check_in_cs : public packet_base
	{
		guid_64						role_guid;
		guid_64						team_guid;
		e_team_type					team_type_id;
		int32						team_sub_type_id;
		e_team_common_check_invoker check_invoker;

		ws2cs_team_common_check_in_cs()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_team_common_check_in_cs;
		}
		void to_proto(faith::ws2cs_proto::team_common_check_in_cs& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_team_guid(team_guid.server_64);
			msg.set_team_type_id(team_type_id);
			msg.set_team_sub_type_id(team_sub_type_id);
			msg.set_check_invoker(check_invoker);
		}
		void from_proto(const faith::ws2cs_proto::team_common_check_in_cs& msg)
		{
			role_guid.server_64 = msg.role_guid();
			team_guid.server_64 = msg.team_guid();
			team_type_id = (e_team_type)msg.team_type_id();
			team_sub_type_id = msg.team_sub_type_id();
			check_invoker = (e_team_common_check_invoker)msg.check_invoker();
		}
	};

	struct cs2ws_team_common_check_in_cs : public packet_base
	{
		guid_64						role_guid;
		guid_64						team_guid;
		e_team_type					team_type_id;
		int32						team_sub_type_id;
		e_team_common_check_invoker check_invoker;
		e_error_code				map_check_ret;
		int32						own_one_stop_flags;
		bool						is_match_one_stop;

		cs2ws_team_common_check_in_cs()
		{
			memset(this, 0, sizeof(*this));
			own_one_stop_flags = -1; //无效值
			wheader = e_msg_index_cs2ws_team_common_check_in_cs;
		}
	};
	
	struct ws2cs_sync_team_start_aim : public packet_base
	{
		guid_64						role_guid;

		ws2cs_sync_team_start_aim()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_sync_team_start_aim;
		}
		void to_proto(faith::ws2cs_proto::sync_team_start_aim& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::sync_team_start_aim& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct cs2ws_team_change_aim_info : public packet_base
	{
		guid_64						role_guid;
		e_team_type					team_type_id;
		int32						team_sub_type_id;

		cs2ws_team_change_aim_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_team_change_aim_info;
		}
	};

	struct cs2ws_after_team_aim_finish_op : public packet_base
	{
		guid_64						role_guid;
		int32						map_check_ret;
		int32						role_one_stop_flags;

		cs2ws_after_team_aim_finish_op()
		{
			memset(this, 0, sizeof(*this));
			map_check_ret = -1;
			role_one_stop_flags = -1;
			wheader = e_msg_index_cs2ws_after_team_aim_finish_op;
		}
	};

	
	/************************************************************************/
	/*                         leigon                                       */
	/************************************************************************/

	struct cs2ws_loading_finish_get_overlord_legion_guid : public packet_base
	{

	};

	struct ws2cs_set_player_legion_info : public packet_base
	{
		guid_64					role_guid;
		s_player_legion_info	player_legion_info;
		bool					is_new_join_legion;

		ws2cs_set_player_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_set_player_legion_info;
			is_new_join_legion = false;
		}
		void to_proto(faith::ws2cs_proto::set_player_legion_info& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			faith::st_proto::st_player_legion_info* st_legion_ptr = msg.mutable_player_legion_info();
			if (st_legion_ptr == nullptr)
			{
				return;
			}
			player_legion_info.to_proto(st_legion_ptr);
			msg.set_is_new_join_legion(is_new_join_legion);
		}
		void from_proto(const faith::ws2cs_proto::set_player_legion_info& msg)
		{
			role_guid.server_64 = msg.role_guid();
			player_legion_info.from_proto(msg.player_legion_info());
			is_new_join_legion = msg.is_new_join_legion();
		}
	};

	struct ws2cs_clear_player_legion_info : public packet_base
	{
		guid_64	role_guid;

		ws2cs_clear_player_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_clear_player_legion_info;
		}
	};

	struct ws2cs_create_legion : public packet_base
	{
		guid_64					legion_guid;
		xchar					legion_name[max_name_size + 1];
		int32					legion_level;
		bool					auto_accept_new_member;
		s_legion_member_info	chief_info;

		ws2cs_create_legion()
		{
			memset(this, 0, sizeof(ws2cs_create_legion));
			wheader = e_msg_index_ws2cs_create_legion;
			legion_level = 1;
		}
		bool to_proto(faith::ws2cs_proto::create_legion& msg)
		{
			msg.set_legion_guid(legion_guid.server_64);
			msg.set_legion_name(legion_name);
			msg.set_legion_level(legion_level);
			msg.set_auto_accept_new_member(auto_accept_new_member);
			faith::st_proto::st_legion_member_info* st_member_ptr = msg.mutable_chief_info();
			if (st_member_ptr == nullptr)
			{
				return false;
			}
			chief_info.to_proto(st_member_ptr);
			return true;
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
		void from_proto(const faith::ws2cs_proto::create_legion& msg)
		{
			legion_guid.server_64 = msg.legion_guid();
			my_memcopy_string(legion_name, max_name_size, msg.legion_name());
			legion_level = msg.legion_level();
			auto_accept_new_member = msg.auto_accept_new_member();
			chief_info.from_proto(msg.chief_info());
		}
	};

	struct cs2ws_create_legion_result : public packet_base
	{
		guid_64			legion_guid;
		s_legion_member_info	chief_info;
		e_legion_error	error_code;

		cs2ws_create_legion_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_create_legion_result;
		}
	};

	struct cs2ws_invite_to_join_legion : public packet_base
	{
		s_client_uid	client_uid;
		s_client_uid	invite_client_uid;

		cs2ws_invite_to_join_legion()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_invite_to_join_legion;
		}
		int32 get_len()
		{
			return sizeof(cs2ws_invite_to_join_legion);
		}
	};

	struct cs2ws_apply_to_join_legion : public packet_base
	{
		guid_64			legion_guid;
		s_client_uid	client_uid;
		cs2ws_apply_to_join_legion()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_apply_to_join_legion;
		}
		int32 get_len()
		{
			return sizeof(cs2ws_apply_to_join_legion);
		}
	};

	struct ws2cs_donate_legion_money : packet_base
	{
		guid_64	role_guid;
// 		int32	legion_level;
		int32	donate_times;

		ws2cs_donate_legion_money()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_donate_legion_money;
		}
		void to_proto(faith::ws2cs_proto::donate_legion_money& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_donate_times(donate_times);
		}
		void from_proto(const faith::ws2cs_proto::donate_legion_money& msg)
		{
			role_guid.server_64 = msg.role_guid();
			donate_times = msg.donate_times();
		}
	};

	struct cs2ws_donate_legion_money : packet_base
	{
		s_client_uid	client_uid;
		int32			money_num;
		int32			battle_achievement;

		cs2ws_donate_legion_money()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_donate_legion_money;
		}
	};

	struct ws2cs_donate_legion_item : packet_base
	{
		guid_64	role_guid;
// 		int32	legion_level;
		int32	donate_item_index;
		int32	donate_item_num;
		
		ws2cs_donate_legion_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_donate_legion_item;
		}
		void to_proto(faith::ws2cs_proto::donate_legion_item& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_donate_item_index(donate_item_index);
			msg.set_donate_item_num(donate_item_num);
		}
		void from_proto(const faith::ws2cs_proto::donate_legion_item& msg)
		{
			role_guid.server_64 = msg.role_guid();
			donate_item_index = msg.donate_item_index();
			donate_item_num = msg.donate_item_num();
		}
	};

	struct cs2ws_donate_legion_item : packet_base
	{
		s_client_uid	client_uid;
		int32			item_index;
		int32			item_num;
		int32			money_num;
		int32			battle_achievement;

		cs2ws_donate_legion_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_donate_legion_item;
		}
	};

	struct ws2cs_enter_legion_boss_map : public packet_base
	{
		guid_64	role_guid;
		int32	boss_map_id;
		guid_64 boss_map_guid;

		ws2cs_enter_legion_boss_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_enter_legion_boss_map;
		}
		void to_proto(faith::ws2cs_proto::enter_legion_boss_map& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_boss_map_id(boss_map_id);
			msg.set_boss_map_guid(boss_map_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::enter_legion_boss_map& msg)
		{
			role_guid.server_64 = msg.role_guid();
			boss_map_id = msg.boss_map_id();
			boss_map_guid.server_64 = msg.boss_map_guid();
		}
	};

	struct cs2ws_kill_legion_boss : public packet_base
	{
		guid_64	role_guid;
		int32	map_template_id;
		xchar	killer_name[max_name_size + 1];

		cs2ws_kill_legion_boss()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_kill_legion_boss;
		}
	};

	struct cs2ws_legion_warehouse_operation : public packet_base
	{
		guid_64 role_guid;
		s_legion_warehouse_info legion_warehouse_info;
		int32 operation_type;
		int32 del_num;
		guid_64 del_array[max_legion_warehouse_info];
		s_legion_warehouse_log legion_warehouse_log;
		cs2ws_legion_warehouse_operation()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_legion_warehouse_operation;
		}
	};

	struct ws2cs_legion_warehouse_operation :public packet_base
	{
		int32 error_id;
		guid_64 role_guid;
		s_legion_warehouse_info legion_warehouse_info;
		int32 operation_type;
		ws2cs_legion_warehouse_operation()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_legion_warehouse_operation;
		}
	};


	struct cs2ws_get_legion_contribution : public packet_base
	{
		int32							member_num;
		s_legion_member_contribution	member_contribution[max_legion_member_num];
		cs2ws_get_legion_contribution()
		{
			memset(this, 0, sizeof(cs2ws_get_legion_contribution));
			wheader = e_msg_index_cs2ws_get_legion_contribution;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&member_contribution - (ULONG_PTR)&wheader;
			return (basic_len + member_num * sizeof(s_legion_member_contribution));
		}
	};

	struct ws2cs_level_up_legion_skill : public packet_base
	{
		guid_64		role_guid;
		int32		skill_template_id;
		int64		donate_contribution;

		ws2cs_level_up_legion_skill()
		{
			memset(this, 0, sizeof(ws2cs_level_up_legion_skill));
			wheader = e_msg_index_ws2cs_level_up_legion_skill;
		}
		void to_proto(faith::ws2cs_proto::level_up_legion_skill& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_skill_template_id(skill_template_id);
			msg.set_donate_contribution(donate_contribution);

		}
		void from_proto(const faith::ws2cs_proto::level_up_legion_skill& msg)
		{
			role_guid.server_64 = msg.role_guid();
			skill_template_id = msg.skill_template_id();
			donate_contribution = msg.donate_contribution();
		}
	};

	struct ws2cs_get_legion_boss_award : public packet_base
	{
		guid_64	role_guid;
		int32	boss_map_id;

		ws2cs_get_legion_boss_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_get_legion_boss_award;
		}
		void to_proto(faith::ws2cs_proto::get_legion_boss_award& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_boss_map_id(boss_map_id);
		}
		void from_proto(const faith::ws2cs_proto::get_legion_boss_award& msg)
		{
			role_guid.server_64 = msg.role_guid();
			boss_map_id = msg.boss_map_id();
		}
	};

	struct ws2cs_request_hold_banquet_end : public packet_base
	{
		guid_64			role_guid;
		e_banquet_level	banquet_level;

		ws2cs_request_hold_banquet_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_request_hold_banquet_end;
		}
	};

	struct ws2cs_get_take_part_in_banquet_award : public packet_base
	{
		guid_64			role_guid;
		e_banquet_level banquet_level;

		ws2cs_get_take_part_in_banquet_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_get_take_part_in_banquet_award;
		}
	};

	struct ws2cs_begin_city_war : public packet_base
	{
		guid_64					city_war_map_guid;
		int32					cur_terr_id;
		s_city_war_legion_score	join_war_legions[max_apply_city_war_legion_num];
		int32					cross_city_idex;
		ws2cs_begin_city_war()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_begin_city_war;
		}
		bool to_proto(faith::ws2cs_proto::begin_city_war& msg)
		{
			msg.set_city_war_map_guid(city_war_map_guid.server_64);
			msg.set_cur_terr_id(cur_terr_id);

			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				faith::st_proto::st_city_war_legion_score *st_city_war_ptr = msg.add_join_war_legions();
				if (st_city_war_ptr == nullptr)
				{
					return false;
				}
				join_war_legions[i].to_proto(st_city_war_ptr);
			}
			msg.set_cross_city_idex(cross_city_idex);
			return true;
		}
		void from_proto(const faith::ws2cs_proto::begin_city_war& msg)
		{
			city_war_map_guid.server_64 = msg.city_war_map_guid();
			cur_terr_id = msg.cur_terr_id();
			for (int32 i = 0; i < msg.join_war_legions_size(); i++)
			{
				const faith::st_proto::st_city_war_legion_score& st_city_war_ref = msg.join_war_legions(i);
				join_war_legions[i].from_proto(st_city_war_ref);
			}
			cross_city_idex = msg.cross_city_idex();
		}
	};

	struct ws2cs_enter_city_war_map : public packet_base
	{
		guid_64	role_guid;
		guid_64 map_guid;
		int32	war_index;

		ws2cs_enter_city_war_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_enter_city_war_map;
		}
		void to_proto(faith::ws2cs_proto::enter_city_war_map& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_map_guid(map_guid.server_64);
			msg.set_war_index(war_index);
		}
		void from_proto(const faith::ws2cs_proto::enter_city_war_map& msg)
		{
			role_guid.server_64 = msg.role_guid();
			map_guid.server_64 = msg.map_guid();
			war_index = msg.war_index();
		}
	};

	struct ws2cs_city_war_operate : public packet_base
	{
		guid_64	city_war_map_guid;
		guid_64	legion_guid;
		guid_64	role_guid;
		int32	operate_type;
		guid_64	prop_npc_guid;

		ws2cs_city_war_operate()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_city_war_operate;
		}
		void to_proto(faith::ws2cs_proto::city_war_operate& msg)
		{
			msg.set_city_war_map_guid(city_war_map_guid.server_64);
			msg.set_legion_guid(legion_guid.server_64);
			msg.set_role_guid(role_guid.server_64);
			msg.set_operate_type(operate_type);
			msg.set_prop_npc_guid(prop_npc_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::city_war_operate& msg)
		{
			city_war_map_guid.server_64 = msg.city_war_map_guid();
			legion_guid.server_64 = msg.legion_guid();
			role_guid.server_64 = msg.role_guid();
			operate_type = msg.operate_type();
			prop_npc_guid.server_64 = msg.prop_npc_guid();
		}
	};

	struct cs2ws_set_city_occupation : public packet_base
	{
		guid_64	territory_map_guid;
		guid_64	occupation_legion_guid;
		int32	cross_city_idex;
		guid_64	second_legion_guid;
		guid_64	third_legion_guid;
		guid_64 first_legion_member_all[max_legion_member_num];
		int32	first_legion_member_num;
		guid_64 second_legion_member_all[max_legion_member_num];
		int32	second_legion_member_num;
		guid_64 third_legion_member_all[max_legion_member_num];
		int32	third_legion_member_num;
		cs2ws_set_city_occupation()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_set_city_occupation;
		}
	};

	struct cs2ws_update_legion_member_info_one : public packet_base
	{
		guid_64	role_guid;
		int32	info_index;
		int32	info_value;

		cs2ws_update_legion_member_info_one()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_update_legion_member_info_one;
		}
	};

	struct ws2cs_set_get_legion_boss_award_log : public packet_base
	{
		s_legion_boss_award_get_log	legion_boss_award_get_log;

		ws2cs_set_get_legion_boss_award_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_set_get_legion_boss_award_log;
		}
		bool to_proto(faith::ws2cs_proto::set_get_legion_boss_award_log& msg)
		{
			faith::st_proto::st_legion_boss_award_get_log* st_log_ptr = msg.mutable_legion_boss_award_get_log();
			if (st_log_ptr == nullptr)
			{
				return false;
			}
			legion_boss_award_get_log.to_proto(st_log_ptr);
			return true;		
		}
		void from_proto(const faith::ws2cs_proto::set_get_legion_boss_award_log& msg)
		{
			const faith::st_proto::st_legion_boss_award_get_log& st_log_ref = msg.legion_boss_award_get_log();

			legion_boss_award_get_log.from_proto(st_log_ref);
		}
	};

	struct ws2cs_get_construction_buff : public packet_base
	{
		guid_64	role_guid;
		int32	construction_index;
		int32	construction_level;

		ws2cs_get_construction_buff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_get_construction_buff;
		}
	};

	struct ws2cs_get_occupation_daily_award : public packet_base
	{
		guid_64	role_guid;
		int32	territory_id;

		ws2cs_get_occupation_daily_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_get_occupation_daily_award;
		}
		void to_proto(faith::ws2cs_proto::get_occupation_daily_award& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_territory_id(territory_id);

		}
		void from_proto(const faith::ws2cs_proto::get_occupation_daily_award& msg)
		{
			role_guid.server_64 = msg.role_guid();
			territory_id = msg.territory_id();
		}
	};

	struct ws2cs_get_bonus_award : public packet_base
	{
		guid_64	role_guid;
		int32	mission_enum;
		int32	sub_type_id;
		int32	finish_count[e_legion_bonus_type_max];
		int32	legion_job;
		bool	get_all;
		ws2cs_get_bonus_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_get_bonus_award;
		}
		void to_proto(faith::ws2cs_proto::get_bonus_award& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_mission_enum(mission_enum);
			msg.set_sub_type_id(sub_type_id);
			for (int32 i = 0; i < e_legion_bonus_type_max; i++)
			{
				msg.add_finish_count(finish_count[i]);
			}
			msg.set_legion_job(legion_job);
			msg.set_get_all(get_all);

		}
		void from_proto(const faith::ws2cs_proto::get_bonus_award& msg)
		{
			role_guid.server_64 = msg.role_guid();
			mission_enum = msg.mission_enum();
			sub_type_id = msg.sub_type_id();
			for (int32 i = 0 ; i < msg.finish_count_size(); i++)
			{
				finish_count[i] = msg.finish_count(i);
			}
			legion_job = msg.legion_job();
			get_all = msg.get_all();
		}
	};

	struct cs2ws_add_bonus_count :public packet_base
	{
		guid_64 legion_guid;
		int32	mission_enum;
		int32	finish_count;
		cs2ws_add_bonus_count()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_add_bonus_count;
		}
	};

	struct cs2ws_convenient_join_legion : public packet_base
	{
		guid_64 role_guid;

		cs2ws_convenient_join_legion()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_convenient_join_legion;
		}
		int32 get_len()
		{
			return sizeof(cs2ws_convenient_join_legion);
		}
	};

	struct ws2cs_get_city_master_stuff : public packet_base
	{
		guid_64 role_guid;

		ws2cs_get_city_master_stuff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_get_city_master_stuff;
		}
		void to_proto(faith::ws2cs_proto::get_city_master_stuff& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::get_city_master_stuff& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct ws2cs_del_city_master_stuff : public packet_base
	{
		guid_64 role_guid;

		ws2cs_del_city_master_stuff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_del_city_master_stuff;
		}
		void to_proto(faith::ws2cs_proto::del_city_master_stuff& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::del_city_master_stuff& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct ws2cs_add_city_war_winner_stuff : public packet_base
	{
		guid_64 role_guid;

		ws2cs_add_city_war_winner_stuff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_add_city_war_winner_stuff;
		}
		void to_proto(faith::ws2cs_proto::add_city_war_winner_stuff& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::add_city_war_winner_stuff& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct ws2cs_del_city_war_winner_stuff : public packet_base
	{
		guid_64 role_guid;

		ws2cs_del_city_war_winner_stuff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_del_city_war_winner_stuff;
		}
		void to_proto(faith::ws2cs_proto::del_city_war_winner_stuff& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::del_city_war_winner_stuff& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct ws2cs_fire_bonfire : public packet_base
	{
		guid_64 role_guid;
// 		int32	legion_level;

		ws2cs_fire_bonfire()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_fire_bonfire;
		}
	};
	struct cs2ws_fire_bonfire_ret : public packet_base
	{
		guid_64		role_guid;
		guid_64		bonfire_guid;
		guid_64		bonfire_map_guid;
		fvector		bonfire_pos;
		uint32		bonfire_end_stamp;

		cs2ws_fire_bonfire_ret()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_fire_bonfire_ret;
		}
	};
	
	struct ws2cs_bonfire_add_fuel : public packet_base
	{
		guid_64 role_guid;
		guid_64 bonfire_map_guid;
// 		int32	legion_level;
		int32	add_fuel_times;

		ws2cs_bonfire_add_fuel()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_add_fuel_to_bonfire;
		}
		void to_proto(faith::ws2cs_proto::bonfire_add_fuel& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_bonfire_map_guid(bonfire_map_guid.server_64);
			msg.set_add_fuel_times(add_fuel_times);
		}
		void from_proto(const faith::ws2cs_proto::bonfire_add_fuel& msg)
		{
			role_guid.server_64 = msg.role_guid();
			bonfire_map_guid.server_64 = msg.bonfire_map_guid();
			add_fuel_times = msg.add_fuel_times();
		}
	};
	struct cs2ws_bonfire_add_fuel_ret : public packet_base
	{
		guid_64		role_guid;

		cs2ws_bonfire_add_fuel_ret()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_add_fuel_to_bonfire_ret;
		}
	};
	
	struct cs2ws_bonfire_dead : public packet_base
	{
		guid_64		bonfire_guid;
		guid_64		legion_guid;

		cs2ws_bonfire_dead()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_bonfire_dead;
		}
	};


	struct ws2cs_draw_bonfire_daily_rwd : public packet_base
	{
		guid_64 role_guid;
		guid_64 bonfire_map_guid;

		ws2cs_draw_bonfire_daily_rwd()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_draw_bonfire_daily_rwd;
		}
		void to_proto(faith::ws2cs_proto::draw_bonfire_daily_rwd& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_bonfire_map_guid(bonfire_map_guid.server_64);

		}
		void from_proto(const faith::ws2cs_proto::draw_bonfire_daily_rwd& msg)
		{
			role_guid.server_64 = msg.role_guid();
			bonfire_map_guid.server_64 = msg.bonfire_map_guid();
		}
	};
	
	struct cs2ws_legion_add_glory_glow : public packet_base
	{
		guid_64		role_guid;
		int32		add_gg_value;

		cs2ws_legion_add_glory_glow()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_legion_add_glory_glow;
		}
	};
	
	struct cs2ws_legion_get_bonus_award_end : public packet_base
	{
		guid_64 role_guid;
		int32 glod_num;
		cs2ws_legion_get_bonus_award_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_legion_get_bonus_award_end;
		}
	};

	struct ws2cs_legion_remove_recruit_cd : public packet_base
	{
		guid_64 role_guid;

		ws2cs_legion_remove_recruit_cd()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_legion_rm_recruit_cd;
		}
		void to_proto(faith::ws2cs_proto::legion_remove_recruit_cd& msg)
		{
			msg.set_role_guid(role_guid.server_64);

		}
		void from_proto(const faith::ws2cs_proto::legion_remove_recruit_cd& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};
	struct cs2ws_legion_remove_recruit_cd_ret : public packet_base
	{
		guid_64		role_guid;

		cs2ws_legion_remove_recruit_cd_ret()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_legion_rm_recruit_cd_ret;
		}
	};
	struct cs2ws_add_legion_event : public packet_base
	{
		guid_64 legion_guid;
		xchar extra_data[max_once_legion_event_size + 1];
		cs2ws_add_legion_event()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_add_legion_event;
		}
	};

	struct cs2ws_add_legion_act_score : public packet_base
	{
		guid_64							legion_guid;
		int64							legion_score;
		e_legion_act_type				activity_type;
		int32							activity_sub_id;
		int32							member_num;
		s_legion_member_attend_activity	member_scores[max_legion_member_num]; //在世界boss中是所有造成伤害的玩家
		cs2ws_add_legion_act_score()
		{
			memset(this, 0, sizeof(cs2ws_add_legion_act_score));
			wheader = e_msg_index_cs2ws_add_legion_act_score;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&member_scores - (ULONG_PTR)&wheader;
			return (basic_len + member_num * sizeof(s_legion_member_attend_activity));
		}
	};

	struct cs2ws_settle_legion_act_rank : public packet_base
	{
		int32							activity_rank_type; //e_legion_rank_type
		int32							activity_sub_id;
		guid_64							special_legion_guid;
		int32							member_num;
		s_legion_member_attend_activity	member_scores[max_hurt_monster_player_num];
		cs2ws_settle_legion_act_rank()
		{
			memset(this, 0, sizeof(cs2ws_settle_legion_act_rank));
			wheader = e_msg_index_cs2ws_settle_legion_act_rank;
			activity_rank_type = -1;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&member_scores - (ULONG_PTR)&wheader;
			return (basic_len + member_num * sizeof(s_legion_member_attend_activity));
		}
	};

	struct cs2ws_legion_boss_damage_player : public packet_base
	{
		guid_64						legion_guid;
		int32						npc_id;
		guid_64						player_guid_arr[max_legion_member_num];
		int32						member_num;
		cs2ws_legion_boss_damage_player()
		{
			memset(this, 0, sizeof(cs2ws_legion_boss_damage_player));
			wheader = e_msg_index_cs2ws_legion_boss_damage_player;
		}
	};

	struct ws2cs_set_legion_average_lv_to_map : public packet_base
	{
		guid_64							map_guid;
		int32							legion_average_lv;
		ws2cs_set_legion_average_lv_to_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_set_legion_average_lv_to_map;
		}
		uint32 get_pak_length() const
		{
			return sizeof(*this);
		}
		void to_proto(faith::ws2cs_proto::set_legion_average_lv_to_map& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			msg.set_legion_average_lv(legion_average_lv);
		}
		void from_proto(const faith::ws2cs_proto::set_legion_average_lv_to_map& msg)
		{
			map_guid.server_64 = msg.map_guid();
			legion_average_lv = msg.legion_average_lv();
		}
	};

	struct ws2cs_set_legion_guid_to_map : public packet_base
	{
		guid_64							map_guid;
		guid_64							legion_guid;
		ws2cs_set_legion_guid_to_map()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_set_legion_guid_to_map;
		}
		void to_proto(faith::ws2cs_proto::set_legion_guid_to_map& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			msg.set_legion_guid(legion_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::set_legion_guid_to_map& msg)
		{
			map_guid.server_64 = msg.map_guid();
			legion_guid.server_64 = msg.legion_guid();
		}
	};

	struct ws2cs_confirm_change_legion_name : public packet_base
	{
		guid_64		role_guid;
		xchar		legion_name[max_name_size +1];
		bool		is_need_change;
		ws2cs_confirm_change_legion_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_confirm_change_legion_name;
		}
		void set_legion_name(std::string name)
		{
			memcpy(legion_name, name.c_str(), name.size() > max_name_size ? max_name_size : name.size());
		}
		void to_proto(faith::ws2cs_proto::confirm_change_legion_name& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_legion_name(legion_name);
			msg.set_is_need_change(is_need_change);
			
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
		void from_proto(const faith::ws2cs_proto::confirm_change_legion_name& msg)
		{
			role_guid.server_64 = msg.role_guid();
			my_memcopy_string(legion_name, max_name_size, msg.legion_name());
			is_need_change = msg.is_need_change();
		}
	};


	struct ws2cs_rem_item_change_legion_name_care : public packet_base
	{
		guid_64		role_guid;
		ws2cs_rem_item_change_legion_name_care()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_rem_item_change_legion_name_care;
		}
		void to_proto(faith::ws2cs_proto::rem_item_change_legion_name_care& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::rem_item_change_legion_name_care& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct cs2ws_confirm_change_legion_name : public packet_base
	{
		guid_64		role_guid;
		int32	m_legion_error_type;
		cs2ws_confirm_change_legion_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_confirm_change_legion_name;
		}
	};

	struct ws2cs_change_gate_legion_name : public packet_base
	{
		guid_64 legion_guid;
		xchar   legion_name[max_name_size + 1];
		ws2cs_change_gate_legion_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_change_gate_legion_name;
		}
		void set_legion_name(xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), sizeof(legion_name) > role_name.size() ? role_name.size() : sizeof(legion_name));
		}


		void to_proto(faith::ws2cs_proto::change_gate_legion_name& msg)
		{
			msg.set_legion_guid(legion_guid.server_64);
			msg.set_legion_name(legion_name);
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
		void from_proto(const faith::ws2cs_proto::change_gate_legion_name& msg)
		{
			legion_guid.server_64 = msg.legion_guid();
			my_memcopy_string(legion_name, max_name_size, msg.legion_name());
		}
	};
	/************************************************************************/
	/*                             activity                                 */
	/************************************************************************/

	struct cs2ws_set_best_record : public packet_base
	{
		int32 map_template_id;
		s_single_map_record record;

		cs2ws_set_best_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_set_best_record;
		}
		bool to_proto(faith::cs2ws_proto::set_best_record& msg)
		{
			msg.set_map_template_id(map_template_id);
			faith::st_proto::st_single_map_record *st_single_ptr = msg.mutable_record();
			if (st_single_ptr == nullptr)
			{
				return false;
			}
			record.to_proto(st_single_ptr);
			return true;
		}
		void from_proto(const faith::cs2ws_proto::set_best_record& msg)
		{
			map_template_id = msg.map_template_id();
			record.from_proto(msg.record());
		}
	};

	/************************************************************************/
	/*                         char_sync_data                               */
	/************************************************************************/

	struct cs2ws_sync_char_data :public packet_base
	{
		guid_64		role_guid;
		int32		data_type;
		int64       data_value;
		int32		sub_data;
		cs2ws_sync_char_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_sync_char_data;
		}
	};
	/************************************************************************/
	/*                         arena                                         */
	/************************************************************************/

	struct ws2cs_fresh_challenge_time :public packet_base
	{
		guid_64 role_guid;
		int32	is_need;
		ws2cs_fresh_challenge_time()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_resp_fresh_challenge_time;
		}
	};

	struct cs2ws_req_get_arena_rank :public packet_base
	{
		guid_64 role_guid;

		cs2ws_req_get_arena_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_req_get_arena_rank;
		}
		void to_proto(faith::cs2ws_proto::req_get_arena_rank& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::cs2ws_proto::req_get_arena_rank& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct ws2cs_rep_get_arena_rank :public packet_base
	{
		guid_64 role_guid;
		int32	arena_rank;
		int32   is_request;
		ws2cs_rep_get_arena_rank()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_rep_get_arena_rank;
		}

		void to_proto(faith::ws2cs_proto::rep_get_arena_rank& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_arena_rank(arena_rank);
			msg.set_is_request(is_request);
		}
		void from_proto(const faith::ws2cs_proto::rep_get_arena_rank& msg)
		{
			role_guid.server_64 = msg.role_guid();
			arena_rank = msg.arena_rank();
			is_request = msg.is_request();
		}
	};

	struct cs2ws_req_challenge_msg :public packet_base
	{
		guid_64 role_guid;
		guid_64 target_guid;
		int32	cost_money_type;
		int32	cost_money_num;
		int32	target_cur_pos;
		int32	self_cur_pos;
		cs2ws_req_challenge_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_req_challenge;
		}
		void to_proto(faith::cs2ws_proto::req_challenge_msg& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_target_guid(target_guid.server_64);
			msg.set_cost_money_type(cost_money_type);
			msg.set_cost_money_num(cost_money_num);
			msg.set_target_cur_pos(target_cur_pos);
			msg.set_self_cur_pos(self_cur_pos);
		}
		void from_proto(const faith::cs2ws_proto::req_challenge_msg& msg)
		{
			role_guid.server_64 = msg.role_guid();
			target_guid.server_64 = msg.target_guid();
			cost_money_type = msg.cost_money_type();
			cost_money_num = msg.cost_money_num();
			target_cur_pos = msg.target_cur_pos();
			self_cur_pos = msg.self_cur_pos();
		}
	};
	
	struct ws2cs_req_challenge_msg :public packet_base
	{
		guid_64 role_guid;
		guid_64 target_guid;
		int32   req_result;
		int32	robot_id;
		int32	cost_money_type;
		int32	cost_money_value;
		int32	target_cur_pos;
		int32	self_cur_pos;
		ws2cs_req_challenge_msg()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_req_challenge;
		}
		void to_proto(faith::ws2cs_proto::req_challenge_msg& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_target_guid(target_guid.server_64);
			msg.set_req_result(req_result);
			msg.set_robot_id(robot_id);
			msg.set_cost_money_type(cost_money_type);
			msg.set_cost_money_value(cost_money_value);
			msg.set_target_cur_pos(target_cur_pos);
			msg.set_self_cur_pos(self_cur_pos);
		}
		void from_proto(const faith::ws2cs_proto::req_challenge_msg& msg)
		{
			role_guid.server_64 = msg.role_guid();
			target_guid.server_64 = msg.target_guid();
			req_result = msg.req_result();
			robot_id = msg.robot_id();
			cost_money_type = msg.cost_money_type();
			cost_money_value = msg.cost_money_value();
			target_cur_pos = msg.target_cur_pos();
			self_cur_pos = msg.self_cur_pos();
		}
	};

	struct cs2ws_req_choose_list :public packet_base
	{
		guid_64 role_guid;
		int32	cur_pos;
		cs2ws_req_choose_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_req_challenge_list;
		}
		void to_proto(faith::cs2ws_proto::req_choose_list& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_cur_pos(cur_pos);
		}
		void from_proto(const faith::cs2ws_proto::req_choose_list& msg)
		{
			role_guid.server_64 = msg.role_guid();
			cur_pos = msg.cur_pos();
		}
	};

	struct ws2cs_req_choose_list :public packet_base
	{
		guid_64 role_guid;
		player_arena_rank target_player[3];

		ws2cs_req_choose_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_req_challenge_list;
		}
		bool to_proto(faith::ws2cs_proto::req_choose_list& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			for (int32 i = 0; i < 3; i++)
			{
				faith::st_proto::st_player_arena_rank* st_arena_ptr = msg.add_target_player();
				if (st_arena_ptr == nullptr)
				{
					return false;
				}
				target_player[i].to_proto(st_arena_ptr);
			}
			return true;
		}
		void from_proto(const faith::ws2cs_proto::req_choose_list& msg)
		{
			role_guid.server_64 = msg.role_guid();

			for (int32 i = 0 ; i < msg.target_player_size(); i++)
			{
				target_player[i].from_proto(msg.target_player(i));
			}
		}

	};

	struct cs2ws_first_three :public packet_base
	{
		guid_64 role_guid;

		cs2ws_first_three()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_first_three;
		}
		void to_proto(faith::cs2ws_proto::first_three& msg)
		{
			msg.set_role_guid(role_guid.server_64);
		}
		void from_proto(const faith::cs2ws_proto::first_three& msg)
		{
			role_guid.server_64 = msg.role_guid();
		}
	};

	struct cs2ws_chellenge_over :public packet_base
	{
		guid_64				role_guid;
		guid_64				target_guid;
		player_arena_rank   role_info;
		int32				chellenge_result;
		int32				target_rank;
		cs2ws_chellenge_over()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_chellenge_over;
		}
		bool to_proto(faith::cs2ws_proto::chellenge_over& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_target_guid(target_guid.server_64);
			faith::st_proto::st_player_arena_rank *st_arena_ptr = msg.mutable_role_info();
			if (st_arena_ptr == nullptr)
			{
				return false;
			}
			role_info.to_proto(st_arena_ptr);
			msg.set_chellenge_result(chellenge_result);
			msg.set_target_rank(target_rank);
			return true;
		}
		void from_proto(const faith::cs2ws_proto::chellenge_over& msg)
		{
			role_guid.server_64 = msg.role_guid();
			target_guid.server_64 = msg.target_guid();
			role_info.from_proto(msg.role_info());
			chellenge_result = msg.chellenge_result();
			target_rank = msg.target_rank();
		}
	};

	/************************************************************************/
	/*                         ranking                                      */
	/************************************************************************/

	struct cs2ws_req_worship_player : public packet_base
	{
		guid_64					role_guid;
		int32					ranking_index;
		int32					worship_type;
		guid_64					target_guid;

		cs2ws_req_worship_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_req_worship_player;
		}
	};

	struct cs2ws_sync_ranking_info : public faith::packet_base
	{
		s_ranking_player_info			role_info;
		int32							server_id;

		cs2ws_sync_ranking_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_sync_ranking_info;
		}

		uint32 get_pak_length() const
		{
			return sizeof(cs2ws_sync_ranking_info);
		}
	};

	struct gate2ws_get_ranking_list_error : public packet_base
	{
		guid_64					role_guid;
		int32					result;
		gate2ws_get_ranking_list_error()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_ranking_list_error;
		}
	};

	struct ws2ws_get_oracle_ranking_info : public faith::packet_base
	{
		int32							server_id;
		ws2ws_get_oracle_ranking_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_oracle_trial_info;
		}
	};

	struct ws2ws_send_oracle_ranking_notice : public faith::packet_base
	{
		guid_64							player_guid;
		xchar							play_name[max_name_size];
		guid_64							legion_guid;
		xchar							legion_name[max_name_size];
		int32							server_id;
		ws2ws_send_oracle_ranking_notice()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_oracle_ranking_notice;
		}
		void set_play_name(xstring role_name)
		{
			memset(play_name, 0, sizeof(play_name));
			memcpy(play_name, role_name.c_str(), role_name.size() > sizeof(play_name) ? sizeof(play_name) : role_name.size());
		}
		void set_legion_name(xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), role_name.size() > sizeof(legion_name) ? sizeof(legion_name) : role_name.size());
		}
	};

	struct cs2ws_sync_vip_level_data : public faith::packet_base
	{
		guid_64				role_guid;
		int32				vip_level;

		cs2ws_sync_vip_level_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_sync_vip_level_data;
		}

		uint32 get_pak_length() const
		{
			return sizeof(cs2ws_sync_vip_level_data);
		}
	};



	struct ws2cs_req_worship_player_end : public packet_base
	{
		guid_64						role_guid;
		guid_64                     target_guid;
		int32						ranking_type;
		int32						worship_type;
		int32						worship_result;
		int64						target_worship_value;

		ws2cs_req_worship_player_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_req_worship_player_end;
		}
		void to_proto(faith::ws2cs_proto::req_worship_player_end& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_target_guid(target_guid.server_64);
			msg.set_ranking_type(ranking_type);
			msg.set_worship_type(worship_type);
			msg.set_worship_result(worship_result);
			msg.set_target_worship_value(target_worship_value);
		}
		void from_proto(const faith::ws2cs_proto::req_worship_player_end& msg)
		{
			role_guid.server_64 = msg.role_guid();
			target_guid.server_64 = msg.target_guid();
			ranking_type = msg.ranking_type();
			worship_type = msg.worship_type();
			worship_result = msg.worship_result();
			target_worship_value = msg.target_worship_value();
		}
	};

	struct ws2cs_sync_first_rank_end : public  packet_base
	{
		guid_64					role_guid;
		int32					ranking_type;
		bool					is_first;
		bool					is_notice;
		ws2cs_sync_first_rank_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_sync_first_rank_end;
		}
		void to_proto(faith::ws2cs_proto::sync_first_rank_end& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_ranking_type(ranking_type);
			msg.set_is_first(is_first);
			msg.set_is_notice(is_notice);
		}
		void from_proto(const faith::ws2cs_proto::sync_first_rank_end& msg)
		{
			role_guid.server_64 = msg.role_guid();
			ranking_type = msg.ranking_type();
			is_first = msg.is_first();
			is_notice = msg.is_notice();
		}
	};

	struct ws2cs_sync_server_avg_lv : public  packet_base
	{
		int32					srv_avg_lv;
		int32					world_lv_cur;
		int32					world_lv_last;

		ws2cs_sync_server_avg_lv()
		{
			memset(this, 0, sizeof(*this));
			srv_avg_lv = -1;
			world_lv_cur = -1;
			world_lv_last = -1;
			wheader = e_msgindex_ws2cs_sync_server_avg_lv;
		}
		void to_proto(faith::ws2cs_proto::sync_server_avg_lv& msg)
		{
			msg.set_srv_avg_lv(srv_avg_lv);
			msg.set_world_lv_cur(world_lv_cur);
			msg.set_world_lv_last(world_lv_last);
		}
		void from_proto(const faith::ws2cs_proto::sync_server_avg_lv& msg)
		{
			srv_avg_lv = msg.srv_avg_lv();
			world_lv_cur = msg.world_lv_cur();
			world_lv_last = msg.world_lv_last();
		}
	};

	struct cs2ws_req_service_rank_info : public  packet_base
	{
		guid_64					role_guid;
		int32					rank_type;

		cs2ws_req_service_rank_info()
		{
			memset(this, 0, sizeof(*this));
			role_guid = 0;
			rank_type = -1;
			wheader = e_msgindex_cs2ws_req_service_rank_info;
		}
	};

	struct ws2cs_req_service_rank_info_end : public  packet_base
	{
		guid_64					role_guid;
		int32					rank_type;
		int32					rank_num;

		ws2cs_req_service_rank_info_end()
		{
			memset(this, 0, sizeof(*this));
			role_guid = 0;
			rank_num = 0;
			rank_type = 0;
			wheader = e_msgindex_ws2cs_req_service_rank_info_end;
		}
		void to_proto(faith::ws2cs_proto::req_service_rank_info_end& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_rank_type(rank_type);
			msg.set_rank_num(rank_num);

		}
		void from_proto(const faith::ws2cs_proto::req_service_rank_info_end& msg)
		{
			role_guid.server_64 = msg.role_guid();
			rank_type = msg.rank_type();
			rank_num = msg.rank_num();
		}
	};


	/************************************************************************/
	/*                         mail                                         */
	/************************************************************************/

	struct cs2ws_send_mail : public packet_base
	{
		s_client_uid  client_uid;
		s_item_info item_list[max_item_per_mail];
		s_mail_info mail_info;
		int32		item_num;
		guid_64		addressee_guid;
		int32		send_times;
		int32		receiver_server_id;

		cs2ws_send_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_mail;
		}
	};

	struct cs2ws_send_mail_to_all_player : public packet_base
	{
		s_client_uid  client_uid;
		s_item_info item_list[max_item_per_mail];
		s_mail_info mail_info;
		int32		item_num;

		cs2ws_send_mail_to_all_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_mail_send_to_all_player;
		}
	};


	struct ws2cs_send_mail : public packet_base
	{
		s_client_uid  client_uid;
		s_item_info item_list[max_item_per_mail];
		s_mail_info mail_info;
		int32		item_num;
		guid_64		addressee_guid;
		int32		send_times;
		ws2cs_send_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_recive_mail;
		}
		bool  to_proto(faith::ws2cs_proto::send_mail& msg)
		{
			msg.set_client_uid(client_uid.fep_uid_64);

			for (int32 i = 0; i < max_item_per_mail; i++)
			{
				faith::db_proto::item_info_db *db_item_ptr = msg.add_item_list();
				if (db_item_ptr == nullptr)
				{
					return false;
				}
				//item_list[i].to_proto(db_item_ptr);
			}
			faith::db_proto::role_mail_row *db_mail_ptr = msg.mutable_mail_info();
			if (db_mail_ptr == nullptr)
			{
				return false;
			}
			mail_info.to_proto(db_mail_ptr);
			msg.set_item_num(item_num);
			msg.set_addressee_guid(addressee_guid.server_64);
			msg.set_send_times(send_times);

			return true; 	
		}
		void from_proto(const faith::ws2cs_proto::send_mail& msg)
		{
			client_uid.fep_uid_64 = msg.client_uid();
			for (int32 i = 0; i <  msg.item_list_size() && i < max_item_per_mail; i++)
			{
				//item_list[i].from_proto(msg.item_list(i));
			}
			mail_info.from_proto(msg.mail_info());
			item_num = msg.item_num();
			addressee_guid.server_64 = msg.addressee_guid();
			send_times = msg.send_times();	
		}
	};

	struct cs2ws_send_globel_message : public packet_base
	{
		s_client_uid  client_uid;
		guid_64		role_guid;
		xchar		message_text[globel_message_max_size + 1];

		cs2ws_send_globel_message()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_globel_message;
		}
	};

	struct ws2cs_receive_globel_message : public packet_base
	{
		s_client_uid  client_uid;
		guid_64		sender_guid;
		guid_64		addressee_guid;
		xchar		message_text[globel_message_max_size + 1];

		ws2cs_receive_globel_message()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_receive_globel_message;
		}
	};

	struct cs2ws_send_notice : public packet_base
	{
		s_client_uid	client_uid;
		guid_64			role_guid;
		int32			notice_id;
		int32			item_num;
		s_item_info		data_ary[chat_max_item];
		xchar			notice_string[globel_message_max_size + 1];

		cs2ws_send_notice()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_notice;
		}
		void clear_item_info()
		{
			memset(data_ary, 0, sizeof(s_item_info)*chat_max_item);
			item_num = 0;
		}
	};

	struct ws2cs_receive_notice : public packet_base
	{
		s_client_uid  client_uid;
		guid_64		sender_guid;
		guid_64		addressee_guid;
		int32			notice_id;
		int32			create_time;
		s_item_info		data_ary[chat_max_item];

		ws2cs_receive_notice()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_receive_notice;
		}
		bool to_proto(faith::ws2cs_proto::receive_notice& msg)
		{
			msg.set_client_uid(client_uid.fep_uid_64);
			msg.set_sender_guid(sender_guid.server_64);
			msg.set_addressee_guid(addressee_guid.server_64);
			msg.set_notice_id(notice_id);
			msg.set_create_time(create_time);
			for (int32 i = 0; i < chat_max_item; i++)
			{
				faith::db_proto::item_info_db *db_item_ptr = msg.add_data_ary();
				if (db_item_ptr == nullptr)
				{
					return false;
				}
				//data_ary[i].to_proto(db_item_ptr);
			}
			return true;
		}
		void from_proto(const faith::ws2cs_proto::receive_notice& msg)
		{
			client_uid.fep_uid_64 = msg.client_uid();
			sender_guid.server_64 = msg.sender_guid();
			addressee_guid.server_64 = msg.addressee_guid();
			notice_id = msg.notice_id();
			create_time = msg.create_time();
			for (int32 i = 0; i < msg.data_ary_size() && i < chat_max_item; i++)
			{
				//data_ary[i].from_proto(msg.data_ary(i));
			}
		}
	};

	struct ws2cs_role_info_change : public packet_base
	{
		guid_64 role_guid;
		int32 role_type;
		int32 role_value;
		ws2cs_role_info_change()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_role_info_change;
		}
		void to_proto(faith::ws2cs_proto::role_info_change& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_role_type(role_type);
			msg.set_role_value(role_value);

		}
		void from_proto(const faith::ws2cs_proto::role_info_change& msg)
		{
			role_guid.server_64 = msg.role_guid();
			role_type = msg.role_type();
			role_value = msg.role_value();
		}
	};

	struct ws2cs_role_right : public packet_base
	{
		guid_64						role_guid;
		e_role_right				right_type;
		int32						right_value;
		int32						right_time;

		ws2cs_role_right()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_role_right;
		}
		void to_proto(faith::ws2cs_proto::role_right& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_right_type(right_type);
			msg.set_right_value(right_value);
			msg.set_right_time(right_time);

		}
		void from_proto(const faith::ws2cs_proto::role_right& msg)
		{
			role_guid.server_64 = msg.role_guid();
			right_type = (e_role_right)msg.right_type();
			right_value = msg.right_value();
			right_time = msg.right_time();
		}
	};
	struct ws2cs_buy_good : public packet_base
	{
		guid_64						role_guid;
		int32						goods_id;
		int32						goods_num;
		int32						store_id;
		bool						is_auto_buy;
		int32						back_string;

		ws2cs_buy_good()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_buy_good;
		}
		void to_proto(faith::ws2cs_proto::buy_good& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_goods_id(goods_id);
			msg.set_goods_num(goods_num);
			msg.set_store_id(store_id);
			msg.set_is_auto_buy(is_auto_buy);
			msg.set_back_string(back_string);
		}
		void from_proto(const faith::ws2cs_proto::buy_good& msg)
		{
			role_guid.server_64 = msg.role_guid();
			goods_id = msg.goods_id();
			goods_num = msg.goods_num();
			store_id = msg.store_id();
			is_auto_buy = msg.is_auto_buy();
			back_string = msg.back_string();

		}
	};

	struct ws2cs_del_unit_item : public packet_base
	{
		int32 cs_unit_index;
		int32 bag_type;
		int32 bag_slot_begin;
		int32 bag_slot_end;
		ws2cs_del_unit_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_del_unit_item;
		}
	};

	struct ws2cs_change_unit_vip : public packet_base
	{
		int32 cs_unit_index;
		int32 vip_value;
		ws2cs_change_unit_vip()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_change_unit_vip;
		}
	};

	struct ws2cs_del_unit_specified_item : public packet_base
	{
		int32 cs_unit_index;
		int32 bag_type;
		int32 item_id;
		ws2cs_del_unit_specified_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_del_unit_specified_item;
		}
	};
	struct ws2cs_add_guide_record : public packet_base
	{
		int32 cs_unit_index;
		int32 guide_trigger_id;
		ws2cs_add_guide_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_add_guide_record;
		}
	};

	struct cs2ws_gm_order : public packet_base
	{
		e_gm_order_type gm_order_type;
		int32			order_param[GM_ORDER_PARAMETER_MAX_SIZE_ON_WS];
		int32			param_num;
		cs2ws_gm_order()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_gm_order;
		}
	};

	struct ws2dp_load_server_attr_val : public packet_base
	{
		ws2dp_load_server_attr_val()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_server_attr_val;
		}
	};

	struct s_server_attr_int
	{
		server_attr_val_type	attr_type;
		int64					attr_val;
		s_server_attr_int()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct dp2ws_load_server_attr_val_end : public packet_base
	{
		int32 attr_num;
		s_server_attr_int attr_int_val[e_srv_attr_val_max];

		dp2ws_load_server_attr_val_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_server_attr_val_end;
		}
	};

	struct ws2dp_save_server_attr_val : public packet_base
	{
		s_server_attr_int attr_int;

		ws2dp_save_server_attr_val()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_server_attr_val;
		}
	};

	struct ws2ws_cross_server_time : public packet_base
	{
		int32 cross_server_time;

		ws2ws_cross_server_time()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_cross_server_time;
		}
	};

	struct ws2cs_recharge : public packet_base
	{
		xchar						order_id[max_recharge_order_length + 1];
		guid_64						role_guid;
		int32						goods_id;
		float						pay_price;
		int64						order_num;
		int32						payment_type;
		int32						direct_diamond;

		ws2cs_recharge()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_recharge;
		}
		void to_proto(faith::ws2cs_proto::recharge& msg)
		{
			msg.set_order_id(order_id);
			msg.set_role_guid(role_guid.server_64);
			msg.set_goods_id(goods_id);
			msg.set_pay_price(pay_price);
			msg.set_order_num(order_num);
			msg.set_payment_type(payment_type);
			msg.set_direct_diamond(direct_diamond);

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

		void from_proto(const faith::ws2cs_proto::recharge& msg)
		{
			my_memcopy_string(order_id, max_recharge_order_length, msg.order_id());
			role_guid.server_64 = msg.role_guid();
			goods_id = msg.goods_id();
			pay_price = msg.pay_price();
			order_num = msg.order_num();
			payment_type = msg.payment_type();
			direct_diamond = msg.direct_diamond();
		}
	};
	struct cs2ws_recharge_end : public packet_base
	{
		xchar						order_id[max_recharge_order_length + 1];
		guid_64						role_guid;
		int32						goods_id;
		int64						order_num;
		int32						payment_type;
		int32						result;
		bool						is_first_recharge;
		cs2ws_recharge_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_recharge_end;
		}
	};

	struct cs2ws_recharge_end_send_mail : public packet_base
	{
		xchar						order_id[max_recharge_order_length + 1];
		guid_64						role_guid;
		int32						goods_id;
		int32						pay_price;
		int32						direct_diamond;
		int64						order_num;
		int32						payment_type;

		cs2ws_recharge_end_send_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_recharge_end_send_mail;
		}
	};

	struct cs2ws_arena_sync_gs_value : public packet_base
	{
		guid_64						role_guid;
		int32						cur_pos;
		int64						gs_value;
		cs2ws_arena_sync_gs_value()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_arena_sync_gs_value;
		}
		void to_proto(faith::cs2ws_proto::arena_sync_gs_value& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_cur_pos(cur_pos);
			msg.set_gs_value(gs_value);
		}
		void from_proto(const faith::cs2ws_proto::arena_sync_gs_value& msg)
		{
			role_guid.server_64 = msg.role_guid();
			cur_pos = msg.cur_pos();
			gs_value = msg.gs_value();
		}
	};


	struct ws2cs_legion_answer_question_right : packet_base
	{
		guid_64					role_guid;
		int32					question_template_id;
		bool					is_answer;				//是否是抢答
		int32					data_num;
		guid_64					legion_play_guid[max_legion_member_num];
		ws2cs_legion_answer_question_right()
		{
			memset(this, 0, sizeof(*this));

			wheader = e_msgindex_ws2cs_legion_answer_question_right;
		}
		void to_proto(faith::ws2cs_proto::legion_answer_question_right& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_question_template_id(question_template_id);
			msg.set_is_answer(is_answer);
			msg.set_data_num(data_num);
			for (int32 i = 0; i < data_num; i++)
			{
				msg.add_legion_play_guid(legion_play_guid[i]);
			}
		}
		void from_proto(const faith::ws2cs_proto::legion_answer_question_right& msg)
		{
			role_guid.server_64 = msg.role_guid();
			question_template_id = msg.question_template_id();
			is_answer = msg.is_answer();
			data_num = msg.data_num();
			for (int32 i = 0; i < data_num; i++)
			{
				legion_play_guid[i] = msg.legion_play_guid(i);
			}

			
		}
	};
	struct ws2cs_spawn_npc : public packet_base
	{
		guid_64					map_guid;
		int32					npc_template_id;
		s_map_pos				pos;
		ws2cs_spawn_npc()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_spawn_npc;
		}
		void to_proto(faith::ws2cs_proto::spawn_npc& msg)
		{
			msg.set_map_guid(map_guid.server_64);
			msg.set_npc_template_id(npc_template_id);
			msg.add_unit_location(pos.unit_location.x);
			msg.add_unit_location(pos.unit_location.y);
			msg.add_unit_location(pos.unit_location.z);
			msg.add_unit_rotation(pos.unit_rotation.roll);
			msg.add_unit_rotation(pos.unit_rotation.pitch);
			msg.add_unit_rotation(pos.unit_rotation.yaw);
		}
		void from_proto(const faith::ws2cs_proto::spawn_npc& msg)
		{
			if (msg.unit_location_size() < 3 || msg.unit_rotation_size() < 3)
			{
				return;
			}
			map_guid.server_64 = msg.map_guid();
			npc_template_id = msg.npc_template_id();
			pos.unit_location.x = msg.unit_location(0);
			pos.unit_location.y = msg.unit_location(1);
			pos.unit_location.z = msg.unit_location(2);
			pos.unit_rotation.roll = msg.unit_rotation(0);
			pos.unit_rotation.pitch = msg.unit_rotation(1);
			pos.unit_rotation.yaw = msg.unit_rotation(2);
		}
	};
	struct ws2cs_modify_money : public packet_base
	{
		int32 cs_unit_index;
		bool is_add_money;
		e_money_type money_type;
		int64 money_value;

		ws2cs_modify_money()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_modify_money;
		}
		void to_proto(faith::ws2cs_proto::modify_money& msg)
		{
			msg.set_cs_unit_index(cs_unit_index);
			msg.set_is_add_money(is_add_money);
			msg.set_money_type(money_type);
			msg.set_money_value(money_value);
		}
		void from_proto(const faith::ws2cs_proto::modify_money& msg)
		{
			cs_unit_index = msg.cs_unit_index();
			is_add_money = msg.is_add_money();
			money_type = (e_money_type)msg.money_type();
			money_value = msg.money_value();
		}
	};
	struct cs2fep_in_game : public packet_base
	{
		xchar					account[max_account_length + 1];
		s_client_uid			client_uid;
		int32					array_index;
		guid_64					role_guid;
		cs2fep_in_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2fep_in_game;
		}
	};
	struct fep2cs_in_game : public packet_base
	{
		xchar					account[max_account_length + 1];
		int32					array_index;
		guid_64					role_guid;
		bool					be_in_game;
		fep2cs_in_game()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_fep2cs_in_game;
		}
	};

	struct ws2ws_kick_out_player : public packet_base
	{
		guid_64					role_guid;
		bool					need_send_save_end;
		ws2ws_kick_out_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_kick_player;
		}
	};

	struct ws2ws_cross_player_legion_info : public packet_base
	{
		guid_64							role_guid;
		s_cross_player_legion_info		legion_info;
		ws2ws_cross_player_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_player_legion_info;
		}
	};

	struct cs2ws_time_limit_activity_common_data_sync : public packet_base
	{
		int32 activity_type;
		int32 data_type;
		int32 data_num;
		int32 condition_list[time_limit_activity_schedule_num];
		int32 condition_num;
		cs2ws_time_limit_activity_common_data_sync()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_time_limit_activity_common_data_sync;
		}
	};
	struct ws2ws_time_limit_activity_common_data_sync : public packet_base
	{
		int32 activity_type;
		int32 data_type;
		int32 data_num;
		int32 condition_list[time_limit_activity_schedule_num];
		int32 condition_num;
		ws2ws_time_limit_activity_common_data_sync()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2ws_time_limit_activity_common_data_sync;
		}
	};
	struct cs2ws_check_time_limit_activity_common_data : public packet_base
	{
		guid_64 user_guid;
		int32 activity_type;
		int32 check_data;
		int32 activity_id;
		cs2ws_check_time_limit_activity_common_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_check_time_limit_activity_common_data;
		}
	};
	struct ws2cs_check_time_limit_activity_common_data : public packet_base
	{
		guid_64 user_guid;
		int32 activity_id;
		bool is_achieve;
		ws2cs_check_time_limit_activity_common_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_check_time_limit_activity_common_data;
		}
		void to_proto(faith::ws2cs_proto::check_time_limit_activity_common_data& msg)
		{
			msg.set_user_guid(user_guid.server_64);
			msg.set_activity_id(activity_id);
			msg.set_is_achieve(is_achieve);

		}
		void from_proto(const faith::ws2cs_proto::check_time_limit_activity_common_data& msg)
		{
			user_guid.server_64 = msg.user_guid();
			activity_id = msg.activity_id();
			is_achieve = msg.is_achieve();
		}
	};
	struct ws2ws_check_time_limit_activity_common_data : public packet_base
	{
		int32 server_id;
		guid_64 user_guid;
		int32 activity_type;
		int32 check_data;
		int32 activity_id;
		ws2ws_check_time_limit_activity_common_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2ws_check_time_limit_activity_common_data;
		}
	};
	struct ws2ws_check_time_limit_activity_common_data_end : public packet_base
	{
		guid_64 user_guid;
		int32 activity_id;
		bool is_achieve;
		ws2ws_check_time_limit_activity_common_data_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2ws_check_time_limit_activity_common_data_end;
		}
	};
	
	struct ws2ws_time_limit_activity_common_data_sync_end : public packet_base
	{
		int32 activity_type;
		int32 activity_schedule;
		ws2ws_time_limit_activity_common_data_sync_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2ws_time_limit_activity_common_data_sync_end;
		}
	};
	
	struct ws2ws_get_time_limit_activity_common_data_to_gate : public packet_base
	{
		guid_64 role_guid;
		int32 server_id;
		ws2ws_get_time_limit_activity_common_data_to_gate()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2ws_get_time_limit_activity_common_data_to_gate;
		}
	};
	struct ws2ws_get_time_limit_activity_common_data_to_ws : public packet_base
	{
		guid_64 role_guid;
		int32 activity_type;
		int32 activity_schedule;
		ws2ws_get_time_limit_activity_common_data_to_ws()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2ws_get_time_limit_activity_common_data_to_ws;
		}
	};
	
	struct ws2cs_set_time_limit_activity_pause_flag : public packet_base
	{
		bool pause_flag;
		ws2cs_set_time_limit_activity_pause_flag()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_set_time_limit_activity_pause_flag;
		}
	};
	
	struct cs2ws_send_role_info_to_gm : public packet_base
	{
		s_client_uid	client_uid;
		s_unit_info		role_info;

		cs2ws_send_role_info_to_gm()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_cs2ws_send_role_info_to_gm;
		}
	};

	struct ws2cs_transfer_this_map_all_player : public packet_base
	{
		guid_64							map_guid;
		ws2cs_transfer_this_map_all_player()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_transfer_all_player_this_map;
		}
		void to_proto(faith::ws2cs_proto::transfer_this_map_all_player& msg)
		{
			msg.set_map_guid(map_guid.server_64);
		}
		void from_proto(const faith::ws2cs_proto::transfer_this_map_all_player& msg)
		{
			map_guid.server_64 = msg.map_guid();
		}
	};

	struct ws2dp_save_account_online_time : public packet_base
	{
		xchar					account[max_account_length + 1];
		int32					online_duration;
		int32					online_time;
		int32					month_recharge_num;
		int32					month_recharge_time;
		ws2dp_save_account_online_time()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_account_online_time;
		}
	};

	struct cs2fep_month_recharge_num : public packet_base
	{
		xchar					account[max_account_length + 1];
		s_client_uid			client_uid;
		int32					array_index;
		guid_64					role_guid;
		int32					recharge_num;
		cs2fep_month_recharge_num()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2fep_month_recharge_num;
		}
	};

	struct ws2cs_set_ladder_world_level : public packet_base
	{
		int64 world_level;
		ws2cs_set_ladder_world_level()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msg_index_ws2cs_set_ladder_world_level;
		}
	};

#pragma pack(pop)
}

#endif 
