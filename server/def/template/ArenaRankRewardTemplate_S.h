#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ArenaRankRewardTemplateSize();
#pragma pack(push, 1)
    struct ArenaRankRewardTemplate : public template_base
    {
        int32 ArenaPos;
        std::vector<int32> ItemRewardArray;
        std::vector<int32> MoneyRewardArray;
        int32 RewardDescription;
        int32 NoticeId;
    };
#pragma pack(pop)
}
