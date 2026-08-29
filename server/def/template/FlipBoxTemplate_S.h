#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void FlipBoxTemplateSize();
#pragma pack(push, 1)
    struct FlipBoxTemplate : public template_base
    {
        int32 FlipBoxType;
        std::vector<int32> RandomItem;
        std::vector<int32> AwardMoney;
        std::vector<int32> BoxSpawnPoint;
    };
#pragma pack(pop)
}
