#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MiniMapMarkTemplateSize();
#pragma pack(push, 1)
    struct MiniMapMarkTemplate : public template_base
    {
        int32 MarkType;
        int32 NameStringId;
        int32 NpcId;
        std::vector<int32> RegionLevel;
        int32 MapGoalX;
        int32 MapGoalY;
        int32 MapGoalZ;
        int32 MiniMapX;
        int32 MiniGoalY;
        int32 MiniGoalZ;
        int32 NpcPriority;
        int32 IfShow;
        int32 MissionId;
        int32 SafePoint;
    };
#pragma pack(pop)
}
