#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ServiceGoalTemplateSize();
#pragma pack(push, 1)
    struct ServiceGoalTemplate : public template_base
    {
        int32 ActiveName;
        int32 ActiveType;
        int32 ConditionType;
        std::vector<int32> ConditionValue;
        int32 TheData;
        int32 NoticeId;
        std::vector<int32> RewardMoneyArray;
        int32 RewardItemArray;
        std::vector<int32> WidgetJumpArray;
    };
#pragma pack(pop)
}
