/********************************************************************
created: 2016年8月23日14:56:28
file base: ranking_cross
file ext: hpp
author: zhangminghai

purpose: about arena system's arena_mgr
*********************************************************************/
#ifndef _RANKING_CROSS_H_
#define _RANKING_CROSS_H_

#include "Logic/ranking_def.hpp"

namespace faith
{
	class  ranking_cross
	{
	public:
		ranking_cross() { clear_data(); };
		~ranking_cross() {};
	public:
	public:
		void							clear_data();
	public:
		ranking_list*					get_rank_list() { return m_ranking_all; }
		ranking_list&					get_rank_list(e_RankingIndex ranking_index) { return m_ranking_all[ranking_index]; }
	private: 
		ranking_list					m_ranking_all[e_RankingIndex_max];
	};
	
}
#endif
