#pragma once

#include "Logic/map_def.hpp"
#include "template/MapTemplate_S.h"

namespace faith
{
	struct cs_map_component
	{
		std::set<int64>	m_role_list;//地图内人物列表(用于判断地图是否需要销毁)人物进出会消除
		std::set<int64>	m_enter_role_list;//地图内人物列表(用于判断是否可以重连进入)人物进出不会消除
		guid_64 m_map_guid;//地图guid
		int32 m_line_id;//地图线id
		int32 m_cs_uid;//地图在哪个服务器上
		int32 m_map_template_id;//地图模板id
		MapTemplate* m_map_template_ptr;//地图模板指针
		e_map_state m_map_state;//地图现在所处的状态
		int64 m_begin_time;//地图开启的时间
		int32 m_is_double_line;//开启双倍
		guid_64 m_own_team_guid;
	};
	struct cs_map_tick_component
	{
		int64 m_empty_end_time;//地图无玩家关闭的时间
	};
}
