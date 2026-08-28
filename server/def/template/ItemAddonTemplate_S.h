#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void ItemAddonTemplateSize();
#pragma pack(push, 1)
    struct ItemAddonTemplate : public template_base
    {
        int32 level;
        std::vector<int32> ItemIdList;
        std::vector<int32> ItemNumList;
        int32 AddonShopId;
        std::vector<int32> MoneyCostList;
        std::vector<float> InheritAddonRatio;
        int32 Noticeld;
        int32 SuccessRate;
        std::vector<int32> EquipBuffArray;
    };
#pragma pack(pop)
}
