/********************************************************************
	file name:	relation_def.h
	created:	2017/05/26 23:18
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _AUCTION_DEF_H_
#define _AUCTION_DEF_H_

#include "char_def.hpp"
#include "item_def.hpp"
#include "mail_def.hpp"

#define MAX_AUCTION_FIND_RET 200

#define FIND_AUCTION_INTERVAL	1000

#define MAX_AUCTION_YUANBAO	1000
#define MAX_AUCTION_MONEY	1000

#pragma pack(push,1)

namespace hld
{
	const int32 max_auction_sell_num = 16;							// 同时出售物品最大数量
	const int32 auction_show_num_per_page = 10;						// 交易行每页数量
	const int32 auction_record_show_num_per_page = 10;				// 交易行记录每页数量

	const int32 auction_trade_record_num_personal = 20;				// 交易行记录数量-个人
	const int32 auction_trade_record_num_legion = 200;				// 交易行记录数量-军团
	const int32 auction_trade_record_num_world = 200;				// 交易行记录数量-世界

	const float auction_find_list_interval = 0.25;					// 交易行搜索CD

	const int32 auction_find_key_word_max_num = 10;					// 交易行搜索关键字个数
	const int32 auction_find_key_word_max_lenth = 40;				// 交易行搜索关键字长度

	const int32 auction_show_another_sell_info_num = 10;			// 其他人最低售价展示数量

	const int32 auction_clear_item_num_per_time = 100;				// 每次清理拍卖行逾期物品个数

	const int32 auction_share_list_max_num = 130;					// 分红列表上限

	const int32 auction_legion_sell_check_interval = 5;			// 军团拍卖红点检查间隔

	const int32 auction_time_out_sell_num = 100;                 // 超时每次返还数量

	const int32 auction_bid_record_failure_num = 50;            // 竞拍失败数目

	const int32 auction_bid_notice_max_num = 50;

	const int32 auction_bid_red_min_interval = 10;

	const int32 auction_person_last_refresh_enum = e_time_type_money_buy_tip_time;
	const int32 auction_one_frame_person_num = 50;

	struct s_auction_info
	{
		guid_64 seller_guid;
		xchar seller_name[max_name_size + 1];
		guid_64 buyer_guid;
		xchar buyer_name[max_name_size + 1];
		int32 trade_type;
		int32 seller_vip_level;
		int32 auction_type;
		int32 auction_sub_type;
		int32 item_color;
		int32 quality_level;
		int32 class_limit;
		xchar item_name[max_item_name_size + 1];
		int64 sell_time;
		int64 deal_time;
		int32 sell_money_type;
		int64 sell_total_money;			// 一口价 价格上限
		int64 sell_single_money;		// 初始价格
		int64 cur_bid_money;
		int32 is_show;
		int32 add_time;
		int32 is_sky_order;
		guid_64 show_guid;
		s_item_info	item_info;
		
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			item_info.clear_data();
		}

		s_auction_info()
		{
			clear_data();
		}
		void set_seller_name(xstring _str)
		{
			memset(seller_name, 0, sizeof(seller_name));
			memcpy(seller_name, _str.c_str(), _str.size() > max_name_size ? max_name_size : _str.size());
		}
		void set_buyer_name(xstring _str)
		{
			memset(buyer_name, 0, sizeof(buyer_name));
			memcpy(buyer_name, _str.c_str(), _str.size() > max_name_size ? max_name_size : _str.size());
		}
		void set_item_name(xstring _str)
		{
			memset(item_name, 0, sizeof(item_name));
			memcpy(item_name, _str.c_str(), _str.size() > max_item_name_size ? max_item_name_size : _str.size());
		}
		
		void set_proto_info(server2dp_proto_s_auction_info * _info) const
		{
			if (nullptr == _info)
			{
				return;
			}
			_info->set_seller_guid(seller_guid.server_64);
			_info->set_seller_name(seller_name);
			_info->set_buyer_guid(buyer_guid.server_64);
			_info->set_buyer_name(buyer_name);
			_info->set_trade_type(trade_type);
			_info->set_seller_vip_level(seller_vip_level);
			_info->set_auction_type(auction_type);
			_info->set_auction_sub_type(auction_sub_type);
			_info->set_item_color(item_color);
			_info->set_quality_level(quality_level);
			_info->set_class_limit(class_limit);
			_info->set_item_name(item_name);
			_info->set_sell_time(sell_time);
			_info->set_deal_time(deal_time);
			_info->set_sell_money_type(sell_money_type);
			_info->set_sell_total_money(sell_total_money);
			_info->set_sell_single_money(sell_single_money);
			_info->set_cur_bid_money(cur_bid_money);
			_info->set_is_show(is_show);
			_info->set_add_time(add_time);
			_info->set_is_sky_order(is_sky_order);
			_info->set_show_guid(show_guid);
		}
		void proto_set_info(server2dp_proto_s_auction_info _info)
		{
			seller_guid.server_64 = _info.seller_guid();
			set_seller_name(_info.seller_name());
			buyer_guid.server_64 = _info.buyer_guid();
			set_buyer_name(_info.buyer_name());
			trade_type = _info.trade_type();
			seller_vip_level = _info.seller_vip_level();
			auction_type = _info.auction_type();
			auction_sub_type = _info.auction_sub_type();
			item_color = _info.item_color();
			quality_level = _info.quality_level();
			class_limit = _info.class_limit();
			set_item_name(_info.item_name());
			sell_time = _info.sell_time();
			deal_time = _info.deal_time();
			sell_money_type = _info.sell_money_type();
			sell_total_money = _info.sell_total_money();
			sell_single_money = _info.sell_single_money();
			cur_bid_money = _info.cur_bid_money();
			is_show = _info.is_show();
			add_time = _info.add_time();
			is_sky_order = _info.is_sky_order();
			show_guid.server_64 = _info.show_guid();
		}
		bool to_proto(hld::st_proto::st_auction_info* st_auction_ptr)
		{
			if (st_auction_ptr == nullptr)
			{
				return false;
			}
			st_auction_ptr->set_seller_guid(seller_guid.server_64);
			st_auction_ptr->set_seller_name(seller_name);
			st_auction_ptr->set_buyer_guid(buyer_guid.server_64);
			st_auction_ptr->set_buyer_name(buyer_name);
			st_auction_ptr->set_trade_type(trade_type);
			st_auction_ptr->set_seller_vip_level(seller_vip_level);
			st_auction_ptr->set_auction_type(auction_type);
			st_auction_ptr->set_auction_sub_type(auction_sub_type);
			st_auction_ptr->set_item_color(item_color);
			st_auction_ptr->set_quality_level(quality_level);
			st_auction_ptr->set_class_limit(class_limit);
			st_auction_ptr->set_item_name(item_name);
			st_auction_ptr->set_sell_time(sell_time);
			st_auction_ptr->set_deal_time(deal_time);
			st_auction_ptr->set_sell_money_type(sell_money_type);
			st_auction_ptr->set_sell_total_money(sell_total_money);
			st_auction_ptr->set_sell_single_money(sell_single_money);
			st_auction_ptr->set_cur_bid_money(cur_bid_money);
			st_auction_ptr->set_is_show(is_show);
			st_auction_ptr->set_add_time(add_time);
			st_auction_ptr->set_is_sky_order(is_sky_order);
			st_auction_ptr->set_show_guid(show_guid);
			hld::st_proto::st_item_info* st_item_ptr = st_auction_ptr->mutable_item_info();
			if (st_item_ptr == nullptr)
			{
				return false;
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
		void from_proto(const hld::st_proto::st_auction_info& st_auction_ref)
		{
			seller_guid.server_64 = st_auction_ref.seller_guid();
			my_memcopy_string(seller_name, max_name_size, st_auction_ref.seller_name());
			buyer_guid.server_64 = st_auction_ref.buyer_guid();
			my_memcopy_string(buyer_name, max_name_size, st_auction_ref.buyer_name());
			trade_type = st_auction_ref.trade_type();
			seller_vip_level = st_auction_ref.seller_vip_level();
			auction_type = st_auction_ref.auction_type();
			auction_sub_type = st_auction_ref.auction_sub_type();
			item_color = st_auction_ref.item_color();
			quality_level = st_auction_ref.quality_level();
			class_limit = st_auction_ref.class_limit();
			my_memcopy_string(item_name, max_item_name_size, st_auction_ref.item_name());
			sell_time = st_auction_ref.sell_time();
			deal_time = st_auction_ref.deal_time();
			sell_money_type = st_auction_ref.sell_money_type();
			sell_total_money = st_auction_ref.sell_total_money();
			sell_single_money = st_auction_ref.sell_single_money();
			cur_bid_money = st_auction_ref.cur_bid_money();
			is_show = st_auction_ref.is_show();
			add_time = st_auction_ref.add_time();
			is_sky_order = st_auction_ref.is_sky_order();
			show_guid.server_64 = st_auction_ref.show_guid();
		}

	};

	struct s_auction_record_info
	{
		guid_64			record_guid;
		guid_64			role_guid;
		bool			is_sell;
		s_auction_info	auction_info;

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			auction_info.clear_data();
		}

		s_auction_record_info()
		{
			clear_data();
		}
	};


	struct s_trading_info
	{
		guid_64 trade_guid;
		guid_64 legion_guid;
		guid_64 buyer_guid;
		xchar buyer_name[max_name_size + 1];
		int32 source_type;
		int32 trading_type;
		int32 trading_lab_type;
		int64 sell_time;
		int64 deal_time;
		int32 sell_money_type;
		int64 sell_cur_money;
		int64 sell_final_money;
		s_item_info	item_info;

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			item_info.clear_data();
		}

		s_trading_info()
		{
			clear_data();
		}
	};

	struct s_auction_share_info
	{
		guid_64			item_guid;
		guid_64			role_guid;
		float			share_rate;
		xchar			share_mail_title[max_mail_title_length + 1];
		xchar			share_mail_content[max_text_contents_length + 1];
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_auction_share_info()
		{
			clear_data();
		}
		void to_proto(hld::st_proto::st_auction_share_info* st_share_ptr)
		{
			if (st_share_ptr == nullptr)
			{
				return;
			}
			st_share_ptr->set_item_guid(item_guid.server_64);
			st_share_ptr->set_role_guid(role_guid.server_64);
			st_share_ptr->set_share_rate(share_rate);
			st_share_ptr->set_share_mail_title(share_mail_title);
			st_share_ptr->set_share_mail_content(share_mail_content);
		}

		void my_memcopy_string(void* dst, size_t max_len, const xstring &str)
		{
			size_t len = str.size();
			if (max_len < len)
			{
				len = max_len;
			}
			memcpy(dst, str.c_str(), len);
		}


		void from_proto(const hld::st_proto::st_auction_share_info& st_shar_ref)
		{
			item_guid.server_64 = st_shar_ref.item_guid();
			role_guid.server_64 = st_shar_ref.role_guid();
			share_rate = st_shar_ref.share_rate();
			my_memcopy_string(share_mail_title, max_mail_title_length, st_shar_ref.share_mail_title());
			my_memcopy_string(share_mail_content, max_text_contents_length, st_shar_ref.share_mail_content());
		}
	};

	struct s_auction_timer_info
	{
		int32 show_time;
		int32 is_update;
		int32 template_id;
		s_auction_info auction_info;

		s_auction_timer_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		bool is_vaild()
		{
			return auction_info.item_info.item_guid.is_valid();
		}
	};

	struct s_auction_bid_replace_info
	{
		guid_64			record_guid;
		guid_64			role_guid;
		int32           create_time;
		int32			is_read;
		s_auction_info  info;

		void clear_data()
		{
		   memset(this, 0, sizeof(*this));
		}

		s_auction_bid_replace_info()
		{
			clear_data();
		}
	};

	struct s_auction_bid_notice_info
	{
		guid_64 role_guid;
		guid_64 item_guid;
		int32   create_time;
		int32   template_id;
		int32   template_num;
		int32   money_type;
		int64   bid_money;
		int32   is_sell;
		guid_64 buy_guid;
		int32   update_time;
		int32   is_read;

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_auction_bid_notice_info()
		{
			clear_data();
		}
	};

	struct s_auction_person_info
	{
		guid_64  role_guid;
		int32    role_class_type;
		int32    role_lv;
		int64    item_time;
		void    clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		s_auction_person_info()
		{
			clear_data();
		}
	};

	enum e_auction_lab_type
	{
		e_auction_lab_type_non, //
		e_auction_lab_type_Equip, //装备
		e_auction_lab_type_equip_materials, //强化
		e_auction_lab_type_mount, //坐骑
		e_auction_lab_type_wing, //羽翼
		e_auction_lab_type_book, //图鉴
		e_auction_lab_type_treasure, //珍品
		e_auction_lab_type_sundries, //杂物
		e_auction_lab_type_max
	};
	enum e_auction_equip_sub_type
	{
		e_auction_equip_sub_type_non, //
		e_auction_equip_sub_type_weapon, //武器
		e_auction_equip_sub_type_cloth, //衣服
		e_auction_equip_sub_type_pants, //裤子
		e_auction_equip_sub_type_hemelt, //头盔
		e_auction_equip_sub_type_handguard, //护手
		e_auction_equip_sub_type_shoes, //鞋子
		e_auction_equip_sub_type_ring, //戒指
		e_auction_equip_sub_type_necklace, //项链
		e_auction_equip_sub_type_earing, //耳环
		e_auction_lab_type_magic_book, //魔法书
		e_auction_equip_sub_type_max
	};
	
	enum e_auction_equip_material_sub_type
	{
		e_auction_equip_material_sub_type_non, //
		e_auction_equip_material_sub_type_upgrade_stone, //强化石
		e_auction_equip_material_sub_type_upgrade_protect, //强化保护材料
		e_auction_equip_material_sub_type_max
	};
	enum e_auction_mount_sub_type
	{
		e_auction_mount_sub_type_non, //
		e_auction_mount_sub_type_unlock, //解锁
		e_auction_mount_sub_type_rare_mount_chip, //神兽碎片
		e_auction_mount_sub_type_max
	};
	enum e_auction_wing_sub_type
	{
		e_auction_wing_sub_type_non, //
		e_auction_wing_sub_type_rare_wing_fragment, //神翼碎片
		e_auction_wing_sub_type_rare_wing_egg, //蛋
		e_auction_wing_sub_type_wing_illusion, //羽翼幻化
		e_auction_wing_sub_type_max
	};

	enum e_auction_book_sub_type
	{
		e_auction_book_sub_type_non, //
		e_auction_book_sub_type_hero, //英雄图鉴
		e_auction_book_sub_type_resonance, //共鸣
		e_auction_book_sub_type_max
	};

	
	enum e_auction_param
	{
		e_auction_param_private_del, //个人拍卖删除时间
		e_auction_param_private_delay_start, // 个人随机上架起始时间
		e_auction_param_private_delay_end, //个人随机上架结束时间
		e_auction_param_wait_bid,  // 世界，军团拍卖等待时间
		e_auction_param_wait_notice, // 品质道具通告时间
		e_auction_param_add_time,    //加时时间
		e_auction_param_add_n,       //加时次数
		e_auction_param_share_money_limit, //共享拍卖分红上限
		e_auction_param_person_refresh_time,  // 个人刷新时间
		e_auction_param_person_refresh_item_num, // 个人刷新物品数量
		e_auction_param_max,
	};

	enum e_auction_req_type
	{
		e_auction_req_type_read = 1,
		e_auction_req_type_red,

	};
}

#pragma pack(pop)

#endif
