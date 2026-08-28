/********************************************************************
created: 2016Äê8ÔÂ23ÈÕ13:56:28
file base: ranking_cross
file ext: cpp
author: zhangminghai

purpose: about arena system's arena_mgr_ws
*********************************************************************/

#include "ranking_cross.h"
namespace hld
{
	void ranking_cross::clear_data()
	{
		for (int32 i = 0; i < e_RankingIndex_max; ++i)
		{
			m_ranking_all[i].clear();
		}
	}
}
