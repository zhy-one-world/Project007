#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ItemSuccinctTemplateSize();
#pragma pack(push, 1)
    struct ItemSuccinctTemplate : public template_base
    {
        std::vector<int32> PropertyID;
        std::vector<int32> PropertyMaxLimit;
        int32 GoldCost;
        int32 DiamondCost;
        int32 MaterialID;
        int32 MaterialCost;
        std::vector<float> GoldSuccinctEffectChance;
        std::vector<float> DiamondSuccinctEffectChance;
        std::vector<float> GoldSuccinctAttUpChance;
        std::vector<int32> GoldPropertyChangeRange;
        std::vector<float> DiamondSuccinctAttUpChance;
        std::vector<int32> DiamondPropertyChangeRange;
        int32 Noticeld;
        int32 MaterialShopID;
    };
#pragma pack(pop)
}
