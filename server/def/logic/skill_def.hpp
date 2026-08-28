/********************************************************************
  created: 2014/07/31
  created: 31:7:2014 17:31
  file base: skill_def
  file ext: hpp
  author: Locke
  
  purpose: 
*********************************************************************/
#ifndef _SKILL_DEF_HPP_
#define _SKILL_DEF_HPP_

#include "logic/type_def.hpp"
#include "Logic/att_def.hpp"
#include "item_def.hpp"
#include "unit_def.hpp"


namespace hld
{
#pragma pack(push,1)
	#define MAX_SKILL_SERIES_NUM	1000      // 技能系列上限
	#define MAX_SKILL_NUM			300	      // 技能数量上限
	const int32 skill_effect_max = 10;	//单个技能最大子弹数量
	const int32 k_max_skill_num_player = 10;
	const int32 skill_type_buff_add = -1;       // buff带来的技能类型填充-1，防止和正常技能产生冲突
	const int32 k_max_buff_num_in_cast_section = 5; //每个cast阶段产生的buff个数上限
	const float k_skill_move_length_error_range = 80;
	const float k_skill_move_square_length_error_range = k_skill_move_length_error_range * k_skill_move_length_error_range;
	const int32 skill_use_change_num = 6;
	const int32 skill_wing_fly_to_mount_fly_id = 13300600;
	const int32	skill_move_max = 2000;
	const int32	skill_mount_stop_series = 104004;

	// 技能类型
	enum e_skill_type
	{
		est_floor_common = 0,        // 普攻
		est_floor_skill_1,           // 一号位置技能
		est_floor_skill_2,           // 二号位置技能
		est_floor_skill_3,           // 三号位置技能
		est_floor_skill_4,           // 四号位置技能
		est_floor_skill_5,           // 五号位置技能
		est_floor_skill_6,           // 六号位置技能
		est_floor_skill_7,           // 七号位置技能
		est_floor_call_mount = 15,   // 召唤坐骑
		est_floor_ride,              // 骑乘技能

		//世界骑乘
		est_ride_add_speed = 20,     // 加速
		est_ride_fly_up,             // 上升技能
		est_ride_fly_down,           // 下降技能
		est_ride_skill_3,            // 三号位置技能
		est_ride_skill_4,            // 四号位置技能
		est_ride_change_mount,       // 切换坐骑
		est_ride_call_wing,          // 召唤翅膀
		est_ride_fly,                // 飞行技能
		est_ride_change_mount_no_anim,// 切换坐骑无动画
		est_ride_call_wing_no_anim,   // 召唤翅膀无动画

		//副本骑乘
		est_fu_ride_common = 40,     // 普攻
		est_fu_ride_fly_up,          // 上升技能
		est_fu_ride_fly_down,        // 下降技能
		est_fu_ride_skill_3,         // 骑乘副本技能1
		est_fu_ride_skill_4,         // 骑乘副本技能2
		est_fu_ride_change_mount,    // 切换坐骑
		est_fu_ride_call_wing,       // 召唤翅膀
		est_fu_ride_fly,             // 飞行技能

		//世界飞行
		est_world_sky_add_speed = 60,// 加速
		est_world_sky_fly_up,        // 飞行中上升
		est_world_sky_fly_down,      // 下降技能
		est_world_sky_skill_3,       // 三号位置技能
		est_world_sky_skill_4,       // 四号位置技能
		est_world_change_mount,      // 切换坐骑
		est_world_sky_sky_to_floor,  // 从飞行到骑乘状态UISkillPointFuRideWidget
		est_world_wingfly_to_mountfly,//翅膀飞行转坐骑飞行
		est_world_mountfly_to_wingfly,//坐骑飞行转翅膀飞行
		est_world_wingfly_to_mountfly_no_anim,//翅膀飞行转坐骑飞行无动画


		//副本飞行
		est_fu_sky_common = 80,      // 普攻
		est_fu_sky_skill_3,          // 飞行副本技能1
		est_fu_sky_fly_down,         // 下降技能
		est_fu_sky_fly_up,           // 飞行中上升
		est_fu_sky_skill_4,          // 飞行副本技能2
		est_fu_sky_change_mount,     // 切换坐骑
		est_fu_sky_sky_to_floor,     // 从飞行到骑乘状态

