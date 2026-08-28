#ifndef _FIRST_TIME_DO_DEF_HPP_
#define _FIRST_TIME_DO_DEF_HPP_

#include "logic/type_def.hpp"

namespace hld
{
#pragma pack(push,1)

	struct s_first_time_do_record
	{
		int32 do_type;

		bool is_valid()
		{
			if (e_first_time_do_type_none == do_type)
			{
				return false;
			}
			return true;
		}

		s_first_time_do_record()
		{
			clear_data();
		}

		void clear_data()
		{
			do_type = e_first_time_do_type_none;
		}
	};

#pragma pack(pop)
}

#endif