
#include "send_skill.h"
#include "logic/skill_manager.h"
#include "skill_def.hpp"

namespace hld
{
	void send_skill::send_skill_spell_cast(s_unit_identifier const& attacker_idf,
		int32 skill_id,
		int64 skill_order,
		int32 damage_num,
		s_unit_identifier const& target_idf,
		const s_map_pos& temp_pos,
		const fvector& dest_pos)
	{
		if (skill_id <= 0)
		{
			return;
		}
		static int32 all_skill_order = -1;
		if (skill_order == 0)
		{
			skill_order = all_skill_order--;
			if (all_skill_order > 0)
			{
				all_skill_order = -1;
			}
		}
		use_skill_param param;
		param.sender_unit_idf = attacker_idf;
		param.target_unit_idf = target_idf;
		param.skill_template_id = skill_id;
		param.pos = temp_pos.unit_location;
		param.yaw = temp_pos.unit_rotation.yaw;
		param.skill_order = skill_order;
		param.damage_num = damage_num;
		param.is_server = true;
		skill_manager::skill_cast(param);
	}
}