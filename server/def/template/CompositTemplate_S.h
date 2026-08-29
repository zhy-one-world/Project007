#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace faith
{
    void CompositTemplateSize();
#pragma pack(push, 1)
    struct CompositTemplate : public template_base
    {
        int32 TypeStringID;
        int32 CompositItemTemplateID;
        std::vector<int32> MaterialList;
        std::vector<int32> CostMoney;
        float Rate;
        int32 noticeID;
        std::vector<int32> RegionArr;
    };
#pragma pack(pop)
}
