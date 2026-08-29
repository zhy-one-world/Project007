
/********************************************************************
created: 2016Äê9ÔÂ13ÈÕ17:46:11
file base: worship_target
file ext: h
author: zhangminghai

purpose: about rankings
*********************************************************************/
#ifndef _WORSHIP_TARGET_H_
#define _WORSHIP_TARGET_H_


#include "Logic/ranking_def.hpp"

namespace faith
{


	class player;
	class cworship_target
	{
		
	public:
		cworship_target();
		~cworship_target(void);


	public:
		void	fast_heart_tick(const int64& new_time);
		void	clear_data();
		void	load_worship_target_from_db(const worship_record* data, int32 data_num);
		bool    load_worship_by_db_lua(const char *data_ptr, int32 data_len);
		void	save_worship_target_to_db(int32 save_type);
		void	set_player_ptr(player* player_ptr);
		void	req_worship_player(e_RankingIndex ranking_index, int32 worship_type,const guid_64& target_player);
		void	req_worship_player_end(e_RankingIndex ranking_index,const int32& worship_type, const guid_64& target_guid, const int32& worship_value, const int32& worship_result);
		void	send_worhip_record();

		void    add_new_record(const guid_64 target_guid, const int32 ranking_index);
		
		void	fresh_data();
		void	set_is_worshiping(bool is_worshiping);


	private:
		bool										m_is_worshiping;
		int64										m_force_reset_worshiping_time;
		player*										m_player_ptr;
		worship_record								m_my_record[DaliyWorshipTime];

	};
}

#endif
