/********************************************************************
created:	2016/04/01
filename: 	fuben_data_set.cpp
file base:	fuben_data_set
file ext:	    cpp
author:		Steven.Han
purpose:
*********************************************************************/

#include "fuben_data_set.h"

namespace hld 
{
    tag_fuben_data* fuben_data_set::get_fuben_data(int32 fuben_id)
    {
        fuben_data_map::iterator it = m_data_map.find(fuben_id );
        if (it != m_data_map.end())
        {
            tag_fuben_data* ret = &( it->second );
            return ret;
        }

        return nullptr;
    }

    void fuben_data_set::add_fuben_data( int32 fuben_id, tag_fuben_data& fuben_data)
    {
        tag_fuben_data* data_ptr = get_fuben_data(fuben_id);
        if (nullptr != data_ptr)
            return;

        std::pair< int32, tag_fuben_data > pair = std::make_pair(fuben_id, fuben_data);
        m_data_map.insert(pair);
    }

    void fuben_data_set::clear_enter_count(void)
    {
        for (fuben_data_map::iterator it = m_data_map.begin(); it != m_data_map.end(); ++it)
        {
            it->second.enter_count = 0;
        }
    }

    void fuben_data_set::clear(void)
    {
        m_data_map.clear();
    }

    
}
