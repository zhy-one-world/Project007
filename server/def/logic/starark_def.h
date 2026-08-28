#ifndef STARARK_DEF_H
#define STARARK_DEF_H
#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace hld
{
#pragma pack(push, 1)

	const int32  starark_round_max = 16;
	const int32  starark_max_db_len = 324;
	const int32  starark_max_step_num = 6;
	const int32  starark_item_id = 31141071;

	const int32  starark_error_sucess = 0;
	const int32  starark_error_unknown = -1;
	const int32  starark_error_not_money = -2;
	const int32  starark_error_not_item = -3;
	const int32  starark_error_times_max = -4;
	const int32  starark_error_not_full = -5;
	const int32  starark_error_bag_full = -6;
	const int32  starark_error_is_exist = -7;




	enum e_starark_inst_data
	{
		e_starark_step,
		e_starark_times,
		e_starark_last_id,
		e_starark_head,
		e_starark_max,
	};

	enum e_starark_step
	{
		e_starark_step_1 = 1,		 
		e_starark_step_2,			
		e_starark_step_3,			
		e_starark_step_4,	       
		e_starark_step_5,		  
		e_starark_step_6,			
	};


	enum e_starark_oper
	{
		e_starark_oper_load,
		e_starark_oper_unload,
		e_starark_oper_change,
		e_starark_oper_buy,
		e_starark_oper_round,
		e_starark_oper_refresh,
		e_starark_oper_five_round,
	};

	enum e_starark_per_weight
	{
		 e_starark_weight_id,
		 e_starark_weight_base,
		 e_starark_weight_1,
		 e_starark_weight_2,
		 e_starark_weight_3,
		 e_starark_weight_max,

	};
	enum e_starark_elem
	{
		e_starark_elem_step_id,
		e_starark_elem_item_id,
		e_starark_elem_item_num,
		e_starark_elem_max,
	};

	struct s_starark_info
	{
		int32 data_ary[e_starark_max];
		xchar body[starark_max_db_len + 1];
		s_starark_info()
		{
			reset();
		}
		void reset()
		{
			memset(this, 0, sizeof(s_starark_info));
		}
	};

	struct s_starark_info_db
	{
		guid_64 role_guid;
		s_starark_info data_info;
		s_starark_info_db()
		{
			memset(this, 0, sizeof(s_starark_info_db));
		}
	};

	struct s_starark_memory_info
	{
		int32 item_id;
		int32 item_num;
		int32 state;
		s_starark_memory_info()
		{
			reset();
		}
		void reset()
		{
			item_id = 0;
			item_num = 0;
			state = 0;
		}
	};

#pragma pack(pop)
}


#endif