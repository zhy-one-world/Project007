#pragma	once

#include "logic/type_def.hpp"
#include "template/ItemTemplate_S.h"
#include "logic/unit.hpp"

namespace hld
{
	struct item_base_component
	{
		int32 m_item_id;
		int32 m_data_array[e_item_base_max];
		ItemTemplate* m_item_template;
		unit* m_owner_ptr;
	};
	enum e_item_del_reason
	{
		e_item_del_reason_cost,//使用后归零
		e_item_del_reason_into_bag,//放入背包归零
		e_item_del_reason_recovery,//装备回收
		e_item_del_reason_clear_bag,//清空背包
	};
	struct item_del_component
	{
		e_item_del_reason del_reason;
	};
}
