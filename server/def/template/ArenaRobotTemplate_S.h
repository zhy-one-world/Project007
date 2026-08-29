#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ArenaRobotTemplateSize();
#pragma pack(push, 1)
    struct ArenaRobotTemplate : public template_base
    {
        int32 Rank;
        int32 NpcId;
        int32 UpgradeId;
        std::vector<float> PropertyRatio;
        std::vector<float> PropertyPlus;
        std::vector<int32> Items;
        int32 Wing;
        int32 Sprite;
        std::vector<int32> EffectBuffArray;
        int32 GsValue;
        int32 MilitaryLevel;
        int32 RobotNameId;
        int32 ExpLevel;
    };
#pragma pack(pop)
}
