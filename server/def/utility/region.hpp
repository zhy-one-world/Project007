#ifndef __REGION_H_
#define __REGION_H_

#include "logic/type_def.hpp"
#include "logic/math.hpp"

namespace faith
{
    //����ӿ�
    class region
    {
    public:
        virtual bool in_region(const fvector& pt) = 0;
        virtual bool in_region(const fvector&& pt) = 0;
        virtual ~region(){};
        virtual bool is_empty() { return false; }
    };
}

#endif
