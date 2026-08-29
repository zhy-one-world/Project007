#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void RewardSettlementTemplateSize();
#pragma pack(push, 1)
    struct RewardSettlementTemplate : public template_base
    {
        int32 Type;
        float Experiencerewardfactor;
        float Coinrewards;
        int32 LowScore;
        int32 HighScore;
        std::vector<int32> ItemRewards;
    };
#pragma pack(pop)
}
