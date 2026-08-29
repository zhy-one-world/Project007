#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ActiveDegreeTemplateSize();
#pragma pack(push, 1)
    struct ActiveDegreeTemplate : public template_base
    {
        int32 Type;
        int32 ActivityName;
        int32 TargetValue;
        int32 AddValue;
        std::vector<int32> WidgetJumpArray;
        int32 CanGoto;
    };
#pragma pack(pop)
}
