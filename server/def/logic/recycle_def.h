#ifndef RECYCLE_DEF_H
#define RECYCLE_DEF_H
#include "logic/type_def.hpp"
#include "char_def.hpp"
namespace hld
{
#pragma pack(push, 1)
	const int32 MAX_RECYCLE_TASK_NUM = 20;
	const int32 e_recycle_error_sucess = 0;
	const int32 e_recycle_error_days = -1;
	const int32 e_recycle_error_con = -2;
	const int32 e_recycle_error_have_get = -3;
	const int32 e_recycle_error_consume = -4;
	const int32 e_recycle_error_task_id = -5;
	const int32 e_recycle_error_task_state = -6;

	const int32 e_recycle_error_invaild_key = -7;
	const int32 e_recycle_error_level = 8;

	const int32 MAX_RECYCLE_INVITED_NUM = 100;


	struct s_recycle_info
	{
		int32 e_start_time_1;
		int32 e_login_state_2;
		int32 e_prop_state_3;
		guid_64 inviter_guid;
		int32   invited_state;
		s_recycle_info()
		{
			e_start_time_1 = 0;
			e_login_state_2 = 0;
			e_prop_state_3 = 0;
			inviter_guid = 0;
			invited_state = 0;
		}
		void reset()
		{
			e_start_time_1 = 0;
			e_login_state_2 = 0;
			e_prop_state_3 = 0;
			inviter_guid = 0;
			invited_state = 0;

		}
		void cycle_reset()
		{
			e_start_time_1 = 0;
			e_login_state_2 = 0;
			e_prop_state_3 = 0;
		}
	}; 

	struct s_recycle_info_db
	{
		guid_64 role_guid;
		s_recycle_info data_info;

	};

	enum e_recycle_task_inst_data
	{
		e_recycle_tk_config_id,
		e_recycle_tk_finish_num,
		e_recycle_tk_state,
		e_recycle_tk_max,
	};

	enum e_recycle_type
	{

		e_recycle_type_recuit = 1,
		e_recycle_type_login,
		e_recycle_type_task,
		e_recycle_type_buy,
		e_recycle_type_reward,
	};

	enum e_recycle_task_type
	{
		e_recycle_task_type_hunter_3 = 3,
		e_recycle_task_type_crusade_4,
		e_recycle_task_type_godness_5,
		e_recycle_task_type_star_6,
		e_recycle_task_type_king_7,
		e_recycle_task_type_fuwen_8,
		e_recycle_task_type_kill_boss_9,
		e_recycle_task_type_legin_reward_10,
		e_recycle_task_type_belief_11,
		e_recycle_task_type_activity_12,
	};

	enum e_recycle_param
	{
		e_recycle_param_open_server,			// 开服时间
		e_recycle_param_level,					// 需要等级
		e_recycle_param_days,					// 需要离线天数
		e_recycle_param_restart_days,			// 重启活动需要天数
		e_recycle_param_invited_open_sevrver,	// 邀请者开服天数
		e_recycle_param_invited_level,			// 邀请者等级
		e_recycle_param_max,
	};

	struct s_recycle_task_info
	{
		int32 data_ary[e_recycle_tk_max];

		s_recycle_task_info()
		{
			reset();
		}
		void reset()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
		bool operator==(const s_recycle_task_info &rhs)
		{
			for (int32 i = 0; i < e_recycle_tk_max; i++)
			{
				if (rhs.data_ary[i] != data_ary[i])
				{
					return false;
				}
			}
			return true;
		}
	};

	struct s_recycle_task_info_db
	{
		guid_64 role_guid;
		s_recycle_task_info data_info;
	};

	struct s_recycle_invited_info
	{
		guid_64 role_guid;
		int32 level;
		int32 head_id;
		int32 class_type;
		xchar invited_name[max_name_size + 1];
		s_recycle_invited_info()
		{
			memset((void *)this, 0, sizeof(s_recycle_invited_info));
		}
		s_recycle_invited_info(guid_64 in_guid, int32 in_level, int32 in_head_id, int32 in_class_type, xchar * in_invited_name)
		{
			role_guid = in_guid;
			level = in_level;
			head_id = in_head_id;
			class_type = in_class_type;
			memcpy(invited_name, in_invited_name, strlen(in_invited_name) > max_name_size ? max_name_size : strlen(in_invited_name));
		}
	};

	struct s_recycle_invited_info_db
	{
		s_recycle_invited_info data_info;
	};

	struct s_recycle_inviter_base_info_db
	{
		guid_64 inviter_guid;
		int32 level;
		s_recycle_inviter_base_info_db()
		{
			memset((void *)this, 0, sizeof(s_recycle_inviter_base_info_db));
		}
	};



	

#pragma pack(pop)
}




#endif
