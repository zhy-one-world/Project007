#ifndef __SEND_SKILL_h__
#define __SEND_SKILL_h__

#include "logic/type_def.hpp"
#include "unit_def.hpp"

namespace hld
{
	class send_skill
	{
	public:
		send_skill(){}
		~send_skill(){}
	public:
		static void send_skill_spell_cast(s_unit_identifier const& attacker_idf, int32 skill_id, int64 skill_order, int32 damage_num, s_unit_identifier const& target_idf, const s_map_pos& temp_pos, const fvector& dest_pos);
	};
}

#endif