
/********************************************************************
created: 2016年9月13日17:46:11
file base: rank_mgr
file ext: h
author: zhy

purpose: about rankings
*********************************************************************/
#ifndef _RANKING_MGR_H_
#define _RANKING_MGR_H_


#include "Logic/ranking_def.hpp"
#include "item.h"


namespace faith
{
	class player;

	struct ranking_data_cs
	{
		int64	ranking_value;
		int64   ranking_time;
		bool	is_update;
		bool	bflag_is_tick_update;
		int32				Peak;
	};

	class cranking_mgr
	{
	public:
		cranking_mgr();
		~cranking_mgr(void);
		void					clear_data();
		void					tick(const int64& tick_time);
		void					set_player_ptr(int32 array_index);
		void					init_ranking_data();
		void					init_ranking_time_and_update();
		bool					get_has_init() { return m_bflag_has_init; };


	public:
		void					set_is_wait_tick_update(e_RankingIndex ranking_type, bool is_wait_tick);
		bool					get_is_wait_tick_update(e_RankingIndex ranking_type);
		void					set_ranking_data_update_flag(e_RankingIndex ranking_type, bool bflag);
		bool					get_ranking_data_update_flag(e_RankingIndex ranking_type);
		bool 					make_ranking_data(s_ranking_player_info& ranking_data, e_RankingIndex ranking_type, player& player_ref, int32& template_id, int32 Peak = -1);

		void					set_ranking_value(e_RankingIndex ranking_type, const  int64& ranking_value);
		void					req_sync_player_rank_data_one(e_RankingIndex ranking_type, int32 supporting_id = -1, int32 Peak = -1);
		bool					check_is_need_set(e_RankingIndex ranking_type, const int64& ranking_value);
		int32					get_player_class_type() { return m_player_class_type;};

		void					set_and_sync_ranking_data(e_RankingIndex ranking_type, const int64& ranking_value, int32 supporting_data_id = -1, int32 Peak = -1);
		void					set_and_sync_single_equip_ranking_data(citem* item_ptre);
		void					set_sub_class_type_ranking_data(e_RankingIndex ranking_type, const int64& ranking_value, int32 Peak = -1);
		void					sync_vip_level_to_ws_ranking();
		void					set_and_sync_ranking_data_str(e_RankingIndex ranking_type, xstring ranking_value, int32 supporting_data_id = -1, int32 Peak = -1);

	public:
		void					save_ranking_to_db();
		void					set_worship_value(const  int64& worship_value) { m_worship_value = worship_value; };
		int64					get_worship_value() { return m_worship_value; };

	private:
		int32					m_array_index;
		bool					m_bflag_has_init;
		
		int32					m_player_class_type;
		ranking_data_cs			m_ranking_data[e_RankingIndex_max];
		int64					m_worship_value;		//被崇拜次数
		int64					m_next_tick_value;		//分钟tick
	};
}

#endif
