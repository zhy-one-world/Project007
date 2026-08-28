#pragma once
#include "logic/type_def.hpp"

namespace hld
{
	class item_set;

	class item_manager
	{
	public:
		static void init_manager();
	public:
		static void heart_tick(int32 unit_array_index, const int64& new_time);
		static void clear_data(int32 unit_array_index);
		static item_set& get_item_set(int32 unit_array_index);
	};
}
