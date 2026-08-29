#pragma	once

#include "logic/type_def.hpp"
#include "template/ElementHeartTemplate_S.h"

namespace faith
{
	struct item_element_component
	{
		int32 m_data_array[e_item_element_max];
		ElementHeartTemplate* m_element_template;
		ElementHeartUpgradeTemplate* m_element_upgrade_template;
	};
}
