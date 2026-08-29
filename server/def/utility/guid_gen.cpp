/********************************************************************
	created:	2014/07/26
	created:	26:7:2014   16:22
	file base:	guid_gen
	file ext:	cpp
	author:		zhy
	
	purpose:	
*********************************************************************/

#include <time.hpp>
#include <random.hpp>
#include "guid_gen.h"
#include <Utility/cs_date.hpp>
#include "server_def.hpp"

namespace faith
{

	uint32 guid_gen::server_id = 0;
	uint32 guid_gen::head_second = 0;
	uint32 guid_gen::tail_second = 0;
	uint32 guid_gen::head_inc = 0;
	uint32 guid_gen::tail_inc = 0;
	bool guid_gen::use_tail = false;

	guid_64 guid_gen::make_guid(guid_64 src_guid)
	{
		guid_64	GUID;
		memset(&GUID, 0, sizeof(GUID));
		GUID.A = server_id;

		//当前时间
		uint32 current_second = utility::time();

		uint32 current_inc = 0;

		if (current_second <= head_second)
		{
			current_second = head_second + 1;
		}
		GUID.B = current_second;
		head_second = current_second;
		return GUID;
	}

	guid_128	guid_gen::make_guid_128()
	{
		guid_64 guid64 = guid_gen::make_guid();

		guid_128 GUID;
		GUID.A = guid64.A;
		GUID.B = guid64.B;
		return GUID;
	}

	void guid_gen::set_server_id(uint32 id)
	{
		server_id = id;
	}
}
