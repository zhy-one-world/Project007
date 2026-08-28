#pragma	once

#include "logic/type_def.hpp"
#include "template/EquipTemplate_S.h"

namespace hld
{
	struct item_equip_component
	{
		int32 m_data_array[e_item_equip_max];
		EquipTemplate* m_equip_template;
	};
}
