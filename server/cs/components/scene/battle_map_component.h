#pragma	once

#include "logic/type_def.hpp"

namespace faith
{
	enum e_battle_map_state
	{
		e_battle_map_state_none,//初始状态
		e_battle_map_state_player_join,//玩家进入状态
		e_battle_map_state_runtime,//正常运行时状态
		e_battle_map_state_end_map,//杀死所有npc关闭地图状态
		e_battle_map_state_close,//地图处于关闭状态
	};
	struct battle_map_component
	{
		e_battle_map_state m_map_state;
	};
}