		//守护神
		est_patron_saint = 100,      // 普攻

		//跳跃
		est_jump = 110,		// 跳跃
		est_old_mount_speed = 111,
		est_old_mount_stop = 112,
		est_mount_speed = 113,	// 坐骑加速
		est_mount_stop = 114,	// 坐骑停止
		est_wing_speed = 116,	// 翅膀加速
		est_wing_stop = 117,	// 翅膀停止

		//冲刺
		est_sprint_forward		=   120,	// 前冲
		est_sprint_back			=   121,	// 后退
		est_sprint_left			=   122,	// 左冲
		est_sprint_right		=   123,	// 右冲
		
		//大世界翅膀飞行  
		est_bigworld_wing_fly_down = 130,		// 翅膀降落
		est_bigworld_wing_fly_up,				// 翅膀起飞 
		est_bigworldwing_fly_roll_up,			// 翻滚
		est_bigworldwing_fly_change_wing,		// 切换翅膀
		est_bigworld_wing_fly_to_wing_walk,		// 从飞行到飞行走路 
		est_bigworld_wing_fly_skill ,			// 翅膀飞行技能，从陆地直接变成飞行状态没有动画

		est_change_mesh_skill_1 = 150,
		est_change_mesh_skill_2,
		est_change_mesh_skill_3,
		est_change_mesh_skill_4,
		e_skill_type_max
	};

	enum e_skill_operation_type
	{
		e_skill_operation_get_init_info = 1,
		e_skill_operation_inst_change = 2,
		e_skill_operation_master = 3,
		e_skill_operation_spell = 4,
		e_skill_operation_cast = 5,
		e_skill_operation_channel = 6,
		e_skill_operation_fly = 7,
		e_skill_operation_hurt = 8,
		e_skill_operation_cancel = 9,
		e_skill_operation_learn = 10,
		e_skill_operation_up_level = 11,
		e_skill_operation_spellcast = 12,
		e_skill_operation_sprint = 13,
	};
	// 技能命中及伤害响应枚举
	enum e_skill_damage_rep_type
	{
		e_skill_damage_reptype_null = 0,
		e_skill_damage_reptype_success = 1,          // 普通命中
		e_skill_damage_reptype_miss = 2,             // 打不中
		e_skill_damage_reptype_critical_imbibe = 3,  // 目标招架暴击
		e_skill_damage_reptype_block = 4,            // 目标格档成功
		e_skill_damage_reptype_imbibe = 5,           // 目标招架成功
		e_skill_damage_reptype_criticalhit = 6,      // 技能使用者暴击成功
		e_skill_damage_reptype_invinc = 7,           // 免疫
		e_skill_damage_reptype_resist = 8,           // 抵抗
		e_skill_damage_reptype_antihurt = 9,         // 吸收
	};
	// 起效类型
	enum e_skill_active_type
	{
		esat_zhudong,
		esat_buff,
	};

	// 技能性质
	enum e_skill_cast_type
	{
		estype_moment,			 // 瞬发技能
		estype_spell,			 // 吟唱技能
		estype_channels,		 // 通道技能
		estype_continuum,		 // 连续技能
		estype_rush,			 // 冲锋技能
		estype_callmountandride, // 召唤坐骑并骑乘
		estype_callwing,		 // 召唤翅膀技能
		estype_callmountandfly,	 // 召唤坐骑并起飞
		estype_flyup,			 // 上升
		estype_flydown,			 // 下降
		estype_sprint,			 // 冲刺技能
		estype_doublemounts,	 // 双人坐骑
		estype_wingflyup,		 // 羽翼上升技能
		estype_wingflydown,		 // 羽翼下降技能
		estype_channels_1,		 // 冰通道技能
		estype_channels_2,		 // 火通道技能
		estype_channels_3,		 // 光通道技能
		estype_channels_4,		 // 暗通道技能
		estype_channels_mount,	 // 坐骑通道技能
		estype_channels_wing,	 // 翅膀通道技能
		estype_max
	};
// 
// 	// 攻击范围
// 	enum e_skill_effect_type
// 	{
// 		esarea_single,		// 单体攻击
// 		esarea_multi,		// 多体（如治疗波、闪电链等，在多个目标间跳转）
// 		esarea_aoe_fixed,	// 施放者周围  area of effect,固定施放区域,施放者周围的一定区域内
// 		esarea_aoe,			// area of effect，任意选择施放区域
// 		esarea_front,		// 指定方向一段距离
// 		esarea_front_angle,	// 角色前方扇形区域
// 		esarea_aoe_target_area,		//在目标周围产生效果，攻击者不变，攻击目标周围的一群
// 		esarea_aoe_random,	// 施放者周围内的一个随机目标
// 		esarea_aoe_transfer,
// 		esarea_weapon,
// 	};

