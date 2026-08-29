#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void ElementHeartGroupTemplateSize();
#pragma pack(push, 1)
    struct ElementHeartGroupTemplate : public template_base
    {
        std::vector<int32> MoneyCost;
        float UpgradeRate;
        int32 DropGroupId;
        int32 Index;
        int32 Noticeld;
        int32 Integral;
    };
#pragma pack(pop)
}
