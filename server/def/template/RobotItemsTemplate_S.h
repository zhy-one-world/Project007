#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void RobotItemsTemplateSize();
#pragma pack(push, 1)
    struct RobotItemsTemplate : public template_base
    {
        std::vector<int32> WarriorEquipment;
        std::vector<int32> MasterEquipment;
        std::vector<int32> KnightEquipment;
        std::vector<int32> AssassinEquipment;
        std::vector<int32> ApplyItem;
        std::vector<int32> ApplyMoney;
        std::vector<int32> VipApplyItem;
    };
#pragma pack(pop)
}
