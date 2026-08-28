/********************************************************************
created: 2017-5-26 16:45:37
file base: treasure_mgr
file ext: h
author: zhy

purpose: about role's treasure_mgr
*********************************************************************/
#ifndef _TREASURE_MGR_H_
#define _TREASURE_MGR_H_
#include "Logic/treasure_def.hpp"

namespace hld
{
	class ctreasure_mgr
	{
	public:
		ctreasure_mgr();
		~ctreasure_mgr(void);
	public:
		void heart_tick(const int64& new_time);
		void set_player_ptr(int32 unit_array_index);
		void clear_data();
	public:
		void save_treasure_to_db(int32 save_type);
		void load_treasure_by_db(const s_treasure_record* treasure_data, int32 data_num);
		void send_treasure_all();
	public:
		void treasure_prize(int32 treasure_type, int32 prize_type, int32 cost_money_type, int32 cost_money_value, int32 prize_score, int32 prize_luck, int32 item_id, int32 item_num, int32 prize_list[treasure_prize_num]);
		void send_treasure_notice(int32 item_id, int32 item_quality, int32 operate_type);
	private:
		std::list<s_treasure_record>	m_prize_record_list;
		int32				m_unit_array_index;						// Ìì¸³ËùÊô½ÇÉ«
	};
}

#endif
