#ifndef _FAITH_LEGION_MSG_HPP_
#define _FAITH_LEGION_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/legion_def.hpp"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		// 军团信息存读消息头
		e_msgindex_ws2dp_load_all_legion_info = faith::e_msg_base_legion,
		e_msgindex_dp2ws_load_all_legion_info,
		e_msgindex_ws2dp_save_legion_info,
		e_msgindex_ws2dp_delete_legion_info,
		e_msgindex_ws2dp_delete_legion_member_info,
		e_msgindex_ws2dp_delete_legion_city_war_member_info,
		// 军团成员信息存读消息头
		e_msgindex_ws2dp_load_all_legion_member_info,
		e_msgindex_ws2dp_load_all_legion_city_war_member_info,
		e_msgindex_dp2ws_load_all_legion_member_info,
		e_msgindex_dp2ws_load_all_legion_city_war_member_info,
		e_msgindex_ws2dp_save_legion_member_info,
		e_msgindex_ws2dp_save_legion_city_war_member_info,

		// 军团入团申请信息存读消息头
		e_msgindex_ws2dp_load_all_legion_applicant,
		e_msgindex_dp2ws_load_all_legion_applicant,
		e_msgindex_ws2dp_save_legion_applicant,
		e_msgindex_ws2dp_del_legion_applicant_one,

		// 军团BOSS记录的信息存读消息头
		e_msgindex_ws2dp_load_all_legion_boss_record,
		e_msgindex_dp2ws_load_all_legion_boss_record,
		e_msgindex_ws2dp_save_legion_boss_record,
		e_msgindex_ws2dp_clear_legion_boss_record,

		// 军团BOSS奖励领取记录
		e_msgindex_ws2dp_load_all_legion_boss_award_get_log,
		e_msgindex_dp2ws_load_all_legion_boss_award_get_log,
		e_msgindex_ws2dp_save_legion_boss_award_get_log,

		// 王城信息的存档
		e_msgindex_ws2dp_load_city_info,
		e_msgindex_dp2ws_load_city_info,
		e_msgindex_ws2dp_save_city_info,
		e_msgindex_ws2dp_load_all_war_bid_info,
		e_msgindex_dp2ws_load_all_war_bid_info,
		e_msgindex_ws2dp_save_war_bid_info,
		e_msgindex_ws2dp_clear_war_bid_info,
		e_msgindex_ws2dp_clear_overlord_legion_stuff,

		// 军团事件的存档
		e_msgindex_ws2dp_load_all_legion_event,
		e_msgindex_dp2ws_load_all_legion_event,
		e_msgindex_ws2dp_save_legion_event,
		e_msgindex_ws2dp_del_legion_event,
		e_msgindex_ws2dp_clear_legion_event,

		//军团分红的存档
		e_msgindex_ws2dp_save_legion_bonus_info,
		e_msgindex_dp2ws_load_legion_bonus_info,
		e_msgindex_ws2dp_load_legion_bonus_info,
		e_msgindex_ws2dp_clear_legion_bonus_info,

		//军团仓库
		e_msgindex_dp2ws_load_legion_warehouse_info,
		e_msgindex_ws2dp_store_legion_warehouse,
		e_msgindex_ws2dp_store_legion_warehouse_log,
		e_msgindex_ws2dp_role_get_legion_warehouse,
		e_msgindex_ws2dp_destroy_legion_warehouse_item,
		e_msgindex_ws2dp_clear_legion_warehouse,
		e_msgindex_ws2dp_load_legion_warehouse_log,
		e_msgindex_dp2ws_load_legion_warehouse_log,

		//跨服霸主战
		e_msgindex_ws2dp_load_cross_city_info,
		e_msgindex_dp2ws_load_cross_city_info_all,
		e_msgindex_gws2ws_legion_rank_with_city_war,
		e_msgindex_gws2ws_cross_city_info_all,
		e_msgindex_ws2dp_save_cross_city_info,
		e_msgindex_ws2gws_change_cross_city,
		e_msgindex_gws2ws_overload_begin_notice,
		e_msgindex_ws2dp_clear_cross_city_info,
		//跨服城战相关

		//竞价
		e_msgindex_ws2ws_legion_proto_apply_city_war_bid,
		e_msgindex_ws2ws_legion_proto_apply_city_war_bid_response,
		e_msgindex_ws2ws_legion_proto_apply_city_war_bid_end,
		e_msgindex_ws2ws_legion_proto_return_city_war_bid_money,

		e_msgindex_ws2ws_legion_proto_push_city_war_info,
		//优先维护
		e_msgindex_ws2ws_set_territory_prior_maintain,
		e_msgindex_ws2ws_set_territory_prior_maintain_response,
		e_msgindex_ws2wsreq_legion_send_territory_info,

		e_msgindex_ws2wsreq_legion_get_city_war_info,

		e_msgindex_ws2wsreq_legion_get_city_war_info_end,

		e_msgindex_ws2wsreq_get_occupation_daily_award,

		e_msgindex_ws2wsreq_get_occupation_daily_award_end,

		e_msgindex_ws2wssend_cross_war_begin_notice,

		e_msgindex_ws2ws_maintain_terrories,

		e_msgindex_ws2ws_end_one_territory_to_all_mem,

		e_msgindex_ws2ws_end_war_info,

		e_msgindex_ws2ws_refresh_reward_info,

		e_msgindex_ws2ws_bid_info_to_ws,

		e_msgindex_ws2ws_clear_bid_info_to_gate,

		e_msgindex_ws2ws_change_gate_legion_name,
	};

	struct ws2dp_load_all_legion_info : public packet_base
	{
		ws2dp_load_all_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_all_legion_info;
		}
	};

	struct dp2ws_load_all_legion_info : public packet_base
	{
		bool			is_end;
		int32			legion_info_num;
		s_legion_info	legion_info[db_legion_count];

		dp2ws_load_all_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_legion_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&legion_info - (ULONG_PTR)&wheader;
			return (basic_len + legion_info_num * sizeof(s_legion_info));
		}
	};

	struct ws2dp_save_legion_info : public packet_base
	{
		s_legion_info	legion_info;

		ws2dp_save_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_legion_info;
		}
	};

	struct ws2dp_delete_legion_info : public packet_base
	{
		guid_64 legion_guid;

		ws2dp_delete_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_delete_legion_info;
		}
	};

	struct ws2dp_delete_legion_member_info : public packet_base
	{
		guid_64 role_guid;

		ws2dp_delete_legion_member_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_delete_legion_member_info;
		}
	};

	struct ws2dp_delete_legion_city_war_member_info : public packet_base
	{
		int32 del_type;
		ws2dp_delete_legion_city_war_member_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_delete_legion_city_war_member_info;
		}
	};

	struct dp2ws_load_all_legion_city_war_member_info : public packet_base
	{
		guid_64					legion_guid;
		int32					member_info_num;
		s_legion_city_war_member_info	member_info[max_legion_member_num];

		dp2ws_load_all_legion_city_war_member_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_legion_city_war_member_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&member_info - (ULONG_PTR)&wheader;
			return (basic_len + member_info_num * sizeof(s_legion_city_war_member_info));
		}
	};

	struct dp2ws_load_all_legion_member_info : public packet_base
	{
		guid_64					legion_guid;
		int32					member_info_num;
		s_legion_member_info	member_info[max_legion_member_num];

		dp2ws_load_all_legion_member_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_legion_member_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&member_info - (ULONG_PTR)&wheader;
			return (basic_len + member_info_num * sizeof(s_legion_member_info));
		}
	};

	struct ws2dp_save_legion_member_info : public packet_base
	{
		guid_64					legion_guid;
		s_legion_member_info	legion_member_info;

		ws2dp_save_legion_member_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_legion_member_info;
		}
	};

	struct ws2dp_save_legion_city_war_member_info : public packet_base
	{
		guid_64							legion_guid;
		s_legion_city_war_member_info	legion_member_info;

		ws2dp_save_legion_city_war_member_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_legion_city_war_member_info;
		}
	};
	struct dp2ws_load_all_legion_applicant : public packet_base
	{
		int32					applicant_info_num;
		s_legion_applicant_info	applicant_info[max_legion_applicant_num];

		dp2ws_load_all_legion_applicant()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_legion_applicant;
		}
	};

	struct ws2dp_save_legion_applicant : public packet_base
	{
		s_legion_applicant_info	applicant_info;

		ws2dp_save_legion_applicant()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_legion_applicant;
		}
	};
	struct ws2dp_clear_legion_boss_record : public packet_base
	{
		guid_64 legion_guid;

		ws2dp_clear_legion_boss_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_legion_boss_record;
		}
	};


	struct ws2dp_load_all_legion_boss_award_get_log : public packet_base
	{
		guid_64	role_guid;

		ws2dp_load_all_legion_boss_award_get_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_all_legion_boss_award_get_log;
		}
	};

	struct dp2ws_load_all_legion_boss_award_get_log : public packet_base
	{
		s_legion_boss_award_get_log legion_boss_award_get_log;

		dp2ws_load_all_legion_boss_award_get_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_legion_boss_award_get_log;
		}
	};

	struct ws2dp_save_legion_boss_award_get_log : public packet_base
	{
		guid_64	role_guid;
		int32	boss_map_id;

		ws2dp_save_legion_boss_award_get_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_legion_boss_award_get_log;
		}
	};

	struct dp2ws_load_city_info : public packet_base
	{
		s_overload_legion_info_db overload_info;
		dp2ws_load_city_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_city_info;
		}
	};

	struct ws2dp_save_city_info : public packet_base
	{
		guid_64	occupation_legion_guid;
		xchar city_master_name[max_name_size + 1];

		ws2dp_save_city_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_city_info;
		}
	};
	struct dp2ws_load_all_war_bid_info : public packet_base
	{
		int32				data_num;
		s_city_war_bid_info	bid_infos[all_territory_bid_info_num];

		dp2ws_load_all_war_bid_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_war_bid_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&bid_infos - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_city_war_bid_info));
		}
	};

	struct ws2dp_save_war_bid_info : public packet_base
	{
		s_city_war_bid_info	bid_info;

		ws2dp_save_war_bid_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_war_bid_info;
		}
	};

	struct ws2dp_clear_war_bid_info : public packet_base
	{
		ws2dp_clear_war_bid_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_war_bid_info;
		}
	};

	struct ws2dp_clear_overlord_legion_stuff : public packet_base
	{
		int32				del_stuff_type;
		guid_64				target_guid;

		ws2dp_clear_overlord_legion_stuff()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_overlord_legion_stuff;
		}
	};

	struct dp2ws_load_all_legion_event : public packet_base
	{
		int32				data_num;
		s_legion_event		event_datas[max_legion_event_num];

		dp2ws_load_all_legion_event()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_all_legion_event;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&event_datas - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_legion_event));
		}
	};

	struct ws2dp_save_legion_event : public packet_base
	{
		s_legion_event	event_data;
		ws2dp_save_legion_event()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_legion_event;
		}
	};

	struct ws2dp_del_legion_event : public packet_base
	{
		guid_64	legion_guid;
		int32	event_idx;
		ws2dp_del_legion_event()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_del_legion_event;
		}
	};

	struct ws2dp_clear_legion_event : public packet_base
	{
		guid_64	legion_guid;
		ws2dp_clear_legion_event()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_legion_event;
		}
	};

	struct dp2ws_load_legion_bonus_info : public packet_base
	{
		int32 data_num;
		s_legion_bonus_info	bonus_info[max_legion_bonus_mission_num*max_legion_count];

		dp2ws_load_legion_bonus_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_legion_bonus_info;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&bonus_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_legion_bonus_info));
		}
	};

	struct ws2dp_save_legion_bonus_info : public packet_base
	{
		s_legion_bonus_info	bonus_info;

		ws2dp_save_legion_bonus_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_legion_bonus_info;
		}
	};

	struct ws2dp_clear_legion_bonus_info : public packet_base
	{
		guid_64	legion_guid;
		ws2dp_clear_legion_bonus_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_legion_bonus_info;
		}
	};

	struct ws2dp_del_legion_applicant_one : public packet_base
	{
		guid_64	role_guid;
		ws2dp_del_legion_applicant_one()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_del_legion_applicant_one;
		}
	};

	struct dp2ws_load_all_legion_warehouse_info : public packet_base
	{
		int32					legion_warehouse_num;
		s_legion_warehouse_info	legion_warehouse[max_legion_warehouse_info];

		dp2ws_load_all_legion_warehouse_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_legion_warehouse_info;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&legion_warehouse - (ULONG_PTR)&wheader;
			return (basic_len + legion_warehouse_num * sizeof(s_legion_warehouse_info));
		}
	};

	struct dp2ws_load_all_legion_warehouse_log : public packet_base
	{
		int32					log_num;
		s_legion_warehouse_log	log[max_legion_warehouse_log];

		dp2ws_load_all_legion_warehouse_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_legion_warehouse_log;
		}

		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&log - (ULONG_PTR)&wheader;
			return (basic_len + log_num * sizeof(s_legion_warehouse_log));
		}
	};


	struct ws2dp_store_legion_warehouse_info : public packet_base
	{
		s_legion_warehouse_info legion_info;

		ws2dp_store_legion_warehouse_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_store_legion_warehouse;
		}
	};
	struct ws2dp_store_legion_warehouse_log : public packet_base
	{
		s_legion_warehouse_log log;

		ws2dp_store_legion_warehouse_log()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_store_legion_warehouse_log;
		}
	};
	struct ws2dp_role_get_legion_warehouse_info : public packet_base
	{
		guid_64 role_guid;
		s_legion_warehouse_info legion_info;
		s_legion_warehouse_log log;

		ws2dp_role_get_legion_warehouse_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_role_get_legion_warehouse;
		}
	};

	struct ws2dp_destroy_legion_item : public packet_base
	{
		int32 del_num;
		guid_64 del_guid_array[max_legion_warehouse_info];
		int32 del_log_num;
		int64 del_item_id[max_legion_warehouse_log];
		s_legion_warehouse_log log;
		ws2dp_destroy_legion_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_destroy_legion_warehouse_item;
		}
	};

	struct ws2dp_clear_legion_warehouse :public packet_base
	{
		guid_64 legion_guid;
		ws2dp_clear_legion_warehouse()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_legion_warehouse;
		}
	};

	struct ws2dp_load_cross_city_info :public packet_base
	{
		ws2dp_load_cross_city_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_cross_city_info;
		}
	};

	struct dp2ws_load_cross_city_info_all : public faith::packet_base
	{
		bool is_empty_msg;
		int32 data_num;
		s_cross_city_info	city_info_msg[max_cross_server_all_city_num];
		dp2ws_load_cross_city_info_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_cross_city_info_all;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&city_info_msg - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_cross_city_info));
		}
	};

	struct ws2dp_save_cross_city_info : public packet_base
	{
		s_cross_city_info	city_info_msg;
		int32				rank_idex;
		ws2dp_save_cross_city_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_cross_city_info;
		}
	};

	struct ws2dp_clear_cross_city_info : public packet_base
	{
		ws2dp_clear_cross_city_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_cross_city_info;
		}
	};

	struct gws2ws_legion_rank_with_cross_city_war : public faith::packet_base
	{
		guid_64 legion_guid;
		int32	rank_idex;
		guid_64	member_guid_all[max_legion_member_num];
		int32	member_num;
		gws2ws_legion_rank_with_cross_city_war()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_gws2ws_legion_rank_with_city_war;
		}
	};

	struct gws2ws_overload_city_begin_notice : public faith::packet_base
	{
		guid_64 legion_guid;
		int32	 group_num;
		int32	notice_type;
		gws2ws_overload_city_begin_notice()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_gws2ws_overload_begin_notice;
		}
	};

	struct gws2ws_cross_city_info_all : public faith::packet_base
	{
		s_cross_city_info city_info_all[max_cross_server_all_city_num];
		int32 data_num;
		gws2ws_cross_city_info_all()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_gws2ws_cross_city_info_all;
		}
	};

	struct ws2gws_change_cross_city_chief : public faith::packet_base
	{
		guid_64 leigon_guid;
		xchar role_name[max_name_size + 1];
		guid_64 role_guid;
		int32	change_type;
		ws2gws_change_cross_city_chief()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2gws_change_cross_city;
		}
	};

	//竞价
	struct ws2ws_legion_proto_apply_city_war_bid :public packet_base
	{
		int32 terr_id;
		int32 bid_index;
		guid_64 role_guid;
		guid_64 legion_guid;
		int32 serverid;
		ws2ws_legion_proto_apply_city_war_bid()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_legion_proto_apply_city_war_bid;
		}
	};
	struct ws2ws_legion_proto_apply_city_war_bid_response :public packet_base
	{
		int32 terr_id;
		int32 bid_index;
		int32  result;
		int32  needprice;
		guid_64 role_guid;
		guid_64 old_legion_guid;
		int32 next_bid_price;

		ws2ws_legion_proto_apply_city_war_bid_response()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_legion_proto_apply_city_war_bid_response;
		}
	};
	struct ws2ws_legion_proto_apply_city_war_bid_end :public packet_base
	{
		int32 terr_id;
		int32 bid_index;
		guid_64 legion_guid;
		guid_64 role_guid;
		int32 next_bid_price;
		int32 server_id;
		xchar legion_name[max_name_size + 1];
		int32 need_price;
		ws2ws_legion_proto_apply_city_war_bid_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_legion_proto_apply_city_war_bid_end;
		}
	};
	//优先维护
	struct ws2ws_set_territory_prior_maintain :public packet_base
	{
		int32 territory_ids[cross_server_territory_city_max];
		int32 serverid;
		guid_64 legin_guid;
		guid_64 role_guid;
		int32 territory_ids_num;
		ws2ws_set_territory_prior_maintain()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_set_territory_prior_maintain;
		}
	};
	struct ws2ws_set_territory_prior_maintain_response :public packet_base
	{
		int32 territory_ids[cross_server_territory_city_max];
		bool prior_rets[cross_server_territory_city_max];
		guid_64 role_guid;
		int32 territory_ids_num;
		ws2ws_set_territory_prior_maintain_response()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_set_territory_prior_maintain_response;
		}
	};
	struct ws2wsreq_legion_send_territory_info :public packet_base
	{
		guid_64 role_guid;
		s_city_war_territory_info info;
		guid_64	legion_guid;
		int32	need_price;
		ws2wsreq_legion_send_territory_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2wsreq_legion_send_territory_info;
		}
	};


	struct ws2wsreq_legion_get_city_war_info :public packet_base
	{
		guid_64 role_guid;
		int32 server_id;
		ws2wsreq_legion_get_city_war_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2wsreq_legion_get_city_war_info;
		}
	};

	struct ws2wsreq_legion_get_city_war_info_end :public packet_base
	{
		guid_64 role_guid;
		s_city_war_territory_info war_info[cross_server_territory_city_max + max_territory_num];
		ws2wsreq_legion_get_city_war_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2wsreq_legion_get_city_war_info_end;
		}
	};
	struct ws2wsreq_get_occupation_daily_award :public packet_base
	{
		guid_64 role_guid;
		guid_64 legion_guid;
		int32 terr_id;
		int32 server_id;
		ws2wsreq_get_occupation_daily_award()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2wsreq_get_occupation_daily_award;
		}
	};
	struct ws2wsreq_get_occupation_daily_award_end :public packet_base
	{
		guid_64 role_guid;
		guid_64 legion_guid;
		int32 terr_id;
		int32 notice;
		ws2wsreq_get_occupation_daily_award_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2wsreq_get_occupation_daily_award_end;
		}
	};
	struct ws2wssend_cross_war_begin_notice :public packet_base
	{
		ws2wssend_cross_war_begin_notice()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2wssend_cross_war_begin_notice;
		}
	};
	struct ws2ws_maintain_terrories :public packet_base
	{
		ws2ws_maintain_terrories()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_maintain_terrories;
		}
	};
	struct ws2ws_end_one_territory_to_all_mem :public packet_base
	{
		guid_64 legion_guid;
		int32 territory_id;
		bool need_send_winner_msg;
		ws2ws_end_one_territory_to_all_mem()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_end_one_territory_to_all_mem;
		}
	};
	struct ws2ws_end_war_info :public packet_base
	{
		guid_64 legion_guid;
		int32 territory_id;
		bool is_winner;
		bool is_defense_win;
		ws2ws_end_war_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_end_war_info;
		}
	};
	struct ws2ws_refresh_reward_info :public packet_base
	{
		guid_64 legion_guid;
		ws2ws_refresh_reward_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_refresh_reward_info;
		}
	};

	struct ws2ws_bid_info_to_ws :public packet_base
	{
		s_city_war_bid_info	bid_infos[cross_server_territory_city_bid_info_max];
		guid_64 legion_guid;
		int32 bid_infos_num;
		ws2ws_bid_info_to_ws()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_bid_info_to_ws;
		}
	};
	struct ws2ws_clear_bid_info_to_gate :public packet_base
	{
		int32 territory_id;
		guid_64 legion_guid;
		ws2ws_clear_bid_info_to_gate()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_clear_bid_info_to_gate;
		}
	};
	struct ws2ws_legion_proto_return_city_war_bid_money :public packet_base
	{
		guid_64 legion_guid;
		int32 price;
		int32 terri_id;
		ws2ws_legion_proto_return_city_war_bid_money()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_legion_proto_return_city_war_bid_money;
		}
	};
	struct ws2ws_legion_proto_push_city_war_info :public packet_base
	{
		s_city_war_territory_info city_info;
		guid_64 legion_guid;
		ws2ws_legion_proto_push_city_war_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_legion_proto_push_city_war_info;
		}
	};

	struct ws2ws_change_gate_legion_name :public packet_base
	{
		guid_64 legion_guid;
		xchar   legion_name[max_name_size + 1];
		ws2ws_change_gate_legion_name()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_change_gate_legion_name;
		}
		void set_legion_name(xstring role_name)
		{
			memset(legion_name, 0, sizeof(legion_name));
			memcpy(legion_name, role_name.c_str(), sizeof(legion_name) > role_name.size() ? role_name.size() : sizeof(legion_name));
		}
	};
#pragma pack(pop)
}

#endif
