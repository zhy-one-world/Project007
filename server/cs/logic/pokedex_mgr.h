/********************************************************************
created: 2015年6月18日15:55
file base: pokedex_mgr
file ext: h
author: XingLong

purpose: about role's pokedex_mgr
*********************************************************************/
#ifndef _POKEDEX_MGR_H_
#define _POKEDEX_MGR_H_

#include "pokedex.h"
#include "Logic/pokedex_def.hpp"

namespace hld
{
	class player;
	class cpokedex_mgr
	{
	public:
		cpokedex_mgr();
		~cpokedex_mgr(void);
	public:
		void									heart_tick(const int64& new_time);
		void									init_pokedex_data_info();
		void									set_player_ptr(player* player_ptr);
		void									clear_data();
		void									load_pokedex_from_db(const s_pokedex_info* pokedex_data, int32 pokedex_num);
		bool                                    load_pokedex_by_db_lua(const char *data_ptr, int32 data_len);
		void									save_pokedex_to_db(int32 save_type);
		int32									get_all_active_pokedex_num();
		int32									get_active_pokedex_num();			//魂魄激活数量
		void									init_manager(const s_pokedex_info* pokedex_data, int32 pokedex_num);
	public:
		void									send_pokedex_all();
		void									send_pokedex_one(int32 pokedex_template_id);
		void									submit_psyche(int32 pokedex_id);
		cpokedex*								get_pokedex_by_templateid(int32 templayeid);
		cpokedex*								get_pokedex_by_index(int32 index);
		cpokedex*								get_palace_pokedex();
		void									check_activate_fetter(int32 pokedex_template_id);
		float									get_add_percent();

	private:
		cpokedex								                    m_pokedex_ary[POKEDEX_NUM_MAX];								// 所有图鉴
		player*													    m_player_ptr;												// 图鉴所属角色名称		
	};
}

#endif
