
/********************************************************************
created: 2016/7/12
created: 12:7:2016 16:33
file base: pokedex_def
file ext: hpp
author: xinglong

purpose:define the type of belief, define the type and the length of pokedex inst data
*********************************************************************/
#ifndef _POKEDEX_DEF_HPP_
#define _POKEDEX_DEF_HPP_

#include <cassert>
#include "logic/type_def.hpp"
#include "char_def.hpp"
#include "template/template_base.h"
#include "game_enum_def_s.h"

namespace hld
{
#pragma pack(push,1)
	const int32 POKEDEX_PSYCHE_MAX = 20;
	const int32 POKEDEX_NUM_MAX = 80;
	const int32 PATRON_SAINT_STAR_MAX_NUM = 10;
	const int32 PATRON_SAINT_LEVEL_MAX_NUM = 10;
	const int32 PER_POKEDEX_PSYCHE_NUM = 20;

	const int32 POKEDEX_START_ID = 60000001;
	const int32 POKEDEX_OPEN_LEVEL_LIMIT = 2;
	const int32 PATRON_SAINT_OPEN_LEVEL_LIMIT = 3;
	const int32 patron_saint_start_id = 62010001;

	static const int32 succinct_mark_req_type_num = 8;
	const int32 succinct_mark_req_color[succinct_mark_req_type_num] = { 0,2,3,4,4,4,5,6 }; //绿，蓝，紫，紫，紫，橙，红
	const int32 succinct_mark_req_grade[succinct_mark_req_type_num] = { 0,1,2,3,4,5,5,5 };

	const int32 tinder_level_start_id = 65010001;
	const int32 tinder_unlock_start_id = 65000001;
	const int32 first_tinder_passive_skill_id = 63000001;
	const int32 tinder_max_level_one_order = 10;
	const int32 protect_default_mssh_id = 91053401;
	const int32 protect_default_animation_bp_id = 91053431;
	const int32 tinder_order_num_one_canto = 6;
	const int32 tinder_canto_num_max = 4;

	const int32 tinder_old_num_max = 30;
	const int32 tinder_item_max = 10;
	const int32 tinder_old_end_id = 65000030;
	const int32 tinder_new_order_max = 34;

	enum EPokedexInfo
	{
		EPokedexInfo_template_id,															    // 模板id
		EPokedexInfo_upgrade_num,																//当前等级
		EPokedexInfo_state,																		// 激活状态
		EPokedexInfo_fetter_state1,																//羁绊
		EPokedexInfo_fetter_state2,
		EPokedexInfo_fetter_state3,
		EPokedexInfo_psyche_num1,																//精魄
		EPokedexInfo_psyche_num2,
		EPokedexInfo_psyche_num3,
		EPokedexInfo_psyche_num4,
		EPokedexInfo_psyche_num5,
		EPokedexInfo_psyche_num6,
		EPokedexInfo_psyche_num7,
		EPokedexInfo_psyche_num8,
		EPokedexInfo_max
	};
	enum EPokedexActiveState
	{
		EPokedexActiveState_no_active = 0,															         // 未激活
		EPokedexActiveState_active,																		// 激活
		
	};


	enum e_protect_god_operation_type
	{
		e_pokedex_operation_protect_max_level,			// 已升满
		e_pokedex_operation_protect_god_headup,			// 升级
		e_pokedex_operation_protect_god_rankup,			// 升阶
		e_pokedex_operation_protect_god_max
	};

	enum e_protect_spirit_operation_type
	{
		e_protect_operation_equipe,			//    装备守护之灵
		e_protect_operation_unequipe,	    //    卸下守护之灵
		e_protect_operation_replace,        //    替换守护之灵
		e_protect_operation_max
	};

	struct s_pokedex_info
	{
		int32					data_ary[e_pokedex_Info_max];
		int32					collect_num[PER_POKEDEX_PSYCHE_NUM];
		s_pokedex_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}

	};

	struct s_protect_info
	{
		int32						data_ary[e_patron_saint_info_max];
		s_protect_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};
	struct s_pokedex_info_db
	{
		guid_64 role_guid;
		s_pokedex_info data_info;
	};


	struct s_tinder_info
	{
		int32 data_ary[e_tinder_info_max];
		s_tinder_info()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
	};

	struct s_tinder_info_db
	{
		guid_64			role_guid;
		s_tinder_info	data_info;

		void clear_data()
		{
			role_guid.clear_data();
			data_info.clear_data();
		}
	};

#pragma pack(pop)
}

#endif
