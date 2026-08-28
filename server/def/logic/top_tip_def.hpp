#ifndef _TOP_TIP_DEF_HPP_
#define _TOP_TIP_DEF_HPP_

#include "logic/type_def.hpp"

namespace hld
{
#pragma pack(push,1)

	const int32 max_plot_tip_entry_num = 5;
	const int32 pop_plot_tip_interval = 3000;

	// 这个枚举是从剧情提示数组中提示每条提示时用的数组下表
	enum e_plot_tip_arr_tuple_index
	{
		e_plot_tip_arr_tuple_index_type,
		e_plot_tip_arr_tuple_index_time,
		e_plot_tip_arr_tuple_index_str_id,
		e_plot_tip_arr_tuple_index_max
	};

	enum e_plot_type
	{
		e_plot_type_when_create,	// 创建时弹出提示
		e_plot_type_when_destroy,	// 销毁时弹出提示
		e_plot_type_after_begin,	// 创建一段时间后弹出提示
		e_plot_type_before_end,		// 销毁之前的一段时间弹出提示
		e_plot_type_max
	};

	struct s_plot_tip_entry
	{
		e_plot_type	type;		// 剧情提示的弹出类型
		int32		time;		// 剧情提示的弹出时间
		int32		str_id;		// 剧情提示的字符串ID

		s_plot_tip_entry()
		{
			clear_data();
		}

		void clear_data()
		{
			memset(this, 0, sizeof(s_plot_tip_entry));
		}

		bool is_valid()
		{
			return (str_id > 0 ? true : false);
		}
	};

#pragma pack(pop)
}

#endif
