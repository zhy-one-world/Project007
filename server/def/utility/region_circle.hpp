#ifndef __REGION_CIRCLE_H_
#define __REGION_CIRCLE_H_

#include "region.hpp"
#include "logic/math.hpp"

namespace faith
{
    class region_circle
    {
    public:
        region_circle(const fvector& center, const i32 radius);
        bool in_region(const fvector& pos);
      //  rectangle get_outer_rect();

        //static region_circle* create(const position_2d& center, const i32 radius);
        //static void destroy(region* _region);
    private:
		fvector m_center;
        i32 m_radius;
        i32 m_radius_quare;
    };
}

#endif
