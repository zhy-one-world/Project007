#pragma	once

#include "logic/type_def.hpp"
#include "item_def.hpp"

namespace hld
{
	struct box_award_component
	{
		bool m_can_get_award;
		e_award_mark_level m_award_level;
		std::vector<s_item_template_info> m_drop_item;
	};
}
