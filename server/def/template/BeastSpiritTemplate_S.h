#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void BeastSpiritTemplateSize();
#pragma pack(push, 1)
    struct BeastSpiritTemplate : public template_base
    {
        int32 BeastSpiritType;
        int32 ItemId;
        std::vector<int32> UseNumArray;
        std::vector<int32> UnlockConditionArray;
        std::vector<float> GetAttributes;
        int32 ShowIndex;
    };
#pragma pack(pop)
}
