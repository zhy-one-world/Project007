#ifndef _GODDESS_MGR_H_
#define _GODDESS_MGR_H_

#include "item.h"

namespace hld
{
	class goddess_mgr
	{
	public:
		goddess_mgr();
		~goddess_mgr();
	public:
		void									set_unit_index(int32 array_index) {m_array_index = array_index;};

	public:
		bool									goddess_gods_on(citem* item, int32 slot);
		bool									goddess_gods_off(citem* item, int32 slot,bool is_auto_off);
		int32									goddess_fight(const guid_64& item_guid);


		citem*									get_fighting_goddess();
		citem*									get_cur_slot_goddess(int32 slot);
		citem*									get_cur_slot_equip(int32 goddess_type,int32 slot);
		citem*									get_select_type_goddess(int32 type);
		void									calcu_cooperate_buff(citem* item_ptr, bool is_add);
		void									calcu_goddess_buff(citem* item_ptr, bool is_add);
		void									enter_reload_goddess();
		void                                    set_star_is_open(citem *item_ptr, bool is_send = false);



	private:
		int32									m_array_index;
	};


}
#endif // !_GODDESS_MGR_H_
