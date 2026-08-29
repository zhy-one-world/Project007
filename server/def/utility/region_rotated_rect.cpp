
#include "region_rotated_rect.hpp"

namespace faith
{
    region_rotated_rect::region_rotated_rect()
    {
        m_half_length = 0;
        m_half_width = 0;
    }

    void region_rotated_rect::set_value(const fvector& pos, bool b_center, const int32 length, const int32 width, frotator rot)
    {
    }
    bool region_rotated_rect::in_region(const fvector&& pos)
    {
            return false;
    }
}
