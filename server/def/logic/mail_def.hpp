/********************************************************************
created: 2016年8月15日
file base: mail
file ext: hpp
author: lxy

purpose: about mail
*********************************************************************/


#ifndef _MAIL_DEF_HPP_
#define _MAIL_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "server_def.hpp"
#include "item_def.hpp"
#include "internet/db.pb.h"

namespace faith
{
#pragma pack(push,1)

	const int32 MAX_MAIL_NUM = 50;											// 最大邮件数量
	const int32 max_text_contents_length = 400;							// 最大文本长度
	const int32 max_mail_title_length = 64;								// 最大标题长度
	const int32 max_item_per_mail = 5;										// 每封邮件最大物品数
	const int32 max_mail_item_num = MAX_MAIL_NUM * max_item_per_mail;		// 邮件物品的最大数量
	const int32 auto_load_mail_time = 300;									// 自动访问数据库更新邮件数据时间
	const int32 check_id_mail_per_time = 100;								// 单次检查ID邮件的处理数量
	const int32 max_other_server_mail_record_num = 2 * max_server_num;		// 其他服务器邮件记录最大数量
	const int32 max_other_server_mail_num = 50;								// 一个其他服存储邮件最大数量
	const int32 max_other_server_mail_item_num = max_other_server_mail_num * max_item_per_mail;	//其他服务器邮件物品最大数量
	enum EMailInfo
	{
		EMailInfo_IsNeedDelete = 0, //对于群发系统邮件 这个是删除时间戳
		EMailInfo_SendTime,
		EMailInfo_ItemGuid11,	//第一个物品GUID前半部分
		EMailInfo_ItemGuid12,	//第一个物品GUID后半部分
		EMailInfo_ItemGuid21,
		EMailInfo_ItemGuid22,
		EMailInfo_ItemGuid31,
		EMailInfo_ItemGuid32,
		EMailInfo_ItemGuid41,
		EMailInfo_ItemGuid42,
		EMailInfo_ItemGuid51,	//第五个物品前半部分
		EMailInfo_ItemGuid52,	//第五个物品后半部分
		EMailInfo_MoneyTyp1,	//12 第一种钱币类型
		EMailInfo_MoneyNum1,	//13 第一种钱币数量
		EMailInfo_MoneyTyp2,
		EMailInfo_MoneyNum2,
		EMailInfo_ReadState,	//1：已读  0：未读
		EMailInfo_SenderGuid1,	//发送者GUID前半部分
		EMailInfo_SenderGuid2,	//发送者GUID后半部分
		EMailInfo_ServerId,		//邮件服务器ID
		EMailInfo_max

	};

	enum EMailOperationTyp
	{
		EMailOperationTyp_Read = 0,
		EMailOperationTyp_GetAndDel,
		EMailOperationTyp_Delete,
		EMailOperationTyp_Force_Delete,
		EMailOperationTyp_Send_Mail,
		EMailOperationTyp_AutoGet,
		EMailOperationTyp_DelReaded,
		EMailOperationTyp_ForceDelAllReaded,
		EMailOperationTyp_max,
	};

	struct s_mail_info
	{
		guid_64					mail_guid;								// guid		
		int32					data_ary[EMailInfo_max];				// 邮件实例数据
		xchar					text_contents[max_text_contents_length + 1];
		xchar					sender_name[max_name_size + 1];
		xchar					mail_title[max_mail_title_length + 1];
		s_mail_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_data_ary(EMailInfo eIndex, int32 value)
		{ 
			if (eIndex>= 0 && eIndex < EMailInfo_max)
			{
				data_ary[eIndex] = value;
			}
		};

		void set_sender_name(xstring p_role_name)
		{
			size_t data_len = p_role_name.size();
			if (data_len > max_name_size)
			{
				data_len = max_name_size;
			}
			memset(sender_name, 0, sizeof(sender_name));
			memcpy(sender_name, p_role_name.c_str(), data_len);
		}

		void set_mail_title(xstring p_title_name)
		{
			size_t data_len = p_title_name.size();
			if (data_len > max_mail_title_length)
			{
				data_len = max_mail_title_length;
			}
			memset(mail_title, 0, sizeof(mail_title));
			memcpy(mail_title, p_title_name.c_str(), data_len);
		}

		void set_mail_content_text(xstring p_content_text)
		{
			size_t data_len = p_content_text.size();
			if (data_len > max_text_contents_length)
			{
				data_len = max_text_contents_length;
			}
			memset(text_contents, 0, sizeof(text_contents));
			memcpy(text_contents, p_content_text.c_str(), data_len);
		}

