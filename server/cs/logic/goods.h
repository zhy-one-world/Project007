/********************************************************************
created: 2016年7月4日11:14:26
file base: mission
author: zhangminghai
purpose: 单个商品
*********************************************************************/
#ifndef _GOODS_HPP_
#define _GOODS_HPP_

#include <Logic/goods_def.hpp>
#include "template/ServerTemplateHead.h"
namespace hld
{
	class cgoods
	{
	public:
		cgoods(void);
		~cgoods(void);
	public:
		void										set_data_use(bool use_value) { m_data_use = use_value; }
		bool&										is_data_use() { return m_data_use; }
		void										clear_data();
		GoodsTemplate*								get_goods_template_ptr() { return m_goods_info_ptr; }
		const s_goods_info&							get_goods_inst() const { return m_goods_data; }
		int32										get_goods_data_by_index(e_goods_info eIndex) {return m_goods_data.data_ary[eIndex];}
		void										set_goods_data(const s_goods_info& goods_data);
		void										set_goods_data_by_index(e_goods_info eIndex, int32 iValue) { m_goods_data.data_ary[eIndex] = iValue; }	
		ItemTemplate*								get_item_template_ptr();
	private:
		bool										m_data_use;
		s_goods_info								m_goods_data;
		GoodsTemplate*								m_goods_info_ptr;
	};
}
#endif
