#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void ElementHeartTemplateSize();
#pragma pack(push, 1)
    struct ElementHeartTemplate : public template_base
    {
        int32 quality_level;
        int32 AttType;
        int32 MaxUpgradeLevel;
        std::vector<float> BasicAtt;
        std::vector<float> UpgradeAttEach;
        int32 UpgradeId;
        std::vector<int32> BuffIdArray;
        int32 ItemTemplateId;
        int32 MergeId;
        std::vector<int32> MergeCost;
        int32 Type;
        int32 UnlockLevel;
        std::vector<int32> UnlockCost;
        int32 UpgradeLevel;
    };
#pragma pack(pop)
}