	//技能作用目标类型
	enum e_skill_target_type
	{
		eskilltarget_enemy,				//敌对
		eskilltarget_friend,			//友好
		eskilltarget_enemyplayer,		//敌对玩家
		eskilltarget_enemynpc,			//敌对npc
		eskilltarget_friendplayer,		//友好玩家
		eskilltarget_friendnpc,			//友好npc
		eskilltarget_team,				//队伍
		eskilltarget_self,				//自己
		eskilltarget_selfpet,			//自己的宠物
		eskilltarget_selfowner,			//自己的主人
		eskilltarget_boss,				//boss
		eskilltarget_groud,				//战场
		eskilltarget_friendplayer_noself,//友好玩家，不包括自己
		eskilltarget_max
	};

    // 伤害类型
    enum e_skill_hurt_type
    {
		edt_skill,     //技能伤害,计算自身属性
		edt_real,      //真实伤害,不计算属性
		edt_percent,   //百分比伤害,不计算属性,直接按总血量的百分比扣除或增加血量
		edt_damage_num,//其他属性造成的伤害
		edt_hp_percent,//按照自身血量百分比炸
		edt_reflect,	//反弹伤害
		edt_kill_once,	//一击必杀
		edt_hp_percent_target,//按照自身血量百分比
		edt_hp_percent_precise,//按照自身血量百分比 精确到0.001%
		edt_hp_percent_precise_wound,//按照自身血量百分比 精确到0.001% 计算减伤属性
    };	
	enum e_skill_warning_type
	{
		e_skill_warning_type_none,
		e_skill_warning_type_round,
		e_skill_warning_type_sector,
		e_skill_warning_type_rectangle,
		e_skill_warning_type_direct_round,
		e_skill_warning_type_random_round
	};
	enum e_skill_hurt_sound_type
	{
		e_skill_hurt_sound_type_near,//近战
		e_skill_hurt_sound_type_ice,//冰魔法
		e_skill_hurt_sound_type_fire,//火魔法
		e_skill_hurt_sound_type_light,//光魔法
		e_skill_hurt_sound_type_dark,//暗魔法
		e_skill_hurt_sound_type_max

	};
	enum e_skill_movement_type
	{
		esmtype_none,				//  0: 不移动角色
		esmtype_charge,				//  1: 冲锋使用skillspeed计算移动到目标点需要的时间
		esmtype_leap,				//  2: 飞身跃斩 使用skillactiontime按照固定时间移动到目标点,一般用于需要和动画配合skillactiontime==动画时长(未使用)
		esmtype_transinmap,			//  3: 传送类:瞬间移动到目标点
		esmtype_trans,				//  4: 移形换位:和自己的兵魂互换位置（未使用）
		esmtype_transinarea,		//  5: 施放者在一定范围内移动,移动收玩家控制,skillspeed移动速度（未使用）
		esmtype_randomtransinarea,	//  6: 施放者在一定范围内随机移动，移动不收玩家控制，skillspeed移动速度（未使用）
		esmtype_recallnomove,		//  7: 不移动角色,需要在目标点召唤出来一个静止不动的aoi object,aoi object bound由目标点和actionrange确定,如陷阱
		esmtype_recallmove,			//  8: 不移动角色,需要在目标点召唤出来一个随机移动的aoi object,aoi object bound由目标点和actionrange确定,aoi object的移动半径由skillmovementparam确定,如风卷残云
		esmtype_pull,				//  9: 拉人,与冲锋相反
		esmtype_freecharge,			// 10: 不选定目标的冲锋
		esmtype_transtarget,		// 11: 瞬移到目标/ 连续运动
	};
	enum e_skill_bullet_move
	{
		e_skill_bullet_move_stop,//原地
		e_skill_bullet_move_line,//直线
		e_skill_bullet_move_follow,//追踪
		e_skill_bullet_move_radian,//扇形
		e_skill_bullet_move_follow_and_hurt,//追踪并且对路径上有伤害
	};
	enum e_skill_bullet_unit
	{
		e_skill_bullet_unit_one,//单体
		e_skill_bullet_unit_all,//群体
	};
	enum e_ammo_type
	{
		e_ammo_type_none,
		e_ammo_type_npc,
	};
	enum e_skill_missile_movement_type
	{
		none = 0,
		repeated = 1,    // 反复运动
		order = 2,       // 顺序运动
		continuous = 4,  // 连续运动
		discrete = 8     // 离散运动
	};

