/********************************************************************
created: 2016年7月4日11:14:26
file base: mission
author: zhangminghai
purpose: 单个商品
*********************************************************************/

#include "goods.h"
#include "template/template_manager.h"

namespace faith
{
	cgoods::cgoods()
	{
		clear_data();
	}

	cgoods::~cgoods(void)
	{

	}
	void cgoods::clear_data()
	{
		m_data_use = false;
		m_goods_data.clear_data();
		m_goods_info_ptr = nullptr;
	}

	void  cgoods::set_goods_data(const s_goods_info& goods_data)
	{
		memcpy(&m_goods_data, &goods_data, sizeof(goods_data));
		m_goods_info_ptr = GET_TEMPLATE(GoodsTemplate, get_goods_data_by_index(e_goods_info_id));
	}
	
	ItemTemplate* cgoods::get_item_template_ptr()
	{
		if (nullptr == m_goods_info_ptr)
		{
			return nullptr;
		}
		ItemTemplate* item_temp_ptr = GET_TEMPLATE(ItemTemplate, m_goods_info_ptr->ItemId);
		return item_temp_ptr;
	}
}