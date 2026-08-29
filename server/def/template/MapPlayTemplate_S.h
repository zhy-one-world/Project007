#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void MapPlayTemplateSize();
#pragma pack(push, 1)
    struct MapPlayTemplate : public template_base
    {
        int32 PlayType;
        int32 NeedEnergy;
        int32 Icon;
        int32 Title;
        int32 Content;
        int32 ExIcon;
        std::vector<int32> BossId;
        std::vector<int32> BornX;
        std::vector<int32> BornY;
        std::vector<int32> BornZ;
        std::vector<int32> BornYaw;
        std::vector<int32> BornPitch;
        std::vector<int32> BornRoll;
    };
#pragma pack(pop)
}