	enum e_skill_use_target_in_war_type
	{
		e_skill_use_target_in_war_type_true,
		e_skill_use_target_in_war_type_false,
	};



	//技能属性消耗
	struct tag_skill_expend
	{
		tag_skill_expend(void) :iattindex(0), itype(0), ivalue(0)
		{
		}
		int32		iattindex;	//属性类型(erolestate)
		int32		itype;		//[1:消耗数值  2:消耗当前百分比 3:消耗最大百分比]
		int32		ivalue;
	};

	//技能cd组
	struct tag_skill_cd_group
	{
		tag_skill_cd_group() : igroupid(0), icdtime(0.f)
		{
		}
		int32		igroupid;
		f32		icdtime;		//单位:秒
	};

	struct use_skill_param
	{
		guid_64	map_guid;
		s_unit_identifier sender_unit_idf;
		s_unit_identifier target_unit_idf;
		int32 skill_template_id;
		int64 skill_order;
		fvector pos;
        int32 yaw;
		bool is_begin;
		bool be_hit;
		bool be_critical;
		bool is_server;
		int32 hit_random;
		int32 effect_index;
		fvector dest_pos;
		int32 damage_num;
		use_skill_param()
		{
			clear_data();
		}
		void clear_data()
		{
			map_guid.clear_data();
			sender_unit_idf.clear_data();
			target_unit_idf.clear_data();
			skill_template_id = 0;
			skill_order = 0;
			pos.clear_data();
			yaw = 0;
			is_begin = false;
			be_hit = false;
			be_critical = false;
			is_server = false;
			hit_random = 0;
			effect_index = 0;
			dest_pos.clear_data();
			damage_num = 0;
		}
	};

	struct use_buff_param
	{
		use_buff_param(void)
		{
			memset(this, 0, sizeof(*this));
		}
		uint32							skill_info_id;
		int32	 						attacker_array_index;
		int32					 		target_array_index;
		double							damage_base_value;
		int32							enmity_value;
		e_skill_hurt_type				damage_style;
		int64							damage_num;
		f32								base_damage_fixrate;
		float							buff_percent;
		bool							be_hit;
		bool							be_critical;
		float							add_percent;
		int32							hit_random;
		bool							server_use_client;
	};
	
	enum e_fix_skill
	{
		e_fix_skill_damage_ratio,
		e_fix_skill_sub_cd_value,
		e_fix_skill_sub_cd_percent,
		e_fix_skill_max
	};

	struct s_fix_skill
	{
		float data[e_fix_skill_max];

