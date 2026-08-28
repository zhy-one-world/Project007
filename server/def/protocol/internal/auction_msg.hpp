/********************************************************************
created: 2016/10/17
file base: world boss
file ext: hpp
author: luoxingyu

purpose:
*********************************************************************/
#ifndef _FAITH_AUCTION_HPP_
#define _FAITH_AUCTION_HPP_

#include "logic/type_def.hpp"
#include "Logic/auction_def.h"
#include "Logic/char_def.hpp"
#include "Logic/login_def.hpp"
#include "base.hpp"
#include "internet/dp2s.pb.h"
#include "internet/cs2ws.pb.h"
#include "internet/ws2cs.pb.h"

namespace hld
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_cs2ws_add_auction_into_db = hld::e_msg_base_auction,
		e_msgindex_ws2dp_add_auction_into_db,
		e_msgindex_dp2ws_auction_sell_end,
		e_msgindex_ws2cs_auction_sell_end,
		e_msgindex_cs2ws_auction_buy,
		e_msgindex_ws2dp_auction_find_buy_target,
		e_msgindex_dp2ws_auction_find_buy_target_end,
		e_msgindex_ws2cs_auction_purchase_success,
		e_msgindex_dp2cs_auction_load_self_selling_end,
		e_msgindex_cs2ws_auction_add_purchase_record,
		e_msgindex_ws2dp_auction_add_purchase_record,
		e_msgindex_cs2ws_auction_sell_success_info,
		e_msgindex_ws2cs_auction_sell_success_info,
		e_msgindex_cs2ws_auction_cancel_sell,
		e_msgindex_ws2dp_auction_cancel_sell,
		e_msgindex_dp2ws_auction_cancel_sell,
		e_msgindex_ws2cs_auction_cancel_sell,
		e_msgindex_cs2dp_auction_req_goods_list,
		e_msgindex_dp2cs_auction_req_goods_list_end,
		e_msgindex_cs2dp_auction_req_trade_record,
		e_msgindex_dp2cs_auction_req_trade_record_end,
		e_msgindex_cs2dp_auction_req_another_sell_info,
		e_msgindex_dp2cs_auction_req_another_sell_info_end,
		e_msgindex_cs2ws_auction_share_sell_reward,

		e_msgindex_cs2ws_auction_bid,
		e_msgindex_ws2dp_auction_find_bid_target,
		e_msgindex_dp2ws_auction_find_bid_target_end,
		e_msgindex_ws2cs_auction_find_bid_target_end,

		e_msgindex_cs2ws_clear_selling_lock,
		e_msgindex_cs2ws_bid_success,
		e_msgindex_ws2dp_bid_success,
		e_msgindex_dp2ws_clear_selling_lock,

		e_msgindex_ws2dp_auction_clear_old_item,
		e_msgindex_dp2ws_auction_clear_old_item_end,

		e_msgindex_ws2dp_auction_add_share_list,

		e_msgindex_ws2dp_auction_req_share_list,
		e_msgindex_dp2ws_auction_req_share_list_end,

		e_msgindex_cs2dp_auction_req_self_bid_list,

		e_msgindex_ws2dp_auction_check_legion_selling,
		e_msgindex_dp2ws_auction_check_legion_selling_end,

		e_msgindex_cs2ws_auction_del_bid_record,
		e_msgindex_ws2dp_auction_del_bid_record,
		e_msgindex_ws2dp_auction_clear_old_bid_record,

		e_msgindex_ws2dp_auction_check_world_sell,
		e_msgindex_dp2ws_auction_check_world_sell_end,

		e_msgindex_ws2dp_auction_show_sell,
		e_msgindex_dp2ws_auction_show_sell_end,

		e_msgindex_ws2dp_auction_time_out,
		e_msgindex_dp2ws_auction_time_out_end,

		e_msgindex_cs2dp_req_bid_record_failure,
		e_msgindex_dp2cs_req_bid_record_failure_end,

		e_msgindex_ws2cs_bid_replace_info,
		e_msgindex_ws2cs_time_out_del,

		e_msgindex_cs2dp_req_bid_notice,
		e_msgindex_dp2cs_req_bid_notice_end,
		e_msgindex_dp2cs_req_bid_notice_red_end,
		e_msgindex_dp2ws_auction_bid_db_sucess,

		e_msgindex_cs2ws_auction_refresh_person,

	};

	struct cs2ws_auction_add_item_into_db : public packet_base
	{
		s_unit_info			role_info;
		s_auction_info		auction_info;
		guid_64				old_item_guid;
		login_fixed_data	third_info;
		cs2ws_auction_add_item_into_db()
		{
			memset(this, 0, sizeof(*this));
			auction_info.clear_data();
			wheader = e_msgindex_cs2ws_add_auction_into_db;
		}
		bool to_proto(hld::cs2ws_proto::auction_add_item_into_db& msg)
		{
			hld::st_proto::st_unit_info* st_unit_ptr = msg.mutable_role_info();
			if (st_unit_ptr == nullptr)
			{
				return false;
			}
			role_info.to_proto(st_unit_ptr);
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			msg.set_old_item_guid(old_item_guid.server_64);

			hld::st_proto::st_login_fixed_data* st_login_ptr = msg.mutable_third_info();
			if (st_login_ptr == nullptr)
			{
				return false;
			}
			third_info.to_proto(st_login_ptr);
			return true;
		}
		void from_proto(const hld::cs2ws_proto::auction_add_item_into_db& msg)
		{
			role_info.from_proto(msg.role_info());
			auction_info.from_proto(msg.auction_info());
			old_item_guid.server_64 = msg.old_item_guid();
			third_info.from_proto(msg.third_info());
		}
	};

	struct ws2dp_auction_add_item_into_db : public packet_base
	{
		s_auction_info		info;
		s_unit_info			role_info;
		bool				is_add_to_log;
		guid_64				old_item_guid;
		login_fixed_data	third_info;
		ws2dp_auction_add_item_into_db()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_add_auction_into_db;
		}
	};

	struct dp2ws_auction_sell_end : public packet_base
	{
		int32				operate_result;
		s_auction_info		info;
		s_unit_info			role_info;
		bool				is_add_to_log;
		login_fixed_data	third_info;
		dp2ws_auction_sell_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_sell_end;
		}
		bool to_proto(hld::dp2s_proto::auction_sell_end& msg)
		{
			msg.set_operate_result(operate_result);
			hld::st_proto::st_auction_info* st_auciton_ptr = msg.mutable_info();
			if (st_auciton_ptr == nullptr)
			{
				return false;
			}
			info.to_proto(st_auciton_ptr);
			hld::st_proto::st_unit_info* st_unit_ptr = msg.mutable_role_info();
			if (st_unit_ptr == nullptr)
			{
				return false;
			}
			role_info.to_proto(st_unit_ptr);
			msg.set_is_add_to_log(is_add_to_log);
			hld::st_proto::st_login_fixed_data * st_login_ptr = msg.mutable_third_info();
			if (st_login_ptr == nullptr)
			{
				return false;
			}
			third_info.to_proto(st_login_ptr);
			return true;
		}
		void from_proto(const hld::dp2s_proto::auction_sell_end& msg)
		{
			operate_result = msg.operate_result();
			info.from_proto(msg.info());
			role_info.from_proto(msg.role_info());
			is_add_to_log = msg.is_add_to_log();
			third_info.from_proto(msg.third_info());
		}
	};

	struct ws2cs_auction_sell_end : public packet_base
	{
		int32			operate_result;
		s_auction_info	info;
		ws2cs_auction_sell_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_auction_sell_end;
		}
		bool  to_proto(hld::ws2cs_proto::auction_sell_end& msg)
		{
			msg.set_operate_result(operate_result);
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
		    info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::ws2cs_proto::auction_sell_end& msg)
		{
			operate_result = msg.operate_result();
			info.from_proto(msg.info());
		}
	};

	struct cs2ws_auction_buy : public packet_base
	{
		guid_64 role_guid;
		guid_64 item_guid;
		cs2ws_auction_buy()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_buy;
		}
	};

	struct ws2dp_find_buy_target : public packet_base
	{
		guid_64 role_guid;
		guid_64 item_guid;
		ws2dp_find_buy_target()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_find_buy_target;
		}
	};

	struct dp2ws_auction_find_buy_target_end : public packet_base
	{
		guid_64 role_guid;
		guid_64 item_guid;
		s_auction_info auction_info;
		dp2ws_auction_find_buy_target_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_find_buy_target_end;
		}
		bool to_proto(hld::dp2s_proto::auction_find_buy_target_end& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_item_guid(item_guid.server_64);
			hld::st_proto::st_auction_info *st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::dp2s_proto::auction_find_buy_target_end& msg)
		{
			role_guid.server_64 = msg.role_guid();
			item_guid.server_64 = msg.item_guid();
			auction_info.from_proto(msg.auction_info());

		}
	};

	struct ws2cs_auction_purchase_success : public packet_base
	{
		guid_64 role_guid;
		s_auction_info auction_info;
		ws2cs_auction_purchase_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_auction_purchase_success;
		}
		bool  to_proto(hld::ws2cs_proto::auction_purchase_success& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::ws2cs_proto::auction_purchase_success& msg)
		{
			role_guid.server_64 = msg.role_guid();
			auction_info.from_proto(msg.auction_info());

		}
	};

	struct dp2cs_auction_load_self_selling_end : public packet_base
	{
		guid_64				role_guid;
		int32							unit_array_index;
		int32				data_num;
		s_auction_info		auction_info[max_auction_sell_num];
		dp2cs_auction_load_self_selling_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_auction_load_self_selling_end;
		}
		void clear_data()
		{
			role_guid.clear_data();
			unit_array_index = 0;
			data_num = 0;
			g_clear_data(auction_info, max_auction_sell_num);
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&auction_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_info));
		}
	};

	struct cs2ws_auction_add_purchase_record : public packet_base
	{
		s_auction_record_info		auction_record_info;
		cs2ws_auction_add_purchase_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_add_purchase_record;
		}
	};

	struct cs2ws_auction_send_sell_success_info : public packet_base
	{
		s_auction_info		auction_info;
		cs2ws_auction_send_sell_success_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_sell_success_info;
		}
	};

	struct ws2cs_auction_send_sell_success_info : public packet_base
	{
		s_auction_info		auction_info;
		ws2cs_auction_send_sell_success_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_auction_sell_success_info;
		}
		bool to_proto(hld::ws2cs_proto::auction_send_sell_success_info& msg)
		{
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::ws2cs_proto::auction_send_sell_success_info& msg)
		{
			auction_info.from_proto(msg.auction_info());
		}
	};

	struct cs2ws_auction_cancel_sell : public packet_base
	{
		guid_64		role_guid;
		guid_64		item_guid;
		cs2ws_auction_cancel_sell()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_cancel_sell;
		}
	};

	struct ws2dp_auction_cancel_sell : public packet_base
	{
		guid_64		role_guid;
		guid_64		item_guid;
		ws2dp_auction_cancel_sell()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_cancel_sell;
		}
	};

	struct dp2ws_auction_cancel_sell : public packet_base
	{
		guid_64				role_guid;
		s_auction_info		auction_info;
		dp2ws_auction_cancel_sell()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_cancel_sell;
		}
		bool  to_proto(hld::dp2s_proto::auction_cancel_sell& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::dp2s_proto::auction_cancel_sell& msg)
		{
			role_guid.server_64 = msg.role_guid();
			auction_info.from_proto(msg.auction_info());

		}
	};

	struct ws2cs_auction_cancel_sell : public packet_base
	{
		guid_64				role_guid;
		s_auction_info		auction_info;
		ws2cs_auction_cancel_sell()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_auction_cancel_sell;
		}
		bool  to_proto(hld::ws2cs_proto::auction_cancel_sell& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::ws2cs_proto::auction_cancel_sell& msg)
		{
			role_guid.server_64 = msg.role_guid();
			auction_info.from_proto(msg.auction_info());

		}
	};

	struct cs2dp_auction_req_goods_list : public packet_base
	{
		guid_64			role_guid;
		int32			main_type;
		int32			sub_type;
		int32			item_color;
		int32			quality_level;
		int32			single_money_sort_mode;
		int32			total_money_sort_mode;
		bool			is_show_gold;
		bool			is_show_diamond;
		int32			show_page;
		int32			trade_type;
		int32           career_type;
		guid_64			seller_guid;
		guid_64			self_guid;
		xchar			key_word[auction_find_key_word_max_lenth + 1];
		cs2dp_auction_req_goods_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_auction_req_goods_list;
		}
	};

	struct dp2cs_auction_req_goods_list_end : public packet_base
	{
		guid_64			role_guid;
		int32			total_info_num;
		int32			cur_page_index;
		int32			data_num;
		s_auction_info	data_info[auction_show_num_per_page];
		dp2cs_auction_req_goods_list_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_auction_req_goods_list_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_info));
		}
	};

	struct cs2dp_auction_req_trade_record : public packet_base
	{
		guid_64			role_guid;
		guid_64			record_owner_guid;
		int32			trade_type;
		int32			page_index;
		cs2dp_auction_req_trade_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_auction_req_trade_record;
		}
	};
	struct dp2cs_auction_req_trade_record_end : public packet_base
	{
		guid_64					role_guid;
		int32					cur_page;
		int32					total_record_num;
		int32					data_num;
		s_auction_record_info	data_info[auction_record_show_num_per_page];
		dp2cs_auction_req_trade_record_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_auction_req_trade_record_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_record_info));
		}
	};

	struct cs2dp_auction_req_other_sell_info : public packet_base
	{
		guid_64			role_guid;
		int32			sell_money_type;
		int32			item_template_id;
		cs2dp_auction_req_other_sell_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_auction_req_another_sell_info;
		}
	};

	struct dp2cs_auction_req_other_sell_info_end : public packet_base
	{
		guid_64			role_guid;
		int32			sell_money_type;
		int32			data_num;
		int32			item_template_id;
		s_auction_info	data_info[auction_show_num_per_page];
		dp2cs_auction_req_other_sell_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_auction_req_another_sell_info_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_info));
		}
	};

	struct cs2ws_auction_bid_info : public packet_base
	{
		guid_64			role_guid;
		guid_64			item_guid;
		int32			price_money_type;
		int32			price_money_value;
		cs2ws_auction_bid_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_bid;
		}
	};

	struct ws2dp_auction_find_bid_target : public packet_base
	{
		guid_64			role_guid;
		guid_64			item_guid;
		int32			price_money_type;
		int32			price_money_value;
		ws2dp_auction_find_bid_target()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_find_bid_target;
		}
	};

	struct cs2ws_auction_share_sell_reward : public packet_base
	{
		s_auction_info	data_info;
		cs2ws_auction_share_sell_reward()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_share_sell_reward;
		}
	};

	struct dp2ws_auction_find_bid_target_end : public packet_base
	{
		guid_64			role_guid;
		guid_64			item_guid;
		int32			price_money_type;
		int32			price_money_value;
		s_auction_info	auction_info;
		dp2ws_auction_find_bid_target_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_find_bid_target_end;
		}
		bool to_proto(hld::dp2s_proto::auction_find_bid_target_end& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_item_guid(item_guid.server_64);
			msg.set_price_money_type(price_money_type);
			msg.set_price_money_value(price_money_value);
			hld::st_proto::st_auction_info * st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::dp2s_proto::auction_find_bid_target_end& msg)
		{
			role_guid.server_64 = msg.role_guid();
			item_guid.server_64 = msg.item_guid();
			price_money_type = msg.price_money_type();
			price_money_value = msg.price_money_value();
			auction_info.from_proto(msg.auction_info());
		}
	};

	struct ws2cs_auction_find_bid_target_end : public packet_base
	{
		guid_64			role_guid;
		s_auction_info	auction_info;
		ws2cs_auction_find_bid_target_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_auction_find_bid_target_end;
		}

		bool  to_proto(hld::ws2cs_proto::auction_find_bid_target_end& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::ws2cs_proto::auction_find_bid_target_end& msg)
		{
			role_guid.server_64 = msg.role_guid();
			auction_info.from_proto(msg.auction_info());

		}
	};

	struct cs2ws_auction_clear_selling_lock : public packet_base
	{
		guid_64			item_guid;
		cs2ws_auction_clear_selling_lock()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_clear_selling_lock;
		}
	};

	struct cs2ws_auction_bid_success : public packet_base
	{
		guid_64			old_buyer_guid;
		s_auction_info	auction_info;
		cs2ws_auction_bid_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_bid_success;
		}
	};

	struct ws2dp_auction_bid_success : public packet_base
	{
		guid_64         new_record_id;
		guid_64			old_buyer_guid;
		s_auction_info	auction_info;
		ws2dp_auction_bid_success()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_bid_success;
		}
	};

	struct dp2ws_auction_clear_selling_lock : public packet_base
	{
		guid_64			item_guid;
		dp2ws_auction_clear_selling_lock()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_clear_selling_lock;
		}
	};

	struct ws2dp_auction_clear_old_item : public packet_base
	{
		int32	trade_type;
		int64	cur_time;
		ws2dp_auction_clear_old_item()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_clear_old_item;
		}
	};

	struct dp2ws_auction_clear_old_item_end : public packet_base
	{
		int32			data_num;
		s_auction_info	data_info[auction_clear_item_num_per_time];
		dp2ws_auction_clear_old_item_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_clear_old_item_end;
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_clear_old_item_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_info));
		}

		bool to_proto(hld::dp2s_proto::auction_clear_old_item_end& msg)
		{
			msg.set_data_num(data_num);
			for (int32 i = 0; i < data_num && i < auction_clear_item_num_per_time; i++)
			{
				hld::st_proto::st_auction_info * auction_ptr = msg.add_data_info();
				if (auction_ptr == nullptr)
				{
					return false;
				}
				data_info[i].to_proto(auction_ptr);
			}
			return true;


		}
		void from_proto(const hld::dp2s_proto::auction_clear_old_item_end& msg)
		{
			data_num = msg.data_num();
			for (int32 i = 0; i < data_num && i < auction_clear_item_num_per_time; i++)
			{
				data_info[i].from_proto(msg.data_info(i));
			}

		}

	};

	struct ws2dp_auction_add_share_list : public packet_base
	{
		int32					data_num;
		s_auction_share_info	data_info[auction_share_list_max_num];
		ws2dp_auction_add_share_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_add_share_list;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_share_info));
		}
	};

	struct ws2dp_auction_add_purchase_record : public packet_base
	{
		s_auction_record_info		auction_record_info;
		ws2dp_auction_add_purchase_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_add_purchase_record;
		}
	};

	struct ws2dp_auction_req_share_list : public packet_base
	{
		guid_64				item_guid;
		int32				item_template_id;
		guid_64				buyer_guid;
		int32				money_type;
		int32				money_value;
		ws2dp_auction_req_share_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_req_share_list;
		}
	};

	struct dp2ws_auction_req_share_list_end : public packet_base
	{
		guid_64					buyer_guid;
		int32					money_type;
		int32					money_value;
		int32					item_template_id;
		int32					data_num;
		s_auction_share_info	data_info[auction_share_list_max_num];
		dp2ws_auction_req_share_list_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_req_share_list_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_share_info));
		}
		bool  to_proto(hld::dp2s_proto::auction_req_share_list_end& msg)
		{
			msg.set_buyer_guid(buyer_guid.server_64);
			msg.set_money_type(money_type);
			msg.set_money_value(money_value);
			msg.set_item_template_id(item_template_id);
			msg.set_data_num(data_num);
			for (int32 i = 0; i < data_num && i < auction_share_list_max_num; i++)
			{
				hld::st_proto::st_auction_share_info * st_share_ptr = msg.add_data_info();
				if (st_share_ptr == nullptr)
				{
					return false;
				}
				data_info[i].to_proto(st_share_ptr);
			}
			return true;
		}
		void from_proto(const hld::dp2s_proto::auction_req_share_list_end& msg)
		{
			buyer_guid.server_64 = msg.buyer_guid();
			money_type = msg.money_type();
			money_value = msg.money_value();
			item_template_id = msg.item_template_id();
			data_num = msg.data_num();

			for (int32 i = 0; i < data_num && i < auction_share_list_max_num; i++)
			{
				data_info[i].from_proto(msg.data_info(i));
			}
		}
	};

	struct cs2dp_auction_req_self_bid_list : public packet_base
	{
		guid_64				role_guid;
		int32				trade_type;
		int32				page_index;
		int64				cur_time;
		cs2dp_auction_req_self_bid_list()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_auction_req_self_bid_list;
		}
	};

	struct ws2dp_auction_check_legion_selling : public packet_base
	{
		guid_64				legion_guid;
		int64				cur_time;
		ws2dp_auction_check_legion_selling()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_check_legion_selling;
		}
	};

	struct dp2ws_auction_check_legion_selling_end : public packet_base
	{
		guid_64				legion_guid;
		int32				selling_num;
		dp2ws_auction_check_legion_selling_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_check_legion_selling_end;
		}
	};

	struct cs2ws_auction_del_bid_record : public packet_base
	{
		guid_64				item_guid;
		cs2ws_auction_del_bid_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_del_bid_record;
		}
	};

	struct ws2dp_auction_del_bid_record : public packet_base
	{
		guid_64				item_guid;
		ws2dp_auction_del_bid_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_del_bid_record;
		}
	};

	struct ws2dp_auction_clear_old_bid_record : public packet_base
	{
		ws2dp_auction_clear_old_bid_record()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_clear_old_bid_record;
		}
	};

	struct ws2dp_auction_check_world_sell: public packet_base
	{
		ws2dp_auction_check_world_sell()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_check_world_sell;
		}
	};
	struct dp2ws_auction_check_world_sell_end : public packet_base
	{
		dp2ws_auction_check_world_sell_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_check_world_sell_end;
		}

		int32 sell_num;
	};
	struct ws2dp_auction_show_sell : public packet_base
	{
		int32 endTime;
		s_auction_info	data_info;
		ws2dp_auction_show_sell()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_show_sell;
		}
	};
	struct dp2ws_auction_show_sell_end : public packet_base
	{
		int32 result;
		s_auction_info	data_info;
		dp2ws_auction_show_sell_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_show_sell_end;
		}
	};

	struct ws2dp_auction_time_out :public packet_base
	{
		int64 cur_mil_sec;
		int64 sell_time;
		int64 deal_time;
		int32 is_first;
		ws2dp_auction_time_out()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_auction_time_out;

		}
	};
	struct dp2ws_aution_time_out_end : public packet_base
	{
		int32			data_num;
		s_auction_info	data_info[auction_time_out_sell_num];
		dp2ws_aution_time_out_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_time_out_end;
		}
		void clear()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_time_out_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_info));
		}
		bool to_proto(hld::dp2s_proto::aution_time_out_end& msg)
		{
			msg.set_data_num(data_num);
			for (int32 i = 0 ; i < data_num && i < auction_time_out_sell_num; i++)
			{
				hld::st_proto::st_auction_info * auction_ptr = msg.add_data_info();
				if (auction_ptr == nullptr)
				{
					return false;
				}
				data_info[i].to_proto(auction_ptr);
			}
			return true;
			 

		}
		void from_proto(const hld::dp2s_proto::aution_time_out_end& msg)
		{
			data_num = msg.data_num();
			for (int32 i = 0; i < data_num && i < auction_time_out_sell_num; i++)
			{
				data_info[i].from_proto(msg.data_info(i));
			}

		}

	};

	struct cs2dp_auction_req_bid_record_failure : public packet_base
	{
		guid_64 role_guid;
		int32 oper_type;
		cs2dp_auction_req_bid_record_failure()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_req_bid_record_failure;
		}
	};

	struct dp2cs_aution_req_bid_record_failure_end : public packet_base
	{
		guid_64         role_guid;
		int32			data_num;
		s_auction_bid_replace_info	data_info[auction_bid_record_failure_num];
		dp2cs_aution_req_bid_record_failure_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_req_bid_record_failure_end;
		}
		void clear_data()
		{
			role_guid.clear_data();
			data_num = 0;
			g_clear_data(data_info, auction_bid_record_failure_num);
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_bid_replace_info));
		}
	};

	struct ws2cs_auction_bid_replace_info: public packet_base
	{
		guid_64			role_guid;
		s_auction_bid_replace_info	auction_replace_info;
		ws2cs_auction_bid_replace_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_bid_replace_info;
		}
	};

	struct ws2cs_auction_time_out_del : public packet_base
	{
		guid_64     role_guid;
		s_auction_info auction_info;
		ws2cs_auction_time_out_del()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_time_out_del;
		}


		bool  to_proto(hld::ws2cs_proto::auction_time_out_del& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			hld::st_proto::st_auction_info* st_auction_ptr = msg.mutable_auction_info();
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			auction_info.to_proto(st_auction_ptr);
			return true;
		}
		void from_proto(const hld::ws2cs_proto::auction_time_out_del& msg)
		{
			role_guid.server_64 = msg.role_guid();
			auction_info.from_proto(msg.auction_info());

		}

	

	};

	struct cs2dp_auction_req_bid_notice: public packet_base
	{
		guid_64 role_guid;
		int32 oper_type;
		cs2dp_auction_req_bid_notice()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2dp_req_bid_notice;
		}
	};

	struct dp2cs_auction_req_bid_notice_end : public packet_base
	{
		guid_64         role_guid;
		int32			data_num;
		s_auction_bid_notice_info	data_info[auction_bid_notice_max_num];
		dp2cs_auction_req_bid_notice_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_req_bid_notice_end;
		}
		uint32 get_pak_length() const
		{
			const int32 basic_len = (ULONG_PTR)&data_info - (ULONG_PTR)&wheader;
			return (basic_len + data_num * sizeof(s_auction_bid_notice_info));
		}
	};

	struct dp2cs_auction_get_red_state_end : public packet_base
	{
		guid_64    role_guid;
		int32      result;
		int32      is_red;
		dp2cs_auction_get_red_state_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2cs_req_bid_notice_red_end;

		}

	};

	struct dp2ws_auction_bid_db_sucess : public packet_base
	{
		guid_64  buy_guid;
		guid_64  item_guid;
		dp2ws_auction_bid_db_sucess()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_auction_bid_db_sucess;
		}
	};


	struct cs2ws_auction_request_refresh_person: public packet_base
	{
		guid_64     role_guid;
		int32       role_class_type;
		int32       role_lv;
		int64       item_time;
		cs2ws_auction_request_refresh_person()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_auction_refresh_person;
		}
	};



#pragma pack(pop)
}
#endif