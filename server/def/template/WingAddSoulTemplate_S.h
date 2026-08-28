#pragma once

#include "logic/type_def.hpp"
#include "template_base.h"

namespace hld
{
    void WingAddSoulTemplateSize();
#pragma pack(push, 1)
    struct WingAddSoulTemplate : public template_base
    {
        float SoulNum;
        std::vector<float> AttArray;
        int32 AddSoulMatirialId;
        int32 AddSoulMatirialNum;
        std::vector<int32> AddSoulMoney;
    };
#pragma pack(pop)
}
