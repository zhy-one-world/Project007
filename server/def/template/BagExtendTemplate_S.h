#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void BagExtendTemplateSize();
#pragma pack(push, 1)
    struct BagExtendTemplate : public template_base
    {
        int32 Time;
        std::vector<int32> MoneyNeed;
    };
#pragma pack(pop)
}
