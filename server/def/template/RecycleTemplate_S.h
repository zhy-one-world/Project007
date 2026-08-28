#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void RecycleTemplateSize();
#pragma pack(push, 1)
    struct RecycleTemplate : public template_base
    {
        int32 RecycleType;
        int32 Pos;
        std::vector<int32> MasterLevel;
        int32 ConditionName;
        int32 ConditionType;
        std::vector<int32> ConditionValue;
        std::vector<int32> RewardItemArray;
        std::vector<int32> WidgetJumpArray;
    };
#pragma pack(pop)
}
