#ifndef _LEGION_DEF_HPP_
#define _LEGION_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/char_def.hpp"
#include "Logic/item_def.hpp"
#include "Logic/ranking_def.hpp"
#include "count_def.hpp"
#include "template/ServerTemplateHead.h"
#include <limits>

namespace faith
{
#pragma pack(push,1)

	const int32 legion_name_size_min_limit = 2;
	const int32 legion_name_size_max_limit = 12;
	const int32 max_legion_count = 999;
	const int32 db_legion_count = 100;
	const int32 once_legion_list_get_num = 15;
	const int32 max_legion_announcement_size = 400;
	const int32 max_legion_announcement_content_size = 200;
	const int32 max_legion_member_num = 250;
	const int32 max_cross_legion_member_num = 50;
	const int64	convenient_join_legion_cd = 10;
	const int32 max_hurt_monster_player_num = 500;

	const int32 max_legion_applicant_num = 50;
	const int32 max_legion_event_num = 500;
	const int32 once_legion_event_list_get_num = 10;
	const int32 max_once_legion_event_size = 256;
	const int32 max_legion_boss_record_num = 20;

	const int32 first_legion_template_id = 38000001;
	const int32 legion_shop_template_id = 34000017;

	const int32 city_master_wing_id = 31040044;
	const int32 city_master_buff_id = 10130011;
	const int32 city_master_title_id = 39000011;
	const int32 city_master_mount_id = 0;
	const int32 city_master_union_mount_id = 31030050;
	const int32 lord_legion_mem_mount_id = 0;
	const int32 city_war_winner_title_id = 39000012;

	const int32 city_war_cross_master_title_id = 39000043;
	const int32 city_war_cross_winner_title_id = 39000044;
	
	const int32 max_take_part_in_banquet_per_role = 10;

	const int32 max_apply_city_war_legion_num = 3;
	const int32 city_war_bid_base_price = 10000;
	const int32 city_war_bid_add_price = 1000;
	const int32 city_war_map_id = 71080001;
	const int32 max_glory_glow = std::numeric_limits<int32>::max() - 300000;
	const int32 territory_occupier_price_flag = max_glory_glow + 1;
	const int32 territory_occupier_bid_idx = 0;
	const int32 max_territory_num = 10;
	const int32 max_territory_num_cross_server = 7;
	const int32 min_territory_num_cross_server = 4;

	const int32 all_territory_bid_info_num = (max_territory_num_cross_server + 1) * max_apply_city_war_legion_num;
	const int32 overlord_war_territory_flag = 121000000;
	const int32 legion_construction_num = 4;
	const int32 max_legion_bonus_mission_num = 10;
	const int32 cross_server_territory_war_flag = 121000005;

	const int32 first_legion_bonus_template_id = 132000000;
	const int32	legion_city_master_online_notice_id = 93000008;
	const int32	new_legion_city_master_online_notice_id = 93000284;
	const int32 new_begin_cross_city_war_notice_id = 93000285;
	const int32	legion_city_war_open_notice_id = 93000022;
	const int32	legion_city_dance_open_notice_id = 93000023;
	const int32 legion_send_overload_notice_id = 93000040;
	const int32 legion_add_member_notice_id = 93000047;
	const int32 legion_del_member_notice_id = 93000048;
	const int32 legion_change_job_title_notice_id = 93000049;
	const int32 legion_on_create_bonfire_notice_id = 93000060;
	const int32 legion_occupied_territory_notice_id = 93000061;
	const int32 legion_on_legion_boss_notice_id = 93000062;
	const int32 legion_world_mistress_notice_id = 93000063;
	const int32 legion_world_mistress_open_notice_id = 93000068;
	const int32 legion_maintenance_money_not_enough_notice_id = 93000072;
	const int32 legion_city_maintenance_money_not_enough_notice_id = 93000073;
	const int32 legion_start_legion_boss_map_id = 71060000;
	const int32 legion_donate_notice_id = 93000151;
	const int32 legion_add_bonfire_successfull_notice_id = 93000201;
	const int32 legion_get_bonus_reward_notice_id = 93000230;
	const int32 legion_stimulate_bonus_notice_id = 93000231;
	const int32 legion_member_change_name_notice_id = 93000255;
	const int32 legion_warehouse_store_notice_id = 93000264;
	const int32 legion_warehouse_get_notice_id = 93000265;
	const int32 legion_member_change_legion_name_notice_id = 93000276;
	const int32	legion_cross_server_city_war_open_notice_id = 93000280;

	const int32 legion_boss_cur_legion_notice_id = 93000421;
	const int32 legion_boss_all_legion_notice_id = 93000422;

