/********************************************************************
created: 2016年4月26日19:19
file base: spirit
file ext: hpp
author: lxy

purpose: about spirit
*********************************************************************/


#ifndef _SPIRIT_DEF_HPP_
#define _SPIRIT_DEF_HPP_


#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"


namespace faith
{
#pragma pack(push,1)

	const int32			max_spirit_level = 100;				// 精灵最大的等级

	enum e_spirit_color
	{
		e_spirit_color_white = 1,		// 白色
		e_spirit_color_green,			// 绿色
		e_spirit_color_blue,			// 蓝色
		e_spirit_color_purple,		// 紫色
		e_spirit_color_orange,		// 橙色
		e_spirit_color_red,			// 紅色
		e_spirit_color_max
	};


	struct s_spirit_info
	{
		guid_64						spirit_guid;								// guid		
		int32						data_ary[e_spirit_info_max];				// 精灵实例数据
		s_spirit_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
			data_ary[e_spirit_info_showing_illusion_idex] = -1;
		}
		void set_data_ary(e_spirit_info eIndex, int32 value) { data_ary[eIndex] = value; };

		bool  to_proto(faith::st_proto::st_spirit_info* st_spirit_ptr)
		{
			if (st_spirit_ptr == nullptr)
			{
				return false;
			}
			st_spirit_ptr->set_spirit_guid(spirit_guid.server_64);
			for (int32 i = 0; i < e_spirit_info_max; i++)
			{
				st_spirit_ptr->add_data_ary(data_ary[i]);
			}
			return true;
		}
		void from_proto(const faith::st_proto::st_spirit_info& st_spirit_ref)
		{
			spirit_guid.server_64 =	st_spirit_ref.spirit_guid();
			for (int32 i = 0; i < e_spirit_info_max && i < st_spirit_ref.data_ary_size(); i++)
			{
				data_ary[i] = st_spirit_ref.data_ary(i);
			}
		}
	};
	struct s_spirit_info_db
	{
		guid_64	role_guid;
		s_spirit_info data_info;

	};
	struct s_spirit_fetter_info
	{	
		int32 data_ary[e_spirit_fetter_info_max];				// 精灵实例数据
		s_spirit_fetter_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_spirit_exchange_info
	{
		int32					 item_id;
		int32					 exchange_count;
		s_spirit_exchange_info()
		{
			item_id = 0;
			exchange_count = 0;
		}
	};

	enum ESpiritColor
	{
		ESpiritColor_non = 0,
		ESpiritColor_blue,
		ESpiritColor_purple,
		ESpiritColor_Max
	};

	enum ESpiritPropertyIdex
	{
		ESpiritProperty_Level,
		ESpiritProperty_PropertyID,
		ESpiritProperty_Points,
		ESpiritProperty_Percent,
		ESpiritProperty_IsShow,
		ESpiritProperty_Max
	};

	enum ESpiritFlairPropertyIdex
	{
		ESpiritFlairProperty_Level,
		ESpiritFlairProperty_PropertyID,
		ESpiritFlairProperty_Points,
		ESpiritFlairProperty_Percent,
		ESpiritFlairProperty_IsShow,
		ESpiritFlairProperty_Max
	};

	enum ESpiritOperateFailMsg
	{
		ESpiritOperateFail_not_in_free_time = 1,
		ESpiritOperateFail_item_bag_no_space = 90060166,
		ESpiritOperateFail_spirit_bag_no_space = 90060166,
		ESpiritOperateFail_no_more_spirit_crystal = 90029053,
		ESpiritOperateFail_no_more_score = 90060168,
		ESpiritOperateFail_equip_slot_full = 90060169,
		ESpiritOperateFail_cant_unequip_fight_spirit = 90060170
	};

	enum ESpiritTipType
	{
		ESpiritTip_none,
		ESpiritTip_role_level_up,//人物升级
		ESpiritTip_role_upgrade,//人物大转职
		ESpiritTip_role_fight,//人物进入战斗
		ESpiritTip_role_idle,//人物静止
		ESpiritTip_daily_mission_done,//完成日常任务
		ESpiritTip_unread_mail,//未读取的邮件
		ESpiritTip_team_invite,//组队邀请
		ESpiritTip_friend_message,//好友信息
		ESpiritTip_legion_invite//军团邀请
	};

	enum ESpirit_jiban_type
	{
		ESpirit_jiban_type_jiban,
		ESpirit_jiban_type_star_map,
	};
#pragma pack(pop)
}



#endif