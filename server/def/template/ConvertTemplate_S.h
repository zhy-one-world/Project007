#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ConvertTemplateSize();
#pragma pack(push, 1)
    struct ConvertTemplate : public template_base
    {
        int32 ConvertType;
        std::vector<int32> CostMoneyArray;
        std::vector<int32> StaticRewardMoneyArray;
        std::vector<int32> DyamicRewardMoneyArray;
        std::vector<int32> AdditionalMoneyArray;
        std::vector<int32> RegionArr;
        std::vector<int32> ExperienceConversionTimes;
        std::vector<int32> RedDiamondFreeConvert;
        int32 FreeChangeGoldDistanceSecond;
    };
#pragma pack(pop)
}
