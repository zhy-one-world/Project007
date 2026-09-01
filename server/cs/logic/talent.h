/********************************************************************
created: 2017年5月26日16:30:43
file base: talent
file ext: h
author: zhy

purpose: about role's talent
*********************************************************************/
#ifndef _TALENT_H_
#define _TALENT_H_

#include <Logic/talent_def.hpp>
#include "template/ServerTemplateHead.h"

namespace faith
{
	class ctalent
	{
	public:
		ctalent(void);
		~ctalent(void);
	public:											
		void										heart_tick(const int64& new_time);																//心跳
		void										clear_data();																					//清空数据
		bool&										is_valid();
		void										set_valid_state(bool is_use) { m_is_data_use = is_use; }
	public:
		const s_talent_info&						get_talent_inst() const { return m_talent_data; }												//获取实例数据	
		void										set_talent_data(const s_talent_info& talent_data);												//设置全部实例信息

		int32										get_data_info(int32 eIndex) const { return m_talent_data.data_ary[eIndex]; }					//获取实例数据
		void										set_data_info(int32 eIndex, int32 iValue) { m_talent_data.data_ary[eIndex] = iValue; }			//设置单项实例数据

		int32&										get_talent_template_id() { return m_talent_data.data_ary[e_talent_info_template_id]; }
		int32										get_talent_series();
		TalentTemplate*								get_talent_template_ptr() const { return m_talent_template_ptr; }								//模板指针

		void										set_talent_buff(const int32& unit_array_index, bool is_add);
		void										set_talent_skill(const int32& unit_array_index, bool is_add);

		int32										get_talent_gs_value();
	public:
		void                                        lua_set_talent_data(int32 template_id, int32 cast_money);

	private:
		bool										m_is_data_use;
		s_talent_info								m_talent_data;							// 天赋实例数据	
		TalentTemplate*								m_talent_template_ptr;					// 赋模板数据指针,减少寻找指针操作数量
	};
}

#endif
