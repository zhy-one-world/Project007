#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void WheelPlateTemplateSize();
#pragma pack(push, 1)
    struct WheelPlateTemplate : public template_base
    {
        std::vector<int32> CurItem;
    };
#pragma pack(pop)
}
