#include <cmath>
#include "region_circle.hpp"

namespace hld
{
    region_circle::region_circle(const fvector& center, const i32 radius):
        m_center(center),
        m_radius(radius),
        m_radius_quare(radius * radius)
    {

    }

    bool region_circle::in_region(const fvector& pos)
    {
        i32 dx = pos.x - m_center.x;;
        i32 dy = pos.y - m_center.y;
        return dx * dx + dy * dy <= m_radius_quare;
    }

	/* rectangle region_circle::get_outer_rect()
	 {
		 rectangle rc;
		 rc.left = m_center.x - m_radius;
		 rc.right = m_center.x + m_radius + 1;
		 rc.bottom = m_center.y - m_radius;
		 rc.top = m_center.y + m_radius + 1;
		 return rc;
	 }*/

    //region_circle* region_circle::create(const position_2d& center, const i32 radius)
    //{
    //    region_circle * region = nullptr;
    //    try
    //    {
    //        region_circle * region = new region_circle(center, radius);
    //    }
    //    catch (...)
    //    {
    //        return nullptr;
    //    }
    //    return region;
    //}

    //void region_circle::destroy(region* _region)
    //{
    //    if (_region)
    //    {
    //        delete _region;
    //    }
    //}
}
