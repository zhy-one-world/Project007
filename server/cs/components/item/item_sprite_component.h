#pragma	once

#include "logic/type_def.hpp"
#include "template/SpiritTemplate_S.h"

namespace hld
{
	struct item_sprite_component
	{
		int32 m_data_array[e_item_sprite_max];
		SpiritTemplate* m_sprite_template;
	};
}