		s_fix_skill()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct attack_trig_skill_param
	{
		int32 need_attack_times;
		int32 cur_attack_times;
		int32 trig_skill;

		attack_trig_skill_param()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	enum class e_skill_state
	{
		none,
		spell,
		cast,
		fly,
		begin,
		max,
	};

	enum class e_skill_att
	{
		e_skill_info_id,
		e_skill_cd_time,
		e_skill_max_cd_time,
		e_skill_channel_time,
		e_skill_max_distance,
		e_skill_max_aoe_distance,
		e_skill_spell_time,
		e_skill_cost_mp,
		e_skill_self_buff_group,
		e_skill_state_filter,
		e_skill_activated,

		e_skill_skill_state,
		e_skill_caster_id,
		e_skill_target_id,
		e_skill_time_stamp,
		e_skill_dir,
		e_skill_start_point,
		e_skill_is_force,
		e_skill_transfer_unit_id,
		e_skill_transfer_time_stamp,
		e_skill_use_skill_flag,
		e_skill_buff_trigger,
		e_skill_item_trigger,
		e_skill_item_id,
		e_skill_spell_total_time,
		e_skill_bhit,
		e_skill_move_time,
		e_skill_repeat_num,
	};

	enum e_missile_type   // move 
	{
		emt_invalid = -1,
		emt_immediately,
		emt_direction, //方向
		emt_trace, //追踪
		emt_channeled, //通道
		emt_point_fall_back, //点退回
		emt_retarget, //反弹
		emt_return, //返回
		emt_transfer, //传送
		emt_max,
	};

	enum e_missile_impact_type
	{
		emit_invalid = -1,
		emit_immediately,
		emit_hit,
		emit_max,
	};

	enum e_missile_effect_behavior
	{
		emeb_invalid = -1,
		emeb_empty,
		emeb_summon,
		emeb_add_buff,
		emeb_hurt,
		emeb_max
	};

	enum e_missile_ammo_type
	{
		emat_invalid = -1,
		emat_empty,
		emat_npc,
		emat_max,
	};

	enum e_missile_effect_unit_type
	{
		emeut_invalid = -1,
		emeut_attacker,
		emeut_target,
		emeut_ammo,
		emeut_max
	};
	enum e_skill_change_info
	{
		e_skill_change_info_series,
		e_skill_change_info_change_id,
		e_skill_change_info_back_id,
		e_skill_change_info_back_time,
		e_skill_change_info_max
	};
	struct s_skill_info
	{
		s_skill_info()
		{
			clear_data();
		}
		void clear_data()
		{
			for (int32 i = 0; i < e_skill_info_max; ++i)
			{
				data_ary[i] = 0;
			}
		}
		int32 data_ary[e_skill_info_max];
		bool to_proto(hld::st_proto::st_skill_info * st_skill_ptr)
		{
			if (st_skill_ptr == nullptr)
			{
				return false;
			}
			for (int32 i = 0; i < e_skill_info_max; i++)
			{
				st_skill_ptr->add_data_ary(data_ary[i]);
			}
			return true;
		}
		void from_proto(const hld::st_proto::st_skill_info& st_skill_ref)
		{
			for (int32 i = 0; i < e_skill_info_max &&  i < st_skill_ref.data_ary_size(); i++)
			{
				data_ary[i] = st_skill_ref.data_ary(i);
			}
		}
	};
	struct s_skill_info_db
	{
		guid_64 role_guid;
		int32 data_ary[e_skill_info_max];
		s_skill_info_db()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_skill_use
	{
		int32 data_ary[skill_use_change_num];
		s_skill_use()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_skill_use_db
	{
		guid_64 role_guid;
		s_skill_use data_info;
	};
	
	struct s_damage_count_all
	{
		double damage_total;
		double damage_elem;
		
		s_damage_count_all()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	
	struct s_hit_critical_calcu_param
	{
		f32		att_hit;
		f32		att_dodge;
		f32		att_hit_prob;
		f32		att_un_hit_prob;
		f32		att_critical;
		f32		att_critical_armor;
		f32		att_crit_prob;
		f32		att_un_crit_prob;
		int32	unit_type;
		int32	branch_type;
		int32	unit_level;
		int32	unit_upgrade_id;
		int32	map_type;
		int32	move_type;
		int32	map_order;
		int32	att_npc_temp_id;
		
		s_hit_critical_calcu_param()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	struct s_legion_skill_info
	{
		int32 data_ary[e_legion_skill_info_max];
		s_legion_skill_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

		bool operator==(const s_legion_skill_info& target) const
		{
			for (int32 i = 0; i < e_legion_skill_info_max; ++i)
			{
				if (data_ary[i] != target.data_ary[i])
				{
					return false;
				}
			}
			return true;
		}

		bool operator!=(const s_legion_skill_info& target) const
		{
			for (int32 i = 0; i < e_legion_skill_info_max; ++i)
			{
				if (data_ary[i] != target.data_ary[i])
				{
					return true;
				}
			}
			return false;
		}

	};
	
	struct s_legion_skill_info_db
	{
		guid_64 role_guid;
		s_legion_skill_info legion_skill_info;
	};


#pragma pack(pop)
}

#endif
