/********************************************************************
created: 2016年10月25日12:02:46
file base: war_state
file ext: h
author: zhy

purpose: about role's war_state
*********************************************************************/
#ifndef _WAR_STATE_H_
#define _WAR_STATE_H_

#include <Logic/war_state_def.hpp>
#include "template/ServerTemplateHead.h"

namespace faith
{
	struct s_unit_war_time
	{
		int32 unit_index;
		int64 war_time;
		bool is_data_use;
		s_unit_war_time()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	typedef std::vector <s_unit_war_time> unit_war_time_vec;
	typedef unit_war_time_vec::iterator unit_war_time_vec_it;
	class c_war_state
	{
	public:
		c_war_state(void);
		~c_war_state(void);
	public:											
		void										heart_tick(const int64& new_time);
		void										clear_data();					
		void										set_unit_ptr(int32 unit_index) { m_unit_array_index = unit_index; }
	public:
		void										war_time_end(int32 unit_index, bool is_self);
		void										add_in_war(int32 unit_index, int32 war_time = -1);
		void										clear_war_state();
	private:
		unit_war_time_vec							m_unit_war_time_vec;//数据数组
		int32										m_unit_war_time_vec_num;//数据数组的数量
		int32										m_unit_war_time_num;//数据数组有多少被使用
		int32										m_unit_array_index;
	};
}

#endif
