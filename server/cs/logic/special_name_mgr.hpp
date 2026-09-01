/********************************************************************
created: 2016年7月14日13:56:28
file base: Special_name_mgr
file ext: h
author: zhangminghai

purpose: about role's Special_name_mgr
*********************************************************************/

#ifndef _SPECIAL_NAME_MGR_H_
#define _SPECIAL_NAME_MGR_H_

#include <Logic/special_name_def.hpp>
#include "template/ServerTemplateHead.h"

namespace faith
{
	class unit;

	class special_name_mgr
	{
	public:
		special_name_mgr();
		~special_name_mgr(void);

		void	set_player_ptr(int32 array_index) { m_array_index = array_index; }
		void	load_data_from_db(const s_special_name_info& special_name_data);
		bool    load_data_by_db_lua(const char *data_ptr, int32 data_len);
		void	save_data_to_db(int32 save_type);
		void	clear_data();
		//void	add_special_name(int32 special_name_id,int32 name_type);
		int32	get_value_by_id(int32 id);
		bool	set_value_by_id(int32 id, int32 value);
		int32	get_name_level_by_type(int32 name_type);
		void	remove_special_name(int32 name_type, int32 name_level);
		bool	upgrade_sepcial_name(int32 name_type, int32 name_level);
		bool    promote_special_name(int32 name_type);
		void    send_special_name_all();
		//void	special_name_change_att(std::vector<f32> const & att_array, int32 add_count, bool is_add);
		void	send_special_name_upgrade_notice(const int32& name_type);
		void	init_client_special_name();
		void	send_special_name(unit& temp_player);
		void	sync_ranking_special_name(int32 name_type);
		void	sync_military_rank_to_ws();
		void	send_special_name_operate_end_msg(bool success, int32 name_type,int32 operate_type);

		int32	get_achievement_title_level();
		int32   get_solider_rank_level();
		int32	get_solider_rank_star();
	private:
		s_special_name_info							m_special_name_ary;
		int32										m_array_index;//所属玩家
		
	};
}
#endif