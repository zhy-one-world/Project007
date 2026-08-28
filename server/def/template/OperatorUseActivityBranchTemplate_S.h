#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void OperatorUseActivityBranchTemplateSize();
#pragma pack(push, 1)
    struct OperatorUseActivityBranchTemplate : public template_base
    {
        int32 ActivityType;
        int32 CycleIndex;
        std::vector<int32> Condition;
        std::vector<int32> Reward;
        std::vector<int32> TextImage;
        std::vector<int32> ParamArr1;
        std::vector<int32> ParamArr2;
        std::vector<int32> ParamArr3;
        std::vector<int32> ParamArr4;
        std::vector<int32> ParamArr5;
    };
#pragma pack(pop)
}
