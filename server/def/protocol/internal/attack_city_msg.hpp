/********************************************************************
created: 2021年9月24日
file base: attack_city_msg
file ext: hpp
author: zhaoyuming

purpose: 
*********************************************************************/

#ifndef _ATTACK_CITY_MSG_HPP_
#define _ATTACK_CITY_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "Logic/char_def.hpp"
#include "Logic/drop_def.hpp"
#include "Logic/attack_city_def.hpp"
#include "core.hpp"
#include "Logic/chat_def.hpp"
namespace hld
{
#pragma pack(push,1)
	enum
	{
		e_msgindex_ws2ws_transfer_attack_city_map_result = hld::e_msg_base_attack_city,	// 传送到活动地图的返回结果
		e_msgindex_ws2cs_attack_city_init_data,												// 地图初始化军团数据		
		e_msgindex_ws2ws_attack_city_legion_check,											// 活动开启前的活动检测
		e_msgindex_ws2ws_attack_city_legion_check_end,										// 活动开启前的活动检测结果
		e_msgindex_ws2dp_attack_city_save_legion_info,										// 保存军团信息
		e_msgindex_ws2dp_attack_city_save_rank_info,										// 保存排名信息
		e_msgindex_ws2dp_attack_city_load_legion_info,										// 加载军团信息
		e_msgindex_dp2ws_attack_city_load_legion_info_end,									// 加载军团信息结果
		e_msgindex_ws2dp_attack_city_load_rank_info,										// 加载排名信息
		e_msgindex_dp2ws_attack_city_load_rank_info_end,									// 加载排名信息结果
		e_msgindex_ws2dp_attack_city_clear_rank_info,										// 清除排名数据
		e_msgindex_ws2dp_attack_city_clear_legion_info,										// 清除军团数据
		e_msgindex_ws2ws_get_attack_city_legion_info,										// 获取军团信息
		e_msgindex_ws2ws_send_attack_city_legion_info,										// 发送军团信息
		e_msgindex_ws2ws_get_attack_city_legion_rank_info,									// 获取军团排名信息
		e_msgindex_ws2ws_send_attack_city_legion_rank_info,									// 发送军团排名信息
		e_msgindex_ws2ws_sync_attack_city_legion_info,										// 同步军团信息
		e_msgindex_cs2ws_send_attack_city_legion_end_info,									// 军团结束信息
		e_msgindex_cs2ws_send_attack_city_legion_reward_info,								// 军团奖励信息
		e_msgindex_cs2ws_send_attack_city_rank_end_info,									// 发送军团排名信息
		e_msgindex_cs2ws_send_attack_city_legion_call,										// 发送军团召集
		e_msgindex_ws2ws_send_attack_city_group_mail,										// 发送分组信息
	};	
	// 活动地图传送
	struct ws2ws_transfer_attack_city_map_result : public hld::packet_base
	{
		int32						result;								// 结果类型
		guid_64						role_guid;							// 角色Guid
		guid_64						legion_guid;						// 军团Guid
		guid_64						map_guid;							// 地图Guid
		int32						map_temp_id;						// 地图模板Id
		int32						war_idex;							// 线
		int32						activity_type;						// 活动类型
		int32						server_id;							// 服务器id
		ws2ws_transfer_attack_city_map_result()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_transfer_attack_city_map_result;
		}
	};
	// 地图初始化
	struct ws2cs_attack_city_init_data : public hld::packet_base
	{
		guid_64						map_guid;								// 地图Guid
		int32						group_level;							// 分组等级
		s_attack_city_legion_info   legion_list[attack_city_group_max_num];	// 军团信息数组
		ws2cs_attack_city_init_data()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2cs_attack_city_init_data;
		}
	};
	// 军团检查
	struct ws2ws_attack_city_legion_check : public hld::packet_base
	{
		guid_64						legion_guid;							// 军团Guid
		ws2ws_attack_city_legion_check()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_attack_city_legion_check;
		}
	};
	// 军团检查结果
	struct ws2ws_attack_city_legion_check_end : public hld::packet_base
	{
		guid_64						legion_guid;							// 军团Guid
		ws2ws_attack_city_legion_check_end()	
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_attack_city_legion_check_end;
		}
	};
	// 保存军团信息
	struct ws2dp_attack_city_save_legion_info : public hld::packet_base
	{
		s_attack_city_legion_info					legion_info;
		ws2dp_attack_city_save_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_attack_city_save_legion_info;
		}
	};
	// 保存排名信息
	struct ws2dp_attack_city_save_rank_info : public hld::packet_base
	{
		s_attack_city_rank_info						rank_info;
		ws2dp_attack_city_save_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_attack_city_save_rank_info;
		}
	};
	// 请求加载军团信息
	struct ws2dp_attack_city_load_legion_info : public hld::packet_base
	{
		ws2dp_attack_city_load_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_attack_city_load_legion_info;
		}
	};
	// 加载军团信息
	struct dp2ws_attack_city_load_legion_info_end : public hld::packet_base
	{
		s_attack_city_legion_info	info_list[attack_city_save_dp_max_num];
		int32						data_num;
		dp2ws_attack_city_load_legion_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_attack_city_load_legion_info_end;
		}
	};
	// 请求加载排名信息
	struct ws2dp_attack_city_load_rank_info : public hld::packet_base
	{
		ws2dp_attack_city_load_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_attack_city_load_rank_info;
		}
	};
	// 加载排名信息
	struct dp2ws_attack_city_load_rank_info_end : public hld::packet_base
	{
		s_attack_city_rank_info		info_list[TopMaxNum];
		int32						data_num;
		dp2ws_attack_city_load_rank_info_end()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_dp2ws_attack_city_load_rank_info_end;
		}
	};
	// 清空排名信息
	struct ws2dp_attack_city_clear_rank_info : public hld::packet_base
	{
		ws2dp_attack_city_clear_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_attack_city_clear_rank_info;
		}
	};
	// 清空军团信息
	struct ws2dp_attack_city_clear_legion_info : public hld::packet_base
	{
		ws2dp_attack_city_clear_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2dp_attack_city_clear_legion_info;
		}
	};
	// 获取军团信息
	struct ws2ws_get_attack_city_legion_info : public hld::packet_base
	{
		guid_64 role_guid;
		guid_64 legion_guid;
		int32	server_id;
		ws2ws_get_attack_city_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_attack_city_legion_info;
		}
	};
	// 获取排名信息
	struct ws2ws_get_attack_city_legion_rank_info : public hld::packet_base
	{
		guid_64 role_guid;
		guid_64 legion_guid;
		int32	server_id;
		ws2ws_get_attack_city_legion_rank_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_get_attack_city_legion_rank_info;
		}
	};
	// 发送攻城战初始化成功
	struct ws2ws_sync_attack_city_legion_info : public hld::packet_base
	{
		ws2ws_sync_attack_city_legion_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_sync_attack_city_legion_info;
		}
	};

	// 发送军团结算结果
	struct cs2ws_send_attack_city_legion_reward_info : public hld::packet_base
	{
		guid_64						legion_guid;
		int32						rank_level;
		s_item_template_info	item_list[50];							// 军团奖励
		guid_64						player_guid[max_legion_member_num];		// 玩家列表
		int32						player_num;
		cs2ws_send_attack_city_legion_reward_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_attack_city_legion_reward_info;
		}
	};
	// 发送军团结束信息
	struct cs2ws_send_attack_city_legion_end_info : public hld::packet_base
	{
		int32							group_level;
		s_attack_city_map_legion_info	info_list[attack_city_group_max_num];
		cs2ws_send_attack_city_legion_end_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_attack_city_legion_end_info;
		}
	};
	// 发送军团排名信息
	struct cs2ws_send_attack_city_rank_end_info : public hld::packet_base
	{
		s_attack_city_rank_info		info_list[max_legion_member_num];
		s_attack_city_rank_info		legion_info;
		cs2ws_send_attack_city_rank_end_info()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_attack_city_rank_end_info;
		}
	};
	// 发送军团排名信息
	struct cs2ws_send_attack_city_legion_call : public hld::packet_base
	{
		guid_64						play_guid;
		guid_64						legion_guid;
		cs2ws_send_attack_city_legion_call()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_cs2ws_send_attack_city_legion_call;
		}
	};
	// 发送军团分组信息
	struct ws2ws_send_attack_city_group_mail : public hld::packet_base
	{
		guid_64						legion_guid[e_attack_city_group_level_max * attack_city_group_max_num];
		ws2ws_send_attack_city_group_mail()
		{
			memset(this, 0, sizeof(*this));
			wheader = e_msgindex_ws2ws_send_attack_city_group_mail;
		}
	};
	

	 

#pragma pack(pop)
}
#endif