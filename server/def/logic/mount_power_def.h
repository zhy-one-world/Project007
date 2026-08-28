
#ifndef _MOUNT_POWER_DEF_H_
#define _MOUNT_POWER_DEF_H_

#include "logic/type_def.hpp"

namespace hld
{
#pragma pack(push,1)

	const int32	mount_power_error_sucess = 0;
	const int32	mount_power_error_unknow = -1;
	const int32 mount_power_error_slot = -2;
	const int32 mount_power_error_slot_id = -3;
	const int32 mount_power_error_max_upgrade = -4;
	const int32 mount_power_error_need_mount_level = - 5;
	const int32 mount_power_error_cost_item = -6;


	const int32 mount_power_slot_start = e_item_info_jewel_slot_0;
	const int32 mount_power_slot_end = mount_power_slot_start + 5;
	const int32 mount_power_slot_max = 3;

	enum e_mount_power_oper
	{
		e_mount_power_oper_equip,
		e_mount_power_oper_star,

	};
	enum e_mount_power
	{
		e_mount_power_0,
		e_mount_power_1,
		e_mount_power_2,
		e_mount_power_max
	};
#pragma pack(pop)
}

#endif
