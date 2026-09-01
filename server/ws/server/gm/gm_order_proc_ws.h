#pragma once

#include "logic/type_def.hpp"
#include "Logic/gm_order_def.hpp"

using namespace std;

namespace faith
{
	class gm_order_proc_ws 
	{
		gm_order_proc_ws();
		~gm_order_proc_ws();
	public:
		static void proc_gm_order(e_gm_order_type order_type, const int32 *order_param, int32 param_num);//处理gm指令专用
	};
}