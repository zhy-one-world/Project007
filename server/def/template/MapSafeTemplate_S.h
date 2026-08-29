#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MapSafeTemplateSize();
#pragma pack(push, 1)
    struct MapSafeTemplate : public template_base
    {
        std::vector<int32> PointAry;
        int32 SafeName;
        int32 IsAllowPK;
        int32 Priority;
        int32 BackgroundMusic;
        int32 AreaType;
        int32 DependentTerritory;
        int32 MissionId;
    };
#pragma pack(pop)
}
