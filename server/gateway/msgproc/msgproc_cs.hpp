/********************************************************************
	created:	2014/06/05
	created:	5:6:2014   10:46
	file base:	msgproc_cs
	file ext:	hpp
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _GATEWAY_MSGPROC_CS_H_
#define _GATEWAY_MSGPROC_CS_H_

#include "logic/type_def.hpp"

namespace faith
{
	void cs2gateway_aoi_msg_func(uint32 connindex, const void *data_ptr, size_t data_len);
	void cs2gateway_in_game_msg_func(uint32 connindex, const void *data_ptr, size_t data_len);
	void cs2gateway_month_recharge_msg_func(uint32 connindex, const void *data_ptr, size_t data_len);
}

#endif

