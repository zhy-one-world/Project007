#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void AwakenTemplateSize();
#pragma pack(push, 1)
    struct AwakenTemplate : public template_base
    {
        int32 Level;
        std::vector<float> AttArray;
        int32 MatirialID;
        int32 MatirialNum;
        int32 GradeLimit;
        int32 NeedPower;
        int32 LevelMax;
    };
#pragma pack(pop)
}
