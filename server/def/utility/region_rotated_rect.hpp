#ifndef __REGION_ROTATED_RECT_H_
#define __REGION_ROTATED_RECT_H_

#include "logic/type_def.hpp"
#include "region.hpp"
#include "vector3d.h"

namespace faith
{
    class region_rotated_rect : public region
    {
    public:
        region_rotated_rect();
        void set_value(const fvector& pos, bool bcenter, const i32 length, const i32 width, frotator rot);
        bool in_region(const fvector&& pos) override;
        bool in_region(const fvector& pos) override { return false; };
    private:
        fvector m_center;
        i32 m_half_length;
        i32 m_half_width;
        fvector m_heading;
        fvector m_side;
    };
}

#endif