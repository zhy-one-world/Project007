/********************************************************************
created: 2016/4/21 
created: 21:4:2016 16:57:56
file base: belief
file ext: h
author: W.H.B.A.(王黄博昂)

purpose: about role's belief
*********************************************************************/
#ifndef _BELIEF_H_
#define _BELIEF_H_

#include <Logic/belief_def.hpp>
#include "template/ServerTemplateHead.h"

namespace hld
{
	class cbelief
	{
	public:
		cbelief(void);
		~cbelief(void);
	public:											
		void										heart_tick(const int64& new_time);																	//
		void										clear_data();																					//清空信仰数据
		int32										get_belief_ins_data(int32 eIndex);																			// get belief data

	public:
		const s_belief_info&						get_belief_inst() const { return m_belief_data; }												//获取实例数据	
		void										set_belief_data(const s_belief_info& belief_data);												//设置全部实例信息

		int32										get_data_info(int32 eIndex) const { return m_belief_data.data_ary[eIndex]; }				//获取实例数据
		void										set_data_info(int32 eIndex, int32 iValue) { m_belief_data.data_ary[eIndex] = iValue; }		//设置单项实例数据

		BeliefTemplate*								get_belief_info_ptr() const { return m_belief_info_ptr; }										//模板指针
		void										refresh_template();
	public:
		int32										get_buff_id();

	private:
		s_belief_info								m_belief_data;						// 女神实例数据	
		BeliefTemplate*								m_belief_info_ptr;					// 女神模板数据指针,减少寻找指针操作数量
	};
}

#endif
