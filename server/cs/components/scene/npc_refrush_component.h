#pragma	once

#include "logic/type_def.hpp"

namespace hld
{
	struct s_refrush_npc
	{
		int32 m_npc_point_id;
		uint64_t m_next_refrush_time;
		s_refrush_npc(int32 npc_point_id, uint64_t next_time)
		{
			m_npc_point_id = npc_point_id;
			m_next_refrush_time = next_time;
		}
	};
	struct npc_refrush_component
	{
		std::list<s_refrush_npc> m_refrush_list;
	};
}
