#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void RedPackageTemplateSize();
#pragma pack(push, 1)
    struct RedPackageTemplate : public template_base
    {
        int32 Type;
        int32 SubType;
        std::vector<int32> CostMoneyArray;
        std::vector<int32> CanGetMoneyArray;
        int32 CanGetTimes;
        int32 NoTiceId;
        int32 LogicID;
    };
#pragma pack(pop)
}
