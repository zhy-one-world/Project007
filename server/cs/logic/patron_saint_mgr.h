/********************************************************************
created: 2017年6月8日15:55
file base: patron_saint
file ext: h
author: Lxy

purpose: about role's pokedex_mgr
*********************************************************************/
#ifndef _PATRON_SAINT_MGR_H_
#define _PATRON_SAINT_MGR_H_

#include "pokedex.h"
#include "Logic/pokedex_def.hpp"
namespace hld
{
	class cpatron_saint_mgr
	{
	public:
		cpatron_saint_mgr();
		~cpatron_saint_mgr(void);
	public:
		void														clear_data();
		void														heart_tick();
		void														set_player_array_index(int32 array_index);
		int32														prop_equip(guid_64 mark_guid, int32 target_slot);
		int32														purchase_prop_slot();
		int32														prop_unequip(guid_64 mark_guid);
		void														tinder_operate(guid_64 mark_guid, int32 target_slot, int32 operate_type);
		int32														get_tinder_star_already_used();

		int32														get_mark_level_sum();
		int32														get_mark_activated_num();
		void														auto_get_equip_slot(); // 守护神装备槽根据转生等级自动开启

		void														sync_tinder_data();

		void														save_tinder_info_to_db(int32 save_type);
		void														load_tinder_from_db(const s_tinder_info& tinder_data);
		bool                                                        load_tinder_by_db_lua(const char *data_ptr, int32 data_len);
		void														send_tinder_info_all();
		void														send_tinder_info_one(int32 info_index);
		void														set_tinder_att_all(bool is_add);
		int32														set_unit_skill();
		bool                                                        is_cur_unlock(int32 new_template_id);
		int32														get_tinder_total_upgrade_num();
		bool                                                        is_matiral(TinderTemplate *new_tinder_template_ptr);
		bool                                                        cut_matiral(TinderTemplate *new_tinder_template_ptr);
		int32														protect_skill_upgrade();
		int32														get_tinder_info(int32 info_index);
		void														set_tinder_info(int32 info_index, int32 info_value);


	private:
		int32													    m_unit_array_index;	
		s_tinder_info												m_tinder_info;		
		std::vector<float>											m_empty_array;
	};
}

#endif