		bool is_has_money() const
		{
			int32 money_typ1 = data_ary[EMailInfo_MoneyTyp1];
			int32 money_num1 = data_ary[EMailInfo_MoneyNum1];
			int32 money_typ2 = data_ary[EMailInfo_MoneyTyp2];
			int32 money_num2 = data_ary[EMailInfo_MoneyNum2];

			if ((money_num1 > 0) && (money_typ1 < e_money_type_max) && (money_typ1 >= e_money_type_exp))
			{
				return true;
			}

			if ((money_num2 > 0) && (money_typ2 < e_money_type_max) && (money_typ2 >= e_money_type_exp))
			{
				return true;
			}
			return false;
		}
		bool is_has_item() const
		{
			for (int32 i = EMailInfo_ItemGuid11; i <= EMailInfo_ItemGuid52; ++i)
			{
				if (data_ary[i] > 0)
				{
					return true;
				}
			}
			return false;
		}

		bool is_has_item_or_money() const
		{
			return is_has_money() || is_has_item();
		}

		void clear_item_and_money()
		{
			for (int32 i = EMailInfo_MoneyTyp1; i <= EMailInfo_MoneyNum2; ++i)
			{
				data_ary[i] = 0;
			}

			for (int32 i = EMailInfo_ItemGuid11; i <= EMailInfo_ItemGuid52; ++i)
			{
				data_ary[i] = 0;
			}
		}
		bool to_proto(faith::db_proto::role_mail_row* db_mail_ptr)
		{
			if (db_mail_ptr == nullptr)
			{
				return false;
			}
			db_mail_ptr->set_mail_guid(mail_guid.server_64);
		    for (int32 i = 0; i < EMailInfo_max; i++)
		    {
				db_mail_ptr->add_data_ary(data_ary[i]);
		    }
			db_mail_ptr->set_text_contents(text_contents);
			db_mail_ptr->set_sender_name(sender_name);
			db_mail_ptr->set_mail_title(mail_title);
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
		void from_proto(const faith::db_proto::role_mail_row& db_mail_ref)
		{
			mail_guid.server_64 = db_mail_ref.mail_guid();
			for (int32 i = 0; i < db_mail_ref.data_ary_size() && i < EMailInfo_max; i++)
			{
				data_ary[i] = db_mail_ref.data_ary(i);
			}
			my_memcopy_string(text_contents, max_text_contents_length, db_mail_ref.text_contents());
			my_memcopy_string(sender_name, max_name_size, db_mail_ref.sender_name());
			my_memcopy_string(mail_title, max_mail_title_length, db_mail_ref.mail_title());
		}
	};

	struct s_mail_info_db
	{
		guid_64 role_guid;
		s_mail_info data_info;
	};

	struct s_id_mail_info
	{
		guid_64 role_guid;
		int64	create_time;
		int32	show_time;
		int32	mail_id;

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_id_mail_info()
		{
			clear_data();
		}
	};

	struct s_other_server_mail_record
	{
		int32	source_server_id;
		int32	target_server_id;
		guid_64 mail_guid;
		guid_64 role_guid;

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		s_other_server_mail_record()
		{
			clear_data();
		}
		bool is_same_server_id(s_other_server_mail_record mail_record) const
		{
			return source_server_id == mail_record.source_server_id && target_server_id == mail_record.target_server_id;
		}
		bool is_valid() const
		{
			return source_server_id > 0 && target_server_id > 0;
		}
	};

