/********************************************************************
created:	2021Äê10ÔÂ09ÈÕ16:04:35
file base:	msgproc_attack_city_ws
file ext:	hpp
author:		zhaoyuming

purpose:
*********************************************************************/

#ifndef _MSGPROC_ATTACK_CITY_WS_HPP_
#define _MSGPROC_ATTACK_CITY_WS_HPP_

#include "db_manager.hpp"

namespace faith
{
	void save_attack_city_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_attack_city_ws_info_end(db_result_type result, uint32 connindex);

	void load_attack_city_ws_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_attack_city_ws_info_end(db_result_type result, uint32 connindex);

	void save_attack_city_rank_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_attack_city_rank_info_end(db_result_type result, uint32 connindex);

	void load_attack_city_rank_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_attack_city_rank_info_end(db_result_type result, uint32 connindex);

	void clear_attack_city_legion_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_attack_city_legion_info_end(db_result_type result, uint32 connindex);

	void clear_attack_city_rank_info(uint32 connindex, const void *data_ptr, size_t data_len);
	void clear_attack_city_rank_info_end(db_result_type result, uint32 connindex);

}
#endif //_MSGPROC_ATTACK_CITY_WS_HPP_