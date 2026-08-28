#pragma	once

#include "logic/type_def.hpp"
#include "template/BoxMapTemplate_S.h"
namespace hld
{
	const int32 g_role_box_search_time_cut = 1719802290;//宝箱搜索时间减少时间戳值，防止int32溢出
	const int32 g_role_box_search_time_interval = 1;//宝箱搜索时间间隔

	enum e_box_map_state
	{
		e_box_map_state_none,//初始状态
		e_box_map_state_player_join,//玩家进入状态
		e_box_map_state_select_npc,//选择npc状态
		e_box_map_state_wait_select_end,//等待客户端做完选择动画状态
		e_box_map_state_fight_monster,//怪物战斗运行时状态
		e_box_map_state_end_map,//杀boss切换地图状态
		e_box_map_state_close,//地图处于关闭状态
	};
	struct box_map_component
	{
		e_box_map_state m_state;//状态
		std::set<int32> m_boss_list;//boss列表
		int32 m_select_box_index;//选中的宝箱id
		int32 m_award_rate;//奖励占比
		BoxMapTemplate* m_box_map_template;
	};
}
