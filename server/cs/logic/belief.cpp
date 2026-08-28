/********************************************************************
created: 2016/4/21
created: 21:4:2016 17:17:59
file base: belief
file ext: h
author: W.H.B.A.(Íõ»Æ²©°º)

purpose: 
*********************************************************************/

#include "belief.h"
#include "utility/guid_gen.h"
#include "template/template_manager.h"

namespace hld
{
	cbelief::cbelief(void)
	{
		clear_data();
	}
	cbelief::~cbelief(void)
	{
	}

	void cbelief::heart_tick(const int64& new_time)
	{
	}
	void cbelief::clear_data()
	{
		m_belief_data.clear_data();
		m_belief_info_ptr = nullptr;
	}

	int32 cbelief::get_belief_ins_data(int32 eIndex)
	{
		if (eIndex < 0 || eIndex >= hld::e_belief_info_max)
		{
			return 0;
		}

		return m_belief_data.data_ary[eIndex];
	}

	void cbelief::set_belief_data(const s_belief_info& belief_data)
	{
		memcpy(&m_belief_data, &belief_data, sizeof(m_belief_data));
		refresh_template();
	}

	void cbelief::refresh_template()
	{
		m_belief_info_ptr = GET_TEMPLATE(BeliefTemplate, m_belief_data.data_ary[e_belief_info_template_id] + m_belief_data.data_ary[e_belief_info_upgrade_count]);
	}

	int32 cbelief::get_buff_id()
	{
		int32 buff_id = 0;
		if (m_belief_info_ptr == nullptr)
		{
			return buff_id;
		}

		if (GAMECONFIG->BeliefBuffArray.size() <= 0)
		{
			return buff_id;
		}

		if (m_belief_data.data_ary[e_belief_info_upgrade_count] == 0)
		{//Ã»½âËø
			return buff_id;
		}

		int32 buff_index = -1;
		for (int32 i = 0; i < e_belief_buff_level_max; ++i)
		{
			if (m_belief_info_ptr->CurRankNumber >= belief_buff_req_array[i])
			{
				buff_index = i;
			}
			else
			{
				break;
			}
		}
		if (buff_index < 0)
		{
			return buff_id;
		}

		int32 real_index = m_belief_info_ptr->BeliefType * e_belief_buff_level_max + buff_index;
		if (real_index < 0
			|| real_index >= GAMECONFIG->BeliefBuffArray.size())
		{
			return buff_id;
		}
		buff_id = GAMECONFIG->BeliefBuffArray[real_index];
		return buff_id;
	}

}
