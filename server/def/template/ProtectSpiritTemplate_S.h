#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ProtectSpiritTemplateSize();
#pragma pack(push, 1)
    struct ProtectSpiritTemplate : public template_base
    {
        int32 Type;
        int32 CurLevel;
        int32 UpgradeTargetValue;
        std::vector<int32> UpgradeCostArray;
        int32 ItemColor;
        int32 MaxLevel;
        std::vector<float> WarriorPropertyArray;
        std::vector<float> KnightPropertyArray;
        std::vector<float> WizardPropertyArray;
        std::vector<float> AssassinPropertyArray;
        int32 NeedTinderOrder;
        int32 SkillId;
        int32 NeedStarLevel;
        std::vector<int32> RegionArr;
    };
#pragma pack(pop)
}
