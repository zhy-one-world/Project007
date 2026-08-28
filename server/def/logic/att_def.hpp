/********************************************************************
  created: 2014/07/15
  created: 15:7:2014 19:41
  file base: att_def
  file ext: hpp
  author: zhy
  
  purpose: 
*********************************************************************/
#ifndef _LOW_ATT_DEF_HPP_
#define _LOW_ATT_DEF_HPP_

//- for serialize
#include "logic/type_def.hpp"
#include "internet/st.pb.h"

/************************************************************************/
/*        基本数据类型、常量和数据结构的定义，客户端和服务器共用        */
/************************************************************************/

namespace hld 
{
#pragma pack(push,1)

	const float in_war_time_max							= 5;		// 脱离战斗的时间
	const int32	game_att_sync = 1000;
	const int32 state_att_sync = 2000;
	const int32 state_trigger_sync = 3000;
	const int32 tier_att_max = 4;									//设置属性最大层级
	const int32 tier_att_min = 3;									//设置属性最小层级



	enum e_att_one
	{
		e_att_one_level,//层级
		e_att_one_att_id,//属性id
		e_att_one_value,//属性值
		e_att_one_percent,//属性百分比
		e_att_one_show,//是否显示到面板中 0不显示，1显示
		e_att_one_max
	};

	struct s_att_one
	{
		int32	att[e_att_one_max];
		s_att_one()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	enum e_pawn_att_index
	{
		epai_self_value,			    // 自身属性点数
		epai_self_rate,			        // 自身属性百分比
		epai_equip_value,			    // 装备属性点数
		epai_equip_rate,			    // 装备属性百分比
		epai_max
	};

	enum e_pawn_att_sync_type
	{
		send_att_to_client,//属性同步
		send_gs_to_client,//GS同步
		send_nothing_to_client,//都不同步
		send_max

	};

	enum e_unit_game_state
	{
		es_none,
		es_loading,
		es_gameing,
		es_saving,
		es_max
	};

	// 角色当前的移动方式
	enum e_move_ment
	{
		e_move_ment_walk,			// 人形态跑路
		e_move_ment_wing_walk,		// 背翅膀跑
		e_move_ment_mount_walk,		// 骑坐骑形态下跑路
		e_move_ment_mount_fly,		// 骑坐骑形态下飞行 
		e_move_ment_wing_fly,		// 翅膀形态下飞行 
		e_move_ment_max 
	};

	enum e_auto_states 
	{
		e_auto_states_none,
		e_auto_states_pathfinding,
		e_auto_states_attaking,
		e_auto_states_max
	};
	enum e_be_hurt_type
	{
		e_be_hurt_type_nomal,//普通
		e_be_hurt_type_count,//次数
		e_be_hurt_type_self,//只能自身掉血
	};
	// 角色
	struct tag_unit_att
	{
		tag_unit_att()
		{
			memset(this, 0, sizeof(*this));
		}
		f32	att[e_unit_attack_att_max];		// 属性数据使用f32型计算和保存
	};

	enum e_base_att_info
	{
		e_base_att_info_none = 0,
		e_base_att_info_strength,						// 力量(防御)		--果实加的属性
		e_base_att_info_agility,						// 敏捷(闪避)
		e_base_att_info_intellect,						// 智力(抗性)
		e_base_att_info_stamina,						// 耐力(生命)
		e_base_att_info_hp_cur,							// 当前生命值
		e_base_att_info_pk_value,						// 玩家pk值
		e_base_att_info_pk_mode,						// 玩家pk模式
		e_base_att_info_community_type,					// 玩家阵营类型
		e_base_att_info_equip_title_id,					// 玩家佩戴称号id
		e_base_att_info_magic_cur,						// 当前魔法值
		e_base_att_info_body_power_cur,					// 当前体力值
		e_base_att_info_body_strength,					// 力量				--玩家自身属性
		e_base_att_info_body_agility,					// 敏捷
		e_base_att_info_body_intellect,					// 智力
		e_base_att_info_body_stamina,					// 耐力
		e_base_att_info_hp_main,						// 在大世界时的血量
		e_base_att_info_cur_energy,						// 当前冲刺能量值
		e_base_att_info_max
	};

	struct s_base_att_info
	{
		int64 data_ary[e_base_att_info_max];

		s_base_att_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
		bool to_proto(hld::st_proto::st_base_att_info * st_base_ptr)
		{
			if (st_base_ptr == nullptr)
			{
				return false;
			}
			for (int32 i = 0 ; i < e_base_att_info_max; i++)
			{
				st_base_ptr->add_data_ary(data_ary[i]);
			}
			return true;
		}
		void from_proto(const hld::st_proto::st_base_att_info& st_base_ref)
		{
			for (int32 i = 0; i < e_base_att_info_max && i < st_base_ref.data_ary_size(); i++)
			{
				data_ary[i] = st_base_ref.data_ary(i);
			}
		}
	};
	struct s_att_info_db
	{
		guid_64 role_guid;
		s_base_att_info data_info;
		s_att_info_db()
		{
			role_guid.server_64 = 0;
			data_info.clear_data();
		}
	};
	struct s_fight_att_info_db
	{
		guid_64		role_guid;
		double		att_value[e_unit_attack_att_max];

		s_fight_att_info_db()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	enum e_level_up_result
	{
		e_level_up_failed_unknown = 0,
		e_level_up_succeed,//成功
		e_level_up_failed_exp,//经验不足
		e_level_up_failed_level_max,//达到等级上限
	};

	//移动协议有变化，客户端全部屏蔽了以下行为的表达，只保留了jump 8 的值
	enum e_sync_pos_move_flag
	{
		syncmoveflag_min				= 0,
		syncmoveflag_doubleclickmove	= 1,
		syncmoveflag_run				= 1,		//角色跑动
		syncmoveflag_duck				= 8,		//潜水
		syncmoveflag_pressjump			= 8,		//角色一直跳动
		syncmoveflag_dooublejump		= 8,		//角色单独跳动
		syncmoveflag_precisedestination = 128,		//
		syncmoveflag_max,
	};

	enum e_buff_shield_type
	{
		ebst_0,
		ebst_1,
		ebst_2,
		ebst_max
	};

	struct s_buff_shield_info
	{
		s_buff_shield_info()
		{
			buff_guid.clear_data();
			shield_type = -1;
			shield_value = 0;
		}
		guid_64 buff_guid;
		int32	shield_type;
		int32 shield_value;
	};


#pragma pack(pop)
}

#endif
