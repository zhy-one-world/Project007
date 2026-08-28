#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void RegionTemplateSize();
#pragma pack(push, 1)
    struct RegionTemplate : public template_base
    {
        int32 RegionCode;
    };
#pragma pack(pop)
}
