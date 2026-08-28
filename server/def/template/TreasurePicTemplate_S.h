#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void TreasurePicTemplateSize();
#pragma pack(push, 1)
    struct TreasurePicTemplate : public template_base
    {
        int32 AwardType;
        int32 AwardIcon;
        int32 ExpLevel;
        std::vector<int32> DropIdList;
    };
#pragma pack(pop)
}
