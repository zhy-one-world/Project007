/********************************************************************
created: 2016年9月24日13:56:28
file base: Special_name_mgr
file ext: h
author: zhangminghai

purpose: about role's Special_name_mgr
*********************************************************************/

#ifndef _CONVERT_MGR_H_
#define _CONVERT_MGR_H_

#include "template/ServerTemplateHead.h"
#include "Logic/char_def.hpp"

namespace faith
{
	class player;
	enum e_convert_end_type
	{
		e_convert_end_success = 0,
		e_convert_end_times,
		e_convert_bind_money,
		e_convert_bind_unkonw
	};


	class convert_mgr
	{
	public:
		convert_mgr();
		~convert_mgr(void);

		void	set_player_ptr(player* ptr) { m_player_ptr = ptr; }
		void	load_data_from_db(const int32* alltime);
		bool    load_convert_by_db_lua(const char *data_ptr, int32 data_len);
		void	save_data_to_db(int32 save_type);
		void	clear_data();
		void	reset_convert_data(bool is_begin);
		void	heart_tick(const int64& new_time);
		void    send_info_all();
		void    send_info_one(int32 convert_type);
		
		int32	get_convert_time(e_convert_type convert_type);
		int32	get_m_all_convert_time(e_convert_type convert_type);
		int32	get_m_vip_convert_time(e_convert_type convert_type);

		void    set_m_all_convert_time(e_convert_type convert_type , int32 new_m_all_convert_time);
		xstring lua_get_time_to_str(e_time_type time_type);
		
		void	force_refresh();
	private:
		int32										m_all_convert_time[e_convert_type_max];
		int32										m_vip_convert_time[e_convert_type_max];
		player*										m_player_ptr;//所属玩家
	};
}
#endif