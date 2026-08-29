/********************************************************************
created: 2018/07/17
file base: cloud_shop_def
file ext: hpp
author: shangxiaoliang

purpose:
*********************************************************************/
#ifndef _CLOUD_SHOP_DEF_HPP_
#define _CLOUD_SHOP_DEF_HPP_
#include "logic/type_def.hpp"

namespace faith
{
#pragma pack(push,1)

	const int32 basic_cloud_shop_template_id = 134000000;	// 云购配置表 基础配置id
	const int32 cloud_shop_day_index_rate = 100;			// 云购配置表 当前第几天索引的乘数系数
	const int32 cloud_shop_server_level_min_id = 1;			// 云购配置表 服务器等级分段最小 ID
	const int32 cloud_shop_server_level_max_id = 1000;		// 云购配置表 服务器等级分段最大 ID
	const int32 cloud_shop_award_str_max_size = 3200;		//数据库存储云购的大奖获得情况数据字符串最大长度
	const int32 cloud_shop_basic_award_size = 4;			//云购基础奖励类别数量
	const int32 max_load_record_num = 1000;

	const int32 cloud_shop_robbery_start_time = 64800;		//抢购开始时间
	const int32 cloud_shop_award_opening_time = 75600;		//开奖时间


	enum e_cloud_shop_buy_type
	{
		e_cloud_shop_buy_type_buy,			//购买
		e_cloud_shop_buy_type_robber,		//抢购
		e_cloud_shop_buy_type_robber_max,
	};

	enum e_client_req_type
	{
		e_client_req_type_info,
		e_client_req_type_buy,
		e_client_req_type_robber,

	};

	enum e_cloud_shop_state
	{
		e_cloud_shop_not_open,			//活动未开放
		e_cloud_shop_wait_for_start,	//活动未开始
		e_cloud_shop_buy,				//可购买
		e_cloud_shop_robber,			//可抢购
		e_cloud_shop_finish,			//活动当天内容结束
	};

	enum e_cloud_shop_communicate_error_define
	{
		e_cloud_shop_communicate_error_define_no_error					= 0,	//没错误
		e_cloud_shop_communicate_error_define_state_error				= 1,	//不在购买时间内进行云购购买（包括购买和抢购）
		e_cloud_shop_communicate_error_define_not_enough_product		= 2,	//云购数量已买完，没有可供购买的数量了（包括购买和抢购）
		e_cloud_shop_communicate_error_define_data_error				= 3,	//数据格式错误
		e_cloud_shop_communicate_error_define_player_offline			= 4,	//玩家不存在
		e_cloud_shop_communicate_error_define_req_type_error			= 5,	//请求参数错误
		e_cloud_shop_communicate_error_define_vip_data_error			= 6,	//vip表未找到数据
		e_cloud_shop_communicate_error_define_not_enough_vip_buy_times	= 7,	//购买次数已用尽（仅指购买）
		e_cloud_shop_communicate_error_define_not_enough_money			= 8,	//钱不足（包括购买和抢购）
	};

	enum e_cloud_shop_role_info
	{
		e_cloud_shop_role_info_activity_type,
		e_cloud_shop_role_info_class_type,
		e_cloud_shop_role_info_server_id,
		e_cloud_shop_role_info_buy_number,
		e_cloud_shop_role_info_robber_number,
		e_cloud_shop_role_info_basic_award_count1,
		e_cloud_shop_role_info_basic_award_count2,
		e_cloud_shop_role_info_basic_award_count3,
		e_cloud_shop_role_info_basic_award_count4,
		e_cloud_shop_role_info_max
	};

	typedef std::map<int32, ui32>  cloud_shop_player_award_info;

	struct cloud_shop_role_record_data
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		int32		info_data[e_cloud_shop_role_info_max];

		cloud_shop_role_record_data()
		{
			clear();
		}

		void clear()
		{
			memset(this, 0, sizeof(*this));
		}

		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}

		int32 get_data(int32 info_type) const
		{
			if (info_type < 0 || info_type >= e_cloud_shop_role_info_max)
			{
				return -1;
			}
			return info_data[info_type];
		}

		void set_data(int32 info_type, int32 data_value)
		{
			if (info_type < 0 || info_type >= e_cloud_shop_role_info_max)
			{
				return;
			}
			info_data[info_type] = data_value;
		}

		void add_data(int32 info_type, int32 add_value)
		{
			if (info_type < 0 || info_type >= e_cloud_shop_role_info_max)
			{
				return;
			}
			info_data[info_type] += add_value;
		}
	};

	struct cloud_shop_big_reward_content
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		int32		big_item_id;
		int32		big_item_count;
		int32       big_item_lock;
		int32		server_id;
		cloud_shop_big_reward_content()
		{
			clear();
		}
		void clear()
		{
			memset(this, 0, sizeof(*this));
		}
		void set_role_name(xstring _role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, _role_name.c_str(), _role_name.size() > sizeof(role_name) ? sizeof(role_name) : _role_name.size());
		}
	};

#pragma pack(pop)
}

#endif
