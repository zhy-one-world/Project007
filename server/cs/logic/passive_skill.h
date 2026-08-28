/*@@

	file name :	passive_skill.h
	
	author : zhy
	
	description : 
	
	change list :

@@*/

#ifndef _PASSIVE_SKILL_H_
#define _PASSIVE_SKILL_H_

//////////////////////////////////////////////////////////////////////////
//
//	file include
//
//////////////////////////////////////////////////////////////////////////
#include "Logic/skill_def.hpp"

//////////////////////////////////////////////////////////////////////////
//
//	class declare
//
//////////////////////////////////////////////////////////////////////////

namespace hld
{
	class skill_s2s_sl_passive_skill_info;
	class player;

	const int32 special_passive_skill_slot = 10000;

	struct s_passive_skill_info
	{
		int32 data_ary[e_passive_skill_info_max];
		s_passive_skill_info()
		{
			clear_data();
		}
		void clear_data()
		{
			memset(this, 0, sizeof(*this));
		}
	};

	class passive_skill
	{
	public:
		passive_skill();
		~passive_skill();
	public:
		void clear_data();
		void heart_tick(const int64& new_time, const int32& tick_time);
		void set_unit_index(int32 array_index) { m_array_index = array_index; }
	public:
		void set_data_info(int32 passive_skill_id, e_passive_skill_info e_info, int32 data_value);
		int32 get_data_info(int32 passive_skill_id, e_passive_skill_info e_info);
	public:
		void save_passive_skill(int32 save_type_ex);
		void load_passive_skill(const skill_s2s_sl_passive_skill_info& load_data);
		void send_passive_info_one(int32 skill_series);
		void send_passive_info_all();
		int32 get_passive_skill_id_by_buff_action_id(int32 buff_action_id);
		s_passive_skill_info* get_passive_skill_by_series(int32 passive_skill_series);
		int32 get_passive_skill_equip_count();
	public:
		int64 get_passive_skill_gs();
	public:
		void passive_skill_up_level(int32 passive_skill_id);
		int32 passive_skill_equip(int32 passive_skill_id, int32 slot = 0);
		void passive_skill_special(int32 passive_skill_id, bool is_add);
	private:
		void set_passive_skill_att(int32 passive_skill_series, bool is_add);
		void set_passive_skill_buff(int32 passive_skill_series, bool is_add);
	private:
		int32 m_array_index;
		std::map<int32, s_passive_skill_info> m_passive_skill_map;
	};
}

#endif
