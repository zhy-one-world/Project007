#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void DropTemplateSize();
#pragma pack(push, 1)
    struct DropTemplate : public template_base
    {
        int32 ClassType;
        std::vector<int32> DropLevelLimit;
        int32 BelongID;
        std::vector<int32> BelongIDParam;
        int32 BelongType;
        int32 Distribute;
        int32 DropType;
        std::vector<int32> IDs;
        std::vector<int32> IDTypes;
        std::vector<int32> Counts;
        std::vector<int32> Bounds;
        std::vector<int32> Weights;
        std::vector<int32> ConstAtt;
        std::vector<int32> LevelChangeId;
    };
#pragma pack(pop)
}