	struct s_other_server_mail_info
	{
		s_mail_info_db mail_info_db;
		s_item_info item_list[max_item_per_mail];
		bool is_valid() const
		{
			return mail_info_db.data_info.mail_guid.is_valid();
		}
		guid_64 get_mail_guid() const
		{
			return mail_info_db.data_info.mail_guid;
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_other_server_mail
	{
		int64 load_tick_time;
		int64 send_tick_time;
		int32 server_id;
		s_other_server_mail_info mail_info[max_other_server_mail_num];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		s_other_server_mail()
		{
			clear_data();
		}
	};

	struct s_other_server_mail_db
	{
		s_mail_info_db mail_info_db[max_other_server_mail_num];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		s_other_server_mail_db()
		{
			clear_data();
		}
	};


	enum e_mail_common_text
	{
		e_mail_common_text_pk_king_title,
		e_mail_common_text_pk_king_context,
		e_mail_common_text_broken_sky_title,
		e_mail_common_text_broken_sky_context,
		e_mail_common_text_city_war_title,
		e_mail_common_text_city_war_win_context,
		e_mail_common_text_city_war_lose_context,
		e_mail_common_text_resource_back_title,
		e_mail_common_text_resource_back_context,
		e_mail_common_text_get_remaining_title,
		e_mail_common_text_get_remaining_context,
		e_mail_common_text_sender_system,
		e_mail_common_text_blessing_title,
		e_mail_common_text_blessing_context,
		e_mail_common_text_auction_purchase_title,
		e_mail_common_text_auction_purchase_content,
		e_mail_common_text_auction_seller_title,
		e_mail_common_text_auction_seller_content,
		e_mail_common_text_auction_cancel_sell_title,
		e_mail_common_text_auction_cancel_sell_content,
		e_mail_common_text_auction_cancel_sell_bag_full_content,
		e_mail_common_text_auction_reward_share_title,
		e_mail_common_text_auction_reward_share_kill_boss_content,
		e_mail_common_text_auction_reward_share_activity_content,
		e_mail_common_text_auction_bid_failed_title,
		e_mail_common_text_auction_bid_failed_content,
		e_mail_common_text_auction_sell_failed_title,
		e_mail_common_text_auction_sell_failed_content,
		e_mail_common_text_force_divorce_title,
		e_mail_common_text_force_divorce_content,
		e_mail_common_text_legion_rank_rwd_title,
		e_mail_common_text_legion_rank_rwd_content,
		e_mail_common_text_active_code_gift_title,
		e_mail_common_text_active_code_gift_context,
		e_mail_common_text_worship_failed_title,
		e_mail_common_text_worship_failed_context,
		e_mail_common_text_join_legion_title,
		e_mail_common_text_join_legion_context,
		e_mail_common_text_daily_mission_bag_full_title,
		e_mail_common_text_daily_mission_bag_full_context,
		e_mail_common_text_against_mission_bag_full_title,
		e_mail_common_text_against_mission_bag_full_context,
		e_mail_common_text_active_recharge_package_title,
		e_mail_common_text_active_recharge_package_context,
		e_mail_common_text_jewel_send_mail_title,
		e_mail_common_text_jewel_send_mail_content,
		e_mail_common_text_arena_reward_mail_title,
		e_mail_common_text_arena_reward_mail_content,
		e_mail_common_text_set_legion_announcement_title,
		e_mail_common_text_set_legion_announcement_context,
		e_mail_common_text_set_legion_job_title,
		e_mail_common_text_set_legion_job_context,
		e_mail_common_text_master_city_war_title,
		e_mail_common_text_master_city_war_context,
		e_mail_common_text_city_bid_title,
		e_mail_common_text_city_bid_context,
		e_mail_common_text_cloud_shop_basic_award_title,
		e_mail_common_text_cloud_shop_basic_award_context,
		e_mail_common_text_cloud_shop_big_award_title,
		e_mail_common_text_cloud_shop_big_award_context,
		e_mail_common_text_cloud_shop_big_buy_failed_title,
		e_mail_common_text_cloud_shop_big_buy_failed_context,
		e_mail_common_text_time_limit_activity_rank_reward_total_recharge_title,
		e_mail_common_text_time_limit_activity_rank_reward_total_recharge_context,
		e_mail_common_text_time_limit_activity_rank_reward_total_cost_title,
		e_mail_common_text_time_limit_activity_rank_reward_total_cost_context,
		e_mail_common_text_time_limit_activity_rank_reward_treasure_title,
		e_mail_common_text_time_limit_activity_rank_reward_treasure_context,
		e_mail_common_text_change_name_inform_legion_member_title,
		e_mail_common_text_change_name_inform_legion_member_content,
		e_mail_common_text_change_name_inform_friend_title,
		e_mail_common_text_change_name_inform_friend_content,
		e_mail_common_text_service_rank_end_time_reward_title,
		e_mail_common_text_service_rank_end_time_reward_context,
		e_mail_common_text_cross_pk_out_16_award_title,
		e_mail_common_text_cross_pk_out_16_award_context,
		e_mail_common_text_cross_pk_16_award_title,
		e_mail_common_text_cross_pk_16_award_context,
		e_mail_common_text_cross_pk_8_award_title,
		e_mail_common_text_cross_pk_8_award_context,
		e_mail_common_text_cross_pk_4_award_title,
		e_mail_common_text_cross_pk_4_award_context,
		e_mail_common_text_cross_pk_2_award_title,
		e_mail_common_text_cross_pk_2_award_context,
		e_mail_common_text_cross_pk_1_award_title,
		e_mail_common_text_cross_pk_1_award_context,
		e_mail_common_text_cross_harry_player_award_title,
		e_mail_common_text_cross_harry_player_award_context,
		e_mail_common_text_cross_harry_server_award_title,
		e_mail_common_text_cross_harry_server_award_context,
		e_mail_common_text_change_legion_name_inform_legion_member_title,
		e_mail_common_text_change_legion_name_inform_legion_member_content,
		e_mail_common_text_cross_city_rank_award_title,
		e_mail_common_text_cross_city_rank_award_context,
		e_mail_common_text_cross_city_rank_award_title_chief,
		e_mail_common_text_cross_city_rank_award_context_chief,
		e_mail_common_text_cross_ladder_refund_title,
		e_mail_common_text_cross_ladder_refund_context,
		e_mail_common_text_cross_ladder_reward_title,
		e_mail_common_text_cross_ladder_reward_context,
		e_mail_common_text_cross_ladder_season_reward_title,
		e_mail_common_text_cross_ladder_season_reward_context,
		e_mail_common_text_cross_ladder_rank_reward_title,
		e_mail_common_text_cross_ladder_rank_reward_context,
		e_mail_common_text_skill_bag_full_send_mail_title,
		e_mail_common_text_skill_bag_full_send_mail_content,
		e_mail_common_text_cross_city_war_send_mail_title,
		e_mail_common_text_cross_city_war_send_mail_content,
		e_mail_common_text_cross_server_terrritory_war_title,
		e_mail_common_text_cross_server_terrritory_war_content,
		e_mail_common_text_time_limit_activity_rank_reward_gs_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_gs_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_mount_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_mount_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_wing_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_wing_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_equip_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_equip_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_spirit_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_spirit_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_cross_gs_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_cross_mount_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_cross_wing_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_cross_equip_up_context,
		e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_title,
		e_mail_common_text_time_limit_activity_rank_reward_cross_spirit_up_context,
		e_mail_common_text_couple_bless_title,
		e_mail_common_text_couple_bless_context,
		e_mail_common_text_couple_marry_title,
		e_mail_common_text_couple_marry_context,
		e_mail_common_text_couple_divorce_title,
		e_mail_common_text_couple_divorce_context,
		e_mail_common_text_oracle_trial_funben_over_title,
		e_mail_common_text_oracle_trial_funben_over_content_one,
		e_mail_common_text_oracle_trial_funben_over_content_two,
		e_mail_common_text_oracle_trial_funben_over_content_three,
		e_mail_common_text_cross_ladder_upgrade_title,
		e_mail_common_text_cross_ladder_upgrade_context,
		e_mail_common_text_overload_city_war_begin_title,
		e_mail_common_text_overload_city_war_begin_context,
		e_mail_common_text_overload_city_war_end_title,
		e_mail_common_text_overload_city_war_end_context,
		e_mail_common_text_cross_overload_city_war_begin_title,
		e_mail_common_text_cross_overload_city_war_begin_context,
		e_mail_common_text_cross_overload_city_war_end_title,
		e_mail_common_text_cross_overload_city_war_end_context,
		e_mail_common_text_city_rank_award_title_chief,
		e_mail_common_text_city_rank_award_context_chief,
		e_mail_common_overload_city_war_end_title,
		e_mail_common_overload_city_war_end_context,
		e_mail_common_cross_ladder_season_begin_title,
		e_mail_common_cross_ladder_season_begin_context,
		e_mail_common_text_auction_time_out_title,
		e_mail_common_text_auction_time_out_content,
		e_mail_common_text_help_reward_title,
		e_mail_common_text_help_reward_content,
		e_mail_common_text_helper_reward_title,
		e_mail_common_text_helper_reward_content,
		e_mail_common_text_city_war_win_reward_title,
		e_mail_common_text_city_war_win_reward_content,
		e_mail_common_cross_ladder_season_ranking_reward_title,
		e_mail_common_cross_ladder_season_ranking_reward_context,
		e_mail_common_legion_welfare_reward_title,
		e_mail_common_legion_welfare_reward_context,
		e_mail_common_text_element_war_season_reward_title,
		e_mail_common_text_element_war_season_reward_context,
		e_mail_common_element_war_season_begin_title,
		e_mail_common_element_war_season_begin_context,
		e_mail_common_element_war_season_ranking_reward_title,
		e_mail_common_element_war_season_ranking_reward_context,
		e_mail_common_text_element_war_upgrade_title,
		e_mail_common_text_element_war_upgrade_context,
		e_mail_common_text_treasure_clear_title,
		e_mail_common_text_treasure_clear_context,
		e_mail_common_text_world_boss_strategy_reward_title,
		e_mail_common_text_world_boss_strategy_reward_context,
		e_mail_common_text_time_limit_activity_rank_reward_cross_total_recharge_title,
		e_mail_common_text_time_limit_activity_rank_reward_cross_total_recharge_context,
		e_mail_common_text_time_attack_city_group_up_title,
		e_mail_common_text_time_attack_city_group_up_context,
		e_mail_common_text_time_attack_city_singl_reward_title,
		e_mail_common_text_time_attack_city_singl_reward_context,
		e_mail_common_text_max //通过这个枚举可以找到字典号配置的地方
	};

#pragma pack(pop)
}

#endif