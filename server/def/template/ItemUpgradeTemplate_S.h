#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ItemUpgradeTemplateSize();
#pragma pack(push, 1)
    struct ItemUpgradeTemplate : public template_base
    {
        int32 EquipSlot;
        std::vector<int32> NoticeId;
        std::vector<float> AttArray;
    };
#pragma pack(pop)
}
