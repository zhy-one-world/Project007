#pragma once

#include "logic/type_def.hpp"

namespace faith
{
#pragma pack(push,1)
	constexpr int32  DROP_MAX_WEIGHT = 1000000;

	enum e_dropbox_owner_type				// 归属类型
	{
		e_dropbox_owner_any,                //无归属,谁都可以拾取
		e_dropbox_owner_team,				//组队归属
		e_dropbox_owner_player, 		    //某玩家,单体归属
		e_dropbox_owner_team_ex,			//打宝系统归属(新)
	};

	enum e_dropbox_distribute
	{
		e_dropbox_distribute_none,			// 无
		e_dropbox_distribute_every_one,		// 每人一份
		e_dropbox_distribute_random,		// 随机分配
		e_dropbox_distribute_max
	};

	enum e_dropbox_ran_type					// 掉落规则  //0为逐个掉落，可能掉落多个物品，每个物品的掉落概率=权重/1000000 1为归一掉落，圆桌理论，最多只会掉落一个物品，所有id总权重小于1000000时，可能出现不掉落物品的情况
	{
		e_dropbox_ran_type_all,             //全部掉落
		e_dropbox_ran_type_one,		      	//归一掉落
	};

	enum e_dropbox_typ						// 掉落盒类型
	{
		e_dropbox_typ_non,					// 空，错误
		e_dropbox_typ_item,					// 物品本身
		e_dropbox_typ_drop_box,				// 另一个掉落盒
		e_dropbox_typ_money,				// 货币掉落
	};

	enum e_dropbox_attribution_rule			// 掉落盒类型
	{
		e_dropbox_attribution_rule_non,		// 空
		e_dropbox_attribution_rule_first,	// 第一刀
		e_dropbox_attribution_rule_killer,	// 击杀者
		e_dropbox_attribution_rule_threat,	// 仇恨列表
		e_dropbox_attribution_rule_dmg,		// 伤害列表
	};
#pragma pack(pop)
}