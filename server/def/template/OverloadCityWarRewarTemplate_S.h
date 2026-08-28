#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void OverloadCityWarRewarTemplateSize();
#pragma pack(push, 1)
    struct OverloadCityWarRewarTemplate : public template_base
    {
        int32 IsCross;
        std::vector<int32> LevelLimit;
        std::vector<int32> ShowItemId;
        std::vector<int32> ShowItemNum;
        std::vector<int32> RewardItemArray;
        std::vector<int32> ChiefRewardArray;
    };
#pragma pack(pop)
}
