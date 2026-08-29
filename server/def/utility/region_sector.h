#ifndef __REGION_SECTOR_H_
#define __REGION_SECTOR_H_

#include "region.hpp"

namespace faith
{
    class region_sector : public region
    {
    public:
        bool in_region(const fvector& pos);
       // rectangle get_outer_rect();

        //static region_sector* create(const position_2d& center, const i32 radius, rotator rot, const i32 angle);
        //static void destroy(region * region);
    private:
        region_sector(const fvector& center, const i32 radius, frotator rot, const i32 angle);
		fvector	m_center;
        i32 m_radius;
        i32 m_radius_quare;
        frotator	m_rot;		//扇形正方向
        const i32 m_angle;	//扇形角度
    };
}

#endif
