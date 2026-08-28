#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void GrowUpFundTemplateSize();
#pragma pack(push, 1)
    struct GrowUpFundTemplate : public template_base
    {
        int32 Type;
        int32 Index;
        int32 TargetStrID;
        int32 TargetValue;
        int32 RewardStrID;
        std::vector<int32> MoneyRewardArray;
        int32 ImageRes;
        int32 ShowOrder;
    };
#pragma pack(pop)
}
