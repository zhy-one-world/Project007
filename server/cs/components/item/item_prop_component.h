#pragma	once

#include "logic/type_def.hpp"

namespace hld
{
	struct item_prop_component
	{
		int32 m_data_array[e_item_prop_max];
		PropBasicAttributeTemplate* m_prop_template;
	};
}
