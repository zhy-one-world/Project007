#pragma	once

#include "logic/type_def.hpp"

namespace hld
{
	enum e_element_map_state
	{
		e_element_map_state_none,//初始状态
		e_element_map_state_player_join,//玩家进入状态
		e_element_map_state_runtime,//正常运行时状态
		e_element_map_state_end_map,//杀死所有npc切换地图状态
		e_element_map_state_close,//地图处于关闭状态
	};
	struct element_map_component
	{
		e_element_map_state m_state;//状态
		int32 m_data_array[e_role_element_map_max];//数据
	};
}
