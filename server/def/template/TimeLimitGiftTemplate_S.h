#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void TimeLimitGiftTemplateSize();
#pragma pack(push, 1)
    struct TimeLimitGiftTemplate : public template_base
    {
        int32 TriggerTime;
        int32 MaxTriggerNum;
        int32 MaxBuyTime;
        int32 LifeTime;
        int32 ChangeTime;
        std::vector<int32> TextImage;
        std::vector<int32> Reward;
        int32 ConsumeType;
        std::vector<int32> MoneyList;
        int32 TriggerType;
        std::vector<int32> NeedLevel;
        std::vector<int32> NeedMission;
        std::vector<int32> MapWinList;
        std::string FuncLock;
        int32 GradeLevel;
        std::vector<int32> SpecialTriggerArray;
    };
#pragma pack(pop)
}
