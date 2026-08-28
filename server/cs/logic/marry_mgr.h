#ifndef _MARRY_MGR_H_
#define _MARRY_MGR_H_

#include "item.h"

namespace hld
{
	class marry_mgr
	{
	public:
		marry_mgr();
		~marry_mgr();

	public:
		void									set_unit_index(int32 array_index) { m_array_index = array_index; };

		//激活婚戒属性（根据消耗物品设置初始等级）
		void									activate_wedding_ring(int32 marry_item_id = 0);

		//关闭婚戒属性（离婚使用）
		void									close_wedding_ring();

		//根据ws发来的消息刷新状态
		void									refresh_wedding_ring_info(bool is_marry, int32 marry_item_id, int32 mate_ring_level, int32 heart_value);

		//刷新对方培养等级
		void									refresh_mate_wedding_ring_level(int32 mate_ring_level);

		//培养婚戒属性
		void									wedding_ring_upgrade(int32 use_up_item);

		//获取培养等级
		int32									get_ring_level();

		//刷新灵犀值
		void									refresh_heart_value(int32 heart_value);

		//增加灵犀值 （会请求ws添加并同步）
		void									add_heart_value(int32 add_value, int32 send_type = 0);

		int32									get_heart_value();

		//扣除祝福所需花费
		void									cost_bless_money(const guid_64& target_guid);

		//使用烟花道具
		void									req_use_fireworks(const guid_64& obj_guid, const std::string& target_name, const std::string& target_blessing);

		//结缘相关操作
		void									req_lucky_operator(e_lucky_info_req_type type);

		//计算属性加成
		//void									set_att_all(bool is_add) {}

		//------------答题相关----------------
		void									begin_marry_dati(bool is_near);
		void									confirm_marry_dati(int32 is_receive);
		void									confirm_marry_operate(int32 operate_type, int32 ext_param_1, int32 ext_param_2);
		void									reset_marry_dati();
		void									sync_dati_marry(int32 stage);
		void									sync_heart_value();
	private:
		int32									m_array_index;
	};


}
#endif
