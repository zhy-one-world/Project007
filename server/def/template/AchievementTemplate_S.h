#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void AchievementTemplateSize();
#pragma pack(push, 1)
    struct AchievementTemplate : public template_base
    {
        int32 PageId;
        int32 AchievementType;
        int32 AchievementNameId;
        int32 AchievementDiscreptionId;
        std::vector<int32> AchievementGoal;
        std::vector<int32> FinishMoney;
    };
#pragma pack(pop)
}