	const int32 legion_init_announcement_str_id = 90095257;
	const int32 legion_event_member_leave_str_model_id = 90095121;
	const int32 legion_event_member_join_str_model_id = 90095122;
	const int32 legion_event_member_post_change_str_model_id = 90095123;
	const int32 legion_event_donate_money_and_item_str_model_id = 90095124;
	const int32 legion_event_kill_boss_str_model_id = 90095125;
	const int32 legion_event_win_overlord_city_war_str_model_id = 90095128;
	const int32 legion_event_occupy_city_str_model_id = 90095129;
	const int32 legion_event_lose_city_by_war_str_model_id = 90095130;
	const int32 legion_event_lose_city_by_maintain_money_not_enough_str_model_id = 90095131;
	const int32 legion_event_on_create_bonfire_str_model_id = 90095132;
	const int32 legion_event_legion_construction_level_up_str_model_id = 90095133;
	const int32 legion_event_donate_money_and_item_no_battle_achievement_str_model_id = 90095134;
	const int32 legion_event_bid_win_str_model_id = 90095135;
	const int32 legion_event_bid_fail_str_model_id = 90095136;
	const int32 legion_main_construction_name_id = 90090132;

	const int32 first_legion_skill_id = 128000001;
	const int32 finally_legion_skill_id = 128000010;

	const int32 legion_member_change_name_event_id = 90096117;
	
	const int32 first_legion_answer_template_id = 144000001;
	const int32 inform_series_right_nums = 10;
	const int32 inform_all_right_nums = 14;
	const int32 first_right_question_notice_id = 93000256;
	const int32 series_right_question_notice_id = 93000257;
	const int32 all_right_question_notice_id = 93000258;
	const int32 prepare_legion_answer_notice_id = 93000259;
	const int32 start_legion_answer_notice_id = 93000262;

	const int32	first_answer_true_notice_id = 93000400;
	const int32 answer_failure_notice_id = 93000401;
	const int32 play_answer_num_notice_id = 93000402;
	const int32 legion_answer_end_notice_id = 93000403;
	const int32 all_legion_answer_end_one_play_notice_id = 93000404;
	const int32 all_legion_answer_end_two_play_notice_id = 93000405;
	const int32 all_legion_answer_end_three_play_notice_id = 93000406;

	const int32 max_legion_warehouse_info = 100;
	const int32 max_legion_warehouse_log = 20;
	const int32 calculate_legion_average_lv_need_member_num = 20;

	const int32 max_cross_server_all_city_num = 15;
	const int32 max_cross_server_one_group_city_num = 3;
	const int32 max_cross_server_city_group_num = 5;

	const int32 cross_server_territory_city_max = 7;
	const int32 cross_server_legion_call_operate_params_max = 5;
	const int32 cross_server_territory_city_max_params = 11;

	const int32 cross_server_territory_city_bid_info_max = cross_server_territory_city_max * max_apply_city_war_legion_num;

	const int32 cross_server_player_legion_save_time = 180;
	const int32 max_legion_answer_num = 14;
	const int32 rank_show_num = 3;

	const int32 overload_city_war_reward_first_temp_id = 155000001;
	const int32 max_overload_city_war = 15;

	const int32 legion_welfare_activity_award_notice_id = 93000425;
	const int32 legion_welfare_kill_boss_notice_id = 93000426;
	const int32 legion_welfare_grant_num = 1;

	const int32 legion_strategy_world_boss_num = 3;

	enum e_del_overlord_stuff_event
	{
		e_del_overlord_stuff_event_none,
		e_del_overlord_stuff_event_all_legion,
		e_del_overlord_stuff_event_all_member,
		e_del_overlord_stuff_event_only_master,
		e_del_overlord_stuff_event_only_one_member,
		e_del_overlord_stuff_event_max,

	};


	enum e_banquet_level
	{
		e_banquet_level_ordinary,
		e_banquet_level_hearty,
		e_banquet_level_luxury,
		e_banquet_level_max
	};

	enum e_overload_notice_type
	{
		e_overload_notice_type_legion_begin,
		e_overload_notice_type_world_begin,
		e_overload_notice_type_cross_city_war_begin_world,
		e_overload_notice_type_cross_city_war_begin_legion,
		e_overload_notice_type_max
	};

	enum e_overload_city_war_type
	{
		e_overload_city_war_type_team_ready,
		e_overload_city_war_type_start_city_war,
	};

	enum e_city_war_interact_npc
	{
		e_city_war_interact_npc_stronghold = 76300011,
		e_city_war_interact_npc_crystal_s = 76300100,
		e_city_war_interact_npc_crystal_m = 76300101,
		e_city_war_interact_npc_crystal_l = 76300102,
		e_city_war_interact_npc_crystal_xl = 76300103,
	};

	enum ELegionInfo
	{
		ELegionInfo_construction_level_main,
		ELegionInfo_construction_level_1,
		ELegionInfo_construction_level_2,
		ELegionInfo_construction_level_3,
		ELegionInfo_asset_money,
		ELegionInfo_asset_item_1,
		ELegionInfo_asset_item_2,
		ELegionInfo_asset_item_3,
		ELegionInfo_asset_item_4,
		ELegionInfo_fighting_power,
		ELegionInfo_auto_accept_new_member,
		ELegionInfo_last_refresh_time,
		ELegionInfo_glory_glow,
		ELegionInfo_act_score_raid,
		ELegionInfo_act_score_bonfire,
		ELegionInfo_legion_welfare_num,
		ELegionInfo_max
	};
	
