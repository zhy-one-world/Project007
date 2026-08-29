/********************************************************************
	created:	2014/11/17
	created:	17:11:2014   18:36
	file base:	buff_event
	file ext:	h
	author:		zhy
	
	purpose:	
*********************************************************************/
#ifndef _BUFF_EVENT_H_
#define _BUFF_EVENT_H_

#include "logic/type_def.hpp"
#include "Logic/buff_def.hpp"
//#include <common/singleton.hpp>
//#include "game_event.h"

struct buff_event;

namespace faith
{
	// buff Event
	struct event_trigger_buf
	{
		e_buff_event_type	trig_event; // 触发事件类型
		guid_64			srcid;      // 触发源
		guid_64			targetid;   // 触发作用的目标
		uint32				id;         // 模板id
		uint32				skillid;
		uint32				skilltimestamp;
	};
}

#endif
