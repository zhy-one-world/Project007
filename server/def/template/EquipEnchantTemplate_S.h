#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void EquipEnchantTemplateSize();
#pragma pack(push, 1)
    struct EquipEnchantTemplate : public template_base
    {
        int32 Level;
        int32 Type;
        int32 Slot;
        std::vector<int32> ItemConsume;
        std::vector<int32> ItemGet;
        int32 Describe;
        std::vector<float> AttArray;
    };
#pragma pack(pop)
}
