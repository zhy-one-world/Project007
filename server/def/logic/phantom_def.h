#ifndef PHANTOM_DEF_H
#define PHANTOM_DEF_H
#include "logic/type_def.hpp"
namespace faith
{
#pragma pack(push, 1)
	const int32 MAX_PHANTOM_NUM = 100;
	const int32 MAX_PHANTOM_PER_NUM = 10;
	const int32 phantom_error_sucess = 0;
	const int32 phantom_error_id_not_exist = -1;
	const int32 phantom_error_not_finish = -2;
	const int32 phantom_error_is_active = -3;

	enum e_phantom_inst_data
	{
		e_phm_config_id,
		e_phm_state,
		e_phm_item_1,
		e_phm_item_2,
		e_phm_item_3,
		e_phm_item_4,
		e_phm_item_5,
		e_phm_item_6,
		e_phm_item_7,
		e_phm_item_8,
		e_phm_item_9,
		e_phm_item_10,
		e_phm_max,
	};
	enum e_phantom_class_type
	{
		e_phm_class_warrior = 1,
		e_phm_class_mage = 2,
		e_phm_class_knight = 3,
		e_phm_class_assassin = 4,
	};
	struct s_phantom_info
	{
		int32 data_ary[e_phm_max];
		s_phantom_info()
		{
			reset();
		}
		void reset()
		{
			memset(data_ary, 0, sizeof(data_ary));
		}
		bool operator==(const s_phantom_info &rhs)
		{
			for (int32 i = 0; i < e_phm_max; i++)
			{
				if (rhs.data_ary[i] != data_ary[i])
				{
					return false;
				}
			}
			return true;
		}
	};

	struct s_phantom_info_db
	{
		guid_64 role_guid;
		s_phantom_info data_info;

	};
#pragma pack(pop)
}




#endif