	struct s_legion_info
	{
		guid_64	legion_guid;
		xchar	legion_name[max_name_size + 1];
		xchar	announcement[max_legion_announcement_size + 1];
		int64	gs_value;
		i64		data_ary[ELegionInfo_max];

		s_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_info));
			data_ary[ELegionInfo_construction_level_main] = 1;
			data_ary[ELegionInfo_construction_level_1] = 1;
			data_ary[ELegionInfo_construction_level_2] = 1;
			data_ary[ELegionInfo_construction_level_3] = 1;
			data_ary[ELegionInfo_asset_money] = 0;
		}
	};

	enum e_legion_job_title
	{
		e_legion_job_title_none,
		e_legion_job_title_commander,
		e_legion_job_title_assistant_chief,
		e_legion_job_title_chief,
		e_legion_job_title_max
	};

	enum e_legion_log_type
	{
		e_legion_log_type_create,
		e_legion_log_type_del,
		e_legion_log_type_join,
		e_legion_log_type_leave,
		e_legion_log_type_change_name,
		e_legion_log_type_announcement,
		e_legion_log_type_appoint,
		e_legion_log_type_max
	};

	struct s_legion_city_war_member_info
	{
		guid_64  role_guid;
		int64	 use_type;
		xchar    role_name[max_name_size + 1];
		int64	 player_temp_id;
		int64	 week_contribution;
		int64    job_title;
		s_legion_city_war_member_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_city_war_member_info));
		}

		bool is_valid()
		{
			return role_guid.is_valid();
		}

		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
	};

	struct s_legion_city_war_member_info_db
	{
		guid_64							legion_guid;
		s_legion_city_war_member_info	legion_member_info;
	};

	struct s_legion_member_info
	{
		guid_64  role_guid;
		xchar    role_name[max_name_size + 1];
		int64	 gs_value;
		int64    data_ary[e_legion_member_info_max];

		s_legion_member_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_member_info));
		}

		bool is_valid()
		{
			return role_guid.is_valid();
		}

		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}

		bool operator < (const s_legion_member_info& other_info) const 
		{
			return data_ary[e_legion_member_info_week_contribution] < other_info.data_ary[e_legion_member_info_week_contribution];
		}

		bool operator > (const s_legion_member_info& other_info) const
		{
			return data_ary[e_legion_member_info_week_contribution] > other_info.data_ary[e_legion_member_info_week_contribution];
		}

		bool to_proto(faith::st_proto::st_legion_member_info *st_member_ptr)
		{
			if (st_member_ptr == nullptr)
			{
				return false;
			}
			st_member_ptr->set_role_guid(role_guid.server_64);
			st_member_ptr->set_role_name(role_name);
			st_member_ptr->set_gs_value(gs_value);
			for (int32 i = 0; i < e_legion_member_info_max; i++)
			{
				st_member_ptr->add_data_ary(data_ary[i]);
			}
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

		void  from_proto(const faith::st_proto::st_legion_member_info& st_member_ref)
		{
			role_guid.server_64 = st_member_ref.role_guid();
			my_memcopy_string(role_name, max_name_size, st_member_ref.role_name());

			gs_value = st_member_ref.gs_value();
			for (int32 i = 0; i < st_member_ref.data_ary_size(); i++)
			{
				data_ary[i] = st_member_ref.data_ary(i);
			}
		}
	};

	struct s_legion_member_info_db
	{
		guid_64					legion_guid;
		s_legion_member_info	legion_member_info;
	};

	struct s_overload_legion_info_db
	{
		guid_64					legion_guid;
		xchar					city_master_name[max_name_size + 1];
	};

	struct legion_member_score
	{
		guid_64	legion_guid;
		guid_64	member_guid;
		xchar	member_name[max_name_size + 1];
		int32	member_level;
		int32	score;
		int32	server_id;

		legion_member_score()
		{
			clear_data();
		}

		void clear_data()
		{
			legion_guid.clear_data();
			member_guid.clear_data();
			member_level = 0;
			score = 0;
			server_id = 0;
			memset(member_name, 0, max_name_size + 1);
		}

		void set_member_name(xstring role_name)
		{
			memset(member_name, 0, max_name_size + 1);
			memcpy(member_name, role_name.c_str(), role_name.size() > sizeof(member_name) ? sizeof(member_name) : role_name.size());
		}
	};

	struct s_legion_answer_info
	{
		int32					answer_successfully_num;
		guid_64					play_guid[max_legion_answer_num];
		bool					is_answer;
		guid_64					the_answer_play_guid[max_legion_member_num];
		int32					the_answer_play_num;
		s_legion_answer_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_answer_info));
		}
		/*void set_legion_name(const xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), role_name.size() > sizeof(legion_name) ? sizeof(legion_name) : role_name.size());
		}*/
		int32 get_play_answer_num(guid_64 role_guid)
		{
			int32 num = 0;
			for (int32 i = 0; i < max_legion_answer_num; ++i)
			{
				if (play_guid[i] == role_guid)
				{
					++num;
				}
			}
			return num;
		}
		guid_64 get_max_num_play()
		{
			guid_64 role_guid;
			int32 num = 0;
			for (int32 i = 0; i < max_legion_answer_num; ++i)
			{
				if (play_guid[i].is_valid() && get_play_answer_num(play_guid[i]) > num)
				{
					num = get_play_answer_num(play_guid[i]);
					role_guid = play_guid[i];
				}
			}
			return role_guid;
		}
	};

	enum e_legion_apply_stage
	{
		e_legion_apply_stage_send_failed,
		e_legion_apply_stage_send_success,
		e_legion_apply_stage_is_accepted,
		e_legion_apply_stage_is_refused,
		e_legion_apply_stage_auto_accepted,
		e_legion_apply_stage_max
	};

	enum ELegionApplicantInfo
	{
		ELegionApplicantInfo_level,
		ELegionApplicantInfo_player_template_id,
		ELegionApplicantInfo_fighting_power,
		ELegionApplicantInfo_vip_level,
		ELegionApplicantInfo_max
	};

	struct s_legion_applicant_info
	{
		guid_64	legion_guid;
		guid_64	role_guid;
		xchar	role_name[max_name_size + 1];
		float	exist_time;
		int64	gs_value;
		i32		data_ary[ELegionApplicantInfo_max];

		s_legion_applicant_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_applicant_info));
		}

		bool is_valid()
		{
			return role_guid.is_valid();
		}

		void set_applicant_name(xstring _role_name)
		{
			memset(role_name, 0, max_name_size + 1);
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
	};

	enum e_legion_event_type
	{
		e_legion_event_type_none,
		e_legion_event_type_legion_create,
		e_legion_event_type_member_join,
		e_legion_event_type_member_leave,
		e_legion_event_type_donate_money,
		e_legion_event_type_donate_item,
		e_legion_event_type_kill_boss,
		e_legion_event_type_max
	};

	enum e_legion_event_extra_data
	{
		e_legion_event_extra_data_0,
		e_legion_event_extra_data_1,
		e_legion_event_extra_data_2,
		e_legion_event_extra_data_max
	};

	struct s_legion_event
	{
		guid_64				legion_guid;
		int32				event_idx;
		xchar				extra_data[max_once_legion_event_size + 1];
		uint32				happen_time;

		s_legion_event()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_event));
		}

		void set_extra_data(xstring data_str)
		{
			memset(extra_data, 0, sizeof(extra_data));
			memcpy(extra_data, data_str.c_str(), data_str.size() > sizeof(extra_data) ? sizeof(extra_data) : data_str.size());
		}
	};

	enum ELegionBossRecord
	{
		ELegionBossRecord_template_id,
		ELegionBossRecord_unlock,
		ELegionBossRecord_killed,
		ELegionBossRecord_max
	};

	struct s_legion_boss_record
	{
		guid_64	legion_guid;
		guid_64	boss_map_guid;
		xchar	killer_name[max_name_size + 1];
		int32	data_ary[ELegionBossRecord_max];

		s_legion_boss_record()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_boss_record));
		}

		void set_killer_name(const xchar* _killer_name, int32 str_size)
		{
			memcpy(killer_name, _killer_name, max_name_size > str_size ? str_size : max_name_size);
		}

		void clear_killer_name()
		{
			memset(killer_name, '\0', max_name_size + 1);
		}
	};

	enum e_city_war_operate_type
	{
		e_city_war_operate_type_none,
		e_city_war_operate_type_occupy_stronghold,
		e_city_war_operate_type_interrupt_occupy,
		e_city_war_operate_type_finish_occupy,
		e_city_war_operate_type_gather_crystal,
		e_city_war_operate_type_interrupt_gather,
		e_city_war_operate_type_finish_gather,
		e_city_war_operate_type_max
	};
	
	struct s_city_war_legion_score
	{
		guid_64	legion_guid;
		char	legion_name[max_name_size + 1];
		int32	score;
		int32	bid_price;

		s_city_war_legion_score()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_city_war_legion_score));
		}

		bool operator < (const s_city_war_legion_score & other)const
		{
			return score > other.score;
		}
		bool to_proto(faith::st_proto::st_city_war_legion_score* st_city_war_ptr)
		{
			if (st_city_war_ptr == nullptr)
			{
				return false;
			}
			st_city_war_ptr->set_legion_guid(legion_guid.server_64);
			st_city_war_ptr->set_legion_name(legion_name);
			st_city_war_ptr->set_score(score);
			st_city_war_ptr->set_bid_price(bid_price);
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
		void from_proto(const faith::st_proto::st_city_war_legion_score& st_city_war_ref)
		{
			legion_guid.server_64 = st_city_war_ref.legion_guid();
			my_memcopy_string(legion_name, max_name_size, st_city_war_ref.legion_name());
			score = st_city_war_ref.score();
			bid_price = st_city_war_ref.bid_price();
		}
	};

	struct s_city_war_bid_info
	{
		int32	territory_id;
		int32	index_in_terr;
		guid_64	legion_guid;
		int32	cur_bid_price;
		int32	is_prior_maintain;
		int32   occupy_time_stamp;
		int32	server_id;
		xchar	legion_name[max_name_size + 1];

		s_city_war_bid_info()
		{
			clear_data();
			index_in_terr = 0;
			territory_id = 0;
		}

		void clear_data()
		{
			legion_guid.clear_data();
			cur_bid_price = 0;
			is_prior_maintain = 0;
			occupy_time_stamp = 0;
			server_id = 0;
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		bool is_data_valid()
		{
			return territory_id > 0;
		}
		void set_legion_name(xstring m_legion_name) {
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, m_legion_name.c_str(), m_legion_name.size() > sizeof(legion_name) ? sizeof(legion_name) : m_legion_name.size());
		}
	};
	struct s_cross_server_city_war_bid_info
	{
		int32   serverid;
		int32	territory_id;
		int32	index_in_terr;
		guid_64	legion_guid;
		int32	cur_bid_price;
		int32	is_prior_maintain;
		int32   occupy_time_stamp;

		s_cross_server_city_war_bid_info()
		{
			clear_data();
		}

		void clear_data()
		{
			serverid = 0;
			territory_id = 0;
			index_in_terr = 0;
			legion_guid.clear_data();
			cur_bid_price = 0;
			is_prior_maintain = 0;
			occupy_time_stamp = 0;
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		bool is_data_valid()
		{
			return territory_id > 0;
		}
	};
	typedef s_city_war_bid_info s_city_war_bid_info_db;
// 	struct s_city_war_bid_info_db
// 	{
// 		int32				bid_id;
// 		s_city_war_bid_info	bid_info;
// 		s_city_war_bid_info_db()
// 		{
// 			bid_id = 0;
// 		}
// 	};
	
	struct s_city_war_territory_info
	{
		int32				territory_id;
		//guid_64			occupier_legion_guid;
		guid_64				city_war_map_guid;
		uint32				city_war_finish_stamp;
		s_city_war_bid_info	bid_legions[max_apply_city_war_legion_num];

		s_city_war_territory_info()
		{
			clear_data();
			territory_id = 0;
		}

		void clear_data()
		{
			//occupier_legion_guid.clear_data();
			city_war_map_guid.clear_data();
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				bid_legions[i].clear_data();
				bid_legions[i].index_in_terr = i;
			}
			city_war_finish_stamp = 0;
		}
		void set_territory_id(int32 id)
		{
			territory_id = id;
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				bid_legions[i].territory_id = id;
			}
		}
		s_city_war_bid_info* get_bid_info(int32 idx)
		{
			if (idx < 0 || idx >= max_apply_city_war_legion_num)
			{
				return nullptr;
			}
			return &bid_legions[idx];
		}
		s_city_war_bid_info* get_bid_info(guid_64 legion)
		{
			if (!legion.is_valid())
			{
				return nullptr;
			}
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				if (bid_legions[i].legion_guid == legion)
				{
					return &bid_legions[i];
				}
			}
			return nullptr;
		}
		void del_bid_info()
		{
			for (int32 i = 0; i < max_apply_city_war_legion_num; i++)
			{
				bid_legions[i].clear_data();
			}
		}
	};
	typedef std::map<int32, s_city_war_territory_info> territory_lists_type;

	struct s_city_war_legion_rank_info
	{
		int32  server_id;
		guid_64 legion_guid;
		int32	occupy_terr_num;
		int32	occupy_terr_score;
		int32	occupy_terr_highest_score;

		s_city_war_legion_rank_info()
		{
			clear_data();
		}
		void clear_data()
		{
			server_id = 0;
			legion_guid.clear_data();
			occupy_terr_num = 0;
			occupy_terr_score = 0;
			occupy_terr_highest_score = 0;
		}
		bool operator > (const s_city_war_legion_rank_info& info) const
		{
			if (occupy_terr_score != info.occupy_terr_score)
			{
				return occupy_terr_score > info.occupy_terr_score;
			}
			if (occupy_terr_num != info.occupy_terr_num)
			{
				return occupy_terr_num > info.occupy_terr_num;
			}
			return occupy_terr_highest_score > info.occupy_terr_highest_score;
		}
		bool operator < (const s_city_war_legion_rank_info& info) const
		{
			return !(*this > info);
		}
	};
	typedef std::list<s_city_war_legion_rank_info> city_war_rank_type;

	struct s_city_war_member_add_gg
	{
		guid_64 mem_guid;
		int32	mem_got_gg;

		s_city_war_member_add_gg()
		{
			clear_data();
		}
		void clear_data()
		{
			mem_guid.clear_data();
			mem_got_gg = 0;
		}
	};
	typedef std::vector<s_city_war_member_add_gg> legion_mem_gg_info_list;
	typedef std::map<guid_64, legion_mem_gg_info_list> legion_add_gg_info;


	struct s_player_legion_info
	{
		guid_64				legion_guid;
		xchar				legion_name[max_name_size + 1];
		int32				legion_level;
		int32				legion_role_num;
		e_legion_job_title	job_title;
		int32				occupied_territorys[max_territory_num];
		int32				legion_construction_level[legion_construction_num];
		int64               legion_power;

		s_player_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_player_legion_info));
		}

		void set_legion_name(std::string name)
		{
			memcpy(legion_name, name.c_str(), name.size() > max_name_size ? max_name_size : name.size());
		}
		bool to_proto(faith::st_proto::st_player_legion_info *st_legion_ptr)
		{
			if (st_legion_ptr == nullptr)
			{
				return false;
			}
			st_legion_ptr->set_legion_guid(legion_guid.server_64);
			st_legion_ptr->set_legion_name(legion_name);
			st_legion_ptr->set_legion_level(legion_level);
			st_legion_ptr->set_legion_role_num(legion_role_num);
			st_legion_ptr->set_job_title(job_title);
			for (int32 i = 0; i < max_territory_num; i++)
			{
				st_legion_ptr->add_occupied_territorys(occupied_territorys[i]);
			}
			for (int32 i = 0; i < legion_construction_num; i++)
			{
				st_legion_ptr->add_legion_construction_level(legion_construction_level[i]);
			}
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
		void from_proto(const faith::st_proto::st_player_legion_info& st_legion_ref)
		{
			legion_guid.server_64 = st_legion_ref.legion_guid();
			my_memcopy_string(legion_name, max_name_size, st_legion_ref.legion_name());
	
			legion_level = st_legion_ref.legion_level();
			legion_role_num = st_legion_ref.legion_role_num();
			job_title = (e_legion_job_title)st_legion_ref.job_title();
			for (int32 i = 0; i < max_territory_num; i++)
			{
				occupied_territorys[i] = st_legion_ref.occupied_territorys(i);
			}
			for (int32 i = 0; i < legion_construction_num; i++)
			{
				legion_construction_level[i] = st_legion_ref.legion_construction_level(i);
			}
		}
	};

	struct s_cross_player_legion_info
	{
		s_player_legion_info	legion_info;
		int32				save_time_stamp;

		s_cross_player_legion_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_cross_player_legion_info));
		}
	};

	struct s_legion_boss_award_get_log
	{
		guid_64 role_guid;
		int32	get_logs_num;
		int32	get_logs[max_legion_boss_record_num];

		s_legion_boss_award_get_log()
		{
			memset(this, 0, sizeof(s_legion_boss_award_get_log));
		}
		bool to_proto(faith::st_proto::st_legion_boss_award_get_log* st_log_ptr)
		{
			if (st_log_ptr == nullptr)
			{
				return false;
			}
			st_log_ptr->set_role_guid(role_guid.server_64);
			st_log_ptr->set_get_logs_num(get_logs_num);
			for (int32 i = 0; i < max_legion_boss_record_num; i++)
			{
				st_log_ptr->add_get_logs(get_logs[i]);
			}
			return true;
		}
		void from_proto(const faith::st_proto::st_legion_boss_award_get_log& st_log_ref)
		{
			role_guid.server_64 = st_log_ref.role_guid();
			get_logs_num = st_log_ref.get_logs_num();
			for (int32 i = 0 ; i < st_log_ref.get_logs_size(); i++)
			{
				get_logs[i] = st_log_ref.get_logs(i);
			}
		}
	};

	struct s_legion_boss_award_get_log_db
	{
		guid_64	role_guid;
		int32	boss_map_id;

		s_legion_boss_award_get_log_db()
		{
			role_guid.clear_data();
			boss_map_id = 0;
		}
	};

	struct s_legion_member_contribution
	{
		guid_64 role_guid;
		int32   contribution_get;
		s_legion_member_contribution()
		{
			clear_data();
		}
		void clear_data() 
		{
			memset(this, 0, sizeof(s_legion_member_contribution));
		}
	};

	enum e_legion_construction_level_up_error
	{
		e_legion_construction_level_up_error_none,
		e_legion_construction_level_up_error_job_title_not_enough,
		e_legion_construction_level_up_error_invalid_index,
		e_legion_construction_level_up_error_sub_constr_level_not_enough,
		e_legion_construction_level_up_error_sub_constr_level_too_high,
		e_legion_construction_level_up_error_table_data_error,
		e_legion_construction_level_up_error_money_not_enough,
		e_legion_construction_level_up_error_item_not_enougn,
		e_legion_construction_level_up_error_already_top_level,
		e_legion_construction_level_up_error_max
	};

	enum e_legion_error
	{
		e_legion_error_none,
		e_legion_error_template_error,
		e_legion_error_member_full,
		e_legion_error_applicant_list_full,
		e_legion_error_applicant_already_exist,
		e_legion_error_player_added_not_online,
		e_legion_error_player_added_is_legion_member,
		e_legion_error_player_added_is_already_join_other_legion,
		e_legion_error_used_all_take_part_in_banquet_times,
		e_legion_error_banquet_already_over,
		e_legion_error_no_city_war_qualification,
		e_legion_error_city_war_not_begin,
		e_legion_error_can_not_dissolve_during_city_war,
		e_legion_error_can_not_leave_legion_during_city_war,
		e_legion_error_error_legion_name_illegal,
		e_legion_error_can_not_dissolve_during_banquet,
		e_legion_error_can_not_kickout_member_during_city_war,
		e_legion_error_repeat_legion_name,
		e_legion_error_full_legion_num,
		e_legion_error_create_legion_invalid_role_state,
		e_legion_error_create_legion_no_enough_level,
		e_legion_error_create_legion_no_enough_money,
		e_legion_error_invalid_banquet_type,
		e_legion_error_can_cur_time_error,
		e_legion_error_can_not_hold_banquet_in_legion_war_day,
		e_legion_error_invalid_hold_banquet_time,
		e_legion_error_request_hold_banquet_again,
		e_legion_error_already_occupy_stronghold,
		e_legion_error_already_get_occupation_daily_award,
		e_legion_error_next_day_get_occupation_daily_award,
		e_legion_error_city_war_gather_crystal_field_not_open,
		e_legion_error_stronghold_occupied_by_others,
		e_legion_error_crystal_gathered_by_others,
		e_legion_error_job_title_num_max,
		e_legion_error_player_joined_other_legion,
		e_legion_error_is_in_legion_answer_time,
		e_legion_error_max
	};

	enum e_legion_warehouse_error
	{
		e_legion_warehouse_error_none,
		e_legion_unknown_error,
		e_legion_cross_server_error,
		e_legion_not_in_legion_error,
		e_legion_not_has_item_error,
		e_legion_warehouse_is_full,
		e_legion_warehouse_item_type_error,
		e_legion_warehouse_item_equip_quality_error,
		e_legion_warehouse_item_equip_equip_on_error,
		e_legion_warehouse_equip_bag_id_full,
		e_legion_warehouse_score_not_enough,
		e_legion_warehouse_lack_of_authority,
		e_legion_warehouse_none_item_by_destroy,
		e_legion_warehouse_item_tempelete_none,
		e_legion_warehouse_max
	};

	enum e_legion_daily_rwd_flag
	{
		e_legion_daily_rwd_flag_daily_everyone = 30,
		e_legion_daily_rwd_flag_bonfire,
	};

	enum e_legion_gm_order
	{
		e_legion_gm_order_money,
		e_legion_gm_order_achievement,
		e_legion_gm_order_emblem,
		e_legion_gm_order_item1,
		e_legion_gm_order_item2,
		e_legion_gm_order_item3,
		e_legion_gm_order_item4,
		e_legion_gm_order_max
	};

	enum e_legion_get_contribution_type
	{
		e_legion_get_contribution_type_donate_money,
		e_legion_get_contribution_type_donate_item,
		e_legion_get_contribution_type_fuel_bonfire,
		e_legion_get_contribution_type_city_war,
		e_legion_get_contribution_type_legion_boss,
		e_legion_get_contribution_type_add_glory_glow,
		e_legion_get_contribution_type_max
	};

	enum e_legion_act_type
	{
		e_legion_act_type_raid,
		e_legion_act_type_bonfire,
		e_legion_act_type_cross_boss,
		e_legion_act_type_max,
	};

	enum e_legion_rank_type
	{
		e_legion_rank_type_fighting_power,
		e_legion_rank_type_city_war,
		e_legion_rank_type_world_elite,
		e_legion_rank_type_world_boss,
		e_legion_rank_type_raid,
		e_legion_rank_type_bonfire,
		e_legion_rank_type_cross_world_boss,
		e_legion_rank_type_max,
	};
	enum e_legion_warehouse_operation_type
	{
		e_legion_warehouse_operation_store,
		e_legion_warehouse_operation_out,
		e_legion_warehouse_operation_destroy,
	};
	
	struct s_legion_member_attend_activity
	{
		guid_64 mem_legion_guid;
		guid_64 mem_guid;
		int64   mem_score;
		int64   mem_rank_num;
		s_legion_member_attend_activity()
		{
			clear_data();
		}
		void clear_data() 
		{
			memset(this, 0, sizeof(s_legion_member_attend_activity));
		}
	};
	typedef std::map<guid_64, s_legion_member_attend_activity> legion_act_attend_mems_map;

	
	struct s_legion_rank_info
	{
		guid_64				legion_guid;
		xchar				legion_name[max_name_size + 1];
		guid_64				chief_guid;
		xchar				chief_name[max_name_size + 1];
		int64				legion_score;

		s_legion_rank_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(s_legion_rank_info));
		}
	};
	//typedef std::list<s_legion_rank_info> legion_rank_list;
	typedef std::list<s_ranking_player_info> legion_rank_list;

	struct s_legion_bonus_info
	{
		guid_64	legion_guid;		
		int32	mission_id;
		int32	finish_count;

		s_legion_bonus_info()
		{
			clear_data();
			legion_guid.clear_data();
			mission_id = 0;
		}

		void clear_data()
		{

			finish_count = 0;
		}

		bool is_valid()
		{
			return legion_guid.is_valid();
		}

		bool is_data_valid()
		{
			return mission_id > 0;
		}
	};
	struct s_legion_bonus_info_db
	{
		guid_64					legion_guid;
		s_legion_bonus_info		legion_bonus_info;
	};

	enum e_legion_bonus_type
	{
		e_legion_bonus_type_donate_count,
		e_legion_bonus_type_legion_boss_man,
		e_legion_bonus_type_bonfire_count,
		e_legion_bonus_city_war_count,
		e_legion_bonus_world_elite,
		e_legion_bonus_world_boss,
		e_legion_bonus_cross_world_boss,
		e_legion_bonus_type_max,
	};

	enum e_add_applicant_ret
	{
		e_add_applicant_ok,
		e_add_applicant_legion_full,
		e_add_applicant_repeat,
		e_add_applicant_have_legion,
	};

	const int32 legion_job_name[e_legion_job_title_max] =
	{
		90201036,
		90201039,
		90201038,
		90201037,
	};

	struct s_legion_warehouse_info
	{
		guid_64 legion_guid;
		int64 add_time;
		s_item_info	item_info;
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			item_info.clear_data();
		}

		s_legion_warehouse_info()
		{
			clear_data();
		}
	};

	struct s_legion_warehouse_log
	{
		guid_64 role_guid;
		guid_64 item_guid;
		guid_64 legion_guid;
		int64 add_time;
		int32 job;
		xchar role_name[max_name_size + 1];
		int32 operation_type;
		int32 item_template_id;
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		s_legion_warehouse_log()
		{
			clear_data();
		}
	};

	enum e_legion_name_error
	{
		e_legion_name_error_change_name_succeed,
		e_legion_name_error_not_chief,
		e_legion_name_error_formal_error,
		e_legion_name_error_repetition,
		e_legion_name_error_contains_illegal_characters,
		e_legion_name_error_cross_server,
		e_legion_name_error_available,
		e_legion_name_error_player_not_legion,
		e_legion_name_error_item_not_enough,
	};

	enum e_change_cross_city_type
	{
		e_change_cross_city_type_change_chief,
		e_change_cross_city_type_change_legion_name,
		e_change_cross_city_type_del_legion,
	};
	
	enum e_legion_use_contribution_type
	{
		e_legion_use_contribution_type_city_war,
		e_legion_use_contribution_type_legion_welfare,
	};

	struct s_cross_db_city_info
	{
		guid_64 legion_guid;
		int32	server_id;
		guid_64	chief_guid;
		int32	legion_rank;
		xchar	legion_name[max_name_size + 1];
		xchar	chief_name[max_name_size + 1];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_cross_db_city_info()
		{
			clear_data();
		}

		void set_legion_name(xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), role_name.size() > sizeof(legion_name) ? sizeof(legion_name) : role_name.size());
		}
		void set_chief_name(xstring role_name)
		{
			memset(chief_name, 0, sizeof(chief_name));
			memcpy(chief_name, role_name.c_str(), role_name.size() > sizeof(chief_name) ? sizeof(chief_name) : role_name.size());
		}

		bool is_vaild()
		{
			return legion_guid.is_valid();
		}
	};

	struct s_cross_city_info
	{
		guid_64 legion_guid;
		int32	server_id;
		guid_64	chief_guid;
		xchar	legion_name[max_name_size + 1];
		xchar	chief_name[max_name_size + 1];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_cross_city_info()
		{
			clear_data();
		}

		void set_legion_name(xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), role_name.size() > sizeof(legion_name) ? sizeof(legion_name) : role_name.size());
		}
		void set_chief_name(xstring role_name)
		{
			memset(chief_name, 0, sizeof(chief_name));
			memcpy(chief_name, role_name.c_str(), role_name.size() > sizeof(chief_name) ? sizeof(chief_name) : role_name.size());
		}

		bool is_vaild()
		{
			return legion_guid.is_valid();
		}
	};
	struct territory_war_bid_info
	{
		s_city_war_bid_info	bid_infos[cross_server_territory_city_bid_info_max];
		int32 bid_infos_num;
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		territory_war_bid_info()
		{
			clear_data();
		}
	};

	struct s_city_war_all_member_info
	{
		guid_64 legion_guid;
		guid_64	all_member_guid[max_legion_member_num];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		s_city_war_all_member_info()
		{
			clear_data();
		}
	};
#pragma pack(pop)
}

#endif