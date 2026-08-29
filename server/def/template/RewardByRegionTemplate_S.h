#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void RewardByRegionTemplateSize();
#pragma pack(push, 1)
    struct RewardByRegionTemplate : public template_base
    {
        int32 RewardType;
        std::vector<int32> ChinaMainRegionReward;
        std::vector<int32> KRRegionReward;
        std::vector<int32> ChinaTWRegionReward;
        std::vector<int32> ENRegionReward;
        std::vector<int32> THARegionReward;
        std::vector<int32> VNRegionReward;
        std::vector<int32> JPRegionReward;
        std::vector<int32> RUSRegionReward;
        std::vector<int32> SGRegionReward;
    };
#pragma pack(pop)
}
