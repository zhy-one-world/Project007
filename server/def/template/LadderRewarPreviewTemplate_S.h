#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void LadderRewarPreviewTemplateSize();
#pragma pack(push, 1)
    struct LadderRewarPreviewTemplate : public template_base
    {
        std::vector<int32> AwardItems;
        std::vector<int32> AwardMoney;
    };
#pragma pack(pop)
}
