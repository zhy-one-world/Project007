/********************************************************************
	created:	2010/03/01
	created:	1:3:2010   14:28
	filename: 	
	file path:	
	file base:	buff_def
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _BUFF_DEF_H_
#define _BUFF_DEF_H_

#include "Logic/att_def.hpp"
#include "Logic/item_def.hpp"
#include "count_def.hpp"
#include "server_def.hpp"
#include <template/ServerTemplateHead.h>

namespace faith
{
#pragma pack(push,1)

	#define BUFF_INVALID			guid_64()
	#define BUFF_SCAN_INTERVAL		9
	#define BUFF_MININTERVAL		1
	#define MAX_BUFF_GROUP			30
	#define MISTAKETIME				5		
	#define BUFF_MAX_ADDIOP			3	
	#define MAX_OTHER_TARGET		10
	#define MAX_BUFF_DESC			512
	#define MAX_ACTION_STRING		128
	#define MAN_UNIT_BUFF_INST		500
	#define MAN_UNIT_BUFF_INST_NPC	200
	#define MAX_ACTION_PARAM		10
	#define PATTERN_EXPRESSION		"%[^(]"
	#define FUNP_DELIMITER			","
	#define FUNP_DELIMITERC			','
	#define FUNL_DELIMITER			"("
	#define FUNL_DELIMITERC			'('
	#define FUN_RDELIMITER			")"
	#define FUN_RDELIMITERC			')'
	#define AND_DELIMITERC			'&'
	#define OR_DELIMITERC			'|'
	#define NOT_DELIMITERC			'!'
	#define FUN_MUL_DELIMITER		'*'
	#define FUN_DIV_DELIMITER		'/'
	#define FUN_ADD_DELIMITER		'+'
	#define FUN_SUB_DELIMITER		'-'
	const int32 buff_guid_owner_up = npc_arrary_index_begin * 10;

	const int32 transfer_buff_id = 10990011;			// 传送中buff特效
	const int32 transfer_finish_buff_id = 10130015;		// 传送后进入场景buff特效

	const int32 one_half_exp_buff_id = 10010035;		// 1.5倍经验buff
	const int32 double_exp_buff_id = 10010014;			// 双倍经验buff
	const int32 treble_exp_buff_id = 10010015;			// 三倍经验buff
	const int32 first_exp_buff_id = 10010039;			// 首次经验副本经验buff


	const int32 item_upgrade_default_start_id = 41000001;							// 强化表起始ID
	const int32 item_addon_default_start_id = 42000001;								// 追加表起始ID

	const int32 weapon_upgrade_buff_series = 221100;
	const int32 equip_upgrade_buff_series = 231101;
	const int32 weapon_addon_buff_series = 240018;
	const int32 equip_addon_buff_series = 240019;

	const int32 equip_awaken_buff_series	= 240026;
	const int32 equip_forge_buff_series		= 240020;
	const int32 equip_enchant_buff_series	= 240024;
	const int32 jewelry_enchant_buff_series = 240025;

	const int32 sky_suit_buff_series = 990654;
	const int32 god_suit_buff_series = 990658;
	const int32 supreme_suit_buff_series = 992150;

	const int32 belief_buff_buff_series = 990264;

	const int32 succinct_weapon_buff_series = 990306;
	const int32 succinct_equip_buff_series = 990307;

	const int32 passive_skill_start = 26000000;
	const int32 passive_skill_end = 26999999;
	const int32 passive_skill_zlzz_id = 26020808; // 真理之治
	const int32 sepc_skill_snow_id = 13029071;   // 法师暴风雪
	const int32 sepc_skill_fire_id = 13029021;  //  法师火雨
	const int32 sepc_skill_range = 500 + 600;  //技能范围
	const int32 sepc_skill_muti_id = 13018071; //战士镜像
	const int32 sepc_skill_mirror_id = 13048111; //刺客惑幻
	const int32 jump_time_limit = 3000; 
	const int32 skill_back_distance = 200; //技能拉回距离偏移

	const int32 max_rune_type = 16;
	const int32 rune_buff_buff_series_array[max_rune_type] = { 
		990289, 990290, 990291, 990292, 
		990293, 990294, 990295, 990296, 
		990297, 990298, 990299, 990300,
		990301, 990302, 990303, 990304 };

	const int32 hp_buff_id = 10010024;

	const int32 max_fashion_buff_type = 4;
	const int32 fashion_buff_array[max_fashion_buff_type] = { 990606, 990607, 990608, 990609 };
	
	enum e_action_type
	{
		e_at_direct,
		e_at_cyc,
		e_at_event,
		e_at_num,
	};
	enum e_buff_type
	{
		e_buff_type_nomal,//普通buff
		e_buff_type_frizing,//正面buff
		e_buff_type_negative,//负面buff
		e_buff_type_max,
	};
	enum e_buff_save_type
	{
		e_buff_svt_not_load, //保存但不加载
		e_buff_svt_save_load, //保存也加载
		e_buff_svt_not_save, //不保存
	};

	enum
	{
		buff_ret_false		=	0x0,
		buff_ret_true		=	0x1,
		buff_ret_keep		=	0x2,//buff继续保持
		buff_ret_del_pile	=	0x4,//buff减少一层叠加
		buff_ret_kill		=	0x8,//buff删除
	};

	enum
	{
		effect_p_op_none,
		effect_p_op_and,
		effect_p_op_or,
		effect_p_op_not,
		effect_p_op_end
	};

	enum
	{
		buff_addi_op_none,
		buff_addi_op_add,
		buff_addi_op_sub,
		buff_addi_op_div,
		buff_addi_op_mul,
		buff_addi_op_end
	};

	enum e_buff_event_type
	{
		using_item,				//使用物品
		using_skill,			//使用技能
		replace_equip,			//更换装备
		buff_disappear,			//buff消失时
		buff_appear,			//buff出现时
		target_move,			//对象移动
		skill_operate_self,		//指定系列技能作用于此目标时
		skill_self_release,		//此目标施放指定种类技能时
		using_emote,			//使用礼仪表情动作
		target_block,			//目标格挡
		target_miss,			//目标躲闪
		target_beblock,			//目标bei格挡
		target_bemiss,			//目标躲闪
		target_critical,		//目标暴击
		target_becritical,		//目标被暴击
		target_attchange,		//属性变化
		target_immunitydamage,	//目标免疫伤害
		target_beimmunitydamage,//目标免疫伤害
		target_damage,			//目标伤害
		target_bedamage,		//目标被伤害
		killtarget,				//杀死目标
		changeweapon,			//换武器
		costmp,					//消耗mp
		curein,					//被治疗时
		levelup,
	};

	struct  buff_inst
	{
		buff_inst()
		{
			buff_template_id	= 0;
			time				= 0;
			pile_count			= 0;
			state				= 0;
			effect_index		= -1;
			skill_template_id	= 0;
		}
		guid_64	sender_guid;
		guid_64	target_guid;
		guid_64	buff_guid;
		guid_64	item_guid;
		uint32		buff_template_id;
		f32			time;
		uint32		pile_count;
		uint32		state;
		int32		skill_template_id;
		fvector		pos;
		frotator	dir;
		int32		effect_index;
	};
	enum EBuffInfo
	{
		EBuffInfo_template_id,//buff模板id
		EBuffInfo_load_type,//buff读档类型
		EBuffInfo_time,//buff时间
		EBuffInfo_pile_count,//buff层级
		EBuffInfo_series,//buff的系
		EBuffInfo_skill_template_id,//技能模板id
		EBuffInfo_percent,//技能百分比
		EBuffInfo_effect_index,//特效下标
		EBuffInfo_max
	};
	struct s_buff_data
	{
		int32	data_ary[EBuffInfo_max];
		s_buff_data()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	struct s_buff_info
	{
		guid_64	buff_guid;
		guid_64	sender_guid;
		int32	data_ary[EBuffInfo_max];
		s_buff_info()
		{
			sender_guid.clear_data();
			buff_guid.clear_data();
			memset(data_ary, 0, sizeof(data_ary));
		}
		void clear_data()
		{
			sender_guid.clear_data();
			buff_guid.clear_data();
			memset(data_ary, 0, sizeof(data_ary));
		}
		bool to_proto(faith::st_proto::st_buff_info * st_buff_ptr)
		{
			if (st_buff_ptr == nullptr)
			{
				return false;
			}
			st_buff_ptr->set_buff_guid(buff_guid.server_64);
			st_buff_ptr->set_sender_guid(sender_guid.server_64);
			for (int32 i = 0; i < EBuffInfo_max; i++)
			{
				st_buff_ptr->add_data_ary(data_ary[i]);
			}
			return true;
		}
		void from_proto(const faith::st_proto::st_buff_info & st_buff_ref)
		{
			buff_guid.server_64 = st_buff_ref.buff_guid();
			sender_guid.server_64 = st_buff_ref.sender_guid();
			for (int32 i = 0; i < EBuffInfo_max && i < st_buff_ref.data_ary_size(); i++)
			{
				data_ary[i] = st_buff_ref.data_ary(data_ary[i]);
			}
		}

	};
	struct s_buff_save_db
	{
		guid_64 role_guid;
		s_buff_info data_info;
		void clear_data()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};

	class buff_item;

	/************************************************************************/
	/*               buff action param固定参数（配置表中固定5列）           */
	/************************************************************************/
	struct buff_param
	{
		buff_param( )
		{
			clear();
		}
		void clear( )
		{ 
			count = 0; 
			memset( params, 0, sizeof(params) ); 
		}
		int32& operator[]( int32 nindex )
		{ 
			return  ( nindex >= 0 && nindex < MAX_ACTION_PARAM ) ? params[nindex] : params[0]; 
		}
		int32 operator=( int32 nvalue )
		{ 
			params[count++] = nvalue;
			return count; 
		}
		int32 operator( )( )
		{ 
			return count;
		}
		int32 params[MAX_ACTION_PARAM];
		int32 count;
	};

	/************************************************************************/
	/*     buff action param默认参数（不在配置表中暴露）				    */
	/************************************************************************/

	struct buff_ex_env_param
	{
		buff_ex_env_param()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
		int32 buff_level;
		uint32 skill_order;
		uint32 skill_template_id;
		int64 skill_cd;
		int32 skill_target_id;
		bool reload;
		guid_64 last_chain_unit;
		fvector pos;
		frotator dir;
		int32 damage_num;
		fvector dest_pos;
		float buff_percent;
		bool be_hit;
		bool be_critical;
		int32 hit_random;
		bool by_client;
	};

	struct buff_env_param 
	{
		buff_env_param()
		{ 
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	///////////////////
	int32 neventsender;
	int32 neventrecever;
	int32 nevent;
	int32 neventvalue;
	uint32 skillid;
	uint32 skillguid;
	guid_64 item_guid;
	///////////////////
	buff_item* pbuff;
	///////////////////
	bool apply;		
	bool is_end;
	///////////////////
	buff_ex_env_param exenv;
	//////////////////		
	};


	typedef bool (*pbuffaction)(buff_env_param&, BuffActionTemplate* buff_action_template_ptr);

	struct buff_action_func
	{
		std::string szname;
		pbuffaction	buffaction; 
	};

#pragma pack(pop)
}

#endif // _buff_def_h_