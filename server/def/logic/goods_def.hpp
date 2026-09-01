/********************************************************************
created: 2016/07/04
created: 4:7:2016 15:07
file base: goods_def
file ext: hpp
author: zhangminghai

purpose:
*********************************************************************/
#ifndef _GOODS_DEF_HPP_
#define _GOODS_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"

namespace faith
{
#pragma pack(push,1)
#define STORE_START_TEMPLATE_ID		28000001	//	商品起始id
#define LIMIT_GOODS_TIME_ARRAY_NUM		5	//抢购商品的时间限制数组位数
const int32 goods_max_num = 40;				//单个商店最大的限量数量
	const int32 MAX_GOODSINST_SIZE_PROP = goods_max_num * e_store_type_max;		//不同商品的最大数量
	enum e_goods_info
	{
		e_goods_info_id = 0,//模板id
		e_goods_info_owner_store,//隶属商店id
		e_goods_info_purchased_num,//已购买的数量
		e_goods_info_last_update_time,//最近一次更新时间
		e_goods_info_max,
	};
	enum e_store_update_type
	{
		e_store_update_null,	//不刷新
		e_store_update_timed,	//定时刷新
		e_store_update_interval,//间隔刷新
	};
	enum e_goods_type
	{
		e_goods_type_recommend = 0,//推荐商品
		e_goods_type_daily,//日常商品
		e_goods_type_cost,//消耗品
		e_goods_type_outline,//外形商品
		e_goods_type_buy_time_limit,//限时购买商品
		e_goods_type_max,
	};
	enum e_request_update_type
	{
		e_request_update_type_normal = 0, //正常刷新
		e_request_update_type_get_server__open_time =1,//	刷新限购类
	};
	enum e_store_open_type
	{
		e_store_open_type_foever = 0,//常驻商店
		e_store_open_type_time,//根据现实时间开启的商店
		e_store_open_type_server,//根据服务器开始时间计算的商店
	};

	enum e_time_limit_goods_on_type
	{
		e_time_limit_goods_on_type_none,
		e_time_limit_goods_on_type_open_server_days,	//服务器开服时间
		e_time_limit_goods_on_type_open_time,			//具体时间
	};

	enum e_time_limit_goods_start_type
	{
		e_time_limit_goods_start_type_data_error,		//数据错误
		e_time_limit_goods_start_type_continuing,		//物品出售中
		e_time_limit_goods_start_type_not_begin,		//物品未开始出售
	};

	struct s_goods_info
	{
		int32 data_ary[e_goods_info_max];
		s_goods_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_goods_info_db
	{
		guid_64 role_guid;
		s_goods_info	data_info;
		s_goods_info_db()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
#pragma pack(pop)
}
#endif