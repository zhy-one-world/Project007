#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void LegionSalaryTemplateSize();
#pragma pack(push, 1)
    struct LegionSalaryTemplate : public template_base
    {
        int32 AimType;
        int32 AimTypeName;
        int32 AimTypeImage;
        std::vector<int32> SubTypeId;
        std::vector<int32> SubAimReward;
        int32 SubAimRewardName;
        std::vector<float> JobRate;
    };
#pragma pack(pop)
}
