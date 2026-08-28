/********************************************************************
	file name:	relation_def.h
	created:	2017/05/26 23:18
	author:		zhy
	purpose:	
*********************************************************************/
#ifndef _MARRY_DEF_HPP_
#define _MARRY_DEF_HPP_

#include "logic/type_def.hpp"
#include "char_def.hpp"

namespace hld
{
#pragma pack(push,1) 

	const int32 max_load_num = 1000;
	//const int32 new_married_item_id = 31090004;

	const int32 marry_team_error_tip_id = 90095176;
	const int32 marry_lack_item = 90095182;
	const int32 marry_aoi_error_id = 90095181;

	const int32 divorce_team_error_tip_id = 90095188;
	const int32 divorce_aoi_error_tip_id = 90095219;

	const int32 dati_max_num = 5;
	enum marry_state
	{
		marry_state_single_none,						//无
		marry_state_main_is_getting_reward,				//正在领奖励
		marry_state_sub_is_getting_reward,				//正在领奖励
		marry_state_main_had_get_reward,				//main领过了
		marry_state_sub_had_get_reward,					//sub领过了
		marry_state_both_had_get_reward,				//都领过了
		marry_state_with_sb,							//结婚
		marry_state_want_to_find_love,					// 
		marry_state_waiting_choice, 					//等待选择
		marry_state_one_main_agree,						//main 同意
		marry_state_one_sub_agree,						//sub 同意
		marry_state_one_refuse,							//拒绝
		marry_state_both_agree, 						//都同意
		marry_state_divorce_no_need_cost_money, 		//不需要花钱的强制离婚
		marry_state_force_divorce,						//强制离婚
		marry_state_max,
	};
	 

	struct marry_couple_data
	{
		guid_64		role_guid;
		xchar		role_name[max_name_size + 1];
		int32		role_class_template_id;
		int32		role_level;
		int32		marry_time;

		marry_couple_data()
		{
			clear();
		}

		void clear()
		{
			memset(this, 0, sizeof(*this));
		}

		void	set_role_name(const xstring& p_role_name)
		{
			memset(role_name, 0, sizeof(role_name));
			memcpy(role_name, p_role_name.c_str(), p_role_name.size() > max_name_size ? max_name_size : p_role_name.size());
		}
	};
	 
	struct marry_record_data
	{
		guid_64		main_couple_guid;
		xchar		main_couple_name[max_name_size + 1];
		guid_64		sub_couple_guid;
		xchar		sub_couple_name[max_name_size + 1];
		int32		marry_state;
		int32		save_event_time;
		int32		main_class_template_id;
		int32		main_level;
		int32		sub_class_template_id;
		int32		sub_level;
		int32		marry_item_id;
		int32		main_ring_level;
		int32		sub_ring_level;
		int32		heart_value;
		marry_record_data()
		{
			clear();
		}
		void clear()
		{
			memset(this, 0, sizeof(*this));
		}

		void	set_main_name(const xchar* p_role_name)
		{
			memset(main_couple_name, 0, sizeof(main_couple_name));
			memcpy(main_couple_name, p_role_name , strlen(p_role_name) > max_name_size ? max_name_size : strlen(p_role_name));
		}
		void	set_sub_name(const xchar* p_role_name)
		{
			memset(sub_couple_name, 0, sizeof(sub_couple_name));
			memcpy(sub_couple_name, p_role_name ,strlen(p_role_name)> max_name_size ? max_name_size : strlen(p_role_name));
		}

	};
	  


	typedef std::map<ui64, marry_record_data> marry_data_map;
	typedef std::vector<marry_record_data> waiting_marry_array;
#pragma pack(pop)
}

#endif // !_GAMEDEFINE_RELATION_H_
