
#include <math.h>
#include "region_sector.h"
#include "logic/type_def.hpp"
#include "vector3d.h"

namespace faith
{
    region_sector::region_sector(const fvector& center, const i32 radius, frotator rot, const i32 angle):
        m_center(center),
        m_radius(radius),
        m_radius_quare(radius * radius),
        m_rot(rot),
        m_angle(angle)
    {

    }

    bool region_sector::in_region(const fvector& pos)
    {
        bool is_in_region_radius = false;
        bool is_in_region_angle = false;

        ////是否在半径范围内
        //i32 dx = pos.x - m_center.x;;
        //i32 dy = pos.y - m_center.y;
        //is_in_region_radius = dx*dx + dy*dy <= m_radius_quare;
        //if (!is_in_region_radius)
        //{
        //    return false;
        //}

        //f32 ddy = (f32)dy;
        //f32 ddx = (f32)dx;

        ////被测量的目标与正方向的夹角
        //f32 angle;
        //angle = (f32)::atan2(ddx, ddy);
        //angle *= math::RADTODEG64;
        ////angle -= 90.0f;

        //if (angle < 0.0f) 
        //    angle += 360.0f;
        //if (angle >= 360.0f) 
        //    angle -= 360.0f;

        ////正方向
        //if (m_rot.yaw >= 0)
        //{
        //    m_rot.yaw = m_rot.yaw % 65535;
        //}
        //else
        //{
        //    m_rot.yaw = (m_rot.yaw % 65535)*(-1);
        //}

        //f32 rot_angle = (m_rot.yaw*2.0f / 65535.0f)*(180.0f);

        //if (rot_angle < 0.0f) 
        //    rot_angle += 360.0f;
        //if (rot_angle >= 360.0f) 
        //    rot_angle -= 360.0f;
        //rot_angle = 450.0f - rot_angle;
        //if (rot_angle < 0.0f) 
        //    rot_angle += 360.0f;
        //if (rot_angle >= 360.0f) 
        //    rot_angle -= 360.0f;

        ////判断被测试的目标点，和正方向的夹角是否在 扇形 角度内
        //f32 fangle = (f32)(m_angle);
        //if ((angle - rot_angle) <= (f32)(fangle) / 2.0f && (angle - rot_angle) >= 0.f)
        //{
        //    is_in_region_angle = true;
        //}
        //else if ((angle - rot_angle) < 0.f && (angle - rot_angle) >= -fangle / 2.0f)
        //{
        //    is_in_region_angle = true;
        //}
        //else
        //{
        //    is_in_region_angle = false;
        //}

        return (is_in_region_radius && is_in_region_angle);
    }

    //rectangle region_sector::get_outer_rect()
    //{
    //    rectangle rc;
    //    rc.left = m_center.x - m_radius;
    //    rc.right = m_center.x + m_radius + 1;
    //    rc.bottom = m_center.y - m_radius;
    //    rc.top = m_center.y + m_radius + 1;
    //    return rc;
    //}

    //region_sector* region_sector::create(const position_2d& center, const i32 radius, rotator rot, const i32 angle)
    //{
    //    region_sector* region = nullptr;
    //    try
    //    {
    //        region = new region_sector(center, radius, rot, angle);
    //    }
    //    catch (...)
    //    {
    //        return nullptr;
    //    }
    //    return region;
    //}

    //void region_sector::destroy(region* _region)
    //{
    //    if (_region)
    //    {
    //        delete _region;
    //    }
    //}
}
