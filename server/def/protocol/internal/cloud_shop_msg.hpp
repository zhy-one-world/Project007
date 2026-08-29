/********************************************************************
created: 2018/07/17
file base: cloud shp
file ext: hpp
author: shangxiaoliang

purpose:
*********************************************************************/
#ifndef _FAITH_CLOUD_SHOP_HPP_
#define _FAITH_CLOUD_SHOP_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"   
#include "Logic/cloud_shop_def.hpp"
#include "internet/ws2cs.pb.h"

namespace faith
{
#pragma pack(push,1)

	enum
	{
		e_msgindex_ws2dp_load_cloud_shop_players_info = faith::e_msg_base_cloud_shop,
		e_msgindex_dp2ws_load_cloud_shop_players_info_end,
		e_msgindex_ws2dp_save_cloud_shop_players_info,
		e_msgindex_ws2dp_clear_cloud_shop_player_info,
		e_msgindex_ws2cs_cloud_shop_buy_req,
		e_msgindex_cs2ws_cloud_shop_buy_resp,
		e_msgindex_ws2dp_load_cloud_shop_not_finish_content,
		e_msgindex_dp2ws_load_cloud_shop_not_finish_content_end,

		//跨服
		e_msgindex_ws2gws_send_buy_cloud,
		e_msgindex_gws2ws_cloud_shop_buy,
		e_msgindex_ws2gws_cloud_shop_buy_end,
		e_msgindex_ws2gws_get_cloud_shop,
	};

	struct ws2dp_clear_cloud_shop_player_info : public faith::packet_base
	{
		int32 clear_type;
		ws2dp_clear_cloud_shop_player_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_clear_cloud_shop_player_info;
		}
	};

	struct ws2dp_save_cloud_shop_role_info : public faith::packet_base
	{
		cloud_shop_role_record_data player_role_info;
		ws2dp_save_cloud_shop_role_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_save_cloud_shop_players_info;
		}
	};

	struct ws2dp_load_cloud_shop_players_info : public faith::packet_base
	{
		ws2dp_load_cloud_shop_players_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_load_cloud_shop_players_info;
		}
	};

	struct dp2ws_load_cloud_shop_role_record_end : public packet_base
	{
		cloud_shop_role_record_data load_data[max_load_record_num];
		int32 load_num;
		bool is_end;
		dp2ws_load_cloud_shop_role_record_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_load_cloud_shop_players_info_end;
		}
	};

	struct ws2cs_cloud_shop_buy_req : public packet_base
	{
		guid_64 role_guid;
		int32 buy_type;			//0: 购买 1:抢购
		int32 have_buy_count;	//已购买数量(不包含抢购数量)
		int32 need_money_type;
		int32 need_money_count;
		ws2cs_cloud_shop_buy_req()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_cloud_shop_buy_req;
		}
		void to_proto(faith::ws2cs_proto::cloud_shop_buy_req& msg)
		{
			msg.set_role_guid(role_guid.server_64);
			msg.set_buy_type(buy_type);
			msg.set_have_buy_count(have_buy_count);
			msg.set_need_money_type(need_money_type);
			msg.set_need_money_count(need_money_count);
		}
		void from_proto(faith::ws2cs_proto::cloud_shop_buy_req& msg)
		{
			role_guid.server_64 = msg.role_guid();
			buy_type = msg.buy_type();
			have_buy_count = msg.have_buy_count();
			need_money_type = msg.need_money_type();
			need_money_count = msg.need_money_count();
		}
	};

	struct cs2ws_cloud_shop_buy_resp : public packet_base
	{
		int32 error_id;			//0: 成功,没错误
		int32 buy_type;
		guid_64 rold_guid;
		int32	server_id;
		int32	sender_server_id;//所在服务器
		cs2ws_cloud_shop_buy_resp()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_cloud_shop_buy_resp;
		}
	};

	struct ws2gws_send_bug_cloud : public packet_base
	{
		int32	sender_server_id;//所在服务器
		int32	req_type;
		int32	server_id;
		guid_64	role_guid;
		xchar	role_name[max_name_size + 1];
		int32	role_class_type;
		ws2gws_send_bug_cloud()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2gws_send_buy_cloud;
		}
		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
	};

	struct ws2gws_get_cloud_shop : public packet_base
	{
		int32	sender_server_id;//所在服务器
		guid_64	role_guid;
		ws2gws_get_cloud_shop()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2gws_get_cloud_shop;
		}
	};

#pragma pack(pop)
}
#endif