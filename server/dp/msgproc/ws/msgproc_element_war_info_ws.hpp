/***********************************************
* created	: 2020年8月20日
* file base : msgproc_element_war_info_ws
* file ext  : hpp
* author    : 赵玉明
* purpose   : 元素争霸
************************************************/

#ifndef _MSGPROC_ELEMENT_WAR_INFO_WS_HPP
#define _MSGPROC_ELEMENT_WAR_INFO_WS_HPP

#include "Logic/map_def.hpp"
#include "Logic/gate_def.hpp"
#include "Logic/element_war_def.hpp"
#include "element_war_msg.hpp"
#include "db_manager.hpp"

namespace faith
{
	//加载玩家信息
	void Load_element_war_load_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void Load_element_war_load_info_end(db_result_type result, uint32 connindex, int32 cur_num);

	//添加玩家
	void save_element_war_add_role_info(uint32 connindex, const void *data_ptr, size_t data_len);
	//更新玩家积分信息
	void save_element_war_update_info(uint32 connindex, const void *data_ptr, size_t data_len);
	//刷新赛季信息
	void save_element_war_refresh_season_info(uint32 connindex, const void *data_ptr, size_t data_len);
	//更新玩家基础信息
	void save_element_war_update_role_info(uint32 connindex, const void *data_ptr, size_t data_len);
	//更新玩家每日信息
	void save_element_war_refresh_ticket_info(uint32 connindex, const void *data_ptr, size_t data_len);
	//更新玩家任务信息
	void save_element_war_refresh_mission_info(uint32 connindex, const void *data_ptr, size_t data_len);
	//删除玩家任务信息
	void save_element_war_refresh_clear_mission_info(uint32 connindex, const void *data_ptr, size_t data_len);

	void ws2dp_element_war_common_end(db_result_type result, uint32 connindex);
	
}

#endif